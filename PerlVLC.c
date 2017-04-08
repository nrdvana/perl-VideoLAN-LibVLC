#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include <vlc/vlc.h>
#include <vlc/libvlc_version.h>
#include <stdint.h>

#include "PerlVLC.h"

MGVTBL PerlVLC_instance_mg_vtbl;
MGVTBL PerlVLC_media_mg_vtbl;
MGVTBL PerlVLC_media_player_mg_vtbl;

SV* PerlVLC_set_mg(SV *obj, MGVTBL *mg_vtbl, void *ptr) {
	MAGIC *mg= NULL;
	
	if (!sv_isobject(obj))
		croak("Can't add magic to non-object");
	
	// Search for existing Magic that would hold this pointer
	for (mg = SvMAGIC(SvRV(obj)); mg; mg = mg->mg_moremagic) {
		if (mg->mg_type == PERL_MAGIC_ext && mg->mg_virtual == mg_vtbl) {
			mg->mg_ptr= ptr;
			return obj;
		}
	}
	sv_magicext(SvRV(obj), NULL, PERL_MAGIC_ext, mg_vtbl, (const char *) ptr, 0);
	return obj;
}

void* PerlVLC_get_mg(SV *obj, MGVTBL *mg_vtbl) {
	MAGIC *mg= NULL;
	if (sv_isobject(obj)) {
		for (mg = SvMAGIC(SvRV(obj)); mg; mg = mg->mg_moremagic) {
			if (mg->mg_type == PERL_MAGIC_ext && mg->mg_virtual == mg_vtbl)
				return (void*) mg->mg_ptr;
		}
	}
	return NULL;
}

/* Mortal sin here, but instead of allocating a struct to pass to the
 * callback and trying to figure out how to free it when needed, I'm
 * just packing the file descriptor, the log level, and the booleans
 * as a single 32-bit integer, then casting it to a pointer...
 */
#define PERLVLC_ERR_FD_MASK   0xFFFFFF
#define PERLVLC_ERR_LEV_SHIFT 24
#define PERLVLC_ERR_LEV_MASK  0xF
#define PERLVLC_ERR_WANT_CONTEXT_BIT 0x10000000
#define PERLVLC_ERR_WANT_OBJECT_BIT  0x20000000

void PerlVLC_log_cb(void *data, int level, const libvlc_log_t *ctx, const char *fmt, va_list args) {
	char buffer[1024], *pos, *lim;
	const char *module, *file, *name, *header;
	int fd, minlev, wrote, line;
	uintptr_t objid;
	
	fd= PTR2UV(data) & PERLVLC_ERR_FD_MASK;
	minlev= (PTR2UV(data) >> PERLVLC_ERR_LEV_SHIFT) & PERLVLC_ERR_LEV_MASK;
	if (level < minlev) return;
	
	((uint16_t*)buffer)[0]= (uint16_t) 1; /* callback ID is always 1 for logger */
	buffer[2]= (uint8_t) level;
	pos= buffer+3;
	lim= buffer+sizeof(buffer)-1; /* lim points to last character to make sure we can terminate the string */
	if (PTR2UV(data) & PERLVLC_ERR_WANT_CONTEXT_BIT) {
		libvlc_log_get_context(ctx, &module, &file, &line);
		wrote= snprintf(pos, lim-pos, "module=%s", module);
		if (wrote > 0 && wrote < lim-pos) pos += wrote+1;
		wrote= snprintf(pos, lim-pos, "file=%s", file);
		if (wrote > 0 && wrote < lim-pos) pos+= wrote+1;
		wrote= snprintf(pos, lim-pos, "line=%d", line);
		if (wrote > 0 && wrote < lim-pos) pos += wrote+1;
	}
	if (PTR2UV(data) & PERLVLC_ERR_WANT_OBJECT_BIT) {
		libvlc_log_get_object(ctx, &name, &header, &objid);
		wrote= snprintf(pos, lim-pos, "name=%s", name);
		if (wrote > 0 && wrote < lim-pos) pos += wrote+1;
		wrote= snprintf(pos, lim-pos, "header=%s", header);
		if (wrote > 0 && wrote < lim-pos) pos += wrote+1;
		wrote= snprintf(pos, lim-pos, "id=%ld", objid);
		if (wrote > 0 && wrote < lim-pos) pos += wrote+1;
	}
	wrote= vsnprintf(pos, lim-pos, fmt, args);
	if (wrote > 0) { pos += wrote; if (pos > lim) pos= lim; }
	
	/* can't report write errors... so just return? */
	wrote= write(fd, buffer, pos-buffer);
}

void PerlVLC_log_extract_attrs(SV *buffer, HV *attrs) {
	size_t len;
	const char *start, *end, *lim, *mid;
	SV *tmp;
	
	start= SvPV(buffer, len);
	lim= start + len;
	start += 2; /* skip callback ID */
	if (start < lim) { /* extract log level */
		tmp= newSViv(*start & 0xFF);
		if (!hv_store(attrs, "level", 5, tmp, 0)) sv_2mortal(tmp);
		start++;
	}
	while (start < lim) {
		end= memchr(start, 0, lim-start);
		if (end > start && end < lim && (mid= memchr(start, '=', end-start))) {
			tmp= newSVpvn(mid+1, end-mid-1);
			if (!hv_store(attrs, start, mid-start, tmp, 0)) sv_2mortal(tmp);
			start= end+1;
		}
		else {
			/* start is the new beginning of the scalar */
			sv_chop(buffer, start);
			break;
		}
	}
}

void PerlVLC_enable_logging(libvlc_instance_t *vlc, int fd, int lev, bool with_context, bool with_object) {
#if ((LIBVLC_VERSION_MAJOR * 10000 + LIBVLC_VERSION_MINOR * 100 + LIBVLC_VERSION_REVISION) >= 20100)
	if (fd < 0 || fd > PERLVLC_ERR_FD_MASK) croak("fd %d out of range", fd);
	if (lev < 0 || lev > PERLVLC_ERR_LEV_MASK) croak("err level %d out of range", lev);
	libvlc_log_set(vlc, &PerlVLC_log_cb,
		NUM2PTR(void*,
			fd
			| (lev << PERLVLC_ERR_LEV_SHIFT)
			| (with_context? PERLVLC_ERR_WANT_CONTEXT_BIT : 0)
			| (with_object? PERLVLC_ERR_WANT_OBJECT_BIT : 0)
		)
	);
#else
	croak("libvlc_log_* API not supported on this version");
#endif
}



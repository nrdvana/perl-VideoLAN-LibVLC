#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include <vlc/vlc.h>
#include <vlc/libvlc_version.h>
#include <stdint.h>

#include "PerlVLC.h"

#define PERLVLC_TRACE warn
//#define PERLVLC_TRACE(...) ((void)0)

static void* PerlVLC_video_lock_cb(void *data, void **planes);
static void PerlVLC_video_unlock_cb(void *data, void *picture, void * const *planes);
static void PerlVLC_video_display_cb(void *data, void *picture);

static SV* PerlVLC_set_mg(SV *obj, MGVTBL *mg_vtbl, void *ptr) {
	MAGIC *mg= NULL;
	
	if (!sv_isobject(obj))
		croak("Can't add magic to non-object");
	
	/* Search for existing Magic that would hold this pointer */
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

SV * PerlVLC_wrap_instance(libvlc_instance_t *inst) {
	PERLVLC_TRACE("PerlVLC_wrap_instance(%p)", inst);
	PerlVLC_set_instance_mg(
		sv_bless(newRV_noinc((SV*)newHV()), gv_stashpv("VideoLAN::LibVLC", GV_ADD)),
		inst
	);
}

int PerlVLC_instance_mg_free(pTHX_ SV *inst_sv, MAGIC *mg) {
	libvlc_instance_t *vlc= (libvlc_instance_t*) mg->mg_ptr;
	PERLVLC_TRACE("libvlc_instance_release(%p)", vlc);
	libvlc_release(vlc);
	return 0;
}

SV * PerlVLC_wrap_media(libvlc_media_t *media) {
	PERLVLC_TRACE("PerlVLC_wrap_media(%p)", media);
    PerlVLC_set_media_mg(
		sv_bless(newRV_noinc((SV*)newHV()), gv_stashpv("VideoLAN::LibVLC::Media", GV_ADD)),
		media
	);
}

int PerlVLC_media_mg_free(pTHX_ SV *media_sv, MAGIC *mg) {
	libvlc_media_t *media= (libvlc_media_t*) mg->mg_ptr;
	PERLVLC_TRACE("libvlc_media_release(%p)", media);
	libvlc_media_release(media);
	return 0;
}

SV * PerlVLC_wrap_media_player(libvlc_media_player_t *player) {
	PERLVLC_TRACE("PerlVLC_wrap_media_player(%p)", player);
	SV *self;
	PerlVLC_player_t *playerinfo;
	if (!player) return &PL_sv_undef;
	
	self= newRV_noinc((SV*)newHV());
	sv_bless(self, gv_stashpv("VideoLAN::LibVLC::MediaPlayer", GV_ADD));
	Newxz(playerinfo, 1, PerlVLC_player_t);
	playerinfo->player= player;
	PerlVLC_set_media_player_mg(self, playerinfo);
	return self;
}

int PerlVLC_media_player_mg_free(pTHX_ SV *player, MAGIC* mg) {
	PerlVLC_player_t *mpinfo= (PerlVLC_player_t*) mg->mg_ptr;
	PERLVLC_TRACE("PerlVLC_media_player_mg_free(%p)", mg->mg_ptr);
	if (!mpinfo) return;
	/* If callbacks have been registered, we need to re-register them without
	 * a reference to mpinfo, so that the media player thread can't possibly
	 * reference the freed memory.
	 */
	if (mpinfo->vlc_video_cb_installed)
		libvlc_video_set_callbacks(mpinfo->player,
			&PerlVLC_video_lock_cb,
			&PerlVLC_video_unlock_cb,
			&PerlVLC_video_display_cb,
			NULL);
	/* Now it should be safe to free mpinfo */
	Safefree(mpinfo);
	/* Then release the reference to the player, which may free it right now,
	 * or maybe not.  libvlc doesn't let us look at the reference count.
	 */
	PERLVLC_TRACE("libvlc_media_player_release(%p)", mpinfo->player);
	libvlc_media_player_release(mpinfo->player);
	return 0;
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

///*------------------------------------------------------------------------------------------------
// * Callback system for video
// */
//
//#define PERLVLC_VIDEO_CB_ERR_EVENT     1
//#define PERLVLC_VIDEO_CB_LOCK_EVENT    2
//#define PERLVLC_VIDEO_CB_UNLOCK_EVENT  3
//#define PERLVLC_VIDEO_CB_DISPLAY_EVENT 4
//#define PERLVLC_VIDEO_CB_FORMAT_EVENT  5
//#define PERLVLC_VIDEO_CB_CLEANUP_EVENT 6
//
///* Sent over pipe from VLC player thread to main thread */
//typedef struct {
//	uint16_t callback_id;    /* ID for keeping things straight with implementation */
//	uint16_t callback_event; /* Integer code defined above */
//	void *picture;           /* Application-chosen opaque value to identify the image */
//	void **planes;           /* Pointers to 32-byte-aligned buffers where image data will be written. */
//	char chroma[4];
//	uint32_t width;
//	uint32_t height;
//	uint32_t pitches[4];
//	uint32_t lines[4];
//} video_cb_event_t;
//
///* Sent back from main thread to VLC player thread */
//typedef struct {
//	int n_planes;
//	void * planes[4];
//	void * picture;
//} video_cb_reply_t;
//
///* Sent from VLC thread */
//typedef struct {
//	uint16_t callback_id;
//	uint16_t callback_event;
//	void *picture;
//
/* This is the callback, and runs from VLC's thread. */
static void* PerlVLC_video_lock_cb(void *data, void **planes) {
//	PerlVLC_player_t *mpinfo= (PerlVLC_player_t*) data;
//	video_cb_event_t event;
//	video_lock_cb_reply_t reply;
//	int wrote, got, i;
//	char buffer[64];
//	
//	if (!data) return NULL; /* happens if the perl MediaPlayer object has been destroyed */
//	
//	/* Write message to LibVLC instance that the callback is ready and needs data */
//	event.callback_id= mpinfo->vlc_cb_id;
//	event.callback_event= PERLVLC_VIDEO_CB_LOCK_EVENT;
//	event.picture= NULL;
//	wrote= write(mpinfo->vlc_cb_fd, &event, sizeof(event));
//	if (wrote != sizeof(event)) {
//		i= snprintf(buffer, sizeof(buffer), "Failed to write lock_cb event; wrote=%d\n", wrote);
//		write(2, buffer, i);
//		return NULL;
//	}
//	
//	/* Read reply from LibVLC, via other file handle. */
//	got= read(mpinfo->vlc_video_lock_cb_fd, &reply, sizeof(reply));
//	if (got == sizeof(reply)) {
//		memcpy(planes, reply.planes, sizeof(void*) * reply.n_planes);
//		return reply.picture;
//	} else {
//		i= snprintf(buffer, sizeof(buffer), "Got invalid reply in lock_cb; got=%d\n", got);
//		write(2, buffer, i);
//		return NULL;
//	}
}

void PerlVLC_video_unlock_cb(void *data, void *picture, void * const *planes) {
//	PerlVLC_player_t *mpinfo= (PerlVLC_player_t*) data;
//	video_cb_event_t event;
//	char buffer[64];
//	int i, wrote;
//	
//	if (!data) return;
//	/* Write message to LibVLC instance that the video frames are ready */
//	event.callback_id= mpinfo->vlc_cb_id;
//	event.callback_event= PERLVLC_VIDEO_CB_UNLOCK_EVENT;
//	event.picture= picture;
//	wrote= write(mpinfo->vlc_cb_fd, &event, sizeof(event));
//	if (wrote != sizeof(event)) {
//		i= snprintf(buffer, sizeof(buffer), "Failed to write unlock_cb event; wrote=%d\n", wrote);
//		write(2, buffer, i);
//	}
}

void PerlVLC_video_display_cb(void *data, void *picture) {
//	PerlVLC_player_t *mpinfo= (PerlVLC_player_t*) data;
//	video_cb_event_t event;
//	char buffer[64];
//	int i, wrote;
//	
//	if (!data) return;
//	/* Write message to LibVLC instance that the video frames are ready */
//	event.callback_id= mpinfo->vlc_cb_id;
//	event.callback_event= PERLVLC_VIDEO_CB_DISPLAY_EVENT;
//	event.picture= picture;
//	wrote= write(mpinfo->vlc_cb_fd, &event, sizeof(event));
//	if (wrote != sizeof(event)) {
//		i= snprintf(buffer, sizeof(buffer), "Failed to write display_cb event; wrote=%d\n", wrote);
//		write(2, buffer, i);
//	}
}

void PerlVLC_enable_video_callbacks(PerlVLC_player_t *mpinfo, int vlc_fd, bool hook_lock, bool hook_unlock, bool hook_display) {
//	libvlc_video_set_callbacks(mpinfo->player,
//		(hook_lock? &PerlVLC_video_lock_cb : &PerlVLC_video_autolock),
//		(hook_unlock? &PerlVLC_video_unlock_cb : NULL),
//		(hook_display? &PerlVLC_video_display_cb : NULL)
//	);
}

//void PerlVLC_parse_video_callback(PerlVLC_player_t *mpinfo, SV *buffer) {
//	
//}
//
//void PerlVLC_reply_to_video_lock() {
//	
//}

/*------------------------------------------------------------------------------------------------
 * Set up the vtable structs for applying magic
 */

static int PerlVLC_mg_nodup(pTHX_ MAGIC *mg, CLONE_PARAMS *param) {
	croak("Can't share VLC objects across perl iThreads");
	return 0;
}
#ifdef MGf_LOCAL
static int PerlVLC_mg_nolocal(pTHX_ SV *var, MAGIC* mg) {
	croak("Can't share VLC objects across perl iThreads");
	return 0;
}
#endif

MGVTBL PerlVLC_instance_mg_vtbl= {
	0, /* get */ 0, /* write */ 0, /* length */ 0, /* clear */
	PerlVLC_instance_mg_free,
	0, PerlVLC_mg_nodup
#ifdef MGf_LOCAL
	, PerlVLC_mg_nolocal
#endif
};
MGVTBL PerlVLC_media_mg_vtbl= {
	0, /* get */ 0, /* write */ 0, /* length */ 0, /* clear */
	PerlVLC_media_mg_free,
	0, PerlVLC_mg_nodup
#ifdef MGf_LOCAL
	, PerlVLC_mg_nolocal
#endif
};
MGVTBL PerlVLC_media_player_mg_vtbl= {
	0, /* get */ 0, /* write */ 0, /* length */ 0, /* clear */
	PerlVLC_media_player_mg_free,
	0, PerlVLC_mg_nodup
#ifdef MGf_LOCAL
	, PerlVLC_mg_nolocal
#endif
};

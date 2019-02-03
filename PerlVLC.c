#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include <vlc/vlc.h>
#include <vlc/libvlc_version.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/socket.h>

#include "PerlVLC.h"

//#define PERLVLC_TRACE(x...) PerlVLC_cb_log_error(x)
#define PERLVLC_TRACE(...) ((void)0)

static void PerlVLC_cb_log_error(const char *fmt, ...);
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

SV * PerlVLC_wrap_instance(libvlc_instance_t *instance) {
	SV *self;
	PerlVLC_vlc_t *vlc;
	PERLVLC_TRACE("PerlVLC_wrap_instance(%p)", instance);
	if (!instance) return &PL_sv_undef;
	self= newRV_noinc((SV*)newHV());
	sv_bless(self, gv_stashpv("VideoLAN::LibVLC", GV_ADD));
	Newxz(vlc, 1, PerlVLC_vlc_t);
	vlc->instance= instance;
	vlc->event_pipe[0]= -1;
	vlc->event_pipe[1]= -1;
	PerlVLC_set_instance_mg(self, vlc);
	return self;
}

int PerlVLC_instance_mg_free(pTHX_ SV *inst_sv, MAGIC *mg) {
	PerlVLC_vlc_t *vlc= (PerlVLC_vlc_t*) mg->mg_ptr;
	PERLVLC_TRACE("PerlVLC_instance_mg_free(%p)", vlc);
	if (!vlc) return 0;
	/* Then release the reference to the player, which may free it right now,
	 * or maybe not.  libvlc doesn't let us look at the reference count.
	 */
	PERLVLC_TRACE("libvlc_instance_release(%p)", vlc->instance);
	libvlc_release(vlc->instance);
	/* Now it should be safe to free mpinfo */
	PERLVLC_TRACE("free(vlc=%p)", vlc);
	Safefree(vlc);
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
	playerinfo->event_pipe= -1;
	playerinfo->vbuf_pipe[0]= -1;
	playerinfo->vbuf_pipe[1]= -1;
	PerlVLC_set_media_player_mg(self, playerinfo);
	return self;
}

int PerlVLC_media_player_mg_free(pTHX_ SV *player_sv, MAGIC* mg) {
	PerlVLC_player_t *mpinfo= (PerlVLC_player_t*) mg->mg_ptr;
	libvlc_media_player_t *player;
	int i;
	PERLVLC_TRACE("PerlVLC_media_player_mg_free(%p)", mpinfo);
	if (!mpinfo) return 0;
	/* Make sure playback has stopped before releasing player.
	 * Also make sure the player isn't blocked inside a callback
	 * waiting for input from us.
	 */
	if (mpinfo->video_cb_installed) {
		/* inform the frame alloc callback that it won't be getting any more buffers */
		shutdown(mpinfo->vbuf_pipe[1], SHUT_WR);
		PERLVLC_TRACE("libvlc_media_player_stop(); # with vbuf_pipe shut down");
		libvlc_media_player_stop(mpinfo->player);
		/* Ensure the callback never uses mpinfo again by giving it NULL instead.
		 * I'm assuming that libvlc API has a mutex in there to ensure orderly behavior...
		 */
		libvlc_video_set_callbacks(mpinfo->player, PerlVLC_video_lock_cb, NULL, NULL, NULL);
	}
	/* Then release the reference to the player, which may free it right now,
	 * or maybe not.  libvlc doesn't let us look at the reference count.
	 */
	PERLVLC_TRACE("libvlc_media_player_release(%p)", mpinfo->player);
	libvlc_media_player_release(mpinfo->player);
	/* VLC shouldn't have any more picture objects at this point. */
	for (i= 0; i < mpinfo->picture_count; i++) {
		mpinfo->pictures[i]->held_by_vlc= 0;
		sv_2mortal((SV*) mpinfo->pictures[i]->self_hv); // release our hidden reference to the perl objects
	}
	if (mpinfo->pictures) Safefree(mpinfo->pictures);
	/* Now it should be safe to free mpinfo */
	PERLVLC_TRACE("free(mpinfo=%p)", mpinfo);
	Safefree(mpinfo);
	return 0;
}

/* This function constructs a PerlVLC_picture_t from a hashref that looks like:
 * {
 *   chroma => $c4,
 *   width => $w,
 *   height => $h,
 *   planes => [
 *      { pitch => $p, lines => $n, buffer => \$scalar },
 *      ...
 *   ]
 * }
 *
 * The buffer is optional.  If not specified, but pitch and lines are nonzero,
 * a buffer will be allocated.
 */
PerlVLC_picture_t* PerlVLC_picture_new_from_hash(SV *args) {
	PerlVLC_picture_t self, *ret;
	HV *hash, *plane_hash;
	SV **item;
	AV *av;
	int i;
	const char *chroma;
	STRLEN len;
	memset(&self, 0, sizeof(self));
	PERLVLC_TRACE("PerlVLC_picture_new_from_hash");

	if (!SvROK(args) || SvTYPE(SvRV(args)) != SVt_PVHV)
		croak("Expected hashref");
	hash= (HV*) SvRV(args);

	if (!(item= hv_fetchs(hash, "chroma", 0)) || !*item || !SvPOK(*item))
		croak("chroma is required");
	chroma= SvPV(*item, len);
	if (len != 4)
		croak("chroma must be 4 characters");
	memcpy(self.chroma, chroma, 4);

	if (!(item= hv_fetchs(hash, "width", 0)) || !*item || !SvOK(*item))
		croak("width is required");
	self.width= SvIV(*item);

	if (!(item= hv_fetchs(hash, "height", 0)) || !*item || !SvOK(*item))
		croak("height is required");
	self.height= SvIV(*item);

	if ((item= hv_fetchs(hash, "plane_pitch", 0)) && *item && SvOK(*item)) {
		if (SvROK(*item) && SvTYPE(SvRV(*item)) == SVt_PVAV) {
			av= (AV*) SvRV(*item);
			for (i= 0; i < 3 && i <= av_len(av) ; i++) {
				item= av_fetch(av, i, 0);
				if (!item || !*item || !SvOK(*item))
					croak("Invalid %s->[%d]", "plane_pitch", i);
				self.pitch[i]= SvIV(*item);
			}
		}
		else
			self.pitch[0]= SvIV(*item);
	}
	if ((item= hv_fetchs(hash, "plane_lines", 0)) && *item && SvOK(*item)) {
		if (SvROK(*item) && SvTYPE(SvRV(*item)) == SVt_PVAV) {
			av= (AV*) SvRV(*item);
			for (i= 0; i < 3 && i <= av_len(av); i++) {
				item= av_fetch(av, i, 0);
				if (!item || !*item || !SvOK(*item))
					croak("Invalid %s->[%d]", "plane_lines", i);
				self.lines[i]= SvIV(*item);
			}
		}
		else
			self.lines[0]= SvIV(*item);
	}
	if ((item= hv_fetchs(hash, "plane", 0)) && *item && SvOK(*item)) {
		av= (SvROK(*item) && SvTYPE(SvRV(*item)) == SVt_PVAV)? (AV*) SvRV(*item) : NULL;
		for (i= 0; av? (i < 3 && i <= av_len(av)) : (i < 1); i++) {
			if (av) item= av_fetch(av, i, 0);
			if (!item || !*item || !SvROK(*item) || !SvPOK(SvRV(*item)))
				croak("Invalid %s->[%d]", "plane", i);
			/* hold a reference to the scalar within the scalar-ref */
			self.plane_buffer_sv[i]= SvRV(*item);
			self.plane[i]= SvPVX(self.plane_buffer_sv[i]);
		}
	}

	/* now make a copy into dynamic memory */
	Newx(ret, 1, PerlVLC_picture_t);
	memcpy(ret, &self, sizeof(PerlVLC_picture_t));
	/* and increment any ref counts to the buffers we are holding onto */
	for (i= 0; i < PERLVLC_PICTURE_PLANES; i++) {
		if (ret->plane_buffer_sv[i])
			SvREFCNT_inc(ret->plane_buffer_sv[i]);
		else if (ret->pitch[i] && ret->lines[i])
			Newx(ret->plane[i], ret->pitch[i]*ret->lines[i], char);
	}
	return ret;
}

SV * PerlVLC_wrap_picture(PerlVLC_picture_t *pic) {
	PERLVLC_TRACE("PerlVLC_wrap_picture(%p)", pic);
	SV *self;
	if (!pic) return &PL_sv_undef;
	if (!pic->self_hv) {
		self= newRV_noinc((SV*) (pic->self_hv= newHV()));
		sv_bless(self, gv_stashpv("VideoLAN::LibVLC::Picture", GV_ADD));
		/* after this, when the HV goes out of scope it calls the mg_free (our destructor) */
		PerlVLC_set_picture_mg(self, pic);
	} else {
		self= newRV_inc((SV*) pic->self_hv);
	}
	return self;
}

/* This shouldn't get called until the self_hv goes out of scope */
int PerlVLC_picture_mg_free(pTHX_ SV *picture_sv, MAGIC *mg) {
	PerlVLC_picture_t *pic= (PerlVLC_picture_t*) mg->mg_ptr;
	PERLVLC_TRACE("PerlVLC_picture_mg_free(%p)", pic);
	if (pic) {
		pic->self_hv= NULL;
		PerlVLC_picture_destroy(pic);
	}
	return 0;
}

void PerlVLC_picture_destroy(PerlVLC_picture_t *pic) {
	int i;
	PERLVLC_TRACE("PerlVLC_picture_destroy(%p)", pic);
	if (pic->held_by_vlc)
		warn("BUG: Picture object destroyed while VLC still has access to it!");
	if (pic->self_hv)
		croak("BUG: Picture object destroyed while Perl still has access to it!");
	/* For each plane, the buffer either came from a perl scalar ref, or was allocated directly. */
	for (i= 0; i < PERLVLC_PICTURE_PLANES; i++) {
		if (pic->plane_buffer_sv[i])
			SvREFCNT_dec(pic->plane_buffer_sv[i]);
		else if (pic->plane[i])
			Safefree(pic->plane[i]);
	}
	Safefree(pic);
}

/*------------------------------------------------------------------------------------------------
 * Callback system.
 *
 * The callbacks work by writing messages to a pipe (socket actually, to avoid SIGPIPE issues)
 * which the main perl thread reads and dispatches to coderefs.
 *
 * All messages start with a little header struct that frames the messages for reliable transfer.
 */

#define PERLVLC_MSG_STREAM_MARKER      0xF0F1
#define PERLVLC_MSG_STREAM_CHECK(m)    ((uint16_t) (0x101 * (m)->event_id * (m)->payload_len))
#define PERLVLC_MSG_LOG                 1
#define PERLVLC_MSG_VIDEO_LOCK_EVENT    2
#define PERLVLC_MSG_VIDEO_TRADE_PICTURE 3
#define PERLVLC_MSG_VIDEO_UNLOCK_EVENT  4
#define PERLVLC_MSG_VIDEO_DISPLAY_EVENT 5
#define PERLVLC_MSG_VIDEO_FORMAT_EVENT  6
#define PERLVLC_MSG_VIDEO_CLEANUP_EVENT 7

#define PERLVLC_MSG_EVENT_MAX           7

/* changes here should be kept in sync with PERLVLC_MSG_BUFFER_SIZE in header */
#define PERLVLC_MSG_HEADER \
	uint16_t event_id; \
	uint16_t callback_id; \

typedef struct PerlVLC_Message {
	PERLVLC_MSG_HEADER
	char     payload[];
} PerlVLC_Message_t;

typedef struct PerlVLC_Message_LogMsg {
	PERLVLC_MSG_HEADER
	uint32_t line;
	uint32_t objid;
	uint8_t module_strlen;
	uint8_t file_strlen;
	uint8_t name_strlen;
	uint8_t header_strlen;
	char stringdata[];
} PerlVLC_Message_LogMsg_t;

typedef struct PerlVLC_Message_TradePicture {
	PERLVLC_MSG_HEADER
	PerlVLC_picture_t *picture;
} PerlVLC_Message_TradePicture_t;

typedef struct PerlVLC_Message_ImgFmt {
	PERLVLC_MSG_HEADER
	char chroma[4];
	unsigned width;
	unsigned height;
	unsigned plane_pitch[3];
	unsigned plane_lines[3];
	unsigned allocated;
} PerlVLC_Message_ImgFmt_t;

SV* PerlVLC_inflate_message(void *buffer, int msglen) {
	HV *obj, *ret= (HV*) sv_2mortal((SV*) newHV());
	AV *plane, *pitch, *lines;
	char *pos, *lim;
	int i;
	PerlVLC_Message_t *msg= (PerlVLC_Message_t*) buffer;
	PerlVLC_Message_LogMsg_t *logmsg;
	PerlVLC_Message_TradePicture_t *picmsg;
	PerlVLC_Message_ImgFmt_t *fmtmsg;

	if (msglen < sizeof(PerlVLC_Message_t))
		croak("Message too short (%d < %ld)", msglen, sizeof(PerlVLC_Message_t));
	switch (msg->event_id) {
	case PERLVLC_MSG_LOG:
		{
			if (msglen < sizeof(PerlVLC_Message_LogMsg_t)+1)
				croak("Message too short (%d < %ld)", msglen, sizeof(PerlVLC_Message_LogMsg_t));
			logmsg= (PerlVLC_Message_LogMsg_t *) msg;
			pos= logmsg->stringdata;
			lim= ((char*)buffer) + msglen;
			if (logmsg->line)
				hv_stores(ret, "line", newSViv(logmsg->line));
			if (logmsg->objid)
				hv_stores(ret, "objid", newSViv(logmsg->objid));
			if (logmsg->module_strlen) {
				if (pos + logmsg->module_strlen + 1 >= lim)
					croak("Message too short");
				hv_stores(ret, "module", newSVpvn(pos, logmsg->module_strlen));
				pos += logmsg->module_strlen+1;
			}
			if (logmsg->file_strlen) {
				if (pos + logmsg->file_strlen + 1 >= lim)
					croak("Message too short");
				hv_stores(ret, "file", newSVpvn(pos, logmsg->file_strlen));
				pos += logmsg->file_strlen+1;
			}
			if (logmsg->name_strlen) {
				if (pos + logmsg->name_strlen + 1 >= lim)
					croak("Message too short");
				hv_stores(ret, "name", newSVpvn(pos, logmsg->name_strlen));
				pos += logmsg->name_strlen+1;
			}
			if (logmsg->header_strlen) {
				if (pos + logmsg->header_strlen + 1 >= lim)
					croak("Message too short");
				hv_stores(ret, "header", newSVpvn(pos, logmsg->header_strlen));
				pos += logmsg->header_strlen+1;
			}
			lim[-1]= '\0'; // for strlen safety
			hv_stores(ret, "message", newSVpvn(pos, strlen(pos)));
		}
		if (0) {
	case PERLVLC_MSG_VIDEO_TRADE_PICTURE:
	case PERLVLC_MSG_VIDEO_UNLOCK_EVENT:
	case PERLVLC_MSG_VIDEO_DISPLAY_EVENT:
			if (msglen < sizeof(PerlVLC_Message_TradePicture_t))
				croak("Message too short (%d < %ld)", msglen, sizeof(PerlVLC_Message_TradePicture_t));
			picmsg= (PerlVLC_Message_TradePicture_t *) msg;
			if (msg->event_id == PERLVLC_MSG_VIDEO_DISPLAY_EVENT)
				picmsg->picture->held_by_vlc= 0;
			/* The picture knows its own HV, so create a new ref to that */
			hv_stores(ret, "picture", newRV_inc((SV*) picmsg->picture->self_hv));
		}
		if (0) {
	case PERLVLC_MSG_VIDEO_FORMAT_EVENT:
			if (msglen < sizeof(PerlVLC_Message_ImgFmt_t))
				croak("Message too short (%d < %ld)", msglen, sizeof(PerlVLC_Message_TradePicture_t));
			fmtmsg= (PerlVLC_Message_ImgFmt_t *) msg;
			hv_stores(ret, "chroma", newSVpvn(fmtmsg->chroma, 4));
			hv_stores(ret, "width", newSViv(fmtmsg->width));
			hv_stores(ret, "height", newSViv(fmtmsg->height));
			hv_stores(ret, "plane_pitch", newRV((SV*) (pitch= newAV())));
			hv_stores(ret, "plane_lines", newRV((SV*) (lines= newAV())));
			for (i= 0; i < 3; i++) {
				av_push(pitch, newSViv(fmtmsg->plane_pitch[i]));
				av_push(lines, newSViv(fmtmsg->plane_pitch[i]));
			}
		}
	default:
		hv_stores(ret, "callback_id", newSViv(msg->callback_id));
		hv_stores(ret, "event_id",  newSViv(msg->event_id));
	}
	return newRV_inc((SV*) ret);
}

/* Log an error from a callback.  The callback is likely in a different thread, so can't access
 * any Perl structures or even stdlib FILE handles, so just write to stderr and hope for the best.
 * Errors shouldn't happen except for bugs, anyway.
 */
static void PerlVLC_cb_log_error(const char *fmt, ...) {
	char buffer[256];
	va_list argp;
	va_start(argp, fmt);
	buffer[0]= '#'; buffer[1]= ' ';
	int len= 2+vsnprintf(buffer+2, sizeof(buffer)-2, fmt, argp);
	va_end(argp);
	if (len < sizeof(buffer)) { buffer[len++]= '\n'; }
	else { len= sizeof(buffer); buffer[len-1]= '\n'; }
	int wrote= write(2, buffer, len);
	(void) wrote; /* nothing we can do about errors, since we're in a callback */
}

/*------------------------------------------------------------------------------------------------
 * Logging Callback
 *
 * VLC provides a callback to receive log messages generated from other threads.
 * This implementation forwards those messages over the event pipe.
 */

#if ((LIBVLC_VERSION_MAJOR * 10000 + LIBVLC_VERSION_MINOR * 100 + LIBVLC_VERSION_REVISION) >= 20100)
void PerlVLC_log_cb(void *opaque, int level, const libvlc_log_t *ctx, const char *fmt, va_list args) {
	char *pos, *lim;
	const char *module, *file, *name, *header;
	int fd, minlev, wrote, line, avail, len;
	uintptr_t objid;
	char buffer[PERLVLC_MSG_BUFFER_SIZE];
	PerlVLC_Message_LogMsg_t *msg= (PerlVLC_Message_LogMsg_t*) buffer;
	PerlVLC_vlc_t *vlc= (PerlVLC_vlc_t*) opaque;
	PERLVLC_TRACE("PerlVLC_log_cb(%s, ...) @ %d", fmt, level);
	
	if (vlc->log_level > level) return;
	memset(msg, 0, sizeof(*msg));
	pos= msg->stringdata;
	lim= buffer + sizeof(buffer);
	if (vlc->log_module || vlc->log_file || vlc->log_line) {
		libvlc_log_get_context(ctx, &module, &file, &line);
		if (module && vlc->log_module && pos + (len= strlen(module)) + 1 < lim) {
			memcpy(pos, module, len+1);
			pos += len+1;
			msg->module_strlen= len;
		}
		if (file && vlc->log_file && pos + (len= strlen(file)) + 1 < lim) {
			memcpy(pos, file, len+1);
			pos += len+1;
			msg->file_strlen= len;
		}
		msg->line= line;
	}
	if (vlc->log_name || vlc->log_header || vlc->log_objid) {
		libvlc_log_get_object(ctx, &name, &header, &objid);
		if (name && vlc->log_name && pos + (len= strlen(name)) + 1 < lim) {
			memcpy(pos, name, len+1);
			pos += len+1;
			msg->name_strlen= len;
		}
		if (header && vlc->log_header && pos + (len= strlen(header)) + 1 < lim) {
			memcpy(pos, header, len+1);
			pos += len+1;
			msg->header_strlen= len;
		}
		msg->objid= objid;
	}
	wrote= vsnprintf(pos, lim-pos, fmt, args);
//	PERLVLC_TRACE("sprintf into %ld bytes = %d", lim-pos, wrote);
	if (wrote > 0) { pos += wrote; if (pos >= lim) pos= lim-1; }
	*pos++ = 0;
	msg->event_id= PERLVLC_MSG_LOG;
	msg->callback_id= (uint16_t) vlc->log_callback_id;
	wrote= send(vlc->event_pipe[1], buffer, pos - buffer, 0);
//	PERLVLC_TRACE("send(%d, %p, %d, 0): %d", vlc->event_pipe[1], buffer, pos - buffer, wrote);
}
#endif

void PerlVLC_set_log_cb(PerlVLC_vlc_t *vlc, int callback_id) {
	PERLVLC_TRACE("PerlVLC_set_log_cb");
#if ((LIBVLC_VERSION_MAJOR * 10000 + LIBVLC_VERSION_MINOR * 100 + LIBVLC_VERSION_REVISION) >= 20100)
	vlc->log_callback_id= callback_id;
	libvlc_log_set(vlc->instance, &PerlVLC_log_cb, vlc);
#else
	croak("Log redirection not suppoted on this version of VLC");
#endif
}

/*------------------------------------------------------------------------------------------------
 * Video Callbacks
 *
 * VLC offers a callback system that lets the host application allocate the buffers for
 * the picture.  This is useful for things like copying to OpenGL textures, or just to
 * get at the raw data.
 *
 */

/* The VLC decoder calls this when it has a new frame of video to decode.
 * It asks us to fill in the values for planes[0..2], normally to a pre-allocated
 * buffer.  We have to wait for a round trip to the user (unless next buffer is
 * already in the pipe).  We then return a value for 'picture' which gets passed
 * back to us during unlock_cb and display_cb.
 */
static void* PerlVLC_video_lock_cb(void *opaque, void **planes) {
	PerlVLC_player_t *mpinfo= (PerlVLC_player_t*) opaque;
	PerlVLC_picture_t *picture;
	int i;
	PerlVLC_Message_t lock_msg;
	PerlVLC_Message_TradePicture_t pic_msg;

	if (!mpinfo) {
		/* If this happens, it is a bug, and probably going to kil the program.  Warn loudly. */
		PerlVLC_cb_log_error("BUG: Video callback received NULL opaque pointer\n");
	}
	else {
		/* Write message to LibVLC instance that the callback is ready and needs data */
		lock_msg.callback_id= mpinfo->callback_id;
		lock_msg.event_id= PERLVLC_MSG_VIDEO_LOCK_EVENT;
		if (send(mpinfo->event_pipe, &lock_msg, sizeof(lock_msg), 0) <= 0) {
			/* This also should never happen, unless event pipe was closed. */
			PerlVLC_cb_log_error("BUG: Video callback can't send event\n");
			/* Might still have a spare buffer to use in the other pipe, though, so continue. */
		}
		
		i= 0;
		if (recv(mpinfo->vbuf_pipe[0], &pic_msg, sizeof(pic_msg), 0) <= 0) {
			/* Should never happen, but could if pipe was closed before video thread stopped. */
			PerlVLC_cb_log_error("BUG: Video callback can't receive picture\n");
		}
		else if (pic_msg.event_id != PERLVLC_MSG_VIDEO_TRADE_PICTURE) {
			/* Should never happen, but could if pipe was closed before video thread stopped. */
			PerlVLC_cb_log_error("BUG: Video callback received mesage ID %d but expected %d\n",
				pic_msg.event_id, PERLVLC_MSG_VIDEO_TRADE_PICTURE);
		}
		else {
			picture= pic_msg.picture;
			for (i= 0; i < 3; i++) planes[i]= picture->plane[i];
			return picture;
		}
	}
	/* I'm guessing this will crash, but maybe libvlc handles it? */
	planes[0]= NULL;
	planes[1]= NULL;
	planes[2]= NULL;
	return NULL;
}

/* The VLC decoder calls this when it has filled the locked buffer with data.
 * We forward this to the user and return immediately.
 */
static void PerlVLC_video_unlock_cb(void *opaque, void *picture, void * const *planes) {
	PerlVLC_player_t *mpinfo= (PerlVLC_player_t*) opaque;
	PerlVLC_Message_TradePicture_t pic_msg;
	int i;
	char buf[128];
	if (!mpinfo) {
		/* If this happens, it is a bug, and probably going to kil the program.  Warn loudly. */
		PerlVLC_cb_log_error("BUG: Video unlock callback received NULL opaque pointer");
		return;
	}
	pic_msg.callback_id= mpinfo->callback_id;
	pic_msg.event_id= PERLVLC_MSG_VIDEO_UNLOCK_EVENT;
	pic_msg.picture= (PerlVLC_picture_t *) picture;
	if (send(mpinfo->event_pipe, &pic_msg, sizeof(pic_msg), 0) <= 0)
		/* This also should never happen, unless event pipe was closed. */
		PerlVLC_cb_log_error("BUG: Video unlock callback can't send event");
}

/* The VLC decoder calls this when it is time to display one of the pictures.
 * The 'picture' argument is whatever we returned in video_lock_cb when this picture
 * was locked/filled, but display order might be different from fill order.
 */
static void PerlVLC_video_display_cb(void *opaque, void *picture) {
	PerlVLC_player_t *mpinfo= (PerlVLC_player_t*) opaque;
	PerlVLC_Message_TradePicture_t pic_msg;
	int i;
	char buf[128];
	if (!mpinfo) {
		/* If this happens, it is a bug, and probably going to kil the program.  Warn loudly. */
		PerlVLC_cb_log_error("BUG: Video unlock callback received NULL opaque pointer");
		return;
	}
	pic_msg.callback_id= mpinfo->callback_id;
	pic_msg.event_id= PERLVLC_MSG_VIDEO_DISPLAY_EVENT;
	pic_msg.picture= (PerlVLC_picture_t *) picture;
	if (send(mpinfo->event_pipe, &pic_msg, sizeof(pic_msg), 0) <= 0)
		/* This also should never happen, unless event pipe was closed. */
		PerlVLC_cb_log_error("BUG: Video unlock callback can't send event");
}

/* The VLC decoder calls this when it knows the format of the media.
 * We relay this to the main thread where the user may opt to change some of the parameters,
 * and where the user should prepare the rendering buffers.
 * The user sends back the count of buffers allocated (why do they need that?) and any modifications
 * to these arguments.
 */
static unsigned PerlVLC_video_format_cb(void **opaque_p, char *chroma_p, unsigned *width_p, unsigned *height_p, unsigned *pitch, unsigned *lines) {
	PerlVLC_player_t *mpinfo= (PerlVLC_player_t*) *opaque_p;
	PerlVLC_Message_ImgFmt_t fmt_msg;
	int i;

	if (!mpinfo) {
		/* If this happens, it is a bug, and probably going to kil the program.  Warn loudly. */
		PerlVLC_cb_log_error("BUG: Video format callback received NULL opaque pointer");
		return 0;
	}
	
	/* Pack up arguments */
	memset(&fmt_msg, 0, sizeof(fmt_msg));
	fmt_msg.callback_id= mpinfo->callback_id;
	fmt_msg.event_id= PERLVLC_MSG_VIDEO_FORMAT_EVENT;
	for (i= 0; i < 4; i++)
		fmt_msg.chroma[i]= chroma_p[i];
	fmt_msg.width= *width_p;
	fmt_msg.height= *height_p;
	for (i= 0; i < 3; i++) {
		fmt_msg.plane_pitch[i]= pitch[i];
		fmt_msg.plane_lines[i]= lines[i];
	}

	/* Send event to main thread */
	if (send(mpinfo->event_pipe, &fmt_msg, sizeof(fmt_msg), 0) <= 0) {
		/* If user has closed the event pipe, just accept the params */
		return 0;
	}

	/* Wait for response */
	i= 0;
	if (recv(mpinfo->vbuf_pipe[0], (char*) &fmt_msg, sizeof(fmt_msg), 0) <= 0
		|| fmt_msg.event_id != PERLVLC_MSG_VIDEO_FORMAT_EVENT
	) {
		/* If this happens, it is a bug, and probably going to kil the program.  Warn loudly. */
		PerlVLC_cb_log_error("BUG: Video format callback did not get valid response");
		return 0;
	}

	/* Apply values back to the arguments (which are read/write) */
	for (i= 0; i < 4; i++)
		chroma_p[i]= fmt_msg.chroma[i];
	*width_p= fmt_msg.width;
	*height_p= fmt_msg.height;
	for (i= 0; i < 3; i++) {
		pitch[i]= fmt_msg.plane_pitch[i];
		lines[i]=   fmt_msg.plane_lines[i];
	}
	/* Return the number allocated */
	return fmt_msg.allocated;
}

static void PerlVLC_video_cleanup_cb(void *opaque) {
	int i;
	char buf[128];
	/* forward message to user that they may clean up the buffers */
	PerlVLC_player_t *mpinfo= (PerlVLC_player_t*) opaque;
	PerlVLC_Message_t msg;
	if (!mpinfo) {
		/* If this happens, it is a bug, and probably going to kil the program.  Warn loudly. */
		PerlVLC_cb_log_error("BUG: Video cleanup callback received NULL opaque pointer");
		return;
	}
	msg.callback_id= mpinfo->callback_id;
	msg.event_id= PERLVLC_MSG_VIDEO_CLEANUP_EVENT;
	if (send(mpinfo->event_pipe, &msg, sizeof(msg), 0) <= 0)
		/* This also should never happen, unless event pipe was closed. */
		PerlVLC_cb_log_error("BUG: Video cleanup callback can't send event");
}

void PerlVLC_enable_video_callbacks(PerlVLC_player_t *mpinfo, bool unlock_cb, bool display_cb, bool format_cb, bool cleanup_cb) {
#if (LIBVLC_VERSION_MAJOR < 2)
	if (format_cb || cleanup_cb)
		croak("Can't support set_format callback on LibVLC %d.%d", LIBVLC_VERSION_MAJOR, LIBVLC_VERSION_MINOR);
#endif
	if (mpinfo->vbuf_pipe[0] < 0)
		croak("Must set vbuf_pipe handles before enabling video callbacks");
	libvlc_video_set_callbacks(
		mpinfo->player,
		PerlVLC_video_lock_cb,
		unlock_cb? PerlVLC_video_unlock_cb : NULL,
		display_cb? PerlVLC_video_display_cb : NULL,
		mpinfo
	);
	mpinfo->video_cb_installed= 1;
#if (LIBVLC_VERSION_MAJOR >= 2)
	if (format_cb) {
		libvlc_video_set_format_callbacks(
			mpinfo->player,
			PerlVLC_video_format_cb,
			cleanup_cb? PerlVLC_video_cleanup_cb : NULL
		);
		mpinfo->video_format_cb_installed= 1;
	}
#endif
}

/* Add a picture to the list held by this object.  The picture must have been
 * wrapped with a Perl hashref prior to this call.
 */
void PerlVLC_player_add_picture(PerlVLC_player_t *player, PerlVLC_picture_t *pic) {
	void *larger;
	if (player->picture_count + 1 < player->picture_alloc) {
		if ((larger= realloc(player->pictures, sizeof(void*) * (player->picture_alloc + 8)))) {
			player->pictures= (PerlVLC_picture_t**) larger;
			player->picture_alloc += 8;
		}
		else croak("Can't grow picture array");
	}
	if (!pic->self_hv) croak("BUG: picture lacks self_hv");
	player->pictures[player->picture_count++]= pic;
	/* maintain a refcnt on the HV */
	SvREFCNT_inc(pic->self_hv);
	if (player->vbuf_pipe[1] >= 0)
		PerlVLC_player_fill_picture_queue(player);
}

/* Remove a specific picture from the list held by this object.  Dies if the picture
 * doesn't belong to this object.
 */
void PerlVLC_player_remove_picture(PerlVLC_player_t *player, PerlVLC_picture_t *pic) {
	int i;
	for (i= 0; i < player->picture_count; i++)
		if (player->pictures[i] == pic) {
			sv_2mortal((SV*) pic->self_hv);
			player->pictures[i]= player->pictures[--player->picture_count];
			return;
		}
	croak("picture %p is not held by player", pic);
}

/* Makes sure VLC thread has at least N pictures assigned for it to use.
 * Dies if pipe is not opened yet or if it fails to write to the pipe.
 * Returns the number of pictures assigned to VLC.
 */
int PerlVLC_player_fill_picture_queue(PerlVLC_player_t *player) {
	PerlVLC_Message_TradePicture_t msg;
	int i, cnt;
	/* Queue up to 4 pictures toward VLC, assuming pipe is open */
	if (player->vbuf_pipe[1] < 0)
		croak("Queue is not initialized");
	for (cnt= 0, i=0; i < player->picture_count; i++)
		if (player->pictures[i]->held_by_vlc) ++cnt;
	for (i= 0; cnt < 4 && i < player->picture_count; i++)
		if (!player->pictures[i]->held_by_vlc) {
			msg.event_id= PERLVLC_MSG_VIDEO_TRADE_PICTURE;
			msg.picture= player->pictures[i];
			if (send(player->vbuf_pipe[1], &msg, sizeof(msg), 0) <= 0)
				croak("Failed to send picture to VLC thread");
			player->pictures[i]->held_by_vlc= 1;
			cnt++;
		}
	return cnt;
}

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
MGVTBL PerlVLC_picture_mg_vtbl= {
	0, /* get */ 0, /* write */ 0, /* length */ 0, /* clear */
	PerlVLC_picture_mg_free,
	0, PerlVLC_mg_nodup
#ifdef MGf_LOCAL
	, PerlVLC_mg_nolocal
#endif
};

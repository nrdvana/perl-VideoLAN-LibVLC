#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include <vlc/vlc.h>
#include <vlc/libvlc_version.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/socket.h>

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
	PERLVLC_TRACE("PerlVLC_media_player_mg_free(%p)", mpinfo);
	if (!mpinfo) return;
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
	if (mpinfo->vbuf_pipe[0])  close(mpinfo->vbuf_pipe[0]);
	if (mpinfo->vbuf_pipe[1])  close(mpinfo->vbuf_pipe[1]);
	/* Now it should be safe to free mpinfo */
	PERLVLC_TRACE("free(mpinfo=%p)", mpinfo);
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
#define PERLVLC_MSG_VIDEO_LOCK_EVENT    2
#define PERLVLC_MSG_VIDEO_TRADE_PICTURE 3
#define PERLVLC_MSG_VIDEO_UNLOCK_EVENT  4
#define PERLVLC_MSG_VIDEO_DISPLAY_EVENT 5
#define PERLVLC_MSG_VIDEO_FORMAT_EVENT  6
#define PERLVLC_MSG_VIDEO_CLEANUP_EVENT 7

#define PERLVLC_MSG_EVENT_MAX           7

#define PERLVLC_MSG_HEADER \
	uint16_t stream_marker; \
	uint16_t object_id; \
	uint8_t  event_id; \
	uint8_t  payload_len; \
	uint16_t stream_check;

typedef struct PerlVLC_Message {
	PERLVLC_MSG_HEADER
	char     payload[];
} PerlVLC_Message_t;

#define PERLVLC_MSG_BUFFER_SIZE (256+sizeof(PerlVLC_Message_t))

static int PerlVLC_send_message(int fd, void *message, size_t message_size) {
	PerlVLC_Message_t *msg= (PerlVLC_Message_t *) message;
	int ofs= 0, wrote;
	if (message_size - sizeof(PerlVLC_Message_t) > 255)
		croak("BUG: PerlVLC message exceeds protocol size");
	msg->payload_len= (uint8_t) (message_size - sizeof(PerlVLC_Message_t));
	/* these help identify the start of messages, in case something goes wrong */
	msg->stream_marker= 0xF0F1;
	msg->stream_check= PERLVLC_MSG_STREAM_CHECK(msg);

	wrote= send(fd, message, message_size, 0);
	while (wrote < message_size) {
		/* errors, nonblocking file handle, or closed file */
		if (wrote <= 0) return 0;
		/* else made some progress, and try again */
		ofs += wrote;
		wrote= send(fd, ((char*) message) + ofs, message_size - ofs, 0);
	}
	return 1;
}

static int PerlVLC_recv_message(int fd, char *buffer, int buflen, int *bufpos) {
	int pos= *bufpos, got, search= 0;
	PerlVLC_Message_t *msg;
	while (1) {
		/* read more until we have a full header */
		while (pos < sizeof(PerlVLC_Message_t)) {
			got= recv(fd, buffer+pos, buflen-pos, 0);
			if (got <= 0) {
				*bufpos= pos;
				return 0;
			}
			pos+= got;
		}
		/* Check whether header is valid before trying to read ->length */
		msg= (PerlVLC_Message_t *) buffer;
		if (msg->stream_marker != PERLVLC_MSG_STREAM_MARKER
			|| !msg->event_id
			|| msg->event_id > PERLVLC_MSG_EVENT_MAX
			|| msg->stream_check != PERLVLC_MSG_STREAM_CHECK(msg)
		) {
			/* If head is not valid, it means corruption of the message stream!  Shouldn't happen... */
			warn("Corrupted message received!  Searching for next.");
			for (search= 0; search < pos-1; search++)
				if (*(uint16_t*)(buffer+search) == PERLVLC_MSG_STREAM_MARKER) break;
			/* shift buffer over so that first byte is the marker we found, or pos itself. */
			memmove(buffer, buffer+search, pos-search);
			pos -= search;
			continue;
		}
		/* Read more of it until we have it all */
		while (pos < sizeof(*msg) + msg->payload_len) {
			got= recv(fd, buffer+pos, buflen-pos, 0);
			if (got <= 0) {
				*bufpos= pos;
				return 0;
			}
		}
		*bufpos= pos;
		return 1;
	}
}

/* Log an error from a callback.  The callback is likely in a different thread, so can't access
 * any Perl structures or even stdlib FILE handles, so just write to stderr and hope for the best.
 * Errors shouldn't happen except for bugs, anyway.
 */
static void PerlVLC_cb_log_error(const char *fmt, ...) {
	char buffer[256];
	va_list argp;
	int len= vsnprintf(buffer, sizeof(buffer), fmt, argp);
	int wrote= write(2, buffer, len);
	(void) wrote; /* nothing we can do about errors, since we're in a callback */
}

/*------------------------------------------------------------------------------------------------
 * Video Callbacks
 *
 * VLC offers a callback system that lets the host application allocate the buffers for
 * the picture.  This is useful for things like copying to OpenGL textures, or just to
 * get at the raw data.
 *
 */
typedef struct PerlVLC_Picture {
	int id;
	void *plane[3];
	unsigned stride[3];
	unsigned height[3];
} PerlVLC_Picture_t;

/* Sent from main app in response to video-lock callback's request */
/* Sent from video-unlock callback to main thread, to deliver filled buffer */

typedef struct PerlVLC_Message_TradePicture {
	PERLVLC_MSG_HEADER
	PerlVLC_Picture_t *picture;
} PerlVLC_Message_TradePicture_t;

/* The VLC decoder calls this when it has a new frame of video to decode.
 * It asks us to fill in the values for planes[0..2], normally to a pre-allocated
 * buffer.  We have to wait for a round trip to the user (unless next buffer is
 * already in the pipe).  We then return a value for 'picture' which gets passed
 * back to us during unlock_cb and display_cb.
 */
static void* PerlVLC_video_lock_cb(void *opaque, void **planes) {
	PerlVLC_player_t *mpinfo= (PerlVLC_player_t*) opaque;
	PerlVLC_Picture_t *picture;
	int i;
	char buf[PERLVLC_MSG_BUFFER_SIZE];
	PerlVLC_Message_t lock_msg;
	PerlVLC_Message_TradePicture_t *pic_msg;

	if (!mpinfo) {
		/* If this happens, it is a bug, and probably going to kil the program.  Warn loudly. */
		PerlVLC_cb_log_error("BUG: Video callback received NULL opaque pointer\n");
	}
	else {
		/* Write message to LibVLC instance that the callback is ready and needs data */
		lock_msg.object_id= mpinfo->object_id;
		lock_msg.event_id= PERLVLC_MSG_VIDEO_LOCK_EVENT;
		if (!PerlVLC_send_message(mpinfo->event_pipe, &lock_msg, sizeof(lock_msg))) {
			/* This also should never happen, unless event pipe was closed. */
			PerlVLC_cb_log_error("BUG: Video callback can't send event\n");
			/* Might still have a spare buffer to use in the other pipe, though. */
		}
		
		i= 0;
		pic_msg= (PerlVLC_Message_TradePicture_t *) buf;
		if (!PerlVLC_recv_message(mpinfo->vbuf_pipe[0], buf, sizeof(buf), &i)) {
			/* Should never happen, but could if pipe was closed before video thread stopped. */
			PerlVLC_cb_log_error("BUG: Video callback can't receive picture\n");
		}
		else if (pic_msg->event_id != PERLVLC_MSG_VIDEO_TRADE_PICTURE) {
			/* Should never happen, but could if pipe was closed before video thread stopped. */
			PerlVLC_cb_log_error("BUG: Video callback received mesage ID %d but expected %d\n",
				pic_msg->event_id, PERLVLC_MSG_VIDEO_TRADE_PICTURE);
		}
		else {
			picture= pic_msg->picture;
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
		PerlVLC_cb_log_error("BUG: Video unlock callback received NULL opaque pointer\n");
		return;
	}
	pic_msg.object_id= mpinfo->object_id;
	pic_msg.event_id= PERLVLC_MSG_VIDEO_UNLOCK_EVENT;
	pic_msg.picture= (PerlVLC_Picture_t *) picture;
	if (!PerlVLC_send_message(mpinfo->event_pipe, &pic_msg, sizeof(pic_msg)))
		/* This also should never happen, unless event pipe was closed. */
		PerlVLC_cb_log_error("BUG: Video unlock callback can't send event\n");
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
		PerlVLC_cb_log_error("BUG: Video unlock callback received NULL opaque pointer\n");
		return;
	}
	pic_msg.object_id= mpinfo->object_id;
	pic_msg.event_id= PERLVLC_MSG_VIDEO_DISPLAY_EVENT;
	pic_msg.picture= (PerlVLC_Picture_t *) picture;
	if (!PerlVLC_send_message(mpinfo->event_pipe, &pic_msg, sizeof(pic_msg)))
		/* This also should never happen, unless event pipe was closed. */
		PerlVLC_cb_log_error("BUG: Video unlock callback can't send event\n");
}

typedef struct PerlVLC_Message_ImgFmt {
	PERLVLC_MSG_HEADER
	char chroma[4];
	unsigned width;
	unsigned height;
	unsigned plane_pitch[3];
	unsigned plane_rows[3];
	unsigned allocated;
} PerlVLC_Message_ImgFmt_t;

/* The VLC decoder calls this when it knows the format of the media.
 * We relay this to the main thread where the user may opt to change some of the parameters,
 * and where the user should prepare the rendering buffers.
 * The user sends back the count of buffers allocated (why do they need that?) and any modifications
 * to these arguments.
 */
static unsigned PerlVLC_video_format_cb(void **opaque_p, char *chroma_p, unsigned *width_p, unsigned *height_p, unsigned *pitches, unsigned *lines) {
	PerlVLC_player_t *mpinfo= (PerlVLC_player_t*) *opaque_p;
	PerlVLC_Message_ImgFmt_t fmt_msg;
	int i;

	if (!mpinfo) {
		/* If this happens, it is a bug, and probably going to kil the program.  Warn loudly. */
		PerlVLC_cb_log_error("BUG: Video format callback received NULL opaque pointer\n");
		return 0;
	}
	
	/* Pack up arguments */
	memset(&fmt_msg, 0, sizeof(fmt_msg));
	fmt_msg.object_id= mpinfo->object_id;
	fmt_msg.event_id= PERLVLC_MSG_VIDEO_FORMAT_EVENT;
	for (i= 0; i < 4; i++)
		fmt_msg.chroma[i]= chroma_p[i];
	fmt_msg.width= *width_p;
	fmt_msg.height= *height_p;
	for (i= 0; i < 3; i++) {
		fmt_msg.plane_pitch[i]= pitches[i];
		fmt_msg.plane_rows[i]= lines[i];
	}

	/* Send event to main thread */
	if (!PerlVLC_send_message(mpinfo->event_pipe, &fmt_msg, sizeof(fmt_msg))) {
		/* If user has closed the event pipe, just accept the params */
		return 0;
	}

	/* Wait for response */
	i= 0;
	if (!PerlVLC_recv_message(mpinfo->vbuf_pipe[0], (char*) &fmt_msg, sizeof(fmt_msg), &i)
		|| fmt_msg.event_id != PERLVLC_MSG_VIDEO_FORMAT_EVENT
	) {
		/* If this happens, it is a bug, and probably going to kil the program.  Warn loudly. */
		PerlVLC_cb_log_error("BUG: Video format callback did not get valid response\n");
		return 0;
	}

	/* Apply values back to the arguments (which are read/write) */
	for (i= 0; i < 4; i++)
		chroma_p[i]= fmt_msg.chroma[i];
	*width_p= fmt_msg.width;
	*height_p= fmt_msg.height;
	for (i= 0; i < 3; i++) {
		pitches[i]= fmt_msg.plane_pitch[i];
		lines[i]=   fmt_msg.plane_rows[i];
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
		PerlVLC_cb_log_error("BUG: Video cleanup callback received NULL opaque pointer\n");
		return;
	}
	msg.object_id= mpinfo->object_id;
	msg.event_id= PERLVLC_MSG_VIDEO_CLEANUP_EVENT;
	if (!PerlVLC_send_message(mpinfo->event_pipe, &msg, sizeof(msg)))
		/* This also should never happen, unless event pipe was closed. */
		PerlVLC_cb_log_error("BUG: Video cleanup callback can't send event\n");
}

void PerlVLC_enable_video_callbacks(PerlVLC_player_t *mpinfo, bool unlock_cb, bool display_cb, bool format_cb, bool cleanup_cb) {
#if (LIBVLC_VERSION_MAJOR < 2)
	if (format_cb)
		croak("Can't support set_format callback on LibVLC %d.%d", LIBVLC_VERSION_MAJOR, LIBVLC_VERSION_MINOR);
#endif
	if (!mpinfo->vbuf_pipe[1]) {
		...
	}
	libvlc_video_set_callbacks(
		mpinfo->player,
		PerlVLC_video_lock_cb,
		unlock_cb? PerlVLC_video_unlock_cb : NULL,
		display_cb? PerlVLC_video_display_cb : NULL,
		mpinfo
	);
#if (LIBVLC_VERSION_MAJOR >= 2)
	if (format_cb)
		libvlc_video_set_format_callbacks(
			mpinfo->player,
			PerlVLC_video_format_cb,
			cleanup_cb? PerlVLC_video_cleanup_cb : NULL
		);
#endif
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

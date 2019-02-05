#include <vlc/vlc.h>

struct PerlVLC_Message;
typedef struct PerlVLC_Message PerlVLC_Message_t;
#define PERLVLC_MSG_BUFFER_SIZE 512
#define PERLVLC_MSG_LOG                 1
#define PERLVLC_MSG_VIDEO_LOCK_EVENT    2
#define PERLVLC_MSG_VIDEO_TRADE_PICTURE 3
#define PERLVLC_MSG_VIDEO_UNLOCK_EVENT  4
#define PERLVLC_MSG_VIDEO_DISPLAY_EVENT 5
#define PERLVLC_MSG_VIDEO_FORMAT_EVENT  6
#define PERLVLC_MSG_VIDEO_CLEANUP_EVENT 7
#define PERLVLC_MSG_EVENT_MAX           7
SV* PerlVLC_inflate_message(void *buffer, int msglen);

extern MGVTBL PerlVLC_instance_mg_vtbl;
extern MGVTBL PerlVLC_media_mg_vtbl;
extern MGVTBL PerlVLC_media_player_mg_vtbl;
extern MGVTBL PerlVLC_picture_mg_vtbl;
extern void* PerlVLC_get_mg(SV *obj, MGVTBL *mg_vtbl);

// Actually saw one codec say "plane 1: pitch not aligned (160%64): disabling direct rendering"
// so I guess we're up to 64 bytes these days...
#define PERLVLC_PLANE_PITCH_MUL 64
#define PERLVLC_PLANE_PITCH_MASK (PERLVLC_PLANE_PITCH_MUL-1)
#define PERLVLC_ALIGN_PLANE(x) ((void*)( (((intptr_t)(x)) + PERLVLC_PLANE_PITCH_MASK) & ~(intptr_t)PERLVLC_PLANE_PITCH_MASK ))

#define PERLVLC_PICTURE_PLANES 3
typedef struct PerlVLC_picture {
	int id;
	char chroma[4];
	HV *self_hv;
	int held_by_vlc;
	unsigned width, height;
	void *plane[PERLVLC_PICTURE_PLANES];
	SV *plane_buffer_sv[PERLVLC_PICTURE_PLANES];
	unsigned pitch[PERLVLC_PICTURE_PLANES];
	unsigned lines[PERLVLC_PICTURE_PLANES];
} PerlVLC_picture_t;

#define PerlVLC_set_picture_mg(obj, ptr)     PerlVLC_set_mg(obj, &PerlVLC_picture_mg_vtbl, (void*) ptr)
#define PerlVLC_get_picture_mg(obj)          ((PerlVLC_picture_t*) PerlVLC_get_mg(obj, &PerlVLC_picture_mg_vtbl))
extern PerlVLC_picture_t* PerlVLC_picture_new_from_hash(SV *args);
extern SV* PerlVLC_wrap_picture(PerlVLC_picture_t *pic);
extern void PerlVLC_picture_destroy(PerlVLC_picture_t *pic);

typedef struct PerlVLC_vlc {
	libvlc_instance_t *instance;
	int event_pipe[2];
	int log_level, log_callback_id;
	int log_module:1, log_file:1, log_line:1, log_name:1, log_header:1, log_objid:1;
} PerlVLC_vlc_t;

#define PerlVLC_set_instance_mg(obj, ptr)     PerlVLC_set_mg(obj, &PerlVLC_instance_mg_vtbl, (void*) ptr)
#define PerlVLC_get_instance_mg(obj)          ((PerlVLC_vlc_t*) PerlVLC_get_mg(obj, &PerlVLC_instance_mg_vtbl))
extern SV * PerlVLC_wrap_instance(libvlc_instance_t *inst);
extern void PerlVLC_set_log_cb(PerlVLC_vlc_t *vlc, int callback_id);

typedef struct PerlVLC_player {
	libvlc_media_player_t *player;
	bool video_cb_installed;
	bool video_format_cb_installed;
	bool trace_pictures;
	int need_format_response;
	int callback_id;
	int event_pipe;
	int picture_count;
	int picture_alloc;
	PerlVLC_picture_t **pictures;
	int vbuf_pipe[2];
} PerlVLC_player_t;

#define PerlVLC_set_media_player_mg(obj, ptr) PerlVLC_set_mg(obj, &PerlVLC_media_player_mg_vtbl, (void*) ptr)
#define PerlVLC_get_media_player_mg(obj)      ((PerlVLC_player_t*) PerlVLC_get_mg(obj, &PerlVLC_media_player_mg_vtbl))
extern SV * PerlVLC_wrap_media_player(libvlc_media_player_t *player);
#define PERLVLC_VIDEO_CALLBACK_LOCK     1
#define PERLVLC_VIDEO_CALLBACK_UNLOCK   2
#define PERLVLC_VIDEO_CALLBACK_DISPLAY  4
#define PERLVLC_VIDEO_CALLBACK_FORMAT   8
#define PERLVLC_VIDEO_CALLBACK_CLEANUP 16
void PerlVLC_enable_video_callbacks(PerlVLC_player_t *mpinfo, int which);
void PerlVLC_player_add_picture(PerlVLC_player_t *player, PerlVLC_picture_t *pic);
void PerlVLC_player_remove_picture(PerlVLC_player_t *player, PerlVLC_picture_t *pic);
int PerlVLC_player_fill_picture_queue(PerlVLC_player_t *player);
void PerlVLC_video_reply_format(PerlVLC_player_t *player, char *chroma, int width, int height, SV *pitch, SV *lines, int alloc_count);
#define PerlVLC_set_media_mg(obj, ptr)        PerlVLC_set_mg(obj, &PerlVLC_media_mg_vtbl, (void*) ptr)
#define PerlVLC_get_media_mg(obj)             ((libvlc_media_t*) PerlVLC_get_mg(obj, &PerlVLC_media_mg_vtbl))
extern SV * PerlVLC_wrap_media(libvlc_media_t *player);

#include "buffer_scalar.c"

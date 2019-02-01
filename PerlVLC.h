#include <vlc/vlc.h>

struct PerlVLC_Message;
typedef struct PerlVLC_Message PerlVLC_Message_t;
#define PERLVLC_MSG_BUFFER_SIZE 264
extern int PerlVLC_send_message(int fd, void *message, size_t message_size);
extern int PerlVLC_recv_message(int fd, char *buffer, int buflen, int *bufpos);
extern int PerlVLC_shift_message(char *buffer, int buflen, int *bufpos);
extern SV * PerlVLC_inflate_message(PerlVLC_Message_t *msg);

extern MGVTBL PerlVLC_instance_mg_vtbl;
extern MGVTBL PerlVLC_media_mg_vtbl;
extern MGVTBL PerlVLC_media_player_mg_vtbl;
extern MGVTBL PerlVLC_picture_mg_vtbl;
extern void* PerlVLC_get_mg(SV *obj, MGVTBL *mg_vtbl);

#define PERLVLC_PICTURE_PLANES 3
typedef struct PerlVLC_picture {
	int id;
	char chroma[4];
	SV *self_sv;
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
	char event_recv_buf[PERLVLC_MSG_BUFFER_SIZE];
	int event_recv_bufpos;
} PerlVLC_vlc_t;

#define PerlVLC_set_instance_mg(obj, ptr)     PerlVLC_set_mg(obj, &PerlVLC_instance_mg_vtbl, (void*) ptr)
#define PerlVLC_get_instance_mg(obj)          ((PerlVLC_vlc_t*) PerlVLC_get_mg(obj, &PerlVLC_instance_mg_vtbl))
extern SV * PerlVLC_wrap_instance(libvlc_instance_t *inst);
extern void PerlVLC_vlc_init_event_pipe(PerlVLC_vlc_t *vlc);
extern void PerlVLC_vlc_enable_logging(PerlVLC_vlc_t *vlc, int lev, bool with_context, bool with_object);

typedef struct PerlVLC_player {
	libvlc_media_player_t *player;
	bool video_cb_installed;
	bool video_format_cb_installed;
	int object_id;
	int event_pipe;
	int picture_count;
	PerlVLC_picture_t **pictures;
	int vbuf_pipe[2];
	/* these fields are for the VLC callbacks */
	char vbuf_recv_buf[PERLVLC_MSG_BUFFER_SIZE];
	int vbuf_recv_bufpos;
} PerlVLC_player_t;

#define PerlVLC_set_media_player_mg(obj, ptr) PerlVLC_set_mg(obj, &PerlVLC_media_player_mg_vtbl, (void*) ptr)
#define PerlVLC_get_media_player_mg(obj)      ((PerlVLC_player_t*) PerlVLC_get_mg(obj, &PerlVLC_media_player_mg_vtbl))
extern SV * PerlVLC_wrap_media_player(libvlc_media_player_t *player);
void PerlVLC_player_init_event_pipe(PerlVLC_player_t *player, PerlVLC_vlc_t *vlc);

#define PerlVLC_set_media_mg(obj, ptr)        PerlVLC_set_mg(obj, &PerlVLC_media_mg_vtbl, (void*) ptr)
#define PerlVLC_get_media_mg(obj)             ((libvlc_media_t*) PerlVLC_get_mg(obj, &PerlVLC_media_mg_vtbl))
extern SV * PerlVLC_wrap_media(libvlc_media_t *player);

#include "buffer_scalar.c"

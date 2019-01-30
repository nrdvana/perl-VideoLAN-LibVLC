#include <vlc/vlc.h>

extern MGVTBL PerlVLC_instance_mg_vtbl;
extern MGVTBL PerlVLC_media_mg_vtbl;
extern MGVTBL PerlVLC_media_player_mg_vtbl;
extern void* PerlVLC_get_mg(SV *obj, MGVTBL *mg_vtbl);

typedef struct {
	libvlc_media_player_t *player;
	bool vlc_video_cb_installed;
	bool vlc_video_format_cb_installed;
	int vlc_cb_id;
	int vlc_cb_fd;
	int video_lock_pipe_rd;
	int video_lock_pipe_wr;
} PerlVLC_player_t;


#define PerlVLC_set_instance_mg(obj, ptr)     PerlVLC_set_mg(obj, &PerlVLC_instance_mg_vtbl, (void*) ptr)
#define PerlVLC_get_instance_mg(obj)          ((libvlc_instance_t*) PerlVLC_get_mg(obj, &PerlVLC_instance_mg_vtbl))

#define PerlVLC_set_media_mg(obj, ptr)        PerlVLC_set_mg(obj, &PerlVLC_media_mg_vtbl, (void*) ptr)
#define PerlVLC_get_media_mg(obj)             ((libvlc_media_t*) PerlVLC_get_mg(obj, &PerlVLC_media_mg_vtbl))

#define PerlVLC_set_media_player_mg(obj, ptr) PerlVLC_set_mg(obj, &PerlVLC_media_player_mg_vtbl, (void*) ptr)
#define PerlVLC_get_media_player_mg(obj)      ((PerlVLC_player_t*) PerlVLC_get_mg(obj, &PerlVLC_media_player_mg_vtbl))

extern SV * PerlVLC_wrap_media_player(libvlc_media_player_t *player);
extern SV * PerlVLC_wrap_instance(libvlc_instance_t *inst);
extern SV * PerlVLC_wrap_media(libvlc_media_t *player);

extern void PerlVLC_enable_logging(libvlc_instance_t *vlc, int fd, int lev, bool with_context, bool with_object);
extern void PerlVLC_log_extract_attrs(SV *buffer, HV *attrs);

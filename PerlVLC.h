#include <vlc/vlc.h>

extern MGVTBL PerlVLC_instance_mg_vtbl;
extern MGVTBL PerlVLC_media_mg_vtbl;
extern MGVTBL PerlVLC_media_player_mg_vtbl;

extern SV* PerlVLC_set_mg(SV *obj, MGVTBL *mg_vtbl, void *ptr);
extern void* PerlVLC_get_mg(SV *obj, MGVTBL *mg_vtbl);

#define PerlVLC_set_instance_mg(obj, ptr)     PerlVLC_set_mg(obj, &PerlVLC_instance_mg_vtbl, (void*) ptr)
#define PerlVLC_get_instance_mg(obj)          ((libvlc_instance_t*) PerlVLC_get_mg(obj, &PerlVLC_instance_mg_vtbl))

#define PerlVLC_set_media_mg(obj, ptr)        PerlVLC_set_mg(obj, &PerlVLC_media_mg_vtbl, (void*) ptr)
#define PerlVLC_get_media_mg(obj)             ((libvlc_media_t*) PerlVLC_get_mg(obj, &PerlVLC_media_mg_vtbl))

#define PerlVLC_set_media_player_mg(obj, ptr) PerlVLC_set_mg(obj, &PerlVLC_media_player_mg_vtbl, (void*) ptr)
#define PerlVLC_get_media_player_mg(obj)      ((libvlc_media_player_t*) PerlVLC_get_mg(obj, &PerlVLC_media_player_mg_vtbl))

extern void PerlVLC_enable_logging(libvlc_instance_t *vlc, int fd, libvlc_log_level lev, int with_context, int with_object);

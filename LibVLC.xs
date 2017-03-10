#define PERL_NO_GET_CONTEXT
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include <vlc/vlc.h>

#include "const-c.inc"
#include "PerlVLC.h"

MODULE = VideoLAN::LibVLC              PACKAGE = VideoLAN::LibVLC

INCLUDE: const-xs.inc

libvlc_instance_t*
libvlc_new(args)
	AV* args
	INIT:
		int argc, i;
		size_t len;
		SV **ep;
		const char **argv;
	CODE:
		argc= av_len(args)+1;
		argv= alloca(sizeof(char*) * (argc + 1));
		if (!argv) croak("alloca failed");
		for (i= 0; i < argc; i++) {
			ep= av_fetch(args, i, 0);
			argv[i]= (ep && *ep)? SvPV(*ep, len) : "";
		}
		argv[argc]= NULL;
		RETVAL= libvlc_new(argc, argv);
		if (!RETVAL)
			croak("libvlc_new failed");
	OUTPUT:
		RETVAL

void
DESTROY(vlc)
	libvlc_instance_t *vlc
	CODE:
		libvlc_release(vlc);

const char*
libvlc_get_changeset()

const char*
libvlc_get_compiler()

const char*
libvlc_get_version()

void
libvlc_set_app_id(vlc, id, version, icon)
	libvlc_instance_t *vlc
	const char *id
	const char *version
	const char *icon

void
libvlc_set_user_agent(vlc, name, http)
	libvlc_instance_t *vlc
	const char *name
	const char *http

void
audio_filter_list(vlc)
	libvlc_instance_t *vlc
	INIT:
		libvlc_module_description_t *mlist, *mcur;
		HV *elem;
	PPCODE:
		for (mcur= mlist= libvlc_audio_filter_list_get(vlc); mcur; mcur= mcur->p_next) {
			elem= newHV();
			hv_store(elem, "name",      4, newSVpv(mcur->psz_name, 0), 0);
			hv_store(elem, "shortname", 9, newSVpv(mcur->psz_shortname, 0), 0);
			hv_store(elem, "longname",  8, newSVpv(mcur->psz_longname, 0), 0);
			hv_store(elem, "help",      4, newSVpv(mcur->psz_help, 0), 0);
			PUSHs(newRV_noinc((SV*)elem));
		}
		libvlc_module_description_list_release(mlist);

void
video_filter_list(vlc)
	libvlc_instance_t *vlc
	INIT:
		libvlc_module_description_t *mlist, *mcur;
		HV *elem;
	PPCODE:
		for (mcur= mlist= libvlc_video_filter_list_get(vlc); mcur; mcur= mcur->p_next) {
			elem= newHV();
			hv_store(elem, "name",      4, newSVpv(mcur->psz_name, 0), 0);
			hv_store(elem, "shortname", 9, newSVpv(mcur->psz_shortname, 0), 0);
			hv_store(elem, "longname",  8, newSVpv(mcur->psz_longname, 0), 0);
			hv_store(elem, "help",      4, newSVpv(mcur->psz_help, 0), 0);
			PUSHs(newRV_noinc((SV*)elem));
		}
		libvlc_module_description_list_release(mlist);


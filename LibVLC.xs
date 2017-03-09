#define PERL_NO_GET_CONTEXT
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include <vlc/vlc.h>
#include <vlc/libvlc_structures.h>
#include <vlc/libvlc_media.h>
#include <vlc/libvlc_media_list.h>
#include <vlc/libvlc_media_library.h>
#include <vlc/libvlc_media_discoverer.h>
#include <vlc/libvlc_media_player.h>
#include <vlc/libvlc_media_list_player.h>
#include <vlc/libvlc_events.h>
#include <vlc/libvlc_version.h>
#include <vlc/libvlc_vlm.h>

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
	OUTPUT:
		RETVAL

void
DESTROY(vlc)
	libvlc_instance_t *vlc
	CODE:
		libvlc_release(vlc);


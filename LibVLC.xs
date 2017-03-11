#define PERL_NO_GET_CONTEXT
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include <vlc/vlc.h>

#include "PerlVLC.h"

MODULE = VideoLAN::LibVLC              PACKAGE = VideoLAN::LibVLC

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
libvlc_audio_filter_list_get(vlc)
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
libvlc_video_filter_list_get(vlc)
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

libvlc_media_t *
libvlc_media_new_location(vlc, mrl)
	libvlc_instance_t *vlc
	const char *mrl

libvlc_media_t *
libvlc_media_new_path(vlc, path)
	libvlc_instance_t *vlc
	const char *path

libvlc_media_t *
libvlc_media_new_fd(vlc, fd)
	libvlc_instance_t *vlc
	int fd

long
libvlc_media_get_duration(media)
	libvlc_media_t *media

char *
libvlc_media_get_meta(media, field_id)
	libvlc_media_t *media
	int field_id

void
libvlc_media_parse(media)
	libvlc_media_t *media

int
libvlc_media_parse_with_options(media, parse_flag, timeout)
	libvlc_media_t *media
	int parse_flag
	int timeout

void
_const_unavailable()
	PPCODE:
		croak("Symbol not available on this version of LibVLC");

MODULE = VideoLAN::LibVLC              PACKAGE = VideoLAN::LibVLC::Media

void
_build_metadata(media)
	libvlc_media_t *media
	INIT:
		HV *meta;
		SV *ref;
		const char* val;
		struct { int code; const char *name; } *attr, attrlist[]= {
			{ libvlc_meta_Title       , "Title"       },
			{ libvlc_meta_Artist      , "Artist"      },
			{ libvlc_meta_Genre       , "Genre"       },
			{ libvlc_meta_Copyright   , "Copyright"   },
			{ libvlc_meta_Album       , "Album"       },
			{ libvlc_meta_TrackNumber , "TrackNumber" },
			{ libvlc_meta_Description , "Description" },
			{ libvlc_meta_Rating      , "Rating"      },
			{ libvlc_meta_Date        , "Date"        },
			{ libvlc_meta_Setting     , "Setting"     },
			{ libvlc_meta_URL         , "URL"         },
			{ libvlc_meta_Language    , "Language"    },
			{ libvlc_meta_NowPlaying  , "NowPlaying"  },
			{ libvlc_meta_Publisher   , "Publisher"   },
			{ libvlc_meta_EncodedBy   , "EncodedBy"   },
			{ libvlc_meta_ArtworkURL  , "ArtworkURL"  },
			{ libvlc_meta_TrackID     , "TrackID"     },
#if ((LIBVLC_VERSION_MAJOR * 10000 + LIBVLC_VERSION_MINOR * 100 + LIBVLC_VERSION_REVISION) >= 20200)
			{ libvlc_meta_TrackTotal  , "TrackTotal"  },
			{ libvlc_meta_Director    , "Director"    },
			{ libvlc_meta_Season      , "Season"      },
			{ libvlc_meta_Episode     , "Episode"     },
			{ libvlc_meta_ShowName    , "ShowName"    },
			{ libvlc_meta_Actors      , "Actors"      },
#endif
#if ((LIBVLC_VERSION_MAJOR * 10000 + LIBVLC_VERSION_MINOR * 100 + LIBVLC_VERSION_REVISION) >= 30000)
			{ libvlc_meta_AlbumArtist , "AlbumArtist" },
			{ libvlc_meta_DiscNumber  , "DiscNumber"  },
			{ libvlc_meta_DiscTotal   , "DiscTotal"   },
#endif
			{ 0, NULL }
		};
	PPCODE:
		ref= sv_2mortal(newRV_noinc((SV*) (meta= newHV())));
		for (attr= attrlist; attr->name; attr++) {
			val= libvlc_media_get_meta(media, attr->code);
			if (val) hv_store(meta, attr->name, strlen(attr->name), newSVpv(val, 0), 0);
		}
		PUSHs(ref);

void
DESTROY(media)
	libvlc_media_t *media
	PPCODE:
		libvlc_media_release(media);

BOOT:
# BEGIN GENERATED BOOT CONSTANTS
  HV* stash= gv_stashpv("VideoLAN::LibVLC", GV_ADD);
  newCONSTSUB(stash, "Paused", newSViv(libvlc_Paused));
  newCONSTSUB(stash, "Playing", newSViv(libvlc_Playing));
  newCONSTSUB(stash, "Buffering", newSViv(libvlc_Buffering));
  newCONSTSUB(stash, "Stopped", newSViv(libvlc_Stopped));
  newCONSTSUB(stash, "Error", newSViv(libvlc_Error));
  newCONSTSUB(stash, "Opening", newSViv(libvlc_Opening));
  newCONSTSUB(stash, "Ended", newSViv(libvlc_Ended));
  newCONSTSUB(stash, "NothingSpecial", newSViv(libvlc_NothingSpecial));
  newCONSTSUB(stash, "track_audio", newSViv(libvlc_track_audio));
  newCONSTSUB(stash, "track_text", newSViv(libvlc_track_text));
  newCONSTSUB(stash, "track_unknown", newSViv(libvlc_track_unknown));
  newCONSTSUB(stash, "track_video", newSViv(libvlc_track_video));
  newCONSTSUB(stash, "meta_Album", newSViv(libvlc_meta_Album));
  newCONSTSUB(stash, "meta_Artist", newSViv(libvlc_meta_Artist));
  newCONSTSUB(stash, "meta_ArtworkURL", newSViv(libvlc_meta_ArtworkURL));
  newCONSTSUB(stash, "meta_Copyright", newSViv(libvlc_meta_Copyright));
  newCONSTSUB(stash, "meta_Date", newSViv(libvlc_meta_Date));
  newCONSTSUB(stash, "meta_Description", newSViv(libvlc_meta_Description));
  newCONSTSUB(stash, "meta_EncodedBy", newSViv(libvlc_meta_EncodedBy));
  newCONSTSUB(stash, "meta_Genre", newSViv(libvlc_meta_Genre));
  newCONSTSUB(stash, "meta_Language", newSViv(libvlc_meta_Language));
  newCONSTSUB(stash, "meta_NowPlaying", newSViv(libvlc_meta_NowPlaying));
  newCONSTSUB(stash, "meta_Publisher", newSViv(libvlc_meta_Publisher));
  newCONSTSUB(stash, "meta_Rating", newSViv(libvlc_meta_Rating));
  newCONSTSUB(stash, "meta_Setting", newSViv(libvlc_meta_Setting));
  newCONSTSUB(stash, "meta_Title", newSViv(libvlc_meta_Title));
  newCONSTSUB(stash, "meta_TrackID", newSViv(libvlc_meta_TrackID));
  newCONSTSUB(stash, "meta_TrackNumber", newSViv(libvlc_meta_TrackNumber));
  newCONSTSUB(stash, "meta_URL", newSViv(libvlc_meta_URL));
#if ((LIBVLC_VERSION_MAJOR * 10000 + LIBVLC_VERSION_MINOR * 100 + LIBVLC_VERSION_REVISION) >= 20200)
  newCONSTSUB(stash, "meta_Actors", newSViv(libvlc_meta_Actors));
  newCONSTSUB(stash, "meta_Director", newSViv(libvlc_meta_Director));
  newCONSTSUB(stash, "meta_Episode", newSViv(libvlc_meta_Episode));
  newCONSTSUB(stash, "meta_Season", newSViv(libvlc_meta_Season));
  newCONSTSUB(stash, "meta_ShowName", newSViv(libvlc_meta_ShowName));
  newCONSTSUB(stash, "meta_TrackTotal", newSViv(libvlc_meta_TrackTotal));
#else
  newXS("VideoLAN::LibVLC::meta_Actors", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::meta_Director", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::meta_Episode", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::meta_Season", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::meta_ShowName", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::meta_TrackTotal", XS_VideoLAN__LibVLC__const_unavailable, file);
#endif
#if ((LIBVLC_VERSION_MAJOR * 10000 + LIBVLC_VERSION_MINOR * 100 + LIBVLC_VERSION_REVISION) >= 30000)
  newCONSTSUB(stash, "video_orient_top_left", newSViv(libvlc_video_orient_top_left));
  newCONSTSUB(stash, "video_orient_top_right", newSViv(libvlc_video_orient_top_right));
  newCONSTSUB(stash, "video_orient_bottom_left", newSViv(libvlc_video_orient_bottom_left));
  newCONSTSUB(stash, "video_orient_bottom_right", newSViv(libvlc_video_orient_bottom_right));
  newCONSTSUB(stash, "video_orient_left_top", newSViv(libvlc_video_orient_left_top));
  newCONSTSUB(stash, "video_orient_left_bottom", newSViv(libvlc_video_orient_left_bottom));
  newCONSTSUB(stash, "video_orient_right_top", newSViv(libvlc_video_orient_right_top));
  newCONSTSUB(stash, "video_orient_right_bottom", newSViv(libvlc_video_orient_right_bottom));
  newCONSTSUB(stash, "video_projection_rectangular", newSViv(libvlc_video_projection_rectangular));
  newCONSTSUB(stash, "video_projection_equirectangular", newSViv(libvlc_video_projection_equirectangular));
  newCONSTSUB(stash, "video_projection_cubemap_layout_standard", newSViv(libvlc_video_projection_cubemap_layout_standard));
  newCONSTSUB(stash, "media_type_unknown", newSViv(libvlc_media_type_unknown));
  newCONSTSUB(stash, "media_type_file", newSViv(libvlc_media_type_file));
  newCONSTSUB(stash, "media_type_directory", newSViv(libvlc_media_type_directory));
  newCONSTSUB(stash, "media_type_disc", newSViv(libvlc_media_type_disc));
  newCONSTSUB(stash, "media_type_stream", newSViv(libvlc_media_type_stream));
  newCONSTSUB(stash, "media_type_playlist", newSViv(libvlc_media_type_playlist));
  newCONSTSUB(stash, "media_parse_local", newSViv(libvlc_media_parse_local));
  newCONSTSUB(stash, "media_parse_network", newSViv(libvlc_media_parse_network));
  newCONSTSUB(stash, "media_fetch_local", newSViv(libvlc_media_fetch_local));
  newCONSTSUB(stash, "media_fetch_network", newSViv(libvlc_media_fetch_network));
  newCONSTSUB(stash, "media_do_interact", newSViv(libvlc_media_do_interact));
  newCONSTSUB(stash, "media_parsed_status_skipped", newSViv(libvlc_media_parsed_status_skipped));
  newCONSTSUB(stash, "media_parsed_status_failed", newSViv(libvlc_media_parsed_status_failed));
  newCONSTSUB(stash, "media_parsed_status_timeout", newSViv(libvlc_media_parsed_status_timeout));
  newCONSTSUB(stash, "media_parsed_status_done", newSViv(libvlc_media_parsed_status_done));
  newCONSTSUB(stash, "media_slave_type_subtitle", newSViv(libvlc_media_slave_type_subtitle));
  newCONSTSUB(stash, "media_slave_type_audio", newSViv(libvlc_media_slave_type_audio));
#else
  newXS("VideoLAN::LibVLC::video_orient_top_left", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::video_orient_top_right", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::video_orient_bottom_left", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::video_orient_bottom_right", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::video_orient_left_top", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::video_orient_left_bottom", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::video_orient_right_top", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::video_orient_right_bottom", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::video_projection_rectangular", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::video_projection_equirectangular", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::video_projection_cubemap_layout_standard", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_type_unknown", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_type_file", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_type_directory", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_type_disc", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_type_stream", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_type_playlist", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_parse_local", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_parse_network", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_fetch_local", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_fetch_network", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_do_interact", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_parsed_status_skipped", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_parsed_status_failed", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_parsed_status_timeout", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_parsed_status_done", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_slave_type_subtitle", XS_VideoLAN__LibVLC__const_unavailable, file);
  newXS("VideoLAN::LibVLC::media_slave_type_audio", XS_VideoLAN__LibVLC__const_unavailable, file);
#endif
# END GENERATED BOOT CONSTANTS
#

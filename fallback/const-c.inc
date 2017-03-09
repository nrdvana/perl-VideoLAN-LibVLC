#define PERL_constant_NOTFOUND	1
#define PERL_constant_NOTDEF	2
#define PERL_constant_ISIV	3
#define PERL_constant_ISNO	4
#define PERL_constant_ISNV	5
#define PERL_constant_ISPV	6
#define PERL_constant_ISPVN	7
#define PERL_constant_ISSV	8
#define PERL_constant_ISUNDEF	9
#define PERL_constant_ISUV	10
#define PERL_constant_ISYES	11

#ifndef NVTYPE
typedef double NV; /* 5.6 and later define NVTYPE, and typedef NV to it.  */
#endif
#ifndef aTHX_
#define aTHX_ /* 5.6 or later define this for threading support.  */
#endif
#ifndef pTHX_
#define pTHX_ /* 5.6 or later define this for threading support.  */
#endif

static int
constant_12 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     LIBVLC_DEBUG LIBVLC_ERROR libvlc_Ended libvlc_Error */
  /* Offset 10 gives the best switch position.  */
  switch (name[10]) {
  case 'O':
    if (memEQ(name, "LIBVLC_ERROR", 12)) {
    /*                         ^        */
#ifdef LIBVLC_ERROR
      *iv_return = LIBVLC_ERROR;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'U':
    if (memEQ(name, "LIBVLC_DEBUG", 12)) {
    /*                         ^        */
#ifdef LIBVLC_DEBUG
      *iv_return = LIBVLC_DEBUG;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'e':
    if (memEQ(name, "libvlc_Ended", 12)) {
    /*                         ^        */
#ifdef libvlc_Ended
      *iv_return = libvlc_Ended;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'o':
    if (memEQ(name, "libvlc_Error", 12)) {
    /*                         ^        */
#ifdef libvlc_Error
      *iv_return = libvlc_Error;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_13 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     LIBVLC_NOTICE libvlc_Paused libvlc_logo_x libvlc_logo_y */
  /* Offset 12 gives the best switch position.  */
  switch (name[12]) {
  case 'E':
    if (memEQ(name, "LIBVLC_NOTIC", 12)) {
    /*                           E      */
#ifdef LIBVLC_NOTICE
      *iv_return = LIBVLC_NOTICE;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'd':
    if (memEQ(name, "libvlc_Pause", 12)) {
    /*                           d      */
#ifdef libvlc_Paused
      *iv_return = libvlc_Paused;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'x':
    if (memEQ(name, "libvlc_logo_", 12)) {
    /*                           x      */
#ifdef libvlc_logo_x
      *iv_return = libvlc_logo_x;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'y':
    if (memEQ(name, "libvlc_logo_", 12)) {
    /*                           y      */
#ifdef libvlc_logo_y
      *iv_return = libvlc_logo_y;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_14 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     LIBVLC_WARNING libvlc_Opening libvlc_Playing libvlc_Stopped */
  /* Offset 7 gives the best switch position.  */
  switch (name[7]) {
  case 'O':
    if (memEQ(name, "libvlc_Opening", 14)) {
    /*                      ^             */
#ifdef libvlc_Opening
      *iv_return = libvlc_Opening;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'P':
    if (memEQ(name, "libvlc_Playing", 14)) {
    /*                      ^             */
#ifdef libvlc_Playing
      *iv_return = libvlc_Playing;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'S':
    if (memEQ(name, "libvlc_Stopped", 14)) {
    /*                      ^             */
#ifdef libvlc_Stopped
      *iv_return = libvlc_Stopped;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'W':
    if (memEQ(name, "LIBVLC_WARNING", 14)) {
    /*                      ^             */
#ifdef LIBVLC_WARNING
      *iv_return = LIBVLC_WARNING;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_16 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     LIBVLC_VERSION_H libvlc_Buffering libvlc_logo_file libvlc_marquee_X
     libvlc_marquee_Y libvlc_meta_Date */
  /* Offset 10 gives the best switch position.  */
  switch (name[10]) {
  case 'S':
    if (memEQ(name, "LIBVLC_VERSION_H", 16)) {
    /*                         ^            */
#ifdef LIBVLC_VERSION_H
      *iv_return = LIBVLC_VERSION_H;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'a':
    if (memEQ(name, "libvlc_meta_Date", 16)) {
    /*                         ^            */
#ifdef libvlc_meta_Date
      *iv_return = libvlc_meta_Date;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'f':
    if (memEQ(name, "libvlc_Buffering", 16)) {
    /*                         ^            */
#ifdef libvlc_Buffering
      *iv_return = libvlc_Buffering;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'o':
    if (memEQ(name, "libvlc_logo_file", 16)) {
    /*                         ^            */
#ifdef libvlc_logo_file
      *iv_return = libvlc_logo_file;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'q':
    if (memEQ(name, "libvlc_marquee_X", 16)) {
    /*                         ^            */
#ifdef libvlc_marquee_X
      *iv_return = libvlc_marquee_X;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    if (memEQ(name, "libvlc_marquee_Y", 16)) {
    /*                         ^            */
#ifdef libvlc_marquee_Y
      *iv_return = libvlc_marquee_Y;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_17 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_MediaFreed libvlc_adjust_Hue libvlc_logo_delay libvlc_meta_Album
     libvlc_meta_Genre libvlc_meta_Title libvlc_track_text */
  /* Offset 12 gives the best switch position.  */
  switch (name[12]) {
  case 'A':
    if (memEQ(name, "libvlc_meta_Album", 17)) {
    /*                           ^           */
#ifdef libvlc_meta_Album
      *iv_return = libvlc_meta_Album;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'F':
    if (memEQ(name, "libvlc_MediaFreed", 17)) {
    /*                           ^           */
#ifdef libvlc_MediaFreed
      *iv_return = libvlc_MediaFreed;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'G':
    if (memEQ(name, "libvlc_meta_Genre", 17)) {
    /*                           ^           */
#ifdef libvlc_meta_Genre
      *iv_return = libvlc_meta_Genre;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'T':
    if (memEQ(name, "libvlc_meta_Title", 17)) {
    /*                           ^           */
#ifdef libvlc_meta_Title
      *iv_return = libvlc_meta_Title;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case '_':
    if (memEQ(name, "libvlc_track_text", 17)) {
    /*                           ^           */
#ifdef libvlc_track_text
      *iv_return = libvlc_track_text;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'd':
    if (memEQ(name, "libvlc_logo_delay", 17)) {
    /*                           ^           */
#ifdef libvlc_logo_delay
      *iv_return = libvlc_logo_delay;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 't':
    if (memEQ(name, "libvlc_adjust_Hue", 17)) {
    /*                           ^           */
#ifdef libvlc_adjust_Hue
      *iv_return = libvlc_adjust_Hue;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_18 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     LIBVLC_VERSION_INT libvlc_logo_enable libvlc_logo_repeat
     libvlc_meta_Actors libvlc_meta_Artist libvlc_meta_Rating
     libvlc_meta_Season libvlc_navigate_up libvlc_track_audio
     libvlc_track_video */
  /* Offset 16 gives the best switch position.  */
  switch (name[16]) {
  case 'N':
    if (memEQ(name, "LIBVLC_VERSION_INT", 18)) {
    /*                               ^        */
#ifdef LIBVLC_VERSION_INT
      *iv_return = LIBVLC_VERSION_INT;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'a':
    if (memEQ(name, "libvlc_logo_repeat", 18)) {
    /*                               ^        */
#ifdef libvlc_logo_repeat
      *iv_return = libvlc_logo_repeat;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'e':
    if (memEQ(name, "libvlc_track_video", 18)) {
    /*                               ^        */
#ifdef libvlc_track_video
      *iv_return = libvlc_track_video;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'i':
    if (memEQ(name, "libvlc_track_audio", 18)) {
    /*                               ^        */
#ifdef libvlc_track_audio
      *iv_return = libvlc_track_audio;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'l':
    if (memEQ(name, "libvlc_logo_enable", 18)) {
    /*                               ^        */
#ifdef libvlc_logo_enable
      *iv_return = libvlc_logo_enable;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'n':
    if (memEQ(name, "libvlc_meta_Rating", 18)) {
    /*                               ^        */
#ifdef libvlc_meta_Rating
      *iv_return = libvlc_meta_Rating;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'o':
    if (memEQ(name, "libvlc_meta_Season", 18)) {
    /*                               ^        */
#ifdef libvlc_meta_Season
      *iv_return = libvlc_meta_Season;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'r':
    if (memEQ(name, "libvlc_meta_Actors", 18)) {
    /*                               ^        */
#ifdef libvlc_meta_Actors
      *iv_return = libvlc_meta_Actors;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 's':
    if (memEQ(name, "libvlc_meta_Artist", 18)) {
    /*                               ^        */
#ifdef libvlc_meta_Artist
      *iv_return = libvlc_meta_Artist;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'u':
    if (memEQ(name, "libvlc_navigate_up", 18)) {
    /*                               ^        */
#ifdef libvlc_navigate_up
      *iv_return = libvlc_navigate_up;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_19 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_adjust_Gamma libvlc_logo_opacity libvlc_marquee_Size
     libvlc_marquee_Text libvlc_meta_Episode libvlc_meta_Setting
     libvlc_meta_TrackID libvlc_position_top */
  /* Offset 17 gives the best switch position.  */
  switch (name[17]) {
  case 'I':
    if (memEQ(name, "libvlc_meta_TrackID", 19)) {
    /*                                ^        */
#ifdef libvlc_meta_TrackID
      *iv_return = libvlc_meta_TrackID;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'd':
    if (memEQ(name, "libvlc_meta_Episode", 19)) {
    /*                                ^        */
#ifdef libvlc_meta_Episode
      *iv_return = libvlc_meta_Episode;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'm':
    if (memEQ(name, "libvlc_adjust_Gamma", 19)) {
    /*                                ^        */
#ifdef libvlc_adjust_Gamma
      *iv_return = libvlc_adjust_Gamma;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'n':
    if (memEQ(name, "libvlc_meta_Setting", 19)) {
    /*                                ^        */
#ifdef libvlc_meta_Setting
      *iv_return = libvlc_meta_Setting;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'o':
    if (memEQ(name, "libvlc_position_top", 19)) {
    /*                                ^        */
#ifdef libvlc_position_top
      *iv_return = libvlc_position_top;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 't':
    if (memEQ(name, "libvlc_logo_opacity", 19)) {
    /*                                ^        */
#ifdef libvlc_logo_opacity
      *iv_return = libvlc_logo_opacity;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'x':
    if (memEQ(name, "libvlc_marquee_Text", 19)) {
    /*                                ^        */
#ifdef libvlc_marquee_Text
      *iv_return = libvlc_marquee_Text;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'z':
    if (memEQ(name, "libvlc_marquee_Size", 19)) {
    /*                                ^        */
#ifdef libvlc_marquee_Size
      *iv_return = libvlc_marquee_Size;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_20 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     LIBVLC_VERSION_EXTRA LIBVLC_VERSION_MAJOR LIBVLC_VERSION_MINOR
     libvlc_VlmMediaAdded libvlc_adjust_Enable libvlc_logo_position
     libvlc_marquee_Color libvlc_meta_Director libvlc_meta_Language
     libvlc_meta_ShowName libvlc_navigate_down libvlc_navigate_left
     libvlc_position_left libvlc_track_unknown */
  /* Offset 16 gives the best switch position.  */
  switch (name[16]) {
  case 'A':
    if (memEQ(name, "LIBVLC_VERSION_MAJOR", 20)) {
    /*                               ^          */
#ifdef LIBVLC_VERSION_MAJOR
      *iv_return = LIBVLC_VERSION_MAJOR;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'I':
    if (memEQ(name, "LIBVLC_VERSION_MINOR", 20)) {
    /*                               ^          */
#ifdef LIBVLC_VERSION_MINOR
      *iv_return = LIBVLC_VERSION_MINOR;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'N':
    if (memEQ(name, "libvlc_meta_ShowName", 20)) {
    /*                               ^          */
#ifdef libvlc_meta_ShowName
      *iv_return = libvlc_meta_ShowName;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'X':
    if (memEQ(name, "LIBVLC_VERSION_EXTRA", 20)) {
    /*                               ^          */
#ifdef LIBVLC_VERSION_EXTRA
      *iv_return = LIBVLC_VERSION_EXTRA;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'a':
    if (memEQ(name, "libvlc_adjust_Enable", 20)) {
    /*                               ^          */
#ifdef libvlc_adjust_Enable
      *iv_return = libvlc_adjust_Enable;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'c':
    if (memEQ(name, "libvlc_meta_Director", 20)) {
    /*                               ^          */
#ifdef libvlc_meta_Director
      *iv_return = libvlc_meta_Director;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'd':
    if (memEQ(name, "libvlc_VlmMediaAdded", 20)) {
    /*                               ^          */
#ifdef libvlc_VlmMediaAdded
      *iv_return = libvlc_VlmMediaAdded;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    if (memEQ(name, "libvlc_navigate_down", 20)) {
    /*                               ^          */
#ifdef libvlc_navigate_down
      *iv_return = libvlc_navigate_down;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'l':
    if (memEQ(name, "libvlc_navigate_left", 20)) {
    /*                               ^          */
#ifdef libvlc_navigate_left
      *iv_return = libvlc_navigate_left;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    if (memEQ(name, "libvlc_position_left", 20)) {
    /*                               ^          */
#ifdef libvlc_position_left
      *iv_return = libvlc_position_left;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'n':
    if (memEQ(name, "libvlc_track_unknown", 20)) {
    /*                               ^          */
#ifdef libvlc_track_unknown
      *iv_return = libvlc_track_unknown;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'o':
    if (memEQ(name, "libvlc_marquee_Color", 20)) {
    /*                               ^          */
#ifdef libvlc_marquee_Color
      *iv_return = libvlc_marquee_Color;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 't':
    if (memEQ(name, "libvlc_logo_position", 20)) {
    /*                               ^          */
#ifdef libvlc_logo_position
      *iv_return = libvlc_logo_position;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'u':
    if (memEQ(name, "libvlc_meta_Language", 20)) {
    /*                               ^          */
#ifdef libvlc_meta_Language
      *iv_return = libvlc_meta_Language;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_21 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_NothingSpecial libvlc_marquee_Enable libvlc_meta_Copyright
     libvlc_meta_EncodedBy libvlc_meta_Publisher libvlc_navigate_right
     libvlc_position_right */
  /* Offset 14 gives the best switch position.  */
  switch (name[14]) {
  case 'S':
    if (memEQ(name, "libvlc_NothingSpecial", 21)) {
    /*                             ^             */
#ifdef libvlc_NothingSpecial
      *iv_return = libvlc_NothingSpecial;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case '_':
    if (memEQ(name, "libvlc_marquee_Enable", 21)) {
    /*                             ^             */
#ifdef libvlc_marquee_Enable
      *iv_return = libvlc_marquee_Enable;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'b':
    if (memEQ(name, "libvlc_meta_Publisher", 21)) {
    /*                             ^             */
#ifdef libvlc_meta_Publisher
      *iv_return = libvlc_meta_Publisher;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'c':
    if (memEQ(name, "libvlc_meta_EncodedBy", 21)) {
    /*                             ^             */
#ifdef libvlc_meta_EncodedBy
      *iv_return = libvlc_meta_EncodedBy;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'e':
    if (memEQ(name, "libvlc_navigate_right", 21)) {
    /*                             ^             */
#ifdef libvlc_navigate_right
      *iv_return = libvlc_navigate_right;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'n':
    if (memEQ(name, "libvlc_position_right", 21)) {
    /*                             ^             */
#ifdef libvlc_position_right
      *iv_return = libvlc_position_right;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'p':
    if (memEQ(name, "libvlc_meta_Copyright", 21)) {
    /*                             ^             */
#ifdef libvlc_meta_Copyright
      *iv_return = libvlc_meta_Copyright;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_22 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_MediaPlayerVout libvlc_VlmMediaChanged libvlc_VlmMediaRemoved
     libvlc_adjust_Contrast libvlc_marquee_Opacity libvlc_marquee_Refresh
     libvlc_marquee_Timeout libvlc_meta_ArtworkURL libvlc_meta_NowPlaying
     libvlc_meta_TrackTotal libvlc_position_bottom libvlc_position_center */
  /* Offset 15 gives the best switch position.  */
  switch (name[15]) {
  case 'C':
    if (memEQ(name, "libvlc_VlmMediaChanged", 22)) {
    /*                              ^             */
#ifdef libvlc_VlmMediaChanged
      *iv_return = libvlc_VlmMediaChanged;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'O':
    if (memEQ(name, "libvlc_marquee_Opacity", 22)) {
    /*                              ^             */
#ifdef libvlc_marquee_Opacity
      *iv_return = libvlc_marquee_Opacity;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'P':
    if (memEQ(name, "libvlc_meta_NowPlaying", 22)) {
    /*                              ^             */
#ifdef libvlc_meta_NowPlaying
      *iv_return = libvlc_meta_NowPlaying;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'R':
    if (memEQ(name, "libvlc_VlmMediaRemoved", 22)) {
    /*                              ^             */
#ifdef libvlc_VlmMediaRemoved
      *iv_return = libvlc_VlmMediaRemoved;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    if (memEQ(name, "libvlc_marquee_Refresh", 22)) {
    /*                              ^             */
#ifdef libvlc_marquee_Refresh
      *iv_return = libvlc_marquee_Refresh;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'T':
    if (memEQ(name, "libvlc_marquee_Timeout", 22)) {
    /*                              ^             */
#ifdef libvlc_marquee_Timeout
      *iv_return = libvlc_marquee_Timeout;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case '_':
    if (memEQ(name, "libvlc_position_bottom", 22)) {
    /*                              ^             */
#ifdef libvlc_position_bottom
      *iv_return = libvlc_position_bottom;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    if (memEQ(name, "libvlc_position_center", 22)) {
    /*                              ^             */
#ifdef libvlc_position_center
      *iv_return = libvlc_position_center;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'c':
    if (memEQ(name, "libvlc_meta_TrackTotal", 22)) {
    /*                              ^             */
#ifdef libvlc_meta_TrackTotal
      *iv_return = libvlc_meta_TrackTotal;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'o':
    if (memEQ(name, "libvlc_adjust_Contrast", 22)) {
    /*                              ^             */
#ifdef libvlc_adjust_Contrast
      *iv_return = libvlc_adjust_Contrast;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'w':
    if (memEQ(name, "libvlc_meta_ArtworkURL", 22)) {
    /*                              ^             */
#ifdef libvlc_meta_ArtworkURL
      *iv_return = libvlc_meta_ArtworkURL;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'y':
    if (memEQ(name, "libvlc_MediaPlayerVout", 22)) {
    /*                              ^             */
#ifdef libvlc_MediaPlayerVout
      *iv_return = libvlc_MediaPlayerVout;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_23 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     LIBVLC_VERSION_REVISION libvlc_MediaMetaChanged libvlc_MediaPlayerMuted
     libvlc_marquee_Position libvlc_meta_Description libvlc_meta_TrackNumber
     libvlc_position_disable */
  /* Offset 12 gives the best switch position.  */
  switch (name[12]) {
  case 'D':
    if (memEQ(name, "libvlc_meta_Description", 23)) {
    /*                           ^                 */
#ifdef libvlc_meta_Description
      *iv_return = libvlc_meta_Description;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'M':
    if (memEQ(name, "libvlc_MediaMetaChanged", 23)) {
    /*                           ^                 */
#ifdef libvlc_MediaMetaChanged
      *iv_return = libvlc_MediaMetaChanged;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'O':
    if (memEQ(name, "LIBVLC_VERSION_REVISION", 23)) {
    /*                           ^                 */
#ifdef LIBVLC_VERSION_REVISION
      *iv_return = LIBVLC_VERSION_REVISION;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'P':
    if (memEQ(name, "libvlc_MediaPlayerMuted", 23)) {
    /*                           ^                 */
#ifdef libvlc_MediaPlayerMuted
      *iv_return = libvlc_MediaPlayerMuted;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'T':
    if (memEQ(name, "libvlc_meta_TrackNumber", 23)) {
    /*                           ^                 */
#ifdef libvlc_meta_TrackNumber
      *iv_return = libvlc_meta_TrackNumber;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'e':
    if (memEQ(name, "libvlc_marquee_Position", 23)) {
    /*                           ^                 */
#ifdef libvlc_marquee_Position
      *iv_return = libvlc_marquee_Position;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'i':
    if (memEQ(name, "libvlc_position_disable", 23)) {
    /*                           ^                 */
#ifdef libvlc_position_disable
      *iv_return = libvlc_position_disable;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_24 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_AudioChannel_Left libvlc_MediaPlayerCorked libvlc_MediaPlayerPaused
     libvlc_MediaStateChanged libvlc_MediaSubItemAdded libvlc_adjust_Brightness
     libvlc_adjust_Saturation libvlc_navigate_activate libvlc_position_top_left
     */
  /* Offset 20 gives the best switch position.  */
  switch (name[20]) {
  case 'L':
    if (memEQ(name, "libvlc_AudioChannel_Left", 24)) {
    /*                                   ^          */
#ifdef libvlc_AudioChannel_Left
      *iv_return = libvlc_AudioChannel_Left;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'd':
    if (memEQ(name, "libvlc_MediaSubItemAdded", 24)) {
    /*                                   ^          */
#ifdef libvlc_MediaSubItemAdded
      *iv_return = libvlc_MediaSubItemAdded;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'l':
    if (memEQ(name, "libvlc_position_top_left", 24)) {
    /*                                   ^          */
#ifdef libvlc_position_top_left
      *iv_return = libvlc_position_top_left;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'n':
    if (memEQ(name, "libvlc_MediaStateChanged", 24)) {
    /*                                   ^          */
#ifdef libvlc_MediaStateChanged
      *iv_return = libvlc_MediaStateChanged;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    if (memEQ(name, "libvlc_adjust_Brightness", 24)) {
    /*                                   ^          */
#ifdef libvlc_adjust_Brightness
      *iv_return = libvlc_adjust_Brightness;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'r':
    if (memEQ(name, "libvlc_MediaPlayerCorked", 24)) {
    /*                                   ^          */
#ifdef libvlc_MediaPlayerCorked
      *iv_return = libvlc_MediaPlayerCorked;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 't':
    if (memEQ(name, "libvlc_adjust_Saturation", 24)) {
    /*                                   ^          */
#ifdef libvlc_adjust_Saturation
      *iv_return = libvlc_adjust_Saturation;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'u':
    if (memEQ(name, "libvlc_MediaPlayerPaused", 24)) {
    /*                                   ^          */
#ifdef libvlc_MediaPlayerPaused
      *iv_return = libvlc_MediaPlayerPaused;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'v':
    if (memEQ(name, "libvlc_navigate_activate", 24)) {
    /*                                   ^          */
#ifdef libvlc_navigate_activate
      *iv_return = libvlc_navigate_activate;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_25 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_AudioChannel_Error libvlc_AudioChannel_Right
     libvlc_MediaListItemAdded libvlc_MediaParsedChanged
     libvlc_MediaPlayerForward libvlc_MediaPlayerOpening
     libvlc_MediaPlayerPlaying libvlc_MediaPlayerStopped
     libvlc_MediaPlayerUnmuted libvlc_playback_mode_loop
     libvlc_position_top_right */
  /* Offset 18 gives the best switch position.  */
  switch (name[18]) {
  case 'C':
    if (memEQ(name, "libvlc_MediaParsedChanged", 25)) {
    /*                                 ^             */
#ifdef libvlc_MediaParsedChanged
      *iv_return = libvlc_MediaParsedChanged;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'F':
    if (memEQ(name, "libvlc_MediaPlayerForward", 25)) {
    /*                                 ^             */
#ifdef libvlc_MediaPlayerForward
      *iv_return = libvlc_MediaPlayerForward;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'O':
    if (memEQ(name, "libvlc_MediaPlayerOpening", 25)) {
    /*                                 ^             */
#ifdef libvlc_MediaPlayerOpening
      *iv_return = libvlc_MediaPlayerOpening;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'P':
    if (memEQ(name, "libvlc_MediaPlayerPlaying", 25)) {
    /*                                 ^             */
#ifdef libvlc_MediaPlayerPlaying
      *iv_return = libvlc_MediaPlayerPlaying;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'S':
    if (memEQ(name, "libvlc_MediaPlayerStopped", 25)) {
    /*                                 ^             */
#ifdef libvlc_MediaPlayerStopped
      *iv_return = libvlc_MediaPlayerStopped;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'U':
    if (memEQ(name, "libvlc_MediaPlayerUnmuted", 25)) {
    /*                                 ^             */
#ifdef libvlc_MediaPlayerUnmuted
      *iv_return = libvlc_MediaPlayerUnmuted;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'd':
    if (memEQ(name, "libvlc_playback_mode_loop", 25)) {
    /*                                 ^             */
#ifdef libvlc_playback_mode_loop
      *iv_return = libvlc_playback_mode_loop;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'e':
    if (memEQ(name, "libvlc_MediaListItemAdded", 25)) {
    /*                                 ^             */
#ifdef libvlc_MediaListItemAdded
      *iv_return = libvlc_MediaListItemAdded;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'l':
    if (memEQ(name, "libvlc_AudioChannel_Error", 25)) {
    /*                                 ^             */
#ifdef libvlc_AudioChannel_Error
      *iv_return = libvlc_AudioChannel_Error;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    if (memEQ(name, "libvlc_AudioChannel_Right", 25)) {
    /*                                 ^             */
#ifdef libvlc_AudioChannel_Right
      *iv_return = libvlc_AudioChannel_Right;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'p':
    if (memEQ(name, "libvlc_position_top_right", 25)) {
    /*                                 ^             */
#ifdef libvlc_position_top_right
      *iv_return = libvlc_position_top_right;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_26 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_AudioChannel_Dolbys libvlc_AudioChannel_Stereo
     libvlc_MediaPlayerBackward libvlc_MediaPlayerUncorked
     libvlc_media_option_unique */
  /* Offset 23 gives the best switch position.  */
  switch (name[23]) {
  case 'a':
    if (memEQ(name, "libvlc_MediaPlayerBackward", 26)) {
    /*                                      ^         */
#ifdef libvlc_MediaPlayerBackward
      *iv_return = libvlc_MediaPlayerBackward;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'b':
    if (memEQ(name, "libvlc_AudioChannel_Dolbys", 26)) {
    /*                                      ^         */
#ifdef libvlc_AudioChannel_Dolbys
      *iv_return = libvlc_AudioChannel_Dolbys;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'k':
    if (memEQ(name, "libvlc_MediaPlayerUncorked", 26)) {
    /*                                      ^         */
#ifdef libvlc_MediaPlayerUncorked
      *iv_return = libvlc_MediaPlayerUncorked;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'q':
    if (memEQ(name, "libvlc_media_option_unique", 26)) {
    /*                                      ^         */
#ifdef libvlc_media_option_unique
      *iv_return = libvlc_media_option_unique;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'r':
    if (memEQ(name, "libvlc_AudioChannel_Stereo", 26)) {
    /*                                      ^         */
#ifdef libvlc_AudioChannel_Stereo
      *iv_return = libvlc_AudioChannel_Stereo;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_27 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_AudioChannel_RStereo libvlc_MediaDiscovererEnded
     libvlc_MediaDurationChanged libvlc_MediaListItemDeleted
     libvlc_MediaListWillAddItem libvlc_MediaPlayerBuffering
     libvlc_media_option_trusted libvlc_playback_mode_repeat
     libvlc_position_bottom_left */
  /* Offset 16 gives the best switch position.  */
  switch (name[16]) {
  case 'I':
    if (memEQ(name, "libvlc_MediaListItemDeleted", 27)) {
    /*                               ^                 */
#ifdef libvlc_MediaListItemDeleted
      *iv_return = libvlc_MediaListItemDeleted;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'W':
    if (memEQ(name, "libvlc_MediaListWillAddItem", 27)) {
    /*                               ^                 */
#ifdef libvlc_MediaListWillAddItem
      *iv_return = libvlc_MediaListWillAddItem;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'b':
    if (memEQ(name, "libvlc_position_bottom_left", 27)) {
    /*                               ^                 */
#ifdef libvlc_position_bottom_left
      *iv_return = libvlc_position_bottom_left;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'e':
    if (memEQ(name, "libvlc_MediaPlayerBuffering", 27)) {
    /*                               ^                 */
#ifdef libvlc_MediaPlayerBuffering
      *iv_return = libvlc_MediaPlayerBuffering;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'i':
    if (memEQ(name, "libvlc_media_option_trusted", 27)) {
    /*                               ^                 */
#ifdef libvlc_media_option_trusted
      *iv_return = libvlc_media_option_trusted;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'm':
    if (memEQ(name, "libvlc_playback_mode_repeat", 27)) {
    /*                               ^                 */
#ifdef libvlc_playback_mode_repeat
      *iv_return = libvlc_playback_mode_repeat;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'n':
    if (memEQ(name, "libvlc_AudioChannel_RStereo", 27)) {
    /*                               ^                 */
#ifdef libvlc_AudioChannel_RStereo
      *iv_return = libvlc_AudioChannel_RStereo;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'o':
    if (memEQ(name, "libvlc_MediaDiscovererEnded", 27)) {
    /*                               ^                 */
#ifdef libvlc_MediaDiscovererEnded
      *iv_return = libvlc_MediaDiscovererEnded;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 't':
    if (memEQ(name, "libvlc_MediaDurationChanged", 27)) {
    /*                               ^                 */
#ifdef libvlc_MediaDurationChanged
      *iv_return = libvlc_MediaDurationChanged;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_28 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_AudioOutputDevice_5_1 libvlc_AudioOutputDevice_6_1
     libvlc_AudioOutputDevice_7_1 libvlc_MediaListPlayerPlayed
     libvlc_MediaPlayerEndReached libvlc_MediaSubItemTreeAdded
     libvlc_playback_mode_default libvlc_position_bottom_right */
  /* Offset 25 gives the best switch position.  */
  switch (name[25]) {
  case '5':
    if (memEQ(name, "libvlc_AudioOutputDevice_5_1", 28)) {
    /*                                        ^         */
#ifdef libvlc_AudioOutputDevice_5_1
      *iv_return = libvlc_AudioOutputDevice_5_1;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case '6':
    if (memEQ(name, "libvlc_AudioOutputDevice_6_1", 28)) {
    /*                                        ^         */
#ifdef libvlc_AudioOutputDevice_6_1
      *iv_return = libvlc_AudioOutputDevice_6_1;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case '7':
    if (memEQ(name, "libvlc_AudioOutputDevice_7_1", 28)) {
    /*                                        ^         */
#ifdef libvlc_AudioOutputDevice_7_1
      *iv_return = libvlc_AudioOutputDevice_7_1;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'd':
    if (memEQ(name, "libvlc_MediaSubItemTreeAdded", 28)) {
    /*                                        ^         */
#ifdef libvlc_MediaSubItemTreeAdded
      *iv_return = libvlc_MediaSubItemTreeAdded;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'g':
    if (memEQ(name, "libvlc_position_bottom_right", 28)) {
    /*                                        ^         */
#ifdef libvlc_position_bottom_right
      *iv_return = libvlc_position_bottom_right;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'h':
    if (memEQ(name, "libvlc_MediaPlayerEndReached", 28)) {
    /*                                        ^         */
#ifdef libvlc_MediaPlayerEndReached
      *iv_return = libvlc_MediaPlayerEndReached;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'u':
    if (memEQ(name, "libvlc_playback_mode_default", 28)) {
    /*                                        ^         */
#ifdef libvlc_playback_mode_default
      *iv_return = libvlc_playback_mode_default;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'y':
    if (memEQ(name, "libvlc_MediaListPlayerPlayed", 28)) {
    /*                                        ^         */
#ifdef libvlc_MediaListPlayerPlayed
      *iv_return = libvlc_MediaListPlayerPlayed;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_29 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_AudioOutputDevice_2F2R libvlc_AudioOutputDevice_3F2R
     libvlc_AudioOutputDevice_Mono libvlc_MediaDiscovererStarted
     libvlc_MediaListPlayerStopped libvlc_MediaListViewItemAdded
     libvlc_MediaPlayerAudioVolume libvlc_MediaPlayerTimeChanged */
  /* Offset 25 gives the best switch position.  */
  switch (name[25]) {
  case '2':
    if (memEQ(name, "libvlc_AudioOutputDevice_2F2R", 29)) {
    /*                                        ^          */
#ifdef libvlc_AudioOutputDevice_2F2R
      *iv_return = libvlc_AudioOutputDevice_2F2R;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case '3':
    if (memEQ(name, "libvlc_AudioOutputDevice_3F2R", 29)) {
    /*                                        ^          */
#ifdef libvlc_AudioOutputDevice_3F2R
      *iv_return = libvlc_AudioOutputDevice_3F2R;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'M':
    if (memEQ(name, "libvlc_AudioOutputDevice_Mono", 29)) {
    /*                                        ^          */
#ifdef libvlc_AudioOutputDevice_Mono
      *iv_return = libvlc_AudioOutputDevice_Mono;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'd':
    if (memEQ(name, "libvlc_MediaListViewItemAdded", 29)) {
    /*                                        ^          */
#ifdef libvlc_MediaListViewItemAdded
      *iv_return = libvlc_MediaListViewItemAdded;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'l':
    if (memEQ(name, "libvlc_MediaPlayerAudioVolume", 29)) {
    /*                                        ^          */
#ifdef libvlc_MediaPlayerAudioVolume
      *iv_return = libvlc_MediaPlayerAudioVolume;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'n':
    if (memEQ(name, "libvlc_MediaPlayerTimeChanged", 29)) {
    /*                                        ^          */
#ifdef libvlc_MediaPlayerTimeChanged
      *iv_return = libvlc_MediaPlayerTimeChanged;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'p':
    if (memEQ(name, "libvlc_MediaListPlayerStopped", 29)) {
    /*                                        ^          */
#ifdef libvlc_MediaListPlayerStopped
      *iv_return = libvlc_MediaListPlayerStopped;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'r':
    if (memEQ(name, "libvlc_MediaDiscovererStarted", 29)) {
    /*                                        ^          */
#ifdef libvlc_MediaDiscovererStarted
      *iv_return = libvlc_MediaDiscovererStarted;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_30 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_AudioOutputDevice_Error libvlc_AudioOutputDevice_SPDIF
     libvlc_MediaListWillDeleteItem libvlc_MediaPlayerMediaChanged
     libvlc_MediaPlayerTitleChanged libvlc_VlmMediaInstanceStarted
     libvlc_VlmMediaInstanceStopped */
  /* Offset 26 gives the best switch position.  */
  switch (name[26]) {
  case 'I':
    if (memEQ(name, "libvlc_MediaListWillDeleteItem", 30)) {
    /*                                         ^          */
#ifdef libvlc_MediaListWillDeleteItem
      *iv_return = libvlc_MediaListWillDeleteItem;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'P':
    if (memEQ(name, "libvlc_AudioOutputDevice_SPDIF", 30)) {
    /*                                         ^          */
#ifdef libvlc_AudioOutputDevice_SPDIF
      *iv_return = libvlc_AudioOutputDevice_SPDIF;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'n':
    if (memEQ(name, "libvlc_MediaPlayerMediaChanged", 30)) {
    /*                                         ^          */
#ifdef libvlc_MediaPlayerMediaChanged
      *iv_return = libvlc_MediaPlayerMediaChanged;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    if (memEQ(name, "libvlc_MediaPlayerTitleChanged", 30)) {
    /*                                         ^          */
#ifdef libvlc_MediaPlayerTitleChanged
      *iv_return = libvlc_MediaPlayerTitleChanged;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'p':
    if (memEQ(name, "libvlc_VlmMediaInstanceStopped", 30)) {
    /*                                         ^          */
#ifdef libvlc_VlmMediaInstanceStopped
      *iv_return = libvlc_VlmMediaInstanceStopped;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'r':
    if (memEQ(name, "libvlc_AudioOutputDevice_Error", 30)) {
    /*                                         ^          */
#ifdef libvlc_AudioOutputDevice_Error
      *iv_return = libvlc_AudioOutputDevice_Error;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    if (memEQ(name, "libvlc_VlmMediaInstanceStarted", 30)) {
    /*                                         ^          */
#ifdef libvlc_VlmMediaInstanceStarted
      *iv_return = libvlc_VlmMediaInstanceStarted;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_31 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_AudioOutputDevice_Stereo libvlc_MediaListViewItemDeleted
     libvlc_MediaListViewWillAddItem libvlc_MediaPlayerLengthChanged
     libvlc_MediaPlayerSnapshotTaken */
  /* Offset 22 gives the best switch position.  */
  switch (name[22]) {
  case 'c':
    if (memEQ(name, "libvlc_AudioOutputDevice_Stereo", 31)) {
    /*                                     ^               */
#ifdef libvlc_AudioOutputDevice_Stereo
      *iv_return = libvlc_AudioOutputDevice_Stereo;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'e':
    if (memEQ(name, "libvlc_MediaListViewItemDeleted", 31)) {
    /*                                     ^               */
#ifdef libvlc_MediaListViewItemDeleted
      *iv_return = libvlc_MediaListViewItemDeleted;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'l':
    if (memEQ(name, "libvlc_MediaListViewWillAddItem", 31)) {
    /*                                     ^               */
#ifdef libvlc_MediaListViewWillAddItem
      *iv_return = libvlc_MediaListViewWillAddItem;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 's':
    if (memEQ(name, "libvlc_MediaPlayerSnapshotTaken", 31)) {
    /*                                     ^               */
#ifdef libvlc_MediaPlayerSnapshotTaken
      *iv_return = libvlc_MediaPlayerSnapshotTaken;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 't':
    if (memEQ(name, "libvlc_MediaPlayerLengthChanged", 31)) {
    /*                                     ^               */
#ifdef libvlc_MediaPlayerLengthChanged
      *iv_return = libvlc_MediaPlayerLengthChanged;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_33 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_MediaListPlayerNextItemSet libvlc_MediaPlayerPausableChanged
     libvlc_MediaPlayerPositionChanged libvlc_MediaPlayerSeekableChanged
     libvlc_VlmMediaInstanceStatusInit */
  /* Offset 21 gives the best switch position.  */
  switch (name[21]) {
  case 'c':
    if (memEQ(name, "libvlc_VlmMediaInstanceStatusInit", 33)) {
    /*                                    ^                  */
#ifdef libvlc_VlmMediaInstanceStatusInit
      *iv_return = libvlc_VlmMediaInstanceStatusInit;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'i':
    if (memEQ(name, "libvlc_MediaPlayerPositionChanged", 33)) {
    /*                                    ^                  */
#ifdef libvlc_MediaPlayerPositionChanged
      *iv_return = libvlc_MediaPlayerPositionChanged;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'k':
    if (memEQ(name, "libvlc_MediaPlayerSeekableChanged", 33)) {
    /*                                    ^                  */
#ifdef libvlc_MediaPlayerSeekableChanged
      *iv_return = libvlc_MediaPlayerSeekableChanged;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'r':
    if (memEQ(name, "libvlc_MediaListPlayerNextItemSet", 33)) {
    /*                                    ^                  */
#ifdef libvlc_MediaListPlayerNextItemSet
      *iv_return = libvlc_MediaListPlayerNextItemSet;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 's':
    if (memEQ(name, "libvlc_MediaPlayerPausableChanged", 33)) {
    /*                                    ^                  */
#ifdef libvlc_MediaPlayerPausableChanged
      *iv_return = libvlc_MediaPlayerPausableChanged;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant_34 (pTHX_ const char *name, IV *iv_return) {
  /* When generated this function returned values for the list of names given
     here.  However, subsequent manual editing may have added or removed some.
     libvlc_MediaListViewWillDeleteItem libvlc_MediaPlayerEncounteredError
     libvlc_MediaPlayerScrambledChanged libvlc_VlmMediaInstanceStatusError
     libvlc_VlmMediaInstanceStatusPause */
  /* Offset 33 gives the best switch position.  */
  switch (name[33]) {
  case 'd':
    if (memEQ(name, "libvlc_MediaPlayerScrambledChange", 33)) {
    /*                                                d      */
#ifdef libvlc_MediaPlayerScrambledChanged
      *iv_return = libvlc_MediaPlayerScrambledChanged;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'e':
    if (memEQ(name, "libvlc_VlmMediaInstanceStatusPaus", 33)) {
    /*                                                e      */
#ifdef libvlc_VlmMediaInstanceStatusPause
      *iv_return = libvlc_VlmMediaInstanceStatusPause;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'm':
    if (memEQ(name, "libvlc_MediaListViewWillDeleteIte", 33)) {
    /*                                                m      */
#ifdef libvlc_MediaListViewWillDeleteItem
      *iv_return = libvlc_MediaListViewWillDeleteItem;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 'r':
    if (memEQ(name, "libvlc_MediaPlayerEncounteredErro", 33)) {
    /*                                                r      */
#ifdef libvlc_MediaPlayerEncounteredError
      *iv_return = libvlc_MediaPlayerEncounteredError;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    if (memEQ(name, "libvlc_VlmMediaInstanceStatusErro", 33)) {
    /*                                                r      */
#ifdef libvlc_VlmMediaInstanceStatusError
      *iv_return = libvlc_VlmMediaInstanceStatusError;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}

static int
constant (pTHX_ const char *name, STRLEN len, IV *iv_return) {
  /* Initially switch on the length of the name.  */
  /* When generated this function returned values for the list of names given
     in this section of perl code.  Rather than manually editing these functions
     to add or remove constants, which would result in this comment and section
     of code becoming inaccurate, we recommend that you edit this section of
     code, and use it to regenerate a new set of constant functions which you
     then use to replace the originals.

     Regenerate these constant functions by feeding this entire source file to
     perl -x

#!/home/silverdirk/perl5/perlbrew/perls/perl-5.20.2/bin/perl -w
use ExtUtils::Constant qw (constant_types C_constant XS_constant);

my $types = {map {($_, 1)} qw(IV)};
my @names = (qw(LIBVLC_DEBUG LIBVLC_ERROR LIBVLC_NOTICE LIBVLC_VERSION_EXTRA
	       LIBVLC_VERSION_H LIBVLC_VERSION_INT LIBVLC_VERSION_MAJOR
	       LIBVLC_VERSION_MINOR LIBVLC_VERSION_REVISION LIBVLC_WARNING
	       libvlc_AudioChannel_Dolbys libvlc_AudioChannel_Error
	       libvlc_AudioChannel_Left libvlc_AudioChannel_RStereo
	       libvlc_AudioChannel_Right libvlc_AudioChannel_Stereo
	       libvlc_AudioOutputDevice_2F2R libvlc_AudioOutputDevice_3F2R
	       libvlc_AudioOutputDevice_5_1 libvlc_AudioOutputDevice_6_1
	       libvlc_AudioOutputDevice_7_1 libvlc_AudioOutputDevice_Error
	       libvlc_AudioOutputDevice_Mono libvlc_AudioOutputDevice_SPDIF
	       libvlc_AudioOutputDevice_Stereo libvlc_Buffering libvlc_Ended
	       libvlc_Error libvlc_MediaDiscovererEnded
	       libvlc_MediaDiscovererStarted libvlc_MediaDurationChanged
	       libvlc_MediaFreed libvlc_MediaListItemAdded
	       libvlc_MediaListItemDeleted libvlc_MediaListPlayerNextItemSet
	       libvlc_MediaListPlayerPlayed libvlc_MediaListPlayerStopped
	       libvlc_MediaListViewItemAdded libvlc_MediaListViewItemDeleted
	       libvlc_MediaListViewWillAddItem
	       libvlc_MediaListViewWillDeleteItem libvlc_MediaListWillAddItem
	       libvlc_MediaListWillDeleteItem libvlc_MediaMetaChanged
	       libvlc_MediaParsedChanged libvlc_MediaPlayerAudioVolume
	       libvlc_MediaPlayerBackward libvlc_MediaPlayerBuffering
	       libvlc_MediaPlayerCorked libvlc_MediaPlayerEncounteredError
	       libvlc_MediaPlayerEndReached libvlc_MediaPlayerForward
	       libvlc_MediaPlayerLengthChanged libvlc_MediaPlayerMediaChanged
	       libvlc_MediaPlayerMuted libvlc_MediaPlayerNothingSpecial
	       libvlc_MediaPlayerOpening libvlc_MediaPlayerPausableChanged
	       libvlc_MediaPlayerPaused libvlc_MediaPlayerPlaying
	       libvlc_MediaPlayerPositionChanged
	       libvlc_MediaPlayerScrambledChanged
	       libvlc_MediaPlayerSeekableChanged
	       libvlc_MediaPlayerSnapshotTaken libvlc_MediaPlayerStopped
	       libvlc_MediaPlayerTimeChanged libvlc_MediaPlayerTitleChanged
	       libvlc_MediaPlayerUncorked libvlc_MediaPlayerUnmuted
	       libvlc_MediaPlayerVout libvlc_MediaStateChanged
	       libvlc_MediaSubItemAdded libvlc_MediaSubItemTreeAdded
	       libvlc_NothingSpecial libvlc_Opening libvlc_Paused
	       libvlc_Playing libvlc_Stopped libvlc_VlmMediaAdded
	       libvlc_VlmMediaChanged libvlc_VlmMediaInstanceStarted
	       libvlc_VlmMediaInstanceStatusEnd
	       libvlc_VlmMediaInstanceStatusError
	       libvlc_VlmMediaInstanceStatusInit
	       libvlc_VlmMediaInstanceStatusOpening
	       libvlc_VlmMediaInstanceStatusPause
	       libvlc_VlmMediaInstanceStatusPlaying
	       libvlc_VlmMediaInstanceStopped libvlc_VlmMediaRemoved
	       libvlc_adjust_Brightness libvlc_adjust_Contrast
	       libvlc_adjust_Enable libvlc_adjust_Gamma libvlc_adjust_Hue
	       libvlc_adjust_Saturation libvlc_logo_delay libvlc_logo_enable
	       libvlc_logo_file libvlc_logo_opacity libvlc_logo_position
	       libvlc_logo_repeat libvlc_logo_x libvlc_logo_y
	       libvlc_marquee_Color libvlc_marquee_Enable
	       libvlc_marquee_Opacity libvlc_marquee_Position
	       libvlc_marquee_Refresh libvlc_marquee_Size libvlc_marquee_Text
	       libvlc_marquee_Timeout libvlc_marquee_X libvlc_marquee_Y
	       libvlc_media_option_trusted libvlc_media_option_unique
	       libvlc_meta_Actors libvlc_meta_Album libvlc_meta_Artist
	       libvlc_meta_ArtworkURL libvlc_meta_Copyright libvlc_meta_Date
	       libvlc_meta_Description libvlc_meta_Director
	       libvlc_meta_EncodedBy libvlc_meta_Episode libvlc_meta_Genre
	       libvlc_meta_Language libvlc_meta_NowPlaying
	       libvlc_meta_Publisher libvlc_meta_Rating libvlc_meta_Season
	       libvlc_meta_Setting libvlc_meta_ShowName libvlc_meta_Title
	       libvlc_meta_TrackID libvlc_meta_TrackNumber
	       libvlc_meta_TrackTotal libvlc_meta_URL libvlc_navigate_activate
	       libvlc_navigate_down libvlc_navigate_left libvlc_navigate_right
	       libvlc_navigate_up libvlc_playback_mode_default
	       libvlc_playback_mode_loop libvlc_playback_mode_repeat
	       libvlc_position_bottom libvlc_position_bottom_left
	       libvlc_position_bottom_right libvlc_position_center
	       libvlc_position_disable libvlc_position_left
	       libvlc_position_right libvlc_position_top
	       libvlc_position_top_left libvlc_position_top_right
	       libvlc_track_audio libvlc_track_text libvlc_track_unknown
	       libvlc_track_video));

print constant_types(), "\n"; # macro defs
foreach (C_constant ("VideoLAN::LibVLC", 'constant', 'IV', $types, undef, 3, @names) ) {
    print $_, "\n"; # C constant subs
}
print "\n#### XS Section:\n";
print XS_constant ("VideoLAN::LibVLC", $types);
__END__
   */

  switch (len) {
  case 12:
    return constant_12 (aTHX_ name, iv_return);
    break;
  case 13:
    return constant_13 (aTHX_ name, iv_return);
    break;
  case 14:
    return constant_14 (aTHX_ name, iv_return);
    break;
  case 15:
    if (memEQ(name, "libvlc_meta_URL", 15)) {
#ifdef libvlc_meta_URL
      *iv_return = libvlc_meta_URL;
      return PERL_constant_ISIV;
#else
      return PERL_constant_NOTDEF;
#endif
    }
    break;
  case 16:
    return constant_16 (aTHX_ name, iv_return);
    break;
  case 17:
    return constant_17 (aTHX_ name, iv_return);
    break;
  case 18:
    return constant_18 (aTHX_ name, iv_return);
    break;
  case 19:
    return constant_19 (aTHX_ name, iv_return);
    break;
  case 20:
    return constant_20 (aTHX_ name, iv_return);
    break;
  case 21:
    return constant_21 (aTHX_ name, iv_return);
    break;
  case 22:
    return constant_22 (aTHX_ name, iv_return);
    break;
  case 23:
    return constant_23 (aTHX_ name, iv_return);
    break;
  case 24:
    return constant_24 (aTHX_ name, iv_return);
    break;
  case 25:
    return constant_25 (aTHX_ name, iv_return);
    break;
  case 26:
    return constant_26 (aTHX_ name, iv_return);
    break;
  case 27:
    return constant_27 (aTHX_ name, iv_return);
    break;
  case 28:
    return constant_28 (aTHX_ name, iv_return);
    break;
  case 29:
    return constant_29 (aTHX_ name, iv_return);
    break;
  case 30:
    return constant_30 (aTHX_ name, iv_return);
    break;
  case 31:
    return constant_31 (aTHX_ name, iv_return);
    break;
  case 32:
    /* Names all of length 32.  */
    /* libvlc_MediaPlayerNothingSpecial libvlc_VlmMediaInstanceStatusEnd */
    /* Offset 17 gives the best switch position.  */
    switch (name[17]) {
    case 'r':
      if (memEQ(name, "libvlc_MediaPlayerNothingSpecial", 32)) {
      /*                                ^                     */
#ifdef libvlc_MediaPlayerNothingSpecial
        *iv_return = libvlc_MediaPlayerNothingSpecial;
        return PERL_constant_ISIV;
#else
        return PERL_constant_NOTDEF;
#endif
      }
      break;
    case 's':
      if (memEQ(name, "libvlc_VlmMediaInstanceStatusEnd", 32)) {
      /*                                ^                     */
#ifdef libvlc_VlmMediaInstanceStatusEnd
        *iv_return = libvlc_VlmMediaInstanceStatusEnd;
        return PERL_constant_ISIV;
#else
        return PERL_constant_NOTDEF;
#endif
      }
      break;
    }
    break;
  case 33:
    return constant_33 (aTHX_ name, iv_return);
    break;
  case 34:
    return constant_34 (aTHX_ name, iv_return);
    break;
  case 36:
    /* Names all of length 36.  */
    /* libvlc_VlmMediaInstanceStatusOpening
       libvlc_VlmMediaInstanceStatusPlaying */
    /* Offset 29 gives the best switch position.  */
    switch (name[29]) {
    case 'O':
      if (memEQ(name, "libvlc_VlmMediaInstanceStatusOpening", 36)) {
      /*                                            ^             */
#ifdef libvlc_VlmMediaInstanceStatusOpening
        *iv_return = libvlc_VlmMediaInstanceStatusOpening;
        return PERL_constant_ISIV;
#else
        return PERL_constant_NOTDEF;
#endif
      }
      break;
    case 'P':
      if (memEQ(name, "libvlc_VlmMediaInstanceStatusPlaying", 36)) {
      /*                                            ^             */
#ifdef libvlc_VlmMediaInstanceStatusPlaying
        *iv_return = libvlc_VlmMediaInstanceStatusPlaying;
        return PERL_constant_ISIV;
#else
        return PERL_constant_NOTDEF;
#endif
      }
      break;
    }
    break;
  }
  return PERL_constant_NOTFOUND;
}


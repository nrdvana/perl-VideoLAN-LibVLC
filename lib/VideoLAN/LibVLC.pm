package VideoLAN::LibVLC;
use 5.008001;
use strict;
use warnings;
use Carp;

our $VERSION = '0.01';

# ABSTRACT - Wrapper for libvlc.so

=head1 SYNOPSIS

  use VideoLAN::LibVLC::MediaPlayer;
  my $player= VideoLAN::LibVLC::MediaPlayer->new("cat_video.mpg");
  $player->video_lock_cb(sub { ... });   # allocate a buffer for video decoding
  $player->video_unlock_cb(sub { ... }); # do something with the decoded video frame
  $player->play; # start VLC thread that decodes and generates callbacks
  my $fh= $player->callback_fh;
  while (1) {
    if (IO::Select->new($fh)->can_read) {  # set up your main loop to watch the $fh
      $player->callback_dispatch # fire perl callbacks
    }
  }

=head1 DESCRIPTION

This module wraps LibVLC.  LibVLC already has a very nice object-ish (but yet
still function-based) API, so this package wraps each group of functions into
a perl object in the logical manner.  One difficulty, however, is that LibVLC
uses its own threads when doing video playback, and most of the "interesting"
possibilities for using LibVLC would require you to write your own callbacks,
which must happen in the main Perl thread.  To work around that (and also not
force you to use this module as your main event loop) this module passes each
VLC callback through a pipe.  This allows you to select() on that file handle
or use an event-driven system like AnyEvent for your program's main loop, and
still gain the parallel processing benefit of VLC running its own thread.

If you're worried about the latency of VLC's playback thread needing a round-
trip to your callbacks, you can create a double-buffering effect by returning
two values from the first callback.  The next VLC callback event will get the
callback result without blocking. (but it still sends another callback event,
so the perl callback still happens and you can stay one step ahead of the VLC
thread)

=cut

require Exporter;
use AutoLoader;

our @ISA = qw(Exporter);

our %EXPORT_TAGS= (
	constants => [qw(
		LIBVLC_DEBUG
		LIBVLC_ERROR
		LIBVLC_NOTICE
		LIBVLC_VERSION_EXTRA
		LIBVLC_VERSION_H
		LIBVLC_VERSION_INT
		LIBVLC_VERSION_MAJOR
		LIBVLC_VERSION_MINOR
		LIBVLC_VERSION_REVISION
		LIBVLC_WARNING
		libvlc_AudioChannel_Dolbys
		libvlc_AudioChannel_Error
		libvlc_AudioChannel_Left
		libvlc_AudioChannel_RStereo
		libvlc_AudioChannel_Right
		libvlc_AudioChannel_Stereo
		libvlc_AudioOutputDevice_2F2R
		libvlc_AudioOutputDevice_3F2R
		libvlc_AudioOutputDevice_5_1
		libvlc_AudioOutputDevice_6_1
		libvlc_AudioOutputDevice_7_1
		libvlc_AudioOutputDevice_Error
		libvlc_AudioOutputDevice_Mono
		libvlc_AudioOutputDevice_SPDIF
		libvlc_AudioOutputDevice_Stereo
		libvlc_Buffering
		libvlc_Ended
		libvlc_Error
		libvlc_MediaDiscovererEnded
		libvlc_MediaDiscovererStarted
		libvlc_MediaDurationChanged
		libvlc_MediaFreed
		libvlc_MediaListItemAdded
		libvlc_MediaListItemDeleted
		libvlc_MediaListPlayerNextItemSet
		libvlc_MediaListPlayerPlayed
		libvlc_MediaListPlayerStopped
		libvlc_MediaListViewItemAdded
		libvlc_MediaListViewItemDeleted
		libvlc_MediaListViewWillAddItem
		libvlc_MediaListViewWillDeleteItem
		libvlc_MediaListWillAddItem
		libvlc_MediaListWillDeleteItem
		libvlc_MediaMetaChanged
		libvlc_MediaParsedChanged
		libvlc_MediaPlayerAudioVolume
		libvlc_MediaPlayerBackward
		libvlc_MediaPlayerBuffering
		libvlc_MediaPlayerCorked
		libvlc_MediaPlayerEncounteredError
		libvlc_MediaPlayerEndReached
		libvlc_MediaPlayerForward
		libvlc_MediaPlayerLengthChanged
		libvlc_MediaPlayerMediaChanged
		libvlc_MediaPlayerMuted
		libvlc_MediaPlayerNothingSpecial
		libvlc_MediaPlayerOpening
		libvlc_MediaPlayerPausableChanged
		libvlc_MediaPlayerPaused
		libvlc_MediaPlayerPlaying
		libvlc_MediaPlayerPositionChanged
		libvlc_MediaPlayerScrambledChanged
		libvlc_MediaPlayerSeekableChanged
		libvlc_MediaPlayerSnapshotTaken
		libvlc_MediaPlayerStopped
		libvlc_MediaPlayerTimeChanged
		libvlc_MediaPlayerTitleChanged
		libvlc_MediaPlayerUncorked
		libvlc_MediaPlayerUnmuted
		libvlc_MediaPlayerVout
		libvlc_MediaStateChanged
		libvlc_MediaSubItemAdded
		libvlc_MediaSubItemTreeAdded
		libvlc_NothingSpecial
		libvlc_Opening
		libvlc_Paused
		libvlc_Playing
		libvlc_Stopped
		libvlc_VlmMediaAdded
		libvlc_VlmMediaChanged
		libvlc_VlmMediaInstanceStarted
		libvlc_VlmMediaInstanceStatusEnd
		libvlc_VlmMediaInstanceStatusError
		libvlc_VlmMediaInstanceStatusInit
		libvlc_VlmMediaInstanceStatusOpening
		libvlc_VlmMediaInstanceStatusPause
		libvlc_VlmMediaInstanceStatusPlaying
		libvlc_VlmMediaInstanceStopped
		libvlc_VlmMediaRemoved
		libvlc_adjust_Brightness
		libvlc_adjust_Contrast
		libvlc_adjust_Enable
		libvlc_adjust_Gamma
		libvlc_adjust_Hue
		libvlc_adjust_Saturation
		libvlc_logo_delay
		libvlc_logo_enable
		libvlc_logo_file
		libvlc_logo_opacity
		libvlc_logo_position
		libvlc_logo_repeat
		libvlc_logo_x
		libvlc_logo_y
		libvlc_marquee_Color
		libvlc_marquee_Enable
		libvlc_marquee_Opacity
		libvlc_marquee_Position
		libvlc_marquee_Refresh
		libvlc_marquee_Size
		libvlc_marquee_Text
		libvlc_marquee_Timeout
		libvlc_marquee_X
		libvlc_marquee_Y
		libvlc_media_option_trusted
		libvlc_media_option_unique
		libvlc_meta_Actors
		libvlc_meta_Album
		libvlc_meta_Artist
		libvlc_meta_ArtworkURL
		libvlc_meta_Copyright
		libvlc_meta_Date
		libvlc_meta_Description
		libvlc_meta_Director
		libvlc_meta_EncodedBy
		libvlc_meta_Episode
		libvlc_meta_Genre
		libvlc_meta_Language
		libvlc_meta_NowPlaying
		libvlc_meta_Publisher
		libvlc_meta_Rating
		libvlc_meta_Season
		libvlc_meta_Setting
		libvlc_meta_ShowName
		libvlc_meta_Title
		libvlc_meta_TrackID
		libvlc_meta_TrackNumber
		libvlc_meta_TrackTotal
		libvlc_meta_URL
		libvlc_navigate_activate
		libvlc_navigate_down
		libvlc_navigate_left
		libvlc_navigate_right
		libvlc_navigate_up
		libvlc_playback_mode_default
		libvlc_playback_mode_loop
		libvlc_playback_mode_repeat
		libvlc_position_bottom
		libvlc_position_bottom_left
		libvlc_position_bottom_right
		libvlc_position_center
		libvlc_position_disable
		libvlc_position_left
		libvlc_position_right
		libvlc_position_top
		libvlc_position_top_left
		libvlc_position_top_right
		libvlc_track_audio
		libvlc_track_text
		libvlc_track_unknown
		libvlc_track_video
	)],
	functions => [qw(
		
	)],
);
our @EXPORT_OK= map { @$_ } values %EXPORT_TAGS;
$EXPORT_TAGS{all}= \@EXPORT_OK;

our $AUTOLOAD;
sub AUTOLOAD {
	# This AUTOLOAD is used to build constants from the constant() XS function
	my $constname;
	($constname = $AUTOLOAD) =~ s/.*:://;
	croak "&VideoLAN::LibVLC::constant not defined" if $constname eq 'constant';
	my ($error, $val) = constant($constname);
	if ($error) { croak $error; }
	{
		no strict 'refs';
		*$AUTOLOAD = sub { $val };
	}
	goto &$AUTOLOAD;
}

require XSLoader;
XSLoader::load('VideoLAN::LibVLC', $VERSION);

sub new {
	my $class= shift;
	my %args= (@_ == 1 && ref($_[0]) eq 'HASH')? %{ $_[0] }
		: (@_ == 1 && ref($_[0]) eq 'ARRAY')? ( argv => $_[0] )
		: ((@_&1) == 0)? @_
		: croak "Expected hashref, even-length list, or arrayref";
	$args{argv} ||= [];
	my $self= VideoLAN::LibVLC::libvlc_new($args{argv});
	use DDP; p $self;
	%$self= %args;
	return $self;
}

1;
__END__

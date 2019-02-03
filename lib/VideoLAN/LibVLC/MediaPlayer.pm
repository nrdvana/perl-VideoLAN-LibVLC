package VideoLAN::LibVLC::MediaPlayer;

use strict;
use warnings;
use VideoLAN::LibVLC qw(
 PERLVLC_MSG_VIDEO_LOCK_EVENT
 PERLVLC_MSG_VIDEO_UNLOCK_EVENT
 PERLVLC_MSG_VIDEO_DISPLAY_EVENT
 PERLVLC_MSG_VIDEO_FORMAT_EVENT
 PERLVLC_MSG_VIDEO_CLEANUP_EVENT );
use Socket qw( AF_UNIX SOCK_STREAM );
use Scalar::Util 'weaken';
use IO::Handle;
use Carp;

# ABSTRACT: Media Player

=head1 DESCRIPTION

This object wraps C<libvlc_media_player_t>

=head1 ATTRIBUTES

=head2 libvlc

Read-only reference to the library instance that created this player.

=head2 media

The L<VideoLAN::LibVLC::Media> object being played

=head2 is_playing

Boolean, whether playback is active

=head2 will_play

Boolean, whether the media player is able to play

=head2 is_seekable

Boolean, whether media is seekable

=head2 can_pause

Boolean, whether playback can be paused

=cut

sub libvlc { shift->{libvlc} }

sub media { my $self= shift; $self->set_media(@_) if @_; $self->{media} }

*is_playing=  *VideoLAN::LibVLC::libvlc_media_player_is_playing;
*will_play=   *VideoLAN::LibVLC::libvlc_media_player_will_play;
*is_seekable= *VideoLAN::LibVLC::libvlc_media_player_is_seekable;
*can_pause=   *VideoLAN::LibVLC::libvlc_media_player_can_pause;

=head2 rate

The requested playback rate (multiple or fraction of real-time)
Writing this attribute calls L</set_rate>.

=head2 length

The length in seconds of the media

=head2 title_count

Number if subtitle tracks in the media, or undef.

=head2 title

Number of the subtitle track currently playing, or undef

"title" is the official language of libvlc, but "subtitle" is what English
speakers probably expect to find.  I decided to stick with the API's
terminology.

=head2 chapter_count

Number of chapters in media, or undef

=head2 chapter

Chapter number currently playing, or undef if no media is playing.
Setting this attribute changes the chapter.

=cut

sub length { &VideoLAN::LibVLC::libvlc_media_player_get_length * .001; }

sub title_count {
	my $n= &VideoLAN::LibVLC::libvlc_media_player_title_count;
	$n >= 0? $n : undef;
}

sub title {
	@_ > 1? &VideoLAN::LibVLC::libvlc_media_player_set_title
	: do { my $n= &VideoLAN::LibVLC::libvlc_media_player_get_title; $n >= 0? $n : undef; }
}

sub chapter_count {
	my $n= &VideoLAN::LibVLC::libvlc_media_player_chapter_count;
	$n >= 0? $n : undef
}

sub chapter {
	@_ > 1? &VideoLAN::LibVLC::libvlc_media_player_set_chapter
	: do { my $n= &VideoLAN::LibVLC::libvlc_media_player_get_chapter; $n >= 0? $n : undef; }
}

=head2 time

The position within the media file, measured in seconds.  Undef until playback begins.
Setting this attribute performs a seek.

=head2 position

The position within the media file, measured as a fraction of media length. (0..1).
Undef until playback begins.
Setting this attribute performs a seek.

=cut

sub time {
	@_ > 1? VideoLAN::LibVLC::libvlc_media_player_set_time($_[0], int($_[1] * 1000))
	: do { my $x= &VideoLAN::LibVLC::libvlc_media_player_get_time; $x >= 0? $x * .001 : undef; };
}

sub position {
	@_ > 1? &VideoLAN::LibVLC::libvlc_media_player_set_position
	: do { my $x= &VideoLAN::LibVLC::libvlc_media_player_get_position; $x >= 0? $x : undef; };
}

=head1 METHODS

=head2 new

  my $player= VideoLAN::LibVLC::MediaPlayer->new(
    libvlc => $vlc,
  );

=cut

sub new {
	my $class= shift;
	my %args= (@_ == 1 && ref($_[0]) eq 'HASH')? { $_[0] }
		: (@_ & 1) == 0? @_
		: croak "Expected hashref or even length list";
	defined $args{libvlc} or croak "Missing required attribute 'libvlc'";
	my $self= !defined $args{media}? VideoLAN::LibVLC::libvlc_media_player_new($args{libvlc})
		: VideoLAN::LibVLC::libvlc_media_player_new_from_media($args{libvlc}->new_media($args{media}));
	%$self= %args;
	return $self;
}

=head2 set_media

Set the player's active media soruce.  May be an instance of
L<VideoLAN::LibVLC::Media> or any valid argument for
L<VideoLAN::LibVLC/new_media>.

This can also be called by setting the L</media> attribute.

=cut

sub set_media {
	my ($self, $media)= @_;
	$media= $self->libvlc->new_media($media)
		unless ref($media) && ref($media)->isa('VideoLAN::LibVLC::Media');
	VideoLAN::LibVLC::libvlc_media_player_set_media($self, $media);
}

=head2 play

=head2 pause

=head2 set_pause

Requires libvlc 1.1.1

=head2 stop

=head2 set_rate

=cut

sub play { VideoLAN::LibVLC::libvlc_media_player_play(shift) == 0 }
*pause = *VideoLAN::LibVLC::libvlc_media_player_pause;
*stop  = *VideoLAN::LibVLC::libvlc_media_player_stop;
*set_pause = *VideoLAN::LibVLC::libvlc_media_player_set_pause
	if defined *VideoLAN::LibVLC::libvlc_media_player_set_pause;
*set_rate  = *VideoLAN::LibVLC::libvlc_media_player_set_rate;

=head2 set_video_title_display

  $player->set_video_title_display( $position, $timeout );

Specify where and how long subtitles should be displayed.
Position is one of:

  use VideoLAN::LibVLC ':position_t';
  # POSITION_DISABLE
  # POSITION_CENTER
  # POSITION_LEFT
  # POSITION_RIGHT
  # POSITION_TOP
  # POSITION_TOP_LEFT
  # POSITION_TOP_RIGHT
  # POSITION_BOTTOM
  # POSITION_BOTTOM_LEFT
  # POSITION_BOTTOM_RIGHT

but may also be an uppercase or lowercase string of one of those minus the
leading "POSITION_".

Timeout is in seconds.

Requires host libvlc 2.1, but only warns if this method is not supported.

=cut

*_set_video_title_display= VideoLAN::LibVLC->can('libvlc_media_player_set_video_title_display')
	|| sub { carp "set_video_title_display not supported by host libvlc" };

sub set_video_title_display {
	my ($self, $pos, $timeout)= @_;
	$timeout= ($timeout || 0) * 1000;
	$pos = VideoLAN::LibVLC::POSITION_DISABLE unless defined $pos;
	if ($pos =~ /[a-z]/i) {
		my $const= VideoLAN::LibVLC->can("POSITION_".uc($pos));
		$pos= defined $const? $const->() : die "No such subtitle position $pos";
	}
	$self->_set_video_title_display($pos, $timeout);
}

sub _vbuf_pipe {
	$_[0]{_vbuf_pipe} //= do {
		socketpair(my $r, my $w, AF_UNIX, SOCK_STREAM, 0)
			or die "socketpair: $!";
		$w->blocking(0);
		# pass file handles to XS
		$_[0]->_set_vbuf_pipe(fileno($r), fileno($w));
		[$r, $w];
	}
}

sub _video_callbacks { $_[0]{_video_callbacks} //= {} }
sub set_video_callbacks {
	my $self= shift;
	my %opts= @_ == 1? %{ $_[0] } : @_;
	$self->{libvlc} or croak "Can't set up callbacks without reference to VLC instance";
	!$self->is_playing or croak "Can't change callbacks during playback";
	my $cur= $self->_video_callbacks;
	# Can't specify 'cleanup' without 'format'
	!$opts{cleanup} || ($opts{format} || $cur->{format})
		or croak "Can't specify 'cleanup' without 'format'";
	for (qw( lock unlock display cleanup format )) {
		my $name= $_;
		if (exists $opts{$_}) {
			$cur->{$_}= $opts{$_};
		} else {
			delete $cur->{$_};
		}
	}
	
	# Make sure we've registered with libvlc's event pipe
	$self->_vbuf_pipe;
	my $event_wr= $self->{libvlc}->_event_pipe->[1];
	weaken($self);
	my $cb_id= $self->{_callback_id} //= $self->{libvlc}->_register_callback(sub {
		$self && $self->_dispatch_callback(@_);
	});
	
	# Now register the callbacks in the XS code
	$self->_enable_video_callbacks(fileno($event_wr), $cb_id, ['lock', keys %$cur]);
	1;
}

sub set_video_format {
	my ($self, $chroma, $width, $height, $pitch, $lines, $alloc_count)= @_;
	!$self->{video_format}
		or croak "Video format already set";
	if ($self->_video_callbacks->{format}) {
		$self->_reply_video_format($chroma, $width, $height, $pitch, $lines, $alloc_count);
	}
	else {
		$self->VideoLAN::LibVLC::libvlc_video_set_format($chroma, $width, $height, $pitch);
	}
	$self->{video_format}{chroma}= $chroma;
	$self->{video_format}{width}=  $width;
	$self->{video_format}{height}= $height;
	$self->{video_format}{plane_pitch}= $pitch;
	$self->{video_format}{plane_lines}= $lines;
	1;
}

my %event_id_to_name= (
	PERLVLC_MSG_VIDEO_LOCK_EVENT()    => 'lock',
	PERLVLC_MSG_VIDEO_UNLOCK_EVENT()  => 'unlock',
	PERLVLC_MSG_VIDEO_DISPLAY_EVENT() => 'display',
	PERLVLC_MSG_VIDEO_FORMAT_EVENT()  => 'format',
	PERLVLC_MSG_VIDEO_CLEANUP_EVENT() => 'cleanup',
);

sub _dispatch_callback {
	my ($self, $event)= @_;
	my $opaque= $self->_video_callbacks->{opaque} // $self;
	if (my $cbname= $event_id_to_name{$event->{event_id}}) {
		my $cb= $self->_video_callbacks->{$cbname};
		$cb && $cb->($opaque, $event);
		# Lock callback should always call fill_queue
		if ($cbname eq 'lock') {
			my $queued= $self->fill_queue;
			carp "Only $queued pictures available to VLC" if $queued < 3;
		}
		# format callback should always record the picture settings
		elsif ($cbname eq 'format') {
			$self->{video_format}{$_}= $event->{$_} for qw( chroma width height plane_pitch plane_lines );
			# and if there wasn't a callback, it should reply to the message
			if (!$cb) {
				$cb->_reply_video_format(@{$event}{qw( chroma width height plane_pitch plane_lines )});
			}
		}
	}
	else {
		warn "Unknown event ".$event->{event_id};
	}
}

sub new_picture {
	my $self= shift;
	my $fmt= $self->{video_format}
		or croak "Video format is not yet known/set";
	$fmt= { %$fmt, @_ == 1? %{$_[0]} : @_ }
		if @_;
	VideoLAN::LibVLC::Picture->new($fmt);
}

sub push_new_picture {
	my $self= shift;
	$self->push_picture($self->new_picture(@_));
}

1;

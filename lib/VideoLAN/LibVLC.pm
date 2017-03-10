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
our @ISA = qw(Exporter);

our %EXPORT_TAGS= (
# BEGIN GENERATED XS CONSTANT LIST
  media_parse_flag_t => [qw( media_do_interact media_fetch_local
    media_fetch_network media_parse_local media_parse_network )],
  media_parsed_status_t => [qw( media_parsed_status_done
    media_parsed_status_failed media_parsed_status_skipped
    media_parsed_status_timeout )],
  media_slave_type_t => [qw( media_slave_type_audio media_slave_type_subtitle
    )],
  media_type_t => [qw( media_type_directory media_type_disc media_type_file
    media_type_playlist media_type_stream media_type_unknown )],
  meta_t => [qw( meta_Actors meta_Album meta_Artist meta_ArtworkURL
    meta_Copyright meta_Date meta_Description meta_Director meta_EncodedBy
    meta_Episode meta_Genre meta_Language meta_NowPlaying meta_Publisher
    meta_Rating meta_Season meta_Setting meta_ShowName meta_Title meta_TrackID
    meta_TrackNumber meta_TrackTotal meta_URL )],
  state_t => [qw( Buffering Ended Error NothingSpecial Opening Paused Playing
    Stopped )],
  track_type_t => [qw( track_audio track_text track_unknown track_video )],
  video_orient_t => [qw( video_orient_bottom_left video_orient_bottom_right
    video_orient_left_bottom video_orient_left_top video_orient_right_bottom
    video_orient_right_top video_orient_top_left video_orient_top_right )],
  video_projection_t => [qw( video_projection_cubemap_layout_standard
    video_projection_equirectangular video_projection_rectangular )],
# END GENERATED XS CONSTANT LIST
	functions => [qw(
		
	)],
);
push @{ $EXPORT_TAGS{constants} }, @{ $EXPORT_TAGS{$_} }
	for grep /_t$/, keys %EXPORT_TAGS;
our @EXPORT_OK= @{ $EXPORT_TAGS{constants} }, @{ $EXPORT_TAGS{functions} };
$EXPORT_TAGS{all}= \@EXPORT_OK;

require XSLoader;
XSLoader::load('VideoLAN::LibVLC', $VERSION);

=head1 ATTRIBUTES

=head2 libvlc_version

Version of LibVLC.  This is a package attribute.

=head2 libvlc_changeset

Precise revision-control version of LibVLC.  This is a package attribute.

=head2 libvlc_compiler

Compiler used to create LibVLC.  This is a package attribute.

=cut

sub libvlc_version   { libvlc_get_version() }
sub libvlc_changeset { libvlc_get_changeset() }
sub libvlc_compiler  { libvlc_get_compiler() }

=head2 argv

A copy of the argv that you passed to the constructor.  Read-only.

=head2 app_id

A java-style name identifying the application.  Defaults to an empty string if you set app_version
or app_icon.

=head2 app_version

The version of your application.  Defaults to empty string if you assign an app_id or app_icon.

=head2 app_icon

The name of the icon for your application.  Defaults to empty string if you assign an app_id or app_version.

=cut

sub argv { croak("read-only attribute") if @_ > 1; $_[0]{argv} }

sub _update_app_id {
	my $self= shift;
	$self->{app_id}= ''      unless defined $self->{app_id};
	$self->{app_version}= '' unless defined $self->{app_version};
	$self->{app_icon}= ''    unless defined $self->{app_icon};
	$self->libvlc_set_app_id($self->{app_id}, $self->{app_version}, $self->{app_icon});
}
sub app_id      { my $self= shift; if (@_) { $self->{app_id}= shift; $self->_update_app_id; } $self->{app_id} }
sub app_version { my $self= shift; if (@_) { $self->{app_version}= shift; $self->_update_app_id; } $self->{app_version} }
sub app_icon    { my $self= shift; if (@_) { $self->{app_icon}= shift; $self->_update_app_id; } $self->{app_icon} }

=head2 user_agent_name

A human-facing description of your application as a user agent for web requests.

=head2 user_agent_http

A HTTP UserAgent string for your application.

=cut

sub _update_user_agent {
	my $self= shift;
	$self->{user_agent_name}= '' unless defined $self->{user_agent_name};
	$self->{user_agent_http}= '' unless defined $self->{user_agent_http};
	$self->libvlc_set_user_agent($self->{user_agent_name}, $self->{user_agent_http});
}
sub user_agent_name { my $self= shift; if (@_) { $self->{user_agent_name}= shift; $self->_update_user_agent; } $self->{user_agent_name} }
sub user_agent_http { my $self= shift; if (@_) { $self->{user_agent_http}= shift; $self->_update_user_agent; } $self->{user_agent_http} }

=head2 audio_filters

An arrayref of all audio filter modules built into LibVLC.

=head2 audio_filter_list

List accessor for audio_filters.

=head2 video_filters

An arrayref of all video filter modules built into LibVLC.

=head2 video_filter_list

List accessor for video_filters.

=cut

sub audio_filters { my $self= shift; $self->{audio_filters} ||= [ $self->libvlc_audio_filter_list_get ] }
sub video_filters { my $self= shift; $self->{video_filters} ||= [ $self->libvlc_video_filter_list_get ] }

sub audio_filter_list { @{ shift->audio_filters } }
sub video_filter_list { @{ shift->video_filters } }

=head1 METHODS

=head2 new

  my $vlc= VideoLAN::VLC->new( \@ARGV );
  my $vlc= VideoLAN::VLC->new( %attributes );
  my $vlc= VideoLAN::VLC->new( \%attributes );

Create a new instance of LibVLC (which directly corresponds to an initialization
of libvlc via C<libvlc_new>)

Note that libvlc suggests against passing command line arguments except for
debugging, since they can differ by version and by platform.

The returned object is based on a hashref, and the libvlc pointer is magically
attached.

=cut

sub new {
	my $class= shift;
	my %args= (@_ == 1 && ref($_[0]) eq 'HASH')? %{ $_[0] }
		: (@_ == 1 && ref($_[0]) eq 'ARRAY')? ( argv => $_[0] )
		: ((@_&1) == 0)? @_
		: croak "Expected hashref, even-length list, or arrayref";
	$args{argv} ||= [];
	my $self= VideoLAN::LibVLC::libvlc_new($args{argv});
	%$self= %args;
	$self->_update_app_id
		if defined $self->{app_id} or defined $self->{app_version} or defined $self->{app_icon};
	$self->_update_user_agent
		if defined $self->{user_agent_name} or defined $self->{user_agent_http};
	return $self;
}

1;
__END__

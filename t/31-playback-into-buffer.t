use strict;
use warnings;
use Test::More;
use FindBin;
use Time::HiRes 'sleep';
use File::Spec::Functions 'catdir';
my $datadir= catdir($FindBin::Bin, 'data');

use_ok('VideoLAN::LibVLC::MediaPlayer') || BAIL_OUT;

my $vlc= new_ok( 'VideoLAN::LibVLC', [], 'init libvlc' );
$vlc->log(sub { note $_[0]->{message}; }, { level => 1 });

subtest custom_framesize => \&test_custom_framesize;
sub test_custom_framesize {
	my $player= new_ok( 'VideoLAN::LibVLC::MediaPlayer', [ libvlc => $vlc ], 'player instance' );
	1 while $vlc->callback_dispatch;

	$player->media(catdir($datadir, 'NASA-solar-flares-2017-04-02.mp4'));
	1 while $vlc->callback_dispatch;

	my $pic;
	$player->trace_pictures(1);
	$player->set_video_callbacks(display => sub { $pic= $_[1]{picture}; $_[0]->push_picture($pic); });
	$player->set_video_format(chroma => 'RGBA', width => 64, height => 64, plane_pitch => 64*4);
	$player->push_new_picture(id => $_) for 0..7;
	ok( $player->play, 'play' );
	for (my $i= 0; !$pic && $i < 100; $i++) {
		sleep .05;
		1 while $vlc->callback_dispatch;
	}
	ok( $pic, 'received picture from display callback' );
	$player->stop;
	for (my $i= 0; $player->is_playing && $i < 100; $i++) {
		sleep .05;
		1 while $vlc->callback_dispatch;
	}
	sleep .05;
}

subtest native_framesize => \&test_native_framesize;
sub test_native_framesize {
	my $player= new_ok( 'VideoLAN::LibVLC::MediaPlayer', [ libvlc => $vlc ], 'player instance' );
	1 while $vlc->callback_dispatch;

	$player->media(catdir($datadir, 'NASA-solar-flares-2017-04-02.mp4'));
	1 while $vlc->callback_dispatch;

	my ($pic, $ready, $done);
	$player->trace_pictures(1);
	$player->set_video_callbacks(
		display => sub { $pic= $_[1]{picture}; $_[0]->push_picture($pic); },
		'format'=> sub {
			my ($p, $event)= @_;
			diag explain $event;
			$p->set_video_format(%$event, alloc_count => 8);
			$p->push_new_picture(id => $_) for 0..7;
		},
		cleanup => sub { $done++ },
	);
	ok( $player->play, 'play' );
	for (my $i= 0; !$pic && $i < 100; $i++) {
		sleep .01;
		1 while $vlc->callback_dispatch;
	}
	ok( $pic, 'received picture from display callback' );
	$player->stop;
	for (my $i= 0; !$done && $i < 100; $i++) {
		sleep .05;
		1 while $vlc->callback_dispatch;
	}
	ok( $done, 'got cleanup event' );
}

done_testing;

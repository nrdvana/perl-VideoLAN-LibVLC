use strict;
use warnings;
use Test::More;
use FindBin;
my $datadir= "$FindBin::Bin/data";

use_ok('VideoLAN::LibVLC::MediaPlayer') || BAIL_OUT;

my $vlc= new_ok( 'VideoLAN::LibVLC', [], 'init libvlc' );
my $player= new_ok( 'VideoLAN::LibVLC::MediaPlayer', [ libvlc => $vlc ], 'player instance' );

$player->media("$datadir/NASA-solar-flares-2017-04-02.mp4");

is( $player->time, undef, 'time = undef' );
is( $player->position, undef, 'position = undef' );
is( $player->will_play, 1, 'will_play = 1' );
ok( $player->play, 'play' );
sleep .5;
$player->pause;
ok( $player->time > 0, 'time > 0' );
ok( $player->position > 0, 'position > 0' );

$player->time(0.5);
is( $player->time, 0.5, 'time = 0.5' );
ok( $player->position < 0.5, 'position < 0.5' );

$player->position(0);
is( $player->time, 0, 'time = 0' );
is( $player->position, 0, 'position = 0' );

$player->stop;

done_testing;

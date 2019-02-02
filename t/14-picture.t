use strict;
use warnings;
use Test::More;
use FindBin;
my $datadir= "$FindBin::Bin/data";

use_ok('VideoLAN::LibVLC') || BAIL_OUT;

my %info= (
	chroma => 'RGBA',
	width => 16,
	height => 10,
	plane_pitch => 64,
	plane_lines => 10,
);
my $picture= new_ok( 'VideoLAN::LibVLC::Picture', [\%info], 'new instance' );
is( $picture->width, 16, 'width' );
is( $picture->height, 10, 'height' );
is( $picture->chroma, 'RGBA', 'chroma' );
is( $picture->plane_pitch(0), 64, 'plane[0]{pitch}' );
is( $picture->plane_lines(0), 10, 'plane[0]{lines}' );
is( $picture->plane(1), undef, 'no plane 1' );

done_testing;

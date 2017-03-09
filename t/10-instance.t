use strict;
use warnings;
use Test::More;

use_ok('VideoLAN::LibVLC') || BAIL_OUT;

new_ok( 'VideoLAN::LibVLC', [], 'new instance, no args' );

done_testing;

use strict;
use warnings;
use Test::More;
use Try::Tiny;
use Log::Any '$log';
use VideoLAN::LibVLC;
sub err(&) { my $code= shift; try { $code->(); '' } catch { "$_" }; }

plan skip_all => 'Log redirection not supported on this version of LibVLC'
	unless VideoLAN::LibVLC->can('libvlc_log_unset');

my $buffer= pack('SC', 1, 2) . join("\0", 'file=', 'foo=bar', 'baz=0', '=blah', 'blah1234567');
my $attrs= VideoLAN::LibVLC::_log_extract_attrs($buffer);
is_deeply( $attrs, { level => 2, file => '', foo => 'bar', baz => 0, '' => 'blah' }, 'extract log attrs' );
is( $buffer, 'blah1234567', 'buffer is trimmed' );


my $vlc= new_ok( 'VideoLAN::LibVLC', [], 'new instance, no args' );

is( err{ $vlc->log(undef) }, '', 'set log to undef when already undef' );

# TODO: find a way to reliably generate log output

my @cb_args;
is( err{ $vlc->log(sub { @cb_args= @_; }); }, '', 'set to callback' );

# TODO: pump the dispatcher to dispatch the logging events

is( err{ $vlc->log($log) }, '', 'set to Log::Any' );

# TODO: pump the dispatcher to dispatch the logging events

is( err{ $vlc->log(undef) }, '', 'unset logger' );

done_testing;

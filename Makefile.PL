use 5.008001;
use ExtUtils::MakeMaker;

our %PREREQ_PM;
BEGIN {
	%PREREQ_PM= (
		'Alien::VideoLAN::LibVLC' => '0.04',
		'ExtUtils::Depends'       => '0.405',
	);
	
	# If the prereqs for this script are missing,
	# write out a Makefile that tells CPAN to install them
	my $use_prereqs_code= join('', map { "use $_ '$PREREQ_PM{$_}'; " } keys %PREREQ_PM).'1;';
	print $use_prereqs_code."\n";
	unless (eval $use_prereqs_code) {
		warn "$@\n";
		WriteMakefile(
				NAME         => 'X11::Xlib',
				PREREQ_FATAL => 1,
				PREREQ_PM    => \%PREREQ_PM,
		);
		exit 1; # not reached
	}
}

my $dep= ExtUtils::Depends->new('VideoLAN::LibVLC');

my %libvlc_info= Alien::VideoLAN::LibVLC->find_libvlc();

$dep->set_libs(join ' ', @{ $libvlc_info{ldflags} });
$dep->set_inc(join ' ', @{ $libvlc_info{cflags} });
$dep->add_c('PerlVLC.c');
$dep->add_xs('LibVLC.xs');
$dep->add_pm(map { my $n= $_; $n =~ s/^lib/\$(INST_LIB)/; $_ => $n } <lib/*/*.pm>, <lib/*/*/*.pm>);
$dep->add_typemaps('typemap');
$dep->install('PerlVLC.h');

mkdir 'build', 0777;
$dep->save_config('build/IFiles.pm');

# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
	NAME              => 'VideoLAN::LibVLC',
	VERSION_FROM      => 'lib/VideoLAN/LibVLC.pm', # finds $VERSION, requires EU::MM from perl >= 5.5
	PREREQ_PM         => \%PREREQ_PM,
	ABSTRACT_FROM     => 'lib/VideoLAN/LibVLC.pm', # retrieve abstract from module
	AUTHOR            => 'Michael Conrad <mike@nrdvana.net>',
	LICENSE           => 'perl',
	CCFLAGS           => '-Wall',
	$dep->get_makefile_vars(),
);
if (eval {require ExtUtils::Constant; 1} && (open my $fh, "<", "lib/VideoLAN/LibVLC.pm")) {
	# Copy the generated constants.inc to the fallback/ directory any time the
	# list of constants changes in LibVLC.pm
	my @names= map { (my $x= $_) =~ /(libvlc_\w+)/i } <$fh>;
	ExtUtils::Constant::WriteConstants(
		NAME         => 'VideoLAN::LibVLC',
		NAMES        => \@names,
		DEFAULT_TYPE => 'IV',
		C_FILE       => 'const-c.inc',
		XS_FILE      => 'const-xs.inc',
	);
}
else {
	use File::Copy;
	use File::Spec;
	foreach my $file ('const-c.inc', 'const-xs.inc') {
		my $fallback = File::Spec->catfile('fallback', $file);
		copy ($fallback, $file) or die "Can't copy $fallback to $file: $!";
	}
}

#	prebuild.pl
#	perl script to set a version number and insert into source and documentation
#
#	use strict;
#	use warnings;
#	
# 	usage:
#
#       >perl prebuild.pl
#

print "\nStart prebuild.pl\n";

$file = "Source\\Common\\Rev_all.h";
$Build = $ENV{'BNumber'};
my ($sec, $min, $hour, $day, $mon, $year, $wday, $yday, $isdst) = localtime(time);
$year += 1900;	# Year is returned as the number of years since 1900
@month = qw( January February March April May June Julu August September October November December );
my $date = sprintf "$month[$mon] %02d, %4d", $day, $year;

GetFileInfo();

ParseFileVersion(); 

$CompleteVersion = "$Major\.$Minor\.$Incr\.$Build";

UpdateSource($file);

UpdateRTF("Documentation\\ReadMe\\ReadMe_Internal.rtf");
UpdateRTF("Documentation\\ReadMe\\ReadMe_NDA.rtf");
UpdateRTF("Documentation\\ReadMe\\ReleaseNotes_Internal.rtf");
UpdateRTF("Documentation\\ReadMe\\ReleaseNotes_NDA.rtf");
UpdateRTF("Documentation\\ReadMe\\ReleaseNotes_NDA_ATI.rtf");

CreateCmd(); 

print "End prebuild.pl\n";
exit;

#####################################
# subs follow......
#####################################

sub GetFileInfo
{
	print "GetFileInfo: Started\n";
	print "GetFileInfo: Resource file is $file\n";
	open(OLD, "< $file")		or die "can't open $file: $!";

	while(<OLD>)
	{
		if( $_ =~ /VERSION_NUMBER/)
		{
			$FileVersion=$_;
		}
		elsif( $_ =~ /VERSION_DATE/)
		{
			$FileDate=$_;
		}else
		{
		#nothing!
		}
	}
	print "GetFileInfo: FileVersion is $FileVersion\n";
	print "GetFileInfo: FileDate is $FileDate\n";
	print "GetFileInfo: Completed\n";
}

sub ParseFileVersion
{
	print "ParseFileVersion: Started\n";
	#parse out the version data from the RC file
	($Toss, $TVersion) = split('\"', $FileVersion, 2);
	($Version, $Toss) = split('\"', $TVersion, 2);
	($Major, $Minor, $Incr)= split('\.',$Version, 3);
	print "ParseFileVersion: Completed\n";
}

sub UpdateSource
{
	print "UpdateSource: Started\n";
	my $source=shift;
	open SOURCE, "<$source"
	or die "Can't open $source for reading: $!";

	my $linecount = 0;
	while (<SOURCE>) {
				if( $_ =~ /VERSION_NUMBER/)
			{
				$new_source[$linecount] = "#define VERSION_NUMBER    \"$CompleteVersion\"\n";
			}
			elsif( $_ =~ /VERSION_DATE/)
			{
				$new_source[$linecount] = "#define VERSION_DATE      \"$date\"\n"; 
			}
			else
			{
				$new_source[$linecount] = $_;
			}
		$linecount++;
	}
	close SOURCE;

	open SOURCE, ">$source"
		or die "Can't open $source for writing: $!";

	foreach my $line (@new_source) {
		print SOURCE $line;
	}

	close SOURCE;
	print "UpdateSource: Completed\n";
}

sub UpdateRTF
{
	print "UpdateRTF: Started\n";
	my $rtf=shift;
	open RTF, "<$rtf"
	or die "Can't open $rtf for reading: $!";

	my $linecount = 0;
	while (<RTF>) {
		s/%VERSION%/$CompleteVersion/g;
		s/%RELEASEDATE%/$date/g;
		$new_rtf[$linecount] = $_;
		$linecount++;
	}
	close RTF;

	open RTF, ">$rtf"
		or die "Can't open $rtf for writing: $!";

	foreach my $line (@new_rtf) {
		print RTF $line;
	}

	close RTF;
	print "UpdateRTF: Completed\n";
}

sub CreateCmd
{
	print "CreateCmd: Started\n";
	
	$CPYRTSTR = "copyright " + (localtime)[5] + 1900;
	#Make a environment batch file
	open(CMD, "> AMDVersion.cmd")	or die "can't open $newfile: $!";
	print(CMD "SET PKG_VERSION=$CompleteVersion\n");
	print(CMD "SET LABEL=$CompleteVersion\n");
	print(CMD "SET AMD_COPYRIGHT_DATE=$CPYRTSTR\n");
	close(CMD);
	print "BuildVersionString: Dropped AMDVersion.cmd\n"; 
	print("CreateCmd: Setup Version will be $Major.$Minor.$Incr.$Build\n");
	print("CreateCmd: BuildCopyriteString: copyright string is $CPYRTSTR\n");
	
	print "CreateCmd: Completed\n";
}
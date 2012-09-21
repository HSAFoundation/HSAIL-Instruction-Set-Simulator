#!perl -w
#
#	generate_manifests.pl
#	Adam Seyer x59511, adam.seyer@amd.com
#	use strict;
#	use warnings;
#	Perl script to get generate manifest files

#
# Paths to the files we need to manipulate
#
my $iManifest = "Installation\\Files\\manifest_internal.xml";
my $nManifest = "Installation\\Files\\manifest_nda.xml";
my $pManifest = "Installation\\Files\\manifest_public.xml";

#
# Get the build number from the env variable BNumber
#
my $PKG_VERSION = $ENV{'PKG_VERSION'};
my $PROJECT_NAME = $ENV{'PROJECT_NAME'};
my $PRODUCTNAME = $ENV{'PRODUCTNAME'};

print("Generate_Manifest.pl: Package Version will be $PKG_VERSION\n");
print("Generate_Manifest.pl: Project Name will be $PROJECT_NAME\n");
print("Generate_Manifest.pl: Creating Internal Manifest File $iManifest\n");
updateManifest($iManifest);
print("Generate_Manifest.pl: Internal Manifest File Created\n");
print("Generate_Manifest.pl: Creating NDA Manifest File $nManifest\n");
updateManifest($nManifest);
print("Generate_Manifest.pl: NDA Manifest File Created\n");
print("Generate_Manifest.pl: Creating Public Manifest File $pManifest\n");
updateManifest($pManifest);
print("Generate_Manifest.pl: Public Manifest File Created\n");

print("Generate_Manifest.pl: Completed\n");
exit;

sub updateManifest
{
############################################################
# Update Manifest
############################################################
my $manifest = shift(@_);
open MAN, "<$manifest"
  or die "Can't open $manifest for reading: $!";

$linecount = 0;
while (<MAN>) {
  s/_PKG_VERSION_/$PKG_VERSION/g;
  s/_PROJECT_NAME_/$PROJECT_NAME/g;
  s/_RMS_PRODUCT_NAME_/$PRODUCTNAME/g;
  $new_man[$linecount] = $_;
  $linecount++;
}

close MAN;

open MAN, ">$manifest"
  or die "Can't open $manifest for writing: $!";

foreach my $line (@new_man) {
  print MAN $line;
}

close MAN;
}
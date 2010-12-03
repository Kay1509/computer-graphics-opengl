#!/usr/bin/perl
#  Download an image file from the web.

##  Usage from command line: "./getURLppm.pl  baseURL img.ppm"
##  Usage in C code:
##   system("./getURLppm.pl http://mysite/ texture1.ppm");
##   tex1 = readppm("./texture1.ppm");

## Example: ./getURLppm.pl http://www.cs.indiana.edu/~hanson fermat.ppm 

use LWP::Simple;
my $URLbase = $ARGV[0];
my $srcFile = $ARGV[1];
if ($URLbase eq "" || $srcFile eq "") {
    print("Usage : getURLppm.pl   http://my-URL/  my-image.ppm\n\n");
    exit(1);
}

print STDERR "Obtaining texture file from URL: ".$URLbase."/".$srcFile."\n";

my $page = get($URLbase."/".$srcFile);
if ($page eq "") {print STDERR "Failed to read $URLbase/$srcFile \n\n";
	      exit(1);}

open(OUT,">".$srcFile)
	|| die("ERROR : cannot create $srcFile!\n\n");

print OUT $page;

# Close the filehandle
close(OUT);

print "..done.\n";

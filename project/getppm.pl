#!/usr/bin/perl 

$remoteppm = $ARGV[0];

use LWP::Simple; 
my $page = get($remoteppm); 
print $page; 

#!/bin/bash
#
#Simple script to convert a directory of jpg images
# to ppm (Portable Pixel Map) images.
#
# Requires the 'convert' command to be installed (ImageMagick)
#
echo "Converting from JPG to PPM"

# Loop through the jpg images in the current directory
# and convert to ppm files
for f in *.jpg
do
    echo "Processing $f"
    convert $f ${f%jpg}ppm
done
echo "Done"

# Exit
exit 0;






##############################################################################
### This script is submitted to BigAdmin by a user of the BigAdmin community.
### Sun Microsystems, Inc. is not responsible for the
### contents or the code enclosed. 
###
###
### Copyright 2008 Sun Microsystems, Inc. ALL RIGHTS RESERVED
### Use of this software is authorized pursuant to the
### terms of the license found at
### http://www.sun.com/bigadmin/common/berkeley_license.html
##############################################################################




#!/bin/sh
# Generated file, master is Makefile.am
. ${srcdir:-.}/common.sh
infile="$srcdir/images/minisblack-1c-8b.tiff"
outfile="o-tiff2rgba-minisblack-1c-8b.tiff"
f_test_convert "$TIFF2RGBA" $infile $outfile
f_tiffinfo_validate $outfile

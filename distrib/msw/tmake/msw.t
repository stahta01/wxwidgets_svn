#!################################################################################
#! File:    msw.t
#! Purpose: tmake template file from which src/msw/files.lst containing the
#!          list of files for wxMSW library is generated by tmake
#! Author:  Vadim Zeitlin
#! Created: 28.01.00
#! Version: $Id$
#!################################################################################
#${
    #! include the code which parses filelist.txt file and initializes
    #! %wxCommon, %wxGeneric, %wxHtml, %wxUNIX, %wxMSW, %wxMOTIF and
    #! %wxOS2PM hashes.
    IncludeTemplate("filelist.t");

    #! find all our sources
    foreach $file (sort keys %wxGeneric) {
        next if $wxGeneric{$file} =~ /\b(PS|G|U|16)\b/;

        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"MSW_SOURCES"} .= "generic/" . $file . " ";
        $project{"GENERICOBJS"} .= $fileobj . " ";
    }

    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} =~ /\b(R|U|16)\b/;

        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"MSW_SOURCES"} .= "common/" . $file . " ";
        $project{"COMMONOBJS"} .= $fileobj . " ";
    }

    foreach $file (sort keys %wxMSW) {
        next if $wxMSW{$file} =~ /\b16\b/;

        ($fileobj = $file) =~ s/cp?p?$/\o/;

        if( $wxMSW{$file} =~ /\bO\b/ ) {
            $project{"MSW_SOURCES"} .= "msw/ole/" . $file . " ";
            $project{"OLEOBJS"} .= $fileobj . " ";
        } else {
            $project{"MSW_SOURCES"} .= "msw/" . $file . " ";
            $project{"GUIOBJS"} .= $fileobj . " ";

            if ( $wxMSW{$file} =~ /\bL\b/ ) {
                $project{"GUI_LOWLEVEL_OBJS"} .= $fileobj . " ";
            }
        }
    }

    foreach $file (sort keys %wxHTML) {
        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"MSW_SOURCES"} .= "html/" . $file . " ";
        $project{"HTMLOBJS"} .= $fileobj . " ";
    }
    #! find all our headers
    foreach $file (sort keys %wxWXINCLUDE) {
        $project{"MSW_HEADERS"} .= $file . " "
    }

    foreach $file (sort keys %wxMSWINCLUDE) {
        $project{"MSW_HEADERS"} .= "msw/" . $file . " "
    }

    foreach $file (sort keys %wxOLEINCLUDE) {
        $project{"MSW_HEADERS"} .= "msw/ole/" . $file . " ";
    }

    foreach $file (sort keys %wxGENERICINCLUDE) {
        $project{"MSW_HEADERS"} .= "generic/" . $file . " "
    }

    foreach $file (sort keys %wxUNIXINCLUDE) {
        $project{"MSW_HEADERS"} .= "unix/" . $file . " "
    }

    foreach $file (sort keys %wxHTMLINCLUDE) {
        $project{"MSW_HEADERS"} .= "html/" . $file . " "
    }

    foreach $file (sort keys %wxPROTOCOLINCLUDE) {
        $project{"MSW_HEADERS"} .= "protocol/" . $file . " "
    }
#$}
# This file was automatically generated by tmake 
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE MSW.T!
ALL_SOURCES = \
		#$ ExpandList("MSW_SOURCES");

ALL_HEADERS = \
		#$ ExpandList("MSW_HEADERS");

COMMONOBJS = \
		#$ ExpandList("COMMONOBJS");

GENERICOBJS = \
		#$ ExpandList("GENERICOBJS");

GUIOBJS = \
		#$ ExpandList("GUIOBJS");

GUI_LOWLEVEL_OBJS = \
		#$ ExpandList("GUI_LOWLEVEL_OBJS");

HTMLOBJS = \
		#$ ExpandList("HTMLOBJS");

OLEOBJS = \
		#$ ExpandList("OLEOBJS");


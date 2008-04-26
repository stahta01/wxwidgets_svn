#!/usr/bin/python

###################################
# Author: Kevin Ollivier
# License: wxWidgets License
###################################

import os
import sys
import builder
import optparse
import types

# builder object
wxBuilder = None

scriptDir = os.path.join(sys.path[0])
contribDir = os.path.join("contrib", "src")

format = "autoconf"
option_dict = { 
            "clean"     : (False, "Clean all files from the build directory"),
            "debug"     : (False, "Build the library in debug symbols"),
            "install"   : (False, "Install the toolkit to the installdir directory, or the default dir."),
            "installdir" : (".", "Directory where built wxWidgets will be installed"),
            "mac_fat_binary" : (False, "Build Mac version as a universal (fat) binary"),
            "no_config" : (False, "Turn off configure step on autoconf builds"),
            "rebake"    : (False, "Regenerate Bakefile and autoconf files"),
            "unicode"   : (False, "Build the library with unicode support"),
            "wxpython"  : (False, "Build the wxWidgets library with all options needed by wxPython"),
          }
          
toggle_opts = {
        "unicode": [{"unicode": "store_true", "ansi": "store_false"}]  
        }
    
parser = optparse.OptionParser(usage="usage: %prog [options]", version="%prog 1.0")

for opt in option_dict:
    default = option_dict[opt][0]
    
    action = "store"
    if type(default) == types.BooleanType:
        action = "store_true"
    parser.add_option("--" + opt, default=default, action=action, dest=opt, help=option_dict[opt][1])

options, arguments = parser.parse_args()


#if len(sys.argv) > 1:
#    for arg in sys.argv[1:]:
#        if arg[0:2] == "no-":
#            options[arg[3:]] = False
#        else:
#            options[arg] = True

def exitIfError(code, msg):
    if code != 0:
        print msg
        sys.exit(code)

if format == "autoconf":
    configure_opts = []
    
    if options.unicode:
        configure_opts.append("--enable-unicode")
        
    if options.debug:
        configure_opts.append("--enable-debug")
        
    if options.mac_fat_binary: 
        configure_opts.append("--enable-universal_binary")
    

    wxpy_configure_opts = [
                        "--with-opengl",
                        "--enable-sound",
                        "--with-sdl",
                        "--with-gnomeprint",
                        "--enable-graphics_ctx",
                        "--enable-mediactrl",
                        "--enable-display",
                        "--enable-geometry",
                        "--enable-debug_flag",
                        "--enable-optimise",
                        "--disable-debugreport",
                        "--enable-monolithic"
                        ]
                        
    if options.installdir != option_dict["installdir"][0]:
        configure_opts.append("--prefix=" + options.installdir)

    if options.wxpython:
        configure_opts.extend(wxpy_configure_opts)

    if options.rebake:
        retval = os.system("make -f autogen.mk")
        exitIfError(retval, "Error running autogen.mk")
        
    print "Configure options: " + `configure_opts`
    wxBuilder = builder.AutoconfBuilder()
    if not options.no_config and not options.clean:
        exitIfError(wxBuilder.configure(configure_opts), "Error running configure")
    
if not wxBuilder:
    print "Builder not available for your specified platform/compiler."
    sys.exit(1)

if options.clean:
    print "Performing cleanup."
    wxBuilder.clean()
    
    if options.wxpython:
        exitIfError(wxBuilder.clean(os.path.join(contribDir, "gizmos")), "Error building gizmos")
        exitIfError(wxBuilder.clean(os.path.join(contribDir, "stc")), "Error building stc")
    
    sys.exit(0)
    

    
exitIfError(wxBuilder.build(), "Error building")
    
if options.wxpython:
    exitIfError(wxBuilder.build(os.path.join(contribDir, "gizmos")), "Error building gizmos")
    exitIfError(wxBuilder.build(os.path.join(contribDir, "stc")), "Error building stc")
    
if options.install:
    wxBuilder.install()
    
    if options.wxpython:
        exitIfError(wxBuilder.install(os.path.join(contribDir, "gizmos")), "Error building gizmos")
        exitIfError(wxBuilder.install(os.path.join(contribDir, "stc")), "Error building stc")
    
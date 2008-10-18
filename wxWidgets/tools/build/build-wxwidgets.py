#!/usr/bin/env python

###################################
# Author: Kevin Ollivier
# License: wxWidgets License
###################################

import os
import re
import sys
import builder
import commands
import glob
import optparse
import platform
import shutil
import types

# builder object
wxBuilder = None

scriptDir = os.path.abspath(sys.path[0])
wxRootDir = os.path.abspath(os.path.join(scriptDir, "..", ".."))
contribDir = os.path.join("contrib", "src")
installDir = None

if sys.platform.startswith("win"):
    contribDir = os.path.join(wxRootDir, "contrib", "build")

if sys.platform.startswith("win"):
    toolkit = "msvc"
else:
    toolkit = "autoconf"

option_dict = { 
            "clean"     : (False, "Clean all files from the build directory"),
            "debug"     : (False, "Build the library in debug symbols"),
            "install"   : (False, "Install the toolkit to the installdir directory, or the default dir."),
            "installdir" : (".", "Directory where built wxWidgets will be installed"),
            "mac_universal_binary" : (False, "Build Mac version as a universal binary"),
            "mac_lipo" : (False, "EXPERIMENTAL: Create a universal binary by merging a PPC and Intel build together."),
            "mac_framework" : (False, "Install the Mac build as a framework"),
            "no_config" : (False, "Turn off configure step on autoconf builds"),
            "rebake"    : (False, "Regenerate Bakefile and autoconf files"),
            "unicode"   : (False, "Build the library with unicode support"),
            "wxpython"  : (False, "Build the wxWidgets library with all options needed by wxPython"),
            "cocoa"     : (False, "Build the Cooca port (Mac only currently)."),
            "osx_cocoa" : (False, "Build the new Cocoa port"), 
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
#            flags[arg[3:]] = False
#        else:
#            flags[arg] = True

def exitIfError(code, msg):
    if code != 0:
        print msg
        sys.exit(code)
        
def getWxRelease():
    global wxRootDir
    configureText = open(os.path.join(wxRootDir, "configure.in"), "r").read()
    
    majorVersion = re.search("wx_major_version_number=(\d)", configureText).group(1)
    minorVersion = re.search("wx_minor_version_number=(\d)", configureText).group(1)
    
    return "%s.%s" % (majorVersion, minorVersion)
        
def doMacLipoBuild(arch, buildDir, installDir, cxxcompiler="g++-4.0", cccompiler="gcc-4.0", target="10.4", flags=""):
    # PPC build
    archInstallDir = installDir + "/" + arch
    os.environ["CXX"] = "%s -arch %s %s" % (cxxcompiler, arch, flags)
    os.environ["CC"] = "%s -arch %s %s" % (cccompiler, arch, flags)
    os.environ["MACOSX_DEPLOYMENT_TARGET"] = target
    archArgs = ["prefix=" + archInstallDir]
    buildRoot = "bld-" + arch
    if buildDir:
        buildRoot = buildDir + "/" + buildRoot
    
    if not os.path.exists(buildRoot):
        os.makedirs(buildRoot)
    
    olddir = os.getcwd()
    os.chdir(buildRoot)
    if args:
        archArgs.extend(args)
    
    if not options.no_config:
    	exitIfError(wxBuilder.configure(options=configure_opts), "Error running configure")
    exitIfError(wxBuilder.build(options=archArgs), "Error building")
    exitIfError(wxBuilder.install(options=["prefix=" + archInstallDir]), "Error Installing")
    
    if options.wxpython and os.path.exists(os.path.join(wxRootDir, contribDir)):
        exitIfError(wxBuilder.build(dir=os.path.join(contribDir, "gizmos"), options=args), "Error building gizmos")
        exitIfError(wxBuilder.install(os.path.join(contribDir, "gizmos"), options=["prefix=" + archInstallDir]), "Error Installing gizmos")
        
        exitIfError(wxBuilder.build(dir=os.path.join(contribDir, "stc"),options=args), "Error building stc")
        exitIfError(wxBuilder.install(os.path.join(contribDir, "stc"),options=["prefix=" + archInstallDir]), "Error installing stc")

    os.chdir(olddir)
    
# compiler / build system specific args
buildDir = None
args = None
installDir = options.installdir

if toolkit == "autoconf":
    configure_opts = []
    
    if options.unicode:
        configure_opts.append("--enable-unicode")
        
    if options.debug:
        configure_opts.append("--enable-debug")
        
    if options.mac_universal_binary: 
        configure_opts.append("--enable-universal_binary")
        
    if options.cocoa:
        configure_opts.append("--with-cocoa")
        
    if options.osx_cocoa:
        configure_opts.append("--with-osx_cocoa")
    
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
                        
    if not options.mac_framework and options.installdir != option_dict["installdir"][0]:
        configure_opts.append("--prefix=" + installDir)

    if options.wxpython:
        configure_opts.extend(wxpy_configure_opts)

    if options.rebake:
        retval = os.system("make -f autogen.mk")
        exitIfError(retval, "Error running autogen.mk")
        
    if options.mac_framework:
        installDir = "/Library/Frameworks/wx.framework/Versions/%s" %  getWxRelease()
        configure_opts.append("--prefix=" + installDir)
        # framework builds always need to be monolithic
        if not "--enable-monolithic" in configure_opts:
            configure_opts.append("--enable-monolithic")
        
    print "Configure options: " + `configure_opts`
    wxBuilder = builder.AutoconfBuilder()
    if not options.no_config and not options.clean and not options.mac_lipo:
        exitIfError(wxBuilder.configure(options=configure_opts), "Error running configure")

elif toolkit == "msvc":
    flags = {}
    buildDir = os.path.join(scriptDir, "..", "msw")
    args = ["-f makefile.vc"]
    if options.unicode:
        flags["wxUSE_UNICODE"] = "1"
        flags["wxUSE_UNICODE_MSLU"] = "1"
        args.append("UNICODE=1")
        args.append("MSLU=1")

    if options.wxpython:
        args.append("OFFICIAL_BUILD=1")
        args.append("SHARED=1")
        args.append("MONOLITHIC=0")
        args.append("USE_OPENGL=1")
        args.append("USE_GDIPLUS=1")
        flags["wxDIALOG_UNIT_COMPATIBILITY "] = "0"
        flags["wxUSE_DEBUG_CONTEXT"] = "1"
        flags["wxUSE_MEMORY_TRACING"] = "1"
        flags["wxUSE_DIALUP_MANAGER"] = "0"
        flags["wxUSE_GLCANVAS"] = "1"
        flags["wxUSE_POSTSCRIPT"] = "1"
        flags["wxUSE_AFM_FOR_POSTSCRIPT"] = "0"
        flags["wxUSE_DISPLAY"] = "1"
        flags["wxUSE_DIB_FOR_BITMAP"] = "1"
        flags["wxUSE_DEBUGREPORT"] = "0"
        flags["wxUSE_GRAPHICS_CONTEXT"] = "1"
        flags["wxUSE_DATEPICKCTRL_GENERIC"] = "1"
    
        # setup the wxPython 'hybrid' build
        if not options.debug:
            args.append("DEBUG_FLAG=1")
            args.append("CXXFLAGS=/D__NO_VC_CRTDBG__")
            args.append("WXDEBUGFLAG=h")
            args.append("BUILD=release")
            flags["wxUSE_MEMORY_TRACING"] = "0"
            flags["wxUSE_DEBUG_CONTEXT"] = "0"

    mswIncludeDir = os.path.join(wxRootDir, "include", "wx", "msw")
    setup0File = os.path.join(mswIncludeDir, "setup0.h")
    setupText = open(setup0File, "rb").read()
    
    for flag in flags:
        setupText, subsMade = re.subn(flag + "\s+?\d", "%s %s" % (flag, flags[flag]), setupText)
        if subsMade == 0:
            print "Flag %s wasn't found!" % flag
            sys.exit(1)

    setupFile = open(os.path.join(mswIncludeDir, "setup.h"), "wb")
    setupFile.write(setupText)
    setupFile.close()
    
    wxBuilder = builder.MSVCBuilder()
    
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

isLipo = False    
if options.mac_lipo:
    if options.mac_universal_binary:
        print "WARNING: Cannot specify both mac_lipo and mac_universal_binary, as they conflict."
        print "         Using mac_universal_binary..."
    else:
        isLipo = True
        # TODO: Add 64-bit when we're building OS X Cocoa
        
        # 2.8, use gcc 3.3 on PPC for 10.3 support...
        macVersion = platform.mac_ver()[0]
        isLeopard = macVersion.find("10.5") != -1
        
        if not isLeopard and os.path.exists(os.path.join(wxRootDir, contribDir)):
            doMacLipoBuild("ppc", buildDir, installDir, cxxcompiler="g++-3.3", cccompiler="gcc-3.3", 
                        target="10.3", flags="-DMAC_OS_X_VERSION_MAX_ALLOWED=1040")
        else:
            doMacLipoBuild("ppc", buildDir, installDir)

        doMacLipoBuild("i386", buildDir, installDir)

        result = os.system("python %s/distrib/scripts/mac/lipo-dir.py %s %s %s" % (wxRootDir, installDir + "/ppc", installDir + "/i386", installDir))
        fname = os.path.abspath(installDir + '/bin/wx-config') 
        data = open(fname).read()
        data = data.replace('ppc/', '')
        data = data.replace('i386/', '')
        open(fname, 'w').write(data)
        
        shutil.rmtree(installDir + "/ppc")
        shutil.rmtree(installDir + "/i386")
  
if not isLipo:
    exitIfError(wxBuilder.build(dir=buildDir, options=args), "Error building")
    
    if options.wxpython and os.path.exists(contribDir):
        exitIfError(wxBuilder.build(os.path.join(contribDir, "gizmos"), options=args), "Error building gizmos")
        exitIfError(wxBuilder.build(os.path.join(contribDir, "stc"),options=args), "Error building stc")
        
    if options.install:
        wxBuilder.install()
        
        if options.wxpython and os.path.exists(contribDir):
            exitIfError(wxBuilder.install(os.path.join(contribDir, "gizmos")), "Error building gizmos")
            exitIfError(wxBuilder.install(os.path.join(contribDir, "stc")), "Error building stc")
        
if options.mac_framework:
    os.chdir(installDir)
    build_string = ""
    if options.debug:
        build_string = "d"
    version = commands.getoutput("bin/wx-config --release")
    basename = commands.getoutput("bin/wx-config --basename")
    os.system("ln -s -f bin Resources")
    os.system("ln -s -f lib/lib%s-%s.dylib ./wx" % (basename, version))
    
    for lib in glob.glob("lib/*.dylib"):
        if not os.path.islink(lib):
            os.system("install_name_tool -id %s/%s" % (installDir, lib))
    
    os.system("ln -s -f include/wx Headers")
    
    for lib in ["GL", "STC", "Gizmos"]:  
        libfile = "lib/lib%s_%s-%s.dylib" % (basename, lib.lower(), version)
        if os.path.exists(libfile):
            frameworkDir = "framework/wx%s/%s" % (lib, version)
            if not os.path.exists(frameworkDir):
                os.makedirs(frameworkDir)
            os.system("ln -s -f ../../../%s %s/wx%s" % (libfile, frameworkDir, lib))        
    
    os.chdir("include")
    
    header_template = """
    
#ifndef __WX_FRAMEWORK_HEADER__
#define __WX_FRAMEWORK_HEADER__

%s

#endif // __WX_FRAMEWORK_HEADER__
"""
    headers = ""
    header_dir = "wx-%s/wx" % version
    for include in glob.glob(header_dir + "/*.h"):
        headers += "wx/" + os.path.basename(include) + "\n"
        
    framework_header = open("wx.h", "w")
    framework_header.write(header_template % headers)
    framework_header.close()
    
    os.system("ln -s -f %s wx" % header_dir)
    
    os.chdir(os.path.join(installDir, "..", ".."))
    os.system("ln -s -f Versions/%s Versions/Current" % os.path.basename(installDir))
    os.system("ln -s -f Versions/Current/Headers Headers" % os.path.basename(installDir))
    os.system("ln -s -f Versions/Current/Resources Resources" % os.path.basename(installDir))
    os.system("ln -s -f Versions/Current/wx wx" % os.path.basename(installDir))
    

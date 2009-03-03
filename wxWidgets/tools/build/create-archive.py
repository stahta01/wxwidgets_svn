#!/usr/bin/env python

import glob
import optparse
import os
import re
import shutil
import string
import sys
import tempfile
import types


## CONSTANTS

scriptDir = os.path.join(sys.path[0])
rootDir = os.path.abspath(os.path.join(scriptDir, "..", ".."))
contribDir = os.path.join("contrib", "src")

dirsToCopy = ["art", "build", "debian", "demos", "docs", "include", "interface", "lib",
                "locale", "samples", "src", "utils"]

dirsToIgnore = [".svn", "CVS"]
excludeExtensions = [".rej", ".orig", ".mine", ".tmp"]

option_dict = {
            "name"          : ("wxWidgets", "Name given to the tarball created (default: wxWidgets)"),
            "postfix"       : ("", "String appended to the version to indicate a special release (default: none)"),
            "compression"   : ("gzip", "Compression to use. Values are: gzip, bzip, zip, all (default: gzip)"),
            }

mswProjectFiles = [ ".vcproj", ".sln", ".dsp", ".dsw", ".vc", ".bat"]
nativeLineEndingFiles = [".cpp", ".h", ".c", ".txt"]



## PARSE OPTIONS

usage="""usage: %prog [options] <output directory>\n
Create a wxWidgets archive and store it in <output directory>.
The output directory must be an absolute, existing path.
Type %prog --help for options.
"""

parser = optparse.OptionParser(usage, version="%prog 1.0")

for opt in option_dict:
    default = option_dict[opt][0]

    action = "store"
    if type(default) == types.BooleanType:
        action = "store_true"
    parser.add_option("--" + opt, default=default, action=action, dest=opt, help=option_dict[opt][1])

options, arguments = parser.parse_args()

if len(arguments) < 1 or not os.path.exists(arguments[0]) or not os.path.isabs(arguments[0]):
    parser.print_usage()
    sys.exit(1)

destDir = arguments[0]
if not os.path.exists(destDir):
    os.makedirs(destDir)

wxVersion = None
VERSION_FILE = os.path.join(rootDir, 'include/wx/version.h')


## HELPER FUNCTIONS

def makeDOSLineEndings(dir, extensions):
    fileList = []
    for root, subFolders, files in os.walk(dir):
        for file in files:
            if os.path.splitext(file)[1] in extensions:
                os.system("unix2dos %s" % os.path.join(root, file))

def getVersion():
    """Returns wxWidgets version as a tuple: (major,minor,release)."""
    global wxVersion
    if wxVersion == None:
        f = open(VERSION_FILE, 'rt')
        lines = f.readlines()
        f.close()
        major = minor = release = None
        for l in lines:
            if not l.startswith('#define'): continue
            splitline = l.strip().split()
            if splitline[0] != '#define': continue
            if len(splitline) < 3: continue
            name = splitline[1]
            value = splitline[2]
            if value == None: continue
            if name == 'wxMAJOR_VERSION': major = int(value)
            if name == 'wxMINOR_VERSION': minor = int(value)
            if name == 'wxRELEASE_NUMBER': release = int(value)
            if major != None and minor != None and release != None:
                break
        wxVersion = (major, minor, release)
    return wxVersion

def allFilesRecursive(dir):
    fileList = []
    for root, subFolders, files in os.walk(dir):
        shouldCopy = True
        for ignoreDir in dirsToIgnore:
            if ignoreDir in root:
                shouldCopy = False

        if shouldCopy:
            for file in files:
                path = os.path.join(root,file)
                for exclude in excludeExtensions:
                    if not os.path.splitext(file)[1] in excludeExtensions:
                        fileList.append(os.path.join(root,file))
    return fileList


## MAKE THE RELEASE!

str_version = "%d.%d.%d" % getVersion()
if options.postfix != "":
    str_version += "-" + options.postfix
    
full_name = options.name + "-" + str_version

copyDir = tempfile.mkdtemp()
wxCopyDir = os.path.join(copyDir, full_name) 

os.makedirs(wxCopyDir)

os.chdir(rootDir)
fileList = []
rootFiles = glob.glob("*")
for afile in rootFiles:
    if os.path.isfile(os.path.abspath(afile)):
        fileList.append(afile)

for dir in dirsToCopy:
    print "Determining files to copy from %s..." % dir
    fileList.extend(allFilesRecursive(dir))

print "Copying files to the temporary folder %s..." % copyDir
for afile in fileList:
    destFile = os.path.join(wxCopyDir, afile)
    dirName = os.path.dirname(destFile)
    if not os.path.exists(dirName):
        os.makedirs(dirName)
    shutil.copy(os.path.join(rootDir, afile), destFile)

# copy include/wx/msw/setup0.h -> include/wx/msw/setup.h
mswSetup0 = os.path.join(wxCopyDir, "include","wx","msw","setup0.h") 
shutil.copy(mswSetup0, mswSetup0.replace("setup0.h", "setup.h")), 
    
all = True if options.compression == "all" else False
    
# make sure they have the DOS line endings everywhere
print "Setting MSW Project files to use DOS line endings..."
makeDOSLineEndings(wxCopyDir, mswProjectFiles)

if all or options.compression == "gzip":
    print "Creating gzip archive..."
    os.chdir(copyDir)
    os.system("tar -czvf %s/%s.tar.gz %s" % (destDir, full_name, "*"))
    os.chdir(rootDir)

if all or options.compression == "bzip":
    print "Creating bzip archive..."
    os.chdir(copyDir)
    os.system("tar -cjvf %s/%s.tar.bz2 %s" % (destDir, full_name, "*"))
    os.chdir(rootDir)

if all or options.compression == "zip":
    os.chdir(copyDir)
    print "Setting DOS line endings on source and text files..."
    makeDOSLineEndings(copyDir, nativeLineEndingFiles)
    print "Creating zip archive..."
    os.system("zip -9 -r %s/%s.zip %s" % (destDir, full_name, "*"))
    os.chdir(rootDir)

shutil.rmtree(copyDir)

#----------------------------------------------------------------------
# Name:        wxversion
# Purpose:     Allows a wxPython program to search for alternate 
#              installations of the wxPython packages and modify sys.path
#              so they will be found when "import wx" is done.
#
# Author:      Robin Dunn
#
# Created:     24-Sept-2004
# RCS-ID:      $Id$
# Copyright:   (c) 2004 by Total Control Software
# Licence:     wxWindows license
#----------------------------------------------------------------------

"""
If you have more than one version of wxPython installed this module
allows your application to choose which version of wxPython will be
imported when it does 'import wx'.  You use it like this:

    import wxversion
    wxversion.require('2.4')
    import wx

Of course the default wxPython version can also be controlled by
setting PYTHONPATH or by editing the wx.pth path configuration file,
but using wxversion will allow an application to manage the version
selection itself rather than depend on the user to setup the
environment correctly.

It works by searching the sys.path for directories matching wx-* and
then comparing them to what was passed to the require function.  If a
match is found then that path is inserted into sys.path.
"""

import sys, os, glob, fnmatch




def require(versions):
    """
    Search for a wxPython installation that matches version.

        :param version: Specifies the version to look for, it can either be
                        a sting or a list of strings.  Each string is
                        compared to the installed wxPythons and the best
                        match is added to the sys.path, allowing an 'import
                        wx' to find that version.

                        The version string is composed of the dotted
                        version number (at least 2 of the 4 components)
                        optionally followed by hyphen ('-') separated
                        options (wx port, unicode/ansi, flavour, etc.)  A
                        match is determined by how much of the installed
                        version matches what is given in the version
                        parameter.  If the version number components don't
                        match then the score is zero, otherwise the score
                        is increased for every specified optional component
                        that is specified and that matches.
    """
    assert not sys.modules.has_key('wx') and not sys.modules.has_key('wxPython'), \
           "wxversion.require() must be called before wxPython is imported"

    bestMatch = None
    bestScore = 0
    if type(versions) == str:
        versions = [versions]
        
    packages = _find_installed()
    for pkg in packages:
        for ver in versions:
            score = pkg.Score(_wxPackageInfo(ver))
            if score > bestScore:
                bestMatch = pkg
                bestScore = score

    assert bestMatch is not None, \
           "Required version of wxPython not found"

    sys.path.insert(0, bestMatch.pathname)
        
        


_pattern = "wx-[0-9].*"
def _find_installed():
    installed = []
    for pth in sys.path:

        # empty means to look in the current dir
        if not pth:
            pth = '.'

        # skip it if it's not a package dir
        if not os.path.isdir(pth):
            continue
        
        base = os.path.basename(pth)

        # if it's a wx path that's already in the sys.path then skip it
        if fnmatch.fnmatchcase(base, _pattern):
            continue

        # now look in the dir for matching subdirs
        for name in glob.glob(os.path.join(pth, _pattern)):
            # make sure it's a directory
            if not os.path.isdir(name):
                continue
            # and has a wx subdir
            if not os.path.exists(os.path.join(name, 'wx')):
                continue
            installed.append(_wxPackageInfo(name, True))

    installed.sort()
    installed.reverse()
    return installed


class _wxPackageInfo(object):
    def __init__(self, pathname, stripFirst=False):
        self.pathname = pathname
        base = os.path.basename(pathname)
        segments = base.split('-')
        if stripFirst:
            segments = segments[1:]
        self.version = tuple([int(x) for x in segments[0].split('.')])
        self.options = segments[1:]


    def Score(self, other):
        score = 0
        # whatever version components given in other must match exactly
        if len(self.version) > len(other.version):
            v = self.version[:len(other.version)]
        else:
            v = self.version
        if v != other.version:
            return 0
        score += 1
        for opt in other.options:
            if opt in self.options:
                score += 1
        return score
    

    # TODO: factor self.options into the sort order?
    def __lt__(self, other):
        return self.version < other.version
    def __gt__(self, other):
        return self.version > other.version
    def __eq__(self, other):
        return self.version == other.version
        
    



if __name__ == '__main__':
    def test(version):
        savepath = sys.path[:]
        require(version)
        print "Asked for %s:\t got: %s" % (version, sys.path[0])
        sys.path = savepath[:]
        
        
    # make some test dirs
    names = ['wx-2.4',
             'wx-2.5.2',
             'wx-2.5.2.9-gtk2-unicode',
             'wx-2.5.2.9-gtk-ansi',
             'wx-2.5.1',
             'wx-2.5.2.8-gtk2-unicode',
             'wx-2.5.3']
    for name in names:
        d = os.path.join('/tmp', name)
        os.mkdir(d)
        os.mkdir(os.path.join(d, 'wx'))

    # setup sys.path to see those dirs
    sys.path.append('/tmp')
    

    # now run some tests
    test("2.4")
    test("2.5")
    test("2.5-gtk2")
    test("2.5.2")
    test("2.5-ansi")
    test("2.5-unicode")
    
    # There isn't a unicode match for this one, but it will give the best
    # available 2.4.  Should it give an error instead?  I don't think so...
    test("2.4-unicode") 

    try:
        # expecting an error on this one
        test("2.6")
    except AssertionError:
        print "Asked for 2.6:\t got: Assertion" 

    # Try asking for multiple versions
    test(["2.6", "2.5.3", "2.5.2-gtk2"])

    # cleanup
    for name in names:
        d = os.path.join('/tmp', name)
        os.rmdir(os.path.join(d, 'wx'))
        os.rmdir(d)

        

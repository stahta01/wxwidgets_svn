#!/usr/bin/env python
#----------------------------------------------------------------------

import sys, os, string
from distutils.core      import setup, Extension
from distutils.file_util import copy_file
from distutils.dir_util  import mkpath
from distutils.dep_util  import newer

from my_distutils import run_swig, contrib_copy_tree

#----------------------------------------------------------------------
# flags and values that affect this script
#----------------------------------------------------------------------

VERSION          = "2.3.1"
DESCRIPTION      = "Cross platform GUI toolkit for Python"
AUTHOR           = "Robin Dunn"
AUTHOR_EMAIL     = "Robin Dunn <robin@alldunn.com>"
URL              = "http://wxPython.org/"
LICENCE          = "wxWindows (LGPL derivative)"
LONG_DESCRIPTION = """\
wxPython is a GUI toolkit for Python that is a wrapper around the
wxWindows C++ GUI library.  wxPython provides a large variety of
window types and controls, all implemented with a native look and
feel (and native runtime speed) on the platforms it is supported
on.
"""


BUILD_GLCANVAS = 1 # If true, build the contrib/glcanvas extension module
BUILD_OGL = 1      # If true, build the contrib/ogl extension module
BUILD_STC = 1      # If true, build the contrib/stc extension module
BUILD_IEWIN = 0    # Internet Explorer wrapper (experimental)
BUILD_XRC = 1      # XML based resource system


CORE_ONLY = 0      # if true, don't build any of the above
GL_ONLY = 0        # Only used when making the -gl RPM.  See the "b" script
                   # for the ugly details

USE_SWIG = 0       # Should we actually execute SWIG, or just use the
                   # files already in the distribution?

IN_CVS_TREE = 0    # Set to true if building in a full wxWindows CVS
                   # tree, otherwise will assume all needed files are
                   # available in the wxPython source distribution

WX_CONFIG = "wx-config"    # Usually you shouldn't need to touch this,
                           # but you can set it to pass an alternate
                           # version of wx-config or alternate flags,
                           # eg. as required by the .deb in-tree build.

# Some MSW build settings

FINAL = 1          # Mirrors use of same flag in wx makefiles,
                   # (0 or 1 only) should probably find a way to
                   # autodetect this...

HYBRID = 0         # If set and not debug or FINAL, then build a
                   # hybrid extension that can be used by the
                   # non-debug version of python, but contains
                   # debugging symbols for wxWindows and wxPython.
                   # wxWindows must have been built with /MD, not /MDd
                   # (using FINAL=hybrid will do it.)

WXDLLVER = '23_1'  # Version part of DLL name


#----------------------------------------------------------------------

def msg(text):
    if __name__ == "__main__":
        print text


#----------------------------------------------------------------------
# Some other globals
#----------------------------------------------------------------------

PKGDIR = 'wxPython'
wxpExtensions = []

force = '--force' in sys.argv or '-f' in sys.argv
debug = '--debug' in sys.argv or '-g' in sys.argv

bcpp_compiling = '-c' in sys.argv and 'my_bcpp' in sys.argv # Bad heuristic

if bcpp_compiling:
    msg("Compiling wxPython by Borland C/C++ Compiler")
    HYBRID=0
    WXBCPPLIBVER = string.replace(WXDLLVER,"_","")
    # Version part of BCPP build LIBRARY name
    WXDLLVER="" # no dll ver path avaible


#----------------------------------------------------------------------
# Check for build flags on the command line
#----------------------------------------------------------------------

for flag in ['BUILD_GLCANVAS', 'BUILD_OGL', 'BUILD_STC', 'BUILD_XRC',
             'CORE_ONLY', 'USE_SWIG', 'IN_CVS_TREE',
             'FINAL', 'HYBRID', ]:
    for x in range(len(sys.argv)):
        if string.find(sys.argv[x], flag) == 0:
            pos = string.find(sys.argv[x], '=') + 1
            if pos > 0:
                vars()[flag] = eval(sys.argv[x][pos:])
                sys.argv[x] = ''

for option in ['WX_CONFIG', 'WXDLLVER', ]:
    for x in range(len(sys.argv)):
        if string.find(sys.argv[x], option) == 0:
            pos = string.find(sys.argv[x], '=') + 1
            if pos > 0:
                vars()[option] = sys.argv[x][pos:]
                sys.argv[x] = ''

sys.argv = filter(None, sys.argv)


if CORE_ONLY:
    BUILD_GLCANVAS = 0
    BUILD_OGL = 0
    BUILD_STC = 0
    BUILD_XRC = 0

#----------------------------------------------------------------------
# Setup some platform specific stuff
#----------------------------------------------------------------------

if os.name == 'nt':
    # Set compile flags and such for MSVC.  These values are derived
    # from the wxWindows makefiles for MSVC, others will probably
    # vary...
    WXDIR = os.environ['WXWIN']
    WXPLAT = '__WXMSW__'
    GENDIR = 'msw'

    if debug:
        FINAL  = 0
        HYBRID = 0

    if HYBRID:
        FINAL = 0

    includes = ['src',
                os.path.join(WXDIR, 'include'),
                ]

    defines = [ ('WIN32', None),        # Some of these are no longer
                ('__WIN32__', None),    # necessary.  Anybody know which?
                ('_WINDOWS', None),
                ('__WINDOWS__', None),
                ('WINVER', '0x0400'),
                ('__WIN95__', None),
                ('STRICT', None),

                (WXPLAT, None),
                ('WXUSINGDLL', '1'),

                ('SWIG_GLOBAL', None),
                ('HAVE_CONFIG_H', None),
                ('WXP_USE_THREAD', '1'),
                ]

    if bcpp_compiling:  # overwrite it
        defines = [
            ('_WINDOWS', None),
            ('WINVER', '0x0400'),
            ('STRICT', None),

            ('WXUSINGDLL', '1'),

            ('SWIG_GLOBAL', None),
            ('HAVE_CONFIG_H', None),
            ('WXP_USE_THREAD', '1'),

            ('WXUSE_DEFINE','1'),
            ('_RTLDLL',None),
            ]


    if not FINAL or HYBRID:
        defines.append( ('__WXDEBUG__', None) )

    libdirs = [os.path.join(WXDIR, 'lib'), 'build\\ilib']

    if FINAL:
        wxdll = 'wx' + WXDLLVER
    elif HYBRID:
        wxdll = 'wx' + WXDLLVER + 'h'
    else:
        wxdll = 'wx' + WXDLLVER + 'd'


    libs = [wxdll]
    if bcpp_compiling:
        libs = ['wx'+WXBCPPLIBVER]

    libs = libs + ['kernel32', 'user32', 'gdi32', 'comdlg32',
            'winspool', 'winmm', 'shell32', 'oldnames', 'comctl32',
            'ctl3d32', 'odbc32', 'ole32', 'oleaut32', 'uuid', 'rpcrt4',
            'advapi32', 'wsock32']


    cflags = [] #['/GX-']  # workaround for internal compiler error in MSVC 5
    lflags = None


    if bcpp_compiling:  # overwrite it
        cflags = ['-5', '-VF',  ### To supplort MSVC spurious semicolons in the class scope
                  ### else, all semicolons at the end of all DECLARE_...CALLBACK... macros must be eliminated
                  '-Hc', '-H='+WXDIR+'\src\msw\wx32.csm',
                  '@'+WXDIR+'\src\msw\wxwin32.cfg'
                  ]


    if not FINAL and HYBRID and not bcpp_compiling:
        cflags = cflags + ['/Od', '/Z7']
        lflags = ['/DEBUG', ]

    elif bcpp_compiling and not FINAL:
        cflags = cflags + ['/Od', '/v', '/y']
        lflags = lflags + ['/v', ]   ## '/PDB:NONE']



elif os.name == 'posix':
    # Set flags for Unix type platforms

    WXDIR = '..'              # assumes IN_CVS_TREE
    WXPLAT = '__WXGTK__'      # and assumes GTK...
    GENDIR = 'gtk'            # Need to allow for Motif eventually too

    includes = ['src']
    defines = [('SWIG_GLOBAL', None),
               ('HAVE_CONFIG_H', None),
               ('WXP_USE_THREAD', '1'),
               ]
    libdirs = []
    libs = []

    cflags = os.popen(WX_CONFIG + ' --cflags', 'r').read()[:-1] + ' ' + \
             os.popen('gtk-config --cflags', 'r').read()[:-1]
    cflags = string.split(cflags)

    lflags = os.popen(WX_CONFIG + ' --libs', 'r').read()[:-1]
    lflags = string.split(lflags)


else:
    raise 'Sorry Charlie...'


#----------------------------------------------------------------------
# Check if the version file needs updated
#----------------------------------------------------------------------

if IN_CVS_TREE and newer('setup.py', 'src/__version__.py'):
    open('src/__version__.py', 'w').write("ver = '%s'\n" % VERSION)



#----------------------------------------------------------------------
# SWIG defaults
#----------------------------------------------------------------------

swig_force = force
swig_args = ['-c++', '-shadow', '-python', '-keyword', '-dnone', #'-dascii',
             '-I./src', '-D'+WXPLAT]
swig_deps = ['src/my_typemaps.i']


#----------------------------------------------------------------------
# Define the CORE extension module
#----------------------------------------------------------------------

if not GL_ONLY:
    msg('Preparing CORE...')
    swig_files = [ 'wx.i', 'windows.i', 'windows2.i', 'windows3.i', 'events.i',
                   'misc.i', 'misc2.i', 'gdi.i', 'mdi.i', 'controls.i',
                   'controls2.i', 'cmndlgs.i', 'stattool.i', 'frames.i', 'image.i',
                   'printfw.i', 'sizers.i', 'clip_dnd.i',
                   'filesys.i', 'streams.i',
                   ##'grid.i', 'html.i', 'htmlhelp.i', 'calendar.i', 'utils.i',
                   ]

    swig_sources = run_swig(swig_files, 'src', GENDIR, PKGDIR,
                            USE_SWIG, swig_force, swig_args, swig_deps)

    copy_file('src/__init__.py', PKGDIR, update=1, verbose=0)
    copy_file('src/__version__.py', PKGDIR, update=1, verbose=0)


    if IN_CVS_TREE:   # update the licence files
        mkpath('licence')
        for file in ['preamble.txt', 'licence.txt', 'licendoc.txt', 'lgpl.txt']:
            copy_file(WXDIR+'/docs/'+file, 'licence/'+file, update=1, verbose=0)


    if os.name == 'nt':
        rc_file = ['src/wxc.rc']
    else:
        rc_file = []


    ext = Extension('wxc', ['src/helpers.cpp',
                            'src/libpy.c',
                            ] + rc_file + swig_sources,

                    include_dirs = includes,
                    define_macros = defines,

                    library_dirs = libdirs,
                    libraries = libs,

                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )
    wxpExtensions.append(ext)


    # Extension for the grid module
    swig_sources = run_swig(['grid.i'], 'src', GENDIR, PKGDIR,
                            USE_SWIG, swig_force, swig_args, swig_deps)
    ext = Extension('gridc', swig_sources,
                    include_dirs =  includes,
                    define_macros = defines,
                    library_dirs = libdirs,
                    libraries = libs,
                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )
    wxpExtensions.append(ext)


    # Extension for the html modules
    swig_sources = run_swig(['html.i', 'htmlhelp.i'], 'src', GENDIR, PKGDIR,
                            USE_SWIG, swig_force, swig_args, swig_deps)
    ext = Extension('htmlc', swig_sources,
                    include_dirs =  includes,
                    define_macros = defines,
                    library_dirs = libdirs,
                    libraries = libs,
                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )
    wxpExtensions.append(ext)


    # Extension for the utils module
    swig_sources = run_swig(['utils.i'], 'src', GENDIR, PKGDIR,
                            USE_SWIG, swig_force, swig_args, swig_deps)
    ext = Extension('utilsc', swig_sources,
                    include_dirs =  includes,
                    define_macros = defines,
                    library_dirs = libdirs,
                    libraries = libs,
                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )
    wxpExtensions.append(ext)


    # Extension for the calendar module
    swig_sources = run_swig(['calendar.i'], 'src', GENDIR, PKGDIR,
                            USE_SWIG, swig_force, swig_args, swig_deps)
    ext = Extension('calendarc', swig_sources,
                    include_dirs =  includes,
                    define_macros = defines,
                    library_dirs = libdirs,
                    libraries = libs,
                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )
    wxpExtensions.append(ext)


#----------------------------------------------------------------------
# Define the GLCanvas extension module
#----------------------------------------------------------------------

if BUILD_GLCANVAS or GL_ONLY:
    msg('Preparing GLCANVAS...')
    location = 'contrib/glcanvas'
    swig_files = ['glcanvas.i']
    other_sources = []

    swig_sources = run_swig(swig_files, location, GENDIR, PKGDIR,
                            USE_SWIG, swig_force, swig_args)

    gl_libs = []
    if os.name == 'posix':
        if '-D__WXDEBUG__' in cflags:
            gl_libs = ['wx_gtkd_gl', 'GL', 'GLU']
        else:
            gl_libs = ['wx_gtk_gl', 'GL', 'GLU']
    else:
        other_sources = [location + '/msw/myglcanvas.cpp']
        gl_libs = ['opengl32', 'glu32']


    ext = Extension('glcanvasc',
                    swig_sources + other_sources,

                    include_dirs = includes,
                    define_macros = defines,

                    library_dirs = libdirs,
                    libraries = libs + gl_libs,

                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )

    wxpExtensions.append(ext)


#----------------------------------------------------------------------
# Define the OGL extension module
#----------------------------------------------------------------------

if not GL_ONLY and BUILD_OGL:
    msg('Preparing OGL...')
    location = 'contrib/ogl'
    OGLLOC = location + '/contrib/src/ogl'
    OGLINC = location + '/contrib/include'

    swig_files = ['ogl.i', 'oglbasic.i', 'oglshapes.i', 'oglshapes2.i',
                  'oglcanvas.i']

    swig_sources = run_swig(swig_files, location, '', PKGDIR,
                            USE_SWIG, swig_force, swig_args)

    # make sure local copy of contrib files are up to date
    if IN_CVS_TREE:
        contrib_copy_tree(WXDIR + '/contrib/include/wx/ogl', OGLINC+'/wx/ogl')
        contrib_copy_tree(WXDIR + '/contrib/src/ogl', OGLLOC)

    ext = Extension('oglc', ['%s/basic.cpp' % OGLLOC,
                             '%s/bmpshape.cpp' % OGLLOC,
                             '%s/composit.cpp' % OGLLOC,
                             '%s/divided.cpp' % OGLLOC,
                             '%s/lines.cpp' % OGLLOC,
                             '%s/misc.cpp' % OGLLOC,
                             '%s/basic2.cpp' % OGLLOC,
                             '%s/canvas.cpp' % OGLLOC,
                             '%s/constrnt.cpp' % OGLLOC,
                             '%s/drawn.cpp' % OGLLOC,
                             '%s/mfutils.cpp' % OGLLOC,
                             '%s/ogldiag.cpp' % OGLLOC,
                             ] + swig_sources,

                    include_dirs =  [OGLINC] + includes,
                    define_macros = defines,

                    library_dirs = libdirs,
                    libraries = libs,

                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )

    wxpExtensions.append(ext)



#----------------------------------------------------------------------
# Define the STC extension module
#----------------------------------------------------------------------

if not GL_ONLY and BUILD_STC:
    msg('Preparing STC...')
    location = 'contrib/stc'
    STCLOC = location + '/contrib/src/stc'
    STCINC = location + '/contrib/include'
    STC_H =  location + '/contrib/include/wx/stc'

    # make sure local copy of contrib files are up to date
    if IN_CVS_TREE:
        contrib_copy_tree(WXDIR + '/contrib/include/wx/stc', STCINC+'/wx/stc')
        contrib_copy_tree(WXDIR + '/contrib/src/stc', STCLOC)


    swig_files = ['stc_.i']
    swig_sources = run_swig(swig_files, location, '', PKGDIR,
                            USE_SWIG, swig_force,
                            swig_args + ['-I'+STC_H, '-I'+location],
                            [STC_H+'/stc.h'])

    # copy a project specific py module to the main package dir
    copy_file(location+'/stc.py', PKGDIR, update=1, verbose=0)

    # add some include dirs to the standard set
    stc_includes = includes[:]
    stc_includes.append('%s/scintilla/include' % STCLOC)
    stc_includes.append('%s/scintilla/src' % STCLOC)
    stc_includes.append(STCINC)

    # and some macro definitions
    stc_defines = defines[:]
    stc_defines.append( ('__WX__', None) )
    stc_defines.append( ('SCI_LEXER', None) )


    ext = Extension('stc_c',
                    ['%s/scintilla/src/AutoComplete.cxx' % STCLOC,
                     '%s/scintilla/src/CallTip.cxx' % STCLOC,
                     '%s/scintilla/src/CellBuffer.cxx' % STCLOC,
                     '%s/scintilla/src/ContractionState.cxx' % STCLOC,
                     '%s/scintilla/src/Document.cxx' % STCLOC,
                     '%s/scintilla/src/Editor.cxx' % STCLOC,
                     '%s/scintilla/src/Indicator.cxx' % STCLOC,
                     '%s/scintilla/src/KeyMap.cxx' % STCLOC,
                     '%s/scintilla/src/KeyWords.cxx' % STCLOC,
                     '%s/scintilla/src/LineMarker.cxx' % STCLOC,
                     '%s/scintilla/src/PropSet.cxx' % STCLOC,
                     '%s/scintilla/src/ScintillaBase.cxx' % STCLOC,
                     '%s/scintilla/src/Style.cxx' % STCLOC,
                     '%s/scintilla/src/ViewStyle.cxx' % STCLOC,
                     '%s/scintilla/src/LexCPP.cxx' % STCLOC,
                     '%s/scintilla/src/LexHTML.cxx' % STCLOC,
                     '%s/scintilla/src/LexLua.cxx' % STCLOC,
                     '%s/scintilla/src/LexOthers.cxx' % STCLOC,
                     '%s/scintilla/src/LexPerl.cxx' % STCLOC,
                     '%s/scintilla/src/LexPython.cxx' % STCLOC,
                     '%s/scintilla/src/LexSQL.cxx' % STCLOC,
                     '%s/scintilla/src/LexVB.cxx' % STCLOC,
                     '%s/scintilla/src/DocumentAccessor.cxx' % STCLOC,
                     '%s/scintilla/src/UniConversion.cxx' % STCLOC,
                     '%s/scintilla/src/WindowAccessor.cxx' % STCLOC,
                     '%s/scintilla/src/PosRegExp.cxx' % STCLOC,

                     '%s/PlatWX.cpp' % STCLOC,
                     '%s/ScintillaWX.cpp' % STCLOC,
                     '%s/stc.cpp' % STCLOC,
                     ] + swig_sources,

                    include_dirs = stc_includes,
                    define_macros = stc_defines,

                    library_dirs = libdirs,
                    libraries = libs,

                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )

    wxpExtensions.append(ext)



#----------------------------------------------------------------------
# Define the IEWIN extension module (experimental)
#----------------------------------------------------------------------

if not GL_ONLY and BUILD_IEWIN:
    msg('Preparing IEWIN...')
    location = 'contrib/iewin'

    swig_files = ['iewin.i', ]

    swig_sources = run_swig(swig_files, location, '', PKGDIR,
                            USE_SWIG, swig_force, swig_args)


    ext = Extension('iewinc', ['%s/IEHtmlWin.cpp' % location,
                             ] + swig_sources,

                    include_dirs =  includes,
                    define_macros = defines,

                    library_dirs = libdirs,
                    libraries = libs,

                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )

    wxpExtensions.append(ext)


#----------------------------------------------------------------------
# Define the XRC extension module
#----------------------------------------------------------------------

if not GL_ONLY and BUILD_XRC:
    msg('Preparing XRC...')
    location = 'contrib/xrc'
    XMLLOC = location + '/contrib/src/xrc'
    XMLINC = location + '/contrib/include'

    swig_files = ['xrc.i']

    swig_sources = run_swig(swig_files, location, '', PKGDIR,
                            USE_SWIG, swig_force, swig_args)

    xmlres_includes = includes[:]
    xmlres_includes.append('%s/expat/xmlparse' % XMLLOC)
    xmlres_includes.append('%s/expat/xmltok' % XMLLOC)
    xmlres_includes.append(XMLINC)


    # make sure local copy of contrib files are up to date
    if IN_CVS_TREE:
        contrib_copy_tree(WXDIR + '/contrib/include/wx/xrc', XMLINC+'/wx/xrc')
        contrib_copy_tree(WXDIR + '/contrib/src/xrc', XMLLOC)

    ext = Extension('xrcc', ['%s/expat/xmlparse/xmlparse.c' % XMLLOC,
                                '%s/expat/xmltok/xmlrole.c' % XMLLOC,
                                '%s/expat/xmltok/xmltok.c' % XMLLOC,

                                '%s/xh_bmp.cpp' % XMLLOC,
                                '%s/xh_bmpbt.cpp' % XMLLOC,
                                '%s/xh_bttn.cpp' % XMLLOC,
                                '%s/xh_cald.cpp' % XMLLOC,
                                '%s/xh_chckb.cpp' % XMLLOC,

                                '%s/xh_chckl.cpp' % XMLLOC,
                                '%s/xh_choic.cpp' % XMLLOC,
                                '%s/xh_combo.cpp' % XMLLOC,
                                '%s/xh_dlg.cpp' % XMLLOC,
                                '%s/xh_frame.cpp' % XMLLOC,

                                '%s/xh_gauge.cpp' % XMLLOC,
                                '%s/xh_html.cpp' % XMLLOC,
                                '%s/xh_listb.cpp' % XMLLOC,
                                '%s/xh_listc.cpp' % XMLLOC,
                                '%s/xh_menu.cpp' % XMLLOC,

                                '%s/xh_notbk.cpp' % XMLLOC,
                                '%s/xh_panel.cpp' % XMLLOC,
                                '%s/xh_radbt.cpp' % XMLLOC,
                                '%s/xh_radbx.cpp' % XMLLOC,
                                '%s/xh_scrol.cpp' % XMLLOC,

                                '%s/xh_sizer.cpp' % XMLLOC,
                                '%s/xh_slidr.cpp' % XMLLOC,
                                '%s/xh_spin.cpp' % XMLLOC,
                                '%s/xh_stbmp.cpp' % XMLLOC,
                                '%s/xh_stbox.cpp' % XMLLOC,

                                '%s/xh_stlin.cpp' % XMLLOC,
                                '%s/xh_sttxt.cpp' % XMLLOC,
                                '%s/xh_text.cpp' % XMLLOC,
                                '%s/xh_toolb.cpp' % XMLLOC,
                                '%s/xh_tree.cpp' % XMLLOC,

                                '%s/xh_unkwn.cpp' % XMLLOC,
                                '%s/xml.cpp' % XMLLOC,
                                '%s/xmlbin.cpp' % XMLLOC,
                                '%s/xmlbinz.cpp' % XMLLOC,
                                '%s/xmlexpat.cpp' % XMLLOC,

                                '%s/xmlres.cpp' % XMLLOC,
                                '%s/xmlrsall.cpp' % XMLLOC,
                                '%s/xmlwrite.cpp' % XMLLOC,

                             ] + swig_sources,

                    include_dirs =  xmlres_includes,
                    define_macros = defines,

                    library_dirs = libdirs,
                    libraries = libs,

                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )

    wxpExtensions.append(ext)




#----------------------------------------------------------------------
# Do the Setup/Build/Install/Whatever
#----------------------------------------------------------------------

if __name__ == "__main__":
    if not GL_ONLY:
        setup(name             = PKGDIR,
              version          = VERSION,
              description      = DESCRIPTION,
              long_description = LONG_DESCRIPTION,
              author           = AUTHOR,
              author_email     = AUTHOR_EMAIL,
              url              = URL,
              licence          = LICENCE,

              packages = [PKGDIR,
                          PKGDIR+'.lib',
                          PKGDIR+'.lib.editor',
                          ],

              ext_package = PKGDIR,
              ext_modules = wxpExtensions,
              )

    else:

        setup(name             = "wxPython-gl",
              version          = VERSION,
              description      = "wxGLCanvas class for wxPython",
              author           = AUTHOR,
              author_email     = AUTHOR_EMAIL,
              url              = URL,
              licence          = LICENCE,

              py_modules = [ "wxPython.glcanvas" ],

              ext_package = PKGDIR,
              ext_modules = wxpExtensions,
              )




#----------------------------------------------------------------------
#----------------------------------------------------------------------

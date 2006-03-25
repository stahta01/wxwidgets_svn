#!/usr/bin/env python
#----------------------------------------------------------------------
# Name:        setup.py
# Purpose:     Distutils script for building wxPython
#
# Author:      Robin Dunn
#
# Created:     12-Oct-2000
# RCS-ID:      $Id$
# Copyright:   (c) 2000 by Total Control Software
# Licence:     wxWindows license
#----------------------------------------------------------------------

import sys, os


# The full contents of the wx.build.config module used to be located
# here in setup.py.  They were split into a separate module so it will
# be installed with wxPython and can then be used by the build scripts
# of other extension modules that wish to be wxPython compatible.
# The split is still fairly new and hasn't been tested by building
# third-party extensions yet, so expect some things to still shift
# back and forth, and also more stuff in config.py will get converted
# to functions, etc.

# This script imports it as just "config" because if wxPython doesn't
# exist yet, then it can't be imported from wx.build.config (since
# wx._core doesn't exist yet.)  So instead we keep the main copy of
# config .py in the same place as setup.py, and then copy it to
# wx/build as needed below.

# To fully support external builds, we need to have a build options
# file that is created whenever a new wxPython build is performed.
# We happen to be doing that here in this script, so make sure to
# remove the build_options.py file, so that config.py will recreate it.

for bo_name in ["build_options.py", "build_options.pyc"]:
    if os.path.exists(bo_name):
        os.remove(bo_name)

sys.setup_is_main =  __name__ == "__main__"  # an icky hack!
from config import *


#----------------------------------------------------------------------
# Update the packaged config file.
#----------------------------------------------------------------------

copy_file('config.py', 'wx/build', update=1, verbose=1)
copy_file('build_options.py', 'wx/build', update=1, verbose=1)
CLEANUP.append('wx/build/config.py')
CLEANUP.append('wx/build/build_options.py')

#----------------------------------------------------------------------
# Update the version file
#----------------------------------------------------------------------

# The version file is unconditionally updated every time setup.py is
# run since the version string can change based on the UNICODE flag

open('wx/__version__.py', 'w').write("""\
# This file was generated by setup.py...

VERSION_STRING  = '%(VERSION)s'
MAJOR_VERSION   = %(VER_MAJOR)s
MINOR_VERSION   = %(VER_MINOR)s
RELEASE_VERSION = %(VER_RELEASE)s
SUBREL_VERSION  = %(VER_SUBREL)s

VERSION = (MAJOR_VERSION, MINOR_VERSION, RELEASE_VERSION,
           SUBREL_VERSION, '%(VER_FLAGS)s')

RELEASE_NUMBER = RELEASE_VERSION  # for compatibility
""" % globals())

CLEANUP.append('wx/__version__.py')


#----------------------------------------------------------------------
# Write the SWIG version to a header file
#----------------------------------------------------------------------

if USE_SWIG:
    try:
        # It may come on either stdout or stderr, depending on the
        # version, so read both.
        i, o, e = os.popen3(SWIG + ' -version', 't')
        stext = o.read() + e.read()
        import re
        match = re.search(r'[0-9]+\.[0-9]+\.[0-9]+$', stext, re.MULTILINE)
        if not match:
            raise 'NotFound'
        SVER = match.group(0)
        open('include/wx/wxPython/swigver.h', 'w').write('''\
// This file was generated by setup.py

#define wxPy_SWIG_VERSION "SWIG-%s"
''' % SVER)
        msg('Using SWIG-' + SVER)

    except:
        msg('\nUnable to get SWIG version number\n')


#----------------------------------------------------------------------
# patch distutils if it can't cope with the "classifiers" or
# "download_url" keywords 
#----------------------------------------------------------------------

if sys.version < '2.2.3': 
    from distutils.dist import DistributionMetadata 
    DistributionMetadata.classifiers = None 
    DistributionMetadata.download_url = None
    depends = {}
else:
    depends = {'depends' : depends}
    

#----------------------------------------------------------------------
# Define the CORE extension module
#----------------------------------------------------------------------

msg('Preparing CORE...')
swig_sources = run_swig(['core.i'], 'src', GENDIR, PKGDIR,
                        USE_SWIG, swig_force, swig_args, swig_deps +
                        [ 'src/_accel.i',
                          'src/_app.i',
                          'src/_app_ex.py',
                          'src/_constraints.i',
                          'src/_core_api.i',
                          'src/_core_ex.py',
                          'src/__core_rename.i',
                          'src/__core_reverse.txt',
                          'src/_defs.i',
                          'src/_event.i',
                          'src/_event_ex.py',
                          'src/_evtloop.i',
                          'src/_evthandler.i',
                          'src/_filesys.i',
                          'src/_gdicmn.i',
                          'src/_image.i',
                          'src/_menu.i',
                          'src/_obj.i',
                          'src/_sizers.i',
                          'src/_gbsizer.i',
                          'src/_streams.i',
                          'src/_validator.i',
                          'src/_window.i',
                          'src/_control.i',
                          ],
                        True)

copy_file('src/__init__.py', PKGDIR, update=1, verbose=0)
CLEANUP.append(opj(PKGDIR, '__init__.py'))


# update the license files
mkpath('licence')
for file in ['preamble.txt', 'licence.txt', 'licendoc.txt', 'lgpl.txt']:
    copy_file(opj(WXDIR, 'docs', file), opj('licence',file), update=1, verbose=0)
    CLEANUP.append(opj('licence',file))
CLEANUP.append('licence')


if os.name == 'nt':
    build_locale_dir(opj(PKGDIR, 'locale'))
    DATA_FILES += build_locale_list(opj(PKGDIR, 'locale'))


if os.name == 'nt':
    rc_file = ['src/wxc.rc']
else:
    rc_file = []


ext = Extension('_core_', ['src/helpers.cpp',
                           ] + rc_file + swig_sources,

                include_dirs = includes,
                define_macros = defines,

                library_dirs = libdirs,
                libraries = libs,

                extra_compile_args = cflags,
                extra_link_args = lflags,

                **depends
                )
wxpExtensions.append(ext)





# Extension for the GDI module
swig_sources = run_swig(['gdi.i'], 'src', GENDIR, PKGDIR,
                        USE_SWIG, swig_force, swig_args, swig_deps +
                        ['src/__gdi_rename.i',
                         'src/_bitmap.i',
                         'src/_colour.i',
                         'src/_dc.i',
                         'src/_gdiobj.i',
                         'src/_imaglist.i',
                         'src/_region.i',
                         'src/_stockobjs.i',
                         'src/_effects.i',
                         'src/_intl.i',
                         'src/_intl_ex.py',
                         'src/_brush.i',
                         'src/_cursor.i',
                         'src/_font.i',
                         'src/_icon.i',
                         'src/_pen.i',
                         'src/_palette.i',
                         'src/_renderer.i',
                         ],
                        True)
ext = Extension('_gdi_', ['src/drawlist.cpp'] + swig_sources,
                include_dirs =  includes,
                define_macros = defines,
                library_dirs = libdirs,
                libraries = libs,
                extra_compile_args = cflags,
                extra_link_args = lflags,
                **depends
                )
wxpExtensions.append(ext)






# Extension for the windows module
swig_sources = run_swig(['windows.i'], 'src', GENDIR, PKGDIR,
                        USE_SWIG, swig_force, swig_args, swig_deps +
                        ['src/__windows_rename.i',
                         'src/__windows_reverse.txt',
                         'src/_panel.i',
                         'src/_toplvl.i',
                         'src/_statusbar.i',
                         'src/_splitter.i',
                         'src/_sashwin.i',
                         'src/_popupwin.i',
                         'src/_tipwin.i',
                         'src/_vscroll.i',
                         'src/_taskbar.i',
                         'src/_cmndlgs.i',
                         'src/_mdi.i',
                         'src/_pywindows.i',
                         'src/_printfw.i',
                         ],
                        True)
ext = Extension('_windows_', swig_sources,
                include_dirs =  includes,
                define_macros = defines,
                library_dirs = libdirs,
                libraries = libs,
                extra_compile_args = cflags,
                extra_link_args = lflags,
                **depends
                )
wxpExtensions.append(ext)




# Extension for the controls module
swig_sources = run_swig(['controls.i'], 'src', GENDIR, PKGDIR,
                        USE_SWIG, swig_force, swig_args, swig_deps +
                        [ 'src/__controls_rename.i',
                          'src/__controls_reverse.txt',
                          'src/_toolbar.i',
                          'src/_button.i',
                          'src/_checkbox.i',
                          'src/_choice.i',
                          'src/_combobox.i',
                          'src/_gauge.i',
                          'src/_statctrls.i',
                          'src/_listbox.i',
                          'src/_textctrl.i',
                          'src/_scrolbar.i',
                          'src/_spin.i',
                          'src/_radio.i',
                          'src/_slider.i',
                          'src/_tglbtn.i',
                          'src/_notebook.i',
                          'src/_listctrl.i',
                          'src/_treectrl.i',
                          'src/_dirctrl.i',
                          'src/_pycontrol.i',
                          'src/_cshelp.i',
                          'src/_dragimg.i',
                          'src/_datectrl.i',
                          ],
                        True)
ext = Extension('_controls_', swig_sources,
                include_dirs =  includes,
                define_macros = defines,
                library_dirs = libdirs,
                libraries = libs,
                extra_compile_args = cflags,
                extra_link_args = lflags,
                **depends
                )
wxpExtensions.append(ext)




# Extension for the misc module
swig_sources = run_swig(['misc.i'], 'src', GENDIR, PKGDIR,
                        USE_SWIG, swig_force, swig_args, swig_deps +
                        [ 'src/__misc_rename.i',
                          'src/__misc_reverse.txt',
                          'src/_settings.i',
                          'src/_functions.i',
                          'src/_misc.i',
                          'src/_tipdlg.i',
                          'src/_timer.i',
                          'src/_log.i',
                          'src/_process.i',
                          'src/_joystick.i',
                          'src/_sound.i',
                          'src/_mimetype.i',
                          'src/_artprov.i',
                          'src/_config.i',
                          'src/_datetime.i',
                          'src/_dataobj.i',
                          'src/_dnd.i',
                          'src/_display.i',
                          'src/_clipbrd.i',
                          'src/_stdpaths.i',
                          ],
                        True)
ext = Extension('_misc_', swig_sources,
                include_dirs =  includes,
                define_macros = defines,
                library_dirs = libdirs,
                libraries = libs,
                extra_compile_args = cflags,
                extra_link_args = lflags,
                **depends
                )
wxpExtensions.append(ext)



##
## Core modules that are not in the "core" namespace start here
##

swig_sources = run_swig(['calendar.i'], 'src', GENDIR, PKGDIR,
                        USE_SWIG, swig_force, swig_args, swig_deps)
ext = Extension('_calendar', swig_sources,
                include_dirs =  includes,
                define_macros = defines,
                library_dirs = libdirs,
                libraries = libs,
                extra_compile_args = cflags,
                extra_link_args = lflags,
                **depends
                )
wxpExtensions.append(ext)


swig_sources = run_swig(['grid.i'], 'src', GENDIR, PKGDIR,
                        USE_SWIG, swig_force, swig_args, swig_deps)
ext = Extension('_grid', swig_sources,
                include_dirs =  includes,
                define_macros = defines,
                library_dirs = libdirs,
                libraries = libs,
                extra_compile_args = cflags,
                extra_link_args = lflags,
                **depends
                )
wxpExtensions.append(ext)



swig_sources = run_swig(['html.i'], 'src', GENDIR, PKGDIR,
                        USE_SWIG, swig_force, swig_args, swig_deps)
ext = Extension('_html', swig_sources,
                include_dirs =  includes,
                define_macros = defines,
                library_dirs = libdirs,
                libraries = libs,
                extra_compile_args = cflags,
                extra_link_args = lflags,
                **depends
                )
wxpExtensions.append(ext)


mediaLibs = libs[:]
if not MONOLITHIC and os.name == 'nt':
    mediaLibs.append(makeLibName('media')[0])
swig_sources = run_swig(['media.i'], 'src', GENDIR, PKGDIR,
                        USE_SWIG, swig_force, swig_args, swig_deps)
ext = Extension('_media', swig_sources,
                include_dirs =  includes,
                define_macros = defines,
                library_dirs = libdirs,
                libraries = mediaLibs,
                extra_compile_args = cflags,
                extra_link_args = lflags,
                **depends
                )
wxpExtensions.append(ext)


swig_sources = run_swig(['webkit.i'], 'src', GENDIR, PKGDIR,
                        USE_SWIG, swig_force, swig_args, swig_deps)
ext = Extension('_webkit', swig_sources,
                include_dirs =  includes,
                define_macros = defines,
                library_dirs = libdirs,
                libraries = libs,
                extra_compile_args = cflags,
                extra_link_args = lflags,
                **depends
                )
wxpExtensions.append(ext)



swig_sources = run_swig(['wizard.i'], 'src', GENDIR, PKGDIR,
                        USE_SWIG, swig_force, swig_args, swig_deps)
ext = Extension('_wizard', swig_sources,
                include_dirs =  includes,
                define_macros = defines,
                library_dirs = libdirs,
                libraries = libs,
                extra_compile_args = cflags,
                extra_link_args = lflags,
                **depends
                )
wxpExtensions.append(ext)



swig_sources = run_swig(['xrc.i'], 'src', GENDIR, PKGDIR,
                        USE_SWIG, swig_force, swig_args, swig_deps +
                        [ 'src/_xrc_rename.i',
                          'src/_xrc_ex.py',
                          'src/_xmlres.i',
                          'src/_xmlsub.i',
                          'src/_xml.i',
                          'src/_xmlhandler.i',
                          ])
ext = Extension('_xrc',
                swig_sources,

                include_dirs =  includes + CONTRIBS_INC,
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

if BUILD_GLCANVAS:
    msg('Preparing GLCANVAS...')
    location = 'contrib/glcanvas'

    swig_sources = run_swig(['glcanvas.i'], location, GENDIR, PKGDIR,
                            USE_SWIG, swig_force, swig_args, swig_deps)

    gl_libs = []
    if os.name == 'posix':
        gl_config = os.popen(WX_CONFIG + ' --libs gl', 'r').read()[:-1]
        gl_lflags = gl_config.split()
        gl_libs = libs
        
    else:
        gl_libs = libs + ['opengl32', 'glu32'] + makeLibName('gl')
        gl_lflags = lflags

    ext = Extension('_glcanvas',
                    swig_sources,

                    include_dirs = includes + CONTRIBS_INC,
                    define_macros = defines,

                    library_dirs = libdirs,
                    libraries = gl_libs,

                    extra_compile_args = cflags,
                    extra_link_args = gl_lflags,
                    )

    wxpExtensions.append(ext)


#----------------------------------------------------------------------
# Define the OGL extension module
#----------------------------------------------------------------------

if BUILD_OGL:
    msg('Preparing OGL...')
    location = 'contrib/ogl'

    swig_sources = run_swig(['ogl.i'], location, GENDIR, PKGDIR,
                            USE_SWIG, swig_force, swig_args, swig_deps +
                            [ '%s/_oglbasic.i' % location,
                              '%s/_oglshapes.i' % location,
                              '%s/_oglshapes2.i' % location,
                              '%s/_oglcanvas.i' % location,
                              '%s/_ogldefs.i' % location,
                              ])

    ext = Extension('_ogl',
                    swig_sources,

                    include_dirs =  includes + [ location ] + CONTRIBS_INC,
                    define_macros = defines + [('wxUSE_DEPRECATED', '0')],

                    library_dirs = libdirs,
                    libraries = libs + makeLibName('ogl'),

                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )

    wxpExtensions.append(ext)



#----------------------------------------------------------------------
# Define the STC extension module
#----------------------------------------------------------------------

if BUILD_STC:
    msg('Preparing STC...')
    location = 'contrib/stc'
    #if os.name == 'nt':
    STC_H = opj(WXDIR, 'contrib', 'include/wx/stc')
    #else:
    #    STC_H = opj(WXPREFIX, 'include/wx-%d.%d/wx/stc' % (VER_MAJOR, VER_MINOR))

## NOTE: need to add something like this to the stc.bkl...

##         # Check if gen_iface needs to be run for the wxSTC sources
##         if (newer(opj(CTRB_SRC, 'stc/stc.h.in'),     opj(CTRB_INC, 'stc/stc.h'  )) or
##             newer(opj(CTRB_SRC, 'stc/stc.cpp.in'),   opj(CTRB_SRC, 'stc/stc.cpp')) or
##             newer(opj(CTRB_SRC, 'stc/gen_iface.py'), opj(CTRB_SRC, 'stc/stc.cpp'))):

##             msg('Running gen_iface.py, regenerating stc.h and stc.cpp...')
##             cwd = os.getcwd()
##             os.chdir(opj(CTRB_SRC, 'stc'))
##             sys.path.insert(0, os.curdir)
##             import gen_iface
##             gen_iface.main([])
##             os.chdir(cwd)


    swig_sources = run_swig(['stc.i'], location, GENDIR, PKGDIR,
                            USE_SWIG, swig_force,
                            swig_args + ['-I'+STC_H, '-I'+location],
                            [opj(STC_H, 'stc.h'),
                             opj(location, "_stc_utf8_methods.py"),
                             opj(location, "_stc_docstrings.i"),
                             opj(location, "_stc_gendocs.i"),
                             ] + swig_deps)

    ext = Extension('_stc',
                    swig_sources,

                    include_dirs = includes + CONTRIBS_INC,
                    define_macros = defines,

                    library_dirs = libdirs,
                    libraries = libs + makeLibName('stc'),

                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )

    wxpExtensions.append(ext)



#----------------------------------------------------------------------
# Define the IEWIN extension module (experimental)
#----------------------------------------------------------------------

if BUILD_IEWIN:
    msg('Preparing IEWIN...')
    location = 'contrib/iewin'

    swig_files = ['iewin.i', ]

    swig_sources = run_swig(swig_files, location, '', PKGDIR,
                            USE_SWIG, swig_force, swig_args, swig_deps)


    ext = Extension('_iewin', ['%s/IEHtmlWin.cpp' % location,
                               '%s/wxactivex.cpp' % location,
                             ] + swig_sources,

                    include_dirs =  includes + CONTRIBS_INC,
                    define_macros = defines,

                    library_dirs = libdirs,
                    libraries = libs,

                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )

    wxpExtensions.append(ext)


#----------------------------------------------------------------------
# Define the ACTIVEX extension module (experimental)
#----------------------------------------------------------------------

if BUILD_ACTIVEX:
    msg('Preparing ACTIVEX...')
    location = 'contrib/activex'
    axloc = opj(location, "wxie")

    swig_files = ['activex.i', ]

    swig_sources = run_swig(swig_files, location, '', PKGDIR,
                            USE_SWIG, swig_force, swig_args, swig_deps +
                            [ '%s/_activex_ex.py' % location])


    ext = Extension('_activex', ['%s/IEHtmlWin.cpp' % axloc,
                                 '%s/wxactivex.cpp' % axloc,
                                 ] + swig_sources,

                    include_dirs =  includes + [ axloc ],
                    define_macros = defines,

                    library_dirs = libdirs,
                    libraries = libs,

                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )

    wxpExtensions.append(ext)


#----------------------------------------------------------------------
# Define the GIZMOS  extension module
#----------------------------------------------------------------------

if BUILD_GIZMOS:
    msg('Preparing GIZMOS...')
    location = 'contrib/gizmos'

    swig_sources = run_swig(['gizmos.i'], location, GENDIR, PKGDIR,
                            USE_SWIG, swig_force, swig_args, swig_deps)

    ext = Extension('_gizmos',
                    [ '%s/treelistctrl.cpp' % opj(location, 'wxCode/src') ] + swig_sources,

                    include_dirs =  includes + [ location, opj(location, 'wxCode/include') ] + CONTRIBS_INC,
                    define_macros = defines,

                    library_dirs = libdirs,
                    libraries = libs + makeLibName('gizmos'),

                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )

    wxpExtensions.append(ext)


#----------------------------------------------------------------------
# Define the ANIMATE  extension module
#----------------------------------------------------------------------

if BUILD_ANIMATE:
    msg('Preparing ANIMATE...')
    location = 'contrib/animate'

    swig_sources = run_swig(['animate.i'], location, GENDIR, PKGDIR,
                            USE_SWIG, swig_force, swig_args, swig_deps)

    ext = Extension('_animate',
                    swig_sources,

                    include_dirs =  includes + CONTRIBS_INC,
                    define_macros = defines,

                    library_dirs = libdirs,
                    libraries = libs + makeLibName('animate'),

                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )

    wxpExtensions.append(ext)



#----------------------------------------------------------------------
# Define the DLLWIDGET  extension module
#----------------------------------------------------------------------

if BUILD_DLLWIDGET:
    msg('Preparing DLLWIDGET...')
    location = 'contrib/dllwidget'
    swig_files = ['dllwidget_.i']

    swig_sources = run_swig(swig_files, location, '', PKGDIR,
                            USE_SWIG, swig_force, swig_args, swig_deps)

    # copy a contrib project specific py module to the main package dir
    copy_file(opj(location, 'dllwidget.py'), PKGDIR, update=1, verbose=0)
    CLEANUP.append(opj(PKGDIR, 'dllwidget.py'))

    ext = Extension('dllwidget_c', [
                                '%s/dllwidget.cpp' % location,
                             ] + swig_sources,

                    include_dirs =  includes + CONTRIBS_INC,
                    define_macros = defines,

                    library_dirs = libdirs,
                    libraries = libs,

                    extra_compile_args = cflags,
                    extra_link_args = lflags,
                    )

    wxpExtensions.append(ext)



    
#----------------------------------------------------------------------
# Tools, scripts data files, etc.
#----------------------------------------------------------------------

if NO_SCRIPTS:
    SCRIPTS = None
else:
    SCRIPTS = [opj('scripts/helpviewer'),
               opj('scripts/img2png'),
               opj('scripts/img2py'),
               opj('scripts/img2xpm'),
               opj('scripts/pyalacarte'),
               opj('scripts/pyalamode'),
               opj('scripts/pycrust'),
               opj('scripts/pyshell'),
               opj('scripts/pywrap'),
               opj('scripts/pywrap'),
               opj('scripts/pywxrc'),
               opj('scripts/xrced'),
               ]



DATA_FILES += find_data_files('wx/tools/XRCed', '*.txt', '*.xrc')
DATA_FILES += find_data_files('wx/py', '*.txt', '*.ico', '*.css', '*.html')
DATA_FILES += find_data_files('wx', '*.txt', '*.css', '*.html')


if NO_HEADERS:
    HEADERS = None
else:
    h_files = glob.glob(opj("include/wx/wxPython/*.h"))
    i_files = glob.glob(opj("src/*.i"))   + \
              glob.glob(opj("src/_*.py")) + \
              glob.glob(opj("src/*.swg"))

    HEADERS = zip(h_files, ["/wxPython"]*len(h_files)) + \
              zip(i_files, ["/wxPython/i_files"]*len(i_files))



if INSTALL_MULTIVERSION:
    EXTRA_PATH = getExtraPath(addOpts=EP_ADD_OPTS, shortVer=not EP_FULL_VER)
    open("src/wx.pth", "w").write(EXTRA_PATH)
    CLEANUP.append("src/wx.pth")
else:
    EXTRA_PATH = None
       


#----------------------------------------------------------------------
# Do the Setup/Build/Install/Whatever
#----------------------------------------------------------------------

if __name__ == "__main__":
    if not PREP_ONLY:
        
        setup(name             = 'wxPython',
              version          = VERSION,
              description      = DESCRIPTION,
              long_description = LONG_DESCRIPTION,
              author           = AUTHOR,
              author_email     = AUTHOR_EMAIL,
              url              = URL,
              download_url     = DOWNLOAD_URL,
              license          = LICENSE,
              platforms        = PLATFORMS,
              classifiers      = filter(None, CLASSIFIERS.split("\n")),
              keywords         = KEYWORDS,

              packages = ['wxPython',
                          'wxPython.lib',
                          'wxPython.lib.colourchooser',
                          'wxPython.lib.editor',
                          'wxPython.lib.mixins',
                          'wxPython.tools',

                          'wx',
                          'wx.build',
                          'wx.lib',
                          'wx.lib.colourchooser',
                          'wx.lib.editor',
                          'wx.lib.floatcanvas',
                          'wx.lib.masked',
                          'wx.lib.mixins',
                          'wx.lib.ogl',
                          'wx.py',
                          'wx.tools',
                          'wx.tools.XRCed',
                          ],

              extra_path = EXTRA_PATH,

              ext_package = PKGDIR,
              ext_modules = wxpExtensions,

              options = { 'build'            : { 'build_base' : BUILD_BASE },
                          },

              scripts =    SCRIPTS,
              data_files = DATA_FILES,
              headers =    HEADERS,

              # Override some of the default distutils command classes with my own
              cmdclass = { 'install' :        wx_install,
                           'install_data':    wx_smart_install_data,
                           'install_headers': wx_install_headers,
                           'clean':           wx_extra_clean,
                           },
              )


        if INSTALL_MULTIVERSION:
            setup(name             = 'wxPython-common',
                  version          = VERSION,
                  description      = DESCRIPTION,
                  long_description = LONG_DESCRIPTION,
                  author           = AUTHOR,
                  author_email     = AUTHOR_EMAIL,
                  url              = URL,
                  download_url     = DOWNLOAD_URL,
                  license          = LICENSE,
                  platforms        = PLATFORMS,
                  classifiers      = filter(None, CLASSIFIERS.split("\n")),
                  keywords         = KEYWORDS,

                  package_dir = { '': 'wxversion' },
                  py_modules = ['wxversion'],

                  data_files = [('', ['src/wx.pth'])],
                  
                  options = { 'build'            : { 'build_base' : BUILD_BASE },
                              },
                  
                  cmdclass = { 'install_data':    wx_smart_install_data,
                               },
                  )
            
#----------------------------------------------------------------------
#----------------------------------------------------------------------

wxWindows 2.1
-------------

Welcome to wxWindows 2, a sophisticated cross-platform C++
framework for writing advanced GUI applications using (where
possible) the native controls.

In addition to common and advanced GUI facilities such as frames, scrolling
windows, toolbars, tree controls, icons, device contexts, printing,
splitter windows and so on, there are wrappers for common file operations,
and facilities for writing TCP/IP applications, thread handling, and more.
Where certain features are not available on a platform, such as MDI and
tree controls on Unix, they are emulated.

A detailed 1100-page reference manual is supplied in HTML, PDF and
Windows Help form: see the docs hierarchy.

For a quick start, point your Web browser at docs/html/index.htm for a list of
important documents and samples.

Platforms supported
-------------------

wxWindows 2 currently supports the following platforms:

- Windows 3.1, Windows 95/98, Windows NT
- Most Unix variants with Motif/Lesstif
- Most Unix variants with GTK+

Most popular C++ compilers are supported; see the release notes
(available via docs/html/index.htm) for details.

Files
-----

The distribution is available in a number of formats.
The most convenient is the platform-specific distribution,
but the zip set forms a more complete distribution, for example if
you wish to take part in wxWindows development. Also, add-ons
such as OGL may only be available in zip form.

Zip set
-------

Depending on what you downloaded, you may have one or more of
these ports. You may unarchive any or all of the ports into
the same directory hierarchy. The zip archive set comprises the
following:

wx200gen.zip            Generic source code and samples (required)
wx200msw.zip            Windows-specific source code
wx200mot.zip            Motif-specific source code
wx200gtk.zip            GTK-specific source code
wx200stubs.zip          Stubs ('empty port') source. Needs
                        wx200gen.zip/tgz.
wx200doc.zip            Documentation source code (not required)
wx200hlp.zip            WinHelp documentation
wx200pdf.zip            Acrobat PDF documentation
wx200htm.zip            HTML documentation
wx200vc.zip             MS VC++ 5/6 project files
wx200bc.zip             Borland C++ 5 project files
wx200cw.zip             Metrowerks CodeWarrior 4.1 project files
jpeg.zip                Optional JPEG library
ogl3.zip                Optional Object Graphics Library
glcanvas.zip            Optional wxGLCanvas class (Motif, GTK, MSW)
tex2rtf2.zip            Tex2RTF documentation tool

wxWindows for GTK distribution
------------------------------

TODO

wxWindows for Motif distribution
--------------------------------

TODO

wxWindows for Windows distribution
----------------------------------

setup.exe               Setup file (Windows 95/98, NT)
setup.*                 Other setup files

Installation
------------

wxWindows 2 needs to be compiled before you can test out
the samples or write your own applications.
For installation information, please see the install.txt file
in the individual directories:

  docs/msw
  docs/gtk
  docs/motif

Licence information
-------------------

For licensing information, please see the files:

  docs/preamble.txt
  docs/licence.txt
  docs/licendoc.txt
  docs/gpl.txt
  docs/lgpl.txt

Although this may seem complex, it is there to allow authors of
proprietary/commercial applications to use wxWindows in
addition to those writing GPL'ed applications. In summary,
the licence is L-GPL plus a clause allowing unrestricted
distribution of application binaries. To answer a FAQ, you
don't have to distribute any source if you wish to write
commercial applications using wxWindows.

Documentation
-------------

See docs/changes.txt for a history of changes to wxWindows 2.
See docs/html/index.htm for an HTML index of the major documents.

The Windows help files are located in docs/winhelp.
The PDF help files are located in docs/pdf.

Further information
-------------------

The wxWindows Web site is located at:

  http://www.wxwindows.org

The wxGTK Web site (with further wxGTK-specific files and
information) is located at:

  http://www.freiburg.linux.de/~wxxt

The main wxWindows ftp site is at:

  ftp://www.remstar.com/pub/wxwin

A wxWindows CD-ROM with the latest distribution plus an HTML
front-end and hundreds of MB of compilers, utilities and other
material may be ordered from the CD-ROM page (see the wxWindows
web site) or contact Julian Smart <julian.smart@ukonline.co.uk>.

Good luck!

The wxWindows Team, November 1999


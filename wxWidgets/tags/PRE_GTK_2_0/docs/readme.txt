wxWindows 2.3.2
---------------

*** Please note that this is an UNSTABLE DEVELOPMENT SNAPSHOT.
*** Unless you need the new features and bug fixes, you may wish to
*** use the official 2.2.x stable series.

Welcome to wxWindows 2, a sophisticated cross-platform C++
framework for writing advanced GUI applications using (where
possible) the native controls.

In addition to common and advanced GUI facilities such as
frames, scrolling windows, toolbars, tree controls, icons,
device contexts, printing, splitter windows and so on, there are
wrappers for common file operations, and facilities for writing
TCP/IP applications, thread handling, and more. Where certain
features are not available on a platform, such as MDI and tree
controls on Unix, they are emulated.

A detailed 1700-page reference manual is supplied in HTML, PDF
and Windows Help form: see the docs hierarchy.

For a quick start, point your Web browser at docs/html/index.htm
for a list of important documents and samples.

Changes in this release
-----------------------

Please see changes.txt for details.

Platforms supported
-------------------

wxWindows 2 currently supports the following platforms:

- Windows 95/98/ME, Windows NT, Windows 2000, Windows XP (see msw/winxp.txt)
- Most Unix variants with Motif/Lesstif
- Most Unix variants with GTK+
- MacOS

Most popular C++ compilers are supported; see the install.txt
file for each platform (available via docs/html/index.htm) for details.
See also http://www.wxwindows.org/platform.htm.

Files
-----

The distribution is available in archive formats appropriate to the
target system. Documentation is available mainly in zip format.
Some add-on libraries (such as the Object Graphics Library) are
available in zip form only. In the following, x.y.z represents
the current version number.

wxWindows for GTK distribution
------------------------------

wxGTK-x.y.z.tgz                    wxGTK source distribution. You will
                                   need the HTML, HTB and/or PDF documentation
                                   (see below)
wxGTK-demos-x.y.z.tgz              wxGTK demos source
wxGTK-samples-x.y.z.tgz            wxGTK samples source
wxGTK-x.y.z-0.src.rpm              wxGTK Linux source as an RPM, without manuals
wxGTK-x.y.z-0.i386.rpm             wxGTK Linux binaries as an RPM, without manuals
wxGTK-devel-x.y.z-0.i386.rpm       wxGTK Linux minimum development system as an RPM

wxWindows for Motif distribution
--------------------------------

wxMotif-x.y.z.tgz                  wxMotif source distribution. Contains
                                   TIFF, JPEG, Tex2RTF source plus HTML
                                   documentation.

wxWindows for MS Windows distribution
-------------------------------------

setup.exe, setup.w*                Setup files (Windows 95/98, NT)
                                   in floppy-disk-sized chunks
wxMSW-x.y.z-setup.zip              Zip archive containing the
                                   setup files
wxMSW-x.y.z.zip                    Zip archive containing all the
                                   files that are in the setup
                                   distribution

wxWindows for MacOS distribution
--------------------------------

wxMac-x.y.z.zip                    Zip archive containing all
                                   source files (excludes documentation)

As well as the core source, the wxMSW distribution contains:

- Windows HTML Help versions of the documentation (docs/htmlhelp);
- Object Graphics Library, in contrib/src/ogl;
- a Dialog Editor binary;
- a Tex2RTF binary;
- Life! sample binary;
- the JPEG library source;
- the TIFF library source.

Documentation files
-------------------

wxWindows-x.y.z-WinHelp.zip        WinHelp documentation
wxWindows-x.y.z-PDF.zip            Acrobat PDF documentation
wxWindows-x.y.z-HTML.zip           HTML documentation
wxWindows-x.y.z-HTMLHelp.zip       Windows HTML Help documentation
wxWindows-x.y.z-HTB.zip            wxHTML documentation (for
                                   use with the helpview utility)
wxWindows-x.y.z-Word.zip           MS Word documentation (currently,
                                   database class documentation only)
wxWindows-x.y.z-DocSource.zip      Documentation source code (not required)

Add-ons
-------

wxWindows-x.y.z-cw.zip             Metrowerks CodeWarrior 4.1 project files
wxWindows-x.y.z-wat.zip            Watcom C++ project files
wxWindows-x.y.z-vc.zip             Visual C++ project files (in wxMSW distr.)
wxWindows-x.y.z-bc.zip             Borland C++ project files (in wxMSW distr.)
jpeg.zip                           JPEG library (already included
                                   in most distributions)
tiff.zip                           TIFF library (already included
                                   in most distributions)
ogl3.zip                           Optional Object Graphics Library
mmedia.zip                         Optional MMedia library (Motif, GTK, MSW)
stc.zip                            Optional wxStyledTextCtrl library
                                   (Motif, GTK, MSW)
tex2rtf2.zip                       Tex2RTF documentation tool
wxGTK-gl-x.y.z-0.i386.rpm          add-on OpenGL binary as an RPM

Installation
------------

wxWindows 2 needs to be compiled before you can test out
the samples or write your own applications.
For installation information, please see the install.txt file
in the individual directories:

  docs/msw
  docs/gtk
  docs/motif
  docs/mac

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
the licence is LGPL plus a clause allowing unrestricted
distribution of application binaries. To answer a FAQ, you
don't have to distribute any source if you wish to write
commercial applications using wxWindows. 

However, if you distribute wxGTK or wxMotif (with Lesstif) version
of your application, don't forget that it is linked against
GTK+ (or Lesstif) which is covered by LGPL *without* exception
notice. Under Linux systems your app is probably linked
against LGPL glibc as well. Please read carefuly LGPL, section 6.
which describes conditions for distribution of closed source applications
linked against LGPL library. Basically you should link dynamicaly and
include source code of LGPL libraries with your product (unless it is
already present in user's system - like glibc usually is).
If compiled with --enable-odbc (Unix only), wxWindows library will 
contain iODBC library which is covered by LGPL.

If you use TIFF image handler, please see src/tiff/COPYRIGHT
for libtiff license details.

If you use JPEG image handler, documentation for your program should
contain following sentence: "This software is based in part on the work of
the Independent JPEG Group". See src/jpeg/README for details.

If you use wxRegEx class on a system without native regular expressions
support (i.e. MS Windows), see src/regex/COPYRIGHT file for Henry Spencer's
regular expression library copyright.

Documentation
-------------

See docs/html/index.htm for an HTML index of the major documents.

See docs/changes.txt for a summary of changes to wxWindows 2.

See docs/tech for an archive of technical notes.

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
material may be ordered from the CD-ROM page: see the wxWindows
web site.

Have fun!

The wxWindows Team, 10th June, 2001


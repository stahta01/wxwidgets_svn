
   Welcome to wxWindows/Gtk 2.4

you have downloaded version 2.4 of the GTK port of the 
wxWindows GUI library.

wxWindows no longer supports GTK 1.0 (as did some early
snapshots) so that you will need GTK 1.2 when using it.
GTK 1.2.6 or above is recommended although some programs 
will work with GTK 1.2.3 onwards. There is now support
for GTK 2.0.

More info about the wxWindows project (and the Windows
and Motif ports in particular) can be found at Julian's 
homepage at:

  http://www.wxwindows.org
  
Information on how to install can be found in the file 
INSTALL.txt, but if you cannot wait, this should work on
many systems:

./configure --with-gtk
make
su <type root password>
make install
ldconfig
exit

In order to use wxGTK with GTK 2.0 and its unicode
support, you need to configure

./configure --with-gtk --enable-gtk2 --enable-unicode
etc.

So far, support for GTK 2.0 has only been tested on
Linux with glibc 2.2.

When you run into problems, please read the INSTALL.txt and 
follow those instructions. If you still don't have any success,
please send a bug report to one of our mailing list, INCLUDING 
A DESCRIPTION OF YOUR SYSTEM AND YOUR PROBLEM, SUCH AS YOUR 
VERSION OF GTK, WXGTK, WHAT DISTRIBUTION YOU USE AND WHAT ERROR 
WAS REPORTED. I know this has no effect, but I tried...

The library produced by the install process will be called 
libwx_gtk.a (static) and libwx_gtk-2.2.so.0.0.0 (shared) so 
that once a binary incompatible version of wxWindows/Gtk comes 
out we'll augment the library version number to avoid linking 
problems.

Please send problems concerning installation, feature requests, 
bug reports or comments to the wxWindows users list. Information 
on how to subscribe is available from my homepage.

Do NOT send any comments directly to me.

wxWindows/Gtk doesn't come with any guarantee whatsoever. It 
might crash your harddisk or destroy your monitor. It doesn't 
claim to be suitable for any special or general purpose.

  Regards,

    Robert Roebling




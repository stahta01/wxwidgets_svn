README for wxPythonSrc-*.tar.gz
-------------------------------

Prior to version 2.3.3 of wxPython I had always made my Linux/Unix
binaries based on the released binary of wxGTK and wxGTK-gl.  This
imposed a few restrictions and so starting with 2.3.3 I have decided
to do a combined binary that inlcudes wxGTK as well as wxPython.  This
allows me a bit more flexibility and is consistent with how the
Windows and Mac OS X binaries are built.

If you are reading this file then you are probably interested in
building your own copy of wxPython from the sources contained in this
archive.  If you wish to use the released wxGTK binary as has been
done in the past then you can still follow the old build directions in
wxPython/BUILD.unix.txt.  If you are building for Windows or Mac OS X
then you should look at wxPython/BUILD.win32.txt or
wxPython/BUILD.osx.txt respectivly.

If, on the other hand, you would like to build Linux/Unix binaries
with a private copy of wxGTK like what I am now distributing then
you'll want to follow the instructions in this file.  (You should
probably still read wxPython/BUILD.unix.txt though since there are
other details there that you may need to be aware of.

Clear as mud?  Good.  Let's get started.


1. We'll be making a private copy of wxGTK so it doesn't conflict with
   one used by wxGTK C++ apps that expect to have the default binary
   installed from RPM or whatever.  I put it in /usr/lib/wxPython, but
   you can use whatever you like.  I'll just set a variable to our wx
   prefix to reference later:

        export WXPREF=/usr/lib/wxPython


2. Make a build directory and configure wxGTK.

        cd wxPythonSrc-2.4.0   # or whatever the top-level dir is
        mkdir build
        cd build
        ../configure --with-gtk \
                     --prefix=$WXPREF \
                     --enable-rpath=$WXPREF/lib \
                     --with-opengl \
		     --enable-geometry \
                     --enable-optimise \
                     --enable-debug_flag \

   You may want to use --enable-debug instead of --enable-optimise if
   you need to run though a debugger and want full debugging symbols.

   SOLARIS NOTE: The --enable-rpath option may cause problems when
   using wxGTK on Solaris when compiling wxPython in step 4 below.
   The woraround is to not use --enable-rpath flag for configure, but
   in that case all wxPython applications must have the
   LD_LIBRARY_PATH set to include $WXPREF/lib, or you can use the
   'crle' program to modify the runtime linking environment.  If this
   is the only installation of wxGTK on the system then you can use a
   system library path for WXPREF and not have to worry about it at
   all.

   If you want to use the image and zlib libraries included with
   wxWindows instead of those already installed on your system, (for
   example, to reduce dependencies on 3rd party libraries) then you
   can add these flags to the configure command:

                     --with-libjpeg=builtin \
                     --with-libpng=builtin \
                     --with-libtiff=builtin \
                     --with-zlib=builtin \

   If you would like to use GTK 2.x and unicode, then add the
   following flags.  Please note that this is still considered beta,
   but does look and work quite nice for the most part:

		     --enable-gtk2 \
		     --enable-unicode \


3. Build and install wxGTK.  (You may need to be root for the last
   step, depending on where your WXPREF is.)

        make
        make install


4. Build and install wxPython.  If you want to use a different version
   of Python than is found by default on the PATH then specify the
   whole pathname in these steps.  The version of Python that runs
   setup.py is the version wxPython will be built and installed for.
   (You will need to be root for the install step unless your Python
   is not in a system location.)

        cd ../wxPython
        python setup.py \
               WX_CONFIG=$WXPREF/bin/wx-config \
               build install

   If you are using GTK 2.x and unicode then do it this way instead:

        python setup.py \
               WX_CONFIG=$WXPREF/bin/wx-config \
	       WXPORT=gtk2 UNICODE=1 \
               build install

   If you get errors about wxGLCanvas or being unable to find libGLU
   or something like that then you can add BUILD_GLCANVAS=0 to the
   setup.py command line to disable the building of the glcanvas
   module.

   If you would like to install to some place besides the Python
   site-packages directory (such as to your home directory) then you
   can add "--root=<path>" after the "install" command.  To use
   wxPython like this you'll need to ensure that the directory
   containing wxPython is contained in the PYTHONPATH environment
   variable.


5. If you havn't already, get a new copy of the demo and documentation
   to go with the wxPython you just built and installed.  See
   http://wxpython.org/download.php#documentation


6. Change to the demo directory and run it like this:

        python demo.py

   SOLARIS NOTE: If you get unresolved symbol errors when importing
   wxPython and you are running on Solaris and building with gcc, then
   you may be able to work around the problem by uncommenting a bit of
   code in setup.py and building again.  Look for 'SunOS' in setup.py
   and uncomment the block containing it.  The problem is that Sun's ld
   does not automatically add libgcc to the link step.

7. That's all, except for the having fun part!



--
Robin Dunn
Software Craftsman
http://wxPython.org  Java give you jitters?  Relax with wxPython!


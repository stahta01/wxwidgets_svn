dnl aclocal.m4 generated automatically by aclocal 1.4

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

dnl ---------------------------------------------------------------------------
dnl
dnl Macros for configure.in for wxWindows by Robert Roebling, Phil Blecker,
dnl Vadim Zeitlin and Ron Lee
dnl
dnl This script is under the wxWindows licence.
dnl
dnl Version: $Id$
dnl ---------------------------------------------------------------------------

dnl ===========================================================================
dnl macros to find the a file in the list of include/lib paths
dnl ===========================================================================

dnl ---------------------------------------------------------------------------
dnl call WX_PATH_FIND_INCLUDES(search path, header name), sets ac_find_includes
dnl to the full name of the file that was found or leaves it empty if not found
dnl ---------------------------------------------------------------------------
AC_DEFUN([WX_PATH_FIND_INCLUDES],
[
ac_find_includes=
for ac_dir in $1;
  do
    if test -f "$ac_dir/$2"; then
      ac_find_includes=$ac_dir
      break
    fi
  done
])

dnl ---------------------------------------------------------------------------
dnl call WX_PATH_FIND_LIBRARIES(search path, header name), sets ac_find_libraries
dnl to the full name of the file that was found or leaves it empty if not found
dnl ---------------------------------------------------------------------------
AC_DEFUN([WX_PATH_FIND_LIBRARIES],
[
ac_find_libraries=
for ac_dir in $1;
  do
    for ac_extension in a so sl dylib; do
      if test -f "$ac_dir/lib$2.$ac_extension"; then
        ac_find_libraries=$ac_dir
        break 2
      fi
    done
  done
])

dnl ---------------------------------------------------------------------------
dnl Path to include, already defined
dnl ---------------------------------------------------------------------------
AC_DEFUN([WX_INCLUDE_PATH_EXIST],
[
  ac_path_to_include=$1
  echo "$2" | grep "\-I$1" > /dev/null
  result=$?
  if test $result = 0; then
    ac_path_to_include=""
  else
    ac_path_to_include=" -I$1"
  fi
])

dnl ---------------------------------------------------------------------------
dnl Path to link, already defined
dnl ---------------------------------------------------------------------------
AC_DEFUN([WX_LINK_PATH_EXIST],
[
  echo "$2" | grep "\-L$1" > /dev/null
  result=$?
  if test $result = 0; then
    ac_path_to_link=""
  else
    ac_path_to_link=" -L$1"
  fi
])

dnl ===========================================================================
dnl C++ features test
dnl ===========================================================================

dnl ---------------------------------------------------------------------------
dnl WX_CPP_NEW_HEADERS checks whether the compiler has "new" <iostream> header
dnl or only the old <iostream.h> one - it may be generally assumed that if
dnl <iostream> exists, the other "new" headers (without .h) exist too.
dnl
dnl call WX_CPP_NEW_HEADERS(actiof-if-true, action-if-false-or-cross-compiling)
dnl ---------------------------------------------------------------------------

AC_DEFUN([WX_CPP_NEW_HEADERS],
[
  if test "$cross_compiling" = "yes"; then
    ifelse([$2], , :, [$2])
  else
    AC_LANG_SAVE
    AC_LANG_CPLUSPLUS

    AC_CHECK_HEADERS(iostream)

    if test "$ac_cv_header_iostream" = "yes" ; then
      ifelse([$1], , :, [$1])
    else
      ifelse([$2], , :, [$2])
    fi

    AC_LANG_RESTORE
  fi
])

dnl ---------------------------------------------------------------------------
dnl WX_CPP_BOOL checks whether the C++ compiler has a built in bool type
dnl
dnl call WX_CPP_BOOL - will define HAVE_BOOL if the compiler supports bool
dnl ---------------------------------------------------------------------------

AC_DEFUN([WX_CPP_BOOL],
[
  AC_CACHE_CHECK([if C++ compiler supports bool], wx_cv_cpp_bool,
  [
    AC_LANG_SAVE
    AC_LANG_CPLUSPLUS

    AC_TRY_COMPILE(
      [
      ],
      [
        bool b = true;

        return 0;
      ],
      [
        wx_cv_cpp_bool=yes
      ],
      [
        wx_cv_cpp_bool=no
      ]
    )

    AC_LANG_RESTORE
  ])

  if test "$wx_cv_cpp_bool" = "yes"; then
    AC_DEFINE(HAVE_BOOL)
  fi
])

dnl ---------------------------------------------------------------------------
dnl a slightly better AC_C_BIGENDIAN macro which allows cross-compiling
dnl ---------------------------------------------------------------------------

AC_DEFUN([WX_C_BIGENDIAN],
[AC_CACHE_CHECK([whether byte ordering is bigendian], ac_cv_c_bigendian,
[ac_cv_c_bigendian=unknown
# See if sys/param.h defines the BYTE_ORDER macro.
AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/param.h>], [
#if !BYTE_ORDER || !BIG_ENDIAN || !LITTLE_ENDIAN
 bogus endian macros
#endif], [# It does; now see whether it defined to BIG_ENDIAN or not.
AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/param.h>], [
#if BYTE_ORDER != BIG_ENDIAN
 not big endian
#endif], ac_cv_c_bigendian=yes, ac_cv_c_bigendian=no)])
if test $ac_cv_c_bigendian = unknown; then
AC_TRY_RUN([main () {
  /* Are we little or big endian?  From Harbison&Steele.  */
  union
  {
    long l;
    char c[sizeof (long)];
  } u;
  u.l = 1;
  exit (u.c[sizeof (long) - 1] == 1);
}], [ac_cv_c_bigendian=no], [ac_cv_c_bigendian=yes], [ac_cv_c_bigendian=unknown])
fi])
if test $ac_cv_c_bigendian = unknown; then
  AC_MSG_WARN([Assuming little-endian target machine - this may be overriden by adding the line "ac_cv_c_bigendian=${ac_cv_c_bigendian='yes'}" to config.cache file])
fi
if test $ac_cv_c_bigendian = yes; then
  AC_DEFINE(WORDS_BIGENDIAN)
fi
])

dnl ---------------------------------------------------------------------------
dnl override AC_ARG_ENABLE/WITH to cache the results in .cache file
dnl ---------------------------------------------------------------------------

AC_DEFUN([WX_ARG_CACHE_INIT],
        [
          wx_arg_cache_file="configarg.cache"
          echo "loading argument cache $wx_arg_cache_file"
          rm -f ${wx_arg_cache_file}.tmp
          touch ${wx_arg_cache_file}.tmp
          touch ${wx_arg_cache_file}
        ])

AC_DEFUN([WX_ARG_CACHE_FLUSH],
        [
          echo "saving argument cache $wx_arg_cache_file"
          mv ${wx_arg_cache_file}.tmp ${wx_arg_cache_file}
        ])

dnl this macro checks for a three-valued command line --with argument:
dnl   possible arguments are 'yes', 'no', 'sys', or 'builtin'
dnl usage: WX_ARG_SYS_WITH(option, helpmessage, variable-name)
AC_DEFUN([WX_ARG_SYS_WITH],
        [
          AC_MSG_CHECKING([for --with-$1])
          no_cache=0
          AC_ARG_WITH($1, [$2],
                      [
                        if test "$withval" = yes; then
                          ac_cv_use_$1='$3=yes'
                        elif test "$withval" = no; then
                          ac_cv_use_$1='$3=no'
                        elif test "$withval" = sys; then
                          ac_cv_use_$1='$3=sys'
                        elif test "$withval" = builtin; then
                          ac_cv_use_$1='$3=builtin'
                        else
                          AC_MSG_ERROR([Invalid value for --with-$1: should be yes, no, sys, or builtin])
                        fi
                      ],
                      [
                        LINE=`grep "$3" ${wx_arg_cache_file}`
                        if test "x$LINE" != x ; then
                          eval "DEFAULT_$LINE"
                        else
                          no_cache=1
                        fi

                        ac_cv_use_$1='$3='$DEFAULT_$3
                      ])

          eval "$ac_cv_use_$1"
          if test "$no_cache" != 1; then
            echo $ac_cv_use_$1 >> ${wx_arg_cache_file}.tmp
          fi

          if test "$$3" = yes; then
            AC_MSG_RESULT(yes)
          elif test "$$3" = no; then
            AC_MSG_RESULT(no)
          elif test "$$3" = sys; then
            AC_MSG_RESULT([system version])
          elif test "$$3" = builtin; then
            AC_MSG_RESULT([builtin version])
          else
            AC_MSG_ERROR([Invalid value for --with-$1: should be yes, no, sys, or builtin])
          fi
        ])

dnl this macro checks for a command line argument and caches the result
dnl usage: WX_ARG_WITH(option, helpmessage, variable-name)
AC_DEFUN([WX_ARG_WITH],
        [
          AC_MSG_CHECKING([for --with-$1])
          no_cache=0
          AC_ARG_WITH($1, [$2],
                      [
                        if test "$withval" = yes; then
                          ac_cv_use_$1='$3=yes'
                        else
                          ac_cv_use_$1='$3=no'
                        fi
                      ],
                      [
                        LINE=`grep "$3" ${wx_arg_cache_file}`
                        if test "x$LINE" != x ; then
                          eval "DEFAULT_$LINE"
                        else
                          no_cache=1
                        fi

                        ac_cv_use_$1='$3='$DEFAULT_$3
                      ])

          eval "$ac_cv_use_$1"
          if test "$no_cache" != 1; then
            echo $ac_cv_use_$1 >> ${wx_arg_cache_file}.tmp
          fi

          if test "$$3" = yes; then
            AC_MSG_RESULT(yes)
          else
            AC_MSG_RESULT(no)
          fi
        ])

dnl like WX_ARG_WITH but uses AC_ARG_ENABLE instead of AC_ARG_WITH
dnl usage: WX_ARG_ENABLE(option, helpmessage, variable-name)
AC_DEFUN([WX_ARG_ENABLE],
        [
          AC_MSG_CHECKING([for --enable-$1])
          no_cache=0
          AC_ARG_ENABLE($1, [$2],
                        [
                          if test "$enableval" = yes; then
                            ac_cv_use_$1='$3=yes'
                          else
                            ac_cv_use_$1='$3=no'
                          fi
                        ],
                        [
                          LINE=`grep "$3" ${wx_arg_cache_file}`
                          if test "x$LINE" != x ; then
                            eval "DEFAULT_$LINE"
                          else
                            no_cache=1
                          fi

                          ac_cv_use_$1='$3='$DEFAULT_$3
                        ])

          eval "$ac_cv_use_$1"
          if test "$no_cache" != 1; then
            echo $ac_cv_use_$1 >> ${wx_arg_cache_file}.tmp
          fi

          if test "$$3" = yes; then
            AC_MSG_RESULT(yes)
          else
            AC_MSG_RESULT(no)
          fi
        ])



dnl ===========================================================================
dnl "3rd party" macros included here because they are not widely available
dnl ===========================================================================


dnl ---------------------------------------------------------------------------
dnl test for availability of iconv()
dnl ---------------------------------------------------------------------------

dnl From Bruno Haible.

AC_DEFUN([AM_ICONV],
[
  dnl Some systems have iconv in libc, some have it in libiconv (OSF/1 and
  dnl those with the standalone portable GNU libiconv installed).

  AC_ARG_WITH([libiconv-prefix],
[  --with-libiconv-prefix=DIR  search for libiconv in DIR/include and DIR/lib], [
    for dir in `echo "$withval" | tr : ' '`; do
      if test -d $dir/include; then CPPFLAGS="$CPPFLAGS -I$dir/include"; fi
      if test -d $dir/lib; then LDFLAGS="$LDFLAGS -L$dir/lib"; fi
    done
   ])

  AC_CACHE_CHECK(for iconv, am_cv_func_iconv, [
    am_cv_func_iconv="no, consider installing GNU libiconv"
    am_cv_lib_iconv=no
    AC_TRY_LINK([#include <stdlib.h>
#include <iconv.h>],
      [iconv_t cd = iconv_open("","");
       iconv(cd,NULL,NULL,NULL,NULL);
       iconv_close(cd);],
      am_cv_func_iconv=yes)
    if test "$am_cv_func_iconv" != yes; then
      am_save_LIBS="$LIBS"
      LIBS="$LIBS -liconv"
      AC_TRY_LINK([#include <stdlib.h>
#include <iconv.h>],
        [iconv_t cd = iconv_open("","");
         iconv(cd,NULL,NULL,NULL,NULL);
         iconv_close(cd);],
        am_cv_lib_iconv=yes
        am_cv_func_iconv=yes)
      LIBS="$am_save_LIBS"
    fi
  ])
  if test "$am_cv_func_iconv" = yes; then
    AC_DEFINE(HAVE_ICONV, 1, [Define if you have the iconv() function.])
    AC_CACHE_CHECK([if iconv needs const], wx_cv_func_iconv_const,
      AC_TRY_COMPILE([
#include <stdlib.h>
#include <iconv.h>
extern
#ifdef __cplusplus
"C"
#endif
#if defined(__STDC__) || defined(__cplusplus)
size_t iconv (iconv_t cd, char * *inbuf, size_t *inbytesleft, char * *outbuf, size_t *outbytesleft);
#else
size_t iconv();
#endif
        ],
        [],
        wx_cv_func_iconv_const="no",
        wx_cv_func_iconv_const="yes"
      )
    )

    iconv_const=
    if test "x$wx_cv_func_iconv_const" != "xyes"; then
        iconv_const="const"
    fi

    AC_DEFINE_UNQUOTED(ICONV_CONST, $iconv_const,
      [Define as const if the declaration of iconv() needs const.])
  fi
  LIBICONV=
  if test "$am_cv_lib_iconv" = yes; then
    LIBICONV="-liconv"
  fi
  AC_SUBST(LIBICONV)
])


dnl ---------------------------------------------------------------------------
dnl test for GTK+ 2.0
dnl ---------------------------------------------------------------------------

dnl AM_PATH_GTK_2_0([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl Test for GTK, and define GTK_CFLAGS and GTK_LIBS
dnl
AC_DEFUN(AM_PATH_GTK_2_0,
[dnl
dnl Get the cflags and libraries from the gtk-config-2.0 script
dnl
AC_ARG_WITH(gtk-prefix,[  --with-gtk-prefix=PFX   Prefix where GTK is installed (optional)],
            gtk_config_prefix="$withval", gtk_config_prefix="")
AC_ARG_WITH(gtk-exec-prefix,[  --with-gtk-exec-prefix=PFX Exec prefix where GTK is installed (optional)],
            gtk_config_exec_prefix="$withval", gtk_config_exec_prefix="")
AC_ARG_ENABLE(gtktest, [  --disable-gtktest       Do not try to compile and run a test GTK program],
                    , enable_gtktest=yes)

  for module in . $4
  do
      case "$module" in
         gthread)
             gtk_config_args="$gtk_config_args gthread"
         ;;
      esac
  done

  if test x$gtk_config_exec_prefix != x ; then
     gtk_config_args="$gtk_config_args --exec-prefix=$gtk_config_exec_prefix"
     if test x${GTK_CONFIG_2_0+set} != xset ; then
        GTK_CONFIG_2_0=$gtk_config_exec_prefix/bin/gtk-config-2.0
     fi
  fi
  if test x$gtk_config_prefix != x ; then
     gtk_config_args="$gtk_config_args --prefix=$gtk_config_prefix"
     if test x${GTK_CONFIG_2_0+set} != xset ; then
        GTK_CONFIG_2_0=$gtk_config_prefix/bin/gtk-config-2.0
     fi
  fi

  AC_PATH_PROG(GTK_CONFIG_2_0, gtk-config-2.0, no)
  min_gtk_version=ifelse([$1], ,1.3.1,$1)
  AC_MSG_CHECKING(for GTK - version >= $min_gtk_version)
  no_gtk=""
  if test "$GTK_CONFIG_2_0" = "no" ; then
    no_gtk=yes
  else
    GTK_CFLAGS=`$GTK_CONFIG_2_0 $gtk_config_args --cflags`
    GTK_LIBS=`$GTK_CONFIG_2_0 $gtk_config_args --libs`
    gtk_config_major_version=`$GTK_CONFIG_2_0 $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    gtk_config_minor_version=`$GTK_CONFIG_2_0 $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    gtk_config_micro_version=`$GTK_CONFIG_2_0 $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_gtktest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $GTK_CFLAGS"
      LIBS="$GTK_LIBS $LIBS"
dnl
dnl Now check if the installed GTK is sufficiently new. (Also sanity
dnl checks the results of gtk-config-2.0 to some extent
dnl
      rm -f conf.gtktest
      AC_TRY_RUN([
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int
main ()
{
  int major, minor, micro;
  char *tmp_version;

  system ("touch conf.gtktest");

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = g_strdup("$min_gtk_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_gtk_version");
     exit(1);
   }

  if ((gtk_major_version != $gtk_config_major_version) ||
      (gtk_minor_version != $gtk_config_minor_version) ||
      (gtk_micro_version != $gtk_config_micro_version))
    {
      printf("\n*** 'gtk-config-2.0 --version' returned %d.%d.%d, but GTK+ (%d.%d.%d)\n",
             $gtk_config_major_version, $gtk_config_minor_version, $gtk_config_micro_version,
             gtk_major_version, gtk_minor_version, gtk_micro_version);
      printf ("*** was found! If gtk-config-2.0 was correct, then it is best\n");
      printf ("*** to remove the old version of GTK+. You may also be able to fix the error\n");
      printf("*** by modifying your LD_LIBRARY_PATH enviroment variable, or by editing\n");
      printf("*** /etc/ld.so.conf. Make sure you have run ldconfig if that is\n");
      printf("*** required on your system.\n");
      printf("*** If gtk-config-2.0 was wrong, set the environment variable GTK_CONFIG_2_0\n");
      printf("*** to point to the correct copy of gtk-config-2.0, and remove the file config.cache\n");
      printf("*** before re-running configure\n");
    }
#if defined (GTK_MAJOR_VERSION) && defined (GTK_MINOR_VERSION) && defined (GTK_MICRO_VERSION)
  else if ((gtk_major_version != GTK_MAJOR_VERSION) ||
           (gtk_minor_version != GTK_MINOR_VERSION) ||
           (gtk_micro_version != GTK_MICRO_VERSION))
    {
      printf("*** GTK+ header files (version %d.%d.%d) do not match\n",
             GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);
      printf("*** library (version %d.%d.%d)\n",
             gtk_major_version, gtk_minor_version, gtk_micro_version);
    }
#endif /* defined (GTK_MAJOR_VERSION) ... */
  else
    {
      if ((gtk_major_version > major) ||
        ((gtk_major_version == major) && (gtk_minor_version > minor)) ||
        ((gtk_major_version == major) && (gtk_minor_version == minor) && (gtk_micro_version >= micro)))
      {
        return 0;
       }
     else
      {
        printf("\n*** An old version of GTK+ (%d.%d.%d) was found.\n",
               gtk_major_version, gtk_minor_version, gtk_micro_version);
        printf("*** You need a version of GTK+ newer than %d.%d.%d. The latest version of\n",
               major, minor, micro);
        printf("*** GTK+ is always available from ftp://ftp.gtk.org.\n");
        printf("***\n");
        printf("*** If you have already installed a sufficiently new version, this error\n");
        printf("*** probably means that the wrong copy of the gtk-config-2.0 shell script is\n");
        printf("*** being found. The easiest way to fix this is to remove the old version\n");
        printf("*** of GTK+, but you can also set the GTK_CONFIG_2_0 environment to point to the\n");
        printf("*** correct copy of gtk-config-2.0. (In this case, you will have to\n");
        printf("*** modify your LD_LIBRARY_PATH enviroment variable, or edit /etc/ld.so.conf\n");
        printf("*** so that the correct libraries are found at run-time))\n");
      }
    }
  return 1;
}
],, no_gtk=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_gtk" = x ; then
     AC_MSG_RESULT(yes (version $gtk_config_major_version.$gtk_config_minor_version.$gtk_config_micro_version))
     ifelse([$2], , :, [$2])
  else
     AC_MSG_RESULT(no)
     if test "$GTK_CONFIG_2_0" = "no" ; then
       echo "*** The gtk-config-2.0 script installed by GTK could not be found"
       echo "*** If GTK was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the GTK_CONFIG_2_0 environment variable to the"
       echo "*** full path to gtk-config-2.0."
     else
       if test -f conf.gtktest ; then
        :
       else
          echo "*** Could not run GTK test program, checking why..."
          CFLAGS="$CFLAGS $GTK_CFLAGS"
          LIBS="$LIBS $GTK_LIBS"
          AC_TRY_LINK([
#include <gtk/gtk.h>
#include <stdio.h>
],      [ return ((gtk_major_version) || (gtk_minor_version) || (gtk_micro_version)); ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding GTK or finding the wrong"
          echo "*** version of GTK. If it is not finding GTK, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
          echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"
          echo "***"
          echo "*** If you have a RedHat 5.0 system, you should remove the GTK package that"
          echo "*** came with the system with the command"
          echo "***"
          echo "***    rpm --erase --nodeps gtk gtk-devel" ],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means GTK was incorrectly installed"
          echo "*** or that you have moved GTK since it was installed. In the latter case, you"
          echo "*** may want to edit the gtk-config-2.0 script: $GTK_CONFIG_2_0" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     GTK_CFLAGS=""
     GTK_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(GTK_CFLAGS)
  AC_SUBST(GTK_LIBS)
  rm -f conf.gtktest
])

# Configure paths for GTK+
# Owen Taylor     97-11-3

dnl AM_PATH_GTK([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl Test for GTK, and define GTK_CFLAGS and GTK_LIBS
dnl
AC_DEFUN(AM_PATH_GTK,
[dnl 
dnl Get the cflags and libraries from the gtk-config script
dnl
AC_ARG_WITH(gtk-prefix,[  --with-gtk-prefix=PFX   Prefix where GTK is installed (optional)],
            gtk_config_prefix="$withval", gtk_config_prefix="")
AC_ARG_WITH(gtk-exec-prefix,[  --with-gtk-exec-prefix=PFX Exec prefix where GTK is installed (optional)],
            gtk_config_exec_prefix="$withval", gtk_config_exec_prefix="")
AC_ARG_ENABLE(gtktest, [  --disable-gtktest       Do not try to compile and run a test GTK program],
		    , enable_gtktest=yes)

  for module in . $4
  do
      case "$module" in
         gthread) 
             gtk_config_args="$gtk_config_args gthread"
         ;;
      esac
  done

  if test x$gtk_config_exec_prefix != x ; then
     gtk_config_args="$gtk_config_args --exec-prefix=$gtk_config_exec_prefix"
     if test x${GTK_CONFIG+set} != xset ; then
        GTK_CONFIG=$gtk_config_exec_prefix/bin/gtk-config
     fi
  fi
  if test x$gtk_config_prefix != x ; then
     gtk_config_args="$gtk_config_args --prefix=$gtk_config_prefix"
     if test x${GTK_CONFIG+set} != xset ; then
        GTK_CONFIG=$gtk_config_prefix/bin/gtk-config
     fi
  fi

  AC_PATH_PROG(GTK_CONFIG, gtk-config, no)
  min_gtk_version=ifelse([$1], ,0.99.7,$1)
  AC_MSG_CHECKING(for GTK - version >= $min_gtk_version)
  no_gtk=""
  if test "$GTK_CONFIG" = "no" ; then
    no_gtk=yes
  else
    GTK_CFLAGS=`$GTK_CONFIG $gtk_config_args --cflags`
    GTK_LIBS=`$GTK_CONFIG $gtk_config_args --libs`
    gtk_config_major_version=`$GTK_CONFIG $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    gtk_config_minor_version=`$GTK_CONFIG $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    gtk_config_micro_version=`$GTK_CONFIG $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_gtktest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $GTK_CFLAGS"
      LIBS="$GTK_LIBS $LIBS"
dnl
dnl Now check if the installed GTK is sufficiently new. (Also sanity
dnl checks the results of gtk-config to some extent
dnl
      rm -f conf.gtktest
      AC_TRY_RUN([
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int 
main ()
{
  int major, minor, micro;
  char *tmp_version;

  system ("touch conf.gtktest");

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = g_strdup("$min_gtk_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_gtk_version");
     exit(1);
   }

  if ((gtk_major_version != $gtk_config_major_version) ||
      (gtk_minor_version != $gtk_config_minor_version) ||
      (gtk_micro_version != $gtk_config_micro_version))
    {
      printf("\n*** 'gtk-config --version' returned %d.%d.%d, but GTK+ (%d.%d.%d)\n", 
             $gtk_config_major_version, $gtk_config_minor_version, $gtk_config_micro_version,
             gtk_major_version, gtk_minor_version, gtk_micro_version);
      printf ("*** was found! If gtk-config was correct, then it is best\n");
      printf ("*** to remove the old version of GTK+. You may also be able to fix the error\n");
      printf("*** by modifying your LD_LIBRARY_PATH enviroment variable, or by editing\n");
      printf("*** /etc/ld.so.conf. Make sure you have run ldconfig if that is\n");
      printf("*** required on your system.\n");
      printf("*** If gtk-config was wrong, set the environment variable GTK_CONFIG\n");
      printf("*** to point to the correct copy of gtk-config, and remove the file config.cache\n");
      printf("*** before re-running configure\n");
    } 
#if defined (GTK_MAJOR_VERSION) && defined (GTK_MINOR_VERSION) && defined (GTK_MICRO_VERSION)
  else if ((gtk_major_version != GTK_MAJOR_VERSION) ||
	   (gtk_minor_version != GTK_MINOR_VERSION) ||
           (gtk_micro_version != GTK_MICRO_VERSION))
    {
      printf("*** GTK+ header files (version %d.%d.%d) do not match\n",
	     GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);
      printf("*** library (version %d.%d.%d)\n",
	     gtk_major_version, gtk_minor_version, gtk_micro_version);
    }
#endif /* defined (GTK_MAJOR_VERSION) ... */
  else
    {
      if ((gtk_major_version > major) ||
        ((gtk_major_version == major) && (gtk_minor_version > minor)) ||
        ((gtk_major_version == major) && (gtk_minor_version == minor) && (gtk_micro_version >= micro)))
      {
        return 0;
       }
     else
      {
        printf("\n*** An old version of GTK+ (%d.%d.%d) was found.\n",
               gtk_major_version, gtk_minor_version, gtk_micro_version);
        printf("*** You need a version of GTK+ newer than %d.%d.%d. The latest version of\n",
	       major, minor, micro);
        printf("*** GTK+ is always available from ftp://ftp.gtk.org.\n");
        printf("***\n");
        printf("*** If you have already installed a sufficiently new version, this error\n");
        printf("*** probably means that the wrong copy of the gtk-config shell script is\n");
        printf("*** being found. The easiest way to fix this is to remove the old version\n");
        printf("*** of GTK+, but you can also set the GTK_CONFIG environment to point to the\n");
        printf("*** correct copy of gtk-config. (In this case, you will have to\n");
        printf("*** modify your LD_LIBRARY_PATH enviroment variable, or edit /etc/ld.so.conf\n");
        printf("*** so that the correct libraries are found at run-time))\n");
      }
    }
  return 1;
}
],, no_gtk=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_gtk" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$GTK_CONFIG" = "no" ; then
       echo "*** The gtk-config script installed by GTK could not be found"
       echo "*** If GTK was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the GTK_CONFIG environment variable to the"
       echo "*** full path to gtk-config."
     else
       if test -f conf.gtktest ; then
        :
       else
          echo "*** Could not run GTK test program, checking why..."
          CFLAGS="$CFLAGS $GTK_CFLAGS"
          LIBS="$LIBS $GTK_LIBS"
          AC_TRY_LINK([
#include <gtk/gtk.h>
#include <stdio.h>
],      [ return ((gtk_major_version) || (gtk_minor_version) || (gtk_micro_version)); ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding GTK or finding the wrong"
          echo "*** version of GTK. If it is not finding GTK, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"
          echo "***"
          echo "*** If you have a RedHat 5.0 system, you should remove the GTK package that"
          echo "*** came with the system with the command"
          echo "***"
          echo "***    rpm --erase --nodeps gtk gtk-devel" ],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means GTK was incorrectly installed"
          echo "*** or that you have moved GTK since it was installed. In the latter case, you"
          echo "*** may want to edit the gtk-config script: $GTK_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     GTK_CFLAGS=""
     GTK_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(GTK_CFLAGS)
  AC_SUBST(GTK_LIBS)
  rm -f conf.gtktest
])


/* ./include/wx/motif/setup.h.  Generated automatically by configure.  */
/* setup.h

   Do not edit this file. It is autogenerated by configure.

   Leave the following blank line there!!  Autoheader needs it.  */


/* ------------------------------------------------------------------------ */
/* Features as requested by configure                                       */
/* ------------------------------------------------------------------------ */

#ifndef __GTKSETUPH__
#define __GTKSETUPH__

#ifdef __GNUG__
#pragma interface
#endif

/* define the system to compile */
#define __UNIX__ 1
#define __LINUX__ 1
/* #undef __SGI__ */
/* #undef __HPUX__ */
/* #undef __SYSV__ */
/* #undef __SVR4__ */
/* #undef __AIX__ */
/* #undef __SUN__ */
/* #undef __SOLARIS__ */
/* #undef __SUNOS__ */
/* #undef __ALPHA__ */
/* #undef __OSF__ */
/* #undef __BSD__ */
/* #undef __FREEBSD__ */
/* #undef __VMS__ */
/* #undef __ULTRIX__ */
/* #undef __DATA_GENERAL__ */

/* ------------------------------------------------------------------------ */
/* compiler options                                                         */
/* ------------------------------------------------------------------------ */

/*
 * Supports bool type
 */
#define HAVE_BOOL 1

/* ------------------------------------------------------------------------ */
/* library options                                                          */
/* ------------------------------------------------------------------------ */

/*
 * Use zlib
 */
#define wxUSE_ZLIB 1
/*
 * Use libpng
 */
#define wxUSE_LIBPNG 1
/*
 * Use libjpeg
 */
#define wxUSE_LIBJPEG 0
/*
 * Use GIF
 */
#define wxUSE_LIBGIF 1
/*
 * Use PCX
 */
#define wxUSE_PCX 1
/*
 * Use PNM
 */
#define wxUSE_PNM 1

// Set to 1 for MS Icons and Cursors format support
#define wxUSE_ICO_CUR       1
 
/*
 * Use iODBC
 */
#define wxUSE_ODBC 0
#define wxODBC_FWD_ONLY_CURSORS 1
/*
 * Use Threads
 */
#define wxUSE_THREADS 0
/*
 * Have glibc2
 */
#define wxHAVE_GLIBC2 1
/*
 * Use libXpm
 */
#define wxHAVE_LIB_XPM 0

#define wxUSE_XPM 1

/* ------------------------------------------------------------------------ */
/* GUI control options (always enabled in wxGTK)                            */
/* ------------------------------------------------------------------------ */
/*
 * Use gauge item
 */
#define wxUSE_GAUGE 1
/*
 * Use combobox item
 */
#define wxUSE_COMBOBOX 1
/*
 * Use scrollbar item
 */
#define wxUSE_SCROLLBAR 1
/*
 * Use scrollbar item
 */
#define wxUSE_RADIOBTN 1
/*
 * Use toolbar item
 */
#define wxUSE_TOOLBAR 1

/* ------------------------------------------------------------------------ */
/* non-GUI options                                                          */
/* ------------------------------------------------------------------------ */

/*
 * Use time and date classes
 */
#define wxUSE_TIMEDATE 1
/*
 * Use config system
 */
#define wxUSE_CONFIG 1
/*
 * Use intl system
 */
#define wxUSE_INTL 1
/*
 * Use streams
 */
#define wxUSE_STREAMS 1
/*
 * Use class serialization
 */
#define wxUSE_SERIAL 0
/*
 * Use sockets
 */
#define wxUSE_SOCKETS 1
/*
 * Use standard C++ streams if 1. If 0, use wxWin
 * streams implementation.
 */
#define wxUSE_STD_IOSTREAM  1
/*
 * Use constraints mechanism
 */
#define wxUSE_CONSTRAINTS 1

/* ------------------------------------------------------------------------ */
/* PS options                                                               */
/* ------------------------------------------------------------------------ */

/*
 * Use font metric files in GetTextExtent for wxPostScriptDC
 * Use consistent PostScript fonts for AFM and printing (!)
 */
#define wxUSE_AFM_FOR_POSTSCRIPT 1
#define WX_NORMALIZED_PS_FONTS 1
/*
 * Use PostScript device context
 */
#define wxUSE_POSTSCRIPT 1

/* ------------------------------------------------------------------------ */
/* wxString options                                                         */
/* ------------------------------------------------------------------------ */

/*
 * Compile wxString with wide character (Unicode) support?
 */
#define wxUSE_UNICODE 1

/*
 * Work around a bug in GNU libc 5.x wcstombs() implementation.
 *
 * Note that you must link your programs with libc.a if you enable this and you
 * have libc 5 (you should enable this for libc6 where wcsrtombs() is
 * thread-safe version of wcstombs()).
 */
#define wxUSE_WCSRTOMBS 0

/*
 * On some platforms overloading on size_t/int doesn't work, yet we'd like
 * to define both size_t and int version of wxString::operator[] because it
 * should really be size_t, but a lot of old, broken code uses int indices.
 */
#define wxUSE_SIZE_T_STRING_OPERATOR 1

/* ------------------------------------------------------------------------ */
/* misc options                                                             */
/* ------------------------------------------------------------------------ */

/*
 * Use Interprocess communication
 */
#define wxUSE_IPC 1
/*
 * Use wxGetResource & wxWriteResource (change .Xdefaults)
 */
#define wxUSE_RESOURCES 1
/*
 * Use clipboard
 */
#define wxUSE_CLIPBOARD 1
/*
 * Use tooltips
 */
#define wxUSE_TOOLTIPS 0
/*
 * Use dnd
 */
#define wxUSE_DRAG_AND_DROP 0
/*
 * Use wxLibrary class
 */
#define wxUSE_DYNLIB_CLASS 0

/* ------------------------------------------------------------------------ */
/* architecture options                                                     */
/* ------------------------------------------------------------------------ */

/*
 * Use the mdi architecture
 */
#define wxUSE_MDI_ARCHITECTURE 1
/*
 * Use the document/view architecture
 */
#define wxUSE_DOC_VIEW_ARCHITECTURE 1
/*
 * Use the print/preview architecture
 */
#define wxUSE_PRINTING_ARCHITECTURE 1

/* ------------------------------------------------------------------------ */
/* Prolog and wxWindows' resource system options                            */
/* ------------------------------------------------------------------------ */

/*
 * Use Prolog IO
 */
#define wxUSE_PROLOGIO 1
/*
 * Use Remote Procedure Call (Needs wxUSE_IPC and wxUSE_PROLOGIO)
 */
#define wxUSE_RPC 0
/*
 * Use wxWindows resource loading (.wxr-files) (Needs wxUSE_PROLOGIO 1)
 */
#define wxUSE_WX_RESOURCES 1

/* ------------------------------------------------------------------------ */
/* the rest                                                                 */
/* ------------------------------------------------------------------------ */

/*
 * Use wxWindows help facility (needs wxUSE_IPC 1)
 */
#define wxUSE_HELP 1
/*
 * Use iostream.h rather than iostream
 */
#define wxUSE_IOSTREAMH 1
/*
 * Use Apple Ieee-double converter
 */
#define wxUSE_APPLE_IEEE 1
/* Compatibility with 1.66 API.
   Level 0: no backward compatibility, all new features
   Level 1: wxDC, OnSize (etc.) compatibility, but
   some new features such as event tables */
#define WXWIN_COMPATIBILITY  0
/*
 * Enables debugging: memory tracing, assert, etc., contains debug level
 */
#define WXDEBUG 1
/*
 * Enables debugging version of wxObject::new and wxObject::delete (IF WXDEBUG)
 * WARNING: this code may not work with all architectures, especially
 * if alignment is an issue.
 */
#define wxUSE_MEMORY_TRACING 0
/*
 * Enable debugging version of global memory operators new and delete
 * Disable it, If this causes problems (e.g. link errors)
 */
#define wxUSE_DEBUG_NEW_ALWAYS 1
/*
 * In debug mode, causes new to be defined to
 * be WXDEBUG_NEW (see object.h). If this causes
 * problems (e.g. link errors), set this to 0.
 */
#define wxUSE_GLOBAL_MEMORY_OPERATORS 0
/*
 * Matthews garbage collection (used for MrEd?)
 */
#define WXGARBAGE_COLLECTION_ON 0
/*
 * Use splines
 */
#define wxUSE_SPLINES 1
/*
 * wxUSE_DYNAMIC_CLASSES is TRUE for the Xt port
 */
#define wxUSE_DYNAMIC_CLASSES 1
/*
 * Disable this if your compiler can't cope
 * with omission of prototype parameters.
 */
#define REMOVE_UNUSED_ARG 1
/*
 * The const keyword is being introduced more in wxWindows.
 * You can use this setting to maintain backward compatibility.
 * If 0:	will use const wherever possible.
 * If 1:	will use const only where necessary
 *              for precompiled headers to work.
 * If 2:	will be totally backward compatible, but precompiled
 *	headers may not work and program size will be larger.
 */
#define CONST_COMPATIBILITY 0

/* ------------------------------------------------------------------------ */
/* System-specific stuff                                                    */
/* ------------------------------------------------------------------------ */

/* acconfig.h
   This file is in the public domain.

   Descriptive text for the C preprocessor macros that
   the distributed Autoconf macros can define.
   No software package will use all of them; autoheader copies the ones
   your configure.in uses into your configuration header file templates.

   The entries are in sort -df order: alphabetical, case insensitive,
   ignoring punctuation (such as underscores).  Although this order
   can split up related entries, it makes it easier to check whether
   a given entry is in the file. */

/* Define if on AIX 3.
   System headers sometimes define this.
   We just want to avoid a redefinition error message.  */
#ifndef _ALL_SOURCE
/* #undef _ALL_SOURCE */
#endif

/* Define if using alloca.c.  */
/* #undef C_ALLOCA */

/* Define if type char is unsigned and you are not using gcc.  */
#ifndef __CHAR_UNSIGNED__
/* #undef __CHAR_UNSIGNED__ */
#endif

/* Define if the closedir function returns void instead of int.  */
/* #undef CLOSEDIR_VOID */

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define to one of _getb67, GETB67, getb67 for Cray-2 and Cray-YMP systems.
   This function is required for alloca.c support on those systems.  */
/* #undef CRAY_STACKSEG_END */

/* Define for DGUX with <sys/dg_sys_info.h>.  */
/* #undef DGUX */

/* Define if you have <dirent.h>.  */
/* #undef DIRENT */

/* Define to the type of elements in the array set by `getgroups'.
   Usually this is either `int' or `gid_t'.  */
#define GETGROUPS_T gid_t

/* The type of 3rd argument to getsockname() - usually size_t or int */
#define SOCKLEN_T size_t

/* Define if the `getloadavg' function needs to be run setuid or setgid.  */
/* #undef GETLOADAVG_PRIVILEGED */

/* Define if the `getpgrp' function takes no argument.  */
/* #undef GETPGRP_VOID */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef gid_t */

/* Define if you have alloca, as a function or macro.  */
/* #undef HAVE_ALLOCA */

/* Define if you have <alloca.h> and it should be used (not on Ultrix).  */
/* #undef HAVE_ALLOCA_H */

/* Define if you don't have vprintf but do have _doprnt.  */
/* #undef HAVE_DOPRNT */

/* Define if you have sched.h */
#define HAVE_SCHED_H 0

/* Define if you have strings.h */
#define HAVE_STRINGS_H 1

/* Define if you have vprintf() */
#define HAVE_VPRINTF 1

/* Define if you have dlopen() */
/* #undef HAVE_DLOPEN */

/* Define if you have shl_load() */
/* #undef HAVE_SHL_LOAD */

/* Define if you have vsnprintf() */
#define HAVE_VSNPRINTF 1

/* Define if you have usleep() */
#define HAVE_USLEEP 1

/* Define if you have nanosleep() */
#define HAVE_NANOSLEEP 0

/* Define if you have vfork() */
#define HAVE_VFORK 1

/* Define if you have uname() */
#define HAVE_UNAME 1

/* Define if you have gethostname() */
/* #undef HAVE_GETHOSTNAME */

/* Define if you have sched_yield */
#define HAVE_SCHED_YIELD 1

/* Define if you have pthread_cancel */
#define HAVE_PTHREAD_CANCEL 1

/* Define if you have all functions to set thread priority */
#define HAVE_THREAD_PRIORITY_FUNCTIONS 1

/* Define if your system has its own `getloadavg' function.  */
/* #undef HAVE_GETLOADAVG */

/* Define if you have the getmntent function.  */
/* #undef HAVE_GETMNTENT */

/* Define if the `long double' type works.  */
#define HAVE_LONG_DOUBLE 1

/* Define if you support file names longer than 14 characters.  */
#define HAVE_LONG_FILE_NAMES 1

/* Define if you have a working `mmap' system call.  */
/* #undef HAVE_MMAP */

/* Define if system calls automatically restart after interruption
   by a signal.  */
/* #undef HAVE_RESTARTABLE_SYSCALLS */

/* Define if your struct stat has st_blksize.  */
#define HAVE_ST_BLKSIZE 1

/* Define if your struct stat has st_blocks.  */
#define HAVE_ST_BLOCKS 1

/* Define if you have the strcoll function and it is properly defined.  */
/* #undef HAVE_STRCOLL */

/* Define if your struct stat has st_rdev.  */
#define HAVE_ST_RDEV 1

/* Define if you have the strftime function.  */
/* #undef HAVE_STRFTIME */

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#define HAVE_SYS_WAIT_H 1

/* Define if your struct tm has tm_zone.  */
#define HAVE_TM_ZONE 1

/* Define if you don't have tm_zone but do have the external array
   tzname.  */
/* #undef HAVE_TZNAME */

/* Define if you have <unistd.h>.  */
#define HAVE_UNISTD_H 1

/* Define if you have <fnmatch.h>.  */
#define HAVE_FNMATCH_H 1

/* Define if utime(file, NULL) sets file's timestamp to the present.  */
/* #undef HAVE_UTIME_NULL */

/* Define if you have <vfork.h>.  */
/* #undef HAVE_VFORK_H */

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

/* Define if you have the wait3 system call.  */
/* #undef HAVE_WAIT3 */

/* Define as __inline if that's what the C compiler calls it.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define if major, minor, and makedev are declared in <mkdev.h>.  */
/* #undef MAJOR_IN_MKDEV */

/* Define if major, minor, and makedev are declared in <sysmacros.h>.  */
/* #undef MAJOR_IN_SYSMACROS */

/* Define if on MINIX.  */
/* #undef _MINIX */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef mode_t */

/* Define if you don't have <dirent.h>, but have <ndir.h>.  */
/* #undef NDIR */

/* Define if you have <memory.h>, and <string.h> doesn't declare the
   mem* functions.  */
/* #undef NEED_MEMORY_H */

/* Define if your struct nlist has an n_un member.  */
/* #undef NLIST_NAME_UNION */

/* Define if you have <nlist.h>.  */
/* #undef NLIST_STRUCT */

/* Define if your C compiler doesn't accept -c and -o together.  */
/* #undef NO_MINUS_C_MINUS_O */

/* Define to `long' if <sys/types.h> doesn't define.  */
/* #undef off_t */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef pid_t */

/* Define if the system does not provide POSIX.1 features except
   with this defined.  */
/* #undef _POSIX_1_SOURCE */

/* Define if you need to in order for stat and other things to work.  */
/* #undef _POSIX_SOURCE */

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE void

/* Define if the setvbuf function takes the buffering type as its second
   argument and the buffer pointer as the third, as on System V
   before release 3.  */
/* #undef SETVBUF_REVERSED */

/* Define SIZESOF for some Objects  */
#define SIZEOF_INT 4
#define SIZEOF_INT_P 4
#define SIZEOF_LONG 4
#define SIZEOF_LONG_LONG 8

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at run-time.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown
 */
/* #undef STACK_DIRECTION */

/* Define if the `S_IS*' macros in <sys/stat.h> do not work properly.  */
/* #undef STAT_MACROS_BROKEN */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define on System V Release 4.  */
/* #undef SVR4 */

/* Define on BSD  */
/* #undef BSD */

/* Define on System V */
/* #undef SYSV */

/* Define if you don't have <dirent.h>, but have <sys/dir.h>.  */
/* #undef SYSDIR */

/* Define if you don't have <dirent.h>, but have <sys/ndir.h>.  */
/* #undef SYSNDIR */

/* Define if `sys_siglist' is declared by <signal.h>.  */
/* #undef SYS_SIGLIST_DECLARED */

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#define TIME_WITH_SYS_TIME 1

/* Define if your <sys/time.h> declares struct tm.  */
/* #undef TM_IN_SYS_TIME */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef uid_t */

/* Define for Encore UMAX.  */
/* #undef UMAX */

/* Define for Encore UMAX 4.3 that has <inq_status/cpustats.h>
   instead of <sys/cpustats.h>.  */
/* #undef UMAX4_3 */

/* Define if you do not have <strings.h>, index, bzero, etc..  */
/* #undef USG */

/* Define if the system is System V Release 4 */
/* #undef SVR4 */

/* Define vfork as fork if vfork does not work.  */
/* #undef vfork */

/* Define if the closedir function returns void instead of int.  */
/* #undef VOID_CLOSEDIR */

/* Define if your processor stores words with the most significant
   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
/* #undef WORDS_BIGENDIAN */

/* Define if lex declares yytext as a char * by default, not a char[].  */
#define YYTEXT_POINTER 1

#endif /* __GTKSETUPH__ */


/* Leave that blank line there!!  Autoheader needs it.
   If you're adding to this file, keep in mind:
   The entries are in sort -df order: alphabetical, case insensitive,
   ignoring punctuation (such as underscores).  */

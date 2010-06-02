/////////////////////////////////////////////////////////////////////////////
// Name:        samples/console/console.cpp
// Purpose:     A sample console (as opposed to GUI) program using wxWidgets
// Author:      Vadim Zeitlin
// Modified by:
// Created:     04.10.99
// RCS-ID:      $Id$
// Copyright:   (c) 1999 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// IMPORTANT NOTE FOR WXWIDGETS USERS:
// If you're a wxWidgets user and you're looking at this file to learn how to
// structure a wxWidgets console application, then you don't have much to learn.
// This application is used more for testing rather than as sample but
// basically the following simple block is enough for you to start your
// own console application:

/*
    int main(int argc, char **argv)
    {
        wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");

        wxInitializer initializer;
        if ( !initializer )
        {
            fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
            return -1;
        }

        static const wxCmdLineEntryDesc cmdLineDesc[] =
        {
            { wxCMD_LINE_SWITCH, "h", "help", "show this help message",
                wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
            // ... your other command line options here...

            { wxCMD_LINE_NONE }
        };

        wxCmdLineParser parser(cmdLineDesc, argc, wxArgv);
        switch ( parser.Parse() )
        {
            case -1:
                wxLogMessage(wxT("Help was given, terminating."));
                break;

            case 0:
                // everything is ok; proceed
                break;

            default:
                wxLogMessage(wxT("Syntax error detected, aborting."));
                break;
        }

        // do something useful here

        return 0;
    }
*/


// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/defs.h"

#include <stdio.h>

#include "wx/string.h"
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/app.h"
#include "wx/log.h"
#include "wx/apptrait.h"
#include "wx/platinfo.h"
#include "wx/wxchar.h"

// without this pragma, the stupid compiler precompiles #defines below so that
// changing them doesn't "take place" later!
#ifdef __VISUALC__
    #pragma hdrstop
#endif

// ----------------------------------------------------------------------------
// conditional compilation
// ----------------------------------------------------------------------------

/*
   A note about all these conditional compilation macros: this file is used
   both as a test suite for various non-GUI wxWidgets classes and as a
   scratchpad for quick tests. So there are two compilation modes: if you
   define TEST_ALL all tests are run, otherwise you may enable the individual
   tests individually in the "#else" branch below.
 */

// what to test (in alphabetic order)? Define TEST_ALL to 0 to do a single
// test, define it to 1 to do all tests.
#define TEST_ALL 0


#if TEST_ALL
    #define TEST_DATETIME
    #define TEST_DIR
    #define TEST_DYNLIB
    #define TEST_ENVIRON
    #define TEST_FILE
    #define TEST_FILECONF
    #define TEST_FILENAME
    #define TEST_FILETIME
    #define TEST_INFO_FUNCTIONS
    #define TEST_LOCALE
    #define TEST_LOG
    #define TEST_MIME
    #define TEST_MODULE
    #define TEST_PATHLIST
    #define TEST_PRINTF
    #define TEST_REGEX
#else // #if TEST_ALL
    #define TEST_DATETIME
    #define TEST_VOLUME
    #define TEST_STDPATHS
    #define TEST_STACKWALKER
    #define TEST_FTP
    #define TEST_SNGLINST
#endif

// some tests are interactive, define this to run them
#ifdef TEST_INTERACTIVE
    #undef TEST_INTERACTIVE

    #define TEST_INTERACTIVE 1
#else
    #define TEST_INTERACTIVE 1
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxDir
// ----------------------------------------------------------------------------

#ifdef TEST_DIR

#include "wx/dir.h"

#ifdef __UNIX__
    static const wxChar *ROOTDIR = wxT("/");
    static const wxChar *TESTDIR = wxT("/usr/local/share");
#elif defined(__WXMSW__) || defined(__DOS__) || defined(__OS2__)
    static const wxChar *ROOTDIR = wxT("c:\\");
    static const wxChar *TESTDIR = wxT("d:\\");
#else
    #error "don't know where the root directory is"
#endif

static void TestDirEnumHelper(wxDir& dir,
                              int flags = wxDIR_DEFAULT,
                              const wxString& filespec = wxEmptyString)
{
    wxString filename;

    if ( !dir.IsOpened() )
        return;

    bool cont = dir.GetFirst(&filename, filespec, flags);
    while ( cont )
    {
        wxPrintf(wxT("\t%s\n"), filename.c_str());

        cont = dir.GetNext(&filename);
    }

    wxPuts(wxEmptyString);
}

#if TEST_ALL

static void TestDirEnum()
{
    wxPuts(wxT("*** Testing wxDir::GetFirst/GetNext ***"));

    wxString cwd = wxGetCwd();
    if ( !wxDir::Exists(cwd) )
    {
        wxPrintf(wxT("ERROR: current directory '%s' doesn't exist?\n"), cwd.c_str());
        return;
    }

    wxDir dir(cwd);
    if ( !dir.IsOpened() )
    {
        wxPrintf(wxT("ERROR: failed to open current directory '%s'.\n"), cwd.c_str());
        return;
    }

    wxPuts(wxT("Enumerating everything in current directory:"));
    TestDirEnumHelper(dir);

    wxPuts(wxT("Enumerating really everything in current directory:"));
    TestDirEnumHelper(dir, wxDIR_DEFAULT | wxDIR_DOTDOT);

    wxPuts(wxT("Enumerating object files in current directory:"));
    TestDirEnumHelper(dir, wxDIR_DEFAULT, wxT("*.o*"));

    wxPuts(wxT("Enumerating directories in current directory:"));
    TestDirEnumHelper(dir, wxDIR_DIRS);

    wxPuts(wxT("Enumerating files in current directory:"));
    TestDirEnumHelper(dir, wxDIR_FILES);

    wxPuts(wxT("Enumerating files including hidden in current directory:"));
    TestDirEnumHelper(dir, wxDIR_FILES | wxDIR_HIDDEN);

    dir.Open(ROOTDIR);

    wxPuts(wxT("Enumerating everything in root directory:"));
    TestDirEnumHelper(dir, wxDIR_DEFAULT);

    wxPuts(wxT("Enumerating directories in root directory:"));
    TestDirEnumHelper(dir, wxDIR_DIRS);

    wxPuts(wxT("Enumerating files in root directory:"));
    TestDirEnumHelper(dir, wxDIR_FILES);

    wxPuts(wxT("Enumerating files including hidden in root directory:"));
    TestDirEnumHelper(dir, wxDIR_FILES | wxDIR_HIDDEN);

    wxPuts(wxT("Enumerating files in non existing directory:"));
    wxDir dirNo(wxT("nosuchdir"));
    TestDirEnumHelper(dirNo);
}

#endif // TEST_ALL

class DirPrintTraverser : public wxDirTraverser
{
public:
    virtual wxDirTraverseResult OnFile(const wxString& WXUNUSED(filename))
    {
        return wxDIR_CONTINUE;
    }

    virtual wxDirTraverseResult OnDir(const wxString& dirname)
    {
        wxString path, name, ext;
        wxFileName::SplitPath(dirname, &path, &name, &ext);

        if ( !ext.empty() )
            name << wxT('.') << ext;

        wxString indent;
        for ( const wxChar *p = path.c_str(); *p; p++ )
        {
            if ( wxIsPathSeparator(*p) )
                indent += wxT("    ");
        }

        wxPrintf(wxT("%s%s\n"), indent.c_str(), name.c_str());

        return wxDIR_CONTINUE;
    }
};

static void TestDirTraverse()
{
    wxPuts(wxT("*** Testing wxDir::Traverse() ***"));

    // enum all files
    wxArrayString files;
    size_t n = wxDir::GetAllFiles(TESTDIR, &files);
    wxPrintf(wxT("There are %u files under '%s'\n"), n, TESTDIR);
    if ( n > 1 )
    {
        wxPrintf(wxT("First one is '%s'\n"), files[0u].c_str());
        wxPrintf(wxT(" last one is '%s'\n"), files[n - 1].c_str());
    }

    // enum again with custom traverser
    wxPuts(wxT("Now enumerating directories:"));
    wxDir dir(TESTDIR);
    DirPrintTraverser traverser;
    dir.Traverse(traverser, wxEmptyString, wxDIR_DIRS | wxDIR_HIDDEN);
}

#if TEST_ALL

static void TestDirExists()
{
    wxPuts(wxT("*** Testing wxDir::Exists() ***"));

    static const wxChar *dirnames[] =
    {
        wxT("."),
#if defined(__WXMSW__)
        wxT("c:"),
        wxT("c:\\"),
        wxT("\\\\share\\file"),
        wxT("c:\\dos"),
        wxT("c:\\dos\\"),
        wxT("c:\\dos\\\\"),
        wxT("c:\\autoexec.bat"),
#elif defined(__UNIX__)
        wxT("/"),
        wxT("//"),
        wxT("/usr/bin"),
        wxT("/usr//bin"),
        wxT("/usr///bin"),
#endif
    };

    for ( size_t n = 0; n < WXSIZEOF(dirnames); n++ )
    {
        wxPrintf(wxT("%-40s: %s\n"),
                 dirnames[n],
                 wxDir::Exists(dirnames[n]) ? wxT("exists")
                                            : wxT("doesn't exist"));
    }
}

#endif // TEST_ALL

#endif // TEST_DIR

// ----------------------------------------------------------------------------
// wxDllLoader
// ----------------------------------------------------------------------------

#ifdef TEST_DYNLIB

#include "wx/dynlib.h"

static void TestDllLoad()
{
#if defined(__WXMSW__)
    static const wxChar *LIB_NAME = wxT("kernel32.dll");
    static const wxChar *FUNC_NAME = wxT("lstrlenA");
#elif defined(__UNIX__)
    // weird: using just libc.so does *not* work!
    static const wxChar *LIB_NAME = wxT("/lib/libc.so.6");
    static const wxChar *FUNC_NAME = wxT("strlen");
#else
    #error "don't know how to test wxDllLoader on this platform"
#endif

    wxPuts(wxT("*** testing basic wxDynamicLibrary functions ***\n"));

    wxDynamicLibrary lib(LIB_NAME);
    if ( !lib.IsLoaded() )
    {
        wxPrintf(wxT("ERROR: failed to load '%s'.\n"), LIB_NAME);
    }
    else
    {
        typedef int (wxSTDCALL *wxStrlenType)(const char *);
        wxStrlenType pfnStrlen = (wxStrlenType)lib.GetSymbol(FUNC_NAME);
        if ( !pfnStrlen )
        {
            wxPrintf(wxT("ERROR: function '%s' wasn't found in '%s'.\n"),
                     FUNC_NAME, LIB_NAME);
        }
        else
        {
            wxPrintf(wxT("Calling %s dynamically loaded from %s "),
                     FUNC_NAME, LIB_NAME);

            if ( pfnStrlen("foo") != 3 )
            {
                wxPrintf(wxT("ERROR: loaded function is not wxStrlen()!\n"));
            }
            else
            {
                wxPuts(wxT("... ok"));
            }
        }

#ifdef __WXMSW__
        static const wxChar *FUNC_NAME_AW = wxT("lstrlen");

        typedef int (wxSTDCALL *wxStrlenTypeAorW)(const wxChar *);
        wxStrlenTypeAorW
            pfnStrlenAorW = (wxStrlenTypeAorW)lib.GetSymbolAorW(FUNC_NAME_AW);
        if ( !pfnStrlenAorW )
        {
            wxPrintf(wxT("ERROR: function '%s' wasn't found in '%s'.\n"),
                     FUNC_NAME_AW, LIB_NAME);
        }
        else
        {
            if ( pfnStrlenAorW(wxT("foobar")) != 6 )
            {
                wxPrintf(wxT("ERROR: loaded function is not wxStrlen()!\n"));
            }
        }
#endif // __WXMSW__
    }
}

#if defined(__WXMSW__) || defined(__UNIX__)

static void TestDllListLoaded()
{
    wxPuts(wxT("*** testing wxDynamicLibrary::ListLoaded() ***\n"));

    puts("\nLoaded modules:");
    wxDynamicLibraryDetailsArray dlls = wxDynamicLibrary::ListLoaded();
    const size_t count = dlls.GetCount();
    for ( size_t n = 0; n < count; ++n )
    {
        const wxDynamicLibraryDetails& details = dlls[n];
        printf("%-45s", (const char *)details.GetPath().mb_str());

        void *addr wxDUMMY_INITIALIZE(NULL);
        size_t len wxDUMMY_INITIALIZE(0);
        if ( details.GetAddress(&addr, &len) )
        {
            printf(" %08lx:%08lx",
                   (unsigned long)addr, (unsigned long)((char *)addr + len));
        }

        printf(" %s\n", (const char *)details.GetVersion().mb_str());
    }
}

#endif

#endif // TEST_DYNLIB

// ----------------------------------------------------------------------------
// wxGet/SetEnv
// ----------------------------------------------------------------------------

#ifdef TEST_ENVIRON

#include "wx/utils.h"

static wxString MyGetEnv(const wxString& var)
{
    wxString val;
    if ( !wxGetEnv(var, &val) )
        val = wxT("<empty>");
    else
        val = wxString(wxT('\'')) + val + wxT('\'');

    return val;
}

static void TestEnvironment()
{
    const wxChar *var = wxT("wxTestVar");

    wxPuts(wxT("*** testing environment access functions ***"));

    wxPrintf(wxT("Initially getenv(%s) = %s\n"), var, MyGetEnv(var).c_str());
    wxSetEnv(var, wxT("value for wxTestVar"));
    wxPrintf(wxT("After wxSetEnv: getenv(%s) = %s\n"),  var, MyGetEnv(var).c_str());
    wxSetEnv(var, wxT("another value"));
    wxPrintf(wxT("After 2nd wxSetEnv: getenv(%s) = %s\n"),  var, MyGetEnv(var).c_str());
    wxUnsetEnv(var);
    wxPrintf(wxT("After wxUnsetEnv: getenv(%s) = %s\n"),  var, MyGetEnv(var).c_str());
    wxPrintf(wxT("PATH = %s\n"),  MyGetEnv(wxT("PATH")).c_str());
}

#endif // TEST_ENVIRON

// ----------------------------------------------------------------------------
// file
// ----------------------------------------------------------------------------

#ifdef TEST_FILE

#include "wx/file.h"
#include "wx/ffile.h"
#include "wx/textfile.h"

static void TestFileRead()
{
    wxPuts(wxT("*** wxFile read test ***"));

    wxFile file(wxT("testdata.fc"));
    if ( file.IsOpened() )
    {
        wxPrintf(wxT("File length: %lu\n"), file.Length());

        wxPuts(wxT("File dump:\n----------"));

        static const size_t len = 1024;
        wxChar buf[len];
        for ( ;; )
        {
            size_t nRead = file.Read(buf, len);
            if ( nRead == (size_t)wxInvalidOffset )
            {
                wxPrintf(wxT("Failed to read the file."));
                break;
            }

            fwrite(buf, nRead, 1, stdout);

            if ( nRead < len )
                break;
        }

        wxPuts(wxT("----------"));
    }
    else
    {
        wxPrintf(wxT("ERROR: can't open test file.\n"));
    }

    wxPuts(wxEmptyString);
}

static void TestTextFileRead()
{
    wxPuts(wxT("*** wxTextFile read test ***"));

    wxTextFile file(wxT("testdata.fc"));
    if ( file.Open() )
    {
        wxPrintf(wxT("Number of lines: %u\n"), file.GetLineCount());
        wxPrintf(wxT("Last line: '%s'\n"), file.GetLastLine().c_str());

        wxString s;

        wxPuts(wxT("\nDumping the entire file:"));
        for ( s = file.GetFirstLine(); !file.Eof(); s = file.GetNextLine() )
        {
            wxPrintf(wxT("%6u: %s\n"), file.GetCurrentLine() + 1, s.c_str());
        }
        wxPrintf(wxT("%6u: %s\n"), file.GetCurrentLine() + 1, s.c_str());

        wxPuts(wxT("\nAnd now backwards:"));
        for ( s = file.GetLastLine();
              file.GetCurrentLine() != 0;
              s = file.GetPrevLine() )
        {
            wxPrintf(wxT("%6u: %s\n"), file.GetCurrentLine() + 1, s.c_str());
        }
        wxPrintf(wxT("%6u: %s\n"), file.GetCurrentLine() + 1, s.c_str());
    }
    else
    {
        wxPrintf(wxT("ERROR: can't open '%s'\n"), file.GetName());
    }

    wxPuts(wxEmptyString);
}

static void TestFileCopy()
{
    wxPuts(wxT("*** Testing wxCopyFile ***"));

    static const wxChar *filename1 = wxT("testdata.fc");
    static const wxChar *filename2 = wxT("test2");
    if ( !wxCopyFile(filename1, filename2) )
    {
        wxPuts(wxT("ERROR: failed to copy file"));
    }
    else
    {
        wxFFile f1(filename1, wxT("rb")),
                f2(filename2, wxT("rb"));

        if ( !f1.IsOpened() || !f2.IsOpened() )
        {
            wxPuts(wxT("ERROR: failed to open file(s)"));
        }
        else
        {
            wxString s1, s2;
            if ( !f1.ReadAll(&s1) || !f2.ReadAll(&s2) )
            {
                wxPuts(wxT("ERROR: failed to read file(s)"));
            }
            else
            {
                if ( (s1.length() != s2.length()) ||
                     (memcmp(s1.c_str(), s2.c_str(), s1.length()) != 0) )
                {
                    wxPuts(wxT("ERROR: copy error!"));
                }
                else
                {
                    wxPuts(wxT("File was copied ok."));
                }
            }
        }
    }

    if ( !wxRemoveFile(filename2) )
    {
        wxPuts(wxT("ERROR: failed to remove the file"));
    }

    wxPuts(wxEmptyString);
}

static void TestTempFile()
{
    wxPuts(wxT("*** wxTempFile test ***"));

    wxTempFile tmpFile;
    if ( tmpFile.Open(wxT("test2")) && tmpFile.Write(wxT("the answer is 42")) )
    {
        if ( tmpFile.Commit() )
            wxPuts(wxT("File committed."));
        else
            wxPuts(wxT("ERROR: could't commit temp file."));

        wxRemoveFile(wxT("test2"));
    }

    wxPuts(wxEmptyString);
}

#endif // TEST_FILE

// ----------------------------------------------------------------------------
// wxFileConfig
// ----------------------------------------------------------------------------

#ifdef TEST_FILECONF

#include "wx/confbase.h"
#include "wx/fileconf.h"

static const struct FileConfTestData
{
    const wxChar *name;      // value name
    const wxChar *value;     // the value from the file
} fcTestData[] =
{
    { wxT("value1"),                       wxT("one") },
    { wxT("value2"),                       wxT("two") },
    { wxT("novalue"),                      wxT("default") },
};

static void TestFileConfRead()
{
    wxPuts(wxT("*** testing wxFileConfig loading/reading ***"));

    wxFileConfig fileconf(wxT("test"), wxEmptyString,
                          wxT("testdata.fc"), wxEmptyString,
                          wxCONFIG_USE_RELATIVE_PATH);

    // test simple reading
    wxPuts(wxT("\nReading config file:"));
    wxString defValue(wxT("default")), value;
    for ( size_t n = 0; n < WXSIZEOF(fcTestData); n++ )
    {
        const FileConfTestData& data = fcTestData[n];
        value = fileconf.Read(data.name, defValue);
        wxPrintf(wxT("\t%s = %s "), data.name, value.c_str());
        if ( value == data.value )
        {
            wxPuts(wxT("(ok)"));
        }
        else
        {
            wxPrintf(wxT("(ERROR: should be %s)\n"), data.value);
        }
    }

    // test enumerating the entries
    wxPuts(wxT("\nEnumerating all root entries:"));
    long dummy;
    wxString name;
    bool cont = fileconf.GetFirstEntry(name, dummy);
    while ( cont )
    {
        wxPrintf(wxT("\t%s = %s\n"),
               name.c_str(),
               fileconf.Read(name.c_str(), wxT("ERROR")).c_str());

        cont = fileconf.GetNextEntry(name, dummy);
    }

    static const wxChar *testEntry = wxT("TestEntry");
    wxPrintf(wxT("\nTesting deletion of newly created \"Test\" entry: "));
    fileconf.Write(testEntry, wxT("A value"));
    fileconf.DeleteEntry(testEntry);
    wxPrintf(fileconf.HasEntry(testEntry) ? wxT("ERROR\n") : wxT("ok\n"));
}

#endif // TEST_FILECONF

// ----------------------------------------------------------------------------
// wxFileName
// ----------------------------------------------------------------------------

#ifdef TEST_FILENAME

#include "wx/filename.h"

#if 0
static void DumpFileName(const wxChar *desc, const wxFileName& fn)
{
    wxPuts(desc);

    wxString full = fn.GetFullPath();

    wxString vol, path, name, ext;
    wxFileName::SplitPath(full, &vol, &path, &name, &ext);

    wxPrintf(wxT("'%s'-> vol '%s', path '%s', name '%s', ext '%s'\n"),
             full.c_str(), vol.c_str(), path.c_str(), name.c_str(), ext.c_str());

    wxFileName::SplitPath(full, &path, &name, &ext);
    wxPrintf(wxT("or\t\t-> path '%s', name '%s', ext '%s'\n"),
             path.c_str(), name.c_str(), ext.c_str());

    wxPrintf(wxT("path is also:\t'%s'\n"), fn.GetPath().c_str());
    wxPrintf(wxT("with volume: \t'%s'\n"),
             fn.GetPath(wxPATH_GET_VOLUME).c_str());
    wxPrintf(wxT("with separator:\t'%s'\n"),
             fn.GetPath(wxPATH_GET_SEPARATOR).c_str());
    wxPrintf(wxT("with both:   \t'%s'\n"),
             fn.GetPath(wxPATH_GET_SEPARATOR | wxPATH_GET_VOLUME).c_str());

    wxPuts(wxT("The directories in the path are:"));
    wxArrayString dirs = fn.GetDirs();
    size_t count = dirs.GetCount();
    for ( size_t n = 0; n < count; n++ )
    {
        wxPrintf(wxT("\t%u: %s\n"), n, dirs[n].c_str());
    }
}
#endif

static void TestFileNameTemp()
{
    wxPuts(wxT("*** testing wxFileName temp file creation ***"));

    static const wxChar *tmpprefixes[] =
    {
        wxT(""),
        wxT("foo"),
        wxT(".."),
        wxT("../bar"),
#ifdef __UNIX__
        wxT("/tmp/foo"),
        wxT("/tmp/foo/bar"), // this one must be an error
#endif // __UNIX__
    };

    for ( size_t n = 0; n < WXSIZEOF(tmpprefixes); n++ )
    {
        wxString path = wxFileName::CreateTempFileName(tmpprefixes[n]);
        if ( path.empty() )
        {
            // "error" is not in upper case because it may be ok
            wxPrintf(wxT("Prefix '%s'\t-> error\n"), tmpprefixes[n]);
        }
        else
        {
            wxPrintf(wxT("Prefix '%s'\t-> temp file '%s'\n"),
                   tmpprefixes[n], path.c_str());

            if ( !wxRemoveFile(path) )
            {
                wxLogWarning(wxT("Failed to remove temp file '%s'"),
                             path.c_str());
            }
        }
    }
}

static void TestFileNameDirManip()
{
    // TODO: test AppendDir(), RemoveDir(), ...
}

static void TestFileNameComparison()
{
    // TODO!
}

static void TestFileNameOperations()
{
    // TODO!
}

static void TestFileNameCwd()
{
    // TODO!
}

#endif // TEST_FILENAME

// ----------------------------------------------------------------------------
// wxFileName time functions
// ----------------------------------------------------------------------------

#ifdef TEST_FILETIME

#include "wx/filename.h"
#include "wx/datetime.h"

static void TestFileGetTimes()
{
    wxFileName fn(wxT("testdata.fc"));

    wxDateTime dtAccess, dtMod, dtCreate;
    if ( !fn.GetTimes(&dtAccess, &dtMod, &dtCreate) )
    {
        wxPrintf(wxT("ERROR: GetTimes() failed.\n"));
    }
    else
    {
        static const wxChar *fmt = wxT("%Y-%b-%d %H:%M:%S");

        wxPrintf(wxT("File times for '%s':\n"), fn.GetFullPath().c_str());
        wxPrintf(wxT("Creation:    \t%s\n"), dtCreate.Format(fmt).c_str());
        wxPrintf(wxT("Last read:   \t%s\n"), dtAccess.Format(fmt).c_str());
        wxPrintf(wxT("Last write:  \t%s\n"), dtMod.Format(fmt).c_str());
    }
}

#if 0
static void TestFileSetTimes()
{
    wxFileName fn(wxT("testdata.fc"));

    if ( !fn.Touch() )
    {
        wxPrintf(wxT("ERROR: Touch() failed.\n"));
    }
}
#endif

#endif // TEST_FILETIME

// ----------------------------------------------------------------------------
// wxLocale
// ----------------------------------------------------------------------------

#ifdef TEST_LOCALE

#include "wx/intl.h"
#include "wx/utils.h"   // for wxSetEnv

static wxLocale gs_localeDefault;
    // NOTE: don't init it here as it needs a wxAppTraits object
    //       and thus must be init-ed after creation of the wxInitializer
    //       class in the main()

// find the name of the language from its value
static const wxChar *GetLangName(int lang)
{
    static const wxChar *languageNames[] =
    {
        wxT("DEFAULT"),
        wxT("UNKNOWN"),
        wxT("ABKHAZIAN"),
        wxT("AFAR"),
        wxT("AFRIKAANS"),
        wxT("ALBANIAN"),
        wxT("AMHARIC"),
        wxT("ARABIC"),
        wxT("ARABIC_ALGERIA"),
        wxT("ARABIC_BAHRAIN"),
        wxT("ARABIC_EGYPT"),
        wxT("ARABIC_IRAQ"),
        wxT("ARABIC_JORDAN"),
        wxT("ARABIC_KUWAIT"),
        wxT("ARABIC_LEBANON"),
        wxT("ARABIC_LIBYA"),
        wxT("ARABIC_MOROCCO"),
        wxT("ARABIC_OMAN"),
        wxT("ARABIC_QATAR"),
        wxT("ARABIC_SAUDI_ARABIA"),
        wxT("ARABIC_SUDAN"),
        wxT("ARABIC_SYRIA"),
        wxT("ARABIC_TUNISIA"),
        wxT("ARABIC_UAE"),
        wxT("ARABIC_YEMEN"),
        wxT("ARMENIAN"),
        wxT("ASSAMESE"),
        wxT("AYMARA"),
        wxT("AZERI"),
        wxT("AZERI_CYRILLIC"),
        wxT("AZERI_LATIN"),
        wxT("BASHKIR"),
        wxT("BASQUE"),
        wxT("BELARUSIAN"),
        wxT("BENGALI"),
        wxT("BHUTANI"),
        wxT("BIHARI"),
        wxT("BISLAMA"),
        wxT("BRETON"),
        wxT("BULGARIAN"),
        wxT("BURMESE"),
        wxT("CAMBODIAN"),
        wxT("CATALAN"),
        wxT("CHINESE"),
        wxT("CHINESE_SIMPLIFIED"),
        wxT("CHINESE_TRADITIONAL"),
        wxT("CHINESE_HONGKONG"),
        wxT("CHINESE_MACAU"),
        wxT("CHINESE_SINGAPORE"),
        wxT("CHINESE_TAIWAN"),
        wxT("CORSICAN"),
        wxT("CROATIAN"),
        wxT("CZECH"),
        wxT("DANISH"),
        wxT("DUTCH"),
        wxT("DUTCH_BELGIAN"),
        wxT("ENGLISH"),
        wxT("ENGLISH_UK"),
        wxT("ENGLISH_US"),
        wxT("ENGLISH_AUSTRALIA"),
        wxT("ENGLISH_BELIZE"),
        wxT("ENGLISH_BOTSWANA"),
        wxT("ENGLISH_CANADA"),
        wxT("ENGLISH_CARIBBEAN"),
        wxT("ENGLISH_DENMARK"),
        wxT("ENGLISH_EIRE"),
        wxT("ENGLISH_JAMAICA"),
        wxT("ENGLISH_NEW_ZEALAND"),
        wxT("ENGLISH_PHILIPPINES"),
        wxT("ENGLISH_SOUTH_AFRICA"),
        wxT("ENGLISH_TRINIDAD"),
        wxT("ENGLISH_ZIMBABWE"),
        wxT("ESPERANTO"),
        wxT("ESTONIAN"),
        wxT("FAEROESE"),
        wxT("FARSI"),
        wxT("FIJI"),
        wxT("FINNISH"),
        wxT("FRENCH"),
        wxT("FRENCH_BELGIAN"),
        wxT("FRENCH_CANADIAN"),
        wxT("FRENCH_LUXEMBOURG"),
        wxT("FRENCH_MONACO"),
        wxT("FRENCH_SWISS"),
        wxT("FRISIAN"),
        wxT("GALICIAN"),
        wxT("GEORGIAN"),
        wxT("GERMAN"),
        wxT("GERMAN_AUSTRIAN"),
        wxT("GERMAN_BELGIUM"),
        wxT("GERMAN_LIECHTENSTEIN"),
        wxT("GERMAN_LUXEMBOURG"),
        wxT("GERMAN_SWISS"),
        wxT("GREEK"),
        wxT("GREENLANDIC"),
        wxT("GUARANI"),
        wxT("GUJARATI"),
        wxT("HAUSA"),
        wxT("HEBREW"),
        wxT("HINDI"),
        wxT("HUNGARIAN"),
        wxT("ICELANDIC"),
        wxT("INDONESIAN"),
        wxT("INTERLINGUA"),
        wxT("INTERLINGUE"),
        wxT("INUKTITUT"),
        wxT("INUPIAK"),
        wxT("IRISH"),
        wxT("ITALIAN"),
        wxT("ITALIAN_SWISS"),
        wxT("JAPANESE"),
        wxT("JAVANESE"),
        wxT("KANNADA"),
        wxT("KASHMIRI"),
        wxT("KASHMIRI_INDIA"),
        wxT("KAZAKH"),
        wxT("KERNEWEK"),
        wxT("KINYARWANDA"),
        wxT("KIRGHIZ"),
        wxT("KIRUNDI"),
        wxT("KONKANI"),
        wxT("KOREAN"),
        wxT("KURDISH"),
        wxT("LAOTHIAN"),
        wxT("LATIN"),
        wxT("LATVIAN"),
        wxT("LINGALA"),
        wxT("LITHUANIAN"),
        wxT("MACEDONIAN"),
        wxT("MALAGASY"),
        wxT("MALAY"),
        wxT("MALAYALAM"),
        wxT("MALAY_BRUNEI_DARUSSALAM"),
        wxT("MALAY_MALAYSIA"),
        wxT("MALTESE"),
        wxT("MANIPURI"),
        wxT("MAORI"),
        wxT("MARATHI"),
        wxT("MOLDAVIAN"),
        wxT("MONGOLIAN"),
        wxT("NAURU"),
        wxT("NEPALI"),
        wxT("NEPALI_INDIA"),
        wxT("NORWEGIAN_BOKMAL"),
        wxT("NORWEGIAN_NYNORSK"),
        wxT("OCCITAN"),
        wxT("ORIYA"),
        wxT("OROMO"),
        wxT("PASHTO"),
        wxT("POLISH"),
        wxT("PORTUGUESE"),
        wxT("PORTUGUESE_BRAZILIAN"),
        wxT("PUNJABI"),
        wxT("QUECHUA"),
        wxT("RHAETO_ROMANCE"),
        wxT("ROMANIAN"),
        wxT("RUSSIAN"),
        wxT("RUSSIAN_UKRAINE"),
        wxT("SAMOAN"),
        wxT("SANGHO"),
        wxT("SANSKRIT"),
        wxT("SCOTS_GAELIC"),
        wxT("SERBIAN"),
        wxT("SERBIAN_CYRILLIC"),
        wxT("SERBIAN_LATIN"),
        wxT("SERBO_CROATIAN"),
        wxT("SESOTHO"),
        wxT("SETSWANA"),
        wxT("SHONA"),
        wxT("SINDHI"),
        wxT("SINHALESE"),
        wxT("SISWATI"),
        wxT("SLOVAK"),
        wxT("SLOVENIAN"),
        wxT("SOMALI"),
        wxT("SPANISH"),
        wxT("SPANISH_ARGENTINA"),
        wxT("SPANISH_BOLIVIA"),
        wxT("SPANISH_CHILE"),
        wxT("SPANISH_COLOMBIA"),
        wxT("SPANISH_COSTA_RICA"),
        wxT("SPANISH_DOMINICAN_REPUBLIC"),
        wxT("SPANISH_ECUADOR"),
        wxT("SPANISH_EL_SALVADOR"),
        wxT("SPANISH_GUATEMALA"),
        wxT("SPANISH_HONDURAS"),
        wxT("SPANISH_MEXICAN"),
        wxT("SPANISH_MODERN"),
        wxT("SPANISH_NICARAGUA"),
        wxT("SPANISH_PANAMA"),
        wxT("SPANISH_PARAGUAY"),
        wxT("SPANISH_PERU"),
        wxT("SPANISH_PUERTO_RICO"),
        wxT("SPANISH_URUGUAY"),
        wxT("SPANISH_US"),
        wxT("SPANISH_VENEZUELA"),
        wxT("SUNDANESE"),
        wxT("SWAHILI"),
        wxT("SWEDISH"),
        wxT("SWEDISH_FINLAND"),
        wxT("TAGALOG"),
        wxT("TAJIK"),
        wxT("TAMIL"),
        wxT("TATAR"),
        wxT("TELUGU"),
        wxT("THAI"),
        wxT("TIBETAN"),
        wxT("TIGRINYA"),
        wxT("TONGA"),
        wxT("TSONGA"),
        wxT("TURKISH"),
        wxT("TURKMEN"),
        wxT("TWI"),
        wxT("UIGHUR"),
        wxT("UKRAINIAN"),
        wxT("URDU"),
        wxT("URDU_INDIA"),
        wxT("URDU_PAKISTAN"),
        wxT("UZBEK"),
        wxT("UZBEK_CYRILLIC"),
        wxT("UZBEK_LATIN"),
        wxT("VIETNAMESE"),
        wxT("VOLAPUK"),
        wxT("WELSH"),
        wxT("WOLOF"),
        wxT("XHOSA"),
        wxT("YIDDISH"),
        wxT("YORUBA"),
        wxT("ZHUANG"),
        wxT("ZULU"),
    };

    if ( (size_t)lang < WXSIZEOF(languageNames) )
        return languageNames[lang];
    else
        return wxT("INVALID");
}

static void TestDefaultLang()
{
    wxPuts(wxT("*** Testing wxLocale::GetSystemLanguage ***"));

    gs_localeDefault.Init(wxLANGUAGE_ENGLISH);

    static const wxChar *langStrings[] =
    {
        NULL,               // system default
        wxT("C"),
        wxT("fr"),
        wxT("fr_FR"),
        wxT("en"),
        wxT("en_GB"),
        wxT("en_US"),
        wxT("de_DE.iso88591"),
        wxT("german"),
        wxT("?"),            // invalid lang spec
        wxT("klingonese"),   // I bet on some systems it does exist...
    };

    wxPrintf(wxT("The default system encoding is %s (%d)\n"),
             wxLocale::GetSystemEncodingName().c_str(),
             wxLocale::GetSystemEncoding());

    for ( size_t n = 0; n < WXSIZEOF(langStrings); n++ )
    {
        const wxChar *langStr = langStrings[n];
        if ( langStr )
        {
            // FIXME: this doesn't do anything at all under Windows, we need
            //        to create a new wxLocale!
            wxSetEnv(wxT("LC_ALL"), langStr);
        }

        int lang = gs_localeDefault.GetSystemLanguage();
        wxPrintf(wxT("Locale for '%s' is %s.\n"),
                 langStr ? langStr : wxT("system default"), GetLangName(lang));
    }
}

#endif // TEST_LOCALE

// ----------------------------------------------------------------------------
// MIME types
// ----------------------------------------------------------------------------

#ifdef TEST_MIME

#include "wx/mimetype.h"

static void TestMimeEnum()
{
    wxPuts(wxT("*** Testing wxMimeTypesManager::EnumAllFileTypes() ***\n"));

    wxArrayString mimetypes;

    size_t count = wxTheMimeTypesManager->EnumAllFileTypes(mimetypes);

    wxPrintf(wxT("*** All %u known filetypes: ***\n"), count);

    wxArrayString exts;
    wxString desc;

    for ( size_t n = 0; n < count; n++ )
    {
        wxFileType *filetype =
            wxTheMimeTypesManager->GetFileTypeFromMimeType(mimetypes[n]);
        if ( !filetype )
        {
            wxPrintf(wxT("nothing known about the filetype '%s'!\n"),
                   mimetypes[n].c_str());
            continue;
        }

        filetype->GetDescription(&desc);
        filetype->GetExtensions(exts);

        filetype->GetIcon(NULL);

        wxString extsAll;
        for ( size_t e = 0; e < exts.GetCount(); e++ )
        {
            if ( e > 0 )
                extsAll << wxT(", ");
            extsAll += exts[e];
        }

        wxPrintf(wxT("\t%s: %s (%s)\n"),
               mimetypes[n].c_str(), desc.c_str(), extsAll.c_str());
    }

    wxPuts(wxEmptyString);
}

static void TestMimeFilename()
{
    wxPuts(wxT("*** Testing MIME type from filename query ***\n"));

    static const wxChar *filenames[] =
    {
        wxT("readme.txt"),
        wxT("document.pdf"),
        wxT("image.gif"),
        wxT("picture.jpeg"),
    };

    for ( size_t n = 0; n < WXSIZEOF(filenames); n++ )
    {
        const wxString fname = filenames[n];
        wxString ext = fname.AfterLast(wxT('.'));
        wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(ext);
        if ( !ft )
        {
            wxPrintf(wxT("WARNING: extension '%s' is unknown.\n"), ext.c_str());
        }
        else
        {
            wxString desc;
            if ( !ft->GetDescription(&desc) )
                desc = wxT("<no description>");

            wxString cmd;
            if ( !ft->GetOpenCommand(&cmd,
                                     wxFileType::MessageParameters(fname, wxEmptyString)) )
                cmd = wxT("<no command available>");
            else
                cmd = wxString(wxT('"')) + cmd + wxT('"');

            wxPrintf(wxT("To open %s (%s) do %s.\n"),
                     fname.c_str(), desc.c_str(), cmd.c_str());

            delete ft;
        }
    }

    wxPuts(wxEmptyString);
}

// these tests were broken by wxMimeTypesManager changes, temporarily disabling
#if 0

static void TestMimeOverride()
{
    wxPuts(wxT("*** Testing wxMimeTypesManager additional files loading ***\n"));

    static const wxChar *mailcap = wxT("/tmp/mailcap");
    static const wxChar *mimetypes = wxT("/tmp/mime.types");

    if ( wxFile::Exists(mailcap) )
        wxPrintf(wxT("Loading mailcap from '%s': %s\n"),
                 mailcap,
                 wxTheMimeTypesManager->ReadMailcap(mailcap) ? wxT("ok") : wxT("ERROR"));
    else
        wxPrintf(wxT("WARN: mailcap file '%s' doesn't exist, not loaded.\n"),
                 mailcap);

    if ( wxFile::Exists(mimetypes) )
        wxPrintf(wxT("Loading mime.types from '%s': %s\n"),
                 mimetypes,
                 wxTheMimeTypesManager->ReadMimeTypes(mimetypes) ? wxT("ok") : wxT("ERROR"));
    else
        wxPrintf(wxT("WARN: mime.types file '%s' doesn't exist, not loaded.\n"),
                 mimetypes);

    wxPuts(wxEmptyString);
}

static void TestMimeAssociate()
{
    wxPuts(wxT("*** Testing creation of filetype association ***\n"));

    wxFileTypeInfo ftInfo(
                            wxT("application/x-xyz"),
                            wxT("xyzview '%s'"), // open cmd
                            wxT(""),             // print cmd
                            wxT("XYZ File"),     // description
                            wxT(".xyz"),         // extensions
                            wxNullPtr           // end of extensions
                         );
    ftInfo.SetShortDesc(wxT("XYZFile")); // used under Win32 only

    wxFileType *ft = wxTheMimeTypesManager->Associate(ftInfo);
    if ( !ft )
    {
        wxPuts(wxT("ERROR: failed to create association!"));
    }
    else
    {
        // TODO: read it back
        delete ft;
    }

    wxPuts(wxEmptyString);
}

#endif // 0

#endif // TEST_MIME

// ----------------------------------------------------------------------------
// module dependencies feature
// ----------------------------------------------------------------------------

#ifdef TEST_MODULE

#include "wx/module.h"

class wxTestModule : public wxModule
{
protected:
    virtual bool OnInit() { wxPrintf(wxT("Load module: %s\n"), GetClassInfo()->GetClassName()); return true; }
    virtual void OnExit() { wxPrintf(wxT("Unload module: %s\n"), GetClassInfo()->GetClassName()); }
};

class wxTestModuleA : public wxTestModule
{
public:
    wxTestModuleA();
private:
    DECLARE_DYNAMIC_CLASS(wxTestModuleA)
};

class wxTestModuleB : public wxTestModule
{
public:
    wxTestModuleB();
private:
    DECLARE_DYNAMIC_CLASS(wxTestModuleB)
};

class wxTestModuleC : public wxTestModule
{
public:
    wxTestModuleC();
private:
    DECLARE_DYNAMIC_CLASS(wxTestModuleC)
};

class wxTestModuleD : public wxTestModule
{
public:
    wxTestModuleD();
private:
    DECLARE_DYNAMIC_CLASS(wxTestModuleD)
};

IMPLEMENT_DYNAMIC_CLASS(wxTestModuleC, wxModule)
wxTestModuleC::wxTestModuleC()
{
    AddDependency(CLASSINFO(wxTestModuleD));
}

IMPLEMENT_DYNAMIC_CLASS(wxTestModuleA, wxModule)
wxTestModuleA::wxTestModuleA()
{
    AddDependency(CLASSINFO(wxTestModuleB));
    AddDependency(CLASSINFO(wxTestModuleD));
}

IMPLEMENT_DYNAMIC_CLASS(wxTestModuleD, wxModule)
wxTestModuleD::wxTestModuleD()
{
}

IMPLEMENT_DYNAMIC_CLASS(wxTestModuleB, wxModule)
wxTestModuleB::wxTestModuleB()
{
    AddDependency(CLASSINFO(wxTestModuleD));
    AddDependency(CLASSINFO(wxTestModuleC));
}

#endif // TEST_MODULE

// ----------------------------------------------------------------------------
// misc information functions
// ----------------------------------------------------------------------------

#ifdef TEST_INFO_FUNCTIONS

#include "wx/utils.h"

#if TEST_INTERACTIVE
static void TestDiskInfo()
{
    wxPuts(wxT("*** Testing wxGetDiskSpace() ***"));

    for ( ;; )
    {
        wxChar pathname[128];
        wxPrintf(wxT("\nEnter a directory name: "));
        if ( !wxFgets(pathname, WXSIZEOF(pathname), stdin) )
            break;

        // kill the last '\n'
        pathname[wxStrlen(pathname) - 1] = 0;

        wxLongLong total, free;
        if ( !wxGetDiskSpace(pathname, &total, &free) )
        {
            wxPuts(wxT("ERROR: wxGetDiskSpace failed."));
        }
        else
        {
            wxPrintf(wxT("%sKb total, %sKb free on '%s'.\n"),
                    (total / 1024).ToString().c_str(),
                    (free / 1024).ToString().c_str(),
                    pathname);
        }
    }
}
#endif // TEST_INTERACTIVE

static void TestOsInfo()
{
    wxPuts(wxT("*** Testing OS info functions ***\n"));

    int major, minor;
    wxGetOsVersion(&major, &minor);
    wxPrintf(wxT("Running under: %s, version %d.%d\n"),
            wxGetOsDescription().c_str(), major, minor);

    wxPrintf(wxT("%ld free bytes of memory left.\n"), wxGetFreeMemory().ToLong());

    wxPrintf(wxT("Host name is %s (%s).\n"),
           wxGetHostName().c_str(), wxGetFullHostName().c_str());

    wxPuts(wxEmptyString);
}

static void TestPlatformInfo()
{
    wxPuts(wxT("*** Testing wxPlatformInfo functions ***\n"));

    // get this platform
    wxPlatformInfo plat;

    wxPrintf(wxT("Operating system family name is: %s\n"), plat.GetOperatingSystemFamilyName().c_str());
    wxPrintf(wxT("Operating system name is: %s\n"), plat.GetOperatingSystemIdName().c_str());
    wxPrintf(wxT("Port ID name is: %s\n"), plat.GetPortIdName().c_str());
    wxPrintf(wxT("Port ID short name is: %s\n"), plat.GetPortIdShortName().c_str());
    wxPrintf(wxT("Architecture is: %s\n"), plat.GetArchName().c_str());
    wxPrintf(wxT("Endianness is: %s\n"), plat.GetEndiannessName().c_str());

    wxPuts(wxEmptyString);
}

static void TestUserInfo()
{
    wxPuts(wxT("*** Testing user info functions ***\n"));

    wxPrintf(wxT("User id is:\t%s\n"), wxGetUserId().c_str());
    wxPrintf(wxT("User name is:\t%s\n"), wxGetUserName().c_str());
    wxPrintf(wxT("Home dir is:\t%s\n"), wxGetHomeDir().c_str());
    wxPrintf(wxT("Email address:\t%s\n"), wxGetEmailAddress().c_str());

    wxPuts(wxEmptyString);
}

#endif // TEST_INFO_FUNCTIONS

// ----------------------------------------------------------------------------
// path list
// ----------------------------------------------------------------------------

#ifdef TEST_PATHLIST

#ifdef __UNIX__
    #define CMD_IN_PATH wxT("ls")
#else
    #define CMD_IN_PATH wxT("command.com")
#endif

static void TestPathList()
{
    wxPuts(wxT("*** Testing wxPathList ***\n"));

    wxPathList pathlist;
    pathlist.AddEnvList(wxT("PATH"));
    wxString path = pathlist.FindValidPath(CMD_IN_PATH);
    if ( path.empty() )
    {
        wxPrintf(wxT("ERROR: command not found in the path.\n"));
    }
    else
    {
        wxPrintf(wxT("Command found in the path as '%s'.\n"), path.c_str());
    }
}

#endif // TEST_PATHLIST

// ----------------------------------------------------------------------------
// regular expressions
// ----------------------------------------------------------------------------

#if defined TEST_REGEX && TEST_INTERACTIVE

#include "wx/regex.h"

static void TestRegExInteractive()
{
    wxPuts(wxT("*** Testing RE interactively ***"));

    for ( ;; )
    {
        wxChar pattern[128];
        wxPrintf(wxT("\nEnter a pattern: "));
        if ( !wxFgets(pattern, WXSIZEOF(pattern), stdin) )
            break;

        // kill the last '\n'
        pattern[wxStrlen(pattern) - 1] = 0;

        wxRegEx re;
        if ( !re.Compile(pattern) )
        {
            continue;
        }

        wxChar text[128];
        for ( ;; )
        {
            wxPrintf(wxT("Enter text to match: "));
            if ( !wxFgets(text, WXSIZEOF(text), stdin) )
                break;

            // kill the last '\n'
            text[wxStrlen(text) - 1] = 0;

            if ( !re.Matches(text) )
            {
                wxPrintf(wxT("No match.\n"));
            }
            else
            {
                wxPrintf(wxT("Pattern matches at '%s'\n"), re.GetMatch(text).c_str());

                size_t start, len;
                for ( size_t n = 1; ; n++ )
                {
                    if ( !re.GetMatch(&start, &len, n) )
                    {
                        break;
                    }

                    wxPrintf(wxT("Subexpr %u matched '%s'\n"),
                             n, wxString(text + start, len).c_str());
                }
            }
        }
    }
}

#endif // TEST_REGEX

// ----------------------------------------------------------------------------
// printf() tests
// ----------------------------------------------------------------------------

/*
   NB: this stuff was taken from the glibc test suite and modified to build
       in wxWidgets: if I read the copyright below properly, this shouldn't
       be a problem
 */

#ifdef TEST_PRINTF

#ifdef wxTEST_PRINTF
    // use our functions from wxchar.cpp
    #undef wxPrintf
    #undef wxSprintf

    // NB: do _not_ use WX_ATTRIBUTE_PRINTF here, we have some invalid formats
    //     in the tests below
    int wxPrintf( const wxChar *format, ... );
    int wxSprintf( wxChar *str, const wxChar *format, ... );
#endif

#include "wx/longlong.h"

#include <float.h>

static void rfg1 (void);
static void rfg2 (void);


static void
fmtchk (const wxChar *fmt)
{
  (void) wxPrintf(wxT("%s:\t`"), fmt);
  (void) wxPrintf(fmt, 0x12);
  (void) wxPrintf(wxT("'\n"));
}

static void
fmtst1chk (const wxChar *fmt)
{
  (void) wxPrintf(wxT("%s:\t`"), fmt);
  (void) wxPrintf(fmt, 4, 0x12);
  (void) wxPrintf(wxT("'\n"));
}

static void
fmtst2chk (const wxChar *fmt)
{
  (void) wxPrintf(wxT("%s:\t`"), fmt);
  (void) wxPrintf(fmt, 4, 4, 0x12);
  (void) wxPrintf(wxT("'\n"));
}

/* This page is covered by the following copyright: */

/* (C) Copyright C E Chew
 *
 * Feel free to copy, use and distribute this software provided:
 *
 *        1. you do not pretend that you wrote it
 *        2. you leave this copyright notice intact.
 */

/*
 * Extracted from exercise.c for glibc-1.05 bug report by Bruce Evans.
 */

#define DEC -123
#define INT 255
#define UNS (~0)

/* Formatted Output Test
 *
 * This exercises the output formatting code.
 */

wxChar *PointerNull = NULL;

static void
fp_test (void)
{
  int i, j, k, l;
  wxChar buf[7];
  wxChar *prefix = buf;
  wxChar tp[20];

  wxPuts(wxT("\nFormatted output test"));
  wxPrintf(wxT("prefix  6d      6o      6x      6X      6u\n"));
  wxStrcpy(prefix, wxT("%"));
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 2; j++) {
      for (k = 0; k < 2; k++) {
        for (l = 0; l < 2; l++) {
          wxStrcpy(prefix, wxT("%"));
          if (i == 0) wxStrcat(prefix, wxT("-"));
          if (j == 0) wxStrcat(prefix, wxT("+"));
          if (k == 0) wxStrcat(prefix, wxT("#"));
          if (l == 0) wxStrcat(prefix, wxT("0"));
          wxPrintf(wxT("%5s |"), prefix);
          wxStrcpy(tp, prefix);
          wxStrcat(tp, wxT("6d |"));
          wxPrintf(tp, DEC);
          wxStrcpy(tp, prefix);
          wxStrcat(tp, wxT("6o |"));
          wxPrintf(tp, INT);
          wxStrcpy(tp, prefix);
          wxStrcat(tp, wxT("6x |"));
          wxPrintf(tp, INT);
          wxStrcpy(tp, prefix);
          wxStrcat(tp, wxT("6X |"));
          wxPrintf(tp, INT);
          wxStrcpy(tp, prefix);
          wxStrcat(tp, wxT("6u |"));
          wxPrintf(tp, UNS);
          wxPrintf(wxT("\n"));
        }
      }
    }
  }
  wxPrintf(wxT("%10s\n"), PointerNull);
  wxPrintf(wxT("%-10s\n"), PointerNull);
}

static void TestPrintf()
{
  static wxChar shortstr[] = wxT("Hi, Z.");
  static wxChar longstr[] = wxT("Good morning, Doctor Chandra.  This is Hal.  \
I am ready for my first lesson today.");
  int result = 0;
  wxString test_format;

  fmtchk(wxT("%.4x"));
  fmtchk(wxT("%04x"));
  fmtchk(wxT("%4.4x"));
  fmtchk(wxT("%04.4x"));
  fmtchk(wxT("%4.3x"));
  fmtchk(wxT("%04.3x"));

  fmtst1chk(wxT("%.*x"));
  fmtst1chk(wxT("%0*x"));
  fmtst2chk(wxT("%*.*x"));
  fmtst2chk(wxT("%0*.*x"));

  wxString bad_format = wxT("bad format:\t\"%b\"\n");
  wxPrintf(bad_format.c_str());
  wxPrintf(wxT("nil pointer (padded):\t\"%10p\"\n"), (void *) NULL);

  wxPrintf(wxT("decimal negative:\t\"%d\"\n"), -2345);
  wxPrintf(wxT("octal negative:\t\"%o\"\n"), -2345);
  wxPrintf(wxT("hex negative:\t\"%x\"\n"), -2345);
  wxPrintf(wxT("long decimal number:\t\"%ld\"\n"), -123456L);
  wxPrintf(wxT("long octal negative:\t\"%lo\"\n"), -2345L);
  wxPrintf(wxT("long unsigned decimal number:\t\"%lu\"\n"), -123456L);
  wxPrintf(wxT("zero-padded LDN:\t\"%010ld\"\n"), -123456L);
  test_format = wxT("left-adjusted ZLDN:\t\"%-010ld\"\n");
  wxPrintf(test_format.c_str(), -123456);
  wxPrintf(wxT("space-padded LDN:\t\"%10ld\"\n"), -123456L);
  wxPrintf(wxT("left-adjusted SLDN:\t\"%-10ld\"\n"), -123456L);

  test_format = wxT("zero-padded string:\t\"%010s\"\n");
  wxPrintf(test_format.c_str(), shortstr);
  test_format = wxT("left-adjusted Z string:\t\"%-010s\"\n");
  wxPrintf(test_format.c_str(), shortstr);
  wxPrintf(wxT("space-padded string:\t\"%10s\"\n"), shortstr);
  wxPrintf(wxT("left-adjusted S string:\t\"%-10s\"\n"), shortstr);
  wxPrintf(wxT("null string:\t\"%s\"\n"), PointerNull);
  wxPrintf(wxT("limited string:\t\"%.22s\"\n"), longstr);

  wxPrintf(wxT("e-style >= 1:\t\"%e\"\n"), 12.34);
  wxPrintf(wxT("e-style >= .1:\t\"%e\"\n"), 0.1234);
  wxPrintf(wxT("e-style < .1:\t\"%e\"\n"), 0.001234);
  wxPrintf(wxT("e-style big:\t\"%.60e\"\n"), 1e20);
  wxPrintf(wxT("e-style == .1:\t\"%e\"\n"), 0.1);
  wxPrintf(wxT("f-style >= 1:\t\"%f\"\n"), 12.34);
  wxPrintf(wxT("f-style >= .1:\t\"%f\"\n"), 0.1234);
  wxPrintf(wxT("f-style < .1:\t\"%f\"\n"), 0.001234);
  wxPrintf(wxT("g-style >= 1:\t\"%g\"\n"), 12.34);
  wxPrintf(wxT("g-style >= .1:\t\"%g\"\n"), 0.1234);
  wxPrintf(wxT("g-style < .1:\t\"%g\"\n"), 0.001234);
  wxPrintf(wxT("g-style big:\t\"%.60g\"\n"), 1e20);

  wxPrintf (wxT(" %6.5f\n"), .099999999860301614);
  wxPrintf (wxT(" %6.5f\n"), .1);
  wxPrintf (wxT("x%5.4fx\n"), .5);

  wxPrintf (wxT("%#03x\n"), 1);

  //wxPrintf (wxT("something really insane: %.10000f\n"), 1.0);

  {
    double d = FLT_MIN;
    int niter = 17;

    while (niter-- != 0)
      wxPrintf (wxT("%.17e\n"), d / 2);
    fflush (stdout);
  }

#ifndef __WATCOMC__
  // Open Watcom cause compiler error here
  // Error! E173: col(24) floating-point constant too small to represent
  wxPrintf (wxT("%15.5e\n"), 4.9406564584124654e-324);
#endif

#define FORMAT wxT("|%12.4f|%12.4e|%12.4g|\n")
  wxPrintf (FORMAT, 0.0, 0.0, 0.0);
  wxPrintf (FORMAT, 1.0, 1.0, 1.0);
  wxPrintf (FORMAT, -1.0, -1.0, -1.0);
  wxPrintf (FORMAT, 100.0, 100.0, 100.0);
  wxPrintf (FORMAT, 1000.0, 1000.0, 1000.0);
  wxPrintf (FORMAT, 10000.0, 10000.0, 10000.0);
  wxPrintf (FORMAT, 12345.0, 12345.0, 12345.0);
  wxPrintf (FORMAT, 100000.0, 100000.0, 100000.0);
  wxPrintf (FORMAT, 123456.0, 123456.0, 123456.0);
#undef        FORMAT

  {
    wxChar buf[20];
    int rc = wxSnprintf (buf, WXSIZEOF(buf), wxT("%30s"), wxT("foo"));

    wxPrintf(wxT("snprintf (\"%%30s\", \"foo\") == %d, \"%.*s\"\n"),
             rc, WXSIZEOF(buf), buf);
#if 0
    wxChar buf2[512];
    wxPrintf ("snprintf (\"%%.999999u\", 10)\n",
            wxSnprintf(buf2, WXSIZEOFbuf2), "%.999999u", 10));
#endif
  }

  fp_test ();

  wxPrintf (wxT("%e should be 1.234568e+06\n"), 1234567.8);
  wxPrintf (wxT("%f should be 1234567.800000\n"), 1234567.8);
  wxPrintf (wxT("%g should be 1.23457e+06\n"), 1234567.8);
  wxPrintf (wxT("%g should be 123.456\n"), 123.456);
  wxPrintf (wxT("%g should be 1e+06\n"), 1000000.0);
  wxPrintf (wxT("%g should be 10\n"), 10.0);
  wxPrintf (wxT("%g should be 0.02\n"), 0.02);

  {
    double x=1.0;
    wxPrintf(wxT("%.17f\n"),(1.0/x/10.0+1.0)*x-x);
  }

  {
    wxChar buf[200];

    wxSprintf(buf,wxT("%*s%*s%*s"),-1,wxT("one"),-20,wxT("two"),-30,wxT("three"));

    result |= wxStrcmp (buf,
                      wxT("onetwo                 three                         "));

    wxPuts (result != 0 ? wxT("Test failed!") : wxT("Test ok."));
  }

#ifdef wxLongLong_t
  {
      wxChar buf[200];

      wxSprintf(buf, "%07" wxLongLongFmtSpec "o", wxLL(040000000000));
      #if 0
        // for some reason below line fails under Borland
      wxPrintf (wxT("sprintf (buf, \"%%07Lo\", 040000000000ll) = %s"), buf);
      #endif

      if (wxStrcmp (buf, wxT("40000000000")) != 0)
      {
          result = 1;
          wxPuts (wxT("\tFAILED"));
      }
      wxUnusedVar(result);
      wxPuts (wxEmptyString);
  }
#endif // wxLongLong_t

  wxPrintf (wxT("printf (\"%%hhu\", %u) = %hhu\n"), UCHAR_MAX + 2, UCHAR_MAX + 2);
  wxPrintf (wxT("printf (\"%%hu\", %u) = %hu\n"), USHRT_MAX + 2, USHRT_MAX + 2);

  wxPuts (wxT("--- Should be no further output. ---"));
  rfg1 ();
  rfg2 ();

#if 0
  {
    wxChar bytes[7];
    wxChar buf[20];

    memset (bytes, '\xff', sizeof bytes);
    wxSprintf (buf, wxT("foo%hhn\n"), &bytes[3]);
    if (bytes[0] != '\xff' || bytes[1] != '\xff' || bytes[2] != '\xff'
        || bytes[4] != '\xff' || bytes[5] != '\xff' || bytes[6] != '\xff')
      {
        wxPuts (wxT("%hhn overwrite more bytes"));
        result = 1;
      }
    if (bytes[3] != 3)
      {
        wxPuts (wxT("%hhn wrote incorrect value"));
        result = 1;
      }
  }
#endif
}

static void
rfg1 (void)
{
  wxChar buf[100];

  wxSprintf (buf, wxT("%5.s"), wxT("xyz"));
  if (wxStrcmp (buf, wxT("     ")) != 0)
    wxPrintf (wxT("got: '%s', expected: '%s'\n"), buf, wxT("     "));
  wxSprintf (buf, wxT("%5.f"), 33.3);
  if (wxStrcmp (buf, wxT("   33")) != 0)
    wxPrintf (wxT("got: '%s', expected: '%s'\n"), buf, wxT("   33"));
  wxSprintf (buf, wxT("%8.e"), 33.3e7);
  if (wxStrcmp (buf, wxT("   3e+08")) != 0)
    wxPrintf (wxT("got: '%s', expected: '%s'\n"), buf, wxT("   3e+08"));
  wxSprintf (buf, wxT("%8.E"), 33.3e7);
  if (wxStrcmp (buf, wxT("   3E+08")) != 0)
    wxPrintf (wxT("got: '%s', expected: '%s'\n"), buf, wxT("   3E+08"));
  wxSprintf (buf, wxT("%.g"), 33.3);
  if (wxStrcmp (buf, wxT("3e+01")) != 0)
    wxPrintf (wxT("got: '%s', expected: '%s'\n"), buf, wxT("3e+01"));
  wxSprintf (buf, wxT("%.G"), 33.3);
  if (wxStrcmp (buf, wxT("3E+01")) != 0)
    wxPrintf (wxT("got: '%s', expected: '%s'\n"), buf, wxT("3E+01"));
}

static void
rfg2 (void)
{
  int prec;
  wxChar buf[100];
  wxString test_format;

  prec = 0;
  wxSprintf (buf, wxT("%.*g"), prec, 3.3);
  if (wxStrcmp (buf, wxT("3")) != 0)
    wxPrintf (wxT("got: '%s', expected: '%s'\n"), buf, wxT("3"));
  prec = 0;
  wxSprintf (buf, wxT("%.*G"), prec, 3.3);
  if (wxStrcmp (buf, wxT("3")) != 0)
    wxPrintf (wxT("got: '%s', expected: '%s'\n"), buf, wxT("3"));
  prec = 0;
  wxSprintf (buf, wxT("%7.*G"), prec, 3.33);
  if (wxStrcmp (buf, wxT("      3")) != 0)
    wxPrintf (wxT("got: '%s', expected: '%s'\n"), buf, wxT("      3"));
  prec = 3;
  test_format = wxT("%04.*o");
  wxSprintf (buf, test_format.c_str(), prec, 33);
  if (wxStrcmp (buf, wxT(" 041")) != 0)
    wxPrintf (wxT("got: '%s', expected: '%s'\n"), buf, wxT(" 041"));
  prec = 7;
  test_format = wxT("%09.*u");
  wxSprintf (buf, test_format.c_str(), prec, 33);
  if (wxStrcmp (buf, wxT("  0000033")) != 0)
    wxPrintf (wxT("got: '%s', expected: '%s'\n"), buf, wxT("  0000033"));
  prec = 3;
  test_format = wxT("%04.*x");
  wxSprintf (buf, test_format.c_str(), prec, 33);
  if (wxStrcmp (buf, wxT(" 021")) != 0)
    wxPrintf (wxT("got: '%s', expected: '%s'\n"), buf, wxT(" 021"));
  prec = 3;
  test_format = wxT("%04.*X");
  wxSprintf (buf, test_format.c_str(), prec, 33);
  if (wxStrcmp (buf, wxT(" 021")) != 0)
    wxPrintf (wxT("got: '%s', expected: '%s'\n"), buf, wxT(" 021"));
}

#endif // TEST_PRINTF

// ----------------------------------------------------------------------------
// FTP
// ----------------------------------------------------------------------------

#ifdef TEST_FTP

#include "wx/protocol/ftp.h"
#include "wx/protocol/log.h"

#define FTP_ANONYMOUS

static wxFTP *ftp;

#ifdef FTP_ANONYMOUS
    static const wxChar *hostname = wxT("ftp.wxwidgets.org");
    static const wxChar *directory = wxT("/pub");
    static const wxChar *filename = wxT("welcome.msg");
#else
    static const wxChar *hostname = "localhost";
    static const wxChar *directory = wxT("/etc");
    static const wxChar *filename = wxT("issue");
#endif

static bool TestFtpConnect()
{
    wxPuts(wxT("*** Testing FTP connect ***"));

#ifdef FTP_ANONYMOUS
    wxPrintf(wxT("--- Attempting to connect to %s:21 anonymously...\n"), hostname);
#else // !FTP_ANONYMOUS
    wxChar user[256];
    wxFgets(user, WXSIZEOF(user), stdin);
    user[wxStrlen(user) - 1] = '\0'; // chop off '\n'
    ftp->SetUser(user);

    wxChar password[256];
    wxPrintf(wxT("Password for %s: "), password);
    wxFgets(password, WXSIZEOF(password), stdin);
    password[wxStrlen(password) - 1] = '\0'; // chop off '\n'
    ftp->SetPassword(password);

    wxPrintf(wxT("--- Attempting to connect to %s:21 as %s...\n"), hostname, user);
#endif // FTP_ANONYMOUS/!FTP_ANONYMOUS

    if ( !ftp->Connect(hostname) )
    {
        wxPrintf(wxT("ERROR: failed to connect to %s\n"), hostname);

        return false;
    }
    else
    {
        wxPrintf(wxT("--- Connected to %s, current directory is '%s'\n"),
                 hostname, ftp->Pwd().c_str());
        ftp->Close();
    }

    return true;
}

#if TEST_INTERACTIVE
static void TestFtpInteractive()
{
    wxPuts(wxT("\n*** Interactive wxFTP test ***"));

    wxChar buf[128];

    for ( ;; )
    {
        wxPrintf(wxT("Enter FTP command (or 'quit' to escape): "));
        if ( !wxFgets(buf, WXSIZEOF(buf), stdin) )
            break;

        // kill the last '\n'
        buf[wxStrlen(buf) - 1] = 0;

        // special handling of LIST and NLST as they require data connection
        wxString start(buf, 4);
        start.MakeUpper();
        if ( start == wxT("LIST") || start == wxT("NLST") )
        {
            wxString wildcard;
            if ( wxStrlen(buf) > 4 )
                wildcard = buf + 5;

            wxArrayString files;
            if ( !ftp->GetList(files, wildcard, start == wxT("LIST")) )
            {
                wxPrintf(wxT("ERROR: failed to get %s of files\n"), start.c_str());
            }
            else
            {
                wxPrintf(wxT("--- %s of '%s' under '%s':\n"),
                       start.c_str(), wildcard.c_str(), ftp->Pwd().c_str());
                size_t count = files.GetCount();
                for ( size_t n = 0; n < count; n++ )
                {
                    wxPrintf(wxT("\t%s\n"), files[n].c_str());
                }
                wxPuts(wxT("--- End of the file list"));
            }
        }
        else if ( start == wxT("QUIT") )
        {
            break;      // get out of here!
        }
        else // !list
        {
            wxChar ch = ftp->SendCommand(buf);
            wxPrintf(wxT("Command %s"), ch ? wxT("succeeded") : wxT("failed"));
            if ( ch )
            {
                wxPrintf(wxT(" (return code %c)"), ch);
            }

            wxPrintf(wxT(", server reply:\n%s\n\n"), ftp->GetLastResult().c_str());
        }
    }

    wxPuts(wxT("\n"));
}
#endif // TEST_INTERACTIVE
#endif // TEST_FTP

// ----------------------------------------------------------------------------
// stack backtrace
// ----------------------------------------------------------------------------

#ifdef TEST_STACKWALKER

#if wxUSE_STACKWALKER

#include "wx/stackwalk.h"

class StackDump : public wxStackWalker
{
public:
    StackDump(const char *argv0)
        : wxStackWalker(argv0)
    {
    }

    virtual void Walk(size_t skip = 1, size_t maxdepth = wxSTACKWALKER_MAX_DEPTH)
    {
        wxPuts(wxT("Stack dump:"));

        wxStackWalker::Walk(skip, maxdepth);
    }

protected:
    virtual void OnStackFrame(const wxStackFrame& frame)
    {
        printf("[%2d] ", (int) frame.GetLevel());

        wxString name = frame.GetName();
        if ( !name.empty() )
        {
            printf("%-20.40s", (const char*)name.mb_str());
        }
        else
        {
            printf("0x%08lx", (unsigned long)frame.GetAddress());
        }

        if ( frame.HasSourceLocation() )
        {
            printf("\t%s:%d",
                   (const char*)frame.GetFileName().mb_str(),
                   (int)frame.GetLine());
        }

        puts("");

        wxString type, val;
        for ( size_t n = 0; frame.GetParam(n, &type, &name, &val); n++ )
        {
            printf("\t%s %s = %s\n", (const char*)type.mb_str(),
                                     (const char*)name.mb_str(),
                                     (const char*)val.mb_str());
        }
    }
};

static void TestStackWalk(const char *argv0)
{
    wxPuts(wxT("*** Testing wxStackWalker ***"));

    StackDump dump(argv0);
    dump.Walk();
    
    wxPuts("\n");
}

#endif // wxUSE_STACKWALKER

#endif // TEST_STACKWALKER

// ----------------------------------------------------------------------------
// standard paths
// ----------------------------------------------------------------------------

#ifdef TEST_STDPATHS

#include "wx/stdpaths.h"
#include "wx/wxchar.h"      // wxPrintf

static void TestStandardPaths()
{
    wxPuts(wxT("*** Testing wxStandardPaths ***"));

    wxTheApp->SetAppName(wxT("console"));

    wxStandardPathsBase& stdp = wxStandardPaths::Get();
    wxPrintf(wxT("Config dir (sys):\t%s\n"), stdp.GetConfigDir().c_str());
    wxPrintf(wxT("Config dir (user):\t%s\n"), stdp.GetUserConfigDir().c_str());
    wxPrintf(wxT("Data dir (sys):\t\t%s\n"), stdp.GetDataDir().c_str());
    wxPrintf(wxT("Data dir (sys local):\t%s\n"), stdp.GetLocalDataDir().c_str());
    wxPrintf(wxT("Data dir (user):\t%s\n"), stdp.GetUserDataDir().c_str());
    wxPrintf(wxT("Data dir (user local):\t%s\n"), stdp.GetUserLocalDataDir().c_str());
    wxPrintf(wxT("Documents dir:\t\t%s\n"), stdp.GetDocumentsDir().c_str());
    wxPrintf(wxT("Executable path:\t%s\n"), stdp.GetExecutablePath().c_str());
    wxPrintf(wxT("Plugins dir:\t\t%s\n"), stdp.GetPluginsDir().c_str());
    wxPrintf(wxT("Resources dir:\t\t%s\n"), stdp.GetResourcesDir().c_str());
    wxPrintf(wxT("Localized res. dir:\t%s\n"),
             stdp.GetLocalizedResourcesDir(wxT("fr")).c_str());
    wxPrintf(wxT("Message catalogs dir:\t%s\n"),
             stdp.GetLocalizedResourcesDir
                  (
                    wxT("fr"),
                    wxStandardPaths::ResourceCat_Messages
                  ).c_str());
                  
    wxPuts("\n");
}

#endif // TEST_STDPATHS

// ----------------------------------------------------------------------------
// wxVolume tests
// ----------------------------------------------------------------------------

#if !defined(__WIN32__) || !wxUSE_FSVOLUME
    #undef TEST_VOLUME
#endif

#ifdef TEST_VOLUME

#include "wx/volume.h"

static const wxChar *volumeKinds[] =
{
    wxT("floppy"),
    wxT("hard disk"),
    wxT("CD-ROM"),
    wxT("DVD-ROM"),
    wxT("network volume"),
    wxT("other volume"),
};

static void TestFSVolume()
{
    wxPuts(wxT("*** Testing wxFSVolume class ***"));

    wxArrayString volumes = wxFSVolume::GetVolumes();
    size_t count = volumes.GetCount();

    if ( !count )
    {
        wxPuts(wxT("ERROR: no mounted volumes?"));
        return;
    }

    wxPrintf(wxT("%u mounted volumes found:\n"), count);

    for ( size_t n = 0; n < count; n++ )
    {
        wxFSVolume vol(volumes[n]);
        if ( !vol.IsOk() )
        {
            wxPuts(wxT("ERROR: couldn't create volume"));
            continue;
        }

        wxPrintf(wxT("%u: %s (%s), %s, %s, %s\n"),
                 n + 1,
                 vol.GetDisplayName().c_str(),
                 vol.GetName().c_str(),
                 volumeKinds[vol.GetKind()],
                 vol.IsWritable() ? wxT("rw") : wxT("ro"),
                 vol.GetFlags() & wxFS_VOL_REMOVABLE ? wxT("removable")
                                                     : wxT("fixed"));
    }
    
    wxPuts("\n");
}

#endif // TEST_VOLUME

// ----------------------------------------------------------------------------
// date time
// ----------------------------------------------------------------------------

#ifdef TEST_DATETIME

#include "wx/math.h"
#include "wx/datetime.h"

#if TEST_INTERACTIVE

static void TestDateTimeInteractive()
{
    wxPuts(wxT("\n*** interactive wxDateTime tests ***"));

    wxChar buf[128];

    for ( ;; )
    {
        wxPrintf(wxT("Enter a date (or 'quit' to escape): "));
        if ( !wxFgets(buf, WXSIZEOF(buf), stdin) )
            break;

        // kill the last '\n'
        buf[wxStrlen(buf) - 1] = 0;
        
        if ( wxString(buf).CmpNoCase("quit") == 0 )
            break;

        wxDateTime dt;
        const wxChar *p = dt.ParseDate(buf);
        if ( !p )
        {
            wxPrintf(wxT("ERROR: failed to parse the date '%s'.\n"), buf);

            continue;
        }
        else if ( *p )
        {
            wxPrintf(wxT("WARNING: parsed only first %u characters.\n"), p - buf);
        }

        wxPrintf(wxT("%s: day %u, week of month %u/%u, week of year %u\n"),
                 dt.Format(wxT("%b %d, %Y")).c_str(),
                 dt.GetDayOfYear(),
                 dt.GetWeekOfMonth(wxDateTime::Monday_First),
                 dt.GetWeekOfMonth(wxDateTime::Sunday_First),
                 dt.GetWeekOfYear(wxDateTime::Monday_First));
    }
    
    wxPuts("\n");
}

#endif // TEST_INTERACTIVE
#endif // TEST_DATETIME

// ----------------------------------------------------------------------------
// single instance
// ----------------------------------------------------------------------------

#ifdef TEST_SNGLINST

#include "wx/snglinst.h"

static bool TestSingleIstance()
{
    wxPuts(wxT("\n*** Testing wxSingleInstanceChecker ***"));

    wxSingleInstanceChecker checker;
    if ( checker.Create(wxT(".wxconsole.lock")) )
    {
        if ( checker.IsAnotherRunning() )
        {
            wxPrintf(wxT("Another instance of the program is running, exiting.\n"));

            return false;
        }

        // wait some time to give time to launch another instance
        wxPuts(wxT("If you try to run another instance of this program now, it won't start."));
        wxPrintf(wxT("Press \"Enter\" to exit wxSingleInstanceChecker test and proceed..."));
        wxFgetc(stdin);
    }
    else // failed to create
    {
        wxPrintf(wxT("Failed to init wxSingleInstanceChecker.\n"));
    }
    
    wxPuts("\n");
    
    return true;
}
#endif // TEST_SNGLINST


// ----------------------------------------------------------------------------
// entry point
// ----------------------------------------------------------------------------

int main(int argc, char **argv)
{
#if wxUSE_UNICODE
    wxChar **wxArgv = new wxChar *[argc + 1];

    {
        int n;

        for (n = 0; n < argc; n++ )
        {
            wxMB2WXbuf warg = wxConvertMB2WX(argv[n]);
            wxArgv[n] = wxStrdup(warg);
        }

        wxArgv[n] = NULL;
    }
#else // !wxUSE_UNICODE
    #define wxArgv argv
#endif // wxUSE_UNICODE/!wxUSE_UNICODE

    wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");

    wxInitializer initializer;
    if ( !initializer )
    {
        fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");

        return -1;
    }

#ifdef TEST_SNGLINST
    if (!TestSingleIstance())
        return 1;
#endif // TEST_SNGLINST

#ifdef TEST_DIR
    #if TEST_ALL
        TestDirExists();
        TestDirEnum();
    #endif
    TestDirTraverse();
#endif // TEST_DIR

#ifdef TEST_DYNLIB
    TestDllLoad();
    TestDllListLoaded();
#endif // TEST_DYNLIB

#ifdef TEST_ENVIRON
    TestEnvironment();
#endif // TEST_ENVIRON

#ifdef TEST_FILECONF
    TestFileConfRead();
#endif // TEST_FILECONF

#ifdef TEST_LOCALE
    TestDefaultLang();
#endif // TEST_LOCALE

#ifdef TEST_LOG
    wxPuts(wxT("*** Testing wxLog ***"));

    wxString s;
    for ( size_t n = 0; n < 8000; n++ )
    {
        s << (wxChar)(wxT('A') + (n % 26));
    }

    wxLogWarning(wxT("The length of the string is %lu"),
                 (unsigned long)s.length());

    wxString msg;
    msg.Printf(wxT("A very very long message: '%s', the end!\n"), s.c_str());

    // this one shouldn't be truncated
    wxPrintf(msg);

    // but this one will because log functions use fixed size buffer
    // (note that it doesn't need '\n' at the end neither - will be added
    //  by wxLog anyhow)
    wxLogMessage(wxT("A very very long message 2: '%s', the end!"), s.c_str());
#endif // TEST_LOG

#ifdef TEST_FILE
    TestFileRead();
    TestTextFileRead();
    TestFileCopy();
    TestTempFile();
#endif // TEST_FILE

#ifdef TEST_FILENAME
    TestFileNameTemp();
    TestFileNameCwd();
    TestFileNameDirManip();
    TestFileNameComparison();
    TestFileNameOperations();
#endif // TEST_FILENAME

#ifdef TEST_FILETIME
    TestFileGetTimes();
    #if 0
    TestFileSetTimes();
    #endif
#endif // TEST_FILETIME

#ifdef TEST_FTP
    wxLog::AddTraceMask(FTP_TRACE_MASK);

    // wxFTP cannot be a static variable as its ctor needs to access
    // wxWidgets internals after it has been initialized
    ftp = new wxFTP;
    ftp->SetLog(new wxProtocolLog(FTP_TRACE_MASK));
    if ( TestFtpConnect() )
        TestFtpInteractive();
    //else: connecting to the FTP server failed

    delete ftp;
#endif // TEST_FTP

#ifdef TEST_MIME
    //wxLog::AddTraceMask(wxT("mime"));
    TestMimeEnum();
#if 0
    TestMimeOverride();
    TestMimeAssociate();
#endif
    TestMimeFilename();
#endif // TEST_MIME

#ifdef TEST_INFO_FUNCTIONS
    TestOsInfo();
    TestPlatformInfo();
    TestUserInfo();

    #if TEST_INTERACTIVE
        TestDiskInfo();
    #endif
#endif // TEST_INFO_FUNCTIONS

#ifdef TEST_PATHLIST
    TestPathList();
#endif // TEST_PATHLIST

#ifdef TEST_PRINTF
    TestPrintf();
#endif // TEST_PRINTF

#if defined TEST_REGEX && TEST_INTERACTIVE
    TestRegExInteractive();
#endif // defined TEST_REGEX && TEST_INTERACTIVE

#ifdef TEST_DATETIME
    #if TEST_INTERACTIVE
        TestDateTimeInteractive();
    #endif
#endif // TEST_DATETIME

#ifdef TEST_STACKWALKER
#if wxUSE_STACKWALKER
    TestStackWalk(argv[0]);
#endif
#endif // TEST_STACKWALKER

#ifdef TEST_STDPATHS
    TestStandardPaths();
#endif

#ifdef TEST_USLEEP
    wxPuts(wxT("Sleeping for 3 seconds... z-z-z-z-z..."));
    wxUsleep(3000);
#endif // TEST_USLEEP

#ifdef TEST_VOLUME
    TestFSVolume();
#endif // TEST_VOLUME

#if wxUSE_UNICODE
    {
        for ( int n = 0; n < argc; n++ )
            free(wxArgv[n]);

        delete [] wxArgv;
    }
#endif // wxUSE_UNICODE

    wxUnusedVar(argc);
    wxUnusedVar(argv);
    return 0;
}

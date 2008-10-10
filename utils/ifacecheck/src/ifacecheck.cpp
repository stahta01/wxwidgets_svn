/////////////////////////////////////////////////////////////////////////////
// Name:        ifacecheck.cpp
// Purpose:     Interface headers <=> real headers coherence checker
// Author:      Francesco Montorsi
// Created:     2008/03/17
// RCS-ID:      $Id$
// Copyright:   (c) 2008 Francesco Montorsi
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/crt.h"
#endif

#include "wx/cmdline.h"
#include "wx/textfile.h"
#include "wx/filename.h"
#include "wx/stopwatch.h"       // for wxGetLocalTime
#include "xmlparser.h"

// global verbosity flag
bool g_verbose = false;



// ----------------------------------------------------------------------------
// IfaceCheckApp
// ----------------------------------------------------------------------------

#define API_DUMP_FILE               "dump.api.txt"
#define INTERFACE_DUMP_FILE         "dump.interface.txt"

#define PROCESS_ONLY_OPTION         "p"
#define USE_PREPROCESSOR_OPTION     "u"

#define MODIFY_SWITCH               "m"
#define DUMP_SWITCH                 "d"
#define HELP_SWITCH                 "h"
#define VERBOSE_SWITCH              "v"

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
    { wxCMD_LINE_OPTION, USE_PREPROCESSOR_OPTION, "use-preproc",
        "uses the preprocessor output to increase the checker accuracy",
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR },
    { wxCMD_LINE_OPTION, PROCESS_ONLY_OPTION, "process-only",
        "processes only header files matching the given wildcard",
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR },
    { wxCMD_LINE_SWITCH, MODIFY_SWITCH, "modify",
        "modify the interface headers to match the real ones" },
    { wxCMD_LINE_SWITCH, DUMP_SWITCH, "dump",
        "dump both interface and API to plain text dump.*.txt files" },
    { wxCMD_LINE_SWITCH, HELP_SWITCH, "help",
        "show help message", wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    { wxCMD_LINE_SWITCH, VERBOSE_SWITCH, "verbose",
        "be verbose" },
    { wxCMD_LINE_PARAM,  NULL, NULL,
        "gccXML", wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
    { wxCMD_LINE_PARAM,  NULL, NULL,
        "doxygenXML", wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
    wxCMD_LINE_DESC_END
};

class IfaceCheckApp : public wxAppConsole
{
public:
    // don't use builtin cmd line parsing:
    virtual bool OnInit() { m_modify=false; return true; }
    virtual int OnRun();

    bool ParsePreprocessorOutput(const wxString& filename);

    bool Compare();
    int CompareClasses(const wxClass* iface, const wxClass* api);
    bool FixMethod(const wxString& header, const wxMethod* iface, const wxMethod* api);

    void ShowProgress();
    void PrintStatistics(long secs);

    bool IsToProcess(const wxString& headername) const
    {
        if (m_strToMatch.IsEmpty())
            return true;
        return wxMatchWild(m_strToMatch, headername, false);
    }

protected:
    wxXmlGccInterface m_gccInterface;                  // "real" headers API
    wxXmlDoxygenInterface m_doxyInterface;             // doxygen-commented headers API

    // was the MODIFY_SWITCH passed?
    bool m_modify;

    // if non-empty, then PROCESS_ONLY_OPTION was passed and this is the
    // wildcard expression to match
    wxString m_strToMatch;
};

IMPLEMENT_APP_CONSOLE(IfaceCheckApp)

int IfaceCheckApp::OnRun()
{
    long startTime = wxGetLocalTime();      // for timing purpose

    wxCmdLineParser parser(g_cmdLineDesc, argc, argv);
    parser.SetLogo(
        wxString::Format("wxWidgets Interface checker utility (built %s against %s)",
                         __DATE__, wxVERSION_STRING));

    // parse the command line...
    bool ok = true;
    wxString preprocFile;
    switch (parser.Parse())
    {
        case 0:
            if (parser.Found(VERBOSE_SWITCH))
                g_verbose = true;

            // IMPORTANT: parsing #define values must be done _before_ actually
            //            parsing the GCC/doxygen XML files
            if (parser.Found(USE_PREPROCESSOR_OPTION, &preprocFile))
            {
                if (!ParsePreprocessorOutput(preprocFile))
                    return 1;
            }

            // in any case set basic std preprocessor #defines:
            m_doxyInterface.AddPreprocessorValue("NULL", "0");

            //g_bLogEnabled = false;

            // parse the two XML files which contain the real and the doxygen interfaces
            // for wxWidgets API:
            if (!m_gccInterface.Parse(parser.GetParam(0)) ||
                !m_doxyInterface.Parse(parser.GetParam(1)))
                return 1;

            g_bLogEnabled = true;

            if (parser.Found(DUMP_SWITCH))
            {
                LogMessage("Dumping real API to '%s'...", API_DUMP_FILE);
                m_gccInterface.Dump(API_DUMP_FILE);

                LogMessage("Dumping interface API to '%s'...", INTERFACE_DUMP_FILE);
                m_doxyInterface.Dump(INTERFACE_DUMP_FILE);
            }
            else
            {
                if (parser.Found(MODIFY_SWITCH))
                    m_modify = true;

                if (parser.Found(PROCESS_ONLY_OPTION, &m_strToMatch))
                {
                    size_t len = m_strToMatch.Len();
                    if (m_strToMatch.StartsWith("\"") &&
                        m_strToMatch.EndsWith("\"") &&
                        len > 2)
                        m_strToMatch = m_strToMatch.Mid(1, len-2);
                }


                ok = Compare();
            }

            PrintStatistics(wxGetLocalTime() - startTime);
            return ok ? 0 : 1;

        default:
            wxPrintf("\nThis utility checks that the interface XML files created by Doxygen are in\n");
            wxPrintf("synch with the real headers (whose contents are extracted by the gcc XML file).\n\n");
            wxPrintf("The 'gccXML' parameter should be the wxapi.xml file created by the 'rungccxml.sh'\n");
            wxPrintf("script which resides in 'utils/ifacecheck'.\n");
            wxPrintf("The 'doxygenXML' parameter should be the index.xml file created by Doxygen\n");
            wxPrintf("for the wxWidgets 'interface' folder.\n\n");
            wxPrintf("Since the gcc XML file does not contain info about #defines, if you use\n");
            wxPrintf("the -%s option, you'll get a smaller number of false warnings.\n",
                     USE_PREPROCESSOR_OPTION);

            // HELP_SWITCH was passed or a syntax error occurred
            return 0;
    }
}

void IfaceCheckApp::ShowProgress()
{
    wxPrint(".");
    //fflush(stdout);
}

bool IfaceCheckApp::Compare()
{
    const wxClassArray& interfaces = m_doxyInterface.GetClasses();
    const wxClass* c;
    int mcount = 0, ccount = 0;

    LogMessage("Comparing the interface API to the real API (%d classes to compare)...",
               interfaces.GetCount());

    if (!m_strToMatch.IsEmpty())
        LogMessage("Processing only header files matching '%s' expression.", m_strToMatch);

    for (unsigned int i=0; i<interfaces.GetCount(); i++)
    {
        // only compare the methods which are available for the port
        // for which the gcc XML was produced
        if (interfaces[i].GetAvailability() != wxPORT_UNKNOWN &&
            (interfaces[i].GetAvailability() & m_gccInterface.GetInterfacePort()) == 0) {

            if (g_verbose)
                LogMessage("skipping class '%s' since it's not available for the %s port.",
                           interfaces[i].GetName(), m_gccInterface.GetInterfacePortName());

            continue;       // skip this method
        }

        // shorten the name of the header so the log file is more readable
        // and also for calling IsToProcess() against it
        wxString header = wxFileName(interfaces[i].GetHeader()).GetFullName();

        if (!IsToProcess(header))
            continue;       // skip this one

        wxString cname = interfaces[i].GetName();

        // search in the real headers for i-th interface class; we search for
        // both class cname and cnameBase since in wxWidgets world tipically
        // class cname is platform-specific while the real public interface of
        // that class is part of the cnameBase class.
        /*c = m_gccInterface.FindClass(cname + "Base");
        if (c) api.Add(c);*/

        c = m_gccInterface.FindClass(cname);
        if (!c)
        {
            // sometimes the platform-specific class is named "wxGeneric" + cname
            // or similar:
            c = m_gccInterface.FindClass("wxGeneric" + cname.Mid(2));
            if (!c)
            {
                c = m_gccInterface.FindClass("wxGtk" + cname.Mid(2));
            }
        }

        if (c) {

            // there is a class with the same (logic) name!
            mcount += CompareClasses(&interfaces[i], c);

        } else {

            LogMessage("%s: couldn't find the real interface for the '%s' class",
                       header, cname);
            ccount++;
        }
    }

    LogMessage("%d methods (%.1f%%) of the interface headers do not exist in the real headers",
               mcount, (float)(100.0 * mcount/m_doxyInterface.GetMethodCount()));
    LogMessage("%d classes (%.1f%%) of the interface headers do not exist in the real headers",
               ccount, (float)(100.0 * ccount/m_doxyInterface.GetClassesCount()));

    return true;
}

int IfaceCheckApp::CompareClasses(const wxClass* iface, const wxClass* api)
{
    const wxMethod *real;
    int count = 0;

    wxASSERT(iface && api);

    // shorten the name of the header so the log file is more readable
    wxString header = wxFileName(iface->GetHeader()).GetFullName();

    for (unsigned int i=0; i<iface->GetMethodCount(); i++)
    {
        const wxMethod& m = iface->GetMethod(i);

        // only compare the methods which are available for the port
        // for which the gcc XML was produced
        if (m.GetAvailability() != wxPORT_UNKNOWN &&
            (m.GetAvailability() & m_gccInterface.GetInterfacePort()) == 0) {

            if (g_verbose)
                LogMessage("skipping method '%s' since it's not available for the %s port.",
                           m.GetAsString(), m_gccInterface.GetInterfacePortName());

            continue;       // skip this method
        }

        // search in the methods of the api classes provided
        real = api->RecursiveUpwardFindMethod(m, &m_gccInterface);

        if (!real)
        {
            bool exit = false;
            wxMethodPtrArray overloads =
                api->RecursiveUpwardFindMethodsNamed(m.GetName(), &m_gccInterface);

#define HACK_TO_AUTO_CORRECT_ONLY_METHOD_ATTRIBUTES        0
#if HACK_TO_AUTO_CORRECT_ONLY_METHOD_ATTRIBUTES
            for (unsigned int k=0; k<overloads.GetCount(); k++)
                if (overloads[k]->MatchesExceptForAttributes(m) &&
                    overloads[k]->IsPureVirtual() == m.IsPureVirtual())
                {
                    // fix default values of results[k]:
                    wxMethod tmp(*overloads[k]);
                    tmp.SetArgumentTypes(m.GetArgumentTypes());

                    // modify interface header
                    if (FixMethod(iface->GetHeader(), &m, &tmp))
                        LogMessage("Adjusted attributes of '%s' method", m.GetAsString());

                    exit = true;
                    break;
                }

            if (!exit)
            {
#endif

            if (overloads.GetCount()==0)
            {
                LogMessage("%s: real '%s' class and their parents have no method '%s'",
                            header, api->GetName(), m.GetAsString());
                // we've found no overloads
            }
            else
            {
                // first, output a warning
                wxString warning = header;
                if (overloads.GetCount()>1)
                    warning += wxString::Format(": in the real headers there are %d overloads of '%s' for "
                                                "'%s' all with different signatures:\n",
                                                overloads.GetCount(), m.GetName(), api->GetName());
                else {
                    warning += wxString::Format(": in the real headers there is a method '%s' for '%s'"
                                                " but has different signature:\n",
                                                m.GetName(), api->GetName());
                }

                // get a list of the prototypes with _all_ possible attributes:
                warning += "\tdoxy header: " + m.GetAsString(true, true, true, true);
                for (unsigned int j=0; j<overloads.GetCount(); j++)
                    warning += "\n\treal header: " + overloads[j]->GetAsString(true, true, true, true);

                wxPrint(warning + "\n");
                count++;

                if (overloads.GetCount()>1)
                {
                    // TODO: decide which of these overloads is the most "similar" to m
                    //       and eventually modify it
                    if (m_modify)
                        wxPrint("\tmanual fix is required\n");
                }
                else
                {
                    wxASSERT(overloads.GetCount() == 1);

                    if (m_modify)
                    {
                        wxPrint("\tfixing it...\n");

                        // try to modify it!
                        FixMethod(iface->GetHeader(), &m, overloads[0]);
                    }
                }
            }

            count++;

#if HACK_TO_AUTO_CORRECT_ONLY_METHOD_ATTRIBUTES
            }
#endif
        }
    }

    return count;
}

bool IfaceCheckApp::FixMethod(const wxString& header, const wxMethod* iface, const wxMethod* api)
{
    wxASSERT(iface && api);

    wxTextFile file;
    if (!file.Open(header)) {
        LogError("\tcan't open the '%s' header file.", header);
        return false;
    }

    // GetLocation() returns the line where the last part of the prototype is placed:
    int end = iface->GetLocation()-1;
    if (end <= 0 || end >= (int)file.GetLineCount()) {
        LogWarning("\tinvalid location info for method '%s': %d.",
                   iface->GetAsString(), iface->GetLocation());
        return false;
    }

    if (!file.GetLine(end).Contains(";")) {
        LogWarning("\tinvalid location info for method '%s': %d.",
                   iface->GetAsString(), iface->GetLocation());
        return false;
    }

    // is this a one-line prototype declaration?
    bool founddecl = false;
    int start;
    if (file.GetLine(end).Contains(iface->GetName()))
    {
        // yes, this prototype is all on this line:
        start = end;
        founddecl = true;
    }
    else
    {
        start = end-1;

        // find the start point of this prototype declaration:
        while (start > 0 &&
            !file.GetLine(start).Contains(";") &&
            !file.GetLine(start).Contains("*/"))
        {
            start--;

            founddecl |= file.GetLine(start).Contains(iface->GetName());
        }

        // start-th line contains either the declaration of another prototype
        // or the closing tag */ of a doxygen comment; start one line below
        start++;
    }

    if (start <= 0 || !founddecl)
    {
        LogError("\tcan't find the beginning of the declaration of '%s' method in '%s' header looking backwards from line %d",
                    iface->GetAsString(), header, end);
        return false;
    }

    // remove the old prototype
    for (int i=start; i<=end; i++)
        file.RemoveLine(start);     // remove (end-start)-nth times the start-th line

#define INDENTATION_STR  wxString("    ")

    // if possible, add also the @deprecated tag in the doxygen comment if it's missing
    int deprecationOffset = 0;
    if (file.GetLine(start-1).Contains("*/") &&
        (api->IsDeprecated() && !iface->IsDeprecated()))
    {
        file.RemoveLine(start-1);
        file.InsertLine(INDENTATION_STR + INDENTATION_STR +
                        "@deprecated @todo provide deprecation description", start-1);
        file.InsertLine(INDENTATION_STR + "*/", start++);

        // we have added a new line in the final balance
        deprecationOffset=1;
    }

    wxMethod tmp(*api);

    // discard API argument names and replace them with those parsed from doxygen XML:
    const wxArgumentTypeArray& doxygenargs = iface->GetArgumentTypes();
    const wxArgumentTypeArray& realargs = api->GetArgumentTypes();
    if (realargs.GetCount() == doxygenargs.GetCount())
    {
        for (unsigned int j=0; j<doxygenargs.GetCount(); j++)
            if (doxygenargs[j]==realargs[j])
                realargs[j].SetArgumentName(doxygenargs[j].GetArgumentName());

        tmp.SetArgumentTypes(realargs);
    }

#define WRAP_COLUMN     80

    wxArrayString toinsert;
    toinsert.Add(INDENTATION_STR + tmp.GetAsString() + ";");

    int nStartColumn = toinsert[0].Find('(');
    wxASSERT(nStartColumn != wxNOT_FOUND);

    // wrap lines too long at comma boundaries
    for (unsigned int i=0; i<toinsert.GetCount(); i++)
    {
        size_t len = toinsert[i].Len();
        if (len > WRAP_COLUMN)
        {
            wxASSERT(i == toinsert.GetCount()-1);

            // break this line
            wxString tmpleft = toinsert[i].Left(WRAP_COLUMN);
            int comma = tmpleft.Find(',', true /* from end */);
            if (comma == wxNOT_FOUND)
                break;     // break out of the for cycle...

            toinsert.Add(wxString(' ', nStartColumn+1) +
                         toinsert[i].Right(len-comma-2));   // exclude the comma and the space after it
            toinsert[i] = tmpleft.Left(comma+1);            // include the comma
        }
    }

    // insert the new lines
    for (unsigned int i=0; i<toinsert.GetCount(); i++)
        file.InsertLine(toinsert[i], start+i);

    // now save the modification
    if (!file.Write()) {
        LogError("\tcan't save the '%s' header file.", header);
        return false;
    }

    // how many lines did we add/remove in total?
    int nOffset = toinsert.GetCount() + deprecationOffset - (end-start+1);
    if (nOffset == 0)
        return false;

    if (g_verbose)
        LogMessage("\tthe final row offset for following methods is %d lines.", nOffset);

    // update the other method's locations for those methods which belong to the modified header
    // and are placed _below_ the modified method
    wxClassPtrArray cToUpdate = m_doxyInterface.FindClassesDefinedIn(header);
    for (unsigned int i=0; i < cToUpdate.GetCount(); i++)
    {
        for (unsigned int j=0; j < cToUpdate[i]->GetMethodCount(); j++)
        {
            wxMethod& m = cToUpdate[i]->GetMethod(j);
            if (m.GetLocation() > iface->GetLocation())
            {
                // update the location of this method
                m.SetLocation(m.GetLocation()+nOffset);
            }
        }
    }

    return true;
}

bool IfaceCheckApp::ParsePreprocessorOutput(const wxString& filename)
{
    wxTextFile tf;
    if (!tf.Open(filename)) {
        LogError("can't open the '%s' preprocessor output file.", filename);
        return false;
    }

    size_t useful = 0;
    for (unsigned int i=0; i < tf.GetLineCount(); i++)
    {
        const wxString& line = tf.GetLine(i);
        wxString defnameval = line.Mid(8);     // what follows the "#define " string

        // the format of this line should be:
        //    #define DEFNAME DEFVALUE
        if (!line.StartsWith("#define ")) {
            LogError("unexpected content in '%s' at line %d.", filename, i+1);
            return false;
        }

        if (defnameval.Contains(" "))
        {
            // get DEFNAME
            wxString defname = defnameval.BeforeFirst(' ');
            if (defname.Contains("("))
                continue;       // this is a macro, skip it!

            // get DEFVAL
            wxString defval = defnameval.AfterFirst(' ').Strip(wxString::both);
            if (defval.StartsWith("(") && defval.EndsWith(")"))
                defval = defval.Mid(1, defval.Len()-2);

            // store this pair in the doxygen interface, where it can be useful
            m_doxyInterface.AddPreprocessorValue(defname, defval);
            useful++;
        }
        else
        {
            // it looks like the format of this line is:
            //    #define DEFNAME
            // we are not interested to symbols #defined to nothing,
            // so we just ignore this line.
        }
    }

    LogMessage("Parsed %d preprocessor #defines from '%s' which will be used later...",
               useful, filename);

    return true;
}

void IfaceCheckApp::PrintStatistics(long secs)
{
    LogMessage("wx real headers contains declaration of %d classes (%d methods)",
               m_gccInterface.GetClassesCount(), m_gccInterface.GetMethodCount());
    LogMessage("wx interface headers contains declaration of %d classes (%d methods)",
               m_doxyInterface.GetClassesCount(), m_doxyInterface.GetMethodCount());
    LogMessage("total processing took %d seconds.", secs);
}



/////////////////////////////////////////////////////////////////////////////
// Name:        helpview.cpp
// Purpose:     wxHtml help browser
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "help.cpp"
#pragma interface "help.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/image.h>
#include <wx/wxhtml.h>
#include <wx/fs_zip.h>

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------


// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
    public:
        // override base class virtuals
        // ----------------------------

        // this one is called on application startup and is a good place for the app
        // initialization (doing it here and not in the ctor allows to have an error
        // return: if OnInit() returns false, the application terminates)

        virtual bool OnInit();
        virtual int OnExit();

    private:
        wxHtmlHelpController *help;
        wxConfig* config;
};


IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
    wxInitAllImageHandlers();
    wxFileSystem::AddHandler(new wxZipFSHandler);

    config = new wxConfig("wxHTMLhelp");
    help = new wxHtmlHelpController;
    help -> UseConfig(config);
    
    if (argc < 2) {
        wxLogError("Usage : helpview <helpfile> [<more helpfiles>]");
        wxLogError("  helpfile may be .hhp, .zip or .htb");
        return FALSE;
    }

    for (int i = 1; i < argc; i++)
        help -> AddBook(argv[i]);

    help -> DisplayContents();

    return TRUE;
}


int MyApp::OnExit()
{
    delete help;
    delete config;

    return 0;
}


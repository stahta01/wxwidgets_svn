/////////////////////////////////////////////////////////////////////////////
// Name:        _functions.i
// Purpose:     SWIG interface defs for various functions and such
//
// Author:      Robin Dunn
//
// Created:     3-July-1997
// RCS-ID:      $Id$
// Copyright:   (c) 2003 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// Not a %module


//---------------------------------------------------------------------------

MAKE_CONST_WXSTRING(FileSelectorPromptStr);
MAKE_CONST_WXSTRING(FileSelectorDefaultWildcardStr);
MAKE_CONST_WXSTRING(DirSelectorPromptStr);

//---------------------------------------------------------------------------
%newgroup;


long wxNewId();
void wxRegisterId(long id);
long wxGetCurrentId();

void wxBell();
void wxEndBusyCursor();

long wxGetElapsedTime(bool resetTimer = True);

DocDeclA(
    void, wxGetMousePosition(int* OUTPUT, int* OUTPUT),
    "GetMousePosition() -> (x,y)");

bool wxIsBusy();
wxString wxNow();
bool wxShell(const wxString& command = wxPyEmptyString);
void wxStartTimer();

DocDeclA(
    int, wxGetOsVersion(int *OUTPUT, int *OUTPUT),
    "GetOsVersion() -> (platform, major, minor)");

wxString wxGetOsDescription();

#if defined(__WXMSW__) || defined(__WXMAC__)
long wxGetFreeMemory();
#else
%inline %{
    long wxGetFreeMemory()
        { wxPyRaiseNotImplemented(); return 0; }
%}
#endif

enum wxShutdownFlags
{
    wxSHUTDOWN_POWEROFF,    // power off the computer
    wxSHUTDOWN_REBOOT       // shutdown and reboot
};

// Shutdown or reboot the PC
bool wxShutdown(wxShutdownFlags wFlags);


void wxSleep(int secs);
void wxUsleep(unsigned long milliseconds);
void wxEnableTopLevelWindows(bool enable);

wxString wxStripMenuCodes(const wxString& in);


wxString wxGetEmailAddress();
wxString wxGetHostName();
wxString wxGetFullHostName();
wxString wxGetUserId();
wxString wxGetUserName();
wxString wxGetHomeDir();
wxString wxGetUserHome(const wxString& user = wxPyEmptyString);

unsigned long wxGetProcessId();

void wxTrap();


// Dialog Functions

wxString wxFileSelector(const wxString& message = wxPyFileSelectorPromptStr,
                        const wxString& default_path = wxPyEmptyString,
                        const wxString& default_filename = wxPyEmptyString,
                        const wxString& default_extension = wxPyEmptyString,
                        const wxString& wildcard = wxPyFileSelectorDefaultWildcardStr,
                        int flags = 0,
                        wxWindow *parent = NULL,
                        int x = -1, int y = -1);

// TODO: wxFileSelectorEx


// Ask for filename to load
wxString wxLoadFileSelector(const wxString& what,
                            const wxString& extension,
                            const wxString& default_name = wxPyEmptyString,
                            wxWindow *parent = NULL);

// Ask for filename to save
wxString wxSaveFileSelector(const wxString& what,
                            const wxString& extension,
                            const wxString& default_name = wxPyEmptyString,
                            wxWindow *parent = NULL);


wxString wxDirSelector(const wxString& message = wxPyDirSelectorPromptStr,
                       const wxString& defaultPath = wxPyEmptyString,
                       long style = wxDD_DEFAULT_STYLE,
                       const wxPoint& pos = wxDefaultPosition,
                       wxWindow *parent = NULL);

wxString wxGetTextFromUser(const wxString& message,
                           const wxString& caption = wxPyEmptyString,
                           const wxString& default_value = wxPyEmptyString,
                           wxWindow *parent = NULL,
                           int x = -1, int y = -1,
                           bool centre = True);

wxString wxGetPasswordFromUser(const wxString& message,
                               const wxString& caption = wxPyEmptyString,
                               const wxString& default_value = wxPyEmptyString,
                               wxWindow *parent = NULL);


// TODO: Need to custom wrap this one...
// int wxGetMultipleChoice(char* message, char* caption,
//                         int LCOUNT, char** choices,
//                         int nsel, int *selection,
//                         wxWindow *parent = NULL, int x = -1, int y = -1,
//                         bool centre = True, int width=150, int height=200);


wxString wxGetSingleChoice(const wxString& message, const wxString& caption,
                           int choices, wxString* choices_array,
                           wxWindow *parent = NULL,
                           int x = -1, int y = -1,
                           bool centre = True,
                           int width=150, int height=200);

int wxGetSingleChoiceIndex(const wxString& message, const wxString& caption,
                           int choices, wxString* choices_array,
                           wxWindow *parent = NULL,
                           int x = -1, int y = -1,
                           bool centre = True,
                           int width=150, int height=200);


int wxMessageBox(const wxString& message,
                 const wxString& caption = wxPyEmptyString,
                 int style = wxOK | wxCENTRE,
                 wxWindow *parent = NULL,
                 int x = -1, int y = -1);

long wxGetNumberFromUser(const wxString& message,
                         const wxString& prompt,
                         const wxString& caption,
                         long value,
                         long min = 0, long max = 100,
                         wxWindow *parent = NULL,
                         const wxPoint& pos = wxDefaultPosition);

// GDI Functions

bool wxColourDisplay();

int wxDisplayDepth();
int wxGetDisplayDepth();

DocDeclA(
    void, wxDisplaySize(int* OUTPUT, int* OUTPUT),
    "DisplaySize() -> (width, height)");
wxSize wxGetDisplaySize();

DocDeclA(
    void, wxDisplaySizeMM(int* OUTPUT, int* OUTPUT),
    "DisplaySizeMM() -> (width, height)");
wxSize wxGetDisplaySizeMM();

DocDeclA(
    void, wxClientDisplayRect(int *OUTPUT, int *OUTPUT, int *OUTPUT, int *OUTPUT),
    "ClientDisplayRect() -> (x, y, width, height)");
wxRect wxGetClientDisplayRect();

void wxSetCursor(wxCursor& cursor);



// Miscellaneous functions

void wxBeginBusyCursor(wxCursor *cursor = wxHOURGLASS_CURSOR);
wxWindow * wxGetActiveWindow();

wxWindow* wxGenericFindWindowAtPoint(const wxPoint& pt);
wxWindow* wxFindWindowAtPoint(const wxPoint& pt);

wxWindow* wxGetTopLevelParent(wxWindow *win);

//bool wxSpawnBrowser(wxWindow *parent, wxString href);




DocDeclStr(
    bool , wxGetKeyState(wxKeyCode key),
    "Get the state of a key (true if pressed or toggled on, false if not.)
This is generally most useful getting the state of the modifier or
toggle keys.  On some platforms those may be the only keys that work.
", "");



//---------------------------------------------------------------------------

#if defined(__WXMSW__) || defined(__WXMAC__)
void wxWakeUpMainThread();
#else
%inline %{
    void wxWakeUpMainThread() {}
%}
#endif

void wxMutexGuiEnter();
void wxMutexGuiLeave();


class wxMutexGuiLocker  {
public:
    wxMutexGuiLocker();
    ~wxMutexGuiLocker();
};


%inline %{
    bool wxThread_IsMain() {
#ifdef WXP_WITH_THREAD
        return wxThread::IsMain();
#else
        return True;
#endif
    }
%}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

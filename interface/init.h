/////////////////////////////////////////////////////////////////////////////
    // Name:        init.h
    // Purpose:     documentation for global functions
    // Author:      wxWidgets team
    // RCS-ID:      $Id$
    // Licence:     wxWindows license
    /////////////////////////////////////////////////////////////////////////////
    
    /**
    Free resources allocated by a successful call to wxEntryStart.
*/
void wxEntryCleanup();


    //@{
/**
    (notice that under Windows CE platform, and only there, the type of 
    @e pCmdLine is @c wchar_t *, otherwise it is @c char *, even in
    Unicode build).
*/
bool wxEntryStart(int& argc, wxChar ** argv);
    bool wxEntryStart(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance = @NULL,
                      char * pCmdLine = @NULL,
                      int nCmdShow = SW_SHOWNORMAL);
//@}


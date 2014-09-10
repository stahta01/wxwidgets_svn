/////////////////////////////////////////////////////////////////////////////
// Name:        taskbarbutton.h
// Purpose:     interface of wxAppProgressIndicator
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxAppProgressIndicator

    A helper class that can be used to update the progress bar in the taskbar button.

    @library{wxcore}
    @category{misc}

    @onlyfor{wxmsw}

    @see wxTaskBarButton
    @since 3.1.0
*/
class WXDLLIMPEXP_CORE wxAppProgressIndicator
{
public:
    /**
        Constructs the wxAppProgressIndicator.

        @param parent
            The parent window of wxAppProgressIndicator. Note that the
            window should has taskbar button showing. If parent is NULL,
            the progress will reflect on the taskbar buttons of all the 
            top level windows.
        @param maxValue
    */
    wxAppProgressIndicator(wxWindow* parent = NULL, int maxValue = 100);
    virtual ~wxAppProgressIndicator();

    /**
        Set the progress value in taskbar button of parent window.

        @param value
            The new value of the progress meter. It should be less than or equal
            to the range.
     */
    void SetValue(int value);

    /**
        Set the progress range in taskbar button of parent window.
     */
    void SetRange(int range);

    /**
        Makes the progress bar run in indeterminate mode.
    */
    bool Pulse();
};

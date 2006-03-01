///////////////////////////////////////////////////////////////////////////////
// Name:        checklst.h
// Purpose:     wxCheckListBox class
// Author:      Robert Roebling
// Modified by:
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __GTKCHECKLISTH__
#define __GTKCHECKLISTH__

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

// there is no "right" choice of the checkbox indicators, so allow the user to
// define them himself if he wants
#ifndef wxCHECKLBOX_CHECKED
    #define wxCHECKLBOX_CHECKED   _T('x')
    #define wxCHECKLBOX_UNCHECKED _T(' ')

    #define wxCHECKLBOX_STRING    _T("[ ] ")
#endif

//Use the native GTK2.0+ checklist?? You should say YYEEESS unless
//there are like some major bugs or something :)
#define wxUSE_NATIVEGTKCHECKLIST 1

//-----------------------------------------------------------------------------
// wxCheckListBox
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxCheckListBox : public wxListBox
{
public:
    wxCheckListBox();
    wxCheckListBox(wxWindow *parent, wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            int nStrings = 0,
            const wxString *choices = (const wxString *)NULL,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxListBoxNameStr);
    wxCheckListBox(wxWindow *parent, wxWindowID id,
            const wxPoint& pos,
            const wxSize& size,
            const wxArrayString& choices,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxListBoxNameStr);

    bool IsChecked( int index ) const;
    void Check( int index, bool check = TRUE );

    int GetItemHeight() const;
    
#if wxUSE_NATIVEGTKCHECKLIST
    void DoCreateCheckList();
#endif

private:
    DECLARE_DYNAMIC_CLASS(wxCheckListBox)
};

#endif   //__GTKCHECKLISTH__

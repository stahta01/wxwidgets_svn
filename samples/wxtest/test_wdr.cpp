//------------------------------------------------------------------------------
// Source code generated by wxDesigner from file: test.wdr
// Do not modify this file, all changes will be lost!
//------------------------------------------------------------------------------

// For compilers that support precompilation
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// Include private header
#include "test_wdr.h"


// Implement window functions

wxSizer *MyDialogFunc( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxStaticBox *item2 = new wxStaticBox( parent, -1, "Text" );
    wxStaticBoxSizer *item1 = new wxStaticBoxSizer( item2, wxVERTICAL );

    wxBoxSizer *item3 = new wxBoxSizer( wxHORIZONTAL );

    wxTextCtrl *item4 = new wxTextCtrl( parent, ID_TEXTCTRL, "", wxDefaultPosition, wxSize(80,-1), 0 );
    item3->Add( item4, 0, wxALIGN_CENTRE|wxALL, 5 );

    item1->Add( item3, 0, wxALIGN_CENTRE|wxALL, 5 );

    item0->Add( item1, 0, wxALIGN_CENTRE|wxALL, 5 );

    if (set_sizer)
    {
        parent->SetAutoLayout( TRUE );
        parent->SetSizer( item0 );
        if (call_fit)
        {
            item0->Fit( parent );
            item0->SetSizeHints( parent );
        }
    }
    
    return item0;
}

// Implement menubar functions

wxMenuBar *MyMenuBarFunc()
{
    wxMenuBar *item0 = new wxMenuBar;
    
    wxMenu* item1 = new wxMenu;
    item1->Append( ID_ABOUT, "About...\tF1", "" );
    item1->Append( ID_TEST, "Test...\tF2", "" );
    item1->Append( ID_QUIT, "Quit\tCtrl-Q", "" );
    item0->Append( item1, "File" );
    
    return item0;
}

// Implement toolbar functions

// Implement bitmap functions


// End of generated file

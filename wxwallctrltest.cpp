/////////////////////////////////////////////////////////////////////////////
// Name:        wxwallctrltest.cpp
// Purpose:     
// Author:      Mokhtar M. Khorshid
// Modified by: 
// Created:     02/06/2008 12:13:29
// Copyright:   Copyright 2008 by Mokhtar M. Khorshid
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
//#include "wx/Wallctrl/WallCtrl.h"
//#include "wx/Wallctrl/WallCtrlPlaneSurface.h"
//#include "wx/Wallctrl/WallCtrlBitmapSource.h"
//#include "wx/wallctrl/wallctrldefaultplanenavigation.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "wxwallctrltest.h"

////@begin XPM images
////@end XPM images


/*!
 * wxWallCtrlTest type definition
 */

IMPLEMENT_DYNAMIC_CLASS( wxWallCtrlTest, wxDialog )


/*!
 * wxWallCtrlTest event table definition
 */

BEGIN_EVENT_TABLE( wxWallCtrlTest, wxDialog )

////@begin wxWallCtrlTest event table entries
////@end wxWallCtrlTest event table entries

END_EVENT_TABLE()


/*!
 * wxWallCtrlTest constructors
 */

wxWallCtrlTest::wxWallCtrlTest()
{
    Init();
}

wxWallCtrlTest::wxWallCtrlTest( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * wxWallCtrlTest creator
 */

bool wxWallCtrlTest::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin wxWallCtrlTest creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end wxWallCtrlTest creation
    return true;
}


/*!
 * wxWallCtrlTest destructor
 */

wxWallCtrlTest::~wxWallCtrlTest()
{
////@begin wxWallCtrlTest destruction
////@end wxWallCtrlTest destruction
}


/*!
 * Member initialisation
 */

void wxWallCtrlTest::Init()
{
////@begin wxWallCtrlTest member initialisation
////@end wxWallCtrlTest member initialisation
}


/*!
 * Control creation for wxWallCtrlTest
 */
/*
void wxWallCtrlTest::CreateControls()
{    
////@begin wxWallCtrlTest content construction
    wxWallCtrlTest* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton4 = new wxButton( itemDialog1, ID_BUTTON1, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );

	////@end wxWallCtrlTest content construction
}
*/

void wxWallCtrlTest::CreateControls()
{    
////@begin wxWallCtrlTest content construction

	fasdfasd;
		fasdfdsa; 
		f; 
////@end wxWallCtrlTest content construction

	/*
        wxWallCtrlTest* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);


	//wxWallCtrl * wallCtrl1 = new wxWallCtrl(this, wxID_ANY,	wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);

	

	//wallCtrl1->SetFocus();
	//SetExtraStyle(wxWANTS_CHARS);

	itemBoxSizer2->Add(wallCtrl1, 1, wxGROW|wxALL, 5);

    
    
    wxWallCtrlBitmapSource * bitmapSource = new wxWallCtrlBitmapSource();
	wxBitmap testBitmap;

	// Just add bitmaps manually.
	for (int i=0; i < 10; ++i)
	{
		testBitmap.LoadFile(wxT("Wall1.png"), wxBITMAP_TYPE_PNG);
		bitmapSource->AppendBitmap(testBitmap);
		testBitmap.LoadFile(wxT("Wall2.png"), wxBITMAP_TYPE_PNG);
		bitmapSource->AppendBitmap(testBitmap);
		testBitmap.LoadFile(wxT("Wall3.png"), wxBITMAP_TYPE_PNG);
		bitmapSource->AppendBitmap(testBitmap);
	}

	wxWallCtrlPlaneSurface * surface = new wxWallCtrlPlaneSurface();
	surface->SetScopeSize(wxSize(16, 2));

	wxWallCtrlDefaultPlaneNavigation * navigation = new wxWallCtrlDefaultPlaneNavigation();
	wallCtrl1->SetDataSource(bitmapSource);
	wallCtrl1->SetSurface(surface);
	wallCtrl1->SetNavigator(navigation);*/
}


/*!
 * Should we show tooltips?
 */

bool wxWallCtrlTest::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap wxWallCtrlTest::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wxWallCtrlTest bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wxWallCtrlTest bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wxWallCtrlTest::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wxWallCtrlTest icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wxWallCtrlTest icon retrieval
}

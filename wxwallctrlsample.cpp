/////////////////////////////////////////////////////////////////////////////
// Name:        wxwallctrlsample.cpp
// Purpose:     
// Author:      Mokhtar M. Khorshid
// Modified by: 
// Created:     06/07/2008 18:28:29
// RCS-ID:      
// Copyright:   Copyright 2008 by Mokhtar M. Khorshid
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 06/07/2008 18:28:29

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "include/wx/wallctrl/wallctrl.h"
#include "include/wx/wallctrl/wallctrlbitmapsource.h"
#include "include/wx/wallctrl/wallctrldefaultplanenavigation.h"

////@begin includes
////@end includes

#include "wxwallctrlsample.h"

////@begin XPM images
////@end XPM images


/*!
 * wxWallCtrlSample type definition
 */

IMPLEMENT_DYNAMIC_CLASS( wxWallCtrlSample, wxDialog )


/*!
 * wxWallCtrlSample event table definition
 */

BEGIN_EVENT_TABLE( wxWallCtrlSample, wxDialog )

////@begin wxWallCtrlSample event table entries
////@end wxWallCtrlSample event table entries

END_EVENT_TABLE()


/*!
 * wxWallCtrlSample constructors
 */

wxWallCtrlSample::wxWallCtrlSample()
{
    Init();
}

wxWallCtrlSample::wxWallCtrlSample( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * wxWallCtrlSample creator
 */

bool wxWallCtrlSample::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin wxWallCtrlSample creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end wxWallCtrlSample creation
    return true;
}


/*!
 * wxWallCtrlSample destructor
 */

wxWallCtrlSample::~wxWallCtrlSample()
{
////@begin wxWallCtrlSample destruction
////@end wxWallCtrlSample destruction

	//delete bitmapSource;
}


/*!
 * Member initialisation
 */

void wxWallCtrlSample::Init()
{
////@begin wxWallCtrlSample member initialisation
////@end wxWallCtrlSample member initialisation
}


/*!
 * Control creation for wxWallCtrlSample
 */

void wxWallCtrlSample::CreateControls()
{    
////@begin wxWallCtrlSample content construction
    wxWallCtrlSample* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxALL, 5);

    wxWallCtrl* itemWallCtrl4 = new wxWallCtrl( itemDialog1, ID_WALLCTRL, wxPoint(0, 0), wxSize(300, 300), wxWANTS_CHARS );
    itemBoxSizer3->Add(itemWallCtrl4, 1, wxGROW|wxALL, 5);

    wxDirPickerCtrl* itemDirPickerCtrl5 = new wxDirPickerCtrl( itemDialog1, ID_DIRPICKERCTRL1, _T(""), _T(""), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
    itemBoxSizer3->Add(itemDirPickerCtrl5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end wxWallCtrlSample content construction

	wxWallCtrl * wall = itemWallCtrl4;

	//wxWallCtrlBitmapSource * source = new wxWallCtrlBitmapSource();
	bitmapSource = new wxWallCtrlBitmapSource();
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
	surface->SetScopeSize(wxSize(2, 2));

	wxWallCtrlDefaultPlaneNavigation * navigation = new wxWallCtrlDefaultPlaneNavigation();
	wall->SetSurface(surface);
	wall->SetDataSource(bitmapSource);
	//delete (bitmapSource);
	wall->SetNavigator(navigation);

}


/*!
 * Should we show tooltips?
 */

bool wxWallCtrlSample::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap wxWallCtrlSample::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wxWallCtrlSample bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wxWallCtrlSample bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wxWallCtrlSample::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wxWallCtrlSample icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wxWallCtrlSample icon retrieval
}

/////////////////////////////////////////////////////////////////////////////
// Name:        playerdg.cpp
// Purpose:     Forty Thieves patience game
// Author:      Chris Breeze
// Modified by:
// Created:     21/07/97
// RCS-ID:      $Id$
// Copyright:   (c) 1993-1998 Chris Breeze
// Licence:   	wxWindows licence
//---------------------------------------------------------------------------
// Last modified: 22nd July 1998 - ported to wxWindows 2.0
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation
#pragma interface
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "scorefil.h"
#include "playerdg.h"

const int ID_LISTBOX = 101;

BEGIN_EVENT_TABLE(PlayerSelectionDialog, wxDialog)
	EVT_SIZE(PlayerSelectionDialog::OnSize)
	EVT_BUTTON(wxID_OK, PlayerSelectionDialog::ButtonCallback)
	EVT_BUTTON(wxID_CANCEL, PlayerSelectionDialog::ButtonCallback)
	EVT_LISTBOX(ID_LISTBOX, PlayerSelectionDialog::SelectCallback)
END_EVENT_TABLE()

PlayerSelectionDialog::PlayerSelectionDialog(
							wxWindow* parent,
							ScoreFile* file
							) :
	wxDialog(parent, -1, "Player Selection",
			wxDefaultPosition, wxSize(250, 200),
			wxDIALOG_MODAL | wxDEFAULT_DIALOG_STYLE),
	m_scoreFile(file)
{
	// enable constraints
	SetAutoLayout (TRUE);

	wxStaticText* msg = new wxStaticText(this, -1, "Please select a name from the list");

	wxListBox* list = new wxListBox(
						this, ID_LISTBOX,
						wxDefaultPosition, wxDefaultSize,
						0, 0,
						wxLB_SINGLE
						);
#if 1
//    Robert Roebling
    
	int numPlayers = 0;
	wxString* players = 0;
	m_scoreFile->GetPlayerList(&players, numPlayers);
	for (int i = 0; i < numPlayers; i++)
	{
		list->Append(players[i]);
	}
	delete players;
#endif

	m_textField = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxDefaultSize, 0);

	m_OK = new wxButton(this, wxID_OK, "OK");
	m_cancel = new wxButton(this, wxID_CANCEL, "Cancel");

	wxLayoutConstraints* layout;

	// Constrain the msg at the top of the window
	layout = new wxLayoutConstraints;
	layout->left.SameAs		(this,	wxLeft,		10);
	layout->top.SameAs		(this,	wxTop,	10);
	layout->height.AsIs();
	layout->width.AsIs();
	msg->SetConstraints(layout);

	// Constrain the OK button
	layout = new wxLayoutConstraints;
	layout->left.SameAs		(this,	wxLeft,		10);
	layout->bottom.SameAs	(this,	 wxBottom,	10);
	layout->height.AsIs();
	layout->width.AsIs();
	m_OK->SetConstraints(layout);

	// Constrain the OK button
	layout = new wxLayoutConstraints;
	layout->left.RightOf	(m_OK,	10);
	layout->bottom.SameAs	(this,	wxBottom,	10);
	layout->height.AsIs();
	layout->width.AsIs();
	m_cancel->SetConstraints(layout);

	// Constrain the Name text entry field
	layout = new wxLayoutConstraints;
	layout->left.SameAs		(this,	wxLeft,		10);
	layout->right.SameAs	(this,	wxRight,	10);
	layout->bottom.SameAs	(m_OK,	wxTop,		10);
	layout->height.AsIs();
	m_textField->SetConstraints(layout);

	// Constrain the list of players
	layout = new wxLayoutConstraints;
	layout->left.SameAs		(this,	wxLeft,		10);
	layout->right.SameAs	(this,	wxRight,	10);
	layout->top.Below		(msg,	10);
	layout->bottom.SameAs	(m_textField,	wxTop,	10);
	list->SetConstraints(layout);

	wxString prevPlayer = m_scoreFile->GetPreviousPlayer();
	if (prevPlayer.Length() > 0)
	{
		list->SetStringSelection(prevPlayer);
		m_textField->SetValue(prevPlayer);
	}

	Layout();
}

PlayerSelectionDialog::~PlayerSelectionDialog()
{
}

void PlayerSelectionDialog::OnSize(wxSizeEvent& event)
{
	Layout();
}

const wxString& PlayerSelectionDialog::GetPlayersName()
{
/*
	m_player = "";
	Show(TRUE);
*/
	return m_player;
}

bool PlayerSelectionDialog::OnClose()
{
	// hide the dialog
	// NB don't return TRUE otherwise delete is called
	m_player = "";
	Show(FALSE);
	return FALSE;
}

void PlayerSelectionDialog::SelectCallback(wxCommandEvent& event)
{
	if (event.GetEventType() == wxEVT_COMMAND_LISTBOX_SELECTED)
	{
#ifdef __WXGTK__
		if (event.IsSelection())
#endif
		m_textField->SetValue(event.GetString());
	}
}

void PlayerSelectionDialog::ButtonCallback(wxCommandEvent& event)
{
	if (event.GetId() == wxID_OK)
	{
		wxString name = m_textField->GetValue();
		if (!name.IsNull() && name.Length() > 0)
		{
			if (name.Contains('@'))
			{
				wxMessageBox("Names should not contain the '@' character", "Forty Thieves");
			}
			else
			{
				m_player = name;
				Show(FALSE);
			}
		}
		else
		{
			wxMessageBox("Please enter your name", "Forty Thieves");
		}
	}
	else
	{
		m_player = "";
		Show(FALSE);
	}
}

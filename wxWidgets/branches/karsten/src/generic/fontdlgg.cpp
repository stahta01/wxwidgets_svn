/////////////////////////////////////////////////////////////////////////////
// Name:        fontdlgg.cpp
// Purpose:     Generic font dialog
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "fontdlgg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <stdio.h>
#include "wx/utils.h"
#include "wx/dialog.h"
#include "wx/listbox.h"
#include "wx/button.h"
#include "wx/stattext.h"
#include "wx/layout.h"
#include "wx/dcclient.h"
#include "wx/choice.h"
#include "wx/checkbox.h"
#endif

#include <string.h>
#include <stdlib.h>

#include "wx/cmndata.h"
#include "wx/generic/fontdlgg.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxGenericFontDialog, wxDialog)

BEGIN_EVENT_TABLE(wxGenericFontDialog, wxDialog)
	EVT_CHECKBOX(wxID_FONT_UNDERLINE, wxGenericFontDialog::OnChangeFont)
	EVT_CHOICE(wxID_FONT_STYLE, wxGenericFontDialog::OnChangeFont)
	EVT_CHOICE(wxID_FONT_WEIGHT, wxGenericFontDialog::OnChangeFont)
	EVT_CHOICE(wxID_FONT_FAMILY, wxGenericFontDialog::OnChangeFont)
	EVT_CHOICE(wxID_FONT_COLOUR, wxGenericFontDialog::OnChangeFont)
	EVT_CHOICE(wxID_FONT_SIZE, wxGenericFontDialog::OnChangeFont)
	EVT_PAINT(wxGenericFontDialog::OnPaint)
END_EVENT_TABLE()

#endif

#define NUM_COLS 48
static wxString wxColourDialogNames[NUM_COLS]={"ORANGE",
				    "GOLDENROD",
				    "WHEAT",
				    "SPRING GREEN",
				    "SKY BLUE",
				    "SLATE BLUE",
				    "MEDIUM VIOLET RED",
				    "PURPLE",

				    "RED",
				    "YELLOW",
				    "MEDIUM SPRING GREEN",
				    "PALE GREEN",
				    "CYAN",
				    "LIGHT STEEL BLUE",
				    "ORCHID",
				    "LIGHT MAGENTA",
				    
				    "BROWN",
				    "YELLOW",
				    "GREEN",
				    "CADET BLUE",
				    "MEDIUM BLUE",
				    "MAGENTA",
				    "MAROON",
				    "ORANGE RED",

				    "FIREBRICK",
				    "CORAL",
				    "FOREST GREEN",
				    "AQUARAMINE",
				    "BLUE",
				    "NAVY",
				    "THISTLE",
				    "MEDIUM VIOLET RED",
				    
				    "INDIAN RED",
				    "GOLD",
				    "MEDIUM SEA GREEN",
				    "MEDIUM BLUE",
				    "MIDNIGHT BLUE",
				    "GREY",
				    "PURPLE",
				    "KHAKI",
				    
				    "BLACK",
				    "MEDIUM FOREST GREEN",
				    "KHAKI",
				    "DARK GREY",
				    "SEA GREEN",
				    "LIGHT GREY",
				    "MEDIUM SLATE BLUE",
				    "WHITE"
				    };

/*
 * Generic wxFontDialog
 */

wxGenericFontDialog::wxGenericFontDialog(void)
{
  dialogParent = NULL;
}

wxGenericFontDialog::wxGenericFontDialog(wxWindow *parent, wxFontData *data):
  wxDialog(parent, -1, "Font", wxPoint(0, 0), wxSize(600, 600), wxDEFAULT_DIALOG_STYLE|wxDIALOG_MODAL)
{
  Create(parent, data);
}

wxGenericFontDialog::~wxGenericFontDialog(void)
{
}

bool wxGenericFontDialog::OnClose(void)
{
  Show(FALSE);
  return FALSE;
}
 
bool wxGenericFontDialog::Create(wxWindow *parent, wxFontData *data)
{
  dialogParent = parent;
  
  if (data)
    fontData = *data;

  InitializeFont();
  CreateWidgets();
  
  return TRUE;
}

int wxGenericFontDialog::ShowModal(void)
{
  int ret = wxDialog::ShowModal();

    if (ret != wxID_CANCEL)
    {
      fontData.chosenFont = dialogFont;
    }

	return ret;
}


void wxGenericFontDialog::OnPaint(wxPaintEvent& event)
{
  wxDialog::OnPaint(event);

  wxPaintDC dc(this);
  PaintFontBackground(dc);
  PaintFont(dc);
}

/*
static void wxGenericChangeFontText(wxTextCtrl& text, wxCommandEvent& event)
{
  if (event.GetEventType() == wxEVENT_TYPE_TEXT_ENTER_COMMAND)
  {
    wxGenericFontDialog *dialog = (wxGenericFontDialog *)text.GetParent();
    dialog->OnChangeFont();
  }
}
*/

void wxGenericFontDialog::CreateWidgets(void)
{
  wxBeginBusyCursor();

  fontRect.x = 5;
#ifdef __X__
  fontRect.y = 125;
#else
  fontRect.y = 115;
#endif
  fontRect.width = 350;
  fontRect.height = 100;

/*
  static char *families[] = { "Roman", "Decorative", "Modern", "Script", "Swiss" };
  static char *styles[] = { "Normal", "Italic", "Slant" };
  static char *weights[] = { "Normal", "Light", "Bold" };
*/
  static wxString families[] = { "Roman", "Decorative", "Modern", "Script", "Swiss" };
  static wxString styles[] = { "Normal", "Italic", "Slant" };
  static wxString weights[] = { "Normal", "Light", "Bold" };

  int x=-1;
  int y=40;
  familyChoice = new wxChoice(this, wxID_FONT_FAMILY, wxPoint(10, 10), wxSize(120, -1), 5, families);
  styleChoice = new wxChoice(this, wxID_FONT_STYLE, wxPoint(140, 10), wxSize(120, -1), 3, styles);
  weightChoice = new wxChoice(this, wxID_FONT_WEIGHT, wxPoint(270, 10), wxSize(120, -1), 3, weights);

  colourChoice = new wxChoice(this, wxID_FONT_COLOUR, wxPoint(10, 40), wxSize(190, -1), NUM_COLS, wxColourDialogNames);
#ifdef __MOTIF__
  // We want the pointSizeText to line up on the y axis with the colourChoice
  colourChoice->GetPosition(&fontRect.x, &y); //NL mod
  y+=3;	//NL mod
#endif

  wxString pointSizes[40];
  int i;
  for ( i = 0; i < 40; i++)
  {
	char buf[5];
	sprintf(buf, "%d", i + 1);
	pointSizes[i] = buf;
  }

  pointSizeChoice = new wxChoice(this, wxID_FONT_SIZE, wxPoint(210, y), wxSize(50, -1), 40, pointSizes);
  underLineCheckBox = new wxCheckBox(this, wxID_FONT_UNDERLINE, "Underline", wxPoint(280, y));

  int rectY;
  pointSizeChoice->GetPosition(&x, &rectY); //NL mod
  fontRect.y = rectY;
  pointSizeChoice->GetSize(&x, &y); //NL mod

  // Calculate the position of the bottom of the pointSizeChoice, and place
  // the fontRect there  (+5 for a nice gap) 

  fontRect.y+=y+5; //NL mod

  int by = (fontRect.y + fontRect.height + 5);

  wxButton *okButton = new wxButton(this, wxID_OK, "OK", wxPoint(5, by));
  (void) new wxButton(this, wxID_OK, "Cancel", wxPoint(50, by));

  familyChoice->SetStringSelection(wxFontFamilyIntToString(dialogFont.GetFamily()));
  styleChoice->SetStringSelection(wxFontStyleIntToString(dialogFont.GetStyle()));
  weightChoice->SetStringSelection(wxFontWeightIntToString(dialogFont.GetWeight()));
  wxString name(wxTheColourDatabase->FindName(fontData.fontColour));
  colourChoice->SetStringSelection(name);
    
  underLineCheckBox->SetValue(dialogFont.GetUnderlined());

  pointSizeChoice->SetSelection(dialogFont.GetPointSize());

  okButton->SetDefault();

  SetClientSize(400, by + 30);

  Centre(wxBOTH);

  wxEndBusyCursor();
}

void wxGenericFontDialog::InitializeFont(void)
{
  int fontFamily = wxSWISS;
  int fontWeight = wxNORMAL;
  int fontStyle = wxNORMAL;
  int fontSize = 12;
  int fontUnderline = FALSE;
  if (fontData.initialFont.Ok())
  {
    fontFamily = fontData.initialFont.GetFamily();
    fontWeight = fontData.initialFont.GetWeight();
    fontStyle = fontData.initialFont.GetStyle();
    fontSize = fontData.initialFont.GetPointSize();
    fontUnderline = fontData.initialFont.GetUnderlined();
  }
  dialogFont = wxFont(fontSize, fontFamily, fontStyle, fontWeight, (fontUnderline != 0));

}

void wxGenericFontDialog::PaintFontBackground(wxDC& dc)
{
  dc.BeginDrawing();

  dc.SetPen(*wxBLACK_PEN);
  dc.SetBrush(*wxWHITE_BRUSH);
  dc.DrawRectangle( fontRect.x, fontRect.y, fontRect.width, fontRect.height);
  dc.EndDrawing();
}

void wxGenericFontDialog::PaintFont(wxDC& dc)
{
  dc.BeginDrawing();
  if (dialogFont.Ok())
  {
    dc.SetFont(dialogFont);
    // Calculate vertical centre
    long w, h;
    dc.GetTextExtent("X", &w, &h);
    float cx = (float)(fontRect.x + 10);
    float cy = (float)(fontRect.y + (fontRect.height/2.0) - (h/2.0));
    dc.SetTextForeground(fontData.fontColour);
    dc.SetClippingRegion( fontRect.x, fontRect.y, (long)(fontRect.width-2.0), (long)(fontRect.height-2.0));
    dc.DrawText("ABCDEFGabcdefg12345", (long)cx, (long)cy);
    dc.DestroyClippingRegion();
	dc.SetFont(wxNullFont);
  }
  dc.EndDrawing();
}

void wxGenericFontDialog::OnChangeFont(wxCommandEvent& WXUNUSED(event))
{
  int fontFamily = wxFontFamilyStringToInt(WXSTRINGCAST familyChoice->GetStringSelection());
  int fontWeight = wxFontWeightStringToInt(WXSTRINGCAST weightChoice->GetStringSelection());
  int fontStyle = wxFontStyleStringToInt(WXSTRINGCAST styleChoice->GetStringSelection());
  int fontSize = atoi(pointSizeChoice->GetStringSelection());
  int fontUnderline = underLineCheckBox->GetValue();

  dialogFont = wxFont(fontSize, fontFamily, fontStyle, fontWeight, (fontUnderline != 0));
  if (colourChoice->GetStringSelection() != "")
  {
    wxColour *col = wxTheColourDatabase->FindColour(colourChoice->GetStringSelection());
    if (col)
    {
      fontData.fontColour = *col;
    }
  }
  wxClientDC dc(this);
  PaintFontBackground(dc);
  PaintFont(dc);
}

char *wxFontWeightIntToString(int weight)
{
  switch (weight)
  {
    case wxLIGHT:
      return "Light";
    case wxBOLD:
      return "Bold";
    case wxNORMAL:
    default:
      return "Normal";
  }
  return "Normal";
}

char *wxFontStyleIntToString(int style)
{
  switch (style)
  {
    case wxITALIC:
      return "Italic";
    case wxSLANT:
      return "Slant";
    case wxNORMAL:
    default:
      return "Normal";
  }
  return "Normal";
}

char *wxFontFamilyIntToString(int family)
{
  switch (family)
  {
    case wxROMAN:
      return "Roman";
    case wxDECORATIVE:
      return "Decorative";
    case wxMODERN:
      return "Modern";
    case wxSCRIPT:
      return "Script";
    case wxSWISS:
    default:
      return "Swiss";
  }
  return "Swiss";
}

int wxFontFamilyStringToInt(char *family)
{
  if (!family)
    return wxSWISS;
    
  if (strcmp(family, "Roman") == 0)
    return wxROMAN;
  else if (strcmp(family, "Decorative") == 0)
    return wxDECORATIVE;
  else if (strcmp(family, "Modern") == 0)
    return wxMODERN;
  else if (strcmp(family, "Script") == 0)
    return wxSCRIPT;
  else return wxSWISS;
}

int wxFontStyleStringToInt(char *style)
{
  if (!style)
    return wxNORMAL;
  if (strcmp(style, "Italic") == 0)
    return wxITALIC;
  else if (strcmp(style, "Slant") == 0)
    return wxSLANT;
  else
    return wxNORMAL;
}

int wxFontWeightStringToInt(char *weight)
{
  if (!weight)
    return wxNORMAL;
  if (strcmp(weight, "Bold") == 0)
    return wxBOLD;
  else if (strcmp(weight, "Light") == 0)
    return wxLIGHT;
  else
    return wxNORMAL;
}



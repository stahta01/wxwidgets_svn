/////////////////////////////////////////////////////////////////////////////
// Name:        radiobox.cpp
// Purpose:     wxRadioBox
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "radiobox.h"
#endif

#include "wx/radiobox.h"
#include "wx/utils.h"

#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>

#include <wx/motif/private.h>

void wxRadioBoxCallback (Widget w, XtPointer clientData,
                    XmToggleButtonCallbackStruct * cbs);

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxRadioBox, wxControl)
#endif

// Radio box item
wxRadioBox::wxRadioBox()
{
    m_selectedButton = -1;
    m_noItems = 0;
    m_noRowsOrCols = 0;
    m_majorDim = 0 ;

    m_formWidget = (WXWidget) 0;
    m_frameWidget = (WXWidget) 0;
    m_labelWidget = (WXWidget) 0;
    m_radioButtons = (WXWidget*) NULL;
    m_radioButtonLabels = (wxString*) NULL;
}

bool wxRadioBox::Create(wxWindow *parent, wxWindowID id, const wxString& title,
             const wxPoint& pos, const wxSize& size,
             int n, const wxString choices[],
             int majorDim, long style,
             const wxValidator& val, const wxString& name)
{
    m_selectedButton = -1;
    m_noItems = n;
    m_formWidget = (WXWidget) 0;
    m_frameWidget = (WXWidget) 0;
    m_labelWidget = (WXWidget) 0;
    m_radioButtons = (WXWidget*) NULL;
    m_radioButtonLabels = (wxString*) NULL;
    m_backgroundColour = parent->GetBackgroundColour();
    m_foregroundColour = parent->GetForegroundColour();
    m_font = parent->GetFont();

    SetName(name);
    SetValidator(val);

    parent->AddChild(this);

    m_windowStyle = (long&)style;

    if (id == -1)
        m_windowId = NewControlId();
    else
        m_windowId = id;

    m_noRowsOrCols = majorDim;

    if (majorDim==0)
        m_majorDim = n ;
    else
        m_majorDim = majorDim ;

    Widget parentWidget = (Widget) parent->GetClientWidget();

    wxString label1(wxStripMenuCodes(title));

    Widget formWidget = XtVaCreateManagedWidget (name.c_str(),
                                        xmFormWidgetClass, parentWidget,
                                        XmNmarginHeight, 0,
                                        XmNmarginWidth, 0,
                                        NULL);

    m_formWidget = (WXWidget) formWidget;

    XmFontList fontList = (XmFontList) m_font.GetFontList(1.0, XtDisplay(parentWidget));
    if (label1 != "")
    {
        wxXmString text(label1);
        (void)XtVaCreateManagedWidget(label1.c_str(),
#if wxUSE_GADGETS
                                             style & wxCOLOURED ? xmLabelWidgetClass
                                                                : xmLabelGadgetClass,
                                             formWidget,
#else
                                             xmLabelWidgetClass, formWidget,
#endif
                                             XmNfontList, fontList,
                                             XmNlabelString, text(),
                                             NULL);
    }

    Widget frameWidget = XtVaCreateManagedWidget ("frame",
                                        xmFrameWidgetClass, formWidget,
                                        XmNshadowType, XmSHADOW_IN,
//                                        XmNmarginHeight, 0,
//                                        XmNmarginWidth, 0,
                                        NULL);

    m_frameWidget = (WXWidget) frameWidget;

    Arg args[3];

    m_majorDim = (n + m_majorDim - 1) / m_majorDim;

    XtSetArg (args[0], XmNorientation, ((style & wxHORIZONTAL) == wxHORIZONTAL ?
                                          XmHORIZONTAL : XmVERTICAL));
    XtSetArg (args[1], XmNnumColumns, m_majorDim);

    Widget radioBoxWidget = XmCreateRadioBox (frameWidget, "radioBoxWidget", args, 2);
    m_mainWidget = (WXWidget) radioBoxWidget;


    if (m_labelWidget)
            XtVaSetValues ((Widget) m_labelWidget,
                       XmNtopAttachment, XmATTACH_FORM,
                       XmNleftAttachment, XmATTACH_FORM,
                       XmNalignment, XmALIGNMENT_BEGINNING,
                       NULL);

    XtVaSetValues (radioBoxWidget,
            XmNtopAttachment, m_labelWidget ? XmATTACH_WIDGET : XmATTACH_FORM,
                     XmNtopWidget, m_labelWidget ? (Widget) m_labelWidget : formWidget,
                     XmNbottomAttachment, XmATTACH_FORM,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNrightAttachment, XmATTACH_FORM,
                     NULL);

    //    if (style & wxFLAT)
    //        XtVaSetValues (radioBoxWidget, XmNborderWidth, 1, NULL);

    m_radioButtons = new WXWidget[n];
    m_radioButtonLabels = new wxString[n];
    int i;
    for (i = 0; i < n; i++)
    {
        wxString str(wxStripMenuCodes(choices[i]));
        m_radioButtonLabels[i] = str;
        m_radioButtons[i] = (WXWidget) XtVaCreateManagedWidget ((char*) (const char*) str,
#if wxUSE_GADGETS
                            xmToggleButtonGadgetClass, radioBoxWidget,
#else
                                    xmToggleButtonWidgetClass, radioBoxWidget,
#endif
                                    XmNfontList, fontList,
                                                 NULL);
        XtAddCallback ((Widget) m_radioButtons[i], XmNvalueChangedCallback, (XtCallbackProc) wxRadioBoxCallback,
                     (XtCallbackProc) this);

    }
    SetSelection (0);

    m_font = parent->GetFont();
    ChangeFont(FALSE);

    //    XtManageChild((Widget) m_formWidget);
    XtManageChild (radioBoxWidget);

    SetCanAddEventHandler(TRUE);
    AttachWidget (parent, m_mainWidget, m_formWidget, pos.x, pos.y, size.x, size.y);

    ChangeBackgroundColour();

    return TRUE;
}


wxRadioBox::~wxRadioBox()
{
    delete[] m_radioButtonLabels;
    delete[] m_radioButtons;

    DetachWidget(m_formWidget);
    DetachWidget(m_mainWidget);

    if (m_labelWidget)
       XtDestroyWidget((Widget) m_labelWidget);
    XtDestroyWidget((Widget) m_mainWidget);
    XtDestroyWidget((Widget) m_formWidget);

    m_mainWidget = (WXWidget) 0;
    m_formWidget = (WXWidget) 0;
    m_labelWidget = (WXWidget) 0;
}

wxString wxRadioBox::GetLabel(int item) const
{
    if (item < 0 || item >= m_noItems)
        return wxEmptyString;

    Widget widget = (Widget) m_radioButtons[item];
    XmString text;
    char *s;
    XtVaGetValues (widget,
                    XmNlabelString, &text,
                    NULL);

    if (XmStringGetLtoR (text, XmSTRING_DEFAULT_CHARSET, &s))
    {
        // Should we free 'text'???
        XmStringFree(text);
        wxString str(s);
        XtFree (s);
        return str;
    }
    else
    {
        XmStringFree(text);
        return wxEmptyString;
    }
}

void wxRadioBox::SetLabel(int item, const wxString& label)
{
    if (item < 0 || item >= m_noItems)
        return;

    Widget widget = (Widget) m_radioButtons[item];
    if (label != "")
    {
        wxString label1(wxStripMenuCodes(label));
        XmString text = XmStringCreateSimple ((char*) (const char*) label1);
        XtVaSetValues (widget,
                        XmNlabelString, text,
                        XmNlabelType, XmSTRING,
                        NULL);
        XmStringFree (text);
    }
}

int wxRadioBox::FindString(const wxString& s) const
{
    int i;
    for (i = 0; i < m_noItems; i++)
        if (s == m_radioButtonLabels[i])
            return i;
    return -1;
}

void wxRadioBox::SetSelection(int n)
{
    if ((n < 0) || (n >= m_noItems))
        return;

    m_selectedButton = n;

    m_inSetValue = TRUE;

    XmToggleButtonSetState ((Widget) m_radioButtons[n], TRUE, FALSE);

    int i;
    for (i = 0; i < m_noItems; i++)
        if (i != n)
            XmToggleButtonSetState ((Widget) m_radioButtons[i], FALSE, FALSE);

    m_inSetValue = FALSE;
}

// Get single selection, for single choice list items
int wxRadioBox::GetSelection() const
{
    return m_selectedButton;
}

// Find string for position
wxString wxRadioBox::GetString(int n) const
{
    if ((n < 0) || (n >= m_noItems))
        return wxEmptyString;
    return m_radioButtonLabels[n];
}

void wxRadioBox::DoSetSize(int x, int y, int width, int height, int sizeFlags)
{
    bool managed = XtIsManaged((Widget) m_formWidget);

    if (managed)
        XtUnmanageChild ((Widget) m_formWidget);

    int xx = x; int yy = y;
    AdjustForParentClientOrigin(xx, yy, sizeFlags);

    if (x > -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
        XtVaSetValues ((Widget) m_formWidget, XmNleftAttachment, XmATTACH_SELF,
                   XmNx, xx, NULL);
    if (y > -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
        XtVaSetValues ((Widget) m_formWidget, XmNtopAttachment, XmATTACH_SELF,
                   XmNy, yy, NULL);

    // Must set the actual RadioBox to be desired size MINUS label size
    Dimension labelWidth = 0, labelHeight = 0, actualWidth = 0, actualHeight = 0;

    if (m_labelWidget)
        XtVaGetValues ((Widget) m_labelWidget, XmNwidth, &labelWidth, XmNheight, &labelHeight, NULL);

    actualWidth = width;
    actualHeight = height - labelHeight;

    if (width > -1)
    {
        XtVaSetValues ((Widget) m_mainWidget, XmNwidth, actualWidth, NULL);
    }
    if (height > -1)
    {
        XtVaSetValues ((Widget) m_mainWidget, XmNheight, actualHeight, NULL);
    }
    if (managed)
        XtManageChild ((Widget) m_formWidget);
}

// Enable a specific button
void wxRadioBox::Enable(int n, bool enable)
{
    if ((n < 0) || (n >= m_noItems))
        return;

    XtSetSensitive ((Widget) m_radioButtons[n], (Boolean) enable);
}

// Enable all controls
bool wxRadioBox::Enable(bool enable)
{
    if ( !wxControl::Enable(enable) )
        return FALSE;

    int i;
    for (i = 0; i < m_noItems; i++)
        XtSetSensitive ((Widget) m_radioButtons[i], (Boolean) enable);

    return TRUE;
}

bool wxRadioBox::Show(bool show)
{
    // TODO: show/hide all children
    return wxControl::Show(show);
}

// Show a specific button
void wxRadioBox::Show(int n, bool show)
{
  // This method isn't complete, and we try do do our best...
  // It's main purpose isn't for allowing Show/Unshow dynamically,
  // but rather to provide a way to design wxRadioBox such:
  //
  //        o Val1  o Val2   o Val3 
  //        o Val4           o Val6 
  //        o Val7  o Val8   o Val9 
  //
  // In my case, this is a 'direction' box, and the Show(5,False) is
  // coupled with an Enable(5,False)
  //
    if ((n < 0) || (n >= m_noItems))
        return;

    XtVaSetValues ((Widget) m_radioButtons[n],
        XmNindicatorOn, (unsigned char) show,
        NULL);

    // Please note that this is all we can do: removing the label
    // if switching to unshow state. However, when switching
    // to the on state, it's the prog. resp. to call SetLabel(item,...)
    // after this call!!
    if (!show)
        wxRadioBox::SetLabel (n, " ");
}

// For single selection items only
wxString wxRadioBox::GetStringSelection () const
{
    int sel = GetSelection ();
    if (sel > -1)
        return this->GetString (sel);
    else
        return wxString("");
}

bool wxRadioBox::SetStringSelection (const wxString& s)
{
    int sel = FindString (s);
    if (sel > -1)
    {
        SetSelection (sel);
        return TRUE;
    }
    else
        return FALSE;
}

void wxRadioBox::Command (wxCommandEvent & event)
{
    SetSelection (event.m_commandInt);
    ProcessCommand (event);
}

void wxRadioBox::ChangeFont(bool keepOriginalSize)
{
    wxWindow::ChangeFont(keepOriginalSize);

    XmFontList fontList = (XmFontList) m_font.GetFontList(1.0, XtDisplay((Widget) GetTopWidget()));

    int i;
    for (i = 0; i < m_noItems; i++)
    {
        WXWidget radioButton = m_radioButtons[i];

        XtVaSetValues ((Widget) radioButton,
                       XmNfontList, fontList,
                       XmNtopAttachment, XmATTACH_FORM,
                       NULL);
    }
}

void wxRadioBox::ChangeBackgroundColour()
{
    wxWindow::ChangeBackgroundColour();

    DoChangeBackgroundColour((Widget) m_frameWidget, m_backgroundColour);

    int selectPixel = wxBLACK->AllocColour(wxGetDisplay());

    int i;
    for (i = 0; i < m_noItems; i++)
    {
        WXWidget radioButton = m_radioButtons[i];

        DoChangeBackgroundColour(radioButton, m_backgroundColour, TRUE);

        XtVaSetValues ((Widget) radioButton,
          XmNselectColor, selectPixel,
          NULL);
    }
}

void wxRadioBox::ChangeForegroundColour()
{
    wxWindow::ChangeForegroundColour();

    int i;
    for (i = 0; i < m_noItems; i++)
    {
        WXWidget radioButton = m_radioButtons[i];

        DoChangeForegroundColour(radioButton, m_foregroundColour);
    }
}

void wxRadioBoxCallback (Widget w, XtPointer clientData,
                    XmToggleButtonCallbackStruct * cbs)
{
  if (!cbs->set)
    return;

  wxRadioBox *item = (wxRadioBox *) clientData;
  int sel = -1;
  int i;
  for (i = 0; i < item->Number(); i++)
    if (item->GetRadioButtons() && ((Widget) (item->GetRadioButtons()[i]) == w))
      sel = i;
  item->SetSel(sel);

  if (item->InSetValue())
    return;

  wxCommandEvent event (wxEVT_COMMAND_RADIOBOX_SELECTED, item->GetId());
  event.m_commandInt = sel;
  event.SetEventObject(item);
  item->ProcessCommand (event);
}


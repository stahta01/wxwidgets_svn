/////////////////////////////////////////////////////////////////////////////
// Name:        combobox.cpp
// Purpose:     wxComboBox class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "combobox.h"
#endif

#include "wx/combobox.h"

#if wxUSE_COMBOBOX

#include <Xm/Xm.h>
#include "combobox/combobox.h"

void  wxComboBoxCallback (Widget w, XtPointer clientData,
		   XmComboBoxSelectionCallbackStruct * cbs);

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxComboBox, wxControl)
#endif

bool wxComboBox::Create(wxWindow *parent, wxWindowID id,
           const wxString& value,
           const wxPoint& pos,
           const wxSize& size,
		   int n, const wxString choices[],
		   long style,
           const wxValidator& validator,
           const wxString& name)
{
    SetName(name);
    SetValidator(validator);
    m_noStrings = n;
    m_windowStyle = style;

    if (parent) parent->AddChild(this);

    if ( id == -1 )
    	m_windowId = (int)NewControlId();
    else
	m_windowId = id;

    Widget parentWidget = (Widget) parent->GetClientWidget();

    Widget buttonWidget = XtVaCreateManagedWidget((char*) (const char*) name,
					 xmComboBoxWidgetClass, parentWidget,
 					 XmNmarginHeight, 0,
					 XmNmarginWidth, 0,
					 XmNshowLabel, False,
       XmNeditable, ((style & wxCB_READONLY) != wxCB_READONLY),
       XmNsorted, ((style & wxCB_SORT) == wxCB_SORT),
       XmNstaticList, ((style & wxCB_SIMPLE) == wxCB_SIMPLE),
					 NULL);

    XtAddCallback (buttonWidget, XmNselectionCallback, (XtCallbackProc) wxComboBoxCallback,
		 (XtPointer) this);
    XtAddCallback (buttonWidget, XmNvalueChangedCallback, (XtCallbackProc) wxComboBoxCallback,
		 (XtPointer) this);

    int i;
    for (i = 0; i < n; i++)
    {
        XmString str = XmStringCreateLtoR((char*) (const char*) choices[i], XmSTRING_DEFAULT_CHARSET);
        XmComboBoxAddItem(buttonWidget, str, 0);
        XmStringFree(str);
        m_stringList.Add(choices[i]);
    }
    m_noStrings = n;

    m_mainWidget = (Widget) buttonWidget;

    XtManageChild (buttonWidget);

    SetValue(value);

    SetCanAddEventHandler(TRUE);
    AttachWidget (parent, m_mainWidget, (WXWidget) NULL, pos.x, pos.y, size.x, size.y);

    SetFont(* parent->GetFont());
    ChangeColour(m_mainWidget);

    return TRUE;
}

wxString wxComboBox::GetValue() const
{
    char *s = XmComboBoxGetString ((Widget) m_mainWidget);
    if (s)
    {
        wxString str(s);
        XtFree (s);
        return str;
    }
    else
        return wxEmptyString;
}

void wxComboBox::SetValue(const wxString& value)
{
    m_inSetValue = TRUE;
    if (!value.IsNull())
        XmComboBoxSetString ((Widget) m_mainWidget, (char*) (const char*) value);
    m_inSetValue = FALSE;
}

// Clipboard operations
void wxComboBox::Copy()
{
    XmComboBoxCopy((Widget) m_mainWidget, CurrentTime);
}

void wxComboBox::Cut()
{
    XmComboBoxCut((Widget) m_mainWidget, CurrentTime);
}

void wxComboBox::Paste()
{
    XmComboBoxPaste((Widget) m_mainWidget);
}

void wxComboBox::SetEditable(bool editable)
{
    // TODO
}

void wxComboBox::SetInsertionPoint(long pos)
{
    XmComboBoxSetInsertionPosition ((Widget) m_mainWidget, (XmTextPosition) pos);
}

void wxComboBox::SetInsertionPointEnd()
{
    XmTextPosition pos = XmComboBoxGetLastPosition ((Widget) m_mainWidget);
    XmComboBoxSetInsertionPosition ((Widget) m_mainWidget, (XmTextPosition) (pos + 1));
}

long wxComboBox::GetInsertionPoint() const
{
    return (long) XmComboBoxGetInsertionPosition ((Widget) m_mainWidget);
}

long wxComboBox::GetLastPosition() const
{
    return (long) XmComboBoxGetLastPosition ((Widget) m_mainWidget);
}

void wxComboBox::Replace(long from, long to, const wxString& value)
{
    XmComboBoxReplace ((Widget) m_mainWidget, (XmTextPosition) from, (XmTextPosition) to,
		 (char*) (const char*) value);
}

void wxComboBox::Remove(long from, long to)
{
    XmComboBoxSetSelection ((Widget) m_mainWidget, (XmTextPosition) from, (XmTextPosition) to,
		      (Time) 0);
    XmComboBoxRemove ((Widget) m_mainWidget);
}

void wxComboBox::SetSelection(long from, long to)
{
    XmComboBoxSetSelection ((Widget) m_mainWidget, (XmTextPosition) from, (XmTextPosition) to,
		      (Time) 0);
}

void  wxComboBoxCallback (Widget w, XtPointer clientData,
		   XmComboBoxSelectionCallbackStruct * cbs)
{
    wxComboBox *item = (wxComboBox *) clientData;

    switch (cbs->reason)
    {
        case XmCR_SINGLE_SELECT:
        case XmCR_BROWSE_SELECT:
        {
            wxCommandEvent event (wxEVT_COMMAND_COMBOBOX_SELECTED, item->GetId());
	        event.m_commandInt = cbs->index - 1;
		//	        event.m_commandString = item->GetString (event.m_commandInt);
	        event.m_extraLong = TRUE;
	        event.SetEventObject(item);
	        item->ProcessCommand (event);
	        break;
        }
        case XmCR_VALUE_CHANGED:
        {
            wxCommandEvent event (wxEVT_COMMAND_TEXT_UPDATED, item->GetId());
	        event.m_commandInt = -1;
		//	        event.m_commandString = item->GetValue();
	        event.m_extraLong = TRUE;
	        event.SetEventObject(item);
	        item->ProcessCommand (event);
            break;
        }
        default:
            break;
    }
}

#endif


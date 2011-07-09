/////////////////////////////////////////////////////////////////////////////
// Name:        src/mobile/generic/mo_tablecell_n.mm
// Purpose:     wxMoTableCell class
// Author:      Julian Smart, Linas Valiukas
// Modified by:
// Created:     2011-06-14
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart, Linas Valiukas
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/brush.h"
    #include "wx/panel.h"
    #include "wx/bmpbuttn.h"
    #include "wx/settings.h"
    #include "wx/dcscreen.h"
    #include "wx/dcclient.h"
    #include "wx/toplevel.h"
#endif

#include "wx/dcbuffer.h"

#include "wx/osx/private.h"
#include "wx/mobile/native/tablectrl.h"
#include "wx/mobile/native/tablecell.h"
#include "wx/mobile/native/settings.h"


#pragma mark wxMoTableCell

IMPLEMENT_CLASS(wxMoTableCell, wxObject)

wxMoTableCell::wxMoTableCell(wxMoTableCtrl* ctrl, const wxString& reuseName, wxMoTableCellStyle cellStyle)
{
    Init();
    
    m_reuseName = reuseName;
    m_cellStyle = cellStyle;
    
    SetCellWidgetImpl(wxWidgetImpl::CreateTableViewCell( this ));
}

wxMoTableCell::~wxMoTableCell()
{
    NSLog(@"cell is being freed");
    // FIXME stub
}

void wxMoTableCell::Init()
{
    m_widgetImpl = NULL;
    
    m_textAlignment = TextAlignmentLeft;
    m_detailTextAlignment = TextAlignmentLeft;
    m_textLineBreakMode = LineBreakModeWordWrap;
    m_detailTextLineBreakMode = LineBreakModeWordWrap;
    m_selected = false;
    m_selectionStyle = SelectionStyleBlue;
    //m_eventHandler = NULL;
    m_accessoryType = AccessoryTypeNone;
    m_accessoryWindow = NULL;
    m_editingAccessoryType = AccessoryTypeNone;
    m_editingAccessoryWindow = NULL;
    //m_editStyle = EditStyleNone;
    m_indentationLevel = 0;
    m_indentationWidth = 0;
    m_contentWindow = NULL;
    m_cellStyle = CellStyleDefault;
    m_detailWidth = 80;
    m_editingMode = false;
    m_shouldIndentWhileEditing = true;
    m_showReorderingButton = false;    
}

void wxMoTableCell::Copy(const wxMoTableCell& cell)
{
    // FIXME stub
}

bool wxMoTableCell::CreateContentWindow(wxMoTableCtrl* ctrl)
{
    // FIXME stub

    return true;
}

// Sets the accessory window
void wxMoTableCell::SetAccessoryWindow(wxWindow* win)
{
    UIView *accessoryView = win->GetPeer()->GetWXWidget();
    if ( !accessoryView ) {
        return;
    }
    
    UITableViewCell *cell = (UITableViewCell *)m_widgetImpl->GetWXWidget();
    if ( !cell ) {
        return;
    }
    
    [cell setAccessoryView:accessoryView];
    m_accessoryWindow = win;
}

/// Gets the custom accessory window.
wxWindow* wxMoTableCell::GetAccessoryWindow() const
{
    return m_accessoryWindow;
}

/// Sets the indentation level.
void wxMoTableCell::SetIndentationLevel(int indentationLevel)
{
    UITableViewCell *cell = (UITableViewCell *)m_widgetImpl->GetWXWidget();
    if ( !cell ) {
        return;
    }
    
    [cell setIndentationLevel:indentationLevel];
    
    m_indentationLevel = indentationLevel;
}

/// Gets the indentation level.
int wxMoTableCell::GetIndentationLevel() const
{
    return m_indentationLevel;
}

/// Sets the indentation width.
void wxMoTableCell::SetIndentationWidth(int indentationWidth)
{
    UITableViewCell *cell = (UITableViewCell *)m_widgetImpl->GetWXWidget();
    if ( !cell ) {
        return;
    }
    
    [cell setIndentationWidth:indentationWidth];
    
    m_indentationWidth = indentationWidth;
}

/// Gets the indentation width.
int wxMoTableCell::GetIndentationWidth() const
{
    return m_indentationWidth;
}

// Sets the editing accessory window
void wxMoTableCell::SetEditingAccessoryWindow(wxWindow* win)
{
    // FIXME stub
}

// Sets editing mode (not yet implemented).
bool wxMoTableCell::SetEditingMode(bool editingMode, bool WXUNUSED(animated))
{
    // FIXME stub
    // TODO

    return true;
}

// Is the delete confirmation button showing for this path?
bool wxMoTableCell::IsDeleteButtonShowing(wxMoTableCtrl* tableCtrl) const
{
    // FIXME stub

    return true;
}

// Prepares the cell for reuse
void wxMoTableCell::PrepareForReuse(wxMoTableCtrl* WXUNUSED(tableCtrl))
{
    // FIXME stub
}


#pragma mark wxMoTableCellContentWindow

IMPLEMENT_DYNAMIC_CLASS(wxMoTableCellContentWindow, wxWindow)

BEGIN_EVENT_TABLE(wxMoTableCellContentWindow, wxWindow)
    EVT_PAINT(wxMoTableCellContentWindow::OnPaint)
    EVT_MOUSE_EVENTS(wxMoTableCellContentWindow::OnMouseEvents)
END_EVENT_TABLE()

bool wxMoTableCellContentWindow::Create(wxWindow* parent,
                                        wxWindowID id,
                                        const wxPoint& pos,
                                        const wxSize& sz,
                                        long style)
{
    // FIXME stub

    return true;
}

void wxMoTableCellContentWindow::Init()
{
    // FIXME stub
}

void wxMoTableCellContentWindow::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    // FIXME stub
}

void wxMoTableCellContentWindow::OnMouseEvents(wxMouseEvent& event)
{
    // FIXME stub
}

/////////////////////////////////////////////////////////////////////////////
// Name:        configtree.h
// Purpose:     wxWindows Configuration Tool tree class
// Author:      Julian Smart
// Modified by:
// Created:     2003-06-03
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "configtree.h"
#endif

// Includes other headers for precompiled compilation
#include "wx/wx.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/imaglist.h"
#include "wx/cshelp.h"

// Include XPM icons
#include "bitmaps/closedfolder.xpm"
#include "bitmaps/closedfolder_dis.xpm"

#include "bitmaps/checked.xpm"
#include "bitmaps/checked_dis.xpm"
#include "bitmaps/unchecked.xpm"
#include "bitmaps/unchecked_dis.xpm"

#include "bitmaps/radioon.xpm"
#include "bitmaps/radioon_dis.xpm"
#include "bitmaps/radiooff.xpm"
#include "bitmaps/radiooff_dis.xpm"

#include "bitmaps/checkedfolder.xpm"
#include "bitmaps/checkedfolder_dis.xpm"
#include "bitmaps/uncheckedfolder.xpm"
#include "bitmaps/uncheckedfolder_dis.xpm"

#include "bitmaps/radiofolderon.xpm"
#include "bitmaps/radiofolderon_dis.xpm"
#include "bitmaps/radiofolderoff.xpm"
#include "bitmaps/radiofolderoff_dis.xpm"

#include "configtree.h"
#include "configtooldoc.h"
#include "configtoolview.h"
#include "wxconfigtool.h"
#include "mainframe.h"

/*
 * ctConfigTreeCtrl
 */

IMPLEMENT_CLASS(ctConfigTreeCtrl, wxTreeCtrl)

BEGIN_EVENT_TABLE(ctConfigTreeCtrl, wxTreeCtrl)
    EVT_MOUSE_EVENTS(ctConfigTreeCtrl::OnMouseEvent)
    EVT_CHAR(ctConfigTreeCtrl::OnKeyDown)
    EVT_TREE_SEL_CHANGED(-1, ctConfigTreeCtrl::OnSelChanged)
    EVT_HELP(-1, ctConfigTreeCtrl::OnHelp)
END_EVENT_TABLE()

ctConfigTreeCtrl::ctConfigTreeCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pt,
                                   const wxSize& sz, long style):
wxTreeCtrl(parent, id, pt, sz, style)
{
    LoadIcons();

    m_contextMenu = NULL;
    m_contextMenu = new wxMenu;

    m_contextMenu->Append(ctID_TREE_PASTE_BEFORE, _("Paste &before this option"));
    m_contextMenu->Append(ctID_TREE_PASTE_AFTER, _("Paste &after this option"));
    m_contextMenu->Append(ctID_TREE_PASTE_AS_CHILD, _("Paste as &child of this option"));
    m_contextMenu->AppendSeparator();
    m_contextMenu->Append(ctID_TREE_COPY, _("C&opy"));
    m_contextMenu->Append(ctID_TREE_CUT, _("Cu&t"));

    SetHelpText(_("This shows configuration settings that you can enable and disable."));
}

// Load the icons and initialize the tree
void ctConfigTreeCtrl::LoadIcons()
{
    m_imageList = new wxImageList(16, 16, TRUE);
    m_iconTable.SetImageList(m_imageList);
    SetImageList(m_imageList);
    
    m_iconTable.AddInfo("Group", wxIcon(closedfolder_xpm), 0, TRUE);
    m_iconTable.AddInfo("Group", wxIcon(closedfolder_dis_xpm), 0, FALSE);
    
    m_iconTable.AddInfo("Checkbox", wxIcon(checked_xpm), 0, TRUE);
    m_iconTable.AddInfo("Checkbox", wxIcon(checked_dis_xpm), 0, FALSE);
    m_iconTable.AddInfo("Checkbox", wxIcon(unchecked_xpm), 1, TRUE);
    m_iconTable.AddInfo("Checkbox", wxIcon(unchecked_dis_xpm), 1, FALSE);
    
    m_iconTable.AddInfo("CheckGroup", wxIcon(checkedfolder_xpm), 0, TRUE);
    m_iconTable.AddInfo("CheckGroup", wxIcon(checkedfolder_dis_xpm), 0, FALSE);
    m_iconTable.AddInfo("CheckGroup", wxIcon(uncheckedfolder_xpm), 1, TRUE);
    m_iconTable.AddInfo("CheckGroup", wxIcon(uncheckedfolder_dis_xpm), 1, FALSE);
    
    m_iconTable.AddInfo("RadioGroup", wxIcon(radiofolderon_xpm), 0, TRUE);
    m_iconTable.AddInfo("RadioGroup", wxIcon(radiofolderon_dis_xpm), 0, FALSE);
    m_iconTable.AddInfo("RadioGroup", wxIcon(radiofolderoff_xpm), 1, TRUE);
    m_iconTable.AddInfo("RadioGroup", wxIcon(radiofolderoff_dis_xpm), 1, FALSE);
    
    m_iconTable.AddInfo("Radiobutton", wxIcon(radioon_xpm), 0, TRUE);
    m_iconTable.AddInfo("Radiobutton", wxIcon(radioon_dis_xpm), 0, FALSE);
    m_iconTable.AddInfo("Radiobutton", wxIcon(radiooff_xpm), 1, TRUE);
    m_iconTable.AddInfo("Radiobutton", wxIcon(radiooff_dis_xpm), 1, FALSE);
}

ctConfigTreeCtrl::~ctConfigTreeCtrl()
{
    SetImageList(NULL);
    delete m_imageList;

    delete m_contextMenu;
}

void ctConfigTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
    ctConfigToolDoc* doc = wxGetApp().GetMainFrame()->GetDocument();
    if (doc)
    {
        ctConfigToolHint hint(NULL, ctSelChanged);
        doc->UpdateAllViews(NULL, & hint);
    }
}

void ctConfigTreeCtrl::OnMouseEvent(wxMouseEvent& event)
{
    int flags = 0;
    wxTreeItemId item = HitTest(wxPoint(event.GetX(), event.GetY()), flags);
    
    if (event.LeftDown())
    {
        if (flags & wxTREE_HITTEST_ONITEMICON)
        {
            ctTreeItemData* data = (ctTreeItemData*) GetItemData(item);
            ctConfigToolDoc* doc = wxGetApp().GetMainFrame()->GetDocument();
            if (doc)
            {
                ctConfigToolView* view = wxDynamicCast(doc->GetFirstView(), ctConfigToolView);
                if (view)
                    view->OnIconLeftDown(this, data->GetConfigItem());
            }
        }
    }
    else if (event.RightDown())
    {
        if ((flags & wxTREE_HITTEST_ONITEMBUTTON) ||
            (flags & wxTREE_HITTEST_ONITEMICON) ||
            (flags & wxTREE_HITTEST_ONITEMINDENT) ||
            (flags & wxTREE_HITTEST_ONITEMLABEL))
        {
            ctTreeItemData* data = (ctTreeItemData*) GetItemData(item);
            ctConfigToolDoc* doc = wxGetApp().GetMainFrame()->GetDocument();
            if (doc && data)
            {
                m_contextItem = data->GetConfigItem();
                PopupMenu(m_contextMenu, event.GetX(), event.GetY());
            }
        }
    }

    event.Skip();
}

void ctConfigTreeCtrl::OnKeyDown(wxKeyEvent& event)
{
    wxTreeItemId id = GetSelection();
    if (event.GetKeyCode() == WXK_SPACE)
    {
        if (id.IsOk())
        {
            ctConfigItem* item = ((ctTreeItemData*) GetItemData(id))->GetConfigItem();
            ctConfigToolDoc* doc = wxGetApp().GetMainFrame()->GetDocument();
            if (doc)
            {
                ctConfigToolView* view = wxDynamicCast(doc->GetFirstView(), ctConfigToolView);
                if (view)
                    view->OnIconLeftDown(this, item);
            }
        }
    }
    else
    {
        event.Skip();
    }
}

// Show help for this window
void ctConfigTreeCtrl::OnHelp(wxHelpEvent& event)
{
    wxPoint pt = ScreenToClient(event.GetPosition());
    int flags = 0;
    wxTreeItemId id = HitTest(pt, flags);
    wxHelpProvider *helpProvider = wxHelpProvider::Get();
    if ( helpProvider && id > 0)
    {
        ctConfigItem* item = ((ctTreeItemData*) GetItemData(id))->GetConfigItem();
        if (item)
        {
            wxString helpTopic = item->GetPropertyString(wxT("help-topic"));
            if (!helpTopic.IsEmpty())
            {
                wxGetApp().GetReferenceHelpController().DisplaySection(helpTopic);
                return;
            }
	    }
    }

    event.Skip();
}

ctTreeItemData::~ctTreeItemData()
{
    if (m_configItem) delete m_configItem;
}


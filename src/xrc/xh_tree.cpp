/////////////////////////////////////////////////////////////////////////////
// Name:        xh_tree.cpp
// Purpose:     XRC resource for wxTreeCtrl
// Author:      Brian Gavin
// Created:     2000/09/09
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Brian Gavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
 
#ifdef __GNUG__
#pragma implementation "xh_tree.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/xrc/xh_tree.h"
#include "wx/treectrl.h"


wxTreeCtrlXmlHandler::wxTreeCtrlXmlHandler() 
: wxXmlResourceHandler() 
{
    ADD_STYLE(wxTR_HAS_BUTTONS);
    ADD_STYLE(wxTR_EDIT_LABELS);
    ADD_STYLE(wxTR_MULTIPLE);
    AddWindowStyles();
}


wxObject *wxTreeCtrlXmlHandler::DoCreateResource()
{ 
   wxTreeCtrl *tree = wxStaticCast(m_instance, wxTreeCtrl);

   if (!tree)
       tree = new wxTreeCtrl;

   tree->Create( m_parentAsWindow,
                GetID(),
                GetPosition(), GetSize(),
                GetStyle(),
                wxDefaultValidator,
                GetName());

    SetupWindow(tree);
    
    return tree;
}



bool wxTreeCtrlXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxTreeCtrl"));
}



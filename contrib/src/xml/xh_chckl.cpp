/////////////////////////////////////////////////////////////////////////////
// Name:        xh_chckl.cpp
// Purpose:     XML resource for wxCheckList
// Author:      Bob Mitchell
// Created:     2000/03/21
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
 
#ifdef __GNUG__
#pragma implementation "xh_chckl.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/xml/xh_chckl.h"
#include "wx/checklst.h"

wxCheckListXmlHandler::wxCheckListXmlHandler() 
: wxXmlResourceHandler(), m_InsideBox(FALSE)
{
    // no styles
    AddWindowStyles();
}

wxObject *wxCheckListXmlHandler::DoCreateResource()
{ 
    if (m_Class == wxT("wxCheckList"))
    {
        // need to build the list of strings from children
        m_InsideBox = TRUE;
        CreateChildrenPrivately(NULL, GetParamNode(wxT("content")));
        wxString *strings = (wxString *) NULL;
        if( strList.GetCount() > 0 )
        {
            strings = new wxString[strList.GetCount()];
            int count = strList.GetCount();
            for( int i = 0; i < count; i++ )
                strings[i]=strList[i];
        }


        wxCheckListBox *control = new wxCheckListBox(m_ParentAsWindow,
                                    GetID(),
                                    GetPosition(), GetSize(),
                                    strList.GetCount(),
                                    strings,
                                    GetStyle(),
                                    wxDefaultValidator,
                                    GetName()
                                    );

        // step through children myself (again.)
        wxXmlNode *n = GetParamNode(wxT("content"));
        if (n) n = n->GetChildren();
        int i = 0;
        while (n)
        {
            if (n->GetType() != wxXML_ELEMENT_NODE ||
                n->GetName() != wxT("item"))
               { n = n->GetNext(); continue; }

            // checking boolean is a bit ugly here (see GetBool() )
            wxString v = n->GetPropVal(wxT("checked"), wxEmptyString);
            v.MakeLower();
            if (v && v == wxT("1"))
                control->Check( i, TRUE );

            i++;        
            n = n->GetNext();
        }
        
        SetupWindow(control);

        if( strings != NULL )
            delete [] strings;
        strList.Clear();    // dump the strings   

        return control;
    }
    else
    {
        // on the inside now.
        // handle <item checked="boolean">Label</item>

        // add to the list
        strList.Add( GetNodeContent(m_Node) );

        return NULL;
    }

}



bool wxCheckListXmlHandler::CanHandle(wxXmlNode *node)
{
    return (IsOfClass(node, wxT("wxCheckList")) ||
           (m_InsideBox && node->GetName() == wxT("item"))
           );
}



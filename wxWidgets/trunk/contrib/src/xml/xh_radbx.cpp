/////////////////////////////////////////////////////////////////////////////
// Name:        xh_radbx.cpp
// Purpose:     XML resource for wxRadioBox
// Author:      Bob Mitchell
// Created:     2000/03/21
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
 
#ifdef __GNUG__
#pragma implementation "xh_radbx.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/xml/xh_radbx.h"
#include "wx/radiobox.h"

#if wxUSE_RADIOBOX

wxRadioBoxXmlHandler::wxRadioBoxXmlHandler() 
: wxXmlResourceHandler() , m_InsideBox(FALSE)
{
    ADD_STYLE(wxRA_SPECIFY_COLS);
    ADD_STYLE(wxRA_HORIZONTAL);
    ADD_STYLE(wxRA_SPECIFY_ROWS);
    ADD_STYLE(wxRA_VERTICAL);
}

wxObject *wxRadioBoxXmlHandler::DoCreateResource()
{ 
    if( m_Node->GetName() == _T("radiobox"))
    {
        // find the selection
        long selection = GetLong( _T("selection"), -1 );

        // need to build the list of strings from children
        m_InsideBox = TRUE;
        CreateChildren( NULL, TRUE /* only this handler */, GetParamNode(_T("buttons")));
        wxString *strings = (wxString *) NULL;
        if( strList.GetCount() > 0 )
        {
            strings = new wxString[strList.GetCount()];
            int count = strList.GetCount();
            for( int i = 0; i < count; i++ )
                strings[i]=strList[i];
        }


        wxRadioBox *control = new wxRadioBox(m_ParentAsWindow,
                                    GetID(),
                                    GetText(_T("label")),
                                    GetPosition(), GetSize(),
                                    strList.GetCount(),
                                    strings,
                                    GetLong( _T("dimension"), 0 ),
                                    GetStyle(),
                                    wxDefaultValidator,
                                    GetName()
                                    );

        if( selection != -1 )
            control->SetSelection( selection );

        SetupWindow(control);

        if( strings != NULL )
            delete [] strings;
        strList.Clear();    // dump the strings   

        return control;
    }
    else
    {
        // on the inside now.
        // handle <item selected="boolean">Label</item>

        // add to the list
        strList.Add( GetNodeContent(m_Node) );

        return NULL;
    }

}



bool wxRadioBoxXmlHandler::CanHandle(wxXmlNode *node)
{
    return( node->GetName() == _T("radiobox") ||
        ( m_InsideBox &&
            node->GetName() == _T("item" )) 
        );
}

#endif

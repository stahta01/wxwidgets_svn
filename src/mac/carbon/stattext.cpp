/////////////////////////////////////////////////////////////////////////////
// Name:        stattext.cpp
// Purpose:     wxStaticText
// Author:      AUTHOR
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "stattext.h"
#endif

#include "wx/app.h"
#include "wx/stattext.h"
#include "wx/notebook.h"
#include "wx/tabctrl.h"
#include "wx/dc.h"
#include "wx/dcclient.h"

#include <stdio.h>

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxStaticText, wxControl)
#endif

#include <wx/mac/uma.h>

BEGIN_EVENT_TABLE(wxStaticText, wxStaticTextBase)
    EVT_PAINT(wxStaticText::OnPaint)
END_EVENT_TABLE()

bool wxStaticText::Create(wxWindow *parent, wxWindowID id,
           const wxString& label,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxString& name)
{
    SetName(name);
    m_backgroundColour = parent->GetBackgroundColour() ;
    m_foregroundColour = parent->GetForegroundColour() ;

    if ( id == -1 )
  	    m_windowId = (int)NewControlId();
    else
	    m_windowId = id;

    m_windowStyle = style;
    m_label = label ;

	bool ret = wxControl::Create( parent, id, pos, size, style , wxDefaultValidator , name );
	SetBestSize( size ) ;
    
    return ret;
}

const wxString punct = " ,.-;:!?";

void wxStaticText::DrawParagraph(wxDC &dc, wxString paragraph)
{
  int x = 0 ;
  int y = 0 ;
  int i = 0 ;
  long width, height ;
  bool linedrawn = true;
  while( paragraph.Length() > 0 )
  {
		dc.GetTextExtent( paragraph , &width , &height ) ;
		 	   		
		if ( width > m_width )
		{
			for ( int p = paragraph.Length() -1 ; p > 0 ; --p )
			{     				  
			  if ((punct.Find(paragraph[p]) != wxNOT_FOUND) || !linedrawn)
				{
				  int blank = (paragraph[p] == ' ') ? 0 : 1;
				  
					dc.GetTextExtent( paragraph.Left(p + blank) , &width , &height ) ;
					
					if ( width <= m_width )
					{	
						int pos = x ;
						if ( HasFlag( wxALIGN_CENTER ) )
						{
							pos += ( m_width - width ) / 2 ;
					  }
						else if ( HasFlag( wxALIGN_RIGHT ) )
						{
							pos += ( m_width - width ) ;
					  }
					  		
  					dc.DrawText( paragraph.Left(p + blank), pos , y) ;
  					y += height ;
						paragraph = paragraph.Mid(p+1) ;
						linedrawn = true;
						break ;
					}
				}
			}
			
			linedrawn = false;
		}
		else
		{
			int pos = x ;
			if ( HasFlag( wxALIGN_CENTER ) )
			{
				pos += ( m_width - width ) / 2 ;
		  }
			else if ( HasFlag( wxALIGN_RIGHT ) )
			{
				pos += ( m_width - width ) ;
		  }
		  		
  		dc.DrawText( paragraph, pos , y) ;
  		paragraph="";
  		y += height ;
		}
	}
}

void wxStaticText::OnDraw( wxDC &dc )
{
    if (m_width <= 0 || m_height <= 0)
        return;

  wxString paragraph;
  int i = 0 ;
  wxString text = m_label;

    PrepareDC(dc);
    
    bool doClear = true ;
	WindowRef window = GetMacRootWindow() ;
	if ( window )
	{
		wxWindow* win = wxFindWinFromMacWindow( window ) ;
		if ( win )
		{
			wxWindow* parent = GetParent() ;
			while ( parent )
			{
				if( parent->MacGetWindowData() )
				{
					break ;
				}
				
				if( parent->IsKindOf( CLASSINFO( wxNotebook ) ) ||  parent->IsKindOf( CLASSINFO( wxTabCtrl ) ))
				{
					if ( ((wxControl*)parent)->GetMacControl() ) {
						Rect rect = { -32767 , -32767 , 32767 , 32767 } ;
						if ( DrawThemeTabPane != (void*)kUnresolvedCFragSymbolAddress )
						{
						  DrawThemeTabPane ( &rect, kThemeStateActive);
						  doClear = false ;
						}
					}
					break ;
				}
				
				parent = parent->GetParent() ;
			} 
		}
	}
	if ( doClear )
		dc.Clear() ;
		
	while (i < text.Length())
	{
	  paragraph += text[i];
	  
  	if (text[i] == 13 || text[i] == 10)
	    DrawParagraph(dc, paragraph);
	    
    ++i;
  }
  if (paragraph.Length() > 0)
	  DrawParagraph(dc, paragraph);
}

void wxStaticText::OnPaint( wxPaintEvent &event ) 
{
    wxPaintDC dc(this);
    OnDraw( dc ) ;
}

wxSize wxStaticText::DoGetBestSize() const
{
	int x,y ;
    int widthTextMax = 0, widthLine,
        heightTextTotal = 0, heightLineDefault = 0, heightLine = 0;
        
    wxString curLine;
    for ( const wxChar *pc = m_label; ; pc++ )
    {
        if ( *pc == wxT('\n') || *pc == wxT('\0') )
        {
            if ( !curLine )
            {
                // we can't use GetTextExtent - it will return 0 for both width
                // and height and an empty line should count in height
                // calculation
                if ( !heightLineDefault )
                    heightLineDefault = heightLine;
                if ( !heightLineDefault )
                    GetTextExtent(_T("W"), NULL, &heightLineDefault);

                heightTextTotal += heightLineDefault;
                
                heightTextTotal++;  // FIXME: why is this necessary?
            }
            else
            {
                GetTextExtent(curLine, &widthLine, &heightLine);
                if ( widthLine > widthTextMax )
                    widthTextMax = widthLine;
                heightTextTotal += heightLine;
                
                heightTextTotal++;  // FIXME: why is this necessary?
            }

            if ( *pc == wxT('\n') ) {
               curLine.Empty();
            }
            else {
               // the end of string
               break;
            }
        }
        else {
            curLine += *pc;
        }
    }

    return wxSize(widthTextMax, heightTextTotal);
}

void wxStaticText::SetLabel(const wxString& st )
{
	SetTitle( st ) ;
	m_label = st ;
	if ( !(GetWindowStyle() & wxST_NO_AUTORESIZE) )
		SetSize( GetBestSize() ) ;

	Refresh() ;	
	MacUpdateImmediately() ;
//    wxClientDC dc(this);
//    OnDraw( dc ) ;
}

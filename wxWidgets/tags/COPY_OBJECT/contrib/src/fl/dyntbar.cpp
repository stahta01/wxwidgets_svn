/////////////////////////////////////////////////////////////////////////////
// Name:        No names yet.
// Purpose:     Contrib. demo
// Author:      Aleksandras Gluchovas
// Modified by:
// Created:     ??/10/98
// RCS-ID:      $Id$
// Copyright:   (c) Aleksandras Gluchovas
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "dyntbar.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"


#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/utils.h"     // import wxMin,wxMax macros

#include "wx/fl/dyntbar.h"
#include "wx/fl/newbmpbtn.h"

IMPLEMENT_DYNAMIC_CLASS(wxDynamicToolBar, wxControl )

BEGIN_EVENT_TABLE( wxDynamicToolBar, wxControl )

	EVT_SIZE ( wxDynamicToolBar::OnSize  )
	EVT_PAINT( wxDynamicToolBar::OnPaint )
	//EVT_ERASE_BACKGROUND( wxDynamicToolBar::OnEraseBackground )

END_EVENT_TABLE()

/***** Implementation for class wxDynToolInfo *****/
		
IMPLEMENT_DYNAMIC_CLASS(wxDynToolInfo, wxToolLayoutItem)

/***** Implementation for class wxDynamicToolBar *****/

wxDynamicToolBar::wxDynamicToolBar()
	: mpLayoutMan( NULL ),
	  mSepartorSize( 8 ),
	  mVertGap ( 0 ),
	  mHorizGap( 0 )
{
}

wxDynamicToolBar::wxDynamicToolBar(wxWindow *parent, const wxWindowID id, 
								   const wxPoint& pos, const wxSize& size,
								   const long style, const int orientation,
								   const int RowsOrColumns, const wxString& name )
	: mpLayoutMan( NULL ),
	  mSepartorSize( 8 ),
	  mVertGap ( 0 ),
	  mHorizGap( 0 )
{
	Create(parent, id, pos, size, style, orientation, RowsOrColumns, name);

	SetBackgroundColour( wxSystemSettings::GetSystemColour( wxSYS_COLOUR_3DFACE) );
}

bool wxDynamicToolBar::Create(wxWindow *parent, const wxWindowID id, 
						      const wxPoint& pos, 
							  const wxSize& size,
							  const long style, 
							  const int orientation, const int RowsOrColumns, 
							  const wxString& name)
{
	// cut&pasted from wxtbatsmpl.h

	if ( ! wxWindow::Create(parent, id, pos, size, style, name) )
		return FALSE;

	SetBackgroundColour( wxSystemSettings::GetSystemColour( wxSYS_COLOUR_3DFACE ));

	return TRUE;
}

bool wxDynamicToolBar::Realize(void)
{
	// FOR NOW:: nothing
	return TRUE;
}

wxDynamicToolBar::~wxDynamicToolBar(void)
{
	if ( mpLayoutMan )
        delete mpLayoutMan;

    size_t i;
	for( i = 0; i != mTools.Count(); ++i )
    {
		delete mTools[i];
    }
}

void wxDynamicToolBar::AddTool( int toolIndex, 
								wxWindow* pToolWindow, 
								const wxSize& size
						      )
{
	wxDynToolInfo* pInfo = new wxDynToolInfo();

	pInfo->mpToolWnd    = pToolWindow;
	pInfo->mIndex       = toolIndex;
	pInfo->mIsSeparator	= FALSE;

	int x,y;
	pToolWindow->GetSize( &x, &y );
	pInfo->mRealSize.x = x;
	pInfo->mRealSize.y = y;
	pInfo->mRect.width = x;
	pInfo->mRect.height = y;

	mTools.Add( pInfo );
}

void wxDynamicToolBar::AddTool( int toolIndex, 
                                const wxString& imageFileName,
                                wxBitmapType imageFileType, 
                                const wxString& labelText, bool alignTextRight,
                                bool isFlat )
{
	wxNewBitmapButton* pBtn =

	  new wxNewBitmapButton( imageFileName, imageFileType,
							 labelText, 
							 ( alignTextRight ) 
							 ? NB_ALIGN_TEXT_RIGHT 
							 : NB_ALIGN_TEXT_BOTTOM,
							 isFlat
						   );

	pBtn->Create( this, toolIndex );

	pBtn->Reshape();
	
	AddTool( toolIndex, pBtn );
}
void wxDynamicToolBar::AddTool( int toolIndex, wxBitmap labelBmp,
								const wxString& labelText, bool alignTextRight,
								bool isFlat )
{
	wxNewBitmapButton* pBtn =

	  new wxNewBitmapButton( labelBmp,
							 labelText, 
							 ( alignTextRight ) 
							 ? NB_ALIGN_TEXT_RIGHT 
							 : NB_ALIGN_TEXT_BOTTOM,
							 isFlat
						   );

	pBtn->Create( this, toolIndex );

	pBtn->Reshape();
	
	AddTool( toolIndex, pBtn );
}


  wxToolBarToolBase* 
	  wxDynamicToolBar::AddTool(const int toolIndex, const wxBitmap& bitmap, 
								const wxBitmap& pushedBitmap,
								const bool toggle, const long xPos, 
								const long yPos, wxObject *clientData,
								const wxString& helpString1, const wxString& helpString2)
{
	wxNewBitmapButton* pBmpBtn = new wxNewBitmapButton( bitmap );

	pBmpBtn->Create( this, toolIndex );

	pBmpBtn->Reshape();

	AddTool( toolIndex, pBmpBtn );

	return NULL;
}


wxDynToolInfo* wxDynamicToolBar::GetToolInfo( int toolIndex )
{
    size_t i;
	for( i = 0; i != mTools.Count(); ++i )
    {
		if ( mTools[i]->mIndex == toolIndex )
            return mTools[i];
    }

	return NULL;
}

void wxDynamicToolBar::RemveTool( int toolIndex )
{
    size_t i;
	for( i = 0; i != mTools.Count(); ++i )
    {
		if ( mTools[i]->mIndex == toolIndex ) 
		{
		    if ( mTools[i]->mpToolWnd )
		    {
			    mTools[i]->mpToolWnd->Destroy();
		    }
            delete mTools[i]; // HVL To be tested!!!
#if wxCHECK_VERSION(2,3,2)
			mTools.RemoveAt(i);
#else
			mTools.Remove(i);
#endif
			Layout();

			return;
		}
    }
	// TODO:: if not found, should it be an assertion?
}

void wxDynamicToolBar::AddSeparator( wxWindow* pSepartorWnd )
{
	wxDynToolInfo* pInfo = new wxDynToolInfo();

	pInfo->mpToolWnd    = pSepartorWnd;
	pInfo->mIndex       = -1;
	pInfo->mIsSeparator	= TRUE;

	if ( pSepartorWnd )
	{
		pSepartorWnd->Create( this, -1 );

		int x,y;
		pSepartorWnd->GetSize( &x, &y );
		pInfo->mRealSize.x = x;
		pInfo->mRealSize.y = y;

		pInfo->mRect.width = x;
		pInfo->mRect.height = y;
	}
	else
	{
		pInfo->mRealSize.x = mSepartorSize;
		pInfo->mRealSize.y = 0;

		pInfo->mRect.width  = mSepartorSize;
		pInfo->mRect.height = 0;
	}

	mTools.Add( pInfo );
}

void wxDynamicToolBar::OnEraseBackground( wxEraseEvent& event )
{
	// FOR NOW:: nothing
}

void wxDynamicToolBar::OnSize( wxSizeEvent& event )
{
	//SetBackgroundColour( wxSystemSettings::GetSystemColour( wxSYS_COLOUR_3DFACE ) );

	Layout();
}

void wxDynamicToolBar::DrawSeparator( wxDynToolInfo& info, wxDC& dc )
{
	// check the orientation of separator
	if ( info.mRect.width < info.mRect.height )
	{
		int midX = info.mRect.x + info.mRect.width/2 - 1;

		dc.SetPen( *wxGREY_PEN );
		dc.DrawLine( midX, info.mRect.y,
			         midX, info.mRect.y + info.mRect.height+1 );

		dc.SetPen( *wxWHITE_PEN );
		dc.DrawLine( midX+1, info.mRect.y,
			         midX+1, info.mRect.y + info.mRect.height+1 );
	}
	else
	{
		int midY = info.mRect.y + info.mRect.height/2 - 1;

		dc.SetPen( *wxGREY_PEN );
		dc.DrawLine( info.mRect.x, midY,
			         info.mRect.x + info.mRect.width+1, midY );

		dc.SetPen( *wxWHITE_PEN );
		dc.DrawLine( info.mRect.x, midY + 1,
			         info.mRect.x + info.mRect.width+1, midY + 1 );
	}
}

void wxDynamicToolBar::OnPaint( wxPaintEvent& event )
{
	// draw separators if any

	wxPaintDC dc(this);

    size_t i;
	for( i = 0; i != mTools.Count(); ++i )
	
		if ( mTools[i]->mIsSeparator ) 
		{
			// check if separator doesn't have it's own window
			// if so, then draw it using built-in drawing method

			if ( !mTools[i]->mpToolWnd )

				DrawSeparator( *mTools[i], dc );
		}
}

// FOR NOW:: quick fix
#include "wx/choice.h"

void wxDynamicToolBar::SizeToolWindows()
{
    size_t i;
	for( i = 0; i != mTools.Count(); ++i )
	{
		wxDynToolInfo& info = *mTools[i];

		if ( !info.mIsSeparator ) 
		{

			// center real rectangle within the rectangle 
			// provided by the layout manager

			int x = info.mRect.x;
			int y = info.mRect.y + (info.mRect.height - info.mRealSize.y)/2;

			// FOR NOW FOR NOW:: quick & dirty fix
			if ( info.mpToolWnd->IsKindOf( CLASSINFO( wxChoice ) ) )
			{
				info.mpToolWnd->SetSize( x,y, 
										info.mRealSize.x - 3, 
										info.mRealSize.y);
			}
			else
				info.mpToolWnd->SetSize( x,y, 
										info.mRealSize.x, 
										info.mRealSize.y );
		}

		// TBD:: size separator window if present
	}
}

bool wxDynamicToolBar::Layout()
{
	if ( !mpLayoutMan )
        mpLayoutMan = CreateDefaulLayout();

	int x,y;
	GetSize( &x, &y );
	wxSize wndDim(x,y);
	wxSize result;

	wxLayoutItemArrayT items;

	// safe conversion
    size_t i;
	for( i = 0; i != mTools.Count(); ++i )
        items.Add( mTools[i] );

	mpLayoutMan->Layout( wndDim, result, items, mVertGap, mHorizGap );;

	SizeToolWindows();
    return TRUE;
}

void wxDynamicToolBar::GetPreferredDim( const wxSize& givenDim, wxSize& prefDim )
{
	if ( !mpLayoutMan )
        mpLayoutMan = CreateDefaulLayout();

	wxLayoutItemArrayT items;

	// safe conversion
    size_t i;
	for( i = 0; i != mTools.Count(); ++i )
        items.Add( mTools[i] );

	mpLayoutMan->Layout( givenDim, prefDim, items, mVertGap, mHorizGap );;
}

void wxDynamicToolBar::SetLayout( LayoutManagerBase* pLayout )
{
	if ( mpLayoutMan )
        delete mpLayoutMan;

	mpLayoutMan = pLayout;

	Layout();
}

void wxDynamicToolBar::EnableTool(const int toolIndex, const bool enable )
{
	wxDynToolInfo* pInfo = GetToolInfo( toolIndex );

	if ( !pInfo )
        return;

	if ( pInfo->mIsSeparator || !pInfo->mpToolWnd )
        return;

	pInfo->mpToolWnd->Enable( enable );
}

/***** Implementation for class BagLayout *****/

void BagLayout::Layout(  const wxSize&       parentDim, 
						 wxSize&	         resultingDim,
						 wxLayoutItemArrayT& items,
						 int		         horizGap,
 		                 int		         vertGap  
					  )
{
	int maxWidth = 0;
	int curY  = 0;
	int nRows = 0;

	size_t i = 0;

	while( i < items.Count() )
	{
		int curX   = 0;
		int height = 0;
		// int nItems = 0;

		// int firstItem  = i;
		int itemsInRow = 0;

		if ( nRows > 0 )
            curY += vertGap;

		// step #1 - arrange horizontal positions of items in the row

		do
		{
			if ( itemsInRow > 0 )
                curX += horizGap;

			wxRect& r = items[i]->mRect;

			if ( curX + r.width > parentDim.x )
            {
				if ( itemsInRow > 0 ) 
                    break;
            }
			r.x = curX;
			r.y = curY;

			curX += r.width;

			height = wxMax( height, r.height );

			++itemsInRow;
			++i;

		} while( i < items.Count() );

		curY += height;

		maxWidth = wxMax( maxWidth, curX );
	}

	resultingDim.x = maxWidth;
	resultingDim.y = curY;
}

//////// stuff from 2.1.15 ///////////

wxToolBarToolBase* wxDynamicToolBar::FindToolForPosition( wxCoord x, wxCoord y ) const
{
	return NULL;
}

bool wxDynamicToolBar::DoInsertTool( size_t pos, wxToolBarToolBase* tool )
{
	return TRUE;
}

bool wxDynamicToolBar::DoDeleteTool( size_t pos, wxToolBarToolBase* tool )
{
	return TRUE;
}

void wxDynamicToolBar::DoEnableTool( wxToolBarToolBase* tool, bool enable )
{
}

void wxDynamicToolBar::DoToggleTool( wxToolBarToolBase* tool, bool toggle )
{
}

void wxDynamicToolBar::DoSetToggle( wxToolBarToolBase* tool, bool toggle )
{
}

wxToolBarToolBase* wxDynamicToolBar::CreateTool( int id, const wxBitmap& bitmap1, const wxBitmap& bitmap2, bool toggle, wxObject* clientData, const wxString& shortHelpString, const wxString& longHelpString )
{
	return NULL;
}

wxToolBarToolBase* wxDynamicToolBar::CreateTool( wxControl* control )
{
	return NULL;
}


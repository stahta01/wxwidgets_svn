// griddemo.cpp
//
//  grid test program


#ifdef __GNUG__
#pragma implementation
#pragma interface
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/colordlg.h"

#include "wx/grid.h"

#include "griddemo.h"

IMPLEMENT_APP( GridApp )



bool GridApp::OnInit()
{
    GridFrame *frame = new GridFrame;
    frame->Show( TRUE );
    
    return TRUE;
}



BEGIN_EVENT_TABLE( GridFrame, wxFrame )
    EVT_MENU( ID_TOGGLEROWLABELS,  GridFrame::ToggleRowLabels )
    EVT_MENU( ID_TOGGLECOLLABELS,  GridFrame::ToggleColLabels )    
    EVT_MENU( ID_TOGGLECONTROLPANEL, GridFrame::ToggleControlPanel )
    EVT_MENU( ID_TOGGLECELLEDIT, GridFrame::ToggleCellEdit )
    EVT_MENU( ID_SETLABELCOLOUR, GridFrame::SetLabelColour )    
    EVT_MENU( ID_SETLABELTEXTCOLOUR, GridFrame::SetLabelTextColour )    
    EVT_MENU( ID_ROWLABELHORIZALIGN, GridFrame::SetRowLabelHorizAlignment )    
    EVT_MENU( ID_ROWLABELVERTALIGN, GridFrame::SetRowLabelVertAlignment )    
    EVT_MENU( ID_COLLABELHORIZALIGN, GridFrame::SetColLabelHorizAlignment )    
    EVT_MENU( ID_COLLABELVERTALIGN, GridFrame::SetColLabelVertAlignment )    
    EVT_MENU( ID_GRIDLINECOLOUR, GridFrame::SetGridLineColour )
    EVT_MENU( ID_CLEARGRID, GridFrame::ClearGrid )
    EVT_MENU( ID_ABOUT, GridFrame::About )
    EVT_MENU( wxID_EXIT, GridFrame::OnQuit )

    EVT_WXGRID_LABEL_LEFT_CLICK( GridFrame::OnLabelLeftClick )
    EVT_WXGRID_CELL_LEFT_CLICK( GridFrame::OnCellLeftClick )
    EVT_WXGRID_ROW_SIZE( GridFrame::OnRowSize )
    EVT_WXGRID_COL_SIZE( GridFrame::OnColSize )
    EVT_WXGRID_RANGE_SELECT( GridFrame::OnRangeSelected )
    EVT_WXGRID_CELL_CHANGE( GridFrame::OnCellValueChanged )
END_EVENT_TABLE()

    
GridFrame::GridFrame()
        : wxFrame( (wxFrame *)NULL, -1, "wxWindows grid class demo",
                   wxDefaultPosition,
                   wxDefaultSize )
{
    int gridW = 600, gridH = 300;
    int logW = gridW, logH = 80;
    
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append( wxID_EXIT, "E&xit" );
    
    wxMenu *viewMenu = new wxMenu;
    viewMenu->Append( ID_TOGGLEROWLABELS,  "&Row labels", "", TRUE );
    viewMenu->Append( ID_TOGGLECOLLABELS,  "&Col labels", "", TRUE );
    viewMenu->Append( ID_TOGGLECONTROLPANEL,  "To&p controls", "", TRUE );
    viewMenu->Append( ID_TOGGLECELLEDIT,  "&In-place editing", "", TRUE );
    viewMenu->Append( ID_SETLABELCOLOUR, "Set &label colour" );
    viewMenu->Append( ID_SETLABELTEXTCOLOUR, "Set label &text colour" );
    
    wxMenu *rowLabelMenu = new wxMenu;

    viewMenu->Append( ID_ROWLABELALIGN, "R&ow label alignment",
                      rowLabelMenu,
                      "Change alignment of row labels" );

    rowLabelMenu->Append( ID_ROWLABELHORIZALIGN, "&Horizontal" );
    rowLabelMenu->Append( ID_ROWLABELVERTALIGN, "&Vertical" );    
    
    wxMenu *colLabelMenu = new wxMenu;

    viewMenu->Append( ID_COLLABELALIGN, "Col l&abel alignment",
                      colLabelMenu,
                      "Change alignment of col labels" );

    colLabelMenu->Append( ID_COLLABELHORIZALIGN, "&Horizontal" );
    colLabelMenu->Append( ID_COLLABELVERTALIGN, "&Vertical" );

    viewMenu->Append( ID_GRIDLINECOLOUR, "&Grid line colour" );
    viewMenu->Append( ID_CLEARGRID, "Cl&ear grid cell contents" );

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append( ID_ABOUT, "&About wxGrid demo" );
    
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( fileMenu, "&File" );
    menuBar->Append( viewMenu, "&View" );
    menuBar->Append( helpMenu, "&Help" );

    SetMenuBar( menuBar );

    grid = new wxGrid( this,
                       -1,
                       wxPoint( 0, 0 ),
                       wxSize( 400, 300 ) );
            
    logWin = new wxTextCtrl( this,
                             -1,
                             wxEmptyString,
                             wxPoint( 0, gridH + 20 ),
                             wxSize( logW, logH ),
                             wxTE_MULTILINE );
                             
    logger = new wxLogTextCtrl( logWin );
    logger->SetActiveTarget( logger );
    logger->SetTimestamp( NULL );

    // this will create a grid and, by default, an associated grid
    // table for string data
    //
    grid->CreateGrid( 100, 100 );

    grid->EnableTopEditControl( TRUE );
    
    grid->SetRowSize( 0, 60 );
    grid->SetCellValue( 0, 0, "Ctrl+Home\nwill go to\nthis cell" );
    
    grid->SetCellValue( 0, 1, "Blah" );
    grid->SetCellValue( 0, 2, "Blah" );

    grid->SetCellValue( 0, 5, "Press\nCtrl+arrow\nto skip over\ncells" );

    grid->SetRowSize( 99, 60 );
    grid->SetCellValue( 99, 99, "Ctrl+End\nwill go to\nthis cell" );
    
    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    topSizer->Add( grid,
                   1,
                   wxEXPAND );
                   
    topSizer->Add( logWin,
                   0, 
                   wxEXPAND );
                   
    SetAutoLayout( TRUE );
    SetSizer( topSizer );
    
    topSizer->Fit( this );
    topSizer->SetSizeHints( this );
    
    Centre();
    SetDefaults();
}


GridFrame::~GridFrame()
{
}

void GridFrame::SetDefaults()
{
    GetMenuBar()->Check( ID_TOGGLEROWLABELS, TRUE );
    GetMenuBar()->Check( ID_TOGGLECOLLABELS, TRUE );
    GetMenuBar()->Check( ID_TOGGLECONTROLPANEL, TRUE );
    GetMenuBar()->Check( ID_TOGGLECELLEDIT, TRUE );
}


void GridFrame::ToggleRowLabels( wxCommandEvent& WXUNUSED(ev) )
{
    if ( GetMenuBar()->IsChecked( ID_TOGGLEROWLABELS ) )
    {
        grid->SetRowLabelSize( grid->GetDefaultRowLabelSize() );
    }
    else
    {
        grid->SetRowLabelSize( 0 );
    }
}


void GridFrame::ToggleColLabels( wxCommandEvent& WXUNUSED(ev) )
{
    if ( GetMenuBar()->IsChecked( ID_TOGGLECOLLABELS ) )
    {
        grid->SetColLabelSize( grid->GetDefaultColLabelSize() );
    }
    else
    {
        grid->SetColLabelSize( 0 );
    }
}


void GridFrame::ToggleControlPanel( wxCommandEvent& WXUNUSED(ev) )
{
    grid->EnableTopEditControl(GetMenuBar()->IsChecked(ID_TOGGLECONTROLPANEL));
}


void GridFrame::ToggleCellEdit( wxCommandEvent& WXUNUSED(ev) )
{
    grid->EnableCellEditControl(
        GetMenuBar()->IsChecked( ID_TOGGLECELLEDIT ) );
}


void GridFrame::SetLabelColour( wxCommandEvent& WXUNUSED(ev) )
{
    wxColourDialog dlg( NULL );
    if ( dlg.ShowModal() == wxID_OK )
    {
        wxColourData retData;
        retData = dlg.GetColourData();
        wxColour colour = retData.GetColour();

        grid->SetLabelBackgroundColour( colour );
    }
}


void GridFrame::SetLabelTextColour( wxCommandEvent& WXUNUSED(ev) )
{
    wxColourDialog dlg( NULL );
    if ( dlg.ShowModal() == wxID_OK )
    {
        wxColourData retData;
        retData = dlg.GetColourData();
        wxColour colour = retData.GetColour();

        grid->SetLabelTextColour( colour );
    }
}


void GridFrame::SetRowLabelHorizAlignment( wxCommandEvent& WXUNUSED(ev) )
{
    int horiz, vert;
    grid->GetRowLabelAlignment( &horiz, &vert );
    
    switch ( horiz )
    {
        case wxLEFT:
            horiz = wxCENTRE;
            break;
            
        case wxCENTRE:
            horiz = wxRIGHT;
            break;

        case wxRIGHT:
            horiz = wxLEFT;
            break;
    }

    grid->SetRowLabelAlignment( horiz, -1 );
}

void GridFrame::SetRowLabelVertAlignment( wxCommandEvent& WXUNUSED(ev) )
{
    int horiz, vert;
    grid->GetRowLabelAlignment( &horiz, &vert );
    
    switch ( vert )
    {
        case wxTOP:
            vert = wxCENTRE;
            break;
            
        case wxCENTRE:
            vert = wxBOTTOM;
            break;

        case wxBOTTOM:
            vert = wxTOP;
            break;
    }

    grid->SetRowLabelAlignment( -1, vert );
}


void GridFrame::SetColLabelHorizAlignment( wxCommandEvent& WXUNUSED(ev) )
{
    int horiz, vert;
    grid->GetColLabelAlignment( &horiz, &vert );
    
    switch ( horiz )
    {
        case wxLEFT:
            horiz = wxCENTRE;
            break;
            
        case wxCENTRE:
            horiz = wxRIGHT;
            break;

        case wxRIGHT:
            horiz = wxLEFT;
            break;
    }

    grid->SetColLabelAlignment( horiz, -1 );
}


void GridFrame::SetColLabelVertAlignment( wxCommandEvent& WXUNUSED(ev) )
{
    int horiz, vert;
    grid->GetColLabelAlignment( &horiz, &vert );
    
    switch ( vert )
    {
        case wxTOP:
            vert = wxCENTRE;
            break;
            
        case wxCENTRE:
            vert = wxBOTTOM;
            break;

        case wxBOTTOM:
            vert = wxTOP;
            break;
    }

    grid->SetColLabelAlignment( -1, vert );
}


void GridFrame::SetGridLineColour( wxCommandEvent& WXUNUSED(ev) )
{
    wxColourDialog dlg( NULL );
    if ( dlg.ShowModal() == wxID_OK )
    {
        wxColourData retData;
        retData = dlg.GetColourData();
        wxColour colour = retData.GetColour();

        grid->SetGridLineColour( colour );
    }
}


void GridFrame::ClearGrid( wxCommandEvent& WXUNUSED(ev) )
{
    grid->ClearGrid();
}


void GridFrame::About(  wxCommandEvent& WXUNUSED(ev) )
{
    (void)wxMessageBox( "\n\nwxGrid demo \n\n"
                        "Michael Bedward \n"
                        "mbedward@ozemail.com.au \n\n",
                        "About",
                        wxOK );
}


void GridFrame::OnSize( wxSizeEvent& WXUNUSED(ev) )
{
    if ( grid && logWin )
    {
        int cw, ch;
        GetClientSize( &cw, &ch );
    
        int gridH = ch - 90;
        int logH  = 80;
        if ( gridH < 0 )
        {
            gridH = ch;
        }
        
        grid->SetSize( 0, 0, cw, gridH );
        logWin->SetSize( 0, gridH + 10, cw, logH );
    }
}

void GridFrame::OnQuit( wxCommandEvent& WXUNUSED(ev) )
{
    Close( TRUE );
}


void GridFrame::OnLabelLeftClick( wxGridEvent& ev )
{
    logBuf = "";
    if ( ev.GetRow() != -1 )
    {
        logBuf << "row label " << ev.GetRow();
    }
    else if ( ev.GetCol() != -1 )
    {
        logBuf << "col label " << ev.GetCol();
    }
    else
    {
        logBuf << "corner label";
    }

    if ( ev.ShiftDown() ) logBuf << " (shift down)";
    wxLogMessage( "%s", logBuf.c_str() );
    
    ev.Skip();
}


void GridFrame::OnCellLeftClick( wxGridEvent& ev )
{
    logBuf = "";
    logBuf << "Cell at row " << ev.GetRow()
           << " col " << ev.GetCol();
    wxLogMessage( "%s", logBuf.c_str() );
    
    // you must call event skip if you want default grid processing
    // (cell highlighting etc.)
    //
    ev.Skip();
}


void GridFrame::OnRowSize( wxGridSizeEvent& ev )
{
    logBuf = "";
    logBuf << "Resized row " << ev.GetRowOrCol();
    wxLogMessage( "%s", logBuf.c_str() );
    
    ev.Skip();
}


void GridFrame::OnColSize( wxGridSizeEvent& ev )
{
    logBuf = "";
    logBuf << "Resized col " << ev.GetRowOrCol();
    wxLogMessage( "%s", logBuf.c_str() );
    
    ev.Skip();
}

void GridFrame::OnRangeSelected( wxGridRangeSelectEvent& ev )
{
    logBuf = "";
    logBuf  << "Selected cells from row " << ev.GetTopRow()
            << " col " << ev.GetLeftCol()
            << " to row " << ev.GetBottomRow()
            << " col " << ev.GetRightCol();
    
    wxLogMessage( "%s", logBuf.c_str() );
    
    ev.Skip();
}

void GridFrame::OnCellValueChanged( wxGridEvent& ev )
{
    logBuf = "";
    logBuf  << "Value changed for cell at"
            << " row " << ev.GetRow() 
            << " col " << ev.GetCol();
    
    wxLogMessage( "%s", logBuf.c_str() );
    
    ev.Skip();
}



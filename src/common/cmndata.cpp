/////////////////////////////////////////////////////////////////////////////
// Name:        cmndata.cpp
// Purpose:     Common GDI data
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "cmndata.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <stdio.h>
    #include "wx/string.h"
    #include "wx/utils.h"
    #include "wx/app.h"
#endif

#include "wx/gdicmn.h"
#include "wx/cmndata.h"
#include "wx/log.h"
#include "wx/prntbase.h"
#include "wx/printdlg.h"

#if wxUSE_FONTDLG
    #include "wx/fontdlg.h"
#endif // wxUSE_FONTDLG

#if wxUSE_PRINTING_ARCHITECTURE
    #include "wx/paper.h"
#endif // wxUSE_PRINTING_ARCHITECTURE

#if defined(__WXMSW__) && !defined(__PALMOS__)
    #include <windowsx.h>
    #include "wx/msw/private.h"

    #ifndef __SMARTPHONE__ /* of WinCE */
        #include <commdlg.h>
    #endif

    #if defined(__WATCOMC__) || defined(__SYMANTEC__) || defined(__SALFORDC__)
        #include <windowsx.h>
        #include <commdlg.h>
    #endif
#endif // MSW

#ifdef __WXMAC__
    #include "wx/mac/private/print.h"
#endif

    #if wxUSE_PRINTING_ARCHITECTURE
        IMPLEMENT_DYNAMIC_CLASS(wxPrintData, wxObject)
        IMPLEMENT_DYNAMIC_CLASS(wxPrintDialogData, wxObject)
        IMPLEMENT_DYNAMIC_CLASS(wxPageSetupDialogData, wxObject)
    #endif // wxUSE_PRINTING_ARCHITECTURE

    IMPLEMENT_DYNAMIC_CLASS(wxFontData, wxObject)
    IMPLEMENT_DYNAMIC_CLASS(wxColourData, wxObject)

#ifndef DMPAPER_USER
     #define DMPAPER_USER                256
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxColourData
// ----------------------------------------------------------------------------

wxColourData::wxColourData()
{
    m_chooseFull = false;
    m_dataColour.Set(0,0,0);
    // m_custColours are wxNullColours initially
}

wxColourData::wxColourData(const wxColourData& data)
    : wxObject()
{
    (*this) = data;
}

wxColourData::~wxColourData()
{
}

void wxColourData::SetCustomColour(int i, const wxColour& colour)
{
    wxCHECK_RET( (i >= 0 && i < 16), _T("custom colour index out of range") );

    m_custColours[i] = colour;
}

wxColour wxColourData::GetCustomColour(int i)
{
    wxCHECK_MSG( (i >= 0 && i < 16), wxColour(0,0,0),
                 _T("custom colour index out of range") );

    return m_custColours[i];
}

void wxColourData::operator=(const wxColourData& data)
{
    int i;
    for (i = 0; i < 16; i++)
        m_custColours[i] = data.m_custColours[i];

    m_dataColour = (wxColour&)data.m_dataColour;
    m_chooseFull = data.m_chooseFull;
}

// ----------------------------------------------------------------------------
// Font data
// ----------------------------------------------------------------------------

wxFontData::wxFontData()
{
    // Intialize colour to black.
    m_fontColour = wxNullColour;

    m_showHelp = false;
    m_allowSymbols = true;
    m_enableEffects = true;
    m_minSize = 0;
    m_maxSize = 0;

    m_encoding = wxFONTENCODING_SYSTEM;
}

wxFontData::~wxFontData()
{
}

#if wxUSE_FONTDLG

wxFontDialogBase::~wxFontDialogBase()
{
}

#endif // wxUSE_FONTDLG

#if wxUSE_PRINTING_ARCHITECTURE
// ----------------------------------------------------------------------------
// Print data
// ----------------------------------------------------------------------------

wxPrintData::wxPrintData()
{
#ifdef __WXMAC__
    m_nativePrintData = wxNativePrintData::Create() ;
#endif
    m_bin = wxPRINTBIN_DEFAULT;
    m_printMode = wxPRINT_MODE_PRINTER;
    m_printOrientation = wxPORTRAIT;
    m_printNoCopies = 1;
    m_printCollate = false;

    // New, 24/3/99
    m_printerName = wxT("");
    m_colour = true;
    m_duplexMode = wxDUPLEX_SIMPLEX;
    m_printQuality = wxPRINT_QUALITY_HIGH;
    m_paperId = wxPAPER_A4;
    m_paperSize = wxSize(210, 297);

    m_nativeData = wxPrintFactory::GetFactory()->CreatePrintNativeData();
}

wxPrintData::wxPrintData(const wxPrintData& printData)
    : wxObject()
{
    (*this) = printData;
}

wxPrintData::~wxPrintData()
{
    m_nativeData->m_ref--;
    if (m_nativeData->m_ref == 0) 
        delete m_nativeData;
        
#ifdef __WXMAC__
    delete m_nativePrintData ;
#endif
}


void wxPrintData::ConvertToNative()
{
#ifdef __WXMAC__
    m_nativePrintData->TransferFrom( this ) ;
#else
    m_nativeData->TransferFrom( *this ) ;
#endif
}

void wxPrintData::ConvertFromNative()
{
#ifdef __WXMAC__
    m_nativePrintData->TransferTo( this ) ;
#else
    m_nativeData->TransferTo( *this ) ;
#endif
}

void wxPrintData::operator=(const wxPrintData& data)
{
    m_printNoCopies = data.m_printNoCopies;
    m_printCollate = data.m_printCollate;
    m_printOrientation = data.m_printOrientation;
    m_printerName = data.m_printerName;
    m_colour = data.m_colour;
    m_duplexMode = data.m_duplexMode;
    m_printQuality = data.m_printQuality;
    m_paperId = data.m_paperId;
    m_paperSize = data.m_paperSize;
    m_bin = data.m_bin;
    m_printMode = data.m_printMode;
    m_filename = data.m_filename;   
    
    m_nativeData = data.GetNativeData();
    m_nativeData->m_ref++;
    
#ifdef __WXMAC__
    m_nativePrintData->CopyFrom( data.m_nativePrintData ) ;
#endif
}

// Is this data OK for showing the print dialog?
bool wxPrintData::Ok() const
{
    m_nativeData->TransferFrom( *this );

    return m_nativeData->Ok();
}

// ----------------------------------------------------------------------------
// Print dialog data
// ----------------------------------------------------------------------------

wxPrintDialogData::wxPrintDialogData()
{
    m_printFromPage = 0;
    m_printToPage = 0;
    m_printMinPage = 0;
    m_printMaxPage = 0;
    m_printNoCopies = 1;
    m_printAllPages = false;
    m_printCollate = false;
    m_printToFile = false;
    m_printSelection = false;
    m_printEnableSelection = false;
    m_printEnablePageNumbers = true;
    
    wxPrintFactory* factory = wxPrintFactory::GetFactory();
    m_printEnablePrintToFile = ! factory->HasOwnPrintToFile();
    
    m_printEnableHelp = false;
#if WXWIN_COMPATIBILITY_2_4
    m_printSetupDialog = false;
#endif
}

wxPrintDialogData::wxPrintDialogData(const wxPrintDialogData& dialogData)
    : wxObject()
{
    (*this) = dialogData;
}

wxPrintDialogData::wxPrintDialogData(const wxPrintData& printData)
{
    m_printFromPage = 1;
    m_printToPage = 0;
    m_printMinPage = 1;
    m_printMaxPage = 9999;
    m_printNoCopies = 1;
    m_printAllPages = false;
    m_printCollate = false;
    m_printToFile = false;
    m_printSelection = false;
    m_printEnableSelection = false;
    m_printEnablePageNumbers = true;
    m_printEnablePrintToFile = true;
    m_printEnableHelp = false;
#if WXWIN_COMPATIBILITY_2_4
    m_printSetupDialog = false;
#endif
    m_printData = printData;
}

wxPrintDialogData::~wxPrintDialogData()
{
}

#ifdef __WXMAC__

void wxPrintDialogData::ConvertToNative()
{
    m_printData.ConvertToNative();
    m_printData.m_nativePrintData->TransferFrom( this ) ;
}

void wxPrintDialogData::ConvertFromNative()
{
    m_printData.ConvertFromNative();
    m_printData.m_nativePrintData->TransferTo( this ) ;
}

#endif


void wxPrintDialogData::operator=(const wxPrintDialogData& data)
{
    m_printFromPage = data.m_printFromPage;
    m_printToPage = data.m_printToPage;
    m_printMinPage = data.m_printMinPage;
    m_printMaxPage = data.m_printMaxPage;
    m_printNoCopies = data.m_printNoCopies;
    m_printAllPages = data.m_printAllPages;
    m_printCollate = data.m_printCollate;
    m_printToFile = data.m_printToFile;
    m_printSelection = data.m_printSelection;
    m_printEnableSelection = data.m_printEnableSelection;
    m_printEnablePageNumbers = data.m_printEnablePageNumbers;
    m_printEnableHelp = data.m_printEnableHelp;
    m_printEnablePrintToFile = data.m_printEnablePrintToFile;
#if WXWIN_COMPATIBILITY_2_4
    m_printSetupDialog = data.m_printSetupDialog;
#endif
    m_printData = data.m_printData;
}

void wxPrintDialogData::operator=(const wxPrintData& data)
{
    m_printData = data;
}

// ----------------------------------------------------------------------------
// wxPageSetupDialogData
// ----------------------------------------------------------------------------

wxPageSetupDialogData::wxPageSetupDialogData()
{
    m_paperSize = wxSize(0, 0);

    CalculatePaperSizeFromId();

    m_minMarginTopLeft = wxPoint(0, 0);
    m_minMarginBottomRight = wxPoint(0, 0);
    m_marginTopLeft = wxPoint(0, 0);
    m_marginBottomRight = wxPoint(0, 0);

    // Flags
    m_defaultMinMargins = false;
    m_enableMargins = true;
    m_enableOrientation = true;
    m_enablePaper = true;
    m_enablePrinter = true;
    m_enableHelp = false;
    m_getDefaultInfo = false;
}

wxPageSetupDialogData::wxPageSetupDialogData(const wxPageSetupDialogData& dialogData)
    : wxObject()
{
    (*this) = dialogData;
}

wxPageSetupDialogData::wxPageSetupDialogData(const wxPrintData& printData)
{
    m_paperSize = wxSize(0, 0);
    m_minMarginTopLeft = wxPoint(0, 0);
    m_minMarginBottomRight = wxPoint(0, 0);
    m_marginTopLeft = wxPoint(0, 0);
    m_marginBottomRight = wxPoint(0, 0);

    // Flags
    m_defaultMinMargins = false;
    m_enableMargins = true;
    m_enableOrientation = true;
    m_enablePaper = true;
    m_enablePrinter = true;
    m_enableHelp = false;
    m_getDefaultInfo = false;

    m_printData = printData;

    // The wxPrintData paper size overrides these values, unless the size cannot
    // be found.
    CalculatePaperSizeFromId();
}

wxPageSetupDialogData::~wxPageSetupDialogData()
{
}

wxPageSetupDialogData& wxPageSetupDialogData::operator=(const wxPageSetupDialogData& data)
{
    m_paperSize = data.m_paperSize;
    m_minMarginTopLeft = data.m_minMarginTopLeft;
    m_minMarginBottomRight = data.m_minMarginBottomRight;
    m_marginTopLeft = data.m_marginTopLeft;
    m_marginBottomRight = data.m_marginBottomRight;
    m_defaultMinMargins = data.m_defaultMinMargins;
    m_enableMargins = data.m_enableMargins;
    m_enableOrientation = data.m_enableOrientation;
    m_enablePaper = data.m_enablePaper;
    m_enablePrinter = data.m_enablePrinter;
    m_getDefaultInfo = data.m_getDefaultInfo;;
    m_enableHelp = data.m_enableHelp;

    m_printData = data.m_printData;

    return *this;
}

wxPageSetupDialogData& wxPageSetupDialogData::operator=(const wxPrintData& data)
{
    m_printData = data;

    return *this;
}

#ifdef __WXMAC__
void wxPageSetupDialogData::ConvertToNative()
{
    m_printData.ConvertToNative();
    m_printData.m_nativePrintData->TransferFrom( this ) ;
}

void wxPageSetupDialogData::ConvertFromNative()
{
    m_printData.ConvertFromNative ();
    m_paperSize = m_printData.GetPaperSize() ;
    CalculateIdFromPaperSize();
    m_printData.m_nativePrintData->TransferTo( this ) ;
    // adjust minimal values

    if ( m_marginTopLeft.x < m_minMarginTopLeft.x )
        m_marginTopLeft.x = m_minMarginTopLeft.x;

    if ( m_marginBottomRight.x < m_minMarginBottomRight.x )
        m_marginBottomRight.x = m_minMarginBottomRight.x;

    if ( m_marginTopLeft.y < m_minMarginTopLeft.y )
        m_marginTopLeft.y = m_minMarginTopLeft.y;

    if ( m_marginBottomRight.y < m_minMarginBottomRight.y )
        m_marginBottomRight.y = m_minMarginBottomRight.y;
}
#endif


// If a corresponding paper type is found in the paper database, will set the m_printData
// paper size id member as well.
void wxPageSetupDialogData::SetPaperSize(const wxSize& sz)
{
    m_paperSize = sz;

    CalculateIdFromPaperSize();
}

// Sets the wxPrintData id, plus the paper width/height if found in the paper database.
void wxPageSetupDialogData::SetPaperSize(wxPaperSize id)
{
    m_printData.SetPaperId(id);

    CalculatePaperSizeFromId();
}

// Use paper size defined in this object to set the wxPrintData
// paper id
void wxPageSetupDialogData::CalculateIdFromPaperSize()
{
    wxASSERT_MSG( (wxThePrintPaperDatabase != (wxPrintPaperDatabase*) NULL),
                  wxT("wxThePrintPaperDatabase should not be NULL. Do not create global print dialog data objects.") );

    wxSize sz = GetPaperSize();

    wxPaperSize id = wxThePrintPaperDatabase->GetSize(wxSize(sz.x* 10, sz.y * 10));
    if (id != wxPAPER_NONE)
    {
        m_printData.SetPaperId(id);
    }
}

// Use paper id in wxPrintData to set this object's paper size
void wxPageSetupDialogData::CalculatePaperSizeFromId()
{
    wxASSERT_MSG( (wxThePrintPaperDatabase != (wxPrintPaperDatabase*) NULL),
                  wxT("wxThePrintPaperDatabase should not be NULL. Do not create global print dialog data objects.") );

    wxSize sz = wxThePrintPaperDatabase->GetSize(m_printData.GetPaperId());

    // sz is in 10ths of a mm, while paper size is in mm
    m_paperSize.x = sz.x / 10;
    m_paperSize.y = sz.y / 10;
}

#endif // wxUSE_PRINTING_ARCHITECTURE

///////////////////////////////////////////////////////////////////////////////
// Name:        src/aui/auibook.cpp
// Purpose:     wxaui: wx advanced user interface - notebook
// Author:      Benjamin I. Williams
// Modified by:
// Created:     2006-06-28
// Copyright:   (C) Copyright 2006, Kirix Corporation, All Rights Reserved
// Licence:     wxWindows Library Licence, Version 3.1
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_AUI

#include "wx/aui/auibook.h"

#ifndef WX_PRECOMP
    #include "wx/settings.h"
    #include "wx/image.h"
#endif

#include "wx/aui/tabmdi.h"
#include "wx/dcbuffer.h"

#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY(wxAuiNotebookPageArray)
WX_DEFINE_OBJARRAY(wxAuiTabContainerButtonArray)

DEFINE_EVENT_TYPE(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGING)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_AUINOTEBOOK_BUTTON)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_AUINOTEBOOK_BEGIN_DRAG)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_AUINOTEBOOK_END_DRAG)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_AUINOTEBOOK_DRAG_MOTION)


IMPLEMENT_CLASS(wxAuiNotebook, wxControl)
IMPLEMENT_DYNAMIC_CLASS(wxAuiNotebookEvent, wxEvent)





// This functions are here for this proof of concept
// and will be factored out later. See dockart.cpp
static wxColor StepColour(const wxColor& c, int percent)
{
    int r = c.Red(), g = c.Green(), b = c.Blue();
    return wxColour((unsigned char)wxMin((r*percent)/100,255),
                    (unsigned char)wxMin((g*percent)/100,255),
                    (unsigned char)wxMin((b*percent)/100,255));
}

// This functions are here for this proof of concept
// and will be factored out later. See dockart.cpp
static wxBitmap BitmapFromBits(const unsigned char bits[], int w, int h,
                               const wxColour& color)
{
    wxImage img = wxBitmap((const char*)bits, w, h).ConvertToImage();
    img.Replace(0,0,0,123,123,123);
    img.Replace(255,255,255,color.Red(),color.Green(),color.Blue());
    img.SetMaskColour(123,123,123);
    return wxBitmap(img);
}

static void DrawButtonS(wxDC& dc,
                       const wxRect& _rect,
                       const wxBitmap& bmp,
                       const wxColour& bkcolour,
                       int button_state)
{
    wxRect rect = _rect;

    if (button_state == wxAUI_BUTTON_STATE_PRESSED)
    {
        rect.x++;
        rect.y++;
    }

    if (button_state == wxAUI_BUTTON_STATE_HOVER ||
        button_state == wxAUI_BUTTON_STATE_PRESSED)
    {
        dc.SetBrush(wxBrush(StepColour(bkcolour, 120)));
        dc.SetPen(wxPen(StepColour(bkcolour, 70)));

        // draw the background behind the button
        dc.DrawRectangle(rect.x, rect.y, 15, 15);
    }

    // draw the button itself
    dc.DrawBitmap(bmp, rect.x, rect.y, true);
}





// -- wxAuiDefaultTabArt class implementation --

wxAuiDefaultTabArt::wxAuiDefaultTabArt()
{
    m_normal_font = *wxNORMAL_FONT;
    m_selected_font = *wxNORMAL_FONT;
    m_selected_font.SetWeight(wxBOLD);
    m_measuring_font = m_selected_font;

    wxColour base_colour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);

    wxColour background_colour = StepColour(base_colour, 95);
    wxColour normaltab_colour = base_colour;
    wxColour selectedtab_colour = *wxWHITE;

    m_bkbrush = wxBrush(background_colour);
    m_normal_bkbrush = wxBrush(normaltab_colour);
    m_normal_bkpen = wxPen(normaltab_colour);
    m_selected_bkbrush = wxBrush(selectedtab_colour);
    m_selected_bkpen = wxPen(selectedtab_colour);
    
    
#if defined( __WXMAC__ )
     static unsigned char close_bits[]={
         0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFE, 0x03, 0xF8, 0x01, 0xF0, 0x19, 0xF3,
         0xB8, 0xE3, 0xF0, 0xE1, 0xE0, 0xE0, 0xF0, 0xE1, 0xB8, 0xE3, 0x19, 0xF3,
         0x01, 0xF0, 0x03, 0xF8, 0x0F, 0xFE, 0xFF, 0xFF };
#elif defined( __WXGTK__)
    static unsigned char close_bits[]={
         0xff, 0xff, 0xff, 0xff, 0x07, 0xf0, 0xfb, 0xef, 0xdb, 0xed, 0x8b, 0xe8,
         0x1b, 0xec, 0x3b, 0xee, 0x1b, 0xec, 0x8b, 0xe8, 0xdb, 0xed, 0xfb, 0xef,
         0x07, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
#else
    static unsigned char close_bits[]={
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0xfb,0xcf,0xf9,
        0x9f,0xfc,0x3f,0xfe,0x3f,0xfe,0x9f,0xfc,0xcf,0xf9,0xef,0xfb,
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
#endif

    static unsigned char left_bits[] = {
       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xfe, 0x3f, 0xfe,
       0x1f, 0xfe, 0x0f, 0xfe, 0x1f, 0xfe, 0x3f, 0xfe, 0x7f, 0xfe, 0xff, 0xfe,
       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    
    static unsigned char right_bits[] = {
       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0x9f, 0xff, 0x1f, 0xff,
       0x1f, 0xfe, 0x1f, 0xfc, 0x1f, 0xfe, 0x1f, 0xff, 0x9f, 0xff, 0xdf, 0xff,
       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
   
    static unsigned char list_bits[] = {
       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
       0x0f, 0xf8, 0xff, 0xff, 0x0f, 0xf8, 0x1f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff,
       0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
   
   
    m_active_close_bmp = BitmapFromBits(close_bits, 16, 16, *wxBLACK);
    m_disabled_close_bmp = BitmapFromBits(close_bits, 16, 16, wxColour(128,128,128));
    
    m_active_left_bmp = BitmapFromBits(left_bits, 16, 16, *wxBLACK);
    m_disabled_left_bmp = BitmapFromBits(left_bits, 16, 16, wxColour(128,128,128));
    
    m_active_right_bmp = BitmapFromBits(right_bits, 16, 16, *wxBLACK);
    m_disabled_right_bmp = BitmapFromBits(right_bits, 16, 16, wxColour(128,128,128));
    
    m_active_windowlist_bmp = BitmapFromBits(list_bits, 16, 16, *wxBLACK);
    m_disabled_windowlist_bmp = BitmapFromBits(list_bits, 16, 16, wxColour(128,128,128));
    
}

wxAuiDefaultTabArt::~wxAuiDefaultTabArt()
{
}

void wxAuiDefaultTabArt::DrawBackground(wxDC* dc,
                                        wxWindow* WXUNUSED(wnd),
                                        const wxRect& rect)
{
    // draw background
    dc->SetBrush(m_bkbrush);
    dc->SetPen(*wxTRANSPARENT_PEN);
    dc->DrawRectangle(-1, -1, rect.GetWidth()+2, rect.GetHeight()+2);

    // draw base line
    dc->SetPen(*wxGREY_PEN);
    dc->DrawLine(0, rect.GetHeight()-1, rect.GetWidth(), rect.GetHeight()-1);
}


// DrawTab() draws an individual tab.
//
// dc       - output dc
// in_rect  - rectangle the tab should be confined to
// caption  - tab's caption
// active   - whether or not the tab is active
// out_rect - actual output rectangle
// x_extent - the advance x; where the next tab should start

void wxAuiDefaultTabArt::DrawTab(wxDC* dc,
                                 wxWindow* wnd,
                                 const wxRect& in_rect,
                                 const wxString& caption_text,
                                 bool active,
                                 int close_button_state,
                                 wxRect* out_tab_rect,
                                 wxRect* out_button_rect,
                                 int* x_extent)
{
    wxCoord normal_textx, normal_texty;
    wxCoord selected_textx, selected_texty;
    wxCoord textx, texty;
    
    // if the caption is empty, measure some temporary text
    wxString caption = caption_text;
    if (caption_text.empty())
        caption = wxT("Xj");
            
    dc->SetFont(m_selected_font);
    dc->GetTextExtent(caption, &selected_textx, &selected_texty);
    
    dc->SetFont(m_normal_font);
    dc->GetTextExtent(caption, &normal_textx, &normal_texty);
        
    // figure out the size of the tab
    wxSize tab_size = GetTabSize(dc, wnd, caption, active, close_button_state, x_extent);

    wxCoord tab_height = tab_size.y;
    wxCoord tab_width = tab_size.x;
    wxCoord tab_x = in_rect.x;
    wxCoord tab_y = in_rect.y + in_rect.height - tab_height;

    // select pen, brush and font for the tab to be drawn

    if (active)
    {
        dc->SetPen(m_selected_bkpen);
        dc->SetBrush(m_selected_bkbrush);
        dc->SetFont(m_selected_font);
        textx = selected_textx;
        texty = selected_texty;
    }
     else
    {
        dc->SetPen(m_normal_bkpen);
        dc->SetBrush(m_normal_bkbrush);
        dc->SetFont(m_normal_font);
        textx = normal_textx;
        texty = normal_texty;
    }


    // -- draw line --

    wxPoint points[7];
    points[0].x = tab_x;
    points[0].y = tab_y + tab_height - 1;
    points[1].x = tab_x + tab_height - 3;
    points[1].y = tab_y + 2;
    points[2].x = tab_x + tab_height + 3;
    points[2].y = tab_y;
    points[3].x = tab_x + tab_width - 2;
    points[3].y = tab_y;
    points[4].x = tab_x + tab_width;
    points[4].y = tab_y + 2;
    points[5].x = tab_x + tab_width;
    points[5].y = tab_y + tab_height - 1;
    points[6] = points[0];


    dc->DrawPolygon(6, points);

    dc->SetPen(*wxGREY_PEN);

    //dc->DrawLines(active ? 6 : 7, points);
    dc->DrawLines(7, points);


    int text_offset;

    int close_button_width = 0;
    if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN)
    {
        close_button_width = m_active_close_bmp.GetWidth();
        text_offset = tab_x + (tab_height/2) + ((tab_width-close_button_width)/2) - (textx/2);
    }
     else
    {
        text_offset = tab_x + (tab_height/3) + (tab_width/2) - (textx/2);
    }
    

    // draw tab text
    dc->DrawText(caption,
                 text_offset,
                 (tab_y + tab_height)/2 - (texty/2) + 1);


    // draw close button if necessary
    if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN)
    {
        wxBitmap bmp;
        if (active)
            bmp = m_active_close_bmp;
             else
            bmp = m_disabled_close_bmp;
            
        wxRect rect(tab_x + tab_width - close_button_width - 1,
                    tab_y + (tab_height/2) - (bmp.GetHeight()/2) + 1,
                    close_button_width,
                    tab_height - 1);
        DrawButtonS(*dc, rect, bmp, *wxWHITE, close_button_state);
        
        *out_button_rect = rect;
    }


    *out_tab_rect = wxRect(tab_x, tab_y, tab_width, tab_height);
}


wxSize wxAuiDefaultTabArt::GetTabSize(wxDC* dc,
                                      wxWindow* WXUNUSED(wnd),
                                      const wxString& caption,
                                      bool WXUNUSED(active),
                                      int close_button_state,
                                      int* x_extent)
{
    wxCoord measured_textx, measured_texty;
    
    dc->SetFont(m_measuring_font);
    dc->GetTextExtent(caption, &measured_textx, &measured_texty);
    
    wxCoord tab_height = measured_texty + 4;
    wxCoord tab_width = measured_textx + tab_height + 5;

    if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN)
        tab_width += m_active_close_bmp.GetWidth();

    *x_extent = tab_width - (tab_height/2) - 1;

    return wxSize(tab_width, tab_height);
}


void wxAuiDefaultTabArt::DrawButton(
                                    wxDC* dc,
                                    wxWindow* WXUNUSED(wnd),
                                    const wxRect& in_rect,
                                    int bitmap_id,
                                    int button_state,
                                    int orientation,
                                    const wxBitmap& bitmap_override,
                                    wxRect* out_rect)
{
    wxBitmap bmp;
    wxRect rect;
    
    if (bitmap_override.IsOk())
    {
        bmp = bitmap_override;
    }
     else
    {
        switch (bitmap_id)
        {
            case wxAUI_BUTTON_CLOSE:
                if (button_state & wxAUI_BUTTON_STATE_DISABLED)
                    bmp = m_disabled_close_bmp;
                     else
                    bmp = m_active_close_bmp;
                break;
            case wxAUI_BUTTON_LEFT:
                if (button_state & wxAUI_BUTTON_STATE_DISABLED)
                    bmp = m_disabled_left_bmp;
                     else
                    bmp = m_active_left_bmp;
                break;
            case wxAUI_BUTTON_RIGHT:
                if (button_state & wxAUI_BUTTON_STATE_DISABLED)
                    bmp = m_disabled_right_bmp;
                     else
                    bmp = m_active_right_bmp;
                break;
            case wxAUI_BUTTON_WINDOWLIST:
                if (button_state & wxAUI_BUTTON_STATE_DISABLED)
                    bmp = m_disabled_windowlist_bmp;
                     else
                    bmp = m_active_windowlist_bmp;
                break;
        }
    }

    if (!bmp.IsOk())
        return;
        
    rect = in_rect;
    
    if (orientation == wxLEFT)
    {
        rect.SetX(in_rect.x);
        rect.SetY(((in_rect.y + in_rect.height)/2) - (bmp.GetHeight()/2));
        rect.SetWidth(bmp.GetWidth());
        rect.SetHeight(bmp.GetHeight());
    }
     else
    {
        rect = wxRect(in_rect.x + in_rect.width - bmp.GetWidth(),
                      ((in_rect.y + in_rect.height)/2) - (bmp.GetHeight()/2),
                      bmp.GetWidth(), bmp.GetHeight());
    }
    
        
    DrawButtonS(*dc, rect, bmp, *wxWHITE, button_state);
    
    *out_rect = rect;
}




// -- GUI helper classes and functions --

class wxAuiCommandCapture : public wxEvtHandler
{
public:
    
    wxAuiCommandCapture() { m_last_id = 0; }
    int GetCommandId() const { return m_last_id; }

    bool ProcessEvent(wxEvent& evt)
    {
        if (evt.GetEventType() == wxEVT_COMMAND_MENU_SELECTED)
        {
            m_last_id = evt.GetId();
            return true;
        }
        
        if (GetNextHandler())
            return GetNextHandler()->ProcessEvent(evt);

        return false;
    }
    
private:
    int m_last_id;
};



int wxAuiDefaultTabArt::ShowWindowList(wxWindow* wnd,
                                       const wxArrayString& items,
                                       int active_idx)
{
	wxMenu menuPopup;

    size_t i, count = items.GetCount();
    for (i = 0; i < count; ++i)
    {
        menuPopup.AppendCheckItem(1000+i, items.Item(i));
    }
    
    if (active_idx != -1)
    {
        menuPopup.Check(1000+active_idx, true);
    }
    
    // find out where to put the popup menu of window
    // items.  Subtract 100 for now to center the menu
    // a bit, until a better mechanism can be implemented
    wxPoint pt = ::wxGetMousePosition();
    pt = wnd->ScreenToClient(pt);
    if (pt.x < 100)
        pt.x = 0;
         else
        pt.x -= 100;
    
    // find out the screen coordinate at the bottom of the tab ctrl
    wxRect cli_rect = wnd->GetClientRect();
    pt.y = cli_rect.y + cli_rect.height;
    
    wxAuiCommandCapture* cc = new wxAuiCommandCapture;
    wnd->PushEventHandler(cc);
    wnd->PopupMenu(&menuPopup, pt);
    int command = cc->GetCommandId();
    wnd->PopEventHandler(true);
    
    if (command >= 1000)
        return command-1000;
        
    return -1;
}

int wxAuiDefaultTabArt::GetBestTabCtrlSize(wxWindow* wnd)
{
    wxClientDC dc(wnd);
    dc.SetFont(m_measuring_font);
    int x_ext = 0;
    wxSize s = GetTabSize(&dc,
                          wnd,
                          wxT("ABCDEFGHIj"),
                          true,
                          wxAUI_BUTTON_STATE_HIDDEN,
                          &x_ext);
    return s.y+3;
}

void wxAuiDefaultTabArt::SetNormalFont(const wxFont& font)
{
    m_normal_font = font;
}

void wxAuiDefaultTabArt::SetSelectedFont(const wxFont& font)
{
    m_selected_font = font;
}

void wxAuiDefaultTabArt::SetMeasuringFont(const wxFont& font)
{
    m_measuring_font = font;
}






// -- wxAuiTabContainer class implementation --


// wxAuiTabContainer is a class which contains information about each
// tab.  It also can render an entire tab control to a specified DC.
// It's not a window class itself, because this code will be used by
// the wxFrameMananger, where it is disadvantageous to have separate
// windows for each tab control in the case of "docked tabs"

// A derived class, wxAuiTabCtrl, is an actual wxWindow-derived window
// which can be used as a tab control in the normal sense.


wxAuiTabContainer::wxAuiTabContainer()
{
    m_tab_offset = 0;
    m_flags = 0;
    m_art = new wxAuiDefaultTabArt;
    
    AddButton(wxAUI_BUTTON_LEFT, wxLEFT);       
    AddButton(wxAUI_BUTTON_RIGHT, wxRIGHT);
    AddButton(wxAUI_BUTTON_WINDOWLIST, wxRIGHT);
    AddButton(wxAUI_BUTTON_CLOSE, wxRIGHT);
}

wxAuiTabContainer::~wxAuiTabContainer()
{
    delete m_art;
}

void wxAuiTabContainer::SetArtProvider(wxAuiTabArt* art)
{
    delete m_art;
    m_art = art;
}

wxAuiTabArt* wxAuiTabContainer::GetArtProvider()
{
    return m_art;
}

void wxAuiTabContainer::SetFlags(unsigned int flags)
{
    m_flags = flags;
    
    // check for new close button settings
    RemoveButton(wxAUI_BUTTON_LEFT);       
    RemoveButton(wxAUI_BUTTON_RIGHT);
    RemoveButton(wxAUI_BUTTON_WINDOWLIST);
    RemoveButton(wxAUI_BUTTON_CLOSE);
    
    
    if (flags & wxAUI_NB_SCROLL_BUTTONS)
    {
        AddButton(wxAUI_BUTTON_LEFT, wxLEFT);       
        AddButton(wxAUI_BUTTON_RIGHT, wxRIGHT);
    }

    if (flags & wxAUI_NB_WINDOWLIST_BUTTON)
    {
        AddButton(wxAUI_BUTTON_WINDOWLIST, wxRIGHT);
    }
    
    if (flags & wxAUI_NB_CLOSE_BUTTON)
    {
        AddButton(wxAUI_BUTTON_CLOSE, wxRIGHT);
    }
}

unsigned int wxAuiTabContainer::GetFlags() const
{
    return m_flags;
}


void wxAuiTabContainer::SetNormalFont(const wxFont& font)
{
    m_art->SetNormalFont(font);
}

void wxAuiTabContainer::SetSelectedFont(const wxFont& font)
{
    m_art->SetSelectedFont(font);
}

void wxAuiTabContainer::SetMeasuringFont(const wxFont& font)
{
    m_art->SetMeasuringFont(font);
}

void wxAuiTabContainer::SetRect(const wxRect& rect)
{
    m_rect = rect;
}

bool wxAuiTabContainer::AddPage(wxWindow* page,
                                const wxAuiNotebookPage& info)
{
    wxAuiNotebookPage page_info;
    page_info = info;
    page_info.window = page;

    m_pages.Add(page_info);

    return true;
}

bool wxAuiTabContainer::InsertPage(wxWindow* page,
                                   const wxAuiNotebookPage& info,
                                   size_t idx)
{
    wxAuiNotebookPage page_info;
    page_info = info;
    page_info.window = page;

    if (idx >= m_pages.GetCount())
        m_pages.Add(page_info);
         else
        m_pages.Insert(page_info, idx);

    return true;
}

bool wxAuiTabContainer::MovePage(wxWindow* page,
                                 size_t new_idx)
{
    int idx = GetIdxFromWindow(page);
    if (idx == -1)
        return false;
    
    // get page entry, make a copy of it
    wxAuiNotebookPage p = GetPage(idx);
    
    // remove old page entry
    RemovePage(page);
    
    // insert page where it should be
    InsertPage(page, p, new_idx);
        
    return true;
}

bool wxAuiTabContainer::RemovePage(wxWindow* wnd)
{
    size_t i, page_count = m_pages.GetCount();
    for (i = 0; i < page_count; ++i)
    {
        wxAuiNotebookPage& page = m_pages.Item(i);
        if (page.window == wnd)
        {
            m_pages.RemoveAt(i);
            return true;
        }
    }

    return false;
}

bool wxAuiTabContainer::SetActivePage(wxWindow* wnd)
{
    bool found = false;

    size_t i, page_count = m_pages.GetCount();
    for (i = 0; i < page_count; ++i)
    {
        wxAuiNotebookPage& page = m_pages.Item(i);
        if (page.window == wnd)
        {
            page.active = true;
            found = true;
        }
         else
        {
            page.active = false;
        }
    }

    return found;
}

void wxAuiTabContainer::SetNoneActive()
{
    size_t i, page_count = m_pages.GetCount();
    for (i = 0; i < page_count; ++i)
    {
        wxAuiNotebookPage& page = m_pages.Item(i);
        page.active = false;
    }
}

bool wxAuiTabContainer::SetActivePage(size_t page)
{
    if (page >= m_pages.GetCount())
        return false;

    return SetActivePage(m_pages.Item(page).window);
}

int wxAuiTabContainer::GetActivePage() const
{
    size_t i, page_count = m_pages.GetCount();
    for (i = 0; i < page_count; ++i)
    {
        wxAuiNotebookPage& page = m_pages.Item(i);
        if (page.active)
            return i;
    }

    return -1;
}

wxWindow* wxAuiTabContainer::GetWindowFromIdx(size_t idx) const
{
    if (idx >= m_pages.GetCount())
        return NULL;

    return m_pages[idx].window;
}

int wxAuiTabContainer::GetIdxFromWindow(wxWindow* wnd) const
{
    size_t i, page_count = m_pages.GetCount();
    for (i = 0; i < page_count; ++i)
    {
        wxAuiNotebookPage& page = m_pages.Item(i);
        if (page.window == wnd)
            return i;
    }
    return -1;
}

wxAuiNotebookPage& wxAuiTabContainer::GetPage(size_t idx)
{
    wxASSERT_MSG(idx < m_pages.GetCount(), wxT("Invalid Page index"));

    return m_pages[idx];
}

wxAuiNotebookPageArray& wxAuiTabContainer::GetPages()
{
    return m_pages;
}

size_t wxAuiTabContainer::GetPageCount() const
{
    return m_pages.GetCount();
}

void wxAuiTabContainer::AddButton(int id,
                                  int location,
                                  const wxBitmap& normal_bitmap,
                                  const wxBitmap& disabled_bitmap)
{
    wxAuiTabContainerButton button;
    button.id = id;
    button.bitmap = normal_bitmap;
    button.dis_bitmap = disabled_bitmap;
    button.location = location;
    button.cur_state = wxAUI_BUTTON_STATE_NORMAL;

    m_buttons.Add(button);
}

void wxAuiTabContainer::RemoveButton(int id)
{
    size_t i, button_count = m_buttons.GetCount();

    for (i = 0; i < button_count; ++i)
    {
        if (m_buttons.Item(i).id == id)
        {
            m_buttons.RemoveAt(i);
            return;
        }
    }
}



size_t wxAuiTabContainer::GetTabOffset() const
{
    return m_tab_offset;
}

void wxAuiTabContainer::SetTabOffset(size_t offset)
{
    m_tab_offset = offset;
}

// Render() renders the tab catalog to the specified DC
// It is a virtual function and can be overridden to
// provide custom drawing capabilities
void wxAuiTabContainer::Render(wxDC* raw_dc, wxWindow* wnd)
{
    wxMemoryDC dc;
    wxBitmap bmp;
    size_t i;
    size_t page_count = m_pages.GetCount();
    size_t button_count = m_buttons.GetCount();

    // create off-screen bitmap
    bmp.Create(m_rect.GetWidth(), m_rect.GetHeight());
    dc.SelectObject(bmp);


    // find out if size of tabs is larger than can be
    // afforded on screen
    int total_width = 0;
    int visible_width = 0;
    for (i = 0; i < page_count; ++i)
    {
        wxAuiNotebookPage& page = m_pages.Item(i);
        
        // determine if a close button is on this tab
        bool close_button = false;
        if ((m_flags & wxAUI_NB_CLOSE_ON_ALL_TABS) != 0 ||
            ((m_flags & wxAUI_NB_CLOSE_ON_ACTIVE_TAB) != 0 && page.active))
        {
            close_button = true;
        }
        
    
        int x_extent = 0;
        wxSize size = m_art->GetTabSize(&dc,
                            wnd,
                            page.caption,
                            page.active,
                            close_button ?
                              wxAUI_BUTTON_STATE_NORMAL :
                              wxAUI_BUTTON_STATE_HIDDEN,
                            &x_extent);
        
        if (i+1 < page_count)
            total_width += x_extent;
             else
            total_width += size.x;
            
        if (i >= m_tab_offset)
        {
            if (i+1 < page_count)
                visible_width += x_extent;
                 else
                visible_width += size.x;
        }
    }
    
    if (total_width > m_rect.GetWidth() - 20 || m_tab_offset != 0)
    {
        // show left/right buttons
        for (i = 0; i < button_count; ++i)
        {
            wxAuiTabContainerButton& button = m_buttons.Item(i);
            if (button.id == wxAUI_BUTTON_LEFT ||
                button.id == wxAUI_BUTTON_RIGHT)
            {
                button.cur_state &= ~wxAUI_BUTTON_STATE_HIDDEN;
            }
        }
    }
     else
    {
        // hide left/right buttons
        for (i = 0; i < button_count; ++i)
        {
            wxAuiTabContainerButton& button = m_buttons.Item(i);
            if (button.id == wxAUI_BUTTON_LEFT ||
                button.id == wxAUI_BUTTON_RIGHT)
            {
                button.cur_state |= wxAUI_BUTTON_STATE_HIDDEN;
            }
        }
    }

    // determine whether left button should be enabled
    for (i = 0; i < button_count; ++i)
    {
        wxAuiTabContainerButton& button = m_buttons.Item(i);
        if (button.id == wxAUI_BUTTON_LEFT)
        {
            if (m_tab_offset == 0)
                button.cur_state |= wxAUI_BUTTON_STATE_DISABLED;
                 else
                button.cur_state &= ~wxAUI_BUTTON_STATE_DISABLED;
        }
        if (button.id == wxAUI_BUTTON_RIGHT)
        {
            if (visible_width < m_rect.GetWidth() - ((int)button_count*16))
                button.cur_state |= wxAUI_BUTTON_STATE_DISABLED;
                 else
                button.cur_state &= ~wxAUI_BUTTON_STATE_DISABLED;
        }
    }



    // draw background
    m_art->DrawBackground(&dc, wnd, m_rect);

    // draw buttons
    int left_buttons_width = 0;
    int right_buttons_width = 0;
    
    int offset = 0;

    // draw the buttons on the right side
    offset = m_rect.x + m_rect.width;
    for (i = 0; i < button_count; ++i)
    {
        wxAuiTabContainerButton& button = m_buttons.Item(button_count - i - 1);
        
        if (button.location != wxRIGHT)
            continue;
        if (button.cur_state & wxAUI_BUTTON_STATE_HIDDEN)
            continue;
                
        wxRect button_rect = m_rect;
        button_rect.SetY(1);
        button_rect.SetWidth(offset);

        m_art->DrawButton(&dc,
                          wnd,
                          button_rect,
                          button.id,
                          button.cur_state,
                          wxRIGHT,
                          wxNullBitmap,
                          &button.rect);

        offset -= button.rect.GetWidth();
        right_buttons_width += button.rect.GetWidth();
    }



    offset = 0;
    
    // draw the buttons on the left side

    for (i = 0; i < button_count; ++i)
    {
        wxAuiTabContainerButton& button = m_buttons.Item(button_count - i - 1);
        
        if (button.location != wxLEFT)
            continue;
        if (button.cur_state & wxAUI_BUTTON_STATE_HIDDEN)
            continue;
   
        wxRect button_rect(offset, 1, 1000, m_rect.height);

        m_art->DrawButton(&dc,
                          wnd,
                          button_rect,
                          button.id,
                          button.cur_state,
                          wxLEFT,
                          wxNullBitmap,
                          &button.rect);
                          
        offset += button.rect.GetWidth();
        left_buttons_width += button.rect.GetWidth();
    }

    offset = left_buttons_width;
    
    // set a clipping region to the tabs don't draw over the buttons
    dc.SetClippingRegion(left_buttons_width, 0,
                 m_rect.GetWidth() - right_buttons_width - left_buttons_width - 2,
                 m_rect.GetHeight());
           
           
    
    // prepare the tab-close-button array
    while (m_tab_close_buttons.GetCount() < page_count)
    {
        wxAuiTabContainerButton tempbtn;
        tempbtn.id = wxAUI_BUTTON_CLOSE;
        tempbtn.location = wxCENTER;
        tempbtn.cur_state = wxAUI_BUTTON_STATE_HIDDEN;
        m_tab_close_buttons.Add(tempbtn);
    }
    
    for (i = 0; i < m_tab_offset; ++i)
    {
        // buttons before the tab offset must be set to hidden
        m_tab_close_buttons.Item(i).cur_state = wxAUI_BUTTON_STATE_HIDDEN;
    }
        
                         
    // draw the tabs

    size_t active = 999;
    int active_offset = 0;

    int x_extent = 0;
    wxRect rect = m_rect;
    rect.y = 0;
    rect.width = 1000;
    rect.height = m_rect.height;

    for (i = m_tab_offset; i < page_count; ++i)
    {
        wxAuiNotebookPage& page = m_pages.Item(i);
        wxAuiTabContainerButton& tab_button = m_tab_close_buttons.Item(i);

        // determine if a close button is on this tab
        bool close_button = false;
        if ((m_flags & wxAUI_NB_CLOSE_ON_ALL_TABS) != 0 ||
            ((m_flags & wxAUI_NB_CLOSE_ON_ACTIVE_TAB) != 0 && page.active))
        {
            close_button = true;
            if (tab_button.cur_state == wxAUI_BUTTON_STATE_HIDDEN)
            {
                tab_button.id = wxAUI_BUTTON_CLOSE;
                tab_button.cur_state = wxAUI_BUTTON_STATE_NORMAL;
                tab_button.location = wxCENTER;
            }
        }
         else
        {
            tab_button.cur_state = wxAUI_BUTTON_STATE_HIDDEN;
        }

        rect.x = offset;

        m_art->DrawTab(&dc,
                       wnd,
                       rect,
                       page.caption,
                       page.active,
                       tab_button.cur_state,
                       &page.rect,
                       &tab_button.rect,
                       &x_extent);

        if (page.active)
        {
            active = i;
            active_offset = offset;
        }
        
        offset += x_extent;
    }

    // draw the active tab again so it stands in the foreground
    if (active >= m_tab_offset && active < m_pages.GetCount())
    {
        wxAuiNotebookPage& page = m_pages.Item(active);

        wxAuiTabContainerButton& tab_button = m_tab_close_buttons.Item(active);

        // determine if a close button is on this tab
        bool close_button = false;
        if ((m_flags & wxAUI_NB_CLOSE_ON_ALL_TABS) != 0 ||
            ((m_flags & wxAUI_NB_CLOSE_ON_ACTIVE_TAB) != 0 && page.active))
        {
            close_button = true;
        }

        rect.x = active_offset;
        m_art->DrawTab(&dc,
                       wnd,
                       rect,
                       page.caption,
                       page.active,
                       tab_button.cur_state,
                       &page.rect,
                       &tab_button.rect,
                       &x_extent);
    }

    dc.DestroyClippingRegion();
    
    raw_dc->Blit(m_rect.x, m_rect.y,
                 m_rect.GetWidth(), m_rect.GetHeight(),
                 &dc, 0, 0);
}


// TabHitTest() tests if a tab was hit, passing the window pointer
// back if that condition was fulfilled.  The function returns
// true if a tab was hit, otherwise false
bool wxAuiTabContainer::TabHitTest(int x, int y, wxWindow** hit) const
{
    if (!m_rect.Contains(x,y))
        return false;
    
    wxAuiTabContainerButton* btn = NULL;
    if (ButtonHitTest(x, y, &btn))
    {
        if (m_buttons.Index(*btn) != wxNOT_FOUND)
            return false;
    }

    size_t i, page_count = m_pages.GetCount();

    for (i = m_tab_offset; i < page_count; ++i)
    {
        wxAuiNotebookPage& page = m_pages.Item(i);
        if (page.rect.Contains(x,y))
        {
            if (hit)
                *hit = page.window;
            return true;
        }
    }

    return false;
}

// ButtonHitTest() tests if a button was hit. The function returns
// true if a button was hit, otherwise false
bool wxAuiTabContainer::ButtonHitTest(int x, int y,
                                      wxAuiTabContainerButton** hit) const
{
    if (!m_rect.Contains(x,y))
        return false;

    size_t i, button_count;
    
    
    button_count = m_buttons.GetCount();
    for (i = 0; i < button_count; ++i)
    {
        wxAuiTabContainerButton& button = m_buttons.Item(i);
        if (button.rect.Contains(x,y))
        {
            if (hit)
                *hit = &button;
            return true;
        }
    }
    
    button_count = m_tab_close_buttons.GetCount();
    for (i = 0; i < button_count; ++i)
    {
        wxAuiTabContainerButton& button = m_tab_close_buttons.Item(i);
        if (button.rect.Contains(x,y))
        {
            if (hit)
                *hit = &button;
            return true;
        }
    }
    
    return false;
}



// the utility function ShowWnd() is the same as show,
// except it handles wxAuiMDIChildFrame windows as well,
// as the Show() method on this class is "unplugged"
static void ShowWnd(wxWindow* wnd, bool show)
{
    if (wnd->IsKindOf(CLASSINFO(wxAuiMDIChildFrame)))
    {
        wxAuiMDIChildFrame* cf = (wxAuiMDIChildFrame*)wnd;
        cf->DoShow(show);
    }
     else
    {
        wnd->Show(show);
    }
}


// DoShowHide() this function shows the active window, then
// hides all of the other windows (in that order)
void wxAuiTabContainer::DoShowHide()
{
    wxAuiNotebookPageArray& pages = GetPages();
    size_t i, page_count = pages.GetCount();

    // show new active page first
    for (i = 0; i < page_count; ++i)
    {
        wxAuiNotebookPage& page = pages.Item(i);
        if (page.active)
        {
            ShowWnd(page.window, true);
            break;
        }
    }

    // hide all other pages
    for (i = 0; i < page_count; ++i)
    {
        wxAuiNotebookPage& page = pages.Item(i);
        ShowWnd(page.window, page.active);
    }
}






// -- wxAuiTabCtrl class implementation --



BEGIN_EVENT_TABLE(wxAuiTabCtrl, wxControl)
    EVT_PAINT(wxAuiTabCtrl::OnPaint)
    EVT_ERASE_BACKGROUND(wxAuiTabCtrl::OnEraseBackground)
    EVT_SIZE(wxAuiTabCtrl::OnSize)
    EVT_LEFT_DOWN(wxAuiTabCtrl::OnLeftDown)
    EVT_LEFT_UP(wxAuiTabCtrl::OnLeftUp)
    EVT_MOTION(wxAuiTabCtrl::OnMotion)
    EVT_LEAVE_WINDOW(wxAuiTabCtrl::OnLeaveWindow)
    EVT_AUINOTEBOOK_BUTTON(-1, wxAuiTabCtrl::OnButton)
END_EVENT_TABLE()


wxAuiTabCtrl::wxAuiTabCtrl(wxWindow* parent,
                           wxWindowID id,
                           const wxPoint& pos,
                           const wxSize& size,
                           long style) : wxControl(parent, id, pos, size, style)
{
    m_click_pt = wxDefaultPosition;
    m_is_dragging = false;
    m_hover_button = NULL;
}

wxAuiTabCtrl::~wxAuiTabCtrl()
{
}

void wxAuiTabCtrl::OnPaint(wxPaintEvent&)
{
    wxPaintDC dc(this);

    dc.SetFont(GetFont());

    if (GetPageCount() > 0)
        Render(&dc, this);
}

void wxAuiTabCtrl::OnEraseBackground(wxEraseEvent& WXUNUSED(evt))
{
}

void wxAuiTabCtrl::OnSize(wxSizeEvent& evt)
{
    wxSize s = evt.GetSize();
    wxRect r(0, 0, s.GetWidth(), s.GetHeight());
    SetRect(r);
}

void wxAuiTabCtrl::OnLeftDown(wxMouseEvent& evt)
{
    CaptureMouse();
    m_click_pt = wxDefaultPosition;
    m_is_dragging = false;
    m_click_tab = NULL;

    wxWindow* wnd;
    if (TabHitTest(evt.m_x, evt.m_y, &wnd))
    {
        wxAuiNotebookEvent e(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGING, m_windowId);
        e.SetSelection(GetIdxFromWindow(wnd));
        e.SetOldSelection(GetActivePage());
        e.SetEventObject(this);
        GetEventHandler()->ProcessEvent(e);

        m_click_pt.x = evt.m_x;
        m_click_pt.y = evt.m_y;
        m_click_tab = wnd;
    }

    if (m_hover_button)
    {
        m_hover_button->cur_state = wxAUI_BUTTON_STATE_PRESSED;
        Refresh();
        Update();
    }
}

void wxAuiTabCtrl::OnLeftUp(wxMouseEvent&)
{
    if (GetCapture() == this)
        ReleaseMouse();

    if (m_is_dragging)
    {
        wxAuiNotebookEvent evt(wxEVT_COMMAND_AUINOTEBOOK_END_DRAG, m_windowId);
        evt.SetSelection(GetIdxFromWindow(m_click_tab));
        evt.SetOldSelection(evt.GetSelection());
        evt.SetEventObject(this);
        GetEventHandler()->ProcessEvent(evt);
        return;
    }

    if (m_hover_button)
    {
        m_hover_button->cur_state = wxAUI_BUTTON_STATE_HOVER;
        Refresh();
        Update();

        if (!(m_hover_button->cur_state & wxAUI_BUTTON_STATE_DISABLED))
        {
            wxAuiNotebookEvent evt(wxEVT_COMMAND_AUINOTEBOOK_BUTTON, m_windowId);
            evt.SetInt(m_hover_button->id);
            evt.SetEventObject(this);
            GetEventHandler()->ProcessEvent(evt);
        }
    }

    m_click_pt = wxDefaultPosition;
    m_is_dragging = false;
    m_click_tab = NULL;
}

void wxAuiTabCtrl::OnMotion(wxMouseEvent& evt)
{
    wxPoint pos = evt.GetPosition();

    // check if the mouse is hovering above a button
    wxAuiTabContainerButton* button;
    if (ButtonHitTest(pos.x, pos.y, &button))
    {
        if (m_hover_button && button != m_hover_button)
        {
            m_hover_button->cur_state = wxAUI_BUTTON_STATE_NORMAL;
            m_hover_button = NULL;
            Refresh();
            Update();
        }
        
        if (button->cur_state != wxAUI_BUTTON_STATE_HOVER)
        {
            button->cur_state = wxAUI_BUTTON_STATE_HOVER;
            Refresh();
            Update();
            m_hover_button = button;
            return;
        }
    }
     else
    {
        if (m_hover_button)
        {
            m_hover_button->cur_state = wxAUI_BUTTON_STATE_NORMAL;
            m_hover_button = NULL;
            Refresh();
            Update();
        }
    }


    if (!evt.LeftIsDown() || m_click_pt == wxDefaultPosition)
        return;

    if (m_is_dragging)
    {
        wxAuiNotebookEvent evt(wxEVT_COMMAND_AUINOTEBOOK_DRAG_MOTION, m_windowId);
        evt.SetSelection(GetIdxFromWindow(m_click_tab));
        evt.SetOldSelection(evt.GetSelection());
        evt.SetEventObject(this);
        GetEventHandler()->ProcessEvent(evt);
        return;
    }


    int drag_x_threshold = wxSystemSettings::GetMetric(wxSYS_DRAG_X);
    int drag_y_threshold = wxSystemSettings::GetMetric(wxSYS_DRAG_Y);

    if (abs(pos.x - m_click_pt.x) > drag_x_threshold ||
        abs(pos.y - m_click_pt.y) > drag_y_threshold)
    {
        wxAuiNotebookEvent evt(wxEVT_COMMAND_AUINOTEBOOK_BEGIN_DRAG, m_windowId);
        evt.SetSelection(GetIdxFromWindow(m_click_tab));
        evt.SetOldSelection(evt.GetSelection());
        evt.SetEventObject(this);
        GetEventHandler()->ProcessEvent(evt);

        m_is_dragging = true;
    }
}

void wxAuiTabCtrl::OnLeaveWindow(wxMouseEvent& WXUNUSED(event))
{
    if (m_hover_button)
    {
        m_hover_button->cur_state = wxAUI_BUTTON_STATE_NORMAL;
        m_hover_button = NULL;
        Refresh();
        Update();
    }
}

void wxAuiTabCtrl::OnButton(wxAuiNotebookEvent& event)
{
    int button = event.GetInt();
    
    if (button == wxAUI_BUTTON_LEFT || button == wxAUI_BUTTON_RIGHT)
    {
        if (button == wxAUI_BUTTON_LEFT)
        {
            if (GetTabOffset() > 0)
            {
                SetTabOffset(GetTabOffset()-1);
                Refresh();
                Update();
            }
        }
         else
        {
            SetTabOffset(GetTabOffset()+1);
            Refresh();
            Update();
        }
    }
     else if (button == wxAUI_BUTTON_WINDOWLIST)
    {
        wxArrayString as;
        
        size_t i, page_count = m_pages.GetCount();
        for (i = 0; i < page_count; ++i)
        {
            wxAuiNotebookPage& page = m_pages.Item(i);
            as.Add(page.caption);
        }

        int idx = GetArtProvider()->ShowWindowList(this, as, GetActivePage());
        
        if (idx != -1)
        {
            wxAuiNotebookEvent e(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGING, m_windowId);
            e.SetSelection(idx);
            e.SetOldSelection(GetActivePage());
            e.SetEventObject(this);
            GetEventHandler()->ProcessEvent(e);
        }
    }
     else
    {
        event.Skip();
    }
}

// wxTabFrame is an interesting case.  It's important that all child pages
// of the multi-notebook control are all actually children of that control
// (and not grandchildren).  wxTabFrame facilitates this.  There is one
// instance of wxTabFrame for each tab control inside the multi-notebook.
// It's important to know that wxTabFrame is not a real window, but it merely
// used to capture the dimensions/positioning of the internal tab control and
// it's managed page windows

class wxTabFrame : public wxWindow
{
public:

    wxTabFrame()
    {
        m_tabs = NULL;
        m_rect = wxRect(0,0,200,200);
        m_tab_ctrl_height = 20;
    }

    void SetTabCtrlHeight(int h)
    {
        m_tab_ctrl_height = h;
    }

    void DoSetSize(int x, int y,
                   int width, int height,
                   int WXUNUSED(sizeFlags = wxSIZE_AUTO))
    {
        m_rect = wxRect(x, y, width, height);
        DoSizing();
    }

    void DoGetClientSize(int* x, int* y) const
    {
        *x = m_rect.width;
        *y = m_rect.height;
    }

    bool Show( bool WXUNUSED(show = true) ) { return false; }

    void DoSizing()
    {
        if (!m_tabs)
            return;

        int tab_height = wxMin(m_rect.height, m_tab_ctrl_height);
        m_tab_rect = wxRect(m_rect.x, m_rect.y, m_rect.width, tab_height);
        m_tabs->SetSize(m_rect.x, m_rect.y, m_rect.width, tab_height);
        m_tabs->SetRect(wxRect(0, 0, m_rect.width, tab_height));
        m_tabs->Refresh();
        m_tabs->Update();

        wxAuiNotebookPageArray& pages = m_tabs->GetPages();
        size_t i, page_count = pages.GetCount();

        for (i = 0; i < page_count; ++i)
        {
            wxAuiNotebookPage& page = pages.Item(i);
            page.window->SetSize(m_rect.x, m_rect.y+tab_height,
                                 m_rect.width, m_rect.height-tab_height);

            if (page.window->IsKindOf(CLASSINFO(wxAuiMDIChildFrame)))
            {
                wxAuiMDIChildFrame* wnd = (wxAuiMDIChildFrame*)page.window;
                wnd->ApplyMDIChildFrameRect();
            }
        }
    }

    void DoGetSize(int* x, int* y) const
    {
        if (x)
            *x = m_rect.GetWidth();
        if (y)
            *y = m_rect.GetHeight();
    }

    void Update()
    {
        // does nothing
    }

public:

    wxRect m_rect;
    wxRect m_tab_rect;
    wxAuiTabCtrl* m_tabs;
    int m_tab_ctrl_height;
};





// -- wxAuiNotebook class implementation --

BEGIN_EVENT_TABLE(wxAuiNotebook, wxControl)
    //EVT_ERASE_BACKGROUND(wxAuiNotebook::OnEraseBackground)
    //EVT_SIZE(wxAuiNotebook::OnSize)
    //EVT_LEFT_DOWN(wxAuiNotebook::OnLeftDown)
    EVT_CHILD_FOCUS(wxAuiNotebook::OnChildFocus)
    EVT_COMMAND_RANGE(10000, 10100,
                      wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGING,
                      wxAuiNotebook::OnTabClicked)
    EVT_COMMAND_RANGE(10000, 10100,
                      wxEVT_COMMAND_AUINOTEBOOK_BEGIN_DRAG,
                      wxAuiNotebook::OnTabBeginDrag)
    EVT_COMMAND_RANGE(10000, 10100,
                      wxEVT_COMMAND_AUINOTEBOOK_END_DRAG,
                      wxAuiNotebook::OnTabEndDrag)
    EVT_COMMAND_RANGE(10000, 10100,
                      wxEVT_COMMAND_AUINOTEBOOK_DRAG_MOTION,
                      wxAuiNotebook::OnTabDragMotion)
    EVT_COMMAND_RANGE(10000, 10100,
                      wxEVT_COMMAND_AUINOTEBOOK_BUTTON,
                      wxAuiNotebook::OnTabButton)
END_EVENT_TABLE()

wxAuiNotebook::wxAuiNotebook()
{
    m_curpage = -1;
    m_tab_id_counter = 10000;
    m_dummy_wnd = NULL;
    m_tab_ctrl_height = 20;
}

wxAuiNotebook::wxAuiNotebook(wxWindow *parent,
                             wxWindowID id,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style) : wxControl(parent, id, pos, size, style)
{
    InitNotebook(style);
}

bool wxAuiNotebook::Create(wxWindow* parent,
                           wxWindowID id,
                           const wxPoint& pos,
                           const wxSize& size,
                           long style)
{
    if (!wxControl::Create(parent, id, pos, size, style))
        return false;

    InitNotebook(style);

    return true;
}

// InitNotebook() contains common initialization
// code called by all constructors
void wxAuiNotebook::InitNotebook(long style)
{
    m_curpage = -1;
    m_tab_id_counter = 10000;
    m_dummy_wnd = NULL;
    m_tab_ctrl_height = 20;
    m_flags = (unsigned int)style;
    
    m_normal_font = *wxNORMAL_FONT;
    m_selected_font = *wxNORMAL_FONT;
    m_selected_font.SetWeight(wxBOLD);

    // choose a default for the tab height
    m_tab_ctrl_height = m_tabs.GetArtProvider()->GetBestTabCtrlSize(this);

    m_dummy_wnd = new wxWindow(this, wxID_ANY, wxPoint(0,0), wxSize(0,0));
    m_dummy_wnd->SetSize(200, 200);
    m_dummy_wnd->Show(false);

    m_mgr.SetManagedWindow(this);

    m_mgr.AddPane(m_dummy_wnd,
              wxAuiPaneInfo().Name(wxT("dummy")).Bottom().Show(false));

    m_mgr.Update();
}

wxAuiNotebook::~wxAuiNotebook()
{
    m_mgr.UnInit();
}

void wxAuiNotebook::SetArtProvider(wxAuiTabArt* art)
{
    m_tabs.SetArtProvider(art);
}

wxAuiTabArt* wxAuiNotebook::GetArtProvider()
{
    return m_tabs.GetArtProvider();
}

void wxAuiNotebook::SetWindowStyleFlag(long style)
{
    wxControl::SetWindowStyleFlag(style);
    
    m_flags = (unsigned int)style;
    
    // if the control is already initialized
    if (m_mgr.GetManagedWindow() == (wxWindow*)this)
    {
        // let all of the tab children know about the new style
        
        wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
        size_t i, pane_count = all_panes.GetCount();
        for (i = 0; i < pane_count; ++i)
        {
            wxAuiPaneInfo& pane = all_panes.Item(i);
            if (pane.name == wxT("dummy"))
                continue;
            wxAuiTabCtrl* tabctrl = ((wxTabFrame*)pane.window)->m_tabs;
            tabctrl->SetFlags(m_flags);
            tabctrl->Refresh();
            tabctrl->Update();
        }
    }
}


bool wxAuiNotebook::AddPage(wxWindow* page,
                            const wxString& caption,
                            bool select,
                            const wxBitmap& bitmap)
{
    return InsertPage(GetPageCount(), page, caption, select, bitmap);
}

bool wxAuiNotebook::InsertPage(size_t page_idx,
                               wxWindow* page,
                               const wxString& caption,
                               bool select,
                               const wxBitmap& bitmap)
{
    wxAuiNotebookPage info;
    info.window = page;
    info.caption = caption;
    info.bitmap = bitmap;
    info.active = false;

    // if there are currently no tabs, the first added
    // tab must be active
    if (m_tabs.GetPageCount() == 0)
        info.active = true;

    m_tabs.InsertPage(page, info, page_idx);

    wxAuiTabCtrl* active_tabctrl = GetActiveTabCtrl();
    if (page_idx >= active_tabctrl->GetPageCount())
        active_tabctrl->AddPage(page, info);
         else
        active_tabctrl->InsertPage(page, info, page_idx);

    DoSizing();
    active_tabctrl->DoShowHide();

    if (select)
    {
        int idx = m_tabs.GetIdxFromWindow(page);
        wxASSERT_MSG(idx != -1, wxT("Invalid Page index returned on wxAuiNotebook::InsertPage()"));

        SetSelection(idx);
    }

    return true;
}


// DeletePage() removes a tab from the multi-notebook,
// and destroys the window as well
bool wxAuiNotebook::DeletePage(size_t page_idx)
{    
    wxWindow* wnd = m_tabs.GetWindowFromIdx(page_idx);
    wxWindow* new_active = NULL;

    // find out which onscreen tab ctrl owns this tab
    wxAuiTabCtrl* ctrl;
    int ctrl_idx;
    if (!FindTab(wnd, &ctrl, &ctrl_idx))
        return false;

    // find a new page and set it as active
    int new_idx = ctrl_idx+1;
    if (new_idx >= (int)ctrl->GetPageCount())
        new_idx = ctrl_idx-1;

    if (new_idx >= 0 && new_idx < (int)ctrl->GetPageCount())
    {
        new_active = ctrl->GetWindowFromIdx(new_idx);
    }
     else
    {
        // set the active page to the first page that
        // isn't the one being deleted
        size_t i, page_count = m_tabs.GetPageCount();
        for (i = 0; i < page_count; ++i)
        {
            wxWindow* w = m_tabs.GetWindowFromIdx(i);
            if (wnd != w)
            {
                new_active = m_tabs.GetWindowFromIdx(i);
                break;
            }
        }
    }

    // remove the tab from main catalog
    if (!m_tabs.RemovePage(wnd))
        return false;

    // remove the tab from the onscreen tab ctrl
    ctrl->RemovePage(wnd);

    // actually destroy the window now
    if (wnd->IsKindOf(CLASSINFO(wxAuiMDIChildFrame)))
    {
        // delete the child frame with pending delete, as is
        // customary with frame windows
        if (!wxPendingDelete.Member(wnd))
            wxPendingDelete.Append(wnd);
    }
     else
    {
        wnd->Destroy();
    }

    RemoveEmptyTabFrames();

    // set new active pane
    if (new_active)
    {
        m_curpage = -1;
        SetSelection(m_tabs.GetIdxFromWindow(new_active));
    }
    
    return true;
}



// RemovePage() removes a tab from the multi-notebook,
// but does not destroy the window
bool wxAuiNotebook::RemovePage(size_t page_idx)
{
    // remove the tab from our own catalog
    wxWindow* wnd = m_tabs.GetWindowFromIdx(page_idx);
    if (!m_tabs.RemovePage(wnd))
        return false;

    // remove the tab from the onscreen tab ctrl
    wxAuiTabCtrl* ctrl;
    int ctrl_idx;
    if (FindTab(wnd, &ctrl, &ctrl_idx))
    {
        ctrl->RemovePage(wnd);
        return true;
    }

    return false;
}

// SetPageText() changes the tab caption of the specified page
bool wxAuiNotebook::SetPageText(size_t page_idx, const wxString& text)
{
    if (page_idx >= m_tabs.GetPageCount())
        return false;

    // update our own tab catalog
    wxAuiNotebookPage& page_info = m_tabs.GetPage(page_idx);
    page_info.caption = text;

    // update what's on screen
    wxAuiTabCtrl* ctrl;
    int ctrl_idx;
    if (FindTab(page_info.window, &ctrl, &ctrl_idx))
    {
        wxAuiNotebookPage& info = ctrl->GetPage(ctrl_idx);
        info.caption = text;
        ctrl->Refresh();
        ctrl->Update();
    }

    return true;
}

// GetSelection() returns the index of the currently active page
int wxAuiNotebook::GetSelection() const
{
    return m_curpage;
}

// SetSelection() sets the currently active page
size_t wxAuiNotebook::SetSelection(size_t new_page)
{
    wxWindow* wnd = m_tabs.GetWindowFromIdx(new_page);
    if (!wnd)
        return m_curpage;

    wxAuiNotebookEvent evt(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGING, m_windowId);
    evt.SetSelection(new_page);
    evt.SetOldSelection(m_curpage);
    evt.SetEventObject(this);
    if (!GetEventHandler()->ProcessEvent(evt) || evt.IsAllowed())
    {
        // program allows the page change
        evt.SetEventType(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED);
        (void)GetEventHandler()->ProcessEvent(evt);



        wxAuiTabCtrl* ctrl;
        int ctrl_idx;
        if (FindTab(wnd, &ctrl, &ctrl_idx))
        {
            m_tabs.SetActivePage(wnd);

            ctrl->SetActivePage(ctrl_idx);
            DoSizing();
            ctrl->DoShowHide();

            int old_curpage = m_curpage;
            m_curpage = new_page;


            // set fonts
            wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
            size_t i, pane_count = all_panes.GetCount();
            for (i = 0; i < pane_count; ++i)
            {
                wxAuiPaneInfo& pane = all_panes.Item(i);
                if (pane.name == wxT("dummy"))
                    continue;
                wxAuiTabCtrl* tabctrl = ((wxTabFrame*)pane.window)->m_tabs;
                if (tabctrl != ctrl)
                    tabctrl->SetSelectedFont(m_normal_font);
                     else
                    tabctrl->SetSelectedFont(m_selected_font);
                tabctrl->Refresh();
            }

            wnd->SetFocus();

            return old_curpage;
        }
    }

    return m_curpage;
}

// GetPageCount() returns the total number of
// pages managed by the multi-notebook
size_t wxAuiNotebook::GetPageCount() const
{
    return m_tabs.GetPageCount();
}

// GetPage() returns the wxWindow pointer of the
// specified page
wxWindow* wxAuiNotebook::GetPage(size_t page_idx) const
{
    wxASSERT(page_idx < m_tabs.GetPageCount());

    return m_tabs.GetWindowFromIdx(page_idx);
}

// DoSizing() performs all sizing operations in each tab control
void wxAuiNotebook::DoSizing()
{
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    size_t i, pane_count = all_panes.GetCount();
    for (i = 0; i < pane_count; ++i)
    {
        if (all_panes.Item(i).name == wxT("dummy"))
            continue;

        wxTabFrame* tabframe = (wxTabFrame*)all_panes.Item(i).window;
        tabframe->DoSizing();
    }
}

// GetActiveTabCtrl() returns the active tab control.  It is
// called to determine which control gets new windows being added
wxAuiTabCtrl* wxAuiNotebook::GetActiveTabCtrl()
{
    if (m_curpage >= 0 && m_curpage < (int)m_tabs.GetPageCount())
    {
        wxAuiTabCtrl* ctrl;
        int idx;

        // find the tab ctrl with the current page
        if (FindTab(m_tabs.GetPage(m_curpage).window,
                    &ctrl, &idx))
        {
            return ctrl;
        }
    }

    // no current page, just find the first tab ctrl
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    size_t i, pane_count = all_panes.GetCount();
    for (i = 0; i < pane_count; ++i)
    {
        if (all_panes.Item(i).name == wxT("dummy"))
            continue;

        wxTabFrame* tabframe = (wxTabFrame*)all_panes.Item(i).window;
        return tabframe->m_tabs;
    }

    // If there is no tabframe at all, create one
    wxTabFrame* tabframe = new wxTabFrame;
    tabframe->SetTabCtrlHeight(m_tab_ctrl_height);
    tabframe->m_tabs = new wxAuiTabCtrl(this,
                                        m_tab_id_counter++,
                                        wxDefaultPosition,
                                        wxDefaultSize,
                                        wxNO_BORDER);
    tabframe->m_tabs->SetFlags(m_flags);
    m_mgr.AddPane(tabframe,
                  wxAuiPaneInfo().Center().CaptionVisible(false));

    m_mgr.Update();

    return tabframe->m_tabs;
}

// FindTab() finds the tab control that currently contains the window as well
// as the index of the window in the tab control.  It returns true if the
// window was found, otherwise false.
bool wxAuiNotebook::FindTab(wxWindow* page, wxAuiTabCtrl** ctrl, int* idx)
{
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    size_t i, pane_count = all_panes.GetCount();
    for (i = 0; i < pane_count; ++i)
    {
        if (all_panes.Item(i).name == wxT("dummy"))
            continue;

        wxTabFrame* tabframe = (wxTabFrame*)all_panes.Item(i).window;

        int page_idx = tabframe->m_tabs->GetIdxFromWindow(page);
        if (page_idx != -1)
        {
            *ctrl = tabframe->m_tabs;
            *idx = page_idx;
            return true;
        }
    }

    return false;
}


void wxAuiNotebook::OnEraseBackground(wxEraseEvent&)
{
}

void wxAuiNotebook::OnSize(wxSizeEvent&)
{
}

void wxAuiNotebook::OnTabClicked(wxCommandEvent& command_evt)
{
    wxAuiNotebookEvent& evt = (wxAuiNotebookEvent&)command_evt;

    wxAuiTabCtrl* ctrl = (wxAuiTabCtrl*)evt.GetEventObject();
    wxASSERT(ctrl != NULL);

    wxWindow* wnd = ctrl->GetWindowFromIdx(evt.GetSelection());
    wxASSERT(wnd != NULL);

    int idx = m_tabs.GetIdxFromWindow(wnd);
    wxASSERT(idx != -1);

    SetSelection(idx);
}

void wxAuiNotebook::OnTabBeginDrag(wxCommandEvent&)
{
    m_last_drag_x = 0;
}

void wxAuiNotebook::OnTabDragMotion(wxCommandEvent& evt)
{
    wxPoint screen_pt = ::wxGetMousePosition();
    wxPoint client_pt = ScreenToClient(screen_pt);
    wxPoint zero(0,0);

    wxAuiTabCtrl* src_tabs = (wxAuiTabCtrl*)evt.GetEventObject();

    wxAuiTabCtrl* dest_tabs = GetTabCtrlFromPoint(client_pt);
    if (dest_tabs == src_tabs)
    {
        if (src_tabs)
        {
            src_tabs->SetCursor(wxCursor(wxCURSOR_ARROW));
        }
        
        // always hide the hint for inner-tabctrl drag
        m_mgr.HideHint();
        
        // if tab moving is not allowed, leave
        if (!(m_flags & wxAUI_NB_TAB_MOVE))
        {
            return;
        }
                
        wxPoint pt = dest_tabs->ScreenToClient(screen_pt);
        wxWindow* dest_location_tab;
        
        // this is an inner-tab drag/reposition
        if (dest_tabs->TabHitTest(pt.x, pt.y, &dest_location_tab))
        {
            int src_idx = evt.GetSelection();
            int dest_idx = dest_tabs->GetIdxFromWindow(dest_location_tab);
            
            // prevent jumpy drag
            if ((src_idx == dest_idx) || dest_idx == -1 ||
                (src_idx > dest_idx && m_last_drag_x <= pt.x) ||
                (src_idx < dest_idx && m_last_drag_x >= pt.x))
            {
                m_last_drag_x = pt.x;
                return;
            }


            wxWindow* src_tab = dest_tabs->GetWindowFromIdx(src_idx);
            dest_tabs->MovePage(src_tab, dest_idx);
            dest_tabs->SetActivePage((size_t)dest_idx);
            dest_tabs->DoShowHide();
            dest_tabs->Refresh();
            m_last_drag_x = pt.x;

        }
        
        return;
    }


    // if tab moving is not allowed, leave
    if (!(m_flags & wxAUI_NB_TAB_SPLIT))
    {
        return;
    }


    if (src_tabs)
    {
        src_tabs->SetCursor(wxCursor(wxCURSOR_SIZING));
    }
    
    
    if (dest_tabs)
    {
        wxRect hint_rect = dest_tabs->GetRect();
        ClientToScreen(&hint_rect.x, &hint_rect.y);
        m_mgr.ShowHint(hint_rect);
    }
     else
    {
        m_mgr.DrawHintRect(m_dummy_wnd, client_pt, zero);
    }
}



void wxAuiNotebook::OnTabEndDrag(wxCommandEvent& command_evt)
{
    wxAuiNotebookEvent& evt = (wxAuiNotebookEvent&)command_evt;

    m_mgr.HideHint();

    // if tab moving is not allowed, leave
    if (!(m_flags & wxAUI_NB_TAB_SPLIT))
    {
        return;
    }
    
    // set cursor back to an arrow
    wxAuiTabCtrl* src_tabs = (wxAuiTabCtrl*)evt.GetEventObject();
    if (src_tabs)
    {
        src_tabs->SetCursor(wxCursor(wxCURSOR_ARROW));
    }
    
    // get the mouse position, which will be used to determine the drop point
    wxPoint mouse_screen_pt = ::wxGetMousePosition();
    wxPoint mouse_client_pt = ScreenToClient(mouse_screen_pt);


    // the src tab control is the control that fired this event
    wxAuiTabCtrl* dest_tabs = NULL;


    // If the pointer is in an existing tab frame, do a tab insert
    wxWindow* hit_wnd = ::wxFindWindowAtPoint(mouse_screen_pt);
    wxTabFrame* tab_frame = (wxTabFrame*)GetTabFrameFromTabCtrl(hit_wnd);
    int insert_idx = -1;
    if (tab_frame)
    {
        dest_tabs = tab_frame->m_tabs;

        if (dest_tabs == src_tabs)
            return;
        
            
        wxPoint pt = dest_tabs->ScreenToClient(mouse_screen_pt);
        wxWindow* target = NULL;
        dest_tabs->TabHitTest(pt.x, pt.y, &target);
        if (target)
        {
            insert_idx = dest_tabs->GetIdxFromWindow(target);
        }
    }
     else
    {
        // If there is no tabframe at all, create one
        wxTabFrame* new_tabs = new wxTabFrame;
        new_tabs->SetTabCtrlHeight(m_tab_ctrl_height);
        new_tabs->m_tabs = new wxAuiTabCtrl(this,
                                            m_tab_id_counter++,
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            wxNO_BORDER);
        new_tabs->m_tabs->SetFlags(m_flags);

        m_mgr.AddPane(new_tabs,
                      wxAuiPaneInfo().Bottom().CaptionVisible(false),
                      mouse_client_pt);
        m_mgr.Update();
        dest_tabs = new_tabs->m_tabs;
    }



    // remove the page from the source tabs
    wxAuiNotebookPage page_info = src_tabs->GetPage(evt.GetSelection());
    page_info.active = false;
    src_tabs->RemovePage(page_info.window);
    if (src_tabs->GetPageCount() > 0)
    {
        src_tabs->SetActivePage((size_t)0);
        src_tabs->DoShowHide();
        src_tabs->Refresh();
    }



    // add the page to the destination tabs
    if (insert_idx == -1)
        insert_idx = dest_tabs->GetPageCount();
    dest_tabs->InsertPage(page_info.window, page_info, insert_idx);

    if (src_tabs->GetPageCount() == 0)
    {
        RemoveEmptyTabFrames();
    }

    DoSizing();
    dest_tabs->DoShowHide();
    dest_tabs->Refresh();

    SetSelection(m_tabs.GetIdxFromWindow(page_info.window));
}



wxAuiTabCtrl* wxAuiNotebook::GetTabCtrlFromPoint(const wxPoint& pt)
{
    // if we've just removed the last tab from the source
    // tab set, the remove the tab control completely
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    size_t i, pane_count = all_panes.GetCount();
    for (i = 0; i < pane_count; ++i)
    {
        if (all_panes.Item(i).name == wxT("dummy"))
            continue;

        wxTabFrame* tabframe = (wxTabFrame*)all_panes.Item(i).window;
        if (tabframe->m_tab_rect.Contains(pt))
            return tabframe->m_tabs;
    }

    return NULL;
}

wxWindow* wxAuiNotebook::GetTabFrameFromTabCtrl(wxWindow* tab_ctrl)
{
    // if we've just removed the last tab from the source
    // tab set, the remove the tab control completely
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    size_t i, pane_count = all_panes.GetCount();
    for (i = 0; i < pane_count; ++i)
    {
        if (all_panes.Item(i).name == wxT("dummy"))
            continue;

        wxTabFrame* tabframe = (wxTabFrame*)all_panes.Item(i).window;
        if (tabframe->m_tabs == tab_ctrl)
        {
            return tabframe;
        }
    }

    return NULL;
}

void wxAuiNotebook::RemoveEmptyTabFrames()
{
    // if we've just removed the last tab from the source
    // tab set, the remove the tab control completely
    wxAuiPaneInfoArray all_panes = m_mgr.GetAllPanes();
    size_t i, pane_count = all_panes.GetCount();
    for (i = 0; i < pane_count; ++i)
    {
        if (all_panes.Item(i).name == wxT("dummy"))
            continue;

        wxTabFrame* tab_frame = (wxTabFrame*)all_panes.Item(i).window;
        if (tab_frame->m_tabs->GetPageCount() == 0)
        {
            m_mgr.DetachPane(tab_frame);

            // use pending delete because sometimes during
            // window closing, refreshs are pending
            if (!wxPendingDelete.Member(tab_frame->m_tabs))
                wxPendingDelete.Append(tab_frame->m_tabs);
            //tab_frame->m_tabs->Destroy();

            delete tab_frame;
        }
    }


    // check to see if there is still a center pane;
    // if there isn't, make a frame the center pane
    wxAuiPaneInfoArray panes = m_mgr.GetAllPanes();
    pane_count = panes.GetCount();
    wxWindow* first_good = NULL;
    bool center_found = false;
    for (i = 0; i < pane_count; ++i)
    {
        if (panes.Item(i).name == wxT("dummy"))
            continue;
        if (panes.Item(i).dock_direction == wxAUI_DOCK_CENTRE)
            center_found = true;
        if (!first_good)
            first_good = panes.Item(i).window;
    }

    if (!center_found && first_good)
    {
        m_mgr.GetPane(first_good).Centre();
    }

    m_mgr.Update();
}

void wxAuiNotebook::OnChildFocus(wxChildFocusEvent& evt)
{
    int idx = m_tabs.GetIdxFromWindow(evt.GetWindow());
    if (idx != -1 && idx != m_curpage)
    {
        SetSelection(idx);
    }
}


void wxAuiNotebook::OnTabButton(wxCommandEvent& command_evt)
{
    wxAuiNotebookEvent& evt = (wxAuiNotebookEvent&)command_evt;
    wxAuiTabCtrl* tabs = (wxAuiTabCtrl*)evt.GetEventObject();

    int button_id = evt.GetInt();

    if (button_id == wxAUI_BUTTON_CLOSE)
    {
        int selection = tabs->GetActivePage();

        if (selection != -1)
        {
            wxWindow* close_wnd = tabs->GetWindowFromIdx(selection);

            if (close_wnd->IsKindOf(CLASSINFO(wxAuiMDIChildFrame)))
            {
                close_wnd->Close();
            }
             else
            {
                int main_idx = m_tabs.GetIdxFromWindow(close_wnd);
                DeletePage(main_idx);
            }
        }
    }
}




#endif // wxUSE_AUI

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/wince/tbarwce.h
// Purpose:     Windows CE wxToolBar class
// Author:      Julian Smart
// Modified by:
// Created:     2003-07-12
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BARWCE_H_
#define _WX_BARWCE_H_

#ifdef __GNUG__
    #pragma interface "tbarwce.h"
#endif

#if wxUSE_TOOLBAR

#include "wx/dynarray.h"

class WXDLLEXPORT wxToolBar : public wxToolBarBase
{
public:
    // ctors and dtor
    wxToolBar() { Init(); }

    wxToolBar(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxNO_BORDER | wxTB_HORIZONTAL,
                const wxString& name = wxToolBarNameStr,
                wxMenuBar* menuBar = NULL)
    {
        Init();

        Create(parent, id, pos, size, style, name, menuBar);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxNO_BORDER | wxTB_HORIZONTAL,
                const wxString& name = wxToolBarNameStr,
                wxMenuBar* menuBar = NULL);

    virtual ~wxToolBar();

    // override/implement base class virtuals
    virtual wxToolBarToolBase *FindToolForPosition(wxCoord x, wxCoord y) const;

    virtual bool Realize();

    virtual void SetToolBitmapSize(const wxSize& size);
    virtual wxSize GetToolSize() const;

    virtual void SetRows(int nRows);

    // implementation only from now on
    // -------------------------------

    virtual void SetWindowStyleFlag(long style);

    virtual bool MSWCommand(WXUINT param, WXWORD id);
    virtual bool MSWOnNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result);

    void OnMouseEvent(wxMouseEvent& event);
    void OnSysColourChanged(wxSysColourChangedEvent& event);

    void SetFocus() {}

    static WXHBITMAP MapBitmap(WXHBITMAP bitmap, int width, int height);

    // Return HMENU for the menu associated with the commandbar
    WXHMENU GetHMenu();

    // Set the wxMenuBar associated with this commandbar
    void SetMenuBar(wxMenuBar* menuBar) { m_menuBar = menuBar; }

    // Returns the wxMenuBar associated with this commandbar
    wxMenuBar* GetMenuBar() const { return m_menuBar; }

protected:
    // common part of all ctors
    void Init();

    // create the native toolbar control
    bool MSWCreateToolbar(const wxPoint& pos, const wxSize& size, wxMenuBar* menuBar);

    // recreate the control completely
    void Recreate();

    // implement base class pure virtuals
    virtual bool DoInsertTool(size_t pos, wxToolBarToolBase *tool);
    virtual bool DoDeleteTool(size_t pos, wxToolBarToolBase *tool);

    virtual void DoEnableTool(wxToolBarToolBase *tool, bool enable);
    virtual void DoToggleTool(wxToolBarToolBase *tool, bool toggle);
    virtual void DoSetToggle(wxToolBarToolBase *tool, bool toggle);

    virtual wxToolBarToolBase *CreateTool(int id,
                                          const wxString& label,
                                          const wxBitmap& bmpNormal,
                                          const wxBitmap& bmpDisabled,
                                          wxItemKind kind,
                                          wxObject *clientData,
                                          const wxString& shortHelp,
                                          const wxString& longHelp);
    virtual wxToolBarToolBase *CreateTool(wxControl *control);

    // override WndProc mainly to process WM_SIZE
    virtual long MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

    // return the appropriate size and flags for the toolbar control
    virtual wxSize DoGetBestSize() const;
    virtual WXDWORD MSWGetStyle(long style, WXDWORD *exstyle) const;

    // handlers for various events
    void HandleMouseMove(WXWPARAM wParam, WXLPARAM lParam);

    // should be called whenever the toolbar size changes
    void UpdateSize();

    // the big bitmap containing all bitmaps of the toolbar buttons
    WXHBITMAP m_hBitmap;

    // the total number of toolbar elements
    size_t m_nButtons;

    // the tool the cursor is in
    wxToolBarToolBase *m_pInTool;

    // The menubar associated with this toolbar
    wxMenuBar*  m_menuBar;

private:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxToolBar)
    DECLARE_NO_COPY_CLASS(wxToolBar)
};

#endif // wxUSE_TOOLBAR

#endif
    // _WX_BARWCE_H_

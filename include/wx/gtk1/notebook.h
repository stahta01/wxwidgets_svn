/////////////////////////////////////////////////////////////////////////////
// Name:        notebook.h
// Purpose:     wxNotebook class
// Author:      Robert Roebling
// Modified by:
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Robert Roebling
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKNOTEBOOKH__
#define __GTKNOTEBOOKH__

#ifdef __GNUG__
#pragma interface
#endif

//-----------------------------------------------------------------------------
// internal class
//-----------------------------------------------------------------------------

class wxGtkNotebookPage;

//-----------------------------------------------------------------------------
// wxNotebook
//-----------------------------------------------------------------------------

class wxNotebook : public wxNotebookBase
{
public:
      // default for dynamic class
    wxNotebook();
      // the same arguments as for wxControl
    wxNotebook(wxWindow *parent,
             wxWindowID id,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = 0,
             const wxString& name = "notebook");
      // Create() function
    bool Create(wxWindow *parent,
              wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = 0,
              const wxString& name = "notebook");
      // dtor
    ~wxNotebook();

  // accessors
  // ---------

    // set the currently selected page, return the index of the previously
    // selected one (or -1 on error)
    // NB: this function will _not_ generate wxEVT_NOTEBOOK_PAGE_xxx events
  int SetSelection(int nPage);
    // get the currently selected page
  int GetSelection() const;

    // set/get the title of a page
  bool SetPageText(int nPage, const wxString& strText);
  wxString GetPageText(int nPage) const;

  // image list stuff: each page may have an image associated with it. All
  // the images belong to an image list, so you have to
  // 1) create an image list
  // 2) associate it with the notebook
  // 3) set for each page it's image
    // associate image list with a control
  void SetImageList(wxImageList* imageList);
  void AssignImageList(wxImageList* imageList);

    // sets/returns item's image index in the current image list
  int  GetPageImage(int nPage) const;
  bool SetPageImage(int nPage, int nImage);

  // control the appearance of the notebook pages
    // set the size (the same for all pages)
  void SetPageSize(const wxSize& size);
    // set the padding between tabs (in pixels)
  void SetPadding(const wxSize& padding);
    // sets the size of the tabs (assumes all tabs are the same size)
  void SetTabSize(const wxSize& sz);

  // operations
  // ----------
    // remove one page from the notebook
  bool DeletePage(int nPage);
    // remove all pages
  bool DeleteAllPages();

    // adds a new page to the notebook (it will be deleted ny the notebook,
    // don't delete it yourself). If bSelect, this page becomes active.
    bool AddPage( wxNotebookPage *win,
                  const wxString& strText,
                  bool select = FALSE,
                  int imageId = -1 );
    // the same as AddPage(), but adds it at the specified position
    bool InsertPage( int position,
                     wxNotebookPage *win,
                     const wxString& strText,
                     bool bSelect = FALSE,
                     int imageId = -1 );

    // handler for tab navigation
    // --------------------------
    void OnNavigationKey(wxNavigationKeyEvent& event);

    // implementation
    // --------------

#if wxUSE_CONSTRAINTS
    void SetConstraintSizes(bool recurse);
    bool DoPhase(int phase);
#endif

    void ApplyWidgetStyle();

    // report if window belongs to notebook
    bool IsOwnGtkWindow( GdkWindow *window );

    // common part of all ctors
    void Init();

    // helper function
    wxGtkNotebookPage* GetNotebookPage(int page) const;

    bool            m_ownsImageList;
    wxList          m_pages;

    // for reasons explained in gtk/notebook.cpp we store the current
    // selection internally instead of querying the notebook for it
    int             m_selection;

protected:
    // remove one page from the notebook but do not destroy it
    virtual wxNotebookPage *DoRemovePage(int nPage);

private:
    DECLARE_DYNAMIC_CLASS(wxNotebook)
    DECLARE_EVENT_TABLE()
};

#endif
    // __GTKNOTEBOOKH__

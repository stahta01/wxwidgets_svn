/////////////////////////////////////////////////////////////////////////////
// Name:        listbox.h
// Purpose:     wxListBox class
// Author:      David Webster
// Modified by:
// Created:     10/09/99
// RCS-ID:      $Id$
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_LISTBOX_H_
#define _WX_LISTBOX_H_

// ----------------------------------------------------------------------------
// simple types
// ----------------------------------------------------------------------------

#if wxUSE_OWNER_DRAWN
  class WXDLLEXPORT wxOwnerDrawn;

  // define the array of list box items
  #include  <wx/dynarray.h>

  WX_DEFINE_EXPORTED_ARRAY(wxOwnerDrawn *, wxListBoxItemsArray);
#endif // wxUSE_OWNER_DRAWN

// forward decl for GetSelections()
class wxArrayInt;

// ----------------------------------------------------------------------------
// List box control
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxListBox : public wxListBoxBase
{
public:
    // ctors and such
    wxListBox();
    wxListBox(wxWindow *parent, wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            int n = 0, const wxString choices[] = NULL,
            long style = 0,
#if wxUSE_VALIDATORS
#  if defined(__VISAGECPP__)
            const wxValidator* validator = wxDefaultValidator,
#  else
            const wxValidator& validator = wxDefaultValidator,
#  endif
#endif
            const wxString& name = wxListBoxNameStr)
    {
        Create(parent, id, pos, size, n, choices, style, validator, name);
    }

    bool Create(wxWindow *parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                int n = 0, const wxString choices[] = NULL,
                long style = 0,
#if wxUSE_VALIDATORS
#  if defined(__VISAGECPP__)
                const wxValidator* validator = wxDefaultValidator,
#  else
                const wxValidator& validator = wxDefaultValidator,
#  endif
#endif
                const wxString& name = wxListBoxNameStr);

    virtual ~wxListBox();

    // implement base class pure virtuals
    virtual void Clear();
    virtual void Delete(int n);

    virtual int GetCount() const;
    virtual wxString GetString(int n) const;
    virtual void SetString(int n, const wxString& s);
    virtual int FindString(const wxString& s) const;

    virtual bool IsSelected(int n) const;
    virtual void SetSelection(int n, bool select = TRUE);
    virtual int GetSelection() const;
    virtual int GetSelections(wxArrayInt& aSelections) const;

    virtual int DoAppend(const wxString& item);
    virtual void DoInsertItems(const wxArrayString& items, int pos);
    virtual void DoSetItems(const wxArrayString& items, void **clientData);

    virtual void DoSetFirstItem(int n);

    virtual void DoSetItemClientData(int n, void* clientData);
    virtual void* DoGetItemClientData(int n) const;
    virtual void DoSetItemClientObject(int n, wxClientData* clientData);
    virtual wxClientData* DoGetItemClientObject(int n) const;

    // wxCheckListBox support
#if wxUSE_OWNER_DRAWN
    bool OS2OnMeasure(WXMEASUREITEMSTRUCT *item);
    bool OS2OnDraw(WXDRAWITEMSTRUCT *item);

    // plug-in for derived classes
    virtual wxOwnerDrawn *CreateItem(size_t n);

    // allows to get the item and use SetXXX functions to set it's appearance
    wxOwnerDrawn *GetItem(size_t n) const { return m_aItems[n]; }

    // get the index of the given item
    int GetItemIndex(wxOwnerDrawn *item) const { return m_aItems.Index(item); }
#endif // wxUSE_OWNER_DRAWN

    // Windows-specific code to set the horizontal extent of the listbox, if
    // necessary. If s is non-NULL, it's used to calculate the horizontal
    // extent. Otherwise, all strings are used.
    virtual void SetHorizontalExtent(const wxString& s = wxEmptyString);

    // Windows callbacks
    virtual WXHBRUSH OnCtlColor(WXHDC pDC, WXHWND pWnd, WXUINT nCtlColor,
                                WXUINT message,
                                WXWPARAM wParam, WXLPARAM lParam);

    bool OS2Command(WXUINT param, WXWORD id);

    virtual void SetupColours();

protected:
    // do we have multiple selections?
    bool HasMultipleSelection() const;

    int m_noItems;
    int m_selected;

    virtual wxSize DoGetBestSize();

#if wxUSE_OWNER_DRAWN
    // control items
    wxListBoxItemsArray m_aItems;
#endif

private:
#if wxUSE_WX_RESOURCES
#  if wxUSE_OWNER_DRAWN
    virtual wxControl *CreateItem(const wxItemResource* childResource,
                                  const wxItemResource* parentResource,
                                  const wxResourceTable *table = (const wxResourceTable *) NULL)
    { return(wxWindowBase::CreateItem(childResource, parentResource, table)); }
#  endif
#endif
    DECLARE_DYNAMIC_CLASS(wxListBox)
};

#endif
    // _WX_LISTBOX_H_

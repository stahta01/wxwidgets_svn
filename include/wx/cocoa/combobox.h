/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/combobox.h
// Purpose:     wxComboBox class
// Author:      David Elliott
// Modified by:
// Created:     2003/07/14
// RCS-ID:      $Id$
// Copyright:   (c) 2003 David Elliott
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_COMBOBOX_H__
#define __WX_COCOA_COMBOBOX_H__

//#include "wx/cocoa/NSTableView.h"

// ========================================================================
// wxComboBox
// ========================================================================
class WXDLLEXPORT wxComboBox: public wxTextCtrl, public wxComboBoxBase //, protected wxCocoaNSTableView
{
    DECLARE_DYNAMIC_CLASS(wxComboBox)
    DECLARE_EVENT_TABLE()
//    WX_DECLARE_COCOA_OWNER(NSComboBox,NSTextField,NSView)
// ------------------------------------------------------------------------
// initialization
// ------------------------------------------------------------------------
public:
    wxComboBox() { }
    wxComboBox(wxWindow *parent, wxWindowID winid,
            const wxString& value = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            int n = 0, const wxString choices[] = NULL,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxComboBoxNameStr)
    {
        Create(parent, winid, value, pos, size, n, choices, style, validator, name);
    }
    wxComboBox(wxWindow *parent, wxWindowID winid,
            const wxString& value,
            const wxPoint& pos,
            const wxSize& size,
            const wxArrayString& choices,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxComboBoxNameStr)
    {
        Create(parent, winid, value, pos, size, choices, style,
               validator, name);
    }

    bool Create(wxWindow *parent, wxWindowID winid,
            const wxString& value = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            int n = 0, const wxString choices[] = NULL,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxComboBoxNameStr);
    bool Create(wxWindow *parent, wxWindowID winid,
            const wxString& value,
            const wxPoint& pos,
            const wxSize& size,
            const wxArrayString& choices,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxComboBoxNameStr);
    virtual ~wxComboBox();

// ------------------------------------------------------------------------
// Cocoa callbacks
// ------------------------------------------------------------------------
protected:
// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
public:
    // wxCombobox methods
    virtual void SetSelection(int);
    // Overlapping methods
    virtual wxString GetStringSelection();
    // wxItemContainer
    virtual void Clear();
    virtual void Delete(int);
    virtual int GetCount() const;
    virtual wxString GetString(int) const;
    virtual void SetString(int, const wxString&);
    virtual int FindString(const wxString&) const;
    virtual int GetSelection() const;
    virtual int DoAppend(const wxString&);
    virtual int DoInsert(const wxString&, int);
    virtual void DoSetItemClientData(int, void*);
    virtual void* DoGetItemClientData(int) const;
    virtual void DoSetItemClientObject(int, wxClientData*);
    virtual wxClientData* DoGetItemClientObject(int) const;
    // wxComboBoxBase pure virtuals
    virtual wxString GetValue() const
    {   return wxTextCtrl::GetValue(); }
    virtual void SetValue(const wxString& value)
    {   return wxTextCtrl::SetValue(value); }
    virtual void Cut() { wxTextCtrl::Cut(); }
    virtual void Copy() { wxTextCtrl::Copy(); }
    virtual void Paste() { wxTextCtrl::Paste(); }
    virtual void SetInsertionPoint(long pos)
    {   wxTextCtrl::SetInsertionPoint(pos); }
    virtual void SetInsertionPointEnd()
    {   wxTextCtrl::SetInsertionPointEnd(); }
    virtual long GetInsertionPoint() const
    {   return wxTextCtrl::GetInsertionPoint(); }
    virtual wxTextPos GetLastPosition() const
    {   return wxTextCtrl::GetLastPosition(); }
    virtual void Replace(long from, long to, const wxString& value)
    {   wxTextCtrl::Replace(from,to,value); }
    virtual void SetSelection(long from, long to)
    {   wxTextCtrl::SetSelection(from,to); }
    virtual void SetEditable(bool editable)
    {   wxTextCtrl::SetEditable(editable); }
    virtual bool IsEditable() const
    {   return !HasFlag(wxCB_READONLY); }
    virtual void Undo()
    {   wxTextCtrl::Undo(); }
    virtual void Redo()
    {   wxTextCtrl::Redo(); }
    virtual void SelectAll()
    {   wxTextCtrl::SelectAll(); }
    virtual bool CanCopy() const
    {   return wxTextCtrl::CanCopy(); }
    virtual bool CanCut() const
    {   return wxTextCtrl::CanCut(); }
    virtual bool CanPaste() const
    {   return wxTextCtrl::CanPaste(); }
    virtual bool CanUndo() const
    {   return wxTextCtrl::CanUndo(); }
    virtual bool CanRedo() const
    {   return wxTextCtrl::CanRedo(); }
};

#endif // __WX_COCOA_COMBOBOX_H__

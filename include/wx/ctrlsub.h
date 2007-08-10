/////////////////////////////////////////////////////////////////////////////
// Name:        wx/ctrlsub.h (read: "wxConTRoL with SUBitems")
// Purpose:     wxControlWithItems interface
// Author:      Vadim Zeitlin
// Modified by:
// Created:     22.10.99
// RCS-ID:      $Id$
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CTRLSUB_H_BASE_
#define _WX_CTRLSUB_H_BASE_

#include "wx/defs.h"

#if wxUSE_CONTROLS

#include "wx/arrstr.h"
#include "wx/control.h"      // base class

// ----------------------------------------------------------------------------
// wxItemContainer defines an interface which is implemented by all controls
// which have string subitems each of which may be selected.
//
// It is decomposed in wxItemContainerImmutable which omits all methods
// adding/removing items and is used by wxRadioBox and wxItemContainer itself.
//
// Examples: wxListBox, wxCheckListBox, wxChoice and wxComboBox (which
// implements an extended interface deriving from this one)
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxItemContainerImmutable
{
public:
    wxItemContainerImmutable() { }
    virtual ~wxItemContainerImmutable();

    // accessing strings
    // -----------------

    virtual unsigned int GetCount() const = 0;
    bool IsEmpty() const { return GetCount() == 0; }

    virtual wxString GetString(unsigned int n) const = 0;
    wxArrayString GetStrings() const;
    virtual void SetString(unsigned int n, const wxString& s) = 0;

    // finding string natively is either case sensitive or insensitive
    // but never both so fall back to this base version for not
    // supported search type
    virtual int FindString(const wxString& s, bool bCase = false) const
    {
        unsigned int count = GetCount();

        for ( unsigned int i = 0; i < count ; ++i )
        {
            if (GetString(i).IsSameAs( s , bCase ))
                return (int)i;
        }

        return wxNOT_FOUND;
    }


    // selection
    // ---------

    virtual void SetSelection(int n) = 0;
    virtual int GetSelection() const = 0;

    // set selection to the specified string, return false if not found
    bool SetStringSelection(const wxString& s);

    // return the selected string or empty string if none
    wxString GetStringSelection() const;

    // this is the same as SetSelection( for single-selection controls but
    // reads better for multi-selection ones
    void Select(int n) { SetSelection(n); }


protected:
    // check that the index is valid
    bool IsValid(unsigned int n) const { return n < GetCount(); }
    bool IsValidInsert(unsigned int n) const { return n <= GetCount(); }
};

// ----------------------------------------------------------------------------
// wxItemContainer extends wxItemContainerImmutable interface with methods
// for adding/removing items.
//
// Classes deriving from this one must override DoInsertItems() to implement
// adding items to the control. This can often be implemented more efficiently
// than simply looping over the elements and inserting them but if this is not
// the case, the generic DoInsertItemsInLoop can be used in implementation, but
// in this case DoInsertItem() needs to be overridden.
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxItemContainer : public wxItemContainerImmutable
{
private:
    // AppendItems() and InsertItems() helpers just call DoAppend/InsertItems()
    // after doing some checks
    //
    // NB: they're defined here so that they're inlined when used in public part
    int AppendItems(const wxArrayStringsAdapter& items,
                    void **clientData,
                    wxClientDataType type)
    {
        if ( items.IsEmpty() )
            return wxNOT_FOUND;

        return DoAppendItems(items, clientData, type);
    }

    int AppendItems(const wxArrayStringsAdapter& items)
    {
        return AppendItems(items, NULL, wxClientData_None);
    }

    int AppendItems(const wxArrayStringsAdapter& items, void **clientData)
    {
        wxASSERT_MSG( m_clientDataItemsType != wxClientData_Object,
                      _T("can't mix different types of client data") );

        return AppendItems(items, clientData, wxClientData_Void);
    }

    int AppendItems(const wxArrayStringsAdapter& items,
                    wxClientData **clientData)
    {
        wxASSERT_MSG( m_clientDataItemsType != wxClientData_Void,
                      _T("can't mix different types of client data") );

        return AppendItems(items, wx_reinterpret_cast(void **, clientData),
                           wxClientData_Object);
    }

    int InsertItems(const wxArrayStringsAdapter& items,
                    unsigned int pos,
                    void **clientData,
                    wxClientDataType type)
    {
        wxASSERT_MSG( !IsSorted(), _T("can't insert items in sorted control") );

        wxCHECK_MSG( pos <= GetCount(), wxNOT_FOUND,
                     _T("position out of range") );

        // not all derived classes handle empty arrays correctly in
        // DoInsertItems() and besides it really doesn't make much sense to do
        // this (for append it could correspond to creating an initially empty
        // control but why would anybody need to insert 0 items?)
        wxCHECK_MSG( !items.IsEmpty(), wxNOT_FOUND,
                     _T("need something to insert") );

        return DoInsertItems(items, pos, clientData, type);
    }

    int InsertItems(const wxArrayStringsAdapter& items, unsigned int pos)
    {
        return InsertItems(items, pos, NULL, wxClientData_None);
    }

    int InsertItems(const wxArrayStringsAdapter& items,
                     unsigned int pos,
                     void **clientData)
    {
        wxASSERT_MSG( m_clientDataItemsType != wxClientData_Object,
                      _T("can't mix different types of client data") );

        return InsertItems(items, pos, clientData, wxClientData_Void);
    }

    int InsertItems(const wxArrayStringsAdapter& items,
                     unsigned int pos,
                     wxClientData **clientData)
    {
        wxASSERT_MSG( m_clientDataItemsType != wxClientData_Void,
                      _T("can't mix different types of client data") );

        return InsertItems(items, pos,
                           wx_reinterpret_cast(void **, clientData),
                           wxClientData_Object);
    }

public:
    wxItemContainer() { m_clientDataItemsType = wxClientData_None; }
    virtual ~wxItemContainer();

    // adding items
    // ------------

    // append single item, return its position in the control (which can be
    // different from the last one if the control is sorted)
    int Append(const wxString& item)
        { return AppendItems(item); }
    int Append(const wxString& item, void *clientData)
        { return AppendItems(item, &clientData); }
    int Append(const wxString& item, wxClientData *clientData)
        { return AppendItems(item, &clientData); }

    // append several items at once to the control, return the position of the
    // last item appended
    int Append(const wxArrayString& items)
        { return AppendItems(items); }
    int Append(const wxArrayString& items, void **clientData)
        { return AppendItems(items, clientData); }
    int Append(const wxArrayString& items, wxClientData **clientData)
        { return AppendItems(items, clientData); }
    int Append(unsigned int n, const wxString *items)
        { return AppendItems(wxArrayStringsAdapter(n, items)); }
    int Append(unsigned int n, const wxString *items, void **clientData)
        { return AppendItems(wxArrayStringsAdapter(n, items), clientData); }
    int Append(unsigned int n,
               const wxString *items,
               wxClientData **clientData)
        { return AppendItems(wxArrayStringsAdapter(n, items), clientData); }

    // only for RTTI needs (separate name)
    void AppendString(const wxString& item)
        { Append(item); }


    // inserting items: not for sorted controls!
    // -----------------------------------------

    // insert single item at the given position, return its effective position
    int Insert(const wxString& item, unsigned int pos)
        { return InsertItems(item, pos); }
    int Insert(const wxString& item, unsigned int pos, void *clientData)
        { return InsertItems(item, pos, &clientData); }
    int Insert(const wxString& item, unsigned int pos, wxClientData *clientData)
        { return InsertItems(item, pos, &clientData); }

    // insert several items at once into the control, return the index of the
    // last item inserted
    int Insert(const wxArrayString& items, unsigned int pos)
        { return InsertItems(items, pos); }
    int Insert(const wxArrayString& items, unsigned int pos, void **clientData)
        { return InsertItems(items, pos, clientData); }
    int Insert(const wxArrayString& items,
               unsigned int pos,
               wxClientData **clientData)
        { return InsertItems(items, pos, clientData); }
    int Insert(unsigned int n, const wxString *items, unsigned int pos)
        { return InsertItems(wxArrayStringsAdapter(n, items), pos); }
    int Insert(unsigned int n,
               const wxString *items,
               unsigned int pos,
               void **clientData)
        { return InsertItems(wxArrayStringsAdapter(n, items), pos, clientData); }
    int Insert(unsigned int n,
               const wxString *items,
               unsigned int pos,
               wxClientData **clientData)
        { return InsertItems(wxArrayStringsAdapter(n, items), pos, clientData); }


    // replacing items
    // ---------------

    void Set(const wxArrayString& items)
        { Clear(); Append(items); }
    void Set(const wxArrayString& items, void **clientData)
        { Clear(); Append(items, clientData); }
    void Set(const wxArrayString& items, wxClientData **clientData)
        { Clear(); Append(items, clientData); }
    void Set(unsigned int n, const wxString *items)
        { Clear(); Append(n, items); }
    void Set(unsigned int n, const wxString *items, void **clientData)
        { Clear(); Append(n, items, clientData); }
    void Set(unsigned int n, const wxString *items, wxClientData **clientData)
        { Clear(); Append(n, items, clientData); }

    // deleting items
    // --------------

    void Clear();
    void Delete(unsigned int pos);


    // client data stuff
    // -----------------

    void SetClientData(unsigned int n, void* clientData);
    void* GetClientData(unsigned int n) const;

    void SetClientObject(unsigned int n, wxClientData* clientData);
    wxClientData* GetClientObject(unsigned int n) const;

    bool HasClientData() const
        { return m_clientDataItemsType != wxClientData_None; }
    bool HasClientObjectData() const
        { return m_clientDataItemsType == wxClientData_Object; }
    bool HasClientUntypedData() const
        { return m_clientDataItemsType == wxClientData_Void; }


    // The control may maintain its items in a sorted order in which case
    // items are automatically inserted at the right position when they are
    // inserted or appended. Derived classes have to override this method if
    // they implement sorting, typically by returning HasFlag(wxXX_SORT)
    virtual bool IsSorted() const { return false; }

protected:
    // there is usually no need to override this method but you can do it if it
    // is more convenient to only do "real" insertions in DoInsertItems() and
    // to implement items appending here (in which case DoInsertItems() should
    // call this method if pos == GetCount() as it can still be called in this
    // case if public Insert() is called with such position)
    virtual int DoAppendItems(const wxArrayStringsAdapter& items,
                              void **clientData,
                              wxClientDataType type)
    {
        return DoInsertItems(items, GetCount(), clientData, type);
    }

    // this method must be implemented to insert the items into the control at
    // position pos which can be GetCount() meaning that the items should be
    // appended; for the sorted controls the position can be ignored
    //
    // the derived classes typically use AssignNewItemClientData() to
    // associate the data with the items as they're being inserted
    //
    // the method should return the index of the position the last item was
    // inserted into or wxNOT_FOUND if an error occurred
    virtual int DoInsertItems(const wxArrayStringsAdapter & items,
                              unsigned int pos,
                              void **clientData,
                              wxClientDataType type) = 0;

    // before the client data is set for the first time for the control which
    // hadn't had it before, DoInitItemClientData() is called which gives the
    // derived class the possibility to initialize its client data storage only
    // when client data is really used
    virtual void DoInitItemClientData() { }
    virtual void DoSetItemClientData(unsigned int n, void *clientData) = 0;
    virtual void *DoGetItemClientData(unsigned int n) const = 0;

    virtual void DoClear() = 0;
    virtual void DoDeleteOneItem(unsigned int pos) = 0;


    // methods useful for the derived classes which don't have any better way
    // of adding multiple items to the control than doing it one by one: such
    // classes should call DoInsertItemsInLoop() from their DoInsert() and
    // override DoInsertOneItem() to perform the real insertion
    virtual int DoInsertOneItem(const wxString& item, unsigned int pos);
    int DoInsertItemsInLoop(const wxArrayStringsAdapter& items,
                            unsigned int pos,
                            void **clientData,
                            wxClientDataType type);


    // helper for DoInsertItems(): n is the index into clientData, pos is the
    // position of the item in the control
    void AssignNewItemClientData(unsigned int pos,
                                 void **clientData,
                                 unsigned int n,
                                 wxClientDataType type);

    // free the client object associated with the item at given position and
    // set it to NULL (must only be called if HasClientObjectData())
    void ResetItemClientObject(unsigned int n);


    // the type of the client data for the items
    wxClientDataType m_clientDataItemsType;
};

// this macro must (unfortunately) be used in any class deriving from both
// wxItemContainer and wxControl because otherwise there is ambiguity when
// calling GetClientXXX() functions -- the compiler can't choose between the
// two versions
#define wxCONTROL_ITEMCONTAINER_CLIENTDATAOBJECT_RECAST                    \
    void SetClientData(void *data)                                         \
        { wxEvtHandler::SetClientData(data); }                             \
    void *GetClientData() const                                            \
        { return wxEvtHandler::GetClientData(); }                          \
    void SetClientObject(wxClientData *data)                               \
        { wxEvtHandler::SetClientObject(data); }                           \
    wxClientData *GetClientObject() const                                  \
        { return wxEvtHandler::GetClientObject(); }                        \
    void SetClientData(unsigned int n, void* clientData)                   \
        { wxItemContainer::SetClientData(n, clientData); }                 \
    void* GetClientData(unsigned int n) const                              \
        { return wxItemContainer::GetClientData(n); }                      \
    void SetClientObject(unsigned int n, wxClientData* clientData)         \
        { wxItemContainer::SetClientObject(n, clientData); }               \
    wxClientData* GetClientObject(unsigned int n) const                    \
        { return wxItemContainer::GetClientObject(n); }

class WXDLLEXPORT wxControlWithItemsBase : public wxControl,
                                           public wxItemContainer
{
public:
    wxControlWithItemsBase() { }

    // we have to redefine these functions here to avoid ambiguities in classes
    // deriving from us which would arise otherwise because both base classses
    // have the methods with the same names - hopefully, a smart compiler can
    // optimize away these simple inline wrappers so we don't suffer much from
    // this
    wxCONTROL_ITEMCONTAINER_CLIENTDATAOBJECT_RECAST

    // usually the controls like list/combo boxes have their own background
    // colour
    virtual bool ShouldInheritColours() const { return false; }

protected:
    // fill in the client object or data field of the event as appropriate
    //
    // calls InitCommandEvent() and, if n != wxNOT_FOUND, also sets the per
    // item client data
    void InitCommandEventWithItems(wxCommandEvent& event, int n);

private:
    DECLARE_NO_COPY_CLASS(wxControlWithItemsBase)
};

// define the platform-specific wxControlWithItems class
#if defined(__WXMSW__)
    #include "wx/msw/ctrlsub.h"
#elif defined(__WXMOTIF__)
    #include "wx/motif/ctrlsub.h"
#else
    class WXDLLEXPORT wxControlWithItems : public wxControlWithItemsBase
    {
    public:
        wxControlWithItems() { }

    private:
        DECLARE_ABSTRACT_CLASS(wxControlWithItems)
        DECLARE_NO_COPY_CLASS(wxControlWithItems)
    };
#endif

#endif // wxUSE_CONTROLS

#endif // _WX_CTRLSUB_H_BASE_

/////////////////////////////////////////////////////////////////////////////
// Name:        _control.i
// Purpose:     SWIG interface defs for wxControl and other base classes
//
// Author:      Robin Dunn
//
// Created:     10-June-1998
// RCS-ID:      $Id$
// Copyright:   (c) 2003 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// Not a %module


//---------------------------------------------------------------------------

MAKE_CONST_WXSTRING(ControlNameStr);

//---------------------------------------------------------------------------
%newgroup;


DocStr(wxControl,
"This is the base class for a control or 'widget'.

A control is generally a small window which processes user input and/or
displays one or more item of data.");

class wxControl : public wxWindow
{
public:
    %pythonAppend wxControl         "self._setOORInfo(self)"
    %pythonAppend wxControl()       ""

    DocCtorStr(
        wxControl(wxWindow *parent,
              wxWindowID id,
              const wxPoint& pos=wxDefaultPosition,
              const wxSize& size=wxDefaultSize,
              long style=0,
              const wxValidator& validator=wxDefaultValidator,
                  const wxString& name=wxPyControlNameStr),
        "Create a Control.  Normally you should only call this from a\n"
        "subclass' __init__ as a plain old wx.Control is not very useful.");

    DocCtorStrName(
        wxControl(),
        "Precreate a Control control for 2-phase creation",
        PreControl);

    DocDeclStr(
        bool , Create(wxWindow *parent,
                      wxWindowID id,
                      const wxPoint& pos=wxDefaultPosition,
                      const wxSize& size=wxDefaultSize,
                      long style=0,
                      const wxValidator& validator=wxDefaultValidator,
                      const wxString& name=wxPyControlNameStr),
        "Do the 2nd phase and create the GUI control.");
    

    DocDeclStr(
        void , Command(wxCommandEvent& event),
        "Simulates the effect of the user issuing a command to the\n"
        "item. See wx.CommandEvent.");
   
    DocDeclStr(
        wxString , GetLabel(),
        "Return a control's text.");
    
    DocDeclStr(
        void , SetLabel(const wxString& label),
        "Sets the item's text.");

    
    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);
};


//---------------------------------------------------------------------------
%newgroup;


DocStr(wxItemContainer,
"wx.ItemContainer defines an interface which is implemented by all
controls which have string subitems, each of which may be
selected, such as wx.ListBox, wx.CheckListBox, wx.Choice and
wx.ComboBox (which implements an extended interface deriving from
this one)

It defines the methods for accessing the control's items and
although each of the derived classes implements them differently,
they still all conform to the same interface.

The items in a wx.ItemContainer have (non empty) string labels
and, optionally, client data associated with them.
");

class wxItemContainer
{
public:
    // wxItemContainer() { m_clientDataItemsType = wxClientData_None; }  ** It's an ABC
    

    %extend {
        DocStr(Append,
               "Adds the item to the control, associating the given data with the\n"
               "item if not None.  The return value is the index of the newly\n"
               "added item which may be different from the last one if the\n"
               "control is sorted (e.g. has wx.LB_SORT or wx.CB_SORT style).");
        int Append(const wxString& item, PyObject* clientData=NULL) {
            if (clientData) {
                wxPyClientData* data = new wxPyClientData(clientData);
                return self->Append(item, data);
            } else
                return self->Append(item);
        }
    }
    
    DocDeclStrName(
        void , Append(const wxArrayString& strings),
        "Apend several items at once to the control.  Notice that calling\n"
        "this method may be much faster than appending the items one by\n"
        "one if you need to add a lot of items.",
        AppendItems);

    
    %extend {
        DocStr(Insert,
               "Insert an item into the control before the item at the pos index,\n"
               "optionally associating some data object with the item.");
        int Insert(const wxString& item, int pos, PyObject* clientData=NULL) {
            if (clientData) {
                wxPyClientData* data = new wxPyClientData(clientData);
                return self->Insert(item, pos, data);
            } else
                return self->Insert(item, pos);
        }
    }
    
    
    DocDeclStr(
        virtual void , Clear(),
        "Removes all items from the control.");
    
    DocDeclStr(
        virtual void , Delete(int n),
        "Deletes the item at the zero-based index 'n' from the control.\n"
        "Note that it is an error (signalled by a PyAssertionError\n"
        "exception if enabled) to remove an item with the index negative\n"
        "or greater or equal than the number of items in the control.");
    


    DocDeclStr(
        virtual int , GetCount() const,
        "Returns the number of items in the control.");
    
    DocDeclStr(
        bool , IsEmpty() const,
        "Returns True if the control is empty or False if it has some items.");
    
    DocDeclStr(
        virtual wxString , GetString(int n) const,
        "Returns the label of the item with the given index.");
    
    DocDeclStr(
        wxArrayString , GetStrings() const,
        "");
    
    DocDeclStr(
        virtual void , SetString(int n, const wxString& s),
        "Sets the label for the given item.");
    
    DocDeclStr(
        virtual int , FindString(const wxString& s) const,
        "Finds an item whose label matches the given string.  Returns the\n"
        "zero-based position of the item, or wx.NOT_FOUND if the string\n"
        "was not found.");
    

    
    DocDeclStr(
        virtual void , Select(int n),
        "Sets the item at index 'n' to be the selected item.");

    %pythoncode { SetSelection = Select }
    
    DocDeclStr(
        virtual int , GetSelection() const,
        "Returns the index of the selected item or wx.NOT_FOUND if no item is selected.");
    

    DocDeclStr(
        wxString , GetStringSelection() const,
        "Returns the label of the selected item or an empty string if no item is selected.");
    


    %extend {
        DocStr(GetClientData,
               "Returns the client data associated with the given item, (if any.)");
        PyObject* GetClientData(int n) {
            wxPyClientData* data = (wxPyClientData*)self->GetClientObject(n);
            if (data) {
                Py_INCREF(data->m_obj);
                return data->m_obj;
            } else {
                Py_INCREF(Py_None);
                return Py_None;
            }
        }

        DocStr(SetClientData,
               "Associate the given client data with the item at position n.");
        void SetClientData(int n, PyObject* clientData) {
            wxPyClientData* data = new wxPyClientData(clientData);
            self->SetClientObject(n, data);
        }
    }
      
};


//---------------------------------------------------------------------------
%newgroup;

DocStr(wxControlWithItems,
"wx.ControlWithItems combines the wx.ItemContainer class with the
wx.Control class, and is used for the base class of various
controls that have items.");

class wxControlWithItems : public wxControl, public wxItemContainer
{
public:
};

//---------------------------------------------------------------------------


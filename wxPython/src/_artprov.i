/////////////////////////////////////////////////////////////////////////////
// Name:        _artprov.i
// Purpose:     SWIG interface stuff for wxArtProvider
//
// Author:      Robin Dunn
//
// Created:     18-June-1999
// RCS-ID:      $Id$
// Copyright:   (c) 2003 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// Not a %module


//---------------------------------------------------------------------------
%newgroup

%{
#include <wx/artprov.h>
%}

//---------------------------------------------------------------------------


// Art clients
MAKE_CONST_WXSTRING(ART_TOOLBAR);
MAKE_CONST_WXSTRING(ART_MENU);
MAKE_CONST_WXSTRING(ART_FRAME_ICON);
MAKE_CONST_WXSTRING(ART_CMN_DIALOG);
MAKE_CONST_WXSTRING(ART_HELP_BROWSER);
MAKE_CONST_WXSTRING(ART_MESSAGE_BOX);
MAKE_CONST_WXSTRING(ART_BUTTON);
MAKE_CONST_WXSTRING(ART_OTHER);

// Art IDs
MAKE_CONST_WXSTRING(ART_ADD_BOOKMARK);
MAKE_CONST_WXSTRING(ART_DEL_BOOKMARK);
MAKE_CONST_WXSTRING(ART_HELP_SIDE_PANEL);
MAKE_CONST_WXSTRING(ART_HELP_SETTINGS);
MAKE_CONST_WXSTRING(ART_HELP_BOOK);
MAKE_CONST_WXSTRING(ART_HELP_FOLDER);
MAKE_CONST_WXSTRING(ART_HELP_PAGE);
MAKE_CONST_WXSTRING(ART_GO_BACK);
MAKE_CONST_WXSTRING(ART_GO_FORWARD);
MAKE_CONST_WXSTRING(ART_GO_UP);
MAKE_CONST_WXSTRING(ART_GO_DOWN);
MAKE_CONST_WXSTRING(ART_GO_TO_PARENT);
MAKE_CONST_WXSTRING(ART_GO_HOME);
MAKE_CONST_WXSTRING(ART_FILE_OPEN);
MAKE_CONST_WXSTRING(ART_PRINT);
MAKE_CONST_WXSTRING(ART_HELP);
MAKE_CONST_WXSTRING(ART_TIP);
MAKE_CONST_WXSTRING(ART_REPORT_VIEW);
MAKE_CONST_WXSTRING(ART_LIST_VIEW);
MAKE_CONST_WXSTRING(ART_NEW_DIR);
MAKE_CONST_WXSTRING(ART_FOLDER);
MAKE_CONST_WXSTRING(ART_GO_DIR_UP);
MAKE_CONST_WXSTRING(ART_EXECUTABLE_FILE);
MAKE_CONST_WXSTRING(ART_NORMAL_FILE);
MAKE_CONST_WXSTRING(ART_TICK_MARK);
MAKE_CONST_WXSTRING(ART_CROSS_MARK);
MAKE_CONST_WXSTRING(ART_ERROR);
MAKE_CONST_WXSTRING(ART_QUESTION);
MAKE_CONST_WXSTRING(ART_WARNING);
MAKE_CONST_WXSTRING(ART_INFORMATION);
MAKE_CONST_WXSTRING(ART_MISSING_IMAGE);

//---------------------------------------------------------------------------

%{  // Python aware wxArtProvider
class wxPyArtProvider : public wxArtProvider  {
public:

    virtual wxBitmap CreateBitmap(const wxArtID& id,
                                  const wxArtClient& client,
                                  const wxSize& size) {
        wxBitmap rval = wxNullBitmap;
        bool blocked = wxPyBeginBlockThreads();
        if ((wxPyCBH_findCallback(m_myInst, "CreateBitmap"))) {
            PyObject* so = wxPyConstructObject((void*)&size, wxT("wxSize"), 0);
            PyObject* ro;
            wxBitmap* ptr;
            PyObject* s1, *s2;
            s1 = wx2PyString(id);
            s2 = wx2PyString(client);
            ro = wxPyCBH_callCallbackObj(m_myInst, Py_BuildValue("(OOO)", s1, s2, so));
            Py_DECREF(so);
            Py_DECREF(s1);
            Py_DECREF(s2);
            if (ro) {
                if (wxPyConvertSwigPtr(ro, (void**)&ptr, wxT("wxBitmap")))
                    rval = *ptr;
                Py_DECREF(ro);
            }
        }
        wxPyEndBlockThreads(blocked);
        return rval;
    }

    PYPRIVATE;
};
%}

// The one for SWIG to see



DocStr(wxPyArtProvider,
"The wx.ArtProvider class is used to customize the look of wxWidgets
application. When wxWidgets needs to display an icon or a bitmap (e.g.
in the standard file dialog), it does not use hard-coded resource but
asks wx.ArtProvider for it instead. This way the users can plug in
their own wx.ArtProvider class and easily replace standard art with
his/her own version. It is easy thing to do: all that is needed is
to derive a class from wx.ArtProvider, override it's CreateBitmap
method and register the provider with wx.ArtProvider.PushProvider::

    class MyArtProvider(wx.ArtProvider):
        def __init__(self):
            wx.ArtProvider.__init__(self)

        def CreateBitmap(self, artid, client, size):
            ...
            return bmp
", "

Identifying art resources
-------------------------

Every bitmap is known to wx.ArtProvider under an unique ID that is
used when requesting a resource from it. The IDs can have one of the
following predefined values.  Additionally, any string recognized by
custom art providers registered using `PushProvider` may be used. When
running under GTK+ 2, GTK+ stock item IDs (e.g. 'gtk-cdrom') may be
used as well.


    * wx.ART_ADD_BOOKMARK
    * wx.ART_DEL_BOOKMARK
    * wx.ART_HELP_SIDE_PANEL
    * wx.ART_HELP_SETTINGS
    * wx.ART_HELP_BOOK
    * wx.ART_HELP_FOLDER
    * wx.ART_HELP_PAGE
    * wx.ART_GO_BACK
    * wx.ART_GO_FORWARD
    * wx.ART_GO_UP
    * wx.ART_GO_DOWN
    * wx.ART_GO_TO_PARENT
    * wx.ART_GO_HOME
    * wx.ART_FILE_OPEN
    * wx.ART_PRINT
    * wx.ART_HELP
    * wx.ART_TIP
    * wx.ART_REPORT_VIEW
    * wx.ART_LIST_VIEW
    * wx.ART_NEW_DIR
    * wx.ART_FOLDER
    * wx.ART_GO_DIR_UP
    * wx.ART_EXECUTABLE_FILE
    * wx.ART_NORMAL_FILE
    * wx.ART_TICK_MARK
    * wx.ART_CROSS_MARK
    * wx.ART_ERROR
    * wx.ART_QUESTION
    * wx.ART_WARNING
    * wx.ART_INFORMATION
    * wx.ART_MISSING_IMAGE 


Clients
-------

The Client is the entity that calls wx.ArtProvider's `GetBitmap` or
`GetIcon` function.  Client IDs serve as a hint to wx.ArtProvider
that is supposed to help it to choose the best looking bitmap. For
example it is often desirable to use slightly different icons in menus
and toolbars even though they represent the same action (e.g.
wx.ART_FILE_OPEN). Remember that this is really only a hint for
wx.ArtProvider -- it is common that `wx.ArtProvider.GetBitmap` returns
identical bitmap for different client values!

    * wx.ART_TOOLBAR
    * wx.ART_MENU
    * wx.ART_FRAME_ICON
    * wx.ART_CMN_DIALOG
    * wx.ART_HELP_BROWSER
    * wx.ART_MESSAGE_BOX
    * wx.ART_BUTTON
    * wx.ART_OTHER (used for all requests that don't fit into any
      of the categories above)
");

MustHaveApp(wxPyArtProvider);
MustHaveApp(wxPyArtProvider::GetBitmap);
MustHaveApp(wxPyArtProvider::GetIcon);

%name(ArtProvider) class wxPyArtProvider /*: public wxObject*/
{
public:

    %pythonAppend wxPyArtProvider "self._setCallbackInfo(self, ArtProvider)"
    wxPyArtProvider();
    
    void _setCallbackInfo(PyObject* self, PyObject* _class);

    DocDeclStr(
        static void , PushProvider(wxPyArtProvider *provider),
        "Add new provider to the top of providers stack.", "");
    

    DocDeclStr(
        static bool , PopProvider(),
        "Remove latest added provider and delete it.", "");
    

    DocDeclStr(
        static bool , RemoveProvider(wxPyArtProvider *provider),
        "Remove provider. The provider must have been added previously!  The
provider is _not_ deleted.", "");
    

    DocDeclStr(
        static wxBitmap , GetBitmap(const wxString& id,
                                    const wxString& client = wxPyART_OTHER,
                                    const wxSize& size = wxDefaultSize),
        "Query the providers for bitmap with given ID and return it. Return
wx.NullBitmap if no provider provides it.", "");
    

    DocDeclStr(
        static wxIcon , GetIcon(const wxString& id,
                          const wxString& client = wxPyART_OTHER,
                                const wxSize& size = wxDefaultSize),
        "Query the providers for icon with given ID and return it.  Return
wx.NullIcon if no provider provides it.", "");
    

    %extend { void Destroy() { delete self; }}
};



//---------------------------------------------------------------------------
%init %{
    wxPyPtrTypeMap_Add("wxArtProvider", "wxPyArtProvider");
%}
//---------------------------------------------------------------------------

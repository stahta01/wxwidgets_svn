/////////////////////////////////////////////////////////////////////////////
// Name:        fs_mem.h
// Purpose:     in-memory file system
// Author:      Vaclav Slavik
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_FS_MEM_H_
#define _WX_FS_MEM_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "fs_mem.h"
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if wxUSE_FILESYSTEM

#include "wx/filesys.h"

#if wxUSE_GUI
    class WXDLLIMPEXP_CORE wxBitmap;
    class WXDLLIMPEXP_CORE wxImage;
#endif // wxUSE_GUI

// ----------------------------------------------------------------------------
// wxMemoryFSHandlerBase
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxMemoryFSHandlerBase : public wxFileSystemHandler
{
public:
    wxMemoryFSHandlerBase();
    ~wxMemoryFSHandlerBase();

    // Add file to list of files stored in memory. Stored data (bitmap, text or
    // raw data) will be copied into private memory stream and available under
    // name "memory:" + filename
    static void AddFile(const wxString& filename, const wxString& textdata);
    static void AddFile(const wxString& filename, const void *binarydata, size_t size);

    // Remove file from memory FS and free occupied memory
    static void RemoveFile(const wxString& filename);

    virtual bool CanOpen(const wxString& location);
    virtual wxFSFile* OpenFile(wxFileSystem& fs, const wxString& location);
    virtual wxString FindFirst(const wxString& spec, int flags = 0);
    virtual wxString FindNext();

protected:
    static bool CheckHash(const wxString& filename);
    static wxHashTable *m_Hash;
};

// ----------------------------------------------------------------------------
// wxMemoryFSHandler
// ----------------------------------------------------------------------------

#if wxUSE_GUI

// add GUI-only operations to the base class
class WXDLLIMPEXP_CORE wxMemoryFSHandler : public wxMemoryFSHandlerBase
{
public:
    // bring the base class versions into the scope, otherwise they would be
    // inaccessible in wxMemoryFSHandler
    using wxMemoryFSHandlerBase::AddFile;

#if wxUSE_IMAGE
    static void AddFile(const wxString& filename, wxImage& image, long type);
#endif // wxUSE_IMAGE

    static void AddFile(const wxString& filename, const wxBitmap& bitmap, long type);
};

#else // !wxUSE_GUI

// just the same thing as the base class in wxBase
class WXDLLIMPEXP_BASE wxMemoryFSHandler : public wxMemoryFSHandlerBase
{
};

#endif // wxUSE_GUI/!wxUSE_GUI

#endif // wxUSE_FILESYSTEM

#endif // _WX_FS_MEM_H_


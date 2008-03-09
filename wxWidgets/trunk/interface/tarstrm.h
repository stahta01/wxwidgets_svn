/////////////////////////////////////////////////////////////////////////////
// Name:        tarstrm.h
// Purpose:     documentation for wxTarInputStream class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxTarInputStream
    @wxheader{tarstrm.h}

    Input stream for reading tar files.

    wxTarInputStream::GetNextEntry returns an
     wxTarEntry object containing the meta-data
    for the next entry in the tar (and gives away ownership). Reading from
    the wxTarInputStream then returns the entry's data. Eof() becomes @true
    after an attempt has been made to read past the end of the entry's data.
    When there are no more entries, GetNextEntry() returns @NULL and sets Eof().

    Tar entries are seekable if the parent stream is seekable. In practice this
    usually means they are only seekable if the tar is stored as a local file and
    is not compressed.

    @library{wxbase}
    @category{streams}

    @seealso
    @ref overview_wxarcbyname "Looking up an archive entry by name"
*/
class wxTarInputStream : public wxArchiveInputStream
{
public:
    //@{
    /**
        Constructor. In a Unicode build the second parameter @a conv is
        used to translate fields from the standard tar header into Unicode. It has
        no effect on the stream's data. @a conv is only used for the standard
        tar headers, any pax extended headers are always UTF-8 encoded.
        If the parent stream is passed as a pointer then the new filter stream
        takes ownership of it. If it is passed by reference then it does not.
    */
    wxTarInputStream(wxInputStream& stream,
                     wxMBConv& conv = wxConvLocal);
    wxTarInputStream(wxInputStream* stream,
                     wxMBConv& conv = wxConvLocal);
    //@}

    /**
        Closes the current entry. On a non-seekable stream reads to the end of
        the current entry first.
    */
    bool CloseEntry();

    /**
        Closes the current entry if one is open, then reads the meta-data for
        the next entry and returns it in a wxTarEntry
        object, giving away ownership. The stream is then open and can be read.
    */
    wxTarEntry* GetNextEntry();

    /**
        Closes the current entry if one is open, then opens the entry specified
        by the @a entry object.
        @a entry should be from the same tar file, and the tar should
        be on a seekable stream.
    */
    bool OpenEntry(wxTarEntry& entry);
};


/**
    @class wxTarClassFactory
    @wxheader{tarstrm.h}

    Class factory for the tar archive format. See the base class
    for details.

    @library{wxbase}
    @category{FIXME}

    @seealso
    @ref overview_wxarc "Archive formats such as zip", @ref overview_wxarcgeneric
    "Generic archive programming", wxTarEntry, wxTarInputStream, wxTarOutputStream
*/
class wxTarClassFactory : public wxArchiveClassFactory
{
public:

};


/**
    @class wxTarOutputStream
    @wxheader{tarstrm.h}

    Output stream for writing tar files.

    wxTarOutputStream::PutNextEntry is used to create
    a new entry in the output tar, then the entry's data is written to the
    wxTarOutputStream. Another call to PutNextEntry() closes the current
    entry and begins the next.

    @library{wxbase}
    @category{streams}

    @seealso
    @ref overview_wxarc "Archive formats such as zip", wxTarEntry, wxTarInputStream
*/
class wxTarOutputStream : public wxArchiveOutputStream
{
public:
    //@{
    /**
        If the parent stream is passed as a pointer then the new filter stream
        takes ownership of it. If it is passed by reference then it does not.
        In a Unicode build the third parameter @a conv is used to translate the
        headers fields into an 8-bit encoding. It has no effect on the stream's data.
        When the @a format is @e wxTAR_PAX, pax extended headers are generated
        when any header field will not fit the standard tar header block or if it
        uses any non-ascii characters.
        Extended headers are stored as extra 'files' within the tar, and will be
        extracted as such by any other tar program that does not understand them.
        The @a conv parameter only affect the standard tar headers, the extended
        headers are always UTF-8 encoded.
        When the @a format is @e wxTAR_USTAR, no extended headers are
        generated, and instead a warning message is logged if any header field
        overflows.
    */
    wxTarOutputStream(wxOutputStream& stream,
                      wxTarFormat format = wxTAR_PAX,
                      wxMBConv& conv = wxConvLocal);
    wxTarOutputStream(wxOutputStream* stream,
                      wxTarFormat format = wxTAR_PAX,
                      wxMBConv& conv = wxConvLocal);
    //@}

    /**
        The destructor calls Close() to finish
        writing the tar if it has not been called already.
    */
    ~wxTarOutputStream();

    /**
        Finishes writing the tar, returning @true if successful.
        Called by the destructor if not called explicitly.
    */
    bool Close();

    /**
        Close the current entry. It is called implicitly whenever another new
        entry is created with CopyEntry()
        or PutNextEntry(), or
        when the tar is closed.
    */
    bool CloseEntry();

    /**
        See wxArchiveOutputStream::CopyArchiveMetaData.
        For the tar format this function does nothing.
    */
    bool CopyArchiveMetaData(wxTarInputStream& s);

    /**
        Takes ownership of @a entry and uses it to create a new entry
        in the tar. @a entry is then opened in @a inputStream and its contents
        copied to this stream.
        For some other archive formats CopyEntry() is much more efficient than
        transferring the data using Read() and Write() since it will copy them
        without decompressing and recompressing them. For tar however it makes
        no difference.
        For tars on seekable streams, @a entry must be from the same tar file
        as @e stream. For non-seekable streams, @a entry must also be the
        last thing read from @e inputStream.
    */
    bool CopyEntry(wxTarEntry* entry, wxTarInputStream& inputStream);

    //@{
    /**
        The tar is zero padded to round its size up to @e BlockingFactor * 512 bytes.
        Defaults to 10 for @e wxTAR_PAX and 20 for @e wxTAR_USTAR
        (see the @ref wxtaroutputstream() constructor), as
        specified in the POSIX standards.
    */
    int GetBlockingFactor();
    void SetBlockingFactor(int factor);
    //@}

    /**
        )
        Create a new directory entry
        (see wxArchiveEntry::IsDir)
        with the given name and timestamp.
        PutNextEntry() can
        also be used to create directory entries, by supplying a name with
        a trailing path separator.
    */
    bool PutNextDirEntry(const wxString& name);

    //@{
    /**
        , @b wxFileOffset@e size = wxInvalidOffset)
        Create a new entry with the given name, timestamp and size.
    */
    bool PutNextEntry(wxTarEntry* entry);
    bool PutNextEntry(const wxString& name);
    //@}
};


/**
    @class wxTarEntry
    @wxheader{tarstrm.h}

    Holds the meta-data for an entry in a tar.

    @library{wxbase}
    @category{FIXME}

    @seealso
    @ref overview_wxarc "Archive formats such as zip", wxTarInputStream,
    wxTarOutputStream
*/
class wxTarEntry : public wxArchiveEntry
{
public:
    //@{
    /**
        Copy constructor.
    */
    wxTarEntry(const wxString& name = wxEmptyString);
    wxTarEntry(const wxTarEntry& entry);
    //@}

    //@{
    /**
        The entry's access time stamp. See also
         wxArchiveEntry::Get/SetDateTime.
    */
    wxDateTime GetAccessTime();
    void SetAccessTime(const wxDateTime& dt);
    //@}

    //@{
    /**
        The entry's creation time stamp. See also
         wxArchiveEntry::Get/SetDateTime.
    */
    wxDateTime GetCreateTime();
    void SetCreateTime(const wxDateTime& dt);
    //@}

    //@{
    /**
        OS specific IDs defining a device, these are only meaningful when
         TypeFlag() is set to @e wxTAR_CHRTYPE
         or @e wxTAR_BLKTYPE.
    */
    int GetDevMajor();
    int GetDevMinor();
    void SetDevMajor(int dev);
    void SetDevMinor(int dev);
    //@}

    //@{
    /**
        The user ID and group ID that has @ref mode() permissions over
        this entry. These values aren't usually useful unless the file will only be
        restored to the same system it originated from. @ref unamegname()
        "Get/SetGroupName and
        Get/SetUserName" can be used instead.
    */
    int GetGroupId();
    int GetUserId();
    void SetGroupId(int id);
    void SetUserId(int id);
    //@}

    //@{
    /**
        The names of the user and group that has @ref mode() permissions
        over this entry. These are not present in very old tars.
    */
    wxString GetGroupName();
    wxString GetUserName();
    void SetGroupName(const wxString& group);
    void SetUserName(const wxString& user);
    //@}

    //@{
    /**
        The filename of a previous entry in the tar that this entry is a link to.
        Only meaningful when TypeFlag() is set
        to @e wxTAR_LNKTYPE or @e wxTAR_SYMTYPE.
    */
    wxString GetLinkName();
    void SetLinkName(const wxString& link);
    //@}

    //@{
    /**
        UNIX permission bits for this entry. Giving read, write and execute permissions
        to the file's @ref unamegname() "User and Group" and to others.
        Symbols are defined for them in wx/file.h.
    */
    int GetMode();
    void SetMode(int mode);
    //@}

    //@{
    /**
        The size of the entry's data in bytes.
        The tar archive format stores the entry's size ahead of the entry's data.
        Therefore when creating an archive on a non-seekable stream it is necessary to
        supply the correct size when each entry is created. For seekable streams this
        is not necessary as wxTarOutputStream will attempt
        to seek back and fix the entry's header when the entry is closed, though it is
        still more efficient if the size is given beforehand.
    */
    void SetSize(wxFileOffset size);
    wxFileOffset GetSize();
    //@}

    //@{
    /**
        Returns the type of the entry. It should be one of the following:
        
        When creating archives use just these values. When reading archives
        any other values should be treated as @e wxTAR_REGTYPE.
    */
    int GetTypeFlag();
    void SetTypeFlag(int type);
    //@}

    //@{
    /**
        A static member that translates a filename into the internal format used
        within the archive. If the third parameter is provided, the bool pointed
        to is set to indicate whether the name looks like a directory name
        (i.e. has a trailing path separator).
    */
    wxString GetInternalName();
    wxString GetInternalName(const wxString& name,
                             wxPathFormat format = wxPATH_NATIVE,
                             bool* pIsDir = NULL);
    //@}

    /**
        Assignment operator.
    */
    wxTarEntry& operator operator=(const wxTarEntry& entry);
};

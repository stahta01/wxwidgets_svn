/////////////////////////////////////////////////////////////////////////////
// Name:        buffer.h
// Purpose:     documentation for wxMemoryBuffer class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxMemoryBuffer
    @wxheader{buffer.h}
    
    A @b wxMemoryBuffer is a useful data structure for storing arbitrary sized
    blocks
    of memory. wxMemoryBuffer guarantees deletion of the memory block when the
    object
    is destroyed. 
    
    @library{wxbase}
    @category{FIXME}
*/
class wxMemoryBuffer 
{
public:
    //@{
    /**
        Create a new buffer.
        
        @param size 
        size of new buffer.
    */
    wxMemoryBuffer(const wxMemoryBuffer& src);
        wxMemoryBuffer(size_t size);
    //@}

    /**
        Append a single byte to the buffer.
        
        @param data 
        New byte to append to the buffer.
    */
    void AppendByte(char data);

    /**
        Ensure that the buffer is big enough and return a pointer to the start
        of the empty space in the buffer. This pointer can be used to directly 
        write data into the buffer, this new data will be appended to
        the existing data.
        
        @param sizeNeeded 
        Amount of extra space required in the buffer for
        the append operation
    */
    void * GetAppendBuf(size_t sizeNeeded);

    /**
        Returns the size of the buffer.
    */
    size_t GetBufSize();

    /**
        Return a pointer to the data in the buffer.
    */
    void* GetData();

    /**
        Returns the length of the valid data in the buffer.
    */
    size_t GetDataLen();

    /**
        Ensure the buffer is big enough and return a pointer to the
        buffer which can be used to directly write into the buffer
        up to @e sizeNeeded bytes.
    */
    void * GetWriteBuf(size_t sizeNeeded);

    /**
        Ensures the buffer has at least @e size bytes available.
    */
    void SetBufSize(size_t size);

    /**
        Sets the length of the data stored in the buffer.  Mainly useful for truncating
        existing data.
        
        @param size 
        New length of the valid data in the buffer. This is
        distinct from the allocated size
    */
    void SetDataLen(size_t size);

    /**
        Update the length after completing a direct append, which
        you must have used GetAppendBuf() to initialise.
        
        @param sizeUsed 
        This is the amount of new data that has been 
        appended.
    */
    void UngetAppendBuf(size_t sizeUsed);

    /**
        Update the buffer after completing a direct write, which
        you must have used GetWriteBuf() to initialise.
        
        @param sizeUsed 
        The amount of data written in to buffer
        by the direct write
    */
    void UngetWriteBuf(size_t sizeUsed);
};

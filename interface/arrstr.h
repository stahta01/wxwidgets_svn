/////////////////////////////////////////////////////////////////////////////
// Name:        arrstr.h
// Purpose:     documentation for wxArrayString class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxArrayString
    @wxheader{arrstr.h}
    
    wxArrayString is an efficient container for storing 
    wxString objects. It has the same features as all 
    wxArray classes, i.e. it dynamically expands when new items
    are added to it (so it is as easy to use as a linked list), but the access
    time to the elements is constant, instead of being linear in number of
    elements as in the case of linked lists. It is also very size efficient and
    doesn't take more space than a C array @e wxString[] type (wxArrayString
    uses its knowledge of internals of wxString class to achieve this).
    
    This class is used in the same way as other dynamic arrays,
    except that no @e WX_DEFINE_ARRAY declaration is needed for it. When a
    string is added or inserted in the array, a copy of the string is created, so
    the original string may be safely deleted (e.g. if it was a @e wxChar * 
    pointer the memory it was using can be freed immediately after this). In
    general, there is no need to worry about string memory deallocation when using
    this class - it will always free the memory it uses itself.
    
    The references returned by wxArrayString::Item, 
    wxArrayString::Last or 
    @ref wxArrayString::operatorindex operator[] are not constant, so the
    array elements may be modified in place like this
    
    @code
    array.Last().MakeUpper();
    @endcode
    
    There is also a variant of wxArrayString called wxSortedArrayString which has
    exactly the same methods as wxArrayString, but which always keeps the string
    in it in (alphabetical) order. wxSortedArrayString uses binary search in its 
    wxArrayString::Index function (instead of linear search for
    wxArrayString::Index) which makes it much more efficient if you add strings to
    the array rarely (because, of course, you have to pay for Index() efficiency
    by having Add() be slower) but search for them often. Several methods should
    not be used with sorted array (basically, all which break the order of items)
    which is mentioned in their description.
    
    Final word: none of the methods of wxArrayString is virtual including its
    destructor, so this class should not be used as a base class.
    
    @library{wxbase}
    @category{containers}
    
    @seealso
    wxArray, wxString, @ref overview_wxstringoverview "wxString overview"
*/
class wxArrayString : public wxArray
{
public:
    //@{
    /**
        Constructor from a wxString array. Pass a size @e sz and array @e arr.
    */
    wxArrayString();
        wxArrayString(const wxArrayString& array);
        wxArrayString(size_t sz, const char** arr);
        wxArrayString(size_t sz, const wchar_t** arr);
        wxArrayString(size_t sz, const wxString* arr);
    //@}

    /**
        Destructor frees memory occupied by the array strings. For the performance
        reasons it is not virtual, so this class should not be derived from.
    */
    ~wxArrayString();

    /**
        Appends the given number of @e copies of the new item @e str to the
        array and returns the index of the first new item in the array.
        
        @b Warning: For sorted arrays, the index of the inserted item will not be,
        in general, equal to GetCount() - 1 because
        the item is inserted at the correct position to keep the array sorted and not
        appended.
        
        See also: Insert()
    */
#define size_t Add(const wxString& str, size_t copies = 1)     /* implementation is private */

    /**
        Preallocates enough memory to store @e nCount items. This function may be
        used to improve array class performance before adding a known number of items
        consecutively.
        
        See also: @ref wxArray::memorymanagement "Dynamic array memory management"
    */
    void Alloc(size_t nCount);

    /**
        Clears the array contents and frees memory.
        
        See also: Empty()
    */
    void Clear();

    /**
        Empties the array: after a call to this function 
        GetCount() will return 0. However, this
        function does not free the memory used by the array and so should be used when
        the array is going to be reused for storing other strings. Otherwise, you
        should use Clear() to empty the array and free
        memory.
    */
    void Empty();

    /**
        Returns the number of items in the array.
    */
    size_t GetCount();

    /**
        Search the element in the array, starting from the beginning if
        @e bFromEnd is @false or from end otherwise. If @e bCase, comparison is
        case sensitive (default), otherwise the case is ignored.
        
        This function uses linear search for wxArrayString and binary search for
        wxSortedArrayString, but it ignores the @e bCase and @e bFromEnd 
        parameters in the latter case.
        
        Returns index of the first item matched or @c wxNOT_FOUND if there is no match.
    */
    int Index(const wxString& sz, bool bCase = @true,
              bool bFromEnd = @false);

    /**
        Insert the given number of @e copies of the new element in the array before the
        position @e nIndex. Thus, for
        example, to insert the string in the beginning of the array you would write
        If @e nIndex is equal to @e GetCount() this function behaves as 
        Add().
        
        @b Warning: this function should not be used with sorted arrays because it
        could break the order of items and, for example, subsequent calls to 
        Index() would then not work!
    */
    void Insert(const wxString& str, size_t nIndex,
                size_t copies = 1);

    /**
        Returns @true if the array is empty, @false otherwise. This function returns the
        same result as @e GetCount() == 0 but is probably easier to read.
    */
    bool IsEmpty();

    /**
        Return the array element at position @e nIndex. An assert failure will
        result from an attempt to access an element beyond the end of array in debug
        mode, but no check is done in release mode.
        
        See also @ref operatorindex() operator[] for the operator
        version.
    */
    wxString Item(size_t nIndex);

    /**
        Returns the last element of the array. Attempt to access the last element of
        an empty array will result in assert failure in debug build, however no checks
        are done in release mode.
    */
    wxString Last();

    /**
        Removes the first item matching this value. An assert failure is provoked by
        an attempt to remove an element which does not exist in debug build.
        
        See also: Index()
    */
    void Remove(const wxString& sz);

    /**
        Removes @e count items starting at position @e nIndex from the array.
    */
    void RemoveAt(size_t nIndex, size_t count = 1);

    /**
        Releases the extra memory allocated by the array. This function is useful to
        minimize the array memory consumption.
        
        See also: Alloc(), @ref wxArray::memorymanagement "Dynamic array memory
        management"
    */
    void Shrink();

    //@{
    /**
        Sorts the array using the specified @e compareFunction for item comparison.
        @e CompareFunction is defined as a function taking two @e const
        wxString parameters and returning an @e int value less than, equal to or
        greater than 0 if the first string is less than, equal to or greater than the
        second one.
    */
    void Sort(bool reverseOrder = @false);
        Warning: void Sort(CompareFunction compareFunction);
    //@}

    /**
        Compares 2 arrays respecting the case. Returns @true if the arrays have
        different number of elements or if the elements don't match pairwise.
    */
    bool operator !=(const wxArrayString& array);

    /**
        Assignment operator.
    */
    wxArrayString operator =(const wxArrayString& array);

    /**
        Compares 2 arrays respecting the case. Returns @true only if the arrays have
        the same number of elements and the same strings in the same order.
    */
    bool operator ==(const wxArrayString& array);

    /**
        Return the array element at position @e nIndex. An assert failure will
        result from an attempt to access an element beyond the end of array in debug
        mode, but no check is done in release mode.
        
        This is the operator version of Item() method.
    */
    wxString operator[](size_t nIndex);
};


// ============================================================================
// Global functions/macros
// ============================================================================

/**
    Splits the given wxString object using the separator @e sep and returns the
    result as a wxArrayString.
    
    If the @e escape character is non-@NULL, then the occurrences of @e sep
    immediately prefixed
    with @e escape are not considered as separators.
    
    Note that empty tokens will be generated if there are two or more adjacent
    separators.
    
    @sa wxJoin
*/
wxArrayString wxSplit(const wxString& str, const wxChar sep,
                      const wxChar escape = '
');

/**
    Concatenate all lines of the given wxArrayString object using the separator @e
    sep and returns
    the result as a wxString.
    
    If the @e escape character is non-@NULL, then it's used as prefix for each
    occurrence of @e sep
    in the strings contained in @e arr before joining them which is necessary
    in order to be able to recover the original array contents from the string
    later using wxSplit.
*/
wxString wxJoin(const wxArrayString& arr, const wxChar sep,
                const wxChar escape = '\');


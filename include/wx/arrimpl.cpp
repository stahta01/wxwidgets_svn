///////////////////////////////////////////////////////////////////////////////
// Name:        listimpl.cpp
// Purpose:     helper file for implementation of dynamic lists
// Author:      Vadim Zeitlin
// Modified by: 
// Created:     16.10.97
// RCS-ID:      $Id$
// Copyright:   (c) 1997 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************
 * Purpose: implements methods of "template" class declared in               *
 *          DECLARE_OBJARRAY macro and which couldn't be implemented inline  *
 *          (because they need the full definition of type T in scope)       *
 *                                                                           *
 * Usage:   1) #include dynarray.h                                           *
 *          2) WX_DECLARE_OBJARRAY                                           *
 *          3) #include arrimpl.cpp                                          *
 *          4) WX_DEFINE_OBJARRAY                                            *
 *****************************************************************************/

// macro implements remaining (not inline) methods of template list
// (it's private to this file)
#undef  _DEFINE_OBJARRAY
#define _DEFINE_OBJARRAY(T, name)                                             \
name::~name()                                                                 \
{                                                                             \
  Empty();                                                                    \
}                                                                             \
                                                                              \
void name::DoCopy(const name& src)                                            \
{                                                                             \
  for ( size_t ui = 0; ui < src.Count(); ui++ )                               \
    Add(src[ui]);                                                             \
}                                                                             \
                                                                              \
name& name::operator=(const name& src)                                        \
{                                                                             \
  Empty();                                                                    \
  DoCopy(src);                                                                \
                                                                              \
  return *this;                                                               \
}                                                                             \
                                                                              \
name::name(const name& src)                                                   \
{                                                                             \
  DoCopy(src);                                                                \
}                                                                             \
                                                                              \
void name::Empty()                                                            \
{                                                                             \
  for ( size_t ui = 0; ui < Count(); ui++ )                                   \
    delete (T*)wxBaseArray::Item(ui);                                         \
                                                                              \
  wxBaseArray::Clear();                                                       \
}                                                                             \
                                                                              \
void name::Remove(size_t uiIndex)                                             \
{                                                                             \
  wxCHECK_RET( uiIndex < Count(), "bad index in " #name "::Remove()" );       \
                                                                              \
  delete (T*)wxBaseArray::Item(uiIndex);                                      \
                                                                              \
  wxBaseArray::Remove(uiIndex);                                               \
}                                                                             \
                                                                              \
void name::Add(const T& item)                                                 \
{                                                                             \
  T* pItem = new T(item);                                                     \
  if ( pItem != NULL )                                                        \
    Add(pItem);                                                               \
}                                                                             \
                                                                              \
void name::Insert(const T& item, size_t uiIndex)                              \
{                                                                             \
  T* pItem = new T(item);                                                     \
  if ( pItem != NULL )                                                        \
    Insert(pItem, uiIndex);                                                   \
}                                                                             \
                                                                              \
int name::Index(const T& Item, bool bFromEnd) const                           \
{                                                                             \
  if ( bFromEnd ) {                                                           \
    if ( Count() > 0 ) {                                                      \
      size_t ui = Count() - 1;                                                \
      do {                                                                    \
        if ( (T*)wxBaseArray::Item(ui) == &Item )                             \
          return ui;                                                          \
        ui--;                                                                 \
      }                                                                       \
      while ( ui != 0 );                                                      \
    }                                                                         \
  }                                                                           \
  else {                                                                      \
    for( size_t ui = 0; ui < Count(); ui++ ) {                                \
      if( (T*)wxBaseArray::Item(ui) == &Item )                                \
        return ui;                                                            \
    }                                                                         \
  }                                                                           \
                                                                              \
  return NOT_FOUND;                                                           \
}                                                                             

// redefine the macro so that now it will generate the class implementation
// old value would provoke a compile-time error if this file is not included
#undef  WX_DEFINE_OBJARRAY
#define WX_DEFINE_OBJARRAY(name) _DEFINE_OBJARRAY(_L##name, name)

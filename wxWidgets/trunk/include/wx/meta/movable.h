/////////////////////////////////////////////////////////////////////////////
// Name:        wx/meta/movable.h
// Purpose:     Test if a type is movable using memmove() etc.
// Author:      Vaclav Slavik
// Created:     2008-01-21
// RCS-ID:      $Id$
// Copyright:   (c) 2008 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_META_MOVABLE_H_
#define _WX_META_MOVABLE_H_

#include "wx/defs.h"
#include "wx/string.h" // for wxIsMovable<wxString> specialization

// Helper to decide if an object of type T is "movable", i.e. if it can be
// copied to another memory location using memmove() or realloc() C functions.
// C++ only gurantees that POD types (including primitive types) are
// movable.
template<typename T>
struct wxIsMovable
{
    // NB: enum, because VC6 can't handle "static const bool value = true;"
    enum { value = false };
};

// Macro to add wxIsMovable<T> specialization for given type that marks it
// as movable:
#define WX_DECLARE_TYPE_MOVABLE(type)                       \
    template<> struct wxIsMovable<type>                     \
    {                                                       \
        enum { value = true };                              \
    };

WX_DECLARE_TYPE_MOVABLE(bool)
WX_DECLARE_TYPE_MOVABLE(unsigned char)
WX_DECLARE_TYPE_MOVABLE(signed char)
WX_DECLARE_TYPE_MOVABLE(unsigned int)
WX_DECLARE_TYPE_MOVABLE(signed int)
WX_DECLARE_TYPE_MOVABLE(unsigned short int)
WX_DECLARE_TYPE_MOVABLE(signed short int)
WX_DECLARE_TYPE_MOVABLE(signed long int)
WX_DECLARE_TYPE_MOVABLE(unsigned long int)
WX_DECLARE_TYPE_MOVABLE(float)
WX_DECLARE_TYPE_MOVABLE(double)
WX_DECLARE_TYPE_MOVABLE(long double)
#if wxWCHAR_T_IS_REAL_TYPE
WX_DECLARE_TYPE_MOVABLE(wchar_t)
#endif
#ifdef wxLongLong_t
WX_DECLARE_TYPE_MOVABLE(wxLongLong_t)
WX_DECLARE_TYPE_MOVABLE(wxULongLong_t)
#endif

// Visual C++ 6.0 can't compile partial template specializations and as this is
// only an optimization, we can live with pointers not being recognized as
// movable types under VC6
#if !wxCHECK_VISUALC_VERSION(7)

// pointers are movable:
template<typename T>
struct wxIsMovable<T*>
{
    enum { value = true };
};
template<typename T>
struct wxIsMovable<const T*>
{
    enum { value = true };
};

#endif // !VC++ < 7

// Our implementation of wxString is written in such way that it's safe to move
// it around. OTOH, we don't know anything about std::string.
// (NB: we don't put this into string.h and choose to include wx/string.h from
// here instead so that rarely-used wxIsMovable<T> code isn't included by
// everything)
#if !wxUSE_STL
WX_DECLARE_TYPE_MOVABLE(wxString)
#endif


#endif // _WX_META_MOVABLE_H_

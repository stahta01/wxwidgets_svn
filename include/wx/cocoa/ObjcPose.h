/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/ObjcPose.h
// Purpose:     Macros for initializing poseAs, among other things
// Author:      David Elliott
// Modified by: 
// Created:     2002/12/03
// RCS-ID:      $Id:
// Copyright:   (c) 2002 David Elliott <dfe@cox.net>
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_PRIVATE_POSER_H__
#define __WX_COCOA_PRIVATE_POSER_H__

/*-------------------------------------------------------------------------
Objective-C Poser class initialization
-------------------------------------------------------------------------*/
#ifdef __OBJC__
#import <objc/objc-class.h>

class wxPoseAsInitializer
{
public:
	wxPoseAsInitializer(Class poser)
	: m_poser(poser)
	, m_next(sm_first)
	{
		sm_first = this;
	}
	~wxPoseAsInitializer()
	{
		class_poseAs(m_poser,m_poser->super_class);
		sm_first = m_next;
	}
	static void InitializePosers()
	{
		while(sm_first)
		{
			delete sm_first;
		}
	};
protected:
	Class m_poser;
	wxPoseAsInitializer *m_next;
	static wxPoseAsInitializer *sm_first;
};

class wxDummyForPoseAsInitializer
{
public:
	wxDummyForPoseAsInitializer(void*) {}
};

#define WX_IMPLEMENT_POSER(poser) \
wxDummyForPoseAsInitializer wxDummyPoseAsInitializerFor##poser(new wxPoseAsInitializer([poser class]))

#endif // __OBJC__

/*-------------------------------------------------------------------------
Basic hashmap stuff, used by everything
-------------------------------------------------------------------------*/
#define WX_DECLARE_OBJC_HASHMAP(ObjcClass) \
class wxCocoa##ObjcClass; \
WX_DECLARE_HASH_MAP(WX_##ObjcClass,wxCocoa##ObjcClass*,wxPointerHash,wxPointerEqual,wxCocoa##ObjcClass##Hash)

#define WX_DECLARE_OBJC_INTERFACE_HASHMAP(ObjcClass) \
public: \
    static inline wxCocoa##ObjcClass* GetFromCocoa(WX_##ObjcClass cocoaObjcClass) \
    { \
        wxCocoa##ObjcClass##Hash::iterator iter = sm_cocoaHash.find(cocoaObjcClass); \
        if(iter!=sm_cocoaHash.end()) \
        { \
            return iter->second; \
        } \
        return NULL; \
    } \
protected: \
    static wxCocoa##ObjcClass##Hash sm_cocoaHash;

#define WX_IMPLEMENT_OBJC_INTERFACE_HASHMAP(ObjcClass) \
wxCocoa##ObjcClass##Hash wxCocoa##ObjcClass::sm_cocoaHash;


/*-------------------------------------------------------------------------
The entire interface, including some boilerplate stuff
-------------------------------------------------------------------------*/
#define WX_DECLARE_OBJC_INTERFACE(ObjcClass) \
WX_DECLARE_OBJC_INTERFACE_HASHMAP(ObjcClass) \
public: \
    inline void Associate##ObjcClass(WX_##ObjcClass cocoaObjcClass) \
    { \
        sm_cocoaHash.insert(wxCocoa##ObjcClass##Hash::value_type(cocoaObjcClass,this)); \
    } \
    inline void Disassociate##ObjcClass(WX_##ObjcClass cocoaObjcClass) \
    { \
        sm_cocoaHash.erase(cocoaObjcClass); \
    }

#define WX_IMPLEMENT_OBJC_INTERFACE(ObjcClass) \
WX_IMPLEMENT_OBJC_INTERFACE_HASHMAP(ObjcClass)

/*-------------------------------------------------------------------------
Stuff to be used by the wxWindows class (not the Cocoa interface)
-------------------------------------------------------------------------*/
#define WX_DECLARE_COCOA_OWNER(ObjcClass,ObjcBase,ObjcRoot) \
public: \
    inline WX_##ObjcClass Get##ObjcClass() { return (WX_##ObjcClass)m_cocoa##ObjcRoot; } \
    inline const WX_##ObjcClass Get##ObjcClass() const { return (WX_##ObjcClass)m_cocoa##ObjcRoot; } \
protected: \
    void Set##ObjcClass(WX_##ObjcClass cocoaObjcClass);

#define WX_IMPLEMENT_COCOA_OWNER(wxClass,ObjcClass,ObjcBase,ObjcRoot) \
void wxClass::Set##ObjcClass(WX_##ObjcClass cocoaObjcClass) \
{ \
    if(m_cocoa##ObjcRoot) \
        Disassociate##ObjcClass((WX_##ObjcClass)m_cocoa##ObjcRoot); \
    Set##ObjcBase(cocoaObjcClass); \
    if(m_cocoa##ObjcRoot) \
        Associate##ObjcClass((WX_##ObjcClass)m_cocoa##ObjcRoot); \
}

#endif // __WX_COCOA_PRIVATE_POSER_H__


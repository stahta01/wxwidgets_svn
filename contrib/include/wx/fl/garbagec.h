/////////////////////////////////////////////////////////////////////////////
// Name:        No names yet.
// Purpose:     Contrib. demo
// Author:      Aleksandras Gluchovas (@Lithuania)
// Modified by:
// Created:     ??/10/98
// RCS-ID:      $Id$
// Copyright:   (c) Aleksandras Gluchovas
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __GARBAGEC_G__
#define __GARBAGEC_G__

#ifdef __GNUG__
    #pragma interface "garbagec.h"
#endif

#include "wx/list.h"

struct GCItem
{
	void*	  mpObj;
	wxList    mRefs;   // references to other nodes
};

inline void* gc_node_to_obj( wxNode* pGCNode )
{
	return ( (GCItem*) (pGCNode->Data()) )->mpObj;
}

// class implements extremely slow, but probably one of the most simple GC algorithms

class GarbageCollector
{
protected:
	wxList mAllNodes;
	wxList mRegularLst;
	wxList mCycledLst;

	wxNode* FindItemNode( void* pForObj );
	void    ResolveReferences();

	wxNode* FindReferenceFreeItemNode();
	void    RemoveReferencesToNode( wxNode* pItemNode );
	void    DestroyItemList( wxList& lst );

public:

	GarbageCollector() {}

	virtual ~GarbageCollector();

	// prepare data for GC alg.

	virtual void AddObject( void* pObj, int refCnt = 1 );
	virtual void AddDependency( void* pObj, void* pDependsOnObj );

	// executes GC alg.

	virtual void ArrangeCollection();

	// access results of the alg.

	wxList& GetRegularObjects();
	wxList& GetCycledObjects();

	// removes all data from GC

	void Reset();
};

#endif /* __GARBAGEC_G__ */


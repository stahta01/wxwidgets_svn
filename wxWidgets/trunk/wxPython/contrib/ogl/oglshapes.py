# This file was created automatically by SWIG.
import oglshapesc

from misc import *

from misc2 import *

from windows import *

from gdi import *

from fonts import *

from clip_dnd import *

from events import *

from streams import *

from utils import *

from mdi import *

from frames import *

from stattool import *

from controls import *

from controls2 import *

from windows2 import *

from cmndlgs import *

from windows3 import *

from image import *

from printfw import *

from sizers import *

from filesys import *

from oglbasic import *
import wx
class wxPseudoMetaFilePtr(wxObjectPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self, delfunc=oglshapesc.delete_wxPseudoMetaFile):
        if self.thisown == 1:
            try:
                delfunc(self)
            except:
                pass
    def Draw(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_Draw(self, *_args, **_kwargs)
        return val
    def Clear(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_Clear(self, *_args, **_kwargs)
        return val
    def Copy(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_Copy(self, *_args, **_kwargs)
        return val
    def Scale(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_Scale(self, *_args, **_kwargs)
        return val
    def ScaleTo(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_ScaleTo(self, *_args, **_kwargs)
        return val
    def Translate(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_Translate(self, *_args, **_kwargs)
        return val
    def Rotate(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_Rotate(self, *_args, **_kwargs)
        return val
    def LoadFromMetaFile(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_LoadFromMetaFile(self, *_args, **_kwargs)
        return val
    def GetBounds(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_GetBounds(self, *_args, **_kwargs)
        return val
    def CalculateSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_CalculateSize(self, *_args, **_kwargs)
        return val
    def SetRotateable(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_SetRotateable(self, *_args, **_kwargs)
        return val
    def GetRotateable(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_GetRotateable(self, *_args, **_kwargs)
        return val
    def SetSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_SetSize(self, *_args, **_kwargs)
        return val
    def SetFillBrush(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_SetFillBrush(self, *_args, **_kwargs)
        return val
    def GetFillBrush(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_GetFillBrush(self, *_args, **_kwargs)
        if val: val = wxBrushPtr(val) 
        return val
    def SetOutlinePen(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_SetOutlinePen(self, *_args, **_kwargs)
        return val
    def GetOutlinePen(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_GetOutlinePen(self, *_args, **_kwargs)
        if val: val = wxPenPtr(val) 
        return val
    def SetOutlineOp(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_SetOutlineOp(self, *_args, **_kwargs)
        return val
    def GetOutlineOp(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_GetOutlineOp(self, *_args, **_kwargs)
        return val
    def IsValid(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_IsValid(self, *_args, **_kwargs)
        return val
    def DrawLine(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_DrawLine(self, *_args, **_kwargs)
        return val
    def DrawRectangle(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_DrawRectangle(self, *_args, **_kwargs)
        return val
    def DrawRoundedRectangle(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_DrawRoundedRectangle(self, *_args, **_kwargs)
        return val
    def DrawArc(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_DrawArc(self, *_args, **_kwargs)
        return val
    def DrawEllipticArc(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_DrawEllipticArc(self, *_args, **_kwargs)
        return val
    def DrawEllipse(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_DrawEllipse(self, *_args, **_kwargs)
        return val
    def DrawPoint(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_DrawPoint(self, *_args, **_kwargs)
        return val
    def DrawText(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_DrawText(self, *_args, **_kwargs)
        return val
    def DrawLines(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_DrawLines(self, *_args, **_kwargs)
        return val
    def DrawPolygon(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_DrawPolygon(self, *_args, **_kwargs)
        return val
    def DrawSpline(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_DrawSpline(self, *_args, **_kwargs)
        return val
    def SetClippingRect(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_SetClippingRect(self, *_args, **_kwargs)
        return val
    def DestroyClippingRect(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_DestroyClippingRect(self, *_args, **_kwargs)
        return val
    def SetPen(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_SetPen(self, *_args, **_kwargs)
        return val
    def SetBrush(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_SetBrush(self, *_args, **_kwargs)
        return val
    def SetFont(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_SetFont(self, *_args, **_kwargs)
        return val
    def SetTextColour(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_SetTextColour(self, *_args, **_kwargs)
        return val
    def SetBackgroundColour(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_SetBackgroundColour(self, *_args, **_kwargs)
        return val
    def SetBackgroundMode(self, *_args, **_kwargs):
        val = oglshapesc.wxPseudoMetaFile_SetBackgroundMode(self, *_args, **_kwargs)
        return val
    def __repr__(self):
        return "<%s.%s instance; proxy of C++ wxPseudoMetaFile instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this)
class wxPseudoMetaFile(wxPseudoMetaFilePtr):
    def __init__(self,*_args,**_kwargs):
        self.this = oglshapesc.new_wxPseudoMetaFile(*_args,**_kwargs)
        self.thisown = 1




class wxPyRectangleShapePtr(wxPyShapePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def _setCallbackInfo(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape__setCallbackInfo(self, *_args, **_kwargs)
        return val
    def SetCornerRadius(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_SetCornerRadius(self, *_args, **_kwargs)
        return val
    def base_OnDelete(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnDelete(self, *_args, **_kwargs)
        return val
    def base_OnDraw(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnDraw(self, *_args, **_kwargs)
        return val
    def base_OnDrawContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnDrawContents(self, *_args, **_kwargs)
        return val
    def base_OnDrawBranches(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnDrawBranches(self, *_args, **_kwargs)
        return val
    def base_OnMoveLinks(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnMoveLinks(self, *_args, **_kwargs)
        return val
    def base_OnErase(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnErase(self, *_args, **_kwargs)
        return val
    def base_OnEraseContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnEraseContents(self, *_args, **_kwargs)
        return val
    def base_OnHighlight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnHighlight(self, *_args, **_kwargs)
        return val
    def base_OnLeftClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnLeftClick(self, *_args, **_kwargs)
        return val
    def base_OnLeftDoubleClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnLeftDoubleClick(self, *_args, **_kwargs)
        return val
    def base_OnRightClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnRightClick(self, *_args, **_kwargs)
        return val
    def base_OnSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnSize(self, *_args, **_kwargs)
        return val
    def base_OnMovePre(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnMovePre(self, *_args, **_kwargs)
        return val
    def base_OnMovePost(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnMovePost(self, *_args, **_kwargs)
        return val
    def base_OnDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnDragRight(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnBeginDragRight(self, *_args, **_kwargs)
        return val
    def base_OnEndDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnEndDragRight(self, *_args, **_kwargs)
        return val
    def base_OnDrawOutline(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnDrawOutline(self, *_args, **_kwargs)
        return val
    def base_OnDrawControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnDrawControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnEraseControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnEraseControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnMoveLink(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnMoveLink(self, *_args, **_kwargs)
        return val
    def base_OnSizingDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnSizingDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnSizingBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnSizingEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnBeginSize(self, *_args, **_kwargs)
        return val
    def base_OnEndSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyRectangleShape_base_OnEndSize(self, *_args, **_kwargs)
        return val
    def __repr__(self):
        return "<%s.%s instance; proxy of C++ wxPyRectangleShape instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this)
class wxPyRectangleShape(wxPyRectangleShapePtr):
    def __init__(self,*_args,**_kwargs):
        self.this = oglshapesc.new_wxPyRectangleShape(*_args,**_kwargs)
        self.thisown = 1
        self._setCallbackInfo(self, wxPyRectangleShape)
        self._setOORInfo(self)




class wxPyControlPointPtr(wxPyRectangleShapePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def _setCallbackInfo(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint__setCallbackInfo(self, *_args, **_kwargs)
        return val
    def SetCornerRadius(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_SetCornerRadius(self, *_args, **_kwargs)
        return val
    def base_OnDelete(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnDelete(self, *_args, **_kwargs)
        return val
    def base_OnDraw(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnDraw(self, *_args, **_kwargs)
        return val
    def base_OnDrawContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnDrawContents(self, *_args, **_kwargs)
        return val
    def base_OnDrawBranches(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnDrawBranches(self, *_args, **_kwargs)
        return val
    def base_OnMoveLinks(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnMoveLinks(self, *_args, **_kwargs)
        return val
    def base_OnErase(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnErase(self, *_args, **_kwargs)
        return val
    def base_OnEraseContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnEraseContents(self, *_args, **_kwargs)
        return val
    def base_OnHighlight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnHighlight(self, *_args, **_kwargs)
        return val
    def base_OnLeftClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnLeftClick(self, *_args, **_kwargs)
        return val
    def base_OnLeftDoubleClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnLeftDoubleClick(self, *_args, **_kwargs)
        return val
    def base_OnRightClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnRightClick(self, *_args, **_kwargs)
        return val
    def base_OnSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnSize(self, *_args, **_kwargs)
        return val
    def base_OnMovePre(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnMovePre(self, *_args, **_kwargs)
        return val
    def base_OnMovePost(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnMovePost(self, *_args, **_kwargs)
        return val
    def base_OnDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnDragRight(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnBeginDragRight(self, *_args, **_kwargs)
        return val
    def base_OnEndDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnEndDragRight(self, *_args, **_kwargs)
        return val
    def base_OnDrawOutline(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnDrawOutline(self, *_args, **_kwargs)
        return val
    def base_OnDrawControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnDrawControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnEraseControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnEraseControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnMoveLink(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnMoveLink(self, *_args, **_kwargs)
        return val
    def base_OnSizingDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnSizingDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnSizingBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnSizingEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnBeginSize(self, *_args, **_kwargs)
        return val
    def base_OnEndSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyControlPoint_base_OnEndSize(self, *_args, **_kwargs)
        return val
    def __repr__(self):
        return "<%s.%s instance; proxy of C++ wxPyControlPoint instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this)
class wxPyControlPoint(wxPyControlPointPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = oglshapesc.new_wxPyControlPoint(*_args,**_kwargs)
        self.thisown = 1
        self._setCallbackInfo(self, wxPyControlPoint)
        self._setOORInfo(self)




class wxPyBitmapShapePtr(wxPyRectangleShapePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def _setCallbackInfo(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape__setCallbackInfo(self, *_args, **_kwargs)
        return val
    def GetBitmap(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_GetBitmap(self, *_args, **_kwargs)
        if val: val = wxBitmapPtr(val) 
        return val
    def GetFilename(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_GetFilename(self, *_args, **_kwargs)
        return val
    def SetBitmap(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_SetBitmap(self, *_args, **_kwargs)
        return val
    def SetFilename(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_SetFilename(self, *_args, **_kwargs)
        return val
    def base_OnDelete(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnDelete(self, *_args, **_kwargs)
        return val
    def base_OnDraw(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnDraw(self, *_args, **_kwargs)
        return val
    def base_OnDrawContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnDrawContents(self, *_args, **_kwargs)
        return val
    def base_OnDrawBranches(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnDrawBranches(self, *_args, **_kwargs)
        return val
    def base_OnMoveLinks(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnMoveLinks(self, *_args, **_kwargs)
        return val
    def base_OnErase(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnErase(self, *_args, **_kwargs)
        return val
    def base_OnEraseContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnEraseContents(self, *_args, **_kwargs)
        return val
    def base_OnHighlight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnHighlight(self, *_args, **_kwargs)
        return val
    def base_OnLeftClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnLeftClick(self, *_args, **_kwargs)
        return val
    def base_OnLeftDoubleClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnLeftDoubleClick(self, *_args, **_kwargs)
        return val
    def base_OnRightClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnRightClick(self, *_args, **_kwargs)
        return val
    def base_OnSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnSize(self, *_args, **_kwargs)
        return val
    def base_OnMovePre(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnMovePre(self, *_args, **_kwargs)
        return val
    def base_OnMovePost(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnMovePost(self, *_args, **_kwargs)
        return val
    def base_OnDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnDragRight(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnBeginDragRight(self, *_args, **_kwargs)
        return val
    def base_OnEndDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnEndDragRight(self, *_args, **_kwargs)
        return val
    def base_OnDrawOutline(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnDrawOutline(self, *_args, **_kwargs)
        return val
    def base_OnDrawControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnDrawControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnEraseControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnEraseControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnMoveLink(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnMoveLink(self, *_args, **_kwargs)
        return val
    def base_OnSizingDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnSizingDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnSizingBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnSizingEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnBeginSize(self, *_args, **_kwargs)
        return val
    def base_OnEndSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyBitmapShape_base_OnEndSize(self, *_args, **_kwargs)
        return val
    def __repr__(self):
        return "<%s.%s instance; proxy of C++ wxPyBitmapShape instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this)
class wxPyBitmapShape(wxPyBitmapShapePtr):
    def __init__(self,*_args,**_kwargs):
        self.this = oglshapesc.new_wxPyBitmapShape(*_args,**_kwargs)
        self.thisown = 1
        self._setCallbackInfo(self, wxPyBitmapShape)
        self._setOORInfo(self)




class wxPyDrawnShapePtr(wxPyRectangleShapePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def _setCallbackInfo(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape__setCallbackInfo(self, *_args, **_kwargs)
        return val
    def CalculateSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_CalculateSize(self, *_args, **_kwargs)
        return val
    def DestroyClippingRect(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_DestroyClippingRect(self, *_args, **_kwargs)
        return val
    def DrawArc(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_DrawArc(self, *_args, **_kwargs)
        return val
    def DrawAtAngle(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_DrawAtAngle(self, *_args, **_kwargs)
        return val
    def DrawEllipticArc(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_DrawEllipticArc(self, *_args, **_kwargs)
        return val
    def DrawLine(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_DrawLine(self, *_args, **_kwargs)
        return val
    def DrawLines(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_DrawLines(self, *_args, **_kwargs)
        return val
    def DrawPoint(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_DrawPoint(self, *_args, **_kwargs)
        return val
    def DrawPolygon(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_DrawPolygon(self, *_args, **_kwargs)
        return val
    def DrawRectangle(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_DrawRectangle(self, *_args, **_kwargs)
        return val
    def DrawRoundedRectangle(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_DrawRoundedRectangle(self, *_args, **_kwargs)
        return val
    def DrawSpline(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_DrawSpline(self, *_args, **_kwargs)
        return val
    def DrawText(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_DrawText(self, *_args, **_kwargs)
        return val
    def GetAngle(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_GetAngle(self, *_args, **_kwargs)
        return val
    def GetMetaFile(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_GetMetaFile(self, *_args, **_kwargs)
        return val
    def GetRotation(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_GetRotation(self, *_args, **_kwargs)
        return val
    def LoadFromMetaFile(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_LoadFromMetaFile(self, *_args, **_kwargs)
        return val
    def Rotate(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_Rotate(self, *_args, **_kwargs)
        return val
    def SetClippingRect(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_SetClippingRect(self, *_args, **_kwargs)
        return val
    def SetDrawnBackgroundColour(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_SetDrawnBackgroundColour(self, *_args, **_kwargs)
        return val
    def SetDrawnBackgroundMode(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_SetDrawnBackgroundMode(self, *_args, **_kwargs)
        return val
    def SetDrawnBrush(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_SetDrawnBrush(self, *_args, **_kwargs)
        return val
    def SetDrawnFont(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_SetDrawnFont(self, *_args, **_kwargs)
        return val
    def SetDrawnPen(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_SetDrawnPen(self, *_args, **_kwargs)
        return val
    def SetDrawnTextColour(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_SetDrawnTextColour(self, *_args, **_kwargs)
        return val
    def Scale(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_Scale(self, *_args, **_kwargs)
        return val
    def SetSaveToFile(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_SetSaveToFile(self, *_args, **_kwargs)
        return val
    def Translate(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_Translate(self, *_args, **_kwargs)
        return val
    def base_OnDelete(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnDelete(self, *_args, **_kwargs)
        return val
    def base_OnDraw(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnDraw(self, *_args, **_kwargs)
        return val
    def base_OnDrawContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnDrawContents(self, *_args, **_kwargs)
        return val
    def base_OnDrawBranches(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnDrawBranches(self, *_args, **_kwargs)
        return val
    def base_OnMoveLinks(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnMoveLinks(self, *_args, **_kwargs)
        return val
    def base_OnErase(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnErase(self, *_args, **_kwargs)
        return val
    def base_OnEraseContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnEraseContents(self, *_args, **_kwargs)
        return val
    def base_OnHighlight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnHighlight(self, *_args, **_kwargs)
        return val
    def base_OnLeftClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnLeftClick(self, *_args, **_kwargs)
        return val
    def base_OnLeftDoubleClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnLeftDoubleClick(self, *_args, **_kwargs)
        return val
    def base_OnRightClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnRightClick(self, *_args, **_kwargs)
        return val
    def base_OnSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnSize(self, *_args, **_kwargs)
        return val
    def base_OnMovePre(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnMovePre(self, *_args, **_kwargs)
        return val
    def base_OnMovePost(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnMovePost(self, *_args, **_kwargs)
        return val
    def base_OnDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnDragRight(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnBeginDragRight(self, *_args, **_kwargs)
        return val
    def base_OnEndDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnEndDragRight(self, *_args, **_kwargs)
        return val
    def base_OnDrawOutline(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnDrawOutline(self, *_args, **_kwargs)
        return val
    def base_OnDrawControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnDrawControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnEraseControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnEraseControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnMoveLink(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnMoveLink(self, *_args, **_kwargs)
        return val
    def base_OnSizingDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnSizingDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnSizingBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnSizingEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnBeginSize(self, *_args, **_kwargs)
        return val
    def base_OnEndSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDrawnShape_base_OnEndSize(self, *_args, **_kwargs)
        return val
    def __repr__(self):
        return "<%s.%s instance; proxy of C++ wxPyDrawnShape instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this)
class wxPyDrawnShape(wxPyDrawnShapePtr):
    def __init__(self,*_args,**_kwargs):
        self.this = oglshapesc.new_wxPyDrawnShape(*_args,**_kwargs)
        self.thisown = 1
        self._setCallbackInfo(self, wxPyDrawnShape)
        self._setOORInfo(self)




class wxOGLConstraintPtr(wxObjectPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Evaluate(self, *_args, **_kwargs):
        val = oglshapesc.wxOGLConstraint_Evaluate(self, *_args, **_kwargs)
        return val
    def SetSpacing(self, *_args, **_kwargs):
        val = oglshapesc.wxOGLConstraint_SetSpacing(self, *_args, **_kwargs)
        return val
    def Equals(self, *_args, **_kwargs):
        val = oglshapesc.wxOGLConstraint_Equals(self, *_args, **_kwargs)
        return val
    def __repr__(self):
        return "<%s.%s instance; proxy of C++ wxOGLConstraint instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this)
class wxOGLConstraint(wxOGLConstraintPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = oglshapesc.new_wxOGLConstraint(*_args,**_kwargs)
        self.thisown = 1




class wxPyCompositeShapePtr(wxPyRectangleShapePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def _setCallbackInfo(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape__setCallbackInfo(self, *_args, **_kwargs)
        return val
    def AddChild(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_AddChild(self, *_args, **_kwargs)
        return val
    def AddConstraint(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_AddConstraint(self, *_args, **_kwargs)
        return val
    def AddConstrainedShapes(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_AddConstrainedShapes(self, *_args, **_kwargs)
        return val
    def AddSimpleConstraint(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_AddSimpleConstraint(self, *_args, **_kwargs)
        return val
    def CalculateSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_CalculateSize(self, *_args, **_kwargs)
        return val
    def ContainsDivision(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_ContainsDivision(self, *_args, **_kwargs)
        return val
    def DeleteConstraint(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_DeleteConstraint(self, *_args, **_kwargs)
        return val
    def DeleteConstraintsInvolvingChild(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_DeleteConstraintsInvolvingChild(self, *_args, **_kwargs)
        return val
    def FindContainerImage(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_FindContainerImage(self, *_args, **_kwargs)
        return val
    def GetConstraints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_GetConstraints(self, *_args, **_kwargs)
        return val
    def GetDivisions(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_GetDivisions(self, *_args, **_kwargs)
        return val
    def MakeContainer(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_MakeContainer(self, *_args, **_kwargs)
        return val
    def Recompute(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_Recompute(self, *_args, **_kwargs)
        return val
    def RemoveChild(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_RemoveChild(self, *_args, **_kwargs)
        return val
    def base_OnDelete(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnDelete(self, *_args, **_kwargs)
        return val
    def base_OnDraw(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnDraw(self, *_args, **_kwargs)
        return val
    def base_OnDrawContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnDrawContents(self, *_args, **_kwargs)
        return val
    def base_OnDrawBranches(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnDrawBranches(self, *_args, **_kwargs)
        return val
    def base_OnMoveLinks(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnMoveLinks(self, *_args, **_kwargs)
        return val
    def base_OnErase(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnErase(self, *_args, **_kwargs)
        return val
    def base_OnEraseContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnEraseContents(self, *_args, **_kwargs)
        return val
    def base_OnHighlight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnHighlight(self, *_args, **_kwargs)
        return val
    def base_OnLeftClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnLeftClick(self, *_args, **_kwargs)
        return val
    def base_OnLeftDoubleClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnLeftDoubleClick(self, *_args, **_kwargs)
        return val
    def base_OnRightClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnRightClick(self, *_args, **_kwargs)
        return val
    def base_OnSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnSize(self, *_args, **_kwargs)
        return val
    def base_OnMovePre(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnMovePre(self, *_args, **_kwargs)
        return val
    def base_OnMovePost(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnMovePost(self, *_args, **_kwargs)
        return val
    def base_OnDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnDragRight(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnBeginDragRight(self, *_args, **_kwargs)
        return val
    def base_OnEndDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnEndDragRight(self, *_args, **_kwargs)
        return val
    def base_OnDrawOutline(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnDrawOutline(self, *_args, **_kwargs)
        return val
    def base_OnDrawControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnDrawControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnEraseControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnEraseControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnMoveLink(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnMoveLink(self, *_args, **_kwargs)
        return val
    def base_OnSizingDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnSizingDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnSizingBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnSizingEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnBeginSize(self, *_args, **_kwargs)
        return val
    def base_OnEndSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyCompositeShape_base_OnEndSize(self, *_args, **_kwargs)
        return val
    def __repr__(self):
        return "<%s.%s instance; proxy of C++ wxPyCompositeShape instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this)
class wxPyCompositeShape(wxPyCompositeShapePtr):
    def __init__(self,*_args,**_kwargs):
        self.this = oglshapesc.new_wxPyCompositeShape(*_args,**_kwargs)
        self.thisown = 1
        self._setCallbackInfo(self, wxPyCompositeShape)
        self._setOORInfo(self)




class wxPyDividedShapePtr(wxPyRectangleShapePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def _setCallbackInfo(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape__setCallbackInfo(self, *_args, **_kwargs)
        return val
    def EditRegions(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_EditRegions(self, *_args, **_kwargs)
        return val
    def SetRegionSizes(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_SetRegionSizes(self, *_args, **_kwargs)
        return val
    def base_OnDelete(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnDelete(self, *_args, **_kwargs)
        return val
    def base_OnDraw(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnDraw(self, *_args, **_kwargs)
        return val
    def base_OnDrawContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnDrawContents(self, *_args, **_kwargs)
        return val
    def base_OnDrawBranches(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnDrawBranches(self, *_args, **_kwargs)
        return val
    def base_OnMoveLinks(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnMoveLinks(self, *_args, **_kwargs)
        return val
    def base_OnErase(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnErase(self, *_args, **_kwargs)
        return val
    def base_OnEraseContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnEraseContents(self, *_args, **_kwargs)
        return val
    def base_OnHighlight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnHighlight(self, *_args, **_kwargs)
        return val
    def base_OnLeftClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnLeftClick(self, *_args, **_kwargs)
        return val
    def base_OnLeftDoubleClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnLeftDoubleClick(self, *_args, **_kwargs)
        return val
    def base_OnRightClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnRightClick(self, *_args, **_kwargs)
        return val
    def base_OnSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnSize(self, *_args, **_kwargs)
        return val
    def base_OnMovePre(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnMovePre(self, *_args, **_kwargs)
        return val
    def base_OnMovePost(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnMovePost(self, *_args, **_kwargs)
        return val
    def base_OnDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnDragRight(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnBeginDragRight(self, *_args, **_kwargs)
        return val
    def base_OnEndDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnEndDragRight(self, *_args, **_kwargs)
        return val
    def base_OnDrawOutline(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnDrawOutline(self, *_args, **_kwargs)
        return val
    def base_OnDrawControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnDrawControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnEraseControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnEraseControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnMoveLink(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnMoveLink(self, *_args, **_kwargs)
        return val
    def base_OnSizingDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnSizingDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnSizingBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnSizingEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnBeginSize(self, *_args, **_kwargs)
        return val
    def base_OnEndSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDividedShape_base_OnEndSize(self, *_args, **_kwargs)
        return val
    def __repr__(self):
        return "<%s.%s instance; proxy of C++ wxPyDividedShape instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this)
class wxPyDividedShape(wxPyDividedShapePtr):
    def __init__(self,*_args,**_kwargs):
        self.this = oglshapesc.new_wxPyDividedShape(*_args,**_kwargs)
        self.thisown = 1
        self._setCallbackInfo(self, wxPyDividedShape)
        self._setOORInfo(self)




class wxPyDivisionShapePtr(wxPyCompositeShapePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def _setCallbackInfo(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape__setCallbackInfo(self, *_args, **_kwargs)
        return val
    def AdjustBottom(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_AdjustBottom(self, *_args, **_kwargs)
        return val
    def AdjustLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_AdjustLeft(self, *_args, **_kwargs)
        return val
    def AdjustRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_AdjustRight(self, *_args, **_kwargs)
        return val
    def AdjustTop(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_AdjustTop(self, *_args, **_kwargs)
        return val
    def Divide(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_Divide(self, *_args, **_kwargs)
        return val
    def EditEdge(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_EditEdge(self, *_args, **_kwargs)
        return val
    def GetBottomSide(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_GetBottomSide(self, *_args, **_kwargs)
        return val
    def GetHandleSide(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_GetHandleSide(self, *_args, **_kwargs)
        return val
    def GetLeftSide(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_GetLeftSide(self, *_args, **_kwargs)
        return val
    def GetLeftSideColour(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_GetLeftSideColour(self, *_args, **_kwargs)
        return val
    def GetLeftSidePen(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_GetLeftSidePen(self, *_args, **_kwargs)
        if val: val = wxPenPtr(val) 
        return val
    def GetRightSide(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_GetRightSide(self, *_args, **_kwargs)
        return val
    def GetTopSide(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_GetTopSide(self, *_args, **_kwargs)
        return val
    def GetTopSidePen(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_GetTopSidePen(self, *_args, **_kwargs)
        if val: val = wxPenPtr(val) 
        return val
    def ResizeAdjoining(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_ResizeAdjoining(self, *_args, **_kwargs)
        return val
    def PopupMenu(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_PopupMenu(self, *_args, **_kwargs)
        return val
    def SetBottomSide(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_SetBottomSide(self, *_args, **_kwargs)
        return val
    def SetHandleSide(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_SetHandleSide(self, *_args, **_kwargs)
        return val
    def SetLeftSide(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_SetLeftSide(self, *_args, **_kwargs)
        return val
    def SetLeftSideColour(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_SetLeftSideColour(self, *_args, **_kwargs)
        return val
    def SetLeftSidePen(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_SetLeftSidePen(self, *_args, **_kwargs)
        return val
    def SetRightSide(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_SetRightSide(self, *_args, **_kwargs)
        return val
    def SetTopSide(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_SetTopSide(self, *_args, **_kwargs)
        return val
    def SetTopSideColour(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_SetTopSideColour(self, *_args, **_kwargs)
        return val
    def SetTopSidePen(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_SetTopSidePen(self, *_args, **_kwargs)
        return val
    def base_OnDelete(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnDelete(self, *_args, **_kwargs)
        return val
    def base_OnDraw(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnDraw(self, *_args, **_kwargs)
        return val
    def base_OnDrawContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnDrawContents(self, *_args, **_kwargs)
        return val
    def base_OnDrawBranches(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnDrawBranches(self, *_args, **_kwargs)
        return val
    def base_OnMoveLinks(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnMoveLinks(self, *_args, **_kwargs)
        return val
    def base_OnErase(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnErase(self, *_args, **_kwargs)
        return val
    def base_OnEraseContents(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnEraseContents(self, *_args, **_kwargs)
        return val
    def base_OnHighlight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnHighlight(self, *_args, **_kwargs)
        return val
    def base_OnLeftClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnLeftClick(self, *_args, **_kwargs)
        return val
    def base_OnLeftDoubleClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnLeftDoubleClick(self, *_args, **_kwargs)
        return val
    def base_OnRightClick(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnRightClick(self, *_args, **_kwargs)
        return val
    def base_OnSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnSize(self, *_args, **_kwargs)
        return val
    def base_OnMovePre(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnMovePre(self, *_args, **_kwargs)
        return val
    def base_OnMovePost(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnMovePost(self, *_args, **_kwargs)
        return val
    def base_OnDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnDragRight(self, *_args, **_kwargs)
        return val
    def base_OnBeginDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnBeginDragRight(self, *_args, **_kwargs)
        return val
    def base_OnEndDragRight(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnEndDragRight(self, *_args, **_kwargs)
        return val
    def base_OnDrawOutline(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnDrawOutline(self, *_args, **_kwargs)
        return val
    def base_OnDrawControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnDrawControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnEraseControlPoints(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnEraseControlPoints(self, *_args, **_kwargs)
        return val
    def base_OnMoveLink(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnMoveLink(self, *_args, **_kwargs)
        return val
    def base_OnSizingDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnSizingDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingBeginDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnSizingBeginDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnSizingEndDragLeft(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnSizingEndDragLeft(self, *_args, **_kwargs)
        return val
    def base_OnBeginSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnBeginSize(self, *_args, **_kwargs)
        return val
    def base_OnEndSize(self, *_args, **_kwargs):
        val = oglshapesc.wxPyDivisionShape_base_OnEndSize(self, *_args, **_kwargs)
        return val
    def __repr__(self):
        return "<%s.%s instance; proxy of C++ wxPyDivisionShape instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this)
class wxPyDivisionShape(wxPyDivisionShapePtr):
    def __init__(self,*_args,**_kwargs):
        self.this = oglshapesc.new_wxPyDivisionShape(*_args,**_kwargs)
        self.thisown = 1
        self._setCallbackInfo(self, wxPyDivisionShape)
        self._setOORInfo(self)






#-------------- FUNCTION WRAPPERS ------------------



#-------------- VARIABLE WRAPPERS ------------------

oglMETAFLAGS_OUTLINE = oglshapesc.oglMETAFLAGS_OUTLINE
oglMETAFLAGS_ATTACHMENTS = oglshapesc.oglMETAFLAGS_ATTACHMENTS

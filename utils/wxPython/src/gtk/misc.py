# This file was created automatically by SWIG.
import miscc
class wxSizePtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            miscc.delete_wxSize(self.this)
    def Set(self,arg0,arg1):
        val = miscc.wxSize_Set(self.this,arg0,arg1)
        return val
    def GetWidth(self):
        val = miscc.wxSize_GetWidth(self.this)
        return val
    def GetHeight(self):
        val = miscc.wxSize_GetHeight(self.this)
        return val
    def __str__(self):
        val = miscc.wxSize___str__(self.this)
        return val
    def __setattr__(self,name,value):
        if name == "width" :
            miscc.wxSize_width_set(self.this,value)
            return
        if name == "height" :
            miscc.wxSize_height_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "width" : 
            return miscc.wxSize_width_get(self.this)
        if name == "height" : 
            return miscc.wxSize_height_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C wxSize instance>"
class wxSize(wxSizePtr):
    def __init__(self,*args) :
        self.this = apply(miscc.new_wxSize,()+args)
        self.thisown = 1




class wxRealPointPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            miscc.delete_wxRealPoint(self.this)
    def __setattr__(self,name,value):
        if name == "x" :
            miscc.wxRealPoint_x_set(self.this,value)
            return
        if name == "y" :
            miscc.wxRealPoint_y_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "x" : 
            return miscc.wxRealPoint_x_get(self.this)
        if name == "y" : 
            return miscc.wxRealPoint_y_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C wxRealPoint instance>"
class wxRealPoint(wxRealPointPtr):
    def __init__(self,*args) :
        self.this = apply(miscc.new_wxRealPoint,()+args)
        self.thisown = 1




class wxPointPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            miscc.delete_wxPoint(self.this)
    def Set(self,arg0,arg1):
        val = miscc.wxPoint_Set(self.this,arg0,arg1)
        return val
    def __str__(self):
        val = miscc.wxPoint___str__(self.this)
        return val
    def __setattr__(self,name,value):
        if name == "x" :
            miscc.wxPoint_x_set(self.this,value)
            return
        if name == "y" :
            miscc.wxPoint_y_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "x" : 
            return miscc.wxPoint_x_get(self.this)
        if name == "y" : 
            return miscc.wxPoint_y_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C wxPoint instance>"
class wxPoint(wxPointPtr):
    def __init__(self,*args) :
        self.this = apply(miscc.new_wxPoint,()+args)
        self.thisown = 1




class wxRectPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            miscc.delete_wxRect(self.this)
    def GetX(self):
        val = miscc.wxRect_GetX(self.this)
        return val
    def SetX(self,arg0):
        val = miscc.wxRect_SetX(self.this,arg0)
        return val
    def GetY(self):
        val = miscc.wxRect_GetY(self.this)
        return val
    def SetY(self,arg0):
        val = miscc.wxRect_SetY(self.this,arg0)
        return val
    def GetWidth(self):
        val = miscc.wxRect_GetWidth(self.this)
        return val
    def SetWidth(self,arg0):
        val = miscc.wxRect_SetWidth(self.this,arg0)
        return val
    def GetHeight(self):
        val = miscc.wxRect_GetHeight(self.this)
        return val
    def SetHeight(self,arg0):
        val = miscc.wxRect_SetHeight(self.this,arg0)
        return val
    def GetPosition(self):
        val = miscc.wxRect_GetPosition(self.this)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def GetSize(self):
        val = miscc.wxRect_GetSize(self.this)
        val = wxSizePtr(val)
        val.thisown = 1
        return val
    def GetLeft(self):
        val = miscc.wxRect_GetLeft(self.this)
        return val
    def GetTop(self):
        val = miscc.wxRect_GetTop(self.this)
        return val
    def GetBottom(self):
        val = miscc.wxRect_GetBottom(self.this)
        return val
    def GetRight(self):
        val = miscc.wxRect_GetRight(self.this)
        return val
    def __setattr__(self,name,value):
        if name == "x" :
            miscc.wxRect_x_set(self.this,value)
            return
        if name == "y" :
            miscc.wxRect_y_set(self.this,value)
            return
        if name == "width" :
            miscc.wxRect_width_set(self.this,value)
            return
        if name == "height" :
            miscc.wxRect_height_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "x" : 
            return miscc.wxRect_x_get(self.this)
        if name == "y" : 
            return miscc.wxRect_y_get(self.this)
        if name == "width" : 
            return miscc.wxRect_width_get(self.this)
        if name == "height" : 
            return miscc.wxRect_height_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C wxRect instance>"
class wxRect(wxRectPtr):
    def __init__(self,*args) :
        self.this = apply(miscc.new_wxRect,()+args)
        self.thisown = 1




class wxPyTimerPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            miscc.delete_wxPyTimer(self.this)
    def Interval(self):
        val = miscc.wxPyTimer_Interval(self.this)
        return val
    def Start(self,*args):
        val = apply(miscc.wxPyTimer_Start,(self.this,)+args)
        return val
    def Stop(self):
        val = miscc.wxPyTimer_Stop(self.this)
        return val
    def __repr__(self):
        return "<C wxPyTimer instance>"
class wxPyTimer(wxPyTimerPtr):
    def __init__(self,arg0) :
        self.this = miscc.new_wxPyTimer(arg0)
        self.thisown = 1




class wxIndividualLayoutConstraintPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Above(self,arg0,*args):
        val = apply(miscc.wxIndividualLayoutConstraint_Above,(self.this,arg0.this,)+args)
        return val
    def Absolute(self,arg0):
        val = miscc.wxIndividualLayoutConstraint_Absolute(self.this,arg0)
        return val
    def AsIs(self):
        val = miscc.wxIndividualLayoutConstraint_AsIs(self.this)
        return val
    def Below(self,arg0,*args):
        val = apply(miscc.wxIndividualLayoutConstraint_Below,(self.this,arg0.this,)+args)
        return val
    def Unconstrained(self):
        val = miscc.wxIndividualLayoutConstraint_Unconstrained(self.this)
        return val
    def LeftOf(self,arg0,*args):
        val = apply(miscc.wxIndividualLayoutConstraint_LeftOf,(self.this,arg0.this,)+args)
        return val
    def PercentOf(self,arg0,arg1,arg2):
        val = miscc.wxIndividualLayoutConstraint_PercentOf(self.this,arg0.this,arg1,arg2)
        return val
    def RightOf(self,arg0,*args):
        val = apply(miscc.wxIndividualLayoutConstraint_RightOf,(self.this,arg0.this,)+args)
        return val
    def SameAs(self,arg0,arg1,*args):
        val = apply(miscc.wxIndividualLayoutConstraint_SameAs,(self.this,arg0.this,arg1,)+args)
        return val
    def Set(self,arg0,arg1,arg2,*args):
        val = apply(miscc.wxIndividualLayoutConstraint_Set,(self.this,arg0,arg1.this,arg2,)+args)
        return val
    def __repr__(self):
        return "<C wxIndividualLayoutConstraint instance>"
class wxIndividualLayoutConstraint(wxIndividualLayoutConstraintPtr):
    def __init__(self,this):
        self.this = this




class wxLayoutConstraintsPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __setattr__(self,name,value):
        if name == "bottom" :
            miscc.wxLayoutConstraints_bottom_set(self.this,value.this)
            return
        if name == "centreX" :
            miscc.wxLayoutConstraints_centreX_set(self.this,value.this)
            return
        if name == "centreY" :
            miscc.wxLayoutConstraints_centreY_set(self.this,value.this)
            return
        if name == "height" :
            miscc.wxLayoutConstraints_height_set(self.this,value.this)
            return
        if name == "left" :
            miscc.wxLayoutConstraints_left_set(self.this,value.this)
            return
        if name == "right" :
            miscc.wxLayoutConstraints_right_set(self.this,value.this)
            return
        if name == "top" :
            miscc.wxLayoutConstraints_top_set(self.this,value.this)
            return
        if name == "width" :
            miscc.wxLayoutConstraints_width_set(self.this,value.this)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "bottom" : 
            return wxIndividualLayoutConstraintPtr(miscc.wxLayoutConstraints_bottom_get(self.this))
        if name == "centreX" : 
            return wxIndividualLayoutConstraintPtr(miscc.wxLayoutConstraints_centreX_get(self.this))
        if name == "centreY" : 
            return wxIndividualLayoutConstraintPtr(miscc.wxLayoutConstraints_centreY_get(self.this))
        if name == "height" : 
            return wxIndividualLayoutConstraintPtr(miscc.wxLayoutConstraints_height_get(self.this))
        if name == "left" : 
            return wxIndividualLayoutConstraintPtr(miscc.wxLayoutConstraints_left_get(self.this))
        if name == "right" : 
            return wxIndividualLayoutConstraintPtr(miscc.wxLayoutConstraints_right_get(self.this))
        if name == "top" : 
            return wxIndividualLayoutConstraintPtr(miscc.wxLayoutConstraints_top_get(self.this))
        if name == "width" : 
            return wxIndividualLayoutConstraintPtr(miscc.wxLayoutConstraints_width_get(self.this))
        raise AttributeError,name
    def __repr__(self):
        return "<C wxLayoutConstraints instance>"
class wxLayoutConstraints(wxLayoutConstraintsPtr):
    def __init__(self) :
        self.this = miscc.new_wxLayoutConstraints()
        self.thisown = 1






#-------------- FUNCTION WRAPPERS ------------------

def wxFileSelector(arg0,*args):
    argl = map(None,args)
    try: argl[5] = argl[5].this
    except: pass
    args = tuple(argl)
    val = apply(miscc.wxFileSelector,(arg0,)+args)
    return val

def wxGetTextFromUser(arg0,*args):
    argl = map(None,args)
    try: argl[2] = argl[2].this
    except: pass
    args = tuple(argl)
    val = apply(miscc.wxGetTextFromUser,(arg0,)+args)
    return val

def wxGetSingleChoice(arg0,arg1,arg2,*args):
    argl = map(None,args)
    try: argl[0] = argl[0].this
    except: pass
    args = tuple(argl)
    val = apply(miscc.wxGetSingleChoice,(arg0,arg1,arg2,)+args)
    return val

def wxGetSingleChoiceIndex(arg0,arg1,arg2,*args):
    argl = map(None,args)
    try: argl[0] = argl[0].this
    except: pass
    args = tuple(argl)
    val = apply(miscc.wxGetSingleChoiceIndex,(arg0,arg1,arg2,)+args)
    return val

def wxMessageBox(arg0,*args):
    argl = map(None,args)
    try: argl[2] = argl[2].this
    except: pass
    args = tuple(argl)
    val = apply(miscc.wxMessageBox,(arg0,)+args)
    return val

wxColourDisplay = miscc.wxColourDisplay

wxDisplayDepth = miscc.wxDisplayDepth

def wxSetCursor(arg0):
    val = miscc.wxSetCursor(arg0.this)
    return val

NewId = miscc.NewId

RegisterId = miscc.RegisterId

def wxBeginBusyCursor(*args):
    argl = map(None,args)
    try: argl[0] = argl[0].this
    except: pass
    args = tuple(argl)
    val = apply(miscc.wxBeginBusyCursor,()+args)
    return val

wxBell = miscc.wxBell

wxDisplaySize = miscc.wxDisplaySize

wxEndBusyCursor = miscc.wxEndBusyCursor

wxExecute = miscc.wxExecute

def wxFindWindowByLabel(arg0,*args):
    argl = map(None,args)
    try: argl[0] = argl[0].this
    except: pass
    args = tuple(argl)
    val = apply(miscc.wxFindWindowByLabel,(arg0,)+args)
    val = wxWindowPtr(val)
    return val

def wxFindWindowByName(arg0,*args):
    argl = map(None,args)
    try: argl[0] = argl[0].this
    except: pass
    args = tuple(argl)
    val = apply(miscc.wxFindWindowByName,(arg0,)+args)
    val = wxWindowPtr(val)
    return val

wxGetMousePosition = miscc.wxGetMousePosition

wxIsBusy = miscc.wxIsBusy

wxNow = miscc.wxNow

wxYield = miscc.wxYield

wxGetResource = miscc.wxGetResource

wxResourceAddIdentifier = miscc.wxResourceAddIdentifier

wxResourceClear = miscc.wxResourceClear

def wxResourceCreateBitmap(arg0):
    val = miscc.wxResourceCreateBitmap(arg0)
    val = wxBitmapPtr(val)
    val.thisown = 1
    return val

def wxResourceCreateIcon(arg0):
    val = miscc.wxResourceCreateIcon(arg0)
    val = wxIconPtr(val)
    val.thisown = 1
    return val

def wxResourceCreateMenuBar(arg0):
    val = miscc.wxResourceCreateMenuBar(arg0)
    val = wxMenuBarPtr(val)
    return val

wxResourceGetIdentifier = miscc.wxResourceGetIdentifier

wxResourceParseData = miscc.wxResourceParseData

wxResourceParseFile = miscc.wxResourceParseFile

wxResourceParseString = miscc.wxResourceParseString



#-------------- VARIABLE WRAPPERS ------------------

wxLeft = miscc.wxLeft
wxTop = miscc.wxTop
wxRight = miscc.wxRight
wxBottom = miscc.wxBottom
wxWidth = miscc.wxWidth
wxHeight = miscc.wxHeight
wxCentre = miscc.wxCentre
wxCenter = miscc.wxCenter
wxCentreX = miscc.wxCentreX
wxCentreY = miscc.wxCentreY
wxUnconstrained = miscc.wxUnconstrained
wxAsIs = miscc.wxAsIs
wxPercentOf = miscc.wxPercentOf
wxAbove = miscc.wxAbove
wxBelow = miscc.wxBelow
wxLeftOf = miscc.wxLeftOf
wxRightOf = miscc.wxRightOf
wxSameAs = miscc.wxSameAs
wxAbsolute = miscc.wxAbsolute

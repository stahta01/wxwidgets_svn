# This file was created automatically by SWIG.
import cmndlgsc

from misc import *

from gdi import *

from windows import *
class wxColourDataPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            cmndlgsc.delete_wxColourData(self.this)
    def GetChooseFull(self):
        val = cmndlgsc.wxColourData_GetChooseFull(self.this)
        return val
    def GetColour(self):
        val = cmndlgsc.wxColourData_GetColour(self.this)
        val = wxColourPtr(val)
        return val
    def GetCustomColour(self,arg0):
        val = cmndlgsc.wxColourData_GetCustomColour(self.this,arg0)
        val = wxColourPtr(val)
        val.thisown = 1
        return val
    def SetChooseFull(self,arg0):
        val = cmndlgsc.wxColourData_SetChooseFull(self.this,arg0)
        return val
    def SetColour(self,arg0):
        val = cmndlgsc.wxColourData_SetColour(self.this,arg0.this)
        return val
    def SetCustomColour(self,arg0,arg1):
        val = cmndlgsc.wxColourData_SetCustomColour(self.this,arg0,arg1.this)
        return val
    def __repr__(self):
        return "<C wxColourData instance>"
class wxColourData(wxColourDataPtr):
    def __init__(self) :
        self.this = cmndlgsc.new_wxColourData()
        self.thisown = 1




class wxColourDialogPtr(wxDialogPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetColourData(self):
        val = cmndlgsc.wxColourDialog_GetColourData(self.this)
        val = wxColourDataPtr(val)
        return val
    def ShowModal(self):
        val = cmndlgsc.wxColourDialog_ShowModal(self.this)
        return val
    def __repr__(self):
        return "<C wxColourDialog instance>"
class wxColourDialog(wxColourDialogPtr):
    def __init__(self,arg0,*args) :
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        args = tuple(argl)
        self.this = apply(cmndlgsc.new_wxColourDialog,(arg0.this,)+args)
        self.thisown = 1
        wxp._StdDialogCallbacks(self)




class wxDirDialogPtr(wxDialogPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetPath(self):
        val = cmndlgsc.wxDirDialog_GetPath(self.this)
        return val
    def GetMessage(self):
        val = cmndlgsc.wxDirDialog_GetMessage(self.this)
        return val
    def GetStyle(self):
        val = cmndlgsc.wxDirDialog_GetStyle(self.this)
        return val
    def SetMessage(self,arg0):
        val = cmndlgsc.wxDirDialog_SetMessage(self.this,arg0)
        return val
    def SetPath(self,arg0):
        val = cmndlgsc.wxDirDialog_SetPath(self.this,arg0)
        return val
    def ShowModal(self):
        val = cmndlgsc.wxDirDialog_ShowModal(self.this)
        return val
    def __repr__(self):
        return "<C wxDirDialog instance>"
class wxDirDialog(wxDirDialogPtr):
    def __init__(self,arg0,*args) :
        argl = map(None,args)
        try: argl[3] = argl[3].this
        except: pass
        args = tuple(argl)
        self.this = apply(cmndlgsc.new_wxDirDialog,(arg0.this,)+args)
        self.thisown = 1
        wxp._StdDialogCallbacks(self)




class wxFileDialogPtr(wxDialogPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetDirectory(self):
        val = cmndlgsc.wxFileDialog_GetDirectory(self.this)
        return val
    def GetFilename(self):
        val = cmndlgsc.wxFileDialog_GetFilename(self.this)
        return val
    def GetFilterIndex(self):
        val = cmndlgsc.wxFileDialog_GetFilterIndex(self.this)
        return val
    def GetMessage(self):
        val = cmndlgsc.wxFileDialog_GetMessage(self.this)
        return val
    def GetPath(self):
        val = cmndlgsc.wxFileDialog_GetPath(self.this)
        return val
    def GetStyle(self):
        val = cmndlgsc.wxFileDialog_GetStyle(self.this)
        return val
    def GetWildcard(self):
        val = cmndlgsc.wxFileDialog_GetWildcard(self.this)
        return val
    def SetDirectory(self,arg0):
        val = cmndlgsc.wxFileDialog_SetDirectory(self.this,arg0)
        return val
    def SetFilename(self,arg0):
        val = cmndlgsc.wxFileDialog_SetFilename(self.this,arg0)
        return val
    def SetFilterIndex(self,arg0):
        val = cmndlgsc.wxFileDialog_SetFilterIndex(self.this,arg0)
        return val
    def SetMessage(self,arg0):
        val = cmndlgsc.wxFileDialog_SetMessage(self.this,arg0)
        return val
    def SetPath(self,arg0):
        val = cmndlgsc.wxFileDialog_SetPath(self.this,arg0)
        return val
    def SetStyle(self,arg0):
        val = cmndlgsc.wxFileDialog_SetStyle(self.this,arg0)
        return val
    def SetWildcard(self,arg0):
        val = cmndlgsc.wxFileDialog_SetWildcard(self.this,arg0)
        return val
    def ShowModal(self):
        val = cmndlgsc.wxFileDialog_ShowModal(self.this)
        return val
    def __repr__(self):
        return "<C wxFileDialog instance>"
class wxFileDialog(wxFileDialogPtr):
    def __init__(self,arg0,*args) :
        argl = map(None,args)
        try: argl[5] = argl[5].this
        except: pass
        args = tuple(argl)
        self.this = apply(cmndlgsc.new_wxFileDialog,(arg0.this,)+args)
        self.thisown = 1
        wxp._StdDialogCallbacks(self)




class wxSingleChoiceDialogPtr(wxDialogPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetSelection(self):
        val = cmndlgsc.wxSingleChoiceDialog_GetSelection(self.this)
        return val
    def GetStringSelection(self):
        val = cmndlgsc.wxSingleChoiceDialog_GetStringSelection(self.this)
        return val
    def SetSelection(self,arg0):
        val = cmndlgsc.wxSingleChoiceDialog_SetSelection(self.this,arg0)
        return val
    def ShowModal(self):
        val = cmndlgsc.wxSingleChoiceDialog_ShowModal(self.this)
        return val
    def __repr__(self):
        return "<C wxSingleChoiceDialog instance>"
class wxSingleChoiceDialog(wxSingleChoiceDialogPtr):
    def __init__(self,arg0,arg1,arg2,arg3,*args) :
        argl = map(None,args)
        try: argl[1] = argl[1].this
        except: pass
        args = tuple(argl)
        self.this = apply(cmndlgsc.new_wxSingleChoiceDialog,(arg0.this,arg1,arg2,arg3,)+args)
        self.thisown = 1
        wxp._StdDialogCallbacks(self)




class wxTextEntryDialogPtr(wxDialogPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetValue(self):
        val = cmndlgsc.wxTextEntryDialog_GetValue(self.this)
        return val
    def SetValue(self,arg0):
        val = cmndlgsc.wxTextEntryDialog_SetValue(self.this,arg0)
        return val
    def ShowModal(self):
        val = cmndlgsc.wxTextEntryDialog_ShowModal(self.this)
        return val
    def __repr__(self):
        return "<C wxTextEntryDialog instance>"
class wxTextEntryDialog(wxTextEntryDialogPtr):
    def __init__(self,arg0,arg1,*args) :
        argl = map(None,args)
        try: argl[3] = argl[3].this
        except: pass
        args = tuple(argl)
        self.this = apply(cmndlgsc.new_wxTextEntryDialog,(arg0.this,arg1,)+args)
        self.thisown = 1
        wxp._StdDialogCallbacks(self)




class wxFontDataPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            cmndlgsc.delete_wxFontData(self.this)
    def EnableEffects(self,arg0):
        val = cmndlgsc.wxFontData_EnableEffects(self.this,arg0)
        return val
    def GetAllowSymbols(self):
        val = cmndlgsc.wxFontData_GetAllowSymbols(self.this)
        return val
    def GetColour(self):
        val = cmndlgsc.wxFontData_GetColour(self.this)
        val = wxColourPtr(val)
        return val
    def GetChosenFont(self):
        val = cmndlgsc.wxFontData_GetChosenFont(self.this)
        val = wxFontPtr(val)
        val.thisown = 1
        return val
    def GetEnableEffects(self):
        val = cmndlgsc.wxFontData_GetEnableEffects(self.this)
        return val
    def GetInitialFont(self):
        val = cmndlgsc.wxFontData_GetInitialFont(self.this)
        val = wxFontPtr(val)
        val.thisown = 1
        return val
    def GetShowHelp(self):
        val = cmndlgsc.wxFontData_GetShowHelp(self.this)
        return val
    def SetAllowSymbols(self,arg0):
        val = cmndlgsc.wxFontData_SetAllowSymbols(self.this,arg0)
        return val
    def SetChosenFont(self,arg0):
        val = cmndlgsc.wxFontData_SetChosenFont(self.this,arg0.this)
        return val
    def SetColour(self,arg0):
        val = cmndlgsc.wxFontData_SetColour(self.this,arg0.this)
        return val
    def SetInitialFont(self,arg0):
        val = cmndlgsc.wxFontData_SetInitialFont(self.this,arg0.this)
        return val
    def SetRange(self,arg0,arg1):
        val = cmndlgsc.wxFontData_SetRange(self.this,arg0,arg1)
        return val
    def SetShowHelp(self,arg0):
        val = cmndlgsc.wxFontData_SetShowHelp(self.this,arg0)
        return val
    def __repr__(self):
        return "<C wxFontData instance>"
class wxFontData(wxFontDataPtr):
    def __init__(self) :
        self.this = cmndlgsc.new_wxFontData()
        self.thisown = 1




class wxFontDialogPtr(wxDialogPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetFontData(self):
        val = cmndlgsc.wxFontDialog_GetFontData(self.this)
        val = wxFontDataPtr(val)
        return val
    def ShowModal(self):
        val = cmndlgsc.wxFontDialog_ShowModal(self.this)
        return val
    def __repr__(self):
        return "<C wxFontDialog instance>"
class wxFontDialog(wxFontDialogPtr):
    def __init__(self,arg0,*args) :
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        args = tuple(argl)
        self.this = apply(cmndlgsc.new_wxFontDialog,(arg0.this,)+args)
        self.thisown = 1
        wxp._StdDialogCallbacks(self)




class wxPageSetupDataPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            cmndlgsc.delete_wxPageSetupData(self.this)
    def EnableHelp(self,arg0):
        val = cmndlgsc.wxPageSetupData_EnableHelp(self.this,arg0)
        return val
    def EnableMargins(self,arg0):
        val = cmndlgsc.wxPageSetupData_EnableMargins(self.this,arg0)
        return val
    def EnableOrientation(self,arg0):
        val = cmndlgsc.wxPageSetupData_EnableOrientation(self.this,arg0)
        return val
    def EnablePaper(self,arg0):
        val = cmndlgsc.wxPageSetupData_EnablePaper(self.this,arg0)
        return val
    def EnablePrinter(self,arg0):
        val = cmndlgsc.wxPageSetupData_EnablePrinter(self.this,arg0)
        return val
    def GetPaperSize(self):
        val = cmndlgsc.wxPageSetupData_GetPaperSize(self.this)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def GetMarginTopLeft(self):
        val = cmndlgsc.wxPageSetupData_GetMarginTopLeft(self.this)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def GetMarginBottomRight(self):
        val = cmndlgsc.wxPageSetupData_GetMarginBottomRight(self.this)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def GetMinMarginTopLeft(self):
        val = cmndlgsc.wxPageSetupData_GetMinMarginTopLeft(self.this)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def GetMinMarginBottomRight(self):
        val = cmndlgsc.wxPageSetupData_GetMinMarginBottomRight(self.this)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def GetOrientation(self):
        val = cmndlgsc.wxPageSetupData_GetOrientation(self.this)
        return val
    def GetDefaultMinMargins(self):
        val = cmndlgsc.wxPageSetupData_GetDefaultMinMargins(self.this)
        return val
    def GetEnableMargins(self):
        val = cmndlgsc.wxPageSetupData_GetEnableMargins(self.this)
        return val
    def GetEnableOrientation(self):
        val = cmndlgsc.wxPageSetupData_GetEnableOrientation(self.this)
        return val
    def GetEnablePaper(self):
        val = cmndlgsc.wxPageSetupData_GetEnablePaper(self.this)
        return val
    def GetEnablePrinter(self):
        val = cmndlgsc.wxPageSetupData_GetEnablePrinter(self.this)
        return val
    def GetEnableHelp(self):
        val = cmndlgsc.wxPageSetupData_GetEnableHelp(self.this)
        return val
    def GetDefaultInfo(self):
        val = cmndlgsc.wxPageSetupData_GetDefaultInfo(self.this)
        return val
    def SetPaperSize(self,arg0):
        val = cmndlgsc.wxPageSetupData_SetPaperSize(self.this,arg0.this)
        return val
    def SetMarginTopLeft(self,arg0):
        val = cmndlgsc.wxPageSetupData_SetMarginTopLeft(self.this,arg0.this)
        return val
    def SetMarginBottomRight(self,arg0):
        val = cmndlgsc.wxPageSetupData_SetMarginBottomRight(self.this,arg0.this)
        return val
    def SetMinMarginTopLeft(self,arg0):
        val = cmndlgsc.wxPageSetupData_SetMinMarginTopLeft(self.this,arg0.this)
        return val
    def SetMinMarginBottomRight(self,arg0):
        val = cmndlgsc.wxPageSetupData_SetMinMarginBottomRight(self.this,arg0.this)
        return val
    def SetOrientation(self,arg0):
        val = cmndlgsc.wxPageSetupData_SetOrientation(self.this,arg0)
        return val
    def SetDefaultMinMargins(self,arg0):
        val = cmndlgsc.wxPageSetupData_SetDefaultMinMargins(self.this,arg0)
        return val
    def SetDefaultInfo(self,arg0):
        val = cmndlgsc.wxPageSetupData_SetDefaultInfo(self.this,arg0)
        return val
    def __repr__(self):
        return "<C wxPageSetupData instance>"
class wxPageSetupData(wxPageSetupDataPtr):
    def __init__(self) :
        self.this = cmndlgsc.new_wxPageSetupData()
        self.thisown = 1




class wxPageSetupDialogPtr(wxDialogPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetPageSetupData(self):
        val = cmndlgsc.wxPageSetupDialog_GetPageSetupData(self.this)
        val = wxPageSetupDataPtr(val)
        return val
    def ShowModal(self):
        val = cmndlgsc.wxPageSetupDialog_ShowModal(self.this)
        return val
    def __repr__(self):
        return "<C wxPageSetupDialog instance>"
class wxPageSetupDialog(wxPageSetupDialogPtr):
    def __init__(self,arg0,*args) :
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        args = tuple(argl)
        self.this = apply(cmndlgsc.new_wxPageSetupDialog,(arg0.this,)+args)
        self.thisown = 1
        wxp._StdDialogCallbacks(self)




class wxPrintDataPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            cmndlgsc.delete_wxPrintData(self.this)
    def EnableHelp(self,arg0):
        val = cmndlgsc.wxPrintData_EnableHelp(self.this,arg0)
        return val
    def EnablePageNumbers(self,arg0):
        val = cmndlgsc.wxPrintData_EnablePageNumbers(self.this,arg0)
        return val
    def EnablePrintToFile(self,arg0):
        val = cmndlgsc.wxPrintData_EnablePrintToFile(self.this,arg0)
        return val
    def EnableSelection(self,arg0):
        val = cmndlgsc.wxPrintData_EnableSelection(self.this,arg0)
        return val
    def GetAllPages(self):
        val = cmndlgsc.wxPrintData_GetAllPages(self.this)
        return val
    def GetCollate(self):
        val = cmndlgsc.wxPrintData_GetCollate(self.this)
        return val
    def GetFromPage(self):
        val = cmndlgsc.wxPrintData_GetFromPage(self.this)
        return val
    def GetMaxPage(self):
        val = cmndlgsc.wxPrintData_GetMaxPage(self.this)
        return val
    def GetMinPage(self):
        val = cmndlgsc.wxPrintData_GetMinPage(self.this)
        return val
    def GetNoCopies(self):
        val = cmndlgsc.wxPrintData_GetNoCopies(self.this)
        return val
    def GetOrientation(self):
        val = cmndlgsc.wxPrintData_GetOrientation(self.this)
        return val
    def GetToPage(self):
        val = cmndlgsc.wxPrintData_GetToPage(self.this)
        return val
    def SetCollate(self,arg0):
        val = cmndlgsc.wxPrintData_SetCollate(self.this,arg0)
        return val
    def SetFromPage(self,arg0):
        val = cmndlgsc.wxPrintData_SetFromPage(self.this,arg0)
        return val
    def SetMaxPage(self,arg0):
        val = cmndlgsc.wxPrintData_SetMaxPage(self.this,arg0)
        return val
    def SetMinPage(self,arg0):
        val = cmndlgsc.wxPrintData_SetMinPage(self.this,arg0)
        return val
    def SetOrientation(self,arg0):
        val = cmndlgsc.wxPrintData_SetOrientation(self.this,arg0)
        return val
    def SetNoCopies(self,arg0):
        val = cmndlgsc.wxPrintData_SetNoCopies(self.this,arg0)
        return val
    def SetPrintToFile(self,arg0):
        val = cmndlgsc.wxPrintData_SetPrintToFile(self.this,arg0)
        return val
    def SetSetupDialog(self,arg0):
        val = cmndlgsc.wxPrintData_SetSetupDialog(self.this,arg0)
        return val
    def SetToPage(self,arg0):
        val = cmndlgsc.wxPrintData_SetToPage(self.this,arg0)
        return val
    def __repr__(self):
        return "<C wxPrintData instance>"
class wxPrintData(wxPrintDataPtr):
    def __init__(self) :
        self.this = cmndlgsc.new_wxPrintData()
        self.thisown = 1




class wxPrintDialogPtr(wxDialogPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetPrintData(self):
        val = cmndlgsc.wxPrintDialog_GetPrintData(self.this)
        val = wxPrintDataPtr(val)
        return val
    def GetPrintDC(self):
        val = cmndlgsc.wxPrintDialog_GetPrintDC(self.this)
        val = wxDCPtr(val)
        val.thisown = 1
        return val
    def ShowModal(self):
        val = cmndlgsc.wxPrintDialog_ShowModal(self.this)
        return val
    def __repr__(self):
        return "<C wxPrintDialog instance>"
class wxPrintDialog(wxPrintDialogPtr):
    def __init__(self,arg0,*args) :
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        args = tuple(argl)
        self.this = apply(cmndlgsc.new_wxPrintDialog,(arg0.this,)+args)
        self.thisown = 1
        wxp._StdDialogCallbacks(self)




class wxMessageDialogPtr(wxDialogPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def ShowModal(self):
        val = cmndlgsc.wxMessageDialog_ShowModal(self.this)
        return val
    def __repr__(self):
        return "<C wxMessageDialog instance>"
class wxMessageDialog(wxMessageDialogPtr):
    def __init__(self,arg0,arg1,*args) :
        argl = map(None,args)
        try: argl[2] = argl[2].this
        except: pass
        args = tuple(argl)
        self.this = apply(cmndlgsc.new_wxMessageDialog,(arg0.this,arg1,)+args)
        self.thisown = 1
        wxp._StdDialogCallbacks(self)






#-------------- FUNCTION WRAPPERS ------------------



#-------------- VARIABLE WRAPPERS ------------------


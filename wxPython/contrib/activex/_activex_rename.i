// A bunch of %rename directives generated by ./distrib/build_renamers.py
// in order to remove the wx prefix from all global scope names.

#ifndef BUILDING_RENAMERS

%rename(ParamX)                             wxParamX;
%rename(FuncX)                              wxFuncX;
%rename(PropX)                              wxPropX;
%rename(ParamXArray)                        wxParamXArray;
%rename(FuncXArray)                         wxFuncXArray;
%rename(PropXArray)                         wxPropXArray;
%rename(ActiveXWindow)                      wxActiveXWindow;
%rename(ActiveXEvent)                       wxActiveXEvent;
%rename(IEHtmlWindowBase)                   wxIEHtmlWindowBase;

#endif

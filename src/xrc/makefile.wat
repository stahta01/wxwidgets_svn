# wxXML makefile

WXDIR = ..\..\..

#XMLDIR=
!error modify makefile.wat to include the path to the XML library

EXTRACPPFLAGS = -I$(WXDIR)\contrib\include;-I$(XMLDIR)\libxml2-2.1.1

!include $(WXDIR)\src\makewat.env

WXXMLLIB = $(WXDIR)\lib\wxxrc_w.lib
THISDIR = $(WXDIR)\contrib\src\xrc
OUTPUTDIR = $(THISDIR)\

NAME = wxxrc
LNK = $(name).lnk

OBJECTS=xml.obj xmlres.obj xmlrsall.obj &
        xh_bttn.obj xh_chckb.obj xh_chckl.obj xh_choic.obj xh_combo.obj xh_dlg.obj &
        xh_gauge.obj xh_html.obj xh_menu.obj xh_notbk.obj xh_panel.obj xh_radbt.obj &
        xh_radbx.obj xh_sizer.obj xh_slidr.obj xh_spin.obj xh_stbmp.obj xh_sttxt.obj &
        xh_text.obj xh_listb.obj xh_toolb.obj xh_stlin.obj xh_bmp.obj &
        xh_bmpbt.obj xh_cald.obj xh_listc.obj xh_scrol.obj xh_stbox.obj &
        xh_tree.obj xh_unkwn.obj xh_frame.obj xh_scwin.obj xh_split.obj


all: $(WXXMLLIB) .SYMBOLIC

$(WXXMLLIB): $(OBJECTS)
	*wlib /b /c /n /P=256 $(WXXMLLIB) $(OBJECTS)

clean:   .SYMBOLIC
    -erase *.obj 
    -erase *.bak 
    -erase *.err 
    -erase *.pch
    -erase $(WXXMLLIB) 
    -erase *.lbc


#!/binb/wmake.exe

# This file was automatically generated by tmake 
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE WAT.T!

#
# File:     makefile.wat
# Author:   Julian Smart
# Created:  1998
#
# Makefile : Builds wxWindows library for Watcom C++, WIN32
!include ..\makewat.env

LIBTARGET   = $(WXDIR)\lib\$(LIBNAME).lib

EXTRATARGETS = png zlib jpeg tiff regex
EXTRATARGETSCLEAN = clean_png clean_zlib clean_jpeg clean_tiff clean_regex
GENDIR=$(WXDIR)\src\generic
COMMDIR=$(WXDIR)\src\common
JPEGDIR=$(WXDIR)\src\jpeg
TIFFDIR=$(WXDIR)\src\tiff
MSWDIR=$(WXDIR)\src\msw
OLEDIR=$(MSWDIR)\ole
HTMLDIR=$(WXDIR)\src\html

DOCDIR = $(WXDIR)\docs

GENERICOBJS= $(OUTPUTDIR)\busyinfo.obj &
	$(OUTPUTDIR)\calctrl.obj &
	$(OUTPUTDIR)\choicdgg.obj &
	$(OUTPUTDIR)\dcbuffer.obj &
	$(OUTPUTDIR)\dcpsg.obj &
	$(OUTPUTDIR)\dirctrlg.obj &
	$(OUTPUTDIR)\dragimgg.obj &
	$(OUTPUTDIR)\grid.obj &
	$(OUTPUTDIR)\gridctrl.obj &
	$(OUTPUTDIR)\gridsel.obj &
	$(OUTPUTDIR)\laywin.obj &
	$(OUTPUTDIR)\logg.obj &
	$(OUTPUTDIR)\numdlgg.obj &
	$(OUTPUTDIR)\panelg.obj &
	$(OUTPUTDIR)\progdlgg.obj &
	$(OUTPUTDIR)\sashwin.obj &
	$(OUTPUTDIR)\scrlwing.obj &
	$(OUTPUTDIR)\spinctlg.obj &
	$(OUTPUTDIR)\splash.obj &
	$(OUTPUTDIR)\splitter.obj &
	$(OUTPUTDIR)\statusbr.obj &
	$(OUTPUTDIR)\tbarsmpl.obj &
	$(OUTPUTDIR)\textdlgg.obj &
	$(OUTPUTDIR)\tipdlg.obj &
	$(OUTPUTDIR)\tipwin.obj &
	$(OUTPUTDIR)\treectlg.obj &
	$(OUTPUTDIR)\wizard.obj

# These are generic things that don't need to be compiled on MSW,
# but sometimes it's useful to do so for testing purposes.
NONESSENTIALOBJS= $(OUTPUTDIR)\accel.obj &
	$(OUTPUTDIR)\caret.obj &
	$(OUTPUTDIR)\colrdlgg.obj &
	$(OUTPUTDIR)\dirdlgg.obj &
	$(OUTPUTDIR)\fdrepdlg.obj &
	$(OUTPUTDIR)\filedlgg.obj &
	$(OUTPUTDIR)\fontdlgg.obj &
	$(OUTPUTDIR)\helpext.obj &
	$(OUTPUTDIR)\helphtml.obj &
	$(OUTPUTDIR)\imaglist.obj &
	$(OUTPUTDIR)\listctrl.obj &
	$(OUTPUTDIR)\mdig.obj &
	$(OUTPUTDIR)\msgdlgg.obj &
	$(OUTPUTDIR)\notebook.obj &
	$(OUTPUTDIR)\paletteg.obj &
	$(OUTPUTDIR)\printps.obj &
	$(OUTPUTDIR)\prntdlgg.obj &
	$(OUTPUTDIR)\statline.obj &
	$(OUTPUTDIR)\tabg.obj &
	$(OUTPUTDIR)\timer.obj

COMMONOBJS = &
	$(OUTPUTDIR)\accesscmn.obj &
	$(OUTPUTDIR)\appcmn.obj &
	$(OUTPUTDIR)\artprov.obj &
	$(OUTPUTDIR)\artstd.obj &
	$(OUTPUTDIR)\choiccmn.obj &
	$(OUTPUTDIR)\clipcmn.obj &
	$(OUTPUTDIR)\clntdata.obj &
	$(OUTPUTDIR)\cmdline.obj &
	$(OUTPUTDIR)\cmdproc.obj &
	$(OUTPUTDIR)\cmndata.obj &
	$(OUTPUTDIR)\config.obj &
	$(OUTPUTDIR)\containr.obj &
	$(OUTPUTDIR)\cshelp.obj &
	$(OUTPUTDIR)\ctrlcmn.obj &
	$(OUTPUTDIR)\ctrlsub.obj &
	$(OUTPUTDIR)\datacmn.obj &
	$(OUTPUTDIR)\datetime.obj &
	$(OUTPUTDIR)\datstrm.obj &
	$(OUTPUTDIR)\db.obj &
	$(OUTPUTDIR)\dbgrid.obj &
	$(OUTPUTDIR)\dbtable.obj &
	$(OUTPUTDIR)\dcbase.obj &
	$(OUTPUTDIR)\dircmn.obj &
	$(OUTPUTDIR)\dlgcmn.obj &
	$(OUTPUTDIR)\dndcmn.obj &
	$(OUTPUTDIR)\dobjcmn.obj &
	$(OUTPUTDIR)\docmdi.obj &
	$(OUTPUTDIR)\docview.obj &
	$(OUTPUTDIR)\dpycmn.obj &
	$(OUTPUTDIR)\dseldlg.obj &
	$(OUTPUTDIR)\dynarray.obj &
	$(OUTPUTDIR)\dynlib.obj &
	$(OUTPUTDIR)\dynload.obj &
	$(OUTPUTDIR)\effects.obj &
	$(OUTPUTDIR)\encconv.obj &
	$(OUTPUTDIR)\event.obj &
	$(OUTPUTDIR)\extended.obj &
	$(OUTPUTDIR)\fddlgcmn.obj &
	$(OUTPUTDIR)\ffile.obj &
	$(OUTPUTDIR)\file.obj &
	$(OUTPUTDIR)\fileconf.obj &
	$(OUTPUTDIR)\filefn.obj &
	$(OUTPUTDIR)\filename.obj &
	$(OUTPUTDIR)\filesys.obj &
	$(OUTPUTDIR)\fontcmn.obj &
	$(OUTPUTDIR)\fontmap.obj &
	$(OUTPUTDIR)\framecmn.obj &
	$(OUTPUTDIR)\fs_inet.obj &
	$(OUTPUTDIR)\fs_mem.obj &
	$(OUTPUTDIR)\fs_zip.obj &
	$(OUTPUTDIR)\ftp.obj &
	$(OUTPUTDIR)\gaugecmn.obj &
	$(OUTPUTDIR)\gdicmn.obj &
	$(OUTPUTDIR)\geometry.obj &
	$(OUTPUTDIR)\gifdecod.obj &
	$(OUTPUTDIR)\hash.obj &
	$(OUTPUTDIR)\hashmap.obj &
	$(OUTPUTDIR)\helpbase.obj &
	$(OUTPUTDIR)\http.obj &
	$(OUTPUTDIR)\iconbndl.obj &
	$(OUTPUTDIR)\imagall.obj &
	$(OUTPUTDIR)\imagbmp.obj &
	$(OUTPUTDIR)\image.obj &
	$(OUTPUTDIR)\imagfill.obj &
	$(OUTPUTDIR)\imaggif.obj &
	$(OUTPUTDIR)\imagiff.obj &
	$(OUTPUTDIR)\imagjpeg.obj &
	$(OUTPUTDIR)\imagpcx.obj &
	$(OUTPUTDIR)\imagpng.obj &
	$(OUTPUTDIR)\imagpnm.obj &
	$(OUTPUTDIR)\imagtiff.obj &
	$(OUTPUTDIR)\imagxpm.obj &
	$(OUTPUTDIR)\intl.obj &
	$(OUTPUTDIR)\ipcbase.obj &
	$(OUTPUTDIR)\layout.obj &
	$(OUTPUTDIR)\lboxcmn.obj &
	$(OUTPUTDIR)\list.obj &
	$(OUTPUTDIR)\log.obj &
	$(OUTPUTDIR)\longlong.obj &
	$(OUTPUTDIR)\matrix.obj &
	$(OUTPUTDIR)\memory.obj &
	$(OUTPUTDIR)\menucmn.obj &
	$(OUTPUTDIR)\mimecmn.obj &
	$(OUTPUTDIR)\module.obj &
	$(OUTPUTDIR)\msgout.obj &
	$(OUTPUTDIR)\mstream.obj &
	$(OUTPUTDIR)\nbkbase.obj &
	$(OUTPUTDIR)\object.obj &
	$(OUTPUTDIR)\paper.obj &
	$(OUTPUTDIR)\popupcmn.obj &
	$(OUTPUTDIR)\prntbase.obj &
	$(OUTPUTDIR)\process.obj &
	$(OUTPUTDIR)\protocol.obj &
	$(OUTPUTDIR)\quantize.obj &
	$(OUTPUTDIR)\radiocmn.obj &
	$(OUTPUTDIR)\regex.obj &
	$(OUTPUTDIR)\rgncmn.obj &
	$(OUTPUTDIR)\sckaddr.obj &
	$(OUTPUTDIR)\sckfile.obj &
	$(OUTPUTDIR)\sckipc.obj &
	$(OUTPUTDIR)\sckstrm.obj &
	$(OUTPUTDIR)\settcmn.obj &
	$(OUTPUTDIR)\sizer.obj &
	$(OUTPUTDIR)\socket.obj &
	$(OUTPUTDIR)\statbar.obj &
	$(OUTPUTDIR)\strconv.obj &
	$(OUTPUTDIR)\stream.obj &
	$(OUTPUTDIR)\string.obj &
	$(OUTPUTDIR)\sysopt.obj &
	$(OUTPUTDIR)\taskbarcmn.obj &
	$(OUTPUTDIR)\tbarbase.obj &
	$(OUTPUTDIR)\textbuf.obj &
	$(OUTPUTDIR)\textcmn.obj &
	$(OUTPUTDIR)\textfile.obj &
	$(OUTPUTDIR)\timercmn.obj &
	$(OUTPUTDIR)\tokenzr.obj &
	$(OUTPUTDIR)\toplvcmn.obj &
	$(OUTPUTDIR)\treebase.obj &
	$(OUTPUTDIR)\txtstrm.obj &
	$(OUTPUTDIR)\unzip.obj &
	$(OUTPUTDIR)\url.obj &
	$(OUTPUTDIR)\utilscmn.obj &
	$(OUTPUTDIR)\valgen.obj &
	$(OUTPUTDIR)\validate.obj &
	$(OUTPUTDIR)\valtext.obj &
	$(OUTPUTDIR)\variant.obj &
	$(OUTPUTDIR)\wfstream.obj &
	$(OUTPUTDIR)\wincmn.obj &
	$(OUTPUTDIR)\wxchar.obj &
	$(OUTPUTDIR)\xpmdecod.obj &
	$(OUTPUTDIR)\zipstrm.obj &
	$(OUTPUTDIR)\zstream.obj

MSWOBJS = $(OUTPUTDIR)\accel.obj &
	$(OUTPUTDIR)\access.obj &
	$(OUTPUTDIR)\app.obj &
	$(OUTPUTDIR)\automtn.obj &
	$(OUTPUTDIR)\bitmap.obj &
	$(OUTPUTDIR)\bmpbuttn.obj &
	$(OUTPUTDIR)\brush.obj &
	$(OUTPUTDIR)\button.obj &
	$(OUTPUTDIR)\caret.obj &
	$(OUTPUTDIR)\checkbox.obj &
	$(OUTPUTDIR)\checklst.obj &
	$(OUTPUTDIR)\choice.obj &
	$(OUTPUTDIR)\clipbrd.obj &
	$(OUTPUTDIR)\colordlg.obj &
	$(OUTPUTDIR)\colour.obj &
	$(OUTPUTDIR)\combobox.obj &
	$(OUTPUTDIR)\control.obj &
	$(OUTPUTDIR)\cursor.obj &
	$(OUTPUTDIR)\data.obj &
	$(OUTPUTDIR)\dataobj.obj &
	$(OUTPUTDIR)\dc.obj &
	$(OUTPUTDIR)\dcclient.obj &
	$(OUTPUTDIR)\dcmemory.obj &
	$(OUTPUTDIR)\dcprint.obj &
	$(OUTPUTDIR)\dcscreen.obj &
	$(OUTPUTDIR)\dde.obj &
	$(OUTPUTDIR)\dialog.obj &
	$(OUTPUTDIR)\dialup.obj &
	$(OUTPUTDIR)\dib.obj &
	$(OUTPUTDIR)\dir.obj &
	$(OUTPUTDIR)\dirdlg.obj &
	$(OUTPUTDIR)\display.obj &
	$(OUTPUTDIR)\dragimag.obj &
	$(OUTPUTDIR)\dropsrc.obj &
	$(OUTPUTDIR)\droptgt.obj &
	$(OUTPUTDIR)\enhmeta.obj &
	$(OUTPUTDIR)\evtloop.obj &
	$(OUTPUTDIR)\fdrepdlg.obj &
	$(OUTPUTDIR)\filedlg.obj &
	$(OUTPUTDIR)\font.obj &
	$(OUTPUTDIR)\fontdlg.obj &
	$(OUTPUTDIR)\fontenum.obj &
	$(OUTPUTDIR)\fontutil.obj &
	$(OUTPUTDIR)\frame.obj &
	$(OUTPUTDIR)\gauge95.obj &
	$(OUTPUTDIR)\gdiimage.obj &
	$(OUTPUTDIR)\gdiobj.obj &
	$(OUTPUTDIR)\glcanvas.obj &
	$(OUTPUTDIR)\gsocket.obj &
	$(OUTPUTDIR)\gsockmsw.obj &
	$(OUTPUTDIR)\helpbest.obj &
	$(OUTPUTDIR)\helpchm.obj &
	$(OUTPUTDIR)\helpwin.obj &
	$(OUTPUTDIR)\icon.obj &
	$(OUTPUTDIR)\imaglist.obj &
	$(OUTPUTDIR)\iniconf.obj &
	$(OUTPUTDIR)\joystick.obj &
	$(OUTPUTDIR)\listbox.obj &
	$(OUTPUTDIR)\listctrl.obj &
	$(OUTPUTDIR)\main.obj &
	$(OUTPUTDIR)\mdi.obj &
	$(OUTPUTDIR)\menu.obj &
	$(OUTPUTDIR)\menuitem.obj &
	$(OUTPUTDIR)\metafile.obj &
	$(OUTPUTDIR)\mimetype.obj &
	$(OUTPUTDIR)\minifram.obj &
	$(OUTPUTDIR)\msgdlg.obj &
	$(OUTPUTDIR)\mslu.obj &
	$(OUTPUTDIR)\nativdlg.obj &
	$(OUTPUTDIR)\notebook.obj &
	$(OUTPUTDIR)\oleutils.obj &
	$(OUTPUTDIR)\ownerdrw.obj &
	$(OUTPUTDIR)\palette.obj &
	$(OUTPUTDIR)\pen.obj &
	$(OUTPUTDIR)\penwin.obj &
	$(OUTPUTDIR)\popupwin.obj &
	$(OUTPUTDIR)\printdlg.obj &
	$(OUTPUTDIR)\printwin.obj &
	$(OUTPUTDIR)\radiobox.obj &
	$(OUTPUTDIR)\radiobut.obj &
	$(OUTPUTDIR)\regconf.obj &
	$(OUTPUTDIR)\region.obj &
	$(OUTPUTDIR)\registry.obj &
	$(OUTPUTDIR)\scrolbar.obj &
	$(OUTPUTDIR)\settings.obj &
	$(OUTPUTDIR)\slider95.obj &
	$(OUTPUTDIR)\snglinst.obj &
	$(OUTPUTDIR)\spinbutt.obj &
	$(OUTPUTDIR)\spinctrl.obj &
	$(OUTPUTDIR)\statbmp.obj &
	$(OUTPUTDIR)\statbox.obj &
	$(OUTPUTDIR)\statbr95.obj &
	$(OUTPUTDIR)\statline.obj &
	$(OUTPUTDIR)\stattext.obj &
	$(OUTPUTDIR)\tabctrl.obj &
	$(OUTPUTDIR)\taskbar.obj &
	$(OUTPUTDIR)\tbar95.obj &
	$(OUTPUTDIR)\textctrl.obj &
	$(OUTPUTDIR)\tglbtn.obj &
	$(OUTPUTDIR)\thread.obj &
	$(OUTPUTDIR)\timer.obj &
	$(OUTPUTDIR)\tooltip.obj &
	$(OUTPUTDIR)\toplevel.obj &
	$(OUTPUTDIR)\treectrl.obj &
	$(OUTPUTDIR)\utils.obj &
	$(OUTPUTDIR)\utilsexc.obj &
	$(OUTPUTDIR)\uuid.obj &
	$(OUTPUTDIR)\volume.obj &
	$(OUTPUTDIR)\wave.obj &
	$(OUTPUTDIR)\window.obj

HTMLOBJS = $(OUTPUTDIR)\helpctrl.obj &
	$(OUTPUTDIR)\helpdata.obj &
	$(OUTPUTDIR)\helpfrm.obj &
	$(OUTPUTDIR)\htmlcell.obj &
	$(OUTPUTDIR)\htmlfilt.obj &
	$(OUTPUTDIR)\htmlpars.obj &
	$(OUTPUTDIR)\htmltag.obj &
	$(OUTPUTDIR)\htmlwin.obj &
	$(OUTPUTDIR)\htmprint.obj &
	$(OUTPUTDIR)\m_dflist.obj &
	$(OUTPUTDIR)\m_fonts.obj &
	$(OUTPUTDIR)\m_hline.obj &
	$(OUTPUTDIR)\m_image.obj &
	$(OUTPUTDIR)\m_layout.obj &
	$(OUTPUTDIR)\m_links.obj &
	$(OUTPUTDIR)\m_list.obj &
	$(OUTPUTDIR)\m_pre.obj &
	$(OUTPUTDIR)\m_style.obj &
	$(OUTPUTDIR)\m_tables.obj &
	$(OUTPUTDIR)\winpars.obj

# Add $(NONESSENTIALOBJS) if wanting generic dialogs, PostScript etc.
OBJECTS = $(COMMONOBJS) $(GENERICOBJS) $(MSWOBJS) $(HTMLOBJS)

SETUP_H=$(ARCHINCDIR)\wx\setup.h

all: $(SETUP_H) $(OUTPUTDIR) $(OBJECTS) $(LIBTARGET) $(EXTRATARGETS) .SYMBOLIC

$(ARCHINCDIR)\wx:
	mkdir $(ARCHINCDIR)
	mkdir $(ARCHINCDIR)\wx

$(OUTPUTDIR):
	@if not exist $^@ mkdir $^@

$(SETUP_H): $(ARCHINCDIR)\wx
    if not exist $(WXDIR)\include\wx\msw\setup.h copy $(WXDIR)\include\wx\msw\setup0.h $(WXDIR)\include\wx\msw\setup.h
	copy $(WXDIR)\include\wx\msw\setup.h $@

LBCFILE=wx$(TOOLKIT).lbc
$(LIBTARGET) : $(OBJECTS)
    %create $(LBCFILE)
    @for %i in ( $(OBJECTS) ) do @%append $(LBCFILE) +%i
    wlib /b /c /n $(LIBPAGESIZE) $^@ @$(LBCFILE)


clean:   .SYMBOLIC $(EXTRATARGETSCLEAN)
    -erase *.obj
    -erase $(LIBTARGET)
    -erase *.pch
    -erase *.err
    -erase *.lbc

cleanall:   clean

$(OUTPUTDIR)\accel.obj:     $(MSWDIR)\accel.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\access.obj:     $(OLEDIR)\access.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\app.obj:     $(MSWDIR)\app.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\automtn.obj:     $(OLEDIR)\automtn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\bitmap.obj:     $(MSWDIR)\bitmap.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\bmpbuttn.obj:     $(MSWDIR)\bmpbuttn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\brush.obj:     $(MSWDIR)\brush.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\button.obj:     $(MSWDIR)\button.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\caret.obj:     $(MSWDIR)\caret.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\checkbox.obj:     $(MSWDIR)\checkbox.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\checklst.obj:     $(MSWDIR)\checklst.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\choice.obj:     $(MSWDIR)\choice.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\clipbrd.obj:     $(MSWDIR)\clipbrd.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\colordlg.obj:     $(MSWDIR)\colordlg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\colour.obj:     $(MSWDIR)\colour.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\combobox.obj:     $(MSWDIR)\combobox.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\control.obj:     $(MSWDIR)\control.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\cursor.obj:     $(MSWDIR)\cursor.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\data.obj:     $(MSWDIR)\data.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dataobj.obj:     $(OLEDIR)\dataobj.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dc.obj:     $(MSWDIR)\dc.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dcclient.obj:     $(MSWDIR)\dcclient.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dcmemory.obj:     $(MSWDIR)\dcmemory.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dcprint.obj:     $(MSWDIR)\dcprint.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dcscreen.obj:     $(MSWDIR)\dcscreen.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dde.obj:     $(MSWDIR)\dde.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dialog.obj:     $(MSWDIR)\dialog.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dialup.obj:     $(MSWDIR)\dialup.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dib.obj:     $(MSWDIR)\dib.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dir.obj:     $(MSWDIR)\dir.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dirdlg.obj:     $(MSWDIR)\dirdlg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\display.obj:     $(MSWDIR)\display.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dragimag.obj:     $(MSWDIR)\dragimag.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dropsrc.obj:     $(OLEDIR)\dropsrc.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\droptgt.obj:     $(OLEDIR)\droptgt.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\enhmeta.obj:     $(MSWDIR)\enhmeta.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\evtloop.obj:     $(MSWDIR)\evtloop.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\fdrepdlg.obj:     $(MSWDIR)\fdrepdlg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\filedlg.obj:     $(MSWDIR)\filedlg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\font.obj:     $(MSWDIR)\font.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\fontdlg.obj:     $(MSWDIR)\fontdlg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\fontenum.obj:     $(MSWDIR)\fontenum.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\fontutil.obj:     $(MSWDIR)\fontutil.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\frame.obj:     $(MSWDIR)\frame.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\gauge95.obj:     $(MSWDIR)\gauge95.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\gdiimage.obj:     $(MSWDIR)\gdiimage.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\gdiobj.obj:     $(MSWDIR)\gdiobj.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\glcanvas.obj:     $(MSWDIR)\glcanvas.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\gsocket.obj:     $(MSWDIR)\gsocket.c
  *$(CC) $(CFLAGS) $<

$(OUTPUTDIR)\gsockmsw.obj:     $(MSWDIR)\gsockmsw.c
  *$(CC) $(CFLAGS) $<

$(OUTPUTDIR)\helpbest.obj:     $(MSWDIR)\helpbest.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\helpchm.obj:     $(MSWDIR)\helpchm.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\helpwin.obj:     $(MSWDIR)\helpwin.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\icon.obj:     $(MSWDIR)\icon.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\imaglist.obj:     $(MSWDIR)\imaglist.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\iniconf.obj:     $(MSWDIR)\iniconf.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\joystick.obj:     $(MSWDIR)\joystick.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\listbox.obj:     $(MSWDIR)\listbox.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\listctrl.obj:     $(MSWDIR)\listctrl.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\main.obj:     $(MSWDIR)\main.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\mdi.obj:     $(MSWDIR)\mdi.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\menu.obj:     $(MSWDIR)\menu.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\menuitem.obj:     $(MSWDIR)\menuitem.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\metafile.obj:     $(MSWDIR)\metafile.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\mimetype.obj:     $(MSWDIR)\mimetype.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\minifram.obj:     $(MSWDIR)\minifram.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\msgdlg.obj:     $(MSWDIR)\msgdlg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\mslu.obj:     $(MSWDIR)\mslu.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\nativdlg.obj:     $(MSWDIR)\nativdlg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\notebook.obj:     $(MSWDIR)\notebook.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\oleutils.obj:     $(OLEDIR)\oleutils.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\ownerdrw.obj:     $(MSWDIR)\ownerdrw.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\palette.obj:     $(MSWDIR)\palette.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\pen.obj:     $(MSWDIR)\pen.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\penwin.obj:     $(MSWDIR)\penwin.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\popupwin.obj:     $(MSWDIR)\popupwin.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\printdlg.obj:     $(MSWDIR)\printdlg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\printwin.obj:     $(MSWDIR)\printwin.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\radiobox.obj:     $(MSWDIR)\radiobox.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\radiobut.obj:     $(MSWDIR)\radiobut.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\regconf.obj:     $(MSWDIR)\regconf.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\region.obj:     $(MSWDIR)\region.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\registry.obj:     $(MSWDIR)\registry.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\scrolbar.obj:     $(MSWDIR)\scrolbar.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\settings.obj:     $(MSWDIR)\settings.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\slider95.obj:     $(MSWDIR)\slider95.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\snglinst.obj:     $(MSWDIR)\snglinst.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\spinbutt.obj:     $(MSWDIR)\spinbutt.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\spinctrl.obj:     $(MSWDIR)\spinctrl.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\statbmp.obj:     $(MSWDIR)\statbmp.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\statbox.obj:     $(MSWDIR)\statbox.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\statbr95.obj:     $(MSWDIR)\statbr95.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\statline.obj:     $(MSWDIR)\statline.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\stattext.obj:     $(MSWDIR)\stattext.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\tabctrl.obj:     $(MSWDIR)\tabctrl.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\taskbar.obj:     $(MSWDIR)\taskbar.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\tbar95.obj:     $(MSWDIR)\tbar95.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\textctrl.obj:     $(MSWDIR)\textctrl.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\tglbtn.obj:     $(MSWDIR)\tglbtn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\thread.obj:     $(MSWDIR)\thread.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\timer.obj:     $(MSWDIR)\timer.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\tooltip.obj:     $(MSWDIR)\tooltip.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\toplevel.obj:     $(MSWDIR)\toplevel.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\treectrl.obj:     $(MSWDIR)\treectrl.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\utils.obj:     $(MSWDIR)\utils.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\utilsexc.obj:     $(MSWDIR)\utilsexc.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\uuid.obj:     $(OLEDIR)\uuid.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\volume.obj:     $(MSWDIR)\volume.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\wave.obj:     $(MSWDIR)\wave.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\window.obj:     $(MSWDIR)\window.cpp
  *$(CXX) $(CXXFLAGS) $<



########################################################
# Common objects (always compiled)

$(OUTPUTDIR)\accesscmn.obj:     $(COMMDIR)\accesscmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\appcmn.obj:     $(COMMDIR)\appcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\artprov.obj:     $(COMMDIR)\artprov.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\artstd.obj:     $(COMMDIR)\artstd.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\choiccmn.obj:     $(COMMDIR)\choiccmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\clipcmn.obj:     $(COMMDIR)\clipcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\clntdata.obj:     $(COMMDIR)\clntdata.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\cmdline.obj:     $(COMMDIR)\cmdline.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\cmdproc.obj:     $(COMMDIR)\cmdproc.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\cmndata.obj:     $(COMMDIR)\cmndata.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\config.obj:     $(COMMDIR)\config.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\containr.obj:     $(COMMDIR)\containr.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\cshelp.obj:     $(COMMDIR)\cshelp.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\ctrlcmn.obj:     $(COMMDIR)\ctrlcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\ctrlsub.obj:     $(COMMDIR)\ctrlsub.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\datacmn.obj:     $(COMMDIR)\datacmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\datetime.obj:     $(COMMDIR)\datetime.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\datstrm.obj:     $(COMMDIR)\datstrm.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\db.obj:     $(COMMDIR)\db.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dbgrid.obj:     $(COMMDIR)\dbgrid.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dbtable.obj:     $(COMMDIR)\dbtable.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dcbase.obj:     $(COMMDIR)\dcbase.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dircmn.obj:     $(COMMDIR)\dircmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dlgcmn.obj:     $(COMMDIR)\dlgcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dndcmn.obj:     $(COMMDIR)\dndcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dobjcmn.obj:     $(COMMDIR)\dobjcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\docmdi.obj:     $(COMMDIR)\docmdi.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\docview.obj:     $(COMMDIR)\docview.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dpycmn.obj:     $(COMMDIR)\dpycmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dseldlg.obj:     $(COMMDIR)\dseldlg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dynarray.obj:     $(COMMDIR)\dynarray.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dynlib.obj:     $(COMMDIR)\dynlib.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dynload.obj:     $(COMMDIR)\dynload.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\effects.obj:     $(COMMDIR)\effects.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\encconv.obj:     $(COMMDIR)\encconv.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\event.obj:     $(COMMDIR)\event.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\extended.obj:     $(COMMDIR)\extended.c
  *$(CC) $(CFLAGS) $<

$(OUTPUTDIR)\fddlgcmn.obj:     $(COMMDIR)\fddlgcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\ffile.obj:     $(COMMDIR)\ffile.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\file.obj:     $(COMMDIR)\file.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\fileconf.obj:     $(COMMDIR)\fileconf.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\filefn.obj:     $(COMMDIR)\filefn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\filename.obj:     $(COMMDIR)\filename.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\filesys.obj:     $(COMMDIR)\filesys.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\fontcmn.obj:     $(COMMDIR)\fontcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\fontmap.obj:     $(COMMDIR)\fontmap.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\framecmn.obj:     $(COMMDIR)\framecmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\fs_inet.obj:     $(COMMDIR)\fs_inet.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\fs_mem.obj:     $(COMMDIR)\fs_mem.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\fs_zip.obj:     $(COMMDIR)\fs_zip.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\ftp.obj:     $(COMMDIR)\ftp.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\gaugecmn.obj:     $(COMMDIR)\gaugecmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\gdicmn.obj:     $(COMMDIR)\gdicmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\geometry.obj:     $(COMMDIR)\geometry.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\gifdecod.obj:     $(COMMDIR)\gifdecod.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\hash.obj:     $(COMMDIR)\hash.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\hashmap.obj:     $(COMMDIR)\hashmap.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\helpbase.obj:     $(COMMDIR)\helpbase.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\http.obj:     $(COMMDIR)\http.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\iconbndl.obj:     $(COMMDIR)\iconbndl.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\imagall.obj:     $(COMMDIR)\imagall.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\imagbmp.obj:     $(COMMDIR)\imagbmp.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\image.obj:     $(COMMDIR)\image.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\imagfill.obj:     $(COMMDIR)\imagfill.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\imaggif.obj:     $(COMMDIR)\imaggif.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\imagiff.obj:     $(COMMDIR)\imagiff.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\imagjpeg.obj:     $(COMMDIR)\imagjpeg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\imagpcx.obj:     $(COMMDIR)\imagpcx.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\imagpng.obj:     $(COMMDIR)\imagpng.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\imagpnm.obj:     $(COMMDIR)\imagpnm.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\imagtiff.obj:     $(COMMDIR)\imagtiff.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\imagxpm.obj:     $(COMMDIR)\imagxpm.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\intl.obj:     $(COMMDIR)\intl.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\ipcbase.obj:     $(COMMDIR)\ipcbase.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\layout.obj:     $(COMMDIR)\layout.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\lboxcmn.obj:     $(COMMDIR)\lboxcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\list.obj:     $(COMMDIR)\list.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\log.obj:     $(COMMDIR)\log.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\longlong.obj:     $(COMMDIR)\longlong.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\matrix.obj:     $(COMMDIR)\matrix.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\memory.obj:     $(COMMDIR)\memory.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\menucmn.obj:     $(COMMDIR)\menucmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\mimecmn.obj:     $(COMMDIR)\mimecmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\module.obj:     $(COMMDIR)\module.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\msgout.obj:     $(COMMDIR)\msgout.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\mstream.obj:     $(COMMDIR)\mstream.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\nbkbase.obj:     $(COMMDIR)\nbkbase.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\object.obj:     $(COMMDIR)\object.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\paper.obj:     $(COMMDIR)\paper.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\popupcmn.obj:     $(COMMDIR)\popupcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\prntbase.obj:     $(COMMDIR)\prntbase.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\process.obj:     $(COMMDIR)\process.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\protocol.obj:     $(COMMDIR)\protocol.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\quantize.obj:     $(COMMDIR)\quantize.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\radiocmn.obj:     $(COMMDIR)\radiocmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\regex.obj:     $(COMMDIR)\regex.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\rgncmn.obj:     $(COMMDIR)\rgncmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\sckaddr.obj:     $(COMMDIR)\sckaddr.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\sckfile.obj:     $(COMMDIR)\sckfile.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\sckipc.obj:     $(COMMDIR)\sckipc.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\sckstrm.obj:     $(COMMDIR)\sckstrm.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\settcmn.obj:     $(COMMDIR)\settcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\sizer.obj:     $(COMMDIR)\sizer.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\socket.obj:     $(COMMDIR)\socket.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\statbar.obj:     $(COMMDIR)\statbar.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\strconv.obj:     $(COMMDIR)\strconv.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\stream.obj:     $(COMMDIR)\stream.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\string.obj:     $(COMMDIR)\string.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\sysopt.obj:     $(COMMDIR)\sysopt.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\taskbarcmn.obj:     $(COMMDIR)\taskbarcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\tbarbase.obj:     $(COMMDIR)\tbarbase.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\textbuf.obj:     $(COMMDIR)\textbuf.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\textcmn.obj:     $(COMMDIR)\textcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\textfile.obj:     $(COMMDIR)\textfile.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\timercmn.obj:     $(COMMDIR)\timercmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\tokenzr.obj:     $(COMMDIR)\tokenzr.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\toplvcmn.obj:     $(COMMDIR)\toplvcmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\treebase.obj:     $(COMMDIR)\treebase.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\txtstrm.obj:     $(COMMDIR)\txtstrm.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\unzip.obj:     $(COMMDIR)\unzip.c
  *$(CC) $(CFLAGS) $<

$(OUTPUTDIR)\url.obj:     $(COMMDIR)\url.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\utilscmn.obj:     $(COMMDIR)\utilscmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\valgen.obj:     $(COMMDIR)\valgen.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\validate.obj:     $(COMMDIR)\validate.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\valtext.obj:     $(COMMDIR)\valtext.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\variant.obj:     $(COMMDIR)\variant.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\wfstream.obj:     $(COMMDIR)\wfstream.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\wincmn.obj:     $(COMMDIR)\wincmn.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\wxchar.obj:     $(COMMDIR)\wxchar.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\xpmdecod.obj:     $(COMMDIR)\xpmdecod.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\zipstrm.obj:     $(COMMDIR)\zipstrm.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\zstream.obj:     $(COMMDIR)\zstream.cpp
  *$(CXX) $(CXXFLAGS) $<




########################################################
# Generic objects (not always compiled, depending on
# whether platforms have native implementations)

$(OUTPUTDIR)\busyinfo.obj:     $(GENDIR)\busyinfo.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\calctrl.obj:     $(GENDIR)\calctrl.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\choicdgg.obj:     $(GENDIR)\choicdgg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dcbuffer.obj:     $(GENDIR)\dcbuffer.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dcpsg.obj:     $(GENDIR)\dcpsg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dirctrlg.obj:     $(GENDIR)\dirctrlg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\dragimgg.obj:     $(GENDIR)\dragimgg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\grid.obj:     $(GENDIR)\grid.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\gridctrl.obj:     $(GENDIR)\gridctrl.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\gridsel.obj:     $(GENDIR)\gridsel.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\laywin.obj:     $(GENDIR)\laywin.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\logg.obj:     $(GENDIR)\logg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\numdlgg.obj:     $(GENDIR)\numdlgg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\panelg.obj:     $(GENDIR)\panelg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\progdlgg.obj:     $(GENDIR)\progdlgg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\sashwin.obj:     $(GENDIR)\sashwin.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\scrlwing.obj:     $(GENDIR)\scrlwing.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\spinctlg.obj:     $(GENDIR)\spinctlg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\splash.obj:     $(GENDIR)\splash.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\splitter.obj:     $(GENDIR)\splitter.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\statusbr.obj:     $(GENDIR)\statusbr.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\tbarsmpl.obj:     $(GENDIR)\tbarsmpl.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\textdlgg.obj:     $(GENDIR)\textdlgg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\tipdlg.obj:     $(GENDIR)\tipdlg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\tipwin.obj:     $(GENDIR)\tipwin.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\treectlg.obj:     $(GENDIR)\treectlg.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\wizard.obj:     $(GENDIR)\wizard.cpp
  *$(CXX) $(CXXFLAGS) $<




########################################################
# HTML objects (always compiled)

$(OUTPUTDIR)\helpctrl.obj:     $(HTMLDIR)\helpctrl.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\helpdata.obj:     $(HTMLDIR)\helpdata.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\helpfrm.obj:     $(HTMLDIR)\helpfrm.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\htmlcell.obj:     $(HTMLDIR)\htmlcell.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\htmlfilt.obj:     $(HTMLDIR)\htmlfilt.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\htmlpars.obj:     $(HTMLDIR)\htmlpars.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\htmltag.obj:     $(HTMLDIR)\htmltag.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\htmlwin.obj:     $(HTMLDIR)\htmlwin.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\htmprint.obj:     $(HTMLDIR)\htmprint.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\m_dflist.obj:     $(HTMLDIR)\m_dflist.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\m_fonts.obj:     $(HTMLDIR)\m_fonts.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\m_hline.obj:     $(HTMLDIR)\m_hline.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\m_image.obj:     $(HTMLDIR)\m_image.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\m_layout.obj:     $(HTMLDIR)\m_layout.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\m_links.obj:     $(HTMLDIR)\m_links.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\m_list.obj:     $(HTMLDIR)\m_list.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\m_pre.obj:     $(HTMLDIR)\m_pre.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\m_style.obj:     $(HTMLDIR)\m_style.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\m_tables.obj:     $(HTMLDIR)\m_tables.cpp
  *$(CXX) $(CXXFLAGS) $<

$(OUTPUTDIR)\winpars.obj:     $(HTMLDIR)\winpars.cpp
  *$(CXX) $(CXXFLAGS) $<



png:   .SYMBOLIC
    cd $(WXDIR)\src\png
    wmake -f makefile.wat all
    cd $(WXDIR)\src\msw

clean_png:   .SYMBOLIC
    cd $(WXDIR)\src\png
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\msw

zlib:   .SYMBOLIC
    cd $(WXDIR)\src\zlib
    wmake -f makefile.wat all
    cd $(WXDIR)\src\msw

clean_zlib:   .SYMBOLIC
    cd $(WXDIR)\src\zlib
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\msw

jpeg:    .SYMBOLIC
    cd $(WXDIR)\src\jpeg
    wmake -f makefile.wat all
    cd $(WXDIR)\src\msw

clean_jpeg:   .SYMBOLIC
    cd $(WXDIR)\src\jpeg
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\msw

tiff:    .SYMBOLIC
    cd $(WXDIR)\src\tiff
    wmake -f makefile.wat all
    cd $(WXDIR)\src\msw

clean_tiff:   .SYMBOLIC
    cd $(WXDIR)\src\tiff
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\msw

regex:    .SYMBOLIC
    cd $(WXDIR)\src\regex
    wmake -f makefile.wat all
    cd $(WXDIR)\src\msw

clean_regex:   .SYMBOLIC
    cd $(WXDIR)\src\regex
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\msw

MFTYPE=wat
self : .SYMBOLIC $(WXDIR)\distrib\msw\tmake\filelist.txt $(WXDIR)\distrib\msw\tmake\$(MFTYPE).t
	cd $(WXDIR)\distrib\msw\tmake
	tmake -t $(MFTYPE) wxwin.pro -o makefile.$(MFTYPE)
	copy makefile.$(MFTYPE) $(WXDIR)\src\msw

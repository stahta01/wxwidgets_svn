#!/binb/wmake.exe

# This file was automatically generated by tmake at 11:30, 2002/01/23
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE WAT.T!

#
# File:     makefile.wat
# Author:   Julian Smart & Vaclav Slavik
# Created:  1998
#
# Makefile : Builds wxWindows library for Watcom C++, DOS+MGL
#
# NOTE: This file is generated from wat.t by tmake, but not all bugs have
# been removed from this process. If wxWindows doesn't compile,
# check the following and edit this makefile accordingly:
#
# - extended.c, gsocket.c, unzip.c must be compiled using $(CC), not $(CCC).
#   They may also be wrongly specified as extended.cpp, etc.

WXDIR = ..\..

!include makewat.env

WXLIB = $(WXDIR)\lib

LIBTARGET   = $(WXLIB)\wx.lib
DUMMY=dummydll
# ODBCLIB     = ..\..\contrib\odbc\odbc32.lib

EXTRATARGETS = png zlib jpeg tiff regex
EXTRATARGETSCLEAN = clean_png clean_zlib clean_jpeg clean_tiff clean_regex
GENDIR=$(WXDIR)\src\generic
COMMDIR=$(WXDIR)\src\common
JPEGDIR=$(WXDIR)\src\jpeg
TIFFDIR=$(WXDIR)\src\tiff
MGLDIR=$(WXDIR)\src\mgl
UNIVDIR=$(WXDIR)\src\univ
UNIVTHEMEDIR=$(WXDIR)\src\univ\themes
HTMLDIR=$(WXDIR)\src\html

DOCDIR = $(WXDIR)\docs

GENERICOBJS= accel.obj &
	busyinfo.obj &
	calctrl.obj &
	caret.obj &
	choicdgg.obj &
	colrdlgg.obj &
	dcpsg.obj &
	dirctrlg.obj &
	dirdlgg.obj &
	dragimgg.obj &
	fdrepdlg.obj &
	filedlgg.obj &
	fontdlgg.obj &
	grid.obj &
	gridctrl.obj &
	gridsel.obj &
	helpext.obj &
	helphtml.obj &
	helpwxht.obj &
	imaglist.obj &
	laywin.obj &
	listctrl.obj &
	logg.obj &
	msgdlgg.obj &
	numdlgg.obj &
	panelg.obj &
	printps.obj &
	prntdlgg.obj &
	progdlgg.obj &
	prop.obj &
	propform.obj &
	proplist.obj &
	sashwin.obj &
	scrlwing.obj &
	spinctlg.obj &
	splash.obj &
	splitter.obj &
	tabg.obj &
	tbarsmpl.obj &
	textdlgg.obj &
	tipdlg.obj &
	tipwin.obj &
	treectlg.obj &
	treelay.obj &
	wizard.obj

COMMONOBJS = &
	y_tab.obj &
	appcmn.obj &
	bmpbase.obj &
	choiccmn.obj &
	clipcmn.obj &
	clntdata.obj &
	cmdline.obj &
	cmdproc.obj &
	cmndata.obj &
	config.obj &
	containr.obj &
	cshelp.obj &
	ctrlcmn.obj &
	ctrlsub.obj &
	datetime.obj &
	datstrm.obj &
	db.obj &
	dbgrid.obj &
	dbtable.obj &
	dcbase.obj &
	dircmn.obj &
	dlgcmn.obj &
	dndcmn.obj &
	dobjcmn.obj &
	docmdi.obj &
	docview.obj &
	dseldlg.obj &
	dynarray.obj &
	dynlib.obj &
	dynload.obj &
	effects.obj &
	encconv.obj &
	event.obj &
	extended.obj &
	fddlgcmn.obj &
	ffile.obj &
	file.obj &
	fileconf.obj &
	filefn.obj &
	filename.obj &
	filesys.obj &
	fontcmn.obj &
	fontmap.obj &
	framecmn.obj &
	fs_inet.obj &
	fs_mem.obj &
	fs_zip.obj &
	ftp.obj &
	gaugecmn.obj &
	gdicmn.obj &
	geometry.obj &
	gifdecod.obj &
	hash.obj &
	helpbase.obj &
	http.obj &
	imagall.obj &
	imagbmp.obj &
	image.obj &
	imaggif.obj &
	imagiff.obj &
	imagjpeg.obj &
	imagpcx.obj &
	imagpng.obj &
	imagpnm.obj &
	imagtiff.obj &
	imagxpm.obj &
	intl.obj &
	ipcbase.obj &
	layout.obj &
	lboxcmn.obj &
	list.obj &
	log.obj &
	longlong.obj &
	matrix.obj &
	memory.obj &
	menucmn.obj &
	mimecmn.obj &
	module.obj &
	mstream.obj &
	nbkbase.obj &
	object.obj &
	odbc.obj &
	paper.obj &
	popupcmn.obj &
	prntbase.obj &
	process.obj &
	protocol.obj &
	quantize.obj &
	radiocmn.obj &
	regex.obj &
	resource.obj &
	sckaddr.obj &
	sckfile.obj &
	sckipc.obj &
	sckstrm.obj &
	sizer.obj &
	socket.obj &
	statbar.obj &
	strconv.obj &
	stream.obj &
	string.obj &
	sysopt.obj &
	tbarbase.obj &
	textbuf.obj &
	textcmn.obj &
	textfile.obj &
	timercmn.obj &
	tokenzr.obj &
	toplvcmn.obj &
	treebase.obj &
	txtstrm.obj &
	unzip.obj &
	url.obj &
	utilscmn.obj &
	valgen.obj &
	validate.obj &
	valtext.obj &
	variant.obj &
	wfstream.obj &
	wincmn.obj &
	wxchar.obj &
	wxexpr.obj &
	xpmdecod.obj &
	zipstrm.obj &
	zstream.obj

MGLOBJS = app.obj &
	bitmap.obj &
	brush.obj &
	clipbrd.obj &
	colour.obj &
	cursor.obj &
	data.obj &
	dc.obj &
	dcclient.obj &
	dcmemory.obj &
	dcscreen.obj &
	dirmgl.obj &
	evtloop.obj &
	font.obj &
	fontenum.obj &
	fontutil.obj &
	gdiobj.obj &
	icon.obj &
	palette.obj &
	pen.obj &
	region.obj &
	settings.obj &
	timer.obj &
	toplevel.obj &
	utils.obj &
	window.obj

UNIVOBJS = bmpbuttn.obj &
	button.obj &
	checkbox.obj &
	checklst.obj &
	choice.obj &
	colschem.obj &
	combobox.obj &
	control.obj &
	dialog.obj &
	framuniv.obj &
	gauge.obj &
	inpcons.obj &
	inphand.obj &
	listbox.obj &
	menu.obj &
	notebook.obj &
	radiobox.obj &
	radiobut.obj &
	renderer.obj &
	scrarrow.obj &
	scrolbar.obj &
	scrthumb.obj &
	slider.obj &
	spinbutt.obj &
	statbmp.obj &
	statbox.obj &
	statline.obj &
	stattext.obj &
	statusbr.obj &
	textctrl.obj &
	theme.obj &
	topluniv.obj &
	winuniv.obj

UNIVTHEMEOBJS = gtk.obj &
	win32.obj

HTMLOBJS = helpctrl.obj &
	helpdata.obj &
	helpfrm.obj &
	htmlcell.obj &
	htmlfilt.obj &
	htmlpars.obj &
	htmltag.obj &
	htmlwin.obj &
	htmprint.obj &
	m_dflist.obj &
	m_fonts.obj &
	m_hline.obj &
	m_image.obj &
	m_layout.obj &
	m_links.obj &
	m_list.obj &
	m_meta.obj &
	m_pre.obj &
	m_style.obj &
	m_tables.obj &
	winpars.obj

# Add $(NONESSENTIALOBJS) if wanting generic dialogs, PostScript etc.
OBJECTS = $(COMMONOBJS) $(GENERICOBJS) $(MGLOBJS) $(UNIVOBJS) $(UNIVTHEMEOBJS) $(HTMLOBJS)

SETUP_H=$(ARCHINCDIR)\wx\setup.h

all:        $(SETUP_H) $(OBJECTS) $(LIBTARGET) $(EXTRATARGETS) .SYMBOLIC

$(ARCHINCDIR)\wx:
    mkdir $(ARCHINCDIR)
    mkdir $(ARCHINCDIR)\wx

$(SETUP_H): $(WXDIR)\include\wx\mgl\setup.h $(ARCHINCDIR)\wx
    copy $(WXDIR)\include\wx\mgl\setup.h $@

$(LIBTARGET) : $(OBJECTS)
    %create tmp.lbc
    @for %i in ( $(OBJECTS) ) do @%append tmp.lbc +%i
    wlib /b /c /n /p=512 $^@ @tmp.lbc

clean:   .SYMBOLIC $(EXTRATARGETSCLEAN)
    -erase *.obj
    -erase $(LIBTARGET)
    -erase *.pch
    -erase *.err
    -erase *.lbc

cleanall:   clean

app.obj:     $(MGLDIR)\app.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

bitmap.obj:     $(MGLDIR)\bitmap.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

brush.obj:     $(MGLDIR)\brush.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

clipbrd.obj:     $(MGLDIR)\clipbrd.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

colour.obj:     $(MGLDIR)\colour.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

cursor.obj:     $(MGLDIR)\cursor.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

data.obj:     $(MGLDIR)\data.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dc.obj:     $(MGLDIR)\dc.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dcclient.obj:     $(MGLDIR)\dcclient.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dcmemory.obj:     $(MGLDIR)\dcmemory.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dcscreen.obj:     $(MGLDIR)\dcscreen.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dirmgl.obj:     $(MGLDIR)\dirmgl.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

evtloop.obj:     $(MGLDIR)\evtloop.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

font.obj:     $(MGLDIR)\font.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

fontenum.obj:     $(MGLDIR)\fontenum.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

fontutil.obj:     $(MGLDIR)\fontutil.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

gdiobj.obj:     $(MGLDIR)\gdiobj.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

icon.obj:     $(MGLDIR)\icon.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

palette.obj:     $(MGLDIR)\palette.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

pen.obj:     $(MGLDIR)\pen.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

region.obj:     $(MGLDIR)\region.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

settings.obj:     $(MGLDIR)\settings.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

timer.obj:     $(MGLDIR)\timer.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

toplevel.obj:     $(MGLDIR)\toplevel.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

utils.obj:     $(MGLDIR)\utils.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

window.obj:     $(MGLDIR)\window.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<



########################################################
# wxUNIVERSAL objects (always compiled)

bmpbuttn.obj:     $(UNIVDIR)\bmpbuttn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

button.obj:     $(UNIVDIR)\button.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

checkbox.obj:     $(UNIVDIR)\checkbox.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

checklst.obj:     $(UNIVDIR)\checklst.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

choice.obj:     $(UNIVDIR)\choice.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

colschem.obj:     $(UNIVDIR)\colschem.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

combobox.obj:     $(UNIVDIR)\combobox.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

control.obj:     $(UNIVDIR)\control.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dialog.obj:     $(UNIVDIR)\dialog.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

framuniv.obj:     $(UNIVDIR)\framuniv.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

gauge.obj:     $(UNIVDIR)\gauge.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

inpcons.obj:     $(UNIVDIR)\inpcons.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

inphand.obj:     $(UNIVDIR)\inphand.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

listbox.obj:     $(UNIVDIR)\listbox.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

menu.obj:     $(UNIVDIR)\menu.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

notebook.obj:     $(UNIVDIR)\notebook.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

radiobox.obj:     $(UNIVDIR)\radiobox.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

radiobut.obj:     $(UNIVDIR)\radiobut.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

renderer.obj:     $(UNIVDIR)\renderer.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

scrarrow.obj:     $(UNIVDIR)\scrarrow.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

scrolbar.obj:     $(UNIVDIR)\scrolbar.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

scrthumb.obj:     $(UNIVDIR)\scrthumb.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

slider.obj:     $(UNIVDIR)\slider.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

spinbutt.obj:     $(UNIVDIR)\spinbutt.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

statbmp.obj:     $(UNIVDIR)\statbmp.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

statbox.obj:     $(UNIVDIR)\statbox.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

statline.obj:     $(UNIVDIR)\statline.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

stattext.obj:     $(UNIVDIR)\stattext.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

statusbr.obj:     $(UNIVDIR)\statusbr.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

textctrl.obj:     $(UNIVDIR)\textctrl.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

theme.obj:     $(UNIVDIR)\theme.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

topluniv.obj:     $(UNIVDIR)\topluniv.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

winuniv.obj:     $(UNIVDIR)\winuniv.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<



gtk.obj:     $(UNIVTHEMEDIR)\gtk.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

win32.obj:     $(UNIVTHEMEDIR)\win32.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<



########################################################
# Common objects (always compiled)

appcmn.obj:     $(COMMDIR)\appcmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

bmpbase.obj:     $(COMMDIR)\bmpbase.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

choiccmn.obj:     $(COMMDIR)\choiccmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

clipcmn.obj:     $(COMMDIR)\clipcmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

clntdata.obj:     $(COMMDIR)\clntdata.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

cmdline.obj:     $(COMMDIR)\cmdline.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

cmdproc.obj:     $(COMMDIR)\cmdproc.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

cmndata.obj:     $(COMMDIR)\cmndata.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

config.obj:     $(COMMDIR)\config.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

containr.obj:     $(COMMDIR)\containr.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

cshelp.obj:     $(COMMDIR)\cshelp.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

ctrlcmn.obj:     $(COMMDIR)\ctrlcmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

ctrlsub.obj:     $(COMMDIR)\ctrlsub.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

datetime.obj:     $(COMMDIR)\datetime.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

datstrm.obj:     $(COMMDIR)\datstrm.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

db.obj:     $(COMMDIR)\db.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dbgrid.obj:     $(COMMDIR)\dbgrid.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dbtable.obj:     $(COMMDIR)\dbtable.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dcbase.obj:     $(COMMDIR)\dcbase.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dircmn.obj:     $(COMMDIR)\dircmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dlgcmn.obj:     $(COMMDIR)\dlgcmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dndcmn.obj:     $(COMMDIR)\dndcmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dobjcmn.obj:     $(COMMDIR)\dobjcmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

docmdi.obj:     $(COMMDIR)\docmdi.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

docview.obj:     $(COMMDIR)\docview.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dseldlg.obj:     $(COMMDIR)\dseldlg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dynarray.obj:     $(COMMDIR)\dynarray.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dynlib.obj:     $(COMMDIR)\dynlib.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dynload.obj:     $(COMMDIR)\dynload.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

effects.obj:     $(COMMDIR)\effects.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

encconv.obj:     $(COMMDIR)\encconv.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

event.obj:     $(COMMDIR)\event.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

extended.obj:     $(COMMDIR)\extended.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

fddlgcmn.obj:     $(COMMDIR)\fddlgcmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

ffile.obj:     $(COMMDIR)\ffile.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

file.obj:     $(COMMDIR)\file.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

fileconf.obj:     $(COMMDIR)\fileconf.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

filefn.obj:     $(COMMDIR)\filefn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

filename.obj:     $(COMMDIR)\filename.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

filesys.obj:     $(COMMDIR)\filesys.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

fontcmn.obj:     $(COMMDIR)\fontcmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

fontmap.obj:     $(COMMDIR)\fontmap.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

framecmn.obj:     $(COMMDIR)\framecmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

fs_inet.obj:     $(COMMDIR)\fs_inet.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

fs_mem.obj:     $(COMMDIR)\fs_mem.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

fs_zip.obj:     $(COMMDIR)\fs_zip.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

ftp.obj:     $(COMMDIR)\ftp.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

gaugecmn.obj:     $(COMMDIR)\gaugecmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

gdicmn.obj:     $(COMMDIR)\gdicmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

geometry.obj:     $(COMMDIR)\geometry.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

gifdecod.obj:     $(COMMDIR)\gifdecod.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

hash.obj:     $(COMMDIR)\hash.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

helpbase.obj:     $(COMMDIR)\helpbase.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

http.obj:     $(COMMDIR)\http.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

imagall.obj:     $(COMMDIR)\imagall.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

imagbmp.obj:     $(COMMDIR)\imagbmp.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

image.obj:     $(COMMDIR)\image.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

imaggif.obj:     $(COMMDIR)\imaggif.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

imagiff.obj:     $(COMMDIR)\imagiff.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

imagjpeg.obj:     $(COMMDIR)\imagjpeg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

imagpcx.obj:     $(COMMDIR)\imagpcx.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

imagpng.obj:     $(COMMDIR)\imagpng.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

imagpnm.obj:     $(COMMDIR)\imagpnm.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

imagtiff.obj:     $(COMMDIR)\imagtiff.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

imagxpm.obj:     $(COMMDIR)\imagxpm.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

intl.obj:     $(COMMDIR)\intl.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

ipcbase.obj:     $(COMMDIR)\ipcbase.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

layout.obj:     $(COMMDIR)\layout.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

lboxcmn.obj:     $(COMMDIR)\lboxcmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

list.obj:     $(COMMDIR)\list.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

log.obj:     $(COMMDIR)\log.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

longlong.obj:     $(COMMDIR)\longlong.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

matrix.obj:     $(COMMDIR)\matrix.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

memory.obj:     $(COMMDIR)\memory.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

menucmn.obj:     $(COMMDIR)\menucmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

mimecmn.obj:     $(COMMDIR)\mimecmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

module.obj:     $(COMMDIR)\module.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

mstream.obj:     $(COMMDIR)\mstream.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

nbkbase.obj:     $(COMMDIR)\nbkbase.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

object.obj:     $(COMMDIR)\object.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

odbc.obj:     $(COMMDIR)\odbc.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

paper.obj:     $(COMMDIR)\paper.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

popupcmn.obj:     $(COMMDIR)\popupcmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

prntbase.obj:     $(COMMDIR)\prntbase.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

process.obj:     $(COMMDIR)\process.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

protocol.obj:     $(COMMDIR)\protocol.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

quantize.obj:     $(COMMDIR)\quantize.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

radiocmn.obj:     $(COMMDIR)\radiocmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

regex.obj:     $(COMMDIR)\regex.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

resource.obj:     $(COMMDIR)\resource.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

sckaddr.obj:     $(COMMDIR)\sckaddr.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

sckfile.obj:     $(COMMDIR)\sckfile.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

sckipc.obj:     $(COMMDIR)\sckipc.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

sckstrm.obj:     $(COMMDIR)\sckstrm.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

sizer.obj:     $(COMMDIR)\sizer.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

socket.obj:     $(COMMDIR)\socket.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

statbar.obj:     $(COMMDIR)\statbar.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

strconv.obj:     $(COMMDIR)\strconv.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

stream.obj:     $(COMMDIR)\stream.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

string.obj:     $(COMMDIR)\string.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

sysopt.obj:     $(COMMDIR)\sysopt.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

tbarbase.obj:     $(COMMDIR)\tbarbase.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

textbuf.obj:     $(COMMDIR)\textbuf.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

textcmn.obj:     $(COMMDIR)\textcmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

textfile.obj:     $(COMMDIR)\textfile.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

timercmn.obj:     $(COMMDIR)\timercmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

tokenzr.obj:     $(COMMDIR)\tokenzr.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

toplvcmn.obj:     $(COMMDIR)\toplvcmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

treebase.obj:     $(COMMDIR)\treebase.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

txtstrm.obj:     $(COMMDIR)\txtstrm.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

unzip.obj:     $(COMMDIR)\unzip.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

url.obj:     $(COMMDIR)\url.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

utilscmn.obj:     $(COMMDIR)\utilscmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

valgen.obj:     $(COMMDIR)\valgen.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

validate.obj:     $(COMMDIR)\validate.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

valtext.obj:     $(COMMDIR)\valtext.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

variant.obj:     $(COMMDIR)\variant.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

wfstream.obj:     $(COMMDIR)\wfstream.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

wincmn.obj:     $(COMMDIR)\wincmn.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

wxchar.obj:     $(COMMDIR)\wxchar.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

wxexpr.obj:     $(COMMDIR)\wxexpr.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

xpmdecod.obj:     $(COMMDIR)\xpmdecod.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

zipstrm.obj:     $(COMMDIR)\zipstrm.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

zstream.obj:     $(COMMDIR)\zstream.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<



y_tab.obj:     $(COMMDIR)\y_tab.c $(COMMDIR)\lex_yy.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) -DUSE_DEFINE $(COMMDIR)\y_tab.c

#  *$(CC) $(CPPFLAGS) $(IFLAGS) -DUSE_DEFINE -DYY_USE_PROTOS $(COMMDIR)\y_tab.c

$(COMMDIR)\y_tab.c:     $(COMMDIR)\dosyacc.c
        copy $(COMMDIR)\dosyacc.c $(COMMDIR)\y_tab.c

$(COMMDIR)\lex_yy.c:    $(COMMDIR)\doslex.c
    copy $(COMMDIR)\doslex.c $(COMMDIR)\lex_yy.c

########################################################
# Generic objects (not always compiled, depending on
# whether platforms have native implementations)

accel.obj:     $(GENDIR)\accel.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

busyinfo.obj:     $(GENDIR)\busyinfo.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

calctrl.obj:     $(GENDIR)\calctrl.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

caret.obj:     $(GENDIR)\caret.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

choicdgg.obj:     $(GENDIR)\choicdgg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

colrdlgg.obj:     $(GENDIR)\colrdlgg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dcpsg.obj:     $(GENDIR)\dcpsg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dirctrlg.obj:     $(GENDIR)\dirctrlg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dirdlgg.obj:     $(GENDIR)\dirdlgg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

dragimgg.obj:     $(GENDIR)\dragimgg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

fdrepdlg.obj:     $(GENDIR)\fdrepdlg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

filedlgg.obj:     $(GENDIR)\filedlgg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

fontdlgg.obj:     $(GENDIR)\fontdlgg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

grid.obj:     $(GENDIR)\grid.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

gridctrl.obj:     $(GENDIR)\gridctrl.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

gridsel.obj:     $(GENDIR)\gridsel.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

helpext.obj:     $(GENDIR)\helpext.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

helphtml.obj:     $(GENDIR)\helphtml.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

helpwxht.obj:     $(GENDIR)\helpwxht.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

imaglist.obj:     $(GENDIR)\imaglist.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

laywin.obj:     $(GENDIR)\laywin.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

listctrl.obj:     $(GENDIR)\listctrl.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

logg.obj:     $(GENDIR)\logg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

msgdlgg.obj:     $(GENDIR)\msgdlgg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

numdlgg.obj:     $(GENDIR)\numdlgg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

panelg.obj:     $(GENDIR)\panelg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

printps.obj:     $(GENDIR)\printps.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

prntdlgg.obj:     $(GENDIR)\prntdlgg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

progdlgg.obj:     $(GENDIR)\progdlgg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

prop.obj:     $(GENDIR)\prop.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

propform.obj:     $(GENDIR)\propform.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

proplist.obj:     $(GENDIR)\proplist.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

sashwin.obj:     $(GENDIR)\sashwin.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

scrlwing.obj:     $(GENDIR)\scrlwing.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

spinctlg.obj:     $(GENDIR)\spinctlg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

splash.obj:     $(GENDIR)\splash.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

splitter.obj:     $(GENDIR)\splitter.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

tabg.obj:     $(GENDIR)\tabg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

tbarsmpl.obj:     $(GENDIR)\tbarsmpl.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

textdlgg.obj:     $(GENDIR)\textdlgg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

tipdlg.obj:     $(GENDIR)\tipdlg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

tipwin.obj:     $(GENDIR)\tipwin.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

treectlg.obj:     $(GENDIR)\treectlg.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

treelay.obj:     $(GENDIR)\treelay.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

wizard.obj:     $(GENDIR)\wizard.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<




########################################################
# HTML objects (always compiled)

helpctrl.obj:     $(HTMLDIR)\helpctrl.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

helpdata.obj:     $(HTMLDIR)\helpdata.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

helpfrm.obj:     $(HTMLDIR)\helpfrm.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

htmlcell.obj:     $(HTMLDIR)\htmlcell.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

htmlfilt.obj:     $(HTMLDIR)\htmlfilt.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

htmlpars.obj:     $(HTMLDIR)\htmlpars.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

htmltag.obj:     $(HTMLDIR)\htmltag.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

htmlwin.obj:     $(HTMLDIR)\htmlwin.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

htmprint.obj:     $(HTMLDIR)\htmprint.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

m_dflist.obj:     $(HTMLDIR)\m_dflist.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

m_fonts.obj:     $(HTMLDIR)\m_fonts.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

m_hline.obj:     $(HTMLDIR)\m_hline.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

m_image.obj:     $(HTMLDIR)\m_image.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

m_layout.obj:     $(HTMLDIR)\m_layout.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

m_links.obj:     $(HTMLDIR)\m_links.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

m_list.obj:     $(HTMLDIR)\m_list.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

m_meta.obj:     $(HTMLDIR)\m_meta.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

m_pre.obj:     $(HTMLDIR)\m_pre.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

m_style.obj:     $(HTMLDIR)\m_style.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

m_tables.obj:     $(HTMLDIR)\m_tables.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<

winpars.obj:     $(HTMLDIR)\winpars.cpp
  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<




png:   .SYMBOLIC
    cd $(WXDIR)\src\png
    wmake -f makefile.wat all
    cd $(WXDIR)\src\mgl

clean_png:   .SYMBOLIC
    cd $(WXDIR)\src\png
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\mgl

zlib:   .SYMBOLIC
    cd $(WXDIR)\src\zlib
    wmake -f makefile.wat all
    cd $(WXDIR)\src\mgl

clean_zlib:   .SYMBOLIC
    cd $(WXDIR)\src\zlib
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\mgl

jpeg:    .SYMBOLIC
    cd $(WXDIR)\src\jpeg
    wmake -f makefile.wat all
    cd $(WXDIR)\src\mgl

clean_jpeg:   .SYMBOLIC
    cd $(WXDIR)\src\jpeg
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\mgl

tiff:    .SYMBOLIC
    cd $(WXDIR)\src\tiff
    wmake -f makefile.wat all
    cd $(WXDIR)\src\mgl

clean_tiff:   .SYMBOLIC
    cd $(WXDIR)\src\tiff
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\mgl

regex:    .SYMBOLIC
    cd $(WXDIR)\src\regex
    wmake -f makefile.wat all
    cd $(WXDIR)\src\mgl

clean_regex:   .SYMBOLIC
    cd $(WXDIR)\src\regex
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\mgl

MFTYPE=watmgl
self : .SYMBOLIC $(WXDIR)\distrib\msw\tmake\filelist.txt $(WXDIR)\distrib\msw\tmake\$(MFTYPE).t
	cd $(WXDIR)\distrib\msw\tmake
	tmake -t $(MFTYPE) wxwin.pro -o makefile.$(MFTYPE)
	copy makefile.$(MFTYPE) $(WXDIR)\src\mgl

// A bunch of %rename directives generated by ./distrib/build_renamers.py
// in order to remove the wx prefix from all global scope names.

#ifndef BUILDING_RENAMERS

%rename(Panel)                              wxPanel;
%rename(ScrolledWindow)                     wxScrolledWindow;
%rename(FULLSCREEN_NOMENUBAR)               wxFULLSCREEN_NOMENUBAR;
%rename(FULLSCREEN_NOTOOLBAR)               wxFULLSCREEN_NOTOOLBAR;
%rename(FULLSCREEN_NOSTATUSBAR)             wxFULLSCREEN_NOSTATUSBAR;
%rename(FULLSCREEN_NOBORDER)                wxFULLSCREEN_NOBORDER;
%rename(FULLSCREEN_NOCAPTION)               wxFULLSCREEN_NOCAPTION;
%rename(FULLSCREEN_ALL)                     wxFULLSCREEN_ALL;
%rename(TOPLEVEL_EX_DIALOG)                 wxTOPLEVEL_EX_DIALOG;
%rename(TopLevelWindow)                     wxTopLevelWindow;
%rename(Frame)                              wxFrame;
%rename(Dialog)                             wxDialog;
%rename(MiniFrame)                          wxMiniFrame;
%rename(SPLASH_CENTRE_ON_PARENT)            wxSPLASH_CENTRE_ON_PARENT;
%rename(SPLASH_CENTRE_ON_SCREEN)            wxSPLASH_CENTRE_ON_SCREEN;
%rename(SPLASH_NO_CENTRE)                   wxSPLASH_NO_CENTRE;
%rename(SPLASH_TIMEOUT)                     wxSPLASH_TIMEOUT;
%rename(SPLASH_NO_TIMEOUT)                  wxSPLASH_NO_TIMEOUT;
%rename(SplashScreenWindow)                 wxSplashScreenWindow;
%rename(SplashScreen)                       wxSplashScreen;
%rename(StatusBar)                          wxStatusBar;
%rename(SP_NOBORDER)                        wxSP_NOBORDER;
%rename(SP_NOSASH)                          wxSP_NOSASH;
%rename(SP_PERMIT_UNSPLIT)                  wxSP_PERMIT_UNSPLIT;
%rename(SP_LIVE_UPDATE)                     wxSP_LIVE_UPDATE;
%rename(SP_3DSASH)                          wxSP_3DSASH;
%rename(SP_3DBORDER)                        wxSP_3DBORDER;
%rename(SP_NO_XP_THEME)                     wxSP_NO_XP_THEME;
%rename(SP_BORDER)                          wxSP_BORDER;
%rename(SP_3D)                              wxSP_3D;
%rename(SPLIT_HORIZONTAL)                   wxSPLIT_HORIZONTAL;
%rename(SPLIT_VERTICAL)                     wxSPLIT_VERTICAL;
%rename(SPLIT_DRAG_NONE)                    wxSPLIT_DRAG_NONE;
%rename(SPLIT_DRAG_DRAGGING)                wxSPLIT_DRAG_DRAGGING;
%rename(SPLIT_DRAG_LEFT_DOWN)               wxSPLIT_DRAG_LEFT_DOWN;
%rename(SplitterWindow)                     wxSplitterWindow;
%rename(SplitterEvent)                      wxSplitterEvent;
%rename(SASH_DRAG_NONE)                     wxSASH_DRAG_NONE;
%rename(SASH_DRAG_DRAGGING)                 wxSASH_DRAG_DRAGGING;
%rename(SASH_DRAG_LEFT_DOWN)                wxSASH_DRAG_LEFT_DOWN;
%rename(SW_NOBORDER)                        wxSW_NOBORDER;
%rename(SW_BORDER)                          wxSW_BORDER;
%rename(SW_3DSASH)                          wxSW_3DSASH;
%rename(SW_3DBORDER)                        wxSW_3DBORDER;
%rename(SW_3D)                              wxSW_3D;
%rename(SASH_TOP)                           wxSASH_TOP;
%rename(SASH_RIGHT)                         wxSASH_RIGHT;
%rename(SASH_BOTTOM)                        wxSASH_BOTTOM;
%rename(SASH_LEFT)                          wxSASH_LEFT;
%rename(SASH_NONE)                          wxSASH_NONE;
%rename(SashWindow)                         wxSashWindow;
%rename(SASH_STATUS_OK)                     wxSASH_STATUS_OK;
%rename(SASH_STATUS_OUT_OF_RANGE)           wxSASH_STATUS_OUT_OF_RANGE;
%rename(SashEvent)                          wxSashEvent;
%rename(LAYOUT_HORIZONTAL)                  wxLAYOUT_HORIZONTAL;
%rename(LAYOUT_VERTICAL)                    wxLAYOUT_VERTICAL;
%rename(LAYOUT_NONE)                        wxLAYOUT_NONE;
%rename(LAYOUT_TOP)                         wxLAYOUT_TOP;
%rename(LAYOUT_LEFT)                        wxLAYOUT_LEFT;
%rename(LAYOUT_RIGHT)                       wxLAYOUT_RIGHT;
%rename(LAYOUT_BOTTOM)                      wxLAYOUT_BOTTOM;
%rename(LAYOUT_LENGTH_Y)                    wxLAYOUT_LENGTH_Y;
%rename(LAYOUT_LENGTH_X)                    wxLAYOUT_LENGTH_X;
%rename(LAYOUT_MRU_LENGTH)                  wxLAYOUT_MRU_LENGTH;
%rename(LAYOUT_QUERY)                       wxLAYOUT_QUERY;
%rename(QueryLayoutInfoEvent)               wxQueryLayoutInfoEvent;
%rename(CalculateLayoutEvent)               wxCalculateLayoutEvent;
%rename(SashLayoutWindow)                   wxSashLayoutWindow;
%rename(LayoutAlgorithm)                    wxLayoutAlgorithm;
%rename(PopupWindow)                        wxPopupWindow;
%rename(TipWindow)                          wxTipWindow;
%rename(TaskBarIcon)                        wxTaskBarIcon;
%rename(TaskBarIconEvent)                   wxTaskBarIconEvent;
%rename(ColourData)                         wxColourData;
%rename(ColourDialog)                       wxColourDialog;
%rename(DirDialog)                          wxDirDialog;
%rename(FileDialog)                         wxFileDialog;
%rename(CHOICEDLG_STYLE)                    wxCHOICEDLG_STYLE;
%rename(MultiChoiceDialog)                  wxMultiChoiceDialog;
%rename(SingleChoiceDialog)                 wxSingleChoiceDialog;
%rename(TextEntryDialog)                    wxTextEntryDialog;
%rename(FontData)                           wxFontData;
%rename(FontDialog)                         wxFontDialog;
%rename(MessageDialog)                      wxMessageDialog;
%rename(ProgressDialog)                     wxProgressDialog;
%rename(FR_DOWN)                            wxFR_DOWN;
%rename(FR_WHOLEWORD)                       wxFR_WHOLEWORD;
%rename(FR_MATCHCASE)                       wxFR_MATCHCASE;
%rename(FR_REPLACEDIALOG)                   wxFR_REPLACEDIALOG;
%rename(FR_NOUPDOWN)                        wxFR_NOUPDOWN;
%rename(FR_NOMATCHCASE)                     wxFR_NOMATCHCASE;
%rename(FR_NOWHOLEWORD)                     wxFR_NOWHOLEWORD;
%rename(FindDialogEvent)                    wxFindDialogEvent;
%rename(FindReplaceData)                    wxFindReplaceData;
%rename(FindReplaceDialog)                  wxFindReplaceDialog;
%rename(FIRST_MDI_CHILD)                    wxFIRST_MDI_CHILD;
%rename(LAST_MDI_CHILD)                     wxLAST_MDI_CHILD;
%rename(MDIParentFrame)                     wxMDIParentFrame;
%rename(MDIChildFrame)                      wxMDIChildFrame;
%rename(MDIClientWindow)                    wxMDIClientWindow;
%rename(PyWindow)                           wxPyWindow;
%rename(PyPanel)                            wxPyPanel;
%rename(PRINT_MODE_NONE)                    wxPRINT_MODE_NONE;
%rename(PRINT_MODE_PREVIEW)                 wxPRINT_MODE_PREVIEW;
%rename(PRINT_MODE_FILE)                    wxPRINT_MODE_FILE;
%rename(PRINT_MODE_PRINTER)                 wxPRINT_MODE_PRINTER;
%rename(PRINT_MODE_STREAM)                  wxPRINT_MODE_STREAM;
%rename(PrintData)                          wxPrintData;
%rename(PageSetupDialogData)                wxPageSetupDialogData;
%rename(PageSetupDialog)                    wxPageSetupDialog;
%rename(PrintDialogData)                    wxPrintDialogData;
%rename(PrintDialog)                        wxPrintDialog;
%rename(PRINTER_NO_ERROR)                   wxPRINTER_NO_ERROR;
%rename(PRINTER_CANCELLED)                  wxPRINTER_CANCELLED;
%rename(PRINTER_ERROR)                      wxPRINTER_ERROR;
%rename(Printer)                            wxPrinter;
%rename(PreviewCanvas)                      wxPreviewCanvas;
%rename(PreviewFrame)                       wxPreviewFrame;
%rename(PREVIEW_PRINT)                      wxPREVIEW_PRINT;
%rename(PREVIEW_PREVIOUS)                   wxPREVIEW_PREVIOUS;
%rename(PREVIEW_NEXT)                       wxPREVIEW_NEXT;
%rename(PREVIEW_ZOOM)                       wxPREVIEW_ZOOM;
%rename(PREVIEW_FIRST)                      wxPREVIEW_FIRST;
%rename(PREVIEW_LAST)                       wxPREVIEW_LAST;
%rename(PREVIEW_GOTO)                       wxPREVIEW_GOTO;
%rename(PREVIEW_DEFAULT)                    wxPREVIEW_DEFAULT;
%rename(ID_PREVIEW_CLOSE)                   wxID_PREVIEW_CLOSE;
%rename(ID_PREVIEW_NEXT)                    wxID_PREVIEW_NEXT;
%rename(ID_PREVIEW_PREVIOUS)                wxID_PREVIEW_PREVIOUS;
%rename(ID_PREVIEW_PRINT)                   wxID_PREVIEW_PRINT;
%rename(ID_PREVIEW_ZOOM)                    wxID_PREVIEW_ZOOM;
%rename(ID_PREVIEW_FIRST)                   wxID_PREVIEW_FIRST;
%rename(ID_PREVIEW_LAST)                    wxID_PREVIEW_LAST;
%rename(ID_PREVIEW_GOTO)                    wxID_PREVIEW_GOTO;
%rename(PreviewControlBar)                  wxPreviewControlBar;
%rename(PrintPreview)                       wxPrintPreview;
%rename(PyPrintPreview)                     wxPyPrintPreview;
%rename(PyPreviewFrame)                     wxPyPreviewFrame;
%rename(PyPreviewControlBar)                wxPyPreviewControlBar;

#endif

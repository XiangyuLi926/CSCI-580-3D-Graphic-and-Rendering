; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFractalOptionsDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Fractal Example.h"
LastPage=0

ClassCount=7
Class1=CFractalExampleApp
Class2=CFractalExampleDoc
Class3=CFractalExampleView
Class4=CMainFrame

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Class6=COpenGLView
Class7=CFractalOptionsDlg
Resource3=IDD_FRACTAL_OPTIONS

[CLS:CFractalExampleApp]
Type=0
HeaderFile=Fractal Example.h
ImplementationFile=Fractal Example.cpp
Filter=N

[CLS:CFractalExampleDoc]
Type=0
HeaderFile=Fractal ExampleDoc.h
ImplementationFile=Fractal ExampleDoc.cpp
Filter=N

[CLS:CFractalExampleView]
Type=0
HeaderFile=Fractal ExampleView.h
ImplementationFile=Fractal ExampleView.cpp
Filter=C
LastObject=CFractalExampleView
BaseClass=COpenGLView
VirtualFilter=VWC

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame



[CLS:CAboutDlg]
Type=0
HeaderFile=Fractal Example.cpp
ImplementationFile=Fractal Example.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_APP_EXIT
Command3=ID_VIEW_OPTIONS
Command4=ID_APP_ABOUT
CommandCount=4

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_VIEW_OPTIONS
Command4=ID_FILE_SAVE
Command5=ID_EDIT_PASTE
Command6=ID_EDIT_UNDO
Command7=ID_EDIT_CUT
Command8=ID_NEXT_PANE
Command9=ID_PREV_PANE
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_EDIT_CUT
Command13=ID_EDIT_UNDO
CommandCount=13

[CLS:COpenGLView]
Type=0
HeaderFile=copenglview.h
ImplementationFile=copenglview.cpp
BaseClass=CView
Filter=C
VirtualFilter=VWC
LastObject=COpenGLView

[DLG:IDD_FRACTAL_OPTIONS]
Type=1
Class=CFractalOptionsDlg
ControlCount=24
Control1=IDC_F_RENDER_TYPE,combobox,1344339970
Control2=IDC_F_AA_LINES,button,1342242819
Control3=IDC_F_INVERT,button,1342242819
Control4=IDC_F_TILE,edit,1350631552
Control5=IDC_F_RSEED,edit,1350631552
Control6=ID_F_SURFACE_H,edit,1350631552
Control7=IDC_F_ITERATIONS,edit,1350631552
Control8=IDC_F_TEX_LINEAR,button,1342242819
Control9=ID_F_CLOUD_H,edit,1350631552
Control10=IDC_F_CLOUD_ITERATIONS,edit,1350631552
Control11=ID_F_TEXIMAGE_H,edit,1350631552
Control12=IDC_F_TEXIMAGE_ITERATIONS,edit,1350631552
Control13=IDOK,button,1342242817
Control14=IDCANCEL,button,1342242816
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,button,1342177287
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352

[CLS:CFractalOptionsDlg]
Type=0
HeaderFile=FractalOptionsDlg.h
ImplementationFile=FractalOptionsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CFractalOptionsDlg
VirtualFilter=dWC


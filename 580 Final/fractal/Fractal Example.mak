# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Fractal Example - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Fractal Example - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "Fractal Example - Win32 Release" && "$(CFG)" !=\
 "Fractal Example - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Fractal Example.mak" CFG="Fractal Example - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Fractal Example - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Fractal Example - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Fractal Example - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "Fractal Example - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Fractal Example.exe"

CLEAN : 
	-@erase ".\Release\Fractal Example.exe"
	-@erase ".\Release\FractalOptionsDlg.obj"
	-@erase ".\Release\Fractal Example.pch"
	-@erase ".\Release\MainFrm.obj"
	-@erase ".\Release\Fractal ExampleDoc.obj"
	-@erase ".\Release\COpenGLView.obj"
	-@erase ".\Release\fractmod.obj"
	-@erase ".\Release\Fractal ExampleView.obj"
	-@erase ".\Release\StdAfx.obj"
	-@erase ".\Release\Fractal Example.obj"
	-@erase ".\Release\Fractal Example.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /I "c:/oglsdk/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /I "c:/oglsdk/include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Fractal Example.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Fractal Example.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Fractal Example.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 c:/oglsdk/lib/opengl.lib c:/oglsdk/lib/glu.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=c:/oglsdk/lib/opengl.lib c:/oglsdk/lib/glu.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)/Fractal Example.pdb"\
 /machine:I386 /out:"$(OUTDIR)/Fractal Example.exe" 
LINK32_OBJS= \
	"$(INTDIR)/FractalOptionsDlg.obj" \
	"$(INTDIR)/MainFrm.obj" \
	"$(INTDIR)/Fractal ExampleDoc.obj" \
	"$(INTDIR)/COpenGLView.obj" \
	"$(INTDIR)/fractmod.obj" \
	"$(INTDIR)/Fractal ExampleView.obj" \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/Fractal Example.obj" \
	"$(INTDIR)/Fractal Example.res"

"$(OUTDIR)\Fractal Example.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Fractal Example - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Fractal Example.exe" "$(OUTDIR)\Fractal Example.bsc"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\Fractal Example.pch"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\Fractal Example.bsc"
	-@erase ".\Debug\COpenGLView.sbr"
	-@erase ".\Debug\MainFrm.sbr"
	-@erase ".\Debug\StdAfx.sbr"
	-@erase ".\Debug\Fractal ExampleDoc.sbr"
	-@erase ".\Debug\fractmod.sbr"
	-@erase ".\Debug\Fractal Example.sbr"
	-@erase ".\Debug\FractalOptionsDlg.sbr"
	-@erase ".\Debug\Fractal ExampleView.sbr"
	-@erase ".\Debug\Fractal Example.exe"
	-@erase ".\Debug\Fractal ExampleView.obj"
	-@erase ".\Debug\COpenGLView.obj"
	-@erase ".\Debug\MainFrm.obj"
	-@erase ".\Debug\StdAfx.obj"
	-@erase ".\Debug\Fractal ExampleDoc.obj"
	-@erase ".\Debug\fractmod.obj"
	-@erase ".\Debug\Fractal Example.obj"
	-@erase ".\Debug\FractalOptionsDlg.obj"
	-@erase ".\Debug\Fractal Example.res"
	-@erase ".\Debug\Fractal Example.ilk"
	-@erase ".\Debug\Fractal Example.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "c:/oglsdk/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "c:/oglsdk/include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/"\
 /Fp"$(INTDIR)/Fractal Example.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Fractal Example.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Fractal Example.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/COpenGLView.sbr" \
	"$(INTDIR)/MainFrm.sbr" \
	"$(INTDIR)/StdAfx.sbr" \
	"$(INTDIR)/Fractal ExampleDoc.sbr" \
	"$(INTDIR)/fractmod.sbr" \
	"$(INTDIR)/Fractal Example.sbr" \
	"$(INTDIR)/FractalOptionsDlg.sbr" \
	"$(INTDIR)/Fractal ExampleView.sbr"

"$(OUTDIR)\Fractal Example.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 c:/oglsdk/lib/opengl.lib c:/oglsdk/lib/glu.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=c:/oglsdk/lib/opengl.lib c:/oglsdk/lib/glu.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/Fractal Example.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/Fractal Example.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Fractal ExampleView.obj" \
	"$(INTDIR)/COpenGLView.obj" \
	"$(INTDIR)/MainFrm.obj" \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/Fractal ExampleDoc.obj" \
	"$(INTDIR)/fractmod.obj" \
	"$(INTDIR)/Fractal Example.obj" \
	"$(INTDIR)/FractalOptionsDlg.obj" \
	"$(INTDIR)/Fractal Example.res"

"$(OUTDIR)\Fractal Example.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Fractal Example - Win32 Release"
# Name "Fractal Example - Win32 Debug"

!IF  "$(CFG)" == "Fractal Example - Win32 Release"

!ELSEIF  "$(CFG)" == "Fractal Example - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "Fractal Example - Win32 Release"

!ELSEIF  "$(CFG)" == "Fractal Example - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=".\Fractal Example.cpp"
DEP_CPP_FRACT=\
	".\StdAfx.h"\
	".\Fractal Example.h"\
	".\MainFrm.h"\
	".\Fractal ExampleDoc.h"\
	".\Fractal ExampleView.h"\
	".\COpenGLView.h"\
	

!IF  "$(CFG)" == "Fractal Example - Win32 Release"


"$(INTDIR)\Fractal Example.obj" : $(SOURCE) $(DEP_CPP_FRACT) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"


!ELSEIF  "$(CFG)" == "Fractal Example - Win32 Debug"


"$(INTDIR)\Fractal Example.obj" : $(SOURCE) $(DEP_CPP_FRACT) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"

"$(INTDIR)\Fractal Example.sbr" : $(SOURCE) $(DEP_CPP_FRACT) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "Fractal Example - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /I "c:/oglsdk/include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Fractal Example.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Fractal Example.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Fractal Example - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /I "c:/oglsdk/include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/"\
 /Fp"$(INTDIR)/Fractal Example.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\StdAfx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Fractal Example.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\StdAfx.h"\
	".\Fractal Example.h"\
	".\MainFrm.h"\
	

!IF  "$(CFG)" == "Fractal Example - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"


!ELSEIF  "$(CFG)" == "Fractal Example - Win32 Debug"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"

"$(INTDIR)\MainFrm.sbr" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=".\Fractal ExampleDoc.cpp"
DEP_CPP_FRACTA=\
	".\StdAfx.h"\
	".\Fractal Example.h"\
	".\Fractal ExampleDoc.h"\
	

!IF  "$(CFG)" == "Fractal Example - Win32 Release"


"$(INTDIR)\Fractal ExampleDoc.obj" : $(SOURCE) $(DEP_CPP_FRACTA) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"


!ELSEIF  "$(CFG)" == "Fractal Example - Win32 Debug"


"$(INTDIR)\Fractal ExampleDoc.obj" : $(SOURCE) $(DEP_CPP_FRACTA) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"

"$(INTDIR)\Fractal ExampleDoc.sbr" : $(SOURCE) $(DEP_CPP_FRACTA) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=".\Fractal ExampleView.cpp"

!IF  "$(CFG)" == "Fractal Example - Win32 Release"

DEP_CPP_FRACTAL=\
	".\StdAfx.h"\
	".\Fractal Example.h"\
	".\Fractal ExampleDoc.h"\
	".\Fractal ExampleView.h"\
	".\FractalOptionsDlg.h"\
	".\fractmod.h"\
	".\COpenGLView.h"\
	

"$(INTDIR)\Fractal ExampleView.obj" : $(SOURCE) $(DEP_CPP_FRACTAL) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"


!ELSEIF  "$(CFG)" == "Fractal Example - Win32 Debug"

DEP_CPP_FRACTAL=\
	".\StdAfx.h"\
	".\Fractal Example.h"\
	".\Fractal ExampleDoc.h"\
	".\Fractal ExampleView.h"\
	".\FractalOptionsDlg.h"\
	".\fractmod.h"\
	".\COpenGLView.h"\
	
NODEP_CPP_FRACTAL=\
	".\twoDR_lines"\
	

"$(INTDIR)\Fractal ExampleView.obj" : $(SOURCE) $(DEP_CPP_FRACTAL) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"

"$(INTDIR)\Fractal ExampleView.sbr" : $(SOURCE) $(DEP_CPP_FRACTAL) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=".\Fractal Example.rc"
DEP_RSC_FRACTAL_=\
	".\res\Fractal Example.ico"\
	".\res\Fractal ExampleDoc.ico"\
	".\res\Fractal Example.rc2"\
	

"$(INTDIR)\Fractal Example.res" : $(SOURCE) $(DEP_RSC_FRACTAL_) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\COpenGLView.cpp

!IF  "$(CFG)" == "Fractal Example - Win32 Release"

DEP_CPP_COPEN=\
	".\StdAfx.h"\
	".\COpenGLView.h"\
	

"$(INTDIR)\COpenGLView.obj" : $(SOURCE) $(DEP_CPP_COPEN) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"


!ELSEIF  "$(CFG)" == "Fractal Example - Win32 Debug"

DEP_CPP_COPEN=\
	".\StdAfx.h"\
	".\COpenGLView.h"\
	
NODEP_CPP_COPEN=\
	".\{"\
	

"$(INTDIR)\COpenGLView.obj" : $(SOURCE) $(DEP_CPP_COPEN) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"

"$(INTDIR)\COpenGLView.sbr" : $(SOURCE) $(DEP_CPP_COPEN) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fractmod.c

!IF  "$(CFG)" == "Fractal Example - Win32 Release"

DEP_CPP_FRACTM=\
	".\fractmod.h"\
	
# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\fractmod.obj" : $(SOURCE) $(DEP_CPP_FRACTM) "$(INTDIR)"
   $(CPP) /nologo /MD /W3 /GX /I "c:/oglsdk/include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "Fractal Example - Win32 Debug"

DEP_CPP_FRACTM=\
	".\fractmod.h"\
	
NODEP_CPP_FRACTM=\
	".\stride"\
	
# SUBTRACT CPP /YX /Yc /Yu

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /I "c:/oglsdk/include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\fractmod.obj" : $(SOURCE) $(DEP_CPP_FRACTM) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\fractmod.sbr" : $(SOURCE) $(DEP_CPP_FRACTM) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FractalOptionsDlg.cpp

!IF  "$(CFG)" == "Fractal Example - Win32 Release"

DEP_CPP_FRACTALO=\
	".\StdAfx.h"\
	".\Fractal Example.h"\
	".\FractalOptionsDlg.h"\
	

"$(INTDIR)\FractalOptionsDlg.obj" : $(SOURCE) $(DEP_CPP_FRACTALO) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"


!ELSEIF  "$(CFG)" == "Fractal Example - Win32 Debug"

DEP_CPP_FRACTALO=\
	".\StdAfx.h"\
	".\Fractal Example.h"\
	".\FractalOptionsDlg.h"\
	
NODEP_CPP_FRACTALO=\
	".\{"\
	

"$(INTDIR)\FractalOptionsDlg.obj" : $(SOURCE) $(DEP_CPP_FRACTALO) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"

"$(INTDIR)\FractalOptionsDlg.sbr" : $(SOURCE) $(DEP_CPP_FRACTALO) "$(INTDIR)"\
 "$(INTDIR)\Fractal Example.pch"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
################################################################################
# Section Fractal Example : {F4FA06A5-156D-11D1-A328-00608CC9A71F}
# 	0:15:COpenGLView.cpp:D:\c++\Fractal Example\COpenGLView.cpp
# 	0:13:COpenGLView.h:D:\c++\Fractal Example\COpenGLView.h
# 	2:18:CLASS: COpenGLView:COpenGLView
# 	2:15:COpenGLView.cpp:COpenGLView.cpp
# 	2:13:COpenGLView.h:COpenGLView.h
# 	2:19:Application Include:Fractal Example.h
# End Section
################################################################################

# Microsoft Developer Studio Project File - Name="nutsnbolts0" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=nutsnbolts0 - Win32 DLL (Release)
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "nutsnbolts0.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "nutsnbolts0.mak" CFG="nutsnbolts0 - Win32 DLL (Debug)"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "nutsnbolts0 - Win32 LIB (Release)" (based on "Win32 (x86) Static Library")
!MESSAGE "nutsnbolts0 - Win32 LIB (Debug)" (based on "Win32 (x86) Static Library")
!MESSAGE "nutsnbolts0 - Win32 DLL (Release)" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "nutsnbolts0 - Win32 DLL (Debug)" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "NUTSNBOLTS_DEBUG=0" /D "HAVE_CONFIG_H" /D "NB_MAKE_DLL" /D "NB_DEBUG=0" /D "NB_INTERNAL" /D "COIN_DLL" /I "lib" /I "..\..\lib" /I "." /I "$(COINDIR)\include" /I "$(COINDIR)\include\Inventor\annex" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "NUTSNBOLTS_DEBUG=0" /D "HAVE_CONFIG_H" /D "NB_MAKE_DLL" /D "NB_DEBUG=0" /D "NB_INTERNAL" /D "COIN_DLL" /I "lib" /I "..\..\lib" /I "." /I "$(COINDIR)\include" /I "$(COINDIR)\include\Inventor\annex" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 $(COINDIR)\lib\coin2.lib opengl32.lib gdi32.lib winmm.lib user32.lib /nologo /dll /machine:I386
# ADD LINK32 $(COINDIR)\lib\coin2.lib opengl32.lib gdi32.lib winmm.lib user32.lib /nologo /dll /machine:I386 /out:"nutsnbolts0.dll" /opt:nowin98
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=..\misc\install-sdk.bat dll release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "NUTSNBOLTS_DEBUG=1" /D "HAVE_CONFIG_H" /D "NB_MAKE_DLL" /D "NB_DEBUG=0" /D "NB_INTERNAL" /D "COIN_DLL" /I "lib" /I "..\..\lib" /I "." /I "$(COINDIR)\include" /I "$(COINDIR)\include\Inventor\annex" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "NUTSNBOLTS_DEBUG=1" /D "HAVE_CONFIG_H" /D "NB_MAKE_DLL" /D "NB_DEBUG=0" /D "NB_INTERNAL" /D "COIN_DLL" /I "lib" /I "..\..\lib" /I "." /I "$(COINDIR)\include" /I "$(COINDIR)\include\Inventor\annex" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 $(COINDIR)\lib\coin2.lib opengl32.lib gdi32.lib winmm.lib user32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 $(COINDIR)\lib\coin2.lib opengl32.lib gdi32.lib winmm.lib user32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /out:"nutsnbolts0d.dll"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=..\misc\install-sdk.bat dll debug
# End Special Build Tool

!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "StaticRelease"
# PROP BASE Intermediate_Dir "StaticRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "StaticRelease"
# PROP Intermediate_Dir "StaticRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_LIB" /D "NUTSNBOLTS_DEBUG=0"  /D "HAVE_CONFIG_H" /D "NB_DEBUG=0" /D "NB_INTERNAL" /D "COIN_DLL" /I "lib" /I "..\..\lib" /I "." /I "$(COINDIR)\include" /I "$(COINDIR)\include\Inventor\annex" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_LIB" /D "NUTSNBOLTS_DEBUG=0"  /D "HAVE_CONFIG_H" /D "NB_DEBUG=0" /D "NB_INTERNAL" /D "COIN_DLL" /I "lib" /I "..\..\lib" /I "." /I "$(COINDIR)\include" /I "$(COINDIR)\include\Inventor\annex" /YX /FD /c
# ADD BASE RSC /l 0x414 /d "NDEBUG"
# ADD RSC /l 0x414 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /machine:I386 /out:"nutsnbolts0s.lib"
# ADD LIB32 /nologo /machine:I386 /out:"nutsnbolts0s.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=..\misc\install-sdk.bat lib release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "StaticDebug"
# PROP BASE Intermediate_Dir "StaticDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "StaticDebug"
# PROP Intermediate_Dir "StaticDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_LIB" /D "NUTSNBOLTS_DEBUG=1"  /D "HAVE_CONFIG_H" /D "NB_DEBUG=0" /D "NB_INTERNAL" /D "COIN_DLL" /I "lib" /I "..\..\lib" /I "." /I "$(COINDIR)\include" /I "$(COINDIR)\include\Inventor\annex" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_LIB" /D "NUTSNBOLTS_DEBUG=1"  /D "HAVE_CONFIG_H" /D "NB_DEBUG=0" /D "NB_INTERNAL" /D "COIN_DLL" /I "lib" /I "..\..\lib" /I "." /I "$(COINDIR)\include" /I "$(COINDIR)\include\Inventor\annex" /YX /FD /c
# ADD BASE RSC /l 0x414 /d "_DEBUG"
# ADD RSC /l 0x414 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /machine:I386 /out:"nutsnbolts0sd.lib"
# ADD LIB32 /nologo /machine:I386 /out:"nutsnbolts0sd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=..\misc\install-sdk.bat lib debug
# End Special Build Tool

!ENDIF

# Begin Target

# Name "nutsnbolts0 - Win32 DLL (Release)"
# Name "nutsnbolts0 - Win32 DLL (Debug)"
# Name "nutsnbolts0 - Win32 LIB (Release)"
# Name "nutsnbolts0 - Win32 LIB (Debug)"
# Begin Group "Documents"
# PROP Default_Filter ";txt"
# End Group
# Begin Group "Source Files"
# PROP Default_Filter "c;cpp;ic;icc;h"

# Begin Group "NutsnBolts/misc sources"
# PROP Default_Filter "c;cpp;ic;icc;h"
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\misc\SoEvent.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\misc"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\misc"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\misc"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\misc"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\misc\NbNamedEvent.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\misc"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\misc"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\misc"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\misc"
!ENDIF
# End Source File
# End Group
# Begin Group "NutsnBolts/navigation sources"
# PROP Default_Filter "c;cpp;ic;icc;h"
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbNavigationControl.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbNavigationMode.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbNavigationState.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbNavigationSystem.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbNavigationVehicle.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbIdleMode.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbRotateMode.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbPanMode.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbPickMode.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbZoomMode.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbCenterMode.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbOrthoCenterMode.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbRollMode.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbPitchMode.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbYawMode.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\extra.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\navigation"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\navigation"
!ENDIF
# End Source File
# End Group
# Begin Group "NutsnBolts/nodes sources"
# PROP Default_Filter "c;cpp;ic;icc;h"
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\nodes\NbViewerNavigationMode.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\nodes"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\nodes"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\nodes"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\nodes"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\nodes\NbEventNode.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\nodes"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\nodes"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\nodes"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\nodes"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\nodes\NbScriptEvent.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts\nodes"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts\nodes"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts\nodes"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts\nodes"
!ENDIF
# End Source File
# End Group
# Begin Group "NutsnBolts sources"
# PROP Default_Filter "c;cpp;ic;icc;h"
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\NutsnBolts.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\NbSceneManager.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\NbSceneManagerP.cpp
!IF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\NutsnBolts"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\NutsnBolts"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\NutsnBolts"
!ELSEIF  "$(CFG)" == "nutsnbolts0 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\NutsnBolts"
!ENDIF
# End Source File
# End Group
# End Group
# Begin Group "Public Headers"

# PROP Default_Filter "h;ic;icc"
# Begin Group "NutsnBolts\misc headers"
# Set Default_Filter "h"
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\misc\NbNamedEvent.h
# End Source File
# End Group
# Begin Group "NutsnBolts\navigation headers"
# Set Default_Filter "h"
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbNavigationControl.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbNavigationMode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbNavigationState.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbNavigationSystem.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbNavigationVehicle.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbIdleMode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbRotateMode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbPanMode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbPickMode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbZoomMode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbCenterMode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbOrthoCenterMode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbRollMode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbPitchMode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbYawMode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\navigation\NbSubMode.h
# End Source File
# End Group
# Begin Group "NutsnBolts\nodes headers"
# Set Default_Filter "h"
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\nodes\NbViewerNavigationMode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\nodes\NbEventNode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\nodes\NbScriptEvent.h
# End Source File
# End Group
# Begin Group "NutsnBolts headers"
# Set Default_Filter "h"
# Begin Source File

SOURCE=lib\NutsnBolts\Basic.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\NutsnBolts.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\NbSceneManager.h
# End Source File
# End Group
# End Group
# Begin Group "Private Headers"

# PROP Default_Filter "h;ic;icc"
# Begin Group "misc local includes"

# PROP Default_Filter "h;ic;icc"
# Begin Source File

SOURCE=..\..\lib\NutsnBolts\misc\SoEvent.h

# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Group
# End Target
# End Project

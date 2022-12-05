# Microsoft Developer Studio Project File - Name="ScorchedEarth" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ScorchedEarth - Win32 MemLeakTest Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ScorchedEarth.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ScorchedEarth.mak" CFG="ScorchedEarth - Win32 MemLeakTest Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ScorchedEarth - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ScorchedEarth - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ScorchedEarth - Win32 MemLeakTest Debug" (based on "Win32 (x86) Application")
!MESSAGE "ScorchedEarth - Win32 Server" (based on "Win32 (x86) Application")
!MESSAGE "ScorchedEarth - Win32 ServerDebug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ScorchedEarth - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W2 /Gi /GX /Zi /O1 /I "alogg/include" /I "fmodapi370/api/inc" /I "agup" /I "netcommon/include" /I "../allegro/include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_MSVC" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 wsock32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /out:".\run\cdirtV6.exe"
# SUBTRACT LINK32 /verbose /incremental:yes /map /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W2 /Gm /GX /ZI /Od /I "./gui" /I "alogg/include" /I "fmodapi370/api/inc" /I "agup" /I "netcommon/include" /I "../allegro/include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_MSVC" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wsock32.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /out:".\run\zcdirt.exe" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 MemLeakTest Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ScorchedEarth___Win32_MemLeakTest_Debug"
# PROP BASE Intermediate_Dir "ScorchedEarth___Win32_MemLeakTest_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./gui" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_MSVC" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./gui" /I "alogg/include" /I "fmodapi370/api/inc" /I "agup" /I "netcommon/include" /I "../allegro/include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_MSVC" /D "MEMLEAKTEST" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib alld.lib fblend.lib memleakcheck/memleakcheckt.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /out:".\run\cdirt_mem.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 wsock32.lib memleakcheck/memleakcheckt.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /out:".\run\testcdirt.exe" /pdbtype:sept /libpath:"alogg\lib\msvc" /libpath:"fmodapi370/api/lib" /libpath:"agup" /libpath:"netcommon/lib" /libpath:"../allegro/lib/msvc"
# SUBTRACT LINK32 /incremental:no

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Server"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ScorchedEarth___Win32_Server"
# PROP BASE Intermediate_Dir "ScorchedEarth___Win32_Server"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Server"
# PROP Intermediate_Dir "Server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W2 /Gi /GX /Zi /O2 /I "./gui" /I "alogg/include" /I "fmodapi370/api/inc" /I "agup" /I "netcommon/include" /I "../allegro/include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_MSVC" /FR /YX /FD /c
# ADD CPP /nologo /MD /W2 /Gi /GX /Zi /O1 /I "./gui" /I "fmodapi370/api/inc" /I "agup" /I "netcommon/include" /I "../allegro/include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "USE_MSVC" /D "DEDICATED_SERVER" /D "ALLEGRO_USE_CONSOLE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 alleg.lib fmodvc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fblend.lib wsock32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /out:".\run\cdirt.exe" /libpath:"hawknl/lib" /libpath:"alogg\lib\msvc" /libpath:"fmodapi370/api/lib" /libpath:"agup" /libpath:"netcommon/lib" /libpath:"../allegro/lib/msvc"
# SUBTRACT BASE LINK32 /verbose /incremental:yes /map /debug /nodefaultlib
# ADD LINK32 wsock32.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /out:".\run\cdserv.exe" /pdbtype:sept /libpath:"fmodapi370/api/lib" /libpath:"agup" /libpath:"netcommon/lib" /libpath:"../allegro/lib/msvc"
# SUBTRACT LINK32 /pdb:none /incremental:yes /map /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 ServerDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ScorchedEarth___Win32_ServerDebug"
# PROP BASE Intermediate_Dir "ScorchedEarth___Win32_ServerDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ServerDebug"
# PROP Intermediate_Dir "ServerDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W2 /Gi /GX /Zi /Od /I "./gui" /I "fmodapi370/api/inc" /I "agup" /I "netcommon/include" /I "../allegro/include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "USE_MSVC" /D "DEDICATED_SERVER" /D "ALLEGRO_USE_CONSOLE" /FR /YX /FD /c
# ADD CPP /nologo /MD /W2 /Gi /GX /Zi /Od /I "./gui" /I "fmodapi370/api/inc" /I "agup" /I "netcommon/include" /I "../allegro/include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "USE_MSVC" /D "DEDICATED_SERVER" /D "ALLEGRO_USE_CONSOLE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib /nologo /subsystem:console /incremental:yes /map /debug /machine:I386 /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /out:".\run\cdserv.exe" /pdbtype:sept /libpath:"fmodapi370/api/lib" /libpath:"agup" /libpath:"netcommon/lib" /libpath:"../allegro/lib/msvc"
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 wsock32.lib /nologo /subsystem:console /incremental:yes /map /debug /machine:I386 /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /out:".\run\cdserv.exe" /pdbtype:sept /libpath:"fmodapi370/api/lib" /libpath:"agup" /libpath:"netcommon/lib" /libpath:"../allegro/lib/msvc"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "ScorchedEarth - Win32 Release"
# Name "ScorchedEarth - Win32 Debug"
# Name "ScorchedEarth - Win32 MemLeakTest Debug"
# Name "ScorchedEarth - Win32 Server"
# Name "ScorchedEarth - Win32 ServerDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "PPCol"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PPCol_Src\PPAMOUNT.C
# End Source File
# Begin Source File

SOURCE=.\PPCol_Src\PPCOL.C
# End Source File
# Begin Source File

SOURCE=.\PPCol_Src\PPCOL.H
# End Source File
# Begin Source File

SOURCE=.\PPCol_Src\PPMAMOUN.C
# End Source File
# Begin Source File

SOURCE=.\PPCol_Src\PPMASKC.C
# End Source File
# End Group
# Begin Group "Editor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Editor\Editor.cpp
# End Source File
# Begin Source File

SOURCE=.\Editor\Editor.h
# End Source File
# Begin Source File

SOURCE=.\Editor\FFormat.cpp
# End Source File
# End Group
# Begin Group "AGup"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\agup\aase.cpp
# End Source File
# Begin Source File

SOURCE=.\agup\aase.h
# End Source File
# Begin Source File

SOURCE=.\agup\abeos.cpp
# End Source File
# Begin Source File

SOURCE=.\agup\abeos.h
# End Source File
# Begin Source File

SOURCE=.\agup\agtk.cpp

!IF  "$(CFG)" == "ScorchedEarth - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 MemLeakTest Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Server"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 ServerDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\agup\agtk.h

!IF  "$(CFG)" == "ScorchedEarth - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 MemLeakTest Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Server"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 ServerDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\agup\agup.cpp
# End Source File
# Begin Source File

SOURCE=.\agup\agup.h
# End Source File
# Begin Source File

SOURCE=.\agup\agupitrn.h
# End Source File
# Begin Source File

SOURCE=.\agup\ans.cpp
# End Source File
# Begin Source File

SOURCE=.\agup\ans.h
# End Source File
# Begin Source File

SOURCE=.\agup\aphoton.cpp

!IF  "$(CFG)" == "ScorchedEarth - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 MemLeakTest Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Server"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 ServerDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\agup\aphoton.h

!IF  "$(CFG)" == "ScorchedEarth - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 MemLeakTest Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Server"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 ServerDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\agup\awin95.cpp
# End Source File
# Begin Source File

SOURCE=.\agup\awin95.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\buystuff.cpp
# End Source File
# Begin Source File

SOURCE=.\Cheats.cpp
# End Source File
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd_args.cpp
# End Source File
# Begin Source File

SOURCE=.\ComputerAI.cpp
# End Source File
# Begin Source File

SOURCE=.\conalleg.cpp
# End Source File
# Begin Source File

SOURCE=.\concmds.cpp
# End Source File
# Begin Source File

SOURCE=.\Config.cpp
# End Source File
# Begin Source File

SOURCE=.\console.cpp
# End Source File
# Begin Source File

SOURCE=.\ConsoleCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\Cvars.cpp
# End Source File
# Begin Source File

SOURCE=.\Defense.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\Drawing.cpp
# End Source File
# Begin Source File

SOURCE=.\drectangles.cpp
# End Source File
# Begin Source File

SOURCE=.\gametext.cpp
# End Source File
# Begin Source File

SOURCE=.\GROUND.cpp
# End Source File
# Begin Source File

SOURCE=.\GUI.Cpp
# End Source File
# Begin Source File

SOURCE=.\Intermission.cpp
# End Source File
# Begin Source File

SOURCE=.\KEYCFG.cpp
# End Source File
# Begin Source File

SOURCE=.\LEVELS.Cpp
# End Source File
# Begin Source File

SOURCE=.\multiplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Music.cpp
# End Source File
# Begin Source File

SOURCE=.\netcommon\src\netcommon.cpp
# End Source File
# Begin Source File

SOURCE=.\ngui.cpp
# End Source File
# Begin Source File

SOURCE=.\particles.cpp
# End Source File
# Begin Source File

SOURCE=.\peffect.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\save_jpeg.cpp

!IF  "$(CFG)" == "ScorchedEarth - Win32 Release"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 MemLeakTest Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Server"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 ServerDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SCORCH.cpp
# End Source File
# Begin Source File

SOURCE=.\script.cpp
# End Source File
# Begin Source File

SOURCE=.\secrets.cpp
# End Source File
# Begin Source File

SOURCE=.\server.cpp
# End Source File
# Begin Source File

SOURCE=.\serveradmin.cpp
# End Source File
# Begin Source File

SOURCE=.\SHOT.cpp
# End Source File
# Begin Source File

SOURCE=.\Specials.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprites.cpp
# End Source File
# Begin Source File

SOURCE=.\TANK.cpp
# End Source File
# Begin Source File

SOURCE=.\Tankdeaths.cpp
# End Source File
# Begin Source File

SOURCE=.\TonyJpegEncoder.cpp

!IF  "$(CFG)" == "ScorchedEarth - Win32 Release"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 MemLeakTest Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Server"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 ServerDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\alphadat.h
# End Source File
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\defense.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs.h
# End Source File
# Begin Source File

SOURCE=.\drectangles.h
# End Source File
# Begin Source File

SOURCE=.\Ground.h
# End Source File
# Begin Source File

SOURCE=.\Keycfg.h
# End Source File
# Begin Source File

SOURCE=.\levelScript.h
# End Source File
# Begin Source File

SOURCE=.\multiplayer.h
# End Source File
# Begin Source File

SOURCE=.\Music.h
# End Source File
# Begin Source File

SOURCE=.\netcommon\include\netcommon.h
# End Source File
# Begin Source File

SOURCE=.\Particles.h
# End Source File
# Begin Source File

SOURCE=.\peffect.h
# End Source File
# Begin Source File

SOURCE=.\scorch.h
# End Source File
# Begin Source File

SOURCE=.\scripts.h
# End Source File
# Begin Source File

SOURCE=.\sedit.h
# End Source File
# Begin Source File

SOURCE=.\Shot.h
# End Source File
# Begin Source File

SOURCE=.\snddat.h
# End Source File
# Begin Source File

SOURCE=.\Specials.h
# End Source File
# Begin Source File

SOURCE=.\sprdat.h
# End Source File
# Begin Source File

SOURCE=.\Sprites.h
# End Source File
# Begin Source File

SOURCE=.\Tank.h
# End Source File
# Begin Source File

SOURCE=.\tankdeaths.h
# End Source File
# Begin Source File

SOURCE=.\TonyJpegEncoder.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Resources\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\Resources\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\Resources\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\Resources\CDirt.rc

!IF  "$(CFG)" == "ScorchedEarth - Win32 Release"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 MemLeakTest Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Server"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 ServerDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Resources\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\Resources\icon1.ico
# End Source File
# Begin Source File

SOURCE="C:\Program Files\Microsoft Visual Studio\VC98\Lib\WINMM.LIB"
# End Source File
# Begin Source File

SOURCE=..\allegro\lib\msvc\alleg.lib

!IF  "$(CFG)" == "ScorchedEarth - Win32 Release"

# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 MemLeakTest Debug"

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 Server"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "ScorchedEarth - Win32 ServerDebug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fblend\lib\msvc\fblend.lib
# End Source File
# Begin Source File

SOURCE=.\fmodapi370\api\lib\fmodvc.lib
# End Source File
# End Group
# End Target
# End Project

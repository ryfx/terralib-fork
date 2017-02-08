# Microsoft Developer Studio Project File - Name="mif2spr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=mif2spr - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mif2spr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mif2spr.mak" CFG="mif2spr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mif2spr - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "mif2spr - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mif2spr - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /O2 /I "..\..\src\STLport" /I "..\..\src\terralib\kernel" /I "..\..\src\terralib\drivers\gisformats" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 terralib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\terralibw\terralib\Release"

!ELSEIF  "$(CFG)" == "mif2spr - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\src\STLport" /I "..\..\src\terralib\kernel" /I "..\..\src\terralib\drivers\gisformats" /D "_CONSOLE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x416 /d "_DEBUG"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 terralib.lib imm32.lib winmm.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\terralibw\terralib\Debug"

!ENDIF 

# Begin Target

# Name "mif2spr - Win32 Release"
# Name "mif2spr - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MIF2Spring.cpp

!IF  "$(CFG)" == "mif2spr - Win32 Release"

!ELSEIF  "$(CFG)" == "mif2spr - Win32 Debug"

# ADD CPP /I "..\..\..\src\STLport"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TeExportSPR.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\terralib\drivers\gisformats\TeImportMIF.cpp

!IF  "$(CFG)" == "mif2spr - Win32 Release"

!ELSEIF  "$(CFG)" == "mif2spr - Win32 Debug"

# ADD CPP /I "..\..\..\src\STLport"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\terralib\kernel\TeMemoryDB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\terralib\drivers\gisformats\TeMIFProjection.cpp

!IF  "$(CFG)" == "mif2spr - Win32 Release"

!ELSEIF  "$(CFG)" == "mif2spr - Win32 Debug"

# ADD CPP /I "..\..\..\src\STLport"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\terralib\drivers\gisformats\TeSPRFile.cpp

!IF  "$(CFG)" == "mif2spr - Win32 Release"

!ELSEIF  "$(CFG)" == "mif2spr - Win32 Debug"

# ADD CPP /I "..\..\..\src\STLport"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\terralib\drivers\gisformats\TeImportExport.h
# End Source File
# Begin Source File

SOURCE=..\..\src\terralib\kernel\TeMemoryDB.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\terralib\drivers\gisformats\TeMIFProjection.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\terralib\drivers\gisformats\TeSPRFile.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

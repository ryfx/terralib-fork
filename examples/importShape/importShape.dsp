# Microsoft Developer Studio Project File - Name="importShape" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=importShape - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "importShape.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "importShape.mak" CFG="importShape - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "importShape - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "importShape - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "importShape - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\src\STLport" /I "." /I "..\..\src\terralib\kernel" /I "..\..\src\terralib\application" /I "..\..\src\terralib\functions" /I "..\..\src\terralib\stat" /I "..\..\src\shapelib" /I "..\..\src\zlib" /I "..\..\src\jpeg\include"  /I "..\..\terralibw\ado" /I "..\..\src\terralib\drivers\MySQL" /I "..\..\src\terralib\drivers\MySQL\include" /I "..\..\src\terralib\drivers\OracleSpatial" /I "..\..\src\terralib\drivers\OracleSpatial\OCI\include" /I "..\..\src\terralib\drivers\PostgreSQL" /I "..\..\src\terralib\drivers\PostgreSQL\includepg" /I "..\..\src\terralib\drivers\PostgreSQL\interfacespg\libpq" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\..\terralibw\terralib\Release\terralib.lib ..\..\terralibw\shapelib\Release\shapelib.lib ..\..\terralibw\jpeg\ijl15.lib ..\..\terralibw\zlib\zlibdll.lib ..\..\terralibw\OracleSpatial\Oci\Oci.lib ..\..\terralibw\MySQL\libMySQL.lib ..\..\terralibw\PostgreSQL\libpq.lib /nologo /subsystem:console /machine:I386 /out:"../Release/importShape.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "importShape - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\src\STLport" /I "." /I "..\..\src\terralib\kernel" /I "..\..\src\terralib\application" /I "..\..\src\terralib\functions" /I "..\..\src\terralib\stat" /I "..\..\src\shapelib" /I "..\..\src\zlib" /I "..\..\src\jpeg\include" /I "..\..\terralibw\ado" /I "..\..\src\terralib\drivers\MySQL" /I "..\..\src\terralib\drivers\MySQL\include" /I "..\..\src\terralib\drivers\OracleSpatial" /I "..\..\src\terralib\drivers\OracleSpatial\OCI\include" /I "..\..\src\terralib\drivers\PostgreSQL" /I "..\..\src\terralib\drivers\PostgreSQL\includepg" /I "..\..\src\terralib\drivers\PostgreSQL\interfacespg\libpq" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x416 /d "_DEBUG"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\..\terralibw\terralib\Debug\terralib.lib ..\..\terralibw\shapelib\Debug\shapelib.lib ..\..\terralibw\jpeg\ijl15.lib ..\..\terralibw\zlib\zlibdll.lib ..\..\terralibw\OracleSpatial\Oci\Oci.lib ..\..\terralibw\MySQL\libMySQL.lib ..\..\terralibw\PostgreSQL\libpq.lib /nologo /subsystem:console /debug /machine:I386 /out:"../Debug/importShape.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "importShape - Win32 Release"
# Name "importShape - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\importShape.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\terralib\functions\TeImportDBF.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\terralib\functions\TeImportSHP.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\terralib\drivers\MySQL\TeMySQL.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\terralib\functions\TeImportExport.h
# End Source File
# Begin Source File

SOURCE=..\..\src\terralib\drivers\MySQL\TeMySQL.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

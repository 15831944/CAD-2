# Microsoft Developer Studio Project File - Name="CAD_PowerLand" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CAD_PowerLand - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CAD_PowerLand.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CAD_PowerLand.mak" CFG="CAD_PowerLand - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CAD_PowerLand - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CAD_PowerLand - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CAD_PowerLand - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386

!ELSEIF  "$(CFG)" == "CAD_PowerLand - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 GDIplus.lib /nologo /subsystem:windows /debug /machine:I386

!ENDIF 

# Begin Target

# Name "CAD_PowerLand - Win32 Release"
# Name "CAD_PowerLand - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CAD_PowerLand.cpp
# End Source File
# Begin Source File

SOURCE=.\CAD_PowerLandDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CAD_PowerLandView.cpp
# End Source File
# Begin Source File

SOURCE=..\Language_ch\ch.rc
# End Source File
# Begin Source File

SOURCE=.\CreateBase.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateEllipse.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateKong.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateLine.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateMCode.cpp
# End Source File
# Begin Source File

SOURCE=.\CreatePoint.cpp
# End Source File
# Begin Source File

SOURCE=.\CreatePolyline.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateRound.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateRoundArc.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateSpline.cpp
# End Source File
# Begin Source File

SOURCE=.\DeleteMoreLine.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAddPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAddRect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgColorToM.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCommandView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMillSide.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgParallel.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgParamClothCut.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgParamGlassGrind.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgParamInoutKnife.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPropertyDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSendFileName.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWorkFor.cpp
# End Source File
# Begin Source File

SOURCE=.\galil\Dmcwin.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawAddPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawArrange.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawBreakTwo.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawChamfer.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawEllipse.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawExtendLine.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawLine.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawMeasure.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawMovePoint.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawMoveView.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawParallel.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawPolyline.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawRound.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawRoundArc.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawSetBeginPos.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawSpline.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawTrim.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawVerticalLine.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawZoomScale.cpp
# End Source File
# Begin Source File

SOURCE=.\GlassGrind.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphIOKinfe.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphScale.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphTogether.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyCopy.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyDelete.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyMirror.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyMove.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyRotate.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenDXFCode.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenGCode.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenPLTCode.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputDXFCode.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputGCode.cpp
# End Source File
# Begin Source File

SOURCE=.\OverCut.cpp
# End Source File
# Begin Source File

SOURCE=.\Position.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableDlgBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToLessPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\TransToGraphList.cpp
# End Source File
# Begin Source File

SOURCE=.\TurnMill.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoRedo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CAD_PowerLand.h
# End Source File
# Begin Source File

SOURCE=.\CAD_PowerLandDoc.h
# End Source File
# Begin Source File

SOURCE=.\CAD_PowerLandView.h
# End Source File
# Begin Source File

SOURCE=.\CreateBase.h
# End Source File
# Begin Source File

SOURCE=.\DataStruct.h
# End Source File
# Begin Source File

SOURCE=.\DeleteMoreLine.h
# End Source File
# Begin Source File

SOURCE=.\DlgAddPoint.h
# End Source File
# Begin Source File

SOURCE=.\DlgAddRect.h
# End Source File
# Begin Source File

SOURCE=.\DlgColorToM.h
# End Source File
# Begin Source File

SOURCE=.\DlgCommandView.h
# End Source File
# Begin Source File

SOURCE=.\DlgMatrix.h
# End Source File
# Begin Source File

SOURCE=.\DlgMillSide.h
# End Source File
# Begin Source File

SOURCE=.\DlgParallel.h
# End Source File
# Begin Source File

SOURCE=.\DlgParamClothCut.h
# End Source File
# Begin Source File

SOURCE=.\DlgParamGlassGrind.h
# End Source File
# Begin Source File

SOURCE=.\DlgParamInoutKnife.h
# End Source File
# Begin Source File

SOURCE=.\DlgPropertyDraw.h
# End Source File
# Begin Source File

SOURCE=.\DlgSendFileName.h
# End Source File
# Begin Source File

SOURCE=.\DlgWorkFor.h
# End Source File
# Begin Source File

SOURCE=.\galil\Dmccom.h
# End Source File
# Begin Source File

SOURCE=.\galil\dmcdrc.h
# End Source File
# Begin Source File

SOURCE=.\galil\dmcdrco.h
# End Source File
# Begin Source File

SOURCE=.\galil\DMCMLIB.H
# End Source File
# Begin Source File

SOURCE=.\galil\Dmcwin.h
# End Source File
# Begin Source File

SOURCE=.\Draw.h
# End Source File
# Begin Source File

SOURCE=.\DrawBase.h
# End Source File
# Begin Source File

SOURCE=.\GlassGrind.h
# End Source File
# Begin Source File

SOURCE=.\GraphIOKinfe.h
# End Source File
# Begin Source File

SOURCE=.\GraphScale.h
# End Source File
# Begin Source File

SOURCE=.\GraphTogether.h
# End Source File
# Begin Source File

SOURCE=.\HarddogInfo.h
# End Source File
# Begin Source File

SOURCE=.\hasp_api.h
# End Source File
# Begin Source File

SOURCE=.\hasp_api_cpp.h
# End Source File
# Begin Source File

SOURCE=.\KnifeData.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Modify.h
# End Source File
# Begin Source File

SOURCE=.\OpenDXFCode.h
# End Source File
# Begin Source File

SOURCE=.\OpenGCode.h
# End Source File
# Begin Source File

SOURCE=.\OpenPLTCode.h
# End Source File
# Begin Source File

SOURCE=.\OutputDXFCode.h
# End Source File
# Begin Source File

SOURCE=.\OutputGCode.h
# End Source File
# Begin Source File

SOURCE=.\OverCut.h
# End Source File
# Begin Source File

SOURCE=.\Position.h
# End Source File
# Begin Source File

SOURCE=.\PropertyBar.h
# End Source File
# Begin Source File

SOURCE=.\ResizableDlgBar.h
# End Source File
# Begin Source File

SOURCE=..\Language_ch\resource.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\spline.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToLessPoint.h
# End Source File
# Begin Source File

SOURCE=.\TransToGraphList.h
# End Source File
# Begin Source File

SOURCE=.\TurnMill.h
# End Source File
# Begin Source File

SOURCE=.\Undo.h
# End Source File
# Begin Source File

SOURCE=.\UndoRedo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\Language_ch\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\CAD_PowerLand.ico
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\CAD_PowerLandDoc.ico
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\cursor3.cur
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\cursor4.cur
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\draw.bmp
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\draw.cur
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\resAddPoint\hori.bmp
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\param.bmp
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=..\Language_ch\res\resAddPoint\vert.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\Harddog.lib
# End Source File
# Begin Source File

SOURCE=.\hasp_windows_104780.lib
# End Source File
# Begin Source File

SOURCE=.\libhasp_cpp_windows_mtd_msc6.lib
# End Source File
# Begin Source File

SOURCE=.\libhasp_cpp_windows_mtd_msc6_d.lib
# End Source File
# Begin Source File

SOURCE=.\C_KnifeDll.lib
# End Source File
# Begin Source File

SOURCE=.\galil\DMCMLIB.lib
# End Source File
# Begin Source File

SOURCE=.\galil\DMC32.lib
# End Source File
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="WebMind" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WebMind - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WebMind.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WebMind.mak" CFG="WebMind - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WebMind - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WebMind - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WebMind - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Wsock32.lib pdh.lib Delayimp.lib winmm.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "WebMind - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WebMind___Win32_Debug"
# PROP BASE Intermediate_Dir "WebMind___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "_WINDOWS" /Fr /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "_WINDOWS" /D "DICT_EXPORTS" /Fr /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /win32
# SUBTRACT BASE MTL /mktyplib203
# ADD MTL /nologo /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Wsock32.lib pdh.lib Delayimp.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /DELAYLOAD:pdh.dll
# ADD LINK32 Wsock32.lib Delayimp.lib winmm.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"nafxcwd.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /libpath:"Debug"

!ENDIF 

# Begin Target

# Name "WebMind - Win32 Release"
# Name "WebMind - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AddPopupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\AddressBarEx.cpp
# End Source File
# Begin Source File

SOURCE=.\AddToThisFavDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AdvTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimateIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\Attribute.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BehaviorFeatureAnalysis.cpp
# End Source File
# Begin Source File

SOURCE=.\BehaviorFeatureHistoryAnalysis.cpp
# End Source File
# Begin Source File

SOURCE=.\C45Learner.cpp
# End Source File
# Begin Source File

SOURCE=.\ChevBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChineseCode.cpp
# End Source File
# Begin Source File

SOURCE=.\CleanAll.cpp
# End Source File
# Begin Source File

SOURCE=.\CollectorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Connection.cpp
# End Source File
# Begin Source File

SOURCE=.\ContentAnalysis.cpp
# End Source File
# Begin Source File

SOURCE=.\Corpus.cpp
# End Source File
# Begin Source File

SOURCE=.\CPARA.CPP
# End Source File
# Begin Source File

SOURCE=.\Crypt.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomizeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomizeToolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\Custsite.cpp
# End Source File
# Begin Source File

SOURCE=.\DataSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DayBaseAnalysis.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Dictionary.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgICPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPageProp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSuggest.cpp
# End Source File
# Begin Source File

SOURCE=.\editview.cpp
# End Source File
# Begin Source File

SOURCE=.\EnTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Evaluation.cpp
# End Source File
# Begin Source File

SOURCE=.\ExternalUtilItem.cpp
# End Source File
# Begin Source File

SOURCE=.\ExternalUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\Extractor.cpp
# End Source File
# Begin Source File

SOURCE=.\FeatureExtraction.cpp
# End Source File
# Begin Source File

SOURCE=.\FixedHtmlView.cpp
# End Source File
# Begin Source File

SOURCE=.\FontStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\Function.cpp
# End Source File
# Begin Source File

SOURCE=.\GenericHTTPClient.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\Google.cpp
# End Source File
# Begin Source File

SOURCE=.\Hash.cpp
# End Source File
# Begin Source File

SOURCE=.\hightime.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\ICSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Idispimp.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\InputAlias.cpp
# End Source File
# Begin Source File

SOURCE=.\InputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InputPassword.cpp
# End Source File
# Begin Source File

SOURCE=.\Instance.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemSet.cpp
# End Source File
# Begin Source File

SOURCE=.\LilacObject.cpp
# End Source File
# Begin Source File

SOURCE=.\LilacString.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkBar.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\List.cpp
# End Source File
# Begin Source File

SOURCE=.\Logging.cpp
# End Source File
# Begin Source File

SOURCE=.\LogTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Markup.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuData.cpp
# End Source File
# Begin Source File

SOURCE=.\Menuorder.cpp
# End Source File
# Begin Source File

SOURCE=.\NewDlgBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PageCache.cpp
# End Source File
# Begin Source File

SOURCE=.\PageDocument.cpp
# End Source File
# Begin Source File

SOURCE=.\PageDocument2.cpp
# End Source File
# Begin Source File

SOURCE=.\PageEval.cpp
# End Source File
# Begin Source File

SOURCE=.\PageRecomm.cpp
# End Source File
# Begin Source File

SOURCE=.\PageRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\PageView.cpp
# End Source File
# Begin Source File

SOURCE=.\Parseit.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupConfirm.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PorterStemming.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressSnakeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxyInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxyList.cpp
# End Source File
# Begin Source File

SOURCE=.\PsychoStateBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PuncMark.cpp
# End Source File
# Begin Source File

SOURCE=.\QueryLearner.cpp
# End Source File
# Begin Source File

SOURCE=.\QuickSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Recommendation.cpp
# End Source File
# Begin Source File

SOURCE=.\regexpr2.cpp
# End Source File
# Begin Source File

SOURCE=.\SAPrefsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SAPrefsSubDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchEngineHabits.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Segment.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectLang.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectURL.cpp
# End Source File
# Begin Source File

SOURCE=.\SelProxyCatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionIdentification.cpp
# End Source File
# Begin Source File

SOURCE=.\SetAliasDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetFormDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\SetSCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleSAH.cpp
# End Source File
# Begin Source File

SOURCE=.\SizableReBar.cpp
# End Source File
# Begin Source File

SOURCE=.\SortListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusBarWithProgress.Cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\stdafxw.cpp
# End Source File
# Begin Source File

SOURCE=.\StopWord.cpp
# End Source File
# Begin Source File

SOURCE=.\SurfInternetPeriod.cpp
# End Source File
# Begin Source File

SOURCE=.\syntax2.cpp
# End Source File
# Begin Source File

SOURCE=.\TabCtrlSSL.cpp
# End Source File
# Begin Source File

SOURCE=.\TabDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\TabPageSSL.cpp
# End Source File
# Begin Source File

SOURCE=.\TextProcessor.cpp
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\UploadAgreement.cpp
# End Source File
# Begin Source File

SOURCE=.\UploadPreview.cpp
# End Source File
# Begin Source File

SOURCE=.\Url.cpp
# End Source File
# Begin Source File

SOURCE=.\UrlAnalysis.cpp
# End Source File
# Begin Source File

SOURCE=.\UrlFilterProp.cpp
# End Source File
# Begin Source File

SOURCE=.\UrlRecordEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\Utility.cpp
# End Source File
# Begin Source File

SOURCE=.\utilityw.cpp
# End Source File
# Begin Source File

SOURCE=.\Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewOnLine.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\WebMind.cpp
# End Source File
# Begin Source File

SOURCE=.\WebMind.idl
# ADD MTL /h "WebMind_i.h" /iid "WebMind_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\WebMind.rc
# End Source File
# Begin Source File

SOURCE=.\WebMindDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\WebMindHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\WebMindNSHandle.cpp
# End Source File
# Begin Source File

SOURCE=.\WebMindOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\WebMindView.cpp
# End Source File
# Begin Source File

SOURCE=.\WinAppEx.cpp
# End Source File
# Begin Source File

SOURCE=.\WinAppSingle.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\wizsheet.cpp
# End Source File
# Begin Source File

SOURCE=.\WordMapping.cpp
# End Source File
# Begin Source File

SOURCE=.\WordSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\WordSeq.cpp
# End Source File
# Begin Source File

SOURCE=.\WordSession.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutCtrl.h
# End Source File
# Begin Source File

SOURCE=.\AddPopupDlg.h
# End Source File
# Begin Source File

SOURCE=.\AddressBar.h
# End Source File
# Begin Source File

SOURCE=.\AddressBarEx.h
# End Source File
# Begin Source File

SOURCE=.\AddToThisFavDlg.h
# End Source File
# Begin Source File

SOURCE=.\AdServer.h
# End Source File
# Begin Source File

SOURCE=.\AdvTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\AnimateIcon.h
# End Source File
# Begin Source File

SOURCE=.\Attribute.h
# End Source File
# Begin Source File

SOURCE=.\BaseTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BehaviorFeatureAnalysis.h
# End Source File
# Begin Source File

SOURCE=.\BehaviorFeatureHistoryAnalysis.h
# End Source File
# Begin Source File

SOURCE=.\C45Learner.h
# End Source File
# Begin Source File

SOURCE=.\ChevBar.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ChineseCode.h
# End Source File
# Begin Source File

SOURCE=.\CleanAll.h
# End Source File
# Begin Source File

SOURCE=.\CollectorDlg.h
# End Source File
# Begin Source File

SOURCE=.\ComboBar.h
# End Source File
# Begin Source File

SOURCE=.\CommonRes.h
# End Source File
# Begin Source File

SOURCE=.\Connection.h
# End Source File
# Begin Source File

SOURCE=.\ContentAnalysis.h
# End Source File
# Begin Source File

SOURCE=.\Corpus.h
# End Source File
# Begin Source File

SOURCE=.\CPARA.H
# End Source File
# Begin Source File

SOURCE=.\Crypt.h
# End Source File
# Begin Source File

SOURCE=.\CustomizeDialog.h
# End Source File
# Begin Source File

SOURCE=.\CustomizeToolbar.h
# End Source File
# Begin Source File

SOURCE=.\Custsite.h
# End Source File
# Begin Source File

SOURCE=.\DataSet.h
# End Source File
# Begin Source File

SOURCE=.\DayBaseAnalysis.h
# End Source File
# Begin Source File

SOURCE=.\DialogHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Dict.h
# End Source File
# Begin Source File

SOURCE=.\Dictionary.h
# End Source File
# Begin Source File

SOURCE=.\DlgBrowser.h
# End Source File
# Begin Source File

SOURCE=.\DlgICPage.h
# End Source File
# Begin Source File

SOURCE=.\DlgPageProp.h
# End Source File
# Begin Source File

SOURCE=.\DlgSuggest.h
# End Source File
# Begin Source File

SOURCE=.\editview.h
# End Source File
# Begin Source File

SOURCE=.\EnTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Evaluation.h
# End Source File
# Begin Source File

SOURCE=.\ExternalUtilItem.h
# End Source File
# Begin Source File

SOURCE=.\ExternalUtils.h
# End Source File
# Begin Source File

SOURCE=.\Extractor.h
# End Source File
# Begin Source File

SOURCE=.\FeatureExtraction.h
# End Source File
# Begin Source File

SOURCE=.\FileOperations.h
# End Source File
# Begin Source File

SOURCE=.\FixedHtmlView.h
# End Source File
# Begin Source File

SOURCE=.\FontStatic.h
# End Source File
# Begin Source File

SOURCE=.\Function.h
# End Source File
# Begin Source File

SOURCE=.\GenericHTTPClient.h
# End Source File
# Begin Source File

SOURCE=.\GlobalData.h
# End Source File
# Begin Source File

SOURCE=.\GlobalSetting.h
# End Source File
# Begin Source File

SOURCE=.\Google.h
# End Source File
# Begin Source File

SOURCE=.\Hash.h
# End Source File
# Begin Source File

SOURCE=.\hightime.h
# End Source File
# Begin Source File

SOURCE=.\hightime.inl
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\ICSession.h
# End Source File
# Begin Source File

SOURCE=.\ICTCLAS50.h
# End Source File
# Begin Source File

SOURCE=.\idispimp.h
# End Source File
# Begin Source File

SOURCE=.\IEHelper.h
# End Source File
# Begin Source File

SOURCE=.\IniFile.h
# End Source File
# Begin Source File

SOURCE=.\InputAlias.h
# End Source File
# Begin Source File

SOURCE=.\InputDlg.h
# End Source File
# Begin Source File

SOURCE=.\InputPassword.h
# End Source File
# Begin Source File

SOURCE=.\Instance.h
# End Source File
# Begin Source File

SOURCE=.\ioapi.h
# End Source File
# Begin Source File

SOURCE=.\iowin32.h
# End Source File
# Begin Source File

SOURCE=.\ItemSet.h
# End Source File
# Begin Source File

SOURCE=.\LilacObject.h
# End Source File
# Begin Source File

SOURCE=.\LilacString.h
# End Source File
# Begin Source File

SOURCE=.\LinkBar.h
# End Source File
# Begin Source File

SOURCE=.\LinkListDlg.h
# End Source File
# Begin Source File

SOURCE=.\List.h
# End Source File
# Begin Source File

SOURCE=.\Logging.h
# End Source File
# Begin Source File

SOURCE=.\LogTrace.h
# End Source File
# Begin Source File

SOURCE=.\macro.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MaleLogger.h
# End Source File
# Begin Source File

SOURCE=.\Markup.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MenuBar.h
# End Source File
# Begin Source File

SOURCE=.\MenuData.h
# End Source File
# Begin Source File

SOURCE=.\Menuorder.h
# End Source File
# Begin Source File

SOURCE=.\MISC.H
# End Source File
# Begin Source File

SOURCE=.\NewDlgBar.h
# End Source File
# Begin Source File

SOURCE=.\Output.h
# End Source File
# Begin Source File

SOURCE=.\PageCache.h
# End Source File
# Begin Source File

SOURCE=.\PageDocument.h
# End Source File
# Begin Source File

SOURCE=.\PageDocument2.h
# End Source File
# Begin Source File

SOURCE=.\PageEval.h
# End Source File
# Begin Source File

SOURCE=.\PageRecomm.h
# End Source File
# Begin Source File

SOURCE=.\PageRepository.h
# End Source File
# Begin Source File

SOURCE=.\PageView.h
# End Source File
# Begin Source File

SOURCE=.\Parseit.h
# End Source File
# Begin Source File

SOURCE=.\PopupConfirm.h
# End Source File
# Begin Source File

SOURCE=.\PopupDlg.h
# End Source File
# Begin Source File

SOURCE=.\PorterStemming.h
# End Source File
# Begin Source File

SOURCE=.\ProgressSnakeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ProgressWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProxyInfo.h
# End Source File
# Begin Source File

SOURCE=.\ProxyList.h
# End Source File
# Begin Source File

SOURCE=.\PSFTP.H
# End Source File
# Begin Source File

SOURCE=.\PsychoStateBar.h
# End Source File
# Begin Source File

SOURCE=.\PuncMark.h
# End Source File
# Begin Source File

SOURCE=.\PUTTY.H
# End Source File
# Begin Source File

SOURCE=.\PUTTYMEM.H
# End Source File
# Begin Source File

SOURCE=.\PUTTYPS.H
# End Source File
# Begin Source File

SOURCE=.\QueryLearner.h
# End Source File
# Begin Source File

SOURCE=.\QuickSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\Recommendation.h
# End Source File
# Begin Source File

SOURCE=.\regexpr2.h
# End Source File
# Begin Source File

SOURCE=.\reimpl2.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\restack.h
# End Source File
# Begin Source File

SOURCE=.\SAPrefsDialog.h
# End Source File
# Begin Source File

SOURCE=.\SAPrefsSubDlg.h
# End Source File
# Begin Source File

SOURCE=.\SearchEngine.h
# End Source File
# Begin Source File

SOURCE=.\SearchEngineHabits.h
# End Source File
# Begin Source File

SOURCE=.\SearchPage.h
# End Source File
# Begin Source File

SOURCE=.\Segment.h
# End Source File
# Begin Source File

SOURCE=.\SelectLang.h
# End Source File
# Begin Source File

SOURCE=.\SelectURL.h
# End Source File
# Begin Source File

SOURCE=.\SelProxyCatDlg.h
# End Source File
# Begin Source File

SOURCE=.\SessionIdentification.h
# End Source File
# Begin Source File

SOURCE=.\SetAliasDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetFormDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetProxy.h
# End Source File
# Begin Source File

SOURCE=.\SetSCDlg.h
# End Source File
# Begin Source File

SOURCE=.\SimpleSAH.h
# End Source File
# Begin Source File

SOURCE=.\SizableReBar.h
# End Source File
# Begin Source File

SOURCE=.\Sizecbar.h
# End Source File
# Begin Source File

SOURCE=.\SortListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StaticHelper.h
# End Source File
# Begin Source File

SOURCE=.\StatusBarWithProgress.H
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\stdafxw.h
# End Source File
# Begin Source File

SOURCE=.\StopWord.h
# End Source File
# Begin Source File

SOURCE=.\SurfInternetPeriod.h
# End Source File
# Begin Source File

SOURCE=.\syntax2.h
# End Source File
# Begin Source File

SOURCE=.\TabCtrlSSL.h
# End Source File
# Begin Source File

SOURCE=.\TabDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\TabPageSSL.h
# End Source File
# Begin Source File

SOURCE=.\TextProcessor.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarEx.h
# End Source File
# Begin Source File

SOURCE=.\ToolMenu.h
# End Source File
# Begin Source File

SOURCE=.\TREE234.H
# End Source File
# Begin Source File

SOURCE=.\TxtDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\unzip.h
# End Source File
# Begin Source File

SOURCE=.\UploadAgreement.h
# End Source File
# Begin Source File

SOURCE=.\UploadPreview.h
# End Source File
# Begin Source File

SOURCE=.\url.h
# End Source File
# Begin Source File

SOURCE=.\UrlAnalysis.h
# End Source File
# Begin Source File

SOURCE=.\UrlFilterProp.h
# End Source File
# Begin Source File

SOURCE=.\UrlRecordEntity.h
# End Source File
# Begin Source File

SOURCE=.\Utility.h
# End Source File
# Begin Source File

SOURCE=.\utilityw.h
# End Source File
# Begin Source File

SOURCE=.\Utils.h
# End Source File
# Begin Source File

SOURCE=.\ViewManager.h
# End Source File
# Begin Source File

SOURCE=.\ViewOnLine.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser.h
# End Source File
# Begin Source File

SOURCE=.\WebFileOpenDlg.h
# End Source File
# Begin Source File

SOURCE=.\WebMind.h
# End Source File
# Begin Source File

SOURCE=.\WebMindDoc.h
# End Source File
# Begin Source File

SOURCE=.\WebMindHelper.h
# End Source File
# Begin Source File

SOURCE=.\WebMindNSHandle.h
# End Source File
# Begin Source File

SOURCE=.\WebMindOptions.h
# End Source File
# Begin Source File

SOURCE=.\WebMindView.h
# End Source File
# Begin Source File

SOURCE=.\WinAppEx.h
# End Source File
# Begin Source File

SOURCE=.\WinAppSingle.h
# End Source File
# Begin Source File

SOURCE=.\WindowTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\WINHELP.H
# End Source File
# Begin Source File

SOURCE=.\WINSTUFF.H
# End Source File
# Begin Source File

SOURCE=.\wizsheet.h
# End Source File
# Begin Source File

SOURCE=.\WordMapping.h
# End Source File
# Begin Source File

SOURCE=.\WordSearch.h
# End Source File
# Begin Source File

SOURCE=.\WordSeq.h
# End Source File
# Begin Source File

SOURCE=.\WordSession.h
# End Source File
# Begin Source File

SOURCE=.\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zip.h
# End Source File
# Begin Source File

SOURCE=.\zlib.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\2008.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\address.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\bitmap16.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\bitmap_e.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\checkboxes.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CollectorTool.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00004.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00005.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00006.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00007.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00008.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00009.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00010.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00011.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00012.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00013.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00014.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00015.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00016.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00017.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00018.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00019.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00020.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00021.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00022.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00023.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00024.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00025.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cursor10.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cursor11.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cursor12.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cursor18.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cursor3.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cursor4.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cursor5.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cursor6.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cursor7.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cursor8.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cursor9.cur
# End Source File
# Begin Source File

SOURCE=.\RES\default.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\dest.ico
# End Source File
# Begin Source File

SOURCE=.\res\drag.cur
# End Source File
# Begin Source File

SOURCE=.\RES\filetype.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\RES\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\RES\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\RES\icon6.ico
# End Source File
# Begin Source File

SOURCE=.\RES\icon7.ico
# End Source File
# Begin Source File

SOURCE=".\RES\images (3).ico"
# End Source File
# Begin Source File

SOURCE=.\RES\LinkList.Bmp
# End Source File
# Begin Source File

SOURCE=.\RES\LinkList.ICO
# End Source File
# Begin Source File

SOURCE=.\RES\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\mainfrm.ico
# End Source File
# Begin Source File

SOURCE=.\RES\markic.ico
# End Source File
# Begin Source File

SOURCE=.\RES\normal.ico
# End Source File
# Begin Source File

SOURCE=.\res\page.ico
# End Source File
# Begin Source File

SOURCE=.\RES\prediction.ico
# End Source File
# Begin Source File

SOURCE=.\res\proxy.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\psycho.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\psycho.ico
# End Source File
# Begin Source File

SOURCE=.\RES\SAVE.ICO
# End Source File
# Begin Source File

SOURCE=.\RES\search.bmp
# End Source File
# Begin Source File

SOURCE=.\res\secure.ico
# End Source File
# Begin Source File

SOURCE=.\RES\smiley.ico
# End Source File
# Begin Source File

SOURCE=.\RES\soso.ico
# End Source File
# Begin Source File

SOURCE=.\RES\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\suggest.ico
# End Source File
# Begin Source File

SOURCE=.\RES\tabctrl.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\TaskBar.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\TBLargeCold.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\TBLargeHot.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\TBSmallCold.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\TBSmallHot.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\track.ico
# End Source File
# Begin Source File

SOURCE=.\RES\tray.ico
# End Source File
# Begin Source File

SOURCE=.\RES\traytest.ico
# End Source File
# Begin Source File

SOURCE=.\RES\WebICDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\WebMind.rc2
# End Source File
# Begin Source File

SOURCE=.\WebMind.rgs
# End Source File
# Begin Source File

SOURCE=.\res\WebMindNSHandle.rgs
# End Source File
# Begin Source File

SOURCE=.\RES\WebMindtype.ico
# End Source File
# End Group
# Begin Group "Analysis"

# PROP Default_Filter ""
# End Group
# Begin Group "ChartCtrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChartCtrl\ChartAxis.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartAxis.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartAxisLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartAxisLabel.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartBalloonLabel.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartBalloonLabel.inl
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartBarSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartBarSerie.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartCandlestickSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartCandlestickSerie.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartCrossHairCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartCrossHairCursor.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartCursor.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartDateTimeAxis.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartDateTimeAxis.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartDragLineCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartDragLineCursor.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartFont.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartFont.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartGanttSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartGanttSerie.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartGradient.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartGradient.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartGrid.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartLabel.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartLabel.inl
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartLegend.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartLegend.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartLineSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartLineSerie.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartLogarithmicAxis.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartLogarithmicAxis.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartMouseListener.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartPointsArray.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartPointsArray.inl
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartPointsSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartPointsSerie.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartSerie.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartSerieBase.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartSerieBase.inl
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartSeriesMouseListener.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartStandardAxis.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartStandardAxis.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartString.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartSurfaceSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartSurfaceSerie.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartTitle.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartXYSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\ChartXYSerie.h
# End Source File
# Begin Source File

SOURCE=.\ChartCtrl\PointsOrdering.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\regex
# End Source File
# Begin Source File

SOURCE=.\WebFileOpenDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WebMind.reg
# End Source File
# End Target
# End Project
# Section WebMind : {01710002-7E02-0173-02BA-580102187D01}
# 	1:8:IDR_WebMind:102
# End Section

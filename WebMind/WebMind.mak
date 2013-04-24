# Microsoft Developer Studio Generated NMAKE File, Based on WebMind.dsp
!IF "$(CFG)" == ""
CFG=WebMind - Win32 Release
!MESSAGE No configuration specified. Defaulting to WebMind - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "WebMind - Win32 Release" && "$(CFG)" != "WebMind - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "WebMind - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\WebMind.exe" "$(OUTDIR)\WebMind.tlb" ".\WebMind_i.h" ".\WebMind_i.c" "$(OUTDIR)\WebMind.bsc"


CLEAN :
	-@erase "$(INTDIR)\AboutCtrl.obj"
	-@erase "$(INTDIR)\AboutCtrl.sbr"
	-@erase "$(INTDIR)\AddPopupDlg.obj"
	-@erase "$(INTDIR)\AddPopupDlg.sbr"
	-@erase "$(INTDIR)\AddressBar.obj"
	-@erase "$(INTDIR)\AddressBar.sbr"
	-@erase "$(INTDIR)\AddressBarEx.obj"
	-@erase "$(INTDIR)\AddressBarEx.sbr"
	-@erase "$(INTDIR)\AddToThisFavDlg.obj"
	-@erase "$(INTDIR)\AddToThisFavDlg.sbr"
	-@erase "$(INTDIR)\AdvTabCtrl.obj"
	-@erase "$(INTDIR)\AdvTabCtrl.sbr"
	-@erase "$(INTDIR)\AnimateIcon.obj"
	-@erase "$(INTDIR)\AnimateIcon.sbr"
	-@erase "$(INTDIR)\Attribute.obj"
	-@erase "$(INTDIR)\Attribute.sbr"
	-@erase "$(INTDIR)\BaseTabCtrl.obj"
	-@erase "$(INTDIR)\BaseTabCtrl.sbr"
	-@erase "$(INTDIR)\BehaviorFeatureAnalysis.obj"
	-@erase "$(INTDIR)\BehaviorFeatureAnalysis.sbr"
	-@erase "$(INTDIR)\BehaviorFeatureHistoryAnalysis.obj"
	-@erase "$(INTDIR)\BehaviorFeatureHistoryAnalysis.sbr"
	-@erase "$(INTDIR)\C45Learner.obj"
	-@erase "$(INTDIR)\C45Learner.sbr"
	-@erase "$(INTDIR)\ChartAxis.obj"
	-@erase "$(INTDIR)\ChartAxis.sbr"
	-@erase "$(INTDIR)\ChartAxisLabel.obj"
	-@erase "$(INTDIR)\ChartAxisLabel.sbr"
	-@erase "$(INTDIR)\ChartBarSerie.obj"
	-@erase "$(INTDIR)\ChartBarSerie.sbr"
	-@erase "$(INTDIR)\ChartCandlestickSerie.obj"
	-@erase "$(INTDIR)\ChartCandlestickSerie.sbr"
	-@erase "$(INTDIR)\ChartCrossHairCursor.obj"
	-@erase "$(INTDIR)\ChartCrossHairCursor.sbr"
	-@erase "$(INTDIR)\ChartCtrl.obj"
	-@erase "$(INTDIR)\ChartCtrl.sbr"
	-@erase "$(INTDIR)\ChartCursor.obj"
	-@erase "$(INTDIR)\ChartCursor.sbr"
	-@erase "$(INTDIR)\ChartDateTimeAxis.obj"
	-@erase "$(INTDIR)\ChartDateTimeAxis.sbr"
	-@erase "$(INTDIR)\ChartDragLineCursor.obj"
	-@erase "$(INTDIR)\ChartDragLineCursor.sbr"
	-@erase "$(INTDIR)\ChartFont.obj"
	-@erase "$(INTDIR)\ChartFont.sbr"
	-@erase "$(INTDIR)\ChartGanttSerie.obj"
	-@erase "$(INTDIR)\ChartGanttSerie.sbr"
	-@erase "$(INTDIR)\ChartGradient.obj"
	-@erase "$(INTDIR)\ChartGradient.sbr"
	-@erase "$(INTDIR)\ChartGrid.obj"
	-@erase "$(INTDIR)\ChartGrid.sbr"
	-@erase "$(INTDIR)\ChartLegend.obj"
	-@erase "$(INTDIR)\ChartLegend.sbr"
	-@erase "$(INTDIR)\ChartLineSerie.obj"
	-@erase "$(INTDIR)\ChartLineSerie.sbr"
	-@erase "$(INTDIR)\ChartLogarithmicAxis.obj"
	-@erase "$(INTDIR)\ChartLogarithmicAxis.sbr"
	-@erase "$(INTDIR)\ChartPointsSerie.obj"
	-@erase "$(INTDIR)\ChartPointsSerie.sbr"
	-@erase "$(INTDIR)\ChartScrollBar.obj"
	-@erase "$(INTDIR)\ChartScrollBar.sbr"
	-@erase "$(INTDIR)\ChartSerie.obj"
	-@erase "$(INTDIR)\ChartSerie.sbr"
	-@erase "$(INTDIR)\ChartStandardAxis.obj"
	-@erase "$(INTDIR)\ChartStandardAxis.sbr"
	-@erase "$(INTDIR)\ChartSurfaceSerie.obj"
	-@erase "$(INTDIR)\ChartSurfaceSerie.sbr"
	-@erase "$(INTDIR)\ChartTitle.obj"
	-@erase "$(INTDIR)\ChartTitle.sbr"
	-@erase "$(INTDIR)\ChartXYSerie.obj"
	-@erase "$(INTDIR)\ChartXYSerie.sbr"
	-@erase "$(INTDIR)\ChevBar.obj"
	-@erase "$(INTDIR)\ChevBar.sbr"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\ChildFrm.sbr"
	-@erase "$(INTDIR)\CleanAll.obj"
	-@erase "$(INTDIR)\CleanAll.sbr"
	-@erase "$(INTDIR)\CollectorDlg.obj"
	-@erase "$(INTDIR)\CollectorDlg.sbr"
	-@erase "$(INTDIR)\ComboBar.obj"
	-@erase "$(INTDIR)\ComboBar.sbr"
	-@erase "$(INTDIR)\Connection.obj"
	-@erase "$(INTDIR)\Connection.sbr"
	-@erase "$(INTDIR)\Corpus.obj"
	-@erase "$(INTDIR)\Corpus.sbr"
	-@erase "$(INTDIR)\CPARA.OBJ"
	-@erase "$(INTDIR)\CPARA.SBR"
	-@erase "$(INTDIR)\Crypt.obj"
	-@erase "$(INTDIR)\Crypt.sbr"
	-@erase "$(INTDIR)\CustomizeDialog.obj"
	-@erase "$(INTDIR)\CustomizeDialog.sbr"
	-@erase "$(INTDIR)\CustomizeToolbar.obj"
	-@erase "$(INTDIR)\CustomizeToolbar.sbr"
	-@erase "$(INTDIR)\Custsite.obj"
	-@erase "$(INTDIR)\Custsite.sbr"
	-@erase "$(INTDIR)\DataSet.obj"
	-@erase "$(INTDIR)\DataSet.sbr"
	-@erase "$(INTDIR)\DialogHeaderCtrl.obj"
	-@erase "$(INTDIR)\DialogHeaderCtrl.sbr"
	-@erase "$(INTDIR)\DlgBrowser.obj"
	-@erase "$(INTDIR)\DlgBrowser.sbr"
	-@erase "$(INTDIR)\DlgICPage.obj"
	-@erase "$(INTDIR)\DlgICPage.sbr"
	-@erase "$(INTDIR)\DlgPageProp.obj"
	-@erase "$(INTDIR)\DlgPageProp.sbr"
	-@erase "$(INTDIR)\DlgSuggest.obj"
	-@erase "$(INTDIR)\DlgSuggest.sbr"
	-@erase "$(INTDIR)\editview.obj"
	-@erase "$(INTDIR)\editview.sbr"
	-@erase "$(INTDIR)\EnTabCtrl.obj"
	-@erase "$(INTDIR)\EnTabCtrl.sbr"
	-@erase "$(INTDIR)\Evaluation.obj"
	-@erase "$(INTDIR)\Evaluation.sbr"
	-@erase "$(INTDIR)\ExternalUtilItem.obj"
	-@erase "$(INTDIR)\ExternalUtilItem.sbr"
	-@erase "$(INTDIR)\ExternalUtils.obj"
	-@erase "$(INTDIR)\ExternalUtils.sbr"
	-@erase "$(INTDIR)\Extractor.obj"
	-@erase "$(INTDIR)\Extractor.sbr"
	-@erase "$(INTDIR)\FeatureExtraction.obj"
	-@erase "$(INTDIR)\FeatureExtraction.sbr"
	-@erase "$(INTDIR)\FixedHtmlView.obj"
	-@erase "$(INTDIR)\FixedHtmlView.sbr"
	-@erase "$(INTDIR)\FontStatic.obj"
	-@erase "$(INTDIR)\FontStatic.sbr"
	-@erase "$(INTDIR)\Function.obj"
	-@erase "$(INTDIR)\Function.sbr"
	-@erase "$(INTDIR)\GenericHTTPClient.obj"
	-@erase "$(INTDIR)\GenericHTTPClient.sbr"
	-@erase "$(INTDIR)\GlobalSetting.obj"
	-@erase "$(INTDIR)\GlobalSetting.sbr"
	-@erase "$(INTDIR)\Google.obj"
	-@erase "$(INTDIR)\Google.sbr"
	-@erase "$(INTDIR)\Hash.obj"
	-@erase "$(INTDIR)\Hash.sbr"
	-@erase "$(INTDIR)\hightime.obj"
	-@erase "$(INTDIR)\hightime.sbr"
	-@erase "$(INTDIR)\HyperLink.obj"
	-@erase "$(INTDIR)\HyperLink.sbr"
	-@erase "$(INTDIR)\ICSession.obj"
	-@erase "$(INTDIR)\ICSession.sbr"
	-@erase "$(INTDIR)\Idispimp.obj"
	-@erase "$(INTDIR)\Idispimp.sbr"
	-@erase "$(INTDIR)\IniFile.obj"
	-@erase "$(INTDIR)\IniFile.sbr"
	-@erase "$(INTDIR)\InputAlias.obj"
	-@erase "$(INTDIR)\InputAlias.sbr"
	-@erase "$(INTDIR)\InputDlg.obj"
	-@erase "$(INTDIR)\InputDlg.sbr"
	-@erase "$(INTDIR)\InputPassword.obj"
	-@erase "$(INTDIR)\InputPassword.sbr"
	-@erase "$(INTDIR)\Instance.obj"
	-@erase "$(INTDIR)\Instance.sbr"
	-@erase "$(INTDIR)\ItemSet.obj"
	-@erase "$(INTDIR)\ItemSet.sbr"
	-@erase "$(INTDIR)\LilacObject.obj"
	-@erase "$(INTDIR)\LilacObject.sbr"
	-@erase "$(INTDIR)\LilacString.obj"
	-@erase "$(INTDIR)\LilacString.sbr"
	-@erase "$(INTDIR)\LinkBar.obj"
	-@erase "$(INTDIR)\LinkBar.sbr"
	-@erase "$(INTDIR)\LinkListDlg.obj"
	-@erase "$(INTDIR)\LinkListDlg.sbr"
	-@erase "$(INTDIR)\List.obj"
	-@erase "$(INTDIR)\List.sbr"
	-@erase "$(INTDIR)\Logging.obj"
	-@erase "$(INTDIR)\Logging.sbr"
	-@erase "$(INTDIR)\LogTrace.obj"
	-@erase "$(INTDIR)\LogTrace.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\Markup.obj"
	-@erase "$(INTDIR)\Markup.sbr"
	-@erase "$(INTDIR)\MenuBar.obj"
	-@erase "$(INTDIR)\MenuBar.sbr"
	-@erase "$(INTDIR)\MenuData.obj"
	-@erase "$(INTDIR)\MenuData.sbr"
	-@erase "$(INTDIR)\Menuorder.obj"
	-@erase "$(INTDIR)\Menuorder.sbr"
	-@erase "$(INTDIR)\WebMind.obj"
	-@erase "$(INTDIR)\WebMind.pch"
	-@erase "$(INTDIR)\WebMind.res"
	-@erase "$(INTDIR)\WebMind.sbr"
	-@erase "$(INTDIR)\WebMind.tlb"
	-@erase "$(INTDIR)\WebMindHelper.obj"
	-@erase "$(INTDIR)\WebMindHelper.sbr"
	-@erase "$(INTDIR)\WebMindNSHandle.obj"
	-@erase "$(INTDIR)\WebMindNSHandle.sbr"
	-@erase "$(INTDIR)\WebMindOptions.obj"
	-@erase "$(INTDIR)\WebMindOptions.sbr"
	-@erase "$(INTDIR)\WebMindView.obj"
	-@erase "$(INTDIR)\WebMindView.sbr"
	-@erase "$(INTDIR)\NewDlgBar.obj"
	-@erase "$(INTDIR)\NewDlgBar.sbr"
	-@erase "$(INTDIR)\PageCache.obj"
	-@erase "$(INTDIR)\PageCache.sbr"
	-@erase "$(INTDIR)\PageDocument.obj"
	-@erase "$(INTDIR)\PageDocument.sbr"
	-@erase "$(INTDIR)\PageDocument2.obj"
	-@erase "$(INTDIR)\PageDocument2.sbr"
	-@erase "$(INTDIR)\PageEval.obj"
	-@erase "$(INTDIR)\PageEval.sbr"
	-@erase "$(INTDIR)\PageRecomm.obj"
	-@erase "$(INTDIR)\PageRecomm.sbr"
	-@erase "$(INTDIR)\PageRepository.obj"
	-@erase "$(INTDIR)\PageRepository.sbr"
	-@erase "$(INTDIR)\PageView.obj"
	-@erase "$(INTDIR)\PageView.sbr"
	-@erase "$(INTDIR)\Parseit.obj"
	-@erase "$(INTDIR)\Parseit.sbr"
	-@erase "$(INTDIR)\PopupConfirm.obj"
	-@erase "$(INTDIR)\PopupConfirm.sbr"
	-@erase "$(INTDIR)\PopupDlg.obj"
	-@erase "$(INTDIR)\PopupDlg.sbr"
	-@erase "$(INTDIR)\PorterStemming.obj"
	-@erase "$(INTDIR)\PorterStemming.sbr"
	-@erase "$(INTDIR)\ProgressSnakeCtrl.obj"
	-@erase "$(INTDIR)\ProgressSnakeCtrl.sbr"
	-@erase "$(INTDIR)\ProgressWnd.obj"
	-@erase "$(INTDIR)\ProgressWnd.sbr"
	-@erase "$(INTDIR)\ProxyInfo.obj"
	-@erase "$(INTDIR)\ProxyInfo.sbr"
	-@erase "$(INTDIR)\ProxyList.obj"
	-@erase "$(INTDIR)\ProxyList.sbr"
	-@erase "$(INTDIR)\PuncMark.obj"
	-@erase "$(INTDIR)\PuncMark.sbr"
	-@erase "$(INTDIR)\QueryLearner.obj"
	-@erase "$(INTDIR)\QueryLearner.sbr"
	-@erase "$(INTDIR)\QuickSearchDlg.obj"
	-@erase "$(INTDIR)\QuickSearchDlg.sbr"
	-@erase "$(INTDIR)\Recommendation.obj"
	-@erase "$(INTDIR)\Recommendation.sbr"
	-@erase "$(INTDIR)\SAPrefsDialog.obj"
	-@erase "$(INTDIR)\SAPrefsDialog.sbr"
	-@erase "$(INTDIR)\SAPrefsSubDlg.obj"
	-@erase "$(INTDIR)\SAPrefsSubDlg.sbr"
	-@erase "$(INTDIR)\SearchEngine.obj"
	-@erase "$(INTDIR)\SearchEngine.sbr"
	-@erase "$(INTDIR)\SearchPage.obj"
	-@erase "$(INTDIR)\SearchPage.sbr"
	-@erase "$(INTDIR)\SelectLang.obj"
	-@erase "$(INTDIR)\SelectLang.sbr"
	-@erase "$(INTDIR)\SelectURL.obj"
	-@erase "$(INTDIR)\SelectURL.sbr"
	-@erase "$(INTDIR)\SelProxyCatDlg.obj"
	-@erase "$(INTDIR)\SelProxyCatDlg.sbr"
	-@erase "$(INTDIR)\SessionIdentification.obj"
	-@erase "$(INTDIR)\SessionIdentification.sbr"
	-@erase "$(INTDIR)\SetAliasDlg.obj"
	-@erase "$(INTDIR)\SetAliasDlg.sbr"
	-@erase "$(INTDIR)\SetFormDataDlg.obj"
	-@erase "$(INTDIR)\SetFormDataDlg.sbr"
	-@erase "$(INTDIR)\SetProxy.obj"
	-@erase "$(INTDIR)\SetProxy.sbr"
	-@erase "$(INTDIR)\SetSCDlg.obj"
	-@erase "$(INTDIR)\SetSCDlg.sbr"
	-@erase "$(INTDIR)\SimpleSAH.obj"
	-@erase "$(INTDIR)\SimpleSAH.sbr"
	-@erase "$(INTDIR)\SizableReBar.obj"
	-@erase "$(INTDIR)\SizableReBar.sbr"
	-@erase "$(INTDIR)\SortListCtrl.obj"
	-@erase "$(INTDIR)\SortListCtrl.sbr"
	-@erase "$(INTDIR)\StaticHelper.obj"
	-@erase "$(INTDIR)\StaticHelper.sbr"
	-@erase "$(INTDIR)\StatusBarWithProgress.obj"
	-@erase "$(INTDIR)\StatusBarWithProgress.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\StopWord.obj"
	-@erase "$(INTDIR)\StopWord.sbr"
	-@erase "$(INTDIR)\TabCtrlSSL.obj"
	-@erase "$(INTDIR)\TabCtrlSSL.sbr"
	-@erase "$(INTDIR)\TabDropTarget.obj"
	-@erase "$(INTDIR)\TabDropTarget.sbr"
	-@erase "$(INTDIR)\TabPageSSL.obj"
	-@erase "$(INTDIR)\TabPageSSL.sbr"
	-@erase "$(INTDIR)\TextProcessor.obj"
	-@erase "$(INTDIR)\TextProcessor.sbr"
	-@erase "$(INTDIR)\TextProgressCtrl.obj"
	-@erase "$(INTDIR)\TextProgressCtrl.sbr"
	-@erase "$(INTDIR)\ToolBarEx.obj"
	-@erase "$(INTDIR)\ToolBarEx.sbr"
	-@erase "$(INTDIR)\ToolMenu.obj"
	-@erase "$(INTDIR)\ToolMenu.sbr"
	-@erase "$(INTDIR)\Url.obj"
	-@erase "$(INTDIR)\Url.sbr"
	-@erase "$(INTDIR)\UrlFilterProp.obj"
	-@erase "$(INTDIR)\UrlFilterProp.sbr"
	-@erase "$(INTDIR)\UrlRecordEntity.obj"
	-@erase "$(INTDIR)\UrlRecordEntity.sbr"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\Utility.sbr"
	-@erase "$(INTDIR)\Utils.obj"
	-@erase "$(INTDIR)\Utils.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\ViewOnLine.obj"
	-@erase "$(INTDIR)\ViewOnLine.sbr"
	-@erase "$(INTDIR)\webbrowser.obj"
	-@erase "$(INTDIR)\webbrowser.sbr"
	-@erase "$(INTDIR)\WebFileOpenDlg.obj"
	-@erase "$(INTDIR)\WebFileOpenDlg.sbr"
	-@erase "$(INTDIR)\WebICDoc.obj"
	-@erase "$(INTDIR)\WebICDoc.sbr"
	-@erase "$(INTDIR)\WinAppEx.obj"
	-@erase "$(INTDIR)\WinAppEx.sbr"
	-@erase "$(INTDIR)\WinAppSingle.obj"
	-@erase "$(INTDIR)\WinAppSingle.sbr"
	-@erase "$(INTDIR)\wizsheet.obj"
	-@erase "$(INTDIR)\wizsheet.sbr"
	-@erase "$(INTDIR)\WordMapping.obj"
	-@erase "$(INTDIR)\WordMapping.sbr"
	-@erase "$(INTDIR)\WordSearch.obj"
	-@erase "$(INTDIR)\WordSearch.sbr"
	-@erase "$(INTDIR)\WordSeq.obj"
	-@erase "$(INTDIR)\WordSeq.sbr"
	-@erase "$(INTDIR)\WordSession.obj"
	-@erase "$(INTDIR)\WordSession.sbr"
	-@erase "$(OUTDIR)\WebMind.bsc"
	-@erase "$(OUTDIR)\WebMind.exe"
	-@erase ".\WebMind_i.c"
	-@erase ".\WebMind_i.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\WebMind.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\WebMind.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WebMind.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AboutCtrl.sbr" \
	"$(INTDIR)\AddPopupDlg.sbr" \
	"$(INTDIR)\AddressBar.sbr" \
	"$(INTDIR)\AddressBarEx.sbr" \
	"$(INTDIR)\AddToThisFavDlg.sbr" \
	"$(INTDIR)\AdvTabCtrl.sbr" \
	"$(INTDIR)\AnimateIcon.sbr" \
	"$(INTDIR)\Attribute.sbr" \
	"$(INTDIR)\BaseTabCtrl.sbr" \
	"$(INTDIR)\BehaviorFeatureAnalysis.sbr" \
	"$(INTDIR)\BehaviorFeatureHistoryAnalysis.sbr" \
	"$(INTDIR)\C45Learner.sbr" \
	"$(INTDIR)\ChevBar.sbr" \
	"$(INTDIR)\ChildFrm.sbr" \
	"$(INTDIR)\CleanAll.sbr" \
	"$(INTDIR)\CollectorDlg.sbr" \
	"$(INTDIR)\ComboBar.sbr" \
	"$(INTDIR)\Connection.sbr" \
	"$(INTDIR)\Corpus.sbr" \
	"$(INTDIR)\CPARA.SBR" \
	"$(INTDIR)\Crypt.sbr" \
	"$(INTDIR)\CustomizeDialog.sbr" \
	"$(INTDIR)\CustomizeToolbar.sbr" \
	"$(INTDIR)\Custsite.sbr" \
	"$(INTDIR)\DataSet.sbr" \
	"$(INTDIR)\DialogHeaderCtrl.sbr" \
	"$(INTDIR)\DlgBrowser.sbr" \
	"$(INTDIR)\DlgICPage.sbr" \
	"$(INTDIR)\DlgPageProp.sbr" \
	"$(INTDIR)\DlgSuggest.sbr" \
	"$(INTDIR)\editview.sbr" \
	"$(INTDIR)\EnTabCtrl.sbr" \
	"$(INTDIR)\Evaluation.sbr" \
	"$(INTDIR)\ExternalUtilItem.sbr" \
	"$(INTDIR)\ExternalUtils.sbr" \
	"$(INTDIR)\Extractor.sbr" \
	"$(INTDIR)\FeatureExtraction.sbr" \
	"$(INTDIR)\FixedHtmlView.sbr" \
	"$(INTDIR)\FontStatic.sbr" \
	"$(INTDIR)\Function.sbr" \
	"$(INTDIR)\GenericHTTPClient.sbr" \
	"$(INTDIR)\GlobalSetting.sbr" \
	"$(INTDIR)\Google.sbr" \
	"$(INTDIR)\Hash.sbr" \
	"$(INTDIR)\hightime.sbr" \
	"$(INTDIR)\HyperLink.sbr" \
	"$(INTDIR)\ICSession.sbr" \
	"$(INTDIR)\Idispimp.sbr" \
	"$(INTDIR)\IniFile.sbr" \
	"$(INTDIR)\InputAlias.sbr" \
	"$(INTDIR)\InputDlg.sbr" \
	"$(INTDIR)\InputPassword.sbr" \
	"$(INTDIR)\Instance.sbr" \
	"$(INTDIR)\ItemSet.sbr" \
	"$(INTDIR)\LilacObject.sbr" \
	"$(INTDIR)\LilacString.sbr" \
	"$(INTDIR)\LinkBar.sbr" \
	"$(INTDIR)\LinkListDlg.sbr" \
	"$(INTDIR)\List.sbr" \
	"$(INTDIR)\Logging.sbr" \
	"$(INTDIR)\LogTrace.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\Markup.sbr" \
	"$(INTDIR)\MenuBar.sbr" \
	"$(INTDIR)\MenuData.sbr" \
	"$(INTDIR)\Menuorder.sbr" \
	"$(INTDIR)\WebMind.sbr" \
	"$(INTDIR)\WebMindHelper.sbr" \
	"$(INTDIR)\WebMindNSHandle.sbr" \
	"$(INTDIR)\WebMindOptions.sbr" \
	"$(INTDIR)\WebMindView.sbr" \
	"$(INTDIR)\NewDlgBar.sbr" \
	"$(INTDIR)\PageCache.sbr" \
	"$(INTDIR)\PageDocument.sbr" \
	"$(INTDIR)\PageDocument2.sbr" \
	"$(INTDIR)\PageEval.sbr" \
	"$(INTDIR)\PageRecomm.sbr" \
	"$(INTDIR)\PageRepository.sbr" \
	"$(INTDIR)\PageView.sbr" \
	"$(INTDIR)\Parseit.sbr" \
	"$(INTDIR)\PopupConfirm.sbr" \
	"$(INTDIR)\PopupDlg.sbr" \
	"$(INTDIR)\PorterStemming.sbr" \
	"$(INTDIR)\ProgressSnakeCtrl.sbr" \
	"$(INTDIR)\ProgressWnd.sbr" \
	"$(INTDIR)\ProxyInfo.sbr" \
	"$(INTDIR)\ProxyList.sbr" \
	"$(INTDIR)\PuncMark.sbr" \
	"$(INTDIR)\QueryLearner.sbr" \
	"$(INTDIR)\QuickSearchDlg.sbr" \
	"$(INTDIR)\Recommendation.sbr" \
	"$(INTDIR)\SAPrefsDialog.sbr" \
	"$(INTDIR)\SAPrefsSubDlg.sbr" \
	"$(INTDIR)\SearchEngine.sbr" \
	"$(INTDIR)\SearchPage.sbr" \
	"$(INTDIR)\SelectLang.sbr" \
	"$(INTDIR)\SelectURL.sbr" \
	"$(INTDIR)\SelProxyCatDlg.sbr" \
	"$(INTDIR)\SessionIdentification.sbr" \
	"$(INTDIR)\SetAliasDlg.sbr" \
	"$(INTDIR)\SetFormDataDlg.sbr" \
	"$(INTDIR)\SetProxy.sbr" \
	"$(INTDIR)\SetSCDlg.sbr" \
	"$(INTDIR)\SimpleSAH.sbr" \
	"$(INTDIR)\SizableReBar.sbr" \
	"$(INTDIR)\SortListCtrl.sbr" \
	"$(INTDIR)\StaticHelper.sbr" \
	"$(INTDIR)\StatusBarWithProgress.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\StopWord.sbr" \
	"$(INTDIR)\TabCtrlSSL.sbr" \
	"$(INTDIR)\TabDropTarget.sbr" \
	"$(INTDIR)\TabPageSSL.sbr" \
	"$(INTDIR)\TextProcessor.sbr" \
	"$(INTDIR)\TextProgressCtrl.sbr" \
	"$(INTDIR)\ToolBarEx.sbr" \
	"$(INTDIR)\ToolMenu.sbr" \
	"$(INTDIR)\Url.sbr" \
	"$(INTDIR)\UrlFilterProp.sbr" \
	"$(INTDIR)\UrlRecordEntity.sbr" \
	"$(INTDIR)\Utility.sbr" \
	"$(INTDIR)\Utils.sbr" \
	"$(INTDIR)\ViewOnLine.sbr" \
	"$(INTDIR)\webbrowser.sbr" \
	"$(INTDIR)\WebICDoc.sbr" \
	"$(INTDIR)\WinAppEx.sbr" \
	"$(INTDIR)\WinAppSingle.sbr" \
	"$(INTDIR)\wizsheet.sbr" \
	"$(INTDIR)\WordMapping.sbr" \
	"$(INTDIR)\WordSearch.sbr" \
	"$(INTDIR)\WordSeq.sbr" \
	"$(INTDIR)\WordSession.sbr" \
	"$(INTDIR)\ChartAxis.sbr" \
	"$(INTDIR)\ChartAxisLabel.sbr" \
	"$(INTDIR)\ChartBarSerie.sbr" \
	"$(INTDIR)\ChartCandlestickSerie.sbr" \
	"$(INTDIR)\ChartCrossHairCursor.sbr" \
	"$(INTDIR)\ChartCtrl.sbr" \
	"$(INTDIR)\ChartCursor.sbr" \
	"$(INTDIR)\ChartDateTimeAxis.sbr" \
	"$(INTDIR)\ChartDragLineCursor.sbr" \
	"$(INTDIR)\ChartFont.sbr" \
	"$(INTDIR)\ChartGanttSerie.sbr" \
	"$(INTDIR)\ChartGradient.sbr" \
	"$(INTDIR)\ChartGrid.sbr" \
	"$(INTDIR)\ChartLegend.sbr" \
	"$(INTDIR)\ChartLineSerie.sbr" \
	"$(INTDIR)\ChartLogarithmicAxis.sbr" \
	"$(INTDIR)\ChartPointsSerie.sbr" \
	"$(INTDIR)\ChartScrollBar.sbr" \
	"$(INTDIR)\ChartSerie.sbr" \
	"$(INTDIR)\ChartStandardAxis.sbr" \
	"$(INTDIR)\ChartSurfaceSerie.sbr" \
	"$(INTDIR)\ChartTitle.sbr" \
	"$(INTDIR)\ChartXYSerie.sbr" \
	"$(INTDIR)\WebFileOpenDlg.sbr"

"$(OUTDIR)\WebMind.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=Wsock32.lib pdh.lib Delayimp.lib winmm.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\WebMind.pdb" /machine:I386 /out:"$(OUTDIR)\WebMind.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AboutCtrl.obj" \
	"$(INTDIR)\AddPopupDlg.obj" \
	"$(INTDIR)\AddressBar.obj" \
	"$(INTDIR)\AddressBarEx.obj" \
	"$(INTDIR)\AddToThisFavDlg.obj" \
	"$(INTDIR)\AdvTabCtrl.obj" \
	"$(INTDIR)\AnimateIcon.obj" \
	"$(INTDIR)\Attribute.obj" \
	"$(INTDIR)\BaseTabCtrl.obj" \
	"$(INTDIR)\BehaviorFeatureAnalysis.obj" \
	"$(INTDIR)\BehaviorFeatureHistoryAnalysis.obj" \
	"$(INTDIR)\C45Learner.obj" \
	"$(INTDIR)\ChevBar.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\CleanAll.obj" \
	"$(INTDIR)\CollectorDlg.obj" \
	"$(INTDIR)\ComboBar.obj" \
	"$(INTDIR)\Connection.obj" \
	"$(INTDIR)\Corpus.obj" \
	"$(INTDIR)\CPARA.OBJ" \
	"$(INTDIR)\Crypt.obj" \
	"$(INTDIR)\CustomizeDialog.obj" \
	"$(INTDIR)\CustomizeToolbar.obj" \
	"$(INTDIR)\Custsite.obj" \
	"$(INTDIR)\DataSet.obj" \
	"$(INTDIR)\DialogHeaderCtrl.obj" \
	"$(INTDIR)\DlgBrowser.obj" \
	"$(INTDIR)\DlgICPage.obj" \
	"$(INTDIR)\DlgPageProp.obj" \
	"$(INTDIR)\DlgSuggest.obj" \
	"$(INTDIR)\editview.obj" \
	"$(INTDIR)\EnTabCtrl.obj" \
	"$(INTDIR)\Evaluation.obj" \
	"$(INTDIR)\ExternalUtilItem.obj" \
	"$(INTDIR)\ExternalUtils.obj" \
	"$(INTDIR)\Extractor.obj" \
	"$(INTDIR)\FeatureExtraction.obj" \
	"$(INTDIR)\FixedHtmlView.obj" \
	"$(INTDIR)\FontStatic.obj" \
	"$(INTDIR)\Function.obj" \
	"$(INTDIR)\GenericHTTPClient.obj" \
	"$(INTDIR)\GlobalSetting.obj" \
	"$(INTDIR)\Google.obj" \
	"$(INTDIR)\Hash.obj" \
	"$(INTDIR)\hightime.obj" \
	"$(INTDIR)\HyperLink.obj" \
	"$(INTDIR)\ICSession.obj" \
	"$(INTDIR)\Idispimp.obj" \
	"$(INTDIR)\IniFile.obj" \
	"$(INTDIR)\InputAlias.obj" \
	"$(INTDIR)\InputDlg.obj" \
	"$(INTDIR)\InputPassword.obj" \
	"$(INTDIR)\Instance.obj" \
	"$(INTDIR)\ItemSet.obj" \
	"$(INTDIR)\LilacObject.obj" \
	"$(INTDIR)\LilacString.obj" \
	"$(INTDIR)\LinkBar.obj" \
	"$(INTDIR)\LinkListDlg.obj" \
	"$(INTDIR)\List.obj" \
	"$(INTDIR)\Logging.obj" \
	"$(INTDIR)\LogTrace.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Markup.obj" \
	"$(INTDIR)\MenuBar.obj" \
	"$(INTDIR)\MenuData.obj" \
	"$(INTDIR)\Menuorder.obj" \
	"$(INTDIR)\WebMind.obj" \
	"$(INTDIR)\WebMindHelper.obj" \
	"$(INTDIR)\WebMindNSHandle.obj" \
	"$(INTDIR)\WebMindOptions.obj" \
	"$(INTDIR)\WebMindView.obj" \
	"$(INTDIR)\NewDlgBar.obj" \
	"$(INTDIR)\PageCache.obj" \
	"$(INTDIR)\PageDocument.obj" \
	"$(INTDIR)\PageDocument2.obj" \
	"$(INTDIR)\PageEval.obj" \
	"$(INTDIR)\PageRecomm.obj" \
	"$(INTDIR)\PageRepository.obj" \
	"$(INTDIR)\PageView.obj" \
	"$(INTDIR)\Parseit.obj" \
	"$(INTDIR)\PopupConfirm.obj" \
	"$(INTDIR)\PopupDlg.obj" \
	"$(INTDIR)\PorterStemming.obj" \
	"$(INTDIR)\ProgressSnakeCtrl.obj" \
	"$(INTDIR)\ProgressWnd.obj" \
	"$(INTDIR)\ProxyInfo.obj" \
	"$(INTDIR)\ProxyList.obj" \
	"$(INTDIR)\PuncMark.obj" \
	"$(INTDIR)\QueryLearner.obj" \
	"$(INTDIR)\QuickSearchDlg.obj" \
	"$(INTDIR)\Recommendation.obj" \
	"$(INTDIR)\SAPrefsDialog.obj" \
	"$(INTDIR)\SAPrefsSubDlg.obj" \
	"$(INTDIR)\SearchEngine.obj" \
	"$(INTDIR)\SearchPage.obj" \
	"$(INTDIR)\SelectLang.obj" \
	"$(INTDIR)\SelectURL.obj" \
	"$(INTDIR)\SelProxyCatDlg.obj" \
	"$(INTDIR)\SessionIdentification.obj" \
	"$(INTDIR)\SetAliasDlg.obj" \
	"$(INTDIR)\SetFormDataDlg.obj" \
	"$(INTDIR)\SetProxy.obj" \
	"$(INTDIR)\SetSCDlg.obj" \
	"$(INTDIR)\SimpleSAH.obj" \
	"$(INTDIR)\SizableReBar.obj" \
	"$(INTDIR)\SortListCtrl.obj" \
	"$(INTDIR)\StaticHelper.obj" \
	"$(INTDIR)\StatusBarWithProgress.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StopWord.obj" \
	"$(INTDIR)\TabCtrlSSL.obj" \
	"$(INTDIR)\TabDropTarget.obj" \
	"$(INTDIR)\TabPageSSL.obj" \
	"$(INTDIR)\TextProcessor.obj" \
	"$(INTDIR)\TextProgressCtrl.obj" \
	"$(INTDIR)\ToolBarEx.obj" \
	"$(INTDIR)\ToolMenu.obj" \
	"$(INTDIR)\Url.obj" \
	"$(INTDIR)\UrlFilterProp.obj" \
	"$(INTDIR)\UrlRecordEntity.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\Utils.obj" \
	"$(INTDIR)\ViewOnLine.obj" \
	"$(INTDIR)\webbrowser.obj" \
	"$(INTDIR)\WebICDoc.obj" \
	"$(INTDIR)\WinAppEx.obj" \
	"$(INTDIR)\WinAppSingle.obj" \
	"$(INTDIR)\wizsheet.obj" \
	"$(INTDIR)\WordMapping.obj" \
	"$(INTDIR)\WordSearch.obj" \
	"$(INTDIR)\WordSeq.obj" \
	"$(INTDIR)\WordSession.obj" \
	"$(INTDIR)\ChartAxis.obj" \
	"$(INTDIR)\ChartAxisLabel.obj" \
	"$(INTDIR)\ChartBarSerie.obj" \
	"$(INTDIR)\ChartCandlestickSerie.obj" \
	"$(INTDIR)\ChartCrossHairCursor.obj" \
	"$(INTDIR)\ChartCtrl.obj" \
	"$(INTDIR)\ChartCursor.obj" \
	"$(INTDIR)\ChartDateTimeAxis.obj" \
	"$(INTDIR)\ChartDragLineCursor.obj" \
	"$(INTDIR)\ChartFont.obj" \
	"$(INTDIR)\ChartGanttSerie.obj" \
	"$(INTDIR)\ChartGradient.obj" \
	"$(INTDIR)\ChartGrid.obj" \
	"$(INTDIR)\ChartLegend.obj" \
	"$(INTDIR)\ChartLineSerie.obj" \
	"$(INTDIR)\ChartLogarithmicAxis.obj" \
	"$(INTDIR)\ChartPointsSerie.obj" \
	"$(INTDIR)\ChartScrollBar.obj" \
	"$(INTDIR)\ChartSerie.obj" \
	"$(INTDIR)\ChartStandardAxis.obj" \
	"$(INTDIR)\ChartSurfaceSerie.obj" \
	"$(INTDIR)\ChartTitle.obj" \
	"$(INTDIR)\ChartXYSerie.obj" \
	"$(INTDIR)\WebFileOpenDlg.obj" \
	"$(INTDIR)\WebMind.res"

"$(OUTDIR)\WebMind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "WebMind - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\WebMind.exe" "$(OUTDIR)\WebMind.tlb" "$(OUTDIR)\WebMind.pch" "$(OUTDIR)\WebMind.bsc"


CLEAN :
	-@erase "$(INTDIR)\AboutCtrl.obj"
	-@erase "$(INTDIR)\AboutCtrl.sbr"
	-@erase "$(INTDIR)\AddPopupDlg.obj"
	-@erase "$(INTDIR)\AddPopupDlg.sbr"
	-@erase "$(INTDIR)\AddressBar.obj"
	-@erase "$(INTDIR)\AddressBar.sbr"
	-@erase "$(INTDIR)\AddressBarEx.obj"
	-@erase "$(INTDIR)\AddressBarEx.sbr"
	-@erase "$(INTDIR)\AddToThisFavDlg.obj"
	-@erase "$(INTDIR)\AddToThisFavDlg.sbr"
	-@erase "$(INTDIR)\AdvTabCtrl.obj"
	-@erase "$(INTDIR)\AdvTabCtrl.sbr"
	-@erase "$(INTDIR)\AnimateIcon.obj"
	-@erase "$(INTDIR)\AnimateIcon.sbr"
	-@erase "$(INTDIR)\Attribute.obj"
	-@erase "$(INTDIR)\Attribute.sbr"
	-@erase "$(INTDIR)\BaseTabCtrl.obj"
	-@erase "$(INTDIR)\BaseTabCtrl.sbr"
	-@erase "$(INTDIR)\BehaviorFeatureAnalysis.obj"
	-@erase "$(INTDIR)\BehaviorFeatureAnalysis.sbr"
	-@erase "$(INTDIR)\BehaviorFeatureHistoryAnalysis.obj"
	-@erase "$(INTDIR)\BehaviorFeatureHistoryAnalysis.sbr"
	-@erase "$(INTDIR)\C45Learner.obj"
	-@erase "$(INTDIR)\C45Learner.sbr"
	-@erase "$(INTDIR)\ChartAxis.obj"
	-@erase "$(INTDIR)\ChartAxis.sbr"
	-@erase "$(INTDIR)\ChartAxisLabel.obj"
	-@erase "$(INTDIR)\ChartAxisLabel.sbr"
	-@erase "$(INTDIR)\ChartBarSerie.obj"
	-@erase "$(INTDIR)\ChartBarSerie.sbr"
	-@erase "$(INTDIR)\ChartCandlestickSerie.obj"
	-@erase "$(INTDIR)\ChartCandlestickSerie.sbr"
	-@erase "$(INTDIR)\ChartCrossHairCursor.obj"
	-@erase "$(INTDIR)\ChartCrossHairCursor.sbr"
	-@erase "$(INTDIR)\ChartCtrl.obj"
	-@erase "$(INTDIR)\ChartCtrl.sbr"
	-@erase "$(INTDIR)\ChartCursor.obj"
	-@erase "$(INTDIR)\ChartCursor.sbr"
	-@erase "$(INTDIR)\ChartDateTimeAxis.obj"
	-@erase "$(INTDIR)\ChartDateTimeAxis.sbr"
	-@erase "$(INTDIR)\ChartDragLineCursor.obj"
	-@erase "$(INTDIR)\ChartDragLineCursor.sbr"
	-@erase "$(INTDIR)\ChartFont.obj"
	-@erase "$(INTDIR)\ChartFont.sbr"
	-@erase "$(INTDIR)\ChartGanttSerie.obj"
	-@erase "$(INTDIR)\ChartGanttSerie.sbr"
	-@erase "$(INTDIR)\ChartGradient.obj"
	-@erase "$(INTDIR)\ChartGradient.sbr"
	-@erase "$(INTDIR)\ChartGrid.obj"
	-@erase "$(INTDIR)\ChartGrid.sbr"
	-@erase "$(INTDIR)\ChartLegend.obj"
	-@erase "$(INTDIR)\ChartLegend.sbr"
	-@erase "$(INTDIR)\ChartLineSerie.obj"
	-@erase "$(INTDIR)\ChartLineSerie.sbr"
	-@erase "$(INTDIR)\ChartLogarithmicAxis.obj"
	-@erase "$(INTDIR)\ChartLogarithmicAxis.sbr"
	-@erase "$(INTDIR)\ChartPointsSerie.obj"
	-@erase "$(INTDIR)\ChartPointsSerie.sbr"
	-@erase "$(INTDIR)\ChartScrollBar.obj"
	-@erase "$(INTDIR)\ChartScrollBar.sbr"
	-@erase "$(INTDIR)\ChartSerie.obj"
	-@erase "$(INTDIR)\ChartSerie.sbr"
	-@erase "$(INTDIR)\ChartStandardAxis.obj"
	-@erase "$(INTDIR)\ChartStandardAxis.sbr"
	-@erase "$(INTDIR)\ChartSurfaceSerie.obj"
	-@erase "$(INTDIR)\ChartSurfaceSerie.sbr"
	-@erase "$(INTDIR)\ChartTitle.obj"
	-@erase "$(INTDIR)\ChartTitle.sbr"
	-@erase "$(INTDIR)\ChartXYSerie.obj"
	-@erase "$(INTDIR)\ChartXYSerie.sbr"
	-@erase "$(INTDIR)\ChevBar.obj"
	-@erase "$(INTDIR)\ChevBar.sbr"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\ChildFrm.sbr"
	-@erase "$(INTDIR)\CleanAll.obj"
	-@erase "$(INTDIR)\CleanAll.sbr"
	-@erase "$(INTDIR)\CollectorDlg.obj"
	-@erase "$(INTDIR)\CollectorDlg.sbr"
	-@erase "$(INTDIR)\ComboBar.obj"
	-@erase "$(INTDIR)\ComboBar.sbr"
	-@erase "$(INTDIR)\Connection.obj"
	-@erase "$(INTDIR)\Connection.sbr"
	-@erase "$(INTDIR)\Corpus.obj"
	-@erase "$(INTDIR)\Corpus.sbr"
	-@erase "$(INTDIR)\CPARA.OBJ"
	-@erase "$(INTDIR)\CPARA.SBR"
	-@erase "$(INTDIR)\Crypt.obj"
	-@erase "$(INTDIR)\Crypt.sbr"
	-@erase "$(INTDIR)\CustomizeDialog.obj"
	-@erase "$(INTDIR)\CustomizeDialog.sbr"
	-@erase "$(INTDIR)\CustomizeToolbar.obj"
	-@erase "$(INTDIR)\CustomizeToolbar.sbr"
	-@erase "$(INTDIR)\Custsite.obj"
	-@erase "$(INTDIR)\Custsite.sbr"
	-@erase "$(INTDIR)\DataSet.obj"
	-@erase "$(INTDIR)\DataSet.sbr"
	-@erase "$(INTDIR)\DialogHeaderCtrl.obj"
	-@erase "$(INTDIR)\DialogHeaderCtrl.sbr"
	-@erase "$(INTDIR)\DlgBrowser.obj"
	-@erase "$(INTDIR)\DlgBrowser.sbr"
	-@erase "$(INTDIR)\DlgICPage.obj"
	-@erase "$(INTDIR)\DlgICPage.sbr"
	-@erase "$(INTDIR)\DlgPageProp.obj"
	-@erase "$(INTDIR)\DlgPageProp.sbr"
	-@erase "$(INTDIR)\DlgSuggest.obj"
	-@erase "$(INTDIR)\DlgSuggest.sbr"
	-@erase "$(INTDIR)\editview.obj"
	-@erase "$(INTDIR)\editview.sbr"
	-@erase "$(INTDIR)\EnTabCtrl.obj"
	-@erase "$(INTDIR)\EnTabCtrl.sbr"
	-@erase "$(INTDIR)\Evaluation.obj"
	-@erase "$(INTDIR)\Evaluation.sbr"
	-@erase "$(INTDIR)\ExternalUtilItem.obj"
	-@erase "$(INTDIR)\ExternalUtilItem.sbr"
	-@erase "$(INTDIR)\ExternalUtils.obj"
	-@erase "$(INTDIR)\ExternalUtils.sbr"
	-@erase "$(INTDIR)\Extractor.obj"
	-@erase "$(INTDIR)\Extractor.sbr"
	-@erase "$(INTDIR)\FeatureExtraction.obj"
	-@erase "$(INTDIR)\FeatureExtraction.sbr"
	-@erase "$(INTDIR)\FixedHtmlView.obj"
	-@erase "$(INTDIR)\FixedHtmlView.sbr"
	-@erase "$(INTDIR)\FontStatic.obj"
	-@erase "$(INTDIR)\FontStatic.sbr"
	-@erase "$(INTDIR)\Function.obj"
	-@erase "$(INTDIR)\Function.sbr"
	-@erase "$(INTDIR)\GenericHTTPClient.obj"
	-@erase "$(INTDIR)\GenericHTTPClient.sbr"
	-@erase "$(INTDIR)\GlobalSetting.obj"
	-@erase "$(INTDIR)\GlobalSetting.sbr"
	-@erase "$(INTDIR)\Google.obj"
	-@erase "$(INTDIR)\Google.sbr"
	-@erase "$(INTDIR)\Hash.obj"
	-@erase "$(INTDIR)\Hash.sbr"
	-@erase "$(INTDIR)\hightime.obj"
	-@erase "$(INTDIR)\hightime.sbr"
	-@erase "$(INTDIR)\HyperLink.obj"
	-@erase "$(INTDIR)\HyperLink.sbr"
	-@erase "$(INTDIR)\ICSession.obj"
	-@erase "$(INTDIR)\ICSession.sbr"
	-@erase "$(INTDIR)\Idispimp.obj"
	-@erase "$(INTDIR)\Idispimp.sbr"
	-@erase "$(INTDIR)\IniFile.obj"
	-@erase "$(INTDIR)\IniFile.sbr"
	-@erase "$(INTDIR)\InputAlias.obj"
	-@erase "$(INTDIR)\InputAlias.sbr"
	-@erase "$(INTDIR)\InputDlg.obj"
	-@erase "$(INTDIR)\InputDlg.sbr"
	-@erase "$(INTDIR)\InputPassword.obj"
	-@erase "$(INTDIR)\InputPassword.sbr"
	-@erase "$(INTDIR)\Instance.obj"
	-@erase "$(INTDIR)\Instance.sbr"
	-@erase "$(INTDIR)\ItemSet.obj"
	-@erase "$(INTDIR)\ItemSet.sbr"
	-@erase "$(INTDIR)\LilacObject.obj"
	-@erase "$(INTDIR)\LilacObject.sbr"
	-@erase "$(INTDIR)\LilacString.obj"
	-@erase "$(INTDIR)\LilacString.sbr"
	-@erase "$(INTDIR)\LinkBar.obj"
	-@erase "$(INTDIR)\LinkBar.sbr"
	-@erase "$(INTDIR)\LinkListDlg.obj"
	-@erase "$(INTDIR)\LinkListDlg.sbr"
	-@erase "$(INTDIR)\List.obj"
	-@erase "$(INTDIR)\List.sbr"
	-@erase "$(INTDIR)\Logging.obj"
	-@erase "$(INTDIR)\Logging.sbr"
	-@erase "$(INTDIR)\LogTrace.obj"
	-@erase "$(INTDIR)\LogTrace.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\Markup.obj"
	-@erase "$(INTDIR)\Markup.sbr"
	-@erase "$(INTDIR)\MenuBar.obj"
	-@erase "$(INTDIR)\MenuBar.sbr"
	-@erase "$(INTDIR)\MenuData.obj"
	-@erase "$(INTDIR)\MenuData.sbr"
	-@erase "$(INTDIR)\Menuorder.obj"
	-@erase "$(INTDIR)\Menuorder.sbr"
	-@erase "$(INTDIR)\WebMind.obj"
	-@erase "$(INTDIR)\WebMind.pch"
	-@erase "$(INTDIR)\WebMind.res"
	-@erase "$(INTDIR)\WebMind.sbr"
	-@erase "$(INTDIR)\WebMind.tlb"
	-@erase "$(INTDIR)\WebMindHelper.obj"
	-@erase "$(INTDIR)\WebMindHelper.sbr"
	-@erase "$(INTDIR)\WebMindNSHandle.obj"
	-@erase "$(INTDIR)\WebMindNSHandle.sbr"
	-@erase "$(INTDIR)\WebMindOptions.obj"
	-@erase "$(INTDIR)\WebMindOptions.sbr"
	-@erase "$(INTDIR)\WebMindView.obj"
	-@erase "$(INTDIR)\WebMindView.sbr"
	-@erase "$(INTDIR)\NewDlgBar.obj"
	-@erase "$(INTDIR)\NewDlgBar.sbr"
	-@erase "$(INTDIR)\PageCache.obj"
	-@erase "$(INTDIR)\PageCache.sbr"
	-@erase "$(INTDIR)\PageDocument.obj"
	-@erase "$(INTDIR)\PageDocument.sbr"
	-@erase "$(INTDIR)\PageDocument2.obj"
	-@erase "$(INTDIR)\PageDocument2.sbr"
	-@erase "$(INTDIR)\PageEval.obj"
	-@erase "$(INTDIR)\PageEval.sbr"
	-@erase "$(INTDIR)\PageRecomm.obj"
	-@erase "$(INTDIR)\PageRecomm.sbr"
	-@erase "$(INTDIR)\PageRepository.obj"
	-@erase "$(INTDIR)\PageRepository.sbr"
	-@erase "$(INTDIR)\PageView.obj"
	-@erase "$(INTDIR)\PageView.sbr"
	-@erase "$(INTDIR)\Parseit.obj"
	-@erase "$(INTDIR)\Parseit.sbr"
	-@erase "$(INTDIR)\PopupConfirm.obj"
	-@erase "$(INTDIR)\PopupConfirm.sbr"
	-@erase "$(INTDIR)\PopupDlg.obj"
	-@erase "$(INTDIR)\PopupDlg.sbr"
	-@erase "$(INTDIR)\PorterStemming.obj"
	-@erase "$(INTDIR)\PorterStemming.sbr"
	-@erase "$(INTDIR)\ProgressSnakeCtrl.obj"
	-@erase "$(INTDIR)\ProgressSnakeCtrl.sbr"
	-@erase "$(INTDIR)\ProgressWnd.obj"
	-@erase "$(INTDIR)\ProgressWnd.sbr"
	-@erase "$(INTDIR)\ProxyInfo.obj"
	-@erase "$(INTDIR)\ProxyInfo.sbr"
	-@erase "$(INTDIR)\ProxyList.obj"
	-@erase "$(INTDIR)\ProxyList.sbr"
	-@erase "$(INTDIR)\PuncMark.obj"
	-@erase "$(INTDIR)\PuncMark.sbr"
	-@erase "$(INTDIR)\QueryLearner.obj"
	-@erase "$(INTDIR)\QueryLearner.sbr"
	-@erase "$(INTDIR)\QuickSearchDlg.obj"
	-@erase "$(INTDIR)\QuickSearchDlg.sbr"
	-@erase "$(INTDIR)\Recommendation.obj"
	-@erase "$(INTDIR)\Recommendation.sbr"
	-@erase "$(INTDIR)\SAPrefsDialog.obj"
	-@erase "$(INTDIR)\SAPrefsDialog.sbr"
	-@erase "$(INTDIR)\SAPrefsSubDlg.obj"
	-@erase "$(INTDIR)\SAPrefsSubDlg.sbr"
	-@erase "$(INTDIR)\SearchEngine.obj"
	-@erase "$(INTDIR)\SearchEngine.sbr"
	-@erase "$(INTDIR)\SearchPage.obj"
	-@erase "$(INTDIR)\SearchPage.sbr"
	-@erase "$(INTDIR)\SelectLang.obj"
	-@erase "$(INTDIR)\SelectLang.sbr"
	-@erase "$(INTDIR)\SelectURL.obj"
	-@erase "$(INTDIR)\SelectURL.sbr"
	-@erase "$(INTDIR)\SelProxyCatDlg.obj"
	-@erase "$(INTDIR)\SelProxyCatDlg.sbr"
	-@erase "$(INTDIR)\SessionIdentification.obj"
	-@erase "$(INTDIR)\SessionIdentification.sbr"
	-@erase "$(INTDIR)\SetAliasDlg.obj"
	-@erase "$(INTDIR)\SetAliasDlg.sbr"
	-@erase "$(INTDIR)\SetFormDataDlg.obj"
	-@erase "$(INTDIR)\SetFormDataDlg.sbr"
	-@erase "$(INTDIR)\SetProxy.obj"
	-@erase "$(INTDIR)\SetProxy.sbr"
	-@erase "$(INTDIR)\SetSCDlg.obj"
	-@erase "$(INTDIR)\SetSCDlg.sbr"
	-@erase "$(INTDIR)\SimpleSAH.obj"
	-@erase "$(INTDIR)\SimpleSAH.sbr"
	-@erase "$(INTDIR)\SizableReBar.obj"
	-@erase "$(INTDIR)\SizableReBar.sbr"
	-@erase "$(INTDIR)\SortListCtrl.obj"
	-@erase "$(INTDIR)\SortListCtrl.sbr"
	-@erase "$(INTDIR)\StaticHelper.obj"
	-@erase "$(INTDIR)\StaticHelper.sbr"
	-@erase "$(INTDIR)\StatusBarWithProgress.obj"
	-@erase "$(INTDIR)\StatusBarWithProgress.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\StopWord.obj"
	-@erase "$(INTDIR)\StopWord.sbr"
	-@erase "$(INTDIR)\TabCtrlSSL.obj"
	-@erase "$(INTDIR)\TabCtrlSSL.sbr"
	-@erase "$(INTDIR)\TabDropTarget.obj"
	-@erase "$(INTDIR)\TabDropTarget.sbr"
	-@erase "$(INTDIR)\TabPageSSL.obj"
	-@erase "$(INTDIR)\TabPageSSL.sbr"
	-@erase "$(INTDIR)\TextProcessor.obj"
	-@erase "$(INTDIR)\TextProcessor.sbr"
	-@erase "$(INTDIR)\TextProgressCtrl.obj"
	-@erase "$(INTDIR)\TextProgressCtrl.sbr"
	-@erase "$(INTDIR)\ToolBarEx.obj"
	-@erase "$(INTDIR)\ToolBarEx.sbr"
	-@erase "$(INTDIR)\ToolMenu.obj"
	-@erase "$(INTDIR)\ToolMenu.sbr"
	-@erase "$(INTDIR)\Url.obj"
	-@erase "$(INTDIR)\Url.sbr"
	-@erase "$(INTDIR)\UrlFilterProp.obj"
	-@erase "$(INTDIR)\UrlFilterProp.sbr"
	-@erase "$(INTDIR)\UrlRecordEntity.obj"
	-@erase "$(INTDIR)\UrlRecordEntity.sbr"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\Utility.sbr"
	-@erase "$(INTDIR)\Utils.obj"
	-@erase "$(INTDIR)\Utils.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\ViewOnLine.obj"
	-@erase "$(INTDIR)\ViewOnLine.sbr"
	-@erase "$(INTDIR)\webbrowser.obj"
	-@erase "$(INTDIR)\webbrowser.sbr"
	-@erase "$(INTDIR)\WebFileOpenDlg.obj"
	-@erase "$(INTDIR)\WebFileOpenDlg.sbr"
	-@erase "$(INTDIR)\WebICDoc.obj"
	-@erase "$(INTDIR)\WebICDoc.sbr"
	-@erase "$(INTDIR)\WinAppEx.obj"
	-@erase "$(INTDIR)\WinAppEx.sbr"
	-@erase "$(INTDIR)\WinAppSingle.obj"
	-@erase "$(INTDIR)\WinAppSingle.sbr"
	-@erase "$(INTDIR)\wizsheet.obj"
	-@erase "$(INTDIR)\wizsheet.sbr"
	-@erase "$(INTDIR)\WordMapping.obj"
	-@erase "$(INTDIR)\WordMapping.sbr"
	-@erase "$(INTDIR)\WordSearch.obj"
	-@erase "$(INTDIR)\WordSearch.sbr"
	-@erase "$(INTDIR)\WordSeq.obj"
	-@erase "$(INTDIR)\WordSeq.sbr"
	-@erase "$(INTDIR)\WordSession.obj"
	-@erase "$(INTDIR)\WordSession.sbr"
	-@erase "$(OUTDIR)\WebMind.bsc"
	-@erase "$(OUTDIR)\WebMind.exe"
	-@erase "$(OUTDIR)\WebMind.map"
	-@erase ".\WebMind_i.c"
	-@erase ".\WebMind_i.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "_WINDOWS" /Fr"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\WebMind.res" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WebMind.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AboutCtrl.sbr" \
	"$(INTDIR)\AddPopupDlg.sbr" \
	"$(INTDIR)\AddressBar.sbr" \
	"$(INTDIR)\AddressBarEx.sbr" \
	"$(INTDIR)\AddToThisFavDlg.sbr" \
	"$(INTDIR)\AdvTabCtrl.sbr" \
	"$(INTDIR)\AnimateIcon.sbr" \
	"$(INTDIR)\Attribute.sbr" \
	"$(INTDIR)\BaseTabCtrl.sbr" \
	"$(INTDIR)\BehaviorFeatureAnalysis.sbr" \
	"$(INTDIR)\BehaviorFeatureHistoryAnalysis.sbr" \
	"$(INTDIR)\C45Learner.sbr" \
	"$(INTDIR)\ChevBar.sbr" \
	"$(INTDIR)\ChildFrm.sbr" \
	"$(INTDIR)\CleanAll.sbr" \
	"$(INTDIR)\CollectorDlg.sbr" \
	"$(INTDIR)\ComboBar.sbr" \
	"$(INTDIR)\Connection.sbr" \
	"$(INTDIR)\Corpus.sbr" \
	"$(INTDIR)\CPARA.SBR" \
	"$(INTDIR)\Crypt.sbr" \
	"$(INTDIR)\CustomizeDialog.sbr" \
	"$(INTDIR)\CustomizeToolbar.sbr" \
	"$(INTDIR)\Custsite.sbr" \
	"$(INTDIR)\DataSet.sbr" \
	"$(INTDIR)\DialogHeaderCtrl.sbr" \
	"$(INTDIR)\DlgBrowser.sbr" \
	"$(INTDIR)\DlgICPage.sbr" \
	"$(INTDIR)\DlgPageProp.sbr" \
	"$(INTDIR)\DlgSuggest.sbr" \
	"$(INTDIR)\editview.sbr" \
	"$(INTDIR)\EnTabCtrl.sbr" \
	"$(INTDIR)\Evaluation.sbr" \
	"$(INTDIR)\ExternalUtilItem.sbr" \
	"$(INTDIR)\ExternalUtils.sbr" \
	"$(INTDIR)\Extractor.sbr" \
	"$(INTDIR)\FeatureExtraction.sbr" \
	"$(INTDIR)\FixedHtmlView.sbr" \
	"$(INTDIR)\FontStatic.sbr" \
	"$(INTDIR)\Function.sbr" \
	"$(INTDIR)\GenericHTTPClient.sbr" \
	"$(INTDIR)\GlobalSetting.sbr" \
	"$(INTDIR)\Google.sbr" \
	"$(INTDIR)\Hash.sbr" \
	"$(INTDIR)\hightime.sbr" \
	"$(INTDIR)\HyperLink.sbr" \
	"$(INTDIR)\ICSession.sbr" \
	"$(INTDIR)\Idispimp.sbr" \
	"$(INTDIR)\IniFile.sbr" \
	"$(INTDIR)\InputAlias.sbr" \
	"$(INTDIR)\InputDlg.sbr" \
	"$(INTDIR)\InputPassword.sbr" \
	"$(INTDIR)\Instance.sbr" \
	"$(INTDIR)\ItemSet.sbr" \
	"$(INTDIR)\LilacObject.sbr" \
	"$(INTDIR)\LilacString.sbr" \
	"$(INTDIR)\LinkBar.sbr" \
	"$(INTDIR)\LinkListDlg.sbr" \
	"$(INTDIR)\List.sbr" \
	"$(INTDIR)\Logging.sbr" \
	"$(INTDIR)\LogTrace.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\Markup.sbr" \
	"$(INTDIR)\MenuBar.sbr" \
	"$(INTDIR)\MenuData.sbr" \
	"$(INTDIR)\Menuorder.sbr" \
	"$(INTDIR)\WebMind.sbr" \
	"$(INTDIR)\WebMindHelper.sbr" \
	"$(INTDIR)\WebMindNSHandle.sbr" \
	"$(INTDIR)\WebMindOptions.sbr" \
	"$(INTDIR)\WebMindView.sbr" \
	"$(INTDIR)\NewDlgBar.sbr" \
	"$(INTDIR)\PageCache.sbr" \
	"$(INTDIR)\PageDocument.sbr" \
	"$(INTDIR)\PageDocument2.sbr" \
	"$(INTDIR)\PageEval.sbr" \
	"$(INTDIR)\PageRecomm.sbr" \
	"$(INTDIR)\PageRepository.sbr" \
	"$(INTDIR)\PageView.sbr" \
	"$(INTDIR)\Parseit.sbr" \
	"$(INTDIR)\PopupConfirm.sbr" \
	"$(INTDIR)\PopupDlg.sbr" \
	"$(INTDIR)\PorterStemming.sbr" \
	"$(INTDIR)\ProgressSnakeCtrl.sbr" \
	"$(INTDIR)\ProgressWnd.sbr" \
	"$(INTDIR)\ProxyInfo.sbr" \
	"$(INTDIR)\ProxyList.sbr" \
	"$(INTDIR)\PuncMark.sbr" \
	"$(INTDIR)\QueryLearner.sbr" \
	"$(INTDIR)\QuickSearchDlg.sbr" \
	"$(INTDIR)\Recommendation.sbr" \
	"$(INTDIR)\SAPrefsDialog.sbr" \
	"$(INTDIR)\SAPrefsSubDlg.sbr" \
	"$(INTDIR)\SearchEngine.sbr" \
	"$(INTDIR)\SearchPage.sbr" \
	"$(INTDIR)\SelectLang.sbr" \
	"$(INTDIR)\SelectURL.sbr" \
	"$(INTDIR)\SelProxyCatDlg.sbr" \
	"$(INTDIR)\SessionIdentification.sbr" \
	"$(INTDIR)\SetAliasDlg.sbr" \
	"$(INTDIR)\SetFormDataDlg.sbr" \
	"$(INTDIR)\SetProxy.sbr" \
	"$(INTDIR)\SetSCDlg.sbr" \
	"$(INTDIR)\SimpleSAH.sbr" \
	"$(INTDIR)\SizableReBar.sbr" \
	"$(INTDIR)\SortListCtrl.sbr" \
	"$(INTDIR)\StaticHelper.sbr" \
	"$(INTDIR)\StatusBarWithProgress.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\StopWord.sbr" \
	"$(INTDIR)\TabCtrlSSL.sbr" \
	"$(INTDIR)\TabDropTarget.sbr" \
	"$(INTDIR)\TabPageSSL.sbr" \
	"$(INTDIR)\TextProcessor.sbr" \
	"$(INTDIR)\TextProgressCtrl.sbr" \
	"$(INTDIR)\ToolBarEx.sbr" \
	"$(INTDIR)\ToolMenu.sbr" \
	"$(INTDIR)\Url.sbr" \
	"$(INTDIR)\UrlFilterProp.sbr" \
	"$(INTDIR)\UrlRecordEntity.sbr" \
	"$(INTDIR)\Utility.sbr" \
	"$(INTDIR)\Utils.sbr" \
	"$(INTDIR)\ViewOnLine.sbr" \
	"$(INTDIR)\webbrowser.sbr" \
	"$(INTDIR)\WebICDoc.sbr" \
	"$(INTDIR)\WinAppEx.sbr" \
	"$(INTDIR)\WinAppSingle.sbr" \
	"$(INTDIR)\wizsheet.sbr" \
	"$(INTDIR)\WordMapping.sbr" \
	"$(INTDIR)\WordSearch.sbr" \
	"$(INTDIR)\WordSeq.sbr" \
	"$(INTDIR)\WordSession.sbr" \
	"$(INTDIR)\ChartAxis.sbr" \
	"$(INTDIR)\ChartAxisLabel.sbr" \
	"$(INTDIR)\ChartBarSerie.sbr" \
	"$(INTDIR)\ChartCandlestickSerie.sbr" \
	"$(INTDIR)\ChartCrossHairCursor.sbr" \
	"$(INTDIR)\ChartCtrl.sbr" \
	"$(INTDIR)\ChartCursor.sbr" \
	"$(INTDIR)\ChartDateTimeAxis.sbr" \
	"$(INTDIR)\ChartDragLineCursor.sbr" \
	"$(INTDIR)\ChartFont.sbr" \
	"$(INTDIR)\ChartGanttSerie.sbr" \
	"$(INTDIR)\ChartGradient.sbr" \
	"$(INTDIR)\ChartGrid.sbr" \
	"$(INTDIR)\ChartLegend.sbr" \
	"$(INTDIR)\ChartLineSerie.sbr" \
	"$(INTDIR)\ChartLogarithmicAxis.sbr" \
	"$(INTDIR)\ChartPointsSerie.sbr" \
	"$(INTDIR)\ChartScrollBar.sbr" \
	"$(INTDIR)\ChartSerie.sbr" \
	"$(INTDIR)\ChartStandardAxis.sbr" \
	"$(INTDIR)\ChartSurfaceSerie.sbr" \
	"$(INTDIR)\ChartTitle.sbr" \
	"$(INTDIR)\ChartXYSerie.sbr" \
	"$(INTDIR)\WebFileOpenDlg.sbr"

"$(OUTDIR)\WebMind.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=Wsock32.lib Delayimp.lib winmm.lib Base.lib /nologo /subsystem:windows /profile /map:"$(INTDIR)\WebMind.map" /debug /machine:I386 /nodefaultlib:"nafxcwd.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /out:"$(OUTDIR)\WebMind.exe" /libpath:"Debug" 
LINK32_OBJS= \
	"$(INTDIR)\AboutCtrl.obj" \
	"$(INTDIR)\AddPopupDlg.obj" \
	"$(INTDIR)\AddressBar.obj" \
	"$(INTDIR)\AddressBarEx.obj" \
	"$(INTDIR)\AddToThisFavDlg.obj" \
	"$(INTDIR)\AdvTabCtrl.obj" \
	"$(INTDIR)\AnimateIcon.obj" \
	"$(INTDIR)\Attribute.obj" \
	"$(INTDIR)\BaseTabCtrl.obj" \
	"$(INTDIR)\BehaviorFeatureAnalysis.obj" \
	"$(INTDIR)\BehaviorFeatureHistoryAnalysis.obj" \
	"$(INTDIR)\C45Learner.obj" \
	"$(INTDIR)\ChevBar.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\CleanAll.obj" \
	"$(INTDIR)\CollectorDlg.obj" \
	"$(INTDIR)\ComboBar.obj" \
	"$(INTDIR)\Connection.obj" \
	"$(INTDIR)\Corpus.obj" \
	"$(INTDIR)\CPARA.OBJ" \
	"$(INTDIR)\Crypt.obj" \
	"$(INTDIR)\CustomizeDialog.obj" \
	"$(INTDIR)\CustomizeToolbar.obj" \
	"$(INTDIR)\Custsite.obj" \
	"$(INTDIR)\DataSet.obj" \
	"$(INTDIR)\DialogHeaderCtrl.obj" \
	"$(INTDIR)\DlgBrowser.obj" \
	"$(INTDIR)\DlgICPage.obj" \
	"$(INTDIR)\DlgPageProp.obj" \
	"$(INTDIR)\DlgSuggest.obj" \
	"$(INTDIR)\editview.obj" \
	"$(INTDIR)\EnTabCtrl.obj" \
	"$(INTDIR)\Evaluation.obj" \
	"$(INTDIR)\ExternalUtilItem.obj" \
	"$(INTDIR)\ExternalUtils.obj" \
	"$(INTDIR)\Extractor.obj" \
	"$(INTDIR)\FeatureExtraction.obj" \
	"$(INTDIR)\FixedHtmlView.obj" \
	"$(INTDIR)\FontStatic.obj" \
	"$(INTDIR)\Function.obj" \
	"$(INTDIR)\GenericHTTPClient.obj" \
	"$(INTDIR)\GlobalSetting.obj" \
	"$(INTDIR)\Google.obj" \
	"$(INTDIR)\Hash.obj" \
	"$(INTDIR)\hightime.obj" \
	"$(INTDIR)\HyperLink.obj" \
	"$(INTDIR)\ICSession.obj" \
	"$(INTDIR)\Idispimp.obj" \
	"$(INTDIR)\IniFile.obj" \
	"$(INTDIR)\InputAlias.obj" \
	"$(INTDIR)\InputDlg.obj" \
	"$(INTDIR)\InputPassword.obj" \
	"$(INTDIR)\Instance.obj" \
	"$(INTDIR)\ItemSet.obj" \
	"$(INTDIR)\LilacObject.obj" \
	"$(INTDIR)\LilacString.obj" \
	"$(INTDIR)\LinkBar.obj" \
	"$(INTDIR)\LinkListDlg.obj" \
	"$(INTDIR)\List.obj" \
	"$(INTDIR)\Logging.obj" \
	"$(INTDIR)\LogTrace.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Markup.obj" \
	"$(INTDIR)\MenuBar.obj" \
	"$(INTDIR)\MenuData.obj" \
	"$(INTDIR)\Menuorder.obj" \
	"$(INTDIR)\WebMind.obj" \
	"$(INTDIR)\WebMindHelper.obj" \
	"$(INTDIR)\WebMindNSHandle.obj" \
	"$(INTDIR)\WebMindOptions.obj" \
	"$(INTDIR)\WebMindView.obj" \
	"$(INTDIR)\NewDlgBar.obj" \
	"$(INTDIR)\PageCache.obj" \
	"$(INTDIR)\PageDocument.obj" \
	"$(INTDIR)\PageDocument2.obj" \
	"$(INTDIR)\PageEval.obj" \
	"$(INTDIR)\PageRecomm.obj" \
	"$(INTDIR)\PageRepository.obj" \
	"$(INTDIR)\PageView.obj" \
	"$(INTDIR)\Parseit.obj" \
	"$(INTDIR)\PopupConfirm.obj" \
	"$(INTDIR)\PopupDlg.obj" \
	"$(INTDIR)\PorterStemming.obj" \
	"$(INTDIR)\ProgressSnakeCtrl.obj" \
	"$(INTDIR)\ProgressWnd.obj" \
	"$(INTDIR)\ProxyInfo.obj" \
	"$(INTDIR)\ProxyList.obj" \
	"$(INTDIR)\PuncMark.obj" \
	"$(INTDIR)\QueryLearner.obj" \
	"$(INTDIR)\QuickSearchDlg.obj" \
	"$(INTDIR)\Recommendation.obj" \
	"$(INTDIR)\SAPrefsDialog.obj" \
	"$(INTDIR)\SAPrefsSubDlg.obj" \
	"$(INTDIR)\SearchEngine.obj" \
	"$(INTDIR)\SearchPage.obj" \
	"$(INTDIR)\SelectLang.obj" \
	"$(INTDIR)\SelectURL.obj" \
	"$(INTDIR)\SelProxyCatDlg.obj" \
	"$(INTDIR)\SessionIdentification.obj" \
	"$(INTDIR)\SetAliasDlg.obj" \
	"$(INTDIR)\SetFormDataDlg.obj" \
	"$(INTDIR)\SetProxy.obj" \
	"$(INTDIR)\SetSCDlg.obj" \
	"$(INTDIR)\SimpleSAH.obj" \
	"$(INTDIR)\SizableReBar.obj" \
	"$(INTDIR)\SortListCtrl.obj" \
	"$(INTDIR)\StaticHelper.obj" \
	"$(INTDIR)\StatusBarWithProgress.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StopWord.obj" \
	"$(INTDIR)\TabCtrlSSL.obj" \
	"$(INTDIR)\TabDropTarget.obj" \
	"$(INTDIR)\TabPageSSL.obj" \
	"$(INTDIR)\TextProcessor.obj" \
	"$(INTDIR)\TextProgressCtrl.obj" \
	"$(INTDIR)\ToolBarEx.obj" \
	"$(INTDIR)\ToolMenu.obj" \
	"$(INTDIR)\Url.obj" \
	"$(INTDIR)\UrlFilterProp.obj" \
	"$(INTDIR)\UrlRecordEntity.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\Utils.obj" \
	"$(INTDIR)\ViewOnLine.obj" \
	"$(INTDIR)\webbrowser.obj" \
	"$(INTDIR)\WebICDoc.obj" \
	"$(INTDIR)\WinAppEx.obj" \
	"$(INTDIR)\WinAppSingle.obj" \
	"$(INTDIR)\wizsheet.obj" \
	"$(INTDIR)\WordMapping.obj" \
	"$(INTDIR)\WordSearch.obj" \
	"$(INTDIR)\WordSeq.obj" \
	"$(INTDIR)\WordSession.obj" \
	"$(INTDIR)\ChartAxis.obj" \
	"$(INTDIR)\ChartAxisLabel.obj" \
	"$(INTDIR)\ChartBarSerie.obj" \
	"$(INTDIR)\ChartCandlestickSerie.obj" \
	"$(INTDIR)\ChartCrossHairCursor.obj" \
	"$(INTDIR)\ChartCtrl.obj" \
	"$(INTDIR)\ChartCursor.obj" \
	"$(INTDIR)\ChartDateTimeAxis.obj" \
	"$(INTDIR)\ChartDragLineCursor.obj" \
	"$(INTDIR)\ChartFont.obj" \
	"$(INTDIR)\ChartGanttSerie.obj" \
	"$(INTDIR)\ChartGradient.obj" \
	"$(INTDIR)\ChartGrid.obj" \
	"$(INTDIR)\ChartLegend.obj" \
	"$(INTDIR)\ChartLineSerie.obj" \
	"$(INTDIR)\ChartLogarithmicAxis.obj" \
	"$(INTDIR)\ChartPointsSerie.obj" \
	"$(INTDIR)\ChartScrollBar.obj" \
	"$(INTDIR)\ChartSerie.obj" \
	"$(INTDIR)\ChartStandardAxis.obj" \
	"$(INTDIR)\ChartSurfaceSerie.obj" \
	"$(INTDIR)\ChartTitle.obj" \
	"$(INTDIR)\ChartXYSerie.obj" \
	"$(INTDIR)\WebFileOpenDlg.obj" \
	"$(INTDIR)\WebMind.res"

"$(OUTDIR)\WebMind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("WebMind.dep")
!INCLUDE "WebMind.dep"
!ELSE 
!MESSAGE Warning: cannot find "WebMind.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "WebMind - Win32 Release" || "$(CFG)" == "WebMind - Win32 Debug"
SOURCE=.\AboutCtrl.cpp

"$(INTDIR)\AboutCtrl.obj"	"$(INTDIR)\AboutCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\AddPopupDlg.cpp

"$(INTDIR)\AddPopupDlg.obj"	"$(INTDIR)\AddPopupDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\AddressBar.cpp

"$(INTDIR)\AddressBar.obj"	"$(INTDIR)\AddressBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\AddressBarEx.cpp

"$(INTDIR)\AddressBarEx.obj"	"$(INTDIR)\AddressBarEx.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\AddToThisFavDlg.cpp

"$(INTDIR)\AddToThisFavDlg.obj"	"$(INTDIR)\AddToThisFavDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\AdvTabCtrl.cpp

"$(INTDIR)\AdvTabCtrl.obj"	"$(INTDIR)\AdvTabCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\AnimateIcon.cpp

"$(INTDIR)\AnimateIcon.obj"	"$(INTDIR)\AnimateIcon.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Attribute.cpp

"$(INTDIR)\Attribute.obj"	"$(INTDIR)\Attribute.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\BaseTabCtrl.cpp

"$(INTDIR)\BaseTabCtrl.obj"	"$(INTDIR)\BaseTabCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\BehaviorFeatureAnalysis.cpp

"$(INTDIR)\BehaviorFeatureAnalysis.obj"	"$(INTDIR)\BehaviorFeatureAnalysis.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\BehaviorFeatureHistoryAnalysis.cpp

"$(INTDIR)\BehaviorFeatureHistoryAnalysis.obj"	"$(INTDIR)\BehaviorFeatureHistoryAnalysis.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\C45Learner.cpp

"$(INTDIR)\C45Learner.obj"	"$(INTDIR)\C45Learner.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ChevBar.cpp

"$(INTDIR)\ChevBar.obj"	"$(INTDIR)\ChevBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ChildFrm.cpp

"$(INTDIR)\ChildFrm.obj"	"$(INTDIR)\ChildFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\CleanAll.cpp

"$(INTDIR)\CleanAll.obj"	"$(INTDIR)\CleanAll.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\CollectorDlg.cpp

"$(INTDIR)\CollectorDlg.obj"	"$(INTDIR)\CollectorDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ComboBar.cpp

"$(INTDIR)\ComboBar.obj"	"$(INTDIR)\ComboBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Connection.cpp

"$(INTDIR)\Connection.obj"	"$(INTDIR)\Connection.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Corpus.cpp

"$(INTDIR)\Corpus.obj"	"$(INTDIR)\Corpus.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\CPARA.CPP

"$(INTDIR)\CPARA.OBJ"	"$(INTDIR)\CPARA.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Crypt.cpp

"$(INTDIR)\Crypt.obj"	"$(INTDIR)\Crypt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\CustomizeDialog.cpp

"$(INTDIR)\CustomizeDialog.obj"	"$(INTDIR)\CustomizeDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\CustomizeToolbar.cpp

"$(INTDIR)\CustomizeToolbar.obj"	"$(INTDIR)\CustomizeToolbar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Custsite.cpp

"$(INTDIR)\Custsite.obj"	"$(INTDIR)\Custsite.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\DataSet.cpp

"$(INTDIR)\DataSet.obj"	"$(INTDIR)\DataSet.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\DialogHeaderCtrl.cpp

"$(INTDIR)\DialogHeaderCtrl.obj"	"$(INTDIR)\DialogHeaderCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\DlgBrowser.cpp

"$(INTDIR)\DlgBrowser.obj"	"$(INTDIR)\DlgBrowser.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\DlgICPage.cpp

"$(INTDIR)\DlgICPage.obj"	"$(INTDIR)\DlgICPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\DlgPageProp.cpp

"$(INTDIR)\DlgPageProp.obj"	"$(INTDIR)\DlgPageProp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\DlgSuggest.cpp

"$(INTDIR)\DlgSuggest.obj"	"$(INTDIR)\DlgSuggest.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\editview.cpp

"$(INTDIR)\editview.obj"	"$(INTDIR)\editview.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\EnTabCtrl.cpp

"$(INTDIR)\EnTabCtrl.obj"	"$(INTDIR)\EnTabCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Evaluation.cpp

"$(INTDIR)\Evaluation.obj"	"$(INTDIR)\Evaluation.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ExternalUtilItem.cpp

"$(INTDIR)\ExternalUtilItem.obj"	"$(INTDIR)\ExternalUtilItem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ExternalUtils.cpp

"$(INTDIR)\ExternalUtils.obj"	"$(INTDIR)\ExternalUtils.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Extractor.cpp

"$(INTDIR)\Extractor.obj"	"$(INTDIR)\Extractor.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\FeatureExtraction.cpp

"$(INTDIR)\FeatureExtraction.obj"	"$(INTDIR)\FeatureExtraction.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\FixedHtmlView.cpp

"$(INTDIR)\FixedHtmlView.obj"	"$(INTDIR)\FixedHtmlView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\FontStatic.cpp

"$(INTDIR)\FontStatic.obj"	"$(INTDIR)\FontStatic.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Function.cpp

"$(INTDIR)\Function.obj"	"$(INTDIR)\Function.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\GenericHTTPClient.cpp

"$(INTDIR)\GenericHTTPClient.obj"	"$(INTDIR)\GenericHTTPClient.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\GlobalSetting.cpp

"$(INTDIR)\GlobalSetting.obj"	"$(INTDIR)\GlobalSetting.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Google.cpp

"$(INTDIR)\Google.obj"	"$(INTDIR)\Google.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Hash.cpp

"$(INTDIR)\Hash.obj"	"$(INTDIR)\Hash.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\hightime.cpp

"$(INTDIR)\hightime.obj"	"$(INTDIR)\hightime.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\HyperLink.cpp

"$(INTDIR)\HyperLink.obj"	"$(INTDIR)\HyperLink.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ICSession.cpp

"$(INTDIR)\ICSession.obj"	"$(INTDIR)\ICSession.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Idispimp.cpp

"$(INTDIR)\Idispimp.obj"	"$(INTDIR)\Idispimp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\IniFile.cpp

"$(INTDIR)\IniFile.obj"	"$(INTDIR)\IniFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\InputAlias.cpp

"$(INTDIR)\InputAlias.obj"	"$(INTDIR)\InputAlias.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\InputDlg.cpp

"$(INTDIR)\InputDlg.obj"	"$(INTDIR)\InputDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\InputPassword.cpp

"$(INTDIR)\InputPassword.obj"	"$(INTDIR)\InputPassword.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Instance.cpp

"$(INTDIR)\Instance.obj"	"$(INTDIR)\Instance.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ItemSet.cpp

"$(INTDIR)\ItemSet.obj"	"$(INTDIR)\ItemSet.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\LilacObject.cpp

"$(INTDIR)\LilacObject.obj"	"$(INTDIR)\LilacObject.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\LilacString.cpp

"$(INTDIR)\LilacString.obj"	"$(INTDIR)\LilacString.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\LinkBar.cpp

"$(INTDIR)\LinkBar.obj"	"$(INTDIR)\LinkBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\LinkListDlg.cpp

"$(INTDIR)\LinkListDlg.obj"	"$(INTDIR)\LinkListDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\List.cpp

"$(INTDIR)\List.obj"	"$(INTDIR)\List.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Logging.cpp

"$(INTDIR)\Logging.obj"	"$(INTDIR)\Logging.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\LogTrace.cpp

"$(INTDIR)\LogTrace.obj"	"$(INTDIR)\LogTrace.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Markup.cpp

"$(INTDIR)\Markup.obj"	"$(INTDIR)\Markup.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\MenuBar.cpp

"$(INTDIR)\MenuBar.obj"	"$(INTDIR)\MenuBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\MenuData.cpp

"$(INTDIR)\MenuData.obj"	"$(INTDIR)\MenuData.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Menuorder.cpp

"$(INTDIR)\Menuorder.obj"	"$(INTDIR)\Menuorder.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\WebMind.cpp

"$(INTDIR)\WebMind.obj"	"$(INTDIR)\WebMind.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\WebMind.idl

!IF  "$(CFG)" == "WebMind - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\WebMind.tlb" /h "WebMind_i.h" /iid "WebMind_i.c" /Oicf /win32 

"$(INTDIR)\WebMind.tlb"	".\WebMind_i.h"	".\WebMind_i.c" : $(SOURCE) "$(INTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "WebMind - Win32 Debug"

MTL_SWITCHES=/nologo /tlb "$(OUTDIR)\WebMind.tlb" /h "WebMind_i.h" /iid "WebMind_i.c" /Oicf /win32 

"$(INTDIR)\WebMind.tlb"	".\WebMind_i.h"	".\WebMind_i.c" : $(SOURCE) "$(INTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\WebMind.rc

"$(INTDIR)\WebMind.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\WebMindHelper.cpp

"$(INTDIR)\WebMindHelper.obj"	"$(INTDIR)\WebMindHelper.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\WebMindNSHandle.cpp

"$(INTDIR)\WebMindNSHandle.obj"	"$(INTDIR)\WebMindNSHandle.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\WebMindOptions.cpp

"$(INTDIR)\WebMindOptions.obj"	"$(INTDIR)\WebMindOptions.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\WebMindView.cpp

"$(INTDIR)\WebMindView.obj"	"$(INTDIR)\WebMindView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\NewDlgBar.cpp

"$(INTDIR)\NewDlgBar.obj"	"$(INTDIR)\NewDlgBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\PageCache.cpp

"$(INTDIR)\PageCache.obj"	"$(INTDIR)\PageCache.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\PageDocument.cpp

"$(INTDIR)\PageDocument.obj"	"$(INTDIR)\PageDocument.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\PageDocument2.cpp

"$(INTDIR)\PageDocument2.obj"	"$(INTDIR)\PageDocument2.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\PageEval.cpp

"$(INTDIR)\PageEval.obj"	"$(INTDIR)\PageEval.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\PageRecomm.cpp

"$(INTDIR)\PageRecomm.obj"	"$(INTDIR)\PageRecomm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\PageRepository.cpp

"$(INTDIR)\PageRepository.obj"	"$(INTDIR)\PageRepository.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\PageView.cpp

"$(INTDIR)\PageView.obj"	"$(INTDIR)\PageView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Parseit.cpp

"$(INTDIR)\Parseit.obj"	"$(INTDIR)\Parseit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\PopupConfirm.cpp

"$(INTDIR)\PopupConfirm.obj"	"$(INTDIR)\PopupConfirm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\PopupDlg.cpp

"$(INTDIR)\PopupDlg.obj"	"$(INTDIR)\PopupDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\PorterStemming.cpp

"$(INTDIR)\PorterStemming.obj"	"$(INTDIR)\PorterStemming.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ProgressSnakeCtrl.cpp

"$(INTDIR)\ProgressSnakeCtrl.obj"	"$(INTDIR)\ProgressSnakeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ProgressWnd.cpp

"$(INTDIR)\ProgressWnd.obj"	"$(INTDIR)\ProgressWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ProxyInfo.cpp

"$(INTDIR)\ProxyInfo.obj"	"$(INTDIR)\ProxyInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ProxyList.cpp

"$(INTDIR)\ProxyList.obj"	"$(INTDIR)\ProxyList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\PuncMark.cpp

"$(INTDIR)\PuncMark.obj"	"$(INTDIR)\PuncMark.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\QueryLearner.cpp

"$(INTDIR)\QueryLearner.obj"	"$(INTDIR)\QueryLearner.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\QuickSearchDlg.cpp

"$(INTDIR)\QuickSearchDlg.obj"	"$(INTDIR)\QuickSearchDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Recommendation.cpp

"$(INTDIR)\Recommendation.obj"	"$(INTDIR)\Recommendation.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SAPrefsDialog.cpp

"$(INTDIR)\SAPrefsDialog.obj"	"$(INTDIR)\SAPrefsDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SAPrefsSubDlg.cpp

"$(INTDIR)\SAPrefsSubDlg.obj"	"$(INTDIR)\SAPrefsSubDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SearchEngine.cpp

"$(INTDIR)\SearchEngine.obj"	"$(INTDIR)\SearchEngine.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SearchPage.cpp

"$(INTDIR)\SearchPage.obj"	"$(INTDIR)\SearchPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SelectLang.cpp

"$(INTDIR)\SelectLang.obj"	"$(INTDIR)\SelectLang.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SelectURL.cpp

"$(INTDIR)\SelectURL.obj"	"$(INTDIR)\SelectURL.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SelProxyCatDlg.cpp

"$(INTDIR)\SelProxyCatDlg.obj"	"$(INTDIR)\SelProxyCatDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SessionIdentification.cpp

"$(INTDIR)\SessionIdentification.obj"	"$(INTDIR)\SessionIdentification.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SetAliasDlg.cpp

"$(INTDIR)\SetAliasDlg.obj"	"$(INTDIR)\SetAliasDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SetFormDataDlg.cpp

"$(INTDIR)\SetFormDataDlg.obj"	"$(INTDIR)\SetFormDataDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SetProxy.cpp

"$(INTDIR)\SetProxy.obj"	"$(INTDIR)\SetProxy.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SetSCDlg.cpp

"$(INTDIR)\SetSCDlg.obj"	"$(INTDIR)\SetSCDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SimpleSAH.cpp

"$(INTDIR)\SimpleSAH.obj"	"$(INTDIR)\SimpleSAH.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SizableReBar.cpp

"$(INTDIR)\SizableReBar.obj"	"$(INTDIR)\SizableReBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\SortListCtrl.cpp

"$(INTDIR)\SortListCtrl.obj"	"$(INTDIR)\SortListCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\StaticHelper.cpp

"$(INTDIR)\StaticHelper.obj"	"$(INTDIR)\StaticHelper.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\StatusBarWithProgress.Cpp

"$(INTDIR)\StatusBarWithProgress.obj"	"$(INTDIR)\StatusBarWithProgress.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "WebMind - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\WebMind.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\WebMind.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "WebMind - Win32 Debug"

CPP_SWITCHES=/nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "_WINDOWS" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\WebMind.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\WebMind.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StopWord.cpp

"$(INTDIR)\StopWord.obj"	"$(INTDIR)\StopWord.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\TabCtrlSSL.cpp

"$(INTDIR)\TabCtrlSSL.obj"	"$(INTDIR)\TabCtrlSSL.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\TabDropTarget.cpp

"$(INTDIR)\TabDropTarget.obj"	"$(INTDIR)\TabDropTarget.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\TabPageSSL.cpp

"$(INTDIR)\TabPageSSL.obj"	"$(INTDIR)\TabPageSSL.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\TextProcessor.cpp

"$(INTDIR)\TextProcessor.obj"	"$(INTDIR)\TextProcessor.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\TextProgressCtrl.cpp

"$(INTDIR)\TextProgressCtrl.obj"	"$(INTDIR)\TextProgressCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ToolBarEx.cpp

"$(INTDIR)\ToolBarEx.obj"	"$(INTDIR)\ToolBarEx.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ToolMenu.cpp

"$(INTDIR)\ToolMenu.obj"	"$(INTDIR)\ToolMenu.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Url.cpp

"$(INTDIR)\Url.obj"	"$(INTDIR)\Url.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\UrlFilterProp.cpp

"$(INTDIR)\UrlFilterProp.obj"	"$(INTDIR)\UrlFilterProp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\UrlRecordEntity.cpp

"$(INTDIR)\UrlRecordEntity.obj"	"$(INTDIR)\UrlRecordEntity.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Utility.cpp

"$(INTDIR)\Utility.obj"	"$(INTDIR)\Utility.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\Utils.cpp

"$(INTDIR)\Utils.obj"	"$(INTDIR)\Utils.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ViewOnLine.cpp

"$(INTDIR)\ViewOnLine.obj"	"$(INTDIR)\ViewOnLine.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\webbrowser.cpp

"$(INTDIR)\webbrowser.obj"	"$(INTDIR)\webbrowser.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\WebICDoc.cpp

"$(INTDIR)\WebICDoc.obj"	"$(INTDIR)\WebICDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\WinAppEx.cpp

"$(INTDIR)\WinAppEx.obj"	"$(INTDIR)\WinAppEx.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\WinAppSingle.cpp

"$(INTDIR)\WinAppSingle.obj"	"$(INTDIR)\WinAppSingle.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\wizsheet.cpp

"$(INTDIR)\wizsheet.obj"	"$(INTDIR)\wizsheet.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\WordMapping.cpp

"$(INTDIR)\WordMapping.obj"	"$(INTDIR)\WordMapping.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\WordSearch.cpp

"$(INTDIR)\WordSearch.obj"	"$(INTDIR)\WordSearch.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\WordSeq.cpp

"$(INTDIR)\WordSeq.obj"	"$(INTDIR)\WordSeq.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\WordSession.cpp

"$(INTDIR)\WordSession.obj"	"$(INTDIR)\WordSession.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"


SOURCE=.\ChartCtrl\ChartAxis.cpp

"$(INTDIR)\ChartAxis.obj"	"$(INTDIR)\ChartAxis.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartAxisLabel.cpp

"$(INTDIR)\ChartAxisLabel.obj"	"$(INTDIR)\ChartAxisLabel.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartBarSerie.cpp

"$(INTDIR)\ChartBarSerie.obj"	"$(INTDIR)\ChartBarSerie.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartCandlestickSerie.cpp

"$(INTDIR)\ChartCandlestickSerie.obj"	"$(INTDIR)\ChartCandlestickSerie.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartCrossHairCursor.cpp

"$(INTDIR)\ChartCrossHairCursor.obj"	"$(INTDIR)\ChartCrossHairCursor.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartCtrl.cpp

"$(INTDIR)\ChartCtrl.obj"	"$(INTDIR)\ChartCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartCursor.cpp

"$(INTDIR)\ChartCursor.obj"	"$(INTDIR)\ChartCursor.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartDateTimeAxis.cpp

"$(INTDIR)\ChartDateTimeAxis.obj"	"$(INTDIR)\ChartDateTimeAxis.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartDragLineCursor.cpp

"$(INTDIR)\ChartDragLineCursor.obj"	"$(INTDIR)\ChartDragLineCursor.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartFont.cpp

"$(INTDIR)\ChartFont.obj"	"$(INTDIR)\ChartFont.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartGanttSerie.cpp

"$(INTDIR)\ChartGanttSerie.obj"	"$(INTDIR)\ChartGanttSerie.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartGradient.cpp

"$(INTDIR)\ChartGradient.obj"	"$(INTDIR)\ChartGradient.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartGrid.cpp

"$(INTDIR)\ChartGrid.obj"	"$(INTDIR)\ChartGrid.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartLegend.cpp

"$(INTDIR)\ChartLegend.obj"	"$(INTDIR)\ChartLegend.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartLineSerie.cpp

"$(INTDIR)\ChartLineSerie.obj"	"$(INTDIR)\ChartLineSerie.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartLogarithmicAxis.cpp

"$(INTDIR)\ChartLogarithmicAxis.obj"	"$(INTDIR)\ChartLogarithmicAxis.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartPointsSerie.cpp

"$(INTDIR)\ChartPointsSerie.obj"	"$(INTDIR)\ChartPointsSerie.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartScrollBar.cpp

"$(INTDIR)\ChartScrollBar.obj"	"$(INTDIR)\ChartScrollBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartSerie.cpp

"$(INTDIR)\ChartSerie.obj"	"$(INTDIR)\ChartSerie.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartStandardAxis.cpp

"$(INTDIR)\ChartStandardAxis.obj"	"$(INTDIR)\ChartStandardAxis.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartSurfaceSerie.cpp

"$(INTDIR)\ChartSurfaceSerie.obj"	"$(INTDIR)\ChartSurfaceSerie.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartTitle.cpp

"$(INTDIR)\ChartTitle.obj"	"$(INTDIR)\ChartTitle.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ChartCtrl\ChartXYSerie.cpp

"$(INTDIR)\ChartXYSerie.obj"	"$(INTDIR)\ChartXYSerie.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\WebFileOpenDlg.cpp

"$(INTDIR)\WebFileOpenDlg.obj"	"$(INTDIR)\WebFileOpenDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WebMind.pch"



!ENDIF 


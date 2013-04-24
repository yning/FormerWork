// WebMind.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "WebMind.h"
#include "HyperLink.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "WebMindView.h"

#include "afxpriv.h"
#include <..\src\occimpl.h>
#include "CustSite.h" 

//#include <afxsock.h>
#include <Winsock.h>
#include <afxmt.h>
#include <wininet.h>
//#include "rasspdm.h"
#include <initguid.h>
#include "WebMind_i.c"
#include "WebMindNSHandle.h"
//add new
#include "SelectURL.h"
#include "SelectLang.h"
#include "InputDlg.h"

//##############################################################
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebMindApp

BEGIN_MESSAGE_MAP(CWebMindApp, CWinApp)
	//{{AFX_MSG_MAP(CWebMindApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebMindApp construction

CWebMindApp::CWebMindApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWebMindApp object
CWebMindApp theApp;
CGlobalSetting theSettings;

	//********************HERE-add by ny**************************************
	inline void EnableMemLeakCheck()
	{
	 _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	}
    #ifdef _DEBUG
    #define new     new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #endif
	//********************TO HERE-add by ny**************************************

/////////////////////////////////////////////////////////////////////////////
// CWebMindApp initialization
BOOL CWebMindApp::InitInstance()
{
	EnableMemLeakCheck();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	//	SetUnhandledExceptionFilter(ExcepCallBack);
	//only one instance is allowed except -m
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		return TRUE;
	}

	HANDLE hSem = CreateSemaphore(NULL, 1, 1, m_pszAppName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		m_bAutoStart = FALSE;
	else
		m_bAutoStart = TRUE;

    HINSTANCE hInstance = AfxGetResourceHandle();
    ASSERT(hInstance != NULL);
	LPTSTR lpszModule;
	try
	{
		 lpszModule = new TCHAR[_MAX_PATH];
    }
	catch(...)
	{
		return FALSE;
	}

	if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
    {
		m_strProfile = lpszModule;
		int i = m_strProfile.ReverseFind('\\');
		m_strProfile= m_strProfile.Left(i+1);
		m_strRoot	= m_strProfile;
		m_strUser	= m_strProfile + "User\\";
		//
		m_strProfile		= m_strUser + "WebMind.ini";
		m_strFormDataPath	= m_strUser + "FormData.ini";
		//
		m_strSkinPath		= m_strRoot + "skin\\";
    }
    delete [] lpszModule;
	
	//First free the string allocated by MFC at CWinApp startup.
	//The string is allocated before InitInstance is called.
	free((void*)m_pszProfileName);
	//Change the name of the .INI file.
	//The CWinApp destructor will free the memory.
	m_pszProfileName=_tcsdup(m_strProfile);
	//delete[]  (char*)m_pszProfileName;
	//m_pszProfileName = m_strProfile;

	//must first create user path
	_CreateFullDir(m_strUser);
	
	HANDLE hfile ;
	CString filename;
	//
	filename = m_strProfile;
	hfile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hfile == INVALID_HANDLE_VALUE)
		CopyFile(m_strRoot+"Resource\\WebMind.ini.default",filename, FALSE);
	else
		CloseHandle(hfile);
	//
	filename = m_strUser+"SearchEngine.ini";
	hfile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hfile == INVALID_HANDLE_VALUE)
		CopyFile(m_strRoot+"Resource\\SearchEngine.ini.default",filename, FALSE);
	else
		CloseHandle(hfile);
	//
	filename = m_strUser+"Filter.ini";
	hfile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hfile == INVALID_HANDLE_VALUE)
		CopyFile(m_strRoot+"Resource\\Filter.ini.default",filename, FALSE);
	else
		CloseHandle(hfile);
	//
	g_bForbidMult = GetProfileInt("Settings", "ForbidMult", FALSE);

	//2 means first run,not select language
	m_bUseLngFile = GetProfileInt("Settings", "UseLngFile", 2);
	m_strLngFile = GetProfileString("Settings", "LngFile", "");
	if (m_bUseLngFile==2)
	{
		m_bUseLngFile = 0;
		WriteProfileInt("Settings", "UseLngFile", m_bUseLngFile );
		//
		CSelectLang dlg;
		dlg.DoModal();
	}
	else
	{
		m_strLngFile = theApp.m_strRoot+"Language\\" + m_strLngFile;
		if (m_strLngFile.GetLength()<1)
			m_bUseLngFile = FALSE;
		else
			_InitLanguageFile(m_strLngFile);
	}
	//
	if (cmdInfo.m_nShellCommand!=CCommandLineInfo::FileNew )
	{
		if (!m_bAutoStart)
		{
			CloseHandle(hSem);
			HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
			while (::IsWindow(hWndPrevious))
			{
				if (::GetProp(hWndPrevious, m_pszAppName))
				{
					if (cmdInfo.m_nShellCommand==CCommandLineInfo::FileOpen)
					{
						if (::GetLastActivePopup(hWndPrevious)==hWndPrevious)
						{
							LPSTR lpData;
							
							//mutex
							HANDLE hWebMindSem = CreateMutex(NULL, TRUE, "WebMindMutex");
							WaitForSingleObject(hWebMindSem, INFINITE);
							HANDLE hMapping = CreateFileMapping
								((HANDLE)0xFFFFFFFF,NULL,
								PAGE_READWRITE,0,2560,"MY2IESHARE");
							if (hMapping==NULL)
							{
								MSGBOX("App:Fail to create share memory!");
							}
							else
							{
								lpData = (LPSTR) MapViewOfFile(hMapping,FILE_MAP_ALL_ACCESS,0,0,0);
								if (lpData==NULL)
								{
									MSGBOX("App:MapViewOfFile Fail");
								}
								else
								{
									sprintf(lpData,"%s",cmdInfo.m_strFileName);
									::SendMessage(hWndPrevious,WM_USER_SHELL_OPEN,NULL,NULL);
								}
								CloseHandle(hMapping);
							}
							ReleaseMutex(hWebMindSem);
						}
					}
					if (::IsIconic(hWndPrevious))
						::ShowWindow(hWndPrevious,SW_RESTORE);
					::SetForegroundWindow(::GetLastActivePopup(hWndPrevious));
					return FALSE;
				}
				hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT);
			}
			return FALSE;
		}
	}
	else if(g_bForbidMult && !m_bAutoStart)
	{
		//only one instance
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
		while (::IsWindow(hWndPrevious))
		{
			if (::GetProp(hWndPrevious, m_pszAppName))
			{
				if (::IsWindowVisible(hWndPrevious)==FALSE)
					::SendMessage(hWndPrevious,WM_HOTKEY,NULL,NULL);
				else
				{
					if (::IsIconic(hWndPrevious))
						::ShowWindow(hWndPrevious,SW_RESTORE);
					::SetForegroundWindow(::GetLastActivePopup(hWndPrevious));
				}
				return FALSE;
			}
			hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT);
		}	
	}
	// initialize Winsock library
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(1, 1);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult == 0)
	{
		if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
		{
			WSACleanup();
		}
	}

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

#if _WIN32_WINNT >= 0x0400
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	CoInitialize(NULL);
#endif

	//*******************************by ny****************************************
	theSettings.m_appName = AfxGetAppName();
	theSettings.loadSettings();
	
	// when to built the release version, un-comment this paragraph

	// check whether we should upload the log files
	//checkingUpload();

	// we will also record WebMind's run time log file
	m_LogTrace.m_strAppName = "WebMind";
	m_LogTrace.SetFileName("Data\\Log.txt");

//	CString strLogFile;
//	strLogFile.Format("%s\\Log.txt", theSettings.m_dataPath);
//	m_LogTrace.SetFileName(strLogFile);

	m_LogTrace.OnStartup(TRUE, FALSE);
//	m_LogTrace.ResetFile(); // Omit this line if you don't want to overwrite the file each time
// *******************************by ny***********************************************



	if (!InitATL())
		return FALSE;

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.

	//get a random ID
    srand( (unsigned)time( NULL ) );
	g_nTrayIconID = rand()/16;

	CString strAbsDir;
	strAbsDir = GetProfileString("Settings", "SkinPath", NULL);
	if(!strAbsDir.IsEmpty())
	{
		DO_RELATIVE_URL(strAbsDir)
		m_strSkinPath = strAbsDir;
	}
	
	//IShellUIHandle
	CCustomOccManager *pMgr = new CCustomOccManager;
	// Create an IDispatch class for extending the Dynamic HTML Object Model 
	m_pDispOM = new CImpIDispatch;
	//Drop target
	m_pDropTarget = new CImpIDropTarget;

	// Set our control containment up but using our control container 
	// management class instead of MFC's default
	AfxEnableControlContainer(pMgr);

	// create main MDI Frame window
	CMainFrame* mf = new CMainFrame();
	if (!mf->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = mf;
	//global main frame point
	::pmf = mf;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	//############################################################################
	LoadConfg();

	int nOpenType;
	CString strURL;

	BOOL bGoHome = GetProfileInt("Settings", "StartupMode", 2);
	//disable the old menu
	::SetMenu(pmf->m_hWnd, NULL);
	pmf->ShowWindow(m_nCmdShow);//这一句开始WebMind的主窗口出来了 by ny
	//max wins
	BOOL run = 	GetProfileInt("Settings", "Running", 0);
	if(!m_bAutoStart)
		run = FALSE;
	g_bMax = GetProfileInt("Settings", "PageMax", 1);
	g_bMax = g_bMax>0 ? 1 : 0;
	//
	pmf->m_bKeepOnePage = GetProfileInt("Settings", "KeepOnePage", FALSE);
	if (pmf->m_bKeepOnePage)
	{
		if (bGoHome!=0 && bGoHome!=6)
			pmf->PostMessage(WM_COMMAND, ID_FILE_NEW_BLANK, 0);
	}
	//
	pmf->m_bAutoRunExternal = GetProfileInt("Settings", "AutoRunExternal", FALSE);
	pmf->m_bConfirmUnsafePage = GetProfileInt("Settings", "ConfirmUnsafePage", TRUE);
	pmf->m_dwDefaultProperty = GetProfileInt("Settings", "DefaultProperty", pmf->GetDefaultProperty());
	pmf->m_bUseTargetFrameName = GetProfileInt("Settings", "UseTargetFrameName", FALSE);



	//#####################################################################
	//!!anything before navigate add prev
	if (cmdInfo.m_nShellCommand==CCommandLineInfo::FileOpen)
	{
		nOpenType = 2;
		strURL = cmdInfo.m_strFileName;
		TCHAR           buf[2048];
		if(IS_FAVURL(strURL))
		{
			// an .URL file is formatted just like an .INI file, so we can
			// use GetPrivateProfileString() to get the information we want
			::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
									  _T(""), buf, 2048,
									  strURL);
			strURL =  buf;
		}
		pmf->NewChildWindow(1, nOpenType, strURL);
	}
	else
	{
		//enable go home
		nOpenType = bGoHome;
		if(bGoHome == 2|| run)// must be first for run
		{
			char tmp[INTERNET_MAX_PATH_LENGTH];
			if (run)
			{
				remove(m_strUser+"LastVisit.ini");
				rename(m_strUser+"Visiting.ini", m_strUser+"LastVisit.ini");
			}
			CString filename = m_strUser+"LastVisit.ini";
			DWORD r = ::GetPrivateProfileString("LastVisit", "url0", NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
			if(r)
			{
				CSelectURL dlg;
				dlg.DoModal();
			}
		}
		else if(bGoHome == 3)
		{
			CChildFrame *tcf;
			//load last visit
			char state[10]="state",url[8]="url"; // x+5
			CString filename = m_strUser+"LastVisit.ini";
			int i=0,nState;
			char tmp[INTERNET_MAX_PATH_LENGTH];
			BOOL r = TRUE;
			//
			BOOL bTemp = pmf->m_nNewSequence;
			pmf->m_nNewSequence = 0;
			while(r)
			{
				itoa(i, state+5, 10);
				itoa(i, url+3, 10);
				r = ::GetPrivateProfileString("LastVisit", url, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
				if (r)
				{
					tcf = pmf->NewChildWindow(1, 2, tmp);
					if (tcf)
					{
						nState = ::GetPrivateProfileInt("LastVisit", state, 0, filename);
						if (nState ==4)
							tcf->m_bLock = TRUE;
					}
				}
				i++;
			}
			pmf->m_nNewSequence  = bTemp;
		}
		else if(bGoHome == 0)
			pmf->PostMessage(WM_COMMAND, ID_FILE_NEW_BLANK, 0);
		else if(bGoHome == 1)//3.2
			pmf->PostMessage(WM_COMMAND, ID_FILE_NEW_S, 0);
		else if(bGoHome == 4)
		{
			pmf->OpenAllFavs(pmf->m_strStartFavFolder);
		}
		else if(bGoHome == 5)
			;//do nothing		
	}
	
	pmf->m_bAutoHideMode = GetProfileInt("Settings", "AutoHide", 0);
	if(pmf->m_bAutoHideMode)
	{
		pmf->AutoHideModeOn();
	}
	//full screen
	pmf->m_bFullScreenMode = GetProfileInt("Settings", "FullScreen", 0);
	if(pmf->m_bFullScreenMode)
	{
		pmf->FullScreenModeOn();
	}

	pmf->UpdateWindow();
	//delay load
	pmf->PostMessage(WM_DELAYLOAD_CONFIG);
	WriteProfileInt("Settings", "Running", 1);

	//############################################################################


	// *******************************by ny***********************************************************
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
//	SetRegistryKey(_T("WebMind-AICML"));

//	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
/*
	m_pBrowserViewTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWebICDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CWebMindView));
	AddDocTemplate(m_pBrowserViewTemplate);
*/
     //m_pEditorViewTemplate = new CMultiDocTemplate(
	//	IDR_WEBIC_EDITOR,
	//	RUNTIME_CLASS(CWebICDoc),
	//	RUNTIME_CLASS(CChildFrame), // custom MDI child frame
	//	RUNTIME_CLASS(CWebICEditorView));//comment by ny
	//AddDocTemplate(m_pEditorViewTemplate);

	// create main MDI Frame window

	//if (!pmf->LoadFrame(IDR_MAINFRAME))//by ny
	//	return FALSE;
//	m_pMainWnd = pmf;

//	m_pBrowserViewTemplate->m_hMenuShared=pmf->NewMenu();


	// Dispatch commands specified on the command line
//	if (!ProcessShellCommand(cmdInfo))
//		return FALSE;

	
	// The main window has been initialized, so show and update it.
	//pMainFrame->ShowWindow(m_nCmdShow, _T("FrameWndState"));
     //	pMainFrame->UpdateWindow();

	// The main window has been initialized, so show and update it.
	//pmf->ShowWindow(m_nCmdShow);//,"FrameWndState" by ny
/*	pmf->UpdateWindow();
	pmf->DragAcceptFiles();	
	CChildFrame *childFrame=(CChildFrame *)pmf->GetActiveFrame();
	if(childFrame)
	{
		CWebMindView *view=(CWebMindView *)childFrame->GetActiveView();
		if(view)
			view->GoHome();
	}

    // Enable DDE Execute open for single instance
    //EnableShellOpen();
    //RegisterShellFileTypes( TRUE );//by ny

	m_LogTrace.WriteLine("Application Initialization Done!\n");*/
// *******************************by ny***********************************************************

	return TRUE;
}

// App command to run the dialog
void CWebMindApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	//aboutDlg.DoModal();
	//   Create   and   show   the   dialog   box 
      int   nRet   =   -1; 
      nRet   =   aboutDlg.DoModal(); 
      //   Handle   the   return   value   from   DoModal 
      switch   (   nRet   ) 
      { 
      case   -1:   
            AfxMessageBox( "Dialog   box   could   not   be   created! "); 
            break; 
      case   IDABORT: 
            //   Do   something 
            break; 
      case   IDOK: 
            //   Do   something 
            break; 
      case   IDCANCEL: 
            //   Do   something 
            break; 
      default: 
            //   Do   something 
            break; 
      }; 
}

/////////////////////////////////////////////////////////////////////////////
// CWebMindApp message handlers

//#pragma optimize( "", on )

int CWebMindApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	try{
	WSACleanup();
	if(m_pDispOM)
		delete m_pDispOM;
	if(m_pDropTarget)
		delete m_pDropTarget;
	}catch(...){}

	if (m_bATLInited)
	{
		_Module.RevokeClassObjects();
		_Module.Term();
	}
	CoUninitialize();
	return CWinApp::ExitInstance();
}

void CWebMindApp::LoadConfg()
{
	char *tmp = NULL;

	//enable popup
	g_bKillPop = GetProfileInt("Settings", "KillPop", 1);
	g_bKillAutoWin = GetProfileInt("Settings", "KillAutoWin", 1);
	g_bKillDupWin = GetProfileInt("Settings", "KillDupWin", 1);
	g_bKillListWin = GetProfileInt("Settings", "KillListWin", 1);
	//
	pmf->m_bNotConfirmPopupFilter = GetProfileInt("Settings", "NotConfirmPopupFilter", 1);
	pmf->m_bPlayPopupFilterSound = GetProfileInt("Settings", "PlayPopupFilterSound", 1);
	pmf->m_nPopDelay = GetProfileInt("Settings", "PopupBound", 120);
	pmf->m_nPopDelay /= 100;
	pmf->m_bFilterRefresh = GetProfileInt("Settings", "FilterRefresh", FALSE);
	pmf->m_bNotFilterClickDialog = GetProfileInt("Settings", "NotFilterClickDialog", TRUE);
	//
	pmf->m_bUseUrlFilter = GetProfileInt("Settings", "UseUrlFilter", TRUE);
	pmf->m_bUseDialogFilter = GetProfileInt("Settings", "UseDialogFilter", TRUE);
	pmf->m_bUseFlyingFilter = GetProfileInt("Settings", "UseFlyingFilter", TRUE);
	if (pmf->m_bUseFlyingFilter)
		pmf->RefreshKillFlyingCode();
	//
	pmf->m_bKillBlank = GetProfileInt("Filter", "KillBlank", TRUE);
	pmf->m_strBlankContent = GetProfileString("Filter", "BlankContent", "<AD>");
	if (pmf->m_bKillBlank)
		pmf->RefreshKillBlankCode();
	//
	pmf->m_bReloadUnShownImage = GetProfileInt("Settings", "ReloadUnShownImage", TRUE);
	//
	pmf->m_nEnableFilter = GetProfileInt("Settings", "EnableFilter", 0xF);
	//window pos
	int top = GetProfileInt("Settings", "top", 50);
	int left = GetProfileInt("Settings", "left", 50);
	int width = GetProfileInt("Settings", "width", 700);
	int height = GetProfileInt("Settings", "height", 500);
	if( width!=0 && top<2000 && top>=-20)
		m_pMainWnd->SetWindowPos(&CWnd::wndTop, left, top, width, height,SWP_HIDEWINDOW);   
	if( GetProfileInt("Settings", "Zoomed", 1))
		m_nCmdShow = SW_SHOWMAXIMIZED;

	pmf->m_nTitleLen = GetProfileInt("Settings", "TabSize", 20);

	//load defaul proxy
	pmf->GetIeProxy();
	pmf->m_bEnProxy = GetProfileInt("Settings", "EnableProxy", 0);
	pmf->m_strCurProxy = GetProfileString("Settings", "CurrentProxy", NULL);
	pmf->m_strCurProxyName = GetProfileString("Settings", "CurrentProxyName", NULL);//pmf->GetProxyName(pmf->m_strCurProxy);
	pmf->m_strProxyByPass = GetProfileString("Settings", "ProxyByPass", "<local>");
	pmf->UpdateProxy();
	
	//load defaul web proxy
	pmf->m_strDefaultWebProxy = GetProfileString("Settings", "DefaultWebProxy", NULL);
	//active new window
	pmf->m_bActiveNewWindow = GetProfileInt("Settings", "ActiveNew", 0);
	//
	pmf->m_bOpenFolderOut = GetProfileInt("Settings", "OpenFolderOut", TRUE);
	//the start favorite folder
	pmf->m_strStartFavFolder = GetProfileString("Settings", "StartFolder", NULL);
	//insert to next tab
	pmf->m_nNewSequence = GetProfileInt("Settings", "NewSequence", 1);
	
	//autosizing tab
	pmf->m_bAutoTab = GetProfileInt("Settings", "AutoTab", 1);

	//max autosizing tab width
	pmf->m_nMaxTabWidth = GetProfileInt("Settings", "MaxTabWidth", 100);
	//min autosizing tab width
	pmf->m_nMinTabWidth = GetProfileInt("Settings", "MinTabWidth", 50);
	//
	g_bShowIndex = GetProfileInt("Settings","ShowIndex", TRUE);
	g_bShowIndex = GetProfileInt("Settings","ShowIndex", TRUE);
	g_bShowDot = GetProfileInt("Settings","ShowDot", TRUE);
	g_bShowRead = GetProfileInt("Settings","ShowRead", TRUE);
	g_bShowFavIcon = GetProfileInt("Settings","ShowFavIcon", TRUE);

	//silent
	g_bSilent = GetProfileInt("Settings", "Silent", TRUE);
	if (g_bSilent != (_RegGetString(HKEY_CURRENT_USER, 
		"Software\\Microsoft\\Internet Explorer\\Main",
		"Disable Script Debugger") == "yes" ? TRUE : FALSE))
	{
		if (g_bSilent)
			_RegSetString(HKEY_CURRENT_USER, 
				"Software\\Microsoft\\Internet Explorer\\Main",
				"Disable Script Debugger",
				"yes");
		else
			_RegSetString(HKEY_CURRENT_USER, 
				"Software\\Microsoft\\Internet Explorer\\Main",
				"Disable Script Debugger",
				"no");
	}
	//default font size
	g_nDefFontSize = GetProfileInt("Settings", "DefFontSize", 2);
	//disable scroll text on status bar
	g_bDisScrollText = GetProfileInt("Settings", "DisScrollText", FALSE);

	//filte blank popup window from same parent
	g_bFilteBlank = GetProfileInt("Settings", "FilteBlank", FALSE);

	//animate icon
	pmf->m_bAnimatIcon = GetProfileInt("Settings", "AnimateIcon", TRUE);

	//lock homepage
	g_bLockHomepage = GetProfileInt("Settings", "LockHomepage", FALSE);

	pmf->m_bDisDownActivex = GetProfileInt("Settings", "DisDownActivex", TRUE);

	//down
	pmf->m_nDownAll = GetProfileInt("Settings", "DownAll", 0);
	//
	pmf->m_nCollectorTrans = GetProfileInt("Settings", "CollectorTrans", 255);
}

void CWebMindApp::SaveConfg()
{
	try{
	//proxy
	if(g_bProxyDirty)
		pmf->m_aProxyCategories.SaveProxies();
	g_bProxyDirty = FALSE;

	//pop filter
	WriteProfileInt("Settings", "KillPop", g_bKillPop);
	WriteProfileInt("Settings", "KillAutoWin", g_bKillAutoWin);
	WriteProfileInt("Settings", "KillDupWin", g_bKillDupWin);
	WriteProfileInt("Settings", "KillListWin", g_bKillListWin);
	//content filter
	WriteProfileInt("Settings", "UseUrlFilter", pmf->m_bUseUrlFilter);
	WriteProfileInt("Settings", "UseFlyingFilter", pmf->m_bUseFlyingFilter);
	WriteProfileInt("Settings", "UseDialogFilter", pmf->m_bUseDialogFilter);
	WriteProfileInt("Settings", "EnableFilter", pmf->m_nEnableFilter);
	//
	WriteProfileInt("Settings", "DisDownActivex", pmf->m_bDisDownActivex);
	//else
	WriteProfileInt("Settings", "TabSize", pmf->m_nTitleLen);

	BYTE *tmp = NULL;
	int i = 1;

	//save defaul web proxy
	WriteProfileString("Settings","DefaultWebProxy", pmf->m_strDefaultWebProxy);
	
	//save defaul  proxy
	WriteProfileString("Settings","CurrentProxy", pmf->m_strCurProxy);
	WriteProfileString("Settings","CurrentProxyName", pmf->m_strCurProxyName);
	WriteProfileString("Settings","ProxyByPass", pmf->m_strProxyByPass);

	//active new
	WriteProfileInt("Settings", "ActiveNew", pmf->m_bActiveNewWindow);
	WriteProfileInt("Settings", "LinkInNewWindow", g_bLinkInNewWindow);
	//start favorite folder
	WriteProfileString("Settings","StartFolder", pmf->m_strStartFavFolder);

	//multiline tab
	WriteProfileInt("Settings", "MultiLineTab", pmf->m_bMultiLineTab);

	//insert to next tab
	WriteProfileInt("Settings", "NewSequence", pmf->m_nNewSequence);

	//Background PIC
	WriteProfileString("Settings", "SkinName", pmf->m_strSkinName);

	//text label
	WriteProfileInt("Settings", "TextLabel", pmf->m_nTextLabel);
	//text label
	WriteProfileInt("Settings", "ColorIcon", pmf->m_bColorIcon);

	//autosizing tab
	WriteProfileInt("Settings","AutoTab", pmf->m_bAutoTab);

	//max autosizing tab width
	WriteProfileInt("Settings","MaxTabWidth", pmf->m_nMaxTabWidth);
	//min autosizing tab width
	WriteProfileInt("Settings","MinTabWidth", pmf->m_nMinTabWidth);
	
	//use shortcut
	WriteProfileInt("Settings","UseSC", pmf->m_bUseSC);

	//use alias
	WriteProfileInt("Settings","UseAlias", pmf->m_bUseAlias);

	//defaut font size
	WriteProfileInt("Settings", "DefFontSize", g_nDefFontSize);
	//!disable scroll text on statusbar
	WriteProfileInt("Settings", "DisScrollText", g_bDisScrollText);

	//toolbar icon
	WriteProfileInt("Settings", "SmallToolbar", pmf->m_bSmallToolBar);

	WriteProfileInt("Settings", "AnimateIcon", pmf->m_bAnimatIcon);
	WriteProfileInt("Settings", "LockHomepage", g_bLockHomepage);
	//lock toolbar
	WriteProfileInt("Settings", "LockToolbar", pmf->m_bLockToolbar);

	// search key
	pmf->SaveSearchKey();
	//
	WriteProfileInt("Settings", "DefaultCharSet", pmf->m_nDefaultCharSet);
	//
	WriteProfileInt("Settings", "DirectOpenFile", pmf->m_bDirectOpenFile);
	WriteProfileInt("Settings", "OpenInNew", pmf->m_bOpenInNew);
	//
	WriteProfileInt("Settings", "DisableBackground", pmf->m_bDisableBackground);
	//
	WriteProfileInt("Search", "SearchMatchCase", pmf->m_bSearchMatchCase);
	WriteProfileInt("Search", "SearchMatchWhole", pmf->m_bSearchMatchWhole);
	WriteProfileInt("Search", "NotSaveKeyword", pmf->m_bNotSaveKeyword);
	//
	WriteProfileInt("Settings", "PageMax", g_bMax);
	WriteProfileInt("Settings", "AutoRunExternal", pmf->m_bAutoRunExternal);

	//must place here, for that this can be changed in status bar
	WriteProfileInt("Mouse", "RMouseDrag", pmf->m_bRMouseDrag );
	WriteProfileInt("Mouse", "RMouseGesture", pmf->m_bRMouseGesture );

	}catch(...){}
}

BOOL CWebMindApp::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class
	try{
	if(pmf!=NULL)
	{
		if( pmf->m_bFavDirty && m_nDelay>30 )
		{
			pmf->PostMessage(WM_UPDATE_FAV,pmf->m_bFavDirty,0);
			pmf->m_bFavDirty = FALSE;
			m_nDelay=0;
		}
		else if(pmf->m_bFavDirty)
			m_nDelay++;
		//auto start
		if(m_bAutoStart)
		{
			static nAutoStartCount=0;
			int max = pmf->m_ExternalUtilList.m_UtilList.GetUpperBound();
			if(nAutoStartCount <= max)
			{
				while(nAutoStartCount <= max && pmf->m_ExternalUtilList.m_UtilList.GetAt(nAutoStartCount)->m_bUtilStart==FALSE)
				{
					nAutoStartCount++;
				}
				if(nAutoStartCount <= max)
				{
					//start the tool
					pmf->StartUtil(nAutoStartCount);
					nAutoStartCount++;
				}
				else
					m_bAutoStart = FALSE;
			}
			else
				m_bAutoStart = FALSE;
		}
		if(pmf->m_bSaveConfig)
		{
			pmf->m_bSaveConfig = FALSE;
			AfxBeginThread(TSaveConfig, NULL);
		}
	}
	//
	if (lCount <= 0)
	{
		ThreadOnIdle(lCount);
	}
	else if (lCount == 1)
	{
		ThreadOnIdle(lCount);
	}
	return lCount < 1;  // more to do if lCount < 1

	}
	catch(...)
	{
		return lCount < 1;
	}
}

BOOL CWebMindApp::ThreadOnIdle(LONG lCount)
{
	ASSERT_VALID(this);

	try{
	if (lCount <= 0)
	{
	}
	else if (lCount >= 0)
	{
		AFX_MODULE_THREAD_STATE* pState = _AFX_CMDTARGET_GETSTATE()->m_thread;
		if (pState->m_nTempMapLock == 0)
		{
			// free temp maps, OLE DLLs, etc.
			AfxLockTempMaps();
			AfxUnlockTempMaps();
		}
	}
	}catch(...){}

	return lCount < 0;  // nothing more to do if lCount >= 0
}

//#######################################################################################
CWebMindModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_WebMindNSHandle, CWebMindNSHandle)
END_OBJECT_MAP()

LONG CWebMindModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CWebMindModule::Lock()
{
	AfxOleLockApp();
	return 1;
}
LPCTSTR CWebMindModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}

BOOL CWebMindApp::InitATL()
{
	m_bATLInited = TRUE;
	_Module.Init(ObjectMap, AfxGetInstanceHandle());
	_Module.dwThreadID = GetCurrentThreadId();

	LPTSTR lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT
	TCHAR szTokens[] = _T("-/");

	BOOL bRun = TRUE;
	LPCTSTR lpszToken = _Module.FindOneOf(lpCmdLine, szTokens);
	while (lpszToken != NULL)
	{
		if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_WebMind, FALSE);
			_Module.UnregisterServer(TRUE); //TRUE means typelib is unreg'd
			bRun = FALSE;
			break;
		}
		if (lstrcmpi(lpszToken, _T("RegServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_WebMind, TRUE);
			_Module.RegisterServer(TRUE);
			bRun = FALSE;
			break;
		}
		lpszToken = _Module.FindOneOf(lpszToken, szTokens);
	}

	if (!bRun)
	{
		m_bATLInited = FALSE;
		_Module.Term();
		return FALSE;
	}

	HRESULT hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
		REGCLS_MULTIPLEUSE);
	if (FAILED(hRes))
	{
		m_bATLInited = FALSE;
		return FALSE;
	}	

	return TRUE;
}

BOOL CWebMindApp::IsDonated()
{
	CString strKey = GetProfileString("Register", "YourName", NULL);
	if (strKey.GetLength())
		return TRUE;
	else
		return FALSE;
}

//#pragma optimize( "s", on )

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_DONATE, OnDonate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	LOADDLG("DialogAbout");
	CDialog::OnInitDialog();

	CString url("http://wsi.gucas.ac.cn/index.php/Main_Page");  
    m_Link.SubclassDlgItem(IDC_HYPERLINK,this); // IDC_STATIC_URL是相应CStatic控件的资源号。  
    m_Link.SetURL(url); //超链接文本  

	// TODO: Add extra initialization here
	CWnd *p;
	CString strAbout;
	CFile f;
	CString filename = theApp.m_strRoot + "thanks.txt";
	if(f.Open(filename, CFile::modeRead|CFile::shareDenyNone))
	{
		DWORD len = f.GetLength();
		char* tmp = new char[len+1];
		f.Read(tmp, len);
		tmp[len]='\0';
		strAbout = tmp;
		delete[] tmp;
		f.Close();
	}	
	p= GetDlgItem(IDC_THANKS);
	p->SetWindowText(strAbout);
	//
	if (theApp.IsDonated())
	{
		LOADSTR(strAbout ,IDS_DONATED);
		CString strName;
		strName = theApp.GetProfileString("Register", "YourName", NULL);
		strAbout += strName;
		//
		p= GetDlgItem(IDC_DONATE);
		p->ShowWindow(SW_HIDE);
	}
	else
	{
		LOADSTR(strAbout ,IDS_UNDONATED);
	}
	p= GetDlgItem(IDC_INFO);
	p->SetWindowText(strAbout);

	if (theApp.m_bUseLngFile)
	{
		p= GetDlgItem(IDC_ABOUT);
		p->GetWindowText(strAbout);
		p->SetWindowText(strAbout);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnDonate() 
{
	// TODO: Add your control notification handler code here
	int ret = MSGBOX(IDS_STR_DONATE, MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ret==IDYES)
	{
		pmf->NewChildWindow(1,2,"http://wsi.gucas.ac.cn/index.php/Contact-us");
		CDialog::OnOK();
	}
	else if(ret== IDNO)
	{
		CInputDlg dlg;
		CString strMsg;
		LOADSTR(strMsg ,IDS_YOUR_NAME);
		dlg.m_strMsg = strMsg;
		if(dlg.DoModal() == IDOK && dlg.m_strOutput.GetLength())
		{
			theApp.WriteProfileString("Register", "YourName", dlg.m_strOutput);
			//
			CWnd *p;
			LOADSTR(strMsg ,IDS_DONATED);
			strMsg += dlg.m_strOutput;
			p= GetDlgItem(IDC_INFO);
			p->SetWindowText(strMsg);
			//
			p= GetDlgItem(IDC_DONATE);
			p->ShowWindow(SW_HIDE);
			//
			MSGBOX(IDS_THANK_DONATE);
		}
	}
}

CView * CWebMindApp::CreateOrActivateBrowserView(BOOL beVisible)
{
	try
	{
		CDocument *pDoc = m_pBrowserViewTemplate->OpenDocumentFile(NULL,FALSE);
		POSITION pos = pDoc->GetFirstViewPosition();
		if(pos != NULL)
		{
			CView* pView = pDoc->GetNextView(pos);
			if(pView && beVisible)
				pView->GetParentFrame()->ShowWindow(SW_SHOW);

//			pView->GetParentFrame()->SetActiveView(pView);

			return pView;
		}   

		/*
		POSITION pos = GetFirstDocTemplatePosition();
		if(pos)
		{
			CDocTemplate * pTemplate = GetNextDocTemplate(pos);
			while(pos &&!pTemplate->IsKindOf(RUNTIME_CLASS(CWebICDoc)))
				pTemplate = GetNextDocTemplate(pos);
			if(pTemplate)
			{
				CDocument *pDoc=pTemplate->OpenDocumentFile(NULL,FALSE);
				POSITION pos = pDoc->GetFirstViewPosition();
				if(pos != NULL)
				{
					CView* pView = pDoc->GetNextView(pos);
					if(pView)
						pView->GetParentFrame()->ShowWindow(SW_SHOW);

					return pView;
				}   
			}
		}
		*/
	}
	catch(...)
	{
	}
	return NULL;
}
//by ny
//#include "DlgUpload.h"
void CWebMindApp::checkingUpload()
{
	if(theSettings.m_numUpload == -1)
		return;

    CHighTime ct;
	CHighTimeSpan tspan;

	ct = CHighTime::GetPresentTime();
	tspan = ct - theSettings.m_lastUpload;
	if(tspan.GetStatus() == 0)
	{
		if(tspan.GetTotalDays() >= 7L)//by ny
		{
			// for weekly upload
			CStringArray m_logWebIC;
			//CFileOperation fileOperator;//by ny

		//fileOperator.GetLILACLog(theSettings.m_dataPath, m_logWebIC);//by ny
			if(m_logWebIC.GetSize() > 0)
			{
			//	CDlgUpload dlgUpload;//by ny
				//dlgUpload.DoModal();//by ny

				theSettings.CheckIntegrity();
			}
		}
	}
}

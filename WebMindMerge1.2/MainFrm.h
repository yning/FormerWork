// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__19E497C1_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
#define AFX_MAINFRM_H__19E497C1_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//########################################################
#include "StatusBarWithProgress.h"
#include "WebMindView.h"
#include "ChildFrm.h"

#include "ChevBar.h"
#include "MenuBar.h"
#include "AddressBarEx.h"
#include "TabDropTarget.h"
#include "ProxyList.h"
#include "ExternalUtilItem.h"
#include "AnimateIcon.h"	// Added by ClassView
#include "AdvTabCtrl.h"
//add new
#include "ComboBar.h"
#include "macro.h"
#include "function.h"
//by ny
#include "Dictionary.h"
#include "Segment.h"
#include "ViewManager.h"
#include "ComboBar.h"
#include "macro.h"
#include "function.h"
#include "PageRepository.h"
#include "PageView.h"
#include "Extractor.h"
#include "LinkBar.h"
#include "C45Learner.h"
#include "QueryLearner.h"
#include "GlobalSetting.h"
//#include "WindowManager.h"

//add ChartCtrl
#include "ChartCtrl/ChartCtrl.h"

//########################################################
//Define the CommandTarget ids
#define HTMLID_FIND 1
#define HTMLID_VIEWSOURCE 2
#define HTMLID_OPTIONS 3 

//toolbar id
#define ID_TOOLBAR_ADDRESS	AFX_IDW_TOOLBAR+1
#define ID_TOOLBAR_TAB		AFX_IDW_TOOLBAR+2
#define ID_TOOLBAR_SEARCH	AFX_IDW_TOOLBAR+3
#define ID_TOOLBAR_STATUS	AFX_IDW_TOOLBAR+4

//some else put here
#define WSM_TRAY			WM_USER+3
#define BORDER				2
#define DEFAULT_PROPERTY	0xFFFFFFFF
#define MAX_UNDO			24	//0x1a
#define MAX_HISTORY			10

//##############################################################
//window list
#define B_WINDOW		0xC00	//512
#define E_WINDOW		0xDFF
//external tool
#define B_EX_TOOL		0xE00	//256
#define E_EX_TOOL		0xEFF
//skin
#define B_SKIN			0xF00	//256
#define E_SKIN			0xFFF
//open all fav
#define B_OPEN_FAV		0x1000	//256
#define E_OPEN_FAV		0x10FF
//translation
#define B_TRANS			0x1100	//256
#define E_TRANS			0x11FF
//web proxy
#define B_WEB_PROXY		0x1200	//256
#define E_WEB_PROXY		0x12FF
//open all fav most
#define B_OPEN_FAV_MOST	0x1300	//256
#define E_OPEN_FAV_MOST	0x13FF
//open fav item
#define B_FAV			0x1400	//19455
#define E_FAV			0x5FFF
//undo close
#define B_UNDO			0x6200	//256
#define E_UNDO			0x62FF
//addto this fav
#define B_ADDTO_FAV		0x6300	//256
#define E_ADDTO_FAV		0x63FF
//proxy server
#define B_PROXY			0x6400	//512
#define E_PROXY			0x65FF
//search item
#define B_SEARCH		0x7600	//512
#define E_SEARCH		0x77FF
//search item for drag menu
#define B_SEARCH_DRAG	0x7800	//512
#define E_SEARCH_DRAG	0x79FF
//go up
#define B_GO_UP			0x7A00	//256
#define E_GO_UP			0x7AFF
//sort save
//#define B_SORT_SAVE		0x7B00	//256
//#define E_SORT_SAVE		0x7BFF
//go back forward history
#define B_HISTORY		0x7C00	//256
#define E_HISTORY		0x7CFF

//not max 0x8000 !!!!
//for that: #define ID_FILE_NEW_BLANK   32772

//########################################################
//open in new window active option
#define INW_NORMAL		0x1
#define INW_ACTIVE		0x2
#define INW_INACTIVE	0x4
#define INW_OTHER_SIDE	0x8	//open to other side

//right click option
#define RC_NOT_FILTER	2

//########################################################
//tab icon
#define TI_NO		-1	//no icon
#define TI_0		0	//blank page
#define TI_1		1	//progress 1	0   ~ 33%
#define TI_2		2	//progress 2	33% ~ 66%
#define TI_3		3	//progress 3	66% ~ 99%
#define TI_LOCK		4
#define TI_100		5	//100%
#define TI_PROTECT	6
#define TI_SAVE		7
#define TI_CLOSE	8
#define TI_READ		9
#define TI_MARK		10

#define TI_MAX		TI_MARK
#define TI_COUNT	11

#define IS_TABICON(nIcon)	(nIcon<TI_COUNT)
//#define IS_FAVICON(nIcon)	(nIcon>TI_MAX)
#define  IS_FAVICON(nIcon) (nIcon>3)//��ô��������Ϊ����Ϊ�ĸı���ԭ�����Ǹ�λͼ�ṹ�����ڹ̶�λͼֻ���ĸ������ֻ�����3�������Ϊ����վͼ��

//########################################################
//work threads
UINT _cdecl SendDocuments(LPVOID file);
typedef struct stagDocuments
{
	char filename[256];
	BOOL IsLocalFile;
}sDocuments;
UINT _cdecl QuickSaveDocuments(LPVOID savefiles);
typedef struct stagSaveFiles
{
	CString base;
	CString filename;
	CMapStringToString mapDLFiles;
	CString root;
}sSaveFiles;
UINT _cdecl TSaveConfig(LPVOID con);

//########################################################
class CCollectorDlg;
class CLinkListDlg;
class CMonitor;
class CMainFrame;

//########################################################
extern CMainFrame* pmf;

extern CStringArray g_astrLinks;
extern CStringArray g_astrFavs;

extern const UINT MSWHELL_ROLLMSG;
extern char	 g_szFile[1024];
extern char* g_strLngBuf;
extern BSTR  g_bstrYes;

extern int g_nPercent;
extern int g_bMax;
extern int g_nNextTab;
extern int g_nDefFontSize;
extern int g_nTrayIconID;

extern BOOL g_bSilent;
extern BOOL g_bDisScrollText;
extern BOOL g_bFilteBlank;
extern BOOL g_bLinkInNewWindow, g_bFileInNewWindow, g_bFavInNewWindow, g_bHisInNewWindow;
extern BOOL g_bConfirmCloseLock;
extern BOOL g_bForbidMult;
extern BOOL g_bVerbose;
extern BOOL g_bLockHomepage;
extern BOOL g_bProxyDirty;
extern BOOL g_bShowIndex;
extern BOOL g_bClickClose;
extern BOOL g_bShowDot;
extern BOOL g_bShowRead;
extern BOOL g_bShowFavIcon;
extern BOOL g_bRTab;
extern BOOL	g_bKillPop,g_bKillAutoWin,g_bKillListWin,g_bKillDupWin;
extern 	CDictionary *pDict; //�ʵ���
extern	CSegment seg;//�ִ���
//########################################################
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	BOOL	m_bTopTab;
	BOOL	m_bReloadUnShownImage;
	BOOL	m_bKillBlank;
	BOOL	m_bNotFilterClickDialog;
	BOOL	m_bUseTargetFrameName;
	BOOL	m_bSimuDragLink;
	BOOL	m_bUseIeKey;
	BOOL	m_bFilterRefresh;
	BOOL	m_bShowGesture;
	BOOL	m_bNotSaveKeyword;
	BOOL	m_bNameByTitle;
	BOOL	m_bSavePageByDate;
	BOOL	m_bActiveNewAddress;
	BOOL	m_bBlankMonitorClip;
	BOOL	m_nTabStyle;
	BOOL	m_bTabDefaultColor;
	BOOL	m_bConfirmUnsafePage;
	BOOL	m_bShowPageReadedIcon;
	BOOL	m_bAutoRunExternal;
	BOOL	m_bKeepOnePage;
	BOOL	m_bShowScriptResult;
	BOOL	m_bSearchMatchCase;
	BOOL	m_bSearchMatchWhole;
	BOOL	m_bOutputAutoClean;
	BOOL	m_bOutputSaveFilter;
	BOOL	m_bOutputSaveUrlFilter;
	BOOL	m_bOutputSaveImg;
	BOOL	m_bOutputSavePage;
	BOOL	m_bOutputSaveText;
	BOOL	m_bOutputSaveTime;
	BOOL	m_bCollectorAddtoBottom;
	BOOL	m_bForbidMainTitle;
	BOOL	m_bNotShowRightMenu;
	BOOL	m_bEncryptStringItem;
	BOOL	m_bCloseCleanCache;
	BOOL	m_bCloseCleanCookies;
	BOOL	m_bCloseCleanFormData;
	BOOL	m_bCloseCleanHistory;
	BOOL	m_bCloseCleanTypedAddress;
	BOOL	m_bCloseCleanTypedSearch;
	BOOL	m_bCloseCleanUndoClose;
	BOOL	m_bDisableBackground;
	BOOL	m_bDirectOpenFile;
	BOOL	m_bOpenInNew;
	BOOL	m_bFavShowUrl;
	BOOL	m_bComboShowIcon;
	BOOL	m_bGoProxyMenu;
	BOOL	m_bMenuLoadLastClose;
	BOOL	m_bNotConfirmPopupFilter;
	BOOL	m_bPlayPopupFilterSound;
	BOOL	m_bShowToolbarLabel,m_bShowAddressBarButton;
	BOOL	m_bShowSearchBarButton;
	BOOL	m_bAutoPopWin;
	BOOL	m_bWheelTab,m_bWheelOnTab;
	BOOL	m_bUseDefaultFavPath;
	BOOL	m_bUseDefaultLinkPath;
	BOOL	m_bMinimizeToTray, m_bCloseToTray;
	BOOL	m_bShowTrayIcon;
	BOOL	m_bCloseAllWhenCloseToTray;
	BOOL	m_bOpenFolderOut;
	BOOL	m_bActiveFileNew;
	BOOL	m_bUrlFilterDirty;
	BOOL	m_bIsAutoSave;

	int		m_nLDbClickTabBlank;
	int		m_nRDbClickTabBlank;
	int		m_nFtpOpenType;//0:self,1:explorer,2:else
	int		m_nCleanAllMode;
	int		m_nManagerIndex;//download manager name
	int		m_nSearchBarAction;
	int		m_nPopFiltered;
	int		m_nCollectorTrans;
	
	DWORD	m_dwFsShow;//full screen
	DWORD	m_dwAhShow;//auto hide
	DWORD	m_dwDefaultProperty;
	POINT	m_ptMouseDown;//last mouse down point
	CTime	m_tStartTime;
	double	m_nPopDelay;

	COLORREF m_crUnSelColor;
	COLORREF m_crUnSelFontColor;
	COLORREF m_crSelFontColor;
	COLORREF m_crSepColor;

	CString m_strBlankContent;
	CString m_strLastNavUrl;
	CString m_strFtpExePath;//ftp url open application path
	CString m_strFormMainPwd;
	CString m_strManagerScript;//download manager script
	CString m_strManagerType;//download manager type
	CString	m_strStatusStringFormat;
	CString m_strAutoSavePath;
	CString m_strHomePage;
	CString	m_strFavPath;
	CString	m_strLinkPath;

	CStringList  m_astrTypedSearch;
	CStringList  m_astrFavoriteFolders;
	CStringList  m_astrPopup;
	CStringList  m_astrUnPopup;

	CStringArray m_astrUrlFilter;
	CStringArray m_astrUrlUnFilter;
	//***********************************************************************
	//by ny
	// decide which model we will use to generate
	// recommendation
	// 0 : Association Rule
	// 1 : IC-word (Naive Bayes)
	// 2 : IC-word (C4.5)
	// 3 : IC-query
	int m_modelSelector;
	// Presentation Policy
	// 0 : top one
	// 1 : randomly pick one
	// 2 : all
	int m_presentationPolicy;
	// Timer ID
	int m_nTimer;
	// the period time in minute to trigue the timer
	int m_nPeriodMinute; // the period in minutes for each tick
	int m_nTickerCheckLatestWebIC;
	int m_nTickerReleaseMemory;
	BOOL m_beVisibleNewWin;
	CViewManager m_wndViewManager;
	//***********************************************************************
// Operations
public:
	BOOL	DoSpecialUrl(CString str);
	BOOL	PromptMenuCommand(UINT nID);
	BOOL	NeedUrlFilter(CString strUrl);
	BOOL	NeedUnUrlFilter(CString strUrl);
	BOOL	UrlFilterStar(CStringArray &pattern, CString url);
	BOOL	NeedPopFilter(CString strUrl);
	BOOL	NeedUnPopFilter(CString strUrl);
	BOOL	PopFilterStar(CStringList &astr, CString url);
	BOOL	DoKeyMsg(MSG* pMsg);
	BOOL	DoMouseMsg(MSG* pMsg);
	BOOL	ActiveViewSetFocus();
	BOOL	IsDownloadAble(CString strUrl);
	BOOL	IsDownloading();
	BOOL	IsImageType(LPCTSTR lpszText);
	BOOL	GetScriptLangCode(CString &strCode, CString &strLang);
	BOOL	ConfirmPopupFilter(CString url, BOOL bConfirm);
	//
	int		FindCacheFavIcon(CString strRoot);
	int		GetFavIcon(CString strUrl);
	//
	int		GetPageKeyWordCount();
	//
	void	SetAllTabFavIcon(CString strRoot, int nIcon);
	void	SaveFilterList2File(BOOL bUrlFilter);
	void	AdjustMenuWidth(CString &str);
	void	DragDropLink(CString str, BOOL bActive);
	void	DragDropUrl(CString str, BOOL bActive);
	void	UpdateFilter(BOOL bEnable);
	void	OpenMenuUrl(CString strUrl, DWORD dwProperty, int nAutoRunCmd);
	void	DeleteFav(HMENU hSubMenu, UINT nID, int nButtonIndex, int nBarType);
	void	UpdateBandInfo(CToolBar *pBar, UINT uBandID);
	void	RefreshKillFlyingCode();
	void	RefreshKillBlankCode();
	void	RunKillFlying(CChildFrame* tcf);
	void	RunKillBlank(CChildFrame* tcf);
	void	RunReloadUnloadImage(CChildFrame* tcf);
	void	ClickPoint(int nInNewWindow);
	void	ReloadTabMenu();
	void	ShowBarLabel();
	void	RunUtils(UINT nID,CChildFrame* tcf);
	void	RunSelectedExternal(CChildFrame* tcf, BOOL bAutoRun);
	void    SetAsDefaultBrowser();
	void	RunDownManagerScript(CString strNavigate,CChildFrame* tcf);
	void	HideSelectBars(DWORD dwShow);
	void	ShowSelectBars(DWORD dwShow);
	void	SaveSearchKey();
	void	MarkTab(CChildFrame *tcf);
	void	CloseAllKindPage(CChildFrame *tcf, int kind);
	void	RunScript(CString strCode);
	void	RunScriptByFrame(CString strCode,CString strLang,CChildFrame* tcf);
	void	RunOneScript(CString strCode,CChildFrame* tcf);
	void	CloseAllTabDirect();
	void	LockTab(CChildFrame *tcf);
	void	DoWithImgUrl(int type);
	void	DoWithLinkUrl(int type,CString strUrl,CString strText);
	void	DoWithText(int type,CString strUrl);
	void	OpenJumpUrl(CString strUrl);
	void	AddToCollector(CString strMsg, int type);
	void	SaveAliasConfig();
	void	InitToolBarImage(BOOL bReplaceIcon=FALSE);//�о�Ӧ����������������InitLinkBarImage()
	void	InitMainAnimIcon();
	void	InitLinkBarImage();
	void	InitFavBarImage();//���Ӧ�����ղ�����image��
	void	InitSystemBarImage();//ϵͳ����image?
	void	InitMenuBarImage();//�˵�����image
	void	InitGoImage();//�ѵ����Ǹ���ַ����GOͼƬ?
	void	InitTaskBarImage();//������ͼƬ
	void	SetMessageText(LPCTSTR lpszText);
	void	SearchKey(CString keyw, CString url);
	void	GetHtmlSelectedText(CString &str);
	void	OnToolsCollector(BOOL bShow);
	void	AddToPopList(CString url, int type);
	void	CollectorDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect);
	void	DropFiles(HDROP hDropInfo);
	void	SetHomePage(CString strUrl);
	void	InitialUrlFilter();
	
	DWORD	GetDefaultProperty();
	CString GetImageUrlByPoint(CPoint pt);
	CString GetHomePage();
	HBITMAP	GetBitmap(LPCTSTR lpszText);
	CChildFrame* GetChildFrameByUrl(CChildFrame* tcfCurrent, CString strUrl);
	CChildFrame* GetChildFrameByTarget(CChildFrame* tcfCurrent, CString strTarget);
	//***********************************************************************
	//by ny	
	void PagesInMemory(CArray<long, long&>&pList);
	int GetBrowserViewNum();
	void ClearHideHistory();
	void dumpAllStuff();
	void ClearWorkSpace();
	void addPageSequence(List *pHistory);
	void addPageSequence(List *pHistory, CMarkup *m_hXML, int &m_seqNum);
	long genID();
	void exportXML(CMarkup *m_hXML);
	int RandModel();
	BOOL    evalRecommendation(CWebMindView *pView, int m_button);
	CView *NewHtmlView(int type,BOOL beVisible=FALSE);
	CPageRepository *getRepository()	{	return m_repository;	}
	CWebMindView *NavigateToUrl(CString URL, int type, BOOL beVisible=FALSE, BOOL beFavorite=FALSE);
	//***********************************************************************
	friend	CTxtDropTarget;


private:
	BOOL	m_bLastIB;

	int		m_nBaseWidth;
	int		m_nNeedIB;

	HICON	m_hNormalIcon;
	HICON	m_hSecuIcon;
	
	CLinkListDlg*	m_pLinkListDlg;
	CCollectorDlg*	m_pCollectDlg;
	CTabDropTarget* ms_TabDropTarget;

	//////////////////////////////////////////////////////////////////////////
	CPageRepository *m_repository;//added by kosko
	//////////////////////////////////////////////////////////////////////////

	//****************************************
	//by ny
	CComboBoxEx  * m_AddressCom;
	CLinkBar       m_wndLinkBar;  
	//****************************************

	BOOL GetAllLinks(CWebMindView* pView);
	BOOL IsBrowsing();

	void OnGoUpPage();
	void SessionHistory(CMenu* pMenu, int ID, CChildFrame* pcf);
	void BuildWinButtons();
	void ChangTabPos(int npos);
	void SaveForm(BOOL bIsGeneral);
	void CloseExternalUtil();
	void LaunchEditor(LPCSTR editor, LPCSTR file);
	void LaunchEmail(LPCSTR mailclient);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//control used by search

// Implementation
public:
	BOOL m_bLockToolbar;
	BOOL m_bShowHiddenFav;
	BOOL m_bUseUrlFilter;
	BOOL m_bUseFlyingFilter;
	BOOL m_bUseDialogFilter;
	BOOL m_bDisDownActivex;
	BOOL m_bAnimatIcon;
	BOOL m_bCateWithSite;
	BOOL m_bImgCateWithSite;
	BOOL m_bIsSecure, m_bLastIsSecure;
	BOOL m_bSmallToolBar;
	BOOL m_bDisplayOpenAll, m_bDisplayAddFav;
	BOOL m_bSaveConfig;
	BOOL m_bRemPsw;
	BOOL m_bUseAlias;
	BOOL m_bUseSC;
	BOOL m_bAutoTab;
	BOOL m_bColorIcon;
	BOOL m_bMultiLineTab;
	BOOL m_bFavDirty;
	BOOL m_bFullScreenMode;
	BOOL m_bFullScreened;
	BOOL m_bAutoHideMode;
	BOOL m_bAutoHided;
	BOOL m_bEnProxy;
	BOOL m_bIsOffline;//if offline state
	BOOL m_abWinButtons[6];
	BOOL m_bStatusBlank;
	BOOL m_bRMouseDrag;
	BOOL m_bRMouseGesture;
	BOOL m_bActiveNewWindow;
	BOOL m_bPopRightMenu;

	int  m_nMaxTabWidth;
	int  m_nMinTabWidth;
	int  m_nTextLabel; //0-no label, 1-show label, 2-selective text
	int  m_nNewSequence;
	int  m_nPreLines;
	int  m_nDialogFilterCount;
	int  m_nEnableFilter;
	int  m_nAutoSave;
	int  m_nAllowDBC, m_nAllowMC, m_nAllowRC;
	int  m_nNewWindow;
	int  m_nUndoStart, m_nUndoEnd;

	int  m_nProtectNum;
	int  m_nCloseSequence; //0-default, 1-next, 2- pre
	int  m_nMenuWidth;
	int  m_nDownAll;//page download all
	int  m_nDefaultCharSet;
	int	 m_nMenuStyle;
	int  m_nLDbClick;//left dbclick
	int  m_nLButtonDown;//left button down action
	int  m_nMButtonDown;//middle button down action
	int  m_nRButtonDown;//right button down action
	int  m_nLeftRight;//left right
	int  m_nRightLeft;//right left
	int	 m_nImgUp;
	int	 m_nImgDown;
	int	 m_nImgLeft;
	int	 m_nImgRight;
	int	 m_nLinkUp;
	int	 m_nLinkDown;
	int	 m_nLinkLeft;
	int	 m_nLinkRight;
	int	 m_nTextUp;
	int	 m_nTextDown;
	int	 m_nTextLeft;
	int	 m_nTextRight;
	int  m_nRTabID;
	int  m_nSwitch;
	int m_tabsAtSameTime;//ͬʱ����������ҳ��

	UINT m_nTitleLen;
	UINT m_nState;
	UINT m_nWin;
	UINT m_nFavSize;
	UINT m_nUpdateTimer, m_nAnimateIconTimer;

	CString	m_strCSE2;
	CString	m_strCSE1;
	CString	m_strCE2;
	CString	m_strCE1;
	CString	m_strSE1;
	CString	m_strSE2;
	CString m_strShortCuts[12];
	CString m_strSkinName;
	CString	m_strStartFavFolder;
	CString m_strUndoURL[MAX_UNDO];
	CString m_strUndoName[MAX_UNDO];
	CString m_strProtectPwd;
	CString m_strDefaultWebProxy;
	CString m_strProxyByPass;
	CString m_strCurProxy, m_strCurProxyName;
	CString m_strIeProxy,m_strIeProxyByPass;
	CString m_strHostName;
	CString m_strDomain;

	CStringList m_astrSkins;

	DWORD m_dwWebProxyFrom;
	CTime m_tLastRes;
	CTime m_tLastIPUpdate;
	HBITMAP m_bmBGPic;
	CRect	m_rMainRect;

	CImageList	m_TabImgList;

	CSearchList	m_SearchList;//search list
	CLinkList	m_LinkList;//link list
	CGestureList m_GestureList;
	CFavIconList m_FavIconList;

	CStatusBarWithProgress  m_wndStatusBar;
	CAdvTabCtrl		m_wndTab;
    //CWebMindTabCtrl m_wndTab;
	CImageList     m_ViewTabImages;
	CChevBar		m_wndReBar;
	CChevBar		m_wndReBar2;
	CToolBar		m_wndToolBar;
	CComboBoxEx*	m_wndAddress;
	CComboBoxEx*	m_wndSearch;
	CAddressBarEx	m_AddressBar;
	CComboBar		m_SearchBar;
	CToolBar		m_SysMenuBar;

	CMapStringToString m_mapAlias;
	CProxyCategory m_aProxyCategories;
	CExternalUtilList m_ExternalUtilList;
		
	CAnimateIcon m_animIcon;
	IInternetSession* m_pSession;
	IClassFactory* m_pFactory;
	CWebMindView* m_pCurView;
	//***********************************************************************
	//by ny
	// State which states the history bar is in:
    // 0: No history Bar
    // 1: All history is shown
    // 2: only destination pages are displayed
    int m_historyStates;
	CListCtrl	m_historylist;
	//***********************************************************************

	//#######################################################
	BOOL DecodeEscap(CString& strUrl);
	BOOL GetFavoriteFolder(CString& strFav);
	BOOL SetDefault(LPCSTR lpszKey, BOOL setdef);	
	BOOL HasExpBar(int nStr);
	BOOL ResolveLink(LPCSTR lpszLinkFile, LPSTR lpszPath);
	BOOL GetProxySetting(CString& proxy, CString& bypass);
	BOOL DelTempFiles(DWORD CacheType, BOOL op=TRUE);

	int  AdjustTabWidth(BOOL bNoReCalSize = FALSE);
	int  GetUrlType(LPCTSTR URL);
	int  GetTabIcon(int nItem);
	int  FindDupURL(LPCSTR lpszURL, CChildFrame* pSelf);
	void SetTabIcon(int nIcon, int nItem, CWebMindView* pView);
	int  OpenAllFavs(LPCSTR lpszPath);
	int  FindTab(CChildFrame* lpFrame, BOOL bCleanBlank=FALSE);
	int  AddNewTab(CChildFrame* tcf, int CurID=-1, LPTSTR label=NULL, BOOL bAddLast=TRUE);
	int  BuildFavoritesMenu(LPCTSTR pszPath, int nStartPos, CMenu* pMenu, int nFixMenu /*=0*/, int FirstLevel /*= FALSE*/, int& nFavs);

	void GetProfileReBarBandsState();
	void WriteProfileReBarBandsState();
	void UpdateUndoMenu();
	void SaveLastClose();
	void OpenLastClose();
	void StopView(int nTabID);
	void AddTrayIcon();
	void DelTrayIcon();
	void GetIeProxy();
	void UpdateProxy();
	void BuildGoUpMenu(CMenu *pPopup);
	void BuildProxyMenu(CMenu *pPopup);
	void BuildSearchMenu(BOOL bMenuBreak);
	void BuildTextDropSearchMenu(CMenu *pPopup);
	void OnEncode();
	void MenuAppendSeparator(CMenu *pMenu);
	void MenuInsertSeparator(CMenu *pMenu, UINT nPosition);
	void MenuAppendOwnerItem(CMenu *pMenu, UINT nFlags, UINT nIDNewItem, CString str);
	void MenuAppendOwnerUrl(CMenu *pMenu, UINT nFlags, UINT nIDNewItem, CString str, CString strUrl);
	void MenuInserOwnerItem(CMenu *pMenu,UINT nPosition, UINT nFlags, UINT nIDNewItem,LPCTSTR lpszNewItem,int icon);
	void GetSiteName(CString& site);
	void UpdateTabTitleIndex(int nItem);
	void OnOptionTabStyle();
	void FixToolbar(BOOL bFixed);
	void UninitialNameSpace();
	void InitialNameSpace();
	void BuildWebProxyMenu(CMenu* pMenu, int nType);
	void StartUtil(int i);
	void DefaultMail(LPCSTR mailto);
	void SaveLastVisit(BOOL bVisiting);
	void AddtoFavorite(BSTR bstrURL, VARIANT* pTitle);
	void ToolBarSetCheck(CCmdUI* pCmdUI, BOOL bCheck);
	void FavMenuAddURL(LPCSTR path, LPCSTR root, BOOL FirstLevel, BOOL IsLink, CMenu *pMenu, int nPos);
	void FavMenuAddFolder(LPCSTR path, LPCSTR root, BOOL FirstLevel, BOOL IsLink, CMenu* pMenu, int& nEndPos);
	void OnOptionMultiTab();
	void OnOptionAutotab();
	void InitToolBar(BOOL bReplaceIcon=FALSE);
	void ShowBackground(BOOL bDisable = FALSE);
	void ShowSkin();
	void AddFavorite(CChildFrame* tcf);
	void FullScreenModeOff(BOOL bOnlyBar=FALSE);
	void FullScreenModeOn(BOOL bOnlyBar=FALSE);
	void AutoHideModeOff();
	void AutoHideModeOn();
	void BuildUtilMenu();
	void GetNetWorkInfo();
	void AddToPopup(CChildFrame* pChFrm);
	void AddMenuBreak(CMenu * pMenu,BOOL bFavMenu=TRUE);
	void InitFavorites(BOOL bOnlyLink=FALSE);
	void SaveTypedURL();
	void LoadTypedURL();
	void LoadTypedSearch();
	void SetTabTitle(LPCSTR lpszTitle, int nItem);
	void SetAddress(LPCTSTR lpszUrl);
	//**********************************	
	//by ny
	void SetViewNames(CString ViewName, CView *pView);
	int GetChildWindowNum();
	void GenerationScheduleSingle(CWebMindView *pView, int m_button, List *m_sessionWords, CStringArray &m_stemQuery, CStringArray &m_evalQuery, CStringArray &m_evalURLs, CStringArray &m_evalTitles, int &m_modelSelector, CString &m_strVersion, CString &m_strModelCode);
	void LoadModelPara();
	void LoadICWordModel();
	void LoadICRelevantModel();
	void LoadICQueryModel();
	void queryingGoogle(CString &m_strQuery, CStringArray &genURLs, CStringArray &genTitles);
	void queryingSearchEngine(CStringArray &m_stemWords, CArray<double, double&> &m_icProbs, CStringArray &stemQuery, CStringArray &genQuery, CStringArray &genURLs, CStringArray &genTitles);
	void modelMFW(List *m_sessionWords, CStringArray &stemQuery, CStringArray &genQuery, CStringArray &genURLs, CStringArray &genTitles);
	void modelICWordC45(List *m_sessionWords, CStringArray &stemQuery, CStringArray &genQuery, CStringArray &genURLs, CStringArray &genTitles);
	void modelICRelevant(List *m_sessionWords, CStringArray &stemQuery, CStringArray &genQuery, CStringArray &genURLs, CStringArray &genTitles);
	void modelICQuery(List *m_sessionWords, CStringArray &stemQuery, CStringArray &genQuery, CStringArray &genURLs, CStringArray &genTitles);
	void modelBase(int model, List *m_sessionWords, CStringArray &stemQuery, CStringArray &genQuery, CStringArray &genURLs, CStringArray &genTitles, C45Learner *m_pDT = NULL, double weight = 100.0);
	bool IsSameURL(const CString &genURL, const CString &viewedURL);
	void excludeExaminedURL(CWebMindView *pView, CStringArray &genURLs, CStringArray &genTitles);
	void getRelevantWords(List *m_sessionWords, CStringArray &m_evalQuery);
	void UpDateHistoryBar();
	void convertOnLine(CViewOnLine *pOnLine, CPageView *pView);
	void releaseMemory()
	{
		if(m_repository)
			m_repository->releaseAllPageCaches();
	}
	HMENU NewMenu();
	//**********************************
	CWnd* GetBandWnd(int id);
	CString	GetBandTitle(int id);
	CString GetStandUrl( CString url );
	CString GetProxyName(CString& proxy);
	
	CMenu* GetMenu();
	CChildFrame* NewChildWindow(int nIniLevel=1, int nOpenType=1, LPCSTR strUrl=NULL, CWebMindView * pParentView=NULL, BOOL bForceActive=FALSE);
	HRESULT ExecCmdTarget(DWORD nCmdID,CFixedHtmlView* pHVW);

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMenu		m_TabMenu;
	CMenu		m_TrayMenu;
	CToolBar	m_LinkBar;
	CMenuBar	m_wndMenuBar;
	CToolBar	m_FavBar;
	CToolBar	m_ExternalTool;
	HINSTANCE   m_hResInst;//
	//**********************************
	//by ny
   //CWebMindView *m_pHistoryThread;
   CAddressBarEx m_wndAddressBar;
	Extractor *m_extractor;
	CDataSet *m_pTrainData;
	// each time the user launches WebMind, a temp file will be created
	// to save the page sequence, and WebMind will also autosave the 
	// page sequence
	CString m_saveXMLFile;
	// each time the user closes one tab, the page view sequence will be
	// put into the xmlHistory which will be write to a file when
	// the user exits WebMind
	CMarkup *m_xmlHistory;
	bool m_bChangedXML;
	// the number of page view sequences that have already been on m_xmlHistory
	int m_numSeq;
	// classifier that can be used for recommendation
    C45Learner *m_modelICWord;
    C45Learner *m_modelICRelevant;
    C45Learner *m_modelICQuery;
	CStringArray m_freeModels;
	CStringArray m_usedModels;
	// whether we should query Google or Altavista
	BOOL m_bQueryingGoogle;
	// whether we have loaded these parameters successfully
	BOOL m_bModelPara;
	// the parameters for each model
	double m_paraICWord[NUM_ATTRIBUTE];
	double m_paraICRelevant[NUM_ATTRIBUTE];
	double m_paraICQuery[NUM_ATTRIBUTE];
	// time check for the latest version of WebMind
	bool m_bNewWebICAvailable;
	CHighTime m_tLastCheck;
	CMenu m_menu;
	HMENU m_sharedMenu;
    //**********************************

	//Add ChartCtrl
    CChartCtrl m_ChartCtrl;
	CString m_strMenuUrl, m_strMenuText;
	CString GetLocalIP();
	CString GetMemory();
	CString GetOnlineTime();


// Generated message map functions
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileNewBlank();
	afx_msg void OnFileNewCurrentpage();
	afx_msg void OnViewAddressbar();
	afx_msg void OnViewTaskbar();
	afx_msg void OnFileCloseall();
	afx_msg void OnFileWorkoffline();
	afx_msg void OnClose();
	afx_msg void OnViewSource();
	afx_msg void OnFileClose();
	afx_msg void OnFileClose2();
	afx_msg void OnToolsInternetoptions();
	afx_msg void OnFavoritesAddtofavorites();
	afx_msg void OnFavoritesExportfavorites();
	afx_msg void OnFavoritesImportfavorites();
	afx_msg void OnFavoritesOrgnizefavorites();
	afx_msg void OnToolsAddtopopupfilter();
	afx_msg void OnRtabAddtopopupfilter();
	afx_msg void OnToolsEditpopupfilterlist();
	afx_msg void OnUpdateFileWorkoffline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenu(CCmdUI* pCmdUI);
	afx_msg void OnToolsEmail();
	afx_msg void OnToolsAddr();
	afx_msg void OnUpdateOptionsUseProxy(CCmdUI* pCmdUI);
	afx_msg void OnOptionsUseProxy();
	afx_msg void OnToolsCleanCookie();
	afx_msg void OnToolsCache();
	afx_msg void OnToolsAll();
	afx_msg void OnToolsHistory();
	afx_msg void OnOptionsSetProxy();
	afx_msg void OnToolsExternalUtilitymanager();
	afx_msg void OnViewFullscreen();
	afx_msg void OnUpdateViewFullscreen(CCmdUI* pCmdUI);
	afx_msg void OnAddrDrop();
	afx_msg void OnAddrFocus();
	afx_msg void OnFileNew();
	afx_msg void OnHelpHelp();
	afx_msg void OnOptionsActivenewwindow();
	afx_msg void OnUpdateOptionsActivenewwindow(CCmdUI* pCmdUI);
	afx_msg void OnRtabAddfav();
	afx_msg void OnViewLinks();
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHelpForum();
	afx_msg void OnHelpMail();
	afx_msg void OnFileCloseKeepcur();
	afx_msg void OnViewRefreshall();
	afx_msg void OnViewStopAll();
	afx_msg void OnHelpHomepage();
	afx_msg void OnFileNewClip();
	afx_msg void OnWindowCascade();
	afx_msg void OnWindowTileHorz();
	afx_msg void OnWindowTileVert();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnWindowRestore();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnWindowMin();
	afx_msg void OnViewMenubar();
	afx_msg void OnToolsSetSkin(UINT nID);
	afx_msg void OnUpdateToolsSetSkin(CCmdUI* pCmdUI);
	afx_msg void OnFileNewWebMind();
	afx_msg void OnFileOpen();
	afx_msg void OnViewToolbartextlabel();
	afx_msg void OnUpdateViewToolbartextlabel(CCmdUI* pCmdUI);
	afx_msg void OnChangeProxy(UINT nID);
	afx_msg void OnChangeWebProxy(UINT nID);
	afx_msg void OnF2(UINT nID);
	afx_msg void OnShortcutUseSc();
	afx_msg void OnUpdateShortcutUseSc(CCmdUI* pCmdUI);
	afx_msg void OnShortcutSc();
	afx_msg void OnShortcutAlias();
	afx_msg void OnUpdateShortcutAlias(CCmdUI* pCmdUI);
	afx_msg void OnShortcutManalias();
	afx_msg void OnViewToolbarsCustomize();
	afx_msg void OnOptionsWebMindoptions();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnNotifyTray(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelTab(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRtabStickname();
	afx_msg void OnTabPre();
	afx_msg void OnTabNext();
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnOptionsUrlinstatus();
	afx_msg void OnUpdateOptionsUrlinstatus(CCmdUI* pCmdUI);
	afx_msg void OnViewFavs();
	afx_msg void OnViewProtect();
	afx_msg void OnViewExTool();
	afx_msg void OnViewSearch();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFileHide();
	afx_msg void OnOptionsOpenlinkinnewwindow();
	afx_msg void OnUpdateOptionsOpenlinkinnewwindow(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnEditUndoclose();
	afx_msg void OnUpdateEditUndoclose(CCmdUI* pCmdUI);
	afx_msg void OnEditEdit();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnNewwindow();
	afx_msg void OnFileSendpagebyemail();
	afx_msg void OnFileQuicksave();
	afx_msg void OnFileSendlinkbyemail();
	afx_msg void OnToolsQuicksearch();
	afx_msg void OnToolsSync();
	afx_msg void OnScFillform();
	afx_msg void OnScSaveform();
	afx_msg void OnScSavegenform();
	afx_msg void OnToolsCollect();
	afx_msg void OnToolsCleanaddr();
	afx_msg void OnAllPageLinks();
	afx_msg void OnViewEncodingAuto();
	afx_msg void OnUpdateViewEncodingAuto(CCmdUI* pCmdUI);
	afx_msg void OnToolCleanWebMindform();
	afx_msg void OnRtabAddalias();
	afx_msg void OnRtabAddsc();
	afx_msg void OnRtabAutosave();
	afx_msg void OnToolsUrlFilter();
	afx_msg void OnUpdateToolsUrlFilter(CCmdUI* pCmdUI);
	afx_msg void OnViewLocktoolbar();
	afx_msg void OnUpdateViewLocktoolbar(CCmdUI* pCmdUI);
	afx_msg void OnFileNewSelect();
	afx_msg void OnOptionsUseIeProxy();
	afx_msg void OnUpdateOptionsUseIeProxy(CCmdUI* pCmdUI);
	afx_msg void OnCleanTypedSearch();
	afx_msg void OnViewAutoHide();
	afx_msg void OnUpdateViewAutohide(CCmdUI* pCmdUI);
	afx_msg void OnViewSysMenuBar();
	afx_msg void OnViewLock();
	afx_msg void OnEndSession(BOOL bEnding);
	afx_msg void OnToolsSearchText();
	afx_msg void OnLastVisited();
	afx_msg void OnWindowManager();
	afx_msg void OnUpdateWindowManager(CCmdUI* pCmdUI);
	afx_msg void OnFileNewSelectedLink();
	afx_msg void OnFileAutoSaveAll();
	afx_msg void OnFileNewCurrentHomePage();
	afx_msg void OnFileNewCurrentUpPage();
	afx_msg void OnGoStartPage();
	afx_msg void OnScSetFormData();
	afx_msg void OnViewFontsIncrease();
	afx_msg void OnViewFontsDecrease();
	afx_msg void OnViewMarkKind();
	afx_msg void OnViewMark();
	afx_msg void OnToolsNoBackground();
	afx_msg void OnUpdateToolsNoBackground(CCmdUI* pCmdUI);
	afx_msg void OnViewSetUndoClose();
	afx_msg void OnToolsCleanUndoClose();
	afx_msg void OnAddtoIe();
	afx_msg void OnFileCloseAlikeTitle();
	afx_msg void OnFileCloseAllLeft();
	afx_msg void OnFileCloseAllRight();
	afx_msg void OnFileCloseAlikeUrl();
	afx_msg void OnFileCloseMarkedUrl();
	afx_msg void OnOptionsAllowAnimate();
	afx_msg void OnUpdateOptionsAllowAnimate(CCmdUI* pCmdUI);
	afx_msg void OnOptionsDisableFlash();
	afx_msg void OnUpdateOptionsDisableFlash(CCmdUI* pCmdUI);
	afx_msg void OnToolsSearchCase();
	afx_msg void OnUpdateToolsSearchCase(CCmdUI* pCmdUI);
	afx_msg void OnToolsSearchWhole();
	afx_msg void OnUpdateToolsSearchWhole(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsQuickSearch(CCmdUI* pCmdUI);
	afx_msg void OnToolsExternalAutorun();
	afx_msg void OnUpdateToolsExternalAutorun(CCmdUI* pCmdUI);
	afx_msg void OnViewCompletelyRefresh();
	afx_msg void OnTextAutoSave();
	afx_msg void OnTextCopy();
	afx_msg void OnTextEdit();
	afx_msg void OnTextSave();
	afx_msg void OnTextSearch();
	afx_msg void OnUrlCopy();
	afx_msg void OnUrlEdit();
	afx_msg void OnUrlFilter();
	afx_msg void OnUrlOpen();
	afx_msg void OnUrlOpenActive();
	afx_msg void OnUrlOpenIe();
	afx_msg void OnUrlOpenJump();
	afx_msg void OnUrlShow();
	afx_msg void OnTextHighLight();
	afx_msg void OnTextFind();
	afx_msg void OnTextOpenLinks();
	afx_msg void OnUrlAddtoFav();
	afx_msg void OnUrlCollector();
	afx_msg void OnTextCollector();
	afx_msg void OnToolsSearchDelete();
	afx_msg void OnProxyImport();
	afx_msg void OnProxyExport();
	afx_msg void OnUrlOpenInactive();
	afx_msg void OnToolsExternalRun();
	afx_msg void OnViewSelectLang();
	afx_msg void OnRtabNewWindow();
	afx_msg void OnGoPageUp();
	afx_msg void OnGoPageDown();
	afx_msg void OnGoPageBottom();
	afx_msg void OnGoPageTop();
	afx_msg void OnHighlightSelectedText();
	afx_msg void OnFindNextSelectedText();
	afx_msg void OnOpenLink();
	afx_msg void OnOpenLinkActive();
	afx_msg void OnOpenLinkDeactive();
	afx_msg void OnFindPrevSelectedText();
	afx_msg void OnNotSaveKey();
	afx_msg void OnUpdateNotSaveKey(CCmdUI* pCmdUI);
	afx_msg void OnUrlOpenOtherSide();
	afx_msg void OnUrlOpenFtp();
	afx_msg void OnRtabProtect();
	afx_msg void OnRtabLock();
	afx_msg void OnRtabMark();
	afx_msg void OnGoPageRight();
	afx_msg void OnGoPageLeft();
	afx_msg void OnUrlShowImg();
	afx_msg void OnRtabCloseKeepcur();
	afx_msg void OnToolsFlyFilter();
	afx_msg void OnUpdateToolsFlyFilter(CCmdUI* pCmdUI);
	afx_msg void OnToolsDisDownActivex();
	afx_msg void OnUpdateToolsDisDownActivex(CCmdUI* pCmdUI);
	afx_msg void OnToolsDialogFilter();
	afx_msg void OnUpdateToolsDialogFilter(CCmdUI* pCmdUI);
	afx_msg void OnEditImgUrl();
	afx_msg void OnFavoritesResetOrder();
	//afx_msg void OnToolUpload();
	//}}AFX_MSG
	afx_msg void DoFont();
	afx_msg void OnNewTab();
	afx_msg void OnUpdateNewTab(CCmdUI* pCmdUI);
	afx_msg void OnViewWinCust(UINT nID);
	afx_msg void OnUpdateViewWinCust(CCmdUI* pCmdUI);
	afx_msg void OnNewAddress();
	afx_msg void DoNothing();
	afx_msg void OnNewAddressEnter();
	afx_msg void OnNewSearchEnter();
	afx_msg CString GetMenuItemUrl(UINT nItemID, HMENU hSysMenu,BOOL bRealUrl=TRUE);
	afx_msg void OpenMenuItem(UINT nID);
	afx_msg void OpenGoUpMenuItem(UINT nID);
	afx_msg void OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnSelChange(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnRClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnRClickElse(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnRClickStatusToolbar(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnEncoding(UINT nID);
	afx_msg void OnFonts(UINT nID);
	afx_msg void OnUtils(UINT nID);
	afx_msg void OnSearch(UINT nID);
	afx_msg void OnShellOpen(WPARAM wParam,LPARAM);
	afx_msg void OnLinkPopup();
	afx_msg void OnFavPopup();
	afx_msg void OnUpdateFav(WPARAM wParam,LPARAM);
	afx_msg void OnUpdateTab(WPARAM wParam,LPARAM);
	afx_msg void OnDelayLoadConfig(WPARAM wParam,LPARAM);
	afx_msg void OnAllFav(UINT nID);
	afx_msg void OnUpdateTabTips(WPARAM wParam,LPARAM);
	afx_msg void OnTabNeedText(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnUpdateToolbar(WPARAM wParam,LPARAM);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnSwitchWins(UINT nID);
	afx_msg void OnRtabRefresh();
	afx_msg void OnRtabStop();
	afx_msg void OnActivateWindow(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHotKey(WPARAM wParam,LPARAM);
	afx_msg void OnScRll(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewStop();
	afx_msg void OnViewRefresh();
	afx_msg void OnFilePrintpreview();
	afx_msg void OnFilePrint();
	afx_msg void OnWindowList(UINT nID);
	afx_msg void OnUndoList(UINT nID);
	afx_msg void OnAddThisFav(UINT nID);
	//add
	afx_msg void OnRMouseDrag();
	afx_msg void OnRMouseGesture();
	afx_msg void OnToolsEnablePopFilter();
	afx_msg void OnUpdateToolsEnablePopFilter(CCmdUI* pCmdUI);
	afx_msg void OnEnableFilter();
	afx_msg void OnUpdateEnableFilter(CCmdUI* pCmdUI);
	afx_msg void OnFindSearchBar();
	afx_msg void OnHighLightSearchBar();
	afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnAppCommand(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMenuRButtonUp(WPARAM w,LPARAM l);

	void OnUpdateRMouseDrag(CCmdUI* pCmdUI);
	void OnUpdateRMouseGesture(CCmdUI* pCmdUI);
	void OnUpdateProgress(CCmdUI* pCmdUI);
	void OnUpdateMsg(CCmdUI* pCmdUI);
	void OnUpdateDisp(CCmdUI* pCmdUI);
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
class CFindWnd
{
private:
	//////////////////
	// This private function is used with EnumChildWindows to find the child
	// with a given class name. Returns FALSE if found (to stop enumerating).
	//
	static BOOL CALLBACK FindChildClassHwnd(HWND hwndParent, LPARAM lParam)
	{
		CFindWnd *pfw = (CFindWnd*)lParam;
		HWND hwnd = FindWindowEx(hwndParent, NULL, pfw->m_classname, NULL);
		if (hwnd)
		{
			pfw->m_hWnd = hwnd;	// found: save it
			return FALSE;			// stop enumerating
		}
		EnumChildWindows(hwndParent, FindChildClassHwnd, lParam); // recurse
		return TRUE;				// keep looking
	}

public:
	LPCSTR m_classname;			// class name to look for
	HWND m_hWnd;				// HWND if found
	// ctor does the work--just instantiate and go
	CFindWnd(HWND hwndParent, LPCSTR classname)	: m_hWnd(NULL), m_classname(classname)
	{
		FindChildClassHwnd(hwndParent, (LPARAM)this);
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__19E497C1_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)

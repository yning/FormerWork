///////////////////////////////////////////////////////////////
// DlgBrowser.cpp : implementation file
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebMind.h"
#include "DlgBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBrowser dialog


CDlgBrowser::CDlgBrowser(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CDlgBrowser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBrowser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pRepository = NULL;
	pCache = NULL;

	bCompare = false;
	bCacheLoaded = false;
	m_strURL.Empty();

	bFirstNavigation = true;
}


void CDlgBrowser::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBrowser)
	DDX_Control(pDX, IDC_EXPLORER, m_browserCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBrowser, CTabPageSSL)
	//{{AFX_MSG_MAP(CDlgBrowser)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBrowser message handlers

BEGIN_EVENTSINK_MAP(CDlgBrowser, CTabPageSSL)
    //{{AFX_EVENTSINK_MAP(CDlgBrowser)
	ON_EVENT(CDlgBrowser, IDC_EXPLORER, 259 /* DocumentComplete */, OnDocumentCompleteExplorer, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CDlgBrowser, IDC_EXPLORER, 251 /* NewWindow2 */, OnNewWindow2Explorer, VTS_PDISPATCH VTS_PBOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



void CDlgBrowser::OnInitialUpdate()
{
	CTabPageSSL::OnInitDialog();

	m_browserCtrl.ModifyStyle(0, WS_HSCROLL | WS_VSCROLL);
}

void CDlgBrowser::OnDocumentCompleteExplorer(LPDISPATCH , VARIANT FAR* ) 
{
	// After the page has been fully loaded, insert it into the repository
	long m_type = m_browserCtrl.GetReadyState();
//	CViewOnLine *pView;
	CPageCache *pageCache;

	pCache = NULL;
	if(!bCompare && m_type == READYSTATE_COMPLETE)
	{
		if(pRepository)
		{
			LPDISPATCH tDisp = m_browserCtrl.GetDocument();
			pageCache = pRepository->insertPage(tDisp);
			pCache = pageCache;
		}
	}
}

void CDlgBrowser::loadGoogleCache()
{
	if(bCacheLoaded || m_strURL.IsEmpty())
		return;

	CString strGoogleCache;
	strGoogleCache.Format("http://www.google.com.hk/search?q=cache:%s", m_strURL);
	m_browserCtrl.Navigate(strGoogleCache, NULL, NULL, NULL, NULL);

	bCacheLoaded = true;
}

void CDlgBrowser::GoToURL(const CString &strURL)
{
	m_strURL = strURL;

	m_browserCtrl.Navigate(strURL, NULL, NULL, NULL, NULL);
}

/*
void CDlgBrowser::OnBeforeNavigate2Explorer(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel) 
{
	if(bFirstNavigation)
		bFirstNavigation = false;
	else
		*Cancel = TRUE;
}
*/

void CDlgBrowser::OnNewWindow2Explorer(LPDISPATCH FAR* , BOOL FAR* Cancel) 
{
	*Cancel = TRUE;
}

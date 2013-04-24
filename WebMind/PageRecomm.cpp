///////////////////////////////////////////////////////////////
// PageRecomm.cpp : implementation file
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebMind.h"
#include "PageRecomm.h"

#include "WizSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CPageRecomm property page

IMPLEMENT_DYNCREATE(CPageRecomm, CPropertyPageEx)

CPageRecomm::CPageRecomm() : CPropertyPageEx(CPageRecomm::IDD)
{
	//{{AFX_DATA_INIT(CPageRecomm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pageBrowsers = NULL;

	bCompare = false;

	genURLs.RemoveAll();
	genTitles.RemoveAll();

	genPages.RemoveAll();

	m_bEnableNext = false;


	strNoteCompare = _T("这是我们为您推荐的和您当前目标相关的页面，")
	                 _T("请浏览以下页面并决定是否最相关，")
	                 _T("您可以点击下一步按钮来评估推荐结果：");

	strNoteNoCompare = _T("这是我们为您推荐的和您当前目标相关的页面，")
	                 _T("请浏览以下页面并决定是否最相关，")	                 
					 _T("您可以点击下一步按钮来评估推荐结果：");
}

CPageRecomm::~CPageRecomm()
{
	if(m_pageBrowsers)
		delete [] m_pageBrowsers;

	genURLs.RemoveAll();
	genTitles.RemoveAll();
}

void CPageRecomm::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageRecomm)
	DDX_Control(pDX, IDC_TAB_PAGE, m_tabPages);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageRecomm, CPropertyPageEx)
	//{{AFX_MSG_MAP(CPageRecomm)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageRecomm message handlers

BOOL CPageRecomm::OnSetActive() 
{
	CWizard97Sheet* pSheet = (CWizard97Sheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);

	if(pSheet)
	{
		if(pSheet->m_bHeaderCtrl)
		{
		    pSheet->m_HeaderCtrl.SetTitle ("推荐");
		    pSheet->m_HeaderCtrl.SetIcon (IDI_PREDICTION);

			if(bCompare)
				pSheet->m_HeaderCtrl.SetDesc (strNoteCompare);
			else
				pSheet->m_HeaderCtrl.SetDesc (strNoteNoCompare);
		}
	}

	if(m_bEnableNext)
		pSheet->SetWizardButtons(PSWIZB_NEXT);
	else
		pSheet->SetWizardButtons(PSWIZB_DISABLEDFINISH);

	CWnd *pWnd = pSheet->GetDlgItem( IDCANCEL );
	if(pWnd)
		pWnd->ShowWindow( FALSE );	

	return CPropertyPageEx::OnSetActive();
}

BOOL CPageRecomm::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	SetTimer(0, 500, NULL);

	// we record the start time,
	// after 5 seconds, if the page is still
	// not ready then we will try to load google's
	// cache, and after 10 seconds, still not 
	// available, we will enable the button to 
	// the next page
	m_timeStart = CHighTime::GetPresentTime();

	// Setup the tab control
	if (m_ilTabs.Create(IDB_BITMAP_EVAL, 16, 1, 255))
		m_tabPages.SetImageList(&m_ilTabs);

	// creat all the browsing pages
	m_pageBrowsers = (CDlgBrowser *) new CDlgBrowser[genURLs.GetSize()];

	// In case of comparison, we wonot check the last page's PageCache
	if(bCompare)
		m_pageBrowsers[genURLs.GetSize() - 1].bCompare = true;


	int id;
	CString buffer;

	// set the point to the repository
	for(id = 0; id < genURLs.GetSize(); id++)
		m_pageBrowsers[id].pRepository = pRepository;

	// if there is no title information, just show the URLs
	if(genTitles.GetSize() <= 0)
		genTitles.Append(genURLs);

    int count=genURLs.GetSize();//add by ny

	for(id = 0; id < genURLs.GetSize(); id++)
	{
		m_pageBrowsers[id].Create(IDD_DIALOG_BROWSER, this);

		// get the title (up to 25 words) of the url that will be opened
		buffer = genTitles[id].Left(25);
		if(genTitles[id].GetLength() > 25)
			buffer += _T("...");

		// open the url
		//CString recomUrl(genURLs[id]);
		//CString left="/url?q=";
        //CString right="&amp";
		CString domain="www.google.com.hk";
		if(genURLs[id].Find("/url?q=")>=0 ||genURLs[id].Find("/search?q=")>=0)
		{
		genURLs[id]= domain+genURLs[id];
		}
		m_pageBrowsers[id].GoToURL(genURLs[id]);

		// if this is the last page, and we are supposed to compare
		// the generated page and his/her IC-page, we will change the icon
		if(bCompare &&( id == (genURLs.GetSize() - 1)))//by ny
			m_tabPages.AddSSLPage (buffer, id, &m_pageBrowsers[id], 0);
		else
			m_tabPages.AddSSLPage (buffer, id, &m_pageBrowsers[id], 1);
	}

	// change the last tab's color in case of comparison
	if(bCompare)
	{
		m_tabPages.SetTabColor(genURLs.GetSize() - 1, RGB(255, 204, 51));
		m_tabPages.Invalidate();
		Invalidate();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageRecomm::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWizard97Sheet* pSheet = (CWizard97Sheet*)GetParent();
	if(pSheet)
		pSheet->m_activePage = m_tabPages.GetSSLActivePage();

	return CPropertyPageEx::OnKillActive();
}

void CPageRecomm::OnTimer(UINT nIDEvent) 
{
	// we will enable the "Next" button until all the pages
	// have been loaded completely.
//	bool enableNext = true;

	int startID;
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);


	CHighTime m_timeCurrent;
	CHighTimeSpan m_timeSpan;

	m_timeCurrent = CHighTime::GetPresentTime();
	m_timeSpan = m_timeCurrent - m_timeStart;

	// if any page cannot be loaded within 30 seconds, then
	// we request its cached page in google,
	// if after another 30 seconds, we will enable the  
	// "Next" button

	if(m_timeSpan.GetSeconds() < 5)//by ny ;original:59
	{
		m_bEnableNext = true;
		//startID = bCompare ? 1 : 0;
		for(int i = 0; i < genURLs.GetSize()-(bCompare?1:0); i++)
		{
			// if the page cannot be loaded within 5 seconds, then
			// we try to load its google cached page
			if(m_pageBrowsers[i].pCache == NULL)
			{
				/*
				if(m_timeSpan.GetSeconds() >= 30)
				{
					long m_type = m_pageBrowsers[i].m_browserCtrl.GetReadyState();
					if(m_type != READYSTATE_INTERACTIVE && m_type != READYSTATE_COMPLETE)
						m_pageBrowsers[i].loadGoogleCache();
				}
				*/

				m_bEnableNext = false;
			}
		}
	}
	else
	{
		m_bEnableNext = true;
	}

	// if all the pages have been loaded, then enable the "Next" button,
	// otherwise, wait until all done

	if(m_bEnableNext)
	{
		pSheet->SetWizardButtons(PSWIZB_NEXT);

		CPageCache *pCache;
		CString strBuffer;

		genPages.RemoveAll();

		startID = bCompare ? 1 : 0;
		for(int j = startID; j < genURLs.GetSize(); j++)
		{
			// go through each browser, check whether its CPageCache has been
			// initialized
			pCache = m_pageBrowsers[j].pCache;
			if(pCache)
			{
				strBuffer.Format("%ld", pCache->getPageno());
				genPages.Add(strBuffer);
			}
			else
				genPages.Add(genURLs[j]);
		}

		KillTimer(0);
	}
	else
	{
		pSheet->SetWizardButtons(PSWIZB_DISABLEDFINISH);
/*
		pSheet->SetFinishText("Next");
		pSheet->SetWizardButtons(PSWIZB_DISABLEDFINISH);

	    CButton* nextButton=(CButton*)pSheet->GetDlgItem(ID_WIZNEXT);
		if(nextButton)
			nextButton->EnableWindow(FALSE);
*/
		CPropertyPageEx::OnTimer(nIDEvent);
	}
}

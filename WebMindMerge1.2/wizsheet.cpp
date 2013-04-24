// SampWizP.cpp : implementation file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "WebMind.h"
#include "WizSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampWizP

IMPLEMENT_DYNAMIC(CWizard97Sheet, CPropertySheetEx)

CWizard97Sheet::CWizard97Sheet(UINT nIDCaption, CWnd* pParentWnd,
	UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark,
	HBITMAP hbmHeader)
: CPropertySheetEx(nIDCaption, pParentWnd, iSelectPage,
				  hbmWatermark, hpalWatermark, hbmHeader)
{
	// add all the pages of the wizard

	AddPage(&m_pageRecomm);
	AddPage(&m_pageEval);

	// set the WIZARD97 flag so we'll get the new look
	m_psh.dwFlags |= PSH_WIZARD97;

	m_activePage = 0;

	m_bHeaderCtrl = false;
	bCompare = false;
//	CreatHeaderCtrl();
}

CWizard97Sheet::CWizard97Sheet(LPCTSTR pszCaption, CWnd* pParentWnd,
	UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark,
	HBITMAP hbmHeader)
: CPropertySheetEx(pszCaption, pParentWnd, iSelectPage,
					  hbmWatermark, hpalWatermark, hbmHeader)

{
	// add all the pages of the wizard
	AddPage(&m_pageRecomm);
	AddPage(&m_pageEval);

	// set the WIZARD97 flag so we'll get the new look
	m_psh.dwFlags |= PSH_WIZARD97;

	m_activePage = 0;

	m_bHeaderCtrl = false;
	bCompare = false;
//	CreatHeaderCtrl();
}

CWizard97Sheet::~CWizard97Sheet()
{
}


BEGIN_MESSAGE_MAP(CWizard97Sheet, CPropertySheetEx)
	//{{AFX_MSG_MAP(CWizard97Sheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizard97Sheet message handlers

BOOL CWizard97Sheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheetEx::OnInitDialog();

	//****uncomment by ny*********************
	/*
	CRect rectWnd;
    int m_nHeaderHeight = 50;        //The height of the header control

    GetWindowRect (rectWnd);
    ScreenToClient (rectWnd);
    SetWindowPos (NULL, 0, 0, rectWnd.Width(), rectWnd.Height() + m_nHeaderHeight,
        SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    
    // create your header control here
    m_HeaderCtrl.CreateEx (NULL, NULL, NULL, 
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
        -1, -1, rectWnd.Width (), m_nHeaderHeight - 5, m_hWnd, 0, 0);

    m_HeaderCtrl.SetTitle ("Recommendation");
//    m_HeaderCtrl.SetIcon (IDR_TRACK);
    m_HeaderCtrl.SetDesc (_T("Please examine each of the pages on the tabs below.\r\nDecide which you think is the best and press Next"));
	
	//****uncomment by ny*********************
*/
	CreatHeaderCtrl();

	return bResult;
}

void CWizard97Sheet::CreatHeaderCtrl()
{
    CRect rectWnd;
    int m_nHeaderHeight = 60;        //The height of the header control

    GetWindowRect (rectWnd);
    ScreenToClient (rectWnd);
    SetWindowPos (NULL, 0, 0, rectWnd.Width(), rectWnd.Height() + m_nHeaderHeight,
        SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    
    // create your header control here
    m_HeaderCtrl.CreateEx (NULL, NULL, NULL, 
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
        -1, -1, rectWnd.Width (), m_nHeaderHeight - 5, m_hWnd, 0, 0);


	CString strNoteCompare;
	CString strNoteNoCompare;

	strNoteCompare =_T("这是我们为您推荐的和您当前目标相关的页面，")
	                 _T("请浏览以下页面并决定是否最相关，")
	                 _T("您可以点击下一步按钮来评估推荐结果：");

	strNoteNoCompare =_T("这是我们为您推荐的和您当前目标相关的页面，")
	                 _T("请浏览以下页面并决定是否最相关，")	                 
					 _T("您可以点击下一步按钮来评估推荐结果：");

    m_HeaderCtrl.SetTitle ("推荐：");
    m_HeaderCtrl.SetIcon (IDI_PREDICTION);
	if(bCompare)
		m_HeaderCtrl.SetDesc (strNoteCompare);
	else
		m_HeaderCtrl.SetDesc (strNoteNoCompare);


	m_bHeaderCtrl = true;
}

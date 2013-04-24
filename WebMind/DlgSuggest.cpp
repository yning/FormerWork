///////////////////////////////////////////////////////////////
// DlgSuggest.cpp : implementation file
//
// This software was written by Tingshao Zhu.
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebMind.h"
#include "DlgSuggest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSuggest dialog


CDlgSuggest::CDlgSuggest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSuggest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSuggest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSuggest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSuggest)
	DDX_Control(pDX, IDC_SECOND_LINE, m_second);
	DDX_Control(pDX, IDC_FIRST_LINE, m_first);
	DDX_Control(pDX, IDC_SUMMARY, m_suggest);
	DDX_Control(pDX, IDC_MARKIC, m_markIC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSuggest, CDialog)
	//{{AFX_MSG_MAP(CDlgSuggest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSuggest message handlers

BOOL CDlgSuggest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_markIC.SetURL("http://www.web-ic.com/lilac/manual.html#markic");
	m_suggest.SetURL("http://www.web-ic.com/lilac/honorarium.html");

	m_first.SetFontStatic("Arial",20,RGB(255,255,255), FS_LEFT | FS_BOLD);
	m_first.SetBackground(RGB(0,128,0));

	m_second.SetFontStatic("Arial",20,RGB(255,255,255),FS_LEFT | FS_BOLD);
	m_second.SetBackground(RGB(0,128,0));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////
// PageRecomm.h : header file
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_PAGERECOMM_H__0E358AC2_6E10_42A6_9F10_29EF1B761492__INCLUDED_)
#define AFX_PAGERECOMM_H__0E358AC2_6E10_42A6_9F10_29EF1B761492__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabCtrlSSL.h"
#include "DlgBrowser.h"
#include "PageRepository.h"
#include "HighTime.h"

/////////////////////////////////////////////////////////////////////////////
// CPageRecomm dialog

class CPageRecomm : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CPageRecomm)

// Construction
public:
	CPageRecomm();
	~CPageRecomm();

// Dialog Data
	//{{AFX_DATA(CPageRecomm)
	enum { IDD = IDD_DIALOG_RECOMM };
	CTabCtrlSSL	m_tabPages;
	//}}AFX_DATA

	CPageRepository *pRepository;

	CHighTime m_timeStart;

	bool m_bEnableNext;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageRecomm)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CDlgBrowser *m_pageBrowsers;

	bool bCompare;

	CStringArray genURLs;
	CStringArray genTitles;
	CStringArray genPages;

protected:
	CImageList m_ilTabs; 

	CString strNoteCompare;
	CString strNoteNoCompare;


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageRecomm)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGERECOMM_H__0E358AC2_6E10_42A6_9F10_29EF1B761492__INCLUDED_)

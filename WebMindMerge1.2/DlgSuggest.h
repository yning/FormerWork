///////////////////////////////////////////////////////////////
// DlgSuggest.h : header file
//
// This software was written by Tingshao Zhu.
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_DLGSUGGEST_H__2D992BCD_5146_4BC4_9176_3AF05A59E34A__INCLUDED_)
#define AFX_DLGSUGGEST_H__2D992BCD_5146_4BC4_9176_3AF05A59E34A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FontStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSuggest dialog
#include "Hyperlink.h"

class CDlgSuggest : public CDialog
{
// Construction
public:
	CDlgSuggest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSuggest)
	enum { IDD = IDD_DIALOG_SUGGEST };
	CFontStatic	m_second;
	CFontStatic	m_first;
	CHyperLink	m_suggest;
	CHyperLink	m_markIC;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSuggest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSuggest)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUGGEST_H__2D992BCD_5146_4BC4_9176_3AF05A59E34A__INCLUDED_)

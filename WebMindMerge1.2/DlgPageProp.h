///////////////////////////////////////////////////////////////
// DlgPageProp.h : header file
//
//
///////////////////////////////////////////////////////////////


#if !defined(AFX_DLGPAGEPROP_H__0DE1B12F_A26D_4E7B_B088_1310917FFDA7__INCLUDED_)
#define AFX_DLGPAGEPROP_H__0DE1B12F_A26D_4E7B_B088_1310917FFDA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDlgPageProp dialog
#include "ViewOnLine.h"

class CDlgPageProp : public CDialog
{
// Construction
public:
	void refreshButtons();
	CDlgPageProp(CViewOnLine *pView);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPageProp)
	enum { IDD = IDD_DIALOG_PAGEPROP };
	CButton	m_okButton;
	CEdit	m_descEdit;
	CEdit	m_labelEdit;
	CString	m_title;
	CString	m_uri;
	BOOL	m_annotated;
	CString	m_label;
	CString	m_desc;
	//}}AFX_DATA

	// the current page view
	CViewOnLine *m_pView;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPageProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPageProp)
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateEditLabel();
	virtual void OnOK();
	afx_msg void OnCheckAnnotated();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPAGEPROP_H__0DE1B12F_A26D_4E7B_B088_1310917FFDA7__INCLUDED_)

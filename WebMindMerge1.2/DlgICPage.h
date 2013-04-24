///////////////////////////////////////////////////////////////
// DlgICPage.h : header file
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_DLGICPAGE_H__68DA793D_60F1_49F7_96D3_7777A34294EB__INCLUDED_)
#define AFX_DLGICPAGE_H__68DA793D_60F1_49F7_96D3_7777A34294EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDlgICPage dialog

class CDlgICPage : public CDialog
{
// Construction
public:
	CDlgICPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgICPage)
	enum { IDD = IDD_DIALOG_ICPAGE };
	CButton	m_okButton;
	CString	m_label;
	CString	m_desc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgICPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void refreshButton();

	// Generated message map functions
	//{{AFX_MSG(CDlgICPage)
	afx_msg void OnUpdateEditLabel();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateEditDesc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGICPAGE_H__68DA793D_60F1_49F7_96D3_7777A34294EB__INCLUDED_)

///////////////////////////////////////////////////////////////
// UploadAgreement.h : header file
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_UPLOADAGREEMENT_H__C7FBCD86_163F_4A79_8388_585389C9B25B__INCLUDED_)
#define AFX_UPLOADAGREEMENT_H__C7FBCD86_163F_4A79_8388_585389C9B25B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabPageSSL.h"
class CDlgUpload;

//#include "DlgUpload.h"

/////////////////////////////////////////////////////////////////////////////
// CUploadAgreement dialog

class CUploadAgreement : public CTabPageSSL
{
// Construction
public:
	void enableOKButton();
	CUploadAgreement(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUploadAgreement)
	enum { IDD = IDD_UPLOAD_AGREEMENT };
	CString	m_agreement;
	//}}AFX_DATA

	CDlgUpload *pDlgParent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUploadAgreement)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUploadAgreement)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPLOADAGREEMENT_H__C7FBCD86_163F_4A79_8388_585389C9B25B__INCLUDED_)

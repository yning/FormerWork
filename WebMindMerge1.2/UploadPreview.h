///////////////////////////////////////////////////////////////
// UploadPreview.h : header file
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_UPLOADPREVIEW_H__0E48A273_944B_4639_AF3E_E763E36291C2__INCLUDED_)
#define AFX_UPLOADPREVIEW_H__0E48A273_944B_4639_AF3E_E763E36291C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabPageSSL.h"
#include "Markup.h"
#include "List.h"
#include "ViewOnLine.h"

/////////////////////////////////////////////////////////////////////////////
// CUploadPreview dialog

class CUploadPreview : public CTabPageSSL
{
// Construction
public:
	CUploadPreview(CWnd* pParent = NULL);   // standard constructor

	void readLogXML(const CString &m_logfile, HTREEITEM tItem);
	void loadHistory();

// Dialog Data
	//{{AFX_DATA(CUploadPreview)
	enum { IDD = IDD_UPLOAD_PREVIEW };
	CButton	m_ctrlRemove;
	CTreeCtrl	m_tHistory;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUploadPreview)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUploadPreview)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDelete();
	afx_msg void OnSelchangedTreeHistory(NMHDR* , LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_UPLOADPREVIEW_H__0E48A273_944B_4639_AF3E_E763E36291C2__INCLUDED_)

///////////////////////////////////////////////////////////////
// DlgBrowser.h : header file
//
//
///////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "webbrowser.h"
//}}AFX_INCLUDES
#if !defined(AFX_DLGBROWSER_H__016E3D43_419D_43D5_B798_8DAC6E1E2BFF__INCLUDED_)
#define AFX_DLGBROWSER_H__016E3D43_419D_43D5_B798_8DAC6E1E2BFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabPageSSL.h"
#include "PageRepository.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBrowser dialog

class CDlgBrowser : public CTabPageSSL
{
// Construction
public:
	void GoToURL(const CString &strURL);
	void loadGoogleCache();
	CDlgBrowser(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBrowser)
	enum { IDD = IDD_DIALOG_BROWSER }; //by ny
	CWebBrowser	m_browserCtrl;
	//}}AFX_DATA

	CPageRepository *pRepository;
	CPageCache *pCache;

	// whether this page is for comparison
	// if yes, we donot load it into the repository
	bool bCompare;

	bool bCacheLoaded;
	CString m_strURL;

	bool bFirstNavigation;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBrowser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBrowser)
	afx_msg void OnDocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT FAR* URL);
	afx_msg void OnNewWindow2Explorer(LPDISPATCH FAR* , BOOL FAR* Cancel);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBROWSER_H__016E3D43_419D_43D5_B798_8DAC6E1E2BFF__INCLUDED_)

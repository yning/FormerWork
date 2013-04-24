#include "ChartCtrl/ChartCtrl.h"
#include "ChartCtrl/ChartLineSerie.h"
#include "resource.h" //
#if !defined(AFX_NEWDLGBAR_H__FEEA7D85_DA14_479D_8593_92E4156D25FC__INCLUDED_)
#define AFX_NEWDLGBAR_H__FEEA7D85_DA14_479D_8593_92E4156D25FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewDlgBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewDlgBar dialog

class CNewDlgBar : public CDialogBar
{
// Construction
public:
	CNewDlgBar(CWnd* pParent = NULL);   // standard constructor
	virtual BOOL Create(CWnd* pParentWnd,UINT nIDTemplate,UINT nStyle,UINT nID);
// Dialog Data
	//{{AFX_DATA(CNewDlgBar)
	enum { IDD = IDD_NEWDLGBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CChartCtrl m_ChartCtrl;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewDlgBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewDlgBar)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG

	afx_msg BOOL OnInitDialog ();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWDLGBAR_H__FEEA7D85_DA14_479D_8593_92E4156D25FC__INCLUDED_)

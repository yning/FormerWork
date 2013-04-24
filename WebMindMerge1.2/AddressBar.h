#if !defined(AFX_ADDRESSBAREX_H__BC428CD6_503C_4B2D_BEB6_D82460A90644__INCLUDED_)
#define AFX_ADDRESSBAREX_H__BC428CD6_503C_4B2D_BEB6_D82460A90644__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddressBarEx.h : header file
//

#include <afxole.h>

class CAddURLDropTarget : public COleDropTarget  
{
public:
	CAddURLDropTarget();
	virtual ~CAddURLDropTarget();
	DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
                                                dwKeyState, CPoint point );
   
	DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
										        dwKeyState, CPoint point );
  	void OnDragLeave(CWnd* pWnd);
   	BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT 
                                          dropEffect, CPoint point );

};


/////////////////////////////////////////////////////////////////////////////
// CAddressBar window

class CAddressBar : public CToolBar
{
// Construction
public:
	CAddressBar();

// Attributes
public:

private:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddressBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	CAddURLDropTarget m_dropTarget;
	BOOL ShowGoButton;
	int FindStringExact(int start, CString& str);
	void GetSelText(int index, CString& str);
	CComboBoxEx* GetAddressBox();
	CComboBoxEx m_wndAddress;
	virtual ~CAddressBar();
	void Init();

	CEdit *GetEditCtrl()	{ return m_wndAddress.GetEditCtrl(); }

	// Generated message map functions
protected:
	//{{AFX_MSG(CAddressBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetInfoTip( NMHDR* pNMHDR, LRESULT* pResult );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDRESSBAREX_H__BC428CD6_503C_4B2D_BEB6_D82460A90644__INCLUDED_)

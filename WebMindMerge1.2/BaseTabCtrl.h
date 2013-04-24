#if !defined(AFX_BASETABCTRL_H__1E6E4FE9_BE01_4DA1_AFA9_A98527A3769B__INCLUDED_)
#define AFX_BASETABCTRL_H__1E6E4FE9_BE01_4DA1_AFA9_A98527A3769B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseTabCtrl window

class CBaseTabCtrl : public CTabCtrl
{
// Construction
public:
	CBaseTabCtrl();
	void SetTabStyle(int nType);
	void SetTabPos(int bTopTab); 
	void SetUnSelColor(COLORREF color);
	void SetSelFontColor(COLORREF crSelFontColor);
	void SetUnSelFontColor(COLORREF crUnSelFontColor);
	void SetSepColor(COLORREF crSepColor);
	int logging(CString funName,CString paraName,CString paraValue);
//属性
public:
	BOOL isCallFromLoop;//这个是用来表明在OnPaint函数里，调用drawItem函数到底是正常调用还是对于未被选择的标签页的调用，因此在while循环里设置isCallFromLoop来标识，从而使得在drawItem函数里好分清楚。
protected:
	COLORREF m_crUnSelColor;
	COLORREF m_crUnSelFontColor;
	COLORREF m_crSelFontColor;
	COLORREF m_crSepColor;

	int m_nTabStyle;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseTabCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();//在注释宏之间的声明在VC中灰色显示。afx_msg宏表示声明的是一个消息响应函数。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawMainBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItemBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual COLORREF GetTabColor(BOOL bSelected = FALSE);
	virtual COLORREF GetTabTextColor(BOOL bSelected = FALSE);
	virtual void PreSubclassWindow();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASETABCTRL_H__1E6E4FE9_BE01_4DA1_AFA9_A98527A3769B__INCLUDED_)

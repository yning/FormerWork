///////////////////////////////////////////////////////////////
// WindowTabCtrl.h : header file
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWTABCTRL_H__C67D43AA_F890_11D1_83BA_0000B43382FE__INCLUDED_)
#define AFX_WINDOWTABCTRL_H__C67D43AA_F890_11D1_83BA_0000B43382FE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CWindowTabCtrl window

class CWindowTabCtrl : public CTabCtrl
{
private:
	BOOL m_bDisplayIcons;
// Construction
public:
	CWindowTabCtrl();

// Attributes
public:

	bool m_bMouseOver;
	bool m_bMouseDown;
	bool m_bMouseUp;

// Operations
public:
	void SetFonts(CFont* pSelFont, CFont* pUnselFont);
	void SetFonts(CFont* pFont, BOOL bSelFont);
	void SetFonts(int nSelWeight = FW_SEMIBOLD, BOOL bSelItalic = FALSE,   
		BOOL bSelUnderline = FALSE, int nUnselWeight = FW_MEDIUM, 
		BOOL bUnselItalic = FALSE, BOOL bUnselUnderline = FALSE);

	CFont* GetTabFont(BOOL bSelFont);

	void SetColors(COLORREF bSelColor, COLORREF bUnselColor, COLORREF rcDocModified);
	COLORREF GetSelColor() const;
	COLORREF GetUnSelColor() const;
	COLORREF GetDocModifiedColor() const;

	void SetIconDisplay(BOOL bDisplayIt = TRUE);

	void SetTabStyle(BOOL bButtons, BOOL bFlatButtons);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindowTabCtrl)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWindowTabCtrl();

// Implementation
protected:
	COLORREF m_crSelColor, m_crUnselColor, m_crDocModified;
	CFont    m_SelFont,	   m_UnselFont;

	// Generated message map functions
protected:
	//{{AFX_MSG(CWindowTabCtrl)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CWindowTabCtrl inlines

inline CFont* CWindowTabCtrl::GetTabFont(BOOL bSelFont)
{
	if (bSelFont)
		return &m_SelFont;
	else
		return &m_UnselFont;
}

inline COLORREF CWindowTabCtrl::GetSelColor() const
{
	return m_crSelColor;
}

inline COLORREF CWindowTabCtrl::GetUnSelColor() const
{
	return m_crUnselColor;
}

inline COLORREF CWindowTabCtrl::GetDocModifiedColor() const
{
	return m_crDocModified;
}

#endif // !defined(AFX_WINDOWTABCTRL_H__C67D43AA_F890_11D1_83BA_0000B43382FE__INCLUDED_)

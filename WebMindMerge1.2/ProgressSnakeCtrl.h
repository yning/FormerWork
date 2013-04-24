//////
// EZSnakeCtrl - An Indefinite progressctrl like netscape's status bar progress
// Copyright V.Lakshmi Narasimhan,ezlux@Yahoo.com.
// Feel free to use,modify,twist,turn or even 
// digest the code for any non commercial purposes.
// I would appreciate constructive suggestions & bug reports.
// Please dont delete the above lines.
///////
#if !defined(AFX_PROGRESSSNAKECTRL_H__0FB972C1_9D3E_11D1_9D77_90672FE32418__INCLUDED_)
#define AFX_PROGRESSSNAKECTRL_H__0FB972C1_9D3E_11D1_9D77_90672FE32418__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressSnakeCtrl.h : header file
//

#include "MemDC.h"

/////////////////////////////////////////////////////////////////////////////
// CProgressSnakeCtrl window

class CProgressSnakeCtrl : public CProgressCtrl
{
// Construction
public:
	CProgressSnakeCtrl();
	virtual ~CProgressSnakeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressSnakeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetMax();
	void ResetProgress();
	int GetTail();
	int GetSize();
	int GetStep();
	int GetPos();

	void SetSnakeStyle(BOOL bSnake = TRUE)
	{
		m_bSnakeMode = bSnake;
	};


	// Generated message map functions
protected:
	void SafeStepTail();
	void SafeStepHead();
	void DrawGradient(CMemDC *pdc, CRect rcLeft, CRect rcGrad, CRect rcRight);

	//{{AFX_MSG(CProgressSnakeCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
    afx_msg LRESULT   OnGetPos(WPARAM, LPARAM);
    afx_msg LRESULT   OnSetPos(WPARAM newPos, LPARAM);
    afx_msg LRESULT   OnStepIt(WPARAM, LPARAM);
    afx_msg void      OnSetRange(WPARAM, LPARAM range);
    afx_msg void      OnSetRange32(WPARAM lower, LPARAM higher);
    afx_msg LRESULT   OnGetRange(WPARAM Type, LPARAM PBRange);
    afx_msg LRESULT   OnSetStep(WPARAM size, LPARAM);

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bTimeForTail;
	BOOL m_bReverse;

	BOOL m_bSnakeMode;

	int m_nStep;
	int m_nPos;
	int m_nTail;
	int m_nSize;
	int m_nMax;
	int m_nMin;

	COLORREF m_clrBg;
	COLORREF m_clrFg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSSNAKECTRL_H__0FB972C1_9D3E_11D1_9D77_90672FE32418__INCLUDED_)

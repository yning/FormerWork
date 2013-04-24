/*********************************************************************
	Copyright (C) 2002 Mustafa Demirhan :)
	
	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.

	3. This notice may not be removed or altered from any source distribution.

	http://www.macroangel.com
	admin@macroangel.com
**********************************************************************/

#if !defined(AFX_DIALOGHEADERCTRL_H__F960E9BB_E879_4E51_BE53_D554C59A1BEA__INCLUDED_)
#define AFX_DIALOGHEADERCTRL_H__F960E9BB_E879_4E51_BE53_D554C59A1BEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogHeaderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogHeaderCtrl window
#include "cpara.h"

class CDialogHeaderCtrl : public CStatic
{
protected:
	CFont		m_BoldFont;
	CFont		m_NormFont;
	CString		m_sTitle, m_sDesc;
	int			m_nLeft, m_nTop;
	HICON		hMainIcon;
	CParagraph	m_Paragraph;

	void UpdateFont(CFont& font, LONG lfWeight = FW_NORMAL);
	
	// Construction
public:
	CDialogHeaderCtrl ();
	CDialogHeaderCtrl(const CString &sTitle, const CString &sDesc, UINT nIcon);
	
	// Attributes
public:
	
	// Operations
public:
	BOOL SetBkImage(UINT nIDResource);
	BOOL SetBkImage(LPCTSTR lpszResourceName);
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogHeaderCtrl)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	void SetIcon (UINT id);
	void SetDesc (const CString &str);
	void SetTitle (const CString &str);
	virtual ~CDialogHeaderCtrl();
	
	// Generated message map functions
protected:
	
	//{{AFX_MSG(CDialogHeaderCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGHEADERCTRL_H__F960E9BB_E879_4E51_BE53_D554C59A1BEA__INCLUDED_)

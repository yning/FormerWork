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

#include "stdafx.h"
#include "DialogHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  DISPLAY_TIMER_ID		150		// timer id
/////////////////////////////////////////////////////////////////////////////
// CDialogHeaderCtrl

CDialogHeaderCtrl::CDialogHeaderCtrl ():
m_nLeft (50), m_nTop (5)
{
	UpdateFont (m_NormFont);
	UpdateFont (m_BoldFont, FW_BOLD);

	hMainIcon = NULL;
	m_Paragraph.SetText ("");
	m_Paragraph.SetJustification (CParagraph::left);
}

CDialogHeaderCtrl::CDialogHeaderCtrl (const CString &sTitle, const CString &sDesc, UINT nIcon):
m_nLeft (50), m_nTop (5)
{
	UpdateFont (m_NormFont);
	UpdateFont (m_BoldFont, FW_BOLD);
	
	m_sTitle = sTitle;
	m_sDesc = sDesc;
	m_Paragraph.SetText (m_sDesc);
	m_Paragraph.SetJustification (CParagraph::left);
	
	hMainIcon = AfxGetApp ()->LoadIcon (nIcon);
}

CDialogHeaderCtrl::~CDialogHeaderCtrl ()
{
	if (hMainIcon)
		DestroyIcon (hMainIcon);
}

BEGIN_MESSAGE_MAP(CDialogHeaderCtrl, CStatic)
	//{{AFX_MSG_MAP(CDialogHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogHeaderCtrl message handlers

void CDialogHeaderCtrl::UpdateFont(CFont& font, LONG lfWeight)
{
	font.DeleteObject();
	
    font.CreateFont( 13, 0, 0, 0, lfWeight, 0,
		0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
		VARIABLE_PITCH | 0x04 | FF_DONTCARE, (LPSTR)"Tahoma");
}

void CDialogHeaderCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect (rect);
	//ScreenToClient (rect);
	
	dc.Draw3dRect (rect.left, rect.top, rect.Width () + 2, rect.Height (), GetSysColor (COLOR_BTNHIGHLIGHT), GetSysColor (COLOR_BTNSHADOW));
	dc.FillSolidRect(rect.left, rect.top, rect.Width () + 2, rect.Height () - 1, GetSysColor(COLOR_WINDOW));
	
	if (hMainIcon)
		dc.DrawIcon (CPoint (m_nTop, m_nTop), hMainIcon);
	
	dc.SelectObject (&m_BoldFont);
	dc.TextOut (rect.left + m_nLeft + 2, rect.top + m_nTop, m_sTitle);
	dc.SelectObject (&m_NormFont);
	
	m_Paragraph.SetPosition (rect.left + m_nLeft, rect.top + m_nTop + 11);
	m_Paragraph.SetWidth (rect.Width () - m_nLeft - 1);
	m_Paragraph.SetHeight (rect.Height () - m_nTop);
	
	m_Paragraph.Paint (&dc);
}

BOOL CDialogHeaderCtrl::OnEraseBkgnd (CDC* pDC) 
{
	return TRUE;
}

void CDialogHeaderCtrl::OnDestroy () 
{
	CStatic::OnDestroy();
}

BOOL CDialogHeaderCtrl::SetBkImage (UINT nIDResource)
{
	return TRUE;
}

BOOL CDialogHeaderCtrl::SetBkImage (LPCTSTR lpszResourceName)
{
    return TRUE;
}

void CDialogHeaderCtrl::SetTitle (const CString &str)
{
	m_sTitle = str;
	Invalidate ();
}

void CDialogHeaderCtrl::SetDesc (const CString &str)
{
	m_sDesc = str;
	m_Paragraph.SetText (m_sDesc);
	Invalidate ();
}

void CDialogHeaderCtrl::SetIcon (UINT id)
{
	if (hMainIcon)
		DestroyIcon (hMainIcon);
	
	if (id)
		hMainIcon = AfxGetApp ()->LoadIcon (id);
	else
		hMainIcon = NULL;

	Invalidate ();
}

void CDialogHeaderCtrl::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CStatic::OnShowWindow(bShow, nStatus);
}

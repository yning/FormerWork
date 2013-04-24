///////////////////////////////////////////////////////////////
// WindowTabCtrl.cpp : implementation file
//
// This software was written by Tingshao Zhu.
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"       // main symbols
#include "WindowTabCtrl.h"
#include "ViewManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define CX_BORDER  1
#define CY_BORDER  1

/////////////////////////////////////////////////////////////////////////////
// CWindowTabCtrl

CWindowTabCtrl::CWindowTabCtrl() : m_bDisplayIcons(TRUE)
{
	m_crSelColor    = RGB(  0,  0, 255);
	m_crUnselColor  = RGB( 50, 50,  50);
	m_crDocModified = RGB(255,  0,   0);
	
	// Get the current font
	LOGFONT lFont;
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 
		sizeof(NONCLIENTMETRICS), &ncm, 0));
	lFont = ncm.lfMessageFont; 
	
	// Create the "Selected" font
	m_SelFont.CreateFontIndirect(&lFont);
	// Create the "UnSelected" font
	m_UnselFont.CreateFontIndirect(&lFont);

	m_bMouseOver = false;
	m_bMouseDown = false;
	m_bMouseUp = false;

}

CWindowTabCtrl::~CWindowTabCtrl()
{
	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWindowTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CWindowTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowTabCtrl message handlers

void CWindowTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (pNMHDR->hwndFrom == NULL)
		return;                    // just to avoid level 4 compilation warning

	int idx = GetCurSel();

	TCITEM tci;
	tci.mask = TCIF_PARAM;
	GetItem(idx, &tci);
	CView* pView = reinterpret_cast<CView *>(tci.lParam);
	static_cast<CMDIFrameWnd*>(AfxGetMainWnd())->MDIActivate(pView->GetParent());
	*pResult = 0;
}

void CWindowTabCtrl::SetIconDisplay(BOOL bDisplayIt)
{
	m_bDisplayIcons = bDisplayIt;
}

int CWindowTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
	{
		TRACE0(_T("OnCreate function call failed!"));
		return -1;
	}
	ModifyStyle(0, TCS_OWNERDRAWFIXED);
	
	HFONT hFont  = reinterpret_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));
	CFont* pFont = CFont::FromHandle(hFont);
	SetFont(pFont);

	SetMinTabWidth( 180 );
	
	return 0;
}

void CWindowTabCtrl::PreSubclassWindow() 
{
	CTabCtrl::PreSubclassWindow();
//	ModifyStyle(0, TCS_OWNERDRAWFIXED); //TODO must go!
}


void CWindowTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	
	CRect rcTab   =  CRect(&(lpDrawItemStruct->rcItem));
	int nTabIndex = lpDrawItemStruct->itemID;
	if (nTabIndex < 0)
		return;
	
	BOOL bSelected = (nTabIndex == GetCurSel());
	
	TCHAR szLabel[_MAX_PATH];
	TCITEM tci;
	tci.mask       = TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM;
	tci.pszText    = szLabel;     
	tci.cchTextMax = _MAX_PATH - 1;    	
	if (!GetItem(nTabIndex, &tci)) 
		return;
	
	CDC dc;
	if (!dc.Attach(lpDrawItemStruct->hDC)) 
		return;

	DWORD dwStyle = GetStyle();
    rcTab.NormalizeRect();
	if ((dwStyle & TCS_BOTTOM) == 0)
	{
		rcTab.DeflateRect(CX_BORDER, CY_BORDER);
		rcTab.top += ::GetSystemMetrics(SM_CYEDGE);
	}
	
	dc.SetBkMode(TRANSPARENT);
	dc.FillSolidRect(rcTab, ::GetSysColor(COLOR_BTNFACE));//remove this line for WinXp
	
	CView* pView    = reinterpret_cast<CView*>(tci.lParam);
	CDocument* pDoc = pView->GetDocument();

	POINT pt;
	bool bCloseView = false;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	// Draw image
	if (m_bDisplayIcons)
	{
		CImageList* pImageList = GetImageList();
		if (pImageList && tci.iImage >= 0) 
		{
			rcTab.left += dc.GetTextExtent(_T(" ")).cx;		// Set a little margin
			
			// Get the height of image 
			IMAGEINFO info;
			pImageList->GetImageInfo(tci.iImage, &info);
			CRect ImageRect(info.rcImage);
			int nYpos = rcTab.top;

			pImageList->Draw(&dc, tci.iImage, CPoint(rcTab.left, nYpos), ILD_TRANSPARENT);
			rcTab.left += ImageRect.Width();

			CRect rectBtn;
			rectBtn.left = rcTab.right-ImageRect.Width()-4;
			rectBtn.top = nYpos;
			rectBtn.bottom = rectBtn.top + ImageRect.Height();
			rectBtn.right = rectBtn.left + ImageRect.Width();
			if (rectBtn.PtInRect(pt))
			{
				if(m_bMouseDown)
					pImageList->Draw(&dc, 3, CPoint(rcTab.right-ImageRect.Width()-4 , nYpos), ILD_TRANSPARENT);
				else
					pImageList->Draw(&dc, 2, CPoint(rcTab.right-ImageRect.Width()-4 , nYpos), ILD_TRANSPARENT);

				if(m_bMouseUp)
				{
					m_bMouseUp = false;
					bCloseView = true;
					pImageList->Draw(&dc, 2, CPoint(rcTab.right-ImageRect.Width()-4 , nYpos), ILD_TRANSPARENT);
				}
			}
			else
			{
				if(m_bMouseUp )
					m_bMouseDown = false;

				m_bMouseUp = false;
				pImageList->Draw(&dc, 2, CPoint(rcTab.right-ImageRect.Width()-4 , nYpos), ILD_TRANSPARENT);
			}
		}
	}
	
	if (bSelected) 
	{
		if (pDoc->IsModified())
			dc.SetTextColor(m_crDocModified);
		else
			dc.SetTextColor(m_crSelColor);
		dc.SelectObject(&m_SelFont);
		rcTab.top -= ::GetSystemMetrics(SM_CYEDGE);
	} 
	else 
	{
		if (pDoc->IsModified())
			dc.SetTextColor(m_crDocModified);
		else
			dc.SetTextColor(m_crUnselColor);
		dc.SelectObject(&m_UnselFont);
	}

	rcTab.left += dc.GetTextExtent(_T(" ")).cx;		// Set a little margin
	
	dc.DrawText(szLabel, rcTab, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	dc.Detach();

	if(bCloseView)
	{
		int idx = GetCurSel();
		TCITEM tci;
		tci.mask = TCIF_PARAM;
		GetItem(idx, &tci);
		CView* pView = reinterpret_cast<CView *>(tci.lParam);
		if(pView)
			pView->PostMessage(WM_COMMAND,ID_FILE_CLOSE,0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWindowTabCtrl operations

void CWindowTabCtrl::SetColors(COLORREF crSelColor, COLORREF crUnselColor, 
							   COLORREF crDocModified)
{
	if ((crSelColor != CLR_NONE) || (crSelColor != CLR_DEFAULT))
		m_crSelColor    = crSelColor;
	if ((crUnselColor != CLR_NONE) || (crUnselColor != CLR_DEFAULT))
		m_crUnselColor  = crUnselColor;
	if ((crDocModified != CLR_NONE) || (crDocModified != CLR_DEFAULT))
		m_crDocModified = crDocModified;

	Invalidate();
}

void CWindowTabCtrl::SetFonts(CFont* pSelFont, CFont* pUnselFont)
{
	ASSERT(pSelFont && pUnselFont);

	LOGFONT lFont;
	int nSelHeight, nUnselHeight;

	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();

	pSelFont->GetLogFont(&lFont);
	m_SelFont.CreateFontIndirect(&lFont);
	nSelHeight = lFont.lfHeight;

	pUnselFont->GetLogFont(&lFont);
	m_UnselFont.CreateFontIndirect(&lFont);
	nUnselHeight = lFont.lfHeight;

	SetFont((nSelHeight > nUnselHeight) ? &m_SelFont : &m_UnselFont);
}

void CWindowTabCtrl::SetFonts(CFont* pFont, BOOL bSelFont)
{
	ASSERT(pFont);

	LOGFONT lFont;

	if (bSelFont)
	{
		m_SelFont.DeleteObject();
		pFont->GetLogFont(&lFont);
		m_SelFont.CreateFontIndirect(&lFont);
		SetFont(&m_SelFont);
	}
	else
	{
		m_UnselFont.DeleteObject();
		pFont->GetLogFont(&lFont);
		m_UnselFont.CreateFontIndirect(&lFont);
		SetFont(&m_UnselFont);
	}
}

void CWindowTabCtrl::SetFonts(int nSelWeight,   BOOL bSelItalic,   BOOL bSelUnderline,
						  int nUnselWeight, BOOL bUnselItalic, BOOL bUnselUnderline)
{
	// Free any memory currently used by the fonts.
	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();

	// Get the current font
	LOGFONT lFont;
	CFont* pFont = GetFont();
	if (pFont)
		pFont->GetLogFont(&lFont);
	else 
	{
		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 
			sizeof(NONCLIENTMETRICS), &ncm, 0));
		lFont = ncm.lfMessageFont; 
	}

	// Create the "Selected" font
	lFont.lfWeight    = static_cast<LONG>(nSelWeight);
	lFont.lfItalic    = static_cast<BYTE>(bSelItalic);
	lFont.lfUnderline = static_cast<BYTE>(bSelUnderline);
	m_SelFont.CreateFontIndirect(&lFont);

	// Create the "Unselected" font
	lFont.lfWeight    = static_cast<LONG>(nUnselWeight);
	lFont.lfItalic    = static_cast<BYTE>(bUnselItalic);
	lFont.lfUnderline = static_cast<BYTE>(bUnselUnderline);
	m_UnselFont.CreateFontIndirect(&lFont);

	SetFont((nSelWeight > nUnselWeight) ? &m_SelFont : &m_UnselFont);
}

void CWindowTabCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// Select the tab which is clicked and activate the attached view
    try
	{
		TCHITTESTINFO   hti;
		hti.pt = CPoint(GetMessagePos());
		ScreenToClient(&hti.pt);
		int iTab = HitTest(&hti);
		if (iTab < 0)
			return;
		SetCurSel(iTab);                    // Select it
		
		TCITEM tci;
		tci.mask = TCIF_PARAM;
		GetItem(iTab, &tci);
		CView* pView = reinterpret_cast<CView*>(tci.lParam);
		static_cast<CMDIFrameWnd*>(AfxGetMainWnd())->MDIActivate(pView->GetParent());
		
		// Now, process the popup menu for the tab control
		POINT  ptScreen = point;
		// Convert the mouse point to screen coordinates since that is what is used by
		// the TrackPopupMenu() function.
		ClientToScreen(&ptScreen);
		CMenu *popupMenu;
		CMenu menu;
		menu.LoadMenu(IDR_TAB_MENU);
		if(menu)
		{
			popupMenu=menu.GetSubMenu(0);
			ASSERT(popupMenu);
			popupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, AfxGetMainWnd());
			popupMenu->DestroyMenu();
		}
	}
	catch(...)
	{
	}
	

	CTabCtrl::OnRButtonDown(nFlags, point);
}

void CWindowTabCtrl::SetTabStyle(BOOL bButtons, BOOL bFlatButtons)
{
	DWORD dwAdd          = 0;
	DWORD dwCurrentStyle = GetStyle();

	if (!bButtons && (dwCurrentStyle & TCS_BUTTONS))
	{
		ModifyStyle(TCS_BUTTONS | TCS_FLATBUTTONS, 0, 0);
		return;
	}
	else if (!bButtons)
		return;
		
	if (bButtons && (dwCurrentStyle & TCS_BUTTONS) && 
		(dwCurrentStyle & TCS_FLATBUTTONS))
		return;

	if (bButtons)
		dwAdd |= TCS_BUTTONS;
	if (bFlatButtons)
		dwAdd |= TCS_FLATBUTTONS;
	ModifyStyle(0, dwAdd, 0);
}

void CWindowTabCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDblClk(nFlags, point);
	return;

	// TODO: Add your message handler code here and/or call default
	int idx = GetCurSel();
	TCITEM tci;
	tci.mask = TCIF_PARAM;
	GetItem(idx, &tci);
	CView* pView = reinterpret_cast<CView *>(tci.lParam);
	if(pView)
		pView->PostMessage(WM_COMMAND,ID_FILE_CLOSE,0);
	else
		MessageBox("error when closing the view");
	CTabCtrl::OnLButtonDblClk(nFlags, point);
}


void CWindowTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rectItem;
	for(int m_iIndex = 0; m_iIndex < GetItemCount(); m_iIndex++)
	{
		GetItemRect(m_iIndex, rectItem);
		if (rectItem.PtInRect(point))
		{
			m_bMouseDown = TRUE;
			InvalidateRect(rectItem);
			break;
		}
	}

	CTabCtrl::OnLButtonDown(nFlags, point);
	return;
}

void CWindowTabCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bMouseOver = false;
	m_bMouseDown = false;

	m_bMouseUp = TRUE;

	// TODO: Add your message handler code here and/or call default
	CRect rectItem;
	for(int m_iIndex = 0; m_iIndex < GetItemCount(); m_iIndex++)
	{
		GetItemRect(m_iIndex, rectItem);
		if (rectItem.PtInRect(point))
		{
			InvalidateRect(rectItem);
			return;
		}
	}

	CTabCtrl::OnLButtonUp(nFlags, point);
	return;
}

void CWindowTabCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_bMouseOver = true;

	TCHITTESTINFO hitTest;
	hitTest.pt = point;

	int iItem = HitTest(&hitTest);
	if (iItem != -1)
	{
		RECT rectItem;
		GetItemRect(iItem, &rectItem);
		InvalidateRect(&rectItem);
		return;
	}

	CTabCtrl::OnMouseMove(nFlags, point);
	return;
}


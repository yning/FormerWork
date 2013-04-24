///////////////////////////////////////////////////////////////
// ViewManager.cpp : implementation file
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"       // main symbols
#include "ViewManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_VIEWTAB           1005
#define ID_DEFDOCTIPS        _T("A new unsaved file")
#define ID_DEFCAPTION        _T("Open File Tabs Bar")
#define ID_TABHEIGHT         26

//
// This function adds a title entry to a popup menu
//
void AddMenuTitle(CMenu* popup, LPCSTR title)
{
    // insert a separator item at the top
    popup->InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR, 0, title);

    // insert title item
    // note: item is not selectable (disabled) but not grayed
    popup->InsertMenu(0, MF_BYPOSITION | MF_STRING | MF_DISABLED, 0, title);

    SetMenuDefaultItem(popup->m_hMenu, 0, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CVViewManager

IMPLEMENT_DYNAMIC(CViewManager, CControlBar)

CViewManager::CViewManager()
{
	m_bClosing      = FALSE;
	m_nLMargin      = 10;
	m_nDockID       = 0;
	m_bWin2000      = TRUE;
	m_beAddLast     = TRUE;

	m_currentSearchPos = -1;
}

CViewManager::~CViewManager()
{
	m_arViews.RemoveAll();
	m_arViewTitles.RemoveAll();
}

BEGIN_MESSAGE_MAP(CViewManager, CControlBar)
	//{{AFX_MSG_MAP(CViewManager)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TTN_NEEDTEXT, 0, OnViewManagerToolTip)
END_MESSAGE_MAP()

void CViewManager::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	try
	{
		CMDIFrameWnd* pFrame = static_cast<CMDIFrameWnd *>(AfxGetApp()->m_pMainWnd);
		if (pFrame == NULL)
			return;        // this is not meant for us...
		
		// Get the active MDI child window.
		CMDIChildWnd* pChild = static_cast<CMDIChildWnd *>(pFrame->GetActiveFrame());
		
		// Get the active view attached to the active MDI child window.
		CView* pActiveView = reinterpret_cast<CView *>(pChild->GetActiveView());
		
		if (pActiveView == NULL) //...Is there a view anyway?
		{
			//...since there is no view hide the tab control, otherwise it looks...guess!
			m_ViewTabCtrl.ShowWindow(SW_HIDE);
			return;
		}
		else
		{
			//...we might have hidden the tab control, show it now
			if (!m_ViewTabCtrl.IsWindowVisible())
				m_ViewTabCtrl.ShowWindow(SW_SHOW);
		}
		
		// Now, we go...
		int iSel = -1;
	//	CString strWin;
		
	//	CMainFrame * mainfrm=(CMainFrame * )pFrame;
		
		for (int t = 0; t < m_arViewTitles.GetSize(); t++)
		{
			CView* pViewAt = static_cast<CView *>(m_arViews.GetAt(t));
			ASSERT(pViewAt!=NULL);

			// ...if there is window title change since the last update set the new title
			// ...find the active view from the view list
			if (pActiveView == static_cast<CView *>(m_arViews.GetAt(t))) 
				iSel = t;
		}
		m_ViewTabCtrl.SetCurSel(iSel);   // set the tab for the active view
	}
	catch(...)
	{
		MessageBox("error");
	}
	// Be polite! update the dialog controls added to the CSizingControlBar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

////////////////////////////////////////////////////////////////////////////
// CViewManager	operations

void CViewManager::AddView(const TCHAR* csName, CView* pView)
{
	if (m_bClosing) 
		return;

	CString cs(csName);

	int index=m_ViewTabCtrl.GetCurSel();
	if(index==-1||m_beAddLast)
		index=m_ViewTabCtrl.GetItemCount();
	else
		index++;
	m_arViews.InsertAt(index,pView);
	m_arViewTitles.InsertAt(index,cs);
    
	if (m_ViewTabCtrl.GetSafeHwnd())
	{
		TCITEM tci;
		tci.mask    = TCIF_TEXT | TCIF_PARAM | TCIF_IMAGE;
		tci.pszText = cs.LockBuffer();
		tci.lParam  = reinterpret_cast<LPARAM>(pView);
		tci.iImage  = 0;
		m_ViewTabCtrl.InsertItem(index, &tci);
		cs.UnlockBuffer();
	}

	CToolTipCtrl *pTT=m_ViewTabCtrl.GetToolTips();
	for(int i=m_ViewTabCtrl.GetItemCount()-1;i>index;i--)
	{
		pTT->UpdateTipText(m_arViewTitles.GetAt(i),&m_ViewTabCtrl,i);
	}
}

void CViewManager::RemoveView(CView* pView)
{
	try
	{
		if (m_bClosing || m_arViews.GetSize() <= 0) 
			return;
		
		int nTabs;
		
		if (m_ViewTabCtrl.GetSafeHwnd())
		{
			for (nTabs = 0; nTabs < m_ViewTabCtrl.GetItemCount(); nTabs++)
			{
				TCITEM tci;
				tci.mask = TCIF_PARAM;
				m_ViewTabCtrl.GetItem(nTabs, &tci);
				if (tci.lParam == reinterpret_cast<LPARAM>(pView))
				{
					m_ViewTabCtrl.DeleteItem(nTabs);
					break;
				}
			}
		}
		
		for (nTabs = 0; nTabs < m_arViews.GetSize(); nTabs++)
		{
			if (static_cast<CView *>(m_arViews.GetAt(nTabs)) == pView)
			{
				m_arViewTitles.RemoveAt(nTabs);
				m_arViews.RemoveAt(nTabs);
				return;
			}
		}
	}
	catch(...)
	{
	}
}

void CViewManager::RemoveAll()
{
	m_arViews.RemoveAll();
	m_arViewTitles.RemoveAll();
}

void CViewManager::SetViewName(const TCHAR* cs, CView* pView)
{
	if (m_bClosing || m_arViews.GetSize() <= 0) 
		return;

	int nTabs;
	CString csName(cs);
	CString csName2=csName;
	if(csName2.GetLength() > 22)
		csName2=csName2.Left(22)+"...";
	if (m_ViewTabCtrl.GetSafeHwnd())
	{
		for (nTabs = 0; nTabs < m_ViewTabCtrl.GetItemCount(); nTabs++)
		{
			TCITEM tci;
			tci.mask = TCIF_PARAM;
			m_ViewTabCtrl.GetItem(nTabs, &tci);
			if (tci.lParam == reinterpret_cast<LPARAM>(pView))
			{
				tci.mask = TCIF_PARAM | TCIF_TEXT;
				tci.pszText = csName2.LockBuffer();
				m_ViewTabCtrl.SetItem(nTabs, &tci);
				csName2.UnlockBuffer();
				m_ViewTabCtrl.Invalidate();
				m_ViewTabCtrl.GetToolTips()->UpdateTipText(csName,&m_ViewTabCtrl,nTabs);
				break;
			}
		}
	}

	for (nTabs = 0; nTabs < m_arViews.GetSize(); nTabs++)
	{
		if (static_cast<CView *>(m_arViews.GetAt(nTabs)) == pView)
		{
			m_arViewTitles.SetAt(nTabs, csName);
			return;
		}
	}
}

int CViewManager::GetWindowNum()
{
	return m_arViews.GetSize();
}

void CViewManager::OnActivateView(const BOOL bActivate, CView* pView)
{
	if (bActivate)
	{
		if (m_ViewTabCtrl.GetSafeHwnd())
		{
			for (int nTabs = 0; nTabs < m_ViewTabCtrl.GetItemCount(); nTabs++)
			{
				TCITEM tci;
				tci.mask = TCIF_PARAM;
				m_ViewTabCtrl.GetItem(nTabs, &tci);
				if (tci.lParam == reinterpret_cast<LPARAM>(pView))
				{
					m_ViewTabCtrl.SetCurSel(nTabs);
					m_ViewTabCtrl.Invalidate();
					break;
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CViewManager message handlers

void CViewManager::OnSize(UINT nType, int cx, int cy) 
{
	CControlBar::OnSize(nType, cx, cy);
	try
	{
		CRect rect, rcClient;
		if (m_ViewTabCtrl.GetSafeHwnd())
		{
			DWORD dwStyle = m_ViewTabCtrl.GetStyle();
			if (dwStyle & TCS_BOTTOM)
			{
				GetClientRect(rect);
				GetClientRect(rcClient);
				int nxOffset = GetSystemMetrics(SM_CXSIZEFRAME);
				m_ViewTabCtrl.SetWindowPos(&wndTop, rcClient.left + nxOffset + m_nLMargin, 
					rcClient.top, rect.Width() - nxOffset * 5 - m_nLMargin, 
					ID_TABHEIGHT, SWP_SHOWWINDOW);
				
				m_sizeMRU = CSize(cx, cy);
			}
			else
			{
				GetClientRect(rect);
				int nxOffset = GetSystemMetrics(SM_CXSIZEFRAME);
				m_ViewTabCtrl.SetWindowPos(&wndTop, nxOffset + m_nLMargin, 3, 
					rect.Width() - nxOffset * 5 - m_nLMargin, ID_TABHEIGHT, SWP_SHOWWINDOW);
				
				m_sizeMRU = CSize(cx, cy);
			}
		}
	}
	catch(...)
	{
	}
}

int CViewManager::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_ViewTabImages.Create(IDB_BITMAP_TABCTRL, 14, 0, RGB(0, 255, 0));

	m_ViewTabCtrl.Create(WS_CHILD | WS_VISIBLE | WS_EX_NOPARENTNOTIFY | 
		TCS_TOOLTIPS | TCS_SINGLELINE | TCS_FOCUSNEVER | TCS_FORCELABELLEFT, 
		CRect(0, 0, 0, 0), this, ID_VIEWTAB);

	TabCtrl_SetExtendedStyle(m_ViewTabCtrl.m_hWnd, TCS_EX_FLATSEPARATORS);

	m_ViewTabCtrl.SetImageList(&m_ViewTabImages);

	// Enable tooltips for all controls
	EnableToolTips(TRUE);
	
	return 0;
}

void CViewManager::OnRButtonDown(UINT nFlags, CPoint point) 
{
	/*
	// Now, process the popup menu for the tab control
	POINT  ptScreen = point;
	// Convert the mouse point to screen coordinates since that is what is used by
	// the TrackPopupMenu() function.
	ClientToScreen(&ptScreen);
	CMenu *popupMenu;
	CMenu menu;
	menu.LoadMenu(IDR_TABBAR_MENU);
	if(menu)
	{
		popupMenu=menu.GetSubMenu(0);
		ASSERT(popupMenu);
//		TranslateMenu(popupMenu,conmeu_item_text);
		// insert a separator item at the top
		popupMenu->InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR, 0, _T(""));
		CString strcontextmenu;
		// insert title item
		// note: item is not selectable (disabled) but not grayed
//		GetText("ConMeuTabbarCaption",strcontextmenu,conmeu_item_text);
		popupMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING | MF_DISABLED, 0, strcontextmenu);
		
		::SetMenuDefaultItem(popupMenu->m_hMenu, 0, TRUE);
		
		popupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, AfxGetMainWnd());
		popupMenu->DestroyMenu();
	}
	*/

	CControlBar::OnRButtonDown(nFlags, point);
}


void CViewManager::OnViewManagerToolTip(NMHDR* pNMHDR, LRESULT* pResult)
{
	try
	{
		*pResult = 0;
		TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
		UINT nID =pNMHDR->idFrom;
		if (pTTT!=NULL  &&  ! (pTTT->uFlags & TTF_IDISHWND))
		{
			// idFrom is actually the HWND of the tool
			if(nID==0&&m_ViewTabCtrl.GetItemCount()>0)
			{
				CString tip=m_arViewTitles.GetAt(0);
				strncpy(pTTT->szText, (LPCSTR)tip, 79);
				pTTT->szText[79]='\0';
				return ;//TRUE;
			}
			
			return ;//FALSE;
		}
	}
	catch(...)
	{
	}

	return;
}

BOOL CViewManager::CreateViewManager(CMDIFrameWnd *pMDIFrameWnd, UINT uID)
{
    if (!Create(ID_DEFCAPTION, pMDIFrameWnd, m_sizeDefault, uID))
	{
		TRACE0(_T("Failed to create Tab bar\n"));  
		return FALSE;      // fail to create
	}

	SetBarStyle(GetBarStyle() | CBRS_TOP |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
 	pMDIFrameWnd->DockControlBar(this); 

	return TRUE;
}

BOOL CViewManager::Create(LPCTSTR lpszWindowName, CWnd* pParentWnd,
                               CSize sizeDefault, UINT nID, DWORD dwStyle)
{
    // must have a parent
    ASSERT_VALID(pParentWnd);
    // cannot be both fixed and dynamic
    // (CBRS_SIZE_DYNAMIC is used for resizng when floating)
    ASSERT (!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

    m_dwStyle = dwStyle & CBRS_ALL; // save the control bar styles
	m_sizeDefault = sizeDefault;    // set fixed size
	m_sizeMRU     = sizeDefault;
    // register and create the window - skip CControlBar::Create()
    CString strWndclass = ::AfxRegisterWndClass(CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW), ::GetSysColorBrush(COLOR_BTNFACE), 0);

    dwStyle &= ~CBRS_ALL;
	dwStyle |= WS_CLIPCHILDREN | CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NORESIZE;
    if (!CWnd::Create(strWndclass, lpszWindowName, dwStyle,
        CRect(0, 0, 0, 0), pParentWnd, nID))
        return FALSE;

    return TRUE;
}

CSize CViewManager::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	
	if (bStretch) // if not docked stretch to fit
		return CSize(bHorz ? 32767 : m_sizeDefault.cx, bHorz ? m_sizeDefault.cy : 32767);
	
	CClientDC dc(NULL);
	HFONT hFont    = reinterpret_cast<HFONT>(SendMessage(WM_GETFONT));
	HFONT hOldFont = NULL;
	if (hFont != NULL)
		hOldFont = reinterpret_cast<HFONT>(dc.SelectObject(hFont));
	TEXTMETRIC tm;
	VERIFY(dc.GetTextMetrics(&tm));
	if (hOldFont != NULL)
		dc.SelectObject(hOldFont);
	
	// get border information
	CSize size;
	CRect rcInside, rcWnd; 
	rcInside.SetRectEmpty();
	CalcInsideRect(rcInside, bHorz);
	GetParentFrame()->GetWindowRect(&rcWnd);
	
	size.cx = rcWnd.Width() + 9;
	size.cy = tm.tmHeight + tm.tmInternalLeading + 
		::GetSystemMetrics(SM_CYBORDER) * 5 - rcInside.Height();
	
	return size;
}

CSize CViewManager::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	UNREFERENCED_PARAMETER(nLength);	   // clear that level 4 warning...

	if (dwMode & LM_HORZDOCK)
	{
		ASSERT(dwMode & LM_HORZ);
		return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZ);
	}

	if (m_sizeMRU.cx > m_sizeDefault.cx)
		return m_sizeMRU;
	else
		return m_sizeDefault;
}

void CViewManager::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	lpwndpos->flags |= SWP_FRAMECHANGED;
	CControlBar::OnWindowPosChanging(lpwndpos);
	try
	{
		UINT nOldDockID = m_nDockID;
		m_nDockID = GetParent()->GetDlgCtrlID();
		if (nOldDockID == m_nDockID)
			return;
		else
		{
			DWORD dwStyle = m_ViewTabCtrl.GetStyle();
			switch(m_nDockID)
			{
			case AFX_IDW_DOCKBAR_TOP:
				if (dwStyle & TCS_BOTTOM)
				{
					DWORD dwNewStyle = dwStyle & ~TCS_BOTTOM;
					m_ViewTabCtrl.ModifyStyle(dwStyle, dwNewStyle);
				}
				break;
			case AFX_IDW_DOCKBAR_BOTTOM:
				if ((dwStyle & TCS_BOTTOM) == 0)
				{
					DWORD dwNewStyle = dwStyle | TCS_BOTTOM;
					m_ViewTabCtrl.ModifyStyle(dwStyle, dwNewStyle);
				}
				break;
			case AFX_IDW_DOCKBAR_LEFT:
				break;
			case AFX_IDW_DOCKBAR_RIGHT:
				break;
			default:
				DWORD dwNewStyle = dwStyle & ~TCS_BOTTOM;
				m_ViewTabCtrl.ModifyStyle(dwStyle, dwNewStyle);
				break;
			}
		}
	}
	catch(...)
	{
	}
}

void CViewManager::DoPaint(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

	// clean background
	COLORREF clr = ::GetSysColor(COLOR_BTNFACE);
	pDC->FillSolidRect(rect, clr);	

	// draw the gripper
	DrawGripper(pDC);
	// It is better to let the underlining control bar take the last shot
	CControlBar::DoPaint(pDC);
}

void CViewManager::DrawGripper(CDC* pDC)
{
	try
	{
		if( (m_dwStyle & CBRS_FLOATING) || m_dwDockStyle == 0 )
			return;
		
		COLORREF clrBtnHilight = ::GetSysColor(COLOR_BTNHILIGHT);
		COLORREF clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);
		
		CRect rcGrip;					 
		GetWindowRect(&rcGrip);
		ScreenToClient(&rcGrip);
		rcGrip.OffsetRect(-rcGrip.left, -rcGrip.top);
		
		if(m_dwStyle & CBRS_ORIENT_HORZ) 
		{
			// gripper at left
			rcGrip.DeflateRect(4, 4);
			rcGrip.right = rcGrip.left + 3;
			pDC->Draw3dRect(rcGrip, clrBtnHilight, clrBtnShadow);
			if (!m_bWin2000)
			{
				rcGrip.OffsetRect(3, 0);
				pDC->Draw3dRect(rcGrip,	clrBtnHilight, clrBtnShadow);
			}
		}
		else 
		{
			// gripper at top
			rcGrip.DeflateRect(4, 4);
			rcGrip.bottom = rcGrip.top + 3;
			pDC->Draw3dRect(rcGrip, clrBtnHilight, clrBtnShadow);
			if (!m_bWin2000)
			{
				rcGrip.OffsetRect(0, 3);
				pDC->Draw3dRect(rcGrip, clrBtnHilight, clrBtnShadow);
			}
		}
	}
	catch(...)
	{
	}
}


void CViewManager::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	OnSize(0, lpwndpos->cx, lpwndpos->cy);
	CControlBar::OnWindowPosChanged(lpwndpos);
}





void CViewManager::SetAddLast(BOOL last)
{
	m_beAddLast=last;
}

BOOL CViewManager::GetAddLast()
{
	return m_beAddLast;
}

int CViewManager::GetViewNum(CRuntimeClass *pViewClass)
{
	CView *pView;
	int count = 0;

	for (int nTabs = 0; nTabs < m_arViews.GetSize(); nTabs++)
	{
		pView = static_cast<CView *>(m_arViews.GetAt(nTabs));
		if(pView->IsKindOf(pViewClass))
			++count;
	}

	return count;
}


// get the first view which matches the givenn ViewClass
CView * CViewManager::GetFirstView(CRuntimeClass *pViewClass)
{
	CView *pView;

	m_currentSearchPos = -1;

	for (int nTabs = 0; nTabs < m_arViews.GetSize(); nTabs++)
	{
		pView = static_cast<CView *>(m_arViews.GetAt(nTabs));
		if(pView->IsKindOf(pViewClass))
		{
			m_currentSearchPos = nTabs;
			return pView;
		}
	}

	return NULL;
}


CView * CViewManager::GetNextView(CRuntimeClass *pViewClass)
{
	CView *pView;

	for (int nTabs = m_currentSearchPos + 1; nTabs < m_arViews.GetSize(); nTabs++)
	{
		pView = static_cast<CView *>(m_arViews.GetAt(nTabs));
		if(pView->IsKindOf(pViewClass))
		{
			m_currentSearchPos = nTabs;
			return pView;
		}
	}

	m_currentSearchPos = m_arViews.GetSize();

	return NULL;
}

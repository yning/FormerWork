// AdvTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WebMind.h"
#include "AdvTabCtrl.h"
#include "MainFrm.h"
#include "malloc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_VIEWTAB 1005
#define PT_IN_ICON(point, rect)		(point.x < rect.left + 20)
#define TAB_NEED_CAPTURE			(g_bClickClose==1 || (g_bClickClose==2 && pmf->m_nTabStyle!=2))

BOOL g_bCloseDown = FALSE;//click icon down
BOOL g_bShowCloseIcon = FALSE;//show close icon
int  g_nCloseTabID;

/////////////////////////////////////////////////////////////////////////////
// CAdvTabCtrl
//
/*
//<summary>
//Log record
//</summayr>
int logging(CString funName,CString paraName,CString paraValue)
{
	CString tt="Function is "+funName+"\t"+paraName+":"+paraValue+"\n";
	char *ll=(char *)malloc((tt.GetLength()+1)*sizeof(char));
	lstrcpy(ll,tt);
	FILE *file=NULL;
	file=fopen("D:\\WebMindMerger1.2\\log.txt","at");// absolute path
	if (NULL==file)
	{
		printf("open error!\n");
		return 0;
	}
	printf("open success!\n");
	int test=fwrite(ll,sizeof(char),strlen(ll),file);
	fclose(file);
	free(ll);
	return 1;
}
*/

CAdvTabCtrl::CAdvTabCtrl()
{
	m_hDragCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_POINTER_DRAG));
	TabCtrlItem.pszText = m_szText;
	TabCtrlItem.cchTextMax = MAX_PATH-1;
	m_crSelColour     = GetSysColor(COLOR_BTNTEXT); //RGB(0,0,255);
	m_crUnselColour   = GetSysColor(COLOR_GRAYTEXT);//COLOR_BTNTEXT); //RGB(50,50,50);
	m_crSelBKColour   = GetSysColor(COLOR_BTNFACE); //RGB(0,0,255);
	m_crUnselBKColour = RGB(214,211,206);//RGB(180,180,180);
	
	
	m_bDragging = FALSE;
	*m_szText = 0;
}

CAdvTabCtrl::~CAdvTabCtrl()
{	
}

BEGIN_MESSAGE_MAP(CAdvTabCtrl, CEnTabCtrl)
	//{{AFX_MSG_MAP(CAdvTabCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvTabCtrl message handlers

void CAdvTabCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	//Handle the button in the rightup
//	m_bMouseOver = true;
	// TODO: Add your message handler code here and/or call default
	if (g_bCloseDown)
	{
		GetItem(g_nCloseTabID, &TabCtrlItem);
		CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
		if (!tcf)
			return;
		CWebMindView *pView = tcf->m_pView;
		int id = GetTabIDFromPoint(point);
		RECT rect;
		GetItemRect(id,&rect);
		//
		if (id==g_nCloseTabID)
		{
			if ( PT_IN_ICON(point, rect) )
			{
				if (!g_bShowCloseIcon)
				{
					g_bShowCloseIcon = 1;
					pView->m_nOlderIcon = pView->m_nOldIcon;
					pmf->SetTabIcon(TI_CLOSE,id, pView);//TI_CLOSE 8
				}
			}
			else
			{
				if (g_bShowCloseIcon)
				{
					g_bShowCloseIcon = 0;
					pmf->SetTabIcon( pView->m_nOlderIcon,g_nCloseTabID, pView);
				}
			}
		}
		else//else tab or not on tab
		{
			if (g_bShowCloseIcon)
			{
				g_bShowCloseIcon = 0;
				pmf->SetTabIcon( pView->m_nOlderIcon,g_nCloseTabID, pView);
			}
		}
	}
	else if(m_bDragging && (nFlags & MK_LBUTTON))
	{
		int id = GetTabIDFromPoint(point);
		if(id<0 || id==m_nDragTabID)
			SetCursor(m_hOldCursor);
		else
			SetCursor(m_hDragCursor);
	}

	CEnTabCtrl::OnMouseMove(nFlags, point);
	
}

void CAdvTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	m_bMouseDown = TRUE;//some problem
	// TODO: Add your message handler code here and/or call default
	if (g_bClickClose==2)
	{
		if (ClickCloseDown(point))
			goto _Exit;
	}
	if(!m_bDragging && (nFlags & MK_LBUTTON))
	{
		m_bDragging = TRUE;
		m_hOldCursor = GetCursor();
		m_nDragTabID = GetTabIDFromPoint(point);
		TabCtrlItem.mask = TCIF_PARAM|TCIF_IMAGE|TCIF_TEXT|TCIF_STATE;
		GetItem(m_nDragTabID, &TabCtrlItem);		
		if(GetCurSel() == m_nDragTabID)
			m_bSel = TRUE;
		else
			m_bSel = FALSE;
	}

_Exit:	
	CEnTabCtrl::OnLButtonDown(nFlags, point);
}

void CAdvTabCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//tabctrl button on the rightup
//	m_bMouseOver = false;
//	m_bMouseDown = false;
	
//	m_bMouseUp = TRUE;
	// TODO: Add your message handler code here and/or call default
	try{
	if (g_bClickClose==2 && g_bCloseDown)
	{
		ClickCloseUp(point);
		goto _Exit;
	}
	if(m_bDragging)
	{
		int id = GetTabIDFromPoint(point);
		m_bDragging = FALSE;
		if(id>=0 && id!=m_nDragTabID)
		{
			if(id<m_nDragTabID)
				id--;
			//insert new tab
			TabCtrlItem.mask = TCIF_TEXT|TCIF_PARAM|TCIF_IMAGE|TCIF_STATE;
			InsertItem( id+1, &TabCtrlItem );
			//delete tab
			if(id+1<m_nDragTabID)
				m_nDragTabID ++;
			DeleteItem(m_nDragTabID);
			//select this tab
			if(m_nDragTabID<id+1)
				id--;
			if(m_bSel)
				SetCurSel(id+1);
			if (g_bShowIndex)
				pmf->UpdateTabTitleIndex( min(id+1, m_nDragTabID) );

			AfxGetMainWnd()->PostMessage(WM_UPDATE_TAB_TIP);
		}
	}
	}catch(...){}
_Exit:	
	CEnTabCtrl::OnLButtonUp(nFlags, point);
}

int CAdvTabCtrl::GetTabIDFromPoint(CPoint &point)
{
	TCHITTESTINFO info;
	info.pt = point;
	info.flags = TCHT_NOWHERE;
	int id = HitTest(&info);
	return id;
}
//<summary>
// double click close
//</summary>
void CAdvTabCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(pmf->m_nAllowDBC>0)
		ProcessClick(nFlags, point, 0);

	CEnTabCtrl::OnLButtonDblClk(nFlags, point);//
}

void CAdvTabCtrl::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(pmf->m_nAllowMC>0)
		ProcessClick(nFlags, point, 1);

	CEnTabCtrl::OnMButtonDown(nFlags, point);
}

void CAdvTabCtrl::ProcessClick(UINT nFlags, CPoint point, int nClickButton)
{
	try
	{
		TCHITTESTINFO hti;
		hti.pt = point;
		hti.flags = TCHT_ONITEM | TCHT_ONITEMICON | TCHT_ONITEMLABEL ;
		int nTabID = HitTest(&hti);
		TCITEM TabCtrlItem;
		TabCtrlItem.mask = TCIF_PARAM;
		GetItem(nTabID, &TabCtrlItem);
		if(TabCtrlItem.lParam != NULL)
		{
			CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if (!tcf || !tcf->m_pView)
				return;
			if (tcf->m_bProtect && !tcf->m_pView->m_bIsActive)
				return;
			//double click close
			if((nClickButton==0 && pmf->m_nAllowDBC==1) || (nClickButton==1 && pmf->m_nAllowMC==1) || (nClickButton==2 && pmf->m_nAllowRC==1))
			{
				//2.2 313 post instead send
				tcf->PostMessage(WM_CLOSE);//
				tcf->m_pView->m_bToClose = TRUE;
				//tcf->MDIDestroy();
			}
			else if((nClickButton==0 && pmf->m_nAllowDBC==2) || (nClickButton==1 && pmf->m_nAllowMC==2) || (nClickButton==2 && pmf->m_nAllowRC==2))
			{
				//save as
				tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_SAVEAS, 0);
			}
			else if((nClickButton==0 && pmf->m_nAllowDBC==3) || (nClickButton==1 && pmf->m_nAllowMC==3) || (nClickButton==2 && pmf->m_nAllowRC==3))
			{
				//save html
				tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_SAVEHTML, 0);
			}
			else if((nClickButton==0 && pmf->m_nAllowDBC==4) || (nClickButton==1 && pmf->m_nAllowMC==4) || (nClickButton==2 && pmf->m_nAllowRC==4))
			{
				//save html
				tcf->m_pView->PostMessage(WM_COMMAND, ID_FILE_AUTOSAVE, 0);
			}
			else if((nClickButton==0 && pmf->m_nAllowDBC==5) || (nClickButton==1 && pmf->m_nAllowMC==5) || (nClickButton==2 && pmf->m_nAllowRC==5))
			{
				pmf->LockTab(tcf);
			}
			else if((nClickButton==0 && pmf->m_nAllowDBC==6) || (nClickButton==1 && pmf->m_nAllowMC==6) || (nClickButton==2 && pmf->m_nAllowRC==6))
			{
				tcf->m_pView->PowerRefresh();
			}
		}
	}
	catch(...){}
}

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx operations

void CAdvTabCtrl::SetColours(COLORREF bSelColour, COLORREF bUnselColour)
{
	m_crSelColour = bSelColour;
	m_crUnselColour = bUnselColour;
	Invalidate();
}

void CAdvTabCtrl::SetBKColours(COLORREF bSelBKColour, COLORREF bUnselBKColour)
{
	m_crSelBKColour = bSelBKColour;
	m_crUnselBKColour = bUnselBKColour;
	Invalidate();
}

void CAdvTabCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (g_bClickClose==1)
		ClickCloseDown(point);

	CEnTabCtrl::OnRButtonDown(nFlags, point);
}

void CAdvTabCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bDragging = FALSE;

	if(pmf->m_nAllowRC>0 && (nFlags & MK_CONTROL)!=0 )
	{
		ProcessClick(nFlags, point, 2);
	}
	else if (g_bClickClose==1 && g_bCloseDown)
	{
		if (ClickCloseUp(point))
			return;//not pop menu
	}

	CEnTabCtrl::OnRButtonUp(nFlags, point);
}

BOOL CAdvTabCtrl::ClickCloseDown(CPoint point)
{
	g_nCloseTabID = GetTabIDFromPoint(point);
	TabCtrlItem.mask = TCIF_PARAM|TCIF_IMAGE|TCIF_TEXT|TCIF_STATE;
	GetItem(g_nCloseTabID, &TabCtrlItem);		
	//
	CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
	if (!tcf)
		return FALSE;
	CWebMindView *pView = tcf->m_pView;
	if ( !(tcf->m_bProtect && !pView->m_bIsActive) )//not close not active protect tab
	{
		RECT rect;
		GetItemRect(g_nCloseTabID,&rect);
		if ( PT_IN_ICON(point, rect) )
		{
			g_bCloseDown = 1;
			g_bShowCloseIcon = 1;
			pView->m_nOlderIcon = pView->m_nOldIcon;
			pmf->SetTabIcon(TI_CLOSE,g_nCloseTabID, pView);
			if (TAB_NEED_CAPTURE)
				SetCapture();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CAdvTabCtrl::ClickCloseUp(CPoint point)
{
	if (g_bCloseDown)
	{
		g_bCloseDown = 0;
		if (TAB_NEED_CAPTURE)
			ReleaseCapture();
		if (g_bShowCloseIcon)		
		{
			g_bShowCloseIcon = 0;
			GetItem(g_nCloseTabID, &TabCtrlItem);
			CChildFrame* tcf = ((CChildFrame*)TabCtrlItem.lParam);
			if (!tcf)	return FALSE ;
			tcf->PostMessage(WM_CLOSE);
			tcf->m_pView->m_bToClose = TRUE;
			pmf->SetTabIcon(tcf->m_pView->m_nOlderIcon,g_nCloseTabID,tcf->m_pView);
			return TRUE;
		}
	}
	return FALSE;
}

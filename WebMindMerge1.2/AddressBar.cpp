// AddressBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "AddressBar.h"

//#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAddURLDropTarget::CAddURLDropTarget()
{
}

CAddURLDropTarget::~CAddURLDropTarget()
{

}
//
// OnDragEnter is called by OLE dll's when drag cursor enters
// a window that is REGISTERed with the OLE dll's
//
DROPEFFECT CAddURLDropTarget::OnDragEnter(CWnd* , COleDataObject* 
           pDataObject, DWORD dwKeyState, CPoint )
{
	if (!pDataObject->IsDataAvailable(CF_TEXT))
	{
		return DROPEFFECT_NONE;
	
	}

    // if the control key is held down, return a drop effect COPY 
    if((dwKeyState&MK_CONTROL) == MK_CONTROL)
        return DROPEFFECT_COPY;
    // Otherwise return a drop effect of MOVE
    else
        return DROPEFFECT_MOVE;    
}

//
// OnDragLeave is called by OLE dll's when drag cursor leaves
// a window that is REGISTERed with the OLE dll's
//
void CAddURLDropTarget::OnDragLeave(CWnd* pWnd)
{
    // Call base class implementation
    COleDropTarget::OnDragLeave(pWnd);
}

// 
// OnDragOver is called by OLE dll's when cursor is dragged over 
// a window that is REGISTERed with the OLE dll's
//
DROPEFFECT CAddURLDropTarget::OnDragOver(CWnd* , COleDataObject* 
           pDataObject, DWORD dwKeyState, CPoint )
{     
	if (!pDataObject->IsDataAvailable(CF_TEXT))
		return DROPEFFECT_NONE;

    if((dwKeyState&MK_CONTROL) == MK_CONTROL)
        return DROPEFFECT_NONE;  
    else
        return DROPEFFECT_MOVE;  // move source
}


BOOL CAddURLDropTarget::OnDrop(CWnd* , COleDataObject* , 
                 DROPEFFECT , CPoint )
{           
	return FALSE;

	/*
    HGLOBAL  hGlobal;
    LPCSTR   pData;                     

	if (pDataObject->IsDataAvailable(CF_TEXT))
	{		
		STGMEDIUM Stg;
//		BOOL bValue = pDataObject->GetData(CF_TEXT, &Stg);		

		TCHAR *strText = (TCHAR*)GlobalLock(Stg.hGlobal);
		
		CString strUrl;
		strUrl.Format("%s",strText);
		try
		{
			CMainFrame *pFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
			
			pFrame->SetAddressComText(strUrl);
			::SendMessage(pFrame->m_hWnd,WM_COMMAND,IDOK,0);
		}
		catch(...)
		{
		}
		GlobalUnlock(Stg.hGlobal);
		GlobalFree(Stg.hGlobal);		

	}

    if((dropEffect&DROPEFFECT_MOVE) != DROPEFFECT_MOVE)
        return FALSE;

    // Get text data from COleDataObject
    hGlobal=pDataObject->GetGlobalData(CF_TEXT);

    // Get pointer to data
    pData=(LPCSTR)GlobalLock(hGlobal);    
    ASSERT(pData!=NULL); 

    // Unlock memory - Send dropped text into the "bit-bucket"
    GlobalUnlock(hGlobal);

    return TRUE;
	*/

}


CAddressBar::CAddressBar()
{
	ShowGoButton = TRUE;
}

CAddressBar::~CAddressBar()
{
}


BEGIN_MESSAGE_MAP(CAddressBar, CToolBar)
	//{{AFX_MSG_MAP(CAddressBar)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT( TBN_GETINFOTIP, OnGetInfoTip )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddressBar message handlers

void CAddressBar::OnSize(UINT nType, int cx, int cy) 
{
	CToolBar::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//===
	if(m_wndAddress)
	{
		CRect rect2,rect3,rect4;
		GetItemRect(1, rect2);
		GetItemRect(2,rect3);
		GetItemRect(3,rect4);
		int w;
		if(ShowGoButton)
			w = cx - rect2.Width() - 2;// -rect3.Width()-rect4.Width()-2;
		else
			w = cx;
		
		SetButtonInfo(0, 0, TBBS_SEPARATOR, w);
		m_wndAddress.SetWindowPos(NULL,  0, 0, w, cy, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS);
		if(!m_wndAddress.IsWindowVisible())
			m_wndAddress.ShowWindow(SW_SHOW);
	}
}


CComboBoxEx* CAddressBar::GetAddressBox()
{
	return &m_wndAddress;
}

void CAddressBar::Init() 
{
	// create a combo box for the address bar
	if (!m_wndAddress.Create(WS_VISIBLE|WS_CHILD|WS_VSCROLL|CBS_DROPDOWN, CRect(0, 0, 10, 120), this, AFX_IDW_TOOLBAR + 1))
	{
		TRACE0("Failed to create combobox\n");
		return ;      // fail to create
	}
	m_wndAddress.SetExtendedStyle(0, m_wndAddress.GetExtendedStyle()|CBES_EX_NOSIZELIMIT);

	/*
	CImageList img;
	HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_FILETYPE));
	img.Create(16, 16, ILC_COLOR8|ILC_MASK, 7, 1);
	ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));
	m_wndAddress.SetImageList(&img);
	img.Detach();

	BOOL success = m_dropTarget.Register(&m_wndAddress);
    if(!success )
      MessageBox("Ole Register Drop Target Failed");   
	*/

	 return;
}

void CAddressBar::GetSelText(int index, CString & str)
{
	if(index<0)
		return;

	try
	{
		static TCHAR sz[MAX_PATH];
		COMBOBOXEXITEM item;

		item.mask = CBEIF_TEXT;
		item.pszText = (LPTSTR)sz;
		item.cchTextMax = MAX_PATH;

		item.iItem = index;
		m_wndAddress.GetItem(&item);
		str = sz;
	}catch(...)
	{
	}
}

int CAddressBar::FindStringExact(int start, CString & str)
{
	//try to add static
	static TCHAR sz[MAX_PATH];

	try
	{
		COMBOBOXEXITEM item;
		item.mask = CBEIF_TEXT;
		item.pszText = (LPTSTR)sz;
		item.cchTextMax = MAX_PATH;

		int nc = m_wndAddress.SendMessage(CB_GETCOUNT, 0, 0);
		for(register int i=start+1;i<nc;i++)
		{
			item.iItem = i;
			m_wndAddress.GetItem(&item);
			if(str.CompareNoCase(sz)==0)
				return i;
		}
	}catch(...)
	{
	}

	return CB_ERR;
}

void CAddressBar::OnGetInfoTip( NMHDR* pNMHDR, LRESULT* pResult )
{
	//  16/08/2002 benben start
    NMTBGETINFOTIP* lptbgit = ( NMTBGETINFOTIP* )pNMHDR;
    CString strTip;
	CString strFull,strNew;
    if(strFull.LoadString( lptbgit->iItem ) )
	{
		//GetText(strFull,strNew,1);

		strNew = strFull;
		( AfxExtractSubString( strTip, strNew, 1, _T('\n') ) );	
	    _tcsncpy( lptbgit->pszText, strTip, lptbgit->cchTextMax );
	}

    *pResult = 0;
}
///////////////////////////////////////////////////////////////
// LinkBar.cpp: implementation of the CLinkBar class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebMind.h"
#include "LinkBar.h"

#include "MainFrm.h"

#include "windows.h"
//#pragma comment(lib,"windows.lib")

IMPLEMENT_DYNAMIC(CLinkBar,CToolBarEx)
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////

CLinkBar::CLinkBar()
{

}

void CLinkBar::Init()
{
/*  static TBBUTTONEX tbButtons[] =
    {
        { {  0, ID_SEARCH_GOOGLE,		TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, true },
        { {  0, ID_SEARCH_ALTAVISTA,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, true },
        { {  0, ID_SEARCH_LYCOS,		TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, true },
        { {  0, ID_SEARCH_VIVISIMO,		TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }, true }
    };
	*/
	// Delete old buttons
    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    while ( tbCtrl.DeleteButton( 0 ) );
	
    // Load buttons from provided array
    m_aButtons.RemoveAll();
	
	//for(int i = 0; i < 4; i++)// comment by ny
		//AddButton(i, tbButtons[i]);// comment by ny

	/*
	CMainFrame *pFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	for(int i=0;i<pFrame->m_linkbarItem.GetSize();i++)
	{
		TBBUTTONEX tbButtons[] =
		{
			{ {  pFrame->m_linkbarItem[i]->icon,pFrame->m_linkbarItem[i]->nID,TBSTATE_ENABLED, TBSTYLE_BUTTON, 0,0 }, true}
        };
		AddButton(i,*tbButtons,pFrame->m_linkbarItem[i]->strName);
	}
	//the linkbar item will never be used so delete it
	for(i=0;i<pFrame->m_linkbarItem.GetSize();i++)
	{
		delete pFrame->m_linkbarItem [i];
	}
	pFrame->m_linkbarItem.RemoveAll();

	CImageList	m_ImageListSmall;
	HIMAGELIST hImageList;
	SHFILEINFO shFi;
	hImageList=(HIMAGELIST)SHGetFileInfo("C:\\",0,&shFi,sizeof(shFi),
		SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	m_ImageListSmall.m_hImageList=hImageList;
	
	//CToolBarCtrl& tbCtrl = GetToolBarCtrl();
	tbCtrl.SetImageList(&m_ImageListSmall);
	m_ImageListSmall.Detach();
	*/

	//CImageList img;//comment by ny
	//HBITMAP hbmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_SEARCH));//comment by ny
	//img.Create(16, 16, ILC_COLOR8|ILC_MASK, 7, 1);//comment by ny
	//ImageList_AddMasked(img.GetSafeHandle(), hbmp, RGB(255,0,255));//comment by ny
	//tbCtrl.SetImageList(&img);//comment by ny
	//img.Detach();//comment by ny
	
	m_eIconOptionsDefault=m_eIconOptions=ioSmallIcons;
	
	SetCustomizeMode(FALSE);
	
	ReloadButtons();
    UpdateParentBandInfo();
}
bool CLinkBar::HasButtonText( UINT )
{
	return TRUE;
}

void CLinkBar::GetButtonTip( UINT nID, CString& strTip)
{
    GetButtonText( nID, strTip);

	/*
	CMainFrame * pMainfrm=(CMainFrame *)AfxGetApp()->m_pMainWnd;
    if( nID>0xe00&& nID<0xfff)
	{
		if((nID-0xe00)<(UINT)pMainfrm->m_astrFavoriteURLs.GetSize())	
		{
			strTip=pMainfrm->m_astrFavoriteURLs[nID-0xe00];
		}
	}
	*/
}

bool CLinkBar::HasButtonTip( UINT )
{
	return TRUE;
}

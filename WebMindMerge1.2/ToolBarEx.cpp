/////////////////////////////////////////////////////////////////////////////
// ToolBarEx.cpp: implementation of the CToolBarEx class.
//
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 by Nikolay Denisov. All rights reserved.
//
// This code is free for personal and commercial use, providing this 
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Please email bug reports, bug fixes, enhancements, requests and
// comments to: nick@actor.ru
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonRes.h"
#include "resource.h"
#include "CustomizeDialog.h"
#include "SizableReBar.h"
#include "ToolBarEx.h"
#include "WinAppEx.h"

#include <afxpriv.h>    // ON_MESSAGE_VOID()

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx

LPCTSTR             CToolBarEx::m_lpszStateInfoEntry = _T("ToolbarStateInfo (v1.01)");
CToolBarEx*         CToolBarEx::m_pToolBar           = 0;
CCustomizeDialog*   CToolBarEx::m_pCustomizeDlg      = 0;
HHOOK               CToolBarEx::m_hCBTHook           = 0;

IMPLEMENT_DYNAMIC( CToolBarEx, CToolBar )

CToolBarEx::CToolBarEx()
{
    m_eTextOptions = toNone;    // no options selected yet
    m_eIconOptions = ioNone;
	m_customize    = TRUE;
}

CToolBarEx::~CToolBarEx()
{
}

BOOL CToolBarEx::Create( CWnd* pParentWnd, DWORD dwStyle, UINT nID /*=AFX_IDW_TOOLBAR*/ )
{
    if ( !CreateEx( pParentWnd,
            TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_TOOLTIPS,
            dwStyle & ~CBRS_TOOLTIPS, CRect( 0, 0, 0, 0 ), nID ) )
    {
        return false;
    }

    GetToolBarCtrl().SetExtendedStyle(
        TBSTYLE_EX_DRAWDDARROWS | TBSTYLE_EX_HIDECLIPPEDBUTTONS );

    return true;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes

void CToolBarEx::SetTextOptions( ETextOptions eTextOptions, bool bUpdate /*=true*/ )
{
    ASSERT( ::IsWindow( m_hWnd ) );
    ASSERT( GetStyle() & TBSTYLE_TOOLTIPS );
    ASSERT( !( GetBarStyle() & CBRS_TOOLTIPS ) );
    ASSERT( IsTextOptionAvailable( eTextOptions ) );

    m_eTextOptions = eTextOptions;

    // Modify toolbar style according to new text options
    ModifyStyle(
        ( eTextOptions == toTextOnRight ) ? 0 : TBSTYLE_LIST,
        ( eTextOptions == toTextOnRight ) ? TBSTYLE_LIST : 0 );

    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    DWORD dwStyleEx = tbCtrl.GetExtendedStyle();
    tbCtrl.SetExtendedStyle(
        ( eTextOptions == toTextOnRight ) ?
            ( dwStyleEx |  TBSTYLE_EX_MIXEDBUTTONS ) :
            ( dwStyleEx & ~TBSTYLE_EX_MIXEDBUTTONS ) );
    VERIFY( tbCtrl.SetMaxTextRows(
        ( eTextOptions == toNoTextLabels ) ? 0 : 1 ) );

    // Modify all (even currently hidden ones) buttons in internal cache
    int nIndex;
    for ( nIndex = 0; nIndex <= m_aButtons.GetUpperBound(); nIndex++ )
    {
        TBBUTTON& tbinfo = m_aButtons[ nIndex ].tbinfo;
        if ( !( tbinfo.fsStyle & TBSTYLE_SEP ) )
        {
            CString strButtonText;
	
			GetButtonText( tbinfo.idCommand,strButtonText);
			//strButtonText  =  m_aButtons[ nIndex ].btntext;
            CString strToAdd( strButtonText, strButtonText.GetLength() + 1 );
            tbinfo.iString = tbCtrl.AddStrings( strToAdd );

            switch ( eTextOptions )
            {
                case toTextLabels:
                    tbinfo.fsStyle &= ~( TBSTYLE_AUTOSIZE | BTNS_SHOWTEXT );
                    break;

                case toTextOnRight:
                    tbinfo.fsStyle |= ( TBSTYLE_AUTOSIZE |
                        ( HasButtonText( tbinfo.idCommand ) ? BTNS_SHOWTEXT : 0 ) );
                    break;

                case toNoTextLabels:
                    tbinfo.fsStyle &= ~BTNS_SHOWTEXT;
                    tbinfo.fsStyle |= TBSTYLE_AUTOSIZE;
                    break;
            }
        }
    }

    // If requested, reflect changes immediately
    if ( bUpdate )
    {
        ReloadButtons();
        UpdateParentBandInfo();
    }
}

ETextOptions CToolBarEx::GetTextOptions() const
{
    return m_eTextOptions;
}

void CToolBarEx::SetIconOptions( EIconOptions eIconOptions, bool bUpdate /*=true*/ )
{
    ASSERT( ::IsWindow( m_hWnd ) );
    ASSERT( IsIconOptionAvailable( eIconOptions ) );

    m_eIconOptions = eIconOptions;

    // Set image list(s) and icon size for selected icon options
    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    tbCtrl.SetImageList( 0 );
    tbCtrl.SetHotImageList( 0 );

    CSize szIcon = ( eIconOptions == ioSmallIcons ) ? szImageSmall : szImageLarge;
    VERIFY( tbCtrl.SetBitmapSize( szIcon ) );

    UINT nIDCold = ( eIconOptions == ioSmallIcons ) ? m_nIDSmallCold : m_nIDLargeCold;
    UINT nIDHot  = ( eIconOptions == ioSmallIcons ) ? m_nIDSmallHot  : m_nIDLargeHot;
    ASSERT( nIDCold != ( UINT )-1 );    // at least there must be "cold" imagelist

    m_imageListCold.DeleteImageList();
    VERIFY( m_imageListCold.Attach( ImageList_LoadImage( AfxGetResourceHandle(),
      MAKEINTRESOURCE( nIDCold ), szIcon.cx, 0, RGB(255,0,255),
        IMAGE_BITMAP, LR_CREATEDIBSECTION ) ) );
    tbCtrl.SetImageList( &m_imageListCold );
/////////////

/////////////
    if ( nIDHot != ( UINT )-1 ) // "hot" imagelist is optional
    {
        m_imageListHot.DeleteImageList();
		//m_imageListHot.Create(16, 16, ILC_COLORDDB|ILC_MASK, 40, 1);
        VERIFY( m_imageListHot.Attach( ImageList_LoadImage( AfxGetResourceHandle(),
            MAKEINTRESOURCE( nIDHot ), szIcon.cx, 0, m_clrMask,
            IMAGE_BITMAP, LR_CREATEDIBSECTION ) ) );
        tbCtrl.SetHotImageList( &m_imageListHot );
    }
	else
	{
		int nCount = m_imageListCold.GetImageCount();
		if(nCount>0)
		{
			m_imageListHot.DeleteImageList();
			m_imageListDisabled.DeleteImageList();
			m_imageListDisabled.Create(szIcon.cx,szIcon.cy,ILC_COLORDDB|ILC_MASK,0,10);
			m_imageListHot.Create(szIcon.cx,szIcon.cy,ILC_COLORDDB|ILC_MASK,0,10);
			for(int nIndex=0;nIndex<nCount;nIndex++)
			{
				HICON hIcon = m_imageListCold.ExtractIcon(nIndex);
			//	m_imageListHot.Add(hIcon);
				AddGloomIcon(&m_imageListHot,hIcon);
				AddGrayIcon(&m_imageListDisabled,hIcon);	
				DestroyIcon(hIcon);
			}
			tbCtrl.SetHotImageList(&m_imageListHot);
			tbCtrl.SetDisabledImageList(&m_imageListDisabled);
		}
	}
    // If requested, reflect changes immediately
    if ( bUpdate )
    {
        ReloadButtons();
        UpdateParentBandInfo();
    }
}

EIconOptions CToolBarEx::GetIconOptions() const
{
    return m_eIconOptions;
}

/////////////////////////////////////////////////////////////////////////////
// Operations

void CToolBarEx::SetBitmaps( UINT nIDSmallCold, UINT nIDSmallHot,
                             UINT nIDLargeCold, UINT nIDLargeHot,
                             EIconOptions eIconOptionsDefault,
                             COLORREF clrMask /*=RGB(255,0,255)*/ )
{
    m_nIDSmallCold = nIDSmallCold;
    m_nIDSmallHot  = nIDSmallHot;
    m_nIDLargeCold = nIDLargeCold;
    m_nIDLargeHot  = nIDLargeHot;
    m_clrMask      = clrMask;

    m_eIconOptionsDefault = eIconOptionsDefault;    // to be used on reset
    SetIconOptions( m_eIconOptionsDefault );        // apply new options
}

void CToolBarEx::SetButtons( int nNumButtons, TBBUTTONEX* lpButtons,
                             ETextOptions eTextOptionsDefault )
{
    // Delete old buttons
    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    while ( tbCtrl.DeleteButton( 0 ) );

    // Load buttons from provided array
    m_aButtons.RemoveAll();
    for ( int nIndex = 0; nIndex < nNumButtons; nIndex++ )
    {
        if ( lpButtons[ nIndex ].bInitiallyVisible )
        {
            VERIFY( tbCtrl.AddButtons( 1, &lpButtons[ nIndex ].tbinfo ) );
        }

        m_aButtons.Add( lpButtons[ nIndex ] );
    }

    m_eTextOptionsDefault = eTextOptionsDefault;    // to be used on reset
    SetTextOptions( m_eTextOptionsDefault );        // apply new options
}

void CToolBarEx::LoadState( LPCTSTR lpszProfileName )
{
    CString strSubKey;
    strSubKey.Format( _T("Software\\%s\\%s\\%s"),
        AfxGetApp()->m_pszRegistryKey,
        AfxGetApp()->m_pszProfileName,
        lpszProfileName );
    GetToolBarCtrl().RestoreState( HKEY_CURRENT_USER,
        strSubKey, m_lpszStateInfoEntry );

// It was found out that TB_SAVERESTORE causes TBN_BEGINADJUST
// and TBN_ENDADJUST to be sent correspondingly at the beggining
// and the end of save/restore process.  So, the following
// call is redundant and therefore was commented out.

//    UpdateParentBandInfo();
}

void CToolBarEx::SaveState( LPCTSTR lpszProfileName )
{
    CString strSubKey;
    strSubKey.Format( _T("Software\\%s\\%s\\%s"),
        AfxGetApp()->m_pszRegistryKey,
        AfxGetApp()->m_pszProfileName,
        lpszProfileName );
    GetToolBarCtrl().SaveState( HKEY_CURRENT_USER,
        strSubKey, m_lpszStateInfoEntry );
}

/////////////////////////////////////////////////////////////////////////////
// Overrides

BOOL CToolBarEx::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
    NMHDR* pNMHDR = ( NMHDR* )lParam;
    if ( ( pNMHDR->code == TTN_NEEDTEXTA ) ||
         ( pNMHDR->code == TTN_NEEDTEXTW ) )
    {
        // If button doesn't have a tip, ignore notification
        *pResult = HasButtonTip( pNMHDR->idFrom ) ? Default() : 0;
        return TRUE;
    }

    return CToolBar::OnNotify( wParam, lParam, pResult );
}

LRESULT CToolBarEx::DoCustomDraw( NMHDR* pNMHDR, CWnd* pToolBar )
{
    LPNMTBCUSTOMDRAW lpNMCustomDraw = ( LPNMTBCUSTOMDRAW )pNMHDR;

    switch ( lpNMCustomDraw->nmcd.dwDrawStage )
    {
        case CDDS_PREPAINT:
            return CDRF_NOTIFYITEMDRAW;

        case CDDS_ITEMPREPAINT:
        {
            UINT nID = lpNMCustomDraw->nmcd.dwItemSpec;
            bool bHot = ( lpNMCustomDraw->nmcd.uItemState & CDIS_HOT ) != 0;
            if ( pToolBar->SendMessage( TB_ISBUTTONCHECKED, nID ) &&
                 pToolBar->SendMessage( TB_ISBUTTONENABLED, nID ) && bHot )
            {
                // I personally don't like when background of checked item
                // which is currently hot is drawn using dither brush
                lpNMCustomDraw->hbrMonoDither = 0;
            }
            // fall through...
        }
        default:
            return CDRF_DODEFAULT;
    }
}

bool CToolBarEx::HasButtonText( UINT /*nID*/ )
{
    return true;
}

bool CToolBarEx::HasButtonTip( UINT nID )
{
    switch ( m_eTextOptions )
    {
        case toTextLabels:
            return false;
        case toTextOnRight:
            return !HasButtonText( nID );
        case toNoTextLabels:
            return true;
        default:
            ASSERT( false );
            return false;
    }
}

void CToolBarEx::GetButtonText( UINT nID, CString& strText )
{
	CString strFull,strNew;
    if(strFull.LoadString( nID ) )
	{
//		GetText(strFull,strNew,menuhint_item_text);
		( AfxExtractSubString( strText, strFull, 1, _T('\n') ) );	
	}
	else
		{

		int index=CommandToIndex(nID);
		if(index>=0&&index<m_itemName.GetCount())
			strText=m_itemName.GetAt(m_itemName.FindIndex(index));
	}
		
}

void CToolBarEx::GetButtonTip( UINT nID, CString& strTip)
{
    GetButtonText( nID, strTip);
}

void CToolBarEx::Init()
{
    ASSERT( false );    // must be overridden
}

bool CToolBarEx::IsTextOptionAvailable( ETextOptions /*eTextOptions*/ ) const
{
    return true;
}

bool CToolBarEx::IsIconOptionAvailable( EIconOptions /*eIconOptions*/ ) const
{
    return true;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

CReBarCtrl& CToolBarEx::GetParentReBarCtrl() const
{
    return STATIC_DOWNCAST( CReBar, GetParent() )->GetReBarCtrl();
}

int CToolBarEx::GetParentBandIndex() const
{
    int nBand = GetParentReBarCtrl().IDToIndex( ( UINT )GetDlgCtrlID() );
    ASSERT( nBand != -1 );
    return nBand;
}

void CToolBarEx::ReloadButtons()
{
    // Reload buttons from internal cache
    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    for ( int nIndex = 0, nButtons = tbCtrl.GetButtonCount(); nIndex < nButtons; nIndex++ )
    {
        TBBUTTON tbinfo;
        VERIFY( tbCtrl.GetButton( 0, &tbinfo ) );
        VERIFY( GetButtonInfo( tbinfo.idCommand, tbinfo ) );
        VERIFY( tbCtrl.DeleteButton( 0 ) );
        VERIFY( tbCtrl.AddButtons( 1, &tbinfo ) );
    }
}

void CToolBarEx::UpdateParentBandInfo()
{
    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    tbCtrl.AutoSize();

    // Calculate desired height and ideal width of the bar
    CRect rcItem;
    int cyChild = 0;
    int cxIdeal = 0;
    for ( int nIndex = 0, nButtons = tbCtrl.GetButtonCount(); nIndex < nButtons; nIndex++ )
    {
        if ( tbCtrl.GetItemRect( nIndex, rcItem ) )
        {
            cxIdeal += rcItem.Width();
            cyChild = max( cyChild, rcItem.Height() );
        }
    }

    // Modify parent band info accordingly
    REBARBANDINFO rbbi;
    rbbi.cbSize     = sizeof( rbbi );
    rbbi.fMask      = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
    rbbi.cxIdeal    = cxIdeal;
    rbbi.cxMinChild = 0;
    rbbi.cyMinChild = cyChild;
    VERIFY( GetParentReBarCtrl().SetBandInfo( GetParentBandIndex(), &rbbi ) );

    Invalidate();   // visual feedback
	
}

bool CToolBarEx::GetButtonInfo( UINT nID, TBBUTTON& tbinfo )
{
    if ( tbinfo.fsStyle & TBSTYLE_SEP )
    {
        return true;
    }

    for ( int nIndex = 0; nIndex <= m_aButtons.GetUpperBound(); nIndex++ )
    {
        if ( ( UINT )m_aButtons[ nIndex ].tbinfo.idCommand == nID )
        {
            tbinfo = m_aButtons[ nIndex ].tbinfo;
            return true;
        }
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////
// CToolBarEx message handlers

BEGIN_MESSAGE_MAP(CToolBarEx, CToolBar)
    //{{AFX_MSG_MAP(CToolBarEx)
    ON_COMMAND(ID_CUSTOMIZE, OnCustomize)
    //}}AFX_MSG_MAP

    ON_MESSAGE( WM_REBAR_CONTEXTMENU, OnReBarContextMenu )
    ON_MESSAGE( WM_REBAR_CHEVRONPUSHED, OnReBarChevronPushed )

    ON_NOTIFY_REFLECT( NM_CUSTOMDRAW, OnCustomDraw )
    ON_NOTIFY_REFLECT( TBN_GETINFOTIP, OnGetInfoTip )

    // Toolbar customization
    ON_NOTIFY_REFLECT( TBN_BEGINADJUST, OnBeginAdjust )
    ON_NOTIFY_REFLECT( TBN_ENDADJUST, OnEndAdjust )
    ON_NOTIFY_REFLECT( TBN_QUERYINSERT, OnQueryInsert )
    ON_NOTIFY_REFLECT( TBN_QUERYDELETE, OnQueryDelete )
    ON_NOTIFY_REFLECT( TBN_INITCUSTOMIZE, OnInitCustomize )
    ON_NOTIFY_REFLECT( TBN_GETBUTTONINFO, OnGetButtonInfo )
    ON_NOTIFY_REFLECT( TBN_RESET, OnReset )
    ON_NOTIFY_REFLECT( TBN_TOOLBARCHANGE, OnToolBarChange )

    // Saving and restoring toolbar
    ON_NOTIFY_REFLECT( TBN_SAVE, OnSave )
    ON_NOTIFY_REFLECT( TBN_RESTORE, OnRestore )
END_MESSAGE_MAP()

void CToolBarEx::OnCustomize()
{
    GetToolBarCtrl().Customize();
}

LRESULT CToolBarEx::OnReBarContextMenu( WPARAM wParam, LPARAM /*lParam*/ )
{
    // Add "Customize..." item to context menu.  This menu item
    // allows user to invoke standard "Customize toolbar" dialog.
    if(!m_customize)
		return 0L;
    CMenu* pMenu = ( CMenu* )wParam;
    ASSERT_VALID( pMenu );

    CString strText,strTextNew;
  //  VERIFY( strText.LoadString( IDS_CUSTOMIZE ) );
//	GetText(strText,strTextNew,conmeu_item_text);					
    VERIFY( pMenu->AppendMenu( MF_STRING | ( ( GetStyle() & CCS_ADJUSTABLE ) ? 0 : MF_GRAYED ),
        ID_CUSTOMIZE, strText ) );

    return 0L;
}

LRESULT CToolBarEx::OnReBarChevronPushed( WPARAM wParam, LPARAM /*lParam*/ )
{
    CRect rcChevron( ( LPCRECT )wParam );

    CToolBarPopup menu( this );
    menu.ShowPopup( TPM_LEFTALIGN | TPM_VERTICAL |
            ( CWinAppEx::GetInstance()->IsWin98_2K() ? TPM_VERPOSANIMATION : 0 ),
        CPoint( rcChevron.left, rcChevron.bottom ), rcChevron );

    return 0L;
}

void CToolBarEx::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
{
    *pResult = DoCustomDraw( pNMHDR, this );
}

void CToolBarEx::OnGetInfoTip( NMHDR* pNMHDR, LRESULT* pResult )
{
    NMTBGETINFOTIP* lptbgit = ( NMTBGETINFOTIP* )pNMHDR;
    CString strTip;
    GetButtonTip( lptbgit->iItem, strTip );
    _tcsncpy( lptbgit->pszText, strTip, lptbgit->cchTextMax );

    *pResult = 0;
}

LRESULT CALLBACK CToolBarEx::CBTProc( int nCode, WPARAM wParam, LPARAM lParam )
{
    ASSERT( m_pToolBar != 0 );
    HWND hWnd = ( HWND )wParam;

    switch ( nCode )
    {
        case HCBT_CREATEWND:
            if ( m_pCustomizeDlg == 0 )
            {
                // This is where we have a good chance to subclass standard
                // "Customize toolbar" dialog in order to add to it some of
                // our new features (ie. text and icon options selectors)
                TCHAR szClassName[ 7 ];
                ::GetClassName( hWnd, szClassName, sizeof( szClassName ) / sizeof( szClassName[ 0 ] ) );
                if ( !lstrcmp( _T("#32770"), szClassName ) )
                {
                    m_pCustomizeDlg = new CCustomizeDialog( m_pToolBar );
                    VERIFY( m_pCustomizeDlg->SubclassWindow( hWnd ) );
                    return 0;
                }
            }
            break;

        case HCBT_DESTROYWND:
            if ( ( m_pCustomizeDlg != 0 ) && ( m_pCustomizeDlg->m_hWnd == hWnd ) )
            {
                m_pCustomizeDlg = 0;
                return 0;
            }
            break;
    }

    return ::CallNextHookEx( m_hCBTHook, nCode, wParam, lParam );
}

void CToolBarEx::OnBeginAdjust( NMHDR* /*pNMHDR*/, LRESULT* pResult )
{
    m_pToolBar = this;
    m_hCBTHook = ::SetWindowsHookEx( WH_CBT, CBTProc, 0, ::GetCurrentThreadId() );
    ASSERT( m_hCBTHook != 0 );

    *pResult = 0;
}

void CToolBarEx::OnEndAdjust( NMHDR* /*pNMHDR*/, LRESULT* pResult )
{
    VERIFY( ::UnhookWindowsHookEx( m_hCBTHook ) );
    m_hCBTHook = 0;
    m_pToolBar = 0;

    UpdateParentBandInfo();

    *pResult = 0;
}

void CToolBarEx::OnQueryInsert( NMHDR* /*pNMHDR*/, LRESULT* pResult )
{
    *pResult = TRUE;    // otherwise Customize dialog will not be shown
}

void CToolBarEx::OnQueryDelete( NMHDR* /*pNMHDR*/, LRESULT* pResult )
{
    *pResult = TRUE;    // why not? :)
}

void CToolBarEx::OnInitCustomize( NMHDR* /*pNMHDR*/, LRESULT* pResult )
{
    *pResult = TBNRF_HIDEHELP;      // no help available yet
}

void CToolBarEx::OnGetButtonInfo( NMHDR* pNMHDR, LRESULT* pResult )
{
    NMTOOLBAR* lpnmtb = ( NMTOOLBAR* )pNMHDR;

    int nItem = lpnmtb->iItem;
    if ( ( 0 <= nItem ) && ( nItem <= m_aButtons.GetUpperBound() ) )
    {
        // Copy button info from internal cache
        lpnmtb->tbButton = m_aButtons[ nItem ].tbinfo;
        if ( !( lpnmtb->tbButton.fsStyle & TBSTYLE_SEP ) )
        {
            CString strText;
            GetButtonText( lpnmtb->tbButton.idCommand, strText );
            _tcsncpy( lpnmtb->pszText, strText, lpnmtb->cchText );
        }

        *pResult = TRUE;
    }
    else
    {
        *pResult = FALSE;
    }
}

void CToolBarEx::OnReset( NMHDR* /*pNMHDR*/, LRESULT* pResult )
{
    // Delete all buttons
    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    while ( tbCtrl.DeleteButton( 0 ) );

    // Load buttons from internal cache
    for ( int nIndex = 0; nIndex <= m_aButtons.GetUpperBound(); nIndex++ )
    {
        if ( m_aButtons[ nIndex ].bInitiallyVisible )
        {
            VERIFY( tbCtrl.AddButtons( 1, &m_aButtons[ nIndex ].tbinfo ) );
        }
    }

    // Set default text & icon options
    SetIconOptions( m_eIconOptionsDefault, false );
    SetTextOptions( m_eTextOptionsDefault, false );
    ReloadButtons();
    UpdateParentBandInfo();

    // Synchronize with Customize dialog
    ASSERT( m_pCustomizeDlg != 0 );
    m_pCustomizeDlg->SetTextOptions( m_eTextOptions, true );
    m_pCustomizeDlg->SetIconOptions( m_eIconOptions, true );

    *pResult = 0;
}

void CToolBarEx::OnToolBarChange( NMHDR* /*pNMHDR*/, LRESULT* pResult )
{
    UpdateParentBandInfo();

    *pResult = 0;
}

void CToolBarEx::OnSave( NMHDR* pNMHDR, LRESULT* pResult )
{
    NMTBSAVE* lpnmtb = ( NMTBSAVE* )pNMHDR;
    if ( lpnmtb->iItem == -1 )
    {
        lpnmtb->cbData  += sizeof( DWORD ) * 2;
        lpnmtb->pData    = ( LPDWORD )::GlobalAlloc( GMEM_FIXED, lpnmtb->cbData );
        lpnmtb->pCurrent = lpnmtb->pData;

        *lpnmtb->pCurrent++ = ( DWORD )m_eTextOptions;
        *lpnmtb->pCurrent++ = ( DWORD )m_eIconOptions;
    }

    *pResult = 0;
}

void CToolBarEx::OnRestore( NMHDR* pNMHDR, LRESULT* pResult )
{
    NMTBRESTORE* lpnmtb = ( NMTBRESTORE* )pNMHDR;
    if ( lpnmtb->iItem == -1 )
    {
        lpnmtb->cButtons = ( lpnmtb->cbData - sizeof( DWORD ) * 2 ) / lpnmtb->cbBytesPerRecord;
        lpnmtb->pCurrent = lpnmtb->pData;

        SetTextOptions( ( ETextOptions )*lpnmtb->pCurrent++, false );
        SetIconOptions( ( EIconOptions )*lpnmtb->pCurrent++, false );
    }
    else
    {
        VERIFY( GetButtonInfo( lpnmtb->tbButton.idCommand, lpnmtb->tbButton ) );
    }

    *pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CToolBarPopup

HHOOK           CToolBarPopup::m_hKeyboardHook  = 0;
CToolBarPopup*  CToolBarPopup::m_pPopup         = 0;

IMPLEMENT_DYNAMIC( CToolBarPopup, CWnd )

CToolBarPopup::CToolBarPopup( CToolBarEx* pToolBar )
{
    ASSERT_VALID( pToolBar );

    m_pToolBar    = pToolBar;
    m_bOverTbCtrl = false;
    m_bTextLabels = false;
}

CToolBarPopup::~CToolBarPopup()
{
}

/////////////////////////////////////////////////////////////////////////////
// Operations

bool CToolBarPopup::ShowPopup( UINT nFlags, CPoint pt, CRect& rcExclude )
{
    CString strWndClass = AfxRegisterWndClass( CS_SAVEBITS,
        ::LoadCursor( 0, IDC_ARROW ), ::GetSysColorBrush( COLOR_MENU ), 0 );

    if ( !CreateEx( WS_EX_PALETTEWINDOW, strWndClass, 0,
            WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_DLGFRAME,
            CRect( 0, 0, 0, 0 ), 0, 0 ) )
    {
        return false;
    }

    SetPosition( nFlags, pt, rcExclude );
    SetCapture();
    SendMessage( WM_SETCURSOR, ( WPARAM )m_hWnd, MAKELPARAM( HTCLIENT, 0 ) );

    m_pPopup = this;
    m_hKeyboardHook = ::SetWindowsHookEx( WH_KEYBOARD, KeyboardProc, 0, ::GetCurrentThreadId() );
    ASSERT( m_hKeyboardHook != 0 );

    // Emulate menu loop
    RunModalLoop( MLF_NOKICKIDLE );

    VERIFY( ::UnhookWindowsHookEx( m_hKeyboardHook ) );
    m_hKeyboardHook = 0;
    m_pPopup = 0;

    DestroyWindow();

    return true;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

void CToolBarPopup::SetPosition( UINT nFlags, CPoint pt, CRect& rcExclude )
{
    // The main purpose of this functions is to find proper position of
    // the popup window, so that we neither exceed screen dimensions nor
    // intersect rcExclude rectangle.  The function also takes into account
    // control flags specified by the user.

    // Calc initial position
    CRect rc;
    m_tbCtrl.GetWindowRect( rc );

    CalcWindowRect( rc, CWnd::adjustBorder );
    rc.OffsetRect( -rc.TopLeft() );
    rc.OffsetRect(
        ( nFlags & TPM_CENTERALIGN ) ? ( pt.x - rc.Width() / 2 ) :
        ( nFlags & TPM_RIGHTALIGN  ) ? ( pt.x - rc.Width() ) : pt.x, 0 );
    rc.OffsetRect( 0,
        ( nFlags & TPM_VCENTERALIGN ) ? ( pt.y - rc.Height() / 2 ) :
        ( nFlags & TPM_BOTTOMALIGN  ) ? ( pt.y - rc.Height() ) : pt.y );

    // Make sure we don't exceed screen dimensions
    CRect rcDesktop;
    GetDesktopWindow()->GetWindowRect( rcDesktop );

    rc.OffsetRect(
        min( rcDesktop.right  - rc.right,  0 ),
        min( rcDesktop.bottom - rc.bottom, 0 ) );
    rc.OffsetRect(
        max( rcDesktop.left   - rc.left,   0 ),
        max( rcDesktop.top    - rc.top,    0 ) );

    // Make sure we don't intersect rcExclude rectangle
    CRect rcTemp;
    if ( !rcExclude.IsRectEmpty() && rcTemp.IntersectRect( rc, rcExclude ) )
    {
        if ( nFlags & TPM_VERTICAL )
        {
            CRect rcUp( rc );
            int nUp = rc.bottom - rcExclude.top;
            rcUp.OffsetRect( 0, -nUp );

            CRect rcDown( rc );
            int nDown = rcExclude.bottom - rc.top;
            rcDown.OffsetRect( 0, nDown );

            bool bUp = false;
            if ( ( rcUp.top >= rcDesktop.top ) && ( rcDown.bottom <= rcDesktop.bottom ) )
            {
                bUp = ( nUp < nDown );
            }
            else if ( rcUp.top >= rcDesktop.top )
            {
                bUp = true;
            }
            else if ( rcDown.bottom <= rcDesktop.bottom )
            {
                bUp = false;
            }
            else
            {
                ASSERT( false );    // this case is not supported yet
            }

            rc = bUp ? rcUp : rcDown;
            nFlags &= ~( TPM_HORPOSANIMATION | TPM_HORNEGANIMATION | TPM_VERPOSANIMATION | TPM_VERNEGANIMATION );
            nFlags |=  ( bUp ? TPM_VERNEGANIMATION : TPM_VERPOSANIMATION );
        }
        else
        {
            CRect rcLeft( rc );
            int nLeft = rc.right - rcExclude.left;
            rcLeft.OffsetRect( -nLeft, 0 );

            CRect rcRight( rc );
            int nRight = rcExclude.right - rc.left;
            rcRight.OffsetRect( nRight, 0 );

            bool bLeft = false;
            if ( ( rcLeft.left >= rcDesktop.top ) && ( rcRight.right <= rcDesktop.right ) )
            {
                bLeft = ( nLeft < nRight );
            }
            else if ( rcLeft.left >= rcDesktop.left )
            {
                bLeft = true;
            }
            else if ( rcRight.right <= rcDesktop.right )
            {
                bLeft = false;
            }
            else
            {
                ASSERT( false );    // this case is not supported yet
            }

            rc = bLeft ? rcLeft : rcRight;
            nFlags &= ~( TPM_HORPOSANIMATION | TPM_HORNEGANIMATION | TPM_VERPOSANIMATION | TPM_VERNEGANIMATION );
            nFlags |=  ( bLeft ? TPM_HORNEGANIMATION : TPM_HORPOSANIMATION );
        }
    }

    Show( nFlags, rc );
}

void CToolBarPopup::Show( UINT nFlags, const CRect& rc )
{
    // On W98/W2K systems the menu animation feature is supported.
    // This function tries to mimic this feature conformably to
    // toolbar popup window.

    // Position window on the screen
    SetWindowPos( &wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(),
        SWP_NOACTIVATE );

    CWinAppEx* pApp = CWinAppEx::GetInstance();
    if ( pApp->IsWin98_2K() && !( nFlags & TPM_NOANIMATION ) ) // W98/W2K specific (menu animation)
    {
        if ( pApp->GetMenuAnimation() )
        {
            DWORD dwAnimationFlags = AW_SLIDE |
                ( ( nFlags & TPM_HORPOSANIMATION ) ? AW_HOR_POSITIVE : 0 ) |
                ( ( nFlags & TPM_HORNEGANIMATION ) ? AW_HOR_NEGATIVE : 0 ) |
                ( ( nFlags & TPM_VERPOSANIMATION ) ? AW_VER_POSITIVE : 0 ) |
                ( ( nFlags & TPM_VERNEGANIMATION ) ? AW_VER_NEGATIVE : 0 );
            if ( dwAnimationFlags == AW_SLIDE )
            {
                // If none of TPM_*ANIMATION flags is set, set default animation
                dwAnimationFlags |= AW_HOR_POSITIVE | AW_VER_POSITIVE;
            }

            if ( pApp->IsWin2K() && pApp->GetMenuFade() )   // W2K specific (fade effect)
            {
                dwAnimationFlags = AW_BLEND;
            }

            VERIFY( ::AnimateWindowWin50( m_hWnd, 200, dwAnimationFlags ) );
            return;
        }
    }

    // The animation feature is whether turned off or unsupported
    // on this system.  Therefore, just show window without activation.
    ShowWindow( SW_SHOWNOACTIVATE );
}

void CToolBarPopup::OnKeyDown( UINT nChar )
{
    switch ( nChar )
    {
        case VK_ESCAPE: // dismiss menu
        case VK_MENU:
            SendMessage( WM_TB_ENDMODALLOOP );
            break;

        case VK_UP:     // select next/prev button
        case VK_DOWN:
        {
            int nLastItem = m_tbCtrl.GetButtonCount() - 1;
            int nHotItem  = m_tbCtrl.GetHotItem();

            for ( int nIndex = 0; nIndex <= nLastItem; nIndex++ )
            {
                if ( nHotItem >= 0 )
                {
                    nHotItem = ( nChar == VK_UP ) ?
                        ( nHotItem ? nHotItem - 1 : nLastItem ) :
                        ( nHotItem == nLastItem ? 0 : nHotItem + 1 );
                }
                else
                {
                    nHotItem = ( nChar == VK_UP ) ?
                        nLastItem : 0;
                }

                m_tbCtrl.SetHotItem( nHotItem );
                if ( m_tbCtrl.GetHotItem() == nHotItem )
                {
                    break;
                }
            }
            break;
        }
        case VK_RETURN: // issue command associated with selected button
        {
            int nHotItem = m_tbCtrl.GetHotItem();
            if ( nHotItem >= 0 )
            {
                TBBUTTON tbinfo;
                VERIFY( m_tbCtrl.GetButton( nHotItem, &tbinfo ) );
                if ( tbinfo.idCommand != 0 )
                {
                    SendMessage( WM_COMMAND, tbinfo.idCommand );
                    break;
                }
            }

            SendMessage( WM_TB_ENDMODALLOOP );
            break;
        }
        default:
            break;
    }
}

LRESULT CALLBACK CToolBarPopup::KeyboardProc( int code, WPARAM wParam, LPARAM lParam )
{
    ASSERT( m_pPopup != 0 );

    if ( code == HC_ACTION )
    {
        CWnd* pCapture = GetCapture();
        if ( ( pCapture == m_pPopup ) ||
             ( pCapture == &m_pPopup->m_tbCtrl ) )
        {
            if ( !( HIWORD( lParam ) & KF_UP ) )
            {
                m_pPopup->OnKeyDown( wParam );
            }

            return 1;
        }
    }

    return ::CallNextHookEx( m_hKeyboardHook, code, wParam, lParam );
}

/////////////////////////////////////////////////////////////////////////////
// Overrides

BOOL CToolBarPopup::OnCommand( WPARAM wParam, LPARAM /*lParam*/ )
{
    // Dismiss menu
    SendMessage( WM_TB_ENDMODALLOOP );

    // Forward command to the original toolbar window
    m_pToolBar->PostMessage( WM_COMMAND, LOWORD( wParam ), 0 );

    return TRUE;    // command was processed
}

BOOL CToolBarPopup::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
    NMHDR* pNMHDR = ( NMHDR* )lParam;
    if ( pNMHDR->hwndFrom == m_tbCtrl.m_hWnd )
    {
        // Handle certain notifications from embedded toolbar control
        switch ( pNMHDR->code )
        {
            case NM_RELEASEDCAPTURE:
                SetCapture();
                *pResult = 0;
                break;

            case NM_CUSTOMDRAW:
                *pResult = m_pToolBar->DoCustomDraw( pNMHDR, &m_tbCtrl );
                break;

            case TBN_GETINFOTIP:
                if ( !m_bTextLabels )
                {
                    NMTBGETINFOTIP* lptbgit = ( NMTBGETINFOTIP* )pNMHDR;
                    CString strTip;
                    m_pToolBar->GetButtonTip( lptbgit->iItem, strTip );
                    _tcsncpy( lptbgit->pszText, strTip, lptbgit->cchTextMax );
                }
                *pResult = 0;
                break;

            default:    // forward message to the parent of the original toolbar
                *pResult = m_pToolBar->GetParent()->SendMessage( WM_NOTIFY, wParam, lParam );
                break;
        }

        return TRUE;
    }

    return CWnd::OnNotify( wParam, lParam, pResult );
}

LRESULT CToolBarPopup::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
    if ( WM_MOUSEFIRST <= message && message <= WM_MOUSELAST )
    {
        DWORD dwPos = ::GetMessagePos();
        CPoint ptScreen( LOWORD( dwPos ), HIWORD( dwPos ) );
        CWnd* pWnd = WindowFromPoint( ptScreen );
        if ( pWnd != 0 )
        {
            CPoint ptClient( ptScreen );
            pWnd->ScreenToClient( &ptClient );

            switch ( message )
            {
                case WM_MOUSEMOVE:
                {
                    // Check if hot item should be changed
                    bool bOverTbCtrl = ( pWnd == &m_tbCtrl );
                    if ( bOverTbCtrl )
                    {
                        int nHit = m_tbCtrl.HitTest( &ptClient );
                        m_tbCtrl.SetHotItem( nHit );

                        // Let tooltip control process mouse event
                        CToolTipCtrl* pTtCtrl = m_tbCtrl.GetToolTips();
                        if ( pTtCtrl != 0 )
                        {
                            MSG msg;
                            msg.hwnd    = m_tbCtrl.m_hWnd;
                            msg.message = WM_MOUSEMOVE;
                            msg.wParam  = wParam;
                            msg.lParam  = MAKELPARAM( ptClient.x, ptClient.y );
                            msg.pt      = ptScreen;
                            msg.time    = ::GetMessageTime();
                            pTtCtrl->RelayEvent( &msg );
                        }
                    }
                    else if ( m_bOverTbCtrl )
                    {
                        m_tbCtrl.SetHotItem( -1 );
                    }

                    m_bOverTbCtrl = bOverTbCtrl;
                    return 0L;
                }
                case WM_LBUTTONDOWN:
                    if ( pWnd != this )
                    {
                        // Dismiss menu if user has clicked outside the window
                        if ( pWnd != &m_tbCtrl )
                        {
                            SendMessage( WM_TB_ENDMODALLOOP );
                        }

                        // Forward this mouse event to the window that was clicked
                        LPARAM nPosition = MAKELPARAM( ptScreen.x, ptScreen.y );
                        WPARAM nHitTest  = pWnd->SendMessage( WM_NCHITTEST, 0, nPosition );
                        if ( nHitTest == HTCLIENT )
                        {
                            nPosition = MAKELPARAM( ptClient.x, ptClient.y );
                            pWnd->PostMessage( WM_LBUTTONDOWN, wParam, nPosition );
                        }
                        else
                        {
                            pWnd->PostMessage( WM_NCLBUTTONDOWN, nHitTest, nPosition );
                        }
                    }
                    return 0L;

                default:
                    break;
            }
        }
    }

    return CWnd::WindowProc( message, wParam, lParam );
}

/////////////////////////////////////////////////////////////////////////
// CToolBarPopup message handlers

BEGIN_MESSAGE_MAP(CToolBarPopup, CWnd)
    //{{AFX_MSG_MAP(CToolBarPopup)
    ON_WM_CAPTURECHANGED()
    ON_WM_CREATE()
    ON_WM_MOUSEACTIVATE()
    //}}AFX_MSG_MAP
    ON_MESSAGE_VOID( WM_TB_ENDMODALLOOP, OnEndModalLoop )
END_MESSAGE_MAP()

void CToolBarPopup::OnEndModalLoop()
{
    EndModalLoop( 0 );
}

void CToolBarPopup::OnCaptureChanged( CWnd* pWnd )
{
    if ( ( pWnd != this ) && ( pWnd != &m_tbCtrl ) && ContinueModal() )
    {
        PostMessage( WM_TB_ENDMODALLOOP );  // dismiss menu
    }

    CWnd::OnCaptureChanged( pWnd );
}

int CToolBarPopup::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
    if ( CWnd::OnCreate( lpCreateStruct ) == -1 )
    {
        return -1;
    }

    // Create embedded toolbar control
    if ( !m_tbCtrl.Create(
            TBSTYLE_FLAT | TBSTYLE_WRAPABLE | TBSTYLE_LIST | TBSTYLE_TOOLTIPS |
            CCS_NODIVIDER | CCS_NOPARENTALIGN | CCS_NORESIZE |
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            CRect( 0, 0, 0, 0 ), this, m_pToolBar->GetDlgCtrlID() ) )
    {
        return -1;
    }

    m_tbCtrl.SetExtendedStyle( TBSTYLE_EX_DRAWDDARROWS | TBSTYLE_EX_MIXEDBUTTONS );
    VERIFY( m_tbCtrl.GetToolTips()->ModifyStyle( 0, TTS_ALWAYSTIP ) );

    // There could be only two modes, depending on text options
    // of the original toolbar: with or without text labels.
    ETextOptions eTextOptions = m_pToolBar->GetTextOptions();
    m_bTextLabels =
        ( eTextOptions == toTextLabels ) ||
        ( eTextOptions == toTextOnRight );

    // Copy all required information from the original toolbar
    CToolBarCtrl& tbCtrl = m_pToolBar->GetToolBarCtrl();
    m_tbCtrl.SetImageList( tbCtrl.GetImageList() );
    m_tbCtrl.SetHotImageList( tbCtrl.GetHotImageList() );
    m_tbCtrl.SetDisabledImageList( tbCtrl.GetDisabledImageList() );

    CRect rcItem, rcClient;
    tbCtrl.GetClientRect( rcClient );

    TBBUTTON tbinfo;
    int nMaxWidth = 0;
    int nButtons = tbCtrl.GetButtonCount();
    int nIndex;
    for ( nIndex = 0; nIndex < nButtons; nIndex++ )
    {
        tbCtrl.GetItemRect( nIndex, rcItem );
        if ( rcItem.right > rcClient.right )
        {
            VERIFY( tbCtrl.GetButton( nIndex, &tbinfo ) );
            if ( !( tbinfo.fsStyle & TBSTYLE_SEP ) && !( tbinfo.fsState & TBSTATE_HIDDEN ) )
            {
                CString strButtonText;
                m_pToolBar->GetButtonText( tbinfo.idCommand, strButtonText );
                CString strToAdd( strButtonText, strButtonText.GetLength() + 1 );
                tbinfo.iString  = m_tbCtrl.AddStrings( strToAdd );
                tbinfo.fsStyle |= TBSTYLE_AUTOSIZE | ( m_bTextLabels ? BTNS_SHOWTEXT : 0 );
                VERIFY( m_tbCtrl.AddButtons( 1, &tbinfo ) );
                VERIFY( m_tbCtrl.GetItemRect( m_tbCtrl.CommandToIndex( tbinfo.idCommand ), rcItem ) );
                nMaxWidth = max( nMaxWidth, rcItem.Width() );
            }
        }
    }

    nButtons = m_tbCtrl.GetButtonCount();
    if ( nButtons == 0 )
    {
        ASSERT( false );    // this should never happen
        return -1;
    }

    if ( m_bTextLabels )
    {
        TBBUTTONINFO tbbi;
        tbbi.cbSize = sizeof( tbbi );
        tbbi.dwMask = TBIF_SIZE | TBIF_STYLE;
        for ( nIndex = 0; nIndex < nButtons; nIndex++ )
        {
            VERIFY( m_tbCtrl.GetButton( nIndex, &tbinfo ) );
            tbbi.cx      = ( WORD )nMaxWidth;
            tbbi.fsStyle = ( BYTE )( tbinfo.fsStyle & ~TBSTYLE_AUTOSIZE );
            VERIFY( m_tbCtrl.SetButtonInfo( tbinfo.idCommand, &tbbi ) );
        }
    }

    m_tbCtrl.AutoSize();

    // Calc toolbar size
    if ( nButtons > 1 )
    {
        m_tbCtrl.SetRows( nButtons, m_bTextLabels, rcClient );
    }
    else
    {
        VERIFY( m_tbCtrl.GetItemRect( 0, rcClient ) );
    }

    m_tbCtrl.MoveWindow( rcClient );

    return 0;
}

int CToolBarPopup::OnMouseActivate( CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/, UINT /*message*/ )
{
    return MA_NOACTIVATEANDEAT; // just in case 
}

void CToolBarEx::AddButton(int index,TBBUTTONEX lpButtons, CString text)
{
	if(text.IsEmpty())
		GetButtonText(lpButtons.tbinfo.idCommand, text);

    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
	if ( lpButtons.bInitiallyVisible )
	{
		VERIFY( tbCtrl.AddButtons( 1, &lpButtons.tbinfo) );
	}
	
	m_aButtons.Add( lpButtons);
	SetTextOptions(index,toTextOnRight,text);
	m_itemName.AddTail(text);
}

void CToolBarEx::SetTextOptions(int index,ETextOptions eTextOptions, CString text)
{
    ASSERT( ::IsWindow( m_hWnd ) );
    ASSERT( GetStyle() & TBSTYLE_TOOLTIPS );
    ASSERT( !( GetBarStyle() & CBRS_TOOLTIPS ) );
    ASSERT( IsTextOptionAvailable( eTextOptions ) );

    m_eTextOptions = eTextOptions;

    // Modify toolbar style according to new text options
    ModifyStyle(
        ( eTextOptions == toTextOnRight ) ? 0 : TBSTYLE_LIST,
        ( eTextOptions == toTextOnRight ) ? TBSTYLE_LIST : 0 );

    CToolBarCtrl& tbCtrl = GetToolBarCtrl();
    DWORD dwStyleEx = tbCtrl.GetExtendedStyle();
    tbCtrl.SetExtendedStyle(
        ( eTextOptions == toTextOnRight ) ?
            ( dwStyleEx |  TBSTYLE_EX_MIXEDBUTTONS ) :
            ( dwStyleEx & ~TBSTYLE_EX_MIXEDBUTTONS ) );
    VERIFY( tbCtrl.SetMaxTextRows(
        ( eTextOptions == toNoTextLabels ) ? 0 : 1 ) );

    // Modify all (even currently hidden ones) buttons in internal cache
    //  for ( nIndex = 0; nIndex <= m_aButtons.GetUpperBound(); nIndex++ )
    {
        TBBUTTON& tbinfo = m_aButtons[ index ].tbinfo;
        if ( !( tbinfo.fsStyle & TBSTYLE_SEP ) )
        {
            CString strButtonText=text;
			CString strToAdd( strButtonText, strButtonText.GetLength() + 1 );
            tbinfo.iString = tbCtrl.AddStrings( strToAdd );

            switch ( eTextOptions )
            {
                case toTextLabels:
                    tbinfo.fsStyle &= ~( TBSTYLE_AUTOSIZE | BTNS_SHOWTEXT );
                    break;

                case toTextOnRight:
                    tbinfo.fsStyle |= ( TBSTYLE_AUTOSIZE |
                        ( HasButtonText( tbinfo.idCommand ) ? BTNS_SHOWTEXT : 0 ) );
                    break;

                case toNoTextLabels:
                    tbinfo.fsStyle &= ~BTNS_SHOWTEXT;
                    tbinfo.fsStyle |= TBSTYLE_AUTOSIZE;
                    break;
            }
        }
    }

    // If requested, reflect changes immediately
//    if ( bUpdate )
  ////  {
        ReloadButtons();
        UpdateParentBandInfo();
   // }
}

void CToolBarEx::SetCustomizeMode(bool customize)
{
	m_customize=customize;
}

bool CToolBarEx::AddGloomIcon(CImageList *imagelist, HICON hIcon)
{
	ICONINFO iconInfo;
	ZeroMemory(&iconInfo,sizeof(iconInfo));
	if(!GetIconInfo(hIcon,&iconInfo))
		return FALSE;
	
	CSize size = ( m_eIconOptions == ioSmallIcons ) ? szImageSmall : szImageLarge;
	CDC myDC;
	myDC.CreateCompatibleDC(0);
	
	CBitmap bmColor;
	bmColor.Attach(iconInfo.hbmColor);
	CBitmap bmMask;
	bmMask.Attach(iconInfo.hbmMask);
	
	CBitmap* pOldBitmap = myDC.SelectObject(&bmColor);
	COLORREF crPixel;
	for(int i=0;i<size.cx;++i)
	{
		for(int j=0;j<size.cy;++j)
		{
			crPixel = myDC.GetPixel(i,j);
			myDC.SetPixel(i,j,DarkenColor(25,crPixel));
		}
	}
	myDC.SelectObject(pOldBitmap);
	imagelist->Add(&bmColor,&bmMask);
	return TRUE;
}

COLORREF CToolBarEx::DarkenColor(long lScale, COLORREF lColor)
{
	long R = MulDiv(GetRValue(lColor),(255-lScale),255);
	long G = MulDiv(GetGValue(lColor),(255-lScale),255);
	long B = MulDiv(GetBValue(lColor),(255-lScale),255);
	
	return RGB(R, G, B); 
}

bool CToolBarEx::AddGrayIcon(CImageList *imagelist, HICON hIcon)
{
	ICONINFO iconInfo;
	ZeroMemory(&iconInfo,sizeof(iconInfo));
	if(!GetIconInfo(hIcon,&iconInfo))
		return FALSE;
	
	CSize size = ( m_eIconOptions == ioSmallIcons ) ? szImageSmall : szImageLarge;
	CDC myDC;
	myDC.CreateCompatibleDC(0);
	
	CBitmap bmColor;
	bmColor.Attach(iconInfo.hbmColor);
	CBitmap bmMask;
	bmMask.Attach(iconInfo.hbmMask);
	
	CBitmap* pOldBitmap = myDC.SelectObject(&bmColor);
	COLORREF crMenu = GetSysColor(COLOR_MENU);
	COLORREF crPixel;
	for(int i=0;i<size.cx;++i)
	{
		for(int j=0;j<size.cy;++j)
		{
			crPixel = myDC.GetPixel(i,j);
			//myDC.SetPixel(i,j,GrayColor(crPixel));
			//myDC.SetPixel(i,j,MidColor(GrayColor(crPixel),crMenu));      
			myDC.SetPixel(i,j,MixedColor(LightenColor(100,GrayColor(crPixel)),crMenu));      
		}
	}
	myDC.SelectObject(pOldBitmap);
	imagelist->Add(&bmColor,&bmMask);
	return TRUE;
}

COLORREF CToolBarEx::MixedColor(COLORREF ColA, COLORREF ColB)
{
	// ( 86a + 14b ) / 100
	int Red   = MulDiv(86,GetRValue(ColA),100) + MulDiv(14,GetRValue(ColB),100);
	int Green = MulDiv(86,GetGValue(ColA),100) + MulDiv(14,GetGValue(ColB),100);
	int Blue  = MulDiv(86,GetBValue(ColA),100) + MulDiv(14,GetBValue(ColB),100);
	
	return RGB( Red,Green,Blue);
}

COLORREF CToolBarEx::LightenColor(long lScale, COLORREF lColor)
{
	long R = MulDiv(255-GetRValue(lColor),lScale,255)+GetRValue(lColor);
	long G = MulDiv(255-GetGValue(lColor),lScale,255)+GetGValue(lColor);
	long B = MulDiv(255-GetBValue(lColor),lScale,255)+GetBValue(lColor);
	
	return RGB(R, G, B); 
}

COLORREF CToolBarEx::GrayColor(COLORREF crColor)
{
	int Gray  = (int)(((int)GetRValue(crColor)*0.3) + GetGValue(crColor)*0.59 + GetBValue(crColor)*0.11);
	return RGB( Gray,Gray,Gray);
}

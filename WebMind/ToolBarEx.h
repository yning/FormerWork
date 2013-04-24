/////////////////////////////////////////////////////////////////////////////
// ToolBarEx.h: interface for the CToolBarEx class.
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

#ifndef __TOOLBAREX_H__
#define __TOOLBAREX_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CustomizeDialog.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx

struct TBBUTTONEX
{
   	TBBUTTON tbinfo;
	bool     bInitiallyVisible;
};

const CSize szImageSmall( 16, 16 );
const CSize szImageLarge( 24, 24);

class CToolBarEx : public CToolBar
{
    DECLARE_DYNAMIC( CToolBarEx );

    friend class CToolBarPopup;
    friend class CCustomizeDialog;

// Construction
public:
    CToolBarEx();
    ~CToolBarEx();

    BOOL Create( CWnd* pParentWnd, DWORD dwStyle, UINT nID = AFX_IDW_TOOLBAR );

// Attributes
public:
    void SetTextOptions( ETextOptions eTextOptions, bool bUpdate = true );
    ETextOptions GetTextOptions() const;
    void SetIconOptions( EIconOptions eIconOptions, bool bUpdate = true );
    EIconOptions GetIconOptions() const;

// Operations
public:
    void SetBitmaps( UINT nIDSmallCold, UINT nIDSmallHot,
                     UINT nIDLargeCold, UINT nIDLargeHot,
                     EIconOptions eIconOptionsDefault,
                     COLORREF clrMask = RGB( 255, 0, 255 ) );
    void SetButtons( int nNumButtons, TBBUTTONEX* lpButtons,
                     ETextOptions eTextOptionsDefault );

    void LoadState( LPCTSTR lpszProfileName );
    void SaveState( LPCTSTR lpszProfileName );

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CToolBarEx)
    protected:
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    //}}AFX_VIRTUAL

    virtual LRESULT DoCustomDraw( NMHDR* pNMHDR, CWnd* pToolBar );

    virtual bool HasButtonText( UINT nID );
    virtual bool HasButtonTip( UINT nID );
    virtual void GetButtonText( UINT nID, CString& strText);
    virtual void GetButtonTip( UINT nID, CString& strTip);

public:
	COLORREF GrayColor(COLORREF crColor);
	COLORREF LightenColor( long lScale, COLORREF lColor);
	COLORREF MixedColor(COLORREF ColA,COLORREF ColB);
	bool AddGrayIcon(CImageList *imagelist ,HICON hIcon);
	COLORREF DarkenColor( long lScale, COLORREF lColor);
	bool AddGloomIcon(CImageList *imagelist, HICON hIcon);
	void SetCustomizeMode(bool customize);
	void SetTextOptions(int index, ETextOptions eTextOptions, CString text );
	void AddButton(int index,TBBUTTONEX  lpButtons,CString text = _T(""));
    virtual void Init();
    virtual bool IsTextOptionAvailable( ETextOptions eTextOptions ) const;
    virtual bool IsIconOptionAvailable( EIconOptions eIconOptions ) const;

// Implementation
protected:
    CReBarCtrl& GetParentReBarCtrl() const;
    int GetParentBandIndex() const;
    void ReloadButtons();
    void UpdateParentBandInfo();
    bool GetButtonInfo( UINT nID, TBBUTTON& tbinfo );

    static LRESULT CALLBACK CBTProc( int nCode, WPARAM wParam, LPARAM lParam );

// Implementation data
protected:
    CArray< TBBUTTONEX, TBBUTTONEX& > m_aButtons;
	CStringList         m_itemName;

    ETextOptions    m_eTextOptions;
    EIconOptions    m_eIconOptions;
    ETextOptions    m_eTextOptionsDefault;
    EIconOptions    m_eIconOptionsDefault;
    UINT            m_nIDSmallCold;
    UINT            m_nIDSmallHot;
    UINT            m_nIDLargeCold;
    UINT            m_nIDLargeHot;
    CImageList      m_imageListCold;
    CImageList      m_imageListHot;
	CImageList		m_imageListDisabled;
    COLORREF        m_clrMask;
	bool            m_customize;
    static LPCTSTR              m_lpszStateInfoEntry;
    static CToolBarEx*          m_pToolBar;
    static CCustomizeDialog*    m_pCustomizeDlg;
    static HHOOK                m_hCBTHook;

// Generated message map functions
protected:
    //{{AFX_MSG(CToolBarEx)
    afx_msg void OnCustomize();
    //}}AFX_MSG

    afx_msg LRESULT OnReBarContextMenu( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnReBarChevronPushed( WPARAM wParam, LPARAM lParam );

    afx_msg void OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnGetInfoTip( NMHDR* pNMHDR, LRESULT* pResult );

    // Toolbar customization
    afx_msg void OnBeginAdjust( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnEndAdjust( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnQueryInsert( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnQueryDelete( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnInitCustomize( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnGetButtonInfo( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnReset( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnToolBarChange( NMHDR* pNMHDR, LRESULT* pResult );

    // Saving and restoring toolbar
    afx_msg void OnSave( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnRestore( NMHDR* pNMHDR, LRESULT* pResult );
    DECLARE_MESSAGE_MAP();
};

/////////////////////////////////////////////////////////////////////////////
// CToolBarPopup

class CToolBarPopup : public CWnd
{
    DECLARE_DYNAMIC( CToolBarPopup );

// Construction
public:
    CToolBarPopup( CToolBarEx* pToolBar );
    ~CToolBarPopup();

// Operations
public:
    bool ShowPopup( UINT nFlags, CPoint pt, CRect& rcExclude );

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CToolBarPopup)
    protected:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

// Implementation
protected:
    void SetPosition( UINT nFlags, CPoint pt, CRect& rcExclude );
    void Show( UINT nFlags, const CRect& rc );
    void OnKeyDown( UINT nChar );

    static LRESULT CALLBACK KeyboardProc( int code, WPARAM wParam, LPARAM lParam );

// Implementation data
protected:
    CToolBarEx*     m_pToolBar;
    CToolBarCtrl    m_tbCtrl;
    bool            m_bOverTbCtrl;
    bool            m_bTextLabels;

    static HHOOK            m_hKeyboardHook;
    static CToolBarPopup*   m_pPopup;

// Generated message map functions
protected:
    //{{AFX_MSG(CToolBarPopup)
    afx_msg void OnCaptureChanged(CWnd *pWnd);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG
    afx_msg void OnEndModalLoop();
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif  // !__MENUBAR_H__

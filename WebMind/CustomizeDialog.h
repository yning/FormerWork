/////////////////////////////////////////////////////////////////////////////
// CustomizeDialog.h: interface for the CCustomizeDialog class.
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

#ifndef __CUSTOMIZEDIALOG_H__
#define __CUSTOMIZEDIALOG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonRes.h"
#include "GlobalData.h"
#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// Options

enum ETextOptions
{
    toTextLabels    =  0,
    toTextOnRight   =  1,
    toNoTextLabels  =  2,
    toNone          = -1,
};

enum EIconOptions
{
    ioSmallIcons    =  0,
    ioLargeIcons    =  1,
    ioNone          = -1,
};

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog

class CCustomizeDialog;

class COptionsDialog : public CDialog
{
// Construction
public:
    COptionsDialog( ETextOptions eTextOptions,
                    EIconOptions eIconOptions );

// Dialog Data
    //{{AFX_DATA(COptionsDialog)
    enum { IDD = IDD_CUSTOMIZE };
    CComboBox m_cbTextOptions;
    CComboBox m_cbIconOptions;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COptionsDialog)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Operations
public:
	void SetLanguage();
    bool SelectTextOption( ETextOptions eTextOptions );
    bool SelectIconOption( EIconOptions eIconOptions );

// Implementation
protected:
    CCustomizeDialog* GetCustomizeDialog() const;

// Implementation data
protected:
    ETextOptions m_eTextOptions;
    EIconOptions m_eIconOptions;

// Generated message map functions
protected:
    //{{AFX_MSG(COptionsDialog)
    virtual BOOL OnInitDialog();
    afx_msg void OnTextOptions();
    afx_msg void OnIconOptions();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CCustomizeDialog dialog

class CToolBarEx;

class CCustomizeDialog : public CWnd
{
    DECLARE_DYNAMIC( CCustomizeDialog );

// Construction
public:
    CCustomizeDialog( CToolBarEx* pToolBar );

// Operations
public:
    void SetTextOptions( ETextOptions eTextOptions, bool bInDialog );
    void SetIconOptions( EIconOptions eIconOptions, bool bInDialog );

    void AddTextOption( CComboBox& cbTextOptions, ETextOptions eTextOptions, UINT nStringID );
    void AddIconOption( CComboBox& cbIconOptions, EIconOptions eIconOptions, UINT nStringID );

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCustomizeDialog)
    protected:
    virtual void PostNcDestroy();
    //}}AFX_VIRTUAL

// Implementation
protected:
    CSize GetButtonSize() const;

// Implementation data
protected:
    COptionsDialog  m_dlgOptions;
    CToolBarEx*     m_pToolBar;

// Generated message map functions
protected:
    //{{AFX_MSG(CCustomizeDialog)
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG
    LRESULT OnInitDialog( WPARAM wParam, LPARAM lParam );
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif  // !__CUSTOMIZEDIALOG_H__

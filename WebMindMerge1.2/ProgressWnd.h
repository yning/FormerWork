// ProgressWnd.h : header file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Copyright 1998.
//
// CProgressWnd is a drop-in popup progress window for use in
// programs that a time consuming. Check out the accompanying HTML 
// doc file for details.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is not sold for
// profit without the authors written consent, and providing that this
// notice and the authors name is included. If the source code in 
// this file is used in any commercial application then an email to 
// me would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 


#ifndef _INCLUDE_PROGRESSWND_H
#define _INCLUDE_PROGRESSWND_H

/////////////////////////////////////////////////////////////////////////////
// CProgressWnd window

#include "ProgressSnakeCtrl.h"

class CProgressWnd : public CWnd
{
// Construction/Destruction
public:
    CProgressWnd();
    CProgressWnd(CWnd* pParent, LPCTSTR pszTitle, BOOL bSnake = FALSE);
    virtual ~CProgressWnd();

    BOOL Create(CWnd* pParent, LPCTSTR pszTitle, BOOL bSnake = FALSE);
    BOOL GoModal(LPCTSTR pszTitle =_T("Progress"), BOOL bSnake = FALSE);

protected:
    void CommonConstruct();

// Operations
public:
    void SetRange(int nLower, int nUpper, int nStep = 1);
                                                    // Set range and step size
    int StepIt(BOOL bEnsureVisible = TRUE);                                   //    "
    int SetStep(int nStep);                         //    "
    int SetPos(int nPos, BOOL bEnsureVisible = TRUE);                           //    "

    void SetText(LPCTSTR strInfo);                 // Set text in text area

    void Clear();                                   // Clear text, reset bar
    void Hide();                                    // Hide window
    void Show();                                    // Show window

    BOOL Cancelled() { return m_bCancelled; }       // Was "Cancel" hit?

	void SetWindowSize(int nNumTextLines, int nWindowWidth = 390);

    void PeekAndPump(BOOL bCancelOnESCkey = TRUE);  // Message pumping for modal operation   

	BOOL IsFocused();
	void ResetProgress();
    
// Implementation
protected:
    void GetPreviousSettings();
    void SaveCurrentSettings();

protected:
    CWnd * m_wRenenableWnd;

    BOOL m_bCancelled;
    BOOL m_bModal;
    BOOL m_bPersistantPosition;
    int  m_nNumTextLines;

    CStatic       m_Text;
    CButton       m_CancelButton;
    CString       m_strTitle,
                  m_strCancelLabel;
    CFont         m_font;

	CProgressSnakeCtrl m_wndProgress;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CProgressWnd)
	public:
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
    //{{AFX_MSG(CProgressWnd)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
    afx_msg void OnCancel();
    DECLARE_MESSAGE_MAP()
};


#endif
/////////////////////////////////////////////////////////////////////////////


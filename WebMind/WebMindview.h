// WebMindView.h : interface of the CWebMindView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WebMindVIEW_H__19E497C7_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)
#define AFX_WebMindVIEW_H__19E497C7_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FixedHtmlView.h"
#include "AddressBarEx.h"
#include "PageRepository.h"//added by kosko

//********************
//by ny
#include "List.h"
#include "ViewOnLine.h"
#include "ProgressWnd.h"
#include "GlobalSetting.h"
extern CGlobalSetting theSettings;
//extern CString strWebICVersion;
//********************

//##############################################################

class CWebMindView : public CFixedHtmlView
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CWebMindView)

// Attributes
public:
	BOOL MoveDirection(CPoint& point, char* Direction);

	int  MouseAction(int kind);
	int  OpenLinkUnderMouse(int nType=0);

	void SetSpecialProperty(DWORD dwNewProperty);
	void SetDefaultProperty();
	void SetFullProperty();
	void PowerRefresh();
	void VScrollPageUp();
	void VScrollPageDown();
	void WBScrollBy(long xpos,long ypos);
	void WBScrollHPage(int bLeft);
	void ExecCommand(DWORD nCmdID);

	CString GetPointUrl(POINT pt);
	CString GetSortSaveFileName(CString &ext);
	//by ny
	void GetLocationURL(CString& strLocURL);
	CString GetLocationURL();
	CWebMindView();
    void GetHTMLText(CString &str);
// Operations
public:
//by ny
    BOOL GetbeInUse();
    void SetbeInUse(BOOL beUsed);
	List *getHistory()	{	return m_history;	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebMindView)
	public:
	virtual void OnInitialUpdate(int nOpenType, LPCSTR strUrl); // called first time after construct
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, BOOL baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnTitleChange(LPCTSTR lpszText);
	virtual void OnDocumentComplete(LPCTSTR lpszUrl);
	virtual void OnProgressChange(long nProgress, long nProgressMax);
	virtual void OnStatusTextChange(LPCTSTR lpszText);
	virtual BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar);
	virtual void OnCommandStateChange(long nCommand, BOOL bEnable);
	virtual void OnNavigateComplete2(LPCTSTR lpszURL);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bExternalRunOK;
	BOOL m_bHighLighted;
	BOOL m_bFindPrev;
	BOOL m_bAutoPopWin;
	BOOL m_bIsCapture;
	BOOL m_bIsSecure;
	BOOL m_bReUsed;
	BOOL m_bFirst;
	BOOL m_bForceRefresh;
	BOOL m_bDelayAdd;
	BOOL m_bIsNewPopup;
	BOOL m_bEnWebProxy;
	BOOL m_bIsPopUp;
	BOOL m_bIsActive;

	int  m_nAutoRunCmd;
	int	 m_nInNewWindow;//force open in new window
	int	 m_nFindIndex;
	int  m_nFindMax;
	int  m_nFilterDup;
	int  m_nDupID;
	int  m_nOldIcon;
	int  m_nOlderIcon;//old icon before lock and protect

	//////////////////////////////////////////////////////////////////////////
	int m_action;//how the user reach the page	
	CString m_currentURL;// current URL that displayed in the view
	CString m_currentTitle;
	CPageRepository *m_repository;//added by kosko
	List *m_history;
	//////////////////////////////////////////////////////////////////////////
	//**************************
	//by ny
	bool m_bChanged;
	BOOL m_beInUse;
    CViewOnLine *m_pLatestPageView;
	//CArray<CHistoryObj*, CHistoryObj*>	m_arHistory;
    //int	m_iHistoryOffset;
	BOOL m_bNavigateForward;
	BOOL m_bNavigateBack;
	// the location of last time's relevant word popup
	// we will only count the new pages from the last location
	// to current page
	int m_iLastOffSet;
	CProgressWnd m_wndProgress;
	UINT m_nTimer;
	//**************************

	UINT m_nFontSize;

	DWORD m_bSpecialProperty;
	DWORD m_dwProperty,m_dwPropertyDefault;

	CString m_strOpenNewUrl;//last open in new window url
	CString m_strHighLightKey;
	CString	m_strFindKey;
	CString m_lpszUrl;
	CString m_strPossibleURL;
	CString m_strWebProxy;

	void ChangeProperty();
	void ToNavigate(LPCTSTR URL, DWORD dwFlags = 0, LPCTSTR lpszTargetFrameName = NULL,  LPCTSTR lpszHeaders = NULL, LPVOID lpvPostData = NULL,  DWORD dwPostDataLen = 0, BOOL bOnlyRefresh = FALSE);

	virtual ~CWebMindView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	afx_msg void OnEditCopy();

// Generated message map functions
protected:
	afx_msg void OnEditCut();
	afx_msg void OnEditFind();
	afx_msg void OnEditPaste();
	afx_msg void OnEditSelectAll();
	afx_msg void OnOptionsLoadimage();
	afx_msg void OnUpdateOptions(CCmdUI* pCmdUI);
	afx_msg void OnOptionsAllowscripts();
	afx_msg void OnOptionsLoadsounds();
	afx_msg void OnOptionsLoadvideos();
	afx_msg void OnOptionsUseWebp();
	afx_msg void OnUpdateOptionsUseWebp(CCmdUI* pCmdUI);
	afx_msg void OnOptionsAllowactivex();
	afx_msg void OnOptionsAllowjavaapplet();
	afx_msg void OnUpdateGoBack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGoForward(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateViewFonts(CCmdUI* pCmdUI);
	//{{AFX_MSG(CWebMindView)
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnFileSavehtml();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFilePrintSetup();
	afx_msg void OnFileProperty();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFileAutoSave();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnViewPagesizeAdd10();
	afx_msg void OnViewPagesize100();
	afx_msg void OnViewPagesize200();
	afx_msg void OnViewPagesize50();
	afx_msg void OnViewPagesizeDec10();
	afx_msg void OnFileImportExport();
	afx_msg void OnFileSaveAs();
	afx_msg void OnToolAnnotate();
	afx_msg void OnToolSuggest();
	afx_msg void OnDialogSuggest();
	afx_msg void OnFileClose();
	//}}AFX_MSG
	afx_msg void OnGoBack();
	afx_msg void OnGoForward();

	DECLARE_MESSAGE_MAP()
private:
	void PushMouseGesture(char cDirection);
	POINT m_ptLClick, m_ptRClick;
	POINT m_ptPrev;
};

#ifndef _DEBUG  // debug version in WebMindView.cpp
/*inline CWebMindDoc* CWebMindView::GetDocument()
   { return (CWebMindDoc*)m_pDocument; }*/
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WebMindVIEW_H__19E497C7_4ECF_11D3_9B1D_0000E85300AE__INCLUDED_)

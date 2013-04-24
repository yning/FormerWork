// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-2001 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

#include <exdispid.h>
#include <mshtmcid.h>	// CMDSETID_Forms3 definition
#include <mshtmhst.h>	// IDM_menu item definitions
#include <mshtml.h>

//JMLF: #ifdef AFX_HTML_SEG
//JMLF: #pragma code_seg(AFX_HTML_SEG)
//JMLF: #endif
#include "editview.h"	// MFC HTML view support

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlEditCtrl
CHtmlEditCtrl::CHtmlEditCtrl()
{
}

CHtmlEditCtrl::~CHtmlEditCtrl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CHtmlEditCtrl message handlers
BEGIN_EVENTSINK_MAP(CHtmlEditCtrl, CWnd)
	ON_EVENT_REFLECT(CHtmlEditCtrl, 252 /* NavigateComplete2 */, _OnNavigateComplete2, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

void CHtmlEditCtrl::_OnNavigateComplete2(LPDISPATCH , VARIANT FAR* )
{
	SetDesignMode(TRUE);
}


BOOL CHtmlEditCtrl::GetDHtmlDocument(IHTMLDocument2 **ppDocument) const
{
	CHtmlEditCtrl *pThis = const_cast<CHtmlEditCtrl*>(this);
	CComQIPtr<IWebBrowser2> pBrowserApp = pThis->GetControlUnknown();
	ASSERT(pBrowserApp != NULL);
	if (pBrowserApp)
	{
		CComPtr<IDispatch> spDispDocument;
		pBrowserApp->get_Document(&spDispDocument);
		if (spDispDocument)
		{
			return S_OK == spDispDocument->QueryInterface(ppDocument) ? TRUE : FALSE;
		}
	}
	return FALSE;
}

BOOL CHtmlEditCtrl::Create(LPCTSTR lpszWindowName, DWORD /*dwStyle*/, const RECT& rect, CWnd* pParentWnd,
						   int nID, CCreateContext *) 
{
	BOOL bRet = FALSE;
	// create the control window

	AfxEnableControlContainer();
	if (CreateControl(CLSID_WebBrowser, lpszWindowName,
				WS_VISIBLE | WS_CHILD, rect, pParentWnd, nID))
	{
		// in order to put the webbrowser in design mode, you must
		// first load a document into it. "about:blank" seems to
		// be the safest thing to load for a good starting point.
		CComQIPtr<IWebBrowser2> pBrowserApp = GetControlUnknown();
		ASSERT(pBrowserApp);
		if (pBrowserApp)
		{
			CComVariant vEmpty;
			LPCTSTR szDoc = GetStartDocument();
			if (szDoc)
			{
				CComBSTR bstrStart(szDoc);
				if (S_OK == pBrowserApp->Navigate(bstrStart, 
													&vEmpty,
													&vEmpty,
													&vEmpty,
													&vEmpty))
				{
					bRet = TRUE;
				}
			}
			else
				bRet = TRUE;

		}
	}

	if (!bRet)
		DestroyWindow();
	return bRet;
}

LPCTSTR CHtmlEditCtrl::GetStartDocument()
{
	LPCTSTR szDefaultDoc = _T("about:blank");
	return szDefaultDoc;
}

/////////////////////////////////////////////////////////////////////////////
// CHtmlEditView
BEGIN_MESSAGE_MAP(CHtmlEditView, CFormView)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// CHtmlEditView

void CHtmlEditView::OnPaint()
{
	Default();
}

CHtmlEditView::CHtmlEditView()
{
	m_bDesignMode = FALSE;
}

CHtmlEditView::~CHtmlEditView()
{
}

void CHtmlEditView::EnableDesignMode(BOOL bDesignMode)
{
	m_bDesignMode = bDesignMode;
//	SetDesignMode(m_bDesignMode);
}


LPCTSTR CHtmlEditView::GetStartDocument()
{
	LPCTSTR szDefaultUrl = _T("about:blank");
	return szDefaultUrl;
}

BOOL CHtmlEditView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
	DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
	CCreateContext* pContext) 
{
	// create the view window itself
	if (!CHtmlView::Create(lpszClassName, lpszWindowName,
				dwStyle, rect, pParentWnd,	nID, pContext))
	{
		return FALSE;
	}
	LPCTSTR szDoc = GetStartDocument();
	if (szDoc)
		Navigate(szDoc);
	return TRUE;
}

BOOL CHtmlEditView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// if it's not something we're intersted in, let it go to the base
	if (nCode < (int)CN_UPDATE_COMMAND_UI)
		return CHtmlView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	// check for command availability
	BOOL bHasExecFunc = FALSE;
	UINT uiElemType = AFX_UI_ELEMTYPE_NORMAL;
	UINT dhtmlCmdID = GetDHtmlCommandMapping(nID, bHasExecFunc, uiElemType);
	if (dhtmlCmdID == AFX_INVALID_DHTML_CMD_ID)
	{
		// No mapping for this command. Use normal routing
		return CHtmlView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}

	long nStatus = QueryStatus(dhtmlCmdID);

	if (nCode == CN_UPDATE_COMMAND_UI)
	{
		// just checking status
		CCmdUI *pUI = static_cast<CCmdUI*>(pExtra);
		if (pUI)
		{
			if(!(nStatus & OLECMDF_LATCHED || nStatus & OLECMDF_ENABLED))
			{
				pUI->Enable(FALSE);
				if (uiElemType & AFX_UI_ELEMTYPE_CHECBOX)
				{
					if (nStatus & OLECMDF_LATCHED)
						pUI->SetCheck(TRUE);
					else
						pUI->SetCheck(FALSE);
				}
				else if (uiElemType & AFX_UI_ELEMTYPE_RADIO)
				{
					if (nStatus & OLECMDF_LATCHED)
						pUI->SetRadio(TRUE);
					else
						pUI->SetRadio(FALSE);
				}

			}
			else
			{
				pUI->Enable(TRUE); // enable
				// check to see if we need to do any other state
				// stuff
				if (uiElemType & AFX_UI_ELEMTYPE_CHECBOX)
				{
					if (nStatus & OLECMDF_LATCHED)
						pUI->SetCheck(TRUE);
					else
						pUI->SetCheck(FALSE);
				}
				else if (uiElemType & AFX_UI_ELEMTYPE_RADIO)
				{
					if (nStatus & OLECMDF_LATCHED)
						pUI->SetRadio(TRUE);
					else
						pUI->SetRadio(FALSE);
				}
			}
			return TRUE;
		}
		return FALSE;
	}

	// querystatus for this DHTML command to make sure it is enabled
	if(!(nStatus & OLECMDF_LATCHED || nStatus & OLECMDF_ENABLED))
	{

		// trying to execute a disabled command
		TRACE("Not executing disabled dhtml editing command %d", dhtmlCmdID);
		return TRUE;
	}

	if (bHasExecFunc)
	{
		return ExecHandler(nID);		
	}

	return S_OK == ExecCommand(dhtmlCmdID, OLECMDEXECOPT_DODEFAULT, NULL, NULL) ? TRUE : FALSE;
}

BOOL CHtmlEditView::GetDHtmlDocument(IHTMLDocument2 **ppDocument) const
{
	CComPtr<IDispatch> spDispDocument;
	if (!ppDocument)
		return FALSE;
	*ppDocument = NULL;

	m_pBrowserApp->get_Document(&spDispDocument);
	if (spDispDocument)
	{
		return S_OK == spDispDocument->QueryInterface(ppDocument) ? TRUE : FALSE;
	}

	return FALSE;
}

void CHtmlEditView::OnNavigateComplete2(LPCTSTR )
{
//	SetDesignMode(TRUE);
	SetDesignMode(m_bDesignMode);
}

BOOL CHtmlEditView::OnPreparePrinting(CPrintInfo* )
{
	return TRUE;
}

UINT CHtmlEditView::GetDHtmlCommandMapping(UINT , BOOL& bHasExecFunc, UINT& uiElemType)
{
	uiElemType = AFX_UI_ELEMTYPE_NORMAL;
	bHasExecFunc = FALSE;
	return AFX_INVALID_DHTML_CMD_ID;
}

BOOL CHtmlEditView::ExecHandler(UINT )
{
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CHtmlEditView diagnostics

#ifdef _DEBUG
void CHtmlEditView::AssertValid() const
{
	CFormView::AssertValid();
}

void CHtmlEditView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

CHtmlEditDoc::CHtmlEditDoc()
{
}

CHtmlEditDoc::~CHtmlEditDoc()
{
}

CHtmlEditView* CHtmlEditDoc::GetView() const
{
	CHtmlEditView* pHtmlView = NULL;

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		pHtmlView = DYNAMIC_DOWNCAST(CHtmlEditView, pView);
		if (pHtmlView != NULL)
			break;
	}

	return pHtmlView;
}

void CHtmlEditDoc::DeleteContents()
{
	CHtmlEditView* pView = GetView();
	if (pView)
	{
		pView->NewDocument();
		pView->Navigate(pView->GetStartDocument());
	}
}

BOOL CHtmlEditDoc::IsModified()
{
	CHtmlEditView* pView = GetView();

	if (pView)
	{
		return (pView->GetIsDirty() != S_FALSE);
	}

	return FALSE;
}

BOOL CHtmlEditDoc::OpenURL(LPCTSTR lpszURL)
{
	BOOL bRet = FALSE;
	CHtmlEditView* pView = GetView();
	if (pView != NULL && lpszURL != NULL && *lpszURL != '\0')
	{
		pView->Navigate(lpszURL);
		bRet = TRUE;
	}

	return bRet;
}

BOOL CHtmlEditDoc::OnOpenDocument(LPCTSTR lpszFileName)
{
	BOOL bRet = FALSE;
	CHtmlEditView* pView = GetView();
	if (pView != NULL)
	{
		pView->Navigate(lpszFileName);
		SetTitle(lpszFileName);
		bRet = TRUE;
	}

	return bRet;
}

BOOL CHtmlEditDoc::OnSaveDocument(LPCTSTR lpszFileName)
{
	BOOL bRet = FALSE;
	CHtmlEditView *pView = GetView();
	if (pView != NULL)
	{
		CFile file;
		if (file.Open(lpszFileName, CFile::modeCreate|CFile::modeWrite))
		{
			CArchive ar(&file, CArchive::store);
			CArchiveStream as(&ar);
			CComPtr<IHTMLDocument2> spHTMLDocument;
			CComQIPtr<IPersistStreamInit> spPSI;
			pView->GetDHtmlDocument(&spHTMLDocument);
			if (spHTMLDocument)
			{
				spPSI = spHTMLDocument;
				if (spPSI)
				{
					if (S_OK == spPSI->Save((IStream*)&as, TRUE))
					{
						SetModifiedFlag(FALSE);
						bRet = TRUE;
					}
				}
			}
		}
	}
	return bRet;
}

BOOL CHtmlEditDoc::OnNewDocument()
{
#ifdef _DEBUG
	if(IsModified())
		TRACE("Warning: OnNewDocument replaces an unsaved document.\n");
#endif

	DeleteContents(); // we want CHtmlEditDoc's DeleteContents to be called here
	m_strPathName.Empty();      // no path name yet
	SetModifiedFlag(FALSE);     // make clean

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CHtmlEditDoc diagnostics

#ifdef _DEBUG
void CHtmlEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHtmlEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif _DEBUG

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations expanded out-of-line

#ifndef _AFX_ENABLE_INLINES

// expand inlines for Html functions
#define _AFXHTML_INLINE
//JMLF: #include "afxhtml.inl"

#endif //!_AFX_ENABLE_INLINES

/////////////////////////////////////////////////////////////////////////////
// Pre-startup code

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

//JMLF: IMPLEMENT_DYNCREATE(CHtmlView, CFormView)
IMPLEMENT_DYNCREATE(CHtmlEditView, CHtmlView)
IMPLEMENT_DYNAMIC(CHtmlEditDoc, CDocument)


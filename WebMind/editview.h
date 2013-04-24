// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-2001 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __AFXHTML7_H__
#define __AFXHTML7_H__

#if _MSC_VER > 1000
#pragma once
#endif
// JMLF:

#include <afxhtml.h>
#include <atlbase.h>

#define _ATLTRY try
#define _ATLCATCHALL() catch(...)

#ifndef __AFXDISP_H__
	#include <afxdisp.h>
#endif

#ifndef __exdisp_h__
	#include <exdisp.h>
#endif

#ifndef __mshtmhst_h__
#include <mshtmhst.h>
#endif

#ifndef __mshtml_h__
#include <mshtml.h>
#endif

#ifndef __mshtmcid_h__
#include <mshtmcid.h>
#endif

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, off)
#endif
#ifndef _AFX_FULLTYPEINFO
#pragma component(mintypeinfo, on)
#endif

#ifdef _AFX_PACKING
#pragma pack(push, _AFX_PACKING)
#endif

#include <afxpriv2.h>

/////////////////////////////////////////////////////////////////////////////
// AFXHTML - MFC Visual HTML classes

// Classes declared in this file

//CObject
	//CCmdTarget;
		//CWnd,CHtmlEditCtrlBase<CHtmlEditCtrl>
			class CHtmlEditCtrl;

//CObject
	//CCmdTarget;
		//CWnd
			//CView
				//CScrollView
					//CFormView
						class CHtmlView;

//CObject
	//CCmdTarget;
		//CWnd
			//CView
				//CScrollView
					//CFormView,CHtmlEditCtrlBase<CHtmlEditView>
						class CHtmlEditView;

//CObject
	//CCmdTarget;
		//CDocument
			class CHtmlEditDoc;


#undef AFX_DATA
#define AFX_DATA//JMLF: AFX_OLE_DATA

/////////////////////////////////////////////////////////////////////////////
// JMLF: From <afxpriv2.h>
// CArchiveStream

/*
class CArchiveStream : public IStream
{
public:
	CArchiveStream(CArchive* pArchive);

// Implementation
	CArchive* m_pArchive;

	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	STDMETHOD(QueryInterface)(REFIID, LPVOID*);

	STDMETHOD(Read)(void*, ULONG, ULONG*);
	STDMETHOD(Write)(const void*, ULONG cb, ULONG*);
	STDMETHOD(Seek)(LARGE_INTEGER, DWORD, ULARGE_INTEGER*);
	STDMETHOD(SetSize)(ULARGE_INTEGER);
	STDMETHOD(CopyTo)(LPSTREAM, ULARGE_INTEGER, ULARGE_INTEGER*,
		ULARGE_INTEGER*);
	STDMETHOD(Commit)(DWORD);
	STDMETHOD(Revert)();
	STDMETHOD(LockRegion)(ULARGE_INTEGER, ULARGE_INTEGER,DWORD);
	STDMETHOD(UnlockRegion)(ULARGE_INTEGER, ULARGE_INTEGER, DWORD);
	STDMETHOD(Stat)(STATSTG*, DWORD);
	STDMETHOD(Clone)(LPSTREAM*);
};
*/



/////////////////////////////////////////////////////////////////////////////
// CHtmlEditCtrlBase

template <class T>
class CHtmlEditCtrlBase 
{
public:
	BOOL SetDesignMode(BOOL bMode) const
	{
		const T* pT = static_cast<const T*>(this);
		CComPtr<IHTMLDocument2> spDoc;
		pT->GetDHtmlDocument(&spDoc);
		if (spDoc)
		{
			if (bMode)
				return S_OK == spDoc->put_designMode(L"On") ? TRUE : FALSE;
			else
				return S_OK == spDoc->put_designMode(L"Off") ? TRUE : FALSE;
		}
		return FALSE;
	}

	HRESULT ExecCommand(const GUID *pGuid, long cmdID, long cmdExecOpt, VARIANT* pInVar=NULL, VARIANT* pOutVar=NULL) const
	{
		const T* pT = static_cast<const T*>(this);
		CComPtr<IHTMLDocument2> spDoc;
		HRESULT hr = E_FAIL;
		pT->GetDHtmlDocument(&spDoc);
		if (spDoc)
		{
			CComQIPtr<IOleCommandTarget> spCmdTarg = spDoc;
			if (spCmdTarg)
			{
				hr = spCmdTarg->Exec(pGuid, cmdID, cmdExecOpt, pInVar , pOutVar);
			}
			else
				hr = E_NOINTERFACE;
		}
		return hr;
	}
	HRESULT ExecCommand(long cmdID, long cmdExecOpt, VARIANT* pInVar=NULL, VARIANT* pOutVar=NULL) const
	{
		return ExecCommand(&CGID_MSHTML, cmdID, cmdExecOpt, 
					pInVar , pOutVar );
	}

	long QueryStatus(long cmdID) const
	{
		const T* pT = static_cast<const T*>(this);
		CComPtr<IHTMLDocument2> spDoc;
		pT->GetDHtmlDocument(&spDoc);
		if (spDoc)
		{
			CComQIPtr<IOleCommandTarget> spCmdTarg = spDoc;
			if (spCmdTarg)
			{
				OLECMD ocmd = {cmdID, 0};
				if (S_OK == spCmdTarg->QueryStatus(&CGID_MSHTML, 1, &ocmd, NULL))
					return ocmd.cmdf;
			}
		}
		// if an error occurred, returning 0 means the command is disabled
		return 0;
	}

	HRESULT GetEvent(IHTMLEventObj **ppEventObj) const
	{
		CComPtr<IHTMLWindow2> sphtmlWnd;
		CComPtr<IHTMLDocument2> sphtmlDoc;

		if (!ppEventObj)
			return E_POINTER;

		*ppEventObj = NULL;
		HRESULT hr = E_FAIL;

		if (GetDocument(&sphtmlDoc))
		{
			hr = sphtmlDoc->get_parentWindow(&sphtmlWnd);
			if(hr == S_OK)
			{
				hr = sphtmlWnd->get_event(ppEventObj);
			}
		}
		return hr;
	}

	HRESULT GetEventSrcElement(IHTMLElement **ppSrcElement) const
	{
		CComPtr<IHTMLEventObj> spEventObj;
		if (!ppSrcElement)
			return E_POINTER;

		*ppSrcElement = NULL;
		HRESULT hr = GetEvent(&spEventObj);
		if (hr == S_OK)
		{
			hr = spEventObj->get_srcElement(ppSrcElement);
		}
		return hr;
	}

	HRESULT GetDocument(IHTMLDocument2** ppDoc) const
	{	
		const T* pT = static_cast<const T*>(this);
		return pT->GetDHtmlDocument(ppDoc) ? S_OK : E_NOINTERFACE;
	}

	HRESULT NewDocument() const
	{
		HRESULT hr = E_FAIL;
		const T* pT = static_cast<const T*>(this);
		CComPtr<IHTMLDocument2> spHTMLDocument;
		CComQIPtr<IPersistStreamInit> spPSI;
		//JMLF: CStreamOnCString stream;

		pT->GetDHtmlDocument(&spHTMLDocument);
		if (spHTMLDocument)
		{
			spPSI = spHTMLDocument;
			if (spPSI)
			{
				hr = spPSI->InitNew();
			}
		}
		return hr;
	}

	HRESULT GetDocumentHTML(CString& szHTML) const
	{
		HRESULT hr = E_NOINTERFACE;
		const T* pT = static_cast<const T*>(this);
		CComPtr<IHTMLDocument2> spHTMLDocument;
		CComQIPtr<IPersistStreamInit> spPSI;
		CStreamOnCString stream;

		pT->GetDHtmlDocument(&spHTMLDocument);
		if (spHTMLDocument)
		{
			spPSI = spHTMLDocument;
			if (spPSI)
			{
				hr = spPSI->Save(static_cast<IStream*>(&stream), FALSE);
				if (hr == S_OK)
				{
					if (!stream.CopyData(szHTML))
						hr = E_OUTOFMEMORY;
				}
			}
		}
		return hr;
	}

	HRESULT SetDocumentHTML(LPCTSTR szHTML) const
	{
		HRESULT hr = E_NOINTERFACE;
		const T* pT = static_cast<const T*>(this);
		CStreamOnCString stream(szHTML);
		CComPtr<IHTMLDocument2> spHTMLDocument;
		CComQIPtr<IPersistStreamInit> spPSI;

		pT->GetDHtmlDocument(&spHTMLDocument);
		if (spHTMLDocument)
		{
			spPSI = spHTMLDocument;
			if (spPSI)
				hr = spPSI->Load(static_cast<IStream*>(&stream));
		}
		return hr;
	}

	HRESULT GetIsDirty() const
	{
		HRESULT hr = E_NOINTERFACE;
		const T* pT = static_cast<const T*>(this);
		CComPtr<IHTMLDocument2> spHTMLDocument;
		CComQIPtr<IPersistStreamInit> spPSI;

		pT->GetDHtmlDocument(&spHTMLDocument);
		if (spHTMLDocument)
		{
			spPSI = spHTMLDocument;
			if (spPSI)
				hr = spPSI->IsDirty();
		}
		return hr;
	}

	HRESULT GetDocumentTitle(CString& szTitle) const
	{
		HRESULT hr = E_FAIL;
		const T* pT = static_cast<const T*>(this);
		CComPtr<IHTMLDocument2> spHTMLDocument;
		CComBSTR bstrTitle;
		pT->GetDHtmlDocument(&spHTMLDocument);
		if (!spHTMLDocument)
			hr = E_NOINTERFACE;
		else
		{
			hr = spHTMLDocument->get_title(&bstrTitle);
			if (hr == S_OK)
				szTitle = COLE2CT(bstrTitle);
		}
		return hr;
	}

	HRESULT GetBlockFormatNames(CStringArray &sa) const
	{
		CComVariant vaRet;
		HRESULT hr = E_FAIL;
		long lStatus = QueryStatus(IDM_GETBLOCKFMTS);
		if (lStatus & OLECMDF_ENABLED || lStatus & OLECMDF_LATCHED)
		{
			if (S_OK == ExecCommand(IDM_GETBLOCKFMTS, OLECMDEXECOPT_DODEFAULT, NULL, &vaRet))
			{						 
				if(vaRet.vt & VT_ARRAY)
				{
					USES_CONVERSION;
					SAFEARRAY *psa = vaRet.parray;
					BSTR bstrElem;
					long lBound = 0,uBound = 0;
					if(S_OK == SafeArrayGetLBound(psa,1,&lBound) &&
					   S_OK == SafeArrayGetUBound(psa,1,&uBound) )
					{
						for(long i=lBound; i<uBound; i++)
						{	
							if( (S_OK == SafeArrayGetElement(psa, &i, &bstrElem)) )
								sa.Add(CString(OLE2T(bstrElem)));
						}		
						hr = S_OK;
					}
				}
			}
		}

		if (vaRet.vt == VT_ERROR)
			hr = V_ERROR(&vaRet);

		return hr;
	}

	HRESULT SetForeColor(LPCTSTR szColor) const
	{
		return ExecHelperSetVal(IDM_FORECOLOR, szColor);
	}

	HRESULT SetForeColor(int nColor) const
	{
		return ExecHelperSetVal(IDM_FORECOLOR, nColor, OLECMDF_SUPPORTED|OLECMDF_ENABLED);
	}

	HRESULT GetForeColor(int &nColor)
	{
		return ExecHelperGetVal(IDM_FORECOLOR, nColor, OLECMDF_ENABLED|OLECMDF_SUPPORTED);
	}

	HRESULT GetBackColor(int& nColor) const
	{
		return ExecHelperGetVal(IDM_BACKCOLOR, nColor, OLECMDF_ENABLED|OLECMDF_SUPPORTED);
	}

	HRESULT SetBackColor(LPCTSTR szColor) const
	{
		return ExecHelperSetVal(IDM_BACKCOLOR, szColor, OLECMDF_ENABLED|OLECMDF_SUPPORTED);
	}

	HRESULT SetBackColor(int nColor) const
	{
		return ExecHelperSetVal(IDM_BACKCOLOR, nColor, OLECMDF_ENABLED|OLECMDF_SUPPORTED);
	}

	HRESULT SetDefaultComposeSettings(LPCSTR szFontName=NULL,
									  unsigned short nFontSize=3,
									  COLORREF crFontColor=0xFF000000,
									  COLORREF crFontBgColor=0xFF000000,
									  bool bBold = false,
									  bool bItalic = false,
									  bool bUnderline = false) const
	{

		HRESULT hr = ExecHelperSetVal(IDM_HTMLEDITMODE, true);
		if (FAILED(hr))
			return hr;

		CString strBuffer;
		TCHAR szFontColor[12];
		TCHAR szBgColor[12];
		if(crFontColor == 0xFF000000)
			szFontColor[0] = 0;
		else
			if (0 > _sntprintf(szFontColor, 12, _T("%d.%d.%d"), GetRValue(crFontColor),
					GetGValue(crFontColor), GetBValue(crFontColor)))
					return E_UNEXPECTED;

		if (crFontBgColor == 0xFF000000)
			szBgColor[0] = 0;
		else
			if (0 > _sntprintf(szBgColor, 12, _T("%d.%d.%d"), GetRValue(crFontBgColor),
					GetGValue(crFontBgColor), GetBValue(crFontBgColor)))
					return E_UNEXPECTED;

		_ATLTRY
		{
		strBuffer.Format(_T("%d,%d,%d,%d,%s,%s,%s"),
						bBold ? 1 : 0,
						bItalic ? 1 : 0,
						bUnderline ? 1 : 0,
						nFontSize,
						szFontColor[0] ? szFontColor : _T(""),
						szBgColor[0] ? szBgColor : _T(""),
						szFontName);
		}
		_ATLCATCHALL()
		{
			return E_UNEXPECTED;
		}
		return ExecHelperSetVal(IDM_COMPOSESETTINGS, (LPCTSTR)strBuffer);
	}

	HRESULT GetBlockFormat(CString& strFormat) const
	{
		return ExecHelperGetVal(IDM_BLOCKFMT, strFormat, OLECMDF_SUPPORTED|OLECMDF_ENABLED);
	}

	HRESULT SetBlockFormat(LPCTSTR szFormat) const
	{
		return ExecHelperSetVal(IDM_BLOCKFMT, szFormat);
	}

	HRESULT GetFontFace(CString& strFace) const
	{
		return ExecHelperGetVal(IDM_FONTNAME, strFace, OLECMDF_ENABLED|OLECMDF_SUPPORTED);
	}

	HRESULT SetFontFace(LPCTSTR szFace) const
	{
		return ExecHelperSetVal(IDM_FONTNAME, szFace);
	}

	HRESULT IE50Paste(LPCTSTR szData) const
	{
		return ExecHelperSetVal(IDM_IE50_PASTE, szData);
	}

	HRESULT GetBookMark(CString& strAnchor) const
	{
		return ExecHelperGetVal(IDM_BOOKMARK, strAnchor);
	}

	HRESULT SetBookMark(LPCTSTR szAnchorName) const
	{
		return ExecHelperSetVal(IDM_BOOKMARK, szAnchorName,OLECMDF_SUPPORTED);
	}

	HRESULT SetOverwriteMode(bool bMode) const
	{
		return ExecHelperSetVal(IDM_OVERWRITE, bMode);
	}

	HRESULT Is1DElement(bool& bValue) const
	{
		return ExecHelperGetVal(IDM_1D_ELEMENT, bValue);
	}

	HRESULT Is2DElement(bool& bValue) const
	{
		return ExecHelperGetVal(IDM_2D_ELEMENT, bValue);
	}

	HRESULT GetFontSize(short& nSize) const
	{
		return ExecHelperGetVal(IDM_FONTSIZE, nSize);
	}

	HRESULT SetFontSize(unsigned short size) const
	{
		return ExecHelperSetVal(IDM_FONTSIZE, (short)size);
	}

	HRESULT GetFrameZone(short& nZone) const
	{
		return ExecHelperGetVal(IDM_GETFRAMEZONE, nZone);
	}

	HRESULT SetCSSEditingLevel(short nLevel) const
	{
		return ExecHelperSetVal(IDM_CSSEDITING_LEVEL, nLevel, 0);
	}

	HRESULT HyperLink(LPCTSTR szUrl = NULL) const
	{
		if (szUrl)
			return ExecHelperSetVal(IDM_HYPERLINK, szUrl, OLECMDF_SUPPORTED,
									OLECMDEXECOPT_DONTPROMPTUSER);
		return ExecHelperNN(IDM_HYPERLINK, OLECMDF_SUPPORTED, OLECMDEXECOPT_DODEFAULT);
	}

	HRESULT Image(LPCTSTR szUrl = NULL) const
	{
		if (szUrl)
			return ExecHelperSetVal(IDM_IMAGE, szUrl, OLECMDF_SUPPORTED,
									OLECMDEXECOPT_DONTPROMPTUSER);
		return ExecHelperNN(IDM_IMAGE, OLECMDF_SUPPORTED, OLECMDEXECOPT_DODEFAULT);
	}

	HRESULT OrderList(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_ORDERLIST, szId);
	}

	HRESULT UnorderList(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_UNORDERLIST, szId);
	}

	HRESULT AddToGlyphTable(LPCTSTR szTag,
							LPCTSTR szImgUrl,
							unsigned short nTagType,
							unsigned short nAlignment,
							unsigned short nPosInfo,
							unsigned short nDirection,
							unsigned int nImgWidth,
							unsigned int nImgHeight) const
	{
		// Check Paramters
		if (!szTag ||
			!szImgUrl ||
			nTagType > 2 ||
			nAlignment > 3 ||
			nPosInfo > 3 ||
			nDirection > 4)
			return E_INVALIDARG;

		CString strGlyphEntry;
		_ATLTRY
		{
			strGlyphEntry.Format(_T("%%%%%s^^%%%%%s^^%%%%%d^^%%%%%d^^%%%%%d^^")
								 _T("%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^%%%%%d^^**"),
								 szTag, szImgUrl, nTagType, nAlignment, nPosInfo,
								 nDirection, nImgWidth, nImgHeight, nImgWidth, nImgHeight);
		}
		_ATLCATCHALL()
		{
			return E_UNEXPECTED;
		}
		CComVariant vIn((LPCTSTR)strGlyphEntry);
		return ExecCommand(IDM_ADDTOGLYPHTABLE, OLECMDEXECOPT_DODEFAULT, &vIn);
	}

	HRESULT EmptyGlyphTable() const
	{
		return ExecHelperNN(IDM_EMPTYGLYPHTABLE);
	}

	HRESULT Button(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_BUTTON, szId);
	}

	HRESULT CheckBox(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_CHECKBOX, szId);
	}

	HRESULT DropDownBox(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_DROPDOWNBOX, szId);
	}

	HRESULT HorizontalLine(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_HORIZONTALLINE, szId);
	}

	HRESULT Iframe(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_IFRAME, szId);
	}

	HRESULT InsFieldSet(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_INSFIELDSET, szId);
	}

	HRESULT InsInputButton(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_INSINPUTBUTTON, szId);
	}

	HRESULT InsInputHidden(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_INSINPUTHIDDEN, szId);
	}

	HRESULT InsInputImage(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_INSINPUTIMAGE, szId);
	}

	HRESULT InsInputPassword(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_INSINPUTPASSWORD, szId);
	}

	HRESULT InsInputReset(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_INSINPUTRESET, szId);
	}

	HRESULT InsInputSubmit(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_INSINPUTSUBMIT, szId);
	}

	HRESULT InsInputUpload(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_INSINPUTUPLOAD, szId);
	}

	HRESULT ListBox(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_LISTBOX, szId);
	}

	HRESULT Marquee(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_MARQUEE, szId);
	}

	HRESULT Paragraph(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_PARAGRAPH, szId);
	}

	HRESULT RadioButton(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_RADIOBUTTON, szId);
	}

	HRESULT SaveAs(LPCTSTR szPath = NULL) const
	{
		return ExecHelperSetVal(IDM_SAVEAS, szPath);
	}

	HRESULT TextArea(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_TEXTAREA, szId);
	}

	HRESULT TextBox(LPCTSTR szId = NULL) const
	{
		return ExecHelperSetVal(IDM_TEXTBOX, szId);
	}

	HRESULT GetAbsolutePosition(bool &bCurValue) const
	{
		bCurValue = (QueryStatus(IDM_ABSOLUTE_POSITION) & OLECMDF_LATCHED) != 0;
		return S_OK;
	}

	HRESULT SetAbsolutePosition(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_ABSOLUTE_POSITION, bNewValue );
	}


	HRESULT Set2DPosition(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_2D_POSITION, bNewValue, 0);
	}

	HRESULT SetAtomicSelection(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_ATOMICSELECTION, bNewValue, 0);
	}

	HRESULT SetAutoURLDetectMode(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_AUTOURLDETECT_MODE, bNewValue, 0);
	}

	HRESULT SetDisableEditFocusUI(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_DISABLE_EDITFOCUS_UI, bNewValue, 0);
	}

	HRESULT SetIE5PasteMode(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_IE50_PASTE_MODE, bNewValue, 0);
	}

	HRESULT SetLiveResize(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_LIVERESIZE, bNewValue, 0);
	}

	HRESULT SetMultiSelect(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_MULTIPLESELECTION, bNewValue, 0);
	}

	HRESULT SetOverrideCursor(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_OVERRIDE_CURSOR, bNewValue, 0);
	}

	HRESULT SetRespectVisInDesign(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_RESPECTVISIBILITY_INDESIGN, bNewValue, 0);
	}

	HRESULT GetShowAlignedSiteTags(bool &bCurValue) const
	{
		bCurValue = (QueryStatus(IDM_SHOWALIGNEDSITETAGS) & OLECMDF_LATCHED) != 0;
		return S_OK;
	}

	HRESULT SetShowAlignedSiteTags(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_SHOWALIGNEDSITETAGS, bNewValue);
	}

	HRESULT GetShowAllTags(bool &bCurValue) const
	{
		bCurValue = (QueryStatus(IDM_SHOWALLTAGS) & OLECMDF_LATCHED) != 0;
		return S_OK;
	}

	HRESULT SetShowAllTags(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_SHOWALLTAGS, bNewValue);
	}

	HRESULT GetShowAreaTags(bool &bCurValue) const
	{
		bCurValue = (QueryStatus(IDM_SHOWAREATAGS) & OLECMDF_LATCHED) != 0;
		return S_OK;
	}

	HRESULT SetShowAreaTags(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_SHOWAREATAGS, bNewValue);
	}

	HRESULT GetShowCommentTags(bool &bCurValue) const
	{
		bCurValue = (QueryStatus(IDM_SHOWCOMMENTTAGS) & OLECMDF_LATCHED) != 0;
		return S_OK;
	}

	HRESULT SetShowCommentTags(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_SHOWCOMMENTTAGS, bNewValue);
	}

	HRESULT GetShowMiscTags(bool &bCurValue) const
	{
		bCurValue = (QueryStatus(IDM_SHOWMISCTAGS) & OLECMDF_LATCHED) != 0;
		return S_OK;
	}

	HRESULT SetShowMiscTags(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_SHOWMISCTAGS, bNewValue);
	}

	HRESULT GetShowScriptTags(bool &bCurValue) const
	{
		bCurValue = (QueryStatus(IDM_SHOWSCRIPTTAGS) & OLECMDF_LATCHED) != 0;
		return S_OK;
	}

	HRESULT SetShowScriptTags(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_SHOWSCRIPTTAGS, bNewValue);
	}

	HRESULT GetShowStyleTags(bool &bCurValue) const
	{
		bCurValue = (QueryStatus(IDM_SHOWSTYLETAGS) & OLECMDF_LATCHED) != 0;
		return S_OK;
	}

	HRESULT SetShowStyleTags(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_SHOWSTYLETAGS, bNewValue);
	}

	HRESULT GetShowUnknownTags(bool &bCurValue) const
	{
		bCurValue = (QueryStatus(IDM_SHOWUNKNOWNTAGS) & OLECMDF_LATCHED) != 0;
		return S_OK;
	}

	HRESULT SetShowUnknownTags(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_SHOWUNKNOWNTAGS, bNewValue);
	}

	HRESULT GetShowBRTags(bool &bCurValue) const
	{
		bCurValue = (QueryStatus(IDM_SHOWWBRTAGS) & OLECMDF_LATCHED) != 0;
		return S_OK;
	}

	HRESULT SetShowBRTags(bool bNewValue) const
	{
		return ExecHelperSetVal(IDM_SHOWWBRTAGS, bNewValue);
	}

	HRESULT PrintDocument() const
	{
		return ExecHelperNN(IDM_PRINT);
	}

	HRESULT PrintDocument(LPCTSTR szPrintTemplate) const
	{
		return ExecHelperSetVal(IDM_PRINT, szPrintTemplate);
	}

	HRESULT PrintDocument(bool bShowPrintDialog) const
	{
		return ExecHelperSetVal(IDM_PRINT, (short)(bShowPrintDialog ? 1 : 0));
	}

	HRESULT PrintPreview() const
	{
		return ExecHelperNN(IDM_PRINTPREVIEW);
	}

	HRESULT PrintPreview(LPCTSTR szPrintTemplate) const
	{
		return ExecHelperSetVal(IDM_PRINTPREVIEW, szPrintTemplate);
	}

	HRESULT Bold() const
	{
		return ExecHelperNN(IDM_BOLD);
	}

	HRESULT Copy() const
	{
		return ExecHelperNN(IDM_COPY);
	}

	HRESULT Cut() const
	{
		return ExecHelperNN(IDM_CUT);
	}

	HRESULT Delete() const
	{
		return ExecHelperNN(IDM_DELETE);
	}

	HRESULT Indent() const
	{
		return ExecHelperNN(IDM_INDENT);
	}

	HRESULT Italic() const
	{
		return ExecHelperNN(IDM_ITALIC);
	}

	HRESULT JustifyCenter() const
	{
		return ExecHelperNN(IDM_JUSTIFYCENTER);
	}

	HRESULT JustifyLeft() const
	{
		return ExecHelperNN(IDM_JUSTIFYLEFT);
	}

	HRESULT JustifyRight() const
	{
		return ExecHelperNN(IDM_JUSTIFYRIGHT);
	}

	HRESULT Outdent() const
	{
		return ExecHelperNN(IDM_OUTDENT);
	}

	HRESULT Paste() const
	{
		return ExecHelperNN(IDM_PASTE);
	}

	HRESULT RemoveFormat() const
	{
		return ExecHelperNN(IDM_REMOVEFORMAT);
	}

	HRESULT SelectAll() const
	{
		return ExecHelperNN(IDM_SELECTALL);
	}

	HRESULT Underline() const
	{
		return ExecHelperNN(IDM_UNDERLINE);
	}

	HRESULT Unlink() const
	{
		return ExecHelperNN(IDM_UNLINK);
	}

	HRESULT ClearSelection() const
	{
		return ExecHelperNN(IDM_CLEARSELECTION);
	}

	HRESULT Font() const
	{
		return ExecHelperNN(IDM_FONT);
	}

	HRESULT RefreshDocument() const
	{
		return ExecHelperNN(IDM_REFRESH, OLECMDF_SUPPORTED);
	}

	HRESULT UnBookmark() const
	{
		return ExecHelperNN(IDM_UNBOOKMARK);
	}

// Implementation
	HRESULT ExecHelperNN(UINT nID,
						 long nMinSupportLevel = OLECMDF_SUPPORTED|OLECMDF_ENABLED,
						 long nExecOpt = OLECMDEXECOPT_DODEFAULT) const
	{
		HRESULT hr = E_FAIL;
		long lStatus = QueryStatus(nID);
		if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
			hr = ExecCommand(nID, nExecOpt);
		return hr;
	}

	HRESULT ExecHelperSetVal(UINT nCmdID, 
							 LPCTSTR szID=NULL,
							 long nMinSupportLevel = OLECMDF_SUPPORTED|OLECMDF_ENABLED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT) const
	{
		long lStatus = QueryStatus(nCmdID);
		if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
		{
			if (szID)
			{
				CComVariant vName(szID);
				return ExecCommand(nCmdID, nExecOpt, &vName);
			}
			else
				return ExecCommand(nCmdID, nExecOpt);
		}
		return E_FAIL;
	}

	HRESULT ExecHelperSetVal(UINT nCmdID,
							 bool bValue,
							 long nMinSupportLevel = OLECMDF_SUPPORTED|OLECMDF_ENABLED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT) const
	{
		HRESULT hr = E_FAIL;
		long lStatus = QueryStatus(nCmdID);
		if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
		{
			CComVariant vIn(bValue);
			hr = ExecCommand(nCmdID, nExecOpt, &vIn);
		}
		return hr;
	}

	HRESULT ExecHelperSetVal(UINT nCmdID,
							 short nNewVal,
							 long nMinSupportLevel = OLECMDF_SUPPORTED|OLECMDF_ENABLED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT) const
	{
		HRESULT hr = E_FAIL;
		long lStatus = QueryStatus(nCmdID);
		if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
		{
			CComVariant vIn((short)nNewVal);
			hr = ExecCommand(nCmdID, nExecOpt, &vIn);
		}
		return hr;
	}

	HRESULT ExecHelperSetVal(UINT nCmdID,
							 int nNewVal,
							 long nMinSupportLevel = OLECMDF_SUPPORTED|OLECMDF_ENABLED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT) const
	{
		HRESULT hr = E_FAIL;
		long lStatus = QueryStatus(nCmdID);
		if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
		{
			CComVariant vIn((int)nNewVal);
			hr = ExecCommand(nCmdID, nExecOpt, &vIn);
		}
		return hr;
	}

	HRESULT ExecHelperGetVal(UINT nCmdID,
							 bool &bValue,
							 long nMinSupportLevel = OLECMDF_SUPPORTED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT) const
	{
		HRESULT hr = E_FAIL;
		bValue = false;
		long lStatus = QueryStatus(nCmdID);
		if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
		{
			CComVariant vOut;
			hr = ExecCommand(nCmdID, nExecOpt, NULL, &vOut);
			if (hr == S_OK)
			{
				switch(vOut.vt)
				{
				case VT_BOOL:
					bValue = vOut.boolVal == VARIANT_TRUE ? true : false;
					break;
				case VT_NULL:
				case VT_EMPTY:
					break;
				case VT_ERROR:
					hr = V_ERROR(&vOut);
					break;
				default
					hr = E_UNEXPECTED;
					break;
				}
			}
		}
		return hr;
	}

	HRESULT ExecHelperGetVal(UINT nCmdID,
							 short &nValue,
							 long nMinSupportLevel = OLECMDF_SUPPORTED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT) const
	{
		int nTempVal;
		HRESULT hr = ExecHelperGetVal(nCmdID, nTempVal, nMinSupportLevel, nExecOpt);
		if (hr == S_OK)
			nValue = (short)nTempVal;
		return hr;
	}


	HRESULT ExecHelperGetVal(UINT nCmdID,
							 int &nValue,
							 long nMinSupportLevel = OLECMDF_SUPPORTED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT) const
	{
		HRESULT hr = E_FAIL;
		long lStatus = QueryStatus(nCmdID);
		nValue = 0;
		if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
		{
			CComVariant vOut;
			hr = ExecCommand(nCmdID, nExecOpt, NULL, &vOut);
			if (hr == S_OK)
			{
				switch (vOut.vt)
				{
				case VT_I4:
					nValue = vOut.lVal;
					break;
				case VT_I2:
					nValue = vOut.iVal;
					break;
				case VT_EMPTY:
				case VT_NULL:
					break;
				case VT_ERROR:
					hr = V_ERROR(&vOut);
					break;
				default:
					hr = E_UNEXPECTED;
					break;
				}
			}
		}
		return hr;
	}

	HRESULT ExecHelperGetVal(UINT nCmdID,
							 CString& strValue,
							 long nMinSupportLevel = OLECMDF_SUPPORTED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT) const
	{
		HRESULT hr = E_FAIL;
		CComVariant vaRet;
		strValue.Empty();
		long lStatus = QueryStatus(nCmdID);
		if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
		{
			hr = ExecCommand(nCmdID, nExecOpt, NULL, &vaRet);
			if (hr == S_OK)
			{
				switch(vaRet.vt)
				{
				case VT_BSTR:
					{
					CW2T szVal(vaRet.bstrVal);
					strValue = szVal;
					}
					break;
				case VT_NULL:
				case VT_EMPTY:
					break;
				case VT_ERROR:
					hr = V_ERROR(&vaRet);
					break;
				default:
					hr = E_UNEXPECTED;
					break;
				}
			}
		}
		return hr;
	}
}; //CHtmlEditCtrlBase

/////////////////////////////////////////////////////////////////////////////
// CHtmlEditCtrl
class CHtmlEditCtrl:
	public CWnd,
	public CHtmlEditCtrlBase<CHtmlEditCtrl>
{
public:
	// Construct
	CHtmlEditCtrl();

	// Create a new instance of the control. This will also call the 
	// contained WebBrowser control's Navigate method to load a default
	// document into the WebBrowser control.
	virtual BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, int nID, CCreateContext *pContext=NULL);

	// Retrieves the URL to a default document to load.
	virtual LPCTSTR GetStartDocument();

	// Retrieves the IHTMLDocument2 interface on the contained WebBrowser control's
	// currently loaded document.
	BOOL GetDHtmlDocument(IHTMLDocument2 **ppDocument) const;
protected:
// Implementation
	virtual ~CHtmlEditCtrl();
	DECLARE_EVENTSINK_MAP()
	void _OnNavigateComplete2(LPDISPATCH pDisp, VARIANT FAR* URL);

};


/////////////////////////////////////////////////////////////////////////////
// CHtmlEditView
#define AFX_INVALID_DHTML_CMD_ID 0xFFFFFFFF
#define AFX_UI_ELEMTYPE_NORMAL	0
#define AFX_UI_ELEMTYPE_CHECBOX	1
#define AFX_UI_ELEMTYPE_RADIO	2


#define DECLARE_DHTMLEDITING_CMDMAP(className) \
public: \
	UINT GetDHtmlCommandMapping(UINT nIDWindowsCommand, BOOL&, UINT&); \
	BOOL ExecHandler(UINT nCmdID); \
	typedef void  (className##::*PFN_##className##_CMDHDLR)(UINT); \
	struct className##dhtml_cmd_mapping \
	{ \
		UINT windowsCommand; \
		UINT dhtmlCommand; \
		UINT nElementType; \
		PFN_##className##_CMDHDLR pfn; \
	}; \
	static className##dhtml_cmd_mapping m_commands[];\

#define DHTMLEDITING_CMD_ENTRY(cmdID, dhtmlcmdID)\
{cmdID, dhtmlcmdID, AFX_UI_ELEMTYPE_NORMAL, NULL}, 

#define DHTMLEDITING_CMD_ENTRY_FUNC(cmdID, dhtmlcmdID, member_func_name)\
{cmdID, dhtmlcmdID, AFX_UI_ELEMTYPE_NORMAL, member_func_name},

#define DHTMLEDITING_CMD_ENTRY_TYPE(cmdID, dhtmlcmdID, elemType)\
{cmdID, dhtmlcmdID, elemType, NULL}, 

#define DHTMLEDITING_CMD_ENTRY_FUNC_TYPE(cmdID, dhtmlcmdID, member_func_name, elemType)\
{cmdID, dhtmlcmdID, elemType, member_func_name},

#define BEGIN_DHTMLEDITING_CMDMAP(className) \
BOOL className##::ExecHandler(UINT nWindowsCommand)\
{\
	const className##dhtml_cmd_mapping *pMapping = className##::m_commands;\
	while (pMapping->dhtmlCommand != 0)\
	 {\
	 if (nWindowsCommand == pMapping->windowsCommand)\
	 {\
		(this->*pMapping->pfn)(pMapping->dhtmlCommand);\
		return TRUE;\
	 }\
	 pMapping++;\
	 }\
	return FALSE;\
}\
UINT className##::GetDHtmlCommandMapping(UINT nIDWindowsCommand, BOOL& bHasExecFunc, UINT& uiElemType)\
{ \
	const className##dhtml_cmd_mapping *pMapping = className##::m_commands;\
	 while (pMapping->dhtmlCommand != 0)\
	 {\
	 if (pMapping->windowsCommand == nIDWindowsCommand)\
	 {\
		 bHasExecFunc = pMapping->pfn != NULL ? TRUE : FALSE;\
		 uiElemType = pMapping->nElementType;\
		 return pMapping->dhtmlCommand;\
	 }\
	 pMapping++;\
	 }\
	return AFX_INVALID_DHTML_CMD_ID;\
}\
className##::className##dhtml_cmd_mapping className##::m_commands[] = {

#define END_DHTMLEDITING_CMDMAP()\
		{0, 0, 0, NULL} \
	}; \

class CHtmlEditView :
	public CHtmlView,
	public CHtmlEditCtrlBase<CHtmlEditView>
{
public:
	CHtmlEditView();

	// Retrieve the IHTMLDocument2 interface associated with the
	// document loaded in the contained WebBrowser control.
	BOOL GetDHtmlDocument(IHTMLDocument2 **ppDocument) const;

	// Create a new window object. This function will also call the
	// contained WebBrowser's Navigate method to load a default document.
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL);

	// Retrieve the name of the default document for this view.
	virtual LPCTSTR GetStartDocument();

	void EnableDesignMode(BOOL bDesignMode = TRUE);
	BOOL IsDesignMode()		{	return m_bDesignMode;	}

// Implementation
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual UINT GetDHtmlCommandMapping(UINT nIDWindowsCommand, BOOL& bHasExecFunc, UINT& uiElemType);
	virtual BOOL ExecHandler(UINT nCmdID);
	virtual void OnNavigateComplete2(LPCTSTR strURL);
	void OnPaint();

protected:

	BOOL m_bDesignMode;



#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CHtmlEditView)
	virtual ~CHtmlEditView();
};

class AFX_NOVTABLE CHtmlEditDoc : public CDocument
{
public:
	// Construction
	CHtmlEditDoc();

	// Get the CHtmlEditView associated with this document
	virtual CHtmlEditView* GetView() const;

	// Open a URL into the view associated with this document
	virtual BOOL OpenURL(LPCTSTR lpszURL);

	// See if the associated view's WebBrowser control contains a 
	// document that has been modified by the user.
	virtual BOOL IsModified();

// Implementation
public:
	virtual ~CHtmlEditDoc();
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszFileName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszFileName);
	virtual BOOL OnNewDocument();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	DECLARE_DYNCREATE(CHtmlEditDoc)
};

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations

#ifdef _AFX_PACKING
#pragma pack(pop)
#endif

#ifdef _AFX_ENABLE_INLINES
#define _AFXHTML_INLINE AFX_INLINE
//JMLF: #include <afxhtml.inl>
#endif

#undef AFX_DATA
#define AFX_DATA

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, on)
#endif
#ifndef _AFX_FULLTYPEINFO
#pragma component(mintypeinfo, off)
#endif

#endif // __AFXHTML7_H__


// WebMindDoc.h : interface of the CWebMindDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WebMindDOC_H__03B2D097_1DC5_4E94_B90D_711B87CF83A6__INCLUDED_)
#define AFX_WebMindDOC_H__03B2D097_1DC5_4E94_B90D_711B87CF83A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "editview.h"

class CWebMindDoc : public CHtmlEditDoc
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CWebMindDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebMindDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	CWebMindDoc();
	virtual ~CWebMindDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWebMindDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WebMindDOC_H__03B2D097_1DC5_4E94_B90D_711B87CF83A6__INCLUDED_)

// WebMindDoc.cpp : implementation of the CWebMindDoc class
//

#include "stdafx.h"
//#include "WebMind.h"

#include "WebMindDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebMindDoc

IMPLEMENT_DYNCREATE(CWebMindDoc, CHtmlEditDoc)

BEGIN_MESSAGE_MAP(CWebMindDoc, CHtmlEditDoc)
	//{{AFX_MSG_MAP(CWebMindDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebMindDoc construction/destruction

CWebMindDoc::CWebMindDoc()
{
	// TODO: add one-time construction code here
}

CWebMindDoc::~CWebMindDoc()
{
}

BOOL CWebMindDoc::OnNewDocument()
{
	if (!CHtmlEditDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWebMindDoc serialization

void CWebMindDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWebMindDoc diagnostics

#ifdef _DEBUG
void CWebMindDoc::AssertValid() const
{
	CHtmlEditDoc::AssertValid();
}

void CWebMindDoc::Dump(CDumpContext& dc) const
{
	CHtmlEditDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWebMindDoc commands


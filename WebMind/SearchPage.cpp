///////////////////////////////////////////////////////////////
// SearchPage.cpp: implementation of the CSearchPage class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SearchPage.h"

#include "Utility.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSearchPage::CSearchPage()
{
	m_url.Empty();
	m_keyword.Empty();
	m_followURL.RemoveAll();

	m_queryCnt = 0;

	m_pageno = -1;
}

CSearchPage::~CSearchPage()
{
}

CLilacObject * CSearchPage::clone()
{
	CSearchPage *theclone;
        
	theclone = (CSearchPage *)new CSearchPage();

	theclone->m_url = m_url;
	theclone->m_keyword = m_keyword;

	theclone->m_followURL.RemoveAll();
	theclone->m_followURL.Append(m_followURL);

	theclone->m_queryCnt = m_queryCnt;
	theclone->m_pageno = m_pageno;

	return theclone; 
}

int CSearchPage::compare(CLilacObject *obj)
{
	CSearchPage *hopsobj;
		
	hopsobj = (CSearchPage *)obj;

	if(hopsobj->m_url.IsEmpty() && m_url.IsEmpty())
		return 0;
	else if(!hopsobj->m_url.IsEmpty() && !m_url.IsEmpty())
		return m_url.Compare(hopsobj->m_url);

	return -1;
}

int CSearchPage::getPageno()
{
	return m_pageno;
}

void CSearchPage::dump()
{
	int indx;
	m_LogTrace.WriteLine("*******************************************************************\n");
	m_LogTrace.WriteLine("  URL : %s(%d)\n", m_url, m_queryCnt);

	m_LogTrace.WriteLine("\n  Followed URL\n");
	for(indx = 0; indx < m_followURL.GetSize(); indx++)
		m_LogTrace.WriteLine("  %s\n", m_followURL[indx]);
}

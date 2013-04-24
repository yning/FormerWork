///////////////////////////////////////////////////////////////
// ICSession.cpp: implementation of the CICSession class.
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ICSession.h"
#include "PageView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CICSession::CICSession()
{
	m_session = (List *) new List();
	m_wordVectors = (List *) new List();
	m_coverage = 0.0;
}

CICSession::~CICSession()
{
	clear();
}

CLilacObject * CICSession::clone()
{
	CICSession *theclone;
        
	theclone = (CICSession *)new CICSession();
	theclone->clear();

	theclone->m_coverage = m_coverage;
	theclone->m_session = m_session->clone();
	theclone->m_wordVectors = m_wordVectors->clone();

	return theclone; 
}

bool CICSession::isICSession()
{
	if(m_session)
	{
		CPageView *pView;
		pView = (CPageView *)m_session->get(m_session->size() - 1);
		if(pView->getDest())
			return true;
	}
	return false;
}

// keep the first (that is, leftmost) nCount page views 
// in m_session.
void CICSession::Left(int nCount)
{
	if(!m_session || m_session->size() <= nCount)
		return;

	int cutNumber;
	cutNumber = m_session->size() - nCount;

	// remove the right most ones
	for(int i = 0; i < cutNumber; i++)
		m_session->remove(m_session->size() - 1);
}


// keep the last (that is, right most) nCount page views 
// in m_session.
void CICSession::Right(int nCount)
{
	if(!m_session || m_session->size() <= nCount)
		return;

	int cutNumber;
	cutNumber = m_session->size() - nCount;

	// remove the left most ones
	for(int i = 0; i < cutNumber; i++)
		m_session->remove( 0 );
}

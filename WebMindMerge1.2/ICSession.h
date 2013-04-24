///////////////////////////////////////////////////////////////
// ICSession.h: interface for the CICSession class.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_SESSION_H__0FB02F2C_EE6A_4773_8ACA_89192021529E__INCLUDED_)
#define AFX_SESSION_H__0FB02F2C_EE6A_4773_8ACA_89192021529E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LilacObject.h"
#include "List.h"

class CICSession : public CLilacObject
{
public:
	CICSession();
	virtual ~CICSession();

	void clear()
	{
		m_coverage = 0.0;

		if(m_session)
		{
			delete m_session;
			m_session = NULL;
		}

		if(m_wordVectors)
		{
			delete m_wordVectors;
			m_wordVectors = NULL;
		}
	}

    CLilacObject *clone();

public:
	void Left(int nCount);
	void Right(int nCount);

	bool isICSession();

	void ClearWordVector()
	{
		if(m_wordVectors)
			m_wordVectors->clear();
	}

	int getSessionLength()
	{
		if(!m_session)
			return 0;

		return m_session->size();
	}

	void dump()
	{
		if(m_session)
			m_session->dump();
	}

	// the CPageView involved in the session
	List *m_session;

	// the real word feature vectors
	List *m_wordVectors;

	// how often the IC-words in the previous pages
	double m_coverage;

};

#endif // !defined(AFX_SESSION_H__0FB02F2C_EE6A_4773_8ACA_89192021529E__INCLUDED_)

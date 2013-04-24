///////////////////////////////////////////////////////////////
// SessionIdentification.h: interface for the CSessionIdentification class.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_SESSIONIDENTIFICATION_H__29B12FFF_1A45_4D47_9CEC_609C90F708CC__INCLUDED_)
#define AFX_SESSIONIDENTIFICATION_H__29B12FFF_1A45_4D47_9CEC_609C90F708CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "List.h"
#include "Hash.h"

#include "PageCache.h"
#include "PageView.h"
#include "PageRepository.h"


// to identify the IC-sessions, at first,
// set the repository and page view sequence, 
// then call Exec(),
// get the IC-sessions from m_ICSessions.

class CSessionIdentification  
{
public:
	CSessionIdentification();
	virtual ~CSessionIdentification();

	void Exec();


private:
	bool MinimumBackward(List *m_session, CString strURL);
	bool MinimumBackLink(List *m_session, int index, CString m_urlCurrent);
	bool checkLink(CPageView *pView, CString strURL);
	bool checkFrame(CPageFrame *pFrame, CString m_urlNext, CString &strTarget);
	CPageCache *getPage(int pageno);
	
public:

	// input data for session identification
	CPageRepository *m_pageRepository;
	List *m_pageviews;

	// output of the generated IC-sessions
	List *m_ICSessions;
};

#endif // !defined(AFX_SESSIONIDENTIFICATION_H__29B12FFF_1A45_4D47_9CEC_609C90F708CC__INCLUDED_)

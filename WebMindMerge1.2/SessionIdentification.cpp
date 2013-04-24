///////////////////////////////////////////////////////////////
// SessionIdentification.cpp: implementation of the CSessionIdentification class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SessionIdentification.h"
#include "ICSession.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSessionIdentification::CSessionIdentification()
{
}

CSessionIdentification::~CSessionIdentification()
{
}

void CSessionIdentification::Exec()
{
	List *m_session;
	CPageView *m_viewCurrent, *m_viewPrev;
	bool forward, afterDest;
    int indx;
	CString m_urlCurrent;
	CICSession *pSession;
	bool bAnnotated;

	m_session = (List *)new List();
	forward = true;
	afterDest = false;

	// clear the list for IC-session at first
	m_ICSessions->clear();

	m_viewCurrent = NULL;
    m_viewPrev = NULL;

    // at first, we check whether there is annotated page or not,
	// if there is no annotation, just copy the sesison.
	bAnnotated = false;
    for(indx = 0; indx < m_pageviews->size(); indx++)
    {
        m_viewCurrent = (CPageView *)m_pageviews->get(indx);

		if(m_viewCurrent->getDest())
		{
			bAnnotated = true;
			m_session->clear();
			break;
		}
		else
			m_session->add(m_viewCurrent->clone());
	}

	if(!bAnnotated)
	{
		if( m_session->size() )
		{
			pSession = (CICSession *) new CICSession();
			pSession->m_session = m_session->clone();
			m_ICSessions->add(pSession);
		}
		delete m_session;

		return;
	}

    // go through each page in the sequence, and construct a page view
    // sequence based on the original page sequence.
	m_session->clear();
    for(indx = 0; indx < m_pageviews->size(); indx++)
    {
        m_viewCurrent = (CPageView *)m_pageviews->get(indx);

        // store the current URL
        m_urlCurrent.Empty();
        m_urlCurrent = m_viewCurrent->getURL();

		if(m_viewCurrent->getDest())
		{
			// we got a session, output it
			m_session->add(m_viewCurrent->clone());

            // add the identified IC-session into the session list
			pSession = (CICSession *) new CICSession();
			pSession->m_session = m_session->clone();
			m_ICSessions->add(pSession);

			afterDest = true;
		}
		else if(afterDest)
		{
			if(m_viewCurrent->searchQuery)
			{
                // well this is a search query, it means that the user
                // starts a new session
				m_session->clear();
				m_session->add(m_viewCurrent->clone());

				forward = true;
			}
			else
			{
                if(MinimumBackward(m_session, m_urlCurrent))
                    forward = false;
                else
                {
                    MinimumBackLink(m_session, indx, m_urlCurrent);
                    m_session->add(m_viewCurrent->clone());
                }
			}

			afterDest = false;
		}
		else
		{
            if(forward)
    			m_session->add(m_viewCurrent->clone());
            else
            {
                if(MinimumBackward(m_session, m_urlCurrent))
                    forward = false;
                else
                {
        			m_session->add(m_viewCurrent->clone());
                    forward = true;
                }
            }
		}

        m_viewPrev = m_viewCurrent;
    }

	if(m_session->size() > 0)
	{
		pSession = (CICSession *) new CICSession();
		pSession->m_session = m_session->clone();
		m_ICSessions->add(pSession);
	}

    delete m_session;
}


// locate the URL from the end of session, and if can find it
// remove all other pageviews after it
// true: the page view is in the session
// false: no such page view
bool CSessionIdentification::MinimumBackward(List *m_session, CString strURL)
{
    if(m_session == NULL || m_session->size() <= 0)
        return false;

    CPageView *pCurrent;
    int indx;

    pCurrent = (CPageView *)new CPageView();
    pCurrent->setURL(strURL);
    if((indx = m_session->RindexOf(pCurrent)) >= 0)
    {
        if(indx < m_session->size()-1)
        {
            while(indx < m_session->size()-1)
                m_session->remove(indx+1);
        }

        return true;
    }

    return false;
}

bool CSessionIdentification::MinimumBackLink(List *m_session, int index, CString m_urlCurrent)
{
    CPageView *m_view;

    for(int i = index; i >= 0; i--)
    {
        m_view = (CPageView *)m_session->get(i);
        if(checkLink(m_view, m_urlCurrent))
        {
            while(i < m_session->size()-1)
                m_session->remove(m_session->size() - 1);
            return true;
        }
    }

    return false;
}


// check whether there is a hyperlink poiints to the url
bool CSessionIdentification::checkLink(CPageView *pView, CString strURL)
{
    if(pView == NULL)
        return false;

    CPageFrame *pFrame;
    CString strTarget;

    if(pView->m_framepages != NULL)
    {
        // this is a frameset, we should check each frame page
        for(int iframe = 0; iframe < pView->m_framepages->size(); iframe++)
        {
            pFrame = (CPageFrame *)pView->m_framepages->get(iframe);
            if(checkFrame(pFrame, strURL, strTarget))
                return true;
        }
    }
    else
    {
        // otherwise, we just take it as a frame page
        pFrame = (CPageFrame *)new CPageFrame();
        pFrame->m_base = pView->m_base;
        pFrame->m_src = pView->m_url;
        pFrame->pageno = pView->pageno;

        if(checkFrame(pFrame, strURL, strTarget))
        {
            delete pFrame;
            return true;
        }
    }

    return false;
}


bool CSessionIdentification::checkFrame(CPageFrame *pFrame, CString m_urlNext, CString &strTarget)
{
	CPageCache *m_page;
    bool result = true;

    strTarget.Empty();

	if(!pFrame || pFrame->pageno == -1)
		return false;

	m_page = getPage(pFrame->pageno);
	if(!m_page)
		return false;

	if(!m_page->SearchURLTarget(m_urlNext, strTarget))
	{
		result = false;
	    if(m_urlNext.GetAt(m_urlNext.GetLength()-1) == '/')
		{
		    m_urlNext.SetAt(m_urlNext.GetLength()-1, '\0');
			result = m_page->SearchURLTarget(m_urlNext, strTarget);
		}
	}

    if(result)
    {
		if(strTarget.IsEmpty())
			strTarget = m_page->getBaseTarget();
    }

    return result;
}

CPageCache * CSessionIdentification::getPage(int pageno)
{
	if(pageno < 0)
		return NULL;

	return m_pageRepository->getPageCache(pageno);
}
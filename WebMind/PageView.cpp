///////////////////////////////////////////////////////////////
// PageView.cpp: implementation of the CPageView class.
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PageView.h"

#include "Url.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPageFrame::CPageFrame()
{
	m_name.Empty();
	m_target.Empty();
	m_src.Empty();
	m_base.Empty();
	pageno = -1;
}

CPageFrame::~CPageFrame()
{
}

CLilacObject * CPageFrame::clone()
{
	CPageFrame *theclone;
        
	theclone = (CPageFrame *)new CPageFrame();

	theclone->m_base = m_base;
	theclone->m_name = m_name;
	theclone->m_src = m_src;
	theclone->m_target = m_target;
	theclone->pageno = pageno;

	return theclone; 
}

int CPageFrame::compare(CLilacObject *obj)
{
	CPageFrame *hopsobj;
		
	hopsobj = (CPageFrame *)obj;

	if(hopsobj->m_src.IsEmpty() && m_src.IsEmpty())
		return 0;
	else if(!hopsobj->m_src.IsEmpty() && !m_src.IsEmpty())
		return m_src.Compare(hopsobj->m_src);

	return -1;
}

void CPageFrame::dump()
{
	m_LogTrace.WriteLine("*******************************************************************\n");
	m_LogTrace.WriteLine("  NAME : %s\n", m_name);
	m_LogTrace.WriteLine("  BASE : %s\n", m_base);
	m_LogTrace.WriteLine("TARGET : %s\n", m_target);
	m_LogTrace.WriteLine("%6d : %s\n", pageno, m_src);
}

int CPageFrame::serialize (CArchive& pArchive)
{
	int nStatus = 0, size;

	// Serialize the object ...
	if (pArchive.IsStoring())
	{
		pArchive << m_name.GetLength();
		if(m_name.GetLength() > 0)
			pArchive << m_name;

		pArchive << m_target.GetLength();
		if(m_target.GetLength() > 0)
			pArchive << m_target;

		pArchive << m_src.GetLength();
		if(m_src.GetLength() > 0)
			pArchive << m_src;

		pArchive << m_base.GetLength();
		if(m_base.GetLength() > 0)
			pArchive << m_base;

		pArchive << pageno;

	}
	else
	{
		pArchive >> size;
		if(size > 0)
			pArchive >> m_name;

		pArchive >> size;
		if(size > 0)
			pArchive >> m_target;

		pArchive >> size;
		if(size > 0)
			pArchive >> m_src;

		pArchive >> size;
		if(size > 0)
			pArchive >> m_base;

		pArchive >> pageno;
	}

	return (nStatus);
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPageView::CPageView()
{
	m_framepages = NULL;
	destination = false;
	searchQuery = false;
	m_url.Empty();
	m_base.Empty();
	m_target.Empty();
	pageno = -1;
	m_action = -1;
}

CPageView::~CPageView()
{
	if(m_framepages)
		delete m_framepages;
}

CLilacObject * CPageView::clone()
{
	CPageView *theclone;
        
	theclone = (CPageView *)new CPageView();

	if(m_framepages)
		theclone->m_framepages = m_framepages->clone();
	theclone->m_url = m_url;
	theclone->m_base = m_base;
	theclone->destination = destination;
	theclone->pageno = pageno;
	theclone->searchQuery = searchQuery;

	theclone->m_action = m_action;

	return theclone; 
}

int CPageView::compare(CLilacObject *obj)
{
	CPageView *hopsobj;
		
	hopsobj = (CPageView *)obj;

	if(hopsobj->m_url.IsEmpty() && m_url.IsEmpty())
		return 0;
	else if(!hopsobj->m_url.IsEmpty() && !m_url.IsEmpty())
		return m_url.Compare(hopsobj->m_url);

	return -1;
}

void CPageView::addFrame(CString strName, CString strSrc, CString strTarget)
{
	if(!m_framepages)
		m_framepages = (List *)new List();

	CPageFrame *m_pFrame = (CPageFrame *) new CPageFrame();
	m_pFrame->m_name = strName;
	m_pFrame->m_src = strSrc;
	m_pFrame->m_target = strTarget;

	m_framepages->add(m_pFrame);
}

void CPageView::setURL(const CString& m_value)
{
	m_url = m_value;
}

CString CPageView::getURL()
{
	return m_url;
}

void CPageView::setBaseURL(const CString& m_value)
{
	m_base = m_value;
}

CString CPageView::getBaseURL()
{
	return m_base;
}

void CPageView::setTarget(const CString& m_value)
{
	m_target = m_value;
}

CString CPageView::getTarget()
{
	return m_target;
}

void CPageView::setDest(bool m_value)
{
	destination = m_value;
}

bool CPageView::getDest()
{
	return destination;
}

void CPageView::setPageno(int value)
{
	pageno = value;
}

int CPageView::getPageno()
{
	return pageno;
}


// build the URL by given current url, base hyperlink and relative url
void CPageView::buildURL(const CString &m_currentURL, const CString &m_baseURL, const CString &m_href, CString& result)
{
	CUrl pUrl(m_currentURL, m_baseURL, m_href);

	result = pUrl.getURL();
	return;
}


void CPageView::modifyFrame(CString m_url, CString m_base, CString m_target, int pageno)
{
	int indx;

	if(m_framepages == NULL)
		return;

	CString m_curl;
	CPageFrame *pFrame;

	for(indx = 0; indx < m_framepages->size(); indx++)
	{
		pFrame = (CPageFrame *)m_framepages->get(indx);
		if(m_url.Compare(pFrame->m_src) == 0)
		{
			if(!m_base.IsEmpty())
				pFrame->m_base = m_base;

			if(!m_target.IsEmpty())
				pFrame->m_target = m_target;

			pFrame->pageno = pageno;
			return;
		}
	}
}


// build the complete url for each frame set
void CPageView::buildFrameURL()
{
	if(m_framepages == NULL)
		return;

	CString m_curl;
	CPageFrame *pFrame;
	for(int indx = 0; indx < m_framepages->size(); indx++)
	{
		pFrame = (CPageFrame *)m_framepages->get(indx);
		buildURL(m_url, m_base, pFrame->m_src, m_curl);
		pFrame->m_src = m_curl;
	}
}

void CPageView::copyFrames(CPageView *pView)
{
	if(m_framepages)
		delete m_framepages;
	m_framepages = pView->m_framepages->clone();

	return; 
}

int CPageView::locateTarget(CString strTarget)
{
	CPageFrame *pFrame;
	int result = -1;

	for(int i = 0; i < m_framepages->size(); i++)
	{
		pFrame = (CPageFrame *)m_framepages->get(i);
		if(strTarget.Compare(pFrame->m_name) == 0)
		{
			result = i;
			break;
		}
	}

	return result; 
}



void CPageView::dump()
{
	m_LogTrace.WriteLine("\n\n");
	m_LogTrace.WriteLine("%11d : %s\n", pageno, m_url);
	m_LogTrace.WriteLine("       BASE : %s\n", m_base);
	m_LogTrace.WriteLine("     TARGET : %s\n", m_target);

	m_LogTrace.WriteLine("FRAME PAGES :\n");
	if(m_framepages)
	{
		CPageFrame *pFrame;
		for(int i = 0; i < m_framepages->size(); i++)
		{
			pFrame = (CPageFrame *)m_framepages->get(i);
			pFrame->dump();
		}
	}
}

// Load a page view from disk file
void CPageView::load(const char *m_fold)
{
	CString m_file;
	m_file.Format("%s\\%d.dat", m_fold, pageno);

	CFile m_rFile( m_file, CFile::modeRead);
	CArchive m_rArch( &m_rFile, CArchive::load );
	serialize(m_rArch);
	m_rArch.Close();
	m_rFile.Close();
}


// save the page view info into a disk file
void CPageView::save(const char *m_fold)
{
	CString m_file;
	m_file.Format("%s\\%d.dat", m_fold, pageno);

	CFile m_wFile( m_file, CFile::modeCreate | CFile::modeWrite);
	CArchive m_wArch( &m_wFile, CArchive::store );
	serialize(m_wArch);
	m_wArch.Close();
	m_wFile.Close();
}

int CPageView::serialize (CArchive& pArchive)
{
	int nStatus = 0, i, size, bint;
	CPageFrame *pFrame;

	// Serialize the object ...
	if (pArchive.IsStoring())
	{
		pArchive << m_url.GetLength();
		if(m_url.GetLength() > 0)
			pArchive << m_url;

		pArchive << m_base.GetLength();
		if(m_base.GetLength() > 0)
			pArchive << m_base;

		pArchive << m_target.GetLength();
		if(m_target.GetLength() > 0)
			pArchive << m_target;

		bint = destination?1:0;
		pArchive << bint;

		bint = searchQuery?1:0;
		pArchive << bint;

		pArchive << m_action;

		size = 0;
		if(m_framepages && m_framepages->size() > 0)
		{
			pArchive << m_framepages->size();
			for(i = 0; i < m_framepages->size(); i++)
			{
				pFrame = (CPageFrame *)m_framepages->get(i);
				pFrame->serialize(pArchive);
			}
		}
		else
			pArchive << size;

	}
	else
	{
		pArchive >> size;
		if(size > 0)
			pArchive >> m_url;

		pArchive >> size;
		if(size > 0)
			pArchive >> m_base;

		pArchive >> size;
		if(size > 0)
			pArchive >> m_target;

		pArchive >> bint;
		destination = (bint == 1)?true:false;

		pArchive >> bint;
		searchQuery = (bint == 1)?true:false;

		pArchive >> m_action;

		pArchive >> size;
		if(size > 0)
		{
			m_framepages = (List *) new List();
			for(i = 0; i < size; i++)
			{
				pFrame = (CPageFrame *)new CPageFrame();
				pFrame->serialize(pArchive);

				m_framepages->add(pFrame);
			}
		}
	}

	return (nStatus);
}

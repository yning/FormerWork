///////////////////////////////////////////////////////////////
// Google.cpp: implementation of the CGoogle class.
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Google.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Shlwapi.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGoogle::CGoogle()
{
	m_hQuery = (Hash *)new Hash();

	m_listGoogle.RemoveAll();
	m_listGoogle.Add(_T("www.google.ch"));
	m_listGoogle.Add(_T("www.google.co.jp"));
	m_listGoogle.Add(_T("www.google.com.au"));
	m_listGoogle.Add(_T("www.google.com.hk"));//change by ny;"www.google.com"
	m_listGoogle.Add(_T("www.google.ca"));
	m_listGoogle.Add(_T("www.google.co.uk"));
	m_listGoogle.Add(_T("www.google.co.th"));
	m_listGoogle.Add(_T("www.google.ie"));
	m_listGoogle.Add(_T("www.google.ro"));
	m_listGoogle.Add(_T("www.google.de"));

	m_pRepository = NULL;

}

CGoogle::~CGoogle()
{
	if(m_hQuery)
		delete m_hQuery;
}


// Output : 
//    true : new query
//    flase : query already executed
bool CGoogle::queryGoogle(const CStringArray &stemWords, CStringArray &urlGoogle)
{
	urlGoogle.RemoveAll();
	if(stemWords.GetSize() == 0)
		return false;

	CString strStemQuery;
	CGoogleQuery *pQuery, *pResult;
	CStringArray m_originalWords;
	CString strBuffer;
	int i;
	CTextProcessor *m_textProcessor;

	strStemQuery.Empty();
	strStemQuery = stemWords[0];
	for(i = 1; i < stemWords.GetSize(); i++)
		strStemQuery += _T("+") + stemWords[i];

	// at first, we search within the hash table, to check
	// whether it is already there
	pQuery = (CGoogleQuery *) new CGoogleQuery();
	pQuery->m_szQuery = strStemQuery;

	pResult = (CGoogleQuery *) m_hQuery->lookup(pQuery);
	delete pQuery;

	if( pResult )
	{
		urlGoogle.Append(pResult->m_arURLs);
		return false;
	}

	// convert to original words
	if(!m_pRepository)
		return false;

	m_textProcessor = m_pRepository->m_envTextProcessor;

	m_originalWords.RemoveAll();
	for(i = 0; i < stemWords.GetSize(); i++)
	{
		strBuffer.Empty();
		m_textProcessor->getOriginalWord(stemWords[i], strBuffer);
		m_originalWords.Add(strBuffer);
	}

	querying(m_originalWords, urlGoogle);

	// inser the query and search results into the hash table
	pQuery = (CGoogleQuery *) new CGoogleQuery();
	pQuery->m_szQuery = strStemQuery;
	pQuery->m_arURLs.Append(urlGoogle);
	m_hQuery->insert(pQuery);

	return true;
}




// querying google
void CGoogle::querying(CStringArray &originalWords, CStringArray &urlGoogle)
{
	CString m_url;
	const char *buffer;
	int i, count;
//	URL *pUrl;
	List *pResults;
	CQueryResult *pResult;

	CString m_strQuery;

	static m_location = 0;

	m_strQuery = originalWords[0];
	for(i = 1; i < originalWords.GetSize(); i++)
		m_strQuery += "+" + originalWords[i];

	urlGoogle.RemoveAll();

	m_strQuery += _T("+-filetype:pdf+-filetype:ps+-filetype:doc+-filetype:xls+-filetype:ppt+-filetype:rtf");

	// to easy google, we sleep for half a minute before annoying it
	if(Utility::genRand(20) < 2)
		Sleep(1500);
	Sleep(500);

	if(m_location > 9)
	{
		Save();
		m_location = 0;
	}

//	m_LogTrace.WriteLine(" Querying : %s\n", m_googleList[m_location]);

	m_url.Format("http://%s/search?q=%s&num=10&start=0", m_listGoogle[m_location], m_strQuery);//m_url.Format("http://%s/search?q=%s&num=10&start=0&lr=lang_en", m_listGoogle[m_location], m_strQuery);
	m_location++;

//	m_LogTrace.WriteLine("%s\n", m_url);

	CUrl pUrl(m_url);
	if(stricmp(pUrl.getContentType(), "text/html") != 0)
	{
//		delete pUrl;
		return;
	}

	if((buffer = pUrl.getContent()) != NULL)
		count = pUrl.getContentLength();
	else
	{
//		delete pUrl;
		return;
	}

	CPageCache *pCache;
	pCache = (CPageCache *) new CPageCache();

	pCache->m_envTextProcessor = m_pRepository->m_envTextProcessor;
	pCache->m_envSearchEngine = m_pRepository->m_envSearchEngine;

	pCache->setURL(m_url);
	pCache->LoadByContent(buffer);

	pResults = pCache->getQueryResults();
	if(pResults)
	{
		for(i = 0; i < pResults->size(); i++)
		{
			pResult = (CQueryResult *) pResults->get(i);

//			m_LogTrace.WriteLine(" %d : %s\n", pResult->getSize(), pResult->getURL());

			if(pResult->getSize() > 0 && pResult->getSize() < 80 && !pResult->IsExternalFileFormat())
				urlGoogle.Add(pResult->getURL());
		}
	}

	delete pCache;
//	delete pUrl;
}





void CGoogle::save(const CString &strFile)
{
	CFile m_wFile( strFile, CFile::modeCreate | CFile::modeWrite);
	CArchive m_wArch( &m_wFile, CArchive::store );
	CGoogleQuery *pQuery;

	m_wArch << m_hQuery->size();
	m_hQuery->resetSeek();
	while( (pQuery = (CGoogleQuery *) m_hQuery->getObject()) != NULL)
		pQuery->serialize(m_wArch);

	m_wArch.Flush();
	m_wArch.Close();
	m_wFile.Close();
}

void CGoogle::load(const CString &strFile)
{
	if(!PathFileExists(strFile))
		return;

	CFile m_rFile( strFile, CFile::modeRead);
	CArchive m_rArch( &m_rFile, CArchive::load );
	int size;
	CGoogleQuery *pQuery;

	m_rArch >> size;
	for(int i = 0; i < size; i++)
	{
		pQuery = (CGoogleQuery *) new CGoogleQuery();
		pQuery->serialize(m_rArch);

		m_hQuery->insert(pQuery);
	}

	m_rArch.Close();
	m_rFile.Close();
}

///////////////////////////////////////////////////////////////
// PageRepository.cpp: implementation of the CPageRepository class.
//
//
///////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "PageRepository.h"

#include "IniFile.h"
#include "Markup.h"

#ifdef   _DEBUG 
#define  new   DEBUG_NEW 
#undef   THIS_FILE 
static char THIS_FILE[] = __FILE__; 
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPageRepository::CPageRepository(const char *corpus_file,
								 const char *path_source,
								 const char *path_cache)
{
	srand((unsigned)time(NULL));

	m_cacheRepository = (Hash *) new Hash(100);
	m_indexURL = (Hash *) new Hash(100);

	m_envTextProcessor = (CTextProcessor *) new CTextProcessor(corpus_file);
	m_envSearchEngine = (CSearchEngine *) new CSearchEngine();

	m_bSaveCache = true;
	m_bSaveHTML = true;

	m_pathSource = path_source;
	m_pathCache = path_cache;
}

CPageRepository::~CPageRepository()
{
	delete m_cacheRepository;
	m_cacheRepository = NULL;

	delete m_indexURL;
	m_indexURL = NULL;

	delete m_envTextProcessor;
	m_envTextProcessor = NULL;

	delete m_envSearchEngine;
	m_envSearchEngine = NULL;
}


// genearate a random number with maximum length of 8 digits
long CPageRepository::genID()
{
	long result = 0L;

	for(int i = 0; i < 8; i++)
		result = result * 10 + rand()%10;

	return result;
}


// retrieve the page ID for given URL
long CPageRepository::getPageID(const CString &url)
{
	return 0;
}


// this is the URL code book in XML
bool CPageRepository::loadURLCode(const char *url_code)
{
	CFile file;
	CFileException ex;

	if( !file.Open( url_code, CFile::modeRead, &ex ) )
		return false;

	// clear the url index and page cache repository
	if(m_cacheRepository)
		delete m_cacheRepository;
	m_cacheRepository = (Hash *) new Hash(100);

	if(m_indexURL)
		delete m_indexURL;
	m_indexURL = (Hash *) new Hash(100);

	CMarkup *m_pXML;
	CPageCache *pCache;
	int nFileLen = file.GetLength();
	unsigned char* pBuffer = NULL;
	BOOL bFound;

	CString strPageID;
	CString strTime;
	CString strURL;

	CIndexURL *pURL, *rURL;

	m_pXML = (CMarkup *) new CMarkup();

	// Allocate Buffer for Ansi file data
	pBuffer = new unsigned char[nFileLen + 1];
	nFileLen = file.Read( pBuffer, nFileLen );
	file.Close();

	pBuffer[nFileLen] = '\0';
	
	CString m_csText;
	m_csText = (char*)pBuffer;

	m_pXML->SetDoc( m_csText );
	m_pXML->ResetPos();

	bFound = m_pXML->FindElem("CodeBook");
	while((bFound = m_pXML->FindChildElem("URL")))
	{
		m_pXML->IntoElem();

		// read in the URL information, including :
		//  PageID, lastmodified, and url
		strPageID = m_pXML->GetAttrib("PageID");
		strTime = m_pXML->GetAttrib("lastModified");
		strURL = m_pXML->GetData();

		pCache = (CPageCache *) new CPageCache();
		pCache->setURL(strURL);
		pCache->setLastModified(strTime);
		pCache->setPageno(atoi(strPageID));
		SetPathInfoCache(pCache);

		pCache->m_envTextProcessor = m_envTextProcessor;
		pCache->m_envSearchEngine = m_envSearchEngine;

		//pCache->Load();//uncomment by ny
		m_cacheRepository->insert(pCache);

		// put the URL into URL index
		pURL = (CIndexURL *) new CIndexURL(strURL);
		if((rURL = (CIndexURL *) m_indexURL->lookup(pURL)) != NULL)
		{
			rURL->addID(atoi(strPageID));
			delete pURL;
		}
		else
		{
			pURL->addID(atoi(strPageID));
			m_indexURL->insert(pURL);
		}

		m_pXML->OutOfElem();
	}

	delete m_pXML;
	delete [] pBuffer;

//	file.Close();

	return true;
}


bool CPageRepository::loadOffLineURLCode(const char *url_code)
{
    FILE *fp;
    char buffer[1024];
    char *code, *url;
    int indx;

	CPageCache *pCache;

    if((fp = fopen(url_code, "rt")) == NULL)
        return false;

    // Read in each line, extract all the items within this line
    while(fgets(buffer, 1024, fp) != NULL)
    {
        buffer[strlen(buffer) - 1] = '\0';
        code = buffer;
        url = code;
        while(*url != ' ')
            ++url;
        *url = '\0';
		++url;
        while (*url == ' ')
            ++url;

        indx = atoi(code);

		pCache = (CPageCache *) new CPageCache();
		pCache->setURL(url);
		pCache->setPageno(indx);
		SetPathInfoCache(pCache);

		pCache->m_envTextProcessor = m_envTextProcessor;
		pCache->m_envSearchEngine = m_envSearchEngine;

		m_cacheRepository->insert(pCache);
    }

	fclose(fp);

    return true;
}

// retrieve the CPageCache given the page ID
CPageCache * CPageRepository::getPageCache(long pageID)
{
	CPageCache *pTemp, *pCache = NULL;
	if(pageID < 0)
		return NULL;

	pTemp = (CPageCache *) new CPageCache();
	pTemp->setPageno(pageID);
	SetPathInfoCache(pTemp);

	if((pCache = (CPageCache *)m_cacheRepository->lookup(pTemp)) != NULL)
	{
		if(! pCache->m_bLoaded)
		{
			pCache->Load();
			pCache->m_bLoaded = true;
		}
	}

	delete pTemp;
	return pCache;
}


// retrieve the CPageCache given the page ID
bool CPageRepository::getURL(long pageID, CString& m_url)
{
	CPageCache *pTemp, *pCache = NULL;
	bool result = false;

	pTemp = (CPageCache *) new CPageCache();
	pTemp->setPageno(pageID);
	SetPathInfoCache(pTemp);

	m_url.Empty();
	if((pCache = (CPageCache *)m_cacheRepository->lookup(pTemp)) != NULL)
	{
		m_url = pCache->getURL();
		result = true;
	}

	delete pTemp;
	return result;
}


#include "Url.h"

// insert an URL into the repository
CPageCache * CPageRepository::insertPage(const CString &strURL, bool bCalLayout)
{
	CUrl pURL(strURL);
	CPageCache *m_pPageCache = NULL;
	CHighTime tLastModified;

	if(pURL.getHeader())
	{
		CString m_type;
		CString m_lastModified;
		bool bExisted;

		m_type = pURL.getContentType();
		pURL.getLastModified(tLastModified);

		m_lastModified = tLastModified.Format("%m/%d/%Y %H:%M:%S");

		m_pPageCache = acquirePageCache(strURL, m_lastModified, bExisted);
		if(m_pPageCache && !bExisted)
		{
			m_pPageCache->m_envTextProcessor = m_envTextProcessor;
			m_pPageCache->m_envSearchEngine = m_envSearchEngine;

			// whether we should calculate these extra layout features
			m_pPageCache->m_bCalLayout = bCalLayout;

			CString strContent;
			CFile sourceFile;
			CFileException ex;

			strContent.Empty();
			strContent = pURL.getContent();

			// set the path information of the PageCache
			SetPathInfoCache(m_pPageCache);

			// we write the source HTML into a local file
			if (sourceFile.Open(m_pPageCache->m_sourceFile, 
				CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate, &ex))
			{
				sourceFile.Write(strContent, strContent.GetLength());
				sourceFile.Close();
			}

			// load the html, if it is not parsed yet, parse the html and save the cache
			m_pPageCache->Load();

			// insert into the cache reposiroty
			m_cacheRepository->insert(m_pPageCache);
		}
	}

	return m_pPageCache;
}



// insert a page into the repository,
// the page is off line, either in a html file, 
// or off-line browsing
void CPageRepository::insertPage(long pageid, const CString& url, IDispatch *pDisp)
{
	CPageCache *pCache;
	CString m_sourceFile;
	CString m_cacheFile;

	pCache = (CPageCache *) new CPageCache();

	pCache->m_envTextProcessor = m_envTextProcessor;
	pCache->m_envSearchEngine = m_envSearchEngine;

	pCache->setPageno(pageid);
	SetPathInfoCache(pCache);

	pCache->setURL(url);

	// set the file name for the source and the cache
	m_sourceFile.Format("%s\\%ld.html", m_pathSource, pageid);
	pCache->m_sourceFile = m_sourceFile;

	m_cacheFile.Format("%s\\\\%ld.dat", m_pathCache, pageid);
	pCache->m_cacheFile = m_cacheFile;

	if(pDisp == NULL)
		pCache->Load();
	else
	{
		if(!pCache->LoadCache())
		{
			pCache->Load(pDisp, m_onLine);
			pCache->SaveCache(); 
		}
	}

	m_cacheRepository->insert(pCache);
}


// insert a on-line page,
// here we should cope with the frame page.
CPageCache *CPageRepository::insertPage(IDispatch *m_pDisp, CViewOnLine *m_pView)
{
	if(m_pDisp == NULL)
		return NULL;

	CPageCache *pCache = NULL;
	bool m_bExisted;

	// try to retrieve the page cache from the
	// hash table
	m_bExisted = false;
	pCache = retrieveCache(m_pDisp, m_bExisted);

	// if it is a new page, then we should create the
	// page cache
	if(!m_bExisted && pCache)
	{
		pCache->m_envTextProcessor = m_envTextProcessor;
		pCache->m_envSearchEngine = m_envSearchEngine;

		pCache->Load(m_pDisp);

		if(m_bSaveCache)
			pCache->SaveCache();

		if(m_bSaveHTML)
			saveOnLineHTML(m_pDisp, pCache);

		m_cacheRepository->insert(pCache);
	}

	if(pCache)
	{
		// if it is on line, then we try to identify whether
		// it is a frameset
		processFrames(pCache, m_pDisp);

		if(m_pView)
		{
			long frameID;

			if(m_pView->m_bFrameSet)
			{
				m_pView->m_url = pCache->getURL();
				m_pView->m_pageID = pCache->getPageno();
				m_pView->m_bFrameSet = false;
			}

			for(int i = 0; i < pCache->NumFrameIDs(); i++)
			{
				frameID = pCache->GetFrameID(i);
				m_pView->m_frames.Add(frameID);
			}
		}
	}

	// after parsing the page content, we release its memory
	if(pCache)
	{
		pCache->Release();//这个释放内存的地方之前有异常 comment by ny
	}

	return pCache;
}


// retrieve the IHTMLDocument2 from a IDispatch
IHTMLDocument2 * CPageRepository::GetDispatchDocument(IDispatch *m_pDisp)
{
	IHTMLDocument2 *pDoc = NULL;

	if(m_pDisp == NULL)
		return NULL;

	CComPtr<IDispatch> spDisp = m_pDisp;
	HRESULT hr = spDisp->QueryInterface(IID_IHTMLDocument2,(void**)&pDoc);
	if(FAILED(hr))
	{
		IHTMLWindow2* pFrameWindow=NULL;
		hr = spDisp->QueryInterface(IID_IHTMLWindow2, (void**)&pFrameWindow);
		if(FAILED(hr))
			return NULL;

		hr = pFrameWindow->get_document(&pDoc);

		if(FAILED(hr))
			return NULL;
	}

	return pDoc;
}

void CPageRepository::processFrames(CPageCache *paCache, IDispatch *m_pDisp)
{
//	MSHTML::IHTMLDocument2Ptr pDoc = NULL;
	IHTMLDocument2 *pDoc = NULL;

	if(m_pDisp == NULL || paCache == NULL)
		return;

	pDoc = GetDispatchDocument(m_pDisp);
	if(!pDoc)
		return;

	IHTMLFramesCollection2* pFrames=NULL;
	LONG celem;
	long i;
	VARIANT frameRequested;
	VARIANT frameOut;
	CPageCache *pCache;

	pDoc->get_frames( &pFrames );
	pFrames->get_length( &celem );
	for(i = 0; i < celem; i++)
	{
		frameRequested.vt = VT_I2;
		frameRequested.iVal = (int)i;
		pFrames->item(&frameRequested, &frameOut);
		if(frameOut.vt == VT_DISPATCH)
		{
			// we will get the inner URL for the frame set,
			// if it is not pointing to "about:blank",
			// then we will insert it into the repository

			BSTR url;
			CString strURL;
			IHTMLDocument2 *pIHTMLDocument2;
			HRESULT hr;

			pIHTMLDocument2 = GetDispatchDocument(frameOut.pdispVal);
			if(pIHTMLDocument2)
			{
				hr = pIHTMLDocument2->get_URL(&url);
				if (hr == S_OK)
				{
					strURL = url;
					if(strURL.CompareNoCase("about:blank") != 0)
					{

						pCache = insertPage(frameOut.pdispVal);
						if(pCache)
							paCache->AddFrameID(pCache->getPageno());
					}
				}

				SysFreeString(url);
			}
		}
	}

}



void CPageRepository::saveOnLineHTML(IDispatch *m_pDisp, CPageCache *m_pCache)
{
	if(m_pDisp == NULL || m_pCache == NULL)
		return;

	IPersistFile *lpPersistFile = NULL;
	IHTMLDocument2 *pDoc = NULL;
	HRESULT hr;

	CComPtr<IDispatch> spDisp = m_pDisp;

	hr = spDisp->QueryInterface(IID_IHTMLDocument2,(void**)&pDoc);
	if(FAILED(hr))
	{
		IHTMLWindow2* pFrameWindow=NULL;
		hr = spDisp->QueryInterface(IID_IHTMLWindow2, (void**)&pFrameWindow);
		if(FAILED(hr))
			return;

		hr = pFrameWindow->get_document(&pDoc);
		if(FAILED(hr))
			return;
	}

	hr = pDoc->QueryInterface(IID_IPersistFile,(void**)&lpPersistFile);
	if(FAILED(hr))
		return;

	CString m_file;
	m_file.Format("%s\\%ld.html", m_pathSource, m_pCache->getPageno());

	WORD wsz [MAX_PATH]; 
	MultiByteToWideChar (CP_ACP, 0, m_file, -1, wsz, MAX_PATH);
	lpPersistFile->Save(wsz,0);

	lpPersistFile->Release();
}


CPageCache *CPageRepository::retrieveCache(IDispatch *m_pDisp, bool& bExisted)
{
	if(m_pDisp == NULL)
		return NULL;

	CString strURL;
	CString strLastModified;
	BSTR bstr;

	IHTMLDocument2 *pDoc = NULL;
	CComPtr<IDispatch> spDisp = m_pDisp;

	bExisted = false;

	HRESULT hr = spDisp->QueryInterface(IID_IHTMLDocument2,(void**)&pDoc);
	if(FAILED(hr))
	{
		IHTMLWindow2* pFrameWindow=NULL;
		hr = spDisp->QueryInterface(IID_IHTMLWindow2, (void**)&pFrameWindow);
		if(FAILED(hr))
			return NULL;

		hr = pFrameWindow->get_document(&pDoc);
		if(FAILED(hr))
			return NULL;
	}

	strURL.Empty();
	strLastModified.Empty();

	if(pDoc->get_URL(&bstr) == S_OK)
	{
		strURL = bstr;
		SysFreeString(bstr);
	}

	if(pDoc->get_lastModified(&bstr) == S_OK)
	{
		strLastModified = bstr;
		SysFreeString(bstr);
	}

	if(strURL.IsEmpty())
		return NULL;

	return acquirePageCache(strURL, strLastModified, bExisted);
}

void CPageRepository::dump()
{
	m_cacheRepository->dump();
}

// save the code of URLs into xml file
void CPageRepository::saveCodeBookXML(const char *url_code)
{
	CMarkup m_xmlCodeBook;
	CString strEntry;
	CString strBuffer;
    CTime ct;
	CPageCache *pCache;

	m_xmlCodeBook.AddElem( "CodeBook" );

	m_cacheRepository->resetSeek();
	while((pCache = (CPageCache *)m_cacheRepository->getObject()) != NULL)
	{
		m_xmlCodeBook.AddChildElem( "URL", pCache->getURL() );
		strBuffer.Format("%ld", pCache->getPageno());
		m_xmlCodeBook.AddChildAttrib( "PageID", strBuffer );

		pCache->getLastModified(strBuffer);
		m_xmlCodeBook.AddChildAttrib( "lastModified", strBuffer );

		m_xmlCodeBook.OutOfElem();

	}

	CString csDoc = m_xmlCodeBook.GetDoc();
	CFile file;
	if ( file.Open(url_code, CFile::modeWrite|CFile::modeCreate) )
	{
		file.Write( csDoc, csDoc.GetLength() );
		file.Close();
	}
}


// acquire a page cache by given an URL and its last modeified time
// check whether there exists such an URL, if not, creat a new PageCache
// for it, otherwise, return the pagecache that for this URL
CPageCache * CPageRepository::acquirePageCache(const CString &strURL, const CString &strLastModified, bool &bExisted)
{
	CIndexURL *pURL, *pIndex;
	CPageCache *pCache, *pTemp=NULL;
	CString tmpLastModified;
	int i;
	bool m_bMatch = false;
	long newID;

	bExisted = false;
	if(strURL.IsEmpty())
		return NULL;

	// check whether this url is in the index hash table
	pURL = (CIndexURL *) new CIndexURL(strURL);
	if((pIndex = (CIndexURL *)m_indexURL->lookup(pURL)) != NULL)
	{
		// if yes, then we check each page to see whether they
		// have the same lastmodified date, 
		// if yes, we have found the same page, just return 
		//    its pagecache pointer
		// otherwise, we should add the new id of this page into
		//    the index hash table.

		pCache = (CPageCache *) new CPageCache();
		pTemp = NULL;
		for(i = 0; i < pIndex->getNumID(); i++)
		{
			pCache->setPageno(pIndex->getID(i));
			SetPathInfoCache(pCache);

			tmpLastModified.Empty();
			if((pTemp = (CPageCache *)m_cacheRepository->lookup(pCache)) != NULL)
				pTemp->getLastModified(tmpLastModified);

			if(!tmpLastModified.IsEmpty())
			{
				if(tmpLastModified.Compare(strLastModified) == 0)
				{
					m_bMatch = true;
					break;
				}
			}
		}

		delete pCache;
		delete pURL;
	}
	else
	{
		pIndex = pURL;
		m_indexURL->insert(pIndex);
	}


	// if we cannot find the url with the same last modified time
	// then we should create a new id for this url

	if(m_bMatch)
	{
		pCache = pTemp;
		bExisted = true;
	}
	else
	{
		pCache = (CPageCache *) new CPageCache();

		do
		{
			newID = genID();
			pCache->setPageno(newID);
			SetPathInfoCache(pCache);
		}while ((pTemp = (CPageCache *)m_cacheRepository->lookup(pCache)) != NULL);

		pCache->setURL(strURL);
		pCache->setLastModified(strLastModified);

		pIndex->addID(newID);

		bExisted = false;
	}

	return pCache;
}


// set the path information for the pagecahe using its page ID
void CPageRepository::SetPathInfoCache(CPageCache *pCache)
{
	if(!pCache)
		return;

	CString m_sourceFile;
	CString m_cacheFile;

	// set the file name for the source and the cache
	m_sourceFile.Format("%s\\%ld.html", m_pathSource, pCache->getPageno());
	pCache->m_sourceFile = m_sourceFile;

	m_cacheFile.Format("%s\\\\%ld.dat", m_pathCache, pCache->getPageno());
	pCache->m_cacheFile = m_cacheFile;

}



void CPageRepository::loadWordMapping(const CString &map_file)
{
	if(!map_file.IsEmpty() && m_envTextProcessor->hasWordMappingRecorded())
	{
		if(PathFileExists(map_file))
		{
			CWordMapping *pMapping;

			if( (pMapping = m_envTextProcessor->getWordMapping()) != NULL)
				pMapping->load(map_file);
		}
	}
}

void CPageRepository::saveWordMapping(const CString &map_file)
{
	if(!map_file.IsEmpty() && m_envTextProcessor->hasWordMappingRecorded())
	{
		CWordMapping *pMapping;

		if( (pMapping = m_envTextProcessor->getWordMapping()) != NULL)
			pMapping->save(map_file);
	}
}



// release the memory of the pages that are not in the exclude list
void CPageRepository::releaseAllPageCaches()
{
	CPageCache *pCache;

	m_cacheRepository->resetSeek();
	while((pCache = (CPageCache *)m_cacheRepository->getObject()) != NULL)
		pCache->Release();
}


// release the memory of the pages that are not in the exclude list
void CPageRepository::releasePages(const CArray<long, long&> &pExcludeList)
{
	if(pExcludeList.GetSize() <= 0)
		return;

	CPageCache *pCache;
	bool bExcluded;

	m_cacheRepository->resetSeek();
	while((pCache = (CPageCache *)m_cacheRepository->getObject()) != NULL)
	{
		bExcluded = false;
		for(int i = 0; i < pExcludeList.GetSize(); i++)
		{
			if(pCache->getPageno() == pExcludeList[i])
			{
				bExcluded = true;
				break;
			}
		}

		if(!bExcluded)
			pCache->Release();
	}
}




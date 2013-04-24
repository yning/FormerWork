///////////////////////////////////////////////////////////////
// PageRepository.h: interface for the CPageRepository class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_PAGEREPOSITORY_H__640CAA65_4C7E_4A6D_BCA4_8AA29B5C25C5__INCLUDED_)
#define AFX_PAGEREPOSITORY_H__640CAA65_4C7E_4A6D_BCA4_8AA29B5C25C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "Hash.h"
#include "LilacObject.h"
//#include "LilacString.h"

#include "PageCache.h"
#include "PageDocument.h"
#include "ViewOnLine.h"

#include "Corpus.h"
#include "TextProcessor.h"
#include "SearchEngine.h"

#include <comdef.h>
#include <atlbase.h>
#include <mshtml.h>
#pragma warning(disable : 4146)	//see Q231931 for explaintation
#import <mshtml.tlb> no_auto_exclude


class CIndexURL : public CLilacObject
{

public:

	CIndexURL()
	{
		m_url = (CLilacString *) new CLilacString();
		m_ids.RemoveAll();
	}

	CIndexURL(const char *url)
	{
		m_url = (CLilacString *) new CLilacString(url);
		m_ids.RemoveAll();
	}

	~CIndexURL()
	{
		delete m_url;
		m_ids.RemoveAll();
	}

	void addID(long id)			{	m_ids.Add(id);				}

	bool checkID(long id)
	{
		for(int i = 0; i < m_ids.GetSize(); i++)
			if(id == m_ids[i])
				return true;
		return false;
	}

	int getNumID()				{	return m_ids.GetSize();		}
	long getID(int index)		{	return m_ids[index];		}
	CLilacString *getURL()		{	return m_url;				}

	int key()					{	return m_url->key();		}
	int compare(CLilacObject *obj)
	{
		CIndexURL *pEntry = (CIndexURL *)obj;
		return m_url->compare(pEntry->getURL());
	}

private:
	CLilacString *m_url;
	CArray<long, long&> m_ids;

};


class CPageRepository  
{
public:
	CPageRepository(const char *corpus_file, const char *path_source, const char *path_cache);
	virtual ~CPageRepository();

public:
	void loadWordMapping(const CString &map_file);
	void saveWordMapping(const CString &map_file);

	void saveCodeBookXML(const char *url_code);
	void dump();

	IHTMLDocument2 *GetDispatchDocument(IDispatch *m_pDisp);

	CPageCache *insertPage(const CString &strURL, bool bCalLayout=false);
	void insertPage(long pageid, const CString& url, IDispatch *pDisp = NULL);
	CPageCache *insertPage(IDispatch *m_pDisp, CViewOnLine *m_pView = NULL);

	CPageCache * getPageCache(long pageID);
	bool getURL(long pageID, CString& m_url);
	long getPageID(const CString& url);

	// whether the page repository is for on line usage or off-line
	void setOnLine(bool value = true)			{	m_onLine = value;		}
	bool isOnLine()								{	return m_onLine;		}

	void setSaveCache(bool value = true)		{	m_bSaveCache = value;	}
	bool getSaveCache()							{	return m_bSaveCache;	}

	void setSaveHTML(bool value = true)			{	m_bSaveHTML = value;	}
	bool getSaveHTML()							{	return m_bSaveHTML;		}
	
	void recordWordMapping(bool value = true)		{	m_envTextProcessor->recordWordMapping(value);			}
	bool hasWordMappingRecorded()					{	return m_envTextProcessor->hasWordMappingRecorded();	}
	CTextProcessor *getTextProcessor()				{	return m_envTextProcessor;								}
	Hash *GetPageCache()							{	return m_cacheRepository;								}

	bool loadOffLineURLCode(const char *url_code);
	bool loadURLCode(const char *url_code);


	// for debug purpose
	void dumpWordMapping()
	{
		if(m_envTextProcessor)
			m_envTextProcessor->dumpWordMapping();
	}

	void releaseAllPageCaches();
	void releasePages(const CArray<long, long&> &pExcludeList);


protected:
	void processFrames(CPageCache *paCache, IDispatch *m_pDisp);
	void saveOnLineHTML(IDispatch *m_pDisp, CPageCache *m_pCache);
	CPageCache *retrieveCache(IDispatch *m_pDisp, bool& bExisted);
	long genID();

public:
	void SetPathInfoCache(CPageCache *pCache);
	CPageCache * acquirePageCache(const CString &strURL, const CString &strLastModified, bool &bExisted);

	CTextProcessor *m_envTextProcessor;
	CSearchEngine *m_envSearchEngine;

	CString m_pathSource;
	CString m_pathCache;

private:
	bool m_onLine;

	Hash *m_cacheRepository;
	Hash *m_indexURL;

	// whether we should save the on-line document into
	// html file or PageCache
	bool m_bSaveCache;
	bool m_bSaveHTML;
};

#endif // !defined(AFX_PAGEREPOSITORY_H__640CAA65_4C7E_4A6D_BCA4_8AA29B5C25C5__INCLUDED_)



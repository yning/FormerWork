///////////////////////////////////////////////////////////////
// SearchEngine.h: interface for the CSearchEngine class.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHENGINE_H__40A5D060_0659_4B3F_B36B_61F1FEC5F366__INCLUDED_)
#define AFX_SEARCHENGINE_H__40A5D060_0659_4B3F_B36B_61F1FEC5F366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LilacObject.h"
#include "LilacString.h"
#include "Hash.h"
#include "Utility.h"

#include "TextProcessor.h"
#include "PageDocument.h"
#define NUM_SEARCH_ENGINES 291

class CQueryResult : public CLilacObject
{
public:
	CQueryResult()
	{
		m_url.Empty();
		m_size = -1;
		m_bSpan = false;
		m_title.Empty();
		m_snippet.Empty();
		m_titles.RemoveAll();
		m_snippets.RemoveAll();
	}

	~CQueryResult()
	{
		m_url.Empty();
		m_title.Empty();
		m_snippet.Empty();
		m_titles.RemoveAll();
		m_snippets.RemoveAll();
	}

	CQueryResult(const CString& url, const CStringArray& title, const CStringArray& snippet, bool bSpan=false, int nSize=-1)
	{
		m_url = url;
		m_size = nSize;

		m_bSpan = bSpan;

		m_titles.RemoveAll();
		m_snippets.RemoveAll();

		m_titles.Append(title);
		m_snippets.Append(snippet);
	}

	int key()
	{
		CLilacString buffer(m_url);

		return buffer.key();
	}

	int compare(CLilacObject *obj)
	{
		CQueryResult *pResult = (CQueryResult *)obj;
		return m_url.Compare(pResult->getURL());
	}

	CLilacObject * clone()
	{
		CQueryResult *pResult;
		pResult = (CQueryResult *) new CQueryResult(m_url, m_titles, m_snippets, m_bSpan, m_size);
		pResult->setTitle(m_title);
		pResult->setSnippet(m_snippet);

		return pResult;
	}

	CString& getURL()					{	return m_url;		}
	int getSize()						{	return m_size;		}
	CStringArray& getTitles()			{	return m_titles;	}
	CStringArray& getSnippets()			{	return m_snippets;	}

	void setTitle(const CString& title)		{	m_title = title;	}
	CString& getTitle()						{	return m_title;		}

	void setSnippet(const CString& snippet)	{	m_snippet = snippet;	}
	CString& getSnippet()					{	return m_snippet;		}

	bool IsExternalFileFormat()				{	return m_bSpan;			}

	int serialize(CArchive& pArchive)
	{
		int nStatus = 0, len, i, value;
		CString buffer;

		try
		{
		    // Serialize the object ...
		    if (pArchive.IsStoring())
			{
				pArchive << m_url.GetLength();
				if(m_url.GetLength() > 0)
					pArchive << m_url;

				value = m_bSpan?1:0;
				pArchive << value;

				pArchive << m_titles.GetSize();
				for(i = 0; i < m_titles.GetSize(); i++)
					pArchive << m_titles[i];

				pArchive << m_snippets.GetSize();
				for(i = 0; i < m_snippets.GetSize(); i++)
					pArchive << m_snippets[i];
	        }
		    else
			{
	            // Read in one node's information
		        pArchive >> len;
				if(len > 0)
					pArchive >> m_url;

	            pArchive >> value;
				m_bSpan = (value == 1)?true:false;

	            pArchive >> len;
				for(i = 0; i < len; i++)
				{
					pArchive >> buffer;
					m_titles.Add(buffer);
				}

	            pArchive >> len;
				for(i = 0; i < len; i++)
				{
					pArchive >> buffer;
					m_snippets.Add(buffer);
				}
		    }
		}
		catch (CException* pException)
		{
			// A read/write error occured
			pException->Delete();
			if (pArchive.IsStoring())
				nStatus = 1;
			else
				nStatus = -1;
	    }

	    return (nStatus);
	}

private:
	CString m_url;

	// the size of the page in k
	int m_size;

	// whether the returned URL points to an external file format
	bool m_bSpan;

	CString m_title;
	CString m_snippet;

	CStringArray m_titles;
	CStringArray m_snippets;

};



struct engine_entry
{
	char *m_engine;
	char *m_queryterm;
};


class CSearchEngine  
{
public:
	CSearchEngine();
	virtual ~CSearchEngine();

	void parseURL(const char *m_url, CTextProcessor *m_textProcessor);
	bool isSearchEngine(const char *host);

	// Search Query result processing
	void parseQueryResult(const char *m_url, CPageTagNode *m_pRoot, CTextProcessor *m_processor, List *m_results);

private:
	struct engine_entry *lookup(const char *host);
   // CPageTagNode*  find(CPageTagNode * tagNode);
	void parseSearchQuery(const char *refurl, const char *m_qterm, CString& m_keywords);

	void insertQueryResult(CString url, CString title, CString snippet, bool bSpan=false, int pageSize=-1);

	void ProcessGoogleCom(CPageTagNode *document);
	void ProcessExciteCom(CPageTagNode *document);
	void ProcessWeatherYahoo(CPageTagNode *document);
	void ProcessHotbotLycos(CPageTagNode *document);
	void ProcessSearchYahoo(CPageTagNode *document);
	void ProcessGoogleYahoo(CPageTagNode *document);
	void ProcessSearchMsn(CPageTagNode *document);
	void ProcessAtomzCom(CPageTagNode *document);
	void ProcessAltaVista(CPageTagNode *document);
	void ProcessAskJeeves(CPageTagNode *document);

public:
	void GoogleSearchResult(CPageTagNode *pContent);
	bool m_bSearch;
	CStringArray m_queryKeyWords;

private:
	Hash *m_engines;
	bool m_bInited;

	List *m_queryResults;
	CTextProcessor *m_textProcessor;

	CString m_engineFile;

	bool m_bRelatedQuery;
	bool m_bLinkQuery;
	bool m_bCacheQuery;

};

#endif // !defined(AFX_SEARCHENGINE_H__40A5D060_0659_4B3F_B36B_61F1FEC5F366__INCLUDED_)


///////////////////////////////////////////////////////////////
// PageCache.h: interface for the CPageCache class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_PAGECACHE_H__4D852FCC_4E20_4A27_88B8_2F08CFA1890F__INCLUDED_)
#define AFX_PAGECACHE_H__4D852FCC_4E20_4A27_88B8_2F08CFA1890F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "StdAfx.h"
#include "LilacObject.h"
//#include "LilacString.h"
#include "List.h"
#include "Hash.h"
#include "Utility.h"

#include "TextProcessor.h"
#include "SearchEngine.h"

#include "PageDocument.h"
//#include "PageDocument2.h"

#define FEATURE_TAG 17

#define IN_TITLE 0
#define IN_META  1


class CPageCacheWord : public CLilacObject
{
public:
	CPageCacheWord()
	{
		Reset();
	}

	CPageCacheWord(const CString& word, int freq = 1)
	{
		Reset();

		m_word = word;
		m_frequency = freq;
	}

	void Reset()
	{
		m_frequency = 0;

		for(int i = 0; i < FEATURE_TAG; i++)
			m_featureTagCnt[i] = 0;

		m_weight = 0.0;

	    m_inTitle = false;
		m_inMeta = false;

		m_inHyperlink = 0;

		m_bJewellTFIDF = false;
		m_weightTFIDF = 0.0;
		m_weightJewell = 0.0;

		m_fgColor = -1.0;
		m_bgColor = -1.0;
		m_fontFamily = -1.0;
		m_fontSize = -1.0;
		m_fontStyle = -1.0;
		m_fontWeight = -1.0;
	}

	int * getTagCnt()
	{
		return m_featureTagCnt;
	}

	int getTagCnt(int tagno)
	{
		if(tagno < 0 || tagno >= FEATURE_TAG)
			return -1;

		return m_featureTagCnt[tagno];
	}

	void setTagCnt(int tagno, int value)
	{
		if(tagno < 0 || tagno >= FEATURE_TAG)
			return;
		m_featureTagCnt[tagno] = value;
	}

	void increaseTagCnt(int tagno)
	{
		if(tagno < 0 || tagno >= FEATURE_TAG)
			return;

		m_featureTagCnt[tagno] += 1;
	}

	CString& getWord()					{	return m_word;		}
	void setWord(const CString& word)	{	m_word = word;		}

	double getWeight()					{	return m_weight;	}
	void setWeight(double value)		{	m_weight = value;	}

	double getWeightIDF()				{	return m_weightIDF;	}
	void setWeightIDF(double value)		{	m_weightIDF = value;}

	int getFreq()						{	return m_frequency;	}
	void setFreq(int value)				{	m_frequency = value;}

	bool getInTitleFlag()				{	return m_inTitle;	}
	void setInTitleFlag(bool value)		{	m_inTitle = value;	}

	bool getInMetaFlag()				{	return m_inMeta;	}
	void setInMetaFlag(bool value)		{	m_inMeta = value;	}

	int getInHyperLink()				{	return m_inHyperlink;	}
	void setInHyperLink(int value)		{	m_inHyperlink = value;	}
	void increaseInHyperlink()			{	++m_inHyperlink;		}

	void computeTFIDFJewell();

	double getTFIDFWeight();
	void setTFIDFWeight(double value)
	{
		m_weightTFIDF = value;
	}

	double getJewellWeight();
	void setJewellWeight(double value)
	{
		m_weightJewell = value;
	}

	int key()
	{
		CLilacString buffer(m_word);

		return buffer.key();
	}

	int compare(CLilacObject *obj)
	{
		CPageCacheWord *pWord = (CPageCacheWord *)obj;
		return m_word.Compare(pWord->getWord());
	}

	CLilacObject * clone()
	{
		CPageCacheWord *pWord = (CPageCacheWord *) new CPageCacheWord();
		pWord->setWeight(m_weight);
		pWord->setWord(m_word);
		for(int i = 0; i < FEATURE_TAG; i++)
			pWord->setTagCnt(i, m_featureTagCnt[i]);

		return pWord;
	}
	
	int serialize(CArchive& pArchive)
	{
		int nStatus = 0, i, flag;

		try
		{
		    // Serialize the object ...
		    if (pArchive.IsStoring())
			{
				// Write information of one node
	            pArchive << m_word;
		        pArchive << m_frequency;

	            pArchive << m_weight;

	            for(i = 0; i < FEATURE_TAG; i++)
		            pArchive << m_featureTagCnt[i];

	            pArchive << m_weightIDF;

				flag = m_inTitle?1:0;
				pArchive << flag;

				flag = m_inMeta?1:0;
				pArchive << flag;

				pArchive << m_inHyperlink;
		    }
			else
	        {
		        // Read in one node's information
			    pArchive >> m_word;
				pArchive >> m_frequency;

	            pArchive >> m_weight;

	            for(i = 0; i < FEATURE_TAG; i++)
		            pArchive >> m_featureTagCnt[i];

	            pArchive >> m_weightIDF;

				pArchive >> flag;
				m_inTitle = (flag == 1)?true:false;

				pArchive >> flag;
				m_inMeta = (flag == 1)?true:false;

				pArchive >> m_inHyperlink;
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

	void calculateTFIDF();
	double JewellScore();

	void dump();

private:
	CString m_word;
	int m_frequency;

	double m_weight;

	int m_featureTagCnt[FEATURE_TAG];

	double m_weightIDF;

    bool m_inTitle;
    bool m_inMeta;

    int m_inHyperlink;

	// whether we have compute the TFIDF and Jewell weights
	bool m_bJewellTFIDF;

	// the TF/IDF and Jewell weights
	double m_weightTFIDF;
	double m_weightJewell;

	// extra features about the layout
public:
	double m_fgColor;
	double m_bgColor;

	double m_fontFamily;
	double m_fontSize;
	double m_fontStyle;
	double m_fontWeight;
};


class CPageCacheURL : public CLilacObject
{
public:

	CStringArray& getAnchorWords()
	{
		return m_anchorWords;
	}

	void addAnchorWord(const CString& m_word)
	{
		m_anchorWords.Add(m_word);
	}

	CString& getURL()
	{
		return m_url;
	}

	void setURL(const CString& url)
	{
		m_url = url;
	}

	CString& getTarget()
	{
		return m_target;
	}

	void setTarget(const CString& target)
	{
		m_target = target;
	}

	int key()
	{
		CLilacString buf(m_url);
		return buf.key();
	}

	int compare(CLilacObject *obj)
	{
		CPageCacheURL *pURL = (CPageCacheURL *)obj;
		return m_url.Compare(pURL->getURL());
	}

	CLilacObject *clone()
	{
		CPageCacheURL *pURL = (CPageCacheURL *) new CPageCacheURL();
		pURL->setURL(m_url);
		pURL->setTarget(m_target);
		for(int i = 0; i < m_anchorWords.GetSize(); i++)
			pURL->addAnchorWord(m_anchorWords[i]);

		return pURL;
	}

	void dump()
	{
		m_LogTrace.WriteLine("\n%s\n", m_url);
		for(int i = 0; i < m_anchorWords.GetSize(); i++)
			m_LogTrace.WriteLine(" %s ", m_anchorWords[i]);

		m_LogTrace.WriteLine("\n\n");
	}

	int serialize(CArchive& pArchive)
	{
		int nStatus = 0, i, len;
		CString buf;

		try
		{
		    // Serialize the object ...
		    if (pArchive.IsStoring())
			{
				// Write information of one node
	            pArchive << m_url;

				len = m_target.GetLength();
				pArchive << len;
				if(len > 0)
					pArchive << m_target;

				len = m_anchorWords.GetSize();
				pArchive << len;
				if(len > 0)
				{
					for(i = 0; i < len; i++)
						pArchive << m_anchorWords[i];
				}
		    }
			else
	        {
		        // Read in one node's information
			    pArchive >> m_url;

				pArchive >> len;
				if(len > 0)
					pArchive >> m_target;

				m_anchorWords.RemoveAll();

		        pArchive >> len;
				for(i = 0; i < len; i++)
				{
					pArchive >> buf;
					m_anchorWords.Add(buf);
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
	CString m_target;
	CStringArray m_anchorWords;

};



/*






  This is the cache for the web page after text processing of
  the content, extarcting the words and URLs.

  Usage :
  1. setPageno, setURL -> Load
  it will check whether there exists the local file for this object,
  if so, Load(const char *), otherwise, parse the HTML and get all
  needed information


  2. setPageno -> Load(const char *)
  Just load in all the information of the page.

  3. setURL -> Load(IDispatch *)
  it will process the text from HTMLDocument from IE.
  First of all, it generates the tree structure of the
  web page, and then do the text processing.

*/

class CPageCache : public CLilacObject
{
public:
	CPageCache();
	virtual ~CPageCache();

	void setPageno(long value)			{	m_pageno = value;	}
	long getPageno()					{	return m_pageno;	}

	void setURL(const CString& url)		{	m_url = url;		}

	void Release();

	bool Load();
	void Load(IDispatch *pDisp, bool online = true);
	void LoadByContent(const CString& m_content);

	bool LoadCache(const char *strCacheFile = NULL);
	void SaveCache(const char *strCacheFile = NULL);

	void computeJewellTFIDF();

	int serialize (CArchive& pArchive);

public:
	int key()			{	return m_pageno;		}

	int compare(CLilacObject *obj)
	{
		CPageCache *pEntry = (CPageCache *)obj;
		return (m_pageno == pEntry->getPageno()) ? 0 : 1;
	}

	void dump();


private:
	void parseTree(CPageTagNode *root);

	void calculateTFIDF();

	void InAnchorText(CString strText);
	void inTitleMeta(CString strText, int location);

	void extractAnchorText(CPageTagNode *pNode, CString &strText);

	void tagFactorCalculation(CPageTagNode *root);

	void textProcessBody(CString strText, int& word_num);
	void textProcessTag(CString strText, int tag_no);
	void textProcessURL(const CString& m_newURL, const CString& m_strTarget, const CString& strText);

	void buildURL(CString m_href, CString &result);
	void InsertWord(const char *pword, int count = 1, int tag = -1);

	int loadWordURL(CArchive &pArchive, int wordsize, int urlsize, int keysize, int resultsize);
	void saveWordURL(CArchive &pArchive);

	CPageCacheURL * searchURL(const CString& m_url);

public:
	CString& getURL()				{	return m_url;			}
	CString& getBaseURL()			{	return baseURL;			}
	CString& getBaseTarget()		{	return baseTarget;		}

	CStringArray& getFrames()		{	return aFrames;			}
	CStringArray& getFrameNames()	{	return aNames;			}
	CStringArray& getFrameTarget()	{	return aTargets;		}
	int getNumFrame()				{	return aFrames.GetSize();	}

	Hash *getAllWords()				{	return m_words;			}
	Hash *getOutURLs()				{	return m_outURLs;		}

	bool isSearchQuery()			{	return m_isSearchQuery;	}
	CStringArray& getQueryWords()	{	return m_queryWords;	}
	List *getQueryResults(){	return m_queryResults;	}
	void getLastModified(CString& date)			{	date = m_lastModified;	}
	void setLastModified(const CString& date)	{	m_lastModified = date;	}

public:
	void Load(CPageDocument *pDocument);
	CPageCacheURL * locateURL(const CString& m_url);
	bool SearchURLTarget(const CString& m_url, CString& m_target);

	bool searchWord(const CString &strWord);

	void AddFrameID(long frameID)	{	aFrameIDs.Add(frameID);			}
	int NumFrameIDs()				{	return aFrameIDs.GetSize();		}
	long GetFrameID(int index)		{	return aFrameIDs[index];		}

public:
	
	CTextProcessor *m_envTextProcessor;
	CSearchEngine *m_envSearchEngine;

	bool m_bLoaded;
	bool m_bCalLayout;

public:
	CString m_sourceFile;
	CString m_cacheFile;

private:

	long m_pageno;
	CString m_url;
	CString m_lastModified;

	// for frame set
	CStringArray aFrames;
	CStringArray aNames;
	CStringArray aTargets;
	CArray<long, long&> aFrameIDs;

	// base URL and target
	CString baseURL;
	CString baseTarget;

	Hash *m_words;
	Hash *m_outURLs;

	bool m_isSearchQuery;
	CStringArray m_queryWords;
	List *m_queryResults;

	// whether we have computed the Jewell and TFIDF weight
	// for each word in the page
	bool m_bJewellTFIDF;
};

#endif // !defined(AFX_PAGECACHE_H__4D852FCC_4E20_4A27_88B8_2F08CFA1890F__INCLUDED_)



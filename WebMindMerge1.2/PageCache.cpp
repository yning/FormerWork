///////////////////////////////////////////////////////////////
// PageCache.cpp: implementation of the CPageCache class.
//
//
///////////////////////////////////////////////////////////////
#include <stdio.h>
#include "StdAfx.h"
#include "PageCache.h"
#include <fstream>
#include <iostream>
using namespace std;
//#include <stdlib.h>
//#include "IniFile.h"
#include "Url.h"

#include "Utility.h"
#include "Shlwapi.h"

#ifdef   _DEBUG 
#define  new   DEBUG_NEW 
#undef   THIS_FILE 
static char THIS_FILE[] = __FILE__; 
#endif

struct tag_weight
{
	char *tag;
	double factor;
} TagWeightArray[] = {
	{"a", 20.0},
	{"title", 50.0},
	{"h1", 10.0}, {"h2", 9.0}, {"h3", 8.0}, {"h4", 7.0}, {"h5", 6.0}, {"h6", 5.0},
	{"em", 15.0}, {"i", 15.0}, {"cite", 10.0},
	{"strong", 15.0}, {"b", 15.0},
	{"u", 10.0},
	{"blink", 20.0},
	{"big", 20.0},
	{"s", 5.0}
};


/*

	{"a", 0.1},
	{"title", 0.95},
	{"h1", 0.20}, {"h2", 0.16}, {"h3", 0.1}, {"h4", 0.1}, {"h5", 0.1}, {"h6", 0.1},
	{"em", 0.1}, {"i", 0.56}, {"cite", 0.1},
	{"strong", 0.15}, {"b", 0.15},
	{"u", -0.91},
	{"blink", 0.15},
	{"big", 0.15},
	{"s", 0.15}

*/


/*

	{"a", 50.0},
	{"title", 20.0},
	{"h1", 10.0}, {"h2", 9.0}, {"h3", 8.0}, {"h4", 7.0}, {"h5", 6.0}, {"h6", 5.0},
	{"em", 15.0}, {"i", 15.0}, {"cite", 10.0},
	{"strong", 15.0}, {"b", 15.0},
	{"u", 10.0},
	{"blink", 20.0},
	{"big", 20.0},
	{"s", 5.0}

*/

double JeWellWeight[19] = {0.1, 0.0, 
	0.1, 0.95, 0.20, 0.16, 0.1, 0.1, 0.1, 0.1,
	0.1, 0.56, 0.1,
	0.15, 0.15, -0.91,
	0.15, 0.15, 0.15};


double CPageCacheWord::JewellScore()
{
	double score = 0.0;

	score += JeWellWeight[0] * (double)m_frequency;
	score += JeWellWeight[1] * (double)m_frequency * m_weightIDF;
	for(int i = 0; i < FEATURE_TAG; i++)
		score += JeWellWeight[i + 2] * (double)m_featureTagCnt[i];

	return score;
}


void CPageCacheWord::calculateTFIDF()
{
	double weight, sum;

	weight = (double)m_frequency * m_weightIDF;

	sum = weight;
	for(int j = 0; j < FEATURE_TAG; j++)
		sum += (double)m_featureTagCnt[j]*TagWeightArray[j].factor;

	m_weight = sum;

}


void CPageCacheWord::computeTFIDFJewell()
{
	if(!m_bJewellTFIDF)
	{
		m_weightTFIDF = (double)m_frequency * m_weightIDF;
		m_weightJewell = JewellScore();
		m_bJewellTFIDF = true;
	}
}

double CPageCacheWord::getTFIDFWeight()
{
	if(!m_bJewellTFIDF)
		computeTFIDFJewell();
	return m_weightTFIDF;
}

double CPageCacheWord::getJewellWeight()
{
	if(!m_bJewellTFIDF)
		computeTFIDFJewell();
	return m_weightJewell;
}

/*
	{"a", 20.0},
	{"title", 50.0},
	{"h1", 10.0}, {"h2", 9.0}, {"h3", 8.0}, {"h4", 7.0}, {"h5", 6.0}, {"h6", 5.0},
	{"em", 15.0}, {"i", 15.0}, {"cite", 10.0},
	{"strong", 15.0}, {"b", 15.0},
	{"u", 10.0},
	{"blink", 20.0},
	{"big", 20.0},
	{"s", 5.0}
*/

void CPageCacheWord::dump()
{
	m_LogTrace.WriteLine("%d,", m_featureTagCnt[0]);
	m_LogTrace.WriteLine("%d,", m_featureTagCnt[1]);

	m_LogTrace.WriteLine("%d,", m_featureTagCnt[2]+m_featureTagCnt[3]+m_featureTagCnt[4]+
		m_featureTagCnt[5]+m_featureTagCnt[6]+m_featureTagCnt[7]);

	m_LogTrace.WriteLine("%d\n", 
		m_featureTagCnt[8] + m_featureTagCnt[9] + m_featureTagCnt[10] +
		m_featureTagCnt[11] + m_featureTagCnt[12] +
		m_featureTagCnt[13] + 
		m_featureTagCnt[14] + 
		m_featureTagCnt[15] + 
		m_featureTagCnt[16]);

	return;

	/*
	m_LogTrace.WriteLine("%d,%lf", m_frequency, m_weight);
	for(int i = 0; i < FEATURE_TAG; i++)
		m_LogTrace.WriteLine(",%d", m_featureTagCnt[i]);
	m_LogTrace.WriteLine("\n");
	return;
	*/


//	m_LogTrace.WriteLine("%d,", m_frequency);
//	m_LogTrace.WriteLine("%lf,", m_weight);
	m_LogTrace.WriteLine("%d,", m_featureTagCnt[0]);
	m_LogTrace.WriteLine("%d\n", m_featureTagCnt[1]);

//	m_LogTrace.WriteLine("%d,", m_featureTagCnt[2]+m_featureTagCnt[3]+m_featureTagCnt[4]+
//		m_featureTagCnt[5]+m_featureTagCnt[6]+m_featureTagCnt[7]);

//	m_LogTrace.WriteLine("%d\n", 
//		m_featureTagCnt[8] + m_featureTagCnt[9] + m_featureTagCnt[10] +
//		m_featureTagCnt[11] + m_featureTagCnt[12] +
//		m_featureTagCnt[13] + 
//		m_featureTagCnt[14] + 
//		m_featureTagCnt[15] + 
//		m_featureTagCnt[16]);

	/*
	m_LogTrace.WriteLine("\n%s (%d : %lf)\n",m_word, m_frequency, m_weight);
	m_LogTrace.WriteLine("[");
	for(int i = 0; i < FEATURE_TAG; i++)
		m_LogTrace.WriteLine(" %d ", m_featureTagCnt[i]);
	m_LogTrace.WriteLine(" ]\n");

	m_LogTrace.WriteLine("In Title : ");
	if(m_inTitle)
		m_LogTrace.WriteLine("TRUE");
	else
		m_LogTrace.WriteLine("FALSE");


	m_LogTrace.WriteLine("	In Meta : ");
	if(m_inMeta)
		m_LogTrace.WriteLine("TRUE");
	else
		m_LogTrace.WriteLine("FALSE");

	m_LogTrace.WriteLine("	#Hyperlink : %d\n", m_inHyperlink);
	*/
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPageCache::CPageCache()
{
	m_words = (Hash *) new Hash(100);
	m_outURLs = (Hash *) new Hash(100);

	m_isSearchQuery = false;
	m_queryResults = (List *) new List();

	m_lastModified.Empty();
	m_bLoaded = false;

	aFrames.RemoveAll();
	aNames.RemoveAll();
	aTargets.RemoveAll();
	aFrameIDs.RemoveAll();

	m_envTextProcessor = NULL;
	m_envSearchEngine = NULL;

	m_bJewellTFIDF = false;
	m_bCalLayout = false;
}

CPageCache::~CPageCache()
{
	delete m_words;
	delete m_outURLs;

	delete m_queryResults;

	aFrames.RemoveAll();
	aNames.RemoveAll();
	aTargets.RemoveAll();
	aFrameIDs.RemoveAll();
}



// load into the cache, check whether the cache file exists,
// if yes, read it in, otherwise return false
bool CPageCache::LoadCache(const char *strCacheFile)
{
	if(strCacheFile == NULL && m_cacheFile.IsEmpty())
		return false;

	/*
	CString sFile;
	sFile = strCacheFile;
	if(sFile.IsEmpty())
		sFile = m_cacheFile;
	*/

	if(strCacheFile != NULL && m_cacheFile.IsEmpty())
		m_cacheFile = strCacheFile;

	if(PathFileExists(m_cacheFile))
	{
		if(m_bLoaded)
			Release();

		CFile m_rFile( m_cacheFile, CFile::modeRead);
		CArchive m_rArch( &m_rFile, CArchive::load );
		int hr = serialize(m_rArch);
		m_rArch.Close();
		m_rFile.Close();

		if(hr == 0)
			m_bLoaded = true;

		return m_bLoaded;
	}
	return false;
}


// save the cache into a cache file
//其实，就是把当前浏览网页的一些信息用二进制写进缓存文件(就是传说中的cache/xx.dat文件)
void CPageCache::SaveCache(const char *strCacheFile)
{
	if(strCacheFile == NULL && m_cacheFile.IsEmpty())
		return;

//保存dat文件名为sFile
	CString sFile;
	sFile = strCacheFile;
	if(sFile.IsEmpty())
		sFile = m_cacheFile;

	CFile m_wFile( sFile, CFile::modeCreate | CFile::modeWrite);
	CArchive m_wArch( &m_wFile, CArchive::store );
	serialize(m_wArch);
	m_wArch.Close();
	m_wFile.Close();
}


// release all the memory that allocated for this object
void CPageCache::Release()
{

	if(m_words)
	{
		delete m_words;
		m_words = NULL;
	}

	if(m_outURLs)
	{
		delete m_outURLs;
		m_outURLs = NULL;
	}

	if(m_queryResults)
	{
		delete m_queryResults;
		m_queryResults = NULL;
	}

	aFrames.RemoveAll();
	aNames.RemoveAll();
	aTargets.RemoveAll();
	aFrameIDs.RemoveAll();

	m_words = (Hash *) new Hash(100);
	m_outURLs = (Hash *) new Hash(100);

	m_queryResults = (List *) new List();

	m_isSearchQuery = false;

//	m_lastModified.Empty();
	m_bLoaded = false;

	aFrames.RemoveAll();
	aNames.RemoveAll();
	aTargets.RemoveAll();
	aFrameIDs.RemoveAll();

//	m_envTextProcessor = NULL;
//	m_envSearchEngine = NULL;

	m_bJewellTFIDF = false;
	m_bCalLayout = false;

}

// load the file into cache, if only HTML source file is available,
// we should parse it, convert into content tree, and save into
// cache file.
bool CPageCache::Load()
{
	bool result = true;

	// at first, we check whether we have such local cache file
	// Read them back
	if(LoadCache())
		return true;

	// if not, then we should parse the html file, and
	// convert it to the CPageTagNode
	CPageDocument *pHtml;

	pHtml = (CPageDocument *) new CPageDocument();
	if(pHtml->loadOffLine(m_pageno, m_sourceFile, m_cacheFile))
		Load(pHtml);
	else
		result = false;

	delete pHtml;

	// then we save the parsed HTML into local file
	// for future work
	SaveCache();

	return result;
}

void CPageCache::LoadByContent(const CString& m_content)
{
	// if not, then we should parse the html file, and
	// convert it to the CPageTagNode
	CPageDocument *pHtml;

	pHtml = (CPageDocument *) new CPageDocument();
	pHtml->loadByContent(m_content);
	Load(pHtml);

	delete pHtml;
}


void CPageCache::Load(IDispatch *pDisp, bool online)
{
	CPageDocument *pHtml;

	pHtml = (CPageDocument *) new CPageDocument();
	pHtml->loadOnLine(pDisp, online);

	Load(pHtml);

	m_bLoaded = true;

	delete pHtml;
}



// load the cache from the page document
void CPageCache::Load(CPageDocument *pDocument)
{
	CFrame *pFrame;
	List *pList;
	CString buffer;
	CPageTagNode *root;

	pDocument->getBaseTarget(baseTarget);
	pDocument->getBaseURL(baseURL);

	buffer.Empty();
	pDocument->getLastModified(buffer);
	if(!buffer.IsEmpty())
		m_lastModified = buffer;

	aFrames.RemoveAll();
	aNames.RemoveAll();
	aTargets.RemoveAll();

    //pDocument->serialize();//add by ny
	pList = pDocument->getFrames();
	if(pList)
	{
		for(int i = 0; i < pList->size(); i++)
		{
			pFrame = (CFrame *) pList->get(i);

			buffer.Empty();
			pFrame->getSrc(buffer);
			aFrames.Add(buffer);

			buffer.Empty();
			pFrame->getName(buffer);
			aNames.Add(buffer);

			buffer.Empty();
			pFrame->getTarget(buffer);
			aTargets.Add(buffer);
		}
	}

	// parse the html tree
	root = pDocument->getDocument();
	if(root)
		parseTree(root);

	// parse the url to check whether it is a search query and
	// extract the key words
	if(!m_url.IsEmpty())
	{
		m_envSearchEngine->parseURL(m_url, m_envTextProcessor);
		m_isSearchQuery = m_envSearchEngine->m_bSearch;
		m_queryWords.RemoveAll();

		if(m_envSearchEngine->m_queryKeyWords.GetSize() > 0)
			m_queryWords.Append(m_envSearchEngine->m_queryKeyWords);

		// if it is a search query, extract the title and snippet
		if(root)
			m_envSearchEngine->parseQueryResult(m_url, root, 
			m_envTextProcessor, m_queryResults);
	}


	// if we are required to calculate the extra layout features
	/*
	if(m_words && m_bCalLayout)
	{
		CPageCacheWord *pWord;
		CPageDocument2 *pDoc2;
		CPageWord *pLayout;

		pDoc2 = (CPageDocument2 *) new CPageDocument2();
		pDoc2->m_envTextProcessor = m_envTextProcessor;

		if(pDoc2->calLayoutRole(pDocument))
		{
			m_words->resetSeek();
			while( (pWord = (CPageCacheWord *)m_words->getObject()) != NULL)
			{
				if( (pLayout = pDoc2->lookup(pWord->getWord())) != NULL)
				{
					pWord->m_fgColor = pLayout->m_fgColor;
					pWord->m_bgColor = pLayout->m_bgColor;

					pWord->m_fontFamily = pLayout->m_fontFamily;
					pWord->m_fontSize = pLayout->m_fontSize;
					pWord->m_fontStyle = pLayout->m_fontStyle;
					pWord->m_fontWeight = pLayout->m_fontWeight;
				}

			}
		}

		delete pDoc2;
	}
	*/
}


void CPageCache::parseTree(CPageTagNode *root)
{
	CPageTagNode *body;
	CPageTagNode *title;
	CPageTagNode *meta;

    CString strText;
    CString strTitle;
    CString strMeta;
    CString strKey;
    CString strDesc;

	int total_word;

	// Process the body text, and the merge it with the sequence word table.
    if((body = root->FindBody()) != NULL)
   	{
		// get the whole text of the page,
		// extract all its words
		strText.Empty();
		body->getText(strText);
		textProcessBody(strText, total_word);

		tagFactorCalculation(root);//change by ny:root
        // extract words in title
        if((title = root->FindTitle()) != NULL)
        {
            title->getText(strTitle);
            inTitleMeta(strTitle, IN_TITLE);
        }

        // extract words in meta(invisible)
        if((meta = root->FindMeta()) != NULL)
        {
            meta->getAttrVal("keyword", strKey);
            meta->getAttrVal("content", strDesc);
            strMeta = strKey + strDesc;
            inTitleMeta(strMeta, IN_META);
        }

        calculateTFIDF();
	}
}



void CPageCache::buildURL(CString m_href, CString &result)
{
	CUrl cUrl(m_url, baseURL, m_href);
	result = cUrl.getURL();

    return;
}


// Insert the word into the hash table, if can find such 
// word, just increase the count, otherwise insert a new
// word into the hash table
void CPageCache::InsertWord(const char *pword, int count, int tag)
{
	CPageCacheWord *ptank;
	CPageCacheWord *pt;
	// look up the word in the Hash table
	ptank = (CPageCacheWord *)new CPageCacheWord(pword, count);
	if((pt = (CPageCacheWord *)m_words->lookup(ptank)) != NULL)
	{
		// if it is there, just increase its occurrence number,
		// and tag count
		pt->setFreq(ptank->getFreq() + pt->getFreq());
		pt->increaseTagCnt(tag);
		delete ptank;
	}
	else
	{
		// otherwise, insert it into the Hash table
		ptank->increaseTagCnt(tag);
		m_words->insert(ptank);
	}

	return;
}


// search for a word in this page, if it exists, return true;
// otherwise false
bool CPageCache::searchWord(const CString &strWord)
{
	if(m_words == NULL || strWord.IsEmpty())
		return false;

	CPageCacheWord *pWord;
	CPageCacheWord *pt;
	bool bExisted = false;

	// look up the word in the Hash table
	pWord = (CPageCacheWord *)new CPageCacheWord(strWord);
	if((pt = (CPageCacheWord *)m_words->lookup(pWord)) != NULL)
		bExisted = true;

	delete pWord;

	return bExisted;
}



// this function takes a string as input, converts characters to lower case;  
// removes all numbers and stopwords, then extract all words from it; removes 
// all stopwords. 
// wordnum is the number of words within the body text
void CPageCache::textProcessBody(CString strText, int& word_num)
{
	CStringArray m_stemWords;

	if(strText.IsEmpty())
		return;

	m_stemWords.RemoveAll();
	m_envTextProcessor->textProcess(strText, m_stemWords);
	word_num = m_stemWords.GetSize();

	for(int i = 0; i < word_num; i++)
	{
		InsertWord(m_stemWords[i], 1);
	}
	m_stemWords.RemoveAll();

    return;
} // The End


// Process the text enclosed by a special tag,
// such as : <h1>Data Mining Software</h1>.
// get each word from the text, increase its m_featureTagCnt
// according to the tag_no
void CPageCache::textProcessTag(CString strText, int tag_no)
{
	CStringArray m_stemWords;
    // char* tagContent=new char[500];
	if(strText.IsEmpty())
		return;

	// extract all the word from text
	m_stemWords.RemoveAll();
	//if((strText[0]>='a'&&strText[0]<='z')||(strText[0]>='A'&&strText[0]<='Z'))
	m_envTextProcessor->textProcessU(strText, m_stemWords);//到底应该用哪一个？为什么加了中文分词就死循环了？
//	else 
//		m_stemWords.Add(strText);
		/*
    const char filename[] = "D:\\tagcontent.txt";
    ofstream o_file;
   // ifstream i_file;
   // string out_text;
   
    //写
	
   o_file.open(filename);
   //	for(int i = 0; i < m_stemWords.GetSize(); i++)
//	{
  //  strcat(tagContent,m_stemWords[i]);
//	}
	o_file << (LPCTSTR)m_stemWords[0];//tagContent
    o_file.close();
*/
	// increase the number
	for(int j = 0; j < m_stemWords.GetSize(); j++)
		InsertWord(m_stemWords[j], 1, tag_no);
	m_stemWords.RemoveAll();

    return;
} // The End


void CPageCache::textProcessURL(const CString& m_newURL, const CString& m_strTarget, const CString& strText)
{
	CPageCacheURL *pNew, *pOld, *pCurrent;
	CStringArray m_stemWords;

	m_stemWords.RemoveAll();
	m_envTextProcessor->textProcessU(strText, m_stemWords);

	pNew = (CPageCacheURL *) new CPageCacheURL();
	pNew->setURL(m_newURL);
	pNew->setTarget(m_strTarget);
	if((pOld = (CPageCacheURL *)m_outURLs->lookup(pNew)) != NULL)
	{
		delete pNew;
		pCurrent = pOld;
	}
	else
	{
		m_outURLs->insert(pNew);
		pCurrent = pNew;
	}

	for(int i = 0; i < m_stemWords.GetSize(); i++)
		pCurrent->addAnchorWord(m_stemWords[i]);
	m_stemWords.RemoveAll();

    return;
} // The End

void CPageCache::extractAnchorText(CPageTagNode *pNode, CString &strText)
{
	CString strTag;
	CPageTagNode *pContent;

	if(pNode->getName(strTag))
	{
		if(strTag.CompareNoCase("a") == 0)
		{
			if(pNode->getText(strText))
			{
				if(strText.IsEmpty())
				{
					pContent = pNode->getContent();
					if(pContent && pContent->getName(strTag))
					{
						if(strTag.CompareNoCase("img") == 0)
							pContent->getAttrVal("alt", strText);
					}
				}
			}
		}

		if(strTag.CompareNoCase("area") == 0)
			pNode->getAttrVal("alt", strText);
	}

    return;
}


// calculate the weight for each word in head, title, a, keyword and description
void CPageCache::tagFactorCalculation(CPageTagNode *root)
{
	CString strName;
	CString strText;
    CString strHref;

	if(root == NULL)
		return;

	// get the tag name of the node
    strName.Empty();
    root->getName(strName);
	if(strName.CompareNoCase("select") == 0 ||
//		strName.CompareNoCase("form") == 0 ||
		strName.CompareNoCase("option") == 0)
		return;

	// extract its value, i.e., the text
    strText.Empty();
    root->getText(strText);
    strText.TrimLeft();
    strText.TrimRight();

	if(!strName.IsEmpty() && !strText.IsEmpty())
	{
		// check with each predefined tag
		for(int i = 0; i < FEATURE_TAG; i++)
		{
			if(strName.CompareNoCase(TagWeightArray[i].tag) == 0)
			{
				// if the text is enclosed by any predefined tag,
				// get all the words in the text, increase the word's
				// m_featureTagCnt
				textProcessTag(strText, i);//处理标签中的文本
				break;
			}
		}
	}

	// now we meet a hyperlink, then extract the text embeded, and also
	// check whether this is URL that followed
	if(!strName.IsEmpty() &&
		(strName.CompareNoCase("a") == 0 || strName.CompareNoCase("area") == 0))
	{
		// get the anchor text of this hyperlink
		strText.Empty();
		extractAnchorText(root, strText);

		InAnchorText(strText);

		if(root->getAttrVal("href", strHref))
		{
			CString m_newURL;
			CString m_strTarget;

			root->getAttrVal("target", m_strTarget);

			buildURL(strHref, m_newURL);
			textProcessURL(m_newURL, m_strTarget, strText);//处理url中的文本
		}
	}

	// recrusively process all other nodes.
	for(CPageTagNode *pNode = root->getContent(); pNode != NULL; pNode = pNode->getNext())
		tagFactorCalculation(pNode);//嵌套处理所有的节点

	return;
}

// processing words in hyperlink, including <a> <area>
void CPageCache::InAnchorText(CString strText)
{
    if(strText.IsEmpty())
        return;

    CPageCacheWord *pNew, *pOld, *pLocal;
	CStringArray m_stemWords;

	m_stemWords.RemoveAll();
	m_envTextProcessor->textProcessU(strText, m_stemWords);

	for(int i = 0; i < m_stemWords.GetSize(); i++)
	{
		pNew = (CPageCacheWord *) new CPageCacheWord();
		pNew->setWord(m_stemWords[i]);
		if((pOld = (CPageCacheWord *)m_words->lookup(pNew)) != NULL)
		{
			delete pNew;
			pLocal = pOld;
		}
		else
		{
			m_words->insert(pNew);
			pLocal = pNew;
		}

		pLocal->increaseInHyperlink();
	}

	m_stemWords.RemoveAll();

    return;
}


// state : 0 title, 1 Meta
void CPageCache::inTitleMeta(CString strText, int location)
{
    if(strText.IsEmpty())
        return;

    CPageCacheWord *pNew, *pOld, *pLocal;
	CStringArray m_stemWords;

	m_stemWords.RemoveAll();
	m_envTextProcessor->textProcessU(strText, m_stemWords);

	for(int i = 0; i < m_stemWords.GetSize(); i++)
	{
		pNew = (CPageCacheWord *) new CPageCacheWord();
		pNew->setWord(m_stemWords[i]);
		if((pOld = (CPageCacheWord *)m_words->lookup(pNew)) != NULL)
		{
			delete pNew;
			pLocal = pOld;
		}
		else
		{
			m_words->insert(pNew);
			pLocal = pNew;
		}

		switch (location)
		{
			case IN_TITLE:
				pLocal->setInTitleFlag(true);
				break;
			case IN_META:
				pLocal->setInMetaFlag(true);
				break;

			default:
				break;
		}
	}

	m_stemWords.RemoveAll();

    return;
}

// compute the Jewell and TFIDF weight for each word in
// the page cache
void CPageCache::computeJewellTFIDF()
{
	if(m_bJewellTFIDF)
		return;

    CPageCacheWord *pLocal;
	double maxTFIDF, minTFIDF;
	double maxJewell, minJewell;
	double score;

	maxTFIDF = -9999.0;
	minTFIDF = 9999.0;

	maxJewell = -9999.0;
	minJewell = 9999.0;

    // call each word to compute its Jewell and TFIDF weights
	m_words->resetSeek();
	while((pLocal = (CPageCacheWord *)m_words->getObject()) != NULL)
	{
		pLocal->computeTFIDFJewell();

		score = pLocal->getTFIDFWeight();
		if(score >= maxTFIDF)
			maxTFIDF = score;
		if(score <= minTFIDF)
			minTFIDF = score;

		score = pLocal->getJewellWeight();
		if(score >= maxJewell)
			maxJewell = score;
		if(score <= minJewell)
			minJewell = score;
	}

	// Normalization
	m_words->resetSeek();
	while((pLocal = (CPageCacheWord *)m_words->getObject()) != NULL)
	{
		score = pLocal->getTFIDFWeight();
		if(maxTFIDF > minTFIDF)
			pLocal->setTFIDFWeight( (score-minTFIDF)/(maxTFIDF-minTFIDF) );

		score = pLocal->getJewellWeight();
		if(maxJewell > minJewell)
			pLocal->setJewellWeight( (score-minJewell)/(maxJewell-minJewell) );
	}

	m_bJewellTFIDF = true;
}


// Calculate word's weight by using TF/IDF
void CPageCache::calculateTFIDF()
{
    CPageCacheWord *pLocal;
    double idf_sum = 0.0, idf_weight;

    // get the idf weight for each word in the page
	m_words->resetSeek();
	while((pLocal = (CPageCacheWord *)m_words->getObject()) != NULL)
    {
		idf_weight = m_envTextProcessor->lookupIDF(pLocal->getWord());
        pLocal->setWeightIDF(idf_weight);
        if(pLocal->getWeightIDF() != -1.0)
            idf_sum += pLocal->getWeightIDF();
    }

    // normalize the IDF weight, and calculate the weight
	m_words->resetSeek();
	while((pLocal = (CPageCacheWord *)m_words->getObject()) != NULL)
    {
        if(pLocal->getWeightIDF() != -1.0)
            pLocal->setWeightIDF( pLocal->getWeightIDF() / idf_sum );
        else
            pLocal->setWeightIDF( 1.0 );

		pLocal->calculateTFIDF();
    }

	return;
}


//将一些信息以二进制的形式存入磁盘文件
int CPageCache::serialize(CArchive &pArchive)
{
	int nStatus = 0, i, len, size, wordsize, urlsize, keysize, resultsize, flag;
	long frameID;
    CString str;
	int hr;

	try
	{
	    // Serialize the object ...
		if (pArchive.IsStoring())
		{
			pArchive << m_url.GetLength();//当前的网页的url
			if(m_url.GetLength() > 0)
				pArchive << m_url;

			pArchive << m_lastModified.GetLength();
			if(m_lastModified.GetLength() > 0)
				pArchive << m_lastModified;

			pArchive << baseURL.GetLength();
			if(baseURL.GetLength() > 0)
				pArchive << baseURL;

			pArchive << baseTarget.GetLength();
			if(baseTarget.GetLength() > 0)
				pArchive << baseTarget;

			pArchive << aFrames.GetSize();
			for(i = 0; i < aFrames.GetSize(); i++)
			{
				pArchive << aFrames[i].GetLength();
				if(aFrames[i].GetLength() > 0)
					pArchive << aFrames[i];

				pArchive << aNames[i].GetLength();
				if(aNames[i].GetLength() > 0)
					pArchive << aNames[i];

				pArchive << aTargets[i].GetLength();
				if(aTargets[i].GetLength() > 0)
					pArchive << aTargets[i];
			}

			pArchive << aFrameIDs.GetSize();
			for(i = 0; i < aFrameIDs.GetSize(); i++)
				pArchive << aFrameIDs[i];


			flag = m_isSearchQuery?1:0;
			pArchive << flag;

			pArchive << m_words->size();
			pArchive << m_outURLs->size();
			pArchive << m_queryWords.GetSize();
			pArchive << m_queryResults->size();
			saveWordURL(pArchive);
		}
		else
		{
			aFrames.RemoveAll();
			aNames.RemoveAll();

			baseURL.Empty();
			baseTarget.Empty();
			m_lastModified.Empty();

			pArchive >> len;
			if(len > 0)
				pArchive >> m_url;

			pArchive >> len;
			if(len > 0)
				pArchive >> m_lastModified;

			pArchive >> len;
			if(len > 0)
				pArchive >> baseURL;

			pArchive >> len;
			if(len > 0)
				pArchive >> baseTarget;

			pArchive >> len;
			for(i = 0; i < len; i++)
			{
				str.Empty();
				pArchive >> size;
				if(size > 0)
					pArchive >> str;
				aFrames.Add(str);

				str.Empty();
				pArchive >> size;
				if(size > 0)
					pArchive >> str;
				aNames.Add(str);

				str.Empty();
				pArchive >> size;
				if(size > 0)
					pArchive >> str;
				aTargets.Add(str);
			}

			pArchive >> len;
			for(i = 0; i < len; i++)
			{
				pArchive >> frameID;
				aFrameIDs.Add(frameID);
			}

			pArchive >> flag;
			m_isSearchQuery = (flag == 1)?true:false;

			pArchive >> wordsize;
			pArchive >> urlsize;
			pArchive >> keysize;
			pArchive >> resultsize;

			hr = loadWordURL(pArchive, wordsize, urlsize, keysize, resultsize);
			if(hr != 0)
				return hr;
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


void CPageCache::dump()
{
	m_LogTrace.WriteLine("-----------------------------------------------------\n");
	m_LogTrace.WriteLine("          URL : %s\n", m_url);
	m_LogTrace.WriteLine("      Page ID : %ld\n", m_pageno);
	m_LogTrace.WriteLine("Last Modified : %s\n\n", m_lastModified);

//	m_words->dump();
//	m_outURLs->dump();

	m_LogTrace.WriteLine("\n");
}


void CPageCache::saveWordURL(CArchive &pArchive)
{
	CPageCacheWord *pWord;
	CPageCacheURL *pURL;
	CQueryResult *pResult;

	m_words->resetSeek();
	while((pWord = (CPageCacheWord*) m_words->getObject()) != NULL)
		pWord->serialize(pArchive);

	m_outURLs->resetSeek();
	while((pURL = (CPageCacheURL *) m_outURLs->getObject()) != NULL)
		pURL->serialize(pArchive);

	for(int j = 0; j < m_queryWords.GetSize(); j++)
		pArchive << m_queryWords[j];

	for(int i = 0; i < m_queryResults->size(); i++)
	{
		pResult = (CQueryResult *) m_queryResults->get(i);
		pResult->serialize(pArchive);
	}
}


int CPageCache::loadWordURL(CArchive &pArchive, int wordsize, int urlsize, int keysize, int resultsize)
{
	CPageCacheWord *pWord;
	CPageCacheURL *pURL;
	CQueryResult *pResult;
	CString buffer;
	int i, whr, uhr, qhr;

	if(!m_words)
		m_words = (Hash *) new Hash(100);

	if(!m_outURLs)
		m_outURLs = (Hash *) new Hash(100);

	if(!m_queryResults)
		m_queryResults = (List *) new List();

	if(wordsize > 0)
	{
		for( i = 0; i < wordsize; i++)
		{
			pWord = (CPageCacheWord *) new CPageCacheWord();
			whr = pWord->serialize(pArchive);
			if(whr != 0)
			{
				m_words->clear();
				return 1;
			}

			m_words->insert(pWord);
		}
	}

	if(urlsize > 0)
	{
		for(i = 0; i < urlsize; i++)
		{
			pURL = (CPageCacheURL *) new CPageCacheURL();
			uhr = pURL->serialize(pArchive);
			if(uhr != 0)
			{
				m_words->clear();
				m_outURLs->clear();
				return 1;
			}

			m_outURLs->insert(pURL);
		}
	}

	m_queryWords.RemoveAll();
	if(keysize > 0)
	{
		for(i = 0; i < keysize; i++)
		{
			pArchive >> buffer;
			m_queryWords.Add(buffer);
		}
	}

	if(resultsize > 0)
	{
		for(i = 0; i < resultsize; i++)
		{
			pResult = (CQueryResult *) new CQueryResult();
			qhr = pResult->serialize(pArchive);
			if(qhr != 0)
			{
				m_words->clear();
				m_outURLs->clear();
				m_queryWords.RemoveAll();
				m_queryResults->clear();

				return 1;
			}

			m_queryResults->add(pResult);
		}
	}

	return 0;
}






CPageCacheURL * CPageCache::searchURL(const CString &m_url)
{
	if(!m_outURLs)
		return NULL;

	CPageCacheURL *pLocator = (CPageCacheURL *)NULL;
	CPageCacheURL *pSearch;

	pSearch = (CPageCacheURL *) new CPageCacheURL();
	pSearch->setURL(m_url);

	pLocator = (CPageCacheURL *)m_outURLs->lookup(pSearch);

	delete pSearch;

    return pLocator;
}

bool CPageCache::SearchURLTarget(const CString &m_url, CString &m_target)
{
	CPageCacheURL *pSearch;
	if((pSearch = searchURL(m_url)) == NULL)
		return false;

	m_target = pSearch->getTarget();
	return true;
}


// given an URL, locate it in the URL hash table
CPageCacheURL * CPageCache::locateURL(const CString &m_url)
{
	return searchURL(m_url);
}



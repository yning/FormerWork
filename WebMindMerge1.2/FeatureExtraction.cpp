///////////////////////////////////////////////////////////////
// FeatureExtraction.cpp: implementation of the CFeatureExtraction class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FeatureExtraction.h"

#include "PageCache.h"

#include "WordSeq.h"
#include "SearchPage.h"
#include "WordSearch.h"
#include "WordSession.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFeatureExtraction::CFeatureExtraction()
{
	m_pICSession = NULL;
	m_sessionWords = NULL;
	m_destWords = NULL;

	m_bICQueryModel = false;
}

CFeatureExtraction::~CFeatureExtraction()
{
	if(m_sessionWords)
		delete m_sessionWords;

	if(m_destWords)
		delete m_destWords;
}

// this is the main function for feature extraction.
void CFeatureExtraction::Exec()
{
	// the first step of the feature vector extraction is to
	// get the browsing bwhvior of each involved word,
	// then extract the word feature vectors

	if(m_pICSession->isICSession())
		m_type = 0;
	else
		m_type = 1;

	genBrowsingBehavior(m_pICSession->m_session);
	genWordVectors(m_pICSession->m_session, m_sessionWords, m_destWords);
}


// generate browsing behavior features from the list
// at first, for each page in the session,
//   we get all the words from CPageCacheWord, and for each word,
//     we put it into a CWordSeq. There will be only one CWordSeq
//     for each word in the whole session, so every time, acquire a
//     word from CPageCache, either we creat a new CWordSeq, or
//     change the parameters of the existing CWordSeq.
//     The WordSeq will be used to generate the real browsing features
//     i.e., WordSession

void CFeatureExtraction::genBrowsingBehavior(List *m_pSession)
{
	// initialization
	if(m_sessionWords)
		delete m_sessionWords;
	m_sessionWords = NULL;

	if(m_destWords)
		delete m_destWords;
	m_destWords = NULL;


	// if there is nothing we can do, just return
    if(m_pSession == NULL || m_pSession->size() <= 2)
        return;


	CPageView *m_viewCurrent, *m_viewNext, *m_viewBack, *m_viewDestination;
    int indx, iframe;

	// will store all the words' browsing features sequential category
	Hash *m_gWords = NULL;

	// store the words' browsing features search category
	Hash *m_searchWords = NULL;

	CPageCache *pPage;

    CPageFrame *pFrame;
    CItemSet *seqState;
    bool hasSearch;

	CString m_urlCurrent;
	CString m_urlFollow;
	bool m_bBacked;
	int scope;

    m_gWords = (Hash *)new Hash();
    m_viewNext = NULL;

    // identify sequential state for each page in the session
    seqState = (CItemSet *)new CItemSet();
    SequentialState(m_pSession, seqState);

    hasSearch = false;

	// if the session is ended with an IC-page, then we should
	// eliminate the final page(i.e., IC-page) when we extract
	// browsing features
	if(m_type == 0)
		scope = m_pSession->size() - 1;
	else
		scope = m_pSession->size();

    // go through each page in the sequence, and construct a page view
    // sequence based on the original page sequence.
    for(indx = 0; indx < scope; indx++)
    {
        m_viewCurrent = (CPageView *)m_pSession->get(indx);

		// just skip all the search result pages
        if(m_viewCurrent->searchQuery)
        {
            hasSearch = true;
            continue;
        }

        // keep the current URL
        m_urlCurrent.Empty();
        m_urlCurrent = m_viewCurrent->getURL();

        // get the next URL in the sequence.
        // if m_urlFollow is empty, it reaches the end of sequence.
        m_urlFollow.Empty();
        if(indx < scope - 1)
        {
            m_viewNext = (CPageView *)m_pSession->get(indx+1);
			if(m_viewNext)
				m_urlFollow = m_viewNext->m_url;
        }

        // to check whether the next page is reached by pressing 
        // Back button
        m_bBacked = false;
        if(indx >= 1)
        {
            m_viewBack = (CPageView *)m_pSession->get(indx-1);
			if(m_viewBack)
			{
	            if(m_viewBack->getPageno() == m_viewCurrent->getPageno())
		            m_bBacked = true;
			}
        }


		// let's process the page if it contains frame
		// if people use <iframe...> the parent page still contains useful content
		pPage = getPage(m_viewCurrent->getPageno());
		if(pPage)
			PageProcess(indx, pPage, seqState, m_urlFollow, m_gWords);

        if(m_viewCurrent->m_framepages)
        {
            for(iframe = 0; iframe < m_viewCurrent->m_framepages->size(); iframe++)
            {
                pFrame = (CPageFrame *)m_viewCurrent->m_framepages->get(iframe);

                m_urlCurrent = pFrame->m_src;
				pPage = getPage(pFrame->pageno);
		    	if(pPage)
			    	PageProcess(indx, pPage, seqState, m_urlFollow, m_gWords);
            }
        }
    }//end for

	// If there is any search result page in the sequence, we
	// should calculate the browsing features of search category.
    m_searchWords = NULL;
    if(hasSearch)
    {
        m_searchWords = (Hash *)new Hash();
        DoSearchQuery(m_pSession, m_searchWords);
    }

	// then we merge the information for every word,
	// sequential or search category
	m_sessionWords = (Hash *)new Hash();
    Merge(m_gWords, seqState, m_searchWords, m_sessionWords);

	if(m_type == 0)
	{
	    m_viewDestination = (CPageView *)m_pSession->get(m_pSession->size() - 1);
		m_destWords = (Hash *)new Hash();
	    getDestinationWords(m_viewDestination, m_destWords);
	}


    delete m_gWords;
    delete seqState;

    if(m_searchWords)
        delete m_searchWords;

    return;
}


// Output the feature extracted from a session
//在一个session中抽取特征并输出
void CFeatureExtraction::genWordVectors(List *pSession, Hash *m_sessionWords, Hash *m_destWords)
{
//	int indx;
	int hitted;
    CWordSession *pWSession, *pws;
    CLilacString *pWLocal, *pwl;
	double ratiohit = 0.0;

    if(m_sessionWords == NULL ||
        m_sessionWords->size() <= 0 )
        return;

	if(m_type == 0 && m_destWords)
	{
	    hitted = 0;
		m_destWords->resetSeek();
		while((pWLocal = (CLilacString *)m_destWords->getObject()) != NULL)
		{
			pWSession = (CWordSession *)new CWordSession();
	        pWSession->Word(pWLocal->get());
		    if((pws = (CWordSession *)m_sessionWords->lookup(pWSession)) != NULL)
			    ++hitted;
	        delete pWSession;
		}
		if(m_destWords->size())
			ratiohit = (double)hitted/(double)m_destWords->size();
	}

	m_pICSession->m_coverage = ratiohit;

	// output the feature data
	m_sessionWords->resetSeek();
	while((pWSession = (CWordSession *)m_sessionWords->getObject()) != NULL)
	{
		if(m_type == 0 && m_destWords)
		{
			pWLocal = (CLilacString *)new CLilacString(pWSession->Word());
			if((pwl = (CLilacString *)m_destWords->lookup(pWLocal)) != NULL)
				pWSession->ICword = true;
	        else
				pWSession->ICword = false;

	        delete pWLocal;
		}

		m_pICSession->m_wordVectors->add(pWSession->clone());
	}
}


CPageCache * CFeatureExtraction::getPage(int pageno)
{
	if(pageno < 0)
		return NULL;

	return m_pageRepository->getPageCache(pageno);
}


// identify the sequential state of each page in the page sequence
//
// If the page doesnot appear in previous sequence, it is forward, 
//     otherwise it is backward
// if the page is the same page as last page, then it is steady,
//     probably the user clicks "Refresh" button or "Back Home" at the
//     home page.
//
//  All the words in the page will share the same sequential status
void CFeatureExtraction::SequentialState(List *pSession, CItemSet *stateSeq)
{
    // page sequential state in session
    // -1 : backward
    //  0 : steady
    //  1 : forward
    int state;
	CStringArray strURLs;
	CPageView *pPrevView;
	CPageView *pView;
	CPageView *pNextView;

	strURLs.RemoveAll();

    for(int j = 0; j < pSession->size() - 1; j++)
    {
        pView = (CPageView *)pSession->get(j);
		pNextView = (CPageView *)pSession->get(j + 1);

		state = 1;
		if(pNextView->m_action >= 0)
		{
			if(pNextView->m_action  == 1)
				state = -1;
		}
		else
		{
			if(FindLocation(strURLs, pNextView->m_url) >= 0)
				state = -1;
		}

        if(j > 0)
        {
			pPrevView = (CPageView *)pSession->get(j-1);
			if(pView->m_url.Compare(pPrevView->m_url) == 0)
				state = 0;
        }

        stateSeq->add(state);
		strURLs.Add(pView->m_url);
    }

	stateSeq->add(1);
	strURLs.RemoveAll();

    return;
}

// search the word in the array, if it is in the array, return its index,
// otherwise -1
int CFeatureExtraction::FindLocation(const CStringArray &srcArray, const CString &strWord)
{
	bool bExisted = false;
	
	for(int index = 0; index < srcArray.GetSize(); index++)
	{
		if(strWord.Compare(srcArray[index]) == 0)
		{
			bExisted = true;
			break;
		}
	}

	if(bExisted)
		return index;
	
	return -1;
}


// Process the text of this web page, and merge its result to the 
// whole sequence's word hash
//    pageno : the page No. in the sequence.
//     pPage : the PageCache
//  seqState : the sequential state of each page in the sequence
// m_hgwords : the hash table that store each word's features.

void CFeatureExtraction::PageProcess(int pageno, CPageCache *pPage, CItemSet *seqState, 
									const CString& m_urlFollow, Hash *m_hgwords)
{
	Hash *m_hWords;
	Hash *m_hURLs;
	CWordSeq *pwordSeq, *pws;
	CPageCacheWord *m_pWord;
	CPageCacheURL *m_pURL;

	// compute Jewell and TFIDF weight for each word in the PageCahe
	pPage->computeJewellTFIDF();

	// at first, we copy the information of each word in PageCache into
	// a WordSeq, and insert them into the hash table.
	// after the whole sequence has been done, it is very possible
	//  that one word appears several times in the Hash table

	m_hWords = pPage->getAllWords();
	m_hWords->resetSeek();
	try
	{
	while((m_pWord = (CPageCacheWord *)m_hWords->getObject()) != NULL)
	{
        pwordSeq = (CWordSeq *)new CWordSeq(); 
        pwordSeq->setWord(m_pWord->getWord());
		pwordSeq->setWeightSize(seqState->size());

		// make sure whether the word occurs anywhere before this page
		if((pws = (CWordSeq *)m_hgwords->lookup(pwordSeq)) != NULL)
		{
			delete pwordSeq;
			pwordSeq = pws;
		}
		else
			m_hgwords->insert(pwordSeq);

		////////////////////////////////////////////////////////////////
		// merge the word's parameters in this page to the WordSeq
		////////////////////////////////////////////////////////////////

		pwordSeq->weightTFIDF[pageno] = m_pWord->getTFIDFWeight();
		pwordSeq->weightJewell[pageno] = m_pWord->getJewellWeight();

		pwordSeq->frequency += m_pWord->getFreq();

		if(m_pWord->getInTitleFlag())
			pwordSeq->inTitleCnt += 1;

		if(m_pWord->getInMetaFlag())
			pwordSeq->inMetaCnt += 1;

		pwordSeq->inHyperlinkCnt += m_pWord->getInHyperLink();
	}
	}
	catch(CException e)
	{
	int i=0;
	}


	// then we check the next URL and find the URL in current page view
	// extract all its anchor words, and locate these words in the 
	// new hash table built above, to update its inFromPages

	m_hURLs = pPage->getOutURLs();

	m_pURL = pPage->locateURL(m_urlFollow);

	if(m_pURL)
	{
		CStringArray& m_words = m_pURL->getAnchorWords();
        pwordSeq = (CWordSeq *)new CWordSeq(); 

		for(int i = 0; i < m_words.GetSize(); i++)
		{
		    pwordSeq->setWord(m_words[i]);
			if((pws = (CWordSeq *)m_hgwords->lookup(pwordSeq)) != NULL)
			{
				if(!m_urlFollow.IsEmpty())
					pws->inFromPages->add(pageno+1);
			}
		}

		delete pwordSeq;
	}

	return;
}



// extract the search attribute of the word in the pages.
void CFeatureExtraction::DoSearchQuery(List *pSession, Hash *m_wordSearch)
{
	CPageView *m_viewCurrent, *m_viewNext;
    int i, j, k, indx, iquery, *state, len, scope;
    List *m_query;
    CSearchPage *pQuery;
    bool backedQuery, hit;
	CString m_urlCurrent;
	CString m_urlFollow;

	// for each search query in the session, get its
	// snippets and extract features from the snippets
    m_query = (List *) new List();
	if(m_type == 0)
		scope = pSession->size() - 1;
	else
		scope = pSession->size();

    for(indx = 0; indx < scope; indx++)
    {
        m_viewCurrent = (CPageView *)pSession->get(indx);

        // store the current URL
        m_urlCurrent.Empty();
        m_urlCurrent = m_viewCurrent->getURL();

        // get the next URL in the sequence.
        // if m_urlFollow is empth, it reaches the end of sequence.
        m_urlFollow.Empty();
        if(indx < scope - 1)
        {
            m_viewNext = (CPageView *)pSession->get(indx+1);
            if(m_viewNext && !m_viewNext->searchQuery)
                m_urlFollow = m_viewNext->m_url;
        }

        if(m_viewCurrent->searchQuery)
        {
			// check whether the current search page has
			// been visited before
            backedQuery = false;
            for(iquery = 0; iquery < m_query->size(); iquery++)
            {
                pQuery = (CSearchPage *)m_query->get(iquery);
                if(pQuery->m_pageno == m_viewCurrent->getPageno())
                {
                    backedQuery = true;

					pQuery->m_queryCnt += 1;

					if(m_viewNext && !m_viewNext->searchQuery)
		                pQuery->m_followURL.Add(m_urlFollow);

                    break;
                }
            }

            if(!backedQuery)
            {
                pQuery = (CSearchPage *)new CSearchPage();
                pQuery->m_url = m_urlCurrent;
				pQuery->m_pageno = m_viewCurrent->getPageno();

				if(m_viewNext && !m_viewNext->searchQuery)
					pQuery->m_followURL.Add(m_urlFollow);

				pQuery->m_queryCnt = 1;
                m_query->add(pQuery);
            }
        }
    }


	CPageCache *pPage;
	List *m_queryResults;
	CQueryResult *m_pResult;

	for(indx = 0; indx < m_query->size(); indx++)
	{
		pQuery = (CSearchPage *)m_query->get(indx);

		pPage = getPage(pQuery->getPageno());
		if(!pPage)
			continue;

		m_queryResults = pPage->getQueryResults();

		// we use the state to identify whether the entry in the 
		// search result is:
		// 0 : untouched
		// 1 : chosen
		// 2 : skipped
		// 3 : backed (if a entry is backed, it must be chosen before)

		len = m_queryResults->size();
		if(len <= 0)
			continue;

		state = (int *) new int[len];
		for(i = 0; i < len; i++)
			state[i] = 0;

		hit = false;
		for(j = 0; j < pQuery->m_followURL.GetSize(); j++)
		{
			for(k = 0; k < m_queryResults->size(); k++)
			{
				m_pResult = (CQueryResult *) m_queryResults->get(k);
				if(pQuery->m_followURL[j].Compare(m_pResult->getURL()) == 0)
				{
					hit = true;

					if(state[k] == 2)
					{
						state[k] = 3;
					}
					else
					{
						state[k] = 1;
						for(i = 0; i < k; i++)
						{
							if(state[i] == 1)
								state[i] = 3;
							else
								state[i] = 2;
						}
					}
				}
			}
		}

		// if none entry matches the followinf URL,
		// it means that the user clicks other area in the search result page.
		// and thus we take all the entry as skipped.
		if(!hit)
		{
			for(i = 0; i < len; i++)
				state[i] = 2;
		}

		// if the follow url can be found in the entry list
		for(i = 0; i < len; i++)
		{
			m_pResult = (CQueryResult *) m_queryResults->get(i);

			CStringArray& m_titles = m_pResult->getTitles();
			for(j = 0; j < m_titles.GetSize(); j++)
			{
				if(state[i] == 0 || state[i] == 2)
					InsertWordSearch(m_wordSearch, m_titles[j], state[i], -1);
				else
				{
					if(pQuery->m_followURL.GetSize() > 0)
					{
						for(k = 0; k < pQuery->m_followURL.GetSize(); k++)
							InsertWordSearch(m_wordSearch, m_titles[j], 1, -1);

						for(k = 0; k < pQuery->m_followURL.GetSize()-1; k++)
							InsertWordSearch(m_wordSearch, m_titles[j], 3, -1);
					}
				}
			}

			CStringArray& m_snippets = m_pResult->getSnippets();
			for(j = 0; j < m_snippets.GetSize(); j++)
			{

				if(state[i] == 0 || state[i] == 2)
					InsertWordSearch(m_wordSearch, m_snippets[j], -1, state[i]);
				else
				{
					if(pQuery->m_followURL.GetSize() > 0)
					{
						for(k = 0; k < pQuery->m_followURL.GetSize(); k++)
							InsertWordSearch(m_wordSearch, m_snippets[j], -1, 1);

						for(k = 0; k < pQuery->m_followURL.GetSize()-1; k++)
							InsertWordSearch(m_wordSearch, m_snippets[j], -1, 3);
					}
				}
			}
		}

		CStringArray& m_keywords = pPage->getQueryWords();
		for(j = 0; j < m_keywords.GetSize(); j++)
		{
			for(k = 0 ; k < pQuery->m_queryCnt; k++)
				InsertWordSearch(m_wordSearch, m_keywords[j], -1, -1, true);
		}
	}
}

// Merge the word feature from sequence, and search result
// and calculate the browsing features for every word
void CFeatureExtraction::Merge(Hash *m_gWords, CItemSet *seqState, Hash *m_searchWords, Hash *m_sessionWords)
{
    if((m_gWords == NULL && m_searchWords == NULL) ||
        (m_gWords != NULL && m_searchWords != NULL && (m_gWords->size() == 0 && m_searchWords->size() == 0)))
        return;

    CWordSearch *pWQuery;
    CWordSession *pWSession, *pws;
    CWordSeq *pWSeq;

    CArray<double, double&> weightTFIDF;
	CArray<double, double&> backwardTFIDF;
	CArray<double, double&> forwardTFIDF;

    CArray<double, double&> weightJewell;
	CArray<double, double&> backwardJewell;
	CArray<double, double&> forwardJewell;

	double weight, sumIDF, avIDF, idf_weight, maxSeqTFIDF, minSeqTFIDF;
	int cntOccurence, cntBackward, cntForward;
    int i, latestPos, numWords, count;
	CTextProcessor *pTextProcessor;

	// first of all, let's grap all the words that in search pages
    if(m_searchWords != NULL && m_searchWords->size() > 0)
    {
		m_searchWords->resetSeek();
		while((pWQuery = (CWordSearch *)m_searchWords->getObject()) != NULL)
        {
            pWSession = (CWordSession *)new CWordSession();

    		pWSession->Word(pWQuery->Word());

	    	pWSession->isKeywordCnt = pWQuery->isKeywordCnt;

		    pWSession->skippedTitleCnt = pWQuery->skippedTitleCnt;
    		pWSession->skippedSnippetCnt = pWQuery->skippedSnippetCnt;

	    	pWSession->chosenTitleCnt = pWQuery->chosenTitleCnt;
		    pWSession->chosenSnippetCnt = pWQuery->chosenSnippetCnt;
    
    		pWSession->untouchedTitleCnt = pWQuery->untouchedTitleCnt;
	    	pWSession->untouchedSnippetCnt = pWQuery->untouchedSnippetCnt;

    		pWSession->bkTitleCnt = pWQuery->bkTitleCnt;
	    	pWSession->bkSnippetCnt = pWQuery->bkSnippetCnt;

            m_sessionWords->insert(pWSession);
        }
    }


    if(m_gWords == NULL || m_gWords->size() == 0)
        return;

	// count how many words in the whole sequence
	// and at the same time, we get the information for 
	// each word's IDF weight, and compute average IDF
	// weight as the IDF weight for these words without IDF
	numWords = 0;
	sumIDF = 0.0;
	count = 0;

	pTextProcessor = m_pageRepository->m_envTextProcessor;

	m_gWords->resetSeek();
	while((pWSeq = (CWordSeq *)m_gWords->getObject()) != NULL)
	{
		numWords += pWSeq->frequency;

		if(pTextProcessor)
		{
			idf_weight = pTextProcessor->lookupIDF(pWSeq->getWord());
			if(idf_weight != -1.0)
			{
				sumIDF += idf_weight;
				++count;
			}
		}
	}

	avIDF = 0.0;
	if(count > 0)
		avIDF = sumIDF/(double)count;

	// now for the words in sequential pages
	m_gWords->resetSeek();

	maxSeqTFIDF = -9999.0;
	minSeqTFIDF = 9999.0;
	while((pWSeq = (CWordSeq *)m_gWords->getObject()) != NULL)
    {
        pWSession = (CWordSession *)new CWordSession();
        pWSession->Word(pWSeq->getWord());

        pWSession->relativeFreq = (double)(pWSeq->frequency)/(double)(m_gWords->size());
		pWSession->ratioOccurences = (double)(pWSeq->frequency)/(double)numWords;

		if(pTextProcessor)
		{
			idf_weight = pTextProcessor->lookupIDF(pWSeq->getWord());
			if(idf_weight != -1.0)
				pWSession->seqTFIDFWeight = (double)(pWSeq->frequency) * idf_weight;
			else
			{
				if(avIDF > 0.0)
					pWSession->seqTFIDFWeight = (double)(pWSeq->frequency) * avIDF;
			}
		}

		if(maxSeqTFIDF <= pWSession->seqTFIDFWeight)
			maxSeqTFIDF = pWSession->seqTFIDFWeight;

		if(minSeqTFIDF >= pWSession->seqTFIDFWeight)
			minSeqTFIDF = pWSession->seqTFIDFWeight;


        pWSession->absFreq = pWSeq->frequency;

        cntOccurence = 0;
        cntBackward = 0;
        cntForward = 0;

		// keep record the latest location of the word in the sequence
		latestPos = -1;

        for(i = 0; i < pWSeq->length; i++)
        {
            if(pWSeq->weightTFIDF[i] > 0.0 || pWSeq->weightJewell[i] > 0.0)
            {
                ++cntOccurence;
                if(seqState->get(i) == -1)
                    ++cntBackward;
                if(seqState->get(i) == 1)
                    ++cntForward;

				latestPos = i;
            }
        }

		// get the relative latest position of the word
		// in the whole sequence
		if(latestPos >= 0 && pWSeq->length > 0)
		{
			pWSession->latestAppearance = (double)(latestPos+1)/(double)pWSeq->length;
		}

		if(pWSeq->length)
			pWSession->ratioWordAppearance = (double)cntOccurence/(double)pWSeq->length;
        
		if(cntOccurence)
		{
			pWSession->ratioBackward = (double)cntBackward/(double)cntOccurence;
		    pWSession->ratioForward = (double)cntForward/(double)cntOccurence;
		}

		weightTFIDF.RemoveAll();
		backwardTFIDF.RemoveAll();
		forwardTFIDF.RemoveAll();

		weightJewell.RemoveAll();
		backwardJewell.RemoveAll();
		forwardJewell.RemoveAll();

        for(i = 0; i < pWSeq->length; i++)
        {
            if((weight = pWSeq->weightTFIDF[i]) > 0.0)
			{
				weightTFIDF.Add(weight);

				if(seqState->get(i) == -1)
					backwardTFIDF.Add(weight);

				if(seqState->get(i) == 1)
					forwardTFIDF.Add(weight);
			}

            if((weight = pWSeq->weightJewell[i]) > 0.0)
			{
				weightJewell.Add(weight);

				if(seqState->get(i) == -1)
					backwardJewell.Add(weight);

				if(seqState->get(i) == 1)
					forwardJewell.Add(weight);
			}
        }

        Utility::CalAveVar(weightTFIDF, pWSession->avTFIDFWeight, pWSession->varTFIDFWeight);
		Utility::CalAveVar(backwardTFIDF, pWSession->avTFIDFWeightBackward, pWSession->varTFIDFWeightBackward);
        Utility::CalAveVar(forwardTFIDF, pWSession->avTFIDFWeightForward, pWSession->varTFIDFWeightForward);


        Utility::CalAveVar(weightJewell, pWSession->avJewellWeight, pWSession->varJewellWeight);
		Utility::CalAveVar(backwardJewell, pWSession->avJewellWeightBackward, pWSession->varJewellWeightBackward);
        Utility::CalAveVar(forwardJewell, pWSession->avJewellWeightForward, pWSession->varJewellWeightForward);

		// identify the trend of the word's weight across the whole sequence
        pWSession->trendTFIDFWeight = TrendIdentification(pWSeq->weightTFIDF, pWSeq->length);
        pWSession->trendJewellWeight = TrendIdentification(pWSeq->weightJewell, pWSeq->length);

		// inFromPages records all the page No. that come from the hyperlink
		// that contains this word (in anchor text)
        if(pWSeq->inHyperlinkCnt > 0)
            pWSession->ratioLinkFollow = (double)pWSeq->inFromPages->size()/(double)pWSeq->inHyperlinkCnt;
        if(seqState->size())
			pWSession->ratioFollow = (double)pWSeq->inFromPages->size()/(double)seqState->size();

        if(pWSeq->inFromPages->size() > 0)
        {
            int backFollow = 0;
            for(i = 0; i < pWSeq->inFromPages->size(); i++)
            {
                if(seqState->get(pWSeq->inFromPages->get(i)) == -1)
                    ++backFollow;
            }

            if(backFollow > 0)
                pWSession->ratioLinkBack = (double)backFollow/(double)pWSeq->inFromPages->size();
        }

		if(cntOccurence)
		{
	        pWSession->ratioInTitle = (double)pWSeq->inTitleCnt/(double)cntOccurence;
		    pWSession->ratioInvisible = (double)pWSeq->inMetaCnt/(double)cntOccurence;
		}

		weightTFIDF.RemoveAll();
		backwardTFIDF.RemoveAll();
		forwardTFIDF.RemoveAll();

		weightJewell.RemoveAll();
		backwardJewell.RemoveAll();
		forwardJewell.RemoveAll();

        // look up the word in the Hash table, if we already have the word,
		// it means that the word has search features, so we just copy the
		// sequential features into the WordSession,
        // otherwise, insert it into the Hash table
        if(m_sessionWords->size() <= 0)
            m_sessionWords->insert(pWSession);
        else if((pws = (CWordSession *)m_sessionWords->lookup(pWSession)) != NULL)
        {
            pws->latestAppearance = pWSession->latestAppearance;

            pws->relativeFreq = pWSession->relativeFreq;
            pws->ratioOccurences = pWSession->ratioOccurences;
            pws->seqTFIDFWeight = pWSession->seqTFIDFWeight;
            pws->absFreq = pWSession->absFreq;

            pws->ratioWordAppearance = pWSession->ratioWordAppearance;

            pws->avTFIDFWeight = pWSession->avTFIDFWeight;
            pws->varTFIDFWeight = pWSession->varTFIDFWeight;

            pws->avJewellWeight = pWSession->avJewellWeight;
            pws->varJewellWeight = pWSession->varJewellWeight;

            pws->trendTFIDFWeight = pWSession->trendTFIDFWeight;
            pws->trendJewellWeight = pWSession->trendJewellWeight;

            pws->ratioLinkFollow = pWSession->ratioLinkFollow;
            pws->ratioFollow = pWSession->ratioFollow;
            pws->ratioLinkBack = pWSession->ratioLinkBack;

            pws->ratioBackward = pWSession->ratioBackward;

            pws->avTFIDFWeightBackward = pWSession->avTFIDFWeightBackward;
            pws->varTFIDFWeightBackward = pWSession->varTFIDFWeightBackward;

            pws->avJewellWeightBackward = pWSession->avJewellWeightBackward;
            pws->varJewellWeightBackward = pWSession->varJewellWeightBackward;

            pws->ratioForward = pWSession->ratioForward;

            pws->avTFIDFWeightForward = pWSession->avTFIDFWeightForward;
            pws->varTFIDFWeightForward = pWSession->varTFIDFWeightForward;

            pws->avJewellWeightForward = pWSession->avJewellWeightForward;
            pws->varJewellWeightForward = pWSession->varJewellWeightForward;
			
            pws->ratioInTitle = pWSession->ratioInTitle;

            pws->ratioInvisible = pWSession->ratioInvisible;

            delete pWSession;
        }
        else
        {
            m_sessionWords->insert(pWSession);
        }
    }

	if(maxSeqTFIDF > minSeqTFIDF)
	{
		// normalize the seqTFIDF
		m_sessionWords->resetSeek();
		while((pWSession = (CWordSession *)m_sessionWords->getObject()) != NULL)
			pWSession->seqTFIDFWeight = (pWSession->seqTFIDFWeight-minSeqTFIDF)/(maxSeqTFIDF-minSeqTFIDF);
	}
}

// get all the words in IC-page which are IC-Words
void CFeatureExtraction::getDestinationWords(CPageView *m_viewDestination, Hash *m_destWords)
{
	CPageCache *pPage;
	CPageFrame *pFrame;

	if(!m_bICQueryModel)
	{
		// get the page cache, and insert all its words into the hash table
		pPage = getPage(m_viewDestination->getPageno());
		if(pPage)
			insertDestinationWords(pPage, m_destWords);

		// process each of its child frame
		if(m_viewDestination->m_framepages)
		{
			for(int iframe = 0; iframe < m_viewDestination->m_framepages->size(); iframe++)
			{
				pFrame = (CPageFrame *)m_viewDestination->m_framepages->get(iframe);

				pPage = getPage(pFrame->pageno);
				if(pPage)
					insertDestinationWords(pPage, m_destWords);
			}
		}
	}
	else
	{
		// for IC-query model, we calculate the score based on the word's 
		// page features, and only take top 10 words as IC-query word set
		CStringArray m_stemWords;
		CArray<double, double&> m_icProbs;

		m_stemWords.RemoveAll();
		m_icProbs.RemoveAll();

		pPage = getPage(m_viewDestination->getPageno());
		if(pPage)
			insertDestinationWords(pPage, m_stemWords, m_icProbs);

		if(m_viewDestination->m_framepages)
		{
			for(int iframe = 0; iframe < m_viewDestination->m_framepages->size(); iframe++)
			{
				pFrame = (CPageFrame *)m_viewDestination->m_framepages->get(iframe);

				pPage = getPage(pFrame->pageno);
				if(pPage)
					insertDestinationWords(pPage, m_stemWords, m_icProbs);
			}
		}

		// sort the words based on their score
		Utility::quickSort(m_stemWords, m_icProbs, 0, m_icProbs.GetSize() - 1);

		for(int i = 0; (i < 10 && i < m_stemWords.GetSize()); i++)
			insertDestinationWord(m_stemWords[i], m_destWords);

		m_stemWords.RemoveAll();
		m_icProbs.RemoveAll();
	}


}

// insert all the words in m_pPage into a hash table which will be used to generate
// the session data
void CFeatureExtraction::insertDestinationWords(CPageCache *m_pPage, Hash *m_destWords)
{
	Hash *m_allWords;
	CPageCacheWord *pDestWord;

	m_allWords = m_pPage->getAllWords();
	m_allWords->resetSeek();
	while((pDestWord = (CPageCacheWord *)m_allWords->getObject()) != NULL)
		insertDestinationWord(pDestWord->getWord(), m_destWords);
}


// get all the words from the page cache, and compute the weight of each word
// sort them, only output the top 10 words
void CFeatureExtraction::insertDestinationWords(CPageCache *m_pPage, CStringArray &m_stemWords, CArray<double, double&> &m_icProbs)
{
	Hash *hWords;
	double score;

	hWords = m_pPage->getAllWords();
	if(hWords == NULL || hWords->size() <= 0)
		return;

	CPageCacheWord *pWord;

	// Here, we deal with each word in the page
	hWords->resetSeek();
	while( (pWord = (CPageCacheWord *)hWords->getObject()) != NULL)
	{
		score = pWord->JewellScore();
		addDestinationStemWord(pWord->getWord(), score, m_stemWords, m_icProbs);
	}
}


void CFeatureExtraction::addDestinationStemWord(const CString &stemWord, double score, CStringArray &m_stemWords, CArray<double, double&> &m_icProbs)
{
	bool existed;
	int pos;
	double sum;

	existed = false;
	for(int i = 0; i < m_stemWords.GetSize(); i++)
	{
		if(stemWord.Compare(m_stemWords[i]) == 0)
		{
			pos = i;
			existed = true;
			break;
		}
	}

	if(existed)
	{
		sum = score + m_icProbs[pos];
		m_icProbs.SetAt(pos, sum);
	}
	else
	{
		m_stemWords.Add(stemWord);
		m_icProbs.Add(score);
	}
}


// insert one stem word into the word hash table
void CFeatureExtraction::insertDestinationWord(const CString &stemWord, Hash *m_destWords)
{
	CLilacString *pWord, *pLocator;

	pWord = (CLilacString *) new CLilacString(stemWord);
	if((pLocator = (CLilacString *) m_destWords->lookup(pWord)) == NULL)
		m_destWords->insert(pWord);
	else
		delete pWord;
}



void CFeatureExtraction::InsertWordSearch(Hash *m_hWord, const char *pword, int tstate, int sstate, bool keyword)
{
	CWordSearch *ptank;
	CWordSearch *pt;

	ptank = (CWordSearch *)new CWordSearch();
	ptank->Word(pword);

	if((pt = (CWordSearch *)m_hWord->lookup(ptank)) != NULL)
	{
		if(tstate >= 0)
		{
			switch (tstate) {
			case 0:
				pt->untouchedTitleCnt += 1;
				break;
			case 1:
				pt->chosenTitleCnt += 1;
				break;
			case 2:
				pt->skippedTitleCnt += 1;
				break;
			case 3:
				pt->chosenTitleCnt += 1;
				pt->bkTitleCnt += 1;
				break;
			}
		}

		if(sstate >= 0)
		{
			switch (sstate) {
			case 0:
				pt->untouchedSnippetCnt += 1;
				break;
			case 1:
				pt->chosenSnippetCnt += 1;
				break;
			case 2:
				pt->skippedSnippetCnt += 1;
				break;
			case 3:
				pt->chosenSnippetCnt += 1;
				pt->bkSnippetCnt += 1;
				break;
			}

		}

		if(keyword)
			pt->isKeywordCnt += 1;

		delete ptank;
	}
	else
	{
		if(tstate > 0)
		{
			switch (tstate) {
			case 0:
				ptank->untouchedTitleCnt = 1;
				break;
			case 1:
				ptank->chosenTitleCnt = 1;
				break;
			case 2:
				ptank->skippedTitleCnt = 1;
				break;
			case 3:
				ptank->chosenTitleCnt = 1;
				ptank->bkTitleCnt = 1;
				break;
			}
		}

		if(sstate > 0)
		{
			switch (sstate) {
			case 0:
				ptank->untouchedSnippetCnt = 1;
				break;
			case 1:
				ptank->chosenSnippetCnt = 1;
				break;
			case 2:
				ptank->skippedSnippetCnt = 1;
				break;
			case 3:
				ptank->chosenSnippetCnt = 1;
				ptank->bkSnippetCnt = 1;
				break;
			}

		}

		if(keyword)
			ptank->isKeywordCnt = 1;

		m_hWord->insert(ptank);
	}

	return;
}


// calculate the trend of the word's weight across the whole sequence,
// we split the whole sequence into left and right part, and compute
// the sum of both sides
int CFeatureExtraction::TrendIdentification(double *weight, int len)
{
    int i, left = 0, right = 0, mid;
    double avleft, sumleft, avright, sumright;

    sumleft = 0.0;
    sumright = 0.0;

    mid = len/2;
    if(len%2 == 0)
    {
        for(i = 0; i < mid; i++)
        {
            if(weight[i] > 0)
            {
                sumleft += weight[i];
                ++left;
            }
        }

        for(i = mid; i < len; i++)
        {
            if(weight[i] > 0)
            {
                sumright += weight[i];
                ++right;
            }
        }
    }
    else
    {
        for(i = 0; i < mid; i++)
        {
            if(weight[i] > 0)
            {
                sumleft += weight[i];
                ++left;
            }
        }

        for(i = mid+1; i < len; i++)
        {
            if(weight[i] > 0)
            {
                sumright += weight[i];
                ++right;
            }
        }
    }

    if(left > right)
        return -1;
    else if(left < right)
        return 1;
    else
    {
        if(left == 0 && right == 0)
            return 0;

        avleft = sumleft/(double)left;
        avright = sumright/(double)right;
        if(avleft > avright)
            return -1;
        else if(avleft < avright)
            return 1;
        else
            return 0;
    }
}

///////////////////////////////////////////////////////////////
// Extractor.h: interface for the Extractor class.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_EXTRACTOR_H__1BD09132_912E_4550_9B61_15386A0B9261__INCLUDED_)
#define AFX_EXTRACTOR_H__1BD09132_912E_4550_9B61_15386A0B9261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "List.h"
#include "PageRepository.h"
#include "FeatureExtraction.h"
#include "SessionIdentification.h"




// The usage of the Extractor Class
// 1. Set the environment
//    setSession, setType, and then setRepository
// 2. Call Exec() to do the session identification and 
//    feature extraction.
// 3. Get the results from m_ICSessions;

class Extractor  
{
public:
	Extractor();
	virtual ~Extractor();

	void setSession(List * pSession)		{	m_pSession = pSession;	}
	void setType(int value = 0)
	{
		m_type = value;
		m_featureExtractor->setType(m_type);
	}

	void setRepository(CPageRepository *pRepository)
	{
		m_pageRepository = pRepository;
		m_sessionIdentifier->m_pageRepository = m_pageRepository;
		m_featureExtractor->setRepository(m_pageRepository);
	}

	// run the IC-session Identification and feature extraction
	void Exec();

	int getSessionNum()
	{
		if(m_ICSessions)
			return m_ICSessions->size();

		return -1;
	}

	List *getSessionWords(int index = 0)
	{
		if(m_ICSessions->size() <= 0)
			return NULL;

		CICSession *pSession;
		pSession = (CICSession *) m_ICSessions->get(index);
		return pSession->m_wordVectors;
	}

	void getDestinationWords(CPageView *m_viewDestination, Hash *m_destWords)
	{
		m_featureExtractor->getDestinationWords(m_viewDestination, m_destWords);
	}

	void clearSessionWords()
	{
		if(m_featureExtractor)
			m_featureExtractor->clearSessionWords();
	}

	// enable the IC-query model, which will only use top 10 words as IC-query word set
	void enableICQueryModel(bool value = true)
	{
		m_featureExtractor->enableICQueryModel(value);
	}

protected:

	CSessionIdentification *m_sessionIdentifier;
	CFeatureExtraction *m_featureExtractor;

private:
	void genICSessions();

	// input :
	//         the page repository
	//         the page sequence
	CPageRepository *m_pageRepository;
	List *m_pSession;

	// 0 : the input session is a IC-session, which means
	//     its last page is an IC-page.
	// 1 : all the pages in the input session will be
	//     involved in the feature extraction.
	     
	int m_type;

public:
	void extractFeature(CICSession *pICSession);
	CICSession * genLatestICSession();

	// List of all IC-sessions
	List *m_ICSessions;
};

#endif // !defined(AFX_EXTRACTOR_H__1BD09132_912E_4550_9B61_15386A0B9261__INCLUDED_)

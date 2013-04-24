///////////////////////////////////////////////////////////////
// Extractor.cpp: implementation of the Extractor class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Extractor.h"
#include "PageCache.h"
#include "WordSeq.h"
#include "SearchPage.h"
#include "WordSearch.h"
#include "WordSession.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Extractor::Extractor()
{
	m_pSession = NULL;
	m_sessionIdentifier = (CSessionIdentification *) new CSessionIdentification();
	m_featureExtractor = (CFeatureExtraction *) new CFeatureExtraction();

	m_ICSessions = (List *) new List();
}

Extractor::~Extractor()
{
	delete m_sessionIdentifier;
	delete m_featureExtractor;
	delete m_ICSessions;
}


// run IC-session identification and feature
// extraction, at first we identify all IC-session
// from the given page sequence, then extract 
// browsing features for each IC-session
void Extractor::Exec()
{
	genICSessions();

	clearSessionWords();

	// then for each IC-session, do the feature extraction
	for(int i = 0; i < m_ICSessions->size(); i++)
	{
		CICSession *pSession;
		pSession = (CICSession *) m_ICSessions->get(i);

		if(pSession->isICSession())
			extractFeature(pSession);
	}
}



// get the latest IC-Session which will be used for recommendation
// generation
CICSession * Extractor::genLatestICSession()
{
	genICSessions();

	if(m_ICSessions->size() > 0)
		return (CICSession *) m_ICSessions->get(m_ICSessions->size()-1)->clone();

	return NULL;
}


// Identify IC-Sessions
void Extractor::genICSessions()
{
	// clear the previous IC-Session list
	m_ICSessions->clear();

	// if there is nothing we can do, just return
    if(m_pSession == NULL || m_pSession->size() <= 2)
        return;

	// at first, let's do session identification
	// for on-line recommendation, since there is
	// no annotation, just take all he pages into a
	// session. But if there is annotation within 
	// the sequence, then identify the IC-Session
	// at first, and all the IC-Sessions are fan-out
	// by m_ICSessions 
	m_sessionIdentifier->m_pageviews = m_pSession;
	m_sessionIdentifier->m_ICSessions = m_ICSessions;
	m_sessionIdentifier->Exec();
}


// extract the feature vector for each involved word
void Extractor::extractFeature(CICSession *pICSession)
{
	if(m_featureExtractor)
	{
		m_featureExtractor->setType(m_type);
		m_featureExtractor->setSession(pICSession);
		m_featureExtractor->Exec();
	}
}

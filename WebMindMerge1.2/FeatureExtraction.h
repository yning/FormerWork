///////////////////////////////////////////////////////////////
// FeatureExtraction.h: interface for the CFeatureExtraction class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_FEATUREEXTRACTION_H__B77270E7_3515_4F56_8A35_9A75D7A5DB3F__INCLUDED_)
#define AFX_FEATUREEXTRACTION_H__B77270E7_3515_4F56_8A35_9A75D7A5DB3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "List.h"
#include "Hash.h"

#include "PageView.h"
#include "ItemSet.h"
#include "PageRepository.h"
#include "ICSession.h"


// How to use the FeatureExtraction Class
// 1. Set up the environment
//    setSession : point to the IC-session
//    setType
//    setRepository
//
// 2. Call Exec()
//


class CFeatureExtraction  
{
public:
	CFeatureExtraction();
	virtual ~CFeatureExtraction();

	void setSession(CICSession *pSession)
	{
		m_pICSession = pSession;
		m_type = m_pICSession->isICSession()?0:1;
	}

	void setType(int value = 0)				{	m_type = value;			}

	void setRepository(CPageRepository *pRepository)
	{
		m_pageRepository = pRepository;
	}

	void Exec();

	int FindLocation(const CStringArray &srcArray, const CString &strWord);

	Hash *getSessionWords()					{	return m_sessionWords;	}
	Hash *getDestWords()					{	return m_destWords;		}

	void getDestinationWords(CPageView *m_viewDestination, Hash *m_destWords);

	void clearSessionWords()
	{
		if(m_sessionWords)
			m_sessionWords->clear();
	}

	void enableICQueryModel(bool value = true)
	{
		m_bICQueryModel = value;
	}

protected:
	void genBrowsingBehavior(List *m_pSession);
	void genWordVectors(List *pSession, Hash *m_sessionWords, Hash *m_destWords);

	void SequentialState(List *pSession, CItemSet *stateSeq);
	CPageCache *getPage(int pageno);

	void PageProcess(int pageno, CPageCache *pPage, CItemSet *seqState, 
					const CString& m_urlFollow, Hash *m_hgwords);

	void DoSearchQuery(List *pSession, Hash *m_wordSearch);
	void InsertWordSearch(Hash *m_hWord, const char *pword, int tstate = -1, int sstate = -1, bool keyword = false);

	void Merge(Hash *m_gWords, CItemSet *seqState, Hash *m_searchWords, Hash *m_sessionWords);

	void insertDestinationWords(CPageCache *m_pPage, Hash *m_destWords);
	void insertDestinationWord(const CString &stemWord, Hash *m_destWords);

	void insertDestinationWords(CPageCache *m_pPage, CStringArray &m_stemWords, CArray<double, double&> &m_icProbs);
	void addDestinationStemWord(const CString &stemWord, double score, CStringArray &m_stemWords, CArray<double, double&> &m_icProbs);

	int TrendIdentification(double *weight, int len);

private:

	CPageRepository *m_pageRepository;

	CICSession *m_pICSession;

	// 0 : the input session is a IC-session, which means
	//     its last page is an IC-page.
	// 1 : all the pages in the input session will be
	//     involved in the feature extraction.
	     
	int m_type;

	Hash *m_sessionWords;
	Hash *m_destWords;

	bool m_bICQueryModel;
};



#endif // !defined(AFX_FEATUREEXTRACTION_H__B77270E7_3515_4F56_8A35_9A75D7A5DB3F__INCLUDED_)

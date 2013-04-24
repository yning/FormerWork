// QueryLearner.cpp: implementation of the CQueryLearner class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QueryLearner.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "Shlwapi.h"
#include "C45Learner.h"


CRuleSet::CRuleSet()
{
	m_ruleSet.RemoveAll();
}

CRuleSet::CRuleSet(const CRuleSet& src)
{
	m_ruleSet.RemoveAll();
	for(int i = 0; i < src.m_ruleSet.GetSize(); i++)
		m_ruleSet.Add(src.m_ruleSet[i]);
}

CRuleSet::~CRuleSet()
{
	m_ruleSet.RemoveAll();
}


// append the rule set from src to the current 
void CRuleSet::append(const CRuleSet& src)
{
	for(int i = 0; i < src.m_ruleSet.GetSize(); i++)
		m_ruleSet.Add(src.m_ruleSet[i]);
}

int CRuleSet::serialize(CArchive& pArchive)
{
	int i, size;

	// Serialize the object ...
	if (pArchive.IsStoring())
	{
		pArchive << m_ruleSet.GetSize();
		for(i = 0; i < m_ruleSet.GetSize(); i++)
			m_ruleSet[i].serialize(pArchive);
	}
	else
	{
		CRuleRec pRule;

		m_ruleSet.RemoveAll();

		pArchive >> size;
		for(i = 0; i < size; i++)
		{
			pRule.serialize(pArchive);
			m_ruleSet.Add(pRule);
		}
	}

	return 0;
}


// save the rule sets into a local file
void CRuleSet::save(const CString &strFile)
{
	CFile m_wFile( strFile, CFile::modeCreate | CFile::modeWrite);
	CArchive m_wArch( &m_wFile, CArchive::store );

	serialize(m_wArch);

	m_wArch.Flush();
	m_wArch.Close();
	m_wFile.Close();
}


// load the rule sets from a local file
void CRuleSet::load(const CString &strFile)
{
	if(!PathFileExists(strFile))
		return;

	CFile m_rFile( strFile, CFile::modeRead);
	CArchive m_rArch( &m_rFile, CArchive::load );

	serialize(m_rArch);

	m_rArch.Close();
	m_rFile.Close();
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQueryLearner::CQueryLearner()
{
	m_strVersion = _T("0.99999999");
	m_dataSet = NULL;
}

CQueryLearner::~CQueryLearner()
{
}

// Serialize the object ...
int CQueryLearner::serialize(CArchive& pArchive)
{
	int len;

	// Serialize the object ...
	if (pArchive.IsStoring())
	{
		len = m_strVersion.GetLength();
		pArchive << len;
		if(len)
			pArchive << m_strVersion;

		m_ruleSet.serialize(pArchive);
	}
	else
	{
		m_strVersion.Empty();
		pArchive >> len;
		if(len)
			pArchive >> m_strVersion;

		m_ruleSet.serialize(pArchive);
	}

	return 0;
}


// save the trained learner
void CQueryLearner::Save(const char *m_fileName)
{
	CFile m_wFile( m_fileName, CFile::modeCreate | CFile::modeWrite);
	CArchive m_wArch( &m_wFile, CArchive::store );

	serialize(m_wArch);

	m_wArch.Close();
	m_wFile.Close();

	return;
}

// load the learner
void CQueryLearner::Load(const char *m_fileName)
{
	CFile m_rFile( m_fileName, CFile::modeRead);
	CArchive m_rArch( &m_rFile, CArchive::load );

	serialize(m_rArch);

	m_rArch.Close();
	m_rFile.Close();

	return;
}


// build the learner
void CQueryLearner::buildLearner()
{
	if(!m_dataSet)
		return;

	C45Learner *pC45Classifier;

	// export the learner
	pC45Classifier = (C45Learner *) new C45Learner();
    pC45Classifier->setDataSet(m_dataSet);
	pC45Classifier->buildClassifier();

	pC45Classifier->getQueryRules(m_ruleSet);

	delete pC45Classifier;

	return;
}

// predict the search query
void CQueryLearner::Predict()
{
	if(!m_dataSet)
		return;

	double maxScore, score;
	CArray<int, int&>tmpWords;
	bool bQuery;

	m_queryWords.RemoveAll();

	maxScore = 0.0;
	// go through each rule, and whenever we find four words that
	// meet the rule, we will exit; otherwise return the words
	// with the highest score
	for(int i = 0; i < m_ruleSet.GetSize(); i++)
	{
		score = searchWords(m_ruleSet.m_ruleSet[i], bQuery, tmpWords);
		if(tmpWords.GetSize() == 0)
			continue;

		if(bQuery)
		{
			m_queryWords.RemoveAll();
			m_queryWords.Append(tmpWords);
			break;
		}

		if(score >= maxScore)
		{
			maxScore = score;

			m_queryWords.RemoveAll();
			m_queryWords.Append(tmpWords);
		}
	}

	// *debug*
	//for(i = 0; i < m_queryWords.GetSize(); i++)
	//	m_LogTrace.WriteLine(" %d ", m_queryWords[i]);
	//m_LogTrace.WriteLine("\n\n");
}

bool CQueryLearner::inList(int wn, CArray<int, int&> &tmpWords)
{
	for(int i = 0;i < tmpWords.GetSize(); i++)
	{
		if(wn == tmpWords[i])
			return true;
	}

	return false;
}



// search the words that could be used in the query
double CQueryLearner::searchWords(CRuleRec& conRules, bool& bQueryWords, CArray<int, int&> &tmpWords)
{
	tmpWords.RemoveAll();
	if(conRules.GetSize() == 0 || m_dataSet->case_num == 0)
		return 0.0;

	CArray<double, double&>canWords[NUM_QUERY_WORD];
	int i, j;
	double fScore;
	bool bQuery;
	for(i = 0; i < m_dataSet->case_num; i++)
	{
		for(j = 0; j < NUM_QUERY_WORD; j++)
		{
			bQuery = checkWord(i, conRules, j, fScore);
			fScore += bQuery?100.0:0.0;
			canWords[j].Add( fScore );
		}
	}

	// we set a threshold for the number of query words
	int cntQueryWords, cntWord;
	cntQueryWords = (int)((double)m_dataSet->case_num * RATIO_QUERY_WORD);

	for(i = 0; i < NUM_QUERY_WORD; i++)
	{
		cntWord = 0;
		for(j = 0; j < canWords[i].GetSize(); j++)
		{
			if(canWords[i][j] > 100.0)
				cntWord++;
		}

		// *debug*
		//m_LogTrace.WriteLine(" %d of words satisfy the query words!\n", cntWord);

		if(cntWord >= cntQueryWords)
			return 0.0;
	}



	// construct the search query
	
	CStringArray wordNo;
	CArray<double, double&>wordScore;
	CString strBuffer;
	double sumScore;
	int pword;
	

	bQueryWords = true;
	sumScore = 0.0;
	for(i = 0; i < NUM_QUERY_WORD; i++)
	{
		wordNo.RemoveAll();
		wordScore.RemoveAll();

		for(j = 0; j < canWords[i].GetSize(); j++)
		{
			if(!inList(j, tmpWords))
			{
				strBuffer.Empty();
				strBuffer.Format("%d", j);
				wordNo.Add(strBuffer);

				wordScore.Add(canWords[i][j]);
			}
		}

		Utility::quickSort(wordNo, wordScore, 0, wordNo.GetSize()-1);
		pword = atoi(wordNo[0]);
		tmpWords.Add(pword);

		if(wordScore[0] < 100.0)
			bQueryWords = false;

		sumScore += wordScore[0];
	}

	return sumScore;
}

// check whether the word meet the condition of the specification of the
// n-th word in the query
bool CQueryLearner::checkWord(int posWord, CRuleRec& conRules, int posQuery, double& sumScore)
{
	bool bMeet;
	double fScore;

	sumScore = 0.0;
	bMeet = true;
	for(int i = 0; i < conRules.GetSize(); i++)
	{
		if(conRules.m_ruleConditions[i].m_iAttribute/NUM_ATTRIBUTE == posQuery)
		{
			if(!meetCondition(posWord, conRules.m_ruleConditions[i], fScore))
			{
				bMeet = false;
				sumScore -= fScore;
			}
			else
			{
				sumScore += fScore;
			}
		}
	}

	return bMeet;
}


// compute the score of the word according to the browsing feature and
// the cut value. 
// I hope that the more the browsing feature satisfies the cut value, the higher
// the score
// check the word for each rule condition
bool CQueryLearner::meetCondition(int posWord, CRuleCondition& conRules, double& fScore)
{
	double valFeature;
	int iAttribute;

	iAttribute = conRules.m_iAttribute%NUM_ATTRIBUTE;
	valFeature = m_dataSet->m_data[posWord][iAttribute];

	switch (conRules.m_iRelation)
	{
	case 0:
		fScore = fabs(valFeature - conRules.m_cutMin);
		if(valFeature == conRules.m_cutMin)
			return true;
	case -1:
		fScore = fabs(valFeature - conRules.m_cutMin);
		if(valFeature <= conRules.m_cutMax)
			return true;
	case 1:
		fScore = fabs(valFeature - conRules.m_cutMin);
		if(valFeature > conRules.m_cutMin)
			return true;
	case 2:
		if(valFeature > conRules.m_cutMin && valFeature <= conRules.m_cutMax)
		{
			fScore = max(fabs(valFeature - conRules.m_cutMin), fabs(valFeature - conRules.m_cutMax));
			return true;
		}
		else
		{
			fScore = min(fabs(valFeature - conRules.m_cutMin), fabs(valFeature - conRules.m_cutMax));
		}
	};

	return false;
}

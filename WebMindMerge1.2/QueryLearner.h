// QueryLearner.h: interface for the CQueryLearner class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUERYLEARNER_H__CD857DE9_C694_481F_B502_8026D074E86D__INCLUDED_)
#define AFX_QUERYLEARNER_H__CD857DE9_C694_481F_B502_8026D074E86D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include <afxtempl.h>

#include "DataSet.h"
#include "Utility.h"

#define RATIO_QUERY_WORD 0.20

class CRuleCondition :  public CObject
{
public:

	CRuleCondition()
	{
		m_type = 0;
		m_iAttribute = -1;
		m_strAttribute.Empty();
		m_iRelation = 0;
		m_cutMin = 0.0;
		m_cutMax = 0.0;
	}

	CRuleCondition(const CRuleCondition& src)
	{
		m_type = src.m_type;
		m_iAttribute = src.m_iAttribute;
		m_strAttribute = src.m_strAttribute;
		m_iRelation = src.m_iRelation;
		m_cutMin = src.m_cutMin;
		m_cutMax = src.m_cutMax;
	}


	CRuleCondition(int iType, int iAttribute, const CString &strAttribute, int iRelation, double cut)
	{
		m_type = iType;
		m_iAttribute = iAttribute;
		m_strAttribute = strAttribute;
		m_iRelation = iRelation;

		m_cutMin = 0.0;
		m_cutMax = 0.0;

		switch (iRelation)
		{
		case 0:
		case 1:
			m_cutMin = cut;
			break;
		case -1:
			m_cutMax = cut;
			break;
		};
	}


	virtual ~CRuleCondition()
	{
	}


public:

	void operator =(const CRuleCondition& src)
	{
		m_type = src.m_type;
		m_iAttribute = src.m_iAttribute;
		m_strAttribute = src.m_strAttribute;
		m_iRelation = src.m_iRelation;
		m_cutMin = src.m_cutMin;
		m_cutMax = src.m_cutMax;
	}


	int serialize (CArchive& pArchive)
	{
		int len;

		if (pArchive.IsStoring())
		{
			pArchive << m_type;
			pArchive << m_iAttribute;

			pArchive << m_strAttribute.GetLength();
			if(m_strAttribute.GetLength())
				pArchive << m_strAttribute;

			pArchive << m_iRelation;
			pArchive << m_cutMin;
			pArchive << m_cutMax;
		}
		else
		{
			pArchive >> m_type;
			pArchive >> m_iAttribute;

			pArchive >> len;
			if(len > 0)
				pArchive >> m_strAttribute;

			pArchive >> m_iRelation;
			pArchive >> m_cutMin;
			pArchive >> m_cutMax;
		}

		return 0;
	}

	void dump()
	{
		m_LogTrace.WriteLine(" %s(%d) ", m_strAttribute, m_iAttribute);
		switch (m_iRelation)
		{
		case 0:
			m_LogTrace.WriteLine(" == ");
			m_LogTrace.WriteLine(" %lf\n", m_cutMin);
			break;
		case 1:
			m_LogTrace.WriteLine(" > ");
			m_LogTrace.WriteLine(" %lf\n", m_cutMin);
			break;
		case -1:
			m_LogTrace.WriteLine(" <= ");
			m_LogTrace.WriteLine(" %lf\n", m_cutMax);
			break;
		case 2:
			m_LogTrace.WriteLine(" (%lf, %lf]\n", m_cutMin, m_cutMax);
			break;
		}
	}


public:

	// the type of the attribute
	// 0 : continuous
	// 1 : discrete
	int m_type;

	// the index of the attribute that will be tested
	int m_iAttribute;

	// the name of the attribute that will be tested
	CString m_strAttribute;

	// the relationship for the attribute
	// -1 : <= Less and equal
	//  0 : == equal
	//  1 : >  greater
	// 2 : range (]
	int m_iRelation; 

	double m_cutMin;
	double m_cutMax;
};



class CRuleRec : public CObject
{
public:
	CRuleRec()
	{
		m_ruleConditions.RemoveAll();
	}

	CRuleRec(const CRuleRec& src)
	{
		CRuleCondition cRule;
		m_ruleConditions.RemoveAll();
		for(int i = 0; i < src.m_ruleConditions.GetSize(); i++)
		{
			cRule = src.m_ruleConditions[i];
			m_ruleConditions.Add(cRule);
		}
	}

	virtual ~CRuleRec()
	{
		m_ruleConditions.RemoveAll();
	}

	void operator =(const CRuleRec& src)
	{
		CRuleCondition cRule;
		m_ruleConditions.RemoveAll();
		for(int i = 0; i < src.m_ruleConditions.GetSize(); i++)
		{
			cRule = src.m_ruleConditions[i];
			m_ruleConditions.Add(cRule);
		}
	}

public:

	// insert one rule into the rule set
	void insert(int type, int iAttribute, const CString &strAttribute, int iRelation, double cut)
	{
		if(cut <= 0.0)
			return;

		CRuleCondition pRule(type, iAttribute, strAttribute, iRelation, cut);
		insert(pRule);
	}

	// insert the rule into the rule set
	void insert(CRuleCondition& pRule)
	{
		int pos = -1;
		for(int i = 0; i < m_ruleConditions.GetSize(); i++)
		{
			if(m_ruleConditions[i].m_iAttribute > pRule.m_iAttribute || 
				(m_ruleConditions[i].m_iAttribute == pRule.m_iAttribute && pRule.m_iRelation == 0))
			{
				pos = i;
				break;
			}

			if(m_ruleConditions[i].m_iAttribute == pRule.m_iAttribute)
			{
				if(m_ruleConditions[i].m_iRelation == pRule.m_iRelation)
				{
					if(m_ruleConditions[i].m_iRelation == 1)
						m_ruleConditions[i].m_cutMin = max(m_ruleConditions[i].m_cutMin, pRule.m_cutMin);

					if(m_ruleConditions[i].m_iRelation == -1)
						m_ruleConditions[i].m_cutMax = min(m_ruleConditions[i].m_cutMax, pRule.m_cutMax);

					if(m_ruleConditions[i].m_iRelation == 2)
					{
						m_ruleConditions[i].m_cutMin = max(m_ruleConditions[i].m_cutMin, pRule.m_cutMin);
						m_ruleConditions[i].m_cutMax = min(m_ruleConditions[i].m_cutMax, pRule.m_cutMax);
					}
				}
				else
				{
					m_ruleConditions[i].m_iRelation = 2;

					if(pRule.m_iRelation == 1)
					{
						if(m_ruleConditions[i].m_cutMin == 0.0)
							m_ruleConditions[i].m_cutMin = pRule.m_cutMin;
						else
							m_ruleConditions[i].m_cutMin = max(m_ruleConditions[i].m_cutMin, pRule.m_cutMin);
					}

					if(pRule.m_iRelation == -1)
					{
						if(m_ruleConditions[i].m_cutMax == 0.0)
							m_ruleConditions[i].m_cutMax = pRule.m_cutMax;
						else
							m_ruleConditions[i].m_cutMax = min(m_ruleConditions[i].m_cutMax, pRule.m_cutMax);
					}
						
					if(pRule.m_iRelation == 2)
					{
						if(m_ruleConditions[i].m_cutMin == 0.0)
							m_ruleConditions[i].m_cutMin = pRule.m_cutMin;
						else
							m_ruleConditions[i].m_cutMin = max(m_ruleConditions[i].m_cutMin, pRule.m_cutMin);

						if(m_ruleConditions[i].m_cutMax == 0.0)
							m_ruleConditions[i].m_cutMax = pRule.m_cutMax;
						else
							m_ruleConditions[i].m_cutMax = min(m_ruleConditions[i].m_cutMax, pRule.m_cutMax);
					}
				}

				return;
			}
		}

		if(pos >= 0)
			m_ruleConditions.InsertAt(pos, pRule);
		else
			m_ruleConditions.Add(pRule);
	}



	BOOL IsEmpty()
	{
		if(m_ruleConditions.GetSize() > 0)
			return FALSE;

		return TRUE;
	}

	bool IsFullyQuery()
	{
		int first=0, second=0, third=0, fourth=0;

		for(int i = 0; i < m_ruleConditions.GetSize(); i++)
		{
			if(m_ruleConditions[i].m_strAttribute[1] == '1')
				first++;

			if(m_ruleConditions[i].m_strAttribute[1] == '2')
				second++;

			if(m_ruleConditions[i].m_strAttribute[1] == '3')
				third++;

			if(m_ruleConditions[i].m_strAttribute[1] == '4')
				fourth++;
		}

		if(first >= 4 && second >= 4 && third >= 4 && fourth >= 4)
		{
			// *debug*
			//m_LogTrace.WriteLine(" %d %d %d %d\n", first, second, third, fourth);

			return true;
		}

		return false;
	}

	// append the conditions of the rule src into the current
	void append(const CRuleRec& src)
	{
		CRuleCondition pRule;

		for(int i = 0; i < src.m_ruleConditions.GetSize(); i++)
		{
			pRule = src.m_ruleConditions[i];
			insert( pRule );
		}
	}

	int serialize (CArchive& pArchive)
	{
		int size, i;
		CRuleCondition pRule;

		if (pArchive.IsStoring())
		{
			pArchive << m_ruleConditions.GetSize();
			for(i = 0; i < m_ruleConditions.GetSize(); i++)
				m_ruleConditions[i].serialize(pArchive);
		}
		else
		{
			m_ruleConditions.RemoveAll();

			pArchive >> size;
			for(i = 0; i < size; i++)
			{
				pRule.serialize(pArchive);
				m_ruleConditions.Add(pRule);
			}
		}

		return 0;
	}

	void dump()
	{
		if(m_ruleConditions.GetSize() > 0)
		{
			m_ruleConditions[0].dump();
			for(int i = 1; i < m_ruleConditions.GetSize(); i++)
			{
				m_LogTrace.WriteLine("  && ");
				m_ruleConditions[i].dump();
			}
		}
	}

	int GetSize()
	{
		return m_ruleConditions.GetSize();
	}

public:
	CArray<CRuleCondition, CRuleCondition&> m_ruleConditions;

};



class CRuleSet  
{
public:
	CRuleSet();
	CRuleSet(const CRuleSet& src);
	virtual ~CRuleSet();

public:

	void append(const CRuleSet& src);

	void save(const CString &strFile);
	void load(const CString &strFile);

	void dump()
	{
		m_LogTrace.WriteLine("\nNumber of Rules : %d\n", m_ruleSet.GetSize());
		for(int i = 0; i < m_ruleSet.GetSize(); i++)
		{
			m_ruleSet[i].dump();
			m_LogTrace.WriteLine("\n");
		}
	}

	int serialize(CArchive& pArchive);


public:
	int GetSize()						{		return m_ruleSet.GetSize();		}


public:
	CArray<CRuleRec, CRuleRec&>m_ruleSet;

};





class CQueryLearner  
{
public:
	CQueryLearner();
	virtual ~CQueryLearner();


public:

	// This is the main interface of CQueryLearner
	// 1.
	//    setDataSet(pTrainData)
	//    buildClassifier()
	//    Save("***.tree");
	//
	// 2.
	//    setDataSet(pTrainData)
	//    buildClassifier()
	//    setDataSet(pTestData)
	//    Evaluate() or Predict()
	// 
	// 3.
	//    setDataSet(pTestData)
	//    Load("***.tree")
	//    Evaluate() or Predict()

	// to retrieve and set the version information of
	// the classifier
	void setVersion(const CString &strVersion)	{	m_strVersion = strVersion;	}
	CString &getVersion()						{	return m_strVersion;		}


	void setDataSet(CDataSet *pData)			{	m_dataSet = pData;			}

	int serialize(CArchive& pArchive);

	void Save(const char *m_fileName);
	void Load(const char *m_fileName);

	void buildLearner();

//	void Evaluate();
	void Predict();

	void dump()
	{
		m_LogTrace.WriteLine("\nVersion %s\n", m_strVersion);
		m_ruleSet.dump();
		m_LogTrace.WriteLine("\n\n");
	}

protected:
	bool inList(int wn, CArray<int, int&> &tmpWords);

	double searchWords(CRuleRec& conRules, bool& bQueryWords, CArray<int, int&> &tmpWords);

	bool checkWord(int posWord, CRuleRec& conRules, int posQuery, double& sumScore);
	bool meetCondition(int posWord, CRuleCondition& conRules, double& fScore);

public:

	CArray<int, int&>m_queryWords;

protected:

	// the version number of the learner
	CString m_strVersion;

	// the data set
	CDataSet *m_dataSet;

	// the rule set
	CRuleSet m_ruleSet;


};

#endif // !defined(AFX_QUERYLEARNER_H__CD857DE9_C694_481F_B502_8026D074E86D__INCLUDED_)

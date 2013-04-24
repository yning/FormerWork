///////////////////////////////////////////////////////////////
// PageDocument2.h: interface for the CPageDocument2 class.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_PAGEDOCUMENT2_H__D5257263_0D69_4C54_AA4E_A0C9E65B69D7__INCLUDED_)
#define AFX_PAGEDOCUMENT2_H__D5257263_0D69_4C54_AA4E_A0C9E65B69D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "LilacObject.h"
#include "LilacString.h"

#include "Hash.h"

#include "pageDocument.h"
#include "TextProcessor.h"
class CRoleValue : public CObject
{
public:
	CRoleValue()
	{
		m_valRole.Empty();
		m_pInstances.RemoveAll();
		m_entropy = 0.0;
	}

	CRoleValue(const CString &strAttr, int numVal)
	{
		m_valRole = strAttr;
		m_pInstances.Add(numVal);
		m_entropy = 0.0;
	}

	CRoleValue(const CRoleValue& src)
	{
		m_valRole = src.m_valRole;
		m_pInstances.Append(src.m_pInstances);
		m_entropy = src.m_entropy;
	}

	virtual ~CRoleValue()
	{
		m_valRole.Empty();
		m_pInstances.RemoveAll();
	}

	void operator =(const CRoleValue& src)
	{
		m_valRole = src.m_valRole;

		m_pInstances.RemoveAll();
		m_pInstances.Append(src.m_pInstances);

		m_entropy = src.m_entropy;
	}

public:

	CString m_valRole;
	CArray<int, int&>m_pInstances;

	double m_entropy;

};


class CElementWord : public CLilacObject
{
public:

	CElementWord()
	{
		m_word.Empty();
		m_frequency = 0;
	}

	CElementWord(const CString& word, int freq = 1)
	{
		m_word = word;
		m_frequency = freq;
	}

	CString& getWord()					{	return m_word;		}
	void setWord(const CString& word)	{	m_word = word;		}

	int getFreq()						{	return m_frequency;	}
	void setFreq(int value)				{	m_frequency = value;}

	int key()
	{
		CLilacString buffer(m_word);
		return buffer.key();
	}

	int compare(CLilacObject *obj)
	{
		CElementWord *pWord = (CElementWord *)obj;
		return m_word.CompareNoCase(pWord->getWord());
	}

	CLilacObject * clone()
	{
		CElementWord *pWord = (CElementWord *) new CElementWord(m_word, m_frequency);
		return pWord;
	}


private:

	CString m_word;
	int m_frequency;
};


class CPageWord : public CLilacObject
{
public:

	CPageWord()
	{
		m_word.Empty();

		m_arColor.RemoveAll();
		m_arBGColor.RemoveAll();

		m_arFontFamily.RemoveAll();
		m_arFontSize.RemoveAll();
		m_arFontStyle.RemoveAll();
		m_arFontWeight.RemoveAll();

		m_fgColor = 0.0;
		m_bgColor = 0.0;
		m_fontFamily = 0.0;
		m_fontSize = 0.0;
		m_fontStyle = 0.0;
		m_fontWeight = 0.0;

	}

	CPageWord(const CString& word)
	{
		m_word = word;

		m_arColor.RemoveAll();
		m_arBGColor.RemoveAll();

		m_arFontFamily.RemoveAll();
		m_arFontSize.RemoveAll();
		m_arFontStyle.RemoveAll();
		m_arFontWeight.RemoveAll();

		m_fgColor = 0.0;
		m_bgColor = 0.0;
		m_fontFamily = 0.0;
		m_fontSize = 0.0;
		m_fontStyle = 0.0;
		m_fontWeight = 0.0;
	}

	CString& getWord()					{	return m_word;		}
	void setWord(const CString& word)	{	m_word = word;		}

	int key()
	{
		CLilacString buffer(m_word);
		return buffer.key();
	}

	int compare(CLilacObject *obj)
	{
		CPageWord *pWord = (CPageWord *)obj;
		return m_word.CompareNoCase(pWord->getWord());
	}

	CLilacObject * clone()
	{
		CPageWord *pWord = (CPageWord *) new CPageWord(m_word);
		Append(m_arColor, pWord->m_arColor);
		m_fgColor = pWord->m_fgColor;

		Append(m_arBGColor, pWord->m_arBGColor);
		m_bgColor = pWord->m_bgColor;

		Append(m_arFontFamily, pWord->m_arFontFamily);
		m_fontFamily = pWord->m_fontFamily;

		Append(m_arFontSize, pWord->m_arFontSize);
		m_fontSize = pWord->m_fontSize;

		Append(m_arFontStyle, pWord->m_arFontStyle);
		m_fontStyle = pWord->m_fontStyle;

		Append(m_arFontWeight, pWord->m_arFontWeight);
		m_fontWeight = pWord->m_fontWeight;

		return pWord;
	}

	void Append(const CArray<double, double&> &arSrc, CArray<double, double&> &arTarget)
	{
		double buffer;

		arTarget.RemoveAll();
		for(int i = 0; i < arSrc.GetSize(); i++)
		{
			buffer = arSrc[i];
			arTarget.Add(buffer);
		}
	}


	void computeScore()
	{
		int i;

		m_fgColor = 0.0;
		for(i = 0; i < m_arColor.GetSize(); i++)
			m_fgColor += m_arColor[i];
		m_arColor.RemoveAll();

		m_bgColor = 0.0;
		for(i = 0; i < m_arBGColor.GetSize(); i++)
			m_bgColor += m_arBGColor[i];
		m_arBGColor.RemoveAll();

		m_fontFamily = 0.0;
		for(i = 0; i < m_arFontFamily.GetSize(); i++)
			m_fontFamily += m_arFontFamily[i];
		m_arFontFamily.RemoveAll();

		m_fontSize = 0.0;
		for(i = 0; i < m_arFontSize.GetSize(); i++)
			m_fontSize += m_arFontSize[i];
		m_arFontSize.RemoveAll();

		m_fontStyle = 0.0;
		for(i = 0; i < m_arFontStyle.GetSize(); i++)
			m_fontStyle += m_arFontStyle[i];
		m_arFontStyle.RemoveAll();

		m_fontWeight = 0.0;
		for(i = 0; i < m_arFontWeight.GetSize(); i++)
			m_fontWeight += m_arFontWeight[i];
		m_arFontWeight.RemoveAll();
	}


public:

	CString m_word;

	CArray<double, double&>m_arColor;
	double m_fgColor;

	CArray<double, double&>m_arBGColor;
	double m_bgColor;

	CArray<double, double&>m_arFontFamily;
	double m_fontFamily;

	CArray<double, double&>m_arFontSize;
	double m_fontSize;

	CArray<double, double&>m_arFontStyle;
	double m_fontStyle;

	CArray<double, double&>m_arFontWeight;
	double m_fontWeight;

};


//
//		CPageDocument2 *pDoc2;
//		pDoc2 = (CPageDocument2 *) new CPageDocument2();
//		pDoc2->m_envTextProcessor = m_pageTree->getRepository()->m_envTextProcessor;
//
//		if(pDoc2->calLayoutRole(m_pageTree))
//			pDoc2->dump();
//
//		delete pDoc2;
//

class CPageDocument2  
{
private:
	Hash *m_hPageWords;

public:
	CPageDocument2();
	virtual ~CPageDocument2();
	void Reset();

public:

	bool calLayoutRole(CPageDocument *pageTree);

	Hash *getWords()
	{
		return m_hPageWords;
	}

	CPageWord *lookup(const CString &szWord);

public:
	bool IsSkipTag(CPageTagNode *pNode);
	void getNodeWords(CPageTagNode *pNode);
	void minusWordFreq(Hash *pWords, CElementWord *pElemWord);
	void minusElementWords(Hash *pWords, Hash *cWords);
	void InsertWord(Hash *hWords, const CString &strWord);
	void calRoleDistribution(CPageTagNode *pNode);

public:
	void dumpBody(CPageTagNode *pNode);
	void statWeight(const CArray<double, double&> &arValue, double &lfMin, double &lfMean);
	void InsertPageWord(const CString &strWord, double dClr, double dBGClr, double dFamily, double fSize, double dStyle, double dWeight);
	double GetWeight(const CArray<CRoleValue, CRoleValue&> &roleInst, const CString &strInstance);
	void calWordRole(CPageTagNode *pNode);
	void ScatterBackGroundColor(CPageTagNode *pNode);
	void dump();
	void calEntropy(CArray<CRoleValue, CRoleValue&> &roleInst, int numWords);
	void addRoleInstance(CArray<CRoleValue, CRoleValue&> &roleInst, const CString &strInstance, int cntWords);
	int countElementWords(CPageTagNode *pNode);
	void calRoleValues(CPageTagNode *pNode);

public:
	CTextProcessor *m_envTextProcessor;

	CArray<CRoleValue, CRoleValue&> m_roleColor;
	CArray<CRoleValue, CRoleValue&> m_roleBGColor;

	CArray<CRoleValue, CRoleValue&> m_roleFontFamily;
	CArray<CRoleValue, CRoleValue&> m_roleFontSize;
	CArray<CRoleValue, CRoleValue&> m_roleFontStyle;
	CArray<CRoleValue, CRoleValue&> m_roleFontWeight;

};

#endif // !defined(AFX_PAGEDOCUMENT2_H__D5257263_0D69_4C54_AA4E_A0C9E65B69D7__INCLUDED_)


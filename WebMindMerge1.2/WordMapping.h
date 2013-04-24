///////////////////////////////////////////////////////////////
// WordMapping.h: interface for the CWordMapping class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_WORDMAPPING_H__6075362D_5E3A_4274_B3D0_449994AB47A0__INCLUDED_)
#define AFX_WORDMAPPING_H__6075362D_5E3A_4274_B3D0_449994AB47A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "LilacObject.h"




#include "LilacString.h"
#include "Hash.h"
#include "Utility.h"

// the structure to store the original word and its
// occurence in all the observed page sequence
class CWordOriginal : public CObject
{
public:
	CWordOriginal()
	{
		m_wordOriginal.Empty();
		m_numOccurence = 0;
	}

	CWordOriginal(const CString &pWord, int nCnt)
	{
		m_wordOriginal = pWord;
		m_numOccurence = nCnt;
	}

	CWordOriginal(const CWordOriginal& src)
	{
		m_wordOriginal = src.m_wordOriginal;
		m_numOccurence = src.m_numOccurence;
	}

	virtual ~CWordOriginal()
	{
	}

	void operator =(const CWordOriginal& src)
	{
		m_wordOriginal = src.m_wordOriginal;
		m_numOccurence = src.m_numOccurence;
	}

    void serialize (CArchive& pArchive)
	{
		int size;

		// Serialize the object ...
		if (pArchive.IsStoring())
		{
			pArchive << m_numOccurence;

			size = m_wordOriginal.GetLength();
			pArchive << size;
			if(size)
				pArchive << m_wordOriginal;
		}
		else
		{
			pArchive >> m_numOccurence;

			m_wordOriginal.Empty();
			pArchive >> size;
			if(size)
				pArchive >> m_wordOriginal;
		}
	}

public:

	// original word
	CString m_wordOriginal;

	// number of its occurence
	int m_numOccurence;
};


// the words of one page
class CWordStem : public CLilacObject
{

public:

    CWordStem()
    {
		m_stem.Empty();
        m_originals.RemoveAll();
    }
    
    ~CWordStem()
    {
		m_stem.Empty();
		m_originals.RemoveAll();
    }

    CLilacObject *clone()
    {
        CWordStem *theclone;
        
        theclone = (CWordStem *)new CWordStem();

		theclone->StemWord(m_stem);

		for(int i = 0; i < m_originals.GetSize(); i++)
			theclone->appendWord(m_originals[i].m_wordOriginal, m_originals[i].m_numOccurence);

        return theclone; 
    }

    void appendWord(const char *m_word, int cntOccurence=1)
	{
		for(int i = 0; i < m_originals.GetSize(); i++)
		{
			if(m_originals[i].m_wordOriginal.CompareNoCase(m_word) == 0)
			{
				m_originals[i].m_numOccurence += cntOccurence;
				return;
			}
		}

		CWordOriginal pWord(m_word, cntOccurence);
		m_originals.Add(pWord);
	}

	void getShortest(CString& word, BOOL bShortest=TRUE)
	{
		if(m_originals.GetSize() <= 0)
			return;

		// get the original words in shortest length
		word = m_originals[0].m_wordOriginal;
		for(int i = 1; i < m_originals.GetSize(); i++)
		{
			if(bShortest)
			{
				if(m_originals[i].m_wordOriginal.GetLength() < word.GetLength())
					word = m_originals[i].m_wordOriginal;
			}
			else
			{
				if(m_originals[i].m_wordOriginal.GetLength() >= word.GetLength())
					word = m_originals[i].m_wordOriginal;
			}
		}

		return;
	}

	void getMostFrequent(CString& word, BOOL bMax=TRUE)
	{
		if(m_originals.GetSize() <= 0)
			return;

		// get the original word in maximum appearance
		int pos = 0;
		for(int i = 1; i < m_originals.GetSize(); i++)
		{
			if(bMax)
			{
				if(m_originals[i].m_numOccurence > m_originals[pos].m_numOccurence)
					pos = i;
			}
			else
			{
				if(m_originals[i].m_numOccurence <= m_originals[pos].m_numOccurence)
					pos = i;
			}

		}

		word = m_originals[pos].m_wordOriginal;

		return;
	}

    void StemWord(const CString& value)
	{
		m_stem = value;
	}

    const CString& StemWord()		    	{ return m_stem; }

    int key()
	{
		CLilacString pString(m_stem);
		return pString.key();
	}

    int compare(CLilacObject *obj)
    {
        CWordStem *hopsobj=NULL;
		
        hopsobj = (CWordStem *)obj;

        CString temp=hopsobj->StemWord();//add by ny
		if (hopsobj!=NULL)//&&((temp[0]>'a'&&temp[0]<'z')||(temp[0]>'A'&&temp[0]<'Z'))
			if(m_stem.Compare(hopsobj->StemWord()) == 0)
				return 0;

        return -1;
    }
    
    int serialize (CArchive& pArchive)
    {
        int nStatus = 0, i, size = 0;

        // Serialize the object ...
        if (pArchive.IsStoring())
        {
			pArchive << m_stem.GetLength();
			if(m_stem.GetLength() > 0)
				pArchive << m_stem;

            pArchive << m_originals.GetSize();
			for(i = 0; i < m_originals.GetSize(); i++)
	            m_originals[i].serialize(pArchive);
        }
        else
        {
			m_stem.Empty();
            pArchive >> size;
			if(size > 0)
				pArchive >> m_stem;

			m_originals.RemoveAll();
            pArchive >> size;
			if(size>4096)//niedong
				size=4096;
			if(size > 0)
			{
				m_originals.SetSize(size);
				for(i = 0; i < size; i++)
		            m_originals[i].serialize(pArchive);
			}
        }

        return (nStatus);
    }
    
	void dump()
	{
		m_LogTrace.WriteLine("\n Stemmed Word : %s\n", m_stem);
		m_LogTrace.WriteLine("-------------------------------\n");
		for(int i = 0; i < m_originals.GetSize(); i++)
			m_LogTrace.WriteLine("%20s : %8d\n", m_originals[i].m_wordOriginal, m_originals[i].m_numOccurence);
	}

private:

	// the word stem
    CString m_stem;

	// all its original words associated with the occurences of the word in
	// the text
	CArray<CWordOriginal, CWordOriginal&> m_originals;
};



class CWordMapping  
{
public:
	void getOriginalWord(const char *stem, CString& word);
	int serialize (CArchive& pArchive);

	void save(const char *m_file);
	void load(const char *m_file);

	void insertStemWord(const char *stem, const char *word);

	CWordMapping();
	virtual ~CWordMapping();

	void dump()
	{
		if(m_mapping)
			m_mapping->dump();
	}

private:
	Hash *m_mapping;
};

#endif // !defined(AFX_WORDMAPPING_H__6075362D_5E3A_4274_B3D0_449994AB47A0__INCLUDED_)

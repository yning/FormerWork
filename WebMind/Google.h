///////////////////////////////////////////////////////////////
// Google.h: interface for the CGoogle class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_GOOGLE_H__761361C7_CB36_4613_B9E9_1EF0F69A9DB5__INCLUDED_)
#define AFX_GOOGLE_H__761361C7_CB36_4613_B9E9_1EF0F69A9DB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LilacObject.h"
#include "LilacString.h"
#include "Hash.h"
#include "Url.h"
#include "Utility.h"
#include "SearchEngine.h"
#include "PageCache.h"
#include "PageRepository.h"

class CGoogleQuery : public CLilacObject
{

public:

    CGoogleQuery()
    {
		m_szQuery.Empty();
		m_arURLs.RemoveAll();
    }
    
    ~CGoogleQuery()
    {
		m_szQuery.Empty();
		m_arURLs.RemoveAll();
    }

    CLilacObject *clone()
    {
        CGoogleQuery *theclone;
        
        theclone = (CGoogleQuery *)new CGoogleQuery();
		theclone->m_szQuery = m_szQuery;
		theclone->m_arURLs.Append(m_arURLs);

        return theclone; 
    }

    int key()
	{
		CLilacString pString(m_szQuery);
		return pString.key();
	}

    int compare(CLilacObject *obj)
    {
        CGoogleQuery *hopsobj;
		
        hopsobj = (CGoogleQuery *)obj;
		if(m_szQuery.CompareNoCase(hopsobj->m_szQuery) == 0)
			return 0;

        return -1;
    }
    
	void dump()
	{
		m_LogTrace.WriteLine("\n*********************************************************************************\n");
		m_LogTrace.WriteLine("      Query : %s\n", m_szQuery);
		m_LogTrace.WriteLine("       URLs :\n");
		for(int i = 0; i < m_arURLs.GetSize(); i++)
			m_LogTrace.WriteLine("               %s\n", m_arURLs[i]);
		m_LogTrace.WriteLine("\n");
	}

	int serialize(CArchive& pArchive)
	{
		int nStatus = 0, i, len, size;
		CString pQuery;

	    // Serialize the object ...
        if (pArchive.IsStoring())
        {
            // Write information of one node
            pArchive << m_szQuery.GetLength();
			if(m_szQuery.GetLength() > 0)
				pArchive << m_szQuery;

			pArchive << m_arURLs.GetSize();
			if(m_arURLs.GetSize() > 0)
			{
	            for(i = 0; i < m_arURLs.GetSize(); i++)
				{
		            pArchive << m_arURLs[i].GetLength();
					if(m_arURLs[i].GetLength() > 0)
						pArchive << m_arURLs[i];
				}
			}
        }
        else
        {
            pArchive >> len;
			m_szQuery.Empty();
			if(len > 0)
				pArchive >> m_szQuery;

			m_arURLs.RemoveAll();

			pArchive >> size;
			if(size > 0)
			{
	            for(i = 0; i < size; i++)
				{
					pQuery.Empty();
		            pArchive >> len;
					if(len > 0)
					{
						pArchive >> pQuery;
						m_arURLs.Add(pQuery);
					}
				}
			}
        }

	    return (nStatus);
	}


public:
    CString m_szQuery;
	CStringArray m_arURLs;
};



class CGoogle  
{
public:
	CGoogle();
	virtual ~CGoogle();

	bool queryGoogle(const CStringArray &stemWords, CStringArray &urlGoogle);

	void setFolder(const CString &strFolder)
	{
		m_strFolder = strFolder;
	}

	void Load()
	{
		CString m_strGoogle;
		m_strGoogle = m_strFolder + _T("\\google.dat");
		load(m_strGoogle);
	}


	void Save()
	{
		CString m_strGoogle;
		m_strGoogle = m_strFolder + _T("\\google.dat");
		save(m_strGoogle);
	}

protected:
	void querying(CStringArray &originalWords, CStringArray &urlGoogle);
	
	void save(const CString &strFile);
	void load(const CString &strFile);

public:
	CPageRepository *m_pRepository;

protected:
	CStringArray m_listGoogle;

	Hash *m_hQuery;

	CString m_strFolder;

};

#endif // !defined(AFX_GOOGLE_H__761361C7_CB36_4613_B9E9_1EF0F69A9DB5__INCLUDED_)

///////////////////////////////////////////////////////////////
// WordMapping.cpp: implementation of the CWordMapping class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WordMapping.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWordMapping::CWordMapping()
{
	m_mapping = (Hash *) new Hash();
}

CWordMapping::~CWordMapping()
{
	delete m_mapping;
}

void CWordMapping::insertStemWord(const char *stem, const char *word)
{
	CWordStem *pStem;
	CWordStem *pResult;

	pStem = (CWordStem *) new CWordStem();
	pStem->StemWord(stem);
	if((pResult = (CWordStem *)m_mapping->lookup(pStem)) != NULL)
	{
		pResult->appendWord(word);
		delete pStem;
	}
	else
	{
		pStem->appendWord(word);
		m_mapping->insert(pStem);
	}

	return;
}

void CWordMapping::getOriginalWord(const char *stem, CString &word)
{
	CWordStem *pStem;
	CWordStem *pResult = NULL;

	pStem = (CWordStem *) new CWordStem();
	pStem->StemWord(stem);
	if((pResult = (CWordStem *)m_mapping->lookup(pStem)) != NULL)
		pResult->getShortest(word);
	else
		word = stem;

	delete pStem;
	return;
}


// Load the word mapping file
void CWordMapping::load(const char *m_file)
{
	CFile m_rFile( m_file, CFile::modeRead);
	CArchive m_rArch( &m_rFile, CArchive::load );
	serialize(m_rArch);
	m_rArch.Close();
	m_rFile.Close();
}


// save word mapping file
void CWordMapping::save(const char *m_file)
{
	CFile m_wFile( m_file, CFile::modeCreate | CFile::modeWrite);
	CArchive m_wArch( &m_wFile, CArchive::store );

	serialize(m_wArch);

	m_wArch.Flush();
	m_wFile.Flush();

	m_wArch.Close();
	m_wFile.Close();
}

int CWordMapping::serialize(CArchive &pArchive)
{
	int nStatus = 0, i, size = 0;
	CWordStem *pStem;

	// Serialize the object ...
	if (pArchive.IsStoring())
	{
		List *m_lWord;

		m_lWord = m_mapping->toList();

		pArchive << m_lWord->size();
		for(i = 0; i < m_lWord->size(); i++)
		{
			pStem = (CWordStem *) m_lWord->get(i);
			pStem->serialize(pArchive);
		}

		delete m_lWord;
	}
	else
	{
		pArchive >> size;
		if(size > 0)
		{
			for(i = 0; i < size; i++)
			{
				pStem = (CWordStem *) new CWordStem();
				pStem->serialize(pArchive);
				m_mapping->insert(pStem);
			}
		}
	}

	return (nStatus);
}


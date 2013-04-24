///////////////////////////////////////////////////////////////
// TextProcessor.h: interface for the CTextProcessor class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTPROCESSOR_H__4EF41588_6A42_435C_8982_DFB1CD4459B8__INCLUDED_)
#define AFX_TEXTPROCESSOR_H__4EF41588_6A42_435C_8982_DFB1CD4459B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Corpus.h"
#include "PorterStemming.h"
#include "PuncMark.h"
#include "StopWord.h"
#include "WordMapping.h"


class CTextProcessor  
{
public:
	CTextProcessor(const char* corpus_file);
	virtual ~CTextProcessor();
	
	void textProcess(const CString& strText, CStringArray& m_stemWords);
	void textProcessU(const CString& strText, CStringArray& m_stemWords);
	void recordWordMapping(bool value = true)
	{
		if(m_wordMapper)
			delete m_wordMapper;
		m_wordMapper = NULL;
		
		m_bMapping = value;
		if(m_bMapping)
			m_wordMapper = (CWordMapping *) new CWordMapping();
		
	}
	
	bool hasWordMappingRecorded()				{	return m_bMapping;	}
	
	void getOriginalWord(const char *stem, CString &word)
	{
		if(m_bMapping && m_wordMapper)
			m_wordMapper->getOriginalWord(stem, word);
		else
			word = stem;
	}
	
	CWordMapping *getWordMapping()
	{
		return m_wordMapper;
	}
	
	// for denugging purpose
	void dumpWordMapping()
	{
		if(m_wordMapper)
			m_wordMapper->dump();
	}
	
	void initEnv();
	
	double lookupIDF(const char *word);
	
private:
	void deInitEnv();
	
	bool m_bInited;
	
	CCorpus *m_envCorpus;
	CPorterStemming *m_envStemmer;
	CPuncMark *m_envPuncmarks;
	CStopWord *m_envStopword;
	
	CWordMapping *m_wordMapper;
	
	bool m_bMapping;
	
	CString m_corpusFile;
};

#endif // !defined(AFX_TEXTPROCESSOR_H__4EF41588_6A42_435C_8982_DFB1CD4459B8__INCLUDED_)

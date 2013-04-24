///////////////////////////////////////////////////////////////
// Corpus.h: interface for the CCorpus class.
//
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_CORPUS_H__3021E1E2_D6E2_410F_A9DE_4676D40F2A13__INCLUDED_)
#define AFX_CORPUS_H__3021E1E2_D6E2_410F_A9DE_4676D40F2A13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NUM_CORPUS_WORDS 497761

struct corpus_word
{
	char * m_word;
	long m_count;
};


class CCorpus  
{
public:
	CCorpus(const char *corpus_file);
	virtual ~CCorpus();
	
	double lookupIDF(const char *word);
	
private:
	void load(const char *wordfreq);
	struct corpus_word *allWords;
	long totalCnt;
};

#endif // !defined(AFX_CORPUS_H__3021E1E2_D6E2_410F_A9DE_4676D40F2A13__INCLUDED_)

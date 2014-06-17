///////////////////////////////////////////////////////////////
// Corpus.cpp: implementation of the CCorpus class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Corpus.h"

#include <math.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "Shlwapi.h"

int cmpcorpusword(void **pfirst, const void *psecond)
{
	struct corpus_word *pcmp = (struct corpus_word *) psecond; 
	const char * pstr1 = (const char *) *pfirst; 
	const char * pstr2 = pcmp->m_word; //extract string from struct

	return strcmp(pstr1, pstr2);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCorpus::CCorpus(const char *corpus_file)
{
	totalCnt = 0L;
	allWords = (struct corpus_word *)NULL;

	load(corpus_file);
}

CCorpus::~CCorpus()
{
	if(allWords)
	{
		for(long i = 0; i < NUM_CORPUS_WORDS; i++)
			delete [] allWords[i].m_word;

		delete [] allWords;
	}
}

void CCorpus::load(const char *wordfreq)
{
	if(!PathFileExists(wordfreq))
		return;

	int fpr;
	char buf[256];
	short slen;
	long wCnt, i;

	allWords = (struct corpus_word*) new struct corpus_word[NUM_CORPUS_WORDS];
	
	fpr = open(wordfreq, _O_BINARY | _O_RDONLY);
	for(i = 0; i < NUM_CORPUS_WORDS; i++)
	{
		read(fpr, &slen, sizeof(short));

		memset( buf, '\0', 256 );
		read(fpr, buf, slen);

		read(fpr, &wCnt, sizeof(long));

		totalCnt += wCnt;

		allWords[i].m_count = wCnt;

		allWords[i].m_word = (char *) new char[strlen(buf) + 1];
		strcpy(allWords[i].m_word, buf);
	}

	close(fpr);
}


double CCorpus::lookupIDF(const char *word)
{
   if(!allWords)
	   return -1.0;

   struct corpus_word *result = NULL;

   // Find the word using a binary search algorithm
   result = (struct corpus_word *)bsearch( 
	   (char *) &word, 
	   (struct corpus_word *)allWords, 
	   NUM_CORPUS_WORDS,
       sizeof( struct corpus_word ), 
	   (int (*)(const void*, const void*))cmpcorpusword );

   if(result)
	   return log( (double)totalCnt/(double)result->m_count );

   return -1.0;

	/*
    CCorpusWord *pword, *pw;
    double result = -1.0;

    pword = (CCorpusWord *)new CCorpusWord(word);
    if((pw = (CCorpusWord *)m_words->lookup(pword)) != NULL)
		result = log( (double)totalCnt/pw->getWeight() );

    delete pword;

    return result;
	*/
}

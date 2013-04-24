///////////////////////////////////////////////////////////////
// WordSearch.h: interface for the CWordSearch class.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_WORDSEARCH_H__0367C1EC_97E5_4CB4_8101_BACC3B62B572__INCLUDED_)
#define AFX_WORDSEARCH_H__0367C1EC_97E5_4CB4_8101_BACC3B62B572__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LilacObject.h"

// the words in the search page
class CWordSearch : public CLilacObject
{

public:

    CWordSearch();
    ~CWordSearch();
    
    void Word(const char *value);
    char *Word();

    void Count(int value);
    int Count();

    CLilacObject *clone();
    int key();
    int compare(CLilacObject *obj);
    void dump();

public:
	int isKeywordCnt;

	int skippedTitleCnt;
	int skippedSnippetCnt;

	int chosenTitleCnt;
	int chosenSnippetCnt;
    
	int untouchedTitleCnt;
	int untouchedSnippetCnt;

	int bkTitleCnt;
	int bkSnippetCnt;

private:

    char *word;

	// number of apperance of keystring or keyint
	int count;
};


#endif // !defined(AFX_WORDSEARCH_H__0367C1EC_97E5_4CB4_8101_BACC3B62B572__INCLUDED_)

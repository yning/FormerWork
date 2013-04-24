///////////////////////////////////////////////////////////////
// WordSeq.h: interface for the CWordSeq class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_WORDSEQ_H__DB31D323_7953_445E_A9CC_618EE2427AB9__INCLUDED_)
#define AFX_WORDSEQ_H__DB31D323_7953_445E_A9CC_618EE2427AB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LilacObject.h"
#include "ItemSet.h"

// the word's information across the whole sequence
class CWordSeq : public CLilacObject
{

public:

    CWordSeq();
    ~CWordSeq();

    void setWord(const CString& value);
    void setWord(const char *value);

    CString& getWord();

	void setWeightSize(int size);

    CLilacObject *clone();
    int key();
    int compare(CLilacObject *obj);
    void dump();

public:

    CString word;

	int frequency;					// the number of occurence of the word in the whole session

	int length;						// the size of weight array
	double *weightTFIDF;			// word's weight in each page
	double *weightJewell;			// word's weight in each page

	int inHyperlinkCnt;				// the number of hyperlinks that the word in

	int inTitleCnt;					// number of pages that word in title
	int inMetaCnt;					// number of pages that word in Meta

	CItemSet *inFromPages;			// list of pageno that the word in the clicked hyperlink that followed
};


#endif // !defined(AFX_WORDSEQ_H__DB31D323_7953_445E_A9CC_618EE2427AB9__INCLUDED_)

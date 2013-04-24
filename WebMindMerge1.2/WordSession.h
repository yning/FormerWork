///////////////////////////////////////////////////////////////
// WordSession.h: interface for the CWordSession class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_WORDSESSION_H__DCDD1E4D_38ED_4D9A_BB5F_7722D0144E98__INCLUDED_)
#define AFX_WORDSESSION_H__DCDD1E4D_38ED_4D9A_BB5F_7722D0144E98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LilacObject.h"
#include "DataSet.h"

// the words of one page
class CWordSession : public CLilacObject
{

public:

    CWordSession();
    ~CWordSession();

	double calWordSignificance();
	double calSignificance(double *weight);
	double calWordICness();
	double calWordICRelevant();
	double calWordICQuery();

	void fillDataEntry(CDataSet *pData, int index);

    void Word(const char *value);
    char *Word();

    CLilacObject *clone();
    int key();
    int compare(CLilacObject *obj);
    
    void dump();
	void DumpWordSession();

	void outputFeatures(FILE *fp_data);

	void outputDataEntry(FILE *fp_data, const CString &strLabel);
	void outputDataEntry(bool bPositive, FILE *fp_data);

public:
	// The relative latest location that the word appears in the sequence
	// divided by the length of the sequence
	double latestAppearance;	

	// number of the occurences of the word in the whole session
	// divided by the number of distinuct words in the sequential pages
	double relativeFreq;				

	// number of the occurences of the word in the whole session
	// divided by the number of occurences of all the words in the sequential pages
	double ratioOccurences;

	// the number of occurences * its idf
	double seqTFIDFWeight;

	// number of the occurences of the word in the whole session
	int absFreq;				

	///////////////////////////////////////////////////////////////////////////////
	// how many times the words in a search query
	int isKeywordCnt;			

	int skippedTitleCnt;
	int skippedSnippetCnt;

	int chosenTitleCnt;
	int chosenSnippetCnt;
    
	int untouchedTitleCnt;
	int untouchedSnippetCnt;

	int bkTitleCnt;
	int bkSnippetCnt;

	///////////////////////////////////////////////////////////////////////////////

	// how often the word appears in the page sequence, 
	// #pages that the word is in / the length of the sequence
    double ratioWordAppearance;

	// the avergae and variance of TF*IDF weight of the word in the 
	// session
    double avTFIDFWeight;
    double varTFIDFWeight;

	// the avergae and variance of Jewell weight of the word in the 
	// session
    double avJewellWeight;
    double varJewellWeight;

	// the trend of the word's weight within the whole sequence,
	// split the whole sequence into left and right side,
	// compute sum of weight for both sides, decide whether 
	// the word's weight decrease or increase
    int trendTFIDFWeight;
    int trendJewellWeight;

	// how often the word in the hyperlink's anchor text that have been
	// followed
    double ratioLinkFollow;

	// how often the word in the page (anywhere) that followed
    double ratioFollow;

	// how often the word in the hyperlink's anchor text but later people
	// back from the followed page
    double ratioLinkBack;

	/////////////////////////////////////////////////////////////////////////////
	// how often the word in the pages that backed up later
    double ratioBackward;

	// the average and variance TF*IDF weight of the word in such pages
    double avTFIDFWeightBackward;
    double varTFIDFWeightBackward;

	// the average and variance Jewell weight of the word in such pages
    double avJewellWeightBackward;
    double varJewellWeightBackward;
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	// how often the word in the pages that are forward
    double ratioForward;

	// the average and variance TF*IDF weight of the word in such pages
    double avTFIDFWeightForward;
    double varTFIDFWeightForward;

	// the average and variance Jewell weight of the word in such pages
    double avJewellWeightForward;
    double varJewellWeightForward;
	/////////////////////////////////////////////////////////////////////////////

	// how often the word in the pages' title
    double ratioInTitle;

	// how often the word appears in the pages' meta "description" and "key"
    double ratioInvisible;

	// whether this word is an IC-word or not
	bool ICword;

private:

    char *word;
};



#endif // !defined(AFX_WORDSESSION_H__DCDD1E4D_38ED_4D9A_BB5F_7722D0144E98__INCLUDED_)

///////////////////////////////////////////////////////////////
// PorterStemming.h: interface for the CPorterStemming class.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_PORTERSTEMMING_H__BE1329EF_72B5_4CED_9C5E_55D6DDDDED32__INCLUDED_)
#define AFX_PORTERSTEMMING_H__BE1329EF_72B5_4CED_9C5E_55D6DDDDED32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef struct {
	int id;                 /* returned if rule fired */
	char *old_end;          /* suffix replaced */
	char *new_end;          /* suffix replacement */
	int old_offset;         /* from end of word to start of suffix */
	int new_offset;         /* from beginning to end of new suffix */
	int min_root_size;      /* min root word size for replacement */
	int (*condition)(char*);/* the replacement test function */
} RuleList;


static int WordSize( char *word );
static int ContainsVowel( char *word );
static int EndsWithCVC( char *word );
static int AddAnE( char *word );
static int RemoveAnE( char *word );
static int ReplaceEnd( char *word, RuleList *rule );


class CPorterStemming  
{
public:
	
	CPorterStemming();
	virtual ~CPorterStemming();
	
	int Stem(char* word);
};

#endif // !defined(AFX_PORTERSTEMMING_H__BE1329EF_72B5_4CED_9C5E_55D6DDDDED32__INCLUDED_)

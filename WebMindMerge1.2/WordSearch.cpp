///////////////////////////////////////////////////////////////
// WordSearch.cpp: implementation of the CWordSearch class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WordSearch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWordSearch::CWordSearch()
{
	word = NULL;
	count = 0;

	isKeywordCnt = 0;

	skippedTitleCnt = 0;
	skippedSnippetCnt = 0;

	chosenTitleCnt = 0;
	chosenSnippetCnt = 0;
    
	untouchedTitleCnt = 0;
	untouchedSnippetCnt = 0;

	bkTitleCnt = 0;
	bkSnippetCnt = 0;
}
    
CWordSearch::~CWordSearch()
{
	if(word)
		delete word;
}

CLilacObject * CWordSearch::clone()
{
	CWordSearch *theclone;
        
	theclone = (CWordSearch *)new CWordSearch();

	if(word)
		theclone->Word(word);

	theclone->Count(count);

	theclone->isKeywordCnt = isKeywordCnt;

	theclone->skippedTitleCnt = skippedTitleCnt;
	theclone->skippedSnippetCnt = skippedSnippetCnt;

	theclone->chosenTitleCnt = chosenTitleCnt;
	theclone->chosenSnippetCnt = chosenSnippetCnt;
    
	theclone->untouchedTitleCnt = untouchedTitleCnt;
	theclone->untouchedSnippetCnt = untouchedSnippetCnt;

	theclone->bkTitleCnt = bkTitleCnt;
	theclone->bkSnippetCnt = bkSnippetCnt;

	return theclone; 
}
    
void CWordSearch::Word(const char *value)
{
	if(word)
		delete word;
	word = (char *)new char[strlen(value) + 1];
	strcpy(word, value);
}

char * CWordSearch::Word()
{
	return word;
}

void CWordSearch::Count(int value)
{
	count = value;
}

int CWordSearch::Count()
{
	return count;
}

int CWordSearch::key()
{/*
	int  h = 0;
	int g;
	char *pstring;

	if(word != NULL)
	{
		pstring = word;

		while (*pstring != 0)
		{
			h = (h << 2) + tolower(*pstring++);
			if ((g = h & 0xf0000000L) != 0)
				h = (h ^ (g >> 24)) ^ g;
		}
	}

	return h;
	*/
	 unsigned  int  hash  =   0 ; 
  unsigned  int  x     =   0 ; 
  	char *pstring;
  if(word!=NULL)
  {
	  pstring = word;
      	while (*pstring != 0)
		{ 
        hash  =  (hash << 4 )  +  *pstring++ ; //hash值左移4位加上一个字符
        if  ((x  =  hash  &   0xF0000000L )  !=   0 )//判断hash值的高4位是否不为0，因为不为0时需要下面特殊处理，否则上面一步的左移4位会把这高四位给移走，造成信息丢失
		{ 
        hash  ^=  (x >> 24 );   //把刚才的高4位跟hash的低5-8位异或
        hash  &=   ~ x;            //把高4位清0
		}  
	   }  
  }
  return  (hash  &   0x7FFFFFFF ) % 100; //希望hash值是一个非负数

}

int CWordSearch::compare(CLilacObject *obj)
{
	CWordSearch *hopsobj;
		
	hopsobj = (CWordSearch *)obj;

	if((word != NULL) && (hopsobj->Word() != NULL))
	{
		if(stricmp(word, hopsobj->Word()) == 0)
			return 0;
		else
			return -1;
	}

	return -1;
}
    
void CWordSearch::dump()
{
}

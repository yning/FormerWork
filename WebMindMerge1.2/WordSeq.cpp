///////////////////////////////////////////////////////////////
// WordSeq.cpp: implementation of the CWordSeq class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "WordSeq.h"
#include "Utility.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWordSeq::CWordSeq()
{
	word.Empty();
	frequency = 0;

	length = 0;
	weightTFIDF = NULL;
	weightJewell = NULL;

	inHyperlinkCnt = 0;

	inTitleCnt = 0;
	inMetaCnt = 0;

	inFromPages = (CItemSet *)new CItemSet();
}
    

CWordSeq::~CWordSeq()
{
	word.Empty();

	if(weightTFIDF)
		delete weightTFIDF;

	if(weightJewell)
		delete weightJewell;

	if(inFromPages)
		delete inFromPages;
}

CLilacObject * CWordSeq::clone()
{
	CWordSeq *theclone;
        
	theclone = (CWordSeq *)new CWordSeq();
	theclone->setWord(word);
	theclone->frequency = frequency;

	theclone->length = length;
	if(length)
	{
		int index;

		theclone->weightTFIDF = (double *)new double[length];
		for(index = 0; index < length; index++)
			theclone->weightTFIDF[index] = weightTFIDF[index];

		theclone->weightJewell = (double *)new double[length];
		for(index = 0; index < length; index++)
			theclone->weightJewell[index] = weightJewell[index];

	}

	theclone->inHyperlinkCnt = inHyperlinkCnt;

	theclone->inTitleCnt = inTitleCnt;
	theclone->inMetaCnt = inMetaCnt;

	theclone->inFromPages = (CItemSet *)inFromPages->clone();

	return theclone; 
}

void CWordSeq::setWord(const CString& value)
{
	word = value;
}
    
void CWordSeq::setWord(const char *value)
{
	word = value;
}

CString& CWordSeq::getWord()
{
	return word;
}


void CWordSeq::setWeightSize(int size)
{
	length = size;

	if(length > 0)
	{
		int index;

		if(weightTFIDF)
			delete weightTFIDF;
		weightTFIDF = (double *)new double[length];
		for(index = 0; index < length; index++)
			weightTFIDF[index] = 0.0;

		if(weightJewell)
			delete weightJewell;
		weightJewell = (double *)new double[length];
		for(index = 0; index < length; index++)
			weightJewell[index] = 0.0;
	}
}

int CWordSeq::key()
{
	/*
	int  g, i, h = 0;

	if(!word.IsEmpty())
	{
		for(i = 0; i < word.GetLength(); i++)
		{
			h = (h << 2) + tolower(word[i]);
			if ((g = h & 0xf0000000L) != 0)
				h = (h ^ (g >> 24)) ^ g;
		}
	}

	return h;
*/
  unsigned  int  hash  =   0 ; 
  unsigned  int  x     =   0 ; 
  if(!word.IsEmpty())
  {
       for(int i = 0; i < word.GetLength(); i++)
	   { 
        hash  =  (hash << 4 )  +   word[i] ; //hash值左移4位加上一个字符
        if  ((x  =  hash  &   0xF0000000L )  !=   0 )//判断hash值的高4位是否不为0，因为不为0时需要下面特殊处理，否则上面一步的左移4位会把这高四位给移走，造成信息丢失
		{ 
        hash  ^=  (x >> 24 );   //把刚才的高4位跟hash的低5-8位异或
        hash  &=   ~ x;            //把高4位清0
		}  
	   }  
  }
  return  (hash  &   0x7FFFFFFF ) % 100; //希望hash值是一个非负数

}

int CWordSeq::compare(CLilacObject *obj)
{
	CWordSeq *hopsobj = (CWordSeq *)obj;
	return word.Compare(hopsobj->getWord());
}


void CWordSeq::dump()
{
	m_LogTrace.WriteLine("\n\n");
	m_LogTrace.WriteLine("Word : %s\n", word);
	m_LogTrace.WriteLine("Frequency : %d\n", frequency);

	if(length > 0)
	{
		int i;

		m_LogTrace.WriteLine("TF*IDF Weight :\n");
		for(i = 0; i < length; i++)
			m_LogTrace.WriteLine(" %f ", weightTFIDF[i]);
		m_LogTrace.WriteLine("\n");

		m_LogTrace.WriteLine("Jewell Weight :\n");
		for(i = 0; i < length; i++)
			m_LogTrace.WriteLine(" %f ", weightJewell[i]);
		m_LogTrace.WriteLine("\n");
	}

	m_LogTrace.WriteLine("inHyperlinkCnt         : %d\n", inHyperlinkCnt);

	m_LogTrace.WriteLine("inTitleCnt : %d\n", inTitleCnt);
	m_LogTrace.WriteLine("inMetaCnt  : %d\n", inMetaCnt);

	m_LogTrace.WriteLine("From Page  :\n");
	if(inFromPages)
	{
		for(int j = 0; j < inFromPages->size(); j++)
			m_LogTrace.WriteLine(" %d ", inFromPages->get(j));
		m_LogTrace.WriteLine("\n\n");
	}
}

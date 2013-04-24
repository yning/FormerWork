///////////////////////////////////////////////////////////////
// TextProcessor.cpp: implementation of the CTextProcessor class.
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TextProcessor.h"
#include "MainFrm.h"
//#include "Dictionary.h"
//#include "Segment.h"

//#pragma comment(lib,"D:\\WebMindMerge1.2\\Debug\\Dict.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextProcessor::CTextProcessor(const char* corpus_file)//calculate TF*IDF for terms
{
	m_envCorpus = (CCorpus *)NULL;
	m_envStemmer = (CPorterStemming *)NULL;
	m_envPuncmarks = (CPuncMark *)NULL;
	m_envStopword = (CStopWord *)NULL;

	m_wordMapper = NULL;

	m_corpusFile = corpus_file;

	m_bInited = false;

	initEnv();
}

CTextProcessor::~CTextProcessor()
{
	deInitEnv();
}

double CTextProcessor::lookupIDF(const char *word)
{
	return m_envCorpus->lookupIDF(word);
}


void CTextProcessor::initEnv()
{
	if(m_envCorpus == (CCorpus *)NULL)
		m_envCorpus = (CCorpus *) new CCorpus(m_corpusFile);

	if(m_envStemmer == (CPorterStemming *)NULL)
		m_envStemmer = (CPorterStemming *) new CPorterStemming();

	if(m_envPuncmarks == (CPuncMark *)NULL)
		m_envPuncmarks = (CPuncMark *) new CPuncMark();

	if(m_envStopword == (CStopWord *)NULL)
		m_envStopword = (CStopWord *) new CStopWord();

	if(m_bMapping)
	{
		if(m_wordMapper)
			delete m_wordMapper;
		m_wordMapper = (CWordMapping *) new CWordMapping();
	}

	m_bInited = true;
}

void CTextProcessor::deInitEnv()
{
	if(m_envCorpus != (CCorpus *)NULL)
		delete m_envCorpus;

	if(m_envStemmer != (CPorterStemming *)NULL)
		delete m_envStemmer;

	if(m_envPuncmarks != (CPuncMark *)NULL)
		delete m_envPuncmarks;

	if(m_envStopword != (CStopWord *)NULL)
		delete m_envStopword;

	if(m_bMapping)
	{
		if(m_wordMapper)
			delete m_wordMapper;
		m_wordMapper = NULL;
	}

	m_bInited = false;
}


// this function takes a string as input, converts characters to lower case;  
// removes all numbers and stopwords, then extract all words from it; removes 
// all stopwords. 
// wordnum is the number of words within the body text
void CTextProcessor::textProcess(const CString& strText, CStringArray& m_stemWords)
{
/*
  TCHAR szFull[_MAX_PATH];   
  TCHAR szDrive[_MAX_DRIVE];   
  TCHAR szDir[_MAX_DIR];   
  ::GetModuleFileName(NULL, szFull, sizeof(szFull)/sizeof(TCHAR));   
  _tsplitpath(szFull, szDrive, szDir, NULL, NULL);   
  _tcscpy(szFull, szDrive);   
  _tcscat(szFull, szDir);  
*/
    char* pt;
	char *s;
    char* word;
    char* str;
	CString strBuffer;
    
    int len;
    int wd_sz = 0;
	CString original;

	strBuffer = strText;
	if(strBuffer.IsEmpty())
		return;
	
	if(!m_bInited)
		initEnv();

	s = (char *) new char[2*strText.GetLength() + 1];
	strcpy(s, (LPCSTR)strBuffer);
	char *temp=s;
    //remove all numbers and punctuation marks in the gicen string "s".
	m_envPuncmarks->RemovePuncMark(s);

	//read the file that need to be segmented
	seg.LoadContent(s);
	//segmentation
	int iSegOutSize = 0;//length of the string
	char *cSegOut = seg.TextSegment(iSegOutSize);

	//str is the result of segmentation
    s=cSegOut;
	
	//FILE *fpout = fopen(sFile,"w");
	//fwrite(cSegOut,sizeof(char),iSegOutSize,fpout);
//	fclose(fpout);	

	//release the memory
	if (*s == '\0')
	{
		//printf("Fail load Data/CoreDict.pdat!");
	//	delete pDict;//comment by ny
	//	return 1;

	}
	
//until here

    // extract a word from string "str", ONE WORD at a time, process it, store it 
    // in a hashtable, and then extract another word.
	str = s;
    pt = str;

	/*
   const char filename[] = "D:\\WebMindMerge1.2\\Debug\\test.txt";
    ofstream o_file;
   // ifstream i_file;
   // string out_text;
   
    //write file
   o_file.open(filename);
    o_file << (LPCTSTR) str;
    o_file.close();
	*/


    while(*str != '\0')
    {
        if(*str != ' ')
        {
            str++;
        }
        else if(*str == ' ')
        {
            len = str - pt;
            if(len > 0)
            {
                word = (char *) new char[len+1];
     
                while(*pt != ' ')
                {
                    word[wd_sz] = *pt;
                    wd_sz++;
                    pt++;
                }
                
                word[wd_sz] = '\0';
                str++;
                pt = str; //finishing extracting a word

				// convert all the characters in the word into lower case
				//strlwr(word);

                // if the extracted word is a stopword, stop processing. 
                // Otherwise, continuing processing this word.

                if(!m_envStopword->isStopWord(word))
                {
					//if the word is keyword
					original = word;

					m_envStemmer->Stem(word);
					if(word!=NULL&&(word[0] >= 'a' && word[0] <= 'z') || (word[0] >= 'A' && word[0] <= 'Z'))
					if(!m_envStopword->isStopWord(word) && strlen(word) > 2)
						//record the original and stemmed word
						if(m_bMapping)
							m_wordMapper->insertStemWord(word, original);

					m_stemWords.Add(word);
               
                } // end if(stopper(word) == 0) statement

                delete word;
            }
            else
            {  // if(len == 0)
                str++; 
                pt++;
            }
        } //end of else if(*str == ' ') statement 
        
        wd_sz = 0;

    } //end of while loop

    len = str - pt;
    if(len > 0)
    {
        wd_sz = 0;
        word = (char *) new char[len+1];
     
        while(*pt != '\0')
        {
            word[wd_sz] = *pt;
            wd_sz++;
            pt++;
        }
        word[wd_sz] = '\0';

        // if the extracted word is a stopword, stop processing. 
        // Otherwise, continuing processing this word.

        if(!m_envStopword->isStopWord(word) && strlen(word) > 2)
        { //if the word is keyword

            m_envStemmer->Stem(word);
            m_stemWords.Add(word);
               
        } // end if(stopper(word) == 0) statement
                
        delete word;
    }

//	delete []s;

    return;
} // The End

// this function takes a string as input, converts characters to lower case;  
// removes all numbers and stopwords, then extract all words from it; removes 
// all stopwords. 
// wordnum is the number of words within the body text
//Only handle the text in URL
void CTextProcessor::textProcessU(const CString& strText, CStringArray& m_stemWords)
{
    char* pt;
	char *s;
    char* word;
    char* str;
	CString strBuffer;
    
    int len;
    int wd_sz = 0;
	CString original;

	strBuffer = strText;
	if(strBuffer.IsEmpty())
		return;
	
	if(!m_bInited)
		initEnv();

	s = (char *) new char[strText.GetLength() + 1];
	strcpy(s, (LPCSTR)strBuffer);

    //remove all numbers and punctuation marks in the gicen string "s".
	m_envPuncmarks->RemovePuncMark(s);

    // extract a word from string "str", ONE WORD at a time, process it, store it 
    // in a hashtable, and then extract another word.
	str = s;
    pt = str;

    while(*str != '\0')
    {
        if(*str != ' ')
        {
            str++;
        }
        else if(*str == ' ')
        {
            len = str - pt;
            if(len > 0)
            {
                word = (char *) new char[len+1];
     
                while(*pt != ' ')
                {
                    word[wd_sz] = *pt;
                    wd_sz++;
                    pt++;
                }
                
                word[wd_sz] = '\0';
                str++;
                pt = str; //finishing extracting a word

				// convert all the characters in the word into lower case
				//strlwr(word);

                // if the extracted word is a stopword, stop processing. 
                // Otherwise, continuing processing this word.

                if(!m_envStopword->isStopWord(word))
                {
					//if the word is keyword
					original = word;

					m_envStemmer->Stem(word);
					
					if(!m_envStopword->isStopWord(word) && strlen(word) > 2)
					{
					 if(word!=NULL&&(word[0] >= 'a' && word[0] <= 'z') || (word[0] >= 'A' && word[0] <= 'Z'))
						//record the original and stemmed word
						if(m_bMapping)
							m_wordMapper->insertStemWord(word, original);

						m_stemWords.Add(word);
					}
               
                } // end if(stopper(word) == 0) statement

                delete word;
            }
            else
            {  // if(len == 0)
                str++; 
                pt++;
            }
        } //end of else if(*str == ' ') statement 
        
        wd_sz = 0;

    } //end of while loop

    len = str - pt;
    if(len > 0)
    {
        wd_sz = 0;
        word = (char *) new char[len+1];
     
        while(*pt != '\0')
        {
            word[wd_sz] = *pt;
            wd_sz++;
            pt++;
        }
        word[wd_sz] = '\0';

        // if the extracted word is a stopword, stop processing. 
        // Otherwise, continuing processing this word.

        if(!m_envStopword->isStopWord(word) && strlen(word) > 2)
        { //if the word is keyword
	        if(word!=NULL&&(word[0] >= 'a' && word[0] <= 'z') || (word[0] >= 'A' && word[0] <= 'Z'))
                m_envStemmer->Stem(word);
            m_stemWords.Add(word);
               
        } // end if(stopper(word) == 0) statement
                
        delete word;
    }

//	delete s;

    return;
} // The End

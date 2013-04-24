///////////////////////////////////////////////////////////////
// WordSession.cpp: implementation of the CWordSession class.
//
//
///////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "WordSession.h"
#include "Utility.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWordSession::CWordSession()
{
	word = NULL;

	latestAppearance = 0.0;

	relativeFreq = 0.0;
	ratioOccurences = 0.0;
	seqTFIDFWeight = 0.0;
	absFreq = 0;

	isKeywordCnt = 0;

	skippedTitleCnt = 0;
	skippedSnippetCnt = 0;

	chosenTitleCnt = 0;
	chosenSnippetCnt = 0;

	untouchedTitleCnt = 0;
	untouchedSnippetCnt = 0;

	bkTitleCnt = 0;
	bkSnippetCnt = 0;

    ratioWordAppearance = 0.0;

    avTFIDFWeight = 0.0;
    varTFIDFWeight = 0.0;

    avJewellWeight = 0.0;
    varJewellWeight = 0.0;

    trendTFIDFWeight = 0;
    trendJewellWeight = 0;

    ratioLinkFollow = 0.0;
    ratioFollow = 0.0;
    ratioLinkBack = 0.0;

    ratioBackward = 0.0;

    avTFIDFWeightBackward = 0.0;
    varTFIDFWeightBackward = 0.0;

    avJewellWeightBackward = 0.0;
    varJewellWeightBackward = 0.0;


    ratioForward = 0.0;

    avTFIDFWeightForward = 0.0;
    varTFIDFWeightForward = 0.0;

    avJewellWeightForward = 0.0;
    varJewellWeightForward = 0.0;

    ratioInTitle = 0.0;

    ratioInvisible = 0.0;

	ICword = false;
}

CWordSession::~CWordSession()
{
	if(word)
		delete word;
}

CLilacObject * CWordSession::clone()
{
    CWordSession *theclone;
    
    theclone = (CWordSession *)new CWordSession();

	if(word)
		theclone->Word(word);

	theclone->latestAppearance = latestAppearance;

	theclone->relativeFreq = relativeFreq;
	theclone->ratioOccurences = ratioOccurences;
	theclone->seqTFIDFWeight = seqTFIDFWeight;
	theclone->absFreq = absFreq;

	theclone->isKeywordCnt = isKeywordCnt;

	theclone->skippedTitleCnt = skippedTitleCnt;
	theclone->skippedSnippetCnt = skippedSnippetCnt;

	theclone->chosenTitleCnt = chosenTitleCnt;
	theclone->chosenSnippetCnt = chosenSnippetCnt;

	theclone->untouchedTitleCnt = untouchedTitleCnt;
	theclone->untouchedSnippetCnt = untouchedSnippetCnt;

	theclone->bkTitleCnt = bkTitleCnt;
	theclone->bkSnippetCnt = bkSnippetCnt;

    theclone->ratioWordAppearance = ratioWordAppearance;

    theclone->avTFIDFWeight = avTFIDFWeight;
    theclone->varTFIDFWeight = varTFIDFWeight;

    theclone->avJewellWeight = avJewellWeight;
    theclone->varJewellWeight = varJewellWeight;


    theclone->trendTFIDFWeight = trendTFIDFWeight;
    theclone->trendJewellWeight = trendJewellWeight;

    theclone->ratioLinkFollow = ratioLinkFollow;
    theclone->ratioFollow = ratioFollow;
    theclone->ratioLinkBack = ratioLinkBack;

    theclone->ratioBackward = ratioBackward;

    theclone->avTFIDFWeightBackward = avTFIDFWeightBackward;
    theclone->varTFIDFWeightBackward = varTFIDFWeightBackward;

    theclone->avJewellWeightBackward = avJewellWeightBackward;
    theclone->varJewellWeightBackward = varJewellWeightBackward;


    theclone->ratioForward = ratioForward;

    theclone->avTFIDFWeightForward = avTFIDFWeightForward;
    theclone->varTFIDFWeightForward = varTFIDFWeightForward;

    theclone->avJewellWeightForward = avJewellWeightForward;
    theclone->varJewellWeightForward = varJewellWeightForward;


    theclone->ratioInTitle = ratioInTitle;

    theclone->ratioInvisible = ratioInvisible;

	theclone->ICword = ICword;

    return theclone; 
}

void CWordSession::Word(const char *value)
{
	if(word)
		delete word;
	word = (char *)new char[strlen(value) + 1];
	strcpy(word, value);
}
char * CWordSession::Word()
{
	return word;
}

int CWordSession::key()
{
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

int CWordSession::compare(CLilacObject *obj)
{
    CWordSession *hopsobj;
	
    hopsobj = (CWordSession *)obj;

	if((word != NULL) && (hopsobj->Word() != NULL))
	{
		if(stricmp(word, hopsobj->Word()) == 0)
			return 0;
        else
	        return -1;
	}

	return -1;
}


// calculate the singnificance of the word based on its browsing features
double CWordSession::calWordSignificance()
{
	double score = 0.0;

	score = 
	latestAppearance * 1.000000 +
	relativeFreq * 1.000000 +
	ratioOccurences * 25.000000 +
	seqTFIDFWeight * 18.000000 +
	isKeywordCnt * 21.000000 +
	skippedTitleCnt * 3.000000 +
	skippedSnippetCnt * 1.000000 +
	chosenTitleCnt * 5.000000 +
	chosenSnippetCnt * 7.000000 +
	untouchedTitleCnt * 20.000000 +
	untouchedSnippetCnt * -3.000000 +
	ratioWordAppearance * 10.000000 +
	avTFIDFWeight * -1.000000 +
	avJewellWeight * 29.000000 +
	varJewellWeight * 54.000000 +
	trendTFIDFWeight * -2.000000 +
	ratioLinkFollow * 35.000000 +
	ratioFollow * -7.000000 +
	ratioLinkBack * 5.000000 +
	ratioBackward * -3.000000 +
	varTFIDFWeightBackward * -1.000000 +
	avJewellWeightBackward * 7.000000 +
	varJewellWeightBackward * -17.000000 +
	avJewellWeightForward * -15.000000 +
	varJewellWeightForward * -5.000000;


	return score;
}


double CWordSession::calSignificance(double *weight)
{
	double score = 0.0;

	score = 

		latestAppearance * weight[0] +	      //是否最近出现
		relativeFreq * weight[1] +            //相对频率
		ratioOccurences * weight[2] +	      //出现比率
		seqTFIDFWeight * weight[3] +          //在序列中TFIDF权重
		isKeywordCnt * weight[4] +		      //是否是关键词
		skippedTitleCnt * weight[5] +         //是否出现在被跳过的标题中
		skippedSnippetCnt * weight[6] +	      //是否出现在被跳过的快照中 
		chosenTitleCnt * weight[7] +          //是否出现在被选中的标题中
		chosenSnippetCnt * weight[8] +	      //是否出现在被选中的快照中
		untouchedTitleCnt * weight[9] +       //是否出现在未点击的标题中
		untouchedSnippetCnt * weight[10] +    //是否出现在未点击的快照中	
		bkTitleCnt * weight[11] +             //是否出现在返回的标题中
		bkSnippetCnt * weight[12] +		      //是否出现在返回的快照中
		ratioWordAppearance * weight[13] +    //词条出现的比率
		avTFIDFWeight * weight[14] +	      //TFIDF权重均值
		varTFIDFWeight * weight[15] +         //TFIDF权重方差
		avJewellWeight * weight[16] +		  //Jewell权重均值
		varJewellWeight * weight[17] +        //Jewell权重方差
		trendTFIDFWeight * weight[18] +	      //TFIDF权重趋势
		trendJewellWeight * weight[19] +      //Jewell权重趋势
		ratioLinkFollow * weight[20] +	      //跟进链接的比率
		ratioFollow * weight[21] +	          //跟进内容比率
		ratioLinkBack * weight[22] +          //返回链接比率
		ratioBackward * weight[23] +          //返回比率
		avTFIDFWeightBackward * weight[24] +  //返回的TFIDF权重均值
		varTFIDFWeightBackward * weight[25] + //返回的TFIDF权重方差
		avJewellWeightBackward * weight[26] + //返回的Jewell权重均值	
		varJewellWeightBackward * weight[27] +//返回的Jewell权重方差
		ratioForward * weight[28] +           //向前的比率
		avTFIDFWeightForward * weight[29] +	  //向前的TFIDF权重均值
		varTFIDFWeightForward * weight[30] +  //向前的TFIDF权重方差
		avJewellWeightForward * weight[31] +  //向前的Jewell权重均值	
		varJewellWeightForward * weight[32] + //向前的Jewell权重方差
		ratioInTitle * weight[33] +			  //出现在标题中的比率
		ratioInvisible * weight[34];          //不可见的比率


	return score;
}


double CWordSession::calWordICness()
{
	return calWordICRelevant();

	double score = 0.0;

	/*
	// V 1.3
	score = 
	latestAppearance * 1.000000 +
	relativeFreq * 1.000000 +
	ratioOccurences * 25.000000 +
	seqTFIDFWeight * 18.000000 +
	isKeywordCnt * 21.000000 +
	skippedTitleCnt * 3.000000 +
	skippedSnippetCnt * 1.000000 +
	chosenTitleCnt * 5.000000 +
	chosenSnippetCnt * 7.000000 +
	untouchedTitleCnt * 20.000000 +
	untouchedSnippetCnt * -3.000000 +
	ratioWordAppearance * 10.000000 +
	avTFIDFWeight * -1.000000 +
	avJewellWeight * 29.000000 +
	varJewellWeight * 54.000000 +
	trendTFIDFWeight * -2.000000 +
	ratioLinkFollow * 35.000000 +
	ratioFollow * -7.000000 +
	ratioLinkBack * 5.000000 +
	ratioBackward * -3.000000 +
	varTFIDFWeightBackward * -1.000000 +
	avJewellWeightBackward * 7.000000 +
	varJewellWeightBackward * -17.000000 +
	avJewellWeightForward * -15.000000 +
	varJewellWeightForward * -5.000000;
	*/

	score = 
	latestAppearance * 1.000000 +
	relativeFreq * 1.000000 +
	ratioOccurences * -3.000000 +
	skippedTitleCnt * 1.000000 +
	ratioWordAppearance * 9.000000 +
	avJewellWeight * 1.000000 +
	ratioLinkFollow * -29.000000 +
	varJewellWeightBackward * -4.000000 +
	avTFIDFWeightForward * -1.000000;

	return score;
}


double CWordSession::calWordICRelevant()
{
	double score = 0.0;

	/*
	// V1.2

	score = 
	latestAppearance * 1.000000 +
	relativeFreq * 1.000000 +
	ratioOccurences * 25.000000 +
	seqTFIDFWeight * 18.000000 +
	isKeywordCnt * 21.000000 +
	skippedTitleCnt * 3.000000 +
	skippedSnippetCnt * 1.000000 +
	chosenTitleCnt * 5.000000 +
	chosenSnippetCnt * 7.000000 +
	untouchedTitleCnt * 20.000000 +
	untouchedSnippetCnt * -3.000000 +
	ratioWordAppearance * 10.000000 +
	avTFIDFWeight * -1.000000 +
	avJewellWeight * 29.000000 +
	varJewellWeight * 54.000000 +
	trendTFIDFWeight * -2.000000 +
	ratioLinkFollow * 35.000000 +
	ratioFollow * -7.000000 +
	ratioLinkBack * 5.000000 +
	ratioBackward * -3.000000 +
	varTFIDFWeightBackward * -1.000000 +
	avJewellWeightBackward * 7.000000 +
	varJewellWeightBackward * -17.000000 +
	avJewellWeightForward * -15.000000 +
	varJewellWeightForward * -5.000000;
	*/

	/*
	// V 1.3
	score = 
	latestAppearance * 1.000000 +
	relativeFreq * 1.000000 +
	seqTFIDFWeight * 2.000000 +
	isKeywordCnt * 3.000000 +
	skippedTitleCnt * 1.000000 +
	chosenTitleCnt * 3.000000 +
	chosenSnippetCnt * 2.000000 +
	untouchedTitleCnt * 1.000000 +
	untouchedSnippetCnt * 1.000000 +
	avJewellWeight * 1.000000 +
	varJewellWeight * 4.000000 +
	ratioLinkFollow * 2.000000 +
	ratioFollow * 2.000000 +
	ratioLinkBack * -1.000000 +
	avTFIDFWeightBackward * -1.000000 +
	varTFIDFWeightBackward * -2.000000 +
	varJewellWeightBackward * -4.000000 +
	ratioForward * -1.000000 +
	avTFIDFWeightForward * 1.000000 +
	varTFIDFWeightForward * 1.000000;
	*/

	score = 
	latestAppearance * 1.000000 +
	relativeFreq * 12.000000 +
	seqTFIDFWeight * 13.000000 +
	isKeywordCnt * 8.000000 +
	skippedTitleCnt * 1.000000 +
	chosenTitleCnt * 3.000000 +
	untouchedTitleCnt * 1.000000 +
	ratioWordAppearance * -3.000000 +
	varTFIDFWeight * 3.000000 +
	ratioLinkFollow * 2.000000 +
	ratioFollow * 1.000000 +
	ratioLinkBack * -67.000000 +
	varTFIDFWeightBackward * -2.000000 +
	avJewellWeightBackward * 10.000000 +
	varJewellWeightBackward * -12.000000;

	return score;
}

double CWordSession::calWordICQuery()
{
	double score = 0.0;

	/*
	// V1.2
	score = 
		latestAppearance * 1.000000 +
		relativeFreq * 1.000000 +
		ratioOccurences * -3.000000 +
		isKeywordCnt * 1.000000 +
		chosenTitleCnt * 3.000000 +
		avJewellWeight * 1.000000 +
		trendTFIDFWeight * 2.000000 +
		ratioFollow * -2.000000 +
		ratioLinkBack * -1.000000 +
		varJewellWeightBackward * -1.000000 +
		avJewellWeightForward * 4.000000 +
		varJewellWeightForward * 4.000000;
	*/

	score = 
		latestAppearance * 1.000000 +
		relativeFreq * 1.000000 +
		ratioOccurences * -2.000000 +
		isKeywordCnt * 1.000000 +
		chosenTitleCnt * 1.000000 +
		avJewellWeight * 1.000000 +
		varTFIDFWeight * 1.000000 +
		ratioLinkBack * -9.000000 +
		varTFIDFWeightBackward * -46.000000 +
		avJewellWeightForward * 4.000000 +
		varJewellWeightBackward * 1.000000;

	return score;
}


// print out all the information of the Word Session
void CWordSession::dump()
{
    m_LogTrace.WriteLine("*******************************************************************\n");
    m_LogTrace.WriteLine("               Word : %s\n", word);
	
	m_LogTrace.WriteLine("  Latest Appearance : %lf\n", latestAppearance);

    m_LogTrace.WriteLine("          Frequency : %d\n", absFreq);
    m_LogTrace.WriteLine(" Relative Frequency : %lf\n", relativeFreq);
    m_LogTrace.WriteLine("    Ratio Frequency : %lf\n", ratioOccurences);
    m_LogTrace.WriteLine("         Seq TF*IDF : %lf\n", seqTFIDFWeight);
	
    m_LogTrace.WriteLine("       isKeywordCnt : %d\n", isKeywordCnt);

    m_LogTrace.WriteLine("    skippedTitleCnt : %d\n", skippedTitleCnt);
    m_LogTrace.WriteLine("  skippedSnippetCnt : %d\n", skippedSnippetCnt);

    m_LogTrace.WriteLine("     chosenTitleCnt : %d\n", chosenTitleCnt);
    m_LogTrace.WriteLine("   chosenSnippetCnt : %d\n", chosenSnippetCnt);

    m_LogTrace.WriteLine("  untouchedTitleCnt : %d\n", untouchedTitleCnt);
    m_LogTrace.WriteLine("untouchedSnippetCnt : %d\n", untouchedSnippetCnt);

    m_LogTrace.WriteLine("         bkTitleCnt : %d\n", bkTitleCnt);
    m_LogTrace.WriteLine("       bkSnippetCnt : %d\n", bkSnippetCnt);

    m_LogTrace.WriteLine("ratioWordAppearance : %lf\n", ratioWordAppearance);
    m_LogTrace.WriteLine("   Mean TF*IDF Weight : %lf\n", avTFIDFWeight);
    m_LogTrace.WriteLine("    Var TF*IDF Weight : %lf\n", varTFIDFWeight);

    m_LogTrace.WriteLine("   Mean Jewell Weight : %lf\n", avJewellWeight);
    m_LogTrace.WriteLine("    Var Jewell Weight : %lf\n", varJewellWeight);

    m_LogTrace.WriteLine("  trend TF*IDF Weight : %d\n", trendTFIDFWeight);
    m_LogTrace.WriteLine("  trend Jewell Weight : %d\n", trendJewellWeight);

    m_LogTrace.WriteLine("    ratioLinkFollow : %lf\n", ratioLinkFollow);
    m_LogTrace.WriteLine("        ratioFollow : %lf\n", ratioFollow);
    m_LogTrace.WriteLine("      ratioLinkBack : %lf\n", ratioLinkBack);

    m_LogTrace.WriteLine("      ratioBackward : %lf\n", ratioBackward);

    m_LogTrace.WriteLine("   av TF*IDF WeightBackward : %lf\n", avTFIDFWeightBackward);
    m_LogTrace.WriteLine("  var TF*IDF WeightBackward : %lf\n", varTFIDFWeightBackward);

    m_LogTrace.WriteLine("   av Jewell WeightBackward : %lf\n", avJewellWeightBackward);
    m_LogTrace.WriteLine("  var Jewell WeightBackward : %lf\n", varJewellWeightBackward);

    m_LogTrace.WriteLine("       ratioForward : %lf\n", ratioForward);

    m_LogTrace.WriteLine("    av TF*IDF WeightForward : %lf\n", avTFIDFWeightForward);
    m_LogTrace.WriteLine("   var TF*IDF WeightForward : %lf\n", varTFIDFWeightForward);

    m_LogTrace.WriteLine("    av Jewell WeightForward : %lf\n", avJewellWeightForward);
    m_LogTrace.WriteLine("   var Jewell WeightForward : %lf\n", varJewellWeightForward);

    m_LogTrace.WriteLine("       ratioInTitle : %lf\n", ratioInTitle);
    m_LogTrace.WriteLine("     ratioInvisible : %lf\n", ratioInvisible);
    m_LogTrace.WriteLine("\n");
}


// dump a feature vector of this Word Session
// for debug purpose
void CWordSession::DumpWordSession()
{
    m_LogTrace.WriteLine(" %s", word);

	m_LogTrace.WriteLine(" %d", absFreq);

    m_LogTrace.WriteLine(" %lf", latestAppearance);
    m_LogTrace.WriteLine(" %lf", relativeFreq);
    m_LogTrace.WriteLine(" %lf", ratioOccurences);
    m_LogTrace.WriteLine(" %lf", seqTFIDFWeight);
	
    m_LogTrace.WriteLine(" %d", isKeywordCnt);

    m_LogTrace.WriteLine(" %d", skippedTitleCnt);
    m_LogTrace.WriteLine(" %d", skippedSnippetCnt);

    m_LogTrace.WriteLine(" %d", chosenTitleCnt);
    m_LogTrace.WriteLine(" %d", chosenSnippetCnt);

    m_LogTrace.WriteLine(" %d", untouchedTitleCnt);
    m_LogTrace.WriteLine(" %d", untouchedSnippetCnt);

    m_LogTrace.WriteLine(" %d", bkTitleCnt);
    m_LogTrace.WriteLine(" %d", bkSnippetCnt);

    m_LogTrace.WriteLine(" %lf", ratioWordAppearance);

    m_LogTrace.WriteLine(" %lf", avTFIDFWeight);
    m_LogTrace.WriteLine(" %lf", varTFIDFWeight);

    m_LogTrace.WriteLine(" %lf", avJewellWeight);
    m_LogTrace.WriteLine(" %lf", varJewellWeight);

    m_LogTrace.WriteLine(" %d", trendTFIDFWeight);
    m_LogTrace.WriteLine(" %d", trendJewellWeight);

    m_LogTrace.WriteLine(" %lf", ratioLinkFollow);
    m_LogTrace.WriteLine(" %lf", ratioFollow);
    m_LogTrace.WriteLine(" %lf", ratioLinkBack);

    m_LogTrace.WriteLine(" %lf", ratioBackward);

    m_LogTrace.WriteLine(" %lf", avTFIDFWeightBackward);
    m_LogTrace.WriteLine(" %lf", varTFIDFWeightBackward);

    m_LogTrace.WriteLine(" %lf", avJewellWeightBackward);
    m_LogTrace.WriteLine(" %lf", varJewellWeightBackward);

    m_LogTrace.WriteLine(" %lf", ratioForward);

    m_LogTrace.WriteLine(" %lf", avTFIDFWeightForward);
    m_LogTrace.WriteLine(" %lf", varTFIDFWeightForward);

    m_LogTrace.WriteLine(" %lf", avJewellWeightForward);
    m_LogTrace.WriteLine(" %lf", varJewellWeightForward);

    m_LogTrace.WriteLine(" %lf", ratioInTitle);
    m_LogTrace.WriteLine(" %lf", ratioInvisible);
    m_LogTrace.WriteLine("\n");

}



// print out one word's feature vector into a session file
void CWordSession::outputDataEntry(bool bPositive, FILE *fp_data)
{
	if(bPositive)
	    outputDataEntry(fp_data, _T("+"));
	else
	    outputDataEntry(fp_data, _T("-"));
}


// print out one word's feature vector and its label into a session file
void CWordSession::outputDataEntry(FILE *fp_data, const CString &strLabel)
{
	ASSERT(fp_data);
	if(!fp_data)
		return;

    fprintf(fp_data,"%lf,", latestAppearance);
    fprintf(fp_data,"%lf,", relativeFreq);
    fprintf(fp_data,"%lf,", ratioOccurences);
    fprintf(fp_data,"%lf,", seqTFIDFWeight);

    fprintf(fp_data,"%d,", isKeywordCnt);

    fprintf(fp_data,"%d,", skippedTitleCnt);
    fprintf(fp_data,"%d,", skippedSnippetCnt);

    fprintf(fp_data,"%d,", chosenTitleCnt);
    fprintf(fp_data,"%d,", chosenSnippetCnt);
    
    fprintf(fp_data,"%d,", untouchedTitleCnt);
    fprintf(fp_data,"%d,", untouchedSnippetCnt);

    fprintf(fp_data,"%d,", bkTitleCnt);
    fprintf(fp_data,"%d,", bkSnippetCnt);

    fprintf(fp_data,"%lf,", ratioWordAppearance);

    fprintf(fp_data,"%lf,", avTFIDFWeight);
    fprintf(fp_data,"%lf,", varTFIDFWeight);

    fprintf(fp_data,"%lf,", avJewellWeight);
    fprintf(fp_data,"%lf,", varJewellWeight);

	if(trendTFIDFWeight == -1)
	    fprintf(fp_data,"descend,");
	else if(trendTFIDFWeight == 0)
	    fprintf(fp_data,"steady,");
	else
	    fprintf(fp_data,"ascend,");

	if(trendJewellWeight == -1)
	    fprintf(fp_data,"descend,");
	else if(trendJewellWeight == 0)
	    fprintf(fp_data,"steady,");
	else
	    fprintf(fp_data,"ascend,");


    fprintf(fp_data,"%lf,", ratioLinkFollow);
    fprintf(fp_data,"%lf,", ratioFollow);
    fprintf(fp_data,"%lf,", ratioLinkBack);

    fprintf(fp_data,"%lf,", ratioBackward);

    fprintf(fp_data,"%lf,", avTFIDFWeightBackward);
    fprintf(fp_data,"%lf,", varTFIDFWeightBackward);

    fprintf(fp_data,"%lf,", avJewellWeightBackward);
    fprintf(fp_data,"%lf,", varJewellWeightBackward);

    fprintf(fp_data,"%lf,", ratioForward);

    fprintf(fp_data,"%lf,", avTFIDFWeightForward);
    fprintf(fp_data,"%lf,", varTFIDFWeightForward);

    fprintf(fp_data,"%lf,", avJewellWeightForward);
    fprintf(fp_data,"%lf,", varJewellWeightForward);

    fprintf(fp_data,"%lf,", ratioInTitle);

    fprintf(fp_data,"%lf,", ratioInvisible);

	// print out the label
	fprintf(fp_data, "%s\n", strLabel);

    fflush(fp_data);
}

// only print out the feature values
void CWordSession::outputFeatures(FILE *fp_data)
{
	ASSERT(fp_data);
	if(!fp_data)
		return;

    fprintf(fp_data,"%lf,", latestAppearance);
    fprintf(fp_data,"%lf,", relativeFreq);
    fprintf(fp_data,"%lf,", ratioOccurences);
    fprintf(fp_data,"%lf,", seqTFIDFWeight);

    fprintf(fp_data,"%d,", isKeywordCnt);

    fprintf(fp_data,"%d,", skippedTitleCnt);
    fprintf(fp_data,"%d,", skippedSnippetCnt);

    fprintf(fp_data,"%d,", chosenTitleCnt);
    fprintf(fp_data,"%d,", chosenSnippetCnt);
    
    fprintf(fp_data,"%d,", untouchedTitleCnt);
    fprintf(fp_data,"%d,", untouchedSnippetCnt);

    fprintf(fp_data,"%d,", bkTitleCnt);
    fprintf(fp_data,"%d,", bkSnippetCnt);

    fprintf(fp_data,"%lf,", ratioWordAppearance);

    fprintf(fp_data,"%lf,", avTFIDFWeight);
    fprintf(fp_data,"%lf,", varTFIDFWeight);

    fprintf(fp_data,"%lf,", avJewellWeight);
    fprintf(fp_data,"%lf,", varJewellWeight);

	if(trendTFIDFWeight == -1)
	    fprintf(fp_data,"descend,");
	else if(trendTFIDFWeight == 0)
	    fprintf(fp_data,"steady,");
	else
	    fprintf(fp_data,"ascend,");

	if(trendJewellWeight == -1)
	    fprintf(fp_data,"descend,");
	else if(trendJewellWeight == 0)
	    fprintf(fp_data,"steady,");
	else
	    fprintf(fp_data,"ascend,");


    fprintf(fp_data,"%lf,", ratioLinkFollow);
    fprintf(fp_data,"%lf,", ratioFollow);
    fprintf(fp_data,"%lf,", ratioLinkBack);

    fprintf(fp_data,"%lf,", ratioBackward);

    fprintf(fp_data,"%lf,", avTFIDFWeightBackward);
    fprintf(fp_data,"%lf,", varTFIDFWeightBackward);

    fprintf(fp_data,"%lf,", avJewellWeightBackward);
    fprintf(fp_data,"%lf,", varJewellWeightBackward);

    fprintf(fp_data,"%lf,", ratioForward);

    fprintf(fp_data,"%lf,", avTFIDFWeightForward);
    fprintf(fp_data,"%lf,", varTFIDFWeightForward);

    fprintf(fp_data,"%lf,", avJewellWeightForward);
    fprintf(fp_data,"%lf,", varJewellWeightForward);

    fprintf(fp_data,"%lf,", ratioInTitle);

    fprintf(fp_data,"%lf,", ratioInvisible);

    fflush(fp_data);
}




// convert to a vector in the DataSet at the position (index)
void CWordSession::fillDataEntry(CDataSet *pData, int index)
{
	int attindex, allindex;

	if(pData->m_attrs.GetSize() <= 0)
		return;

	attindex = 0;
	allindex = 0;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = latestAppearance;
		attindex++;
	}
	allindex++;


	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = relativeFreq;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = ratioOccurences;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = seqTFIDFWeight;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = (double)isKeywordCnt;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = (double)skippedTitleCnt;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = (double)skippedSnippetCnt;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = (double)chosenTitleCnt;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = (double)chosenSnippetCnt;
		attindex++;
	}
	allindex++;
    
	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = (double)untouchedTitleCnt;
		attindex++;
	}
	allindex++;


	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = (double)untouchedSnippetCnt;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = (double)bkTitleCnt;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = (double)bkSnippetCnt;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = ratioWordAppearance;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = avTFIDFWeight;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = varTFIDFWeight;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = avJewellWeight;
		attindex++;
	}
	allindex++;


	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = varJewellWeight;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = (double)(trendTFIDFWeight+1);
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = (double)(trendJewellWeight+1);
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = ratioLinkFollow;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = ratioFollow;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = ratioLinkBack;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = ratioBackward;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = avTFIDFWeightBackward;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = varTFIDFWeightBackward;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = avJewellWeightBackward;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = varJewellWeightBackward;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = ratioForward;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = avTFIDFWeightForward;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = varTFIDFWeightForward;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = avJewellWeightForward;
		attindex++;
	}
	allindex++;


	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = varJewellWeightForward;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = ratioInTitle;
		attindex++;
	}
	allindex++;

	if(! pData->m_attrs[allindex].isIgnore())
	{
		pData->m_data[index][attindex] = ratioInvisible;
		attindex++;
	}
	allindex++;

	if(ICword)
		pData->m_data[index][attindex] = 1.0;
	else
		pData->m_data[index][attindex] = 0.0;
}

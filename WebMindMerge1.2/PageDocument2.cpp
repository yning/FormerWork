///////////////////////////////////////////////////////////////
// PageDocument2.cpp: implementation of the CPageDocument2 class.
//
//
///////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "PageDocument2.h"
#include "PageRepository.h"

#include <math.h>

#ifdef   _DEBUG 
#define  new   DEBUG_NEW 
#undef   THIS_FILE 
static char THIS_FILE[] = __FILE__; 
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPageDocument2::CPageDocument2()
{
	m_hPageWords = NULL;

	m_envTextProcessor = NULL;

	m_roleColor.RemoveAll();
	m_roleBGColor.RemoveAll();

	m_roleFontFamily.RemoveAll();
	m_roleFontSize.RemoveAll();
	m_roleFontStyle.RemoveAll();
	m_roleFontWeight.RemoveAll();
}

CPageDocument2::~CPageDocument2()
{
	Reset();
}


void CPageDocument2::Reset()
{
	if(m_hPageWords)
	{
		delete m_hPageWords;
		m_hPageWords = NULL;
	}

	m_roleColor.RemoveAll();
	m_roleBGColor.RemoveAll();

	m_roleFontFamily.RemoveAll();
	m_roleFontSize.RemoveAll();
	m_roleFontStyle.RemoveAll();
	m_roleFontWeight.RemoveAll();
}

CPageWord *CPageDocument2::lookup(const CString &szWord)
{
	if(!m_hPageWords)
		return NULL;

	CPageWord *pWord;
	CPageWord *pt;

	pWord = (CPageWord *)new CPageWord(szWord);

	// look up the word in the Hash table
	pt = (CPageWord *)m_hPageWords->lookup(pWord);

	delete pWord;

	return pt;
}


bool CPageDocument2::calLayoutRole(CPageDocument *pageTree)
{
//	CMainFrame * pMainFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	if(!pageTree || !m_envTextProcessor)
		return false;

//	CTextProcessor *m_envTextProcessor;
//	m_envTextProcessor = pageTree->getRepository()->m_envTextProcessor;
//	if(!m_envTextProcessor)
//		return false;

	CPageTagNode *root;
	int numWords;

	m_hPageWords = (Hash *)new Hash();

	if((root = pageTree->getDocument()) != NULL)
	{
		CPageTagNode *body;
	    if((body = root->FindBody()) != NULL)
   		{
			///////////////////////////////////////////////////////////////////

			// scatter the background settings
			if(!pageTree->m_clrBackGround.IsEmpty())
			{
				if(body->getBGColor().CompareNoCase(_T("transparent")) == 0)
					body->setBGColor(pageTree->m_clrBackGround);
			}
			ScatterBackGroundColor(body);

			///////////////////////////////////////////////////////////////////

			getNodeWords(body);
			numWords = body->m_numWords;

			calRoleDistribution(body);

			///////////////////////////////////////////////////////////////////

			calRoleValues(body);

			///////////////////////////////////////////////////////////////////
			//calEntropyTime = CHighTime::GetPresentTime();

			calEntropy(m_roleColor, numWords);
			calEntropy(m_roleBGColor, numWords);

			calEntropy(m_roleFontFamily, numWords);
			calEntropy(m_roleFontSize, numWords);
			calEntropy(m_roleFontStyle, numWords);
			calEntropy(m_roleFontWeight, numWords);

			///////////////////////////////////////////////////////////////////

			calWordRole(body);
//			dump();

			///////////////////////////////////////////////////////////////////
		}
	}

	CPageWord *pWord;
	m_hPageWords->resetSeek();
	while( (pWord = (CPageWord *)m_hPageWords->getObject()) != NULL )
		pWord->computeScore();


	return true;
}


// convert the transparent background color into a real color
// if the background color of parent node is not transparent,
//  we will change the child node with transparent background color to the parent's color
void CPageDocument2::ScatterBackGroundColor(CPageTagNode *pNode)
{
	ASSERT(pNode != NULL);

	CPageTagNode *content;
	if(!pNode->getBGColor().IsEmpty() && pNode->getBGColor().CompareNoCase(_T("transparent")) != 0)
	{
		for(content = pNode->getContent(); content != NULL; content = content->getNext())
		{
			if(content->getBGColor().CompareNoCase(_T("transparent")) == 0)
				content->setBGColor(pNode->getBGColor());
		}
	}

	for(content = pNode->getContent(); content != NULL; content = content->getNext())
		ScatterBackGroundColor(content);
}

void CPageDocument2::getNodeWords(CPageTagNode *pNode)
{
	ASSERT(pNode && m_envTextProcessor);

	CString strText;

	// at first, let's get all the text of this node,
	// process all the words, and insert the words into a hash table
	strText.Empty();
	pNode->getText(strText);
	if(!strText.IsEmpty())
	{
		CStringArray m_stemWords;

		if(pNode->m_hWords)
			pNode->m_hWords->clear();
		else
			pNode->m_hWords = (Hash *) new Hash();

		m_envTextProcessor->textProcess(strText, m_stemWords);
		pNode->m_numWords = m_stemWords.GetSize();
		for(int i = 0; i < m_stemWords.GetSize(); i++)
		{
//			m_LogTrace.WriteLine("%s\n", m_stemWords[i]);
			InsertWord(pNode->m_hWords, m_stemWords[i]);
		}

		m_stemWords.RemoveAll();
	}
}

void CPageDocument2::calRoleDistribution(CPageTagNode *pNode)
{
	if(pNode->getContent() == NULL)
		return;

	// if it has child nodes, process all its child nodes
	CPageTagNode *content;
	for(content = pNode->getContent(); content != NULL; content = content->getNext())
	{
		getNodeWords(content);
		minusElementWords(pNode->m_hWords, content->m_hWords);
	}

	for(content = pNode->getContent(); content != NULL; content = content->getNext())
		calRoleDistribution(content);
}


// insert a word into the word hash table
void CPageDocument2::InsertWord(Hash *hWords, const CString &strWord)
{
	if(!hWords)
		return;

	CElementWord *ptank;
	CElementWord *pt;

	// look up the word in the Hash table
	ptank = (CElementWord *)new CElementWord(strWord);
	if((pt = (CElementWord *)hWords->lookup(ptank)) != NULL)
	{
		// if it is there, just increase its occurrence number,
		// and tag count
		pt->setFreq(ptank->getFreq() + pt->getFreq());
		delete ptank;
	}
	else
	{
		// otherwise, insert it into the Hash table
		hWords->insert(ptank);
	}
}


// minus the parent node's word-count 
void CPageDocument2::minusElementWords(Hash *pWords, Hash *cWords)
{
	if(!pWords || !cWords)
		return;

	CElementWord *pElemWord;
	cWords->resetSeek();
	while( (pElemWord = (CElementWord *)cWords->getObject()) != NULL )
		minusWordFreq(pWords, pElemWord);


}

void CPageDocument2::minusWordFreq(Hash *pWords, CElementWord *pElemWord)
{
	CElementWord *pt;

	// look up the word in the Hash table
	// if it is there, just decrease its occurrence number,
	if((pt = (CElementWord *)pWords->lookup(pElemWord)) != NULL)
	{
		pt->setFreq(pt->getFreq() - pElemWord->getFreq());

		if(pt->getFreq() <= 0)
			pt->setFreq(0);
	}

}

void CPageDocument2::calRoleValues(CPageTagNode *pNode)
{
	int cntWords;

	cntWords = countElementWords(pNode);
	if(cntWords > 0)
	{
		addRoleInstance(m_roleColor, pNode->getColor(), cntWords);
		addRoleInstance(m_roleBGColor, pNode->getBGColor(), cntWords);

		addRoleInstance(m_roleFontFamily, pNode->getFontFamily(), cntWords);
		addRoleInstance(m_roleFontSize, pNode->getFontSize(), cntWords);
		addRoleInstance(m_roleFontStyle, pNode->getFontStyle(), cntWords);
		addRoleInstance(m_roleFontWeight, pNode->getFontWeight(), cntWords);
	}

	CPageTagNode *content;
	for(content = pNode->getContent(); content != NULL; content = content->getNext())
		calRoleValues(content);

}


// cound the number of the word for each node
int CPageDocument2::countElementWords(CPageTagNode *pNode)
{
	if(!pNode)
		return 0;

	Hash *hWords = pNode->m_hWords;
	if(!hWords || IsSkipTag(pNode))
		return 0;

	CElementWord *pElemWord;
	int cnt, sum = 0;
	hWords->resetSeek();
	while( (pElemWord = (CElementWord *)hWords->getObject()) != NULL )
	{
		if((cnt = pElemWord->getFreq()) > 0)
			sum += cnt;
	}

	return sum;
}

void CPageDocument2::addRoleInstance(CArray<CRoleValue, CRoleValue&> &roleInst, const CString &strInstance, int cntWords)
{
	if(strInstance.IsEmpty())
		return;

	for(int i = 0; i < roleInst.GetSize(); i++)
	{
		if(strInstance.CompareNoCase(roleInst[i].m_valRole) == 0)
		{
			roleInst[i].m_pInstances.Add(cntWords);
			return;
		}
	}

	CRoleValue instValue(strInstance, cntWords);
	roleInst.Add(instValue);
}

void CPageDocument2::calEntropy(CArray<CRoleValue, CRoleValue&> &roleInst, int numWords)
{
	double sum, prob;
//	FILE *fp;
//	fp = fopen("entropy.txt", "at");

	for(int i = 0; i < roleInst.GetSize(); i++)
	{
		sum = 0.0;
		for(int j = 0; j < roleInst[i].m_pInstances.GetSize(); j++)
		{
			prob = (double)roleInst[i].m_pInstances[j]/(double)numWords;
			sum += -1.0*prob*log(prob);
		}

		roleInst[i].m_entropy = 0.0;
		if(sum > 0.0)
			roleInst[i].m_entropy = 1.0/sum;

//		fprintf(fp, "%s %lf\n", roleInst[i].m_valRole, sum);

	}

//	fprintf(fp, "\n\n");
//	fflush(fp);
//	fclose(fp);
}


bool CPageDocument2::IsSkipTag(CPageTagNode *pNode)
{
	ASSERT(pNode);

	CString strTag;
	pNode->getName(strTag);
	if(strTag.CompareNoCase("table") == 0 ||
		strTag.CompareNoCase("tr") == 0 ||
		strTag.CompareNoCase("select") == 0 ||
		strTag.CompareNoCase("option") == 0)
		return true;

	return false;
}


// calculate the weight for each word in the node
void CPageDocument2::calWordRole(CPageTagNode *pNode)
{
	Hash *hWords = pNode->m_hWords;
	CElementWord *pElemWord;

	if(!IsSkipTag(pNode) && hWords)
	{
		hWords->resetSeek();
		while( (pElemWord = (CElementWord *)hWords->getObject()) != NULL )
		{
			if(pElemWord->getFreq() > 0)
			{
				InsertPageWord( pElemWord->getWord(),
					GetWeight(m_roleColor, pNode->getColor()),
					GetWeight(m_roleBGColor, pNode->getBGColor()),
					GetWeight(m_roleFontFamily, pNode->getFontFamily()),
					GetWeight(m_roleFontSize, pNode->getFontSize()),
					GetWeight(m_roleFontStyle, pNode->getFontStyle()),
					GetWeight(m_roleFontWeight, pNode->getFontWeight()) );
			}
		}
	}

	CPageTagNode *content;
	for(content = pNode->getContent(); content != NULL; content = content->getNext())
		calWordRole(content);

	return;
}


double CPageDocument2::GetWeight(const CArray<CRoleValue, CRoleValue&> &roleInst, const CString &strInstance)
{
	if(roleInst.GetSize() <= 1 || strInstance.IsEmpty())
		return 0.0;

	for(int i = 0; i < roleInst.GetSize(); i++)
	{
		if(strInstance.CompareNoCase(roleInst[i].m_valRole) == 0)
			return roleInst[i].m_entropy;
	}

	return 0.0;
}

void CPageDocument2::statWeight(const CArray<double, double&> &arValue, double &lfMin, double &lfMean)
{
	lfMin = 0.0;
	lfMean = 0.0;

	if(arValue.GetSize() == 0)
		return;

	if(arValue.GetSize() == 1)
	{
		lfMin = arValue[0];
		lfMean = arValue[0];

		return;
	}

	double sum = 0.0, vMin = 9999.99;
	for(int i = 0; i < arValue.GetSize(); i++)
	{
		sum += arValue[i];
		if(vMin >= arValue[i])
			vMin = arValue[i];
	}

	lfMin = vMin;
	lfMean = sum/(double)arValue.GetSize();
}


void CPageDocument2::InsertPageWord(const CString &strWord, double dClr, double dBGClr, double dFamily, double dSize, double dStyle, double dWeight)
{
//	FILE *fp = fopen("log.txt", "at");
//	fprintf(fp, "%s %lf %lf %lf %lf %lf %lf\n", strWord, dClr, dBGClr, dFamily, dSize, dStyle, dWeight);
//	fclose(fp);

	CPageWord *pWord;
	CPageWord *pt;

	pWord = (CPageWord *)new CPageWord(strWord);

	// look up the word in the Hash table
	if((pt = (CPageWord *)m_hPageWords->lookup(pWord)) != NULL)
	{
		delete pWord;
	}
	else
	{
		// otherwise, insert it into the Hash table
		m_hPageWords->insert(pWord);
		pt = pWord;
	}

	if(dClr != 0.0)
		pt->m_arColor.Add(dClr);

	if(dBGClr != 0.0)
		pt->m_arBGColor.Add(dBGClr);

	if(dFamily != 0.0)
		pt->m_arFontFamily.Add(dFamily);

	if(dSize != 0.0)
		pt->m_arFontSize.Add(dSize);

	if(dStyle != 0.0)
		pt->m_arFontStyle.Add(dStyle);

	if(dWeight != 0.0)
		pt->m_arFontWeight.Add(dWeight);
}

void CPageDocument2::dumpBody(CPageTagNode *pNode)
{
	Hash *hWords = pNode->m_hWords;
	CElementWord *pElemWord;

	if(hWords)
	{
		FILE *fp = fopen("log.txt", "at");
		CString strName;

		hWords->resetSeek();
		while( (pElemWord = (CElementWord *)hWords->getObject()) != NULL )
		{
			pNode->getName(strName);
			fprintf(fp, "<%s> %s %d\n", strName, pElemWord->getWord(), pElemWord->getFreq());
			fflush(fp);
		}

		fclose(fp);
	}

	CPageTagNode *content;
	for(content = pNode->getContent(); content != NULL; content = content->getNext())
		dumpBody(content);
}


void CPageDocument2::dump()
{
	if(!m_hPageWords)
		return;

	FILE *fp;
	int i;
	fp = fopen("entropy.html", "wt");

	fprintf(fp, "<html>\n<head></head>\n<body>\n");

	fprintf(fp, "<h1>Fore-ground Color</h1>\n");
	for(i = 0; i < m_roleColor.GetSize(); i++)
		fprintf(fp, "<font style=color:%s;>%lf</font>&nbsp;%s<br>\n", m_roleColor[i].m_valRole, m_roleColor[i].m_entropy, m_roleColor[i].m_valRole);


	fprintf(fp, "<h1>Back-ground Color</h1>\n");
	for(i = 0; i < m_roleBGColor.GetSize(); i++)
		fprintf(fp, "<font style=background-color:%s;>%lf</font>&nbsp;%s<br>\n", m_roleBGColor[i].m_valRole, m_roleBGColor[i].m_entropy, m_roleBGColor[i].m_valRole);

	fprintf(fp, "<h1>Font-Family</h1>\n");
	for(i = 0; i < m_roleFontFamily.GetSize(); i++)
		fprintf(fp, "<font style=font-family:%s;>%lf</font>&nbsp;%s<br>\n", m_roleFontFamily[i].m_valRole, m_roleFontFamily[i].m_entropy, m_roleFontFamily[i].m_valRole);

	fprintf(fp, "<h1>Font-Size</h1>\n");
	for(i = 0; i < m_roleFontSize.GetSize(); i++)
		fprintf(fp, "<font style=font-size:%s;>%lf</font>&nbsp;%s<br>\n", m_roleFontSize[i].m_valRole, m_roleFontSize[i].m_entropy, m_roleFontSize[i].m_valRole);

	fprintf(fp, "<h1>Font-Style</h1>\n");
	for(i = 0; i < m_roleFontStyle.GetSize(); i++)
		fprintf(fp, "<font style=font-style:%s;>%lf</font>&nbsp;%s<br>\n", m_roleFontStyle[i].m_valRole, m_roleFontStyle[i].m_entropy, m_roleFontStyle[i].m_valRole);

	fprintf(fp, "<h1>Font-Weight</h1>\n");
	for(i = 0; i < m_roleFontWeight.GetSize(); i++)
		fprintf(fp, "<font style=font-weight:%s;>%lf</font>&nbsp;%s<br>\n", m_roleFontWeight[i].m_valRole, m_roleFontWeight[i].m_entropy, m_roleFontWeight[i].m_valRole);


	fprintf(fp, "<br><br><br><br>\n");

	CPageWord *pWord;
//	double lfMean, lfMin;
//	double sum;
	m_hPageWords->resetSeek();
	while( (pWord = (CPageWord *)m_hPageWords->getObject()) != NULL )
	{

		for(i = 0; i < 20 - pWord->getWord().GetLength(); i++)
			fprintf(fp, "&nbsp;");

		fprintf(fp, "%s&nbsp;&nbsp;", pWord->getWord());
		fprintf(fp, " %lf", pWord->m_fgColor);
		fprintf(fp, " %lf", pWord->m_bgColor);
		fprintf(fp, " %lf", pWord->m_fontFamily);
		fprintf(fp, " %lf", pWord->m_fontSize);
		fprintf(fp, " %lf", pWord->m_fontStyle);
		fprintf(fp, " %lf", pWord->m_fontWeight);
		fprintf(fp, "<br>\n");

		/*
//		fprintf(fp, "<font color=#ff0000>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Color</font>&nbsp;");
		sum = 0.0;
		for(i = 0; i < pWord->m_arColor.GetSize(); i++)
			sum += pWord->m_arColor[i];
		fprintf(fp, " %lf", sum);
//		fprintf(fp, "<br>\n");

//		for(i = 0; i < pWord->m_arColor.GetSize(); i++)
//			fprintf(fp, " %lf", pWord->m_arColor[i]);


//		fprintf(fp, "<font color=#ff0000>Back Ground Color</font>&nbsp;");
		sum = 0.0;
		for(i = 0; i < pWord->m_arBGColor.GetSize(); i++)
			sum += pWord->m_arBGColor[i];
		fprintf(fp, " %lf", sum);
//		fprintf(fp, "<br>\n");

//		for(i = 0; i < pWord->m_arBGColor.GetSize(); i++)
//			fprintf(fp, " %lf", pWord->m_arBGColor[i]);



//		fprintf(fp, "<font color=#ff0000>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Font Family</font>&nbsp;");
		sum = 0.0;
		for(i = 0; i < pWord->m_arFontFamily.GetSize(); i++)
			sum += pWord->m_arFontFamily[i];
		fprintf(fp, " %lf", sum);
//		fprintf(fp, "<br>\n");

//		for(i = 0; i < pWord->m_arFontFamily.GetSize(); i++)
//			fprintf(fp, " %lf", pWord->m_arFontFamily[i]);



//		fprintf(fp, "<font color=#ff0000>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Font Size</font>&nbsp;");
		sum = 0.0;
		for(i = 0; i < pWord->m_arFontSize.GetSize(); i++)
			sum += pWord->m_arFontSize[i];
		fprintf(fp, " %lf", sum);
//		fprintf(fp, "<br>\n");

//		for(i = 0; i < pWord->m_arFontSize.GetSize(); i++)
//			fprintf(fp, " %lf", pWord->m_arFontSize[i]);


//		fprintf(fp, "<font color=#ff0000>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Font Style</font>&nbsp;");
		sum = 0.0;
		for(i = 0; i < pWord->m_arFontStyle.GetSize(); i++)
			sum += pWord->m_arFontStyle[i];
		fprintf(fp, " %lf", sum);
//		fprintf(fp, "<br>\n");

//		for(i = 0; i < pWord->m_arFontStyle.GetSize(); i++)
//			fprintf(fp, " %lf", pWord->m_arFontStyle[i]);


//		fprintf(fp, "<font color=#ff0000>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Font Weight</font>&nbsp;");
		sum = 0.0;
		for(i = 0; i < pWord->m_arFontWeight.GetSize(); i++)
			sum += pWord->m_arFontWeight[i];
		fprintf(fp, " %lf", sum);
//		fprintf(fp, "<br>\n");

//		for(i = 0; i < pWord->m_arFontWeight.GetSize(); i++)
//			fprintf(fp, " %lf", pWord->m_arFontWeight[i]);

		fprintf(fp, "<br>\n");
		*/


		/*
		fprintf(fp, "%s", pWord->getWord());

		statWeight(pWord->m_arColor, lfMin, lfMean);
		fprintf(fp, " %lf %lf", lfMin, lfMean);

		statWeight(pWord->m_arBGColor, lfMin, lfMean);
		fprintf(fp, " %lf %lf", lfMin, lfMean);

		statWeight(pWord->m_arFontFamily, lfMin, lfMean);
		fprintf(fp, " %lf %lf", lfMin, lfMean);

		statWeight(pWord->m_arFontSize, lfMin, lfMean);
		fprintf(fp, " %lf %lf", lfMin, lfMean);

		statWeight(pWord->m_arFontStyle, lfMin, lfMean);
		fprintf(fp, " %lf %lf", lfMin, lfMean);

		statWeight(pWord->m_arFontWeight, lfMin, lfMean);
		fprintf(fp, " %lf %lf\n", lfMin, lfMean);

		fprintf(fp, "<br>\n");
		*/

	}


	fprintf(fp, "</body>\n</html>");
	fflush(fp);
	fclose(fp);
}


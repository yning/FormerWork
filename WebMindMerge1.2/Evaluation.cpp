///////////////////////////////////////////////////////////////
// Evaluation.cpp: implementation of the CEvaluation class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebMind.h"
#include "Evaluation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEvaluation::CEvaluation()
{
	pMainFrame = NULL;
	pRepository = NULL;

	bCompare = false;

	genQuery.RemoveAll();
	genURLs.RemoveAll();
	genTitles.RemoveAll();

	genPages.RemoveAll();

	m_chosenPage = -1;

//	m_evalQuery = -1;
	m_chosenQueryWords.RemoveAll();

	m_evaluation = -1;
	m_action = -1;

}

CEvaluation::~CEvaluation()
{
	genQuery.RemoveAll();
	genURLs.RemoveAll();
	genTitles.RemoveAll();
}

#include "WizSheet.h"

bool CEvaluation::runEvaluation()
{
	CWizard97Sheet dlgSheet(_T("LILAC-1"), NULL, 0, NULL, NULL, NULL);

	dlgSheet.bCompare = bCompare;

	dlgSheet.m_pageRecomm.bCompare = bCompare;
	dlgSheet.m_pageRecomm.pRepository = pRepository;
	dlgSheet.m_pageRecomm.bCompare = bCompare;
	dlgSheet.m_pageRecomm.genURLs.Append(genURLs);
	dlgSheet.m_pageRecomm.genTitles.Append(genTitles);

	dlgSheet.m_pageEval.pRepository = pRepository;
	dlgSheet.m_pageEval.bCompare = bCompare;
	dlgSheet.m_pageEval.genQuery.Append(genQuery);
	dlgSheet.m_pageEval.genURLs.Append(genURLs);
	dlgSheet.m_pageEval.genTitles.Append(genTitles);

	int nResponse = dlgSheet.DoModal();
	if (nResponse == IDCANCEL)
	{
        return false;
	}


	// then we retrieve all the values that set by the user

	CPageCache *pCache;
	CString strBuffer;
	int nGenPages;

	nGenPages = genURLs.GetSize() - (bCompare?1:0);
	// we then add all the pages (ID/URL) into an array
	for(int i = 0; i < nGenPages; i++)
	{
		// go through each browser, check whether its CPageCache has been
		// initialized
		pCache = dlgSheet.m_pageRecomm.m_pageBrowsers[i].pCache;
		if(pCache == NULL)
			genPages.Add(genURLs[i]);
		else
		{
			strBuffer.Format("%ld", pCache->getPageno());
			genPages.Add(strBuffer);
		}
	}

	m_chosenPage = 0;
//	m_chosenPage = dlgSheet.m_pageEval.m_recomm;

	m_chosenQueryWords.RemoveAll();
	m_chosenQueryWords.Append(dlgSheet.m_pageEval.chosenQueryWords);

	m_preference = dlgSheet.m_pageEval.m_preference;
	m_evaluation = dlgSheet.m_pageEval.m_eval;

	m_action = dlgSheet.m_pageEval.m_action;

	return true;
}

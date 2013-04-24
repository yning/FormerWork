///////////////////////////////////////////////////////////////
// Evaluation.h: interface for the CEvaluation class.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_EVALUATION_H__F58FF470_8E1B_40A4_B88A_D770DB5AE07F__INCLUDED_)
#define AFX_EVALUATION_H__F58FF470_8E1B_40A4_B88A_D770DB5AE07F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MainFrm.h"
#include "PageRepository.h"

class CEvaluation  
{
public:
	CEvaluation();
	virtual ~CEvaluation();

public:
	bool runEvaluation();

public:
	CMainFrame *pMainFrame;
	CPageRepository *pRepository;

	CStringArray genQuery;
	CStringArray genURLs;
	CStringArray genTitles;

	CStringArray genPages;

	bool bCompare;

public:
	int m_chosenPage;

	CStringArray m_chosenQueryWords;
	// evaluation of the query
	//int m_evalQuery;

	// the user's preference of his own IC-page and the generated one
	int m_preference;

	// evaluation of the returned page
	int m_evaluation;

	int m_action;

};

#endif // !defined(AFX_EVALUATION_H__F58FF470_8E1B_40A4_B88A_D770DB5AE07F__INCLUDED_)

///////////////////////////////////////////////////////////////
// ViewOnLine.h: interface for the CViewOnLine class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWONLINE_H__54E73D81_A23A_4F96_884F_172F9D0D6C4E__INCLUDED_)
#define AFX_VIEWONLINE_H__54E73D81_A23A_4F96_884F_172F9D0D6C4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxtempl.h>
#include <afxdisp.h>

#include "LilacObject.h"
#include "Utility.h"
#include "HighTime.h"
#include "Markup.h"

// the action that leads to the page that will be shown on the tab
#define HYPERLINK_CLICK	0
#define BACKWARD		1
#define FORWARD			2
#define TYPEIN			3
#define NEWWINDOW2		4
#define FROMRECOMM		5
#define BOOKMARK		6

#define CURRENTTAB		7
#define	CURRENTTABIC	8
#define NEWTAB			9
#define NEWTABIC		10


// rank of the page either a normal page or an IC-page
#define NORMAL			0
#define ICPAGE			1


// keep track of the status of the page, either
// in activate or deactivate status
class CPageStatus : public CObject
{
public:
	CPageStatus()
	{
		m_bActivate = FALSE;
	}

	CPageStatus(const CPageStatus& src)
	{
		m_bActivate = src.m_bActivate;
		m_timeStamp = src.m_timeStamp;
	}

	virtual ~CPageStatus()
	{
	}

	void operator =(const CPageStatus& src)
	{
		m_bActivate = src.m_bActivate;
		m_timeStamp = src.m_timeStamp;
	}

	BOOL operator !=(const CPageStatus& src)
	{
		if( m_bActivate != src.m_bActivate ||
			m_timeStamp != src.m_timeStamp)
			return TRUE;

		return FALSE;
	}

public:

    BOOL m_bActivate;
    CHighTime m_timeStamp;
};


class CPageEvaluation
{
public:
	CPageEvaluation()
	{
		m_buttonClicked = -1;
		m_horizon = 0;
		m_model = -1;
		m_modelVersion.Empty();
		m_modelCode.Empty();
		m_stemQuery.RemoveAll();
		m_presentationPolicy = -1;
		m_genPages.RemoveAll();
		m_chosenPage = -1;
		m_preference = -1;
		m_evaluation = -1;
		m_action = -1;

		m_chosenQueryWords.RemoveAll();

		m_bRelevant = false;
		m_relevantWords.RemoveAll();

		m_bJewellGoogle = false;
		m_modelQuery.Empty();
		m_checkURLs.Empty();
	}

	~CPageEvaluation()
	{
		m_modelCode.Empty();
		m_stemQuery.RemoveAll();
		m_genPages.RemoveAll();

		m_chosenQueryWords.RemoveAll();

		m_relevantWords.RemoveAll();

		m_modelQuery.Empty();
		m_checkURLs.Empty();
	}

	CPageEvaluation *clone()
	{
		CPageEvaluation *pClone;

		pClone = (CPageEvaluation *) new CPageEvaluation();

		pClone->m_timeEvalStart = m_timeEvalStart;
		pClone->m_timeEvalEnd = m_timeEvalEnd;

		pClone->m_buttonClicked = m_buttonClicked;

		pClone->m_bJewellGoogle = m_bJewellGoogle;
		pClone->m_modelQuery = m_modelQuery;
		pClone->m_checkURLs = m_checkURLs;

		pClone->m_bRelevant = m_bRelevant;
		pClone->m_relevantWords.Append(m_relevantWords);

		pClone->m_horizon = m_horizon;

		pClone->m_model = m_model;
		pClone->m_modelVersion = m_modelVersion;
		pClone->m_modelCode = m_modelCode;

		pClone->m_stemQuery.Append(m_stemQuery);

		pClone->m_presentationPolicy = m_presentationPolicy;

		pClone->m_genPages.Append(m_genPages);

		pClone->m_chosenPage = m_chosenPage;

		pClone->m_preference = m_preference;
		pClone->m_evaluation = m_evaluation;
		pClone->m_action = m_action;

		pClone->m_chosenQueryWords.Append(m_chosenQueryWords);

		return pClone;
	}


	void dump()
	{
		CString strBuffer;
		int i;

		m_LogTrace.WriteLine(" ===================  Evaluation  ===================\n");//commented by kosko
	
		LPCTSTR format="%m/%d/%Y %H:%M:%S.%s";
		strBuffer = m_timeEvalStart.Format(format);
		m_LogTrace.WriteLine("              Start : %s\n", strBuffer);

		strBuffer = m_timeEvalEnd.Format(format);
		m_LogTrace.WriteLine("                End : %s\n\n", strBuffer);

		if(m_buttonClicked == 0)
			m_LogTrace.WriteLine("     Button Clicked : Mark IC\n\n");
		else
			m_LogTrace.WriteLine("     Button Clicked : Suggest\n\n");

		m_LogTrace.WriteLine("            Horizon : %d\n\n", m_horizon);

		if(m_bJewellGoogle)
		{
			m_LogTrace.WriteLine("        Query Model : %s\n", m_modelQuery);
			m_LogTrace.WriteLine("       Checked URLs : %s\n", m_checkURLs);
		}
		
		if(m_bRelevant)
		{
			m_LogTrace.WriteLine("     Relevant Words : ");
			for(i = 0; i < m_relevantWords.GetSize(); i++)
				m_LogTrace.WriteLine(" %s ", m_relevantWords[i]);
			m_LogTrace.WriteLine("\n\n");
		}


		m_LogTrace.WriteLine("              Model : %d\n", m_model);
		m_LogTrace.WriteLine("            Version : %s\n", m_modelVersion);
		

		m_LogTrace.WriteLine("Model Code Sequence : %s\n", m_modelCode);
		m_LogTrace.WriteLine("Presentation Policy : %d\n", m_presentationPolicy);

		m_LogTrace.WriteLine("    Generated Pages : ");
		for(i = 0; i < m_genPages.GetSize(); i++)
			m_LogTrace.WriteLine(" %s ", m_genPages[i]);
		m_LogTrace.WriteLine("\n");

		m_LogTrace.WriteLine("        Chosen Page : %d\n", m_chosenPage);
		m_LogTrace.WriteLine("         Preference : %d\n", m_preference);
		m_LogTrace.WriteLine("         Evaluation : %d\n", m_evaluation);
		m_LogTrace.WriteLine("             Action : %d\n", m_action);

		m_LogTrace.WriteLine("\n");
	}


public:
	// the beginning and ending time of the evaluation
	CHighTime m_timeEvalStart;
	CHighTime m_timeEvalEnd;

	// which button the user clicks to trigger the evaluation
	// CLICK_MAKEIC 0
	// CLICK_IGIVEUP 1
	int m_buttonClicked;

	///////////////////////////////////////////////////////////////////
	// whether this structure is for Jewell google testing session
	///////////////////////////////////////////////////////////////////
	bool m_bJewellGoogle;
	CString m_modelQuery;
	CString m_checkURLs;


	///////////////////////////////////////////////////////////////////
	// whether this evaluation structure is for relevant words
	///////////////////////////////////////////////////////////////////
	bool m_bRelevant;
	// keep the list of relevant words that the user picks out from
	// the word list that generated by WebMind
	CStringArray m_relevantWords;


	// Horizon
	int m_horizon;			

	// decide which model we will use to generate
	// recommendation
	// 0 : Association Rule
	// 1 : IC-word (Naive Bayes/C4.5)
	// 2 : IC-word (RNN)
	// 3 : IC-query
	int m_model;

	// the version of the model that used to generate recommendation
	CString m_modelVersion;
	
	// keep track of the recommendation generated from different models.
	// every four digits for one trying on a model,
	// first two digits is the code of the model, 
	// and the last two digits is the number of recommendation generated.
	// for example:
	//   0000 0110
	//  call association rule, but no recommendation, 
	//  then try IC-word (Naive Bayes), 10 URLs returned
	//  99 is the code of back up model
	CString m_modelCode;


	// the query words (after stemming) that actually retrieve the suggest page
	CStringArray m_stemQuery;

	// Presentation Policy
	// 0 : top one
	// 1 : randomly pick one
	// 2 : all
	int m_presentationPolicy;

	// the list of pages that displayed in the recommendatino property page
	// if the page cannot be loaded, then its URL will be used
	CStringArray m_genPages;

	// which page that chosen for evaluation, if there are more than
	// one page, then the user must choose the best one, and then
	// evaluate it by comparing to his own IC-page
	int m_chosenPage;

	// the evaluation of the page, 
	//  0 : Prefer my own IC-page
	//  1 : No preference
	//  2 : prefer to the suggested page
	int m_preference;

	// the evaluation of the page, 
	//  0 : fully answer my queation
	//  1 : somewhat answer my question
	//  2 : interesting but not so relevant
	//  3 : Not related at all
	int m_evaluation;

	// the user's action to the new page
	//  0 : forget the new page
	//  1 : open the new page in current tab
	//  2 : open the suggested page in a new Tab
	int m_action;


	// the chosen search key words 
	// we will also show the key words that we send to search engine
	// the subject will be required to check ONLY the relevant words
	// from the list
	CStringArray m_chosenQueryWords;

};

class CViewOnLine : public CLilacObject  
{
public:
	CViewOnLine();
	virtual ~CViewOnLine();

public:
	void dump()
	{
		m_LogTrace.WriteLine("    URL : %s\n", m_url);
		m_LogTrace.WriteLine("Page ID : %ld\n", m_pageID);
		if(m_frames.GetSize() > 0)
		{
			m_LogTrace.WriteLine(" Frames : ");
			for(int i = 0; i < m_frames.GetSize(); i++)
				m_LogTrace.WriteLine(" %ld ", m_frames[i]);
			m_LogTrace.WriteLine("\n");
		}
		m_LogTrace.WriteLine("\n");

		if(m_seqStatus.GetSize())
		{
			CString strBuffer;

			m_LogTrace.WriteLine(" Page Status : \n");
			for(int i = 0; i < m_seqStatus.GetSize(); i++)
			{
				if(m_seqStatus[i].m_bActivate)
					m_LogTrace.WriteLine(" Activate ");
				else
					m_LogTrace.WriteLine(" Deactivate ");

				strBuffer = m_seqStatus[i].m_timeStamp.Format("%m/%d/%Y %H:%M:%S.%s");
				m_LogTrace.WriteLine(" %s\n", strBuffer);
			}
			m_LogTrace.WriteLine("\n");

		}

		if(m_pEvaluation)
			m_pEvaluation->dump();
	}

    CLilacObject *clone();

    void exportXML(CMarkup &m_hXML);
	void importXML(CMarkup &m_hXML);

	bool isValid()
	{
		return (m_pageID >= 0)?true:false;
	}

	bool isICPage()
	{
		return (rank == ICPAGE)?true:false;
	}

	bool isSessionPage()
	{
		return (isICPage() || m_pEvaluation != NULL);
	}

public:

    CString m_title;
    CString m_url;
	long m_pageID;

	// the time that the page has been totally loaded
    CHighTime m_timeStamp;

	// the sequence of the page status, either activate 
	// or deactivate
	CArray<CPageStatus, CPageStatus&> m_seqStatus;

	// how the user reaches the page
	int m_action;

	// whether this page is labelled as IC-page
    int rank;

	// the label and optional description of the IC-page
    CString label;
    CString desc;

	// the evelation of the suggested page which is generated on this page
	CPageEvaluation *m_pEvaluation;

	// if there is any included frames
	CArray<long, long&> m_frames;

	// whether it has frames
	bool m_bFrameSet;
};


#endif // !defined(AFX_VIEWONLINE_H__54E73D81_A23A_4F96_884F_172F9D0D6C4E__INCLUDED_)

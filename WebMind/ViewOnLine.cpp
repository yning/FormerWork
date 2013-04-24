///////////////////////////////////////////////////////////////
// ViewOnLine.cpp: implementation of the CViewOnLine class.
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ViewOnLine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CViewOnLine::CViewOnLine()
{
	m_action = HYPERLINK_CLICK;
	m_bFrameSet = false;
	m_frames.RemoveAll();
	m_seqStatus.RemoveAll();
	rank = NORMAL;

	m_pEvaluation = NULL;

	m_pageID = -1;
	m_url.Empty();
}

CViewOnLine::~CViewOnLine()
{
	m_frames.RemoveAll();

	if(m_pEvaluation)
		delete m_pEvaluation;
}

CLilacObject * CViewOnLine::clone()
{
    CViewOnLine *theclone;

    theclone = (CViewOnLine *) new CViewOnLine();

	theclone->desc = desc;
	theclone->label = label;
	theclone->m_action = m_action;
	theclone->m_pageID = m_pageID;
	theclone->m_timeStamp = m_timeStamp;

    for (int j = 0; j < m_seqStatus.GetSize(); j++)
        theclone->m_seqStatus.Add(m_seqStatus[j]);

	theclone->m_title = m_title;
	theclone->m_url = m_url;
	theclone->rank = rank;

	if(m_pEvaluation)
		theclone->m_pEvaluation = m_pEvaluation->clone();


	theclone->m_bFrameSet = m_bFrameSet;
    for (int i = 0; i < m_frames.GetSize(); i++)
        theclone->m_frames.Add(m_frames[i]);

    return theclone;
}

void CViewOnLine::exportXML(CMarkup &m_hXML)
{
	CString strBuffer;
	CHighTime ct;
	int k;

	LPCTSTR format="%m/%d/%Y %H:%M:%S.%s";
	m_hXML.AddChildElem( "URL", m_url );
	strBuffer.Format("%ld", m_pageID);
	m_hXML.AddChildAttrib( "PageID", strBuffer );

	ct = m_timeStamp;
	strBuffer = ct.Format(format);
	m_hXML.AddChildAttrib("TimeStamp", strBuffer );

	strBuffer.Format("%d", m_action);
	m_hXML.AddChildAttrib("Action", strBuffer );

	m_hXML.AddChildElem( "PageStatus", "" );
	m_hXML.IntoElem();
	for(k = 0; k < m_seqStatus.GetSize(); k++)
	{
		ct = m_seqStatus[k].m_timeStamp;
		strBuffer = ct.Format(format);

		if(m_seqStatus[k].m_bActivate)
			m_hXML.AddChildElem( "Status", "Activate" );
		else
			m_hXML.AddChildElem( "Status", "DeActivate" );

		m_hXML.AddChildAttrib( "TimeStamp", strBuffer );
	}
	m_hXML.OutOfElem();

	if(m_pEvaluation)
	{
		// if there is evaluation information, let's dump it//dump:×ª´¢¡¢ÇåÀí

		CPageEvaluation *pEvaluation;
		pEvaluation = m_pEvaluation;

//			pEvaluation->dump();

		m_hXML.AddChildElem( "Evaluation", "" );

		// timing information
		strBuffer = pEvaluation->m_timeEvalStart.Format(format);
		m_hXML.AddChildAttrib( "TimeStart", strBuffer );

		strBuffer = pEvaluation->m_timeEvalEnd.Format(format);
		m_hXML.AddChildAttrib( "TimeEnd", strBuffer );

		// which button
		strBuffer.Format("%d", pEvaluation->m_buttonClicked);
		m_hXML.AddChildAttrib( "ButtonClicked", strBuffer );

		m_hXML.IntoElem();

		// horizon
		strBuffer.Format("%d", pEvaluation->m_horizon);
		m_hXML.AddChildElem( "Horizon", strBuffer );


		// whether this is for Jewell Google testing
		if(pEvaluation->m_bJewellGoogle)
		{
			m_hXML.AddChildElem( "JewellGoogle", pEvaluation->m_modelQuery );
			m_hXML.AddChildAttrib( "CheckedURL", pEvaluation->m_checkURLs );
		}


		// whether this is for relevant words,
		// if yes, output all relevant words (in stem)
		if(pEvaluation->m_bRelevant)
		{
			strBuffer.Empty();

			if(pEvaluation->m_relevantWords.GetSize() > 0)
			{
				strBuffer = pEvaluation->m_relevantWords[0];
				for(k = 1; k < pEvaluation->m_relevantWords.GetSize(); k++)
					strBuffer += _T(";") + pEvaluation->m_relevantWords[k];
			}

			m_hXML.AddChildElem( "RelevantWord", strBuffer );
		}

		strBuffer.Format("%d", pEvaluation->m_model);
		m_hXML.AddChildElem("Model", strBuffer );
		m_hXML.AddChildAttrib( "ModelSeqCode", pEvaluation->m_modelCode );
		m_hXML.AddChildAttrib( "Version", pEvaluation->m_modelVersion );

		strBuffer.Format("%d", pEvaluation->m_presentationPolicy);
		m_hXML.AddChildElem( "PresentationPolicy", strBuffer );


		if(pEvaluation->m_stemQuery.GetSize() > 0)
		{
			strBuffer = pEvaluation->m_stemQuery[0];
			for(k = 1; k < pEvaluation->m_stemQuery.GetSize(); k++)
				strBuffer += _T(";") + pEvaluation->m_stemQuery[k];

			m_hXML.AddChildElem( "QueryWord", strBuffer );
		}

		if(pEvaluation->m_chosenQueryWords.GetSize() > 0)
		{
			strBuffer = pEvaluation->m_chosenQueryWords[0];
			for(k = 1; k < pEvaluation->m_chosenQueryWords.GetSize(); k++)
				strBuffer += _T(";") + pEvaluation->m_chosenQueryWords[k];

			m_hXML.AddChildElem( "RelevantQuery", strBuffer );
		}

		strBuffer.Format("%d", pEvaluation->m_preference);
		m_hXML.AddChildElem( "Preference", strBuffer );

		strBuffer.Format("%d", pEvaluation->m_evaluation);
		m_hXML.AddChildElem( "PageEvaluation", strBuffer );

		strBuffer.Format("%d", pEvaluation->m_action);
		m_hXML.AddChildElem( "Action", strBuffer );

		if(pEvaluation->m_genPages.GetSize() > 0)
		{
			strBuffer.Empty();

			strBuffer = pEvaluation->m_genPages[0];
			for(k = 1; k < pEvaluation->m_genPages.GetSize(); k++)
				strBuffer += _T(" ; ") + pEvaluation->m_genPages[k];

			m_hXML.AddChildElem( "Recommendation", strBuffer );

			strBuffer.Format("%d", pEvaluation->m_genPages.GetSize());
			m_hXML.AddChildAttrib( "Number", strBuffer );

			strBuffer.Format("%d", pEvaluation->m_chosenPage);
			m_hXML.AddChildAttrib( "Chosen", strBuffer );
		}

		m_hXML.OutOfElem();
	}

	m_hXML.AddChildElem( "Label", label );
	strBuffer.Format("%d", rank);
	m_hXML.AddChildAttrib( "Rank", strBuffer );

	m_hXML.AddChildElem( "FrameSet", "" );
	m_hXML.IntoElem();
	for(int j = 0; j < m_frames.GetSize(); j++)
	{
		strBuffer.Format("%ld", m_frames[j]);
		m_hXML.AddChildElem( "Frame", strBuffer );
	}
	m_hXML.OutOfElem();


}


// import the CViewOnLine from the XML
void CViewOnLine::importXML(CMarkup &m_hXML)
{
	bool bFound;

	CString strPageID;
	CString strTime;
	CString strAction;
	CString strURL;
	CString strRank;
	CString strLabel;
	CString strFrame;

    CHighTime ct;

	CString strBuffer;

	if((bFound = m_hXML.FindChildElem("URL")) == true)
	{
		m_hXML.IntoElem();

		strPageID = m_hXML.GetAttrib("PageID");
		strTime = m_hXML.GetAttrib("TimeStamp");
		strAction = m_hXML.GetAttrib("Action");
		strURL = m_hXML.GetData();

		m_url = strURL;
		m_action = atoi(strAction);
		m_pageID = atoi(strPageID);
		m_timeStamp.ParseDateTime(strTime);

		m_hXML.OutOfElem();
	}


	// read in the page status sequence
	if((bFound = m_hXML.FindChildElem("PageStatus")) == true)
	{
		m_hXML.IntoElem();

		CPageStatus pageStatus;
		CString strStatus;

		while( (bFound = m_hXML.FindChildElem("Status")) == true )
		{
			m_hXML.IntoElem();

			strStatus = m_hXML.GetData();
			if(strStatus.CompareNoCase("Activate") == 0)
				pageStatus.m_bActivate = TRUE;
			else
				pageStatus.m_bActivate = FALSE;

			strTime = m_hXML.GetAttrib("TimeStamp");
			pageStatus.m_timeStamp.ParseDateTime(strTime);

			m_seqStatus.Add(pageStatus);

			m_hXML.OutOfElem();
		}

		m_hXML.OutOfElem();
	}

	// read in the evaluation information
	if((bFound = m_hXML.FindChildElem("Evaluation")) == true)
	{
		m_hXML.IntoElem();

		CString buttonClicked;

		m_pEvaluation = (CPageEvaluation *) new CPageEvaluation();

		strTime = m_hXML.GetAttrib("TimeStart");
		m_pEvaluation->m_timeEvalStart.ParseDateTime(strTime);

		strTime = m_hXML.GetAttrib("TimeEnd");
		m_pEvaluation->m_timeEvalEnd.ParseDateTime(strTime);

		buttonClicked = m_hXML.GetAttrib("ButtonClicked");
		m_pEvaluation->m_buttonClicked = atoi(buttonClicked);

		if( (bFound = m_hXML.FindChildElem("Horizon")) == true )
		{
			m_hXML.IntoElem();
			strBuffer = m_hXML.GetData();
			m_pEvaluation->m_horizon = atoi(strBuffer);
			m_hXML.OutOfElem();
		}


		// whether this is for Jewell Google testing
		// concanate all the checked URLs by ";"
		if( (bFound = m_hXML.FindChildElem("JewellGoogle")) == true )
		{
			m_pEvaluation->m_bJewellGoogle = true;

			m_hXML.IntoElem();

			m_pEvaluation->m_modelQuery = m_hXML.GetData();
			m_pEvaluation->m_checkURLs = m_hXML.GetAttrib("CheckedURL");

			m_hXML.OutOfElem();
		}

		if( (bFound = m_hXML.FindChildElem("RelevantWord")) == true )
		{
			m_pEvaluation->m_bRelevant = true;

			m_hXML.IntoElem();
			strBuffer = m_hXML.GetData();

			Utility::parseSequence(strBuffer, _T(";"), m_pEvaluation->m_relevantWords);

			m_hXML.OutOfElem();
		}
				

		if( (bFound = m_hXML.FindChildElem("Model")) == true )
		{
			m_hXML.IntoElem();
			strBuffer = m_hXML.GetData();
			m_pEvaluation->m_model = atoi(strBuffer);

			strBuffer = m_hXML.GetAttrib("ModelSeqCode");
			m_pEvaluation->m_modelCode = strBuffer;

			strBuffer = m_hXML.GetAttrib("Version");
			m_pEvaluation->m_modelVersion = strBuffer;

			m_hXML.OutOfElem();
		}

		if( (bFound = m_hXML.FindChildElem("PresentationPolicy")) == true )
		{
			m_hXML.IntoElem();
			strBuffer = m_hXML.GetData();
			m_pEvaluation->m_presentationPolicy = atoi(strBuffer);
			m_hXML.OutOfElem();
		}

		if( (bFound = m_hXML.FindChildElem("QueryWord")) == true )
		{
			m_hXML.IntoElem();
			strBuffer = m_hXML.GetData();
			Utility::parseSequence(strBuffer, _T(";"), m_pEvaluation->m_stemQuery);
			m_hXML.OutOfElem();
		}

		if( (bFound = m_hXML.FindChildElem("RelevantQuery")) == true )
		{
			m_hXML.IntoElem();
			strBuffer = m_hXML.GetData();
			Utility::parseSequence(strBuffer, _T(";"), m_pEvaluation->m_chosenQueryWords);
			m_hXML.OutOfElem();
		}

		if( (bFound = m_hXML.FindChildElem("Preference")) == true )
		{
			m_hXML.IntoElem();
			strBuffer = m_hXML.GetData();
			m_pEvaluation->m_preference = atoi(strBuffer);
			m_hXML.OutOfElem();
		}

		if( (bFound = m_hXML.FindChildElem("PageEvaluation")) == true )
		{
			m_hXML.IntoElem();
			strBuffer = m_hXML.GetData();
			m_pEvaluation->m_evaluation = atoi(strBuffer);
			m_hXML.OutOfElem();
		}

		if( (bFound = m_hXML.FindChildElem("Action")) == true )
		{
			m_hXML.IntoElem();
			strBuffer = m_hXML.GetData();
			m_pEvaluation->m_action = atoi(strBuffer);
			m_hXML.OutOfElem();
		}

		if( (bFound = m_hXML.FindChildElem("Recommendation")) == true )
		{
			m_hXML.IntoElem();
			// here we get the list of page ids for recommendation
			strBuffer = m_hXML.GetData();
			Utility::parseSequence(strBuffer, _T(" ; "), m_pEvaluation->m_genPages);

			strBuffer = m_hXML.GetAttrib("Chosen");
			m_pEvaluation->m_chosenPage = atoi(strBuffer);
			m_hXML.OutOfElem();
		}

//		m_pEvaluation->dump();

		m_hXML.OutOfElem();
	}


	if((bFound = m_hXML.FindChildElem("Label")) == true)
	{
		m_hXML.IntoElem();

		strRank = m_hXML.GetAttrib("Rank");
		strLabel = m_hXML.GetData();

		label.Empty();
		label.Format("%s", strLabel);
		rank = atoi(strRank);

		m_hXML.OutOfElem();
	}

	if((bFound = m_hXML.FindChildElem("FrameSet")) == true)
	{
		m_hXML.IntoElem();

		m_frames.RemoveAll();
		while((bFound = m_hXML.FindChildElem("Frame")) == true)
		{
			long m_frameID;

			m_hXML.IntoElem();

			strFrame = m_hXML.GetData();
			m_frameID = atoi(strFrame);
			m_frames.Add(m_frameID);

			m_bFrameSet = true;
	
			m_hXML.OutOfElem();
		}

		m_hXML.OutOfElem();
	}
}



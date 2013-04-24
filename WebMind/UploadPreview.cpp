///////////////////////////////////////////////////////////////
// UploadPreview.cpp : implementation file
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebMind.h"
#include "UploadPreview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUploadPreview dialog


CUploadPreview::CUploadPreview(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CUploadPreview::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUploadPreview)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUploadPreview::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUploadPreview)
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_ctrlRemove);
	DDX_Control(pDX, IDC_TREE_HISTORY, m_tHistory);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUploadPreview, CTabPageSSL)
	//{{AFX_MSG_MAP(CUploadPreview)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_HISTORY, OnSelchangedTreeHistory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUploadPreview message handlers

BOOL CUploadPreview::OnInitDialog() 
{
	CTabPageSSL::OnInitDialog();
	
	loadHistory();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// Load in the hostory for previewing
void CUploadPreview::loadHistory()
{
	CStringArray m_trainLogs;
	HTREEITEM prootitem;
//	CViewOnLine *pView;

	// at first, let's obtain the list of all log files
	m_trainLogs.RemoveAll();
	Utility::getLILACLog(theSettings.m_dataPath, m_trainLogs);

	// then for each log file, read all the page sequence into
	// a list, and then add the log items into the tree.
	for(int i = 0; i < m_trainLogs.GetSize(); i++)
	{
		// read in the page sequence
		prootitem = m_tHistory.InsertItem(m_trainLogs[i], TVI_ROOT);
		readLogXML(m_trainLogs[i], prootitem);
	}
}

// read in the log in XML, and put it into a list
void CUploadPreview::readLogXML(const CString &m_logfile, HTREEITEM tItem)
{
	CMarkup m_pXML;
	CFile file( m_logfile, CFile::modeRead );

	int nFileLen = file.GetLength();
	unsigned char* pBuffer = NULL;
	BOOL bFound;

    CHighTime ct;

	List *m_logSeq;
	CViewOnLine *pView;

	HTREEITEM pItem;
	CString strBuffer;

	// Allocate Buffer for Ansi file data
	pBuffer = new unsigned char[nFileLen + 1];
	nFileLen = file.Read( pBuffer, nFileLen );
	file.Close();
	pBuffer[nFileLen] = '\0';
	
	CString m_csText;
	m_csText = (char*)pBuffer;

	m_pXML.SetDoc( m_csText );
	m_pXML.ResetPos();

	m_logSeq = (List *) new List();

	bFound = m_pXML.FindElem("WebICLOG");
	while((bFound = m_pXML.FindChildElem()) == true)
	{
		m_pXML.IntoElem();

		while((bFound = m_pXML.FindChildElem()) == true)
		{
			m_pXML.IntoElem();

			pView = (CViewOnLine *) new CViewOnLine();
			pView->importXML(m_pXML);

			m_logSeq->add(pView);
			
			m_pXML.OutOfElem();
		}

		if(m_logSeq->size() > 0)
		{
			// format the presentation
			pView = (CViewOnLine *) m_logSeq->get(0);
			ct = pView->m_timeStamp;
			strBuffer = ct.Format("%m/%d/%Y %H:%M:%S.%s");

			pView = (CViewOnLine *) m_logSeq->get(m_logSeq->size() - 1);
			ct = pView->m_timeStamp;

			strBuffer += _T(" <===> ") + ct.Format("%m/%d/%Y %H:%M:%S.%s");


			pItem = m_tHistory.InsertItem(strBuffer, tItem);

			// show each click in the page sequence
			for(int j = 0; j < m_logSeq->size(); j++)
			{
				pView = (CViewOnLine *) m_logSeq->get(j);

				ct = pView->m_timeStamp;
				strBuffer = ct.Format("%m/%d/%Y %H:%M:%S.%s");
				strBuffer += _T(" ") + pView->m_url;

				m_tHistory.InsertItem(strBuffer, pItem);
			}
		}

		m_logSeq->clear();

		m_pXML.OutOfElem();
	}

	delete [] pBuffer;
	delete m_logSeq;
}


void CUploadPreview::OnButtonDelete() 
{
	HTREEITEM hItem;
	HTREEITEM pItem;
	CString strFile;
	CString strMessage;
	
	hItem = m_tHistory.GetSelectedItem();
	if(hItem)
	{
		pItem = m_tHistory.GetParentItem( hItem );
		if(!pItem)
		{
			strFile = m_tHistory.GetItemText(hItem);
			strMessage = _T("Are you sure that you want to remove the Web log file from uploading : \r\n\r\n") +
				strFile + _T(" ?\r\n");

			if(MessageBox(strMessage, "Preview ...", MB_ICONWARNING | MB_YESNO) == IDYES)
			{
				DeleteFile(strFile);
				m_tHistory.DeleteItem(hItem);
			}

		}
	}
}

void CUploadPreview::OnSelchangedTreeHistory(NMHDR* , LRESULT* pResult) 
{
	HTREEITEM hItem;
	HTREEITEM pItem;
	BOOL activateRemove;
	
	activateRemove = FALSE;
	hItem = m_tHistory.GetSelectedItem();
	if(hItem)
	{
		pItem = m_tHistory.GetParentItem( hItem );
		if(!pItem)
			activateRemove = TRUE;
	}
	m_ctrlRemove.EnableWindow(activateRemove);
	
	*pResult = 0;
}

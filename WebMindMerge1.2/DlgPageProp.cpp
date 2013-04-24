///////////////////////////////////////////////////////////////
// DlgPageProp.cpp : implementation file
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebMind.h"
#include "DlgPageProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPageProp dialog


CDlgPageProp::CDlgPageProp(CViewOnLine *pView)
	: CDialog(CDlgPageProp::IDD, NULL)
{
	//{{AFX_DATA_INIT(CDlgPageProp)
	//}}AFX_DATA_INIT
	m_pView = pView;
	m_title = m_pView->m_title;
	m_uri = m_pView->m_url;
	m_annotated = (m_pView->rank == 0)?FALSE:TRUE;
	m_label = m_pView->label;
	m_desc = m_pView->desc;
}


void CDlgPageProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPageProp)
	DDX_Control(pDX, IDOK, m_okButton);
	DDX_Control(pDX, IDC_EDIT_DESC, m_descEdit);
	DDX_Control(pDX, IDC_EDIT_LABEL, m_labelEdit);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_title);
	DDX_Text(pDX, IDC_EDIT_URI, m_uri);
	DDX_Check(pDX, IDC_CHECK_ANNOTATED, m_annotated);
	DDX_Text(pDX, IDC_EDIT_LABEL, m_label);
	DDX_Text(pDX, IDC_EDIT_DESC, m_desc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPageProp, CDialog)
	//{{AFX_MSG_MAP(CDlgPageProp)
	ON_EN_UPDATE(IDC_EDIT_LABEL, OnUpdateEditLabel)
	ON_BN_CLICKED(IDC_CHECK_ANNOTATED, OnCheckAnnotated)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPageProp message handlers

BOOL CDlgPageProp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	refreshButtons();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPageProp::refreshButtons()
{
//	UpdateData();

	if(m_annotated)
	{
		m_labelEdit.EnableWindow();
		m_descEdit.EnableWindow();

		if(m_label.GetLength() > 0)
	        m_okButton.EnableWindow(TRUE);
		else
			m_okButton.EnableWindow(FALSE);
	}
	else
	{
		m_labelEdit.EnableWindow(FALSE);
		m_descEdit.EnableWindow(FALSE);
	}
}

void CDlgPageProp::OnUpdateEditLabel() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
	refreshButtons();
}

void CDlgPageProp::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	if(m_annotated)
	{
		m_pView->rank = 1;
		m_pView->label = m_label;
		m_pView->desc = m_desc;
	}
	else
	{
		m_pView->rank = 0;
		m_pView->label.Empty();
		m_pView->desc.Empty();
	}


	CDialog::OnOK();
}

void CDlgPageProp::OnCheckAnnotated() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	refreshButtons();
}

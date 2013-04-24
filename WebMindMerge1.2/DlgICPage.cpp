///////////////////////////////////////////////////////////////
// DlgICPage.cpp : implementation file
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebMind.h"
#include "DlgICPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgICPage dialog


CDlgICPage::CDlgICPage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgICPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgICPage)
	m_label = _T("");
	m_desc = _T("");
	//}}AFX_DATA_INIT
}


void CDlgICPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgICPage)
	DDX_Control(pDX, IDOK, m_okButton);
	DDX_Text(pDX, IDC_EDIT_LABEL, m_label);
	DDX_Text(pDX, IDC_EDIT_DESC, m_desc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgICPage, CDialog)
	//{{AFX_MSG_MAP(CDlgICPage)
	ON_EN_UPDATE(IDC_EDIT_LABEL, OnUpdateEditLabel)
	ON_EN_UPDATE(IDC_EDIT_DESC, OnUpdateEditDesc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgICPage message handlers


BOOL CDlgICPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    if(m_label.GetLength() > 0)
        m_okButton.EnableWindow(TRUE);
    else
        m_okButton.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgICPage::OnUpdateEditLabel() 
{
	refreshButton();
}

void CDlgICPage::OnUpdateEditDesc() 
{
	refreshButton();
}

void CDlgICPage::refreshButton()
{
    UpdateData();
    if(m_label.GetLength() > 0 )
        m_okButton.EnableWindow(TRUE);
    else
        m_okButton.EnableWindow(FALSE);
}


BOOL CDlgICPage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
    {
		CString	classname;
		CWnd	*pwnd = FromHandle(pMsg->hwnd);
			
		if (GetClassName(pwnd->GetSafeHwnd(),classname.GetBuffer(20),20) != 0)
		{
			if (classname == "Edit")
				pMsg->wParam = VK_TAB;
		}
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////
// UploadAgreement.cpp : implementation file
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebMind.h"
#include "UploadAgreement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DlgUpload.h"

/////////////////////////////////////////////////////////////////////////////
// CUploadAgreement dialog


CUploadAgreement::CUploadAgreement(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CUploadAgreement::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUploadAgreement)
	//}}AFX_DATA_INIT

	m_agreement = _T("\r\n")
		_T("This study is part of a research project aimed at developing ")
		_T("technologies that help Internet users find the information they are ")
		_T("looking for more quickly. The research is conducted by Dr. Tingshao Zhu, ")
		_T("and Yue Ning. Some of the results ")
		_T("of this research will be included in Yue Ning's Master. dissertation. ")
		_T("If you decide to participate in this study, you will be asked to install ")
		_T("a Web browser called WebMind on your home computer. WebMind will track your ")
		_T("Internet usage for 8 weeks. While using WebMind to find information, you ")
		_T("will be asked to click on a button whenever you discover a page that was ")
		_T("helpful to you or when you give up on your search. If you find a useful page, ")
		_T("WebMind will ask you to compare it to a page chosen by WebMind. ")
		_T("If you have given up, WebMind will give you a page to rate. The rating ")
		_T("generally takes less than a minute. You may delete data already collected ")
		_T("if you later decide you don't want to submit it.\r\n\r\n")
		_T("The automated data collection procedure described above will tell us about ")
		_T("how you use existing web browsers to locate pages on the Internet, but it ")
		_T("will not tell us about what you really want from your Internet browser, nor ")
		_T("will it tell us how you felt about using the system. In addition to ")
		_T("participating in the data collection above, you may also participate in ")
		_T("a short questionnaire to help us understand your needs better.  ")
		_T("The questionnaire is optional and you may elect to skip questions on the ")
		_T("questionnaire or completely withdraw your participation at any point. ")
		_T("An additional honorarium will be paid to you for completing the questionnaire.\r\n\r\n")
		_T("All collected data will be kept strictly confidential. You will be assigned ")
		_T("a unique ID number for the duration of the study and all of your data will ")
		_T("be submitted under this ID. The ID protects your identity, but allows us to ")
		_T("group together the data you send us at various times over the course of the ")
		_T("experiment. The ID will also be used to calculate your compensation.\r\n\r\n")
		_T("We may publish reports based on this data in the future, but any personally ")
		_T("identifying details will be removed. Finally, apart from the payment you are ")
		_T("to receive as compensation for your participation, there are no other tangible ")
		_T("benefits being offered to you for participating in this research.\r\n\r\n");
				 
	pDlgParent = NULL;
}


void CUploadAgreement::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUploadAgreement)
	DDX_Text(pDX, IDC_EDIT_AGREEMENT, m_agreement);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUploadAgreement, CTabPageSSL)
	//{{AFX_MSG_MAP(CUploadAgreement)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUploadAgreement message handlers


BOOL CUploadAgreement::OnInitDialog() 
{
	CTabPageSSL::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

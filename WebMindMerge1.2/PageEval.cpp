///////////////////////////////////////////////////////////////
// PageEval.cpp : implementation file
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebMind.h"
#include "PageEval.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "WizSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CPageEval property page

IMPLEMENT_DYNCREATE(CPageEval, CPropertyPageEx)

CPageEval::CPageEval() : CPropertyPageEx(CPageEval::IDD)
{
	//{{AFX_DATA_INIT(CPageEval)
	m_eval = -1;
	m_action = 0;
	m_recomm = 0;
	m_preference = -1;
	m_bNone = FALSE;
	//}}AFX_DATA_INIT

	bCompare = false;

	m_bEvalQuery = false;

	chosenQueryWords.RemoveAll();
//	m_evalQuery = 0;

	genQuery.RemoveAll();
	genURLs.RemoveAll();
	genTitles.RemoveAll();
}

CPageEval::~CPageEval()
{
	genQuery.RemoveAll();
	genURLs.RemoveAll();
	genTitles.RemoveAll();
}

void CPageEval::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageEval)
	DDX_Control(pDX, IDC_BUTTON_DESELECT_ALL, m_btnClearAll);
	DDX_Control(pDX, IDC_RADIO_ICPREFER, m_btnPreferIC);
	DDX_Control(pDX, IDC_RADIO_NOPREFER, m_btnPreferNo);
	DDX_Control(pDX, IDC_RADIO_NEWPREFER, m_btnPreferNew);
	DDX_Control(pDX, IDC_PREFERENCE, m_staticPreference);
	DDX_Control(pDX, IDC_LIST_QUERY, m_listQuery);
	DDX_Control(pDX, IDC_COMBO_RECOMM, m_recommPages);
	DDX_Radio(pDX, IDC_RADIO_BETTER, m_eval);
	DDX_Radio(pDX, IDC_RADIO_FORGET, m_action);
	DDX_CBIndex(pDX, IDC_COMBO_RECOMM, m_recomm);
	DDX_Radio(pDX, IDC_RADIO_ICPREFER, m_preference);
	DDX_Check(pDX, IDC_CHECK_NONE, m_bNone);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageEval, CPropertyPageEx)
	//{{AFX_MSG_MAP(CPageEval)
	ON_BN_CLICKED(IDC_RADIO_BETTER, OnRadioBetter)
	ON_BN_CLICKED(IDC_RADIO_SOSO, OnRadioSoso)
	ON_BN_CLICKED(IDC_RADIO_WORSE, OnRadioWorse)
	ON_BN_CLICKED(IDC_RADIO_FORGET, OnRadioForget)
	ON_BN_CLICKED(IDC_RADIO_SWITCH, OnRadioSwitch)
	ON_BN_CLICKED(IDC_RADIO_OPEN, OnRadioOpen)
	ON_BN_CLICKED(IDC_RADIO_OPENIC, OnRadioOpenic)
	ON_BN_CLICKED(IDC_RADIO_NOWTAB, OnRadioNowtab)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_DESELECT_ALL, OnButtonDeselectAll)
	ON_BN_CLICKED(IDC_RADIO_ICPREFER, OnRadioIcprefer)
	ON_BN_CLICKED(IDC_RADIO_NOPREFER, OnRadioNoprefer)
	ON_BN_CLICKED(IDC_RADIO_NEWPREFER, OnRadioNewprefer)
	ON_LBN_SELCHANGE(IDC_LIST_QUERY, OnSelchangeListQuery)
	ON_BN_CLICKED(IDC_CHECK_NONE, OnCheckNone)
	ON_BN_CLICKED(IDC_RADIO_INTERESTING, OnRadioInteresting)
	ON_BN_CLICKED(IDC_RADIO_REMOTE, OnRadioRemote)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageEval message handlers

BOOL CPageEval::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWizard97Sheet* pSheet = (CWizard97Sheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);

	UpdateData();
	if(pSheet)
	{
		if(pSheet->m_bHeaderCtrl)
		{
		    pSheet->m_HeaderCtrl.SetTitle ("评估");
		    pSheet->m_HeaderCtrl.SetIcon (IDI_EVALUATION);
			pSheet->m_HeaderCtrl.SetDesc (_T("请评价推荐页面是否与您的需求相关，\
并选择能够准确描述您搜索任务的关键词，\
在点击完成按钮之前您可以选择关键词并评估：") );
		}


		if(m_eval >= 0 && (!bCompare || m_preference >= 0) && m_bEvalQuery)
			pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_FINISH);	
		else
			pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_DISABLEDFINISH);	
	}

	if(pSheet)
	{
		// if the compared is also represented
		if(bCompare)
		{
			if(pSheet->m_activePage == 0)
				m_recomm = 0;
			else
				m_recomm = pSheet->m_activePage - 1;
		}
		else
			m_recomm = pSheet->m_activePage;

		UpdateData(FALSE);
	}

	return CPropertyPageEx::OnSetActive();
}

BOOL CPageEval::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	for(int i = (bCompare?1:0); i < genTitles.GetSize(); i++)
		m_recommPages.AddString(genTitles[i]);

    //m_listQuery= new CCheckListBox();//ADD BY NY
	m_listQuery.SetCheckStyle(BS_AUTOCHECKBOX);

	InitListCtrl(m_listQuery);

	CWizard97Sheet* pSheet = (CWizard97Sheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);

	if(pSheet)
		pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_DISABLEDFINISH);	
	
	m_staticPreference.EnableWindow(bCompare);
	m_btnPreferIC.EnableWindow(bCompare);
	m_btnPreferNo.EnableWindow(bCompare);
	m_btnPreferNew.EnableWindow(bCompare);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageEval::OnRadioBetter() 
{
	enableFinish();
}

void CPageEval::OnRadioSoso() 
{
	enableFinish();
}

void CPageEval::OnRadioInteresting() 
{
	enableFinish();
}

void CPageEval::OnRadioRemote() 
{
	enableFinish();
}

void CPageEval::OnRadioWorse() 
{
	enableFinish();
}

void CPageEval::enableFinish()
{
	UpdateData();

	CWizard97Sheet* pSheet = (CWizard97Sheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);

	if(pSheet)
	{
		if(m_eval >= 0 && (!bCompare || m_preference >= 0) && m_bEvalQuery)
			pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_FINISH);	
		else
			pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_DISABLEDFINISH);	
	}
}

BOOL CPageEval::OnKillActive() 
{
	getQueryWords();

	return CPropertyPageEx::OnKillActive();
}

void CPageEval::OnRadioForget() 
{
	UpdateData();
}

void CPageEval::OnRadioSwitch() 
{
	UpdateData();
}

void CPageEval::OnRadioNowtab() 
{
	UpdateData();
}

void CPageEval::OnRadioOpen() 
{
	UpdateData();
}

void CPageEval::OnRadioOpenic() 
{
	UpdateData();
}

void CPageEval::InitListCtrl(CCheckListBox &list)
{
	CString strBuffer;
	CTextProcessor *pText;
	pText = pRepository->getTextProcessor();

	for(int i = 0; i < genQuery.GetSize(); i++)
	{
		strBuffer.Empty();

		if(pText)
			pText->getOriginalWord(genQuery[i], strBuffer);
		else
			strBuffer = genQuery[i];

		list.AddString(strBuffer);
		list.SetCheck(i, FALSE);
	}
}


void CPageEval::OnRadioIcprefer() 
{
	enableFinish();
}

void CPageEval::OnRadioNoprefer() 
{
	enableFinish();
}

void CPageEval::OnRadioNewprefer() 
{
	enableFinish();
}

void CPageEval::OnOK() 
{
	CPropertyPageEx::OnOK();
}



BOOL CPageEval::OnWizardFinish() 
{
	getQueryWords();
	
	return CPropertyPageEx::OnWizardFinish();
}


void CPageEval::getQueryWords()
{
	UpdateData();

	chosenQueryWords.RemoveAll();
	for(int i = 0; i < m_listQuery.GetCount(); i++)
	{
		if(m_listQuery.GetCheck(i))
			chosenQueryWords.Add(genQuery[i]);
	}
}

void CPageEval::OnButtonSelectAll() 
{
	UpdateData();

	for(int i = 0; i < m_listQuery.GetCount(); i++)
		m_listQuery.SetCheck(i, TRUE);

	m_bEvalQuery = true;
	m_btnClearAll.EnableWindow(TRUE);
	m_bNone = false;
	UpdateData(FALSE);

	enableFinish();
}

void CPageEval::OnButtonDeselectAll() 
{
	for(int i = 0; i < m_listQuery.GetCount(); i++)
		m_listQuery.SetCheck(i, FALSE);

	m_bEvalQuery = true;
	m_btnClearAll.EnableWindow(FALSE);

	m_bNone = false;
	UpdateData(FALSE);

	enableFinish();
}

void CPageEval::OnSelchangeListQuery() 
{
	m_bEvalQuery = true;

	bool bNone = true;
	for(int i = 0; i < m_listQuery.GetCount(); i++)
	{
		if(m_listQuery.GetCheck(i))
		{
			bNone = false;
			break;
		}
	}

	if(bNone)
		m_btnClearAll.EnableWindow(FALSE);
	else
		m_btnClearAll.EnableWindow();

	m_bNone = false;
	UpdateData(FALSE);

	enableFinish();
}

void CPageEval::OnCheckNone() 
{
	UpdateData();

	for(int i = 0; i < m_listQuery.GetCount(); i++)
		m_listQuery.SetCheck(i, FALSE);

	m_btnClearAll.EnableWindow(FALSE);

	m_bEvalQuery = true;
	enableFinish();
}



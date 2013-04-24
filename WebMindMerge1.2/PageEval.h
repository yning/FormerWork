///////////////////////////////////////////////////////////////
// PageEval.h : header file
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_PAGEEVAL_H__C6F385AB_4D55_4271_9680_73303C1B5BFE__INCLUDED_)
#define AFX_PAGEEVAL_H__C6F385AB_4D55_4271_9680_73303C1B5BFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageRepository.h"

/////////////////////////////////////////////////////////////////////////////
// CPageEval dialog

class CPageEval : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CPageEval)

// Construction
public:
	CPageEval();
	~CPageEval();

	void InitListCtrl(CCheckListBox &list);
	void enableFinish();

	void getQueryWords();

// Dialog Data
	//{{AFX_DATA(CPageEval)
	enum { IDD = IDD_DIALOG_EVAL };
	CButton	m_btnClearAll;
	CButton	m_btnPreferIC;
	CButton	m_btnPreferNo;
	CButton	m_btnPreferNew;
	CStatic	m_staticPreference;
	CCheckListBox	m_listQuery;
	CComboBox	m_recommPages;
	int		m_eval;
	int		m_action;
	int		m_recomm;
	int		m_preference;
	BOOL	m_bNone;
	//}}AFX_DATA

	CPageRepository *pRepository;

	bool bCompare;

	bool m_bEvalQuery;

	CStringArray chosenQueryWords;

	CStringArray genQuery;
	CStringArray genURLs;
	CStringArray genTitles;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageEval)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual void OnOK();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageEval)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioBetter();
	afx_msg void OnRadioSoso();
	afx_msg void OnRadioWorse();
	afx_msg void OnRadioForget();
	afx_msg void OnRadioSwitch();
	afx_msg void OnRadioOpen();
	afx_msg void OnRadioOpenic();
	afx_msg void OnRadioNowtab();
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonDeselectAll();
	afx_msg void OnRadioIcprefer();
	afx_msg void OnRadioNoprefer();
	afx_msg void OnRadioNewprefer();
	afx_msg void OnSelchangeListQuery();
	afx_msg void OnCheckNone();
	afx_msg void OnRadioInteresting();
	afx_msg void OnRadioRemote();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEEVAL_H__C6F385AB_4D55_4271_9680_73303C1B5BFE__INCLUDED_)

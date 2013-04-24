///////////////////////////////////////////////////////////////
// DlgUpload.h : header file
//
// This software was written by Tingshao Zhu.
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_DLGUPLOAD_H__B2E589B2_9DAC_4B36_B488_CC99374C29EC__INCLUDED_)
#define AFX_DLGUPLOAD_H__B2E589B2_9DAC_4B36_B488_CC99374C29EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabCtrlSSL.h"
#include "UploadAgreement.h"
#include "UploadPreview.h"

#include "FileOperations.h" 

#include "SFTPTransfer.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgUpload dialog

class GenericHTTPClient;

class CDlgUpload : public CDialog
{
// Construction
public:
	CDlgUpload(CWnd* pParent = NULL);   // standard constructor

	void refreshOKButton();

//	void EnableOKButton(BOOL bEnable);

// Dialog Data
	//{{AFX_DATA(CDlgUpload)
	enum { IDD = IDD_DIALOG_UPLOAD };
	CButton	m_okCtrl;
	CTabCtrlSSL	m_tabOptions;
	CString	m_userID;
	//}}AFX_DATA

	CUploadAgreement	m_dlgAgreement;
	CUploadPreview		m_dlgPreview;

	bool m_bRemoveDataFold;

	BOOL m_bAccept;

	CFileOperation fileOperator;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUpload)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void HTTPDownloadModel(GenericHTTPClient *pClient, const CString &userID, const CString &modelFile, const CString &localFile);
	bool HTTPUpload(GenericHTTPClient *pClient, const CString &userID, const CString &localFile, const CString &fileStem);

	void creatModelID(const CString &strModel, CString &strRemoteModel, BOOL bIDFolder=TRUE);
	void DownloadModel(CSFTPTransfer &dlg, const CString &strModel);

	void RemoveWebICLog(const CString &sDataPath);

	void CreatStemFileName(CString &stemFileName);
	void creatRemoteFileName(CString &sRemoteFile, CString &sSymbolFile, BOOL bIDFolder=TRUE);
	bool UploadSubjectFolder(CSFTPTransfer &dlg, const CString &strZip, const CString &strIniFile, BOOL bFolder=TRUE);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUpload)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUpdateEditId();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DLGUPLOAD_H__B2E589B2_9DAC_4B36_B488_CC99374C29EC__INCLUDED_)

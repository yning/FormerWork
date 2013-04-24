///////////////////////////////////////////////////////////////
// DlgUpload.cpp : implementation file
//
// This software was written by Tingshao Zhu.
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "HighTime.h"
#include "GenericHTTPClient.h"

#include "WebMind.h"
#include "Shlwapi.h"
#include "DlgUpload.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "zipper.h"
#include "ProgressWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgUpload dialog


CDlgUpload::CDlgUpload(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUpload::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUpload)
	//}}AFX_DATA_INIT

	m_bRemoveDataFold = true;
	m_bAccept = true;

	m_userID = theSettings.m_strUserID;
	m_userID.TrimLeft();
	m_userID.TrimRight();

}


void CDlgUpload::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUpload)
	DDX_Control(pDX, IDOK, m_okCtrl);
	DDX_Control(pDX, IDC_TAB_OPTIONS, m_tabOptions);
	DDX_Text(pDX, IDC_EDIT_ID, m_userID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUpload, CDialog)
	//{{AFX_MSG_MAP(CDlgUpload)
	ON_EN_UPDATE(IDC_EDIT_ID, OnUpdateEditId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUpload message handlers

BOOL CDlgUpload::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int nPageID = 0;

	m_dlgAgreement.Create(IDD_UPLOAD_AGREEMENT, this);
	m_dlgAgreement.pDlgParent = this;
	m_tabOptions.AddSSLPage (_T("Upload Agreement"), nPageID++, &m_dlgAgreement);

	m_dlgPreview.Create(IDD_UPLOAD_PREVIEW, this);
	m_tabOptions.AddSSLPage (_T("Previewing Data"), nPageID++, &m_dlgPreview);

	refreshOKButton();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUpload::OnOK() 
{
	CDialog::OnOK();

	// TODO: Add extra validation here
	CZipper zip;
	CString strStem;
	CString sZipPath;
	CString sDataPath;
	bool success = false;

	CString strIniFile;
	strIniFile = theSettings.m_location + _T("\\") + theSettings.m_appName + _T(".ini");

	sDataPath = theSettings.m_dataPath;
	if(sDataPath.IsEmpty() || !PathFileExists(sDataPath))
		return;

	// get the file stem
	CreatStemFileName(strStem);

	// this is the zip file that created
	sZipPath = theSettings.m_location + _T("\\") + strStem + _T(".zip");

	// compress the whole "Data" folder into the zip file
	if (zip.OpenZip(sZipPath))
	{
		zip.AddFolderToZip(sDataPath);
		success = true;
	}

	zip.CloseZip();

	// if we can compress the file
	if(!success)
	{
		MessageBox("Sorry, the log files cannot be compressed, please contact LILAC!");
		return;
	}

	CString sBuffer;
	CString sSymbol;
	bool bFTPUpload = false;
	bool bHTTPUpload = false;

	CProgressWnd wndProgress(this, "Upload and Download ... ");
    wndProgress.SetText("Connecting...");         
	wndProgress.SetRange(0, 6);

	// ftp dialog
//	CFTPTransferDlg dlg;
	CSFTPTransfer dlg;
	dlg.m_sServer = _T("adelade.cs.ualberta.ca");
	dlg.m_bBinary = TRUE;
	dlg.m_bPromptOverwrite = FALSE;

	dlg.m_sUserName = _T("lilacfans");
	dlg.m_sPassword = _T("lilacfans");

	// upload log files by using ftp
	wndProgress.SetText(_T("SecureFTP Uploading ...\r\n\r\n") + sZipPath);
	wndProgress.StepIt();
	wndProgress.PeekAndPump();

//	bFTPUpload = UploadSubjectFolder(dlg, sZipPath, strIniFile);
	if( bFTPUpload )
	{
		// download models

		wndProgress.SetText(_T("SecureFTP Downloading ...\r\n\r\nmodel.att"));
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		DownloadModel(dlg, theSettings.m_modelAtt);

		wndProgress.SetText(_T("SecureFTP Downloading ...\r\n\r\nicquery.mod"));
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		DownloadModel(dlg, theSettings.m_modelQuery);
	
		wndProgress.SetText(_T("SecureFTP Downloading ...\r\n\r\nicrelevant.mod"));
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		DownloadModel(dlg, theSettings.m_modelRelevant);
		
		wndProgress.SetText(_T("SecureFTP Downloading ...\r\n\r\nicword.mod"));
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		DownloadModel(dlg, theSettings.m_modelWord);
		
		wndProgress.SetText(_T("SecureFTP Downloading ...\r\n\r\npara.est"));
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		DownloadModel(dlg, theSettings.m_modelPara);
	}
	else
	{
		// if cannot upload by ftp, then try http
		GenericHTTPClient *pClient=new GenericHTTPClient();

		wndProgress.SetText(_T("HTTP Uploading ...\r\n\r\n") + sZipPath);
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		bHTTPUpload = HTTPUpload(pClient, m_userID, sZipPath, strStem);

		if( bHTTPUpload )
		{
			
			wndProgress.SetText(_T("HTTP Downloading ...\r\n\r\nicquery.mod"));
			wndProgress.StepIt();
			wndProgress.PeekAndPump();
			HTTPDownloadModel(pClient, m_userID, "icquery.mod", theSettings.m_modelQuery);
			
			wndProgress.SetText(_T("HTTP Downloading ...\r\n\r\nicrelevant.mod"));
			wndProgress.StepIt();
			wndProgress.PeekAndPump();
			HTTPDownloadModel(pClient, m_userID, "icrelevant.mod", theSettings.m_modelRelevant);
			
			wndProgress.SetText(_T("HTTP Downloading ...\r\n\r\nicword.mod"));
			wndProgress.StepIt();
			wndProgress.PeekAndPump();
			HTTPDownloadModel(pClient, m_userID, "icword.mod", theSettings.m_modelWord);
			
			wndProgress.SetText(_T("HTTP Downloading ...\r\n\r\nmodel.att"));
			wndProgress.StepIt();
			wndProgress.PeekAndPump();
			HTTPDownloadModel(pClient, m_userID, "model.att", theSettings.m_modelAtt);
			
			wndProgress.SetText(_T("HTTP Downloading ...\r\n\r\npara.est"));
			wndProgress.StepIt();
			wndProgress.PeekAndPump();
			HTTPDownloadModel(pClient, m_userID, "para.est", theSettings.m_modelPara);
		}

		delete pClient;
	}

	wndProgress.Hide();

	if(bFTPUpload || bHTTPUpload)
	{
		// delete the zip file at first
		if(!fileOperator.Delete(sZipPath))
			m_LogTrace.WriteLine("%s\n", fileOperator.GetErrorString());

		if(!sDataPath.IsEmpty())
		{
			if(m_bRemoveDataFold)
				fileOperator.Delete(sDataPath);
			else
				RemoveWebICLog(sDataPath);
		}

		m_userID.TrimLeft();
		m_userID.TrimRight();

		theSettings.m_lastUpload = CHighTime::GetPresentTime();
		theSettings.m_numUpload += 1;
		theSettings.m_strUserID = m_userID;
		theSettings.saveSettings();
	}
	else
	{
		MessageBox(
			_T("You can't connect to the LILAC ftp server, either your Internet connection is off ")
			_T("or you have input wrong ID!\r\n")
			_T("Please double check your internet connection and your LILAC ID number before next uploading!\r\n\r\n")
			_T("If you have any questions, please contact us at : lilac@cs.ualberta.ca\r\n\r\n"),
			"Upload", MB_ICONWARNING | MB_OK );

		if(!fileOperator.Delete(sZipPath))
			m_LogTrace.WriteLine("%s\n", fileOperator.GetErrorString());

		/*
		if( MessageBox(
			_T("You can't connect to the LILAC ftp server, either your Internet connection is off \r\n or the ftp service is prohibited for your case!\r\n")
			_T("If your Internet connection is on, you can try to upload your data to our http server.\r\n\r\n")
			_T("Do you want to upload by using http?"),
			"Upload", MB_ICONWARNING | MB_YESNO ) == IDYES)
		{
			CWebICApp *app=(CWebICApp *)AfxGetApp();
			app->GotoURL(_T("http://www.web-ic.com/lilac/upload.php"));
		}
		else
		{
			if(!fileOperator.Delete(sZipPath))
				m_LogTrace.WriteLine("%s\n", fileOperator.GetErrorString());
		}
		*/
	}
}


// upload log and protocol file into ftp server
bool CDlgUpload::UploadSubjectFolder(CSFTPTransfer &dlg, const CString &strZip, const CString &strIniFile, BOOL bFolder)
{
	CString strRemote;
	CString strSymbol;

	creatRemoteFileName(strRemote, strSymbol, bFolder);

	// upload the log file
	dlg.m_bDownload = FALSE;
	dlg.m_sLocalFile = strZip;
	dlg.m_sRemoteFile = strRemote;

	if (dlg.DoModal())
	{
		// upload the symbol file to follow the upload protocol.
		dlg.m_sLocalFile = strIniFile;
		dlg.m_sRemoteFile = strSymbol;
		dlg.DoModal();

		return true;
	}

	return false;
}


void CDlgUpload::creatRemoteFileName(CString &sRemoteFile, CString &sSymbolFile, BOOL bIDFolder)
{
	CString strStem;
	CreatStemFileName(strStem);

	sRemoteFile.Empty();
	sSymbolFile.Empty();
	if(bIDFolder)
	{
		sRemoteFile.Format("ftp_data/%s/data/%s.zip", m_userID, strStem);
		sSymbolFile.Format("ftp_data/%s/data/%s.uploadComplete", m_userID, strStem);
	}
	else
	{
		sRemoteFile.Format("ftp_data/anonymous/data/%s.zip", strStem);
		sSymbolFile.Format("ftp_data/anonymous/data/%s.uploadComplete", strStem);
	}
}

void CDlgUpload::CreatStemFileName(CString &stemFileName)
{
    CHighTime ct;
	CString sTime;

	ct = CHighTime::GetPresentTime();
	sTime = ct.Format("%m%d%s");

	stemFileName.Empty();
	m_userID.TrimLeft();
	m_userID.TrimRight();
	stemFileName.Format("%s-%s-%d", m_userID, sTime, theSettings.m_numUpload + 1);

}


void CDlgUpload::RemoveWebICLog(const CString &sDataPath)
{
	CStringArray m_logWebIC;
	fileOperator.GetLILACLog(sDataPath, m_logWebIC);
	for(int i = 0; i < m_logWebIC.GetSize(); i++)
		fileOperator.Delete(m_logWebIC[i]);
}

// download the model file
void CDlgUpload::DownloadModel(CSFTPTransfer &dlg, const CString &strModel)
{
	CString strRemoteModel;
	CString strTmp;

	strTmp = strModel + _T(".tmp");

	dlg.m_bDownload = TRUE;
	dlg.m_sLocalFile = strTmp;

//	dlg.m_sLocalFile = strModel;

	creatModelID(strModel, strRemoteModel);
	dlg.m_sRemoteFile = strRemoteModel;

	// *debug*
	//m_LogTrace.WriteLine("\nDownload Models File :\n%s\n%s\n", strTmp, strRemoteModel);

	if (dlg.DoModal())
	{
		fileOperator.SetOverwriteMode(true);
		fileOperator.Move(strTmp, strModel);
		return;
	}

	/*
	if( MessageBox("I cannot find your folder at LILAC server, please report this to LILAC. \r\n\r\nDo you want to update your current model with the generic one?", 
		"Updating Model",
		MB_ICONWARNING | MB_OKCANCEL ) == IDOK)
	{
		creatModelID(strModel, strRemoteModel, FALSE);
		dlg.m_sRemoteFile = strRemoteModel;
		if(dlg.DoModal() == IDOK)
			fileOperator.Move(strTmp, strModel);
	}
	*/
}


void CDlgUpload::creatModelID(const CString &strModel, CString &strRemoteModel, BOOL bIDFolder)
{
	fileOperator.ParsePath(strModel);

	strRemoteModel.Empty();
	if(bIDFolder)
		strRemoteModel.Format("ftp_data/%s/models/%s%s", m_userID, fileOperator.GetFileTitle(), fileOperator.GetExtName());
	else
		strRemoteModel.Format("ftp_data/anonymous/models/%s%s", fileOperator.GetFileTitle(), fileOperator.GetExtName());
}


void CDlgUpload::OnUpdateEditId() 
{
	UpdateData();
	m_userID.TrimLeft();
	m_userID.TrimRight();

	refreshOKButton();
}

/*id CDlgUpload::EnableOKButton(BOOL bEnable)
{
	m_bAccept = bEnable;
	refreshOKButton();
}
*/

void CDlgUpload::refreshOKButton() 
{
//	if(m_bAccept && !m_userID.IsEmpty())
	if(!m_userID.IsEmpty())
		m_okCtrl.EnableWindow(TRUE);
	else
		m_okCtrl.EnableWindow(FALSE);
}



void CDlgUpload::HTTPDownloadModel(GenericHTTPClient *pClient, const CString &userID, const CString &modelFile, const CString &localFile)
{
	CString strModel;
	strModel = _T("http://www.web-ic.com/http_data/") + userID + _T("/models/") + modelFile;
	if( pClient->Request(strModel) )
	{
		DWORD dwSize;
		TCHAR *pResponse;

		dwSize = pClient->QueryResponseSize();
		pResponse = pClient->QueryResponse();
		if(dwSize && strstr(pResponse, "404 Not Found") == NULL)
		{
			CFile m_wFile( localFile, CFile::modeCreate | CFile::modeWrite);

			m_wFile.Write(pResponse, dwSize);
			m_wFile.Flush();

			m_wFile.Close();
		}
	}
}


bool CDlgUpload::HTTPUpload(GenericHTTPClient *pClient, const CString &userID, const CString &localFile, const CString &fileStem) 
{
	pClient->InitilizePostArguments();
	pClient->AddPostArguments("uploadzip", localFile, true);
	pClient->AddPostArguments("userID", userID);
	pClient->AddPostArguments("uploadFileName", fileStem);

	if(pClient->Request("http://www.web-ic.com/lilacupload.php", GenericHTTPClient::RequestPostMethodMultiPartsFormData))
	{ 
		LPCTSTR szResult=pClient->QueryHTTPResponse();

		if(strstr(szResult, "Submission failed") != NULL)
			return false;

		return true;
	}

	return false;
}

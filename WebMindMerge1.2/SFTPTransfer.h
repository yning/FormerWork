// SFTPTransfer.h: interface for the CSFTPTransfer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SFTPTRANSFER_H__108DD673_3C69_461E_A4C0_B458C97A059A__INCLUDED_)
#define AFX_SFTPTRANSFER_H__108DD673_3C69_461E_A4C0_B458C97A059A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


extern "C" {
#include "psftp.h"
#include "putty.h"
}


class CSFTPTransfer  
{
public:
	CSFTPTransfer();
	virtual ~CSFTPTransfer();

public:
	BOOL DoModal();

public:

	//Public Member variables
	BOOL			m_bDownload;           //TRUE if it's a download, FALSE if an upload
	CString			m_sServer;             //e.g. "ftp.microsoft.com"
	CString			m_sRemoteFile;         //e.g. "/pub/somefile.ext"
	CString			m_sLocalFile;          //e.g. "c:\temp\somfile.txt"
	CString			m_sUserName;           //Username to login to the server with
	CString			m_sPassword;           //password to login to the server with
	int				m_nPort;               //If you want to change the port to make access on, by default it will be 21

	BOOL          m_bBinary;             //TRUE if binary transfer, FALSE for ascii transfer
	BOOL          m_bPromptOverwrite;    //Should the user be prompted to overwrite files


protected:
	int psftp_init();
	int psftp_deinit();

	void parseSequence(const CString &strSeq, const CString &strSplitter, CStringArray &strList);

protected:
	BOOL			m_bInited;




};

#endif // !defined(AFX_SFTPTRANSFER_H__108DD673_3C69_461E_A4C0_B458C97A059A__INCLUDED_)

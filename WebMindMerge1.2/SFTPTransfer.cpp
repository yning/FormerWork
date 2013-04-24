// SFTPTransfer.cpp: implementation of the CSFTPTransfer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SFTPTransfer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// the secure ftp interface

CSFTPTransfer::CSFTPTransfer()
{
	m_bDownload = TRUE;
	m_nPort = 0;
	m_bBinary = TRUE;
	m_bPromptOverwrite = TRUE;

	m_bInited = FALSE;
}



CSFTPTransfer::~CSFTPTransfer()
{
	if(m_bInited)
	{
		psftp_deinit();
	}
}

int CSFTPTransfer::psftp_init()
{
    int portnumber = 0;

    flags = FLAG_STDERR | FLAG_INTERACTIVE;
    cmdline_tooltype = TOOLTYPE_FILETRANSFER;
    ssh_get_line = &console_get_line;
    sk_init();

    /* Load Default Settings before doing anything else. */
    do_defaults(NULL, &cfg);
    loaded_session = FALSE;

	set_user((char *)(LPCSTR)m_sUserName, &cfg);
	set_passwd((char *)(LPCSTR)m_sPassword, &cfg);


	back = NULL;

	int ret;
	ret = psftp_connect((char *)(LPCSTR)m_sServer, (char *)(LPCSTR)m_sUserName, m_nPort);
//	sfree(userhost);
	if (ret)
	    return 1;
	if (do_sftp_init())
	    return 1;

	m_bInited = TRUE;
	return 0;
}

int CSFTPTransfer::psftp_deinit()
{
    if (back != NULL && back->socket(backhandle) != NULL)
	{
		char ch;
		back->special(backhandle, TS_EOF);
		sftp_recvdata(&ch, 1);
    }
    
	random_save_seed();
    cmdline_cleanup();
    console_provide_logctx(NULL);
    do_sftp_cleanup();
    backhandle = NULL;
    back = NULL;
    sk_cleanup();

	m_bInited = FALSE;
	return 0;
}


BOOL CSFTPTransfer::DoModal()
{
	int i, ret = 0;

	if(!m_bInited)
	{
		ret = psftp_init();
		if(ret != 0)
			return FALSE;
	}

	if(m_sLocalFile.IsEmpty() || m_sRemoteFile.IsEmpty())
		return FALSE;

	bool m_bRemoteFromRoot = false;
	if(m_sRemoteFile[0] == '/')
		m_bRemoteFromRoot = true;

	CStringArray m_arRemotePath;
	CStringArray m_arLocalPath;
	parseSequence(m_sRemoteFile, "/", m_arRemotePath);
	parseSequence(m_sLocalFile, "\\", m_arLocalPath);

	CString m_remoteFolder;
	m_remoteFolder.Empty();
	if(m_bRemoteFromRoot)
		m_remoteFolder = "/";

	for(i = 0; i < m_arRemotePath.GetSize()-1; i++)
		m_remoteFolder += m_arRemotePath[i] + _T("/");
	if(!m_remoteFolder.IsEmpty())
	{
		ret = sftp_cd(m_remoteFolder);
		if(ret == 0)
			return FALSE;
	}

	CString m_localFolder;
	m_localFolder.Empty();
	for(i = 0; i < m_arLocalPath.GetSize()-1; i++)
		m_localFolder += m_arLocalPath[i] + _T("\\");
	sftp_lcd(m_localFolder);

	if(m_bDownload)
		ret = sftp_get(m_arRemotePath[m_arRemotePath.GetSize()-1]);
	else
		ret = sftp_put(m_arLocalPath[m_arLocalPath.GetSize()-1]);

	sftp_cd(homedir);

	if(ret == 0)
		return FALSE;

	return TRUE;
}



void CSFTPTransfer::parseSequence(const CString &strSeq, const CString &strSplitter, CStringArray &strList)
{
	CString strBuffer;
	CString strUnit;
	int index;

	strList.RemoveAll();
	strBuffer = strSeq;

	while( (index = strBuffer.Find(strSplitter)) != -1)
	{
		strUnit.Empty();
		strUnit = strBuffer.Left(index);
		if(!strUnit.IsEmpty())
			strList.Add(strUnit);

		strBuffer = strBuffer.Right(strBuffer.GetLength() - index - strSplitter.GetLength());
	}

	if(!strBuffer.IsEmpty())
		strList.Add(strBuffer);

	strBuffer.Empty();
}
///////////////////////////////////////////////////////////////
// FileOperations.h: Declaration of CFileOperation class and CFileExeption class.
//
///////////////////////////////////////////////////////////////

#if !defined(BASE_FILE_OPERATIONS_)
#define BASE_FILE_OPERATIONS_

#define PATH_ERROR			-1
#define PATH_NOT_FOUND		0
#define PATH_IS_FILE		1
#define PATH_IS_FOLDER		2

class CFExeption
{
public:
	CFExeption(DWORD dwErrCode);
	CFExeption(CString sErrText);
	CString GetErrorText() {return m_sError;}
	DWORD GetErrorCode() {return m_dwError;}

private:
	CString m_sError;
	DWORD m_dwError;
};

class CFileOperation
{
public:
	CFileOperation();										// constructor
	bool Delete(CString sPathName);							// delete file or folder
	bool Copy(CString sSource, CString sDest);				// copy file or folder
	bool Move(CString sSource, CString sDest);				// move file or folder
	bool Rename(CString sSource, CString sDest);			// rename file or folder

	CString GetErrorString() {return m_sError;}				// return error description
	DWORD GetErrorCode() {return m_dwError;}				// return error code
	void ShowError()
	{
		// show error message
		MessageBox(NULL, m_sError, _T("Error"), MB_OK | MB_ICONERROR);
	}
	void SetAskIfReadOnly(bool bAsk = true)					// sets behavior with readonly files(folders)
	{
		m_bAskIfReadOnly = bAsk;
	}
	bool IsAskIfReadOnly()									// return current behavior with readonly files(folders)
	{
		return m_bAskIfReadOnly;
	}
	bool CanDelete(CString sPathName);						// check attributes
	void SetOverwriteMode(bool bOverwrite = false)			// sets overwrite mode on/off
	{
		m_bOverwriteMode = bOverwrite;
	}
	bool IsOverwriteMode()									// return current overwrite mode
	{
		return m_bOverwriteMode;
	}
	int CheckPath(CString sPath);
	bool IsAborted() {return m_bAborted;}

	bool IsFileExist(CString sPathName);

	bool GetSubDir(LPCTSTR lpszName, CStringArray &subDirs);
	void DoGetSubDir(LPCTSTR sSource, CStringArray &subDirs);

	bool GetFileList(const CString &sSource, const CString &strExtension, CStringArray &m_fileList, bool bFullPath=true);
	void DoGetFileList(const CString &sSource, const CString &strExtension, CStringArray &m_fileList, bool bFullPath=true);
	
	bool isLILACLogFile(const CString &m_file);
	bool GetLILACLog(const CString &strPath, CStringArray &m_logList);

	bool CreatFullPath(const CString &strBuffer);


protected:
	void DoDelete(CString sPathName);
	void DoCopy(CString sSource, CString sDest, bool bDelteAfterCopy = false);
	void DoFileCopy(CString sSourceFile, CString sDestFile, bool bDelteAfterCopy = false);
	void DoFolderCopy(CString sSourceFolder, CString sDestFolder, bool bDelteAfterCopy = false);
	void DoRename(CString sSource, CString sDest);
	void PreparePath(CString &sPath);
	void Initialize();
	void CheckSelfRecursion(CString sSource, CString sDest);
	bool CheckSelfCopy(CString sSource, CString sDest);
	CString ChangeFileName(CString sFileName);
	CString ParseFolderName(CString sPathName);

private:
	CString m_sError;
	DWORD m_dwError;
	bool m_bAskIfReadOnly;
	bool m_bOverwriteMode;
	bool m_bAborted;
	int m_iRecursionLimit;

public:
	void ParsePath(const CString &sSource);
	CString GetDrive()			{	return _sDrive;			}
	CString GetDir()			{	return _sDir;			}
	CString GetFileTitle()		{	return _sFileTitle;		}
	CString GetExtName()		{	return _sExtName;		}

private:
	CString			_sDrive;
	CString			_sDir;
	CString			_sFileTitle;
	CString			_sExtName;

};

#endif
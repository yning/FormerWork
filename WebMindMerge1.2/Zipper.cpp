// Zipper.cpp: implementation of the CZipper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Zipper.h"

#include "zip.h"
#include "iowin32.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const UINT BUFFERSIZE = 2048;

CZipper::CZipper(LPCTSTR szFilePath, LPCTSTR szRootFolder, bool bAppend) : m_uzFile(0)
{
	ZeroMemory(&m_info, sizeof(m_info));

	if (szFilePath)
		OpenZip(szFilePath, szRootFolder, bAppend);
}

CZipper::~CZipper()
{
	CloseZip();
}

// simple interface
bool CZipper::ZipFile(LPCTSTR szFilePath)
{
	// make zip path
	char szDrive[_MAX_DRIVE], szFolder[MAX_PATH], szName[_MAX_FNAME];
	_splitpath(szFilePath, szDrive, szFolder, szName, NULL);

	char szZipPath[MAX_PATH];
	_makepath(szZipPath, szDrive, szFolder, szName, "zip");

	CZipper zip;

	if (zip.OpenZip(szZipPath, false))
		return zip.AddFileToZip(szFilePath, false);

	return FALSE;
}
	
bool CZipper::ZipFolder(LPCTSTR szFilePath, bool bIgnoreFilePath)
{
	// make zip path
	char szDrive[_MAX_DRIVE], szFolder[MAX_PATH], szName[_MAX_FNAME];
	_splitpath(szFilePath, szDrive, szFolder, szName, NULL);

	char szZipPath[MAX_PATH];
	_makepath(szZipPath, szDrive, szFolder, szName, "zip");

	CZipper zip;

	if (zip.OpenZip(szZipPath, FALSE))
		return zip.AddFolderToZip(szFilePath, bIgnoreFilePath);

	return FALSE;
}
	
// works with prior opened zip
bool CZipper::AddFileToZip(LPCTSTR szFilePath, bool bIgnoreFilePath)
{
	if (!m_uzFile)
		return FALSE;

	// we don't allow paths beginning with '..\' because this would be outside
	// the root folder
	if (!bIgnoreFilePath && strstr(szFilePath, "..\\") == szFilePath)
		return false;

	TRACE ("CZipper::AddFileToZip(%s)\n", szFilePath);

	bool bFullPath = (strchr(szFilePath, ':') != NULL);

	// if the file is relative then we need to append the root before opening
	char szFullFilePath[MAX_PATH];
	
	lstrcpy(szFullFilePath, szFilePath);
	PrepareSourcePath(szFullFilePath);

	// if the file is a fullpath then remove the root path bit
	char szFileName[MAX_PATH] = "";

	if (bIgnoreFilePath)
	{
		char szName[_MAX_FNAME], szExt[_MAX_EXT];
		_splitpath(szFilePath, NULL, NULL, szName, szExt);

		_makepath(szFileName, NULL, NULL, szName, szExt);
	}
	else if (bFullPath)
	{
		// check the root can be found
		if (0 != _strnicmp(szFilePath, m_szRootFolder, lstrlen(m_szRootFolder)))
			return false;

		// else
		lstrcpy(szFileName, szFilePath + lstrlen(m_szRootFolder));
	}
	else // relative path
	{
		// if the path begins with '.\' then remove it
		if (strstr(szFilePath, ".\\") == szFilePath)
			lstrcpy(szFileName, szFilePath + 2);
		else
			lstrcpy(szFileName, szFilePath);
	}

	// load input file
	HANDLE hInputFile = ::CreateFile(szFullFilePath, 
									GENERIC_READ,
									0,
									NULL,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_READONLY,
									NULL);

	if (hInputFile == INVALID_HANDLE_VALUE)
		return FALSE;

	int nRet = zipOpenNewFileInZip(m_uzFile, 
									szFileName,
									NULL, 
									NULL, 
									0,
									NULL,
									0, 
									NULL,
									Z_DEFLATED,
									Z_DEFAULT_COMPRESSION);

	if (nRet == ZIP_OK)
	{
		m_info.nFileCount++;

		// read the file and output to zip
		char pBuffer[BUFFERSIZE];
		DWORD dwBytesRead = 0, dwFileSize = 0;

		while (nRet == ZIP_OK && ::ReadFile(hInputFile, pBuffer, BUFFERSIZE, &dwBytesRead, NULL))
		{
			dwFileSize += dwBytesRead;

			if (dwBytesRead)
				nRet = zipWriteInFileInZip(m_uzFile, pBuffer, dwBytesRead);
			else
				break;
		}

		m_info.dwUncompressedSize += dwFileSize;
	}

	zipCloseFileInZip(m_uzFile);
	::CloseHandle(hInputFile);

	return (nRet == ZIP_OK);
}

bool CZipper::AddFileToZip(LPCTSTR szFilePath, LPCTSTR szRelFolderPath)
{
	if (!m_uzFile)
		return FALSE;

	// szRelFolderPath cannot contain drive info
	if (szRelFolderPath && strchr(szRelFolderPath, ':'))
		return FALSE;

	// if the file is relative then we need to append the root before opening
	char szFullFilePath[MAX_PATH];
	
	lstrcpy(szFullFilePath, szFilePath);
	PrepareSourcePath(szFullFilePath);

	HANDLE hInputFile = ::CreateFile(szFullFilePath, 
									GENERIC_READ,
									0,
									NULL,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_READONLY,
									NULL);

	if (hInputFile == INVALID_HANDLE_VALUE)
		return FALSE;

	// strip drive info off filepath
	char szName[_MAX_FNAME], szExt[_MAX_EXT];
	_splitpath(szFilePath, NULL, NULL, szName, szExt);

	// prepend new folder path 
	char szFileName[MAX_PATH];
	_makepath(szFileName, NULL, szRelFolderPath, szName, szExt);

	// open the file in the zip making sure we remove any leading '\'
	int nRet = zipOpenNewFileInZip(m_uzFile, 
									szFileName + (szFileName[0] == '\\' ? 1 : 0),
									NULL, 
									NULL, 
									0,
									NULL,
									0, 
									NULL,
									Z_DEFLATED,
									Z_DEFAULT_COMPRESSION);

	if (nRet == ZIP_OK)
	{
		m_info.nFileCount++;

		// read the file and output to zip
		char pBuffer[BUFFERSIZE];
		DWORD dwBytesRead = 0, dwFileSize = 0;

		while (nRet == ZIP_OK && ::ReadFile(hInputFile, pBuffer, BUFFERSIZE, &dwBytesRead, NULL))
		{
			dwFileSize += dwBytesRead;

			if (dwBytesRead)
				nRet = zipWriteInFileInZip(m_uzFile, pBuffer, dwBytesRead);
			else
				break;
		}

		m_info.dwUncompressedSize += dwFileSize;
	}

	zipCloseFileInZip(m_uzFile);
	::CloseHandle(hInputFile);

	return (nRet == ZIP_OK);
}

bool CZipper::AddFolderToZip(LPCTSTR szFolderPath, bool bIgnoreFilePath)
{
	if (!m_uzFile)
		return FALSE;

	m_info.nFolderCount++;

	// if the path is relative then we need to append the root before opening
	char szFullPath[MAX_PATH];
	
	lstrcpy(szFullPath, szFolderPath);
	PrepareSourcePath(szFullPath);

	// build searchspec
	char szDrive[_MAX_DRIVE], szFolder[MAX_PATH], szName[_MAX_FNAME];
	_splitpath(szFullPath, szDrive, szFolder, szName, NULL);
	lstrcat(szFolder, szName);

	char szSearchSpec[MAX_PATH];
	_makepath(szSearchSpec, szDrive, szFolder, "*", "*");

	WIN32_FIND_DATA finfo;
	HANDLE hSearch = FindFirstFile(szSearchSpec, &finfo);

	if (hSearch != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			if (finfo.cFileName[0] != '.') 
			{
				char szItem[MAX_PATH];
				_makepath(szItem, szDrive, szFolder, finfo.cFileName, NULL);
				
				if (finfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					AddFolderToZip(szItem, bIgnoreFilePath);
				}
				else 
					AddFileToZip(szItem, bIgnoreFilePath);
			}
		} 
		while (FindNextFile(hSearch, &finfo));
		
		FindClose(hSearch);
	}

	return TRUE;
}

// extended interface
bool CZipper::OpenZip(LPCTSTR szFilePath, LPCTSTR szRootFolder, bool bAppend)
{
	CloseZip();

	m_szRootFolder[0] = 0;
	m_uzFile = zipOpen(szFilePath, bAppend ? 1 : 0);
	ZeroMemory(&m_info, sizeof(m_info));

	if (m_uzFile)
	{
		if (!szRootFolder)
		{
			char szDrive[_MAX_DRIVE], szFolder[MAX_PATH];
			_splitpath(szFilePath, szDrive, szFolder, NULL, NULL);

			_makepath(m_szRootFolder, szDrive, szFolder, NULL, NULL);
		}
		else if (lstrlen(szRootFolder))
		{
			_makepath(m_szRootFolder, NULL, szRootFolder, "", NULL);
		}
	}

	return (m_uzFile != NULL);
}

bool CZipper::CloseZip()
{
	int nRet = m_uzFile ? zipClose(m_uzFile, NULL) : ZIP_OK;
	m_uzFile = NULL;

	return (nRet == ZIP_OK);
}

void CZipper::GetFileInfo(Z_FileInfo& info)
{
	info = m_info;
}

void CZipper::PrepareSourcePath(LPTSTR szPath)
{
	bool bFullPath = (strchr(szPath, ':') != NULL);

	// if the file is relative then we need to append the root before opening
	if (!bFullPath)
	{
		char szTemp[MAX_PATH];
		lstrcpy(szTemp, szPath);

		_makepath(szPath, NULL, m_szRootFolder, szTemp, NULL);
	}
}

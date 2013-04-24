///////////////////////////////////////////////////////////////
// LogTrace.cpp: Implementation of the CLogTrace class
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxdisp.h>
#include "LogTrace.h"


//////////////////////////////////////////////////////
//  Construction/Destruction

CLogTrace::CLogTrace()
{
	m_bActive = FALSE;
	m_bTimeStamp = TRUE;
}


CLogTrace::~CLogTrace()
{
}

////////////////////////////////////////////////////////
//  CLogTrace operations

void CLogTrace::ResetFile()
{
	CStdioFile f;
	CFileException fe;
	CString s;

	if (m_strFileName.IsEmpty())
		return;

	if (f.Open(m_strFileName, CFile::modeWrite | CFile::modeCreate, &fe) == FALSE)
		return;

	f.Close();
}



// bActive tells us if we want the trace to be active or not
// bTimeStamp tells us if we want time stamps on each line
// eliminating the time stamp allows us to use this class for a regular log file
void CLogTrace::OnStartup(BOOL bActive, BOOL bTimeStamp)
{
	m_bActive = bActive;
	m_bTimeStamp = bTimeStamp;
	if (bTimeStamp == FALSE)
		return;

	// these ***'s help to indicate when one ru of the program ends and another starts
	// because we don't always overwrite the file each time

	WriteLine("\n\n******************************************\n\n");
	WriteLine("%s Log Trace %s\n\n", m_strAppName, COleDateTime::GetCurrentTime().Format());
}


void CLogTrace::WriteLine(const char* fmt, ...)
{
	if (!AfxIsValidString(fmt, -1))
		return;

	// parse that string format
	try
	{
		va_list argptr;
		va_start(argptr, fmt);
		_vsnprintf(m_tBuf, TBUF_SIZE, fmt, argptr);
		va_end(argptr);
	}
	catch (...)
	{
		m_tBuf[0] = 0;
	}

	Write(m_tBuf);
}

// function to write a line of text to the log file
void CLogTrace::Write(LPCTSTR szLine)
{
	CStdioFile f;
	CFileException fe;
	CString s;

	if (m_bActive == FALSE)
		return;

	if (m_strFileName.IsEmpty())
		return;

	if (f.Open(m_strFileName, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate, &fe) == FALSE)
		return;

	try
	{
		f.SeekToEnd();
		if (m_bTimeStamp)
			s.Format("%s\t%s", COleDateTime::GetCurrentTime().Format(), szLine);
		else
			s.Format("%s", szLine);
		f.WriteString(s);
	}
	catch (CException* e)
	{
		e->Delete();
	}
	f.Close();
}


// function to set the log file name.  don't pass a fill path!
// just pass something like "log.txt"
// the file will be placed in the same dir as the exe file
void CLogTrace::SetFileName(LPCTSTR szFileName)
{
	TCHAR drive[_MAX_PATH], dir[_MAX_PATH], name[_MAX_PATH], ext[_MAX_PATH];
	const char *path = _pgmptr ;

	_splitpath(path, drive, dir, name, ext);
	m_strFileName.Format("%s%s%s", drive, dir, szFileName);
}
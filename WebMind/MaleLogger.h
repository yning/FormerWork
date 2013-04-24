// CMaleLogger class provides easy-to-use, powerful, extra low CPU consuming
// and thread safe logging system to multithreaded programms.
// It essentially supports multidirectional log storage system
// (a single instance provides logging to different files independently).
// Include this header file where it is needed.
// I. Common simple usage of thread-depended class instance:
// Define file prefix like g_FilePrefix="C:\MyApp_" to get files look
// like "C:\MyApp_[2002-03-05].log"
//  1. Global definition: CMaleLogger g_Logger;
//  2. Call g_Logger.AddLog(g_FilePrefix, "Some log message..."); at your
//     code where logging is needed.
//  3. Call g_Logger.Process() class member function periodically inside your own thread.
//
// II. Common simple usage of stand-alone class instance:
// Define file prefix like g_FilePrefix="C:\MyApp_" to get files look
// like "C:\MyApp_[2002-03-05].log"
//  1. Global definition: CMaleLogger g_Logger(0, "log", NULL, NULL, true);
//  2. Call g_Logger.AddLog(g_FilePrefix, "Some log message..."); at your
//     code where logging is needed.
//
// You may use this code freely without any restrictions except you MAY NOT change
// the name of the class and the names of its member functions, variables and the name of the mutex object.
// Feel free to e-mail me any comments, suggestions and bug reports.
// (C) 2002 Arlen Albert. arlen_albert@pisem.net

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _MALE_LOGGER
#define _MALE_LOGGER

#include <afxmt.h>
#include <io.h>

class CMaleLogger
{
private:
	CMapStringToPtr m_MLQueue;
	CMapStringToPtr m_MLQueueHeaderTime;
	bool (*fpHeaderCallback)(CString &iLogHeader, const CString &iLogPath, const CTime &iHeaderTime);
	bool (*fpOverflowCallback)(const CFileException *fe, const HANDLE ihProcessThread, int &iOverflowLimiter, int &iUpdateFrequency, CString &iLogPath, const CStringList *iQueuedMessages, const CString &iFilePathTimeStampFormat, const CString &iMessageTimeStampFormat);
	CMutex *m_pMLMutex;
	HANDLE m_hProcessThread;
	DWORD m_InternalThreadID;
	POSITION m_Pos;
	int m_UpdateFrequency;
	int m_OverflowLimiter;

	CString m_FileExt;
	CString m_FilePathTimeStampFormat;
	CString m_MessageTimeStampFormat;

	friend static unsigned long __stdcall ProcessThread(void *iMaleLoggerInstance)
	{
		CMaleLogger *pLogger=(CMaleLogger*) iMaleLoggerInstance;
		if(!pLogger)
		{
			pLogger->m_hProcessThread=NULL;
			return TRUE;
		}
		
		while(true)
		{
			Sleep(pLogger->m_UpdateFrequency);
			pLogger->Process();
		}

		return FALSE;
	}

	void QueueLogMessage(CString &iLogPath, CString &iLogMessage, bool iUseTimeStampForFilePath, bool iUseTimeStampForMessage)
	{
		CTime TimeStamp=CTime::GetCurrentTime();
		CString Path=iLogPath, Message;
		if(iUseTimeStampForFilePath)
			Path+=TimeStamp.Format(m_FilePathTimeStampFormat);
		Path+=m_FileExt;
		if(iUseTimeStampForMessage)
			Message=TimeStamp.Format(m_MessageTimeStampFormat)+iLogMessage+"\n";
		else
			Message=iLogMessage+"\n";
		CStringList *List;
		if(m_MLQueue.Lookup(Path, (void*&)List))
			List->AddTail(Message);
		else
		{
			List=new CStringList;
			List->AddTail(Message);
			m_MLQueue.SetAt(Path, List);
			m_MLQueueHeaderTime.SetAt(Path, (void*)TimeStamp.GetTime());
		}
	}

	void Processor(bool iIgnoreAccess)
	{
		CString Path;
		CStringList *List;
		if(!m_Pos)
			m_Pos=m_MLQueue.GetStartPosition();
		m_MLQueue.GetNextAssoc(m_Pos, Path, (void*&)List);
		CStdioFile file;
		CFileException e;
		if(file.Open(Path, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::typeText,&e))
		{
			if(!file.SeekToEnd())
			{
				CString Header;
				if(fpHeaderCallback)
				{
					void *HeaderTime;
					if(m_MLQueueHeaderTime.Lookup(Path, HeaderTime))
					{
						if(fpHeaderCallback(Header, Path, (CTime&) HeaderTime))
							file.WriteString(Header+"\n");
					}
				}
			}

			file.WriteString(List->GetHead());
			file.Close();
			if(!iIgnoreAccess)
			{
				List->RemoveHead();
				if(List->IsEmpty())
				{
					delete List;
					m_MLQueue.RemoveKey(Path);
					m_MLQueueHeaderTime.RemoveKey(Path);
					m_Pos=NULL;
				}
			}
		}
		else
		{
			if(!iIgnoreAccess)
			{
				if(m_OverflowLimiter&&List->GetCount()>m_OverflowLimiter)
				{
					if(fpOverflowCallback)
					{
						CString OldPath(Path);
						if(fpOverflowCallback(&e, m_hProcessThread, m_OverflowLimiter, m_UpdateFrequency, Path, List, m_FilePathTimeStampFormat, m_MessageTimeStampFormat))
						{
							if(OldPath.CompareNoCase(Path))
							{
								m_MLQueue.RemoveKey(OldPath);
								m_MLQueue.SetAt(Path,List);
								void *HeaderTime;
								if(m_MLQueueHeaderTime.Lookup(OldPath, HeaderTime))
									m_MLQueueHeaderTime.SetAt(Path,HeaderTime);
								m_MLQueueHeaderTime.RemoveKey(OldPath);
								m_Pos=NULL;
							}
						}
					
						if(List->IsEmpty())
						{
							delete List;
							m_MLQueue.RemoveKey(Path);
							m_MLQueueHeaderTime.RemoveKey(Path);
							m_Pos=NULL;
						}
					}
					else
					{
						List->RemoveHead();
						if(List->IsEmpty())
						{
							delete List;
							m_MLQueue.RemoveKey(Path);
							m_MLQueueHeaderTime.RemoveKey(Path);
							m_Pos=NULL;
						}
					}
				}
				else
				{
					switch(e.m_lOsError)
					{
					case ERROR_PATH_NOT_FOUND:
						MakeSurePathExists(e.m_strFileName, true, true);
						break;
					default:
						break;
					}
				}
			}
		}
		
		if(iIgnoreAccess)
		{
			List->RemoveHead();
			if(List->IsEmpty())
			{
				delete List;
				m_MLQueue.RemoveKey(Path);
				m_MLQueueHeaderTime.RemoveKey(Path);
				m_Pos=NULL;
			}
		}
	}

public:
	CMaleLogger(int iOverflowLimiter=0, CString iFileExt="log", bool (*ifpHeaderCallback)(CString &iLogHeader, const CString &iLogPath, const CTime &iHeaderTime)=NULL, bool (*ifpOverflowCallback)(const CFileException *fe, const HANDLE ihProcessThread, int &iOverflowLimiter, int &iUpdateFrequency, CString &iLogPath, const CStringList *iQueuedMessages, const CString &iFilePathTimeStampFormat, const CString &iMessageTimeStampFormat)=NULL, bool iUseInternalThread=false, int iUpdateFrequency=3000, CString iFilePathTimeStampFormat="[%Y-%m-%d]", CString iMessageTimeStampFormat="[%d-%m-%Y (%H:%M:%S)] ")
	{
		m_pMLMutex=new CMutex(FALSE, "MaleLogger");
		m_OverflowLimiter=iOverflowLimiter;
		m_UpdateFrequency=iUpdateFrequency;
		if(!iFileExt.IsEmpty())
			m_FileExt="."+iFileExt;
		fpHeaderCallback=ifpHeaderCallback;
		fpOverflowCallback=ifpOverflowCallback;
		m_hProcessThread=NULL;
		m_InternalThreadID=0;
		m_FilePathTimeStampFormat=iFilePathTimeStampFormat;
		m_MessageTimeStampFormat=iMessageTimeStampFormat;
		m_Pos=NULL;
		if(iUseInternalThread)
			StartInternalThread();
	}

	~CMaleLogger()
	{
		KillInternalThread();
		Flash(true);
		delete m_pMLMutex;
	}

	void SetUpdateFrequency(int iUpdateFrequency=3000)
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		m_UpdateFrequency=iUpdateFrequency;
	}

	int GetUpdateFrequency()
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		return m_UpdateFrequency;
	}

	void SetOverflowLimiter(int iOverflowLimiter=0)
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		m_OverflowLimiter=iOverflowLimiter;
	}

	int GetOverflowLimiter()
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		return m_OverflowLimiter;
	}

	void SetFileExt(CString iFileExt="log")
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		if(!iFileExt.IsEmpty())
			m_FileExt="."+iFileExt;
		else
			m_FileExt=iFileExt;
	}

	CString GetFileExt()
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		return m_FileExt;
	}

	void SetHeaderCallback(bool (*ifpHeaderCallback)(CString &iLogHeader, const CString &iLogPath, const CTime &iHeaderTime)=NULL)
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		fpHeaderCallback=ifpHeaderCallback;
	}

	void SetOverflowCallback(bool (*ifpOverflowCallback)(const CFileException *fe, const HANDLE ihProcessThread, int &iOverflowLimiter, int &iUpdateFrequency, CString &iLogPath, const CStringList *iQueuedMessages, const CString &iFilePathTimeStampFormat, const CString &iMessageTimeStampFormat)=NULL)
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		fpOverflowCallback=ifpOverflowCallback;
	}

	void StartInternalThread()
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		if(m_hProcessThread)
			return;
		m_hProcessThread=::CreateThread(NULL, 0, ProcessThread, this, 0, &m_InternalThreadID);
	}

	void KillInternalThread()
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		if(m_hProcessThread)
			TerminateThread(m_hProcessThread,0);
		m_hProcessThread=NULL;
	}

	const HANDLE GetInternalThreadHandle()
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		return m_hProcessThread;
	}

	DWORD GetInternalThreadID()
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		return m_InternalThreadID;
	}

	void SetFilePathTimeStampFormat(CString iFilePathTimeStampFormat="[%Y-%m-%d]")
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		m_FilePathTimeStampFormat=iFilePathTimeStampFormat;
	}

	CString GetFilePathTimeStampFormat()
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		return m_FilePathTimeStampFormat;
	}

	void SetMessageTimeStampFormat(CString iMessageTimeStampFormat="[%d-%m-%Y (%H:%M:%S)] ")
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		m_MessageTimeStampFormat=iMessageTimeStampFormat;
	}

	CString GetMessageTimeStampFormat()
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		return m_MessageTimeStampFormat;
	}

	void AddLog(CString iLogPath, CString iLogMessage, bool iUseTimeStampForFilePath=true, bool iUseTimeStampForMessage=true)
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		QueueLogMessage(iLogPath, iLogMessage, iUseTimeStampForFilePath, iUseTimeStampForMessage);
	}

	bool AddLog(int iStringResourceLogPathID, CString iLogMessage, bool iUseTimeStampForFilePath=true, bool iUseTimeStampForMessage=true)
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		CString LogPath;
		if(!LogPath.LoadString(iStringResourceLogPathID))
			return false;
		QueueLogMessage(LogPath, iLogMessage, iUseTimeStampForFilePath, iUseTimeStampForMessage);
		return true;
	}

	bool AddLog(int iStringResourceLogPathID, int iStringResourceLogMessageID, bool iUseTimeStampForFilePath=true, bool iUseTimeStampForMessage=true)
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		CString LogPath, LogMessage;
		if(!LogPath.LoadString(iStringResourceLogPathID))
			return false;
		if(!LogMessage.LoadString(iStringResourceLogMessageID))
			return false;
		QueueLogMessage(LogPath, LogMessage, iUseTimeStampForFilePath, iUseTimeStampForMessage);
		return true;
	}

	bool AddLog(CString iLogPath, int iStringResourceLogMessageID, bool iUseTimeStampForFilePath=true, bool iUseTimeStampForMessage=true)
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		CString LogMessage;
		if(!LogMessage.LoadString(iStringResourceLogMessageID))
			return false;
		QueueLogMessage(iLogPath, LogMessage, iUseTimeStampForFilePath, iUseTimeStampForMessage);
		return true;
	}

	void AddTimeStampedLog(CString iLogPath, CString iLogMessage)
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		CTime TimeStamp=CTime::GetCurrentTime();
		QueueLogMessage(TimeStamp.Format(iLogPath), TimeStamp.Format(iLogMessage), false, false);
	}

	bool AddTimeStampedLog(int iStringResourceLogPathID, CString iLogMessage)
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		CString LogPath;
		if(!LogPath.LoadString(iStringResourceLogPathID))
			return false;
		CTime TimeStamp=CTime::GetCurrentTime();
		QueueLogMessage(TimeStamp.Format(LogPath), TimeStamp.Format(iLogMessage), false, false);
		return true;
	}

	bool AddTimeStampedLog(int iStringResourceLogPathID, int iStringResourceLogMessageID)
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		CString LogPath, LogMessage;
		if(!LogPath.LoadString(iStringResourceLogPathID))
			return false;
		if(!LogMessage.LoadString(iStringResourceLogMessageID))
			return false;
		CTime TimeStamp=CTime::GetCurrentTime();
		QueueLogMessage(TimeStamp.Format(LogPath), TimeStamp.Format(LogMessage), false, false);
		return true;
	}

	bool AddTimeStampedLog(CString iLogPath, int iStringResourceLogMessageID)
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		CString LogMessage;
		if(!LogMessage.LoadString(iStringResourceLogMessageID))
			return false;
		CTime TimeStamp=CTime::GetCurrentTime();
		QueueLogMessage(TimeStamp.Format(iLogPath), TimeStamp.Format(LogMessage), false, false);
		return true;
	}

	void Process()
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		if(!m_MLQueue.IsEmpty())
			Processor(false);
	}

	void Flash(bool iIgnoreAccess=true)
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		while(!m_MLQueue.IsEmpty())
		{
			Processor(iIgnoreAccess);
			Sleep(0);
		}
	}

	BOOL IsEmpty()
	{
		CSingleLock lock(m_pMLMutex, TRUE);
		return m_MLQueue.IsEmpty();
	}

	static BOOL MakeSurePathExists(CString &iPath, bool iFilenameIncluded=true, bool iIgnoreResult=false)
	{
		int Pos=0;
		while((Pos=iPath.Find('\\',Pos+1))!=-1) 
			CreateDirectory(iPath.Left(Pos),NULL);
		if(!iFilenameIncluded)
			CreateDirectory(iPath,NULL);
		if(!iIgnoreResult)
			return ((!iFilenameIncluded)?!_access(iPath,0):!_access(iPath.Left(iPath.ReverseFind('\\')),0));
		return TRUE;
	}
};

#endif //_MALE_LOGGER
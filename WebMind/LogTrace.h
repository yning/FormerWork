///////////////////////////////////////////////////////////////
// LogTrace.h: interface of a logger.
//
//
///////////////////////////////////////////////////////////////

#ifndef __LOGTRACE_H__
#define __LOGTRACE_H__

class CLogTrace
{
	// Construction/Destruction
public:
	CLogTrace();
	~CLogTrace();
	
	// Attributes
public:
	CString m_strAppName;
	
protected:
	BOOL m_bActive;
	CString m_strFileName;
	BOOL m_bTimeStamp;
	
	enum {TBUF_SIZE = 8192};
	char		m_tBuf[TBUF_SIZE];
	
	// Operations
public:
	void WriteLine(const char* fmt, ...);
	void Write(LPCTSTR szLine);
	
	void ResetFile();
	void OnStartup(BOOL bActive, BOOL bTimeStamp);
	void SetFileName(LPCTSTR szFileName);
	
	// Inlines
public:
	inline void SetActive(BOOL bSet)				{		m_bActive = bSet;		}
	inline CString GetFileName()					{		return m_strFileName;	}
};


#endif // __LOGTRACE_H__
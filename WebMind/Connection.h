///////////////////////////////////////////////////////////////
// Connection.h: interface for the Connection class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_CONNECTION_H__6ACAA1FA_9EEA_4E08_8648_5B8904FCC9ED__INCLUDED_)
#define AFX_CONNECTION_H__6ACAA1FA_9EEA_4E08_8648_5B8904FCC9ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxinet.h>
#include "HighTime.h"
#include "Url.h"
#include <stdlib.h>


class Connection  
{
public:
	Connection();
	virtual ~Connection();
	
	BOOL GetHeader(CString &m_url);
	int GetContentLength();
	CString GetContentType();
	CString GetContent();
	BOOL Retrieve(const char *pszURL, CString &m_newLocation);

	//add by ny
	static TCHAR toHex(BYTE val);
	static CString urlEncoding( CString &sIn );
	static void UnicodeToUTF_8(char* pOut,WCHAR* pText);
	static void Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer);
	static void GB2312ToUTF_8(CString& pOut,CString pText, int pLen);
    static CString UrlUTF8(CString str);
private:
	void ParseHeader(CHttpFile* pFile);
	
private:
	CString content;
	
public:
	CString contentType;
	CHighTime lastModified;
	int contentLength;
	
};

#endif // !defined(AFX_CONNECTION_H__6ACAA1FA_9EEA_4E08_8648_5B8904FCC9ED__INCLUDED_)

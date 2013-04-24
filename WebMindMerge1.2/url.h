///////////////////////////////////////////////////////////////
// Url.h: interface for the CUrl class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_URL_H__6F5CFE40_B6E9_44B3_82BD_E640F8917D17__INCLUDED_)
#define AFX_URL_H__6F5CFE40_B6E9_44B3_82BD_E640F8917D17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(lib, "wininet.lib")
#include "wininet.h"
#include "Afxinet.h"

#include "LilacString.h"
#include "LilacObject.h"
#include "HighTime.h"
#include "Connection.h"

class CUrl : public CLilacObject
{
public:
	CUrl();
	CUrl(const CUrl& pURL);
	CUrl(const CString &strURL);
	CUrl(const CString &m_currentURL, const CString &m_baseURL, const CString &relativeURL);
	CUrl(const CString &relativeURL, CUrl &parent);

	virtual ~CUrl();

public:
	CLilacObject *clone()
	{
		CUrl *pClone = new CUrl(m_url);

		pClone->setContent(m_content);
		pClone->setHeader(m_bHeader);
		pClone->setContentType(m_contentType);
		pClone->setContentLength(m_contentLength);
		pClone->setLastModified(m_lastModified);

		return pClone;
	}

    int key()
	{
		CLilacString pString(m_url);
		return pString.key();
	}

    int compare(CLilacObject *obj)
    {
        CUrl *pUrl = (CUrl *)obj;

		if(stricmp(m_url, pUrl->getURL()) == 0)
			return 0;
        else
	        return -1;
	}

	void dump();


public:
	void parse(const CString &strURL);
	void set(const CString &strURL);
	void clear();
	// add by ny

	//***********************************
public:
	LPCTSTR getURL() const				{	return m_url;			}

	INTERNET_SCHEME getScheme()	const	{	return m_nScheme;		}
	INTERNET_PORT getPort()	const		{	return m_nPort;			}
	LPCTSTR getSchemeName()	const		{	return m_strScheme;		}
	LPCTSTR getHostName() const			{	return m_strHostName;	}
	LPCTSTR getUserName() const			{	return m_strUserName;	}
	LPCTSTR getPassword() const 		{	return m_strPassword;	}
	LPCTSTR getURLPath() const			{	return m_strURLPath;	}
	LPCTSTR getExtraInfo() const		{	return m_strExtraInfo;	}

	LPCTSTR getPath() const				{	return m_strURLPath + m_strExtraInfo;	}

protected:
	void combine(const CString &parentURL, const CString &relativeURL);

protected:
	CString m_url;

	CString m_strScheme;
	INTERNET_SCHEME m_nScheme;
	CString m_strHostName;
	INTERNET_PORT m_nPort;
	CString m_strUserName;
	CString m_strPassword;
	CString m_strURLPath;
	CString m_strExtraInfo;



public:
	void	clearContent()									{	m_content.Empty();				}

	const char * getContent();
	void setContent(const CString sContent)					{	m_content = sContent;			}

	BOOL getHeader();
	void setHeader(BOOL bHeader)							{	m_bHeader = bHeader;			}

	char * getContentType();
	void setContentType(const CString &cType)				{	m_contentType = cType;			}

	int		getContentLength();
	void setContentLength(int iLength)						{	m_contentLength = iLength;		}

	void getLastModified(CHighTime &lastModified);
	void setLastModified(const CHighTime &lastModified)		{	m_lastModified = lastModified;	}

protected:
	CString m_content;

	BOOL m_bHeader;

	CString m_contentType;
	int m_contentLength;
	CHighTime m_lastModified;



};

#endif // !defined(AFX_URL_H__6F5CFE40_B6E9_44B3_82BD_E640F8917D17__INCLUDED_)

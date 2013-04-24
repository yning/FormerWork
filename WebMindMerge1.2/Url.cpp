///////////////////////////////////////////////////////////////
// Url.cpp: implementation of the CUrl class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Url.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUrl::CUrl()
{
	clear();
}

CUrl::CUrl(const CUrl& pURL)
{
	clear();

	m_url = pURL.getURL();

	m_strScheme = pURL.getSchemeName();
	m_nScheme = pURL.getScheme();
	m_strHostName = pURL.getHostName();
	m_nPort = pURL.getPort();
	m_strUserName = pURL.getUserName();
	m_strPassword = pURL.getPassword();
	m_strURLPath = pURL.getURLPath();
	m_strExtraInfo = pURL.getExtraInfo();
}

CUrl::CUrl(const CString &strURL)
{
	parse(strURL);
}

CUrl::CUrl(const CString &m_currentURL, const CString &m_baseURL, const CString &relativeURL)
{
	combine(m_baseURL.IsEmpty()?m_currentURL:m_baseURL, relativeURL);
}

CUrl::CUrl(const CString &relativeURL, CUrl &parent)
{
	combine(parent.getURL(), relativeURL);
}

// combine the relative URL according to its parent URL
void CUrl::combine(const CString &parentURL, const CString &relativeURL)
{
	char szUrl[_MAX_PATH];
	DWORD dwSize = _MAX_PATH;
	if(InternetCombineUrl ( parentURL, relativeURL, szUrl, &dwSize, ICU_DECODE | ICU_NO_ENCODE))
		parse(szUrl);
}

CUrl::~CUrl()
{

}


void CUrl::dump()
{
	TRACE("--------------------------------------------------------------------------\n");
	TRACE(" %s\n", m_url);
	TRACE("     scheme: %s\n", m_strScheme);
	TRACE("  host name: %s\n", m_strHostName);
	TRACE("       port: %d\n", m_nPort);
	TRACE("  user name: %s\n", m_strUserName);
	TRACE("   password: %s\n", m_strPassword);
	TRACE("   url path: %s\n", m_strURLPath);
	TRACE(" extra info: %s\n", m_strExtraInfo);
	
	TRACE("scheme type: ");
	switch(m_nScheme) {
		case INTERNET_SCHEME_PARTIAL: TRACE("partial.\n"); break;
		case INTERNET_SCHEME_UNKNOWN: TRACE("unknown.\n"); break;
		case INTERNET_SCHEME_DEFAULT: TRACE("default.\n"); break;
		case INTERNET_SCHEME_FTP: TRACE("FTP.\n"); break;
		case INTERNET_SCHEME_GOPHER: TRACE("GOPHER.\n"); break;
		case INTERNET_SCHEME_HTTP: TRACE("HTTP.\n"); break;
		case INTERNET_SCHEME_HTTPS: TRACE("HTTPS.\n"); break;
		case INTERNET_SCHEME_FILE: TRACE("FILE.\n"); break;
		case INTERNET_SCHEME_NEWS: TRACE("NEWS.\n"); break;
		case INTERNET_SCHEME_MAILTO: TRACE("MAILTO.\n"); break;
		default: TRACE("%d\n", m_nScheme);
	}

}

void CUrl::clear()
{
	m_url.Empty();

	m_strScheme = _T("unknown");
	m_nScheme = INTERNET_SCHEME_UNKNOWN;
	m_strHostName.Empty();
	m_nPort = 80;
	m_strUserName.Empty();
	m_strPassword.Empty();
	m_strURLPath.Empty();
	m_strExtraInfo.Empty();

	m_content.Empty();

	m_bHeader = FALSE;
	m_contentType.Empty();
	m_contentLength = 0;
	m_lastModified = CHighTime::GetPresentTime();
}

void CUrl::set(const CString &strURL)
{
	parse(strURL);
}

void CUrl::parse(const CString &strURL)
{
	// initialization
	clear();

	// canonicalize the url 
	char szUrl[_MAX_PATH];
	DWORD dwSize = _MAX_PATH;
    TCHAR outurl[256];
	wchar_t outurl2[256];
	if(!InternetCanonicalizeUrl (strURL, szUrl, &dwSize, ICU_DECODE | ICU_NO_ENCODE))
		return;
	/*
    CString url(strURL);

	USES_CONVERSION; 
    LPWSTR inurl=new wchar_t[url.GetLength()+1]; 
    wcscpy(inurl,T2W((LPCTSTR)url));
	CUrlEncodeUnicode::Encode(inurl,outurl,256);
	CUrlEncodeUnicode::Decode(outurl, outurl2, 256);  	
	m_url = outurl2;
*/
	// parse the url to get all the url components
	m_url = szUrl;
	URL_COMPONENTS uc;
	char Scheme[_MAX_PATH];
	char HostName[_MAX_PATH];
	char UserName[_MAX_PATH];
	char Password[_MAX_PATH];
	char UrlPath[_MAX_PATH];
	char ExtraInfo[_MAX_PATH];

	uc.dwStructSize = sizeof(uc);
	uc.lpszScheme = Scheme;
	uc.dwSchemeLength = _MAX_PATH;

	uc.lpszHostName = HostName;
	uc.dwHostNameLength = _MAX_PATH;

	uc.lpszUserName = UserName;
	uc.dwUserNameLength = _MAX_PATH;

	uc.lpszPassword = Password;
	uc.dwPasswordLength = _MAX_PATH;

	uc.lpszUrlPath = UrlPath;
	uc.dwUrlPathLength = _MAX_PATH;

	uc.lpszExtraInfo = ExtraInfo;
	uc.dwExtraInfoLength = _MAX_PATH;

	if(InternetCrackUrl(m_url, 0, ICU_ESCAPE, &uc))
	{
		m_strScheme = uc.lpszScheme;
		m_nScheme = uc.nScheme;
		m_strHostName = uc.lpszHostName;
		m_nPort = uc.nPort;
		m_strUserName = uc.lpszUserName;
		m_strPassword = uc.lpszPassword;
		m_strURLPath = uc.lpszUrlPath;
		m_strExtraInfo = uc.lpszExtraInfo;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
// Processing the content, including the content, and the header

// retrieve the header of this URL
BOOL CUrl::getHeader()
{
	Connection m_link;
	CString finalLocationURL(m_url);
	if(m_link.GetHeader(finalLocationURL))
	{
		if(!finalLocationURL.IsEmpty())
			parse(finalLocationURL);

		m_contentType = m_link.contentType;
		m_contentLength = m_link.contentLength;
		m_lastModified = m_link.lastModified;

		m_bHeader = TRUE;

		return TRUE;
	}

	return FALSE;
}

char * CUrl::getContentType()
{
	if(!m_bHeader)
		getHeader();

	return (char *)((LPCSTR)m_contentType);
}

void CUrl::getLastModified(CHighTime &lastModified)
{
	if(!m_bHeader)
		getHeader();

	lastModified = m_lastModified;
}

int CUrl::getContentLength()
{
	if(!m_bHeader)
		getHeader();

	return m_contentLength;
}

const char * CUrl::getContent()
{
	Connection m_link;
	CString m_newLocation;

	m_newLocation.Empty();
	if(m_link.Retrieve(m_url, m_newLocation))
	{
		if(!m_newLocation.IsEmpty())
			parse(m_newLocation);

		m_contentLength = m_link.GetContentLength();
//		m_contentType = m_link.GetContentType();
		m_content = m_link.GetContent();

		return (LPCSTR)m_content;
	}
	else
		return NULL;
}

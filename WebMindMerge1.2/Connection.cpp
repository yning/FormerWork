///////////////////////////////////////////////////////////////
// Connection.cpp: implementation of the Connection class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <afxwin.h>
#include <afxinet.h>
#include "Connection.h"

#include <iostream.h>
#include <stdlib.h>

#include <algorithm>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DWORD   dwAccessType = PRE_CONFIG_INTERNET_ACCESS;
DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;
const TCHAR szHeaders[] = _T("Accept: text/*\r\nUser-Agent: CSUofA\r\n");

Connection::Connection()
{
	content.Empty();
	contentType.Empty();
}

Connection::~Connection()
{
	content.Empty();
	contentType.Empty();
}

char* chomp(char* s) 
{ 
unsigned   len   =   strlen(s); 
char *p1   =   s; 
char   *p2   =   s+len-1; 
while   (*p1   ==   ' ')   p1++; 
while   (*p2   ==   '\n')   p2--; 
memcpy(s,   p1,   p2-p1+1); 
*(s+(p2-p1)+1)   =   0; 
return   s; 
} 

TCHAR Connection::toHex(BYTE val)
{
	if (val>9) return ('A')+val-10;
	return ('0')+val;
}

CString Connection::urlEncoding( CString &sIn )
{
     CString sOut;

     for( int ix = 0; ix < sIn.GetLength(); ix++ )
     {
         BYTE buf[4];
         memset( buf, 0, 4 );
         if( isalnum( (BYTE)sIn[ix] ) )
         {
             buf[0] = sIn[ix];
         }
		 else if('/'==sIn[ix])
         {
             buf[0] = sIn[ix];
         }
		  else if('?'==sIn[ix])
         {
             buf[0] = sIn[ix];
         }
		 else if('.'==sIn[ix])
         {
             buf[0] = sIn[ix];
         }
		 else if(':'==sIn[ix])
         {
             buf[0] = sIn[ix];
         }
		  else if('+'==sIn[ix])
         {
             buf[0] = sIn[ix];
         }
		  else if('-'==sIn[ix])
         {
             buf[0] = sIn[ix];
         }
		  else if('&'==sIn[ix])
         {
             buf[0] = sIn[ix];
         }
		   else if('='==sIn[ix])
         {
             buf[0] = sIn[ix];
         }
		   else if(','==sIn[ix])
         {
             buf[0] = sIn[ix];
         }
         else if ( isspace( (BYTE)sIn[ix] ) )
         {
             buf[0] = '+';
         }
         else
         {
             buf[0] = '%';
             buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
             buf[2] = toHex( (BYTE)sIn[ix] % 16);

         }
         sOut += (char *)buf;
     }
     return sOut;
}



void Connection::UnicodeToUTF_8(char* pOut,WCHAR* pText)
{
    // 注意 WCHAR高低字的顺序,低字节在前，高字节在后
    char* pchar = (char *)pText;
    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));
    return;
}

void Connection::Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer)
{
    ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
    return;
}
//GB2312 转为 UTF-8
void Connection::GB2312ToUTF_8(CString& pOut,CString pText, int pLen)
{
    char buf[4];
    memset(buf,0,4);
    pOut.Empty();
    int i = 0;
    while(i < pLen)
    {
        //如果是英文直接复制就可以
        if( pText[i] >= 0)
        {
            char asciistr[2]={0};
            asciistr[0] = (pText[i++]);
            pOut=pOut+asciistr;
        }
        else
        {
            WCHAR pbuffer;
            Gb2312ToUnicode(&pbuffer,(char *)pText[i]);//pText+i
            UnicodeToUTF_8(buf,&pbuffer);
            pOut=pOut+buf;
            i += 2;
        }
    }
    return;
}

CString Connection::UrlUTF8(CString str)
{
    CString tt;
    CString dd;
    GB2312ToUTF_8(tt,str,(int)strlen(str));
    size_t len=tt.GetLength();
    for (size_t i=0;i<len;i++)
    {
        if(isalnum((BYTE)tt.GetBuffer(i)))
        {
            char tempbuff[2]={0};
            sprintf(tempbuff,"%c",(BYTE)tt.GetBuffer(i));
            dd=dd+tempbuff;
        }
        else if (isspace((BYTE)tt.GetBuffer(i)))
        {
            dd=dd+"+";
        }
        else
        {
            char tempbuff[4];
            sprintf(tempbuff,"%%%X%X",((BYTE)tt.GetBuffer(i)) >>4,((BYTE)tt.GetBuffer(i)) %16);
            dd=dd+tempbuff;
        }
    }
    return dd;
}

BOOL Connection::Retrieve(const char *pszURL, CString &m_newLocation)
{
	//CString tempURL("http://www.baidu.com/s?wd=%D6%D0%CE%C4&rsv_bp=0&rsv_spt=3&inputT=3776");
    CInternetSession session(_T("Retriever - CS UofA"), 1, dwAccessType);
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
//	TCHAR outurl[256];
	//wchar_t outurl2[256];
	//if(!InternetCanonicalizeUrl (strURL, szUrl, &dwSize, ICU_DECODE | ICU_NO_ENCODE))
	//	return;
	//这里对传入的pszURL参数进行URLencode处理，只对其中的中文进行encode：
    CString url(pszURL); 
	//CString outUrl;
	url= urlEncoding(url);

	try
	{
    	// check to see if this is a reasonable URL
	    CString strServerName;
    	CString strObject;
    	INTERNET_PORT nPort;
	    DWORD dwServiceType;

	    if (!AfxParseURL(url, dwServiceType, strServerName, strObject, nPort) ||
		    dwServiceType != INTERNET_SERVICE_HTTP)
        {
            if (pFile != NULL)
			{
	            delete pFile;
				pFile = NULL;
			}

       	    if (pServer != NULL)
			{
        	    delete pServer;
				pServer = NULL;
			}

            session.Close();

		    return FALSE;
        }

	    pServer = session.GetHttpConnection(strServerName, nPort);

		pFile=(CHttpFile*)session.OpenURL(url);
	    //pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
		//	strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
    	//pFile->AddRequestHeaders(szHeaders);
	    // pFile->SendRequest();

    	DWORD dwRet;
	    pFile->QueryInfoStatusCode(dwRet);

    	// if access was denied, prompt the user for the password
	    if (dwRet == HTTP_STATUS_DENIED)
    	{
			if (pFile != NULL)
			{
				delete pFile;
				pFile = NULL;
			}

			if (pServer != NULL)
			{
				delete pServer;
				pServer = NULL;
			}

			session.Close();

			return FALSE;
			
//			DWORD dwPrompt;
//			dwPrompt = pFile->ErrorDlg(NULL, ERROR_INTERNET_INCORRECT_PASSWORD,
//				FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL);
//
//			// if the user cancelled the dialog, bail out
//			if (dwPrompt != ERROR_INTERNET_FORCE_RETRY)
//			{
//				if (pFile != NULL)
//					delete pFile;
//				if (pServer != NULL)
//					delete pServer;
//				session.Close();
//
//				return FALSE;
//			}
//
//			pFile->SendRequest();
//			pFile->QueryInfoStatusCode(dwRet);
    	}

//	    CString strNewLocation;
//    	pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);

	    // were we redirected?
    	// these response status codes come from WININET.H

    	if (dwRet == HTTP_STATUS_MOVED ||
		dwRet == HTTP_STATUS_REDIRECT ||
		dwRet == HTTP_STATUS_REDIRECT_METHOD)
    	{
	    	CString strNewLocation;
		    pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);

    		int nPlace = strNewLocation.Find(_T("Location: "));
	    	if (nPlace == -1)
            {
                if (pFile != NULL)
				{
	                delete pFile;
					pFile = NULL;
				}

       	        if (pServer != NULL)
				{
        	        delete pServer;
					pServer = NULL;
				}

                session.Close();

		    	return FALSE;
            }

    		strNewLocation = strNewLocation.Mid(nPlace + 10);
	    	nPlace = strNewLocation.Find('\n');
		    if (nPlace > 0)
			    strNewLocation = strNewLocation.Left(nPlace);

    		// close up the redirected site
	    	pFile->Close();
		    delete pFile;
			pFile = NULL;

    		pServer->Close();
	    	delete pServer;
			pServer = NULL;

    		// figure out what the new place was
		    if (!AfxParseURL(strNewLocation, dwServiceType, strServerName, strObject, nPort))
            {
				CUrl pURL(url);
				CUrl cURL(strNewLocation, pURL);
				strNewLocation = cURL.getURL();
				if (!AfxParseURL(strNewLocation, dwServiceType, strServerName, strObject, nPort))
				{
	                session.Close();
					return FALSE;
				}
            }

    		if (dwServiceType != INTERNET_SERVICE_HTTP)
            {
                session.Close();
	    		return FALSE;
            }
    
    		// try again at the new location
	    	pServer = session.GetHttpConnection(strServerName, nPort);
			pFile=(CHttpFile*)session.OpenURL(url);
		   // pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			//	strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
    	//	pFile->AddRequestHeaders(szHeaders);
	    //	pFile->SendRequest();

    		pFile->QueryInfoStatusCode(dwRet);
	    	if (dwRet != HTTP_STATUS_OK)
            {
                if (pFile != NULL)
				{
	                delete pFile;
					pFile = NULL;
				}

       	        if (pServer != NULL)
				{
        	        delete pServer;
					pServer = NULL;
				}

                session.Close();
		    	return FALSE;
            }

			if(!strNewLocation.IsEmpty())
				m_newLocation = strNewLocation;
    	}

    	if (dwRet != HTTP_STATUS_OK)
        {
            if (pFile != NULL)
			{
	            delete pFile;
				pFile = NULL;
			}

       	    if (pServer != NULL)
			{
        	    delete pServer;
				pServer = NULL;
			}

            session.Close();
	    	return FALSE;
        }

		ParseHeader(pFile);

    	content.Empty();

    	TCHAR sz[1024];
//	int nRead;

    	sz[0] = '\0';

	    while (pFile->ReadString(sz, 1023))
    	{
		//	WCHAR * temp=new WCHAR[sizeof(sz)];
		//	UTF_8ToUnicode(temp,sz);
			char* temp=chomp(sz);			
	    	contentLength += sizeof(temp);
		    content +=temp;
			int cd=content.GetLength();
			TCHAR aaa=content.GetAt(cd-1);
    		sz[0] = '\0';
    	}
//	content += '\0';
        //content = Convert(content,CP_ACP,CP_UTF8);
    	pFile->Close();
	    pServer->Close();
    }
	catch (CInternetException* )
	{
		if (pFile != NULL)
		{
			delete pFile;
			pFile = NULL;
		}

		if (pServer != NULL)
		{
			delete pServer;
			pServer = NULL;
		}

        session.Close();

		content.Empty();

        return FALSE;
    }

    if (pFile != NULL)
	{
	    delete pFile;
		pFile = NULL;
	}

   	if (pServer != NULL)
	{
    	delete pServer;
		pServer = NULL;
	}

    session.Close();

	return TRUE;
}

CString Connection::GetContent()
{
	return content;
}

CString Connection::GetContentType()
{
	return contentType;
}

int Connection::GetContentLength()
{
	return contentLength;
}

BOOL Connection::GetHeader(CString &m_url)
{
    CInternetSession session(_T("Retriever - CS UofA"), 1, dwAccessType);
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
   	CString strNewLocation;

	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 3*60*1000);
	strNewLocation.Empty();

	try
	{
    	// check to see if this is a reasonable URL
	    CString strServerName;
    	CString strObject;
    	INTERNET_PORT nPort;
	    DWORD dwServiceType;

	    if (!AfxParseURL((LPCTSTR)m_url, dwServiceType, strServerName, strObject, nPort) )
//		    || dwServiceType != INTERNET_SERVICE_HTTP)
        {
            if (pFile != NULL)
	            delete pFile;
       	    if (pServer != NULL)
        	    delete pServer;
            session.Close();

		    return FALSE;
        }

	    pServer = session.GetHttpConnection(strServerName, nPort);

	    pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_HEAD,
			strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
	    pFile->SendRequest();

    	DWORD dwRet;
	    pFile->QueryInfoStatusCode(dwRet);

    	// if access was denied, prompt the user for the password
	    if (dwRet == HTTP_STATUS_DENIED)
    	{
			if (pFile != NULL)
				delete pFile;
			if (pServer != NULL)
				delete pServer;

			session.Close();

			return FALSE;
		}

	    // were we redirected?
    	// these response status codes come from WININET.H

    	if (dwRet == HTTP_STATUS_MOVED ||
		dwRet == HTTP_STATUS_REDIRECT ||
		dwRet == HTTP_STATUS_REDIRECT_METHOD)
    	{
		    pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);

    		int nPlace = strNewLocation.Find(_T("Location: "));
	    	if (nPlace == -1)
            {
                if (pFile != NULL)
	                delete pFile;
       	        if (pServer != NULL)
        	        delete pServer;
                session.Close();

		    	return FALSE;
            }

    		strNewLocation = strNewLocation.Mid(nPlace + 10);
	    	nPlace = strNewLocation.Find('\n');
		    if (nPlace > 0)
			    strNewLocation = strNewLocation.Left(nPlace);

    		// close up the redirected site
	    	pFile->Close();
		    delete pFile;
    		pServer->Close();
	    	delete pServer;

    		// figure out what the old place was
		    if (!AfxParseURL(strNewLocation, dwServiceType, strServerName, strObject, nPort))
            {
                session.Close();
    			return FALSE;
            }

    		if (dwServiceType != INTERNET_SERVICE_HTTP)
            {
                session.Close();
	    		return FALSE;
            }
    
    		// try again at the new location
	    	pServer = session.GetHttpConnection(strServerName, nPort);
		    pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_HEAD,
				strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
	    	pFile->SendRequest();

    		pFile->QueryInfoStatusCode(dwRet);
	    	if (dwRet != HTTP_STATUS_OK)
            {
                if (pFile != NULL)
	                delete pFile;
       	        if (pServer != NULL)
        	        delete pServer;
                session.Close();
		    	return FALSE;
            }
    	}

    	if (dwRet != HTTP_STATUS_OK)
        {
            if (pFile != NULL)
	            delete pFile;
       	    if (pServer != NULL)
        	    delete pServer;
            session.Close();
	    	return FALSE;
        }

		m_url.Empty();
		if(!strNewLocation.IsEmpty())
			m_url = strNewLocation;

		ParseHeader(pFile);

    	pFile->Close();
	    pServer->Close();
    }
	catch (CInternetException* )
	{
		if (pFile != NULL)
			delete pFile;
		if (pServer != NULL)
			delete pServer;
        session.Close();

		content.Empty();

        return FALSE;
    }

    if (pFile != NULL)
	    delete pFile;
   	if (pServer != NULL)
    	delete pServer;
    session.Close();

	return TRUE;
}



void Connection::ParseHeader(CHttpFile* pFile)
{
	if(pFile == NULL)
		return;

   	contentType.Empty();
   	pFile->QueryInfo(HTTP_QUERY_CONTENT_TYPE, contentType);
   	int tPlace = contentType.Find(_T(";"));
   	if (tPlace != -1)
    	contentType = contentType.Left(tPlace);

	SYSTEMTIME pSystemTime;
	if( (tPlace = pFile->QueryInfo(HTTP_QUERY_LAST_MODIFIED, &pSystemTime)) == 0)
		lastModified = CHighTime::GetPresentTime();
	else
	{
		FILETIME lpFileTime, LocalFileTime;
		SystemTimeToFileTime(&pSystemTime, &lpFileTime);
		FileTimeToLocalFileTime(&lpFileTime, &LocalFileTime);
		lastModified = LocalFileTime;
	}

    contentLength = 0;
	int size = sizeof(int);
   	pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &contentLength, (unsigned long *)&size);
}


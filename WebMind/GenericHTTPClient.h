/*
 * $ Generic HTTP Client
 * ----------------------------------------------------------------------------------------------------------------
 *
 * name :          GenericHTTPClient
 *
 * version tag :     0.1.0
 *
 * description :    HTTP Client using WININET
 *
 * author :          Heo Yongsun ( gooshin@opentown.net )
 *
 * This code distributed under BSD LICENSE STYLE.
 */

#ifndef __GENERIC_HTTP_CLIENT
#define __GENERIC_HTTP_CLIENT

#include <afxwin.h>
#include <tchar.h>
#include <wininet.h>


// STL without warnings
// http://www.codeproject.com/vcpp/stl/stl_without_warnings.asp#xx623299xx
//
// by By Oskar Wieland 
//
//

// use stl
#pragma warning(push)

#include <yvals.h>              // warning numbers get enabled in yvals.h 

#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4245)  // conversion from 'type1' to 'type2', 
                                // signed/unsigned mismatch
#pragma warning(disable: 4663)  // C++ language change: to explicitly specialize 
                                // class template 'vector'
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4786)  // identifier was truncated to 'number' characters
                                // in the debug information
#include <vector>

#pragma warning(pop)

// PRE-DEFINED CONSTANTS
#define __DEFAULT_AGENT_NAME	"MERONG(0.9/;p)"

// PRE-DEFINED BUFFER SIZE
#define	__SIZE_HTTP_ARGUMENT_NAME	256
#define __SIZE_HTTP_ARGUMENT_VALUE	1024

#define __HTTP_VERB_GET	"GET"
#define __HTTP_VERB_POST "POST"
#define __HTTP_ACCEPT_TYPE "*/*"
#define __HTTP_ACCEPT "Accept: */*\r\n"
#define __SIZE_HTTP_BUFFER	100000
#define __SIZE_HTTP_RESPONSE_BUFFER	100000
#define __SIZE_HTTP_HEAD_LINE	2048

#define __SIZE_BUFFER	1024
#define __SIZE_SMALL_BUFFER	256

class GenericHTTPClient {
public:					
	typedef struct __GENERIC_HTTP_ARGUMENT{							// ARGUMENTS STRUCTURE
		TCHAR	szName[__SIZE_HTTP_ARGUMENT_NAME];
		TCHAR	szValue[__SIZE_HTTP_ARGUMENT_VALUE];
		DWORD	dwType;
		operator==(const __GENERIC_HTTP_ARGUMENT &argV){
			return !_tcscmp(szName, argV.szName) && !_tcscmp(szValue, argV.szValue);
		}
	} GenericHTTPArgument;

	enum RequestMethod{															// REQUEST METHOD
		RequestUnknown=0,
		RequestGetMethod=1,
		RequestPostMethod=2,
		RequestPostMethodMultiPartsFormData=3
	};

	enum TypePostArgument{													// POST TYPE 
		TypeUnknown=0,
		TypeNormal=1,
		TypeBinary=2
	};

	// CONSTRUCTOR & DESTRUCTOR
	GenericHTTPClient();
	virtual ~GenericHTTPClient();

	static GenericHTTPClient::RequestMethod GetMethod(int nMethod);
	static GenericHTTPClient::TypePostArgument GetPostArgumentType(int nType);

	// Connection handler	
	BOOL Connect(LPCTSTR szAddress, LPCTSTR szAgent = __DEFAULT_AGENT_NAME, unsigned short nPort = INTERNET_DEFAULT_HTTP_PORT, LPCTSTR szUserAccount = NULL, LPCTSTR szPassword = NULL);
	BOOL Close();
	VOID InitilizePostArguments();

	// HTTP Arguments handler	
	VOID AddPostArguments(LPCTSTR szName, DWORD nValue);
	VOID AddPostArguments(LPCTSTR szName, LPCTSTR szValue, BOOL bBinary = FALSE);

	// HTTP Method handler 
	BOOL Request(LPCTSTR szURL, int nMethod = GenericHTTPClient::RequestGetMethod, LPCTSTR szAgent = __DEFAULT_AGENT_NAME);
	BOOL RequestOfURI(LPCTSTR szURI, int nMethod = GenericHTTPClient::RequestGetMethod);

//	BOOL Response(PBYTE pHeaderBuffer, DWORD dwHeaderBufferLength, PBYTE pBuffer, DWORD dwBufferLength, DWORD &dwResultSize);	
	BOOL Response();	

	LPCTSTR QueryHTTPResponse();
	LPCTSTR QueryHTTPResponseHeader();	

	DWORD QueryResponseSize();	
	TCHAR *QueryResponse();

	// General Handler
	DWORD GetLastError();
	LPCTSTR GetContentType(LPCTSTR szName);
	VOID ParseURL(LPCTSTR szURL, LPTSTR szProtocol, LPTSTR szAddress, DWORD &dwPort, LPTSTR szURI);

	
protected:				
	std::vector<GenericHTTPArgument> _vArguments;				// POST ARGUMENTS VECTOR

	TCHAR		*_szHTTPResponseHTML;		// RECEIVE HTTP BODY
	TCHAR		_szHTTPResponseHeader[__SIZE_HTTP_BUFFER];	// RECEIVE HTTP HEADR

//	TCHAR		_szHTTPResponseHTML[__SIZE_HTTP_BUFFER];		// RECEIVE HTTP BODY
//	TCHAR		_szHTTPResponseHeader[__SIZE_HTTP_BUFFER];	// RECEIVE HTTP HEADR

	HINTERNET _hHTTPOpen;				// internet open handle
	HINTERNET _hHTTPConnection;		// internet connection hadle
	HINTERNET _hHTTPRequest;		// internet request hadle

	DWORD		_dwResponseSize;	// http response size

	DWORD		_dwError;					// LAST ERROR CODE
	LPCTSTR		_szHost;					 //	 HOST NAME
	DWORD		_dwPort;					//  PORT

	// HTTP Method handler
	DWORD ResponseOfBytes(PBYTE pBuffer, DWORD dwSize);
	DWORD GetPostArguments(LPTSTR szArguments, DWORD dwLength);
	BOOL RequestPost(LPCTSTR szURI);
	BOOL RequestPostMultiPartsFormData(LPCTSTR szURI);
	BOOL RequestGet(LPCTSTR szURI);
	DWORD AllocMultiPartsFormData(PBYTE &pInBuffer, LPCTSTR szBoundary = "--MULTI-PARTS-FORM-DATA-BOUNDARY-");
	VOID FreeMultiPartsFormData(PBYTE &pBuffer);
	DWORD GetMultiPartsFormDataLength();
};

#endif	// #ifndef __GENERIC_HTTP_CLIENT

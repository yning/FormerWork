/////////////////////////////////////////////////////////////////////////////
/* 
DESCRIPTION
-----------

	Limiting an application to a single Instance - the MFC way
	
	Another approach for limiting application instance    
	http://www.codeproject.com/threads/singleinstancemfc.asp


Usage
-----

Derive the application class from CWinAppEx and just check InitInstance() 
function's ( with some GUID in parameter) return value and exit instance 
in case it is FALSE. Also add EnableShellOpen and RegisterShellFileTypes 
for SDI and MDI applications. That is all. ( The previous instance will 
popup the main window itself if you won't override OnAnotherInstance virtual 
function to do some specific actions when another instance is started ). 
Note: if lpszUID is not specified in the InitInstance(), the instance will 
not be limited. 


//...		
BOOL CSomeApp::InitInstance( void )
{
    // CSomeApp is derived from CWinAppEx
    
    if( CWinAppEx::InitInstance( _T( "{6BDD1FC6-...}" ) ) )
        return FALSE;
    
    AfxEnableControlContainer();
    
    //...
    
    // Enable DDE Execute open
    EnableShellOpen();
    RegisterShellFileTypes( TRUE );
    
    //...
    
    return TRUE;
}
//...

*/
/////////////////////////////////////////////////////////////////////////////




// Copyright(c) Armen Hakobyan 2002.
// http://www.codeproject.com/script/profile/whos_who.asp?vt=arts&id=25653
// mailto:armenh@web.am

#ifndef __WINAPPSINGLE_H__
#define __WINAPPSINGLE_H__
#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

// Auxiliary AFX functions
/*
#if (_MFC_VER < 0x0700) // 0x0421, 0x0600, 0x0700
	#include <..\src\afximpl.h>
#else
	#include <..\src\mfc\afximpl.h>	
#endif
*/

// CWinAppSingle

class CWinAppSingle : public CWinApp
{
	DECLARE_DYNCREATE( CWinAppSingle )

protected:
	CWinAppSingle( LPCTSTR lpszAppName = NULL );
	virtual ~CWinAppSingle( void );
	
public:	
	BOOL PostInstanceMessage( WPARAM wParam, LPARAM lParam );		
	BOOL EnableTokenPrivilege( LPCTSTR lpszSystemName, BOOL bEnable = TRUE );
	
public:
	AFX_INLINE void RegisterShellFileTypes( BOOL bCompat = FALSE );
	AFX_INLINE void UnregisterShellFileTypes( void );

public:
	virtual BOOL InitInstance( LPCTSTR lpszUID = NULL );
	virtual BOOL PreTranslateMessage( LPMSG pMsg );
	virtual BOOL OnAnotherInstanceMessage( LPMSG pMsg) ;

protected:
	BOOL FindAnotherInstance( LPCTSTR lpszUID );
	void RegisterShellFileTypesEx( BOOL bCompat = FALSE, BOOL bRegister = TRUE );
		
protected:
	DECLARE_MESSAGE_MAP()

protected:	
	UINT	m_uMsgCheckInst;
	
};

AFX_INLINE CWinAppSingle* GetApp( void )
	{ return STATIC_DOWNCAST( CWinAppSingle, AfxGetApp() ); }

AFX_INLINE void CWinAppSingle::RegisterShellFileTypes( BOOL bCompat /*FALSE*/ )
	{ RegisterShellFileTypesEx( bCompat, TRUE ); }

AFX_INLINE void CWinAppSingle::UnregisterShellFileTypes( void) 
	{ RegisterShellFileTypesEx( FALSE, FALSE ); }

#endif // __WINAPP_H__
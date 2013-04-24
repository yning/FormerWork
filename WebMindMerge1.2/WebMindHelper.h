// WebMindHelper.h: interface for the CWebMindHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WebMindHELPER_H__74460E23_1598_11D4_9BCE_0000E85300AE__INCLUDED_)
#define AFX_WebMindHELPER_H__74460E23_1598_11D4_9BCE_0000E85300AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WebMindView.h"

CWebMindView* GetCurrentView(int nFlag = 0, BOOL bInNewWindow = FALSE);
CWebMindView * CreateNewView();

#endif // !defined(AFX_WebMindHELPER_H__74460E23_1598_11D4_9BCE_0000E85300AE__INCLUDED_)

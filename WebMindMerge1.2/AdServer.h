///////////////////////////////////////////////////////////////
// AdServer.h: interface for the CAdServer class.
//
// This software was written by Tingshao Zhu.
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_ADSERVER_H__C0BD7794_66A0_48FB_B50E_51C9F9E9E484__INCLUDED_)
#define AFX_ADSERVER_H__C0BD7794_66A0_48FB_B50E_51C9F9E9E484__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NUM_AD_SERVERS 66


class CAdServer  
{
public:
	CAdServer();
	virtual ~CAdServer();

public:
	bool isAdServer(const char *dns);
};

#endif // !defined(AFX_ADSERVER_H__C0BD7794_66A0_48FB_B50E_51C9F9E9E484__INCLUDED_)

///////////////////////////////////////////////////////////////
// SearchPage.h: interface for the CSearchPage class.
//
// This software was written by Tingshao Zhu.
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHPAGE_H__927C1176_DEC1_48D8_B48F_A8B8C06836B8__INCLUDED_)
#define AFX_SEARCHPAGE_H__927C1176_DEC1_48D8_B48F_A8B8C06836B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LilacObject.h"

class CSearchPage : public CLilacObject
{
public:
    CSearchPage();
    ~CSearchPage();

	int getPageno();

    CLilacObject *clone();
    int compare(CLilacObject *obj);
    void dump();

public:

    CString m_url;					// url of the search page
	CString m_keyword;				// the search keywords
    CStringArray m_followURL;		// the URLs that follow this search page

	int m_queryCnt;
	int m_pageno;
};

#endif // !defined(AFX_SEARCHPAGE_H__927C1176_DEC1_48D8_B48F_A8B8C06836B8__INCLUDED_)

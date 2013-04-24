///////////////////////////////////////////////////////////////
// StopWord.h: interface for the CStopWord class.
//
// This software was written by Tingshao Zhu.
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_STOPWORD_H__E4F924FD_A124_4D8B_9AC0_326495795DCA__INCLUDED_)
#define AFX_STOPWORD_H__E4F924FD_A124_4D8B_9AC0_326495795DCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NUM_STOP_WORDS 1752

class CStopWord  
{
public:
	CStopWord();
	virtual ~CStopWord();
	
public:
	bool isStopWord(const char* word);
	
};

#endif // !defined(AFX_STOPWORD_H__E4F924FD_A124_4D8B_9AC0_326495795DCA__INCLUDED_)

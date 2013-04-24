///////////////////////////////////////////////////////////////
// PuncMark.h: interface for the CPuncMark class.
//
// This software was written by Tingshao Zhu.
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_PUNCMARK_H__11CB0450_5262_43D4_86D9_6B24984CF460__INCLUDED_)
#define AFX_PUNCMARK_H__11CB0450_5262_43D4_86D9_6B24984CF460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
    char punctuation_marks[] = {',', '.', '?', '/',
                                ';', ':', '\"', '|',
                                '\'' ,'\\', '{', '}',
                                '[', ']', '~', '`',
                                '!', '@', '#', '$',
                                '%', '^', '&', '*',
                                '(', ')', '_', '+',
                                '=', '-', '0', '1',
                                '2', '3', '4', '5',
                                '6', '7', '8', '9',
                                '<', '>', '\r', '\n'
                                };
*/


#define NUM_PUNC_MARKS 44


class CPuncMark  
{
public:
	CPuncMark();
	virtual ~CPuncMark();

public:
	void RemovePuncMark(char *str);

};

#endif // !defined(AFX_PUNCMARK_H__11CB0450_5262_43D4_86D9_6B24984CF460__INCLUDED_)

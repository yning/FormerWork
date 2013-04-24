/****************************************************************************
 *
 *Copyright (c) 2004-2006
 *  Software Research Lab.
 *  Institute of Computing Tech.
 *  Chinese Academy of Sciences
 *  All rights reserved.
 *
 * This file is the confidential and proprietary property of 
 * Institute of Computing Tech. and the posession or use of this file requires 
 * a written license from the author.
 * Filename:  Segment.h
 * Abstract:       
 *			  interface for the CSegment class.
 * Author:   王小飞 
 *          (wangxiaofei@tsinghua.org.cn)
 * Date:     2004-12-5
 *
 * Notes:	
 *                
 * 
 ****************************************************************************/

#pragma once
#include "Dictionary.h"
#include "Dict.h"
#include "utility.h"
#include <fstream>
#include <fcntl.h>
#include <TIME.H>

#if !defined(AFX_SEGMENT_H__238276E8_BB45_4DC7_AF64_26EDA5602BE6__INCLUDED_)
#define AFX_SEGMENT_H__238276E8_BB45_4DC7_AF64_26EDA5602BE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  BUFFERSIZE  5000
#define FILTER_LEN	50

class DICT_API CSegment  
{
public:
	CSegment();
	virtual ~CSegment();
	bool Init(CDictionary *dict);
	bool LoadFile(const char *pszFileName); //将待分词文件读入内存
	bool LoadContent(const char *fileContent);
	//分词函数，返回切分好的字符串和字符串长度
	char *TextSegment(int &iOutSize);	
	bool Free();
	void MMSegment(int &nIter);
public:
	char *m_lpBaseAddress;	  //文件基址
	long m_lLen;			  //文件长度
//	bool m_IfIn;
	char *m_cSegTxt;

private:
	
	CDictionary *m_pDict;		 //词典对象
	int m_nDictArraySize;		//词典数组长度
};

#endif // !defined(AFX_SEGMENT_H__238276E8_BB45_4DC7_AF64_26EDA5602BE6__INCLUDED_)




///////////////////////////////////////////////////////////////
// Utility.h: interface for the Utility class.
//
// This software was written by Tingshao Zhu.
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_UTILITY_H__32F93D8E_8F45_4D8F_B5A6_571E7404BAA9__INCLUDED_)
#define AFX_UTILITY_H__32F93D8E_8F45_4D8F_B5A6_571E7404BAA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "LogTrace.h"
extern CLogTrace m_LogTrace;



////////////////////////////////////////////////////////////////////////////////
/// Some definitions for global usage

// number of words we get from the observed sequence which will be
// used as search query
#define NUM_QUERY_WORD 4


// Number of attributes
#define NUM_ATTRIBUTE 35
enum AttrType {
	Continuous  = 0,
        Nominal    = 1
};


// number of top relevant words
#define NUM_TOP_RELEVANT 15

// minimum number of positive samples for training a personalized model
#define MIN_POSITIVE_NUM 100

// Number of effective pages that each session should include
// effective : not backward or forward action on the page
#define NUM_EFFECTIVE_PAGE 5

struct SortItem
{
	CString m_label;
	double m_value;
};

/*
//functions defined by kosko
CString LoadFile(CString filename);
CTime ConvertStringToCTime(CString time);
CString ConvertCtimeToCString(CTime);
*/
class Utility  
{
public:
	Utility();
	virtual ~Utility();
	
	static void CalAveVar(CArray<double, double&> &weight, double& average, double& variance);
	static int genRand(int length);
	static void trim(char *buffer);
	
	static void bubbleSort(CStringArray &m_normalWords, CArray<double, double&> &m_icProbs, int Fp, int Lp);
	static void quickSort(CStringArray &m_normalWords, CArray<double, double&> &m_icProbs, int Fp, int Lp);
	
	static void GetSubDir(LPCTSTR lpszName, CStringArray &subDirs);
	static void getFileList(const CString &strPath, const CString &strExtension, CStringArray &m_fileList, bool bFullPath = true);
	
	static bool isLILACLogFile(const CString &m_file);
	static void getLILACLog(const CString &strPath, CStringArray &m_logList);
	
	static void parseSequence(const CString &strSeq, const CString &strSplitter, CStringArray &strList);
	static int FindPattern(const CString &strPattern, const CStringArray &arQuery);

	//functions defined by kosko
	static CString LoadFile(CString filename);
	static CTime ConvertCStringToCTime(CString time);
	static CString ConvertCtimeToCString(CTime);
	static CTime ConvertCStringToCTime_Date(CString);
	static CString ConvertCTime_DateToCString(CTime);
	static CTime ConvertCStringToCTime_Hour(CString);
	static CString ConvertCTime_HourToCString(CTime);
	static int ConvertCStringToInt(CString);
	static CString ConvertIntToCString(int);
	static CString ConvertDoubleToCString(double);
	static double ConvertCStringToDouble(CString);
//	static vector<string> StringSpilt(string content,char tokenizer);
	static bool BinarySearch(char* strs[],int,char *);
	static CString ConvertUrlToDomain(CString url);
	static CString GetCurrentPath();
	static CString GetCurrentPath_DoubleSlash();
	static CString Utf8ToStringT(LPSTR str);
	static CString UrlDecode_gb2312(LPCTSTR url);
	static CString UrlDecode_utf8(LPCTSTR url);
	static CString GetKeyWordsFromUrl(LPCTSTR url);
	static int FindLetterFromString(const char* str,char ch);
};

#endif // !defined(AFX_UTILITY_H__32F93D8E_8F45_4D8F_B5A6_571E7404BAA9__INCLUDED_)

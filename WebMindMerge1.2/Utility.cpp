///////////////////////////////////////////////////////////////
// Utility.cpp: implementation of the Utility class.
//
// This software was written by Tingshao Zhu.
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Utility.h"
#include "regexpr2.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace regex;

CLogTrace m_LogTrace;

int compare( const void *arg1, const void *arg2 );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Utility::Utility()
{

}

Utility::~Utility()
{

}

void Utility::CalAveVar(CArray<double, double&> &weight, double& average, double& variance)
{
	average = 0.0;
	variance = 0.0;
	if(weight.GetSize() <= 0)
		return;

    double sum;
    int i;

    sum = 0.0;
    for(i = 0; i < weight.GetSize(); i++)
        sum += weight[i];

    average = sum/(double)weight.GetSize();

    sum = 0.0;
    for(i = 0; i < weight.GetSize(); i++)
        sum += (weight[i]-average)*(weight[i]-average)/(double)weight.GetSize();

    variance = sqrt(sum);

    return;
}

// generate a random number from 0 - length-1
int Utility::genRand(int length)
{
	int pos;

	pos = (int)((double)length *(double)rand()/(double)RAND_MAX);
	if(pos >= length)
		pos = length - 1;

	return pos;
}


void Utility::trim(char *buffer)
{
	if(buffer == NULL)
		return;

	for(int i = strlen(buffer) - 1; i >= 0; i--)
	{
		if(buffer[i] == '\r' || buffer[i] == '\n' || buffer[i] == ' ')
			buffer[i] = '\0';
	}
}



void Utility::bubbleSort(CStringArray &m_normalWords, CArray<double, double&> &m_icProbs, int Fp, int Lp)
{
    double temp;
	CString strBuffer;

	for(int i = 0; i < m_normalWords.GetSize()-1; i++)
	{
		for(int j = 0; j < m_normalWords.GetSize(); j++)
		{
			if(m_icProbs[i] < m_icProbs[j])
			{
				temp = m_icProbs[j];
				m_icProbs.SetAt(j, m_icProbs[i]);
				m_icProbs.SetAt(i, temp);

				strBuffer = m_normalWords[j];
				m_normalWords.SetAt(j, m_normalWords[i]);
				m_normalWords.SetAt(i, strBuffer);
			}
		}
	}
}


void Utility::quickSort(CStringArray &m_normalWords, CArray<double, double&> &m_icProbs, int Fp, int Lp)
{
	struct SortItem *m_data;
	int i, numItems;

	numItems = m_normalWords.GetSize();

	m_data = (struct SortItem *) new struct SortItem[numItems];

	for(i = 0; i < numItems; i++)
	{
		m_data[i].m_label = m_normalWords[i];
		m_data[i].m_value = m_icProbs[i];
	}

	qsort( (void *)m_data, (size_t)numItems, sizeof( struct SortItem ), compare );

	m_normalWords.RemoveAll();
	m_icProbs.RemoveAll();

	for(i = 0; i < numItems; i++)
	{
		m_normalWords.Add(m_data[i].m_label);
		m_icProbs.Add(m_data[i].m_value);
	}

	delete [] m_data;

}

int compare( const void *arg1, const void *arg2 )
{
	struct SortItem *pArg1 = (struct SortItem *)arg1;
	struct SortItem *pArg2 = (struct SortItem *)arg2;

	if(pArg1->m_value > pArg2->m_value)
		return -1;
	else if(pArg1->m_value == pArg2->m_value)
		return 0;
	else
		return 1;
}




/*
void Utility::quickSort(CStringArray &m_normalWords, CArray<double, double&> &m_icProbs, int Fp, int Lp)
{
    int i, Lower, Middle;
    double Thresh, temp;
	CString strBuffer;

    if ( Fp < Lp )
    {
        Thresh = m_icProbs[Lp];

    	//  Isolate all items with values <= threshold  
    	Middle = Fp;

        for ( i = Fp ; i < Lp ; i++ )
        { 
            if ( m_icProbs[i] > Thresh )
            { 
                if ( i != Middle )
                {
                    temp = m_icProbs[Middle];
                    m_icProbs.SetAt(Middle, m_icProbs[i]);
                    m_icProbs.SetAt(i, temp);

					strBuffer = m_normalWords[Middle];
					m_normalWords.SetAt(Middle, m_normalWords[i]);
					m_normalWords.SetAt(i, strBuffer);

                }
                Middle++; 
            } 
        } 

        //  Extract all values equal to the threshold  
        Lower = Middle - 1;
        for ( i = Lower ; i >= Fp ; i-- )
	    {
	        if ( m_icProbs[i] == Thresh )
	        { 
		        if ( i != Lower )
                {
                    temp = m_icProbs[Lower];
                    m_icProbs.SetAt(Lower, m_icProbs[i]);
                    m_icProbs.SetAt(i, temp);

					strBuffer = m_normalWords[Lower];
					m_normalWords.SetAt(Lower, m_normalWords[i]);
					m_normalWords.SetAt(i, strBuffer);
                }

                Lower--;
            } 
        } 

        //  Sort the lower values  
        quickSort(m_normalWords, m_icProbs, Fp, Lower);

        //  Position the middle element  
        if ( Middle != Lp )
        {
			temp = m_icProbs[Middle];
			m_icProbs.SetAt(Middle, m_icProbs[Lp]);
			m_icProbs.SetAt(Lp, temp);

			strBuffer = m_normalWords[Middle];
			m_normalWords.SetAt(Middle, m_normalWords[Lp]);
			m_normalWords.SetAt(Lp, strBuffer);
        }

	    //  Sort the higher values  
    	quickSort(m_normalWords, m_icProbs, Middle+1, Lp);
    }

}

*/

// get all the sub directories under the folder (lpszName)
// only the sub-directories' name, not with the full path
void Utility::GetSubDir(LPCTSTR lpszName, CStringArray &subDirs)
{
	CString strPattern;
	WIN32_FIND_DATA info;
    HANDLE hp;
    char *cp;
	
	subDirs.RemoveAll();
	strPattern.Format("%s\\*.*",lpszName);

    hp = FindFirstFile(strPattern, &info);
    if(!hp || hp==INVALID_HANDLE_VALUE)
        return;

    do
    {
		cp = info.cFileName;
        if(cp[1]==0 && *cp=='.')
            continue;
        else if(cp[2]==0 && *cp=='.' && cp[1]=='.')
            continue;

		if(info.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			subDirs.Add(info.cFileName);

    } while(FindNextFile(hp,&info));

	FindClose(hp);

	return;
}


// get the list of all files under the folder (strPath) with file extension (strExtension)
void Utility::getFileList(const CString &strPath, const CString &strExtension, CStringArray &m_fileList, bool bFullPath)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	CString lpPatternName;
	CString strFileName;

	m_fileList.RemoveAll();

	lpPatternName.Format("%s\\*.%s", strPath, strExtension);
	hFind = FindFirstFile(lpPatternName, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		if(bFullPath)
		{
			strFileName.Format("%s\\%s", strPath, FindFileData.cFileName);
			m_fileList.Add(strFileName);
		}
		else
			m_fileList.Add(FindFileData.cFileName);


		while( FindNextFile(hFind, &FindFileData) )
		{
			if(bFullPath)
			{
				strFileName.Format("%s\\%s", strPath, FindFileData.cFileName);
				m_fileList.Add(strFileName);
			}
			else
				m_fileList.Add(FindFileData.cFileName);
		}

		FindClose(hFind);
	}

}

bool Utility::isLILACLogFile(const CString &m_file)
{
	if(m_file.Find("WebMind") != -1)
		return true;

	return false;
}

void Utility::getLILACLog(const CString &strPath, CStringArray &m_logList)
{
	CStringArray fileList;
	int i;

	m_logList.RemoveAll();

	fileList.RemoveAll();
	getFileList(strPath, _T("xml"), fileList, false);

	i = 0;
	while (i < fileList.GetSize())
	{
		if(!isLILACLogFile(fileList[i]))
			fileList.RemoveAt(i);
		else
			++i;
	}

	if(fileList.GetSize())
	{
		CString strBuffer;

		for(i = 0; i < fileList.GetSize(); i++)
		{
			strBuffer = strPath + _T("\\") + fileList[i];
			m_logList.Add(strBuffer);
		}
	}

	return;
}



void Utility::parseSequence(const CString &strSeq, const CString &strSplitter, CStringArray &strList)
{
	CString strBuffer;
	CString strUnit;
	int index;

	strList.RemoveAll();
	strBuffer = strSeq;

	while( (index = strBuffer.Find(strSplitter)) != -1)
	{
		strUnit.Empty();
		strUnit = strBuffer.Left(index);
		if(!strUnit.IsEmpty())
			strList.Add(strUnit);

		strBuffer = strBuffer.Right(strBuffer.GetLength() - index - strSplitter.GetLength());
	}

	if(!strBuffer.IsEmpty())
		strList.Add(strBuffer);

	strBuffer.Empty();
}

// find the location of the pattern string in the string array 0-based index
// -1 if fails
int Utility::FindPattern(const CString &strPattern, const CStringArray &arQuery)
{
	for(int i = 0; i < arQuery.GetSize(); i++)
	{
		if(strPattern.CompareNoCase(arQuery[i]) == 0)
			return i;
	}

	return -1;
}


//added by kosko
CString Utility::LoadFile(CString filename)
{
  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;

  CString res;
  const  char* fn = (LPCTSTR)filename;//CString->char*
  pFile = fopen (fn, "rb" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  // obtain file size:
  fseek (pFile , 0 , SEEK_END);//fseek+ftell获取文件的大小
  lSize = ftell (pFile);
  rewind (pFile);//Set position indicator to the beginning
  
  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
  
  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pFile);//fread读文件
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
  
  /* the whole file is now loaded in the memory buffer. */
  
  // terminate

  res.Format("%s",buffer);
  fclose (pFile);
  free (buffer);
  return res;
}
CTime Utility::ConvertCStringToCTime(CString time)
{
	const char* buf=(LPCTSTR)time ;// 时间类型是这个类型的:"2004-04-12   12:05:30 " CString->char*
	int   year,   month,   day,   hour,   minute,   second; 
	sscanf(buf,   "%d-%d-%d   %d:%d:%d ",   &year,   &month,   &day,   &hour,   &minute,   &second); 
	CTime   tm(year,   month,   day,   hour,   minute,   second); 
	return tm;
}
CString Utility::ConvertCtimeToCString(CTime time)
{
	return time.Format("%Y-%m-%d %H:%M:%S");
}
CTime Utility::ConvertCStringToCTime_Date(CString time)
{
	const char* buf=(LPCTSTR)time ;// 时间类型是这个类型的:"2004-04-12   12:05:30 " CString->char*
	int   year,   month,   day; 
	sscanf(buf,   "%d-%d-%d ",   &year,   &month,   &day); 
	CTime   tm(year,   month,   day); 
	return tm;
}
CString Utility::ConvertCTime_DateToCString(CTime time)
{
	return time.Format("%Y-%m-%d");
}
CTime Utility::ConvertCStringToCTime_Hour(CString time)
{
	const char* buf=(LPCTSTR)time ;// 时间类型是这个类型的:"2004-04-12   12:05:30 " CString->char*
	int   hour,   minute,   second; 
	sscanf(buf,   "%d-%d-%d ",   &hour,   &minute,   &second); 
	CTime   tm(hour,   minute,   second); 
	return tm;
}
CString Utility::ConvertCTime_HourToCString(CTime time)
{
		return time.Format("%H:%M:%S");
}
int Utility::ConvertCStringToInt(CString word)
{
	int n=atoi((char*)LPCTSTR(word));
	return n;
}

CString Utility::ConvertIntToCString(int n)
{
	CString s;
	s.Format("%d",n);
	return s;
}

double Utility::ConvertCStringToDouble(CString word)
{
	double d=atof((char*)LPCTSTR(word));
	return d;
}

CString Utility::ConvertDoubleToCString(double d)
{
	CString s;
	s.Format("%f",d);
	return s;
}

/***********************************************************
*Description: 按指定标记分隔字符串
*Parameters: content:待分隔字符串 tokenizer:指定标记
*Return: 分隔后的字符串数组，存入vector
*Anthor: kosko
**********************************************************
vector<string> Utility::StringSpilt(string content,char tokenizer)
{
	vector<string> strvec;
	string strtemp;
	
	string::size_type pos1, pos2;
	pos2 = content.find(tokenizer);
	pos1 = 0;        
	while (string::npos != pos2)
	{
		strvec.push_back(content.substr(pos1, pos2 - pos1));
		
		pos1 = pos2 + 1;
		pos2 = content.find(tokenizer, pos1);
	}
	strvec.push_back(content.substr(pos1));	
	return strvec;
}
*/

/*******************************************************
*Description: 对strs进行二分搜索，目标是查找dest
*Parameters: dest:目标串 strs:源串,这里假定源串为已排序好的
*Return: 查到为true，未查到为false
*Anthor: kosko
*******************************************************/
bool Utility::BinarySearch(char* strs[],int len,char* dest)
{
	int l=0,r=len-1,mid=(l+r)/2;
	while (l<=r)
	{
		char* mstr=strs[mid];
		if (strcmp(dest,mstr)<0)
			r=mid-1;
		else if (strcmp(dest,mstr)>0)
			l=mid+1;
		else
			return true;
		mid=(l+r)/2;
	}
	return false;
}

/***************************************************
*Description: 获取url的主机域名
*Parameters: url
*Return: 主机域名
*Anthor: kosko
***************************************************/
CString Utility::ConvertUrlToDomain(CString url)
{
	char* pHttpUrl=(LPSTR)(LPCTSTR)url;
	char *pSt,*pEnd;
	char sHostName[50];
	//获得主机名
	memset(sHostName,0,sizeof(sHostName));
	pSt = strstr(pHttpUrl,"://");
	if(pSt==NULL)
	{
		pEnd = strchr(pHttpUrl,'/');
		if(NULL==pEnd) strcpy(sHostName,pHttpUrl);
		else strncpy(sHostName,pHttpUrl,pEnd-pHttpUrl);
	}
	else
	{
		pEnd = strchr(pSt+strlen("://"),'/');
		if(NULL==pEnd) strcpy(sHostName,pSt+strlen("://"));
		else strncpy(sHostName,pSt+strlen("://"),pEnd-(pSt+strlen("://")));
	}
	CString host=sHostName;
	return host;
}

/************************************************************************
*Description: to retrieve the current path xx/xx...
*Parameters: none
*Return: the current path
*Anthor: kosko
************************************************************************/
CString Utility::GetCurrentPath()
{
	//以下是为了获取当前系统路径存入szFull
	TCHAR szFull[_MAX_PATH];   
	TCHAR szDrive[_MAX_DRIVE];   
	TCHAR szDir[_MAX_DIR];   
	::GetModuleFileName(NULL, szFull, sizeof(szFull)/sizeof(TCHAR));   
	_tsplitpath(szFull, szDrive, szDir, NULL, NULL);   
	_tcscpy(szFull, szDrive);   
	_tcscat(szFull, szDir);  
	CString path;
	path=szFull;
	return path;
}

/************************************************************************
*Description: to retrieve the current path xx//xx...
*Parameters: none
*Return: the current path
*Anthor: kosko
************************************************************************/
CString Utility::GetCurrentPath_DoubleSlash()
{
	CString debugPath=GetCurrentPath();
	int start = 0;
	int length=debugPath.GetLength();
    for(int i=0;i<length;i++)
	{
		if (start>=length)break;
		start = debugPath.Find('\\', start);
		debugPath.Insert(start, '\\');
		start++;
		start++;
	} 
	return debugPath;
}

/***********************************************************************************************
*Description:
*The following three functions are used to decode urlcode. Funciton 'UrlDecode_gb2312' is used
*to decode gb2312 encoded url, while Funciton 'UrlDecode_utf8' is used to decode utf8 encode url.
*Url is often encoded when the url is a searchengine url. 
*Anthor: kosko
************************************************************************************************/

#define IsHexNum(c) ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))

CString Utility::Utf8ToStringT(LPSTR str)
{
    _ASSERT(str);
    USES_CONVERSION;
    WCHAR *buf;
    int length = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    buf = new WCHAR[length+1];
    ZeroMemory(buf, (length+1) * sizeof(WCHAR));
    int re=MultiByteToWideChar(CP_UTF8, 0, str, -1, buf, length);
	if (re==0)//表示MultiByteToWideChar函数失败了，即utf8解析不出来
		return _T("");

    return (CString(W2T(buf)));
}

CString Utility::UrlDecode_utf8(LPCTSTR url)
{
    _ASSERT(url);
    USES_CONVERSION;
    LPSTR _url = T2A(const_cast<LPTSTR>(url));
    int i = 0;
    int length = (int)strlen(_url);
    CHAR *buf = new CHAR[length];
    ZeroMemory(buf, length);
    LPSTR p = buf;
    while(i < length)
    {
        if(i <= length -3 && _url[i] == '\%' && IsHexNum(_url[i+1]) && IsHexNum(_url[i+2]))
        {
            sscanf(_url + i + 1, "%x", p++);
            i += 3;
        }
        else
        {
            *(p++) = _url[i++];
        }
    }
    return Utf8ToStringT(buf);
}

CString Utility::UrlDecode_gb2312(LPCTSTR url)
{
	
    _ASSERT(url);
    USES_CONVERSION;
    LPSTR _url = T2A(const_cast<LPTSTR>(url));
    int i = 0;
    int length = (int)strlen(_url);
    CHAR *buf = new CHAR[length];
    ZeroMemory(buf, length);
    LPSTR p = buf;
    while(i < length)
    {
        if(i <= length -3 && _url[i] == '\%' && IsHexNum(_url[i+1]) && IsHexNum(_url[i+2]))
        {
            sscanf(_url + i + 1, "%x", p++);
            i += 3;
        }
        else
        {
            *(p++) = _url[i++];
        }
    }
	return buf;
}

/*
*Description:从url里获取用户查询的关键词
*Parameter:url
*Return:keywords
*Anthor: kosko
*/
CString Utility::GetKeyWordsFromUrl(LPCTSTR url)
{
//	CString pa=_T("(?:yahoo.+?[\?|&]p=|openfind.+?query=|google.+?q=|lycos.+?query=|onseek.+?keyword=|search\.tom.+?word=|search\.qq\.com.+?word=|zhongsou\.com.+?word=|search\.msn\.com.+?q=|yisou\.com.+?p=|sina.+?word=|sina.+?query=|sina.+?_searchkey=|sohu.+?word=|sohu.+?key_word=|sohu.+?query=|163.+?q=|baidu.+?wd=|baidu.+?kw=|baidu.+?word=|3721\.com.+?p=|Alltheweb.+?q=|soso.+?w=|115.+?q=|youdao.+?q=|sogou.+?query=|bing.+?q=|114.+?kw=)([^&]*)");
	rpattern pattern("(?:yahoo.+?[\?|&]p=|openfind.+?query=|google.+?q=|lycos.+?query=|onseek.+?keyword=|search\.tom.+?word=|search\.qq\.com.+?word=|zhongsou\.com.+?word=|search\.msn\.com.+?q=|yisou\.com.+?p=|sina.+?word=|sina.+?query=|sina.+?_searchkey=|sohu.+?word=|sohu.+?key_word=|sohu.+?query=|163.+?q=|baidu.+?wd=|baidu.+?kw=|baidu.+?word=|3721\.com.+?p=|Alltheweb.+?q=|soso.+?w=|115.+?q=|youdao.+?q=|sogou.+?query=|bing.+?q=|114.+?kw=)([^&]*)");
	match_results results;
	match_results::backref_type br = pattern.match(url,results);
	CString keywords=_T("");
	int groups=0;
	if (br.matched)
	{
		
        for (int iter = 1;iter < results.cbackrefs();iter += 2) // 目前还没有找到读取子表达式更好的方法
        {
            match_results::backref_type br_Temp = results.backref(iter);
            std::string strValue = br_Temp.str();
			if (strValue[strValue.length()-1]=='+')
				strValue=strValue.substr(0,strValue.length()-1);
			groups=FindLetterFromString(strValue.c_str(),'%');//找出关键词编码中的组数 %bc算1组
			LPCTSTR   lpstr   =   (LPCTSTR)strValue.c_str();
			keywords=UrlDecode_utf8(lpstr);
			int len=keywords.GetLength()/2;//这里是有问题的，比如搜"安徽",明显的groups/3是2，即表示2个字，而keywords.GetLength()是4
			if (keywords==_T(""))//utf8解析失败
			{
				keywords=UrlDecode_gb2312(lpstr);
			}else if (groups/3!=len)
			{
				keywords=UrlDecode_gb2312(lpstr);
			}
		}
		//MessageBox("匹配成功");
		printf("ok\n");
	}else
		//MessageBox("匹配失败");
		printf("failed");
	return keywords;
}

/*
*Description:从url里获取用户查询的关键词
*Parameter: string str,char ch
*Return: the number of letters in str
*Anthor: kosko
*/
int Utility::FindLetterFromString(const char* str,char ch)
{
	int cnt=0;
	int len=strlen(str);
	for (int i=0;i<len;i++)
		if (str[i]==ch)
			cnt++;
	return cnt;
}	
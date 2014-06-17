// ChineseCode.h: interface for the CChineseCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHINESECODE_H__E9206178_CA96_4B8A_AB2A_AD94C9CD12B1__INCLUDED_)
#define AFX_CHINESECODE_H__E9206178_CA96_4B8A_AB2A_AD94C9CD12B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
using namespace std;

class ChineseCode
{
   public:
       static void UTF_8ToUnicode(wchar_t* pOut,char *pText);  
       static void UnicodeToUTF_8(char* pOut,CString pText); 
       static void UnicodeToGB2312(char* pOut,wchar_t uData); 
       static void Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer);
      // static void GB2312ToUTF_8(string& pOut,char *pText, int pLen);
       static void UTF_8ToGB2312(string &pOut, char *pText, int pLen);
};

#endif // !defined(AFX_CHINESECODE_H__E9206178_CA96_4B8A_AB2A_AD94C9CD12B1__INCLUDED_)

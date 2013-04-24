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
 * Filename:  utility.cpp
 * Abstract:       
 *			  Utility functions for Chinese Language Processing
 * Author:   王小飞 
 *          (wangxiaofei@tsinghua.org.cn)
 * Date:     2004-12-5
 *
 * Notes:	
 *                
 * 
 ****************************************************************************/

#include "StdAfxw.h"
//#include <stdio.h>
#include "utilityw.h"


void GetChar(const char *cWord,int &niter,char *cWordArray)
{	
	
	if(*(cWord + niter)< 0)		//双字节
	{
		*cWordArray = *(cWord+niter);
		*(cWordArray+1) = *(cWord+niter+1);
		*(cWordArray+2) = '\0';
		niter+=2;
	}
	else						//单字节
	{		
		*cWordArray = *(cWord+niter);
		*(cWordArray+1) = '\0';
		niter++;
	}
}

int GetChar(char *cWord)
{
	int nWordLen;
	if(*cWord < 0)		//双字节
	{	
		nWordLen = 2;
	}
	else						//单字节
	{
		nWordLen =1;
	}
	return nWordLen;
}

//获得汉字序列码

int GetInnerCode(const char *sChar)
{
	int nLen=strlen(sChar);
	//0：其他单字节符号
	//1: digit
	//2: double-bytes digit
	//3: double-bytes charcter
	//4: Punctuation
	//5:换行符、回车符、制表符和空格
	//6-63:single character
	if(sChar[0]>'0'-1&&sChar[0]<'9'+1)//num
		return 1;
	if(sChar[0]>'a'-1&&sChar[0]<'z'+1||sChar[0]>'A'-1&&sChar[0]<'Z'+1)	//charcter-started word
		return ((int)sChar[0]-59);

	if(sChar[0]=='\n'||sChar[0]=='\t'||sChar[0]=='\r'||sChar[0]==' ')
		return 5;
	if(nLen == 1)		//其他单字节符号
		return 0;

	if(sChar[0]==-93&&sChar[1]>-81&&sChar[1]<-70)
		return 2;
	if(sChar[0]==-93&&(sChar[1]>=-63&&sChar[1]<=-38||sChar[1]>=-31&&sChar[1]<=-16))
		return 3;
	if(sChar[0]==-95 || sChar[0] == -93)//Punctuation
		return 4;

	//#define CC_ID(c1,c2) ((unsigned char)(c1)-176)*94+((unsigned char)(c2)-161)
	int Innercode;
	if((sChar[0])> -81 && sChar[1]> -96)     //GB2312
		Innercode = (80+sChar[0])*94+(sChar[1]+95)+RESERVE_ITEMS;
	else 
	{ 
		if(sChar[0] < -86)		// GBK/3
			Innercode = (127+sChar[0])*191+(sChar[1]+192)+6768+RESERVE_ITEMS;
		else Innercode = (86+sChar[0])*97+(sChar[1]+192)+12880+RESERVE_ITEMS;  // GBK/4
	}
	return Innercode;
}//*/

int GetInnerCode(const char *sChar,int nLen)
{
	//0：其他单字节符号
	//1: digit
	//2: double-bytes digit
	//3: double-bytes charcter
	//4: Punctuation
	//5:换行符、回车符、制表符和空格
	//6-63:single character
	if(sChar[0]>'0'-1&&sChar[0]<'9'+1)//num
		return 1;
	if(sChar[0]>'a'-1&&sChar[0]<'z'+1||sChar[0]>'A'-1&&sChar[0]<'Z'+1)	//charcter-started word
		return ((int)sChar[0]-59);

	if(sChar[0]=='\n'||sChar[0]=='\t'||sChar[0]=='\r'||sChar[0]==' ')
		return 5;
	if(nLen == 1)		//其他单字节符号
		return 0;

	if(sChar[0]==-93&&sChar[1]>-81&&sChar[1]<-70)
		return 2;
	if(sChar[0]==-93&&(sChar[1]>=-63&&sChar[1]<=-38||sChar[1]>=-31&&sChar[1]<=-16))
		return 3;
	if(sChar[0]==-95 || sChar[0] == -93)//Punctuation
		return 4;


	int Innercode;
	if((sChar[0])> -81 && sChar[1]> -96)     //GB2312
		Innercode = (80+sChar[0])*94+(sChar[1]+95)+RESERVE_ITEMS;
	else 
	{ 
		if(sChar[0] < -86)		// GBK/3
			Innercode = (127+sChar[0])*191+(sChar[1]+192)+6768+RESERVE_ITEMS;
		else Innercode = (86+sChar[0])*97+(sChar[1]+192)+12880+RESERVE_ITEMS;  // GBK/4
	}
	return Innercode;
}


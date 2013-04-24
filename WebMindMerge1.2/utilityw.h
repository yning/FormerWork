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
 * Filename:  utility.h
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

#include <stdio.h>
#include <string.h>
#define  SEPERATOR_LINK  "\t\n\r 　"

#define RESERVE_ITEMS   64          //58+5,大小写英文字母和其他一些符号

//从字符串中切分出一个汉字或其他符号
void GetChar(const char *cWord,int &niter,char *cWordArray);	
int GetChar(char *cWord);
int GetInnerCode(const char *sChar);			//获得序列码
int GetInnerCode(const char *sChar,int nLen);
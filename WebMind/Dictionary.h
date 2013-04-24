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
 * Filename:  Dictionary.h
 * Abstract:
 *            dictionary class definition
 *			  interface for the CDictionary class.
 * Author:   王小飞 
 *          (wangxiaofei@tsinghua.org.cn)
 * Date:     2004-12-5
 *
 * Notes:	
 *                
 * 
 ****************************************************************************/
// Dictionary.h: interface for the CDictionary class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "StdAfxw.h"
#include <vector>
#include "Dict.h"
#include "utilityw.h"
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
using namespace std;



#define MaxWordSize  30				//词长
#define Maxsize 5000				//一个状态的转移状态总数



class DICT_API CDictionary
{
public:
	CDictionary(void);
	~CDictionary(void);	

	vector <int>  *m_vBase;				// base数组
	vector <int>  *m_vCheck;			//check数组

	unsigned int  m_nDictItemCount;  //词典总词条数
	unsigned int m_nDictArraySize;   //词典数组长度
	unsigned int m_nWordMaxSize;    //词典最大词长度
	bool m_IsDirty;				//数组是否被修改过

	
private:

	char *m_lpBaseAddress;      //读入文件的基址
	long m_lLen;                //读入文件的长度 
	
	//用于建索引
	char **m_pcWordArray;
	int *m_piWordLenArray;
	
	//状态信息结构
	struct StateInfo
	{
		int nStateSub;			//当前状态的base和check数组下标
		int nNextStateCount;	//所有下一状态数
		int *pNextID;			//下一状态ID数组
	};
	class Comp
	{
	public:
		bool operator()(const StateInfo &sInfo1,const StateInfo &sInfo2)
		{
			return sInfo1.nNextStateCount > sInfo2.nNextStateCount;
		}
	};


public:
	
//	void DictStat(const char *pszFileName);			 //统计词典信息	
	//保存数组
	//IsNew为false时保存已经打开并且经过修改的数组；为true时重新建数组文件
	bool SaveArray(const char *cFileName,const char *cArrayFileName,bool IsDirty);							// 保存数组文件	
	bool DictLoad(const char* pszFileName);			//读入词典文本文件
	unsigned int GetArraySize();					//获得数组大小
	bool DeleteWord(const char * cWord);			//删除词语
	bool AddWord(const char *cWord);				//增加词语

	int SearchWord(const char *cWord);				//查找词语
	bool Load(const char *pszFileName);
/*	{
		ifstream Dictarrayin(pszFileName);
	if(Dictarrayin)
	{	
		Dictarrayin >> m_nDictItemCount >> m_nWordMaxSize >> m_nDictArraySize;

		m_vBase = new vector<int>;
		m_vCheck = new vector<int>;
		m_vBase->resize(m_nDictArraySize);
		m_vCheck->resize(m_nDictArraySize);
		
		for(int i=0;i<m_nDictArraySize;i++)
		{
			Dictarrayin >> (*m_vBase)[i];
			Dictarrayin >> (*m_vCheck)[i];
		}
		Dictarrayin.close();	
		return true;
	}
	else
	{
		printf("不能打开词典数组文件，请重新生成！");
		return false;
	}
	}//读入词典数组*/
//	float TimeStat(const char *pzFileName);
	bool FreeWordArray();							//释放m_piWordArray等几个数组的空间
private:
	
	bool Dict_Traversal(StateInfo *m_pStateInfo,int &nStateCount,int n);			//遍历词表
	//添加状态信息到结构数组中
	void AddInfo(char cWord[],char *cState,int n,int nLen,StateInfo *m_pStateInfo,int &nStateCount);   
	bool Construct_Arrays(const char * pszFileName);	 //构造数组，生成数组文件
	bool FindVal(StateInfo sInfo);						// 确定base值和check值
	bool FindVal(int nCurrSub,int nNextID);
	int GetStatArraySub(const char *cWord);				// 获得当前状态在base数组中的下标
	void Sort(StateInfo *pStateInfo,int nStateCount);   // 对状态数组排序
	//判断容器下标是否超出当前大小,如果超过则增长到下标大小
	bool ConsidSize(int nSub);				
	//在Check数组中查找值为nSub的项，返回项数
	int Find(int nSub,int *nSubArray,int *nIDArray,int *nValArray);		
	int Find(int nSub,int *nSubArray);
	int Find(int nSub);
	//增加字串
	bool AddChar(const char *cWord,int nSub);	
	//重新确定下标为nSub的数组元素值
	bool ReLocate(int nSub,int nID,int &nNextSub);		
};

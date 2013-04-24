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
 *			implementation of the CDictionary class
 * Author:  Wang Xiaofei
 *          (wangxiaofei@tsinghua.org.cn)
 * Date:     2004-12-5
 *
 * Notes:	
 *                
 * 
 ****************************************************************************/

#include "stdafx.h"
#include "Dictionary.h"
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define SINGLE_WORD_INNERCODE 20902



using namespace std;


CDictionary::CDictionary(void)
{	
	m_nDictItemCount = 0;
	m_nWordMaxSize = 0;
	m_piWordLenArray = NULL;
	m_pcWordArray = NULL;
	m_IsDirty = false;
	m_vBase = NULL;
	m_vCheck =NULL;

}

CDictionary::~CDictionary(void)
{		
	FreeWordArray();
}


/*********************************************************************
 *
 *  Func Name  : DictLoad
 *
 *  Description: 读入文本词典
 *              
 *
 *  Parameters : pszFileName: 词典文件名
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-10-14
 *********************************************************************/
bool CDictionary::DictLoad(const char *pszFileName)
{
	FILE *fpin = fopen(pszFileName,"r");

	if(fpin ==NULL)
	{
		return false;
	}
	else
	{
		
		fseek(fpin,0L,SEEK_SET);
		fscanf(fpin,"DictItemCount = %d\n",&m_nDictItemCount);
		fscanf(fpin,"MaxWordSize = %d\n",&m_nWordMaxSize);
		
		m_pcWordArray = new char *[m_nDictItemCount];
		m_piWordLenArray = new int[m_nDictItemCount];

		if(m_pcWordArray == NULL || m_piWordLenArray == NULL)
		{
			return false;
		}

		int i=0;
		char *buff = new char[m_nWordMaxSize];
		if(buff == NULL)
		{
			return false;
		}
		
		while(!feof(fpin))
		{
			fscanf(fpin,"%s\n",buff);
			if(strlen(buff) == 0)
				break;
			if(i > m_nDictItemCount-1)
			{
				printf("i > m_nDictItemCount\n");
				return false;
			}
			m_pcWordArray[i] = new char[MaxWordSize];
			if(m_pcWordArray[i] == NULL)
			{
				delete[] buff;
				buff = NULL;
				return false;
			}
			strcpy(m_pcWordArray[i],buff);
	
			m_piWordLenArray[i] = strlen(m_pcWordArray[i]);
			i++;	
			buff[0] = '\0';
		}
		delete[] buff;
		fclose(fpin);
		return true;
	}
}


/*********************************************************************
 *
 *  Func Name  : Dict_Traversal
 *
 *  Description: 遍历词表              
 *
 *  Parameters : pStateInfo:储存状态信息的数组
 *				 nStateCount: 数组中的状态数
 *			     n:状态的字数				
 *  Returns    : true: success false: failure
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-10-14
 *				2.modify 2005-5-20
 *********************************************************************/
bool CDictionary::Dict_Traversal(StateInfo *pStateInfo,int &nStateCount,int n)
{	
	char cWordId[MaxWordSize];			
	char cState[MaxWordSize];
	cState[0] = '\0';

	for(int i=0;i<m_nDictItemCount;i++)
	{
		AddInfo(m_pcWordArray[i],cState,n,m_piWordLenArray[i],pStateInfo,nStateCount);
	}

	return true;
}

/*********************************************************************
 *
 *  Func Name  : AddInfo
 *
 *  Description: 增加状态信息
 *              
 *
 *  Parameters : cWord: 当前词
 *				 iWordId:当前词的id号
 *				 pStateInfo:状态信息数组
 *               cState: 状态信息数组中的前一状态
 *				 n: 状态字数
 *				 nLen: 词长 
 *			     nStateCount: 数组中的状态信息数			
 *  Returns    : void
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-10-14
 *				2.modify 2005-5-20
 *********************************************************************/
void CDictionary::AddInfo(char cWord[],char *cState,int n,int nLen,StateInfo *pStateInfo,int &nStateCount)
{	
	int nIter = 0;
	char WordFir[MaxWordSize],buff[3];
	char *cWordFir = WordFir;
	GetChar(cWord,nIter,cWordFir);
	for(int i=1;i<n;i++)	//获得cWord的前n个字
	{	
		if(nIter == nLen)
			return;
		else 
		{
			GetChar(cWord,nIter,buff);
			strcat(cWordFir,buff);
		}
	}
	//如果当前状态后面还有输入变量
	if(nLen > nIter)	
	{	
		//获得cWord的第n+1个字
		char WordSec[3];
		char *cWordSec = WordSec;
		GetChar(cWord,nIter,cWordSec); 
		if(strcmp(cWordFir,cState) != 0)		//当前状态与数组中前一状态不同
		{	
			//将当前状态的信息加入状态信息数组
			pStateInfo[nStateCount].nStateSub = GetStatArraySub(cWordFir); 
			pStateInfo[nStateCount].pNextID = new int[Maxsize];
			pStateInfo[nStateCount].pNextID[0] = GetInnerCode(cWordSec);
			pStateInfo[nStateCount].nNextStateCount = 1;
			nStateCount++;;
			while((*cState++ = *cWordFir++) != '\0');  
		}
		else									//相同
		{	
			int nCount = pStateInfo[nStateCount-1].nNextStateCount;
			if(nCount == 0)
			{	
				pStateInfo[nStateCount-1].pNextID = new int[Maxsize];
				pStateInfo[nStateCount-1].pNextID[0] = GetInnerCode(cWordSec); 
				pStateInfo[nStateCount-1].nNextStateCount = 1;
				
			}
			else 
			{
				if(pStateInfo[nStateCount-1].pNextID[nCount-1] != GetInnerCode(cWordSec))
				{
					pStateInfo[nStateCount-1].pNextID[nCount] = GetInnerCode(cWordSec); 
					pStateInfo[nStateCount-1].nNextStateCount++;
				}
			}
		}
	}
	//如果当前状态已经是一个完整的词条
	else 
	{
		if(strcmp(cWordFir,cState) != 0)		//当前状态与数组中前一状态不同
		{	
			pStateInfo[nStateCount].nStateSub = GetStatArraySub(cWordFir); 
			pStateInfo[nStateCount].pNextID = NULL;
			pStateInfo[nStateCount].nNextStateCount = 0;
			
			ConsidSize(pStateInfo[nStateCount].nStateSub);
			(*m_vBase)[pStateInfo[nStateCount].nStateSub] = (-1) * pStateInfo[nStateCount].nStateSub;
			nStateCount++;
			while((*cState++ = *cWordFir++) != '\0');
		}
	}
}

/*********************************************************************
 *
 *  Func Name  : Construct_Arrays
 *
 *  Description: 构造数组
 *              
 *
 *  Parameters : pszFileName: 词典文件名
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-10-14
 *				2.modify 2005-5-20
 *********************************************************************/
bool CDictionary::Construct_Arrays(const char *pszFileName)
{
	bool bSuccess = true;   //是否构造成功
	m_vBase = new vector<int>;
	m_vCheck = new vector<int>;
	if(m_vBase == NULL || m_vCheck == NULL)
	{
		printf("new array fail\n");
		exit(-1);
	}
	if(DictLoad(pszFileName) == false)
		return false;
	if(m_nDictItemCount == 0)        //词典为空或读入失败
		bSuccess = false;
	else
	{		
		StateInfo *pStateInfo;
		int nStateArrayCount =0;
		int &nStateCount = nStateArrayCount;	

		for(int n=0;n< SINGLE_WORD_INNERCODE+RESERVE_ITEMS;n++)
		{
			m_vBase->push_back(0);
			m_vCheck->push_back(-1);//把前SINGLE_WORD_INNERCODE+RESERVE_ITEMS个位置留给首字
		}

		int i;
		for(i = 1;i<(m_nWordMaxSize)+1;i++)          //遍历词表
		{	printf("第 %d 轮\n",i);
			pStateInfo = new StateInfo[m_nDictItemCount]; 
			nStateCount = 0;
			Dict_Traversal(pStateInfo,nStateCount,i);
			Sort(pStateInfo,nStateCount);
			int k;
                        printf("%d\n",nStateCount);
			for(k = 0;k < nStateCount;k++)      //从下一状态较多的词数开始赋值
			{
				bSuccess = FindVal(pStateInfo[k]);
				if (pStateInfo[k].nNextStateCount> 0)
				{
					delete[] pStateInfo[k].pNextID;
					pStateInfo[k].pNextID = NULL;
					pStateInfo[k].nNextStateCount = 0;
				}
				
			}
			delete[] pStateInfo;
		}
		for(int j=0;j< SINGLE_WORD_INNERCODE+RESERVE_ITEMS;j++)
			(*m_vCheck)[j] = 0;
	}
	return bSuccess;
}  


/*********************************************************************
 *
 *  Func Name  : FindVal
 *
 *  Description: 确定当前状态的base值和其转移状态的check值
 *              
 *
 *  Parameters : sInfo: 当前状态
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-11-20
 *********************************************************************/
bool CDictionary::FindVal(StateInfo sInfo)
{
	bool bFound = true; 
	if (sInfo.nNextStateCount != 0)      //后面还有字
	{	
		bFound = false;
		int i = 1;
		while(!bFound)
		{	
			int k;
			for(k = 0; k < sInfo.nNextStateCount; k++)
			{
				if(i+sInfo.pNextID[k] < SINGLE_WORD_INNERCODE+RESERVE_ITEMS)
					break;
				if(ConsidSize(i+sInfo.pNextID[k]))
				{	
					if(i == sInfo.nStateSub )
						break;
					if(((*m_vBase)[i+sInfo.pNextID[k]] != 0) || ((*m_vCheck)[i+sInfo.pNextID[k]] != 0))
						break;
				}
			}
			if(k == sInfo.nNextStateCount) //找到i，确定相应的base和check值
			{	
				if(!ConsidSize(sInfo.nStateSub))
					(*m_vBase)[sInfo.nStateSub] = i;
				else
				{
					int nbaseval = (*m_vBase)[sInfo.nStateSub];
					if(nbaseval < 0)  //如果本身已经成词
						(*m_vBase)[sInfo.nStateSub] = (-1)*i;
					else (*m_vBase)[sInfo.nStateSub] = i; 				
				}
				for(k = 0; k < sInfo.nNextStateCount;k++)  //确定check值
				{
					(*m_vCheck)[i+sInfo.pNextID[k]] = sInfo.nStateSub;
				}
				bFound = true;
			}
			else i++;
		}
	}
	return bFound;
}


/*********************************************************************
 *
 *  Func Name  : FindVal
 *
 *  Description: 确定只有一个转移状态的base值及其转移状态的check值
 *              
 *
 *  Parameters : nCurrSub: 当前状态数组下标
 *				 nNextID: 输入变量
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-12-5
 *********************************************************************/
bool CDictionary::FindVal(int nCurrSub,int nNextID)
{
	bool bFound = false;
	int i = 1;
	while(!bFound)
	{
		ConsidSize(i+nNextID);
		if(i == (-1)*nNextID)
			i++;
		else if((*m_vBase)[i+nNextID]!=0 || (*m_vCheck)[i+nNextID]!=0 || 
			(i+nNextID)<SINGLE_WORD_INNERCODE+RESERVE_ITEMS)
			i++;
		else 
			bFound = true;
	}
	if(!ConsidSize(nCurrSub))
		(*m_vBase)[nCurrSub] = i;
	else
	{
		if((*m_vBase)[nCurrSub]<0)			//如果本身已经成词
			(*m_vBase)[nCurrSub] = (-1)*i;
		else
			(*m_vBase)[nCurrSub] = i;
	}
	(*m_vCheck)[i+nNextID] = nCurrSub;

	return bFound;
}



/*********************************************************************
 *
 *  Func Name  : SaveArray
 *
 *  Description: 保存数组文件
 *              
 *
 *  Parameters : cFileName: 词典文本文件名
 *				 cArrayFileName: 词典数组文件名
 *			     IsNew:词典是否被修改过，false保存当前数组，true重新构造数组
 *
 *  Returns    : true: success false: failure
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-12-5
 *				2.modify 2005-5-20
 *********************************************************************/
bool CDictionary::SaveArray(const char *cFileName,const char *cArrayFileName,bool IsNew)
{	
//	ofstream outbase("BaseArrayOut.txt");
//	ofstream outcheck("CheckArrayOut.txt");
	
	if(IsNew)		//新建数组文件
	{
		if(!Construct_Arrays(cFileName))
		{
			return false;	
		}
	
	}

	ofstream outDict(cArrayFileName);
	if(!outDict)
	{
		printf("无法保存数组文件");
		return false;
	}
	m_nDictArraySize = GetArraySize();
	outDict << m_nDictItemCount << '\n' << m_nWordMaxSize << '\n' << m_nDictArraySize << '\n';
	int i;
	for(i = 0; i<m_nDictArraySize;i++)
	{
		outDict << (*m_vBase)[i] << '\n' << (*m_vCheck)[i] << '\n';
	}
	outDict << endl;
	outDict.close();
	if(IsNew)
	{
		FreeWordArray();
	}




	return true;	
}



/*********************************************************************
 *
 *  Func Name  : Sort
 *
 *  Description: 排序
 *              
 *  Parameters : StateInfo: 要排序的状态信息数组
 *				 nStateCount: 状态数
 *			    				
 *  Returns    : void
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-10-15
 *********************************************************************/
void CDictionary::Sort(StateInfo *pStateInfo,int nStateCount)
{	
	stable_sort(pStateInfo,pStateInfo+nStateCount,Comp());
}

/*********************************************************************
 *
 *  Func Name  : DictStat
 *
 *  Description: 词典信息统计
 *              
 *
 *  Parameters : pszFileName: 词典文件名
 *			    				
 *  Returns    : void
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-10-15
 *********************************************************************/
/*
void CDictionary::DictStat(const char *pszFileName)
{	
	int nCount = 0;
	int nWordMaxSize = 0;
	char buff[30];
	if(DictLoad(pszFileName))
	{
		char *lpszCur = m_lpBaseAddress;
		while(lpszCur < (m_lpBaseAddress + m_lLen ))
		{	
			sscanf(lpszCur,"%s\n",buff);
			if(nWordMaxSize < strlen(buff))
				nWordMaxSize = strlen(buff);
			char *lpszDest  = strchr(lpszCur,(int)('\n'));
			if(lpszDest != NULL)
				lpszCur = lpszDest + 1;
			nCount++;
		}
	}
	else 
		printf("词典读入失败！");
	m_nDictItemCount = nCount;
	m_nWordMaxSize = (int)ceil((float)(nWordMaxSize/2));
}
*/

/*********************************************************************
 *
 *  Func Name  : ConsidSize
 *
 *  Description: 判断容器下标是否超出当前大小,如果超过则增长到下标大小
 *              
 *
 *  Parameters : nSub: 容器下标
 *			    				
 *  Returns    : true: 不超过 false: 超过
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-11-5
 *				2.modify 2005-5-20
 *********************************************************************/
bool CDictionary::ConsidSize(int nSub)
{	
	bool less = true;
	int nSize = GetArraySize();
	if(nSub >= nSize)
	{	
		less = false;
		for(int i = 0;i < (nSub - nSize +1);i++ )
		{
			m_vBase->push_back(0);
			m_vCheck->push_back(0);
		}
	}
	return less;
} 


/*********************************************************************
 *
 *  Func Name  : GetStatArraySub
 *
 *  Description: 获得词的数组下标
 *              
 *
 *  Parameters : cWord: 词语
 *			    				
 *  Returns    : 数组下标
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-10-15
 *********************************************************************/
int CDictionary::GetStatArraySub(const char* cWord)
{
	int nLen = strlen(cWord);
	int nIt = 0;
	int &nIter = nIt;
	char charac[3];
	GetChar(cWord,nIter,charac);
	int ID = GetInnerCode(charac);
	int nSub;
	nSub = ID;
	while(nIter < nLen)
	{	
		GetChar(cWord,nIter,charac);
		ID = GetInnerCode(charac);
		nSub = abs((*m_vBase)[nSub]) + ID;
	}
	return nSub;
}


/*********************************************************************
 *
 *  Func Name  : GetArraySize
 *
 *  Description: 获得数组长度
 *              
 *
 *  Parameters : 
 *			    				
 *  Returns    : 数组长度
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-10-15
 *********************************************************************/
unsigned int CDictionary::GetArraySize()
{	
	return (unsigned int)m_vBase->size();
}


/*********************************************************************
 *
 *  Func Name  : Load
 *
 *  Description: 读入数组文件
 *              
 *
 *  Parameters : pszFileName: 词典数组文件名
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-10-15
 *				2.modify 2005-5-20
 *********************************************************************/
bool CDictionary::Load(const char *pszFileName)
{
/*
//这段注释掉的，是原作者写的读取文件函数，分一篇11kb的文章需要1.7s-1.8s左右，
//我分析后，认为就是下面这段读取文件的代码造成的,因此注释掉，采用我自己写的scanf读取文件方法 by kosko

	ifstream Dictarrayin(pszFileName);
	//long begintime=clock();
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
		//long endtime=clock();
	    //printf("test load function:%lf",(endtime-begintime)/1000.0);
		return true;
	}
	else
	{
		printf("不能打开词典数组文件，请重新生成！");
		return false;
	}
*/
	//////////////////////////////////////////////////////////////////////////
	//下面该用scanf来读文件, 是我自己写的，测试后，分一篇11kb的文章，只需要0.07s-0.08s，效率基本改进了20倍+ by kosko
	//其实还可以进一步改进的,用fread可以进一步改进，看来ifstream实在是害人不浅 by kosko
	long begintime=clock();
	FILE *f=freopen(pszFileName,"r",stdin);
	if (f)
	{
		scanf("%d",&m_nDictItemCount);
		scanf("%d",&m_nWordMaxSize);
		scanf("%d",&m_nDictArraySize);
		m_vBase = new vector<int>;
		m_vCheck = new vector<int>;
		m_vBase->resize(m_nDictArraySize);
		m_vCheck->resize(m_nDictArraySize);
		for (int i=0;i<m_nDictArraySize;i++)
		{
			scanf("%d",&((*m_vBase)[i]));
			scanf("%d",&((*m_vCheck)[i]));
		}
		long endtime=clock();
		printf("test load function %lf",(endtime-begintime)/1000.0);
		return true;
	}
	else
	{
		printf("不能打开词典数组文件，请重新生成！");
		return false;
	}
}



/*********************************************************************
 *
 *  Func Name  : SearchWord
 *
 *  Description: 查找词语
 *              
 *
 *  Parameters : cWord: 待查词
 *			    				
 *  Returns    : 0:success -1:fail
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-10-15
 *				2.modify 2005-5-20
 *********************************************************************/
int CDictionary::SearchWord(const char *cWord)
{
	int iSize = GetArraySize();
	int nLen = strlen(cWord);
	int nIter = 0;
	char cSingWord[3];
	char *pWord = cSingWord;
	GetChar(cWord,nIter,cSingWord);
	int ID = GetInnerCode(cSingWord);
	if(ID<0 || ID > iSize)
		return -1;
	int iSub = ID;
	int k = iSub ;
	
	
		while (nIter < nLen) 
		{	
			GetChar(cWord,nIter,cSingWord);	
			iSub = abs((*m_vBase)[iSub]) + GetInnerCode(cSingWord);  
			if(iSub > iSize)
			{
				return -1;
			}
			if((*m_vCheck)[iSub] != k)
			{
				return -1;
			}
			k = iSub;
		}
		if((*m_vBase)[iSub] < 0)
		{
			return 0;
		}

	return -1;
}

/*********************************************************************
 *
 *  Func Name  : AddChar
 *
 *  Description: 添加字串
 *              
 *
 *  Parameters : cWord: 要添加的字串
 *				 nSub: 上一状态数组下标
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-12-5
 *				2.modify 2005-5-20
 *********************************************************************/
bool CDictionary::AddChar(const char *cWord,int nSub)
{

	int nLen,nID;
	int nBaseVal,nCheckVal,nCurrentSub=0;
	int &nCurrSub = nCurrentSub;
	nLen = GetChar((char *)cWord);
	nID = GetInnerCode(cWord,nLen);
	nCurrSub = abs((*m_vBase)[nSub]) + nID;
	bool bIsDirty;
	ConsidSize(nCurrSub);
	if(nLen == strlen(cWord))	 
	{
		if(nSub != 0)
		{
			m_nDictItemCount++;
			bIsDirty = ReLocate(nSub,nID,nCurrSub);
			if(!bIsDirty || (*m_vBase)[nCurrSub] >0)
			{
				(*m_vBase)[nCurrSub] = (-1)*(*m_vBase)[nCurrSub];
			}
			else if(bIsDirty)
				(*m_vBase)[nCurrSub] = (-1)*nCurrSub;
			
		}
		else 
		{
			nBaseVal = (*m_vBase)[nCurrSub];
			nCheckVal = (*m_vCheck)[nCurrSub];
			if(nBaseVal == 0 &&  nCheckVal== 0)
			{	
				(*m_vBase)[nCurrSub] = (-1) * nCurrSub;
				(*m_vCheck)[nCurrSub] = nSub;

			}
		}
	}
	else
	{	
		if(nSub != 0)
		{
			bIsDirty = ReLocate(nSub,nID,nCurrSub);
			if(bIsDirty)
			{
				while (nLen < strlen(cWord))
				{
					int n = GetChar((char*)(cWord+nLen));
					nID = GetInnerCode(cWord+nLen,n);
					FindVal(nCurrSub,nID);	
					nCurrSub = (*m_vBase)[nCurrSub]+nID;
					nLen += n;
				}

				(*m_vBase)[nCurrSub] = (-1)*nCurrSub;
				return true;
			}
			else
				AddChar(cWord+nLen,nCurrSub);
		}
		else
		{
			nSub = nCurrSub;
			int nTempLen = GetChar((char *)(cWord+nLen));
			nID = GetInnerCode(cWord+nLen,nTempLen);
			nCurrSub = abs((*m_vBase)[nSub])+nID;
			ConsidSize(nCurrSub);
			bIsDirty = ReLocate(nSub,nID,nCurrSub);
			nLen += nTempLen;
			if(nLen == strlen(cWord))
			{
				if(!bIsDirty || (*m_vBase)[nCurrSub] >0)
				{
					(*m_vBase)[nCurrSub] = (-1)*(*m_vBase)[nCurrSub];
				}
				else if(bIsDirty)
					(*m_vBase)[nCurrSub] = (-1)*nCurrSub;
			}
			else 
			{
				if(bIsDirty)
				{
					while (nLen < strlen(cWord))
					{
						int n = GetChar((char*)(cWord+nLen));
						nID = GetInnerCode(cWord+nLen,n);
						FindVal(nCurrSub,nID);	
						nCurrSub = (*m_vBase)[nCurrSub]+nID;
						nLen += n;
					}
					(*m_vBase)[nCurrSub] = (-1)*nCurrSub;
					return true;
				}
				else 
					AddChar(cWord+nLen,nCurrSub);
			}
		}
	}
	return true;
}

/*********************************************************************
 *
 *  Func Name  : AddWord
 *
 *  Description: 添加词语
 *
 *  Parameters : cWord: 待添加的词
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-12-5
 *********************************************************************/
bool CDictionary::AddWord(const char *cWord)
{
	if(SearchWord(cWord)!=-1)
	{
		printf("要添加的词已经在词典中！");
		return true;
	}
	int nLen = strlen(cWord);
	m_nDictItemCount++;
	bool bSucc = AddChar(cWord,0);
	if(bSucc==true)
	{
		m_nDictItemCount++;
		m_IsDirty = true;
		if((int)ceil((float)nLen/2) > m_nWordMaxSize)
			m_nWordMaxSize = ceil((float)(nLen/2));
	}

	return bSucc;
}

/*********************************************************************
 *
 *  Func Name  : DeleteWord
 *
 *  Description: 删除词语
 *              
 *
 *  Parameters : cWord：待删除词
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-12-5
 *********************************************************************/
bool CDictionary::DeleteWord(const char *cWord)
{
	if(SearchWord(cWord) == -1)
	{
		printf("要删除的词在词典中不存在");
		return false;
	}
	int nLen = strlen(cWord);
	int nIter = 0;
	int nID,nWordLen,nNextCount;
	vector<int> nBaseVal,nCheckVal,nSubArray;
	nWordLen = GetChar((char *)(cWord+nIter));
	nID = GetInnerCode(cWord+nIter,nWordLen);
	nIter += nWordLen;
	nSubArray.push_back(nID);
	while(nIter < nLen)		//统计相关信息
	{
		nWordLen = GetChar((char *)(cWord+nIter));
		nID = GetInnerCode(cWord+nIter,nWordLen);
		nIter += nWordLen;
		int nSub = abs((*m_vBase)[*(nSubArray.end()-1)])+nID;
		nSubArray.push_back(nSub);
	}
	int nSize = nSubArray.size();
	
	if((*m_vBase)[nSubArray[nSize-1]] == (-1)*nSubArray[nSize-1])	//要删除的词为最大词
	{
		nNextCount = Find(nSubArray[nSize-2]);
		(*m_vBase)[nSubArray[nSize-1]] = 0;
		(*m_vCheck)[nSubArray[nSize-1]] = 0;
		nNextCount--;
		int i;
		for(i=nSize-2;i>-1;i--)
		{
			if((*m_vBase)[nSubArray[i]] > 0 && nNextCount == 0)
			{
				nNextCount = Find(nSubArray[i-1]);
				(*m_vBase)[nSubArray[i]] = 0;
				(*m_vCheck)[nSubArray[i]] = 0;
				nNextCount--;
			}
			else if((*m_vBase)[nSubArray[i]] > 0 && nNextCount > 0)
			{
				(*m_vBase)[nSubArray[i]] = 0;
				(*m_vCheck)[nSubArray[i]] = 0;
				break;
			}
			else 
				break;
		}		
	}
	else					//并非最大词
	{
		(*m_vBase)[nSubArray[nSize-1]] = abs((*m_vBase)[nSubArray[nSize-1]]);
	}
	
	m_nDictItemCount--;
	m_IsDirty = true;
//	if(ceil((float)nLen/2) == m_nWordMaxSize)
//		m_nWordMaxSize--;
	return true;
}


/*********************************************************************
 *
 *  Func Name  : Relocate
 *
 *  Description: 重新确定下标为nSub的base值
 *              
 *
 *  Parameters : nSub: 当前状态数组下标
 *				 nID: 输入变量
 *				 nNextSub: 转移状态数组下标
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-12-5
 *				2.modify 2005-5-20
 *********************************************************************/
bool CDictionary::ReLocate(int nSub,int nID,int &nNextSub)
{
	if(nNextSub > 6767+RESERVE_ITEMS && (*m_vCheck)[nNextSub] == 0)		//空位置
	{
		(*m_vCheck)[nNextSub] = nSub;
		m_IsDirty = true;
		return true;
	}
	else if((*m_vCheck)[nNextSub] != nSub)		//位置非空
	{
		int *nSubArray = new int[Maxsize];
		int *nValArray = new int[Maxsize];
		int *nIDArray = new int[Maxsize];
		int nCount = Find(nSub,nSubArray,nIDArray,nValArray);
		if(nCount == 0)
			FindVal(nSub,nID);
		else
		{
			StateInfo reInfo;
			reInfo.nNextStateCount = nCount+1;
			reInfo.nStateSub = nSub;
			reInfo.pNextID = new int[Maxsize];
		
			for(int i=0;i<nCount;i++)
			{
				reInfo.pNextID[i] = *(nIDArray+i);

			}
			reInfo.pNextID[nCount] = nID;
			FindVal(reInfo);
			int *nArray = new int[Maxsize];
			for(int k=0;k<nCount;k++)
			{
				//把原来的值复制到新位置
				int nOtherNextSub = abs((*m_vBase)[nSub])+nIDArray[k];
				(*m_vBase)[nOtherNextSub] = nValArray[k];

				int nThirCount = Find(*(nSubArray+k),nArray);
				
				for(int j=0;j<nThirCount;j++)
				{
					(*m_vCheck)[nArray[j]] = nOtherNextSub;
				}

				//原位置腾空
				(*m_vBase)[*(nSubArray+k)] = 0;
				(*m_vCheck)[*(nSubArray+k)] = 0;
				
			}
			delete [] nArray;
			delete [] reInfo.pNextID;
			reInfo.pNextID = NULL;
		}	
		nNextSub = abs((*m_vBase)[nSub])+nID;
		
		delete [] nIDArray;
		delete [] nSubArray;
		delete [] nValArray;
		return true;
	}
	return false;
}

/*********************************************************************
 *
 *  Func Name  : Find
 *
 *  Description: 统计一个状态的转移状态总数
 *              
 *  Parameters : nSub: 当前状态数组下标
 *				 nSubArray: 转移状态下标数组
 *				 nIDArray: 输入变量数组
 *				 nValArray: 转移状态base值数组
 *			    				
 *  Returns    : 转移状态总数
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-12-5
 *				2.modify 2005-5-20
 *********************************************************************/
int CDictionary::Find(int nSub,int *nSubArray,int *nIDArray,int *nValArray)
{
	int i;
	int nCount = 0;
	for(i=(SINGLE_WORD_INNERCODE+RESERVE_ITEMS);i<m_nDictArraySize;i++)
	{
		if((*m_vCheck)[i] == nSub)
		{
			*nSubArray = i;
			*nIDArray = i - abs((*m_vBase)[nSub]);
			*nValArray = (*m_vBase)[i];
			nSubArray++;
			nIDArray++;
			nValArray++;
			nCount++;
		}
	}
	return nCount;
}

/*********************************************************************
 *
 *  Func Name  : Find
 *
 *  Description: 统计一个状态的转移状态总数
 *              
 *  Parameters : nSub: 当前状态数组下标
 *				 nSubArray: 转移状态下标数组
 *			    				
 *  Returns    : 转移状态总数
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-12-5
 *********************************************************************/
int CDictionary::Find(int nSub,int *nSubArray)
{
	int i;
	int nCount = 0;
	for(i=(SINGLE_WORD_INNERCODE+RESERVE_ITEMS);i<m_nDictArraySize;i++)
	{
		if((*m_vCheck)[i] == nSub)
		{
			*nSubArray = i;
			nSubArray++;
			nCount++;
		}
	}
	return nCount;
}

/*********************************************************************
 *
 *  Func Name  : Find
 *
 *  Description: 统计一个状态的转移状态总数
 *              
 *  Parameters : nSub: 当前状态数组下标
 *			    				
 *  Returns    : 转移状态总数
 *  Author     : 王小飞  
 *  History    : 
 *              1.create 2004-12-5
 *********************************************************************/
int CDictionary::Find(int nSub)
{
	int i;
	int nCount = 0;
	for(i=0;i<m_nDictArraySize;i++)
	{
		if((*m_vCheck)[i] == nSub)
		{
			nCount++;
		}
	}
	return nCount;
}

bool CDictionary::FreeWordArray()
{
	if(m_vBase!=NULL)
	{
		delete m_vBase;
		m_vBase = NULL;
	}
	if(m_vCheck!=NULL)
	{
		delete m_vCheck;
		m_vCheck = NULL;
	}
	if(m_piWordLenArray != NULL)
	{
		delete[] m_piWordLenArray;
		m_piWordLenArray = NULL;
	}
	if(m_pcWordArray != NULL)
	{
		for(int i=0;i<m_nDictItemCount;i++)
		{
			delete[] m_pcWordArray[i];
			m_pcWordArray[i] = NULL;
		}
		m_pcWordArray = NULL;
	}
	m_nDictItemCount = 0;
	m_nWordMaxSize = 0;
	m_nDictArraySize = 0;

	return true;
}

/*
float CDictionary::TimeStat(const char *pzFileName)
{
	double aveTime = 0;
	//unsigned long nItemCount = 0;
	char buff[256];
	LARGE_INTEGER lInt;
	double dTotal = 0;
	double dFreq,dMinus;
	LONGLONG lEnd,lStart;
	Load("OutDictArray.dct");

	ifstream infile(pzFileName);
	ofstream oCannotFind("cannot.txt");
	if(infile)
	{	
		QueryPerformanceFrequency(&lInt);
		dFreq =(double)lInt.QuadPart;
		QueryPerformanceCounter(&lInt);				
		lStart = lInt.QuadPart;
		while(!infile.eof())
		{
			infile >> buff;
			if( strcmp(buff,"") != 0)
			{
				bool is = SearchWord(buff);
			//	nItemCount++;
				if(!is)
				{
						//CString cStat;
						//cStat.Format("璇?%s 鍦ㄨ瘝鍏镐腑鏃犳硶鎵惧埌",buff);
						//AfxMessageBox((LPCSTR)cStat);						
					oCannotFind << buff << '\n';
				}
			}
				
		}
	}		
	oCannotFind.close();
//	}
	QueryPerformanceCounter(&lInt);
	lEnd = lInt.QuadPart;
	dMinus = (double)(lEnd - lStart);
	dTotal = dMinus / dFreq;
	
	CString cStat;
	cStat.Format("%e",dTotal);
	AfxMessageBox(cStat);
	
	return dTotal;
}//*/

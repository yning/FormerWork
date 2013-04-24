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
 *  Description: �����ı��ʵ�
 *              
 *
 *  Parameters : pszFileName: �ʵ��ļ���
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : ��С��  
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
 *  Description: �����ʱ�              
 *
 *  Parameters : pStateInfo:����״̬��Ϣ������
 *				 nStateCount: �����е�״̬��
 *			     n:״̬������				
 *  Returns    : true: success false: failure
 *  Author     : ��С��  
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
 *  Description: ����״̬��Ϣ
 *              
 *
 *  Parameters : cWord: ��ǰ��
 *				 iWordId:��ǰ�ʵ�id��
 *				 pStateInfo:״̬��Ϣ����
 *               cState: ״̬��Ϣ�����е�ǰһ״̬
 *				 n: ״̬����
 *				 nLen: �ʳ� 
 *			     nStateCount: �����е�״̬��Ϣ��			
 *  Returns    : void
 *  Author     : ��С��  
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
	for(int i=1;i<n;i++)	//���cWord��ǰn����
	{	
		if(nIter == nLen)
			return;
		else 
		{
			GetChar(cWord,nIter,buff);
			strcat(cWordFir,buff);
		}
	}
	//�����ǰ״̬���滹���������
	if(nLen > nIter)	
	{	
		//���cWord�ĵ�n+1����
		char WordSec[3];
		char *cWordSec = WordSec;
		GetChar(cWord,nIter,cWordSec); 
		if(strcmp(cWordFir,cState) != 0)		//��ǰ״̬��������ǰһ״̬��ͬ
		{	
			//����ǰ״̬����Ϣ����״̬��Ϣ����
			pStateInfo[nStateCount].nStateSub = GetStatArraySub(cWordFir); 
			pStateInfo[nStateCount].pNextID = new int[Maxsize];
			pStateInfo[nStateCount].pNextID[0] = GetInnerCode(cWordSec);
			pStateInfo[nStateCount].nNextStateCount = 1;
			nStateCount++;;
			while((*cState++ = *cWordFir++) != '\0');  
		}
		else									//��ͬ
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
	//�����ǰ״̬�Ѿ���һ�������Ĵ���
	else 
	{
		if(strcmp(cWordFir,cState) != 0)		//��ǰ״̬��������ǰһ״̬��ͬ
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
 *  Description: ��������
 *              
 *
 *  Parameters : pszFileName: �ʵ��ļ���
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : ��С��  
 *  History    : 
 *              1.create 2004-10-14
 *				2.modify 2005-5-20
 *********************************************************************/
bool CDictionary::Construct_Arrays(const char *pszFileName)
{
	bool bSuccess = true;   //�Ƿ���ɹ�
	m_vBase = new vector<int>;
	m_vCheck = new vector<int>;
	if(m_vBase == NULL || m_vCheck == NULL)
	{
		printf("new array fail\n");
		exit(-1);
	}
	if(DictLoad(pszFileName) == false)
		return false;
	if(m_nDictItemCount == 0)        //�ʵ�Ϊ�ջ����ʧ��
		bSuccess = false;
	else
	{		
		StateInfo *pStateInfo;
		int nStateArrayCount =0;
		int &nStateCount = nStateArrayCount;	

		for(int n=0;n< SINGLE_WORD_INNERCODE+RESERVE_ITEMS;n++)
		{
			m_vBase->push_back(0);
			m_vCheck->push_back(-1);//��ǰSINGLE_WORD_INNERCODE+RESERVE_ITEMS��λ����������
		}

		int i;
		for(i = 1;i<(m_nWordMaxSize)+1;i++)          //�����ʱ�
		{	printf("�� %d ��\n",i);
			pStateInfo = new StateInfo[m_nDictItemCount]; 
			nStateCount = 0;
			Dict_Traversal(pStateInfo,nStateCount,i);
			Sort(pStateInfo,nStateCount);
			int k;
                        printf("%d\n",nStateCount);
			for(k = 0;k < nStateCount;k++)      //����һ״̬�϶�Ĵ�����ʼ��ֵ
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
 *  Description: ȷ����ǰ״̬��baseֵ����ת��״̬��checkֵ
 *              
 *
 *  Parameters : sInfo: ��ǰ״̬
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : ��С��  
 *  History    : 
 *              1.create 2004-11-20
 *********************************************************************/
bool CDictionary::FindVal(StateInfo sInfo)
{
	bool bFound = true; 
	if (sInfo.nNextStateCount != 0)      //���滹����
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
			if(k == sInfo.nNextStateCount) //�ҵ�i��ȷ����Ӧ��base��checkֵ
			{	
				if(!ConsidSize(sInfo.nStateSub))
					(*m_vBase)[sInfo.nStateSub] = i;
				else
				{
					int nbaseval = (*m_vBase)[sInfo.nStateSub];
					if(nbaseval < 0)  //��������Ѿ��ɴ�
						(*m_vBase)[sInfo.nStateSub] = (-1)*i;
					else (*m_vBase)[sInfo.nStateSub] = i; 				
				}
				for(k = 0; k < sInfo.nNextStateCount;k++)  //ȷ��checkֵ
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
 *  Description: ȷ��ֻ��һ��ת��״̬��baseֵ����ת��״̬��checkֵ
 *              
 *
 *  Parameters : nCurrSub: ��ǰ״̬�����±�
 *				 nNextID: �������
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : ��С��  
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
		if((*m_vBase)[nCurrSub]<0)			//��������Ѿ��ɴ�
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
 *  Description: ���������ļ�
 *              
 *
 *  Parameters : cFileName: �ʵ��ı��ļ���
 *				 cArrayFileName: �ʵ������ļ���
 *			     IsNew:�ʵ��Ƿ��޸Ĺ���false���浱ǰ���飬true���¹�������
 *
 *  Returns    : true: success false: failure
 *  Author     : ��С��  
 *  History    : 
 *              1.create 2004-12-5
 *				2.modify 2005-5-20
 *********************************************************************/
bool CDictionary::SaveArray(const char *cFileName,const char *cArrayFileName,bool IsNew)
{	
//	ofstream outbase("BaseArrayOut.txt");
//	ofstream outcheck("CheckArrayOut.txt");
	
	if(IsNew)		//�½������ļ�
	{
		if(!Construct_Arrays(cFileName))
		{
			return false;	
		}
	
	}

	ofstream outDict(cArrayFileName);
	if(!outDict)
	{
		printf("�޷����������ļ�");
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
 *  Description: ����
 *              
 *  Parameters : StateInfo: Ҫ�����״̬��Ϣ����
 *				 nStateCount: ״̬��
 *			    				
 *  Returns    : void
 *  Author     : ��С��  
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
 *  Description: �ʵ���Ϣͳ��
 *              
 *
 *  Parameters : pszFileName: �ʵ��ļ���
 *			    				
 *  Returns    : void
 *  Author     : ��С��  
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
		printf("�ʵ����ʧ�ܣ�");
	m_nDictItemCount = nCount;
	m_nWordMaxSize = (int)ceil((float)(nWordMaxSize/2));
}
*/

/*********************************************************************
 *
 *  Func Name  : ConsidSize
 *
 *  Description: �ж������±��Ƿ񳬳���ǰ��С,����������������±��С
 *              
 *
 *  Parameters : nSub: �����±�
 *			    				
 *  Returns    : true: ������ false: ����
 *  Author     : ��С��  
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
 *  Description: ��ôʵ������±�
 *              
 *
 *  Parameters : cWord: ����
 *			    				
 *  Returns    : �����±�
 *  Author     : ��С��  
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
 *  Description: ������鳤��
 *              
 *
 *  Parameters : 
 *			    				
 *  Returns    : ���鳤��
 *  Author     : ��С��  
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
 *  Description: ���������ļ�
 *              
 *
 *  Parameters : pszFileName: �ʵ������ļ���
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : ��С��  
 *  History    : 
 *              1.create 2004-10-15
 *				2.modify 2005-5-20
 *********************************************************************/
bool CDictionary::Load(const char *pszFileName)
{
/*
//���ע�͵��ģ���ԭ����д�Ķ�ȡ�ļ���������һƪ11kb��������Ҫ1.7s-1.8s���ң�
//�ҷ�������Ϊ����������ζ�ȡ�ļ��Ĵ�����ɵ�,���ע�͵����������Լ�д��scanf��ȡ�ļ����� by kosko

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
		printf("���ܴ򿪴ʵ������ļ������������ɣ�");
		return false;
	}
*/
	//////////////////////////////////////////////////////////////////////////
	//�������scanf�����ļ�, �����Լ�д�ģ����Ժ󣬷�һƪ11kb�����£�ֻ��Ҫ0.07s-0.08s��Ч�ʻ����Ľ���20��+ by kosko
	//��ʵ�����Խ�һ���Ľ���,��fread���Խ�һ���Ľ�������ifstreamʵ���Ǻ��˲�ǳ by kosko
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
		printf("���ܴ򿪴ʵ������ļ������������ɣ�");
		return false;
	}
}



/*********************************************************************
 *
 *  Func Name  : SearchWord
 *
 *  Description: ���Ҵ���
 *              
 *
 *  Parameters : cWord: �����
 *			    				
 *  Returns    : 0:success -1:fail
 *  Author     : ��С��  
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
 *  Description: ����ִ�
 *              
 *
 *  Parameters : cWord: Ҫ��ӵ��ִ�
 *				 nSub: ��һ״̬�����±�
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : ��С��  
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
 *  Description: ��Ӵ���
 *
 *  Parameters : cWord: ����ӵĴ�
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : ��С��  
 *  History    : 
 *              1.create 2004-12-5
 *********************************************************************/
bool CDictionary::AddWord(const char *cWord)
{
	if(SearchWord(cWord)!=-1)
	{
		printf("Ҫ��ӵĴ��Ѿ��ڴʵ��У�");
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
 *  Description: ɾ������
 *              
 *
 *  Parameters : cWord����ɾ����
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : ��С��  
 *  History    : 
 *              1.create 2004-12-5
 *********************************************************************/
bool CDictionary::DeleteWord(const char *cWord)
{
	if(SearchWord(cWord) == -1)
	{
		printf("Ҫɾ���Ĵ��ڴʵ��в�����");
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
	while(nIter < nLen)		//ͳ�������Ϣ
	{
		nWordLen = GetChar((char *)(cWord+nIter));
		nID = GetInnerCode(cWord+nIter,nWordLen);
		nIter += nWordLen;
		int nSub = abs((*m_vBase)[*(nSubArray.end()-1)])+nID;
		nSubArray.push_back(nSub);
	}
	int nSize = nSubArray.size();
	
	if((*m_vBase)[nSubArray[nSize-1]] == (-1)*nSubArray[nSize-1])	//Ҫɾ���Ĵ�Ϊ����
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
	else					//��������
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
 *  Description: ����ȷ���±�ΪnSub��baseֵ
 *              
 *
 *  Parameters : nSub: ��ǰ״̬�����±�
 *				 nID: �������
 *				 nNextSub: ת��״̬�����±�
 *			    				
 *  Returns    : true: success false: failure
 *  Author     : ��С��  
 *  History    : 
 *              1.create 2004-12-5
 *				2.modify 2005-5-20
 *********************************************************************/
bool CDictionary::ReLocate(int nSub,int nID,int &nNextSub)
{
	if(nNextSub > 6767+RESERVE_ITEMS && (*m_vCheck)[nNextSub] == 0)		//��λ��
	{
		(*m_vCheck)[nNextSub] = nSub;
		m_IsDirty = true;
		return true;
	}
	else if((*m_vCheck)[nNextSub] != nSub)		//λ�÷ǿ�
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
				//��ԭ����ֵ���Ƶ���λ��
				int nOtherNextSub = abs((*m_vBase)[nSub])+nIDArray[k];
				(*m_vBase)[nOtherNextSub] = nValArray[k];

				int nThirCount = Find(*(nSubArray+k),nArray);
				
				for(int j=0;j<nThirCount;j++)
				{
					(*m_vCheck)[nArray[j]] = nOtherNextSub;
				}

				//ԭλ���ڿ�
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
 *  Description: ͳ��һ��״̬��ת��״̬����
 *              
 *  Parameters : nSub: ��ǰ״̬�����±�
 *				 nSubArray: ת��״̬�±�����
 *				 nIDArray: �����������
 *				 nValArray: ת��״̬baseֵ����
 *			    				
 *  Returns    : ת��״̬����
 *  Author     : ��С��  
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
 *  Description: ͳ��һ��״̬��ת��״̬����
 *              
 *  Parameters : nSub: ��ǰ״̬�����±�
 *				 nSubArray: ת��״̬�±�����
 *			    				
 *  Returns    : ת��״̬����
 *  Author     : ��С��  
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
 *  Description: ͳ��һ��״̬��ת��״̬����
 *              
 *  Parameters : nSub: ��ǰ״̬�����±�
 *			    				
 *  Returns    : ת��״̬����
 *  Author     : ��С��  
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
						//cStat.Format("�?%s 在词典中无法找到",buff);
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

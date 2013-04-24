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
 * Author:   ��С�� 
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



#define MaxWordSize  30				//�ʳ�
#define Maxsize 5000				//һ��״̬��ת��״̬����



class DICT_API CDictionary
{
public:
	CDictionary(void);
	~CDictionary(void);	

	vector <int>  *m_vBase;				// base����
	vector <int>  *m_vCheck;			//check����

	unsigned int  m_nDictItemCount;  //�ʵ��ܴ�����
	unsigned int m_nDictArraySize;   //�ʵ����鳤��
	unsigned int m_nWordMaxSize;    //�ʵ����ʳ���
	bool m_IsDirty;				//�����Ƿ��޸Ĺ�

	
private:

	char *m_lpBaseAddress;      //�����ļ��Ļ�ַ
	long m_lLen;                //�����ļ��ĳ��� 
	
	//���ڽ�����
	char **m_pcWordArray;
	int *m_piWordLenArray;
	
	//״̬��Ϣ�ṹ
	struct StateInfo
	{
		int nStateSub;			//��ǰ״̬��base��check�����±�
		int nNextStateCount;	//������һ״̬��
		int *pNextID;			//��һ״̬ID����
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
	
//	void DictStat(const char *pszFileName);			 //ͳ�ƴʵ���Ϣ	
	//��������
	//IsNewΪfalseʱ�����Ѿ��򿪲��Ҿ����޸ĵ����飻Ϊtrueʱ���½������ļ�
	bool SaveArray(const char *cFileName,const char *cArrayFileName,bool IsDirty);							// ���������ļ�	
	bool DictLoad(const char* pszFileName);			//����ʵ��ı��ļ�
	unsigned int GetArraySize();					//��������С
	bool DeleteWord(const char * cWord);			//ɾ������
	bool AddWord(const char *cWord);				//���Ӵ���

	int SearchWord(const char *cWord);				//���Ҵ���
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
		printf("���ܴ򿪴ʵ������ļ������������ɣ�");
		return false;
	}
	}//����ʵ�����*/
//	float TimeStat(const char *pzFileName);
	bool FreeWordArray();							//�ͷ�m_piWordArray�ȼ�������Ŀռ�
private:
	
	bool Dict_Traversal(StateInfo *m_pStateInfo,int &nStateCount,int n);			//�����ʱ�
	//���״̬��Ϣ���ṹ������
	void AddInfo(char cWord[],char *cState,int n,int nLen,StateInfo *m_pStateInfo,int &nStateCount);   
	bool Construct_Arrays(const char * pszFileName);	 //�������飬���������ļ�
	bool FindVal(StateInfo sInfo);						// ȷ��baseֵ��checkֵ
	bool FindVal(int nCurrSub,int nNextID);
	int GetStatArraySub(const char *cWord);				// ��õ�ǰ״̬��base�����е��±�
	void Sort(StateInfo *pStateInfo,int nStateCount);   // ��״̬��������
	//�ж������±��Ƿ񳬳���ǰ��С,����������������±��С
	bool ConsidSize(int nSub);				
	//��Check�����в���ֵΪnSub�����������
	int Find(int nSub,int *nSubArray,int *nIDArray,int *nValArray);		
	int Find(int nSub,int *nSubArray);
	int Find(int nSub);
	//�����ִ�
	bool AddChar(const char *cWord,int nSub);	
	//����ȷ���±�ΪnSub������Ԫ��ֵ
	bool ReLocate(int nSub,int nID,int &nNextSub);		
};

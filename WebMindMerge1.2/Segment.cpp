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
 * Filename:  Segment.cpp
 * Abstract:
 *			  interface for the CSegment class.
 * Author:   ��С��
 *          (wangxiaofei@tsinghua.org.cn)
 * Date:     2004-12-5
 *
 * Notes:	
 *                
 * 
 ****************************************************************************/
 // Segment.cpp: implementation of the CSegment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include "Segment.h"
#include "utility.h"
#include <fstream>
#include <fcntl.h>
#include <TIME.H>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define FILTER_LEN	50

CSegment::CSegment()
{
//	m_IfIn = false;
	m_cSegTxt = NULL;
	m_lpBaseAddress = NULL;
}

CSegment::~CSegment()
{
	if(m_cSegTxt!=NULL)
	{
		delete[] m_cSegTxt;
		m_cSegTxt = NULL;
	}
	if(m_lpBaseAddress!=NULL)
	{
		delete[] m_lpBaseAddress;
		m_lpBaseAddress = NULL;
	}
}

bool CSegment::Init(CDictionary *dict)
{
	if(dict == NULL)
	{
		return false;
	}
	else
	{
		m_pDict = dict;
		m_cSegTxt = NULL;
		return true;
	}
	
}

/*********************************************************************
 *
 *  Func Name  : LoadFile
 *
 *  Description: ������ִ��ļ�
 *              
 *  Parameters : pszFileName: ���ش��ִ��ļ���
 *			    				
 *  Returns    : true:success false: failure
 *  Author     : ��С��  
 *  History    : 
 *              1.create 2004-11-5
 *********************************************************************/
bool CSegment::LoadFile(const char *pszFileName)
{
	FILE *fpin = fopen(pszFileName,"r");
	if(fpin == NULL)
	{
		return false;
	}
	else
	{
		if(m_lpBaseAddress != NULL)
		{
			delete[] m_lpBaseAddress;
			m_lpBaseAddress = NULL;
		}
		fseek(fpin,0L,SEEK_END);
		m_lLen = ftell(fpin);
		m_lpBaseAddress = new char[(int)m_lLen+1];
		memset(m_lpBaseAddress,0,sizeof(char)*(int)m_lLen);
		if(m_lpBaseAddress == NULL)
		{
			return false;
		}
		
		fseek(fpin,0L,SEEK_SET);
		fread(m_lpBaseAddress,sizeof(char),(int)m_lLen,fpin);
		m_lpBaseAddress[(int)m_lLen] = '\0';
		fclose(fpin);
		return true;
	}
}

//added by kosko
//ֱ�Ӽ����ļ�����
bool CSegment::LoadContent(const char *fileContent)
{
	if(m_lpBaseAddress != NULL)
	{
		delete[] m_lpBaseAddress;
		m_lpBaseAddress = NULL;
	}
	m_lLen=strlen(fileContent);
	m_lpBaseAddress = new char[(int)m_lLen+1];
	memset(m_lpBaseAddress,0,sizeof(char)*(int)m_lLen);
	if(m_lpBaseAddress == NULL)
	{
		return false;
	}
	strcpy(m_lpBaseAddress,fileContent);
	return true;
}
/*********************************************************************
 *
 *  Func Name  : MMSegment
 *
 *  Description: �������ƥ��ִ�
 *              
 *  Parameters : nIter: �ִʽ������
 *			    				
 *  Returns    : void
 *  Author     : ��С��  
 *  History    : 
 *              1.create 2004-11-5
 *********************************************************************/
void CSegment::MMSegment(int &nIter)
{	
	int nSub = 0;
	int nID;
	int nStringLen;
	int nWordLen = 0;
	int nCheckVal = 0;
	int nBaseVal = 0;
	int nPreSub;
	int nTestWordLen = 0;
	bool IsFirst = true;
	char *lpszCur = m_lpBaseAddress;
	char *pIter = m_lpBaseAddress;	
	char *cFileSize = m_lpBaseAddress + m_lLen;
//	bool bIfSpace = false;		//�Ƿ���Ҫ�ӿո�

	while(lpszCur < cFileSize)
	{
		nStringLen = GetChar(pIter);
		nID = GetInnerCode(pIter,nStringLen);
		pIter += nStringLen;
			
		if(!IsFirst)			
		{
			if(nID >= RESERVE_ITEMS)			//���ֻ򵥴�
			{	
				nPreSub = nSub;
				nSub = abs(nBaseVal) + nID;
				if(nSub > m_nDictArraySize-1)
				{
					strncpy(m_cSegTxt+nIter,lpszCur,sizeof(char)*nWordLen);
					strncpy(m_cSegTxt+nIter+nWordLen," ",sizeof(" "));
					nIter += (nWordLen+1);
					lpszCur += nWordLen;
					pIter = lpszCur;
					IsFirst = true;
					nTestWordLen = 0;
					nWordLen = 0;
				}
				else
				{
					nCheckVal = (*m_pDict->m_vCheck)[nSub];
					nBaseVal = (*m_pDict->m_vBase)[nSub];
					if(nCheckVal != nPreSub || nBaseVal == 0)			//״̬���ܼ���
					{
						strncpy(m_cSegTxt+nIter,lpszCur,sizeof(char)*nWordLen);
						strncpy(m_cSegTxt+nIter+nWordLen," ",sizeof(" "));
						nIter += nWordLen+1;
						lpszCur += nWordLen;
						pIter = lpszCur;
						IsFirst = true;
						nWordLen = 0;
						nTestWordLen = 0;
					}
					else if(nCheckVal == nPreSub && nBaseVal < 0)    //�ɴ�
					{
						nTestWordLen += nStringLen;	
						nWordLen = nTestWordLen;
						if(nBaseVal == (-1)*nSub)		//����
						{
							strncpy(m_cSegTxt+nIter,lpszCur,sizeof(char)*nWordLen);
							strncpy(m_cSegTxt+nIter+nWordLen," ",sizeof(" "));
							nIter += nWordLen + 1;
							lpszCur += nWordLen;
							pIter = lpszCur;
							IsFirst = true;
							nTestWordLen = 0;
							nWordLen = 0;
						}
					}
					else						//���ɴʵ�״̬�ɼ���
					{
						nTestWordLen += nStringLen;	
					}
				}
			}
			else							//�Ǻ���
			{
				strncpy(m_cSegTxt+nIter,lpszCur,sizeof(char)*nWordLen);
				strncpy(m_cSegTxt+nIter+nWordLen," ",sizeof(" "));
				nIter += nWordLen+1;
				lpszCur += nWordLen;
				pIter = lpszCur;
				IsFirst = true;
				nWordLen = 0;
				nTestWordLen = 0;
			}
		}
		else		//	first character
		{
			if(nID >= RESERVE_ITEMS)								//����
			{
			/*	if(bIfSpace == true)
				{
					strncpy(m_cSegTxt+nIter," ",sizeof(" "));
					nIter++;
					bIfSpace = false;
				}//*/
				if(nID > m_nDictArraySize-1)
				{
					strncpy(m_cSegTxt+nIter,lpszCur,sizeof(char)*nStringLen);
					strncpy(m_cSegTxt+nIter+nStringLen," ",sizeof(" "));
					nIter += (nStringLen+1);
					lpszCur += nStringLen;
					pIter = lpszCur;
					nTestWordLen = 0;
					nWordLen = 0;
				}
				else 
				{
					nTestWordLen += nStringLen;
					nBaseVal = (*m_pDict->m_vBase)[nID];
					nCheckVal = (*m_pDict->m_vCheck)[nID];
					nWordLen = nTestWordLen;
					if(nBaseVal == 0 || nCheckVal != 0 || nBaseVal == (-1)*nID)	//���ֲ�Ϊ�ʵ��е����ֻ��ߵ��ֳ�����
					{
						strncpy(m_cSegTxt+nIter,lpszCur,sizeof(char)*nWordLen);
						strncpy(m_cSegTxt+nIter+nWordLen," ",sizeof(" "));
						nIter += (nWordLen + 1);
						lpszCur = pIter;
						nWordLen = nTestWordLen = 0;
					}
					else
					{
						nSub = nID;
						IsFirst = false;
					}
				}	
			}// end of if(nID >= RESERVE_ITEMS)
			else										//�Ǻ��ֻ򵥴�
			{
				nWordLen = nStringLen;
			
				nStringLen = GetChar(pIter);
				nID = GetInnerCode(pIter,nStringLen);
				while(nID<RESERVE_ITEMS &&  nID!= 5 && nID != 4 && pIter+nStringLen < cFileSize)							//�Ǻ��֡��ո񡢻س��ͱ��
				{
					nWordLen += nStringLen;
					pIter += nStringLen;
					nStringLen = GetChar(pIter);
					nID = GetInnerCode(pIter,nStringLen);
				}
				if(nWordLen <= FILTER_LEN)	
				{
					if(nID != 5)
					{
						strncpy(m_cSegTxt+nIter,lpszCur,sizeof(char)*nWordLen);
						strncpy(m_cSegTxt+nIter+nWordLen," ",sizeof(" "));
						nWordLen++;
						nIter += nWordLen;
					}
					else
					{
						nWordLen += nStringLen;
						pIter += nStringLen;
						strncpy(m_cSegTxt+nIter,lpszCur,sizeof(char)*nWordLen);
						nIter += nWordLen;
					}	
				}
			
				lpszCur = pIter;
			}// nID < RESERVE_ITEMS

		}// first character
	}// end of while(*lpszCur < cFileSize)
}

char *CSegment::TextSegment(int &iOutSize)
{
	

	int nIter = 0;
	
	if(m_pDict == NULL || m_lLen == 0 || m_lpBaseAddress == NULL)
	{
		printf("δ����ʵ������ļ���δ������ִ��ļ�\n");
		return NULL;
	}

	char *lpszCur = m_lpBaseAddress;
	m_nDictArraySize = m_pDict->m_nDictArraySize;
	if(m_nDictArraySize == 0)
	{
		printf("�ʵ����鳤��Ϊ�գ����������ɣ�");
		return NULL;
	}
	
	if(m_cSegTxt != NULL)
	{
		delete[] m_cSegTxt;
		m_cSegTxt = NULL;
	}
	m_cSegTxt = new char[m_lLen*2];
	if(m_cSegTxt == NULL)
	{
		printf("new array error\n");
		return NULL;
	}
	
			
 	MMSegment(nIter);
	m_cSegTxt[nIter] = '\0';
	iOutSize = nIter;

		
	printf("�ִ���ɣ�");
	m_lpBaseAddress = NULL;
	return m_cSegTxt;

		//else printf("�޷����ɷִ��ļ���");
}

bool CSegment::Free()
{
	if(m_cSegTxt!=NULL)
	{
		delete[] m_cSegTxt;
		m_cSegTxt = NULL;
	}
	return true;
}
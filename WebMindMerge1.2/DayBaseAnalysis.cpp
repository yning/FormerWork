#include "StdAfx.h"
#include <iostream>
//#include <map>
#include <string>
#include <fstream>
#include<algorithm>
using namespace std;

#include "DayBaseAnalysis.h"
#include "Utility.h"
#include "hightime.h"
#include "Markup.h"
#define  SearchEngineLen 24
#define  SocailNetworkLen 12
map<CString,int> DayBaseAnalysis::dic_SearchEngineCount;//记录使用搜索引擎的次数(日期->次数)
map<CString,int> DayBaseAnalysis::dic_SocailNetworkCount;//记录使用社交网络的次数(日期-次数)

char* DayBaseAnalysis::SearchEngines[]={"www.baidu.com","www.soso.com","www.google.com","www.google.ca",
"www.google.ch","www.google.com.au","www.google.co.jp","www.google.ro","www.google.de",
"search.excite.com","search.weather.yahoo.com","www.hotbot.lycos.com","web.ask.com",
"www.google.com.hk","www.sogou.com","www.youdao.com","www.google.co.uk","www.google.co.th","www.google.ie",
"search.yahoo.com","google.yahoo.com","auto.search.msn.com","search.atomz.com","www.altavista.com"
};	
char* DayBaseAnalysis::SocailNetworks[]={"www.renren.com","weibo.com","t.qq.com","www.kaixin001.com",
"www.snsyx.com","bai.sohu.com","qzone.qq.com","tieba.baidu.com","www.tianya.cn","www.mop.com",
"www.newsmth.net","kyxk.net"};

bool cmp(char* p1,char* p2)
{
	return	strcmp(p1,p2)<0;
}

void DayBaseAnalysis::Init()
{
	sort(SearchEngines,SearchEngines+SearchEngineLen,cmp);
	sort(SocailNetworks,SocailNetworks+SocailNetworkLen,cmp);
}

void DayBaseAnalysis::AnalysisUrl(CString strUrl)
{
	//sort(SocailNetworks);
	typedef pair<CString,int> m_pair;
	CHighTime cht=CHighTime::GetPresentTime();
	CTime ct(cht.GetYear(),cht.GetMonth(),cht.GetDay(),cht.GetHour(),cht.GetMinute(),cht.GetSecond());
	CString currentTime=Utility::ConvertCTime_DateToCString(ct);	
	//char *stru=(LPSTR)(LPCTSTR)strUrl;
	//if (Utility::BinarySearch(SearchEngines,SearchEngineLen,stru))
	if (IsSearchEngine(strUrl))
	{
			if (dic_SearchEngineCount.count(currentTime)==0)
				dic_SearchEngineCount.insert(m_pair(currentTime,1));
			else
				dic_SearchEngineCount[currentTime]++;
	}
	else if (IsSocialNetWork(strUrl))
	{
		if (dic_SocailNetworkCount.count(currentTime)==0)
		{
			dic_SocailNetworkCount.insert(m_pair(currentTime,1));
			int tt=dic_SocailNetworkCount[currentTime];
		}
		else
			dic_SocailNetworkCount[currentTime]++;
	}
}

void DayBaseAnalysis::LoadTodayHistory(CString strFileName)
{
	typedef pair<CString,int> m_pair;
	CHighTime cht=CHighTime::GetPresentTime();
	CTime ct(cht.GetYear(),cht.GetMonth(),cht.GetDay(),cht.GetHour(),cht.GetMinute(),cht.GetSecond());
	CString currentTime=Utility::ConvertCTime_DateToCString(ct);	
	CString buff =Utility::LoadFile(strFileName); 
	CMarkup xml;
	xml.Load(strFileName);
	xml.IntoElem();
	if (xml.FindElem(_T("Record")))
	{
		CString date=xml.GetAttrib(_T("Date"));
		if (date.Compare(currentTime)==0)
		{
			xml.FindChildElem(_T("SearchEngine"));
			xml.IntoElem();
			CString dn=xml.GetData();//获取元素值
			int cnt=atoi((LPSTR)(LPCTSTR)dn);
			dic_SearchEngineCount[currentTime]=cnt;
			xml.OutOfElem();
			xml.FindChildElem(_T("SocialNetwork"));
			xml.IntoElem();
			CString dd=xml.GetData();//获取元素值
			int cn=atoi((LPSTR)(LPCTSTR)dd);
			dic_SocailNetworkCount[currentTime]=cn;
			xml.OutOfElem();
		}
	}
	xml.OutOfElem();
}

void DayBaseAnalysis::SaveTodayHistory(CString strFileName)
{
	typedef pair<CString,int> m_pair;
	CHighTime cht=CHighTime::GetPresentTime();
	CTime ct(cht.GetYear(),cht.GetMonth(),cht.GetDay(),cht.GetHour(),cht.GetMonth(),cht.GetSecond());
	CString currentTime=Utility::ConvertCTime_DateToCString(ct);	
	CMarkup xml;
	xml.Load(strFileName);//这样的写法是对的
	xml.IntoElem();
	if (xml.FindElem(_T("Record")))
	{//更新xml
		CString date=xml.GetAttrib(_T("Date"));
		if (currentTime.Compare(date)==0)
		{
			xml.FindChildElem(_T("SearchEngine"));
			xml.IntoElem();
			CString temp;
			temp.Format("%d",dic_SearchEngineCount[currentTime]);
			xml.SetData(temp);
			xml.OutOfElem();
			xml.FindChildElem(_T("SocialNetwork"));
			xml.IntoElem();
			int tt=dic_SocailNetworkCount[currentTime];
			temp.Format("%d",dic_SocailNetworkCount[currentTime]);
			xml.SetData(temp);
			xml.OutOfElem();
		}
		else
		{
			//写xml
			xml.AddElem(_T("Record"));
			xml.AddAttrib(_T("Date"),currentTime);
			xml.IntoElem();
			xml.AddElem(_T("SearchEngine"),Utility::ConvertIntToCString(dic_SearchEngineCount[currentTime]));
			xml.OutOfElem();
			xml.IntoElem();
			xml.AddElem(_T("SocialNetwork"),Utility::ConvertIntToCString(dic_SocailNetworkCount[currentTime]));
			xml.OutOfElem();
		}
	}
	else
	{//写xml
		xml.AddElem(_T("Record"));
		xml.AddAttrib(_T("Date"),currentTime);
		xml.IntoElem();
		xml.AddElem(_T("SearchEngine"),Utility::ConvertIntToCString(dic_SearchEngineCount[currentTime]));
		xml.OutOfElem();
		xml.IntoElem();
		xml.AddElem(_T("SocialNetwork"),Utility::ConvertIntToCString(dic_SocailNetworkCount[currentTime]));
		xml.OutOfElem();
	}
	xml.OutOfElem();
	xml.Save(strFileName);
}

bool DayBaseAnalysis::IsSearchEngine(CString strUrl)
{
	char *stru=(LPSTR)(LPCTSTR)strUrl;
	if (Utility::BinarySearch(SearchEngines,SearchEngineLen,stru))
		return true;
	return false;
}

bool DayBaseAnalysis::IsSocialNetWork(CString strUrl)
{
	char *stru=(LPSTR)(LPCTSTR)strUrl;
	if (Utility::BinarySearch(SocailNetworks,SocailNetworkLen,stru))
		return true;
	return false;
}
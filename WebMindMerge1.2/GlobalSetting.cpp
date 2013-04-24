///////////////////////////////////////////////////////////////
// GlobalSetting.cpp: implementation of the CGlobalSetting class.
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalSetting.h"

#include "Shlwapi.h"
#include "IniFile.h"
#include "Utility.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobalSetting::CGlobalSetting()
{
	defaultValue();
}

CGlobalSetting::~CGlobalSetting()
{
}

void CGlobalSetting::defaultValue()
{
	m_bHistory = false;
	m_bAnnotation = false;
	m_bRecomm = false;

	m_wordMapping.Empty();

	m_strUserID.Empty();

	m_numUpload = -1;
}



// load the settings.
bool CGlobalSetting::loadSettings(bool m_bOnLine)
{
	CString strIniFile;
	CString debugPath;   
    GetModuleFileName(NULL,debugPath.GetBuffer(MAX_PATH),MAX_PATH);   
    debugPath.ReleaseBuffer();
	debugPath.Replace("WebMind.exe","");
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
	//char temppath[50];
	//GetCurrentDirectory(50,temppath);
    strIniFile.Format(debugPath+"%s"+"records.ini",m_appName);//%s\\,temppath
   //int im=m_appName.IsEmpty();
	//int ipath= !PathFileExists(strIniFile);
	// if the app name is empty or we cannot find the
	// config file in current folder, then use the default
	// values; otherwise, read in the configuration from
	// the config file.	
    //	cout<<im<<ipath;
	if(m_appName.IsEmpty()|| !PathFileExists(strIniFile) )
	{
		defaultValue();
		return false;
	}

	CIniFile iniFile;
	CString location;
	CString key;
	CString strBuffer;
	int num;
	CString m_rootPath;

	iniFile.SetPath(strIniFile);
	bool testfor=iniFile.ReadFile();

	location.Empty();
	location = iniFile.GetValue("General", "location");
	location.TrimLeft();
	location.TrimRight();

	m_location.Empty();
	m_location = location;
    start = 0;
    length=m_location.GetLength();
    for(int j=0;j<length;j++)
	{
		if (start>=length)break;
    	start = m_location.Find('\\', start);
     m_location.Insert(start, '\\');
     start++;
	 start++;
	} 
	if(m_location.IsEmpty())
	{
		// get the current directory
		TCHAR lpszCurDir[MAX_PATH+1];
		CString lpcurdir;
		GetModuleFileName(NULL,lpcurdir.GetBuffer(MAX_PATH),MAX_PATH);   
       lpcurdir.ReleaseBuffer();
    	lpcurdir.Replace("WebMind.exe","");
		strcpy(lpszCurDir,lpcurdir.GetBuffer(0));
		lpcurdir.ReleaseBuffer();
		//GetCurrentDirectory(MAX_PATH+1, lpszCurDir);
		lpszCurDir[MAX_PATH+1] = '\0'; 
		m_location = lpszCurDir;
		start = 0;
	    length=m_location.GetLength();
       for(int k=0;k<length;k++)
	   {
		  if (start>=length)break;
        	start = m_location.Find('\\', start);
         m_location.Insert(start, '\\');
       start++;
	    start++;
	   } 

	}

	m_bHistory = true;
	m_bAnnotation = true;
	m_bRecomm = true;

	if(m_location.IsEmpty())
		m_rootPath.Empty();
	else
		m_rootPath = m_location;// +  _T("\\"); 

	m_dataPath = m_rootPath + _T("Data");

	m_pageSource = m_rootPath + iniFile.GetValue("History", "pageSource");
	m_pageCache = m_rootPath + iniFile.GetValue("History", "pageCache");
	m_pageView = m_rootPath + iniFile.GetValue("History", "pageView");
	m_urlCode = m_rootPath + iniFile.GetValue("History", "urlcode");
	m_wordMapping = m_rootPath + iniFile.GetValue("History", "wordmapping");

	if(!PathFileExists(m_urlCode))
		m_bUrlCode = false;
	else
		m_bUrlCode = true;

	// read in the resource information
	m_wordFreq = m_rootPath + iniFile.GetValue("Resource", "wordfreq");

	// read in model file
	m_modelAtt = m_rootPath + iniFile.GetValue("Model", "att");
	m_modelWord = m_rootPath + iniFile.GetValue("Model", "word");
	m_modelRelevant = m_rootPath + iniFile.GetValue("Model", "relevant");
	m_modelQuery = m_rootPath + iniFile.GetValue("Model", "query");
	m_modelPara = m_rootPath + iniFile.GetValue("Model", "para");


	// read in the upload information
	key = iniFile.GetValue("Upload", "latest");
	num = iniFile.GetValueI("Upload", "number");
	m_strUserID = iniFile.GetValue("Upload", "userid");
/*	if(key.IsEmpty())
	{
		// if this is the first time
		m_lastUpload = CHighTime::GetPresentTime();
		m_numUpload = 0;

		key = m_lastUpload.Format("%m/%d/%Y:%H:%M:%S");
		iniFile.SetValue("Upload", "latest", key);
		iniFile.SetValueI("Upload", "number", m_numUpload);
		iniFile.WriteFile();
	}
	else
	{
		m_lastUpload.ParseDateTime(key);
		m_numUpload = num;

		if(num == 0)
		{
			iniFile.SetValueI("Upload", "number", m_numUpload);
			iniFile.WriteFile();
		}
	}
*/
	// checking integrity
	CheckIntegrity();

	if(m_bHistory && m_bRecomm)
		return true;

	return false;
}

void CGlobalSetting::saveSettings()
{
	CString strIniFile;
	strIniFile = m_location + _T("\\") + m_appName + _T("records.ini");

	// if the app name is empty or we cannot find the
	// config file in current folder, then use the default
	// values; otherwise, read in the configuration from
	// the config file.
	if(m_appName.IsEmpty() || !PathFileExists(strIniFile))
		return;

	CIniFile iniFile;
	CString key;

	iniFile.SetPath(strIniFile);
	iniFile.ReadFile();

	key = m_lastUpload.Format("%m/%d/%Y:%H:%M:%S");
	iniFile.SetValue("Upload", "latest", key);
	iniFile.SetValueI("Upload", "number", m_numUpload);
	iniFile.SetValue("Upload", "userid", m_strUserID);
	iniFile.WriteFile();
}

// Checking Iintegrity
void CGlobalSetting::CheckIntegrity()
{

	if(!m_bHistory)
	{
		m_bRecomm = false;
		m_bAnnotation = false;
	}

	if(!PathFileExists(m_urlCode))
		m_bUrlCode = false;
	else
		m_bUrlCode = true;

	if(m_bHistory)
	{
		CString strPathData;

		if(m_location.IsEmpty())
			strPathData.Format("Data");
		else
			strPathData.Format("%s\\Data", m_location);


		if(!PathFileExists(strPathData))
		{
			CreateDirectory(strPathData, NULL);

			CreateDirectory(m_pageSource, NULL);
			CreateDirectory(m_pageCache, NULL);
		}
	}

	if(m_bHistory && m_bRecomm)
	{
		if( !PathFileExists(m_wordFreq) )
			m_bRecomm = false;

		// if there is no model file, then we can do nothing for recommendation
		if( PathFileExists(m_modelWord) && PathFileExists(m_modelRelevant) && PathFileExists(m_modelQuery) && PathFileExists(m_modelAtt) )
			m_bRecomm = true;
		else
			m_bRecomm = false;
	}
}

///////////////////////////////////////////////////////////////
// GlobalSetting.h: interface for the CGlobalSetting class.
//
///////////////////////////////////////////////////////////////
#include "HighTime.h"

#if !defined(AFX_GLOBALSETTING_H__BB49D1CD_7520_4037_B1DD_2FD88BC91E7F__INCLUDED_)
#define AFX_GLOBALSETTING_H__BB49D1CD_7520_4037_B1DD_2FD88BC91E7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGlobalSetting  
{
public:
	CGlobalSetting();
	virtual ~CGlobalSetting();

public:
	bool loadSettings(bool m_bOnLine = true);
	void saveSettings();

	void CheckIntegrity();

public:

	CString m_location;

	CString m_appName;

	// last time to upload the log files, and the number
	// upload so far
	CHighTime m_lastUpload;
	CString m_strUserID;
	int m_numUpload;

	// if m_bHistory is set, then check whether there is 
	// the folder to hold the downloaded files and history file,
	// if no such folder, we then create it.
	bool m_bHistory;			// whether record the history

	// if m_bAnnotation is set, then check whether there is 
	// the folder to hold the annotated history,
	// if no such folder, we then create it.
	bool m_bAnnotation;			// whether annotation provided

	// if we cannot find model file, then it will be set as
	// false
	bool m_bRecomm;				// whether recommendation provided

	CString m_dataPath;

	CString m_pageSource;
	CString m_pageCache;
	CString m_pageView;

	CString m_wordMapping;

	CString m_urlCode;
	bool m_bUrlCode;

	CString m_wordFreq;

	// model used for recommendation
	CString m_modelAtt;
	CString m_modelWord;
	CString m_modelRelevant;
	CString m_modelQuery;
	CString m_modelPara;


protected:
	void defaultValue();
};

#endif // !defined(AFX_GLOBALSETTING_H__BB49D1CD_7520_4037_B1DD_2FD88BC91E7F__INCLUDED_)

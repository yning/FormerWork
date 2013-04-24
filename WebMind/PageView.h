///////////////////////////////////////////////////////////////
// PageView.h: interface for the CPageView class.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_PAGEVIEW_H__A9FA5DF2_63F8_49E0_8AF8_98129D612569__INCLUDED_)
#define AFX_PAGEVIEW_H__A9FA5DF2_63F8_49E0_8AF8_98129D612569__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LilacObject.h"
#include "List.h"
#include "Utility.h"

// each frame within a page
class CPageFrame : public CLilacObject
{
public:
    CPageFrame();
    ~CPageFrame();

    CLilacObject *clone();
    int compare(CLilacObject *obj);

    void dump();

    int serialize (CArchive& pArchive);

public:

	// frame's name
    CString m_name;

	// target
    CString m_target;

	// its url
    CString m_src;

	// base
    CString m_base;

	// page ID to retrieve its page cache
    int pageno;
};


// page view information
class CPageView : public CLilacObject
{
public:
    CPageView();
    ~CPageView();

    CLilacObject *clone();
    int compare(CLilacObject *obj);

	void addFrame(CString strName, CString strSrc, CString strTarget);

	void setURL(const CString& m_value);
	CString getURL();

	void setBaseURL(const CString& m_value);
	CString getBaseURL();

	void setTarget(const CString& m_value);
	CString getTarget();

    void setDest(bool m_value);
	bool getDest();

    void setPageno(int value);
    int getPageno();


    void buildURL(const CString &m_currentURL, const CString &m_baseURL, const CString &m_href, CString& result);
    void buildFrameURL();

    void modifyFrame(CString m_url, CString m_base, CString m_target, int pageno);
    void copyFrames(CPageView *pView);

    int locateTarget(CString strTarget);

    void dump();

    void load(const char *m_fold);
    void save(const char *m_fold);

    int serialize (CArchive& pArchive);

public:
	// the list of all frame set of this page
	// list of CPageFrame
    List *m_framepages;

	// url itself
    CString m_url;

	// the action that leads to this page view
	int m_action;

	// its base url and target
    CString m_base;
    CString m_target;

	// page ID
    int pageno;

	// whether it is an IC-Page
    bool destination;

	// whether it is a search result page
    bool searchQuery;

};



#endif // !defined(AFX_PAGEVIEW_H__A9FA5DF2_63F8_49E0_8AF8_98129D612569__INCLUDED_)

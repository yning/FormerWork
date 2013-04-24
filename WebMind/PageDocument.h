///////////////////////////////////////////////////////////////
// PageDocument.h: interface for the CPageDocument class.
///////////////////////////////////////////////////////////////

#if !defined(AFX_PAGEDOCUMENT_H__6C24F4B0_67F2_4BC8_B6F8_FB7FB0BD5CC7__INCLUDED_)
#define AFX_PAGEDOCUMENT_H__6C24F4B0_67F2_4BC8_B6F8_FB7FB0BD5CC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LilacObject.h"
#include "List.h"
#include "Hash.h"

#include <afxtempl.h>
#include <mshtml.h>
#include <comdef.h>

#include "ICTCLAS50.h"
#include <stdio.h>
#include <string.h>

class CPageRepository;

class CHTMLNode
{
public:
	CHTMLNode * FindMeta();
	CHTMLNode * FindTitle();
	CHTMLNode();
	~CHTMLNode();

    void ReleaseChild();
	void ReleaseElement();

	void InsertNodeAtStart(CHTMLNode *node);
	void InsertNodeAtEnd( CHTMLNode *node);
	void InsertNodeAsParent(CHTMLNode *node);
	void InsertNodeBeforeElement(CHTMLNode *node);
	void InsertNodeAfterElement(CHTMLNode *node);

    CHTMLNode *getContent()			{ return content; }
    CHTMLNode *getNext()			{ return next; }
    CHTMLNode *getPrev()			{ return prev; }
    CHTMLNode *getParent()			{ return parent; }
    CHTMLNode *getLast()			{ return last; }

	CHTMLNode * getFirst(const char *tag_name, int order = 1);
	CHTMLNode * getLast(const char *tag_name);

	CHTMLNode * FindElement(long sourceIndex);

	CHTMLNode * FindBody();

	bool getText(CString& strText);
	bool getName(CString& strTag);

	bool getAttrVal(const char *attr, CString& strValue);
	bool getID(CString& strID);
	long getSourceIndex();

	bool getColor(CString& strValue);
	bool getBGColor(CString& strValue);

	bool getFontFamily(CString& strValue);
	bool getFontSize(CString& strValue);
	bool getFontStyle(CString& strValue);
	bool getFontWeight(CString& strValue);

	bool getClientHeight(long *height);
	bool getClientWidth(long *width);

	bool getOffsetHeight(long *height);
	bool getOffsetWidth(long *width);

	void setElement(IHTMLElement *newElement, IHTMLElement2 *pElem2 = NULL)
	{
		pElement = newElement;
		pElement2 = pElem2;
		if(pElem2)
			pElem2->get_currentStyle(&scpStyle);//终极问题处 //个人感觉应该是pElem2不合格
		//这里,pElem2的IDispatch是个IUnknow，也就是没有实质内容，故而肯定报错
	}

	IHTMLElement *getElement()
	{
		return pElement;
	}


protected:

	IHTMLElement *pElement;

	IHTMLElement2 *pElement2;
	IHTMLCurrentStyle *scpStyle;

    CHTMLNode *parent;
    CHTMLNode *next;         //  The next sibling node
    CHTMLNode *prev;         //  The previous sibling node
    CHTMLNode *content;      //  The first node of my children
    CHTMLNode *last;         //  The last node of my children
};


// the attribute and value pair of any tag
class CTagAttrVal : public CObject
{
public:
	CTagAttrVal()
	{
		m_strAttr.Empty();
		m_strVal.Empty();
	}

	CTagAttrVal(const CString &strAttr, const CString &strVal)
	{
		m_strAttr = strAttr;
		m_strVal = strVal;
	}

	CTagAttrVal(const CTagAttrVal& src)
	{
		m_strAttr = src.m_strAttr;
		m_strVal = src.m_strVal;
	}

	virtual ~CTagAttrVal()
	{
		m_strAttr.Empty();
		m_strVal.Empty();
	}

	void operator =(const CTagAttrVal& src)
	{
		m_strAttr = src.m_strAttr;
		m_strVal = src.m_strVal;
	}

public:
	CString m_strAttr;
	CString m_strVal;
};


class CPageTagNode
{
public:
	void removeChild();
	CPageTagNode();
	virtual ~CPageTagNode();

    void clear();
   
	void InsertNodeAtStart(CPageTagNode *node);
	void InsertNodeAtEnd( CPageTagNode *node);
	void InsertNodeAsParent(CPageTagNode *node);
	void InsertNodeBeforeElement(CPageTagNode *node);
	void InsertNodeAfterElement(CPageTagNode *node);

    CPageTagNode *getContent()		{ return content; }
    CPageTagNode *getNext()			{ return next; }
    CPageTagNode *getPrev()			{ return prev; }
    CPageTagNode *getParent()		{ return parent; }
    CPageTagNode *getLast()			{ return last; }

	CPageTagNode * getFirst(const char *tag_name, int order = 1);
	CPageTagNode * getLast(const char *tag_name);

    CPageTagNode *Find(int nid);    // find the tag node whose ID is nid

	CPageTagNode * FindMeta();
	CPageTagNode * FindTitle();
	CPageTagNode * FindBody();

    bool checkForResultsDiv(CPageTagNode *node);//add by ny
    bool checkForResultsItem(CPageTagNode *node);//add by ny
    CPageTagNode* findSearch(CPageTagNode * tagNode);//add by ny
	CPageTagNode* findSearchItem(CPageTagNode * tagNode);//add by ny

	void addAttrVal(const char *attr, const CString &strValue)
	{
		CTagAttrVal pTag(attr, strValue);
		m_attributeValues.Add(pTag);
//		m_attr.Add(attr);
//		m_value.Add(strValue);
	}
	bool getAttrVal(const char *attr, CString& strValue);

	void setText(const CString &strText){m_innerText=strText;}
	bool getText(CString& strText)
    {
        strText = m_innerText;
        return true;
    }

	void setName(const CString &strName)    { m_tagName = strName; }
	bool getName(CString& strName)
    {
        strName = m_tagName;
        return true;
    }

	void setColor(const CString &strColor)	{	m_strColor = strColor;	}
	CString& getColor()						{	return m_strColor;		}

	void setBGColor(const CString &strColor)	{	m_strBGColor = strColor;	}
	CString& getBGColor()						{	return m_strBGColor;		}

	void setFontFamily(const CString &strFont)	{	m_strFontFamily = strFont;	}
	CString& getFontFamily()						{	return m_strFontFamily;		}

	void setFontSize(const CString &strFont)	{	m_strFontSize = strFont;	}
	CString& getFontSize()						{	return m_strFontSize;		}

	void setFontStyle(const CString &strFont)	{	m_strFontStyle = strFont;	}
	CString& getFontStyle()						{	return m_strFontStyle;		}

	void setFontWeight(const CString &strFont)	{	m_strFontWeight = strFont;	}
	CString& getFontWeight()						{	return m_strFontWeight;		}

	void setID(int nid)	{ id = nid; }
	int getID()			{ return id; }

	void setPID(int nid)	{ pid = nid; }
	int getPID()			{ return pid; }

	void setCount(int nid)	{ count = nid; }
	int getCount()
    {
        if(count < 0)
            countSubNode();
        return count;
    }

    void serialize (CArchive& pArchive);

    void countSubNode();

public:
	int m_numWords;			// number of words of this element
	Hash *m_hWords;			// hash table for each word and its occurence of this element
	CString m_tagName;//by ny  change from protected to public
	CString m_innerText;//by ny  change from protected to public
    CString m_id;// id of the html tag; add by ny
protected:



	int id;					// ID for this node
	int pid;				// parent ID

    int count;              // number of nodes under this node

	CArray<CTagAttrVal, CTagAttrVal&> m_attributeValues;

    CPageTagNode *parent;
    CPageTagNode *next;         //  The next sibling node
    CPageTagNode *prev;         //  The previous sibling node
    CPageTagNode *content;      //  The first node of my children
    CPageTagNode *last;         //  The last node of my children

protected:

	// these features are used for layout
	CString m_strColor;		// the fore-ground color
	CString m_strBGColor;	// background color

	CString m_strFontFamily;	// font family
	CString m_strFontSize;		// font size
	CString m_strFontStyle;		// font style
	CString m_strFontWeight;	// font weight


//////////////////////////////////////////////////////////////////////////////////////////
// for VIPS implementation
//
// To get the VIPS block, you should call
// checkNode();
//////////////////////////////////////////////////////////////////////////////////////////

public:

	void checkNode()
	{
		checkInlineValidText();
		checkNodeVirtualText();
	}

	bool isInlineNode()							{	return m_bInlineNode;			}
	void InlineNode(bool bValue = true)			{	m_bInlineNode = bValue;			}

	bool isValidNode()							{	return m_bValidNode;			}
	void ValidNode(bool bValue = true)			{	m_bValidNode = bValue;			}

	bool isTextNode()							{	return m_bTextNode;				}
	void TextNode(bool bValue = true)			{	m_bTextNode = bValue;			}

	bool isVirtualTextNode()					{	return m_bVirtualTextNode;		}
	void VirtualTextNode(bool bValue = true)	{	m_bVirtualTextNode = bValue;	}

	void setClientHeight(long lValue)			{	m_lClientHeight = lValue;		}
	long getClientHeight()						{	return m_lClientHeight;			}

	void setOffsetHeight(long lValue)			{	m_lOffsetHeight = lValue;		}
	long getOffsetHeight()						{	return m_lOffsetHeight;			}

	void setClientWidth(long lValue)			{	m_lClientWidth = lValue;		}
	long getClientWidth()						{	return m_lClientWidth;			}

	void setOffsetWidth(long lValue)			{	m_lOffsetWidth = lValue;		}
	long getOffsetWidth()						{	return m_lOffsetWidth;			}

	void setDOCValue(long lValue)				{	m_lDOCValue = lValue;			}
	long getDOCValue()							{	return m_lDOCValue;				}

	void setNodeStatus(int iValue)				{	m_iNodeStatus = iValue;			}
	int getNodeStatus()							{	return m_iNodeStatus;			}

	void setDividable(bool bValue=true)			{	m_bDividable = bValue;			}
	bool IsDividable()							{	return m_bDividable;			}

protected:

	bool isInlineTag(const char *szTag);

	// check whether a inline/valid/text text tag
	void checkInlineValidText();
	// check whether a virtual text node
	void checkNodeVirtualText();

	// the height/width of the object without taking into 
	// account any margin, border, scroll bar, or padding 
	// that might have been applied to the object.
	long m_lClientHeight;
	long m_lClientWidth;

	long m_lOffsetHeight;
	long m_lOffsetWidth;

	//these features are used for segamentation
	bool m_bInlineNode;

	bool m_bValidNode;
	bool m_bTextNode;
	bool m_bVirtualTextNode;

	// Doc Value;
	long m_lDOCValue;

	// node Status
	// cut, not in pool, in pool
	int m_iNodeStatus;

	// dividable
	bool m_bDividable;

};




class CPageDocument;

class CFrame : public CLilacObject  
{
public:
	CFrame()
	{
		m_name.Empty();
		m_target.Empty();
		m_src.Empty();
//		m_childFrame = NULL;
	}

	~CFrame()
	{
	}

    void serialize (CArchive& pArchive)
	{
		int len;

        if (pArchive.IsStoring())
        {
            pArchive << m_src.GetLength();
            if(m_src.GetLength() > 0)
                pArchive << m_src;

            pArchive << m_name.GetLength();
            if(m_name.GetLength() > 0)
                pArchive << m_name;

            pArchive << m_target.GetLength();
            if(m_target.GetLength() > 0)
                pArchive << m_target;
		}
		else
		{
			m_src.Empty();
            pArchive >> len;
            if(len)
                pArchive >> m_src;

			m_name.Empty();
            pArchive >> len;
            if(len)
                pArchive >> m_name;

			m_target.Empty();
            pArchive >> len;
            if(len)
                pArchive >> m_target;
		}
	}


public:
	void getName(CString& name)				{	name = m_name;	}
	void setName(const CString& name)		{	m_name = name;	}

	void getTarget(CString& target)				{	target = m_target;	}
	void setTarget(const CString& target)		{	m_target = target;	}

	void getSrc(CString& src)				{	src = m_src;	}
	void setSrc(const CString& src)			{	m_src = src;	}

public:
	CLilacObject * clone()
	{
		CFrame *pFrame = (CFrame *) new CFrame();

		pFrame->setName(m_name);
		pFrame->setTarget(m_target);
		pFrame->setSrc(m_src);

		return pFrame;
	}

private:
	CString m_name;
	CString m_target;

	CString m_src;
};



/*

  This class is the intermediate format of the web page
  which is in tree structure. There are several ways to
  creat the tree structure of any given web page.

  Usage :

  1. setPageNo(pageID) -> Load()
     at first, it will check whether there is serialized file
	 of this web page. If not, it will parse the original HTML
	 file, and after that, save the parsed results into local file.
	 Otherwise, just read in the tree structure from local file.

  2. Load(localFile)
     read in the structure of the web page from local file.

  3. ParseHTML(htmlFile)
     load in the HTML of the web page, parse it.

  4. ParseHTML(IDispatch*)
     get the tree strcuture from the HTMLDocument.

*/

class CPageDocument : public CLilacObject  
{
public:
	CPageDocument();
	virtual ~CPageDocument();


	bool loadOffLine(long pageID, const CString &m_sourceFile, const CString &m_cacheFile);
	void loadCache(const char *m_contentFile);
	void loadHTML(const CString& m_htmlfile);

	void loadOnLine(IDispatch *pDisp, bool online = true);
	void loadOnLineDocument(IHTMLDocument2 *pDoc);

	void loadByContent(const CString& m_content);

	void serialize (CArchive& pArchive);

public:
	int key()
	{
		return m_pageID;
	}

	int compare(CLilacObject *obj)
	{
		CPageDocument *pEntry = (CPageDocument *)obj;
		return (m_pageID == pEntry->getPageID()) ? 0 : 1;
	}

public:
	int getPageID()								{	return m_pageID;	}
	void setPageID(int value)					{	m_pageID = value;	}

	void getURL(CString& url)					{	url = m_url;	}
	void setURL(const CString& url)				{	m_url = url;	}

	void getLastModified(CString& date)			{	date = m_lastModified;	}
	void setLastModified(const CString& date)	{	m_lastModified = date;	}

	void getBaseURL(CString& url)				{	url = m_baseURL;	}
	void setBaseURL(const CString& url)			{	m_baseURL = url;	}

	void getBaseTarget(CString& target)			{	target = m_baseTarget;	}
	void setBaseTarget(const CString& target)	{	m_baseTarget = target;	}

	CPageTagNode *getDocument()					{	return m_document;	}
	List *getFrames()							{	return m_frames;	}

	void setRepository(CPageRepository *pRepository)	{	m_repository = pRepository;	}
	CPageRepository *getRepository()					{	return m_repository;	}

public:
	CString m_clrBackGround;

protected:

	void saveTagNode(CPageTagNode *pNode, CArchive& pArchive);
	void insertTagNode(CPageTagNode *pNode);


	void getContentTree(IHTMLDocument2 *pDoc);
	void SaveAsTagNode(CHTMLNode *phNode, CPageTagNode *ptNode);

	long getSourceIndex(IHTMLElement *pElement);

private:
	long m_pageID;

	CString m_url;
	CString m_lastModified;

	CPageTagNode *m_document;
	int m_tagCnt;

	CString m_baseURL;
	CString m_baseTarget;

	List *m_frames;

	CPageRepository *m_repository;
};

#endif // !defined(AFX_PAGEDOCUMENT_H__6C24F4B0_67F2_4BC8_B6F8_FB7FB0BD5CC7__INCLUDED_)



///////////////////////////////////////////////////////////////
// PageDocument.cpp: implementation of the CPageDocument class.
//
//
///////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "PageDocument.h"
#include "Utility.h"
#include "IniFile.h"
#include "PageRepository.h"

#include "ICTCLAS50.h"
#include <stdio.h>
#include <string.h>

//#include "Dictionary.h"
//#include "Segment.h"

//#pragma comment(lib,"Dict.lib")
//////////////////////////////////////////////////////////////////////////
//added by kosko
#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")
//////////////////////////////////////////////////////////////////////////

#ifdef   _DEBUG 
#define  new   DEBUG_NEW 
#undef   THIS_FILE 
static char THIS_FILE[] = __FILE__; 
#endif

const IID IID_IHTMLElement2={0x3050f434,0x98b5,0x11cf,{0xbb,0x82,0x00,0xaa,0x00,0xbd,0xce,0x0b}};

//////////////////////////////////////////////////////////////////////
// CHTMLNode Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHTMLNode::CHTMLNode()
{
	pElement = NULL;

	pElement2 = NULL;
	scpStyle = NULL;

    parent = (CHTMLNode *)NULL;
    next = (CHTMLNode *)NULL;
    prev = (CHTMLNode *)NULL;
    content = (CHTMLNode *)NULL;
    last = (CHTMLNode *)NULL;
}

CHTMLNode::~CHTMLNode()
{
    /*
    CHTMLNode *pCurr, *pPrev;

    //  Remove my content.
    for (pCurr = last; NULL != pCurr; pCurr = pPrev )
    {
        pPrev = pCurr->prev;
        if (pCurr == content)
            content = NULL;
        delete pCurr;
    }
    */
}

void CHTMLNode::ReleaseChild()
{
    CHTMLNode *pCurr, *pPrev;

    //  Remove my content.
    for (pCurr = last; NULL != pCurr; pCurr = pPrev )
    {
        pPrev = pCurr->prev;
        if (pCurr == content)
            content = NULL;
        pCurr->ReleaseChild();
        delete pCurr;
    }
}


void CHTMLNode::ReleaseElement()
{
    CHTMLNode *pCurr;

	if(pElement)
	{
		pElement->Release();
		pElement = NULL;
	}

	if(pElement2)
	{
		pElement2->Release();
		pElement2 = NULL;
	}

	if(scpStyle)
	{
		scpStyle->Release();
		scpStyle = NULL;
	}

    //  Remove element in each child node.
    for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
		pCurr->ReleaseElement();

}


bool CHTMLNode::getName(CString& strTag)
{
    strTag.Empty();

	if(pElement)
	{
		BSTR bstr;
		if(pElement->get_tagName(&bstr) == S_OK)
		{
			strTag = bstr;
            SysFreeString(bstr);
			return true;
		}
	}

	return false;
}

bool CHTMLNode::getID(CString& strID)
{
    strID.Empty();

	if(pElement)
	{
		BSTR bstr;
		if(pElement->get_id(&bstr) == S_OK)
		{
			strID = bstr;
            SysFreeString(bstr);
			return true;
		}
	}

	return false;
}

long CHTMLNode::getSourceIndex()
{
	if(pElement)
	{
		long sourceIndex;
		if(pElement->get_sourceIndex(&sourceIndex) == S_OK)
			return sourceIndex;
	}

	return -1L;
}


bool CHTMLNode::getText(CString& strText)
{
    strText.Empty();

	if(pElement)
	{
		BSTR bstr;
		if(pElement->get_innerText(&bstr) == S_OK)
		{
			strText = bstr;
            SysFreeString(bstr);
			return true;
		}
	}

	return false;
}

bool CHTMLNode::getAttrVal(const char *attr, CString& strValue)
{
    VARIANT varAttr;
	HRESULT hr;

	strValue.Empty();

   if(pElement)
	{
		BSTR sUnicode = _bstr_t(attr).copy() ;
//		sUnicode = SysAllocString(A2W(attr));   

        hr = pElement->getAttribute(sUnicode, 2, &varAttr);
        if(hr == S_OK && varAttr.vt != VT_NULL)
		{
			if(varAttr.vt == VT_I4)
				strValue.Format("%d", varAttr.iVal);
			else if(varAttr.vt == VT_BSTR)
				strValue = varAttr.bstrVal;
			else if(varAttr.vt == VT_BOOL)
				strValue = (varAttr.boolVal) ? _T("TRUE") : _T("FALSE");
		}

		SysAllocString(sUnicode);
		return true;
	}

	return false;
}


bool CHTMLNode::getColor(CString& strValue)
{
	strValue.Empty();

	if(scpStyle)
	{
	    VARIANT varClr;
		HRESULT hr;

		varClr.vt = VT_BSTR;
		hr = scpStyle->get_color(&varClr);
		if(hr == S_OK)
		{
			strValue = varClr.bstrVal;

			if(strValue.CompareNoCase(_T("#fff")) == 0)
				strValue = _T("#ffffff");

			if(strValue.CompareNoCase(_T("#000")) == 0)
				strValue = _T("#000000");

			return true;
		}
	}

	return false;
}


bool CHTMLNode::getBGColor(CString& strValue)
{
	strValue.Empty();

	if(scpStyle)
	{
	    VARIANT varClr;
		HRESULT hr;

		varClr.vt = VT_BSTR;
		hr = scpStyle->get_backgroundColor(&varClr);
		if(hr == S_OK)
		{
			strValue = varClr.bstrVal;

			if(strValue.CompareNoCase(_T("#fff")) == 0)
				strValue = _T("#ffffff");

			if(strValue.CompareNoCase(_T("#000")) == 0)
				strValue = _T("#000000");

			return true;
		}
	}

	return false;
}


bool CHTMLNode::getFontFamily(CString& strValue)
{
	strValue.Empty();

	if(scpStyle)
	{
		BSTR bstr;
		HRESULT hr;

		hr = scpStyle->get_fontFamily(&bstr);
		if(hr == S_OK)
		{
			strValue = bstr;

			int pos;
			if((pos = strValue.Find( ',' )) > 0)
				strValue = strValue.Left(pos);

			return true;
		}
	}

	return false;
}

bool CHTMLNode::getFontSize(CString& strValue)
{
	strValue.Empty();

	if(scpStyle)
	{
	    VARIANT varFont;
		HRESULT hr;

		varFont.vt = VT_BSTR;
		hr = scpStyle->get_fontSize(&varFont);
		if(hr == S_OK)
		{
			strValue = varFont.bstrVal;
			return true;
		}
	}

	return false;
}


bool CHTMLNode::getFontStyle(CString& strValue)
{
	strValue.Empty();

	if(scpStyle)
	{
		BSTR bstr;
		HRESULT hr;

		hr = scpStyle->get_fontStyle(&bstr);
		if(hr == S_OK)
		{
			strValue = bstr;
			return true;
		}
	}

	return false;
}

bool CHTMLNode::getFontWeight(CString& strValue)
{
	strValue.Empty();

	if(scpStyle)
	{
	    VARIANT varFont;
		HRESULT hr;

		hr = scpStyle->get_fontWeight(&varFont);
		if(hr == S_OK)
		{
			if(varFont.vt == VT_I4)
				strValue.Format("%d", varFont.iVal);
			else if(varFont.vt == VT_BSTR)
				strValue = varFont.bstrVal;

			return true;
		}
	}

	return false;
}


bool CHTMLNode::getOffsetHeight(long *height)
{
	long lValue;

	*height = 0L;

	if(pElement)
	{
		HRESULT hr;
		hr = pElement->get_offsetHeight(&lValue);
		if(hr == S_OK)
		{
			*height = lValue;
			return true;
		}

	}

	return false;
}

bool CHTMLNode::getOffsetWidth(long *width)
{
	long lValue;

	*width = 0L;

	if(pElement)
	{
		HRESULT hr;
		hr = pElement->get_offsetWidth(&lValue);
		if(hr == S_OK)
		{
			*width = lValue;
			return true;
		}

	}

	return false;
}



bool CHTMLNode::getClientHeight(long *height)
{
	long lValue;

	*height = 0L;

	if(pElement2)
	{
		HRESULT hr;
		hr = pElement2->get_clientHeight(&lValue);
		if(hr == S_OK)
		{
			*height = lValue;
			return true;
		}

	}

	return false;
}

bool CHTMLNode::getClientWidth(long *width)
{
	long lValue;

	*width = 0L;

	if(pElement2)
	{
		HRESULT hr;
		hr = pElement2->get_clientWidth(&lValue);
		if(hr == S_OK)
		{
			*width = lValue;
			return true;
		}

	}

	return false;
}



/* insert node into markup tree at the start of lexer node's content */
void CHTMLNode::InsertNodeAtStart(CHTMLNode *node)
{
    node->parent = this;

    if (content == NULL)
        last = node;
    else
        content->prev = node; // AQ added 13 Apr 2000
    
    node->next = content;
    node->prev = NULL;
    content = node;
}

/* insert node into markup tree at the end of lexer node's content*/
void CHTMLNode::InsertNodeAtEnd( CHTMLNode *node)
{
    node->parent = this;
    node->prev = last;

    if (last != NULL)
        last->next = node;
    else
        content = node;

    last = node;
}


/*
 insert node into markup tree in pace of element
 which is moved to become the child of the node
*/
void CHTMLNode::InsertNodeAsParent(CHTMLNode *node)
{
    node->content = this;
    node->last = this;
    node->parent = parent;
    parent = node;
    
    if (node->parent->content == this)
        node->parent->content = node;

    if (node->parent->last == this)
        node->parent->last = node;

    node->prev = prev;
    prev = NULL;

    if (node->prev)
        node->prev->next = node;

    node->next = next;
    next = NULL;

    if (node->next)
        node->next->prev = node;
}


/* insert new node into markup tree list before lexer node */
void CHTMLNode::InsertNodeBeforeElement(CHTMLNode *node)
{
    node->parent = parent;      //  we share the same parent
    node->next = this;
    node->prev = prev;

    if (prev)
        prev->next = node;
    prev = node;

    if (parent->content == this)
        parent->content = node;
}


/* insert new node into markup tree list after lexer node */
void CHTMLNode::InsertNodeAfterElement(CHTMLNode *node)
{
    node->parent = parent;      //  we share the same parent
    node->prev = this;

    // AQ - 13Jan2000 fix for parent == null
    if (parent != NULL && parent->last == this)
    {
        //*!  lexer assumes that the new node has no further siblings.  Do we
        //*!  want to make this assumption, or search to the end of list before
        //*!  setting parent->last?
        parent->last = node;
    }
    else
    {
        if (next != NULL)
            next->prev = node;
        node->next = next;
    }

    next = node;
}


CHTMLNode * CHTMLNode::getFirst(const char *tag_name, int order)
{
    CHTMLNode *pCurr;
	CString strTag;
	int tagno;

    // Navigate through the child nodes.
	tagno = 0;
	for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
	{
		if(pCurr->getName(strTag))
		{
			if(stricmp(tag_name, strTag) == 0)
			{
				++tagno;
				if(tagno == order)
					return pCurr;
			}
		}
	}

	return NULL;
}

CHTMLNode * CHTMLNode::getLast(const char *tag_name)
{
    CHTMLNode *pCurr;
	CString strTag;

    // Navigate through the child nodes.
	for (pCurr = getLast(); NULL != pCurr; pCurr = pCurr->getPrev() )
	{
		if(pCurr->getName(strTag))
		{
			if(stricmp(tag_name, strTag) == 0)
				return pCurr;
		}
	}

	return NULL;
}


CHTMLNode * CHTMLNode::FindElement(long sourceIndex)
{
	CString strTag;
    CHTMLNode *pCurr;
	CHTMLNode *pBody;

	if(getSourceIndex() == sourceIndex)
		return this;
	
    // Navigate through the child nodes.
	for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
	{
		if((pBody = pCurr->FindElement(sourceIndex)) != NULL)
			return pBody;
	}

	return NULL;
}




CHTMLNode * CHTMLNode::FindBody()
{
	CString strTag;
    CHTMLNode *pCurr;
	CHTMLNode *pBody;

	// skip the root node
	if(getName(strTag))
	{
		if(strTag.CompareNoCase("body") == 0)
			return this;
	}

	
    // Navigate through the child nodes.
	for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
	{
		if((pBody = pCurr->FindBody()) != NULL)			
			return pBody;
	}

	return NULL;
}




CHTMLNode * CHTMLNode::FindTitle()
{
	CString strTag;
    CHTMLNode *pCurr;
	CHTMLNode *pBody;

	// skip the root node
	if(getName(strTag))
	{
		if(strTag.CompareNoCase("title") == 0)
			return this;
	}

	
    // Navigate through the child nodes.
	for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
	{
		if((pBody = pCurr->FindTitle()) != NULL)
			return pBody;
	}

	return NULL;
}

CHTMLNode * CHTMLNode::FindMeta()
{
	CString strTag;
    CHTMLNode *pCurr;
	CHTMLNode *pBody;

	// skip the root node
	if(getName(strTag))
	{
		if(strTag.CompareNoCase("meta") == 0)
			return this;
	}

	
    // Navigate through the child nodes.
	for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
	{
		if((pBody = pCurr->FindMeta()) != NULL)
			return pBody;
	}

	return NULL;
}


//////////////////////////////////////////////////////////////////////
// CPageTagNode Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPageTagNode::CPageTagNode()
{
    parent = (CPageTagNode *)NULL;
    next = (CPageTagNode *)NULL;
    prev = (CPageTagNode *)NULL;
    content = (CPageTagNode *)NULL;
    last = (CPageTagNode *)NULL;

	m_tagName.Empty();
	m_innerText.Empty();
	m_id.Empty();

	m_attributeValues.RemoveAll();

    count = -1;

	m_numWords = 0;
	m_hWords = NULL;


	// for segamentation
	m_bInlineNode = false;

	m_bValidNode = true;
	m_bTextNode = false;
	m_bVirtualTextNode = false;

	m_lClientHeight = 0L;
	m_lClientWidth = 0L;

	m_lOffsetHeight = 0L;
	m_lOffsetWidth = 0L;

	m_iNodeStatus = 0;
	m_bDividable = true;
}

CPageTagNode::~CPageTagNode()
{
	m_tagName.Empty();
	m_innerText.Empty();
	m_id.Empty();

	m_attributeValues.RemoveAll();

	if(m_hWords)
		delete m_hWords;

//	m_name.Empty();
//	m_text.Empty();

//	m_attr.RemoveAll();
//	m_value.RemoveAll();

}

void CPageTagNode::removeChild()
{
    CPageTagNode *pCurr, *pPrev;

    //  Remove my content.
    for (pCurr = last; NULL != pCurr; pCurr = pPrev )
    {
        pPrev = pCurr->prev;
        if (pCurr == content)
            content = NULL;
        pCurr->removeChild();
        delete pCurr;
    }
}


void CPageTagNode::clear()
{
	parent = (CPageTagNode *)NULL;
	next = (CPageTagNode *)NULL;
	prev = (CPageTagNode *)NULL;
	content = (CPageTagNode *)NULL;
	last = (CPageTagNode *)NULL;

	m_tagName.Empty();
	m_innerText.Empty();

	m_attributeValues.RemoveAll();

//	m_attr.RemoveAll();
//	m_value.RemoveAll();

    count = -1;
}



char *inline_tags[] =
{
"a",	"abbr",	"acronym",	"address",
"b",	"bdo",		"big",		"ol",//"blockquote",
"cite",	"code",	"del",	"dfn",	"em",	"font",		"i",		"ins",
"kbd",	"listing",	"plaintext",		"pre",		"q",
"s",	"samp",		"small",	"strike",	"strong",	"sub",	"sup",
"tt",	"u",		"var",		"xmp"
};

bool CPageTagNode::isInlineTag(const char *szTag)
{
	int numStop;
    int low, high, mid;

	numStop = 30;

    low = 0;
    high = numStop - 1;

    while (low <= high)
    {
        mid = (low + high) / 2;

        if (stricmp(szTag, inline_tags[mid]) < 0)
            high = mid - 1;
        else if (stricmp(szTag, inline_tags[mid]) > 0)
            low = mid + 1;
        else
            return true;
    }
    
    return false;

}

// check whether the current tag is a inline text format tag or not
// and navigate through all the child node by broad first search
void CPageTagNode::checkInlineValidText()
{
	m_bInlineNode = isInlineTag(m_tagName);

	if(m_lClientHeight <= 1L || m_lClientWidth <= 1L)
		m_bValidNode = false;
	else
		m_bValidNode = true;

	if(!m_innerText.IsEmpty() && getContent() == NULL)
		m_bTextNode = true;
	else
		m_bTextNode = false;

    CPageTagNode *pCurr;

	// Navigate through the child nodes.
	for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
		pCurr->checkNode();
}

void CPageTagNode::checkNodeVirtualText()
{
	m_bVirtualTextNode = false;

	if(m_bInlineNode)
	{
		CPageTagNode *pNode;
		bool bAllTextNode;

		bAllTextNode = true;
		for (pNode = getContent(); NULL != pNode; pNode = pNode->getNext() )
		{
			if(!pNode->isTextNode())
				pNode->checkNodeVirtualText();

			if(!pNode->isTextNode() && !pNode->isVirtualTextNode())
			{
				bAllTextNode = false;
				break;
			}
		}

		if(bAllTextNode)
			m_bVirtualTextNode = true;
	}
}



// count the sub node under this node, inclusively
void CPageTagNode::countSubNode()
{
    CPageTagNode *pCurr;

    // if it is a leaf, set the count as 1
    if((pCurr = getContent()) == NULL)
        pCurr->setCount(1);
    else
    {
        count = 0;

        // Navigate through the child nodes.
	    for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
	    	pCurr->countSubNode();

	    for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
            count += pCurr->getCount();
        ++count;
    }
    
}


bool CPageTagNode::getAttrVal(const char *attr, CString& strValue)
{
	for(int i = 0; i < m_attributeValues.GetSize(); i++)
	{
		if(m_attributeValues[i].m_strAttr.CompareNoCase(attr) == 0)
		{
            strValue.Empty();
            if(!m_attributeValues[i].m_strVal.IsEmpty())
			    strValue = m_attributeValues[i].m_strVal;

			return true;
		}
	}

	return false;
}


/* insert node into markup tree at the start of lexer node's content */
void CPageTagNode::InsertNodeAtStart(CPageTagNode *node)
{
    node->parent = this;

    if (content == NULL)
        last = node;
    else
        content->prev = node; // AQ added 13 Apr 2000
    
    node->next = content;
    node->prev = NULL;
    content = node;
}

/* insert node into markup tree at the end of lexer node's content*/
void CPageTagNode::InsertNodeAtEnd( CPageTagNode *node)
{
    node->parent = this;
    node->prev = last;

    if (last != NULL)
        last->next = node;
    else
        content = node;

    last = node;
}


/*
 insert node into markup tree in pace of element
 which is moved to become the child of the node
*/
void CPageTagNode::InsertNodeAsParent(CPageTagNode *node)
{
    node->content = this;
    node->last = this;
    node->parent = parent;
    parent = node;
    
    if (node->parent->content == this)
        node->parent->content = node;

    if (node->parent->last == this)
        node->parent->last = node;

    node->prev = prev;
    prev = NULL;

    if (node->prev)
        node->prev->next = node;

    node->next = next;
    next = NULL;

    if (node->next)
        node->next->prev = node;
}


/* insert new node into markup tree list before lexer node */
void CPageTagNode::InsertNodeBeforeElement(CPageTagNode *node)
{
    node->parent = parent;      //  we share the same parent
    node->next = this;
    node->prev = prev;

    if (prev)
        prev->next = node;
    prev = node;

    if (parent->content == this)
        parent->content = node;
}


/* insert new node into markup tree list after lexer node */
void CPageTagNode::InsertNodeAfterElement(CPageTagNode *node)
{
    node->parent = parent;      //  we share the same parent
    node->prev = this;

    // AQ - 13Jan2000 fix for parent == null
    if (parent != NULL && parent->last == this)
    {
        //*!  lexer assumes that the new node has no further siblings.  Do we
        //*!  want to make this assumption, or search to the end of list before
        //*!  setting parent->last?
        parent->last = node;
    }
    else
    {
        if (next != NULL)
            next->prev = node;
        node->next = next;
    }

    next = node;
}


CPageTagNode * CPageTagNode::getFirst(const char *tag_name, int order)
{
    CPageTagNode *pCurr;
	CString strTag;
	int tagno;

    // Navigate through the child nodes.
	tagno = 0;
	pCurr = NULL;
	for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
	{
		if(pCurr->getName(strTag))
		{
			if(stricmp(tag_name, strTag) == 0)
			{
				++tagno;
				if(tagno == order)
					return pCurr;
			}
		}
	}

	return NULL;
}

CPageTagNode * CPageTagNode::getLast(const char *tag_name)
{
    CPageTagNode *pCurr;
	CString strTag;

    // Navigate through the child nodes.
	for (pCurr = getLast(); NULL != pCurr; pCurr = pCurr->getPrev() )
	{
		if(pCurr->getName(strTag))
		{
			if(stricmp(tag_name, strTag) == 0)
				return pCurr;
		}
	}

	return NULL;
}


CPageTagNode * CPageTagNode::Find(int nid)
{
    CPageTagNode *pCurr;
	CPageTagNode *pBody;

	// skip the root node
	if(id == nid)
		return this;
	
    // Navigate through the child nodes.
	for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
	{
		if((pBody = pCurr->Find(nid)) != NULL)
			return pBody;
	}

	return NULL;
}



CPageTagNode * CPageTagNode::FindBody()
{
	CString strTag;
    CPageTagNode *pCurr;
	CPageTagNode *pBody;

	// skip the root node
	if(getName(strTag))
	{
		if(strTag.CompareNoCase("body") == 0)
			return this;
	}

	
    // Navigate through the child nodes.
	for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
	{
		if((pBody = pCurr->FindBody()) != NULL)
			return pBody;
	}

	return NULL;
}




CPageTagNode * CPageTagNode::FindTitle()
{
	CString strTag;
    CPageTagNode *pCurr;
	CPageTagNode *pBody;

	// skip the root node
	if(getName(strTag))
	{
		if(strTag.CompareNoCase("title") == 0)
			return this;
	}

	
    // Navigate through the child nodes.
	for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
	{
		if((pBody = pCurr->FindTitle()) != NULL)
			return pBody;
	}

	return NULL;
}

CPageTagNode * CPageTagNode::FindMeta()
{
	CString strTag;
    CPageTagNode *pCurr;
	CPageTagNode *pBody;

	// skip the root node
	if(getName(strTag))
	{
		if(strTag.CompareNoCase("meta") == 0)
			return this;
	}

	
    // Navigate through the child nodes.
	for (pCurr = getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
	{
		if((pBody = pCurr->FindMeta()) != NULL)
			return pBody;
	}

	return NULL;
}


// Serialize the object ...
void CPageTagNode::serialize(CArchive& pArchive)
{
	int i, len, size;
    CString attr;
    CString value;

	if (pArchive.IsStoring())
	{
		// Write information of one node
		pArchive << id;
		if(parent != NULL)
			pid = parent->getID();

		pArchive << pid;

		pArchive << m_tagName.GetLength();
		if(m_tagName.GetLength() > 0)
			pArchive << m_tagName;

		pArchive << m_innerText.GetLength();
		if(m_innerText.GetLength() > 0)
			pArchive << m_innerText;

		pArchive << m_attributeValues.GetSize();
		for(i = 0; i < m_attributeValues.GetSize(); i++)
		{
			pArchive << m_attributeValues[i].m_strAttr.GetLength();
			if(m_attributeValues[i].m_strAttr.GetLength() > 0)
				pArchive << m_attributeValues[i].m_strAttr;

			pArchive << m_attributeValues[i].m_strVal.GetLength();
			if(m_attributeValues[i].m_strVal.GetLength() > 0)
				pArchive << m_attributeValues[i].m_strVal;
		}
	}
	else
	{
		clear();

		// Read in one node's information
		pArchive >> id;
		pArchive >> pid;

		pArchive >> len;
		if(len > 0)
			pArchive >> m_tagName;

		pArchive >> len;
		if(len > 0)
			pArchive >> m_innerText;

		pArchive >> len;
		m_attributeValues.RemoveAll();
		for(i = 0; i < len; i++)
		{
			attr.Empty();
			pArchive >> size;
			if(size > 0)
				pArchive >> attr;

			value.Empty();
			pArchive >> size;
			if(size > 0)
				pArchive >> value;

			m_attributeValues.Add(CTagAttrVal(attr, value));
		}
	}
}

//************add by ny*****************************
bool CPageTagNode::checkForResultsDiv(CPageTagNode *node)
{
	node->getAttrVal("id", m_id);
   if(stricmp("ires",m_id)==0) return true;//stricmp("search",m_id)==0
   else return false;
}

bool CPageTagNode::checkForResultsItem(CPageTagNode *node)
{
	CString m_class;
	node->getAttrVal("class", m_class);
   if(stricmp("r",m_class)==0) return true;//stricmp("s",m_class)==0||
   else return false;
}

CPageTagNode* CPageTagNode::findSearch(CPageTagNode * tagNode)
{
	CPageTagNode *rval=NULL, *parent=NULL, *ab = NULL;
	CString wName;
	//rval = tagNode->getFirst("div",1);
	//if(rval==NULL)return tagNode;

	 tagNode->getName(wName);
	 if((stricmp(wName, "div") == 0)&&tagNode->checkForResultsDiv(tagNode))
	 {
		rval = tagNode;
		return rval;
	 }
	 else
	 {
       ab=tagNode->getContent();
        while(ab!=NULL)
		{
	            rval = findSearch(ab);
                if (rval!=NULL) return rval;
                else ab=ab->getNext();         
		}
        return rval;
	 }
}

CPageTagNode* CPageTagNode::findSearchItem(CPageTagNode * tagNode)
{
	CPageTagNode *rval=NULL, *parent=NULL, *ab = NULL;
	CString wName;
	//rval = tagNode->getFirst("div",1);
	//if(rval==NULL)return tagNode;

	 tagNode->getName(wName);
	 if((stricmp(wName, "H3") == 0))//(stricmp(wName, "H3") == 0)&& tagNode->checkForResultsItem(tagNode)
	 {
		rval = tagNode;
		return rval;
	 }
	 else
	 {
       ab=tagNode->getContent();
        while(ab!=NULL)
		{
	            rval = findSearchItem(ab);
                if (rval!=NULL) return rval;
                else ab=ab->getNext();         
		}
        return rval;
	 }
}
//////////////////////////////////////////////////////////////////////
// CPageDocument Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPageDocument::CPageDocument()
{
	m_pageID = -1;

	m_document = (CPageTagNode *)NULL;
	m_tagCnt = 0;

	m_lastModified.Empty();

	m_frames = NULL;

	m_clrBackGround.Empty();

	m_repository = NULL;
}

CPageDocument::~CPageDocument()
{
    if(m_document)
    {
        m_document->removeChild();
        delete m_document;
    }

	if(m_frames)
		delete m_frames;
}



// --------------------------------------------------------------------------
// Serialization
// Save the object into local cache file
// --------------------------------------------------------------------------


// Serialize the object ...
void CPageDocument::serialize (CArchive& pArchive)
{
	int i, len, size;
    CPageTagNode *pNode;

	if (pArchive.IsStoring())
	{
		pArchive << m_pageID;

		pArchive << m_url.GetLength();
		if(m_url.GetLength() > 0)
			pArchive << m_url;

		pArchive << m_lastModified.GetLength();
		if(m_lastModified.GetLength() > 0)
			pArchive << m_lastModified;

		pArchive << m_baseURL.GetLength();
		if(m_baseURL.GetLength() > 0)
			pArchive << m_baseURL;

		pArchive << m_baseTarget.GetLength();
		if(m_baseTarget.GetLength() > 0)
			pArchive << m_baseTarget;

		size = 0;
		if(m_frames)
		{
			pArchive << m_frames->size();
			CFrame *pFrame;

			for(i = 0; i < m_frames->size(); i++)
			{
				pFrame = (CFrame *) m_frames->get(i);
				pFrame->serialize(pArchive);
			}
		}
		else
			pArchive << size;

		pArchive << m_tagCnt;
		if(m_document)
			saveTagNode(m_document, pArchive);
	}
	else
	{
		pArchive >> m_pageID;

		m_url.Empty();
		pArchive >> len;
		if(len)
			pArchive >> m_url;

		m_lastModified.Empty();
		pArchive >> len;
		if(len)
			pArchive >> m_lastModified;

		m_baseURL.Empty();
		pArchive >> len;
		if(len)
			pArchive >> m_baseURL;

		m_baseTarget.Empty();
		pArchive >> len;
		if(len)
			pArchive >> m_baseTarget;

		if(m_frames)
		{
			delete m_frames;
			m_frames = NULL;
		}

		pArchive >> size;
		if(size)
		{
			CFrame *pFrame;

			m_frames = (List *) new List();
			for(i = 0; i < size; i++)
			{
				pFrame = (CFrame *) new CFrame();
				pFrame->serialize(pArchive);
				m_frames->add(pFrame);
			}
		}


		if(m_document)
			delete m_document;
		m_document = (CPageTagNode *)NULL;

		// Read in one node's information
		pArchive >> m_tagCnt;
			
		for(i = 0; i < m_tagCnt; i++)
		{
			pNode = (CPageTagNode *)new CPageTagNode();
			pNode->serialize(pArchive);

			insertTagNode(pNode);
		}
	}
}

// serialize the document tree into archive.
void CPageDocument::saveTagNode(CPageTagNode *pNode, CArchive& pArchive)
{
    CPageTagNode *pCurr;

    pNode->serialize(pArchive);

    // Navigate through the child nodes.
    for (pCurr = pNode->getContent(); NULL != pCurr; pCurr = pCurr->getNext() )
        saveTagNode(pCurr, pArchive);
}

// insert the tag node into the document tree.
void CPageDocument::insertTagNode(CPageTagNode *pNode)
{
    CPageTagNode *pTag;

    if(m_document == NULL)
        m_document = pNode;
    else
    {
        pTag = m_document->Find(pNode->getPID());
        if(pTag != NULL)
            pTag->InsertNodeAtEnd(pNode);
        else
            delete pNode;
    }
}


// --------------------------------------------------------------------------
// The following functions are for off-line processing
//
// Input : pageID
//
// at first, construct the file name for local cache and
// the html file.
// if the local cache exists, just load it in
// else read in the html file, parse it, generate the
// tree structure, and then save it in local cache file 
// --------------------------------------------------------------------------


// load the off-line web page, if it is in
// html source, parse it at first and then 
// build the whole tree structure; otherwise
// just load the object from local file
bool CPageDocument::loadOffLine(long pageID, const CString &m_sourceFile, const CString &m_cacheFile)
{
	/*
	CString m_file;
	CString m_lfile;
	CFileStatus rStatus;
    FILE *fp;

	
	CIniFile iniFile;
	CString location;
	CString buffer;

	// read in the settings
	iniFile.SetPath("c:\\lilac.ini");
	iniFile.ReadFile();
	location = iniFile.GetValue("general", "location");

	buffer = iniFile.GetValue("IcWordFinder", "pageSource");
	m_file.Format("%s\\%s\\%d.html", location, buffer, pageID);

	buffer = iniFile.GetValue("IcWordFinder", "pageTree");
	m_lfile.Format("%s\\%s\\%d.dat", location, buffer, pageID);
	

	m_file.Format("%s\\%d.html", theSettings.m_pageSource, pageID);
	m_lfile.Format("%s\\%d.dat", theSettings.m_pageCache, pageID);
*/
	m_pageID = pageID;

	// at first, we check whether we have such local html file
	// Read them back
	if (PathFileExists(m_cacheFile))
		loadCache(m_cacheFile);
	else
	{
		if (!PathFileExists(m_sourceFile))
			return false;

		// if not, then we should parse the html file, and
		// convert it to the CPageTagNode

		loadHTML(m_sourceFile);

		// then we save the parsed HTML into local file
		// for future work
		CFile m_wFile( m_cacheFile, CFile::modeCreate | CFile::modeWrite);
		CArchive m_wArch( &m_wFile, CArchive::store );
		serialize(m_wArch);
		m_wArch.Close();
		m_wFile.Close();
	}

	return true;
}



// load the object from cache file
void CPageDocument::loadCache(const char *m_contentFile)
{
	CFile m_rFile( m_contentFile, CFile::modeRead);
	CArchive m_rArch( &m_rFile, CArchive::load );
	serialize(m_rArch);
	m_rArch.Close();
	m_rFile.Close();
}


// load the object from off-line html file, this function will
// call the parser to parse the html file, and fill the
// object.
void CPageDocument::loadHTML(const CString& m_htmlfile)
{
	CFile f;
    CString csWholeFile;
	IHTMLDocument2 *pDoc = NULL;

	// Open the file and read its content into a string
	if(! f.Open(m_htmlfile, CFile::modeRead|CFile::shareDenyNone))
	{
		m_LogTrace.WriteLine("ERROR : Cannot Open HTML File %s\n", m_htmlfile);
		return;
	}

	csWholeFile.Empty();
	f.Read(csWholeFile.GetBuffer(f.GetLength()), f.GetLength());
	csWholeFile.ReleaseBuffer(f.GetLength());
	f.Close();

	m_baseURL.Empty();
	m_baseTarget.Empty();

    if(csWholeFile.IsEmpty())
        return;

	//parse the html content

	CoCreateInstance(CLSID_HTMLDocument, NULL, CLSCTX_INPROC_SERVER, 
			IID_IHTMLDocument2, (void**)&pDoc);
		
//	pDoc->put_designMode(L"On"); // switches MSHTML Editor on

	//put the code into SAFEARRAY and write it into document
	SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	VARIANT *param;
	bstr_t bsData = (LPCTSTR)csWholeFile;
	HRESULT hr = SafeArrayAccessData(psa, (LPVOID*)&param);

	param->vt = VT_BSTR;
	param->bstrVal = (BSTR)bsData;

	hr = pDoc->write(psa);
	hr = pDoc->close();
		
	SafeArrayDestroy(psa);

	// convert the parse result into tree structure
	getContentTree(pDoc);

//	if(pDoc)
//		pDoc->Release();
}


void CPageDocument::loadByContent(const CString& m_content)
{

	IHTMLDocument2 *pDoc = NULL;


	m_baseURL.Empty();
	m_baseTarget.Empty();

	//parse the html content

	CoCreateInstance(CLSID_HTMLDocument, NULL, CLSCTX_INPROC_SERVER, 
			IID_IHTMLDocument2, (void**)&pDoc);
		
//	pDoc->put_designMode(L"On"); // switches MSHTML Editor on

	//put the code into SAFEARRAY and write it into document
	SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	VARIANT *param;
		long begintime=clock();


	bstr_t bsData = (LPCTSTR)m_content;
	HRESULT hr = SafeArrayAccessData(psa, (LPVOID*)&param);

	param->vt = VT_BSTR;
	param->bstrVal = (BSTR)bsData;

	hr = pDoc->write(psa);
	hr = pDoc->close();
		
	SafeArrayDestroy(psa);

	// convert the parse result into tree structure
	getContentTree(pDoc);

//	if(pDoc)
//		pDoc->Release();
}



// --------------------------------------------------------------------------
// The following functions are for on-line processing
// 
// --------------------------------------------------------------------------

void CPageDocument::loadOnLine(IDispatch *pDisp, bool online)
{
//	MSHTML::IHTMLDocument2Ptr pDoc = NULL;
	IHTMLDocument2 *pDoc = NULL;

	if(pDisp == NULL)
	{
		MessageBox(NULL, "NULL HTMLDocument Pointer", "HTML Parse", MB_OK);
		return;
	}

	CComPtr<IDispatch> spDisp = pDisp;

	HRESULT hr = spDisp->QueryInterface(IID_IHTMLDocument2,(void**)&pDoc);//获取IHTMLDocument2的对象指针，并赋值到pDoc里
	if(FAILED(hr))
	{
		IHTMLWindow2* pFrameWindow=NULL;
		hr = spDisp->QueryInterface(IID_IHTMLWindow2, (void**)&pFrameWindow);
		if(FAILED(hr))
			return;

		hr = pFrameWindow->get_document(&pDoc);

		if(FAILED(hr))
			return;

		/*
		MessageBox(NULL, "Failed to get HTML document COM object", "HTML Parse", MB_OK);
		return;
		*/
	}

	// process the inner document of web page

	// convert the parse result into tree structure

	getContentTree(pDoc);

	if(online)
	{
		BSTR bstr;
		if(pDoc->get_lastModified(&bstr) == S_OK)
		{
			m_lastModified = bstr;
			SysFreeString(bstr);
		}

//		loadOnLineDocument(pDoc);
	}

	if(pDoc)
		pDoc->Release();
}

void CPageDocument::loadOnLineDocument(IHTMLDocument2 *pDoc)
{

	// check whether there are frame pages,
	// if so, we should create the objects for
	// these frame pages
	IHTMLFramesCollection2* pFrames=NULL;
	LONG celem;
	long i;
	VARIANT frameRequested;
	VARIANT frameOut;
	IHTMLWindow2* pFrameWindow=NULL;
	IHTMLDocument2* pFrameDoc=NULL;
	BSTR bstr;
	CString strURL;
	HRESULT hr;

	pDoc->get_frames( &pFrames );
	pFrames->get_length( &celem );
	for(i = 0; i < celem; i++)
	{
		frameRequested.vt = VT_I2;
		frameRequested.iVal = (int)i;
		hr = pFrames->item(&frameRequested, &frameOut);
		if(hr == S_OK && frameOut.vt == VT_DISPATCH)
		{
			if(FAILED(frameOut.pdispVal->QueryInterface(IID_IHTMLWindow2, (void**)&pFrameWindow)))
				continue;

			IPersistFile *lpPersistFile = NULL;
			if(FAILED(frameOut.pdispVal->QueryInterface(IID_IPersistFile, (void**)&lpPersistFile)))
				continue;

			if(FAILED(pFrameWindow->get_document(&pFrameDoc)))
				continue;

			if(pFrameDoc->get_URL(&bstr) == S_OK)
			{
				strURL = bstr;
				SysFreeString(bstr);
			}

//			m_LogTrace.WriteLine("%s : ", strURL);

			if(pFrameDoc->get_lastModified(&bstr) == S_OK)
			{
				strURL = bstr;
				SysFreeString(bstr);
			}

//			m_LogTrace.WriteLine("%s\n", strURL);
			
		}
	}
}


// --------------------------------------------------------------------------
// The following functions are for HTML parsing
// 
// --------------------------------------------------------------------------

long CPageDocument::getSourceIndex(IHTMLElement *pElement)
{
	if(pElement)
	{
		long sourceIndex;
		if(pElement->get_sourceIndex(&sourceIndex) == S_OK)
			return sourceIndex;
	}

	return -9999L;
}




void CPageDocument::getContentTree(IHTMLDocument2 *pDoc)
{
	//declare our MSHTML variables and create a document
//	IHTMLElement *pElement;
	IHTMLElement *cElement;
	IHTMLElement *fElement;
	CString strTag;
	CString strValue;
	CString strName;
	CString strTarget;
//	CHTMLNode *pNode, *fNode, *bNode, *tNode, *tempNode;
	CHTMLNode *pNode, *fNode;
	CHTMLNode *m_root = NULL;

	IHTMLElementCollection* pColl = NULL;
	HRESULT hr;


	// Initialization

	// retrieve the back ground color of the whole document
	VARIANT varClr;
	varClr.vt = VT_BSTR;
	if((hr = pDoc->get_bgColor(&varClr)) == S_OK)
		m_clrBackGround = varClr.bstrVal;


	m_baseURL.Empty();
	m_baseTarget.Empty();

    if(m_document)
    {
        m_document->removeChild();
        delete m_document;
		m_document = NULL;
    }

	m_tagCnt = 0;
	m_document = (CPageTagNode *) new CPageTagNode();
	m_document->setID(m_tagCnt);

	if(m_frames)
	{
		delete m_frames;
		m_frames = NULL;
	}

	// clear all previous HTML tree
	m_root = (CHTMLNode *)new CHTMLNode();

	if(pColl)
		pColl->Release();
	pColl = NULL;


	LONG celem = 0L;
	long i, pID, parentID;

	IDispatch* pElemDisp; 
	IHTMLElement2* cElement2;

	VARIANT varIndex;
	VARIANT var2;
	CString strBuffer;
	bool bInserted;

	CHTMLNode **nodeColl = NULL;

	// Go Through each element in the html and build the parse tree,
	// at the same time, identify the body , frame and ifrmae tags.
	hr = pDoc->get_all( &pColl );
	hr = pColl->get_length( &celem );

	if(celem > 0)
	{
		nodeColl = (CHTMLNode **) new CHTMLNode *[celem];
		for(i = 0; i < celem; i++)
			nodeColl[i] = NULL;
	}

	for(i = 0; i < celem; i++)
	{
		varIndex.vt = VT_UINT;
		varIndex.lVal = i;
		VariantInit( &var2 );

		pElemDisp = NULL;
		hr = pColl->item( varIndex, var2, &pElemDisp );
		if(pElemDisp)
		{
			pElemDisp->QueryInterface( IID_IHTMLElement, (void **)&cElement );
			pElemDisp->QueryInterface( IID_IHTMLElement2, (void **)&cElement2 );// by ny
		}

		if(cElement)
		{
			pID = getSourceIndex(cElement);

			bInserted = false;

			hr = cElement->get_parentElement(&fElement);
			if(hr == S_OK)
			{
				pNode = NULL;

				if(fElement == NULL)
					pNode = m_root;
				else
				{
					parentID = getSourceIndex(fElement);

					if(parentID >= 0 && parentID < celem)
						pNode = nodeColl[parentID];

//					if(parentID >= 0)
//						pNode = m_root->FindElement(parentID);

					fElement->Release();
				}

				if(pNode)
				{
					fNode = (CHTMLNode *)new CHTMLNode();
					fNode->setElement(cElement, cElement2);

					nodeColl[pID] = fNode;

					pNode->InsertNodeAtEnd(fNode);
					bInserted = true;
				}
			}

			if(!bInserted)
			{
				cElement->Release();
				cElement2->Release();
			}

			// Let's check the body, frame and iframe tags
			if(bInserted && fNode->getName(strTag))
			{
				if(stricmp("base", strTag) == 0)
				{
					m_baseURL.Empty();
					fNode->getAttrVal("href", m_baseURL);

					m_baseTarget.Empty();
					fNode->getAttrVal("target", m_baseTarget);
				}

				if((stricmp("frame", strTag) == 0) || (stricmp("iframe", strTag) == 0))
				{
					fNode->getAttrVal("src", strValue);
					if(!strValue.IsEmpty())
					{
						CFrame *pFrame;

						strName.Empty();
						fNode->getAttrVal("name", strName);

						strTarget.Empty();
						fNode->getAttrVal("target", strTarget);


						if(!m_frames)
							m_frames = (List *) new List();

						pFrame = (CFrame *) new CFrame();

						// for the off-line html file, all its frame pages
						// are all off-line, so we are quite sure that 
						// given au URL, we can get its pageID.
						pFrame->setName(strName);
						pFrame->setTarget(strTarget);
						pFrame->setSrc(strValue);

						m_frames->add(pFrame);
					}
				}
			}
		}

		if(pElemDisp)
			pElemDisp->Release();
	}

	if(celem > 0)
		delete [] nodeColl;

	// convert the HTML node to PageTagNode
	SaveAsTagNode(m_root, m_document);


	// release the HTML node tree
	m_root->ReleaseElement();
    m_root->ReleaseChild();
	delete m_root;

	if(pColl)
		pColl->Release();
}



void CPageDocument::SaveAsTagNode(CHTMLNode *phNode, CPageTagNode *ptNode)
{
	CHTMLNode *pNode;
    CPageTagNode *pCurr;
	CString str;
	CString strTag;
	CString strValue;
	CString strName;
	long lValue;

    // Navigate through the child nodes.
	for (pNode = phNode->getContent(); NULL != pNode; pNode = pNode->getNext() )
	{
		pCurr = (CPageTagNode *) new CPageTagNode();

		++m_tagCnt;
		pCurr->setID(m_tagCnt);
		pCurr->setPID(ptNode->getID());

		str.Empty();
		pNode->getName(str);
		pCurr->setName(str);

		str.Empty();
		pNode->getText(str);
		pCurr->setText(str);

		str.Empty();
		if(pNode->getAttrVal("href", str))
			pCurr->addAttrVal("href", str);

		str.Empty();
		if(pNode->getAttrVal("alt", str))
			pCurr->addAttrVal("alt", str);

		str.Empty();
		if(pNode->getAttrVal("target", str))
			pCurr->addAttrVal("target", str);

		str.Empty();
		if(pNode->getAttrVal("src", str))
			pCurr->addAttrVal("src", str);

		str.Empty();
		if(pNode->getAttrVal("keyword", str))
			pCurr->addAttrVal("keyword", str);

		str.Empty();
		if(pNode->getAttrVal("content", str))
			pCurr->addAttrVal("content", str);
         // add by ny
		str.Empty();
		if(pNode->getAttrVal("id", str))
			pCurr->addAttrVal("id", str);

		// retrieve the color/bgcolor and its font properties of the object
		str.Empty();
		if(pNode->getColor(str))
			pCurr->setColor(str);

		str.Empty();
		if(pNode->getBGColor(str))
			pCurr->setBGColor(str);

		str.Empty();
		if(pNode->getFontFamily(str))
			pCurr->setFontFamily(str);

		str.Empty();
		if(pNode->getFontSize(str))
			pCurr->setFontSize(str);

		str.Empty();
		if(pNode->getFontStyle(str))
			pCurr->setFontStyle(str);

		str.Empty();
		if(pNode->getFontWeight(str))
			pCurr->setFontWeight(str);

		// retrieve the height/width of the object
		if(pNode->getClientHeight(&lValue))
			pCurr->setClientHeight(lValue);

		if(pNode->getOffsetHeight(&lValue))
			pCurr->setOffsetHeight(lValue);

		if(pNode->getClientWidth(&lValue))
			pCurr->setClientWidth(lValue);

		if(pNode->getOffsetWidth(&lValue))
			pCurr->setOffsetWidth(lValue);

        ptNode->InsertNodeAtEnd(pCurr);

		SaveAsTagNode(pNode, pCurr);
	}
}

// cpara.cpp : implementation file
//

#include "stdafx.h"
#include "cpara.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CParagraph, CObject, 0)

CParagraph::CParagraph() : CObject()
{
    m_width=0;
	m_height=0;	
	m_fontLocal=0;
	m_fontName="";
	m_fontHeight=0;
	m_fontWeight=0;
	m_justification=CParagraph::left;
}

CParagraph::CParagraph(CString& in_Text, CPoint& in_Position, int in_MaxWidth, int in_MaxHeight) : CObject()
{    
    m_position=in_Position;
    m_width=in_MaxWidth;
    m_height=in_MaxHeight;
	m_fontName="";
	m_fontHeight=0;
	m_fontWeight=0;
    SetText(in_Text);    
	m_justification=CParagraph::left;
}

CParagraph::CParagraph(LPCSTR in_Text, CPoint& in_Position, int in_MaxWidth, int in_MaxHeight) : CObject()
{    
    m_position=in_Position;
    m_width=in_MaxWidth;
    m_height=in_MaxHeight;
	m_fontName="";
	m_fontHeight=0;
	m_fontWeight=0;
    SetText((CString)in_Text);    
	m_justification=CParagraph::left;
}

CParagraph::~CParagraph()
{
    CleanUpWords();
}

CString CParagraph::GetText()
{
    return m_originalText;
}

void CParagraph::SetJustification(justify in_justified)
{
    m_justification=in_justified;
}

CPoint CParagraph::GetPosition()
{
    return m_position;
}

int CParagraph::GetWidth()
{
    return m_width;
} 

int CParagraph::GetHeight()
{
    return m_height;
}

CParagraph::operator const char*()
{
    return (const char*)m_originalText;
}

void CParagraph::Serialize(CArchive& in_Archive)
{
    if(in_Archive.IsStoring())
	{
	  in_Archive << m_position;
	  in_Archive << (WORD)m_width;
	  in_Archive << (WORD)m_height;
	  in_Archive << m_originalText;
	  in_Archive << m_fontName;
	  in_Archive << (unsigned short)m_fontHeight;
	  in_Archive << (unsigned short)m_fontWeight;
	}
	else
	{
	  in_Archive >> m_position;
	  in_Archive >> (WORD&)m_width;
	  in_Archive >> (WORD&)m_height;
	  in_Archive >> m_originalText;
	  in_Archive >> m_fontName;
	  in_Archive >> (unsigned short&)m_fontHeight;
	  in_Archive >> (unsigned short&)m_fontWeight;
	  SetText(m_originalText);
	  SetFont(m_fontName,m_fontHeight,m_fontWeight);
	}
}

void CParagraph::SetText(LPCSTR in_Text)
{
    SetText((CString)in_Text);
}

void CParagraph::SetText(CString& in_Text)
{
    m_originalText=in_Text;
    CleanUpWords();
	
	int max=m_originalText.GetLength() + 10;
    char* temp=new char[max+1];
	const char* orig=(const char*)m_originalText;
	char* copy=temp;
	int spaceCount=0;
	for(int l=0;l<max;l++)
	{
	 switch(orig[0])
	 {
	   case 0xd:
	     break;
	   case 0xa:
		 copy[0]= 32;
		 copy++;
		 break;
	   case 32:
	     if(spaceCount==0)
		 {
		   copy[0]= orig[0];
		   copy++;
         }
		 spaceCount++;
	     break;
	   default:
	   	 copy[0]=orig[0];
		 copy++;
		 spaceCount=0;
		 break;
	 }
	 orig++;
	}    
	copy[0]=0;

    char oldchar;
    char* wordPos=temp;
    char* runner=strchr(wordPos,32);
    while(runner!=NULL)
    {
     oldchar=runner[1];
     runner[1]=0;
     m_words.AddTail((CObject*)new CString(wordPos));
     runner[1]=oldchar;
	 if (runner [2] == '\n')
	 {
		 wordPos = runner + 2;
		 m_words.AddTail((CObject*)new CString("\n"));
	 }
     wordPos=runner+1;
     runner=strchr(wordPos,32);     
    }
    m_words.AddTail((CObject*)new CString(wordPos));    
    delete[] temp;

}

void CParagraph::SetPosition(CPoint& in_Point)
{
    m_position=in_Point;    
}

void CParagraph::SetPosition(int in_Xpos, int in_Ypos)
{
    m_position.x=in_Xpos;
	m_position.y=in_Ypos;
}

void CParagraph::SetWidth(int in_Width)
{
    m_width=in_Width;    
}

void CParagraph::SetHeight(int in_Height)
{
    m_height=in_Height;    
}

void CParagraph::SetFont(CString& in_FontName, int in_Height, int in_Weight)
{
    m_fontName=in_FontName;
	m_fontHeight=in_Height;
	if(in_Weight==1)
	 m_fontWeight=FW_BOLD;
	else
	 m_fontWeight=FW_NORMAL;
	m_font.DeleteObject();
	m_fontLocal=m_font.CreateFont(m_fontHeight,0,0,0,m_fontWeight,0,0,0,0,0,0,0,0,m_fontName);
}

void CParagraph::Paint(CDC* in_pCDC)
{
    POSITION pos=m_words.GetHeadPosition();
	CString line;
	int m_runWidth=2;
	int m_runHeight=2;
	CSize size;
	CString dots("...");
	CSize dotSize=in_pCDC->GetTextExtent((LPCSTR)dots,3);
	CString* pCurrent=0;
	CString* pNext=(CString*)m_words.GetNext(pos);
	while(pNext!=0)
	{
	  pCurrent=pNext;
	  if(pos!=NULL)
	   pNext=(CString*)m_words.GetNext(pos);
	  else
	   pNext=0;
	  if(pNext!=0)
	  {
	    size=in_pCDC->GetTextExtent((LPCSTR)*pCurrent,pCurrent->GetLength());
		CSize size2=in_pCDC->GetTextExtent((LPCSTR)*pNext,pNext->GetLength());
		size.cx+=size2.cx;
		if(!FitsIntoBlock(m_runWidth,m_runHeight,size))
		{
		  size=in_pCDC->GetTextExtent((LPCSTR)*pCurrent,pCurrent->GetLength());
		  size.cx+=dotSize.cx;
		  if(!FitsIntoBlock(m_runWidth,m_runHeight,size))
		  {		    
            in_pCDC->TextOut(GetJustifiedPosition(m_runWidth+dotSize.cx),m_position.y+m_runHeight,dots);            
		  }
		  else
		  {
		    line+=*pCurrent;
			line+=dots;
			in_pCDC->TextOut(GetJustifiedPosition(m_runWidth+size.cx),m_position.y+m_runHeight,line);
		  }
		  line="";
	      break;
		}
	  }
	  size=in_pCDC->GetTextExtent((LPCSTR)*pCurrent,pCurrent->GetLength());
	  if(!FitsIntoBlock(m_runWidth,m_runHeight,size))
	  {
		  if(FitsIntoBlock(m_runWidth,m_runHeight,dotSize))
		  {		    
            in_pCDC->TextOut(GetJustifiedPosition(m_runWidth+dotSize.cx),m_position.y+m_runHeight,dots);       
		  }
		  line="";
		  break;
	  }
	  if((m_runWidth+size.cx)>m_width)
	  {
	   	  in_pCDC->TextOut(GetJustifiedPosition(m_runWidth),m_position.y+m_runHeight,line);
		  m_runWidth=2;
		  m_runHeight+=size.cy;
		  line="";
	  }
	  line+=*pCurrent;	  
	  m_runWidth+=size.cx;
	}
	if(!line.IsEmpty())
	{
	  in_pCDC->TextOut(GetJustifiedPosition(m_runWidth),m_position.y+m_runHeight,line);
	}
}

int CParagraph::FitsIntoBlock(int in_width, int in_height, CSize& in_size)
{
    if(in_width+in_size.cx<m_width)    
	{
	 //if(in_height+in_size.cy<m_height)
	 return 1;
    }
	else
	{
	 if(in_height+(in_size.cy*2)<m_height)
	  return 1;
	}
	return 0;
}

int CParagraph::GetJustifiedPosition(int in_width)
{   
    switch(m_justification)
	{
	  case right:
	   return m_position.x+((m_width-2)-in_width);
	   break;
	  case center:
	   return m_position.x+((m_width-in_width)/2)+2;
	   break;
	  default:
	   return m_position.x+2;
	   break;
	}
    return m_position.x+2;
}

void CParagraph::CleanUpWords()
{    
    POSITION pos=m_words.GetHeadPosition();    
    while(pos!=NULL)    
     delete (CString*)m_words.GetNext(pos);
    m_words.RemoveAll();
}

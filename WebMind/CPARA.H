#ifndef PARAGRAPH

#include <afx.h>
#include <afxwin.h>

class CParagraph : CObject
{
  public:
    static enum justify { left,center,right };
	justify m_justification;
  public:
    CParagraph();
    CParagraph(CString& in_Text, CPoint& in_Position, int maxWidth=0, int maxHeight=0);
	CParagraph(LPCSTR in_Text, CPoint& in_Position, int maxWidth=0, int maxHeight=0);
    ~CParagraph();
	CString GetText();
	CPoint GetPosition();
	int GetWidth();
	int GetHeight();
	void SetJustification(justify in_justified);
	void SetText(LPCSTR in_Text);
    void SetText(CString& in_Text);
    void SetPosition(CPoint& in_Point);
	void SetPosition(int xPos, int yPos);
    void SetWidth(int in_Width);
    void SetHeight(int in_Height);
	void SetFont(CString& in_FontName, int in_height, int in_weight=0);
    void Paint(CDC* in_pCDC);    
	virtual void Serialize(CArchive& ar);
	operator const char*();
  protected:
    CParagraph(const CParagraph&);
    CParagraph& operator=(const CParagraph&);
    void CleanUpWords();
    int GetJustifiedPosition(int in_width);	
	int FitsIntoBlock(int in_width, int in_height, CSize& in_size);
	DECLARE_SERIAL(CParagraph);
  private:
    CObList m_words;
    CString m_originalText;
	CFont m_font;
	int m_fontLocal;
	CString m_fontName;
	int m_fontHeight;
	int m_fontWeight;
    CPoint m_position;
    WORD m_width;
    WORD m_height;    
};
#define PARAGRAPH
#endif
#define PARAGRAPH

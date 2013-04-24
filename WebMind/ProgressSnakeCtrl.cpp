// ProgressSnakeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ProgressSnakeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressSnakeCtrl

CProgressSnakeCtrl::CProgressSnakeCtrl()
{
	m_bReverse=FALSE;
	m_bTimeForTail=FALSE;

	m_nMax=100;
	m_nMin=0;

	m_clrBg=RGB(255,255,255);
	m_clrFg=RGB(0, 0, 0);

	m_nSize=50;
	m_nStep=1;
	m_nPos=0;
	m_nTail=0;

	m_bSnakeMode = TRUE;
}

CProgressSnakeCtrl::~CProgressSnakeCtrl()
{
}


BEGIN_MESSAGE_MAP(CProgressSnakeCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CProgressSnakeCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
    ON_MESSAGE(PBM_GETPOS, OnGetPos)
    ON_MESSAGE(PBM_GETRANGE, OnGetRange)
    ON_MESSAGE(PBM_SETPOS, OnSetPos)
    ON_MESSAGE(PBM_SETRANGE, OnSetRange)
    ON_MESSAGE(PBM_SETRANGE32, OnSetRange32)
    ON_MESSAGE(PBM_SETSTEP, OnSetStep)
    ON_MESSAGE(PBM_STEPIT, OnStepIt)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressSnakeCtrl message handlers

BOOL CProgressSnakeCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{    
     return TRUE;
}

void CProgressSnakeCtrl::OnSize(UINT nType, int cx, int cy) 
{
    CProgressCtrl::OnSize(nType, cx, cy);
	m_nPos    = -1;   // Force update if SetPos called
}


void CProgressSnakeCtrl::OnSetRange(WPARAM, LPARAM range)
{
    OnSetRange32(LOWORD(range), HIWORD(range));
}

void CProgressSnakeCtrl::OnSetRange32(WPARAM Lower, LPARAM Upper)
{
    m_nMax = (int)Upper;
    m_nMin = (int)Lower;
}

LRESULT CProgressSnakeCtrl::OnGetRange(WPARAM Type, LPARAM range)
{
    BOOL bType = (BOOL)Type;
    PPBRANGE pRange = (PPBRANGE)range;
    if (pRange)
    {
        pRange->iHigh = m_nMax;
        pRange->iLow = m_nMin;
    }
    return (LRESULT)(bType ? m_nMin : m_nMax);
}

LRESULT CProgressSnakeCtrl::OnGetPos(WPARAM, LPARAM)
{
    return (LRESULT)m_nPos;
}

LRESULT CProgressSnakeCtrl::OnSetPos(WPARAM newPos, LPARAM) 
{    
    if (!::IsWindow(m_hWnd))
        return -1;

	m_nPos = (int)newPos;
	if(m_nPos <= m_nMin)
		m_nPos = m_nMin;

	if(m_nPos >= m_nMax)
		m_nPos = m_nMax;

	RedrawWindow();

    return (LRESULT)m_nPos;
}

LRESULT CProgressSnakeCtrl::OnStepIt(WPARAM, LPARAM) 
{    
	if(!m_bReverse)
	{
		if(m_nPos>=m_nMax)
		{
			m_nPos=m_nMax;

			if(!m_bTimeForTail)
				m_bTimeForTail=TRUE;
			else if(m_nTail>=m_nMax)
			{
				m_bTimeForTail=FALSE;
				m_nPos=m_nMax-m_nStep;
				m_nTail=m_nMax;
				m_bReverse=TRUE;
			}
			else
				SafeStepTail();
		}
		else
		{
			SafeStepHead();
			SafeStepTail();
		}
	}
	else
	{
		if(m_nPos<=m_nMin)
		{
			m_nPos=m_nMin;
			if(!m_bTimeForTail)
				m_bTimeForTail=TRUE;
			else if(m_nTail<=m_nMin)
			{
				m_bTimeForTail=FALSE;
				m_nPos=m_nStep+m_nMin;
				m_nTail=m_nMin;
				m_bReverse=FALSE;
			}
			else
				SafeStepTail();
		}
		else
		{
			SafeStepHead();
			SafeStepTail();
		}
	}

	return (LRESULT)SetPos(m_nPos);
}

LRESULT CProgressSnakeCtrl::OnSetStep(WPARAM nStep, LPARAM)
{
	int nOldStep = m_nStep;

	m_nStep = (int)nStep;
	return (LRESULT)nOldStep;
}


void CProgressSnakeCtrl::OnPaint() 
{
    CPaintDC PaintDC(this); // device context for painting
    CMemDC dc(&PaintDC);

	CRect rect,rcLeft,rcRight,rcSnake;
	GetClientRect(&rect);

	rcLeft.CopyRect(&rect);
	rcRight.CopyRect(&rect);
	rcSnake.CopyRect(&rect); 

	float nTailpos=(float)(m_nTail-m_nMin)/(float)(m_nMax-m_nMin)*(float)rect.right;
	float nHeadpos=(float)(m_nPos-m_nMin)/(float)(m_nMax-m_nMin)*(float)rect.right;
	
	if(m_bSnakeMode)
	{
		rcSnake.left=(long)(m_bReverse?nHeadpos:nTailpos);
		rcSnake.right=(long)(m_bReverse?nTailpos:nHeadpos);

		rcLeft.right=rcSnake.left;
		rcRight.left=rcSnake.right;

		DrawGradient(&dc, rcLeft, rcSnake, rcRight);
	}
	else
	{
		rcRight.left=(long)nHeadpos;
	}

	CBrush brush;
	VERIFY(brush.CreateSolidBrush(m_clrBg));

	if(m_bSnakeMode)
		dc.FillRect(&rcLeft, &brush);
	dc.FillRect(&rcRight,&brush);

	VERIFY(brush.DeleteObject());

//	ReleaseDC(pdc);		
	// Do not call CStatic::OnPaint() for painting messages
}

/***********************************************************
//////////Modified DrawGradient from CGradientProgressControl
//////////Thanks::matt weagle (mweagle@redrose.net)
//////////This is the code that actually draws the snake 
*************************************************************/
void CProgressSnakeCtrl::DrawGradient(CMemDC *pdc, CRect rcLeft, CRect rcGrad, CRect rcRight)
{
	RECT rectFill;			   // Rectangle for filling band
	float fStep;              // How wide is each band?
	CBrush brush;			// Brush to fill in the bar	

	COLORREF m_clrCurEnd;
	COLORREF m_clrCurStart;
	COLORREF m_clrFill;
	
	// First find out the largest color distance between the start and end colors.  This distance
	// will determine how many steps we use to carve up the client region and the size of each
	// gradient rect.
	int r, g, b;							// First distance, then starting value
	float rStep, gStep, bStep;		// Step size for each color
	
	m_clrCurStart = m_bReverse?m_clrFg:m_clrBg;
	m_clrCurEnd = m_bReverse?m_clrBg:m_clrFg;

	// Get the color differences
	r = (GetRValue(m_clrCurEnd) - GetRValue(m_clrCurStart));
	g = (GetGValue(m_clrCurEnd) - GetGValue(m_clrCurStart));
	b =  (GetBValue(m_clrCurEnd) - GetBValue(m_clrCurStart));
	
	// Make the number of steps equal to the greatest distance
	int nSteps = max(abs(r), max(abs(g), abs(b)));
	
	// Determine how large each band should be in order to cover the
	// client with nSteps bands (one for every color intensity level)
	fStep = ((float)abs(rcGrad.right-rcGrad.left)) / (float)nSteps;
	
	// Calculate the step size for each color
	rStep = r/(float)nSteps;
	gStep = g/(float)nSteps;
	bStep = b/(float)nSteps;
	
	// Reset the colors to the starting position
	r = GetRValue(m_clrCurStart);
	g = GetGValue(m_clrCurStart);
	b = GetBValue(m_clrCurStart);
	
	// Start filling bands
	for (int iOnBand = 0; iOnBand < nSteps; iOnBand++) 
	{
		::SetRect(&rectFill,
			(int)(rcGrad.left+iOnBand * fStep),       // Upper left X
			rcGrad.top,									 // Upper left Y
			rcGrad.left+(int)((iOnBand+1) * fStep),          // Lower right X
			rcGrad.bottom);			// Lower right Y

		if(rectFill.left<rcGrad.left || rectFill.right>rcGrad.right)
		return;

		m_clrFill = RGB(r+rStep*iOnBand, g + gStep*iOnBand, b + bStep *iOnBand);

		// CDC::FillSolidRect is faster, but it does not handle 8-bit color depth
		VERIFY(brush.CreateSolidBrush(m_clrFill));
		pdc->FillRect(&rectFill,&brush);
		VERIFY(brush.DeleteObject());
	}
}

void CProgressSnakeCtrl::SafeStepHead()
{
   if(m_bReverse)
   {
	   if(m_nMin < m_nPos-m_nStep)
		  m_nPos-=m_nStep;
	  else
		  m_nPos=m_nMin;
   }
   else
   {
	  if(m_nMax > m_nPos+m_nStep)
		  m_nPos+=m_nStep;
	  else
		  m_nPos=m_nMax;
   }
}

void CProgressSnakeCtrl::SafeStepTail()
{
	if(!m_bTimeForTail && 
		((m_bReverse && m_nMax-m_nPos < m_nSize) || 
		 (!m_bReverse && m_nPos-m_nMin < m_nSize)) )
		return;

   if(m_bReverse)
   {
	  if(m_nMin < (m_nTail-m_nStep))
		  m_nTail-=m_nStep;
	  else
		  m_nTail=m_nMin;
   }
   else
   {
	  if(m_nMax > (m_nTail+m_nStep))
		  m_nTail+=m_nStep;
	  else
		  m_nTail=m_nMax;
   }
}

void CProgressSnakeCtrl::ResetProgress()
{
	m_bReverse=FALSE;
	m_nTail=m_nPos=m_nMin;
}

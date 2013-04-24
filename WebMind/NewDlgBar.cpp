// NewDlgBar.cpp : implementation file
//

#include "stdafx.h"
#include "WebMind.h"
#include "NewDlgBar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewDlgBar dialog


CNewDlgBar::CNewDlgBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CNewDlgBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CNewDlgBar::Create(CWnd* pParentWnd,UINT nIDTemplate,UINT nStyle,UINT nID) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bRes= CDialogBar::Create(pParentWnd,nIDTemplate,nStyle,nID );
	OnInitDialog();
	return bRes;
	//return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CNewDlgBar::OnInitDialog()   
{
	UpdateData(FALSE);
	
	//绘图:代码kosko加的
	// Disable the refresh of the control (avoid multiple refresh).
	m_ChartCtrl.EnableRefresh(false);
	// Create a bottom and left axes
	CChartStandardAxis* pBottomAxis =
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	CChartStandardAxis* pLeftAxis =
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	// Sets the min and max values of the bottom and left axis to -15 -> 15
	pBottomAxis->SetMinMax(0,1);
	pLeftAxis->SetMinMax(0,1);
	
	// Add a new series of type line to the control and add data to it
	//CChartLineSerie* pLineSeries = m_ChartCtrl.CreateLineSerie();
	// Specifies that the points in the series are not ordered (needed to be able
	// to draw an ellipse).
	//pLineSeries->SetSeriesOrdering(poNoOrdering);
	//for (int i=0;i<10;i++)
	//{
	//	double X = 0.1*i;
	//	double Y = 0.1*i;
	//	pLineSeries->AddPoint(X,Y);
	//}
	
	// Defines the different colors (back color, axes color, ...)
	COLORREF BackColor = RGB(0,50,0);
	COLORREF GridColor = RGB(0,180,0);
	COLORREF TextColor = RGB(0,180,0);
	COLORREF SerieColor = RGB(0,255,0);
	
	// Specifies a sunken border for the control
	m_ChartCtrl.SetEdgeType(EDGE_SUNKEN);
	
	// Sets the color of the border and the back color
	m_ChartCtrl.SetBorderColor(TextColor);
	//m_ChartCtrl.SetBackColor(BackColor);
	
	//Sets the color of the different elements of the bottom axis
	m_ChartCtrl.GetBottomAxis()->SetAxisColor(TextColor);
	m_ChartCtrl.GetBottomAxis()->SetTextColor(TextColor);
	m_ChartCtrl.GetBottomAxis()->GetGrid()->SetColor(GridColor);
	
	// Sets the color of the different elements of the left axis
	m_ChartCtrl.GetLeftAxis()->SetAxisColor(TextColor);
	m_ChartCtrl.GetLeftAxis()->SetTextColor(TextColor);
	m_ChartCtrl.GetLeftAxis()->GetGrid()->SetColor(GridColor);
	
	// Sets the color of the title, change the font to Times New Roman
	// and add a string
	m_ChartCtrl.GetTitle()->SetColor(TextColor);
	m_ChartCtrl.GetTitle()->SetFont(100,_T("Times New Roman"));
	//m_ChartCtrl.GetTitle()->AddString(_T("WebMind Browser"));
	
	// Change the color of the line series
	//pLineSeries->SetColor(SerieColor);
	
	// Finally re-enable the refresh of the control. This will refresh the
	// control if any refresh was still 'pending'.
	m_ChartCtrl.EnableRefresh(true);
	//绘图:代码kosko加的
	
	return TRUE;
}

void CNewDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewDlgBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CNewDlgBar, CDialogBar)
	//{{AFX_MSG_MAP(CNewDlgBar)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	//在此处定义
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewDlgBar message handlers

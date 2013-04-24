/////////////////////////////////////////////////////////////////////////////
/* 
DESCRIPTION
-----------

	CAboutCtrl - Aboutbox Credits Control
	
	This control can be use as an about box for your applications   
	http://www.codeproject.com/staticctrl/aboutctrl.asp


Usage
-----
To use the class in your application you need to do the following:

1. Insert the .h and .cpp files into your project. 
2. Add a static control into your dialog resource. 
3. Add a member variable for that static control. 

4. Modify the variable declaration. 
    Change this: CStatic m_AboutCtrl; 
    To this: CAboutCtrl m_AboutCtrl; 

5. In your dialog's constructor set text for the credits: 
   CString strCredits = "\tCAboutCtrl Example\n\n"
                     "\rProgrammed by:\n"
                     "Pablo van der Meer\n\n"
                     "\rSpecial thanks to:\nwww.codeproject.com\n\n"
                     "\rCopyright © 2002\n\rPablo Software Solutions\n"
                     "\rAll right reserved.\n\n"
                     "http:\\www.pablovandermeer.nl\n";
    m_AboutCtrl.SetCredits(strCredits);


*/
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_ABOUTCTRL_H__B75B0B9E_367D_11D6_AB38_00D0B70C3D79__INCLUDED_)
#define AFX_ABOUTCTRL_H__B75B0B9E_367D_11D6_AB38_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAboutCtrl : public CStatic
{
// Construction
public:
	CAboutCtrl();

// Attributes
protected:
	HDC m_hLogoDC; 
	HDC m_hBackgroundDC; 
	HDC m_hMemDC; 
	HDC m_hCreditsDC; 

	// logo dimensions
	int m_cxLogo, m_cyLogo;
	// logo angle
	int m_xAngle, m_yAngle;
	// logo 'fly' speed
	int m_nSpeed;
	// logo position
	int m_xPos, m_yPos; 

	int m_cxData;
	int m_cyData;

	int m_nCounter;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetCredits(LPCTSTR lpszCredits);
	virtual ~CAboutCtrl();

	// Generated message map functions
protected:
	CString m_strCredits;
	void Initialize();
	void AnimateLogo();
	void LoadPicture(int nResourceID, HDC &hDestinationDC, int &nWidth, int &nHeight, HDC hDC);
	void LoadCredits(HDC &hDestination, int nWidth, int nHeight, HDC hDC);

	//{{AFX_MSG(CAboutCtrl)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTCTRL_H__B75B0B9E_367D_11D6_AB38_00D0B70C3D79__INCLUDED_)

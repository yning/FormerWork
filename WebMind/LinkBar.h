///////////////////////////////////////////////////////////////
// LinkBar.h: interface for the CLinkBar class.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_LINKBAR_H__34BC25F5_7800_48F6_8884_91C579B2C3EA__INCLUDED_)
#define AFX_LINKBAR_H__34BC25F5_7800_48F6_8884_91C579B2C3EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBarEx.h"

class CLinkBar : public CToolBarEx
{
    DECLARE_DYNAMIC( CLinkBar )
public:
    CLinkBar();
public:
	//CImageList	m_ImageListSmall;
// Overrides
public:

    virtual void Init();
protected:
    virtual bool HasButtonText( UINT nID );
	virtual void GetButtonTip( UINT nID, CString& strTip);
	virtual bool HasButtonTip( UINT nID );
//	virtual void GetButtonText( UINT nID, CString& strText );
};

#endif // !defined(AFX_LINKBAR_H__34BC25F5_7800_48F6_8884_91C579B2C3EA__INCLUDED_)

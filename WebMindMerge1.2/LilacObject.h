///////////////////////////////////////////////////////////////
// LilacObject.h: interface for the CLilacObject class.
//
//
///////////////////////////////////////////////////////////////


#if !defined(AFX_LILACOBJECT_H__413CBF33_0B51_457F_AC01_6F5285518A76__INCLUDED_)
#define AFX_LILACOBJECT_H__413CBF33_0B51_457F_AC01_6F5285518A76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLilacObject  
{
public:
	
	// Constructor/Destructor
	CLilacObject();
	virtual ~CLilacObject();
	
	// To ensure a consistent comparison interface and to allow comparison
	// of all kinds of different objects, we will define a comparison functions.
	// 0 : Equal
	// 1 : Not
	virtual int		compare(CLilacObject *);
	
	// To allow a deep copy of data structures we will define a standard interface...
	// This member will return a copy of itself, freshly allocated and deep copied.
	virtual CLilacObject	*clone();
	
	// To generate a key for this object, this will be used in Hash table
	virtual int         key();
	
	// To print out the content within the object
	virtual void        dump();
	
};

#endif // !defined(AFX_LILACOBJECT_H__413CBF33_0B51_457F_AC01_6F5285518A76__INCLUDED_)

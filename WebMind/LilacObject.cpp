///////////////////////////////////////////////////////////////
// LilacObject.cpp: implementation of the CLilacObject class.
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LilacObject.h"

//////////////////////////////////////////////////////////////////////
// Object Interface
//////////////////////////////////////////////////////////////////////

CLilacObject::CLilacObject()
{
}

CLilacObject::~CLilacObject()
{
}

int CLilacObject::compare(CLilacObject *)
{
	return 0;
}

CLilacObject *CLilacObject::clone()
{
	return new CLilacObject;
}

int CLilacObject::key()
{
	return -1;
}

void CLilacObject::dump()
{
	printf("Dump Object\n");
}

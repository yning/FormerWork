///////////////////////////////////////////////////////////////
// Instance.cpp: implementation of the CInstance class.
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Instance.h"
#include "Utility.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDInstance::CDInstance()
{
	m_attVal.RemoveAll();
}

CDInstance::CDInstance(const CDInstance& src)
{
	m_attVal.RemoveAll();
	m_attVal.Append(src.getAttValue());
}

CDInstance::~CDInstance()
{
	m_attVal.RemoveAll();
}

void CDInstance::RemoveAll()
{
	m_attVal.RemoveAll();
}

void CDInstance::SetSize(int size)
{
	m_attVal.SetSize(size);
}

void CDInstance::operator =(const CDInstance& src)
{
	m_attVal.RemoveAll();
	m_attVal.Append(src.getAttValue());
}

void CDInstance::SetAt(int index, short value)
{
	m_attVal.SetAt(index, value);
}

short CDInstance::operator [](int index) const
{
	return m_attVal[index];
}

const CArray<short, short&>& CDInstance::getAttValue() const
{
	return m_attVal;
}

int CDInstance::serialize (CArchive& pArchive)
{
	int nStatus = 0, i, size;
	short avalue;

	// Serialize the object ...
	if (pArchive.IsStoring())
	{
		pArchive << m_attVal.GetSize();
		for(i = 0; i < m_attVal.GetSize(); i++)
			pArchive << m_attVal[i];
	}
	else
	{
		pArchive >> size;
		for(i = 0; i < size; i++)
		{
			pArchive >> avalue;
			m_attVal.Add(avalue);
		}
	}

	return (nStatus);
}

void CDInstance::dump()
{
	for(int i = 0; i < m_attVal.GetSize(); i++)
		m_LogTrace.WriteLine(" %6d ", m_attVal[i]);
	m_LogTrace.WriteLine("\n");
}




CInstance::CInstance()
{
	m_attVal.RemoveAll();
}

CInstance::~CInstance()
{
	m_attVal.RemoveAll();
}

void CInstance::SetSize(int size)
{
	m_attVal.SetSize(size);
}

void CInstance::operator =(const CInstance& src)
{
	m_attVal.RemoveAll();
	m_attVal.Append(src.getAttValue());
}

void CInstance::SetAt(int index, double value)
{
	m_attVal.SetAt(index, value);
}

double CInstance::operator [](int index) const
{
	return m_attVal[index];
}

const CArray<double, double&>& CInstance::getAttValue() const
{
	return m_attVal;
}

int CInstance::serialize (CArchive& pArchive)
{
	int nStatus = 0, i, size;
	double avalue;

	// Serialize the object ...
	if (pArchive.IsStoring())
	{
		pArchive << m_attVal.GetSize();
		for(i = 0; i < m_attVal.GetSize(); i++)
			pArchive << m_attVal[i];
	}
	else
	{
		pArchive >> size;
		for(i = 0; i < size; i++)
		{
			pArchive >> avalue;
			m_attVal.Add(avalue);
		}
	}

	return (nStatus);
}

void CInstance::dump()
{
	for(int i = 0; i < m_attVal.GetSize(); i++)
		m_LogTrace.WriteLine(" %8.2lf ", m_attVal[i]);
	m_LogTrace.WriteLine("\n");
}

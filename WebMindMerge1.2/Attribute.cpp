///////////////////////////////////////////////////////////////
// Attribute.cpp: implementation of the CAttribute class.
//
// This software was written by Tingshao Zhu.
//
// Copyright (C) 2000-2004 Tingshao Zhu. All rights reserved!
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Attribute.h"
#include "Utility.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAttribute::CAttribute()
{
	m_discretValue.RemoveAll();
	m_type = Nominal;

	m_discreteType = 0;
	m_splits.RemoveAll();
	numPrecision = 0.01;

	m_ignore = false;
}

CAttribute::CAttribute(const CAttribute& src)
{
	m_name = src.getName();
	m_type = src.getType();

	m_discretValue.RemoveAll();
	m_discretValue.Append(src.getConstDiscretValue());

	m_splits.RemoveAll();
	m_splits.Append(src.getSplits());

	numPrecision = src.getPrecision();
	m_ignore = src.isIgnore();
}

CAttribute::~CAttribute()
{
	m_discretValue.RemoveAll();
	m_splits.RemoveAll();
}

void CAttribute::qucikSortSplits(int Fp, int Lp)
{
    int Lower, Middle;
    double Thresh, temp;
    int i;

    if ( Fp < Lp )
    {
        Thresh = m_splits[Lp];

    	/*  Isolate all items with values <= threshold  */
    	Middle = Fp;

        for ( i = Fp ; i < Lp ; i++ )
        { 
            if ( m_splits[i] <= Thresh )
            { 
                if ( i != Middle )
                {
                    temp = m_splits[Middle];
                    m_splits.SetAt(Middle, m_splits[i]);
                    m_splits.SetAt(i, temp);
                }
                Middle++; 
            } 
        } 

        /*  Extract all values equal to the threshold  */
        Lower = Middle - 1;
        for ( i = Lower ; i >= Fp ; i-- )
	    {
	        if ( m_splits[i] == Thresh )
	        { 
		        if ( i != Lower )
                {
                    temp = m_splits[Lower];
                    m_splits.SetAt(Lower, m_splits[i]);
                    m_splits.SetAt(i, temp);
                }
                Lower--;
            } 
        } 

        /*  Sort the lower values  */
        qucikSortSplits(Fp, Lower);

        /*  Position the middle element  */
        if ( Middle != Lp )
        {
            temp = m_splits[Middle];
            m_splits.SetAt(Middle, m_splits[Lp]);
            m_splits.SetAt(Lp, temp);
        }

	    /*  Sort the higher values  */
    	qucikSortSplits(Middle+1, Lp);
    }
}


void CAttribute::setName(const char *name)
{
	m_name.Empty();
	m_name = name;
}

char * CAttribute::getName() const
{
	return (char *)((LPCTSTR)m_name);
}

void CAttribute::setType(AttrType type)
{
	m_type = type;
}

AttrType CAttribute::getType() const
{
	return m_type;
}

int CAttribute::getSize()
{
	if(m_type == Nominal)
		return m_discretValue.GetSize();
	else
		return m_splits.GetSize() + 1;
}

void CAttribute::addDiscretValue(CString value)
{
	m_discretValue.Add(value);
}

void CAttribute::addSplit(double value)
{
	m_splits.Add(value);
}

const CStringArray& CAttribute::getConstDiscretValue() const
{
	return m_discretValue;
}

CStringArray& CAttribute::getDiscretValue()
{
	return m_discretValue;
}


int CAttribute::clearDiscretValue()
{
	m_discretValue.RemoveAll();
	return 0;
}

bool CAttribute::isIgnore() const
{
	return m_ignore;
}

void CAttribute::setIgnore(bool value)
{
	m_ignore = value;
}

const CArray<double, double&>& CAttribute::getSplits() const
{
	return m_splits;
}

int CAttribute::getSplitNum()
{
	return m_splits.GetSize();
}

double CAttribute::getSplit(int index)
{
	return m_splits[index];
}

void CAttribute::setPrecision(double precision)
{
	numPrecision = precision;
}

double CAttribute::getPrecision() const
{
	return numPrecision;
}

void CAttribute::operator =(const CAttribute& src)
{
	m_name = src.getName();
	m_type = src.getType();

	m_discretValue.RemoveAll();
	m_discretValue.Append(src.getConstDiscretValue());

	m_splits.RemoveAll();
	m_splits.Append(src.getSplits());

	numPrecision = src.getPrecision();
	m_ignore = src.isIgnore();
}

int CAttribute::getDiscretIndex(CString value)
{
	for(int index = 0; index < m_discretValue.GetSize(); index++)
	{
		if (value.Compare(m_discretValue[index]) == 0)
			return index;
	}
	return -1;
}


bool CAttribute::getDiscretValue(int index, CString& value)
{
	if(index >= 0 && index < m_discretValue.GetSize())
	{
		value = m_discretValue[index];
		return true;
	}

	return false;
}



int CAttribute::getDiscretNumber()
{
	return m_discretValue.GetSize();
}


int CAttribute::serialize (CArchive& pArchive)
{
	int nStatus = 0, i, size, len, type;
	CString dvalue;
	double split;

	// Serialize the object ...
	if (pArchive.IsStoring())
	{
		len = m_name.GetLength();
		pArchive << len;
		if(len)
			pArchive << m_name;

		pArchive << numPrecision;

		pArchive << m_type;
		if(m_type == Nominal)
		{
			pArchive << m_discretValue.GetSize();
			for(i = 0; i < m_discretValue.GetSize(); i++)
			{
				pArchive << m_discretValue[i].GetLength();
				if(m_discretValue[i].GetLength() > 0)
					pArchive << m_discretValue[i];
			}
		}
		else
		{
			pArchive << m_splits.GetSize();
			for(i = 0; i < m_splits.GetSize(); i++)
				pArchive << m_splits[i];
		}

	}
	else
	{
		m_discretValue.RemoveAll();
		m_splits.RemoveAll();

		pArchive >> len;
		if(len)
			pArchive >> m_name;

		pArchive >> numPrecision;

		m_type = Continuous;
		pArchive >> type;
		if(type)
			m_type = Nominal;

		if(m_type == Nominal)
		{
			pArchive >> size;
			for(i = 0; i < size; i++)
			{
				dvalue.Empty();
				pArchive >> len;
				if(len)
				{
					pArchive >> dvalue;
					m_discretValue.Add(dvalue);
				}
			}
		}
		else
		{
			pArchive >> size;
			for(i = 0; i < size; i++)
			{
				pArchive >> split;
				m_splits.Add(split);
			}
		}
	}

	return (nStatus);
}

void CAttribute::dump()
{
	m_LogTrace.WriteLine("\n");

	m_LogTrace.WriteLine("Name : %s\n", m_name);
	m_LogTrace.WriteLine("Precision : %lf\n", numPrecision);

	if(m_type == Continuous)
	{
		m_LogTrace.WriteLine("Type : Continuous\n");

		for(int i = 0; i < m_splits.GetSize(); i++)
			m_LogTrace.WriteLine("%lf\n", m_splits[i]);
	}
	else
	{
		m_LogTrace.WriteLine("Type : Nominal\n");

		for(int j = 0; j < m_discretValue.GetSize(); j++)
			m_LogTrace.WriteLine("%s\n", m_discretValue[j]);
	}

	if(m_ignore)
		m_LogTrace.WriteLine("IGNORED\n");


	m_LogTrace.WriteLine("\n");
}


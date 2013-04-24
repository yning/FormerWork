///////////////////////////////////////////////////////////////
// Attribute.h: interface for the CAttribute class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_ATTRIBUTE_H__FE2FEA85_C1DA_462F_879A_78CD4FBBA910__INCLUDED_)
#define AFX_ATTRIBUTE_H__FE2FEA85_C1DA_462F_879A_78CD4FBBA910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Utility.h"

class CAttribute : public CObject
{
public:
	CAttribute();
	CAttribute(const CAttribute& src);
	virtual ~CAttribute();

    void qucikSortSplits(int Fp, int Lp);

	void setName(const char *name);
	char * getName() const;

	void setType(AttrType type);
	AttrType getType() const;

	int getSize();

	void addDiscretValue(CString value);
	const CStringArray& getConstDiscretValue() const;
	bool getDiscretValue(int index, CString& value);
	CStringArray& getDiscretValue();
	int clearDiscretValue();

	bool isIgnore() const;
	void setIgnore(bool value = true);

	const CArray<double, double&>& getSplits() const;
	int getSplitNum();
	double getSplit(int index);
	void addSplit(double value);

	void setPrecision(double precision);
	double getPrecision() const;

	void operator =(const CAttribute& src);

    int getDiscretIndex(CString value);
	int getDiscretNumber();

    int serialize (CArchive& pArchive);

	void setDiscreteType(int type)		{	m_discreteType = type;	}
	int getDiscreteType()				{	return m_discreteType;	}

    void dump();

private:

    CString m_name;
    AttrType m_type;

    // for nominal attribute, to store all
    // the values
    CStringArray m_discretValue;

	// the method for discretization
	int m_discreteType;

    // for continuous attribute, store all
    // the split point
	CArray<double, double&> m_splits;

    double numPrecision;

	bool m_ignore;
};

#endif // !defined(AFX_ATTRIBUTE_H__FE2FEA85_C1DA_462F_879A_78CD4FBBA910__INCLUDED_)

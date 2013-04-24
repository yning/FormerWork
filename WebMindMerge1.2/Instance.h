///////////////////////////////////////////////////////////////
// Instance.h: interface for the CInstance class.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_INSTANCE_H__671A5326_6485_49E6_968C_5D34E3716FA6__INCLUDED_)
#define AFX_INSTANCE_H__671A5326_6485_49E6_968C_5D34E3716FA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDInstance : public CObject
{
public:
    CDInstance();
    CDInstance(const CDInstance& src);
    ~CDInstance();

    void RemoveAll();

    void SetSize(int size);

	void operator =(const CDInstance& src);

    void SetAt(int index, short value);

    short operator [](int index) const;

	const CArray<short, short&>& getAttValue() const;

    int serialize (CArchive& pArchive);

    void dump();

private:

	CArray<short, short&> m_attVal;
};



class CInstance : public CObject
{
public:
    CInstance();
    ~CInstance();

    void SetSize(int size);

	void operator =(const CInstance& src);

    void SetAt(int index, double value);
    double operator [](int index) const;
	const CArray<double, double&>& getAttValue() const;
    int serialize (CArchive& pArchive);
    void dump();

private:

	CArray<double, double&> m_attVal;
};


#endif // !defined(AFX_INSTANCE_H__671A5326_6485_49E6_968C_5D34E3716FA6__INCLUDED_)

///////////////////////////////////////////////////////////////
// DataSet.h: interface for the CDataSet class.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_DATASET_H__E98C301B_D7C3_11D6_B3F6_0050040A6622__INCLUDED_)
#define AFX_DATASET_H__E98C301B_D7C3_11D6_B3F6_0050040A6622__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Instance.h"
#include "Attribute.h"

#include "Utility.h"

class CDataSet  
{
public:

	CDataSet();
	virtual ~CDataSet();

	void clearData();

public:

    // Load data from other source
    bool ReadName(FILE *f, CString& s);

    bool LoadMLC45Data(const char *Fn);
    bool LoadMLC45Attributes(const char *Fn);

    bool LoadWekaARFF(const char *Fn);

    // calculate the split points for continuous attributes
	void calGainDeta(double T, int begin, int end, int attindex, double& gain, double& deta);
	double Entropy(double T , int begin, int end, int attindex);
	double find_best_cut_point(int begin , int end, int attindex);
	void cut(int begin, int end, int attindex);
	bool calSplitPoints(int attindex);

    // discretize all the continuous attributes and the discret all the data
	//void DiscretizeInstance(const CInstance& pInstance, CDInstance& pDInsatnce);
	void calSplits();
	void Discretize();

	void calPrecision();

	void DeC45fy();
	void C45fy();


	void Swap(int src, int target);
    void Quicksort(int Fp, int Lp, int Att);
	void quickSortAt(int attindex);


public:

    // set the number of attributes
	void setAttSize(int size)
	{
		m_attrs.SetSize(size);
	}

	int getAttSize()
	{
		return m_attrs.GetSize();
	}

	int getAttSize(int attindex)
	{
		return m_attrs[attindex].getSize();
	}

	void getAttName(int attindex, CString& buffer)
	{
		if(attindex < 0 || attindex >= getAttSize())
			return;
		buffer = m_attrs[attindex].getName();
	}

	void setAttPrecision(int attindex, double precision)
	{
		if(attindex < 0 || attindex >= getAttSize())
			return;

		m_attrs[attindex].setPrecision(precision);
	}

	double getAttPrecision(int attindex)
	{
		if(attindex < 0 || attindex >= getAttSize())
			return -1.0;

		return m_attrs[attindex].getPrecision();
	}


	bool isIgnore(int attindex)
	{
		if(attindex < 0 || attindex >= getAttSize())
			return true;

		return m_attrs[attindex].isIgnore();
	}


	void setIgnore(int attindex, bool value)
	{
		if(attindex < 0 || attindex >= getAttSize())
			return;

		m_attrs[attindex].setIgnore(value);
	}

    // set the type of the specified attribute
	void setAttName(int attindex, const char *name)
	{
		if(attindex < 0 || attindex >= m_attrs.GetSize())
			return;
		m_attrs[attindex].setName(name);
	}

	void setAttType(int attindex, AttrType type)
	{
		if(attindex < 0 || attindex >= m_attrs.GetSize())
			return;
		m_attrs[attindex].setType(type);
	}

	AttrType getAttType(int attindex)
	{
		return m_attrs[attindex].getType();
	}

    bool isAttContinuous(int index)
    {
        return (m_attrs[index].getType() == Continuous);
    }

    int getNumSplits(int attr)
    {
        return m_attrs[attr].getSplitNum();
    }


    int getDiscretIndex(int attr, CString value)
    {
        return m_attrs[attr].getDiscretIndex(value);
    }

    bool getDiscretValue(int attr, int index, CString& value)
    {
		if(isAttContinuous(attr))
			return false;

        return m_attrs[attr].getDiscretValue(index, value);
    }

    int getDiscretNumber(int attindex)
    {
		if(isAttContinuous(attindex) || attindex < 0 || attindex >= m_attrs.GetSize())
			return 0;

		return m_attrs[attindex].getDiscretNumber();
    }

	// get the number of class
	int getClassNum()
	{
		int classIndex;
		classIndex = m_attrs.GetSize() - 1;

		return  getDiscretNumber( classIndex );
	}


	// get the name of class
	void getClassName(int index, CString &value)
	{
		int classIndex;
		classIndex = m_attrs.GetSize() - 1;
		value.Empty();
		if(index >= 0 && index < getDiscretNumber(classIndex))
			getDiscretValue(classIndex, index, value);

		return;
	}


	// get the maximum number of discret value of all attributes
	// except the last one which is the class
	int getMaxDiscrVal()
	{
		int maxNum = 2, dVal;
		for(int i = 0; i < getAttSize() - 1; i++)
			if( (dVal = getDiscretNumber(i)) >= maxNum)
				maxNum = dVal;

		return maxNum;
	}

	void setAttDiscreteType(int attindex, int type)
	{
		if(attindex < 0 || attindex >= m_attrs.GetSize() || !isAttContinuous(attindex))
			return;

		m_attrs[attindex].setDiscreteType(type);
	}

    int getSplitNum(int attr)
    {
		if(! isAttContinuous(attr))
			return -1;

		return m_attrs[attr].getSplitNum();
    }

    double getSplit(int attr, int index)
    {
		if(! isAttContinuous(attr))
			return -1;

		return m_attrs[attr].getSplit(index);
    }


    int clearDiscretValue(int attr)
    {
		if(isAttContinuous(attr))
			return -1;

		return m_attrs[attr].clearDiscretValue();
    }


    // add the discret value into the nominal attribute
	void addDiscretValue(int attindex, CString value)
    {
		if(attindex < 0 || attindex >= m_attrs.GetSize())
			return;

		m_attrs[attindex].addDiscretValue(value);
    }

	void addSplit(int attindex, double value)
    {
		if(attindex < 0 || attindex >= m_attrs.GetSize())
			return;

		m_attrs[attindex].addSplit(value);
    }

    int appendAttribute(const CAttribute& src)
    {
        CAttribute pAttribute(src);
        return m_attrs.Add(pAttribute);
    }

    int appendAttribute()
    {
        CAttribute pAttribute;
        return m_attrs.Add(pAttribute);
    }

    void appendAttributes(CArray<CAttribute, CAttribute&>& srcAtt)
    {
		m_attrs.Append(srcAtt);
    }

    int serialize (CArchive& pArchive)
    {
        int nStatus = 0, i, size;
        CInstance pInstance;

        // Serialize the object ...
        if (pArchive.IsStoring())
        {
            pArchive << m_attrs.GetSize();
            for(i = 0; i < m_attrs.GetSize(); i++)
                m_attrs[i].serialize(pArchive);
        }
        else
        {
			m_attrs.RemoveAll();
			clearData();

            pArchive >> size;
            for(i = 0; i < size; i++)
            {
		        CAttribute pAttr;
                pAttr.serialize(pArchive);
                m_attrs.Add(pAttr);
            }
        }

        return (nStatus);
    }

    void dump()
    {
        int i, j;

	    m_LogTrace.WriteLine("\n\n-------------------- Data Set --------------------\n");
        
		m_LogTrace.WriteLine("\nATTRIBUTES : %d\n\n", m_attrs.GetSize());
        for(i = 0; i < m_attrs.GetSize(); i++)
            m_attrs[i].dump();
		
        m_LogTrace.WriteLine("\nDATA\n\n");
        for(i = 0; i < case_num; i++)
		{
			for(j = 0; j < getAttSize(); j++)
				m_LogTrace.WriteLine(" %6.3lf ", m_data[i][j]);
			m_LogTrace.WriteLine("\n");
		}
		m_LogTrace.WriteLine("\n");

	    m_LogTrace.WriteLine("\n--------------------------------------------------\n\n");
		
    }


public:
	short **m_fullData;

    // the real data
	double **m_data;

    // the attributes
	CArray<CAttribute, CAttribute&> m_attrs;

	int case_num;

	bool m_c45fied;
private:

    // used for data loading from other sources
    int Delimiter;
};

#endif // !defined(AFX_DATASET_H__E98C301B_D7C3_11D6_B3F6_0050040A6622__INCLUDED_)

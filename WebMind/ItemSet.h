///////////////////////////////////////////////////////////////
// ItemSet.h: interface for the CItemSet class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSET_H__119A0646_7BB6_468C_8EA2_9FE98825923B__INCLUDED_)
#define AFX_ITEMSET_H__119A0646_7BB6_468C_8EA2_9FE98825923B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LilacObject.h"

typedef int Item;

// Relationship between two ItemSet s
#define TOTALEQUAL 0
#define MAKEUP     1
#define TOTALDIFF  2
#define CROSS      3


class CItemSet : public CLilacObject
{

public:

    CItemSet();
    ~CItemSet();

    void add(Item theitem);
    void add(int index, Item theitem);
    
    void concat(CItemSet *src);
    void clear();
    CLilacObject *clone();
    int compare(CLilacObject *obj);
    Item get(int index);
    int indexOf(Item theitem, bool ascend = true);
    Item remove(int index);

    CItemSet *left(int nCount);

    CItemSet *sub(int bgn, int end);
    CItemSet *substract(CItemSet *aset);

  
    void dump();
    
    bool keeporder()									{		return m_keeporder;			}
    void keeporder(bool value)							{		m_keeporder = value;		}
    
    int size()											{		return count;				}
    
    long support()										{		return m_support;			}
    void support(long value)							{		m_support = value;			}
    
    double weight()										{		return m_weight;			}
    void weight(double value)							{		m_weight = value;			}

    int serialize (CArchive& pArchive)
    {
        int size, pageno;

        // Serialize the object ...
        if (pArchive.IsStoring())
        {
            pArchive << count;
			if(count > 0)
			{
				for(int i = 0; i < count; i++)
					pArchive << m_items[i];
			}
        }
        else
        {
            pArchive >> size;
			if(size > 0)
			{
				for(int j = 0; j < size; j++)
				{
					pArchive >> pageno;
					add(pageno);
				}
			}
        }

        return 0;
    }
	
private:

    Item *m_items;
    int count;

    bool m_keeporder;
    
    long m_support;
    double m_weight;
	
};


#endif // !defined(AFX_ITEMSET_H__119A0646_7BB6_468C_8EA2_9FE98825923B__INCLUDED_)

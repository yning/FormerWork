///////////////////////////////////////////////////////////////
// ItemSet.cpp: implementation of the CItemSet class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ItemSet.h"
#include "LilacObject.h"
#include "Utility.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemSet::CItemSet()
{
	m_items = (Item *)NULL;
    count = 0;
    m_support = 0;
    m_weight = 0.0;
    m_keeporder = true;
}

CItemSet::~CItemSet()
{
	delete m_items;
}



//---------------------------------------------------------------------------
//    CItemSet Methods
//---------------------------------------------------------------------------

void CItemSet::add(Item theitem)
{
    Item *newitems;
    int i;

    if(!m_keeporder && (indexOf(theitem) >= 0))
        return;

    newitems = (Item *) new Item[count+1];

    for(i = 0; i < count; i++)
        newitems[i] = m_items[i];
    newitems[i] = theitem;
    delete m_items;

    m_items = newitems;
    count++;

    return;
}



void CItemSet::add(int index, Item theitem)
{
    Item *newitems;
    int i;

    if(!m_keeporder && (indexOf(theitem) >= 0))
        return;

    if(index < 0)
        index = 0;
    else if(index >= count)
        index = count;

    newitems = (Item *) new Item[count+1];

    for(i = 0; i < index; i++)
        newitems[i] = m_items[i];
    newitems[index] = theitem;
    for(i = index; i < count; i++)
        newitems[i+1] = m_items[i];

    delete m_items;
    m_items = newitems;

    count++;

    return;
}


void CItemSet::concat(CItemSet *src)
{

    for(int i = 0; i < src->size(); i++)
        add(src->get(i));

    return;
}


void CItemSet::clear()
{
    if(m_items != (Item *)NULL)
        delete m_items;
    m_items = (Item *)NULL;

    count = 0;
    m_support = 0;
    m_weight = 0.0;
}


CLilacObject * CItemSet::clone()
{
    CItemSet *theclone;
    int i;

    theclone = (CItemSet *) new CItemSet();
	theclone->keeporder(m_keeporder);

    for (i = 0; i < count; i++)
        theclone->add(m_items[i]);

    theclone->support(m_support);
    theclone->weight(m_weight);

    return(theclone);
}


int CItemSet::compare(CLilacObject *obj)
{
    CItemSet *target = (CItemSet *)obj;
	int numFound = 0, pitem, pos, srcp, targetp, j;
	int result = TOTALDIFF;
    bool found;

	if(!m_keeporder)
	{
		for(int i = 0; i < count; i++)
		{
			pitem = m_items[i];
			if((pos = target->indexOf(pitem)) >= 0)
				++numFound;
		}
	}
	else
	{
	    srcp = 0;
	    targetp = -1;
		while(srcp < count)
	    {
		    found = false;
			for(j = targetp+1; j < target->size(); j++)
	        {
		        if(m_items[srcp] == target->get(j))
			    {
				    numFound++;
					targetp = j;
	                found = true;
		            break;
			    }
	        }
	
		    if(!found)
			    break;
	        else
		        srcp++;
		}
	}

    if(numFound > 0)
    {
        if((numFound == count) && (target->size() == count))
            result = TOTALEQUAL;
        else if((numFound == count) && (count < target->size()))
            result = MAKEUP;
        else
            result = CROSS;
    }

    return result;
}



Item CItemSet::get(int index)
{

    if((index >= 0) && (index < count))
        return m_items[index];
    else
        return(-1);
}


int CItemSet::indexOf(Item theitem, bool ascend)
{
	int i;

	if(ascend)
	{
	    for(i = 0; i < count; i++)
		{
			if(m_items[i] == theitem)
				return i;
	    }
	}
	else
	{
	    for(i = count-1; i >= 0; i--)
		{
			if(m_items[i] == theitem)
				return i;
		}
	}

    return(-1);
}


Item CItemSet::remove(int index)
{
    Item *newitems;
    int number = 0;
    Item result = -1;

    newitems = (Item *) new Item[count-1];

    for(int i = 0; i < count; i++)
    {
        if(i != index)
            newitems[number++] = m_items[i];
        else
            result = m_items[i];
    }

    delete m_items;
    m_items = newitems;
    count--;

    return(result);

}



CItemSet * CItemSet::left(int nCount)
{
    CItemSet *result = (CItemSet *)NULL;

    if((nCount >= 0) && (nCount < count))
    {
        result = (CItemSet *) new CItemSet();
        for(int i = 0; i < nCount; i++)
            result->add(m_items[i]);
    }

    return result;
}





CItemSet * CItemSet::sub(int bgn, int end)
{
    CItemSet *result = (CItemSet *)NULL;

    if((bgn >= 0) && (bgn < count) && (end >= 0) && (end < count))
    {
        result = (CItemSet *) new CItemSet();
        for(int i = bgn; i <= end; i++)
            result->add(m_items[i]);
    }

    return result;
}



CItemSet * CItemSet::substract(CItemSet *aset)
{
    CItemSet *result;
    int pos, oneitem;

    result = (CItemSet *)clone();

    for(int i = 0; i < aset->size(); i++)
    {
        oneitem = aset->get(i);
        pos = result->indexOf(oneitem);
        if(pos >= 0)
            result->remove(pos);
    }

    return(result);
}


void CItemSet::dump()
{
    for(int i = 0; i < count; i++)
        m_LogTrace.WriteLine(" %d ", m_items[i]);

    m_LogTrace.WriteLine(" (%d)\n", m_support);
}


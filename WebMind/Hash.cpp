///////////////////////////////////////////////////////////////
// Hash.cpp: A Hash class which holds objects of type Object.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Hash.h"

Hash::Hash(int size)
{
    hash_size = size;
	head = (HashNode **) new HashNode *[hash_size];
	if(head == NULL)
	{
		MessageBox(NULL, _T("0000"), NULL, 0);
	}
    memset(head, 0, hash_size* sizeof(HashNode *));
    //for(int i = 0; i < hash_size; i++)
     // head[i] = (HashNode *)NULL;
    
    number = 0;

	m_currentIndex = -1;
	m_currentNode = (HashNode *)NULL;
}


Hash::~Hash()
{
    clear();

	delete head;
	head = NULL;
}


void Hash::insert(CLilacObject *object)
{
		if( hash_size == 0 ) return;
/*		if( lookup(object) ) return;

		// Add first in bucket
		int slot = hashValue(object->key()) % hash_size;
		HashNode* pItem = new HashNode;
		pItem->object = object;
		pItem->next = head[slot];
		head[slot] = pItem;
*/
    HashNode *node = new HashNode;
   int slot = hashValue(object->key()) % hash_size;
    node->object = object;
    node->next = head[slot];
    head[slot] = node;
    
    number++;
}


CLilacObject *Hash::lookup(CLilacObject *object)
{
   
	if( hash_size == 0 || object == NULL) return NULL;


	int slot = hashValue(object->key()) % hash_size;
	//if(slot >= hash_size) return NULL;
	if(slot == -5)
	{
		int t = 0;
		int addr = (int)head[-5];
		slot = hashValue(object->key()) % hash_size;
	}

	for(HashNode* node = head[slot]; node != NULL; node = node->next ) { //const 
		if(object->compare(node->object) == 0)
			return node->object; 
	}

	return NULL;
	/* 
	HashNode *node;
    int d=object->key();
	int x=hashValue(d);

    for(node = head[hashValue(object->key())]; node != NULL; node = node->next)
    {
        if(node!=INVALID_HANDLE_VALUE&&object->compare(node->object) == 0)
            return node->object;
    }
    return NULL;*/
}


void Hash::clear()
{
    HashNode *node;

    ASSERT(head);

    for(int i = 0; i < hash_size; i++)
    {
    	node = head[i];
        while(node != NULL)
        {
            head[i] = node->next;
    	    delete node->object;
        	delete node;
            node = head[i];
        }
        
        head[i] = NULL;
    }

    number = 0;

	m_currentIndex = -1;
	m_currentNode = (HashNode *)NULL;
}


List * Hash::toList()
{
    List *result = (List *)new List();
    HashNode *node;
    
    for(int i = 0; i < hash_size; i++)
    {
    	node = head[i];
        while(node != NULL)
        {
            result->add(node->object->clone());
            node = node->next;
        }
    }
    
    return result;
}



// get the first available object, maybe not in the 
// insert order
CLilacObject * Hash::getFirstObject()
{
	m_currentIndex = -1;
	m_currentNode = (HashNode *)NULL;
    
    for(int i = 0; i < hash_size; i++)
    {
		if(head[i] != NULL)
		{
			m_currentIndex = i;
			m_currentNode = head[i];
			return m_currentNode->object;
        }
    }
    
    return NULL;
}

// get the next available object, maybe not in the 
// insert order
CLilacObject * Hash::getObject()
{
	if(m_currentIndex < 0)
		return getFirstObject();

	if(m_currentIndex >= hash_size)
		return NULL;

	if(m_currentNode->next != NULL)
	{
		m_currentNode = m_currentNode->next;
		return m_currentNode->object;
	}
	else
	{
		++m_currentIndex;
		if(m_currentIndex == hash_size)
			return NULL;

		for(; m_currentIndex < hash_size; m_currentIndex++)
		{
			if(head[m_currentIndex] != NULL)
			{
				m_currentNode = head[m_currentIndex];
				return m_currentNode->object;
			}
		}

	}

	return NULL;
}


void Hash::dump()
{
    HashNode *node;
    
    for(int i = 0; i < hash_size; i++)
    {
    	node = head[i];
        while(node != NULL)
        {
            node->object->dump();
            node = node->next;
        }
    }
}

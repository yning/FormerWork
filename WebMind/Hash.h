///////////////////////////////////////////////////////////////
// Hash.h: A Hash class which holds objects of type Object.
//
//
///////////////////////////////////////////////////////////////

#ifndef	_HASH_H_
#define	_HASH_H_

#include "LilacObject.h"
#include "List.h"

struct hashnode
{
    CLilacObject *object;
    struct hashnode	*next;
};


typedef struct hashnode HashNode;

class Hash
{
public:
	void dump();
	
    // Constructor/Destructor
    Hash(int size = 100);
    ~Hash();
	
    // add() will append an Object to the end of the hash
    void insert(CLilacObject *);
	
    // look up the object in the hash table, 
    // return the object, otherwise NULL
    CLilacObject * lookup(CLilacObject *);
	
    // clear() will set the hash to empty.  This call will 
    // DELETE objects that were in the hash before this call.
    void clear();
	
    // convert the Objects in hash to list
    List* toList();
    
    // Access to the number of elements
    int	size()									
	{			
		return number;				
	}
	
	CLilacObject *getFirstObject();
	CLilacObject *getObject();
	
	void resetSeek()
	{
		m_currentIndex = -1;
		m_currentNode = (HashNode *)NULL;
	}
	
protected:
    int hashValue(int key)								
	{			
		return key % hash_size;		
	}
    int hashValue(long key)								
	{			
		return key % hash_size;		
	}
	
private:
    // Pointers into the list
    HashNode **head;
	
    // Its nice to keep track of how many things we contain...
    int number;
    
    int hash_size;
	
	int m_currentIndex;
	HashNode* m_currentNode;
};

#endif

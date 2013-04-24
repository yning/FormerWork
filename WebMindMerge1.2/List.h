///////////////////////////////////////////////////////////////
// List.h: interface for the List class.
//
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_LIST_H__C59685F0_2F21_4084_A072_D86EB4E57559__INCLUDED_)
#define AFX_LIST_H__C59685F0_2F21_4084_A072_D86EB4E57559__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LilacObject.h"

typedef struct listnode ListNode;
struct listnode
{
    CLilacObject		*object;
    
    struct listnode	*next;
    struct listnode	*prev;
};



class List
{
public:
	int RindexOf(CLilacObject *obj);

    // Constructor/Destructor
    List();
    ~List();

    // add() will append an Object to the end of the list
    void add(CLilacObject *);

    // add() will insert an object at the given position.  If the
    // position is larger than the number of objects in the list, the
    // object is appended; no new objects are created between the end
    // of the list and the given position.
    void	add(int position, CLilacObject *object);

	// append the objects in a list to this list
    void	add(List *plist);

    // clear() will set the list to empty.  This call will 
    // DELETE objects that were in the list before this call.
    void	clear();

    // Deep copy member function
    List *clone();

    // Direct access to list items.  This can only be used to retrieve
    // objects from the list.
    CLilacObject *get(int index);

    // Get the index number of an object.  If the object is not found,
    // returnes -1
    int			indexOf(CLilacObject *);

    // Remove the object at the given position.
    void remove(int position);

    // Find the given object in the list and remove it from the list.
    // The object will NOT be deleted.  If the object is not found,
    // NOTOK will be returned, else OK.
    void		remove(CLilacObject *);


    // Remove the node from the list, should set the current pointer.
    void remove(ListNode *node);

    // Access to the number of elements
    int			size() {return number;}

    // set() will replace the object already at the given position
    // with the new object.  If there is no object at the position,the
    // list is extended with nil objects until the position is reached
    // and then the given object is put there.  (This really makes the
    // List analogous to a dynamic array...)
    void	set(int position, CLilacObject *object);
    
    void	dump();

	void	keepContent(bool value = true)
	{
		m_keepContent = value;
	}


private:
    // Pointers into the list
    listnode		*head;
    listnode		*tail;

    // For list traversal it is nice to know where we are...
    ListNode		*current;
    int			current_index;

    // Its nice to keep track of how many things we contain...
    int			number;

	// when we want to delete the list, whether we want to
	// keep the content untouched or not.
	// true : only delete the list node.
	// flase : delete the list node and content
	bool		m_keepContent;
};

#endif // !defined(AFX_LIST_H__C59685F0_2F21_4084_A072_D86EB4E57559__INCLUDED_)

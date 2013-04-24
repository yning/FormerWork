///////////////////////////////////////////////////////////////
// List.cpp: implementation of the List class.
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "List.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//*********************************************************************
// List::List()
//   Constructor
//
List::List()
{
    head = tail = current = (ListNode *)NULL;
    number = 0;
    current_index = -1;
	m_keepContent = false;
}


//*********************************************************************
// List::~List()
//   Destructor
//
List::~List()
{
    clear();
}


//*********************************************************************
// void List::add(CLilacObject *object)
//   Add an object to the list.
//
void List::add(CLilacObject *object)
{
    ListNode *node = (ListNode *)new ListNode;
    
    node->next = (ListNode *)NULL;
    node->prev = tail;
    node->object = object;
    
    if (tail != (ListNode *)NULL)
    {
    	tail->next = node;
    	tail = node;
    }
    else
    	head = tail = node;

    number++;
}


//*********************************************************************
// void List::add(int position, CLilacObject *object)
//   Adds the specified item to the the scrolling list at the 
//   position indicated by the index.
//
void List::add(int position, CLilacObject *object)
{
    ListNode *ln = head;
    
    for (int i = 0; i < position && ln; i++, ln = ln->next)
    	;
    
    if (!ln)
    {
        // if position is greater than the current scope, 
        // append the object to the list
        add(object);
    }
    else
    {
        ListNode *node;
		
		node = (ListNode *)new ListNode;
    
        node->next = (ListNode *)NULL;
        node->prev = (ListNode *)NULL;
        node->object = object;
        
    	if (ln == head)
	    {
	        node->next = head;
    	    node->next->prev = node;
	        head = node;
    	}
	    else
    	{
	        node->next = ln;
	        node->prev = ln->prev;
    	    node->prev->next = node;
	        node->next->prev = node;
    	}
        
        if(current_index >= position)
            current_index++;
        
        number++;
    }
    
    return;
}



// Append plist to the current list
void List::add(List *plist)
{
	CLilacObject *pobject;
	for(int i = 0; i < plist->size(); i++)
	{
		pobject = plist->get(i);
		add(pobject->clone());
	}

	return;
}


//*********************************************************************
// void List::clear()
//   Removes all nodes from this list, and the objects as well.
//
void List::clear()
{
    ListNode *node;
    
    while (head)
    {
    	node = head;
    	head = head->next;

		if(!m_keepContent&&!node)
			delete node->object;
		if (node!=NULL)
    		delete node;
    }
    
    head = tail = current = (ListNode *)NULL;
    number = 0;
    current_index = -1;
}


//*********************************************************************
// void List::dump()
//   dump all the content in the link list.
//
void List::dump()
{
    for(ListNode *node = head; node != (ListNode *)NULL; node = node->next)
        node->object->dump();
}



//*********************************************************************
// List *List::clone()
//   Return a deep copy of the list.
//
List *List::clone()
{
    List	*list = new List();
    ListNode *node;

    for(node = head; node != (ListNode *)NULL; node = node->next)
        list->add(node->object->clone());
        
    return list;
}



//*********************************************************************
// CLilacObject *List::get(int index)
//   Gets the item associated with the specified index.
//
CLilacObject *List::get(int index)
{
    if (index < 0 || index >= number)
        return (CLilacObject *)NULL;

    // if we want to retrieve exact the current node or the node next to current
    if (current_index == index)
    	return current->object;
    if (current && current_index >= 0 && index == current_index + 1)
    {
    	current = current->next;
	    if (!current)
    	{
	        current_index = -1;
    	    return (CLilacObject *)NULL;
	    }
    	current_index = index;
	    return current->object;
    }

    ListNode *temp = head;
    
    for (int i = 0; temp && i < index; i++)
    	temp = temp->next;

    if (temp)
    {
    	current_index = index;
	    current = temp;
    	return temp->object;
    }
    
   	return (CLilacObject *)NULL;
}



//*********************************************************************
// int List::indexOf(CLilacObject *obj)
//    Searches for the first occurence of the given argument, 
//    testing for equality using the equals method.
//
int List::indexOf(CLilacObject *obj)
{
    ListNode *temp = head;
    int			index = 0;

    while (temp && obj->compare(temp->object) != 0)
    {
    	temp = temp->next;
	    index++;
    }
    
    if (index >= number)
    	return -1;
    else
    	return index;
}


int List::RindexOf(CLilacObject *obj)
{
    ListNode *temp = tail;
    int			index = number - 1;

    while (temp && obj->compare(temp->object) != 0)
    {
    	temp = temp->prev;
	    index--;
    }
    
    if (index < 0)
    	return -1;
    else
    	return index;
}



//*********************************************************************
// void List::remove(int position)
//   Remove the item at the specified position from this scrolling list.
//
void List::remove(int position)
{
    if (position < 0 || position >= number)
        return;
    
    ListNode *node;
    
    if (current_index == position)
        node = current;
    else if (current && current_index >= 0 && position == current_index + 1)
    	node = current->next;
    else
    {
        node = head;
    
        for (int i = 0; node && i < position; i++)
         	node = node->next;
    }
    
    if(node)
        remove(node);

    return;    
}

//*********************************************************************
// void List::remove(CLilacObject *object)
//   Removes the first occurrence of an item from the list.
//
void List::remove(CLilacObject *object)
{
    
    for(ListNode *node = head; node != (ListNode *)NULL; node = node->next)
    {
    	if (object->compare(node->object) == 0)
    	{
            remove(node);
            break;
    	}
    }
    
    return;
}


//*********************************************************************
// void List::remove(ListNode *node)
//   Remove the node from this scrolling list.
//
void List::remove(ListNode *node)
{
    
    if (head == tail)
    {
        // if only one node in the list
        head = tail = (ListNode *)NULL;
    }
    else if (head == node)
    {
        head = head->next;
        head->prev = (ListNode *)NULL;
    }
    else if (tail == node)
    {
        tail = tail->prev;
        tail->next = (ListNode *)NULL;
    }
    else
    {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }

    delete node->object;
    delete node;
    
    number--;
    
    // we should keep the current correctly, but ...
    current_index = -1;
    current = (ListNode *)NULL;
    
    return;
}


//*********************************************************************
// void List::set(int position, CLilacObject *object)
//   Replaces the item at the specified index in the scrolling list with the new object.
//
void List::set(int position, CLilacObject *object)
{
    ListNode *temp = head;
    
    //
    // First make sure that there is something there!
    //
    while (number < position + 1)
    	add((CLilacObject *)NULL);

    //
    // Now find the listnode to put the new object in
    //
    for (int i = 0; temp && i < position; i++)
    	temp = temp->next;

    if(temp->object != (CLilacObject *)NULL)
        delete temp->object;
    
    temp->object = object;
    
    current = temp;
    current_index = position;
    
    return;
}


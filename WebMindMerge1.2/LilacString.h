///////////////////////////////////////////////////////////////
// CLilacString.h: interface for the CLilacString class.
//
///////////////////////////////////////////////////////////////

#if !defined(AFX_TZSTRING_H__6A906335_5187_4145_80B4_92C7EF512939__INCLUDED_)
#define AFX_TZSTRING_H__6A906335_5187_4145_80B4_92C7EF512939__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LilacObject.h"

class CLilacObject;
class ostream;

class CLilacString : public CLilacObject
{
public:
	int key();
    CLilacString()	{Length = 0,Allocated = 0;}			// Create an empty string
    CLilacString(int init);								// initial allocated length
    CLilacString(const char *s);						// from null terminated s
    CLilacString(char *s, int len);						// from s with length len
    CLilacString(const CLilacString &s);				// Copy constructor

    // This can be used for performance reasons if it is known the
    // String will need to grow.
    CLilacString(const CLilacString &s, int allocation_hint);
	
    ~CLilacString();

    int			length() const;
    char		*get() const;
    operator 		char*()	{return get();}

    // Interpretation
    int			as_integer(int def = 0);

    // If it is not posible to use the constructor with an initial
    // allocation size, use the following member to set the size.
    void		allocate(int init)	{reallocate_space(init);}

    // allocate space for a new char *, and cope the String in.
    char		*new_char() const;

    // Assignment
    void		operator = (const CLilacString &s);
    void		operator = (char *s);
    void		operator += (CLilacString &s);
    void		operator += (char *s);

    // Appending
    inline CLilacString		&operator << (char *);
    inline CLilacString		&operator << (char);
    CLilacString		&operator << (unsigned char c) {return *this<<(char)c;}
    CLilacString		&operator << (int);
    CLilacString		&operator << (long);
    CLilacString		&operator << (short i)		{return *this<<(int)i;}
    CLilacString		&operator << (CLilacString &);
    CLilacString		&operator << (CLilacString *s)	{return *this << *s;}

    // Access to specific characters
    char		&operator [] (int n);
    char		last();

    void        removeLast()
    {
        if(Length > 0)
            Data[Length -1] = '\0';
    }

    // Removing
    char		operator >> (char c);
									
    // Comparison
    //  Return:
    //	 0 : 'this' is equal to 's'.
    //	-1 : 'this' is less than 's'.
    //	 1 : 'this' is greater than 's'.
    int			compare(CLilacObject *obj);
    int			nocase_compare(CLilacString &s);

    // Searching for parts
    int			lastIndexOf(char c);
    int			lastIndexOf(char c, int pos);
    int			indexOf(char c);
    int			indexOf(char c, int pos);
    int			indexOf(char *);
    int			indexOf(char *, int pos);
    
    // Manipulation
    void		append(CLilacString &s);
    void		append(char *s);
    void		append(char *s, int n);
    void		append(char ch);

    CLilacString		&chop(int n = 1);
    CLilacString		&chop(char ch = '\n');
    CLilacString		&chop(char *str = "\r\n");

    // SubStrings
    // The string starting at postion 'start' and length 'len'.
    CLilacString		sub(int start, int len) const;
    CLilacString		sub(int start) const;


    // Non-member operators
    friend CLilacString	operator +  (CLilacString &a, CLilacString &b);
    friend int		operator == (CLilacString &a, CLilacString &b);
    friend int		operator != (CLilacString &a, CLilacString &b);
    friend int		operator <  (CLilacString &a, CLilacString &b);
    friend int		operator >  (CLilacString &a, CLilacString &b);
    friend int		operator <= (CLilacString &a, CLilacString &b);
    friend int		operator >= (CLilacString &a, CLilacString &b);


    void		lowercase();
    void		uppercase();

    void		replace(char c1, char c2);
    void		remove(char *);

    CLilacObject		*clone();


private:
    int			Length;		// Current Length
    int			Allocated;	// Total space allocated
    char		*Data;		// The actual contents

    void		copy_data_from(const char *s, int len, int dest_offset = 0);
    void		copy(char *s, int len, int allocation_hint);

    // Possibly make Data bigger.
    void		reallocate_space(int len);

    // Allocate some space for the data.  Delete Data if it
    // has been allocated.
    void		allocate_space(int len);

    // Allocate some space without rounding
	 void 	allocate_fix_space(int len);
	
};

extern char *form(char *, ...);
extern char *vform(char *, va_list);

// Inline methods.
inline CLilacString &CLilacString::operator << (char *str)
{
    append(str);
    return *this;
}

inline CLilacString &CLilacString::operator << (char ch)
{
    append(ch);
    return *this;
}

inline int CLilacString::length() const
{
    return Length;
}

inline void CLilacString::operator += (CLilacString &s)
{
    append(s);
}

inline void CLilacString::operator += (char *s)
{
    append(s);
}


#endif // !defined(AFX_TZSTRING_H__6A906335_5187_4145_80B4_92C7EF512939__INCLUDED_)

///////////////////////////////////////////////////////////////
// CLilacString.cpp: implementation of the CLilacString class.
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "LilacObject.h"
#include "LilacString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Implementation of String class

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>



const int MinimumAllocationSize = 4;	// Should be power of two.

CLilacString::CLilacString(int init)
{
	Length = 0;
    Allocated = init;
    Data = new char[init];
}

CLilacString::CLilacString(const char *s)
{
    Allocated = 0;
    Length = 0;

    int	len;
    if (s)
    {
        len = strlen(s);
        copy((char *)s, len, len);
    }
}

CLilacString::CLilacString(char *s, int len)
{
    Allocated = 0;
    Length = 0;
    if (s && len != 0)
        copy(s, len, len);
}

CLilacString::CLilacString(const CLilacString &s)
{
    Allocated = 0;
    Length = 0;
    if (s.length() != 0)
        copy(s.Data, s.length(), s.length());
}

//
// This can be used for performance reasons if it is known the
// String will need to grow.
//
CLilacString::CLilacString(const CLilacString &s, int allocation_hint)
{
    Allocated = 0;
    Length = 0;
    if (s.length() != 0)
    {
        if (allocation_hint < s.length())
            allocation_hint = s.length();
        copy(s.Data, s.length(), allocation_hint);
    }
}

CLilacString::~CLilacString()
{
    if (Allocated)
        delete [] Data;
}

void CLilacString::operator = (const CLilacString &s)
{
    allocate_space(s.length());
    Length = s.length();
    copy_data_from(s.Data, Length);
}

void CLilacString::operator = (char *s)
{
    if (s)
    {
        int len = strlen(s);
        allocate_fix_space(len);
        Length = len;
        copy_data_from(s, Length);	
    }
    else
        Length = 0;
}

void CLilacString::append(CLilacString &s)
{
    if (s.length() == 0)
        return;
    int	new_len = Length + s.length();

    reallocate_space(new_len);
    copy_data_from(s.Data, s.length(), Length);
    Length = new_len;
}

void CLilacString::append(char *s)
{
    if (!s)
        return;
	
    append(s,strlen(s));
}

void CLilacString::append(char *s, int slen)
{
    if (!s || !slen)
        return;

    int	new_len = Length + slen;

    if (new_len + 1 > Allocated)
        reallocate_space(new_len);
    copy_data_from(s, slen, Length);
    Length = new_len;
}

void CLilacString::append(char ch)
{
    int new_len = Length +1;
    if (new_len + 1 > Allocated)
    	reallocate_space(new_len);
    Data[Length] = ch;
    Length = new_len;
}

int CLilacString::compare(CLilacObject *obj)
{
    CLilacString	*s = (CLilacString *) obj;
    int	len;
    int	result;
    char	*p1 = Data;
    char	*p2 = s->Data;

    len = Length;
    result = 0;

    if (Length > s->Length)
    {
        result = 1;
        len = s->Length;
    }
    else if (Length < s->Length)
        result = -1;

    while (len>0)//change by ny; while(len)
    {
        if (*p1 > *p2)
            return 1;
        if (*p1 < *p2)
            return -1;
        p1++;
        p2++;
        len--;
    }
    
    // Strings are equal up to the shortest length.
    // The result depends upon the length difference.
    return result;
}


char *CLilacString::get() const
{
    static char	*null = "";
    
    if (!Allocated)
    	return null;
    Data[Length] = '\0';	// We always leave room for this.
    return Data;
}

char *CLilacString::new_char() const
{
    char	*r;
    if (!Allocated)
    {
    	r = new char[1];
	    *r = '\0';
    	return r;
    }
    Data[Length] = '\0';	// We always leave room for this.
    r = new char[Length + 1];
    strcpy(r, Data);
    return r;
}


int CLilacString::as_integer(int def)
{
    if (Length <= 0)
    	return def;
    Data[Length] = '\0';
    return atoi(Data);
}


CLilacString CLilacString::sub(int start, int len) const
{
    if (start > Length)
    	return 0;

    if (len > Length - start)
    	len = Length - start;

    return CLilacString(Data + start, len);
}

CLilacString CLilacString::sub(int start) const
{
    return sub(start, Length - start);
}

int CLilacString::indexOf(char *str)
{
    char	*c;    

	// Set the first char after string end to zero to prevent finding
    // substrings including symbols after actual end of string
    if (!Allocated)
    	return -1;
    Data[Length] = '\0';
    
    if ((c = strstr(Data, str)) != NULL)
    	return(c -Data);
    
    return -1;
}

int CLilacString::indexOf(char ch)
{
    int		i;
    for (i = 0; i < Length; i++)
    {
    	if (Data[i] == ch)
	        return i;
    }
    
    return -1;
}

int CLilacString::indexOf(char ch, int pos)
{
    if (pos >= Length)
        return -1;
    for (int i = pos; i < Length; i++)
    {
    	if (Data[i] == ch)
	        return i;
    }
    return -1;
}

int CLilacString::lastIndexOf(char ch, int pos)
{
    if (pos >= Length)
    	return -1;
    while (pos >= 0)
    {
    	if (Data[pos] == ch)
	        return pos;
    	pos--;
    }
    return -1;
}

int CLilacString::lastIndexOf(char ch)
{
    return lastIndexOf(ch, Length - 1);
}

CLilacString &CLilacString::operator << (int i)
{
    char	str[20];
    sprintf(str, "%d", i);
    append(str);
    return *this;
}

CLilacString &CLilacString::operator << (long l)
{
    char	str[20];
    sprintf(str, "%ld", l);
    append(str);
    return *this;
}

CLilacString &CLilacString::operator << (CLilacString &s)
{
    append(s.get(), s.length());
    return *this;
}

char CLilacString::operator >> (char c)
{
    c = '\0';
	
    if (Allocated && Length)
    {
    	c = Data[Length - 1];
	    Data[Length - 1] = '\0';
    	Length--;
    }

    return c;
}


char CLilacString::last()
{
    if (Length > 0)
    	return Data[Length - 1];
    else
    	return 0;
}


char &CLilacString::operator [] (int n)
{
    static char	null = 0;
    
    if (n >= Length)
    	return null;
    else if (n < 0)
    	return (*this)[Length + n];
    else
    	return Data[n];
}


void CLilacString::lowercase()
{
    for (int i = 0; i < Length; i++)
    	Data[i] = tolower((unsigned char)Data[i]);
}


void CLilacString::uppercase()
{
    for (int i = 0; i < Length; i++)
    {
    	if (islower(Data[i]))
	        Data[i] = toupper((unsigned char)Data[i]);
    }
}


void CLilacString::replace(char c1, char c2)
{
    for (int i = 0; i < Length; i++)
	{
    	if (Data[i] == c1)
	        Data[i] = c2;
	}
}


void CLilacString::remove(char *chars)
{
    if (Length <= 0)
    	return;

    char	*good, *bad;
    int		skipped = 0;

    good = bad = Data;
    for (int i = 0; i < Length; i++)
    {
    	if (strchr(chars, *bad))
	        skipped++;
    	else
	        *good++ = *bad;
    	bad++;
    }
    Length -= skipped;
}


CLilacObject *CLilacString::clone()
{
    return new CLilacString(*this);
}


CLilacString &CLilacString::chop(int n)
{
    Length -= n;
    if (Length < 0)
    	Length = 0;
    return *this;
}


CLilacString &CLilacString::chop(char ch)
{
	while (Length > 0 && Data[Length - 1] == ch)
	    Length--;
    return *this;
}


CLilacString &CLilacString::chop(char *str)
{
	while (Length > 0 && strchr(str, Data[Length - 1]))
	    Length--;
    return *this;
}



//------------------------------------------------------------------------
// Non member operators.
//
CLilacString operator + (CLilacString &a, CLilacString &b)
{
    CLilacString	result(a, a.length() + b.length());
	
    result.append(b);
    return result;
}

int operator == (CLilacString &a, CLilacString &b)
{
    if (a.Length != b.Length)
		return 0;

    return a.compare((CLilacObject *) &b) == 0;
}

int operator != (CLilacString &a, CLilacString &b)
{
    return a.compare((CLilacObject *) &b) != 0;
}

int operator < (CLilacString &a, CLilacString &b)
{
    return a.compare((CLilacObject *) &b) == -1;
}

int operator > (CLilacString &a, CLilacString &b)
{
    return a.compare((CLilacObject *) &b) == 1;
}

int operator <= (CLilacString &a, CLilacString &b)
{
    return a.compare((CLilacObject *) &b) <= 0;
}

int operator >= (CLilacString &a, CLilacString &b)
{
    return a.compare((CLilacObject *) &b) >= 0;
}


//------------------------------------------------------------------------
// Private Methods.
//

void CLilacString::copy_data_from(const char *s, int len, int dest_offset)
{
    memcpy(Data + dest_offset, s, len);
}

void CLilacString::allocate_space(int len)
{
    len++;				// In case we want to add a null.

    if (len <= Allocated)
		return;

    if (Allocated)
		delete [] Data;

    Allocated = MinimumAllocationSize;
    while (Allocated < len)
		Allocated <<= 1;

    Data = new char[Allocated];
}

void CLilacString::allocate_fix_space(int len)
{
	len++;				// In case we want to add a null.

    if (len <= Allocated)
		return;

    if (Allocated)
		delete [] Data;

    Allocated = len;
    if (Allocated < MinimumAllocationSize)
		Allocated = MinimumAllocationSize;
    Data = new char[Allocated];
}

void CLilacString::reallocate_space(int len)
{
	char	*old_data = 0;
	int	 old_data_len = 0;

    if (Allocated)
	{
		old_data = Data;
	    old_data_len = Length;
	    Allocated = 0;
	}
    allocate_space(len);
    if (old_data)
	{
		copy_data_from(old_data, old_data_len);
		delete [] old_data;
	}
}

void CLilacString::copy(char *s, int len, int allocation_hint)
{
	if (len == 0 || allocation_hint == 0)
		return;         // We're not actually copying anything!
	allocate_fix_space(allocation_hint);
	Length = len;
	copy_data_from(s, len);
}


int CLilacString::key()
{
/*
	int  g, h = 0;

	for(int i = 0; i < Length; i++)
	{
		h = (h << 2) + Data[i];//tolower(Data[i]);
		if ((g = h & 0xf0000000L) != 0)
			h = (h ^ (g >> 24)) ^ g;
	}

	return h;
*/
  unsigned  int  hash  =   0 ; 
  unsigned  int  x     =   0 ; 


  //while  ( * str) 
	  for(int i = 0; i < Length; i++)
  { 
    hash  =  (hash << 4 )  +   Data[i] ; //hash值左移4位加上一个字符
    if  ((x  =  hash  &   0xF0000000L )  !=   0 )//判断hash值的高4位是否不为0，因为不为0时需要下面特殊处理，否则上面一步的左移4位会把这高四位给移走，造成信息丢失
    { 
      hash  ^=  (x >> 24 );   //把刚才的高4位跟hash的低5-8位异或
      hash  &=   ~ x;            //把高4位清0
    }  
  }  
  return  (hash  &   0x7FFFFFFF ) % 100; //希望hash值是一个非负数

}

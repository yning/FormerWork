///////////////////////////////////////////////////////////////
// PuncMark.cpp: implementation of the CPuncMark class.
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PuncMark.h"

char punctuation_marks[NUM_PUNC_MARKS] = {',', '.', '?', '/',
';', ':', '\"', '|',
'\'' ,'\\', '{', '}',
'[', ']', '~', '`',
'!', '@', '#', '$',
'%', '^', '&', '*',
'(', ')', '_', '+',
'=', '-', '0', '1',
'2', '3', '4', '5',
'6', '7', '8', '9',
'<', '>', '\r', '\n'
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPuncMark::CPuncMark()
{
}

CPuncMark::~CPuncMark()
{
}


void CPuncMark::RemovePuncMark(char *str)
{
	
    for(int i = 0; i < strlen(str); i++)
    {
       // if(((str[i] >= '0' && str[i] <= '9') || (str[i] >= '*' && str[i] <= '/')))
         //   str[i] = ' ';
		
			
			for(int j = 0; j < NUM_PUNC_MARKS; j++)
			{
            if(str[i] == punctuation_marks[j])
			str[i] = ' ';
			}
        
    }
	
    return;
}

#include "Utils.h"
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//added by kosko
CString LoadFile(CString filename)
{
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;
	
	CString res;
	const  char* fn = (LPCTSTR)filename;//CString->char*
	pFile = fopen (fn, "rb" );
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
	
	// obtain file size:
	fseek (pFile , 0 , SEEK_END);//fseek+ftell获取文件的大小
	lSize = ftell (pFile);
	rewind (pFile);//Set position indicator to the beginning
	
	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
	
	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);//fread读文件
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
	
	/* the whole file is now loaded in the memory buffer. */
	
	// terminate
	
	res.Format("%s",buffer);
	fclose (pFile);
	free (buffer);
	return res;
}
CTime ConvertStringToCTime(CString time)
{
	const char* buf=(LPCTSTR)time ;// 时间类型是这个类型的:"2004-04-12   12:05:30 " CString->char*
	int   year,   month,   day,   hour,   minute,   second; 
	sscanf(buf,   "%d-%d-%d   %d:%d:%d ",   &year,   &month,   &day,   &hour,   &minute,   &second); 
	CTime   tm(year,   month,   day,   hour,   minute,   second); 
	return tm;
}
CString ConvertCtimeToCString(CTime time)
{
	return time.Format("%Y-%m-%d %H:%M:%S");
}





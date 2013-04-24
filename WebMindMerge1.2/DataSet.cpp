///////////////////////////////////////////////////////////////
// DataSet.cpp: implementation of the CDataSet class.
//
//
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataSet.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataSet::CDataSet()
{
	case_num = 0;
	m_data = NULL;
	m_fullData = NULL;

	m_c45fied = false;
}

CDataSet::~CDataSet()
{
	if(m_data)
	{
		for(int i = 0; i < case_num; i++)
			delete m_data[i];
		delete m_data;
	}

	if(m_fullData)
	{
		for(int i = 0; i < case_num; i++)
			delete m_fullData[i];
		delete m_fullData;
	}
}

void CDataSet::clearData()
{
	if(m_data)
	{
		for(int i = 0; i < case_num; i++)
			delete m_data[i];
		delete m_data;

        m_data = NULL;
	}
}


void CDataSet::Swap(int src, int target)
{
    if(src == target)
        return;

	double buffer;

	for(int i = 0; i < getAttSize(); i++)
	{
		buffer = m_data[src][i];
		m_data[src][i] = m_data[target][i];
		m_data[target][i] = buffer;
	}
}

void CDataSet::Quicksort(int Fp, int Lp, int Att)
{
    int Lower, Middle;
    double Thresh;
    int i;

    if ( Fp < Lp )
    {
        Thresh = m_data[Lp][Att];

    	/*  Isolate all items with values <= threshold  */
    	Middle = Fp;

        for ( i = Fp ; i < Lp ; i++ )
        { 
            if ( m_data[i][Att] <= Thresh )
            { 
                if ( i != Middle )
                    Swap(Middle, i);
                Middle++; 
            } 
        } 

        /*  Extract all values equal to the threshold  */
        Lower = Middle - 1;
        for ( i = Lower ; i >= Fp ; i-- )
	    {
	        if ( m_data[i][Att] == Thresh )
	        { 
		        if ( i != Lower )
                    Swap(Lower, i);
                Lower--;
            } 
        } 

        /*  Sort the lower values  */
        Quicksort(Fp, Lower, Att);

        /*  Position the middle element  */
		Swap(Middle, Lp);

	    /*  Sort the higher values  */
    	Quicksort(Middle+1, Lp, Att);
    }
}


void CDataSet::calPrecision()
{
    double lastVal, currentVal, deltaSum, precision;
    int attindex, distinct;

    for(attindex = 0; attindex < getAttSize(); attindex++)
    {
        if(!isAttContinuous(attindex))
            continue;

        Quicksort(0, case_num-1, attindex);

        deltaSum = 0.0;
        distinct = 0;
        precision = 0.01;
        lastVal = m_data[0][attindex];
        for(int i = 1; i < case_num; i++)
        {
            currentVal = m_data[i][attindex];
            if(currentVal != lastVal)
            {
                deltaSum += currentVal - lastVal;
                lastVal = currentVal;
                distinct++;
            }
        }

        if(distinct > 0)
            setAttPrecision(attindex, deltaSum/(double)distinct);
    }
}


void CDataSet::quickSortAt(int attindex)
{
    if(!isAttContinuous(attindex))
        return;

    Quicksort(0, case_num-1, attindex);
}


bool CDataSet::calSplitPoints(int attindex)
{
    if(!isAttContinuous(attindex))
        return false;

	quickSortAt(attindex);

	cut(0, case_num - 1, attindex);

    if(m_attrs[attindex].getSplitNum() > 1)
        m_attrs[attindex].qucikSortSplits(0, m_attrs[attindex].getSplitNum() - 1);

	return true;
}


void CDataSet::cut(int begin, int end, int attindex)
{
	double gain,T,deta,temp;
	int size,tp;

	size = end-begin+1;
	T = find_best_cut_point(begin, end, attindex);

	if(T != m_data[begin][attindex])
	{
		calGainDeta(T, begin, end, attindex, gain, deta);

		temp = (log((double)size-1)/(double)log(2))/(double) size;
		temp += deta/(double)size;

		addSplit(attindex, T);

		if( temp < gain)
		{
			for(int i = begin ;i <= end ;i++)
			{
				if(m_data[i][attindex] >= T)
				{
					tp = i-1;
					break;
				}
			}

			cut(begin, tp, attindex);
			cut(tp+1, end, attindex);
		}
	}
}


double CDataSet::find_best_cut_point(int begin , int end, int attindex)
{
	int Stempcount=0, classindex;
	double result,rt,T,result2;
	bool flag=false,flag2=false;

	classindex = getAttSize() - 1;
	for( int j = begin ;j < end; j++)
	{
		if (m_data[j][classindex] != m_data[j+1][classindex])
		{
			flag2 = true;
			T = (m_data[j][attindex] + m_data[j+1][attindex])/2.0;

			rt = Entropy(T, begin, end, attindex);

			if(flag == false)
			{
				result = rt;
				result2 = T;
				flag = true;
			}
			else
			{
				if(result > rt)
				{
					result = rt;
					result2 = T;
				}
			}
		}
	}

	if(flag2)
		return result2;
    else
		return m_data[begin][attindex];
}


double CDataSet::Entropy(double T , int begin, int end, int attindex)
{
	double result = 0.0, entlower = 0.0, entgande = 0.0, p;
	CArray<double, double&> lower;
	CArray<double, double&> gande;
	int i, att_num, size;

	size = end - begin + 1;
	att_num = getAttSize();

	lower.SetSize(getDiscretNumber(att_num - 1));
	gande.SetSize(getDiscretNumber(att_num - 1));
	for(i = 0; i < lower.GetSize(); i++)
	{
		lower.SetAt(i, result);
		gande.SetAt(i, result);
	}
	
	if(size > 0)
	{

		int count_s1=0,count_s2=0;
		for(i = begin; i <= end; i++)
		{
			if(m_data[i][attindex] < T)
			{
				lower[(int)m_data[i][att_num-1]] += 1.0;
				count_s1 += 1;
			}
			else
			{
				gande[(int)m_data[i][att_num-1]] += 1.0;
				count_s2 += 1;
			}
		}

		for(i = 0; i < getDiscretNumber(att_num-1); i++)
		{
			p = lower[i]/ (double)count_s1;
			if(p)
				entlower += (-(p*logl(p)/logl(2)));

			p = gande[i]/ (double)count_s2;
			if(p)
				entgande += (-(p*logl(p)/logl(2)));
		}

		result= ((double)count_s1*entlower + (double)count_s2*entgande)/(double)size;
	}

	lower.RemoveAll();
	gande.RemoveAll();

	return result;
}


void CDataSet::calGainDeta(double T, int begin, int end, int attindex, double& gain, double& deta)
{
	double result = 0.0, entlower = 0.0, entgande = 0.0, entall = 0.0, p;
	CArray<double, double&> lower;
	CArray<double, double&> gande;
	CArray<double, double&> all;
	int i, att_num, class_num, size;

	size = end - begin + 1;
	att_num = getAttSize();
	class_num = getDiscretNumber(att_num - 1);
	gain = 0.0;
	deta = 0.0;

	lower.SetSize(class_num);
	gande.SetSize(class_num);
	all.SetSize(class_num);

	for(i = 0; i < class_num; i++)
	{
		lower[i] = 0.0;
		gande[i] = 0.0;
		all[i] = 0.0;
	}
	
	if(size > 0)
	{
		int count_lower = 0,count_gande = 0, count_all = 0;
		int num_all = 0, num_lower = 0, num_gande = 0;

		for(i = begin; i <= end; i++)
		{
			all[(int)m_data[i][att_num-1]] += 1.0;
			count_all += 1;

			if(m_data[i][attindex] < T)
			{
				lower[(int)m_data[i][att_num-1]] += 1.0;
				count_lower += 1;
			}
			else
			{
				gande[(int)m_data[i][att_num-1]] += 1.0;
				count_gande += 1;
			}
		}

		for(i = 0; i < class_num; i++)
		{
			if(all[i] > 0.0)
				num_all++;
			p = all[i]/ (double)size;
			if(p)
				entall += (-(p*logl(p)/logl(2)));

			if(lower[i] > 0.0)
				num_lower++;
			p = lower[i]/ (double)count_lower;
			if(p)
				entlower += (-(p*logl(p)/logl(2)));

			if(gande[i] > 0.0)
				num_gande++;
			p = gande[i]/ (double)count_gande;
			if(p)
				entgande += (-(p*logl(p)/logl(2)));
		}

		deta = log(pow(3,num_all)-2.0)/log(2.0) - 
			(double)num_all*entall + (double)num_lower*entlower + (double)num_gande*entgande;

		gain = entall - 
			((double)count_lower*entlower + (double)count_gande*entgande)/(double)size;
	}

	lower.RemoveAll();
	gande.RemoveAll();
	all.RemoveAll();

	return;
}



#define  Space(s)	(s == ' ' || s == '\n' || s == '\t')
#define  SkipComment	while ( ( c = getc(f) ) != '\n' )
#define  Unknown	-999.0


/*************************************************************************/
/*									 */
/*  Read a name from file f into string s, setting Delimiter.		 */
/*									 */
/*  - Embedded periods are permitted, but periods followed by space	 */
/*    characters act as delimiters.					 */
/*  - Embedded spaces are permitted, but multiple spaces are replaced	 */
/*    by a single space.						 */
/*  - Any character can be escaped by '\'.				 */
/*  - The remainder of a line following '|' is ignored.			 */
/*									 */
/*************************************************************************/


bool CDataSet::ReadName(FILE *f, CString& s)
{
    int c;

    s.Empty();

    /*  Skip to first non-space character  */
    while ( ( c = getc(f) ) == '|' || c == '%' || Space(c) )
    {
    	if ( c == '|' || c == '%')
            SkipComment;
    }

    /*  Return false if no names to read  */
    if ( c == EOF )
    {
    	Delimiter = EOF;
	    return false;
    }

    /*  Read in characters up to the next delimiter  */
    while ( c != ':' && c != ',' && c != '\n' && c != '|' && c != '%' && c != '{' && c != '}' && c != EOF )
    {
    	if ( c == '.' )
	    {
	        if ( ( c = getc(f) ) == '|' || c == '%' || Space(c) )
                break;
            s.Insert(s.GetLength(), '.');
	    }

	    if ( c == '\\' )
	        c = getc(f);

	    s.Insert(s.GetLength(), c);

	    if ( c == ' ' )
	        while ( ( c = getc(f) ) == ' ' );
	    else
	        c = getc(f);
    }

    if ( c == '|' || c == '%' )
        SkipComment;

    Delimiter = c;

    /*  Strip trailing spaces  */
    s.TrimLeft();
    s.TrimRight();

    return true;
}



/*************************************************************************/
/*									 */
/*  Read the names of classes, attributes and legal attribute values.	 */
/*  On completion, these names are stored in:				 */
/*	ClassName	-  class names					 */
/*	AttName		-  attribute names				 */
/*	AttValName	-  attribute value names			 */
/*  with:								 */
/*	MaxAttVal	-  number of values for each attribute		 */
/*									 */
/*  Other global variables set are:					 */
/*	MaxAtt		-  maximum attribute number			 */
/*	MaxClass	-  maximum class number				 */
/*	MaxDiscrVal	-  maximum discrete values for any attribute	 */
/*									 */
/*  Note:  until the number of attributes is known, the name		 */
/*	   information is assembled in local arrays			 */
/*									 */
/*************************************************************************/


bool CDataSet::LoadMLC45Attributes(const char *Fn)
{
    FILE *Nf;
    CString Buffer;
    int attindex;
    CStringArray ClassName;

    /*  Open names file  */
    if ( ! ( Nf = fopen(Fn, "r") ) )
        return false;

    /*  Get class names from names file  */
    do
    {
    	ReadName(Nf, Buffer);
        ClassName.Add(Buffer);
    } while ( Delimiter == ',' );

    while ( ReadName(Nf, Buffer) )
    {
	    if ( Delimiter != ':' )
            continue;

        attindex = appendAttribute();
        setAttName(attindex, Buffer);
        setAttType(attindex, Nominal);
    
    	do
	    {
	        if ( ! ( ReadName(Nf, Buffer) ) )
                break;

	        addDiscretValue(attindex, Buffer);
        } while ( Delimiter == ',' );

	    if ( getDiscretNumber(attindex) == 1 )
	    {
	        /*  Check for special treatment  */
    	    if ( ! strcmp(Buffer, "continuous") )
	        {
                setAttType(attindex, Continuous);
                clearDiscretValue(attindex);
            }
	        else if ( ! strcmp(Buffer, "ignore") )
	        {
				m_attrs[attindex].setIgnore();
                clearDiscretValue(attindex);
	        }
        }
    }

    attindex = appendAttribute();
    setAttName(attindex, "class");
    setAttType(attindex, Nominal);
    for(int i = 0; i < ClassName.GetSize(); i++)
        addDiscretValue(attindex, ClassName[i]);

    ClassName.RemoveAll();
    fclose(Nf);

    return true;
}



/*************************************************************************/
/*									 */
/*  Read a raw case description from file Df.				 */
/*									 */
/*  For each attribute, read the attribute value from the file.		 */
/*  If it is a discrete valued attribute, find the associated no.	 */
/*  of this attribute value (if the value is unknown this is 0).	 */
/*									 */
/*  Returns the Description of the case (i.e. the array of		 */
/*  attribute values).							 */
/*									 */
/*************************************************************************/


bool CDataSet::LoadMLC45Data(const char *Fn)
{
    FILE *Df;
    CString buffer;
    int Att, index;
    char *endname;
    bool result;
	CArray<CInstance, CInstance&> m_buffer;

    /*  Open names file  */
    if ( ! ( Df = fopen(Fn, "r") ) )
        return false;

    // Read in the data
    result = ReadName(Df, buffer);
    while ( result )
    {
        CInstance pInstance;
        
        pInstance.SetSize(m_attrs.GetSize());
        index = m_buffer.Add(pInstance);

        for(Att = 0; Att < getAttSize(); Att++)
        {
    	    if ( m_attrs[Att].isIgnore() )
	        {
    		    /*  Skip this value  */
                m_buffer[index].SetAt(Att, 0.0);
    	    }
	        else if ( getAttType(Att) == Nominal )
	        {
		        /*  Discrete value  */ 
	            if ( ! ( strcmp(buffer, "?") ) )
                    m_buffer[index].SetAt(Att, Unknown);
	            else
					m_buffer[index].SetAt(Att, (double)getDiscretIndex(Att, buffer));
	        }
	        else
	        {
	    	    /*  Continuous value  */
    	        if ( ! ( strcmp(buffer, "?") ) )
                    m_buffer[index].SetAt(Att, Unknown);
                else
		            m_buffer[index].SetAt(Att, strtod((LPCTSTR)buffer, &endname));
	        }

	        result = ReadName(Df, buffer);
        }
    }

    fclose(Df);

	case_num = m_buffer.GetSize();
	m_data = (double **) new double *[case_num];
	for(index = 0; index < case_num; index++)
		m_data[index] = (double *) new double[getAttSize()];

	// copy the data to m_data
	for(index = 0; index < case_num; index++)
	{
		for(Att = 0; Att < getAttSize(); Att++)
			m_data[index][Att] = m_buffer[index][Att];
	}

	m_buffer.RemoveAll();


    return true;
}


bool CDataSet::LoadWekaARFF(const char *Fn)
{
    FILE *Df;
    CString buffer;
    int Att, index;
    char *endname;
    bool result;
	CArray<CInstance, CInstance&> m_buffer;

    /*  Open names file  */
    if ( ! ( Df = fopen(Fn, "r") ) )
        return false;

    // Read in @RELATION ts
    ReadName(Df, buffer);

    result = ReadName(Df, buffer);
    while ( result && (buffer.Find("@attribute") >= 0 || buffer.Find("@ATTRIBUTE") >= 0))
    {
        // read in the attribute name
        Att = appendAttribute();

        for(int i = 0; i < buffer.GetLength(); i++)
            if(buffer[i] == '\t')
                buffer.SetAt(i, ' ');

        if(Delimiter == '\n')
        {
            buffer = buffer.Right(buffer.GetLength() - buffer.Find(" "));
            buffer.TrimLeft();

            setAttName(Att, buffer.Left(buffer.Find(" ")));
            setAttType(Att, Continuous);
        }
        else
        {
            buffer = buffer.Right(buffer.GetLength() - buffer.Find(" "));
            buffer.TrimLeft();
            buffer.TrimRight();

            setAttName(Att, buffer);
            setAttType(Att, Nominal);

        	do
	        {
	            if ( ! ( ReadName(Df, buffer) ) )
                    break;

    	        addDiscretValue(Att, buffer);
            } while ( Delimiter == ',' );
        }

        result = ReadName(Df, buffer);
    }


    // Read in the data
    result = ReadName(Df, buffer);
    while ( result )
    {
        CInstance pInstance;
        
        pInstance.SetSize(m_attrs.GetSize());
        index = m_buffer.Add(pInstance);

        for(Att = 0; Att < getAttSize(); Att++)
        {
    	    if ( m_attrs[Att].isIgnore() )
	        {
    		    /*  Skip this value  */
                m_buffer[index].SetAt(Att, 0.0);
    	    }
	        else if ( getAttType(Att) == Nominal )
	        {
		        /*  Discrete value  */ 
	            if ( ! ( strcmp(buffer, "?") ) )
                    m_buffer[index].SetAt(Att, Unknown);
	            else
                    m_buffer[index].SetAt(Att, (double)getDiscretIndex(Att, buffer));
	        }
	        else
	        {
	    	    /*  Continuous value  */
    	        if ( ! ( strcmp(buffer, "?") ) )
                    m_buffer[index].SetAt(Att, Unknown);
                else
		            m_buffer[index].SetAt(Att, strtod((LPCTSTR)buffer, &endname));
	        }

	        result = ReadName(Df, buffer);
        }
    }

    fclose(Df);

	case_num = m_buffer.GetSize();
	m_data = (double **) new double *[case_num];
	for(index = 0; index < case_num; index++)
		m_data[index] = (double *) new double[getAttSize()];

	// copy the data to m_data
	for(index = 0; index < case_num; index++)
	{
		for(Att = 0; Att < getAttSize(); Att++)
			m_data[index][Att] = m_buffer[index][Att];
	}

	m_buffer.RemoveAll();

    return true;
}


/*
// discret all the continuous data, and save it into m_fullData
void CDataSet::Discretize(CDataSet *pData)
{
    int i, j, k, index = 0;
    CDataSet *pSource;

	m_fullData = (short **) new short *[case_num];
	for(i = 0; i < case_num; i++)
		m_fullData[i] = (short *) new short[getAttSize()];

    if(!pData)
        pSource = this;
    else
        pSource = pData;

    // if the discretization is done by using splits from
    // other dataset, then we donot need to discret its own
    // continuous attributes.
    if(pSource == this)
    {
    	for(i = 0; i < getAttSize(); i++)
	    {
		    if(isAttContinuous(i))
			    calSplitPoints(i);
	    }
    }

	// copy the data to m_data
	for(i = 0; i < case_num; i++)
    {
        for(j = 0; j < getAttSize(); j++)
        {
            if(getAttType(j) == Nominal)
            {
                m_fullData[i][j] = (short)m_data[i][j];
            }
            else
            {
                index = 0;
                for(k = 0; k < pSource->getSplitNum(j); k++)
                {
                    if(m_data[i][j] > pSource->getSplit(j, k))
                        index = k+1;
                }

                m_fullData[i][j] = (short)index;
            }
        }
    }

//		DiscretizeInstance(m_data[i], m_fullData[i]);

}
*/

// calculate the splits for discretization
void CDataSet::calSplits()
{
   	for(int i = 0; i < getAttSize(); i++)
    {
	    if(isAttContinuous(i))
		    calSplitPoints(i);
    }
}

// discret all the continuous data, and save it into m_fullData
void CDataSet::Discretize()
{
    int i, j, k, index = 0;

	m_fullData = (short **) new short *[case_num];
	for(i = 0; i < case_num; i++)
		m_fullData[i] = (short *) new short[getAttSize()];

	// copy the data to m_data
	for(i = 0; i < case_num; i++)
    {
        for(j = 0; j < getAttSize(); j++)
        {
            if(getAttType(j) == Nominal)
            {
                m_fullData[i][j] = (short)m_data[i][j];
            }
            else
            {
                index = 0;
                for(k = 0; k < getSplitNum(j); k++)
                {
                    if(m_data[i][j] > getSplit(j, k))
                        index = k+1;
                }

                m_fullData[i][j] = (short)index;
            }
        }
    }
}


// convert the data into the format that fit for C45
void CDataSet::C45fy()
{
	// if it is already c45fied, just return
	if(m_c45fied)
		return;

	// just increase the index of all the nominal 
	// attributes
	for(int i = 0; i < case_num; i++)
	{
		for(int j = 0; j < getAttSize() - 1; j++)
		{
			if(!isAttContinuous(j))
			{
				if(m_data[i][j] == -999.00)
					m_data[i][j] = 0.00;
				else
					m_data[i][j] += 1.00;
			}
		}
	}
	m_c45fied = true;

}

void CDataSet::DeC45fy()
{
	if(!m_c45fied)
		return;

	// just increase the index of all the nominal 
	// attributes
	for(int i = 0; i < case_num; i++)
	{
		for(int j = 0; j < getAttSize() - 1; j++)
		{
			if(!isAttContinuous(j))
			{
				if(m_data[i][j] == 0.00)
					m_data[i][j] = 999.00;
				else
					m_data[i][j] -= 1.00;
			}
		}
	}
	m_c45fied = true;
}

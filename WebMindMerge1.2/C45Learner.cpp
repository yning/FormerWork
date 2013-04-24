// C45Learner.cpp: implementation of the C45Learner class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C45Learner.h"

int CTreeRecord::serializeNode(CArchive& pArchive)
{
	int i, cSubset;
	DiscrValue v;
	int Bytes, attVal, nStatus = 0;
	short m_nNodeType = 0;
	CException *pException;

	TRY
	{
		// Serialize the object ...
		if (pArchive.IsStoring())
		{
			pArchive << version.GetLength();
			if(version.GetLength())
				pArchive << version;

			pArchive << NodeType;
			pArchive << Leaf;
			pArchive << Items;
			pArchive << Errors;

			ForEach(v, 0, m_pLearner->MaxClass)
				pArchive << ClassDist[v];

			if ( NodeType )
			{
				pArchive << Tested;
				pArchive << Forks;

				switch ( NodeType )
				{
					case BrDiscr:
						break;

					case ThreshContin:
						pArchive << Cut;
						pArchive << Lower;
						pArchive << Upper;
						break;

					case BrSubset:
						attVal = m_pLearner->m_dataSet->getDiscretNumber(Tested);
						Bytes = (attVal>>3) + 1;

						pArchive << Bytes;

						ForEach(v, 1, Forks)
						{
							for(i = 0; i < Bytes; i++)
							{
								cSubset = (int)Subset[v][i];
								pArchive << cSubset;
							}
						}	
	
						break;
				}

			}
		}
		else
		{
			pArchive >> Bytes;
			version.Empty();
			if(Bytes)
				pArchive >> version;

			pArchive >> m_nNodeType;

			pArchive >> Leaf;
			pArchive >> Items;
			pArchive >> Errors;

			ClassDist = (ItemCount *) new ItemCount[m_pLearner->MaxClass+1];
			ForEach(v, 0, m_pLearner->MaxClass)
				pArchive >> ClassDist[v];

			NodeType = m_nNodeType;
			if ( NodeType )
			{
				pArchive >> Tested;
				pArchive >> Forks;

				switch ( NodeType )
				{
					case BrDiscr:
						break;

					case ThreshContin:
						pArchive >> Cut;
						pArchive >> Lower;
						pArchive >> Upper;
						break;

					case BrSubset:
						Subset = NULL;

						pArchive >> Bytes;

						Subset = (Set *) new Set[Forks + 1];
						ForEach(v, 1, Forks)
						{
							Subset[v] = (Set)NULL;

							Subset[v] = (Set) new char[Bytes];
							for(i = 0; i < Bytes; i++)
							{
								pArchive >> cSubset;
								Subset[v][i] = (char)cSubset;
							}
						}
	
						break;
				}
			}
		}

	}
    CATCH_ALL (pException)
	{
		// A read/write error occured
		pException->Delete();
		if (pArchive.IsStoring())
			nStatus = 1;
		else
			nStatus = -1;
    }

	END_CATCH_ALL


    return (nStatus);
}




int CTreeRecord::serialize (CArchive& pArchive)
{
	int nStatus = 0, nResult = 0;
	DiscrValue v;

	nStatus = serializeNode(pArchive);
	if(nStatus != 0)
		return nStatus;

	// *debug*
//	m_LogTrace.WriteLine(" Return Status %d\n", nStatus);
//	m_LogTrace.WriteLine(" Forks %d\n", Forks);

	if (pArchive.IsStoring())
	{
		nResult = 0;
		if ( NodeType )
		{
			ForEach(v, 1, Forks)
			{
				if( ( Branch[v]->serialize(pArchive)) != 0)
					return 1;
			}
		}
	}
	else
	{
		nResult = 0;
		if ( NodeType )
		{
			Branch = (Tree *) new Tree[Forks + 1];
			ForEach(v, 1, Forks)
				Branch[v] = NULL;

			ForEach(v, 1, Forks)
			{
				Branch[v] = (Tree) new TreeRec();
				Branch[v]->m_pLearner = m_pLearner;
				if( Branch[v]->serialize(pArchive) != 0)
					return -1;
			}
		}
	}

	return 0;

	/*
	int i, cSubset;
	DiscrValue v;
	int Bytes, attVal;

	// Serialize the object ...
	if (pArchive.IsStoring())
	{
		pArchive << version.GetLength();
		if(version.GetLength())
			pArchive << version;

		pArchive << NodeType;
		pArchive << Leaf;
		pArchive << Items;
		pArchive << Errors;

		ForEach(v, 0, m_pLearner->MaxClass)
		{
			pArchive << ClassDist[v];
		}

		if ( NodeType )
		{
			pArchive << Tested;
			pArchive << Forks;

			switch ( NodeType )
			{
				case BrDiscr:
					break;

				case ThreshContin:
					pArchive << Cut;
					pArchive << Lower;
					pArchive << Upper;
					break;

				case BrSubset:
					attVal = m_pLearner->m_dataSet->getDiscretNumber(Tested);
					Bytes = (attVal>>3) + 1;

					pArchive << Bytes;

					ForEach(v, 1, Forks)
					{
						for(i = 0; i < Bytes; i++)
						{
							cSubset = (int)Subset[v][i];
							pArchive << cSubset;
						}
					}

					break;
			}

			ForEach(v, 1, Forks)
			{
				Branch[v]->serialize(pArchive);
			}
			
		}
	}
	else
	{
		pArchive >> Bytes;
		version.Empty();
		if(Bytes)
			pArchive >> version;

		pArchive >> NodeType;
		pArchive >> Leaf;
		pArchive >> Items;
		pArchive >> Errors;

		ClassDist = (ItemCount *) new ItemCount[m_pLearner->MaxClass+1];
		ForEach(v, 0, m_pLearner->MaxClass)
		{
			pArchive >> ClassDist[v];
		}

		if ( NodeType )
		{
			pArchive >> Tested;
			pArchive >> Forks;

			switch ( NodeType )
			{
				case BrDiscr:
					break;

				case ThreshContin:
					pArchive >> Cut;
					pArchive >> Lower;
					pArchive >> Upper;
					break;

				case BrSubset:

					pArchive >> Bytes;

					Subset = (Set *) new Set[Forks + 1];
					ForEach(v, 1, Forks)
					{
						Subset[v] = (Set) new char[Bytes];
						for(i = 0; i < Bytes; i++)
						{
							pArchive >> cSubset;
							Subset[v][i] = (char)cSubset;
						}
					}

					break;
			}

			Branch = (Tree *) new Tree[Forks + 1];
			ForEach(v, 1, Forks)
			{
				Branch[v] = (Tree) new TreeRec();
				Branch[v]->m_pLearner = m_pLearner;
				Branch[v]->serialize(pArchive);
			}
		}
	}
	*/
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C45Learner::C45Learner()
{
	m_numNode = 0;


	TRIALS = 10;

	GAINRATIO  = true;
	SUBSET     = false;
	BATCH      = true;
	UNSEENS    = false;
	PROBTHRESH = false;

	MINOBJS   = 2;
	WINDOW    = 0;
	INCREMENT = 0;

	CF = 0.25;

	MaxDiscrVal = 2;

	AllKnown = true;

	ClassSum = NULL;
	PossibleValues = NULL;

	ThreshErrs = NULL;
	RHSErr = NULL;
	LHSErr = NULL;
	CVals = NULL;

	Raw = NULL;
	Pruned = NULL;

	DecisionTree = NULL;

	ConfusionMat = NULL;
	Precision = NULL;
	Recall = NULL;

	m_strVersion = _T("0.99999999");
}

C45Learner::~C45Learner()
{
	if(Raw)
	{
		for(int i = 0; i < TRIALS; i++)
		{
			if(Raw[i])
				ReleaseTree(Raw[i]);
		}
		delete [] Raw;
	}

	if(Pruned)
	{
		for(int i = 0; i < TRIALS; i++)
		{
			if(Pruned[i])
				ReleaseTree(Pruned[i]);
		}
		delete [] Pruned;

		DecisionTree = NULL;
	}

	if(DecisionTree)
		ReleaseTree(DecisionTree);


    if ( ClassSum )
		delete [] ClassSum;

	if(ConfusionMat)
		delete [] ConfusionMat;

	if(Precision)
		delete [] Precision;

	if(Recall)
		delete [] Recall;

}

// set the dataset for training and testing
void C45Learner::setDataSet(CDataSet *pData)
{
	m_dataSet = pData;

	MaxAtt = m_dataSet->getAttSize() - 2;
	MaxItem = m_dataSet->case_num - 1;
	MaxClass = m_dataSet->getClassNum() - 1;
	MaxDiscrVal = m_dataSet->getMaxDiscrVal();

	m_dataSet->C45fy();
}

// save the trained decision tree
BOOL C45Learner::Save(const char *m_fileName)
{
	if(!DecisionTree)
		return TRUE;

	BOOL bResult = TRUE;

	DecisionTree->version = m_strVersion;

	CFile m_wFile( m_fileName, CFile::modeCreate | CFile::modeWrite);
	CArchive m_wArch( &m_wFile, CArchive::store );

	if(DecisionTree->serialize(m_wArch) != 0)
		bResult = FALSE;

	m_wArch.Close();
	m_wFile.Close();

	return bResult;
}

// load the decision tree
BOOL C45Learner::Load(const char *m_fileName)
{
	BOOL bResult = TRUE;

	if(DecisionTree)
	{
		ReleaseTree(DecisionTree);
		DecisionTree = NULL;
	}

	DecisionTree = (Tree) new TreeRec();
	DecisionTree->m_pLearner = this;

	CFile m_rFile( m_fileName, CFile::modeRead);
	CArchive m_rArch( &m_rFile, CArchive::load );

	if(DecisionTree->serialize(m_rArch) != 0)
	{
		ReleaseTree(DecisionTree);
		DecisionTree = NULL;

		bResult = FALSE;
	}

	m_rArch.Close();
	m_rFile.Close();

	if(DecisionTree)
		m_strVersion = DecisionTree->version;

	return bResult;
}


// evaluate the dataset, print out the confusion matrix
//�������ݼ�����ӡ����������
void C45Learner::Evaluate()
{
	if(DecisionTree)
	{
		Evaluate(true, DecisionTree);
		CalPrecisionRecall(MaxClass + 1);
	}
}

// compute the precision and recall based on the
// confusion matrix
//���ڻ�������������׼ȷ�ʺ��ٻ���
void C45Learner::CalPrecisionRecall(int numClass)
{
	if(!ConfusionMat)
		return;

	if(Precision)
		delete [] Precision;

	if(Recall)
		delete [] Recall;

	int pos, correct, j, truenum, predictnum, index;
	Precision = (double *) new double[numClass];
	Recall = (double *) new double[numClass];
	ForEach(pos, 0, numClass - 1)
	{
		Precision[pos] = 0.00;
		Recall[pos] = 0.00;
	}

	ForEach(pos, 0, numClass - 1)
	{
		correct = ConfusionMat[pos*numClass + pos];

		truenum = 0;
		ForEach(j, pos*numClass, (pos+1)*numClass-1)
			truenum += ConfusionMat[j];

		predictnum = 0;
		ForEach(j, 0, numClass - 1)
		{
			index = j*numClass + pos;
			predictnum += ConfusionMat[index];
		}

		if(predictnum)
			Precision[pos] = (double)correct/(double)predictnum;
		if(truenum)
			Recall[pos] = (double)correct/(double)truenum;
	}

}


// predict the class for each item in the dataset
void C45Learner::Predict()
{
	if(DecisionTree)
		Predict(DecisionTree);
}

// this is the main function to build the tree
void C45Learner::buildClassifier()
{
	if(!m_dataSet)
		return;

	short Best;

    /*  Build decision trees  */
	if ( BATCH )
    {
		TRIALS = 1;
		OneTree();
		Best = 0;
	}
    else
    {
		Best = BestTree();
    }

    /*  Soften thresholds in best tree  */
    if ( PROBTHRESH )
		SoftenThresh(Pruned[Best]);

	DeInitialiseTreeData();

	DecisionTree = Pruned[Best];
}


void C45Learner::OneTree()
{
    InitialiseTreeData();
    InitialiseWeights();

    Raw = (Tree *) new Tree[1];
	Raw[0] = NULL;

    AllKnown = true;
	Raw[0] = FormTree(0, MaxItem);

	Pruned = (Tree *) new Tree[1];
	Pruned[0] = NULL;

	Pruned[0] = CopyTree(Raw[0]);
	if ( Prune(Pruned[0]) )
    {
//		printf("\nSimplified ");
//		PrintTree(Pruned[0]);
    }
}



// Allocate space for tree tables
void C45Learner::InitialiseTreeData()
{
    DiscrValue v;
    Attribute a;

    Tested	= (bool *) new bool[MaxAtt+1];
    ForEach(a, 0, MaxAtt)
		Tested[a] = false;

    Gain	= (double *) new double[MaxAtt+1];
    Info	= (double *) new double[MaxAtt+1];
    Bar		= (double *) new double[MaxAtt+1];

    Subset = (Set **) new Set *[MaxAtt+1];
    ForEach(a, 0, MaxAtt)
    {
		if ( m_dataSet->getDiscretNumber(a) )
		{
			Subset[a]  = (Set *) new Set[MaxDiscrVal+1];
		    ForEach(v, 0, m_dataSet->getDiscretNumber(a))
				Subset[a][v] = (Set) new char[(m_dataSet->getDiscretNumber(a)>>3) + 1];
		}
    }
    Subsets = (short *) new short[MaxAtt+1];

    SplitGain = (double *) new double[MaxItem+1];
    SplitInfo = (double *) new double[MaxItem+1];

    Weight = (ItemCount *) new ItemCount[MaxItem+1];

    Freq  = (ItemCount **) new ItemCount *[MaxDiscrVal+1];
    ForEach(v, 0, MaxDiscrVal)
		Freq[v]  = (ItemCount *) new ItemCount[MaxClass+1];

    ValFreq = (ItemCount *) new ItemCount[MaxDiscrVal+1];
    ClassFreq = (ItemCount *) new ItemCount[MaxClass+1];

    Slice1 = (ItemCount *) new ItemCount[MaxClass+2];
    Slice2 = (ItemCount *) new ItemCount[MaxClass+2];

    UnknownRate = (double *) new double[MaxAtt+1];

    /*  Check whether all attributes have many discrete values  */
    MultiVal = true;
    if ( ! SUBSET )
    {
		for ( a = 0 ; MultiVal && a <= MaxAtt ; a++ )
		{
			if ( !m_dataSet->isIgnore(a) )
				MultiVal = m_dataSet->getDiscretNumber(a) >= 0.3 * (MaxItem + 1);
		}
    }
}


void C45Learner::DeInitialiseTreeData()
{
    DiscrValue v;
    Attribute a;

    delete [] Tested;

    delete [] Gain;
    delete [] Info;
    delete [] Bar;

    ForEach(a, 0, MaxAtt)
    {
		if ( m_dataSet->getDiscretNumber(a) )
		{
		    ForEach(v, 0, m_dataSet->getDiscretNumber(a))
				delete [] Subset[a][v];
			delete [] Subset[a];
		}
    }
	delete [] Subset;

    delete [] Subsets;

    delete [] SplitGain;
    delete [] SplitInfo;

    delete [] Weight;

    ForEach(v, 0, MaxDiscrVal)
		delete [] Freq[v];
	delete [] Freq;

    delete [] ValFreq;
    delete [] ClassFreq;

    delete [] Slice1;
    delete [] Slice2;

    delete [] UnknownRate;
}


// Initialise the weight of each item
void C45Learner::InitialiseWeights()
{
    ItemNo i;

    ForEach(i, 0, MaxItem)
        Weight[i] = 1.0;
}




/*************************************************************************/
/*								 	 */
/*  Build a decision tree for the cases Fp through Lp:		 	 */
/*								 	 */
/*  - if all cases are of the same class, the tree is a leaf and so	 */
/*      the leaf is returned labelled with this class		 	 */
/*								 	 */
/*  - for each attribute, calculate the potential information provided 	 */
/*	by a test on the attribute (based on the probabilities of each	 */
/*	case having a particular value for the attribute), and the gain	 */
/*	in information that would result from a test on the attribute	 */
/*	(based on the probabilities of each case with a particular	 */
/*	value for the attribute being of a particular class)		 */
/*								 	 */
/*  - on the basis of these figures, and depending on the current	 */
/*	selection criterion, find the best attribute to branch on. 	 */
/*	Note:  this version will not allow a split on an attribute	 */
/*	unless two or more subsets have at least MINOBJS items. 	 */
/*								 	 */
/*  - try branching and test whether better than forming a leaf	 	 */
/*								 	 */
/*************************************************************************/

Tree C45Learner::FormTree(ItemNo Fp, ItemNo Lp)
{
    ItemNo i, Kp, Ep;
    ItemCount Cases, NoBestClass, KnownCases;
    double Factor, BestVal, Val, AvGain=0;
    Attribute Att, BestAtt, Possible=0;
    ClassNo c, BestClass;
    Tree Node;
    DiscrValue v;
    bool PrevAllKnown;

    Cases = CountItems(Fp, Lp);

    /*  Generate the class frequency distribution  */
    ForEach(c, 0, MaxClass)
		ClassFreq[c] = 0.0;
    ForEach(i, Fp, Lp)
		ClassFreq[ Class(i) ] += Weight[i];

    /*  Find the most frequent class  */
    BestClass = 0;
    ForEach(c, 0, MaxClass)
    {
		if ( ClassFreq[c] > ClassFreq[BestClass] )
			BestClass = c;
    }
    NoBestClass = ClassFreq[BestClass];
    Node = Leaf(ClassFreq, BestClass, Cases, Cases - NoBestClass);

    /*  If all cases are of the same class or there are not enough
	cases to divide, the tree is a leaf  */
	if ( NoBestClass == Cases  || Cases < 2 * MINOBJS )
	{
		return Node;
	}

    /*  For each available attribute, find the information and gain  */
    ForEach(Att, 0, MaxAtt) 
    { 
		Gain[Att] = -Epsilon;

		if ( m_dataSet->isIgnore(Att) || Att >= m_dataSet->getAttSize() - 1)
			continue;

        if ( m_dataSet->getDiscretNumber(Att) )
		{
			/*  discrete valued attribute  */
		    if ( SUBSET && m_dataSet->getDiscretNumber(Att) > 2 )
			{
				EvalSubset(Att, Fp, Lp, Cases);
			}
		    else if ( ! Tested[Att] )
			{
		        EvalDiscreteAtt(Att, Fp, Lp, Cases);
			}
		}
		else
		{ 
			/*  continuous attribute  */
		    EvalContinuousAtt(Att, Fp, Lp);
		} 

		/*  Update average gain, excluding attributes with very many values  */
		if ( Gain[Att] > -Epsilon &&
	     ( MultiVal || m_dataSet->getDiscretNumber(Att) < 0.3 * (MaxItem + 1) ) )
		{
			Possible++;
		    AvGain += Gain[Att];
		}
    } 

    /*  Find the best attribute according to the given criterion  */

    BestVal = -Epsilon;
    BestAtt = None;
    AvGain  = Possible ? AvGain/(double)Possible : 1E6 ;

    ForEach(Att, 0, MaxAtt) 
    { 
		if ( Gain[Att] > -Epsilon )
		{ 
			Val = Worth(Info[Att], Gain[Att], AvGain);

		    if ( Val >= BestVal ) 
		    { 
			    BestAtt  = Att; 
				BestVal = Val;
		    } 
		} 
    } 

    /*  Decide whether to branch or not  */ 
    if ( BestAtt != None )
    { 
		/*  Build a node of the selected test  */
        if ( m_dataSet->getDiscretNumber(BestAtt) )
		{
		    /*  Discrete valued attribute  */
		    if ( SUBSET && m_dataSet->getDiscretNumber(BestAtt) > 2 )
			{
			    SubsetTest(Node, BestAtt);
			}
		    else
			{
				DiscreteTest(Node, BestAtt);
			}
		}
		else
		{ 
			/*  Continuous attribute  */
		    ContinTest(Node, BestAtt);
		} 

		/*  Remove unknown attribute values  */
		PrevAllKnown = AllKnown;

		Kp = Group(0, Fp, Lp, Node) + 1;
		if ( Kp != Fp )
			AllKnown = false;
		KnownCases = Cases - CountItems(Fp, Kp-1);
		UnknownRate[BestAtt] = (double)(Cases - KnownCases) / (Cases + 0.001);

		/*  Recursive divide and conquer  */
		Tested[BestAtt] = true;

		Ep = Kp - 1;
		Node->Errors = 0;

		ForEach(v, 1, Node->Forks)
		{
		    Ep = Group(v, Kp, Lp, Node);

		    if ( Kp <= Ep )
		    {
				Factor = CountItems(Kp, Ep) / KnownCases;

				ForEach(i, Fp, Kp-1)
				{
				    Weight[i] *= Factor;
				}

				Node->Branch[v] = FormTree(Fp, Ep);
				Node->Errors += Node->Branch[v]->Errors;

				Group(0, Fp, Ep, Node);
				ForEach(i, Fp, Kp-1)
				{
				    Weight[i] /= Factor;
				}
		    }
		    else
		    {
				Node->Branch[v] = Leaf(Node->ClassDist, BestClass, 0.0, 0.0);
			}
		}

		Tested[BestAtt] = false;
		AllKnown = PrevAllKnown;

		/*  See whether we would have been no worse off with a leaf  */
		if ( Node->Errors >= Cases - NoBestClass - Epsilon )
		{ 
			if(Node->NodeType)
			{
				// this is fogotten in Quinlan's C4.5
				// it is a memory leak in the original version
				ForEach(v, 1, Node->Forks)
					ReleaseTree(Node->Branch[v]);
				delete [] Node->Branch;
				Node->Branch = NULL;

				if ( Node->NodeType == BrSubset )
				{
				    ForEach(v, 1, Node->Forks)
						delete [] Node->Subset[v];
					delete Node->Subset;
					Node->Subset = NULL;
				}
			}

		    Node->NodeType = 0;
			Node->Tested = None;
		} 
    }

    return Node; 
}

// Return the total weight of items from Fp to Lp
ItemCount C45Learner::CountItems(ItemNo Fp, ItemNo Lp)
{
    ItemCount Sum=0.0, *Wt, *LWt;

    if ( AllKnown )
		return (ItemCount)(Lp - Fp + 1);

    for ( Wt = Weight + Fp, LWt = Weight + Lp ; Wt <= LWt ; )
		Sum += *Wt++;

    return Sum;
}


int C45Learner::Class(int caseNo)
{
	return (int)m_dataSet->m_data[caseNo][m_dataSet->getAttSize() - 1];
}


// Construct a leaf in a given node
Tree C45Learner::Leaf(ItemCount *ClassFreq, ClassNo NodeClass, ItemCount Cases, ItemCount Errors)
{
    Tree Node;
	int i;

    Node = (Tree) new TreeRec();

	Node->nodeID = m_numNode;
	++m_numNode;

    Node->ClassDist = (ItemCount *) new ItemCount[MaxClass+1];
	ForEach(i, 0, MaxClass)
		Node->ClassDist[i] = ClassFreq[i];
    
    Node->NodeType	= 0; 
    Node->Leaf		= NodeClass;
    Node->Items		= Cases;
    Node->Errors	= Errors;

	Node->Branch	= NULL;
	Node->Subset	= NULL;

	Node->m_pLearner = this;

    return Node; 
}


/*************************************************************************/
/*								 	 */
/*  Group together the items corresponding to branch V of a test 	 */
/*  and return the index of the last such			 	 */
/*								 	 */
/*  Note: if V equals zero, group the unknown values		 	 */
/*								 	 */
/*************************************************************************/

ItemNo C45Learner::Group(DiscrValue V, ItemNo Fp, ItemNo Lp, Tree TestNode)
{
    ItemNo i;
    Attribute Att;
    double Thresh;
    Set SS;

    Att = TestNode->Tested;

    if ( V )
    {
		/*  Group items on the value of attribute Att, and depending
	    on the type of branch  */

		switch ( TestNode->NodeType )
		{
		    case BrDiscr:

				ForEach(i, Fp, Lp)
				{
				    if ( DVal(i, Att) == V )
						Swap(Fp++, i);
				}
				break;

		    case ThreshContin:

				Thresh = TestNode->Cut;
				ForEach(i, Fp, Lp)
				{
					if ( (CVal(i, Att) <= Thresh) == (V == 1) )
						Swap(Fp++, i);
				}
				break;

		    case BrSubset:

				SS = TestNode->Subset[V];
				ForEach(i, Fp, Lp)
				{
					if ( In(DVal(i, Att), SS) )
						Swap(Fp++, i);
				}
				break;
		}
    }
    else
    {
		/*  Group together unknown values  */

		switch ( TestNode->NodeType )
		{
			case BrDiscr:
			case BrSubset:

				ForEach(i, Fp, Lp)
				{
				    if ( ! DVal(i, Att) )
						Swap(Fp++, i);
				}
				break;

			case ThreshContin:

				ForEach(i, Fp, Lp)
				{
				    if ( CVal(i, Att) == VUnknown )
						Swap(Fp++, i);
				}
				break;
		}
    }

    return Fp - 1;
}

// Exchange items at a and b
//����a��b
void C45Learner::Swap(ItemNo a, ItemNo b)
{
    ItemCount HoldW;

	m_dataSet->Swap(a, b);

    HoldW = Weight[a];
    Weight[a] = Weight[b];
    Weight[b] = HoldW;
}

// get the discrete value of the case at the column
//���ø��е���ɢֵ
short C45Learner::DVal(int caseNo, int Att)
{
	return (short) (m_dataSet->m_data[caseNo][Att]);
}

// get the continuous value of the case at the column
//���ø��е�����ֵ
double C45Learner::CVal(int caseNo, int Att)
{
	return m_dataSet->m_data[caseNo][Att];
}




/*************************************************************************/
/*									 */
/*  Evaluate subsetting a discrete attribute and form the chosen	 */
/*  subsets Subset[Att][], setting Subsets[Att] to the number of	 */
/*  subsets, and the Info[] and Gain[] of a test on the attribute	 */
/*									 */
/*************************************************************************/

void C45Learner::EvalSubset(Attribute Att, ItemNo Fp, ItemNo Lp, ItemCount Items)
{
    DiscrValue V1, V2, BestV1, BestV2, Barred;
    ItemCount KnownItems;
    ClassNo c;
    double BaseInfo, MinGain, ThisGain, ThisInfo,
	Val, BestVal, BestGain, BestInfo,
	PrevVal, PrevGain, PrevInfo;
    short Blocks=0, MissingValues=0, ReasonableSubsets, Bytes, b;
    bool MergedSubsets = false;
    int SaveMINOBJS;

    SaveMINOBJS = MINOBJS;
    MINOBJS = 1;

    /*  First compute Freq[][], ValFreq[], base info, and the gain
	and total info of a split on discrete attribute Att  */

    ComputeFrequencies(Att, Fp, Lp);

    KnownItems = Items - ValFreq[0];
    if ( KnownItems < Epsilon )
    {
		Gain[Att] = -Epsilon;
		Info[Att] = 0;
		return;
    }

    BaseInfo = DiscrKnownBaseInfo(KnownItems, m_dataSet->getDiscretNumber(Att));

    PrevGain = ComputeGain(BaseInfo, UnknownRate[Att], m_dataSet->getDiscretNumber(Att),KnownItems);
    PrevInfo = TotalInfo(ValFreq, 0, m_dataSet->getDiscretNumber(Att)) / Items;
    PrevVal = Worth(PrevInfo, PrevGain, Epsilon);

    /*  Eliminate unrepresented attribute values from Freq[] and ValFreq[]
	and form a separate subset for each represented attribute value  */

    Bytes = (m_dataSet->getDiscretNumber(Att)>>3) + 1;
    ClearBits(Bytes, Subset[Att][0]);

    ForEach(V1, 1, m_dataSet->getDiscretNumber(Att))
    {
		if ( ValFreq[V1] > 0.5 )
		{
		    if ( ++Blocks < V1 )
		    {
				ValFreq[Blocks] = ValFreq[V1];
				ForEach(c, 0, MaxClass)
				{
				    Freq[Blocks][c] = Freq[V1][c];
				}
		    }
		    ClearBits(Bytes, Subset[Att][Blocks]);
		    SetBit(V1, Subset[Att][Blocks]);
		}
		else
		{
		    SetBit(V1, Subset[Att][0]);
		    MissingValues++;
		}
    }

    /*  Merge any single-class subsets with others of the same class  */
    /*  Note: have ValFreq[V] > 0 for all V  */

    ForEach(V1, 1, Blocks-1)
    {
		for ( c = 0 ; Freq[V1][c] < 0.1 ; c++ )
	    ;

		if ( Freq[V1][c] < ValFreq[V1] - 0.1 )
			continue;

		/*  Now have a single class -- look for others  */

		for ( V2 = V1+1 ; V2 <= Blocks ; )
		{
		    if ( Freq[V2][c] < ValFreq[V2] - 0.1 )
				V2++;
		    else
		    {
				/*  Merge these subsets  */
				Combine(V1, V2, Blocks);

				ForEach(b, 0, Bytes-1)
				{
				    Subset[Att][V1][b] |= Subset[Att][V2][b];
				    Subset[Att][V2][b] = Subset[Att][Blocks][b];
				}

				Blocks--;
				MergedSubsets = true;
		    }
		}
    }

    if ( MergedSubsets )
    {
		PrevGain = ComputeGain(BaseInfo, UnknownRate[Att], Blocks, KnownItems);
		PrevInfo = TotalInfo(ValFreq, 0, Blocks) / Items;
		PrevVal = Worth(PrevInfo, PrevGain, Epsilon);
    }

    /*  Examine possible pair mergers and hill-climb  */
    MinGain = PrevGain / 2;

    while ( Blocks > 2 )
    {
		BestVal = BestV1 = 0;
		BestGain = -Epsilon;

		/*  Check reasonable subsets; if less than 3, bar mergers
	    involving the largest block  */

		ReasonableSubsets = 0;
		Barred = 1;

		ForEach(V1, 1, Blocks)
		{
			if ( ValFreq[V1] >= SaveMINOBJS )
				ReasonableSubsets++;

		    if ( ValFreq[V1] > ValFreq[Barred] )
				Barred = V1;
		}

		if ( ReasonableSubsets >= 3 )
			Barred = 0;

		/*  For each possible pair of values, calculate the gain and
	    total info of a split in which they are treated as one.
	    Keep track of the pair with the best gain.  */

		ForEach(V1, 1, Blocks-1)
		{
			ForEach(V2, V1+1, Blocks)
		    {
				if ( V1 == Barred || V2 == Barred )
					continue;

				Combine(V1, V2, Blocks);

				ThisGain = ComputeGain(BaseInfo, UnknownRate[Att],
					Blocks-1, KnownItems);
				ThisInfo = TotalInfo(ValFreq, 0, Blocks-1) / Items;
				Val      = Worth(ThisInfo, ThisGain, Epsilon);

				/*  Force a split if
					less than two reasonable subsets, or
					using GAIN criterion
				    Prefer this split to the previous one if
					gain >= MinGain (and previous < MinGain), or
					val >= previous best val  */

				if ( ThisGain >= MinGain && BestGain < MinGain ||
					Val >= BestVal ||
				     ! BestV1 && ( ! GAINRATIO || ReasonableSubsets < 2 ) )
				{
				    BestVal  = Val;
				    BestGain = ThisGain;
				    BestInfo = ThisInfo;
				    BestV1   = V1;
				    BestV2   = V2;
				}

				Uncombine(V1, V2);
			}
		}

		if ( GAINRATIO && ReasonableSubsets >= 2 &&
		     ( ! BestV1 || BestVal < PrevVal + 1E-5 ||
			   BestVal == PrevVal && BestGain < PrevGain ) )
			break;

		PrevGain = BestGain;
		PrevInfo = BestInfo;
        PrevVal = BestVal;

		Combine(BestV1, BestV2, Blocks);

		ForEach(b, 0, Bytes-1)
		{
			Subset[Att][BestV1][b] |= Subset[Att][BestV2][b];
		    Subset[Att][BestV2][b] = Subset[Att][Blocks][b];
		}

		Blocks--;
    }

    MINOBJS = SaveMINOBJS;

    if ( PrevVal <= 0 )
    {
		Gain[Att] = -Epsilon;
		Info[Att] = 0;
    }
    else
    {
		Gain[Att] = ComputeGain(BaseInfo, UnknownRate[Att], Blocks, KnownItems);
		Info[Att] = PrevInfo;

		if ( MissingValues )
		{
			Blocks++;
		    CopyBits(Bytes, Subset[Att][0], Subset[Att][Blocks]);
		}

		Subsets[Att] = Blocks;
    }
}



/*************************************************************************/
/*									 */
/*  Compute frequency tables Freq[][] and ValFreq[] for attribute	 */
/*  Att from items Fp to Lp, and set the UnknownRate for Att		 */
/*									 */
/*************************************************************************/

void C45Learner::ComputeFrequencies(Attribute Att, ItemNo Fp, ItemNo Lp)
{
    ClassNo c;
    DiscrValue v;
    ItemNo p;

    ResetFreq(m_dataSet->getDiscretNumber(Att));

    /*  Determine the frequency of each class amongst cases
	with each possible value for the given attribute  */
    ForEach(p, Fp, Lp)
    { 
//		Case = Item[p];

		Freq[ DVal(p, Att) ][ Class(p) ] += Weight[p];
    } 

    /*  Determine the frequency of each possible value for the
	given attribute  */

    ForEach(v, 0, m_dataSet->getDiscretNumber(Att)) 
    { 
		ForEach(c, 0, MaxClass)
		{
			ValFreq[v] += Freq[v][c];
		}
    }

    /*  Set the rate of unknown values of the attribute  */
 
    UnknownRate[Att] = ValFreq[0] / CountItems(Fp, Lp);
}


// Zero the frequency tables Freq[][] and ValFreq[] up to MaxVal
// to make sure that all the values in Freq and ValFreq
// have been resetted, we just clear out all the values
void C45Learner::ResetFreq(DiscrValue MaxVal)
{
    DiscrValue v;
    ClassNo c;

    ForEach(v, 0, MaxDiscrVal)
    { 
		ForEach(c, 0, MaxClass)
			Freq[v][c] = 0.0F;

		ValFreq[v] = 0.0F;
    } 
}


/*************************************************************************/
/*									 */
/*  Return the base info for items with known values of a discrete	 */
/*  attribute, using the frequency table Freq[][]			 */
/*	 								 */
/*************************************************************************/

double C45Learner::DiscrKnownBaseInfo(ItemCount KnownItems, DiscrValue MaxVal)
{
    ClassNo c;
    ItemCount ClassCount;
    double Sum=0;
    DiscrValue v;

    ForEach(c, 0, MaxClass)
    {
		ClassCount = 0;
		ForEach(v, 1, MaxVal)
		{
		    ClassCount += Freq[v][c];
		}
		Sum += ClassCount * Log(ClassCount);
    }

    return (KnownItems * Log(KnownItems) - Sum) / KnownItems;
}


/*************************************************************************/
/*									 */
/*  Given tables Freq[][] and ValFreq[], compute the information gain.	 */
/*									 */
/*	    Parameters:							 */
/*		BaseInfo:	average information for all items with	 */
/*				known values of the test attribute	 */
/*		UnknownRate:	fraction of items with unknown ditto	 */
/*		MaxVal:		number of forks				 */
/*		TotalItems:	number of items with known values of	 */
/*				test att				 */
/*									 */
/*  where Freq[x][y] contains the no. of cases with value x for a	 */
/*  particular attribute that are members of class y,			 */
/*  and ValFreq[x] contains the no. of cases with value x for a		 */
/*  particular attribute						 */
/*									 */
/*************************************************************************/
//������Ϣ����
double C45Learner::ComputeGain(double BaseInfo, double UnknFrac, DiscrValue MaxVal, ItemCount TotalItems)
{
    DiscrValue v;
    double ThisInfo=0.0, ThisGain;
    short ReasonableSubsets=0;

    /*  Check whether all values are unknown or the same  */
    if ( ! TotalItems )
		return -Epsilon;

    /*  There must be at least two subsets with MINOBJS items  */
    ForEach(v, 1, MaxVal)
    {
		if ( ValFreq[v] >= MINOBJS )
			ReasonableSubsets++;
    }
    if ( ReasonableSubsets < 2 )
		return -Epsilon;

    /*  Compute total info after split, by summing the
	info of each of the subsets formed by the test  */
    ForEach(v, 1, MaxVal)
    {
		ThisInfo += TotalInfo(Freq[v], 0, MaxClass);
    }

    /*  Set the gain in information for all items, adjusted for unknowns  */
    ThisGain = (1 - UnknFrac) * (BaseInfo - ThisInfo / TotalItems);

    return ThisGain;
}

// Compute the total information in V[ MinVal..MaxVal ]
double C45Learner::TotalInfo(ItemCount V[], DiscrValue MinVal, DiscrValue MaxVal)
{
    DiscrValue v;
    double Sum=0.0;
    ItemCount N, TotalItems=0;

    ForEach(v, MinVal, MaxVal)
    {
		N = V[v];

		Sum += N * Log(N);
		TotalItems += N;
    }

    return TotalItems * Log(TotalItems) - Sum;
}


/*************************************************************************/
/*									 */
/*  Determine the worth of a particular split according to the		 */
/*  operative criterion							 */
/*									 */
/*	    Parameters:							 */
/*		SplitInfo:	potential info of the split		 */
/*		SplitGain:	gain in info of the split		 */
/*		MinGain:	gain above which the Gain Ratio		 */
/*				may be used				 */
/*									 */
/*  If the Gain criterion is being used, the information gain of	 */
/*  the split is returned, but if the Gain Ratio criterion is		 */
/*  being used, the ratio of the information gain of the split to	 */
/*  its potential information is returned.				 */
/*									 */
/*************************************************************************/

double C45Learner::Worth(double ThisInfo, double ThisGain, double MinGain)
{
    if ( GAINRATIO )
    {
		if ( ThisGain >= MinGain - Epsilon && ThisInfo > Epsilon )
			return ThisGain / ThisInfo;
		else
		    return -Epsilon;
    }
    else
		return ( ThisInfo > 0 && ThisGain > -Epsilon ? ThisGain : -Epsilon );
}


/*************************************************************************/
/*									 */
/*  Combine the distribution figures of discrete attribute values	 */
/*  x and y, putting the combined figures in Freq[x][] and		 */
/*  ValFreq[x][], and saving old values in Slice1 and Slice2		 */
/*									 */
/*************************************************************************/

void C45Learner::Combine(DiscrValue x, DiscrValue y, DiscrValue Last)
{
    ClassNo c;

    ForEach(c, 0, MaxClass)
    {
		Slice1[c] = Freq[x][c];
		Slice2[c] = Freq[y][c];

		Freq[x][c] += Freq[y][c];
		Freq[y][c]  = Freq[Last][c];
    }

    Slice1[MaxClass+1] = ValFreq[x];
    Slice2[MaxClass+1] = ValFreq[y];

    ValFreq[x] += ValFreq[y];
    ValFreq[y]  = ValFreq[Last];
}


/*************************************************************************/
/*									 */
/*  Restore old class distribution figures of discrete attribute	 */
/*  values x and y from Slice1 and Slice2				 */
/*									 */
/*************************************************************************/
void C45Learner::Uncombine(DiscrValue x, DiscrValue y)
{
    ClassNo c;

    ForEach(c, 0, MaxClass)
    {
		Freq[x][c] = Slice1[c];
		Freq[y][c] = Slice2[c];
    }

    ValFreq[x] = Slice1[MaxClass+1];
    ValFreq[y] = Slice2[MaxClass+1];
}


// Set Info[] and Gain[] for discrete partition of items Fp to Lp
void C45Learner::EvalDiscreteAtt(Attribute Att, ItemNo Fp, ItemNo Lp, ItemCount Items)
{
    ItemCount KnownItems;

    ComputeFrequencies(Att, Fp, Lp);

    KnownItems = Items - ValFreq[0];

    /*  Special case when no known values of the attribute  */
    if ( Items <= ValFreq[0] )
    {
//		Verbosity(2) printf("\tAtt %s: no known values\n", AttName[Att]);

		Gain[Att] = -Epsilon;
		Info[Att] = 0.0;
		return;
    }

    Gain[Att] = ComputeGain(
		DiscrKnownBaseInfo(KnownItems, m_dataSet->getDiscretNumber(Att)), 
		UnknownRate[Att], 
		m_dataSet->getDiscretNumber(Att), 
		KnownItems);

    Info[Att] = TotalInfo(ValFreq, 0, m_dataSet->getDiscretNumber(Att)) / Items;

//	Verbosity(2)
//	{
//		printf("\tAtt %s", AttName[Att]);
//		Verbosity(3) PrintDistribution(Att, MaxAttVal[Att], true);
//		printf("\tinf %.3f, gain %.3f\n", Info[Att], Gain[Att]);
//	}

}


/*************************************************************************/
/*								  	 */
/*  Continuous attributes are treated as if they have possible values	 */
/*	0 (unknown), 1 (less than cut), 2(greater than cut)	  	 */
/*  This routine finds the best cut for items Fp through Lp and sets	 */
/*  Info[], Gain[] and Bar[]						 */
/*								  	 */
/*************************************************************************/

void C45Learner::EvalContinuousAtt(Attribute Att, ItemNo Fp, ItemNo Lp)
{
    ItemNo i, BestI, Xp, Tries=0;
    ItemCount Items, KnownItems, LowItems, MinSplit;
    ClassNo c;
    double AvGain=0, Val, BestVal, BaseInfo, ThreshCost;

//	Verbosity(2) printf("\tAtt %s", AttName[Att]);
//	Verbosity(3) printf("\n");

    ResetFreq(2);

    /*  Omit and count unknown values */
    Items = CountItems(Fp, Lp);
    Xp = Fp;
    ForEach(i, Fp, Lp)
    {
		if ( CVal(i, Att) == VUnknown )
		{
		    Freq[ 0 ][ Class(i) ] += Weight[i];
			Swap(Xp, i);
		    Xp++;
		}
    }

    ValFreq[0] = 0;
    ForEach(c, 0, MaxClass)
    {
		ValFreq[0] += Freq[0][c];
    }

    KnownItems = Items - ValFreq[0];
    UnknownRate[Att] = 1.0 - KnownItems / Items;

    /*  Special case when very few known values  */
    if ( KnownItems < 2 * MINOBJS )
    {
//		Verbosity(2) printf("\tinsufficient cases with known values\n");

		Gain[Att] = -Epsilon;
		Info[Att] = 0.0;
		return;
    }

    Quicksort(Xp, Lp, Att);

    /*  Count base values and determine base information  */
    ForEach(i, Xp, Lp)
    {
		Freq[ 2 ][ Class(i) ] += Weight[i];
		SplitGain[i] = -Epsilon;
		SplitInfo[i] = 0;
    }

    BaseInfo = TotalInfo(Freq[2], 0, MaxClass) / KnownItems;

    /*  Try possible cuts between items i and i+1, and determine the
	information and gain of the split in each case.  We have to be wary
	of splitting a small number of items off one end, as we can always
	split off a single item, but this has little predictive power.  */

    MinSplit = 0.10 * KnownItems / (MaxClass + 1);
    if ( MinSplit <= MINOBJS )
		MinSplit = MINOBJS;
    else
	{
		if ( MinSplit > 25 )
			MinSplit = 25;
	}

    LowItems = 0;
    ForEach(i, Xp, Lp - 1)
    {
		c = Class(i);
		LowItems   += Weight[i];
		Freq[1][c] += Weight[i];
		Freq[2][c] -= Weight[i];

		if ( LowItems < MinSplit )
			continue;
		else
		{
			if ( LowItems > KnownItems - MinSplit )
				break;
		}

		if ( CVal(i, Att) < CVal(i+1, Att) - 1E-5 )
		{
		    ValFreq[1] = LowItems;
		    ValFreq[2] = KnownItems - LowItems;
		    SplitGain[i] = ComputeGain(BaseInfo, UnknownRate[Att], 2, KnownItems);
		    SplitInfo[i] = TotalInfo(ValFreq, 0, 2) / Items;
		    AvGain += SplitGain[i];
		    Tries++;

//		    Verbosity(3)
//			{
//				printf("\t\tCut at %.3f  (gain %.3f, val %.3f):",
//					( CVal(Item[i],Att) + CVal(Item[i+1],Att) ) / 2,
//					SplitGain[i],
//					Worth(SplitInfo[i], SplitGain[i], Epsilon));
//					PrintDistribution(Att, 2, true);
//			}

		}
    }

    /*  Find the best attribute according to the given criterion  */
    ThreshCost = Log(Tries) / Items;

    BestVal = 0;
    BestI   = None;
    ForEach(i, Xp, Lp - 1)
    {
		if ( (Val = SplitGain[i] - ThreshCost) > BestVal )
		{
		    BestI   = i;
			BestVal = Val;
		}
    }

    /*  If a test on the attribute is able to make a gain,
	set the best break point, gain and information  */ 

    if ( BestI == None )
    {
		Gain[Att] = -Epsilon;
		Info[Att] = 0.0;

//		Verbosity(2) printf("\tno gain\n");
    }
    else
    {
		Bar[Att]  = (CVal(BestI,Att) + CVal(BestI+1,Att)) / 2;
		Gain[Att] = BestVal;
		Info[Att] = SplitInfo[BestI];

//		Verbosity(2)
//			printf("\tcut=%.3f, inf %.3f, gain %.3f\n",Bar[Att], Info[Att], Gain[Att]);

    }
}


// Sort items from Fp to Lp on attribute a
void C45Learner::Quicksort(ItemNo Fp, ItemNo Lp, Attribute Att)
{
    ItemNo Lower, Middle;
    double Thresh;
    ItemNo i;

    if ( Fp < Lp )
    {
		Thresh = CVal(Lp, Att);

		/*  Isolate all items with values <= threshold  */
		Middle = Fp;

		for ( i = Fp ; i < Lp ; i++ )
		{ 
		    if ( CVal(i, Att) <= Thresh )
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
			if ( CVal(i, Att) == Thresh )
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



// Construct and return a node for a test on a subset of values
void C45Learner::SubsetTest(Tree Node, Attribute Att)
{
    short S, Bytes;
	short attVal;

    Sprout(Node, Subsets[Att]);

    Node->NodeType	= BrSubset;
    Node->Tested	= Att;
    Node->Errors	= 0;

//	m_LogTrace.WriteLine("Node ID %d(%d)\n", Node->nodeID, Node->NodeType);

	attVal = m_dataSet->getDiscretNumber(Att);
    Bytes = (attVal>>3) + 1;
    Node->Subset = (Set *) new Set[Subsets[Att] + 1];
    ForEach(S, 1, Node->Forks)
    {
		Node->Subset[S] = (Set) new char[Bytes];
		CopyBits(Bytes, Subset[Att][S], Node->Subset[S]);
    }
}

// Insert branches in a node
void C45Learner::Sprout(Tree Node, DiscrValue Branches)
{
	short S;

    Node->Forks = Branches;
    Node->Branch = (Tree *) new Tree[Branches+1];
    ForEach(S, 0, Node->Forks)
		Node->Branch[S] = NULL;

//	m_LogTrace.WriteLine("Sprout at Node %d(%d), Forks :%d\n", 
//Node->nodeID, Node->NodeType, Node->Forks);
}

// Construct and return a node for a test on a discrete attribute
void C45Learner::DiscreteTest(Tree Node, Attribute Att)
{
    Sprout(Node, m_dataSet->getDiscretNumber(Att));

    Node->NodeType	= BrDiscr;
    Node->Tested	= Att;
    Node->Errors	= 0;

//	m_LogTrace.WriteLine("Node ID %d(%d)\n", Node->nodeID, Node->NodeType);
}

// Change a leaf into a test on a continuous attribute
void C45Learner::ContinTest(Tree Node, Attribute Att)
{
    double Thresh;

    Sprout(Node, 2);

    Thresh = GreatestValueBelow(Att, Bar[Att]);

    Node->NodeType	= ThreshContin;
    Node->Tested	= Att;
    Node->Cut		=
    Node->Lower		=
    Node->Upper		= Thresh;
    Node->Errors    = 0;

//	m_LogTrace.WriteLine("Node ID %d(%d)\n", Node->nodeID, Node->NodeType);

}

// Return the greatest value of attribute Att below threshold t
double C45Learner::GreatestValueBelow(Attribute Att, double t)
{
    ItemNo i;
    double v, Best;
    bool NotYet=true;

    ForEach(i, 0, MaxItem)
    {
		v = CVal(i, Att);
		if ( v != VUnknown && v <= t && ( NotYet || v > Best ) )
		{
		    Best = v;
			NotYet = false;
		}
    }

    return Best;
}

// Grow and prune TRIALS trees and select the best of them
short C45Learner::BestTree()
{
    short t, Best=0;

    InitialiseTreeData();

    TargetClassFreq = (ItemNo *) new ItemNo[MaxClass+1];

    Raw    = (Tree *) new Tree[TRIALS];
    ForEach(t, 0, TRIALS-1 )
		Raw[t] = NULL;

    Pruned = (Tree *) new Tree[TRIALS];
    ForEach(t, 0, TRIALS-1 )
		Pruned[t] = NULL;

    /*  If necessary, set initial size of window to 20% (or twice
	the sqrt, if this is larger) of the number of data items,
	and the maximum number of items that can be added to the
	window at each iteration to 20% of the initial window size  */

    if ( ! WINDOW )
		WINDOW = (int) Max(2.0*sqrt((double)MaxItem+1.0), (double)(MaxItem+1)/5.0);

    if ( ! INCREMENT )
		INCREMENT = Max(WINDOW / 5, 1);

    FormTarget(WINDOW);

    /*  Form set of trees by iteration and prune  */

    ForEach(t, 0, TRIALS-1 )
    {
        FormInitialWindow();

//		printf("\n--------\nTrial %d\n--------\n\n", t);

		Raw[t] = Iterate(WINDOW, INCREMENT);

//		printf("\n");
//		PrintTree(Raw[t]);

//		SaveTree(Raw[t], ".unpruned");

		Pruned[t] = CopyTree(Raw[t]);
		if ( Prune(Pruned[t]) )
		{
//			printf("\nSimplified ");
//			PrintTree(Pruned[t]);
		}

		if ( Pruned[t]->Errors < Pruned[Best]->Errors )
			Best = t;
    }

	delete [] TargetClassFreq;

//	printf("\n--------\n");

    return Best;
}



/*************************************************************************/
/*									 */
/*  The windowing approach seems to work best when the class		 */
/*  distribution of the initial window is as close to uniform as	 */
/*  possible.  FormTarget generates this initial target distribution,	 */
/*  setting up a TargetClassFreq value for each class.			 */
/*									 */
/*************************************************************************/

void C45Learner::FormTarget(ItemNo Size)
{
    ItemNo i, *ClassFreq;
    ClassNo c, Smallest, ClassesLeft=0;

    ClassFreq = (ItemNo *) new ItemNo[MaxClass+1];

    /*  Generate the class frequency distribution  */
    ForEach(i, 0, MaxItem)
		ClassFreq[ Class(i) ]++;

    /*  Calculate the no. of classes of which there are items  */
    ForEach(c, 0, MaxClass)
    {
		if ( ClassFreq[c] )
		    ClassesLeft++;
		else
			TargetClassFreq[c] = 0;
    }

    while ( ClassesLeft )
    {
		/*  Find least common class of which there are some items  */
		Smallest = -1;
		ForEach(c, 0, MaxClass)
		{
		    if ( ClassFreq[c] &&
			 ( Smallest < 0 || ClassFreq[c] < ClassFreq[Smallest] ) )
			{
				Smallest = c;
			}
		}

		/*  Allocate the no. of items of this class to use in the window  */
		TargetClassFreq[Smallest] = Min(ClassFreq[Smallest], Round(Size/ClassesLeft));

		ClassFreq[Smallest] = 0;

		Size -= TargetClassFreq[Smallest];
		ClassesLeft--;
    }

    delete [] ClassFreq;
	ClassFreq = NULL;
}


/*************************************************************************/
/*									 */
/*  Form initial window, attempting to obtain the target class profile	 */
/*  in TargetClassFreq.  This is done by placing the targeted number     */
/*  of items of each class at the beginning of the set of data items.	 */
/*									 */
/*************************************************************************/

void C45Learner::FormInitialWindow()
{
    ItemNo i, Start=0, More;
    ClassNo c;

    Shuffle();

    ForEach(c, 0, MaxClass)
    {
		More = TargetClassFreq[c];

		for ( i = Start ; More ; i++ )
		{
		    if ( Class(i) == c )
			{
				Swap(Start, i);
				Start++;
				More--;
		    }
		}
    }
}

// Shuffle the data items randomly
void C45Learner::Shuffle()
{
    ItemNo This, Alt, Left;
	double ran;
//	Description Hold;

    This = 0;
    for( Left = MaxItem+1 ; Left ; )
    {
		ran = Random;
        Alt = This + (int)((double)Left*Random);
		Left--;
//		Alt = This + (Left--) * Random;
		m_dataSet->Swap(This, Alt);
		++This;

//		Hold = Item[This];
//		Item[This++] = Item[Alt];
//		Item[Alt] = Hold;

    }
}


/*************************************************************************/
/*									 */
/*  Grow a tree iteratively with initial window size Window and		 */
/*  initial window increment IncExceptions.				 */
/*									 */
/*  Construct a classifier tree using the data items in the		 */
/*  window, then test for the successful classification of other	 */
/*  data items by this tree.  If there are misclassified items,		 */
/*  put them immediately after the items in the window, increase	 */
/*  the size of the window and build another classifier tree, and	 */
/*  so on until we have a tree which successfully classifies all	 */
/*  of the test items or no improvement is apparent.			 */
/*									 */
/*  On completion, return the tree which produced the least errors.	 */
/*									 */
/*************************************************************************/

Tree C45Learner::Iterate(ItemNo Window, ItemNo IncExceptions)
{
    Tree Classifier, BestClassifier=Nil;
    ItemNo i, Errors, TotalErrors, BestTotalErrors=MaxItem+1,
	   Exceptions, Additions;
    ClassNo Assigned;
    short Cycle=0;

//	printf("Cycle   Tree    -----Cases----");
//	printf("    -----------------Errors-----------------\n");
//	printf("        size    window   other");
//	printf("    window  rate   other  rate   total  rate\n");
//	printf("-----   ----    ------  ------");
//	printf("    ------  ----  ------  ----  ------  ----\n");

    do
    {
		/*  Build a classifier tree with the first Window items  */
		InitialiseWeights();
		AllKnown = true;
		Classifier = FormTree(0, Window-1);

		/*  Error analysis  */
		Errors = Round(Classifier->Errors);

		/*  Move all items that are incorrectly classified by the
	    classifier tree to immediately after the items in the
	    current window.  */

		Exceptions = Window;
		ForEach(i, Window, MaxItem)
		{
		    Assigned = Category(i, Classifier);
		    if ( Assigned != Class(i) )
		    {
				Swap(Exceptions, i);
				Exceptions++;
		    }
		}
        Exceptions -= Window;
		TotalErrors = Errors + Exceptions;

//		/*  Print error analysis  */
//		printf("%3d  %7d  %8d  %6d  %8d%5.1f%%  %6d%5.1f%%  %6d%5.1f%%\n",
//			++Cycle, TreeSize(Classifier), Window, MaxItem-Window+1,
//			Errors, 100*(double)Errors/Window,
//			Exceptions, 100*Exceptions/(MaxItem-Window+1.001),
//			TotalErrors, 100*TotalErrors/(MaxItem+1.0));

		/*  Keep track of the most successful classifier tree so far  */
		if ( ! BestClassifier || TotalErrors < BestTotalErrors )
		{
			if ( BestClassifier )
				ReleaseTree(BestClassifier);
		    BestClassifier = Classifier;
		    BestTotalErrors = TotalErrors;
        }
		else
			ReleaseTree(Classifier);

		/*  Increment window size  */
		Additions = Min(Exceptions, IncExceptions);
		Window = Min(Window + Max(Additions, Exceptions / 2), MaxItem + 1);
    } while ( Exceptions );

    return BestClassifier;
}


// Categorize a case description using the given decision tree
ClassNo C45Learner::Category(int caseNo, Tree DecisionTree)
{
    ClassNo c, BestClass;

    if ( ! ClassSum )
		ClassSum = (double *) new double[MaxClass+1];

    ForEach(c, 0, MaxClass)
		ClassSum[c] = 0.0;

    Classify(caseNo, DecisionTree, 1.0);

//	m_LogTrace.WriteLine(" %d       ", caseNo);

    BestClass = 0;
    ForEach(c, 0, MaxClass)
    {
//		Verbosity(5) printf("class %s weight %.2f\n", ClassName[c], ClassSum[c]);
//		m_LogTrace.WriteLine(" %.2f       ", ClassSum[c]);

		if ( ClassSum[c] > ClassSum[BestClass] )
			BestClass = c;
    }
//	m_LogTrace.WriteLine("\n");

    return BestClass;
}



/*************************************************************************/
/*                                                              	 */
/*  Classify a case description using the given subtree by adjusting	 */
/*  the value ClassSum for each class					 */
/*                                                              	 */
/*************************************************************************/
void C45Learner::Classify(int caseNo, Tree T, double fWeight)
{
    DiscrValue v, dv;
    double Cv;
    Attribute a;
    ClassNo c;

    switch ( T->NodeType )
    {
        case 0:  /* leaf */

		    if ( T->Items > 0 )
		    {
				/*  Update from ALL classes  */
				ForEach(c, 0, MaxClass)
				{
				    if ( T->ClassDist[c] )
						ClassSum[c] += fWeight * T->ClassDist[c] / T->Items;
				}
		    }
			else
				ClassSum[T->Leaf] += fWeight;

		    return;

		case BrDiscr:  /* test of discrete attribute */

		    a = T->Tested;
		    v = DVal(caseNo, a);

		    if ( v && v <= T->Forks )	/*  Make sure not new discrete value  */
				Classify(caseNo, T->Branch[v], fWeight);
		    else
		    {
				ForEach(v, 1, T->Forks)
				    Classify(caseNo, T->Branch[v], (fWeight * T->Branch[v]->Items) / T->Items);
		    }

		    return;

		case ThreshContin:  /* test of continuous attribute */

		    a = T->Tested;
		    Cv = CVal(caseNo, a);

		    if ( Cv == VUnknown )
		    {
				ForEach(v, 1, 2)
				    Classify(caseNo, T->Branch[v], (fWeight * T->Branch[v]->Items) / T->Items);
			}
		    else
		    {
				v = ( Cv <= T->Cut ? 1 : 2 );
				Classify(caseNo, T->Branch[v], fWeight);
			}

		    return;

		case BrSubset:  /* subset test on discrete attribute  */

		    a = T->Tested;
		    dv = DVal(caseNo, a);

		    if ( dv )
		    {
				ForEach(v, 1, T->Forks)
				{
				    if ( In(dv, T->Subset[v]) )
					{
						Classify(caseNo, T->Branch[v], fWeight);
						return;
					}
				}
		    }

		    /*  Value unknown or not found in any of the subsets  */
		    ForEach(v, 1, T->Forks)
				Classify(caseNo, T->Branch[v], (fWeight * T->Branch[v]->Items) / T->Items);

		    return;
    } 
}


// Free up space taken up by tree Node
void C45Learner::ReleaseTree(Tree Node)
{
    DiscrValue v;

	if(!Node)
		return;

//	m_LogTrace.WriteLine("Releasing Node %d \n", Node->nodeID);
//	m_LogTrace.WriteLine("   ID %d\n", Node->nodeID);
//	m_LogTrace.WriteLine(" Type %d\n", Node->NodeType);
//	m_LogTrace.WriteLine("Forks %d\n", Node->Forks);

	if ( Node->NodeType )
    {
		if(Node->Branch)
		{
			ForEach(v, 1, Node->Forks)
				ReleaseTree(Node->Branch[v]);
			delete [] Node->Branch;
		}
		
		if ( Node->NodeType == BrSubset && Node->Subset)
		{
		    ForEach(v, 1, Node->Forks)
			{
				if(Node->Subset[v])
					delete [] Node->Subset[v];
			}
			delete [] Node->Subset;
		}
    }

	if(Node->ClassDist)
	    delete [] Node->ClassDist;

    delete Node;
}


// Return a copy of tree T
Tree C45Learner::CopyTree(Tree T)
{
    DiscrValue v;
    Tree m_newTree;
	ClassNo c;

    m_newTree = (Tree) new TreeRec();

	m_newTree->version = T->version;
	m_newTree->nodeID = T->nodeID;
	m_newTree->NodeType = T->NodeType;
	m_newTree->Leaf = T->Leaf;
	m_newTree->Items = T->Items;
	m_newTree->Errors = T->Errors;
	m_newTree->Tested = T->Tested;
	m_newTree->Forks = T->Forks;
	m_newTree->Cut = T->Cut;
	m_newTree->Lower = T->Lower;
	m_newTree->Upper = T->Upper;

	m_newTree->m_pLearner = T->m_pLearner;

//    memcpy(New, T, sizeof(TreeRec));

    m_newTree->ClassDist = (ItemCount *) new ItemCount[MaxClass+1];
	ForEach(c, 0, MaxClass)
		m_newTree->ClassDist[c] = T->ClassDist[c]; 
//    memcpy(New->ClassDist, T->ClassDist, (MaxClass + 1) * sizeof(ItemCount));

    if ( T->NodeType )
    {
		m_newTree->Branch = (Tree *) new Tree[T->Forks + 1];
		ForEach(v, 1, T->Forks)
			m_newTree->Branch[v] = CopyTree(T->Branch[v]);
    }

    return m_newTree;
}


// Prune tree T, returning true if tree has been modified
bool C45Learner::Prune(Tree T)
{
    ItemNo i;
    Attribute a;
	short attVal;

    InitialiseWeights();
    AllKnown = true;

//	Verbosity(1) printf("\n");

    Changed = false;

    EstimateErrors(T, 0, MaxItem, 0, true);

    if ( SUBSET )
    {
		if ( ! PossibleValues )
		    PossibleValues = (Set *) new Set[MaxAtt+1];

		ForEach(a, 0, MaxAtt)
		{
			attVal = m_dataSet->getDiscretNumber(a);
			if ( attVal )
		    {
				PossibleValues[a] = (Set) new char[(attVal>>3) + 1];
				ClearBits((attVal>>3) + 1, PossibleValues[a]);
				ForEach(i, 1, attVal)
				{
					SetBit(i, PossibleValues[a]);
				}
			}
		}

		CheckPossibleValues(T);



		ForEach(a, 0, MaxAtt)
		{
			if ( m_dataSet->getDiscretNumber(a) )
				delete [] PossibleValues[a];
		}
		delete [] PossibleValues;
		PossibleValues = NULL;

    }

    return Changed;
}


// Estimate the errors in a given subtree
double C45Learner::EstimateErrors(Tree T, ItemNo Fp, ItemNo Lp, short Sh, bool UpdateTree)
{
    ItemNo i, Kp, Ep;
    ItemCount Cases, KnownCases, *LocalClassDist, TreeErrors, LeafErrors,
	ExtraLeafErrors, BranchErrors, Factor, MaxFactor;
    DiscrValue v, MaxBr;
    ClassNo c, BestClass;
	bool PrevAllKnown;

    /*  Generate the class frequency distribution  */
    Cases = CountItems(Fp, Lp);
    LocalClassDist = (ItemCount *) new ItemCount[MaxClass+1];
	ForEach(i, 0, MaxClass)
		LocalClassDist[i] = 0.0;

    ForEach(i, Fp, Lp)
		LocalClassDist[ Class(i) ] += Weight[i];

	/*
	m_LogTrace.WriteLine("Local Class Distribution\n");
	ForEach(i, 0, MaxClass)
		m_LogTrace.WriteLine("%d : %lf\n", i, LocalClassDist[i]);
	m_LogTrace.WriteLine("\n");
	*/

    /*  Find the most frequent class and update the tree  */
    BestClass = T->Leaf;
    ForEach(c, 0, MaxClass)
    {
		if ( LocalClassDist[c] > LocalClassDist[BestClass] )
			BestClass = c;
    }
    LeafErrors = Cases - LocalClassDist[BestClass];
    ExtraLeafErrors = AddErrs(Cases, LeafErrors);

    if ( UpdateTree )
    {
		T->Items = Cases;
		T->Leaf  = BestClass;

		ForEach(c, 0, MaxClass)
			T->ClassDist[c] = LocalClassDist[c]; 
//		memcpy(T->ClassDist, LocalClassDist, (MaxClass + 1) * sizeof(ItemCount));
    }

    if ( ! T->NodeType )	/*  leaf  */
    {
		TreeErrors = LeafErrors + ExtraLeafErrors;

		if ( UpdateTree )
		{
			T->Errors = TreeErrors;

//		    LocalVerbosity(1)
//		    {
//				Intab(Sh);
//		    	printf("%s (%.2f:%.2f/%.2f)\n", ClassName[T->Leaf], T->Items, LeafErrors, T->Errors);
//			}

		}

		delete [] LocalClassDist;
		LocalClassDist = NULL;

		return TreeErrors;
    }

    /*  Estimate errors for each branch  */
    Kp = Group(0, Fp, Lp, T) + 1;
    KnownCases = CountItems(Kp, Lp);

    PrevAllKnown = AllKnown;
    if ( Kp != Fp )
		AllKnown = false;

    TreeErrors = MaxFactor = 0;

    ForEach(v, 1, T->Forks)
    {
		Ep = Group(v, Kp, Lp, T);

		if ( Kp <= Ep )
		{
			Factor = CountItems(Kp, Ep) / KnownCases;

		    if ( Factor >= MaxFactor )
			{
				MaxBr = v;
				MaxFactor = Factor;
		    }

		    ForEach(i, Fp, Kp-1)
		    {
				Weight[i] *= Factor;
		    }

		    TreeErrors += EstimateErrors(T->Branch[v], Fp, Ep, Sh+1, UpdateTree);

		    Group(0, Fp, Ep, T);
		    ForEach(i, Fp, Kp-1)
			{
				Weight[i] /= Factor;
		    }
		}
    }
 
    AllKnown = PrevAllKnown;

    if ( ! UpdateTree )
    {
		delete [] LocalClassDist;
		LocalClassDist = NULL;

		return TreeErrors;
    }

    /*  See how the largest branch would fare  */
    BranchErrors = EstimateErrors(T->Branch[MaxBr], Fp, Lp, -1000, false);

//	LocalVerbosity(1)
//	{
//		Intab(Sh);
//		printf("%s:  [%d%%  N=%.2f  tree=%.2f  leaf=%.2f+%.2f  br[%d]=%.2f]\n",
//			AttName[T->Tested],
//			(int) ((TreeErrors * 100) / (T->Items + 0.001)),
//			T->Items, TreeErrors, LeafErrors, ExtraLeafErrors,
//			MaxBr, BranchErrors);
//	}

    /*  See whether tree should be replaced with leaf or largest branch  */

    if ( LeafErrors + ExtraLeafErrors <= BranchErrors + 0.1 &&
	 LeafErrors + ExtraLeafErrors <= TreeErrors + 0.1 )
    {
//		LocalVerbosity(1)
//		{
//			Intab(Sh);
//			printf("Replaced with leaf %s\n", ClassName[T->Leaf]);
//		}

		T->NodeType = 0;
		T->Errors = LeafErrors + ExtraLeafErrors;
		Changed = true;
    }
    else if ( BranchErrors <= TreeErrors + 0.1 )
    {
//		LocalVerbosity(1)
//		{
//			Intab(Sh);
//			printf("Replaced with branch %d\n", MaxBr);
//		}

		AllKnown = PrevAllKnown;
		EstimateErrors(T->Branch[MaxBr], Fp, Lp, Sh, true);
		memcpy((char *) T, (char *) T->Branch[MaxBr], sizeof(TreeRec));
		Changed = true;
    }
    else
    {
		T->Errors = TreeErrors;
    }

    AllKnown = PrevAllKnown;

    delete [] LocalClassDist;
	LocalClassDist = NULL;

    return T->Errors;
}


// Remove unnecessary subset tests on missing values
void C45Learner::CheckPossibleValues(Tree T)
{
    Set HoldValues;
    int v, Bytes, b;
    Attribute A;
    char Any=0;
	short attVal;

    if ( T->NodeType == BrSubset )
    {
		A = T->Tested;

		attVal = m_dataSet->getDiscretNumber(A);
		Bytes = (attVal>>3) + 1;
		HoldValues = (Set) new char[Bytes];

		/*  See if last (default) branch can be simplified or omitted  */
		ForEach(b, 0, Bytes-1)
		{
		    T->Subset[T->Forks][b] &= PossibleValues[A][b];
			Any |= T->Subset[T->Forks][b];
		}

		if ( ! Any )
			T->Forks--;

		/*  Process each subtree, leaving only values in branch subset  */
		CopyBits(Bytes, PossibleValues[A], HoldValues);

		ForEach(v, 1, T->Forks)
		{
		    CopyBits(Bytes, T->Subset[v], PossibleValues[A]);
		    CheckPossibleValues(T->Branch[v]);
		}

		CopyBits(Bytes, HoldValues, PossibleValues[A]);

		delete [] HoldValues;
    }
    else if ( T->NodeType )
    {
		ForEach(v, 1, T->Forks)
		{
			CheckPossibleValues(T->Branch[v]);
		}
    }
}


/*************************************************************************/
/*									 */
/*  Compute the additional errors if the error rate increases to the	 */
/*  upper limit of the confidence level.  The coefficient is the	 */
/*  square of the number of standard deviations corresponding to the	 */
/*  selected confidence level.  (Taken from Documenta Geigy Scientific	 */
/*  Tables (Sixth Edition), p185 (with modifications).)			 */
/*									 */
/*************************************************************************/


double Val[] = {  0,  0.001, 0.005, 0.01, 0.05, 0.10, 0.20, 0.40, 1.00},
      Dev[] = {4.0,  3.09,  2.58,  2.33, 1.65, 1.28, 0.84, 0.25, 0.00};

double C45Learner::AddErrs(ItemCount N, ItemCount e)
{
    static double Coeff=0;
    double Val0, Pr;

    if ( ! Coeff )
    {
		/*  Compute and retain the coefficient value, interpolating from
	    the values in Val and Dev  */

		int i;
		i = 0;
		while ( CF > Val[i] )
			i++;

		Coeff = Dev[i-1] + (Dev[i] - Dev[i-1]) * (CF - Val[i-1]) /(Val[i] - Val[i-1]);
		Coeff = Coeff * Coeff;
    }

    if ( e < 1E-6 )
    {
		return N * (1 - exp(log(CF) / N));
    }
    else if ( e < 0.9999 )
    {
		Val0 = N * (1 - exp(log(CF) / N));
		return Val0 + e * (AddErrs(N, 1.0) - Val0);
    }
    else if ( e + 0.5 >= N )
    {
		return 0.67 * (N - e);
    }
    else
    {
		Pr = (e + 0.5 + Coeff/2 + sqrt(Coeff * ((e + 0.5) * (1 - (e + 0.5)/N) + Coeff/4)) ) / (N + Coeff);
		return (N * Pr - e);
    }
}


// Soften all thresholds for continuous attributes in tree T
void C45Learner::SoftenThresh(Tree T)
{
    CVals = (double *) new double[MaxItem+1];
    LHSErr = (short *) new short[MaxItem+1];
    RHSErr = (short *) new short[MaxItem+1];
    ThreshErrs = (ItemNo *) new ItemNo[MaxItem+1];

    InitialiseWeights();

    ScanTree(T, 0, MaxItem);

    delete [] ThreshErrs;
	ThreshErrs = NULL;
	
    delete [] RHSErr;
	RHSErr = NULL;

    delete [] LHSErr;
	LHSErr = NULL;

    delete [] CVals;
	CVals = NULL;
}


/*************************************************************************/
/*								  	 */
/*  Calculate upper and lower bounds for each test on a continuous	 */
/*  attribute in tree T, using data items from Fp to Lp			 */
/*								  	 */
/*************************************************************************/

void C45Learner::ScanTree(Tree T, ItemNo Fp, ItemNo Lp)
{
    short v;
    double Val, Se, Limit, Lower, Upper;
    ItemNo i, Kp, Ep, LastI, Errors, BaseErrors;
    ClassNo CaseClass, Class1, Class2;
    bool LeftThresh=false;
//	Description CaseDesc;
    Attribute Att;

    /*  Stop when get to a leaf  */
    if ( ! T->NodeType )
		return;

    /*  Group the unknowns together  */
    Kp = Group(0, Fp, Lp, T);

    /*  Soften a threshold for a continuous attribute  */
    Att = T->Tested;

    if ( T->NodeType == ThreshContin )
    {
//		printf("\nTest %s <> %g\n", AttName[Att], T->Cut);

		Quicksort(Kp+1, Lp, Att);

		ForEach(i, Kp+1, Lp)
		{
			/*  See how this item would be classified if its
			value were on each side of the threshold  */

//			CaseDesc = Item[i];
			CaseClass = Class(i);
			Val = CVal(i, Att);
		
			Class1 = Category(i, T->Branch[1]);
			Class2 = Category(i, T->Branch[2]);

			CVals[i] = Val;
			LHSErr[i] = (Class1 != CaseClass ? 1 : 0);
			RHSErr[i] = (Class2 != CaseClass ? 1 : 0);
		}

		/*  Set Errors to total errors if take above thresh branch,
	    and BaseErrors to errors if threshold has original value  */

		Errors = BaseErrors = 0;
		ForEach(i, Kp+1, Lp)
		{
		    Errors += RHSErr[i];

		    if ( Below(CVals[i], T->Cut) )
		    {
				BaseErrors += LHSErr[i];
			}
		    else
		    {
				BaseErrors += RHSErr[i];
		    }
		}

		/*  Calculate standard deviation of the number of errors  */
		Se = sqrt( (BaseErrors+0.5) * (Lp-Kp-BaseErrors+0.5) / (Lp-Kp+1) );
		Limit = BaseErrors + Se;

//		Verbosity(1)
//		{
//			printf("\t\t\tBase errors %d, items %d, se=%.1f\n", BaseErrors, Lp-Kp, Se);
//			printf("\n\tVal <=   Errors\t\t+Errors\n");
//			printf("\t         %6d\n", Errors);
//		}

		/*  Set ThreshErrs[i] to the no. of errors if the threshold were i  */
		ForEach(i, Kp+1, Lp)
		{
			ThreshErrs[i] = Errors = Errors + LHSErr[i] - RHSErr[i];

			if ( i == Lp || CVals[i] != CVals[i+1] )
			{
//				Verbosity(1)
//					printf("\t%6g   %6d\t\t%7d\n",CVals[i], Errors, Errors - BaseErrors);
			}
		}

		/*  Choose Lower and Upper so that if threshold were set to
	    either, the number of items misclassified would be one
	    standard deviation above BaseErrors  */

		LastI = Kp+1;
		Lower = Min(T->Cut, CVals[LastI]);
		Upper = Max(T->Cut, CVals[Lp]);
		while ( CVals[LastI+1] == CVals[LastI] )
			LastI++;

		while ( LastI < Lp )
		{
			i = LastI + 1;
			while ( i < Lp && CVals[i+1] == CVals[i] )
				i++;

			if ( ! LeftThresh && ThreshErrs[LastI] > Limit &&
				ThreshErrs[i] <= Limit && Below(CVals[i], T->Cut) )
			{
				Lower = CVals[i] - (CVals[i] - CVals[LastI]) * (Limit - ThreshErrs[i]) / (ThreshErrs[LastI] - ThreshErrs[i]);
				LeftThresh = true;
			}
			else if ( ThreshErrs[LastI] <= Limit && ThreshErrs[i] > Limit && ! Below(CVals[i], T->Cut) )
			{
				Upper = CVals[LastI] + (CVals[i] - CVals[LastI]) * (Limit - ThreshErrs[LastI]) / (ThreshErrs[i] - ThreshErrs[LastI]);
				if ( Upper < T->Cut )
					Upper = T->Cut;
			}

			LastI = i;
		}

		T->Lower = Lower;
		T->Upper = Upper;

//		Verbosity(1) printf("\n");
//		printf("\tLower = %g, Upper = %g\n", T->Lower, T->Upper);
    }

    /*  Recursively scan each branch  */
    ForEach(v, 1, T->Forks)
    {
		Ep = Group(v, Kp+1, Lp, T);

		if ( Kp < Ep )
		{
			ScanTree(T->Branch[v], Kp+1, Ep);
			Kp = Ep;
		}
    }
}

// Print report of errors for each of the trials
void C45Learner::Evaluate(bool CMInfo, short Saved)
{
//	ClassNo RawClass;
    ClassNo RealClass, PrunedClass;
    short t;
//	ItemNo *ConfusionMat;
	ItemNo i, RawErrors, PrunedErrors;
	int pos;

    if ( CMInfo )
	{
		if(ConfusionMat)
			delete [] ConfusionMat;
		ConfusionMat = (ItemNo *) new ItemNo[(MaxClass+1)*(MaxClass+1)];
		ForEach(pos, 0, (MaxClass+1)*(MaxClass+1) - 1 )
			ConfusionMat[pos] = 0;

//		ConfusionMat = (ItemNo *) new ItemNo[(MaxClass+1)*(MaxClass+1)];
//	    ForEach(pos, 0, (MaxClass+1)*(MaxClass+1) - 1 )
//			ConfusionMat[pos] = 0;
	}

//    printf("\n");

//	if ( TRIALS > 1 )
//	{
//		printf("Trial\t Before Pruning           After Pruning\n");
//		printf("-----\t----------------   ---------------------------\n");
//	}
//	else
//	{
//		printf("\t Before Pruning           After Pruning\n");
//		printf("\t----------------   ---------------------------\n");
//	}

//	printf("\tSize      Errors   Size      Errors   Estimate\n\n");

    ForEach(t, 0, TRIALS-1)
    {
		RawErrors = PrunedErrors = 0;

		ForEach(i, 0, MaxItem)
		{
		    RealClass = Class(i);

//			RawClass = Category(i, Raw[t]);
//			if ( RawClass != RealClass )
//				RawErrors++;

			PrunedClass = Category(i, Pruned[t]);

			if ( PrunedClass != RealClass )
				PrunedErrors++;

		    if ( CMInfo && t == Saved )
			{
				pos = RealClass*(MaxClass+1)+PrunedClass;
//				pos = RealClass*(MaxClass+1)+RawClass;
				ConfusionMat[pos]++;
			}
		}
	}

//	m_LogTrace.WriteLine(" %d -- %d\n", RawErrors, PrunedErrors);

    if ( CMInfo )
    {
		CalPrecisionRecall(MaxClass + 1);
		PrintConfusionMatrix();
//		delete [] ConfusionMat;
    }

}

void C45Learner::Evaluate(bool CMInfo, Tree T)
{
    ClassNo RealClass, PrunedClass;
//	ItemNo *ConfusionMat;
	ItemNo i, RawErrors, PrunedErrors;
	int pos;

    if ( CMInfo )
	{
		if(ConfusionMat)
			delete [] ConfusionMat;
		ConfusionMat = (ItemNo *) new ItemNo[(MaxClass+1)*(MaxClass+1)];
		ForEach(pos, 0, (MaxClass+1)*(MaxClass+1) - 1 )
			ConfusionMat[pos] = 0;
	}

	RawErrors = PrunedErrors = 0;

	ForEach(i, 0, MaxItem)
	{
	    RealClass = Class(i);

		PrunedClass = Category(i, T);

//		m_LogTrace.WriteLine("%d <--> %d\n", RealClass, PrunedClass);

		if ( PrunedClass != RealClass )
			PrunedErrors++;

	    if ( CMInfo )
		{
			pos = RealClass*(MaxClass+1)+PrunedClass;
			ConfusionMat[pos]++;
		}
	}

    if ( CMInfo )
    {
		CalPrecisionRecall(MaxClass + 1);
		PrintConfusionMatrix();

		delete [] ConfusionMat;
		ConfusionMat = NULL;
    }
}



// Routine for printing confusion matrices
void C45Learner::PrintConfusionMatrix()
{
    short Row, Col;
	int attNum;

    if ( MaxClass > 20 )
		return;  /* Don't print nonsensical matrices */

	attNum = m_dataSet->getAttSize();
	CString buffer;

    /*  Print the heading, then each row  */
    m_LogTrace.WriteLine("\n\n\t");
    ForEach(Col, 0, MaxClass)
    {
		m_LogTrace.WriteLine("  (%c)", 'a' + Col);
    }

    m_LogTrace.WriteLine("\t<-classified as\n\t");
    ForEach(Col, 0, MaxClass)
    {
		m_LogTrace.WriteLine(" ----");
    }
    m_LogTrace.WriteLine("\n");

    ForEach(Row, 0, MaxClass)
    {
		m_LogTrace.WriteLine("\t");
		ForEach(Col, 0, MaxClass)
		{
			if ( ConfusionMat[Row*(MaxClass+1) + Col] )
			{
				m_LogTrace.WriteLine("%5d", ConfusionMat[Row*(MaxClass+1) + Col]);
			}
			else
			{
				m_LogTrace.WriteLine("     ");
			}
		}
		m_dataSet->getDiscretValue(attNum - 1, Row, buffer);
		m_LogTrace.WriteLine("\t(%c): %s\n", 'a' + Row, buffer);
    }
    m_LogTrace.WriteLine("\n");

    ForEach(Row, 0, MaxClass)
	{
		m_LogTrace.WriteLine(" %lf %lf\n", Precision[Row], Recall[Row]);
	}
    m_LogTrace.WriteLine("\n");

}

// scan the whole tree and print out the node type and ID
//DEL void C45Learner::ScanTree(Tree T)
//DEL {
//DEL     DiscrValue v;
//DEL 
//DEL //	m_LogTrace.WriteLine("NodeID : %d, Type : %d\n", T->nodeID, T->NodeType);
//DEL 
//DEL     if ( T->NodeType )
//DEL     {
//DEL 		ForEach(v, 1, T->Forks)
//DEL 		{
//DEL 			m_LogTrace.WriteLine("%d -> %d;\n", T->nodeID, T->Branch[v]->nodeID);
//DEL 			ScanTree(T->Branch[v]);
//DEL 		}
//DEL     }
//DEL }

void C45Learner::ExportDot(Tree T, const char *dot, const char *graph)
{
	FILE *fp;
	if((fp = fopen(dot, "wt")) == NULL)
		return;

	if(graph)
		fprintf(fp, "digraph %s {\n", graph);
	else
		fprintf(fp, "digraph G {\n");

	ExportDotLine(T, fp);

	fprintf(fp, "}");
	fclose(fp);
}

void C45Learner::ExportDotLine(Tree T, FILE *fp)
{
    DiscrValue v;
	short pNode, cNode;
	CString pName;
	CString cName;

    if ( T->NodeType )
    {
		pNode = T->Tested;
		m_dataSet->getAttName(pNode, pName);

		ForEach(v, 1, T->Forks)
		{
			cNode = T->Branch[v]->Tested;
			cName.Empty();
			m_dataSet->getAttName(cNode, cName);

			if(!cName.IsEmpty())
				fprintf(fp, "%s -> %s;\n", pName, cName);
			ExportDotLine(T->Branch[v], fp);
		}
    }
}

/*
// Save entire decision tree T in file with extension Extension
void C45Learner::SaveTree(Tree T, const char *m_fileName)
{
	FILE *TRf;
	if((TRf = fopen(m_fileName, "w")) == NULL)
		return;

    putc('\n', TRf);
    OutTree(T, TRf);

	fflush(TRf);
	fclose(TRf);

//    SaveDiscreteNames(TRf);
}

// Save tree T as characters
void C45Learner::OutTree(Tree T, FILE *TRf)
{
	DiscrValue v;
	int Bytes, attVal;

	m_LogTrace.WriteLine("Node : %d, Leaf : %d\n", T->NodeType, T->Leaf);

	StreamOut((char *) &T->NodeType, sizeof(short), TRf);
	StreamOut((char *) &T->Leaf, sizeof(ClassNo), TRf);
	StreamOut((char *) &T->Items, sizeof(ItemCount), TRf);
	StreamOut((char *) &T->Errors, sizeof(ItemCount), TRf);
	StreamOut((char *) T->ClassDist, (MaxClass + 1) * sizeof(ItemCount), TRf);

	if ( T->NodeType )
	{
		StreamOut((char *) &T->Tested, sizeof(Attribute), TRf);
		StreamOut((char *) &T->Forks, sizeof(short), TRf);

		switch ( T->NodeType )
		{
			case BrDiscr:
				break;

			case ThreshContin:
				StreamOut((char *) &T->Cut, sizeof(double), TRf);
				StreamOut((char *) &T->Lower, sizeof(double), TRf);
				StreamOut((char *) &T->Upper, sizeof(double), TRf);
				break;

			case BrSubset:
				attVal = m_dataSet->getDiscretNumber(T->Tested);
				Bytes = (attVal>>3) + 1;
				ForEach(v, 1, T->Forks)
				{
					StreamOut((char *) T->Subset[v], Bytes, TRf);
				}

				break;
		}

		ForEach(v, 1, T->Forks)
		{
			OutTree(T->Branch[v], TRf);
		}
	}
}



// Retrieve entire decision tree with extension Extension
Tree C45Learner::GetTree(const char *m_fileName)
{
	FILE *TRf;

	TRf = fopen(m_fileName, "r");
	if(TRf == NULL || getc(TRf) == EOF)
		return NULL;

	Tree Hold;

    Hold = InTree(TRf);
	fclose(TRf);

//    RecoverDiscreteNames(TRf);
    return Hold;
}


// Retrieve tree from saved characters
Tree C45Learner::InTree(FILE *TRf)
{
	Tree T;
	DiscrValue v;
	int Bytes, attVal;

	T = (Tree) new TreeRec();

	StreamIn((char *) &T->NodeType, sizeof(short), TRf);
	StreamIn((char *) &T->Leaf, sizeof(ClassNo), TRf);
	StreamIn((char *) &T->Items, sizeof(ItemCount), TRf);
	StreamIn((char *) &T->Errors, sizeof(ItemCount), TRf);

	m_LogTrace.WriteLine("Node : %d, Leaf : %d\n", T->NodeType, T->Leaf);

	T->ClassDist = (ItemCount *) new ItemCount[MaxClass+1];
	StreamIn((char *) T->ClassDist, (MaxClass + 1) * sizeof(ItemCount), TRf);

	if ( T->NodeType )
	{
		StreamIn((char *) &T->Tested, sizeof(Attribute), TRf);
		StreamIn((char *) &T->Forks, sizeof(short), TRf);

		switch ( T->NodeType )
		{
			case BrDiscr:
				break;

			case ThreshContin:
				StreamIn((char *) &T->Cut, sizeof(double), TRf);
				StreamIn((char *) &T->Lower, sizeof(double), TRf);
				StreamIn((char *) &T->Upper, sizeof(double), TRf);
				break;

			case BrSubset:
				T->Subset = (Set *) new Set[T->Forks + 1];

				attVal = m_dataSet->getDiscretNumber(T->Tested);
				Bytes = (attVal>>3) + 1;
				ForEach(v, 1, T->Forks)
				{
					T->Subset[v] = (Set) new char[Bytes];
					StreamIn((char *) T->Subset[v], Bytes, TRf);
				}
		}

		T->Branch = (Tree *) new Tree[T->Forks + 1];
		ForEach(v, 1, T->Forks)
		{
			T->Branch[v] = InTree(TRf);
		}
	}

	return T;
}
*/

/*
///////////////////////////////////////////////////////////////////////////
//	Save attribute values read with "discrete N"
///////////////////////////////////////////////////////////////////////////
void C45Learner::SaveDiscreteNames()
{
    Attribute Att;
    DiscrValue v;
    int Length;

    ForEach(Att, 0, MaxAtt)
    {
		if ( m_dataSet->isAttContinuous(Att) ) continue;

		StreamOut((char *) &MaxAttVal[Att], sizeof(int));

		ForEach(v, 1, MaxAttVal[Att])
		{
			Length = strlen(AttValName[Att][v]) + 1;

		    StreamOut((char *) &Length, sizeof(int));
		    StreamOut((char *) AttValName[Att][v], Length);
		}
    }
}

///////////////////////////////////////////////////////////////////////////
//	Recover attribute values read with "discrete N"
///////////////////////////////////////////////////////////////////////////
void C45Learner::RecoverDiscreteNames()
{
    Attribute Att;
    DiscrValue v;
    int Length;

    ForEach(Att, 0, MaxAtt)
    {
		if ( m_dataSet->isAttContinuous(Att) ) continue;

		StreamIn(&MaxAttVal[Att], sizeof(int));

		ForEach(v, 1, MaxAttVal[Att])
		{
			StreamIn(&Length, sizeof(int));

		    AttValName[Att][v] = (char *) malloc(Length);
		    StreamIn(AttValName[Att][v], Length);
		}
    }
}
*/

/*
// Stream characters to/from file TRf from/to an address
void C45Learner::StreamOut(char *s, int n, FILE *TRf)
{
	while ( n-- )
		putc(*s++, TRf);
}

void C45Learner::StreamIn(char *s, int n, FILE *TRf)
{
	while ( n-- )
		*s++ = getc(TRf);
}
*/

int C45Learner::TreeSize()
{
	if(DecisionTree)
		return TreeSize(DecisionTree);

	return -1;
}

int C45Learner::TreeSize(Tree Node)
{
    int Sum=0;
    DiscrValue v;

    if ( Node->NodeType )
    {
		ForEach(v, 1, Node->Forks)
		{
			Sum += TreeSize(Node->Branch[v]);
		}
    }

    return Sum + 1;
}


// predict the classes of the items in the dataset
void C45Learner::Predict(Tree T)
{
    ClassNo PredictedClass, RealClass;
    ItemNo i;

	ForEach(i, 0, MaxItem)
	{
	    RealClass = Class(i);

		PredictedClass = Category(i, T);

//		m_LogTrace.WriteLine("%d <--> %d\n", RealClass, PredictedClass);

		SetClass(i, PredictedClass);
	}
}

void C45Learner::SetClass(int caseNo, int pClass)
{
	m_dataSet->m_data[caseNo][m_dataSet->getAttSize() - 1] = (double)pClass;
}



////////////////////////////////////////////////////////////////////////////
// the following is for the visulization

void C45Learner::CalcLayout()
{
	double unit;

	max_depth = -9999;

	m_pTrainData = NULL;
	m_labelInternal = FALSE;
	m_labelLeaf = FALSE;

	CalcLeafNum(DecisionTree, 0);

//	unit = 2.0*3.1415926535/(double)m_root->num_leaf;
	unit = 1.0/(double)DecisionTree->num_leaf;
	CalcDegree(DecisionTree, unit);
}

void C45Learner::CalcLeafNum(Tree pStart, int m_depth)
{
    DiscrValue v;
	Tree pChild;

	pStart->num_leaf = 0;

	if(pStart->NodeType == 0)
	{
		pStart->num_leaf = 1;
		max_depth = max(m_depth, max_depth);
		return;
	}

	ForEach(v, 1, pStart->Forks)
	{
		pChild = pStart->Branch[v];
		CalcLeafNum(pChild, m_depth + 1);

		pStart->num_leaf += pChild->num_leaf;
	}

	return;
}

void C45Learner::CalcDegree(Tree pStart, double m_unit)
{
    DiscrValue v;
	pStart->degree = (double)pStart->num_leaf * m_unit;

    if ( pStart->NodeType )
    {
		ForEach(v, 1, pStart->Forks)
		{
			CalcDegree(pStart->Branch[v], m_unit);
		}
    }
}

void C45Learner::DrawMarker(CDC &dc, int x, int y)
{
	CPen pen( PS_SOLID, 2, RGB(0,0,255) ), *pPenOld;
	CBrush brush( RGB(0,0,255) ), *pBrushOld;
	CRect rectMarker;

	pPenOld = dc.SelectObject( &pen );
	pBrushOld = dc.SelectObject( &brush );

	rectMarker.left = x - 3;
	rectMarker.top = y - 3;
	rectMarker.right = x + 3;
	rectMarker.bottom = y + 3;
	dc.Ellipse( rectMarker);

	dc.SelectObject( pPenOld );
	dc.SelectObject( pBrushOld );
	return;
}

void C45Learner::DrawArrow(CDC &dc, int startX, int startY, int endX, int endY)
{
	double slopy , cosy , siny;
	double Par = 10.0;//length of Arrow (>)
	slopy = atan2( ( startY - endY ),
		( startX - endX ) );
	cosy = cos( slopy );
	siny = sin( slopy );

	//draw a line between the 2 endpoint
	dc.MoveTo( startX, startY );
	dc.LineTo( endX, endY );
	
	//here is the tough part - actually drawing the arrows
	//a total of 6 lines drawn to make the arrow shape
	dc.MoveTo( endX, endY );
	dc.LineTo( endX + int( Par * cosy - ( Par / 2.0 * siny ) ),
		endY + int( Par * siny + ( Par / 2.0 * cosy ) ) );
	dc.MoveTo( endX + int( Par * cosy + Par / 2.0 * siny ),
		endY - int( Par / 2.0 * cosy - Par * siny ) );
	dc.LineTo( endX, endY );
}

void C45Learner::DrawText(CDC &pDC, const CString &strText, int px, int py, COLORREF clrText)
{
	/*
	int nCharCount = strText.GetLength();
	CPoint Location;

	pDC.SetBkMode(TRANSPARENT);

	Location.x = px;
	Location.y = py;

	//
	// Set the font:
	//
	CFont font;
	LOGFONT lf;										//Stores this fonts LogFont for quick retrieval
	lf.lfHeight			= 10;
	lf.lfWidth			= 9;
	lf.lfEscapement		= 0;
	lf.lfOrientation	= 0;
	lf.lfWeight			= FW_NORMAL;				//FW_NORMAL;
	lf.lfItalic			= 0;
	lf.lfUnderline		= 0;
	lf.lfStrikeOut		= 0;
	lf.lfCharSet		= ANSI_CHARSET;
	lf.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	lf.lfQuality		= ANTIALIASED_QUALITY;
	lf.lfPitchAndFamily	= VARIABLE_PITCH | FF_ROMAN;
	
	strcpy( lf.lfFaceName, "Courier New" );
	
	font.CreateFontIndirect(&lf);

	CFont* pOldFont = pDC.SelectObject(&font);

	pDC.SetTextColor( RGB(255,0,0) );

	CSize size;

	GetTextExtentPoint32(	pDC.m_hDC,				// handle to device context
							(LPCTSTR)strText,		// pointer to text string
							strText.GetLength(),	// number of characters in string
							&size);					// pointer to structure for string size

	CRect TextRect(Location, size);
	pDC.SetTextColor( RGB(255,0,0) );
	pDC.DrawText(strText, &TextRect, DT_LEFT);		//  << The Text DrawText Command >>

	// Reset the Device Context :
	pDC.SelectObject(pOldFont);

	*/



	RECT rc;
	int len;
	TEXTMETRIC m_tm;

	pDC.GetTextMetrics(&m_tm);
	len = strText.GetLength();
	pDC.SetTextColor( clrText );

	rc.left = px - (len*m_tm.tmAveCharWidth)/2;
	rc.right = px + len*m_tm.tmAveCharWidth;
	rc.top = py+5;
	rc.bottom = py + 25;

	pDC.DrawText(strText, -1, &rc, DT_TOP|DT_NOPREFIX|DT_SINGLELINE);
}



////////////////////////////////////////////////////////////////////////
// Draw Disc Tree
// 
////////////////////////////////////////////////////////////////////////
void C45Learner::DrawDiscTree(CDC &dc)
{
	ASSERT(m_pTrainData);


	CWnd *pWnd;
	CRect rectArea;
	CRect rectCircue;

	pWnd = dc.GetWindow();
	pWnd->GetClientRect(&rectArea);

	m_radius = min(rectArea.Width(), rectArea.Height())/2 - 4.0;
	centreY = rectArea.top + rectArea.Height()/2;
	centreX = rectArea.left + rectArea.Width()/2;

	CPen pen (PS_SOLID, 1, RGB(192,192,192)), *pOldPen;
	double m_circue;
	pOldPen = dc.SelectObject(&pen);

	for(int i = max_depth; i > 0; i--)
	{
		m_circue = (double)i/(double)max_depth*m_radius;
		rectCircue.top = centreY - (int)m_circue;
		rectCircue.bottom = centreY + (int)m_circue;
		rectCircue.left = centreX - (int)m_circue;
		rectCircue.right = centreX + (int)m_circue;
		dc.Ellipse(rectCircue);
	}
	dc.SelectObject(pOldPen);

	DrawDiscTreeNode(dc, DecisionTree, 0.0, 0);

}

void C45Learner::DrawDiscTreeNode(CDC &dc, Tree pStart, double offset, int m_depth)
{
	double sumDegree = 0, degree, m_circue;
	Tree pChild;
	int px, py, cx, cy;
    DiscrValue v;
	CString strValue;
	CString strLabel;
	CString strBranch;

	m_circue = (double)m_depth/(double)max_depth*m_radius;
	px = centreX + (int)(m_circue*cos(offset+pStart->degree*3.1415926535));
	py = centreY - (int)(m_circue*sin(offset+pStart->degree*3.1415926535));

	DrawMarker(dc, px, py);

	if(pStart->NodeType == 0)
	{
		if(m_labelLeaf)
		{
			m_pTrainData->getClassName(pStart->Leaf, strValue);
			strLabel.Format("%s : %lf", strValue, pStart->Errors);

			DrawText(dc, strValue, px, py);
		}

		return;
	}

	if(m_labelInternal)
	{
		m_pTrainData->getAttName(pStart->Tested, strValue);
		DrawText(dc, strValue, px, py);
	}

	ForEach(v, 1, pStart->Forks)
	{
		pChild = pStart->Branch[v];

		degree = offset + sumDegree;

		m_circue = (double)(m_depth+1)/(double)max_depth*m_radius;

		cx = centreX + (int)(m_circue*cos(degree+pChild->degree*3.1415926535));
		cy = centreY - (int)(m_circue*sin(degree+pChild->degree*3.1415926535));

		DrawArrow(dc, px, py, cx, cy);

		if(m_labelBranch)
		{
			strBranch.Empty();

		    switch ( pStart->NodeType )
		    {
				case BrDiscr:
					m_pTrainData->getDiscretValue(pStart->Tested, v-1, strBranch);
//				    printf("%s = %s:", AttName[Att], AttValName[Att][v]);
				    break;

				case ThreshContin:
					if(v == 1)
						strBranch.Format("<=%lf", pStart->Cut);
					else
						strBranch.Format(">%lf", pStart->Cut);

//				    if ( T->Lower != T->Upper )
//				    {
//						printf("[%g,%g]", T->Lower, T->Upper);
//				    }
				    break;
			}

			DrawText(dc, strBranch, (px+cx)/2, (py+cy)/2, RGB(0, 0, 255));
		}


		DrawDiscTreeNode(dc, pChild, degree, m_depth+1);

		sumDegree += pChild->degree*2.0*3.1415926535;
	}

	return;
}

////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// Draw Disc Tree
// 
////////////////////////////////////////////////////////////////////////
void C45Learner::DrawDecisionTree(CDC &dc)
{
	ASSERT(m_pTrainData);

	CWnd *pWnd;
	CRect rectArea;

	pWnd = dc.GetWindow();
	pWnd->GetClientRect(&rectArea);

	width = rectArea.Width() - 4;
	height = rectArea.Height() - 10;

	DrawDecisionTreeNode(dc, DecisionTree, 0.0, 0);
}

void C45Learner::DrawDecisionTreeNode(CDC &dc, Tree pStart, double offset, int m_depth)
{
	double sumDegree = 0, degree, m_circue;
	Tree pChild;
	int px, py, cx, cy;
    DiscrValue v;
	CString strValue;
	CString strLabel;
	CString strBranch;

	px = (int)(offset + pStart->degree * (double)width / 2.0);
	py = (int)((double)m_depth/(double)max_depth*(double)height) + 5;

	DrawMarker(dc, px, py);

	if(pStart->NodeType == 0)
	{
		if(m_labelLeaf)
		{
			m_pTrainData->getClassName(pStart->Leaf, strValue);
			strLabel.Format("%s : %lf", strValue, pStart->Errors);
			DrawText(dc, strValue, px, py);
		}

		return;
	}

	if(m_labelInternal)
	{
		m_pTrainData->getAttName(pStart->Tested, strValue);
		DrawText(dc, strValue, px, py);
	}

	sumDegree = 0.0;

	ForEach(v, 1, pStart->Forks)
	{
		pChild = pStart->Branch[v];

		degree = offset + sumDegree;

		m_circue = (double)(m_depth+1)/(double)max_depth*m_radius;

		cx = (int)(degree + pChild->degree * (double)width / 2.0);
		cy = (int)((double)(m_depth+1)/(double)max_depth*(double)height) + 5;

		DrawArrow(dc, px, py, cx, cy);

		if(m_labelBranch)
		{
			strBranch.Empty();

		    switch ( pStart->NodeType )
		    {
				case BrDiscr:
					m_pTrainData->getDiscretValue(pStart->Tested, v-1, strBranch);
//				    printf("%s = %s:", AttName[Att], AttValName[Att][v]);
				    break;

				case ThreshContin:
					if(v == 1)
						strBranch.Format("<=%lf", pStart->Cut);
					else
						strBranch.Format(">%lf", pStart->Cut);

//				    if ( T->Lower != T->Upper )
//				    {
//						printf("[%g,%g]", T->Lower, T->Upper);
//				    }
				    break;
			}

			DrawText(dc, strBranch, (px+cx)/2, (py+cy)/2, RGB(0, 0, 255));
		}


		DrawDecisionTreeNode(dc, pChild, degree, m_depth+1);

		sumDegree += pChild->degree * (double)width;
	}

	return;
}




////////////////////////////////////////////////////////////////////////////
// the following is for producing the rules from the root to the leaves

void C45Learner::getQueryRules(CRuleSet &ruleSet)
{
	TraverseConditions(DecisionTree);
	GetRuleSet(DecisionTree, ruleSet);
}

void C45Learner::SaveRules(const CString &strRule)
{
	TraverseConditions(DecisionTree);

	CRuleSet m_ruleSet;
	GetRuleSet(DecisionTree, m_ruleSet);
	m_ruleSet.save(strRule);

	PrintConditions(DecisionTree);
	m_ruleSet.dump();
}

// get all the rules
void C45Learner::GetRuleSet(Tree pStart, CRuleSet &ruleSet)
{
	CString strValue;
    DiscrValue v;
	Tree pChild;

	if(pStart->NodeType == 0)
	{
		m_dataSet->getClassName(pStart->Leaf, strValue);
		if(strValue.CompareNoCase("fully") == 0 && pStart->m_ruleRec.IsFullyQuery())
		{
			int pos = -1;
			for(int i = 0; i < ruleSet.m_ruleSet.GetSize(); i++)
			{
				if(pStart->m_ruleRec.GetSize() >= ruleSet.m_ruleSet[i].GetSize())
				{
					pos = i;
					break;
				}
			}

			if(pos >= 0)
				ruleSet.m_ruleSet.InsertAt(pos, pStart->m_ruleRec);
			else
				ruleSet.m_ruleSet.Add(pStart->m_ruleRec);

//			ruleSet.m_ruleSet.Add(pStart->m_ruleRec);
		}

		return;
	}

	ForEach(v, 1, pStart->Forks)
	{
		pChild = pStart->Branch[v];
		GetRuleSet(pChild, ruleSet);
	}
}


void C45Learner::PrintOutRules()
{
	TraverseConditions(DecisionTree);
	PrintConditions(DecisionTree);
}

void C45Learner::PrintConditions(Tree pStart)
{
	CString strValue;
    DiscrValue v;
	Tree pChild;
	FILE *fp;

	if(pStart->NodeType == 0)
	{
		fp = fopen("rules.txt", "a+t");

		m_dataSet->getClassName(pStart->Leaf, strValue);
		if(strValue.CompareNoCase("fully") == 0)
		{
			fprintf(fp, "%s <-- \n", strValue);
			fprintf(fp, "%s\n\n", pStart->m_strPath);
		}

		fflush(fp);
		fclose(fp);

//		m_LogTrace.WriteLine("%s <-- \n", strValue);
//		m_LogTrace.WriteLine("%s\n\n", pStart->m_strPath);

		return;
	}

	ForEach(v, 1, pStart->Forks)
	{
		pChild = pStart->Branch[v];
		PrintConditions(pChild);
	}
}


void C45Learner::TraverseConditions(Tree pStart)
{
	CString strAttName;
	CString strBranch;
	Tree pChild;
    DiscrValue v;

	if(pStart->NodeType == 0)
		return;

	m_dataSet->getAttName(pStart->Tested, strAttName);
	ForEach(v, 1, pStart->Forks)
	{
		pChild = pStart->Branch[v];

		strBranch.Empty();

	    switch ( pStart->NodeType )
	    {
			case BrDiscr:
				m_dataSet->getDiscretValue(pStart->Tested, v-1, strBranch);
				strBranch = _T(" ==") + strBranch;
				pChild->m_ruleRec.insert(1, pStart->Tested, strAttName, 0, v-1);
			    break;

			case ThreshContin:
				if(v == 1)
				{
					strBranch.Format(" <=%lf", pStart->Cut);
					pChild->m_ruleRec.insert(0, pStart->Tested, strAttName, -1, pStart->Cut);
				}
				else
				{
					strBranch.Format(" >%lf", pStart->Cut);
					pChild->m_ruleRec.insert(0, pStart->Tested, strAttName, 1, pStart->Cut);
				}

			    break;
		}

		if(!pStart->m_ruleRec.IsEmpty())
			pChild->m_ruleRec.append(pStart->m_ruleRec);

		if(pStart->m_strPath.IsEmpty())
			pChild->m_strPath = strAttName + strBranch;
		else
			pChild->m_strPath = pStart->m_strPath + _T(" &&\r\n") + strAttName + strBranch;

		TraverseConditions(pChild);
	}

	return;
}






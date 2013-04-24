// C45Learner.h: interface for the C45Learner class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_C45LEARNER_H__1532B046_9B37_441B_ACD3_EB85BEAB26BC__INCLUDED_)
#define AFX_C45LEARNER_H__1532B046_9B37_441B_ACD3_EB85BEAB26BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <math.h>
#include <stdio.h>

#include "DataSet.h"
#include "QueryLearner.h"

typedef  char	*Set;
typedef  int	ItemNo;		/* data item number */
typedef  double	ItemCount;	/* count of (partial) items */

typedef  short	ClassNo,	/* class number, 0..MaxClass */
		DiscrValue;	/* discrete attribute value (0 = ?) */
typedef  short	Attribute;	/* attribute number, 0..MaxAtt */

#define  VUnknown  -999		/* unknown value for continuous attribute */

#define  BrDiscr	1	/* node types:	branch */
#define  ThreshContin	2	/*		threshold cut */
#define  BrSubset	3	/*		subset test */

class C45Learner;

class CTreeRecord;
typedef class CTreeRecord *Tree;
typedef class CTreeRecord TreeRec;

class CTreeRecord
{
public:

	short	NodeType;	/* 0=leaf 1=branch 2=cut 3=subset */
	ClassNo	Leaf;		/* most frequent class at this node */
	ItemCount	Items,		/* no of items at this node */
		*ClassDist,	/* class distribution of items */
		Errors;		/* no of errors at this node */
	Attribute	Tested; 	/* attribute referenced in test */
	short	Forks;		/* number of branches at this node */
	double	Cut,		/* threshold for continuous attribute */
		Lower,		/* lower limit of soft threshold */
		Upper;		/* upper limit ditto */
	Set		*Subset;	/* subsets of discrete values  */
	Tree	*Branch;	/* Branch[x] = (sub)tree for outcome x */

	int nodeID;

	C45Learner *m_pLearner;

public:
	CTreeRecord()
	{
		version.Empty();
		m_strPath.Empty();
	    
		NodeType	= 0; 
		Branch	= NULL;
		Subset	= NULL;

		ClassDist = NULL;

		Forks = 0;
	}

    int serialize (CArchive& pArchive);
    int serializeNode (CArchive& pArchive);


public:
	double degree;
	int num_leaf;

	// the rule for each node, from the root of the tree
	CString m_strPath;
	CRuleRec m_ruleRec;

	CString version;

};


//typedef  struct _tree_record *Tree;
//typedef  struct _tree_record
//{
//	short	NodeType;	/* 0=leaf 1=branch 2=cut 3=subset */
//	ClassNo	Leaf;		/* most frequent class at this node */
//	ItemCount	Items,		/* no of items at this node */
//		*ClassDist,	/* class distribution of items */
//		Errors;		/* no of errors at this node */
//	Attribute	Tested; 	/* attribute referenced in test */
//	short	Forks;		/* number of branches at this node */
//	double	Cut,		/* threshold for continuous attribute */
//		Lower,		/* lower limit of soft threshold */
//		Upper;		/* upper limit ditto */
//	Set		*Subset;	/* subsets of discrete values  */
//	Tree	*Branch;	/* Branch[x] = (sub)tree for outcome x */
//
//	int nodeID;
//} TreeRec;


//#define  IGNORE		1	/* special attribute status: do not use */
#define  DISCRETE	2	/* ditto: collect values as data read */
#define	 Epsilon                1E-3

#define	 None			-1
#define	 Nil			0               /*null pointer*/

#define	ForEach(var,F,L)	for(var=F; var<=L; ++var) 

#define	 Bit(b)			(1 << (b))
#define	 In(b,s)		((s[(b) >> 3]) & Bit((b) & 07))

#define	 ClearBits(n,s)		memset(s,0,n)
#define	 CopyBits(n,f,t)	memcpy(t,f,n)
#define	 SetBit(b,s)		(s[(b) >> 3] |= Bit((b) & 07))

#define	 Round(x)		((int) (x+0.5))
#define	 Log2			0.69314718055994530942
#define	 Log(x)			((x) <= 0 ? 0.0 : log(x) / Log2)

#define	Below(v,t)	(v <= t + 1E-6)

#define	 Max(a,b)               ((a)>(b) ? a : b) 
#define	 Min(a,b)               ((a)<(b) ? a : b) 

#define	 Random			((double)rand() / (double)RAND_MAX)


class C45Learner  
{
public:
	C45Learner();
	virtual ~C45Learner();

	int TreeSize();
	int TreeSize(Tree Node);

public:
	// This is the main interface of C45
	// 1.
	//    setDataSet(pTrainData)
	//    buildClassifier()
	//    Save("***.tree");
	//
	// 2.
	//    setDataSet(pTrainData)
	//    buildClassifier()
	//    setDataSet(pTestData)
	//    Evaluate() or Predict()
	// 
	// 3.
	//    setDataSet(pTestData)
	//    Load("***.tree")
	//    Evaluate() or Predict()


	void setDataSet(CDataSet *pData);
	void buildClassifier();

	BOOL Save(const char *m_fileName);
	BOOL Load(const char *m_fileName);

//	void SaveTree(CArchive &pArchive, Tree T);
//	void LoadTree(CArchive &pArchive, Tree T);

	void Evaluate();
	ItemNo *ConfusionMat;
	double *Precision;
	double *Recall;

	void Predict();

	// to retrieve and set the version information of
	// the classifier
	void setVersion(const CString &strVersion)
	{
		m_strVersion = strVersion;
	}

	CString &getVersion()
	{
		return m_strVersion;
	}

protected:
	int		*TargetClassFreq;
	Tree	*Raw;
	Tree	*Pruned;

	Tree	DecisionTree;

	ItemCount *Weight;		// Weight[i]  = current fraction of item i 
	ItemCount **Freq;		// Freq[x][c] = no. items of class c with outcome x
	ItemCount *ValFreq;		// ValFreq[x]   = no. items with outcome x 
	ItemCount *ClassFreq;	// ClassFreq[c] = no. items of class c 

	double *Gain;		// Gain[a] = info gain by split on att a
	double *Info;		// Info[a] = potential info of split on att a 
	double *Bar;			// Bar[a]  = best threshold for contin att a 
	double *UnknownRate;	// UnknownRate[a] = current unknown rate for att a 

	bool *Tested;	// Tested[a] set if att a has already been tested 
	bool MultiVal;	// true when all atts have many values 


	//  External variables initialised here  
	double *SplitGain;	// SplitGain[i] = gain with att value of item i as threshold 
	double *SplitInfo;	// SplitInfo[i] = potential info ditto 

	ItemCount *Slice1;	// Slice1[c]    = saved values of Freq[x][c] in subset.c 
	ItemCount *Slice2;	// Slice2[c]    = saved values of Freq[y][c] 

	Set **Subset;	// Subset[a][s] = subset s for att a 

	short *Subsets;	// Subsets[a] = no. subsets for att a 

	ItemNo		MaxItem;	// MaxItem is set to the number of data items

	double	*ClassSum;		/* ClassSum[c] = total weight of class c */
	
	Set	*PossibleValues;
	bool Changed;

	// used for Soften thresholds for continuous attributes
	short *LHSErr;	/*  Does a misclassification occur with this value of an att  */
	short *RHSErr;	/*  if the below or above threshold branches are taken  */

	ItemNo	*ThreshErrs;	/*  ThreshErrs[i] is the no. of misclassifications if thresh is i  */

	double	*CVals;		/*  All values of a continuous attribute  */

	bool		AllKnown;

	CString m_strVersion;


public:
	void OneTree();
	void PrintConfusionMatrix();
	void CalPrecisionRecall(int numClass);
	void Evaluate(bool CMInfo, short Saved);
	void ScanTree(Tree T, ItemNo Fp, ItemNo Lp);
	void SoftenThresh(Tree T);
	double AddErrs(ItemCount N, ItemCount e);
	void CheckPossibleValues(Tree T);
	double EstimateErrors(Tree T, ItemNo Fp, ItemNo Lp, short Sh, bool UpdateTree);
	bool Prune(Tree T);
	Tree CopyTree(Tree T);
	void ReleaseTree(Tree Node);
	void Classify(int caseNo, Tree T, double fWeight);
	ClassNo Category(int caseNo, Tree DecisionTree);
	Tree Iterate(ItemNo Window, ItemNo IncExceptions);
	void Shuffle();
	void FormInitialWindow();
	void FormTarget(ItemNo Size);
	short BestTree();
	double GreatestValueBelow(Attribute Att, double t);
	void ContinTest(Tree Node, Attribute Att);
	void DiscreteTest(Tree Node, Attribute Att);
	void Sprout(Tree Node, DiscrValue Branches);
	void SubsetTest(Tree Node, Attribute Att);
	void Quicksort(ItemNo Fp, ItemNo Lp, Attribute Att);
	void EvalContinuousAtt(Attribute Att, ItemNo Fp, ItemNo Lp);
	void EvalDiscreteAtt(Attribute Att, ItemNo Fp, ItemNo Lp, ItemCount Items);
	void Uncombine(DiscrValue x, DiscrValue y);
	void Combine(DiscrValue x, DiscrValue y, DiscrValue Last);
	double Worth(double ThisInfo, double ThisGain, double MinGain);
	double TotalInfo(ItemCount V[], DiscrValue MinVal, DiscrValue MaxVal);
	double ComputeGain(double BaseInfo, double UnknFrac, DiscrValue MaxVal, ItemCount TotalItems);
	double DiscrKnownBaseInfo(ItemCount KnownItems, DiscrValue MaxVal);
	void ResetFreq(DiscrValue MaxVal);
	void ComputeFrequencies(Attribute Att, ItemNo Fp, ItemNo Lp);
	void EvalSubset(Attribute Att, ItemNo Fp, ItemNo Lp, ItemCount Items);
	double CVal(int caseNo, int Att);
	short DVal(int caseNo, int Att);
	void Swap(ItemNo a, ItemNo b);
	ItemNo Group(DiscrValue V, ItemNo Fp, ItemNo Lp, Tree TestNode);
	Tree Leaf(ItemCount *ClassFreq, ClassNo NodeClass, ItemCount Cases, ItemCount Errors);
	int Class(int caseNo);
	ItemCount CountItems(ItemNo Fp, ItemNo Lp);
	Tree FormTree(ItemNo Fp, ItemNo Lp);
	void InitialiseWeights();

	void InitialiseTreeData();
	void DeInitialiseTreeData();

//	Tree GetTree(const char *m_fileName);
//	Tree InTree(FILE *TRf);
//	void SaveTree(Tree T, const char *m_fileName);
//	void OutTree(Tree T, FILE *TRf);

//	void SaveDiscreteNames();
//	void RecoverDiscreteNames();

//	void StreamOut(char *s, int n, FILE *TRf);
//	void StreamIn(char *s, int n, FILE *TRf);

	void ExportDotLine(Tree T, FILE *fp);
	void ExportDot(Tree T, const char *dot, const char *graph=NULL);

	void SetClass(int caseNo, int pClass);
	void Predict(Tree T);
	void Evaluate(bool CMInfo, Tree T);

private:

public:
	// parameters of the C45 learner

	// Gain criterion used
	bool GAINRATIO;

	// Tests on discrete attribute groups
	bool SUBSET;

	// Trees evaluated on unseen cases
	bool UNSEENS;

	// Probability thresholds used
	bool PROBTHRESH;

	// Sensible test requires 2 branches with at least cases
	int MINOBJS;

	// Pruning confidence level
	double CF;

	// ------------------ in Batch ------------------
	// Windowing disabled (now the default)
	bool BATCH;

	// ------------------ in Windowing ------------------
	// number of trials for enabled Windowing 
	int TRIALS;
	// Initial window size of %d items
	int WINDOW;
	// Maximum window increment of %d items
	int INCREMENT;

	CDataSet *m_dataSet;

	short MaxAtt;			// maximum attribute number
	short MaxClass;			// maximum class number
	short MaxDiscrVal;		// maximum discrete values for any attribute


	// number of tree node involved in the decision tree
	int m_numNode;


	// the following is for the visulization
public:

	int max_depth;

	double m_radius;
	int centreX, centreY;

	int width;
	int height;

	CDataSet *m_pTrainData;

	BOOL m_labelInternal;
	BOOL m_labelBranch;
	BOOL m_labelLeaf;

public:
	void CalcLayout();
	void CalcLeafNum(Tree pStart, int m_depth);
	void CalcDegree(Tree pStart, double m_unit);

	void DrawMarker(CDC &dc, int x, int y);
	void DrawArrow(CDC &dc, int startX, int startY, int endX, int endY);
	void DrawText(CDC &pDC, const CString &strText, int px, int py, COLORREF clrText=RGB(255,0,0));

	void DrawDiscTree(CDC &dc);
	void DrawDiscTreeNode(CDC &dc, Tree pStart, double offset, int m_depth);

	void DrawDecisionTree(CDC &dc);
	void DrawDecisionTreeNode(CDC &dc, Tree pStart, double offset, int m_depth);


	// the following is for producing rules
public:
	void PrintOutRules();
	void SaveRules(const CString &strRule);
	void getQueryRules(CRuleSet &ruleSet);


protected:
	void TraverseConditions(Tree pStart);
	void PrintConditions(Tree pStart);
	void GetRuleSet(Tree pStart, CRuleSet &ruleSet);

};

#endif // !defined(AFX_C45LEARNER_H__1532B046_9B37_441B_ACD3_EB85BEAB26BC__INCLUDED_)

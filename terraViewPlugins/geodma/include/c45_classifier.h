#ifndef __C45_CLASSIFIER_H
#define __C45_CLASSIFIER_H

/**
  \file c45_classifier.h
  \brief This file contains structures and functions for Decision Tree Classifier, algorithm C4.5.
  \author Alexandre Copertino Jardim <alexcj@dpi.inpe.br>
  \author Thales Sehn Korting <tkorting@dpi.inpe.br>
*/

// #define _DEBUG 

#include <base_classifier.h>

#include <TeRaster.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

/**
* Estrutura de árvore para o c4.5
*/
class Tree
{
  public:
    int NodeType; /* 0=leaf 1=branch 2=cut 3=subset */
    int Leaf;   /* most frequent class at this node */
    float Items,    /* no of items at this node */
          //*ClassDist, /* class distribution of items */
          Errors;   /* no of errors at this node */
    int Tested;   /* attribute referenced in test */
    int Forks;    /* number of branches at this node */
    float Cut,    /* threshold for continuous attribute */
          Lower,    /* lower limit of soft threshold */
          Upper;    /* upper limit ditto */
    char *Subset; /* subsets of discrete values  */
    Tree **Branch;  /* Branch[x] = (sub)tree for outcome x */

    vector<float> classDist_;
};

typedef std::map<std::string, TeClassSample > TeClassColor;

/**
* Algoritimo c4.5 para classificação de dados
*/
class c45_classifier
{
  public:
    //! Construtor
    c45_classifier(vector<string> attName);

    //! Treinamento da estrutura de árvores
    bool train( TeClassSampleSet::iterator itBegin, TeClassSampleSet::iterator itEnd, const vector<int> dimensions );
  // 	int train( TeClassSampleSet &sampleSet, vector<string> attName );

    //! Classifica conjunto de dados de entrada
    bool classify( vector<TeClassSample>::iterator itBegin, vector<TeClassSample>::iterator itEnd, const vector<int> dimensions, list<int>& classifyResult);

    //! Carrega treinamento anterior
    void load_c45(string filename);

    //! Classifica segundo uma arvore gerada anteriormente
    bool classify_from_file( vector<TeClassSample>::iterator itBegin, vector<TeClassSample>::iterator itEnd, const vector<int> dimensions, list<int>& classifyResult, string filename);

    //! Retorna uma string contendo a árvore
    string get_tree();
    string format_number(double n);

    //! Monta a matriz de confusão.
    int confusionMatrix( TeClassSampleSet &valuateSet );

    /*!
    * Faz arquivos .name e .data para comparar com c4.5 original
    */
    void makeFiles();
    
    void print_test( TeClassSampleSet::iterator input, TeClassSampleSet::iterator input_end, const vector<int> vetor );

  private:
    typedef vector<float> ItemCount;

    void getData();

    //! Grow and prune a single tree from all data.
    void oneTree();

    void initialiseTreeData();

    //! Build a decision tree for the cases Fp through Lp.
    Tree* formTree( int fp, int lp, int actual_it );

    //! Construct a leaf in a given node.
    Tree* leaf( vector<float> classFreq, int nodeClass, int cases, int errors );

    //! This routine finds the best cut for items Fp through Lp and sets.
    void evalContinuousAtt( int att, int fp, int lp );

    //! Zero the frequency tables Freq[][] and ValFreq[] up to MaxVal.
    void resetFreq( int maxVal );

    void quicksort(int fp, int lp, int att );

    void exchange( int a, int b );

    //!  Compute the total information in V[ MinVal..MaxVal. 
    float totalInfo( ItemCount &v, int minVal, int maxVal );

    //! Given tables Freq[][] and ValFreq[], compute the information gain.
    float computeGain( float baseInfo, float unknFrac, int maxVal, float totalItems );

    //! Determine the worth of a particular split according to the operative criterion.
    float worth( float thisInfo, float thisGain, float minGain );

    //! Group together the items corresponding to branch V of a test and return the index of the last such.
    int group(int v, int fp, int lp, Tree* TestNode);

    //! Change a leaf into a test on a continuous attribute. 
    void continTest( Tree* node, int att );

    //! Construct and return a node for a test on a discrete attribute.
    void sprout( Tree* node, int branches );
    
    //! Return the greatest value of attribute Att below threshold t.
    float greatestValueBelow( int Att, float t );

    //! So para mostrar a árvore.
    void showTree( Tree* tree, int sh);

    //! So para mostrar a árvore.
    void showBranch(int sh, Tree* tree, int v);

    //! So para mostrar a árvore.
    int MaxLine(Tree* St);

    //! So para mostrar a árvore.
    void show(Tree* T, int Sh);

    //! return tree in a string
    string var_show(Tree* T, int Sh);
    string var_showBranch(int sh, Tree* tree, int v);

    TeClassColor *classColor_;
    TeClassSampleSet* sampleSet_;

    /*!
    * Classifica um Sample
    */
    int classifySample( Tree &tree, TeClassSample &sample );

    /*!
    * Endereço do primeiro no da árvore.
    */
    Tree* tree_;

    int maxClass_; /* class max number */

    vector<string> className_; /* class names */
    vector<string> attName_; /* att names */

    vector<float> gain_; /* Gain[a] = info gain by split on att a */
    vector<float> info_; /* Info[a] = potential info of split on att a */
    vector<float> bar_; /* Bar[a]  = best threshold for contin att a */

    vector<float> unknownRate_; /* UnknownRate[a] = current unknown rate for att a */

    vector<float> weight_; /* Weight[i]  = current fraction of item i */

    vector<ItemCount> freq_; /* Freq[x][c] = no. items of class c with outcome x */

    vector<float> valFreq_; /* ValFreq[x]   = no. items with outcome x */
    vector<float> classFreq_; /* ClassFreq[c] = no. items of class c */

    vector<float> slice1_; /* Slice1[c]    = saved values of Freq[x][c] in subset.c */
    vector<float> slice2_; /* Slice2[c]    = saved values of Freq[y][c] */

    vector<float> splitGain_; /* SplitGain[i] = gain with att value of item i as threshold */
    vector<float> splitInfo_; /* SplitInfo[i] = potential info ditto */

    typedef vector<int> set_;
    vector<set_> subset_; /* Subset[a][s] = subset s for att a */

    vector<int> subsets_; /* Subsets[a] = no. subsets for att a */
    
    vector<bool> tested_; /* Tested[a] set if att a has already been tested */

    const int MINOBJS;
    const float epsilon;
    const double LOG2;
    float Log( float x )
    {
      if( x<= 0) return 0.0;
      return (float) log(x) / LOG2;
    }//((x) <= 0 ? 0.0 : log((float)x) / Log2)

    bool multiVal_; /* true when all atts have many values */
    bool gainRatio_;
    bool allKnown_;

    int maxItem_; /* max data item number */
    int maxAtt_; /* max attribute nunber */

    vector<TeClassSample> item_;

    int maxDricrVal_;

    const int TabSize, Width;

    int subTree_;		/* highest subtree to be printed */
    Tree* subdef_[100];		/* pointers to subtrees */
	
};

#endif

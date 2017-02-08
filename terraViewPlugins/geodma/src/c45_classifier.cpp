#include <c45_classifier.h>

c45_classifier::c45_classifier( vector<string> attName ): MINOBJS(5), epsilon((float) 1E-3), LOG2(0.69314718055994530942), gainRatio_(true), allKnown_(true), maxDricrVal_(2), TabSize(4), Width(80), subTree_(0) 
{
  #ifdef _DEBUG
  cout << "c45_classifier()" << endl;
  #endif

  attName_.clear();
  for (unsigned i = 0; i < attName.size(); i++)
    attName_.push_back(attName[i]);
  maxAtt_ = attName_.size();
}

void c45_classifier::load_c45(string filename)
{
  TeClassSampleSet training_samples;
  TeClassSample training_sample;
  int total_classes,
      total_attributes,
      total_samples;
  double attribute;
  string class_name;

  ifstream input_file( filename.c_str() );
  input_file >> total_classes;
  for (int i = 0; i < total_classes; i++)
    input_file >> class_name;

  input_file >> total_attributes;
  vector<int> clas_dim;
  string tmp;
  for (int i = 0; i < total_attributes; i++)
  {
    input_file >> tmp;
    clas_dim.push_back(i);
  }

  input_file >> total_samples;
  for (int i = 0; i < total_samples; i++)
  {
    training_sample.clear();
    for (int i = 0; i < total_attributes; i++)
    {
      input_file >> attribute;
      training_sample.push_back(attribute);
    }
    input_file >> class_name;
    training_samples[class_name].push_back(training_sample);
  }
  input_file.close();

  train( training_samples.begin(), training_samples.end(), clas_dim );
}

//! Monta Matriz de confusão com base nos dados de validação.
int c45_classifier::confusionMatrix( TeClassSampleSet &valuateSet )
{
  int maxi, classNumber;
  int coluna;
  double overallAccuracy = 0.0;

  vector< vector<int> > confusionMatrix_;
  confusionMatrix_.resize( className_.size() );

  for(unsigned c=0; c<className_.size(); ++c )
    confusionMatrix_[c].resize( className_.size() );
  
  coluna = 0;
  TeClassSampleSet::const_iterator interator;
  interator = valuateSet.begin();
  for(; interator != valuateSet.end(); ++interator)
  {
    maxi=interator->second.size();
    for( int i = 0; i < maxi; ++i )
    {
      TeClassSample *tcs = new TeClassSample(interator->second[i]);
      classNumber = classifySample( *tree_, *tcs);
      confusionMatrix_[classNumber][coluna] += 1;
    }
    coluna++;
  }

  double total = 0.0;
  for( unsigned i = 0; i < className_.size(); ++i )
  {
    for( unsigned j = 0; j < className_.size(); ++j )
    {
      cout << setw(2) << confusionMatrix_[i][j] << "\t";
      total += confusionMatrix_[i][j];
      if( i == j)
        overallAccuracy += confusionMatrix_[i][j];
    }
    cout << endl;
  }

  overallAccuracy = overallAccuracy / total;

  cout << endl;
  cout << "Overall Accuracy: " << overallAccuracy << endl;

  return 0;
}


//! Zero the frequency tables Freq[][] and ValFreq[] up to MaxVal.
void c45_classifier::resetFreq( int maxVal )
{
  int v, c;
  
  for( v=0; v <= maxVal; ++v ) 
  { 
    for( c=0; c <= maxClass_; ++c )
      freq_[v][c] = 0;

    valFreq_[v] = 0;
  } 
}
/*************************************************************************/
/*                   */
/*  Group together the items corresponding to branch V of a test   */
/*  and return the index of the last such        */
/*                   */
/*  Note: if V equals zero, group the unknown values       */
/*                   */
/*************************************************************************/
int c45_classifier::group(int v, int fp, int lp, Tree* TestNode)
{
  int i;
  int att;
  float thresh;
  
  att = TestNode->Tested;

  if ( v )
  {
    /*  Group items on the value of attribute Att, and depending
      on the type of branch  */
    thresh = TestNode->Cut;
    for(i=fp; i<=lp; ++i)
      if ( (item_[i][att] <= thresh) == (v == 1) ) 
        exchange(fp++, i);
  }
  else
  {
    for(i=fp; i<=lp; ++i)
      if ( item_[i][att] == -999 ) 
        exchange(fp++, i);
  }

  return fp-1;
}

float c45_classifier::greatestValueBelow( int Att, float t )
{
  #ifdef _DEBUG
  //cout << "greatestValueBelow(" << Att << ", " << t << ")" << endl;
  #endif

  int i;
  float v, best;
  bool notYet = true;

  for(i=0; i<=maxItem_; ++i)
  {
    v = (float) item_[i][Att];
    if ( v != -999 && v <= t && ( notYet || v > best ) )
    {
      best = v;
      notYet = false;
    }
  }

  #ifdef _DEBUG
  //cout << "end greatestValueBelow(" << Att << ", " << t << ") -> " << best << endl;
  #endif

  return best;
}

void c45_classifier::sprout( Tree* node, int branches )
{
  node->Forks = branches;
  node->Branch = (Tree **) calloc(branches+1, sizeof(Tree*));
}

void c45_classifier::continTest( Tree* node, int att )
{
  float thresh;
  
  sprout(node, 2);

  thresh = greatestValueBelow(att, bar_[att]);

  node->NodeType = 2;
  node->Tested = att;
  node->Cut = thresh;
  node->Lower = thresh;
  node->Upper = thresh;
  node->Errors = 0;
}

/*************************************************************************/
/*                   */
/*  Determine the worth of a particular split according to the     */
/*  operative criterion              */
/*                   */
/*      Parameters:              */
/*    SplitInfo:  potential info of the split    */
/*    SplitGain:  gain in info of the split    */
/*    MinGain:  gain above which the Gain Ratio    */
/*        may be used        */
/*                   */
/*  If the Gain criterion is being used, the information gain of   */
/*  the split is returned, but if the Gain Ratio criterion is    */
/*  being used, the ratio of the information gain of the split to  */
/*  its potential information is returned.         */
/*                   */
/*************************************************************************/
float c45_classifier::worth( float thisInfo, float thisGain, float minGain)
{
  if ( gainRatio_ )
  {
    if ( thisGain >= minGain - epsilon && thisInfo > epsilon )
      return thisGain / thisInfo;
    else
      return -epsilon;
  }
  else
    return ( thisInfo > 0 && thisGain > -epsilon ? thisGain : -epsilon );
}

/*************************************************************************/
/*                   */
/*  Given tables Freq[][] and ValFreq[], compute the information gain.   */
/*                   */
/*      Parameters:              */
/*    BaseInfo: average information for all items with   */
/*        known values of the test attribute   */
/*    UnknownRate:  fraction of items with unknown ditto   */
/*    MaxVal:   number of forks        */
/*    TotalItems: number of items with known values of   */
/*        test att         */
/*                   */
/*  where Freq[x][y] contains the no. of cases with value x for a  */
/*  particular attribute that are members of class y,      */
/*  and ValFreq[x] contains the no. of cases with value x for a    */
/*  particular attribute             */
/*                   */
/*************************************************************************/
float c45_classifier::computeGain( float baseInfo, float unknFrac, int maxVal, float totalItems )
{
  #ifdef _DEBUG
  //cout << "computeGain(" << baseInfo << ", " << unknFrac  << ", " << maxVal << ", " << totalItems << ")" << endl;
  #endif

  int v;
  float thisInfo=0.0,thisGain;
  int reasonableSubsets=0;

  /*  Check whether all values are unknown or the same  */
  if ( !totalItems )
    return -epsilon;

  /*  There must be at least two subsets with MINOBJS items  */

  for( v = 1; v <= maxVal; ++v )
  {
    if ( valFreq_[v] >= MINOBJS )
      reasonableSubsets++;
  }

  if ( reasonableSubsets < 2 ) 
    return -epsilon;

  /*  Compute total info after split, by summing the
  info of each of the subsets formed by the test  */

  for( v = 1; v <= maxVal; ++v )
    thisInfo += totalInfo( freq_[v], 0, maxClass_ );

  /*  Set the gain in information for all items, adjusted for unknowns  */
  thisGain = ( 1 - unknFrac ) * ( baseInfo - thisInfo / totalItems );

  #ifdef _DEBUG
  //cout << "end computeGain(" << baseInfo << ", " << unknFrac  << ", " << maxVal << ", " << totalItems << ")-> " << thisGain << endl;
  #endif

  return thisGain;
}

float c45_classifier::totalInfo( ItemCount &v,  int minVal, int maxVal)
{
  #ifdef _DEBUG
  //cout << "totalInfo(" << minVal << ", " << maxVal << ")" << endl;
  #endif

  int i;
  float sum = 0.0;
  float n, totalItems = 0;

  for( i = minVal; i <= maxVal; ++i )
  {
    n = v[i];
    sum += n * Log( n );
    totalItems += n;
  }

  #ifdef _DEBUG
  //cout << "end totalInfo(" << minVal << ", " << maxVal << ") ->" << totalItems * Log(totalItems) - sum << endl;
  #endif

  return totalItems * Log(totalItems) - sum;
}

void c45_classifier::exchange( int a, int b )
{
  TeClassSample hold = item_[a];
  item_[a] = item_[b];
  item_[b] = hold;

  float holdw = weight_[a];
  weight_[a] = weight_[b];
  weight_[b] = holdw;
}

void c45_classifier::quicksort(int fp, int lp, int att )
{
  register float thresh;
  register int lower, middle, i;

  if( fp < lp )
  {
    thresh = (float) item_[lp][att];

    /*  Isolate all items with values <= threshold  */
    middle = fp;

    for ( i = fp ; i < lp ; i++ )
    { 
      if ( item_[i][att] <= thresh )
      { 
        if ( i != middle )
          exchange( middle, i );
        middle++; 
      } 
    }

    /*  Extract all values equal to the threshold  */
    lower = middle - 1;

    for ( i = lower; i >= fp; i-- )
    {
      if ( item_[i][att] == thresh )
      { 
        if ( i != lower )
          exchange( lower, i );
        lower--;
      } 
    } 

    /*  Sort the lower values  */
    quicksort(fp, lower, att);

    /*  Position the middle element  */
    exchange(middle, lp);

    /*  Sort the higher values  */
    quicksort(middle+1, lp, att);
  }
}

/*************************************************************************/
/*                     */
/*  Continuous attributes are treated as if they have possible values  */
/*  0 (unknown), 1 (less than cut), 2(greater than cut)      */
/*  This routine finds the best cut for items Fp through Lp and sets   */
/*  Info[], Gain[] and Bar[]             */
/*                     */
/*************************************************************************/
void c45_classifier::evalContinuousAtt( int att, int fp, int lp )
{
  #ifdef _DEBUG
  //cout << "evalContinuousAtt("<< att << ", " <<  fp << ", " << lp << ")" << endl;
  #endif

  int i;
  int knownItems, bestI;
  int c;
  int tries = 0;

  float minSplit, lowItems, avGain = 0, threshCost, bestVal, Val;

  resetFreq(2);
  int items = lp - fp + 1;
  int xp = fp;

  for( c = 0; c <= maxClass_; ++ c)
    valFreq_[0] += freq_[0][c];

  knownItems = items - (int) valFreq_[0];
  unknownRate_[att] = 1.0 - (float) knownItems / (float) items;

  quicksort( xp, lp, att );

  /*  Count base values and determine base information  */
  for( i = xp; i <= lp; ++i )
  {
    freq_[ 2 ][ item_[i][maxAtt_ + 1] ] += weight_[i]; //??????
    splitGain_[i] = -epsilon;
    splitInfo_[i] = 0;
  }
  float baseInfo = totalInfo( freq_[2], 0, maxClass_) / knownItems;

  minSplit = 0.10 * (float) knownItems / (float) ( maxClass_ + 1 );
  if ( minSplit <= MINOBJS )
    minSplit = (float) MINOBJS;
  else if ( minSplit > 25 )
    minSplit = 25;

  lowItems = 0;
  
  for( i = xp; i <= ( lp - 1 ); ++i )
  {
    c = item_[i][maxAtt_ + 1]; //?????
    lowItems += weight_[i];
    freq_[1][c] += weight_[i];
    freq_[2][c] -= weight_[i];

    if ( lowItems < minSplit ) 
      continue;
    else if ( lowItems > knownItems - minSplit ) 
      break;

    if ( item_[i][att] < item_[i+1][att] - 1E-5 )
    {
      valFreq_[1] = lowItems;
      valFreq_[2] = knownItems - lowItems;
      splitGain_[i] = computeGain( baseInfo, unknownRate_[att], 2, (float) knownItems );
      splitInfo_[i] = totalInfo( valFreq_, 0, 2 ) / items;
      avGain += splitGain_[i];
      tries++;
    }
  }
  threshCost = Log(tries) / (float) items;

  bestVal = 0;
  bestI = -1;
  for( i = xp; i < lp; ++i )
  {
    if ( ( Val = splitGain_[i] - threshCost ) > bestVal )
    {
      bestI = i;
      bestVal = Val;
    }
  }
  /*  If a test on the attribute is able to make a gain,
  set the best break point, gain and information  */ 
  if ( bestI == -1 )
  {
    gain_[att] = -epsilon;
    info_[att] = 0.0;
  }
  else
  {
    bar_[att]  = ( item_[bestI][att] + item_[bestI+1][att] ) / 2;
    gain_[att] = bestVal;
    info_[att] = splitInfo_[bestI];
  }

  #ifdef _DEBUG
  //cout << "end evalContinuousAtt()" << endl;
  #endif
}

void c45_classifier::initialiseTreeData()
{
  int i;
  multiVal_ = false;
  maxItem_ = 0;
  TeClassSampleSet::const_iterator interator;
  interator = sampleSet_->begin();
  for(; interator != sampleSet_->end(); ++interator)
    maxItem_ += interator->second.size();

  maxItem_--;
  maxAtt_--; // TIRAR ???
  maxClass_--; // TIRAR ???

  tested_.resize(maxAtt_+1);
  gain_.resize(maxAtt_+1);
  info_.resize(maxAtt_+1);
  bar_.resize(maxAtt_+1);
  subset_.resize( maxAtt_+1 );
  subsets_.resize( maxAtt_+1 );
  splitGain_.resize( maxItem_+1 );
  splitInfo_.resize( maxItem_+1 );
  weight_.resize( maxItem_+1 );
  freq_.resize( maxDricrVal_+1 );
  for(i=0; i<=maxDricrVal_; ++i)
    freq_[i].resize( maxClass_+1 );

  valFreq_.resize( maxDricrVal_+1 );
  classFreq_.resize( maxClass_+1 );
  slice1_.resize( maxClass_+2 );
  slice2_.resize( maxClass_+2 );
  unknownRate_.resize( maxAtt_+1 );
}

Tree* c45_classifier::leaf( vector<float> classFreq, int nodeClass, int cases, int errors )
{
  Tree* node = new Tree();
  node->classDist_ = classFreq;
  node->NodeType = 0; 
  node->Leaf = nodeClass;
  node->Items = (float) cases;
  node->Errors = (float) errors;

  return node;
}

/*************************************************************************/
/*!                  */
/*  Build a decision tree for the cases Fp through Lp:       */
/*                   */
/*  - if all cases are of the same class, the tree is a leaf and so  */
/*      the leaf is returned labelled with this class      */
/*                   */
/*  - for each attribute, calculate the potential information provided   */
/*  by a test on the attribute (based on the probabilities of each   */
/*  case having a particular value for the attribute), and the gain  */
/*  in information that would result from a test on the attribute  */
/*  (based on the probabilities of each case with a particular   */
/*  value for the attribute being of a particular class)     */
/*                   */
/*  - on the basis of these figures, and depending on the current  */
/*  selection criterion, find the best attribute to branch on.   */
/*  Note:  this version will not allow a split on an attribute   */
/*  unless two or more subsets have at least MINOBJS items.    */
/*                   */
/*  - try branching and test whether better than forming a leaf    */
/*                   */
/*************************************************************************/
Tree* c45_classifier::formTree( int fp, int lp, int actual_it )
{
  #ifdef _DEBUG
  cout << "formTree(" << fp << ", " << lp << ", " << actual_it << ")" << endl;
  #endif

  actual_it++;

  int i, kp, ep;
  int knownCases;
  float factor,bestVal, val, avGain=0;
  int att, bestAtt, possible=0;
  bool prevAllKnown;
  int cases = lp - fp + 1;

  /*  Generate the class frequency distribution  */
  for(i = 0; i <= maxClass_; ++i)
    classFreq_[i] =  0.0 ;

  for(i = fp; i <= lp; ++i)
  {
    classFreq_[ item_[i][maxAtt_ + 1] ]+= weight_[i]; //??????????
  }
  int bestClass = 0;
  for(i = 1; i <= maxClass_; ++i)
  {
    if( classFreq_[i] > classFreq_[bestClass])
      bestClass = i;
  }
  float noBestClass = classFreq_[bestClass];
  Tree* node = new Tree();

  node = leaf( classFreq_, bestClass, cases, cases - (int) noBestClass );
  /*  If all cases are of the same class or there are not enough
  cases to divide, the tree is a leaf  */
  if ( noBestClass == cases  || cases < 2 * MINOBJS )
  {
    return node;
  }
  for(att = 0; att <= maxAtt_; ++att)
  {
    gain_[att] = -epsilon;
    evalContinuousAtt(att, fp, lp);

    /*  Update average gain, excluding attributes with very many values  */
    if ( gain_[att] > -epsilon && ( multiVal_ || /*maxAttVal_[att]*/0 < 0.3 * (maxItem_ + 1) ) )
    {
      possible++;
      avGain += gain_[att];
    }
  }

  /*  Find the best attribute according to the given criterion  */
  bestVal = -epsilon;
  bestAtt = -1;
  avGain  = ( possible ? avGain / (float) possible : (float) 1E6 );

  for(att = 0; att <= maxAtt_; ++att)
  { 
    if ( gain_[att] > -epsilon )
    { 
      val = worth(info_[att], gain_[att], avGain);
      if ( val > bestVal ) 
      { 
        bestAtt  = att; 
        bestVal = val;
      } 
    }
  } 
  /*  Decide whether to branch or not  */ 
  if ( bestAtt != -1 )
  { 
    continTest(node, bestAtt);

    /*  Remove unknown attribute values  */
    prevAllKnown = allKnown_;
    kp = group(0, fp, lp, node) + 1;
    if ( kp != fp ) 
      allKnown_ = false;
    knownCases = cases - kp-1 - fp + 1;
    unknownRate_[bestAtt] = (float) (cases - knownCases) / (float) (cases + 0.001);

    /*  Recursive divide and conquer  */
    tested_[bestAtt] = true;
    ep = kp - 1;
    node->Errors = 0;
    int v;
    for(v = 1; v <= node->Forks; ++v)
    {
      ep = group(v, kp, lp, node);

      if ( kp <= ep && actual_it < 1000 )
      {
        factor = (ep - kp + 1) / (float) knownCases;

        for(i=fp; i<=kp-1; ++i)
          weight_[i] *= factor;

        node->Branch[v] = formTree(fp, ep, actual_it);
        node->Errors += node->Branch[v]->Errors;

        group(0, fp, ep, node);
        for(i = fp; i <= kp-1; ++i)
          weight_[i] /= factor;
      }
      else
        node->Branch[v] = leaf( node->classDist_, bestClass, 0, 0 );
    }
    tested_[bestAtt] = false;
    allKnown_ = prevAllKnown;

    /*  See whether we would have been no worse off with a leaf  */
    if ( node->Errors >= cases - noBestClass - epsilon )
      node->NodeType = 0;
  } 

  #ifdef _DEBUG
  cout << "end formTree()" << endl;
  #endif
  return node; 
}

void c45_classifier::oneTree()
{
  #ifdef _DEBUG
  //cout << "oneTree()" << endl;
  #endif

  initialiseTreeData();
  for(int i = 0; i <= maxItem_; ++i)
    weight_[i] =  1.0;

  tree_ = new Tree();
  tree_ = formTree(0, maxItem_, 0);

  #ifdef _DEBUG
  //cout << "end oneTree()" << endl;
  #endif
}

void c45_classifier::getData()
{
  #ifdef _DEBUG
  //cout << "getData()" << endl;
  #endif

  int i;
  int maxi;
  float classe = 0.0;

  TeClassSampleSet::const_iterator interator;
  interator = sampleSet_->begin();
  for(; interator != sampleSet_->end(); ++interator)
  {
    maxi=interator->second.size();
    for(i = 0; i < maxi; ++i)
    {
      TeClassSample description;
      description = interator->second[i];
      description.push_back( classe );
      item_.push_back( description );
    }
    classe+=1;
  }

  #ifdef _DEBUG
  cout << "end getData()" << endl;
  #endif
}

bool c45_classifier::classify( vector<TeClassSample>::iterator itBegin, vector<TeClassSample>::iterator itEnd, const vector<int> dimensions, list<int>& classifyResult)
{
  #ifdef _DEBUG
  cout << "classify()" << endl;
  #endif

  vector<TeClassSample> samples;
  vector<TeClassSample>::iterator attr_it = itBegin;
  while (attr_it != itEnd)
  {
    TeClassSample data = (*attr_it);
    TeClassSample tmp_input;
    for (unsigned i = 0; i < dimensions.size(); i++)
      tmp_input.push_back( data[ dimensions[ i ] ] );
    samples.push_back(tmp_input);
    ++attr_it;
  }

  for (unsigned i = 0; i < samples.size(); i++)
    classifyResult.push_back( classifySample( *tree_, samples[ i ] ) );

  #ifdef _DEBUG
  cout << "end classify()" << endl;
  #endif

  return true;
}

bool c45_classifier::classify_from_file( vector<TeClassSample>::iterator itBegin, vector<TeClassSample>::iterator itEnd, const vector<int> dimensions, list<int>& classifyResult, string filename)
{
  #ifdef _DEBUG
  cout << "classify_from_file()" << endl;
  #endif

  TeClassSampleSet training_samples;
  TeClassSample training_sample;
  int total_classes,
      total_attributes;
  string class_name;

  // fixing input file 
  string unformatted_filename = filename;
  filename = filename + ".tmp";
  ifstream unformatted_input_file( unformatted_filename.c_str() );
  ofstream formatted_input_file( filename.c_str() );

  string tmp;
  // print decision tree
  do
  {
    unformatted_input_file >> tmp;
    if ( tmp != "###" )
      formatted_input_file << tmp << " ";
  } while (tmp != "###");
  formatted_input_file << endl << "###" << endl;
  // print parameters
  unformatted_input_file >> total_classes;
  set<string> ordered_class_names;
  set<string>::iterator it_names;
  for (int i = 0; i < total_classes; i++)
  {
    unformatted_input_file >> tmp;
    ordered_class_names.insert( TeConvertToLowerCase( tmp ) );
  }
  formatted_input_file << total_classes << endl;
  for ( it_names = ordered_class_names.begin(); it_names != ordered_class_names.end(); it_names++ )
  {
    formatted_input_file << *it_names << endl;
  }
  unformatted_input_file >> total_attributes;
  set<string> ordered_attributes_names;
  for (int i = 0; i < total_attributes; i++)
  {
    unformatted_input_file >> tmp;
    ordered_attributes_names.insert( TeConvertToLowerCase( tmp ) );
  }
  formatted_input_file << total_attributes << endl;
  for ( it_names = ordered_attributes_names.begin(); it_names != ordered_attributes_names.end(); it_names++ )
  {
    formatted_input_file << *it_names << endl;
  }
  formatted_input_file << "###" << endl;
  formatted_input_file.close();

  ifstream input_file( filename.c_str() );
  // consume decision tree
  do
  {
    input_file >> tmp;
  } while (tmp != "###");

  // retrieve decision tree parameters: total_classes and total_attributes
  input_file >> total_classes;
  map<string, int> classes_indexes;
  for (int i = 0; i < total_classes; i++)
  {
    input_file >> class_name;
    classes_indexes[ class_name ] = i;
  }
  input_file >> total_attributes;
  vector<string> attributes_names;
  map<string, int> attributes_indexes;
  for (int i = 0; i < total_attributes; i++)
  {
    input_file >> tmp;
    attributes_names.push_back( tmp );
    attributes_indexes[ tmp ] = i;
  }
  input_file.close();

  // classify data
  vector<TeClassSample>::iterator attr_it = itBegin;
  while (attr_it != itEnd)
  {
    ifstream c45_file( filename.c_str() );
    TeClassSample data = ( *attr_it );
    TeClassSample sample;
    for (unsigned i = 0; i < dimensions.size(); i++)
      sample.push_back( data[ dimensions[ i ] ] );

    int attribute_index,
        class_index,
        sample_class = -1,
        level = 0;
    string attribute_name,
           operation,
           value,
           class_name,
           tmp;
    bool next_step,
         found_complement = false;
    while ( sample_class == -1 )
    {
      next_step = false;
      if ( !found_complement )
      {
        for (int i = 0; i < ( level - 1 ); i++)
          c45_file >> tmp; // reading '|'
        c45_file >> attribute_name;
      }
      c45_file >> operation >> value;
      attribute_index = attributes_indexes[ TeConvertToLowerCase( attribute_name ) ];
      if (operation == "<=")
        next_step = sample [ attribute_index ] <= atof( value.c_str() );
      else
        next_step = sample[ attribute_index ] > atof( value.c_str() );
      if ( next_step )
      {
        c45_file >> tmp;
        if (tmp == "->")
        {
          c45_file >> class_name;
          class_index = classes_indexes[ TeConvertToLowerCase( class_name ) ];
          sample_class = class_index;
        }
        else if (tmp == "|")
        {
          found_complement = false;
          level++;
        }
      }
      else
      {
        // consume sub-decisions
        found_complement = false;
        while ( !found_complement )
        {
          c45_file >> tmp;
          found_complement = ( tmp == attribute_name );
          string next_elements;
          if ( found_complement )
          {
            c45_file >> tmp;
            next_elements += tmp;
            c45_file >> tmp;
            next_elements += " " + tmp;
            found_complement = ( tmp == value );
            for ( unsigned i = 0; i < next_elements.length(); i++ )
              c45_file.unget();
          }
        }
      }
    }
    classifyResult.push_back( sample_class );
    ++attr_it;
    c45_file.close();
  }
  #ifdef _DEBUG
  cout << "end classify from file()" << endl;
  #endif
  return true;
}

bool c45_classifier::train( TeClassSampleSet::iterator itBegin, TeClassSampleSet::iterator itEnd, const vector<int> dimensions )
{
  #ifdef _DEBUG
  cout << "train()" << endl;
  #endif

  TeClassSampleSet sampleSet;
  TeClassSampleSet::iterator attr_it = itBegin;
  while (attr_it != itEnd)
  {
    vector<TeClassSample> vector_sample = (*attr_it).second;
    string vector_class = (*attr_it).first;

    vector<TeClassSample> tmp_vector_sample;
    vector<TeClassSample>::iterator vit = vector_sample.begin();
    while ( vit != vector_sample.end() )
    {
      TeClassSample tmp_input;
      for (unsigned i = 0; i < dimensions.size(); i++)
        tmp_input.push_back( (*vit)[ dimensions[i] ] );
      tmp_vector_sample.push_back( tmp_input );
      ++vit;
    }
    sampleSet[ vector_class ] = tmp_vector_sample;
    ++attr_it;
  }
  sampleSet_ = &sampleSet;
  TeClassSampleSet::const_iterator interator = sampleSet_->begin();
  
  for(; interator != sampleSet_->end(); ++interator)
    className_.push_back(interator->first);
  maxClass_ = className_.size();

//  makeFiles();

  getData();
  oneTree();

  #ifdef _DEBUG
  cout << "end train()" << endl;
  #endif
  return true;
}

int c45_classifier::classifySample( Tree &subTree, TeClassSample &sample )
{
  if( subTree.NodeType == 0 ) // Leaf
    return subTree.Leaf;
  else
  {
    if( sample[ subTree.Tested ] <= subTree.Cut )
      c45_classifier::classifySample( *subTree.Branch[ 1 ], sample );
    else
      c45_classifier::classifySample( *subTree.Branch[ 2 ], sample );
  }
}

void c45_classifier::show(Tree* T, int Sh)
{
  int v, MaxV;
  if ( T->NodeType )
  {
    /*  See whether separate subtree needed  */
    if ( T != 0 && Sh && Sh * TabSize + MaxLine(T) > Width )
    {
      if ( subTree_ < 99 )
      {
        subdef_[++subTree_] = T;
        printf("[S%d]", subTree_);
      }
      else
        printf("[S??]");
    }
    else
    {
      MaxV = T->Forks;
      /*  Print simple cases first */
      for(v=1; v<=MaxV; ++v)
      {
        if ( ! T->Branch[v]->NodeType )
          showBranch(Sh, T, v);
      }
      /*  Print subtrees  */
      for(v=1; v<=MaxV; ++v)
      {
        if ( T->Branch[v]->NodeType )
          showBranch(Sh, T, v);
      }
    }
  }
  else
  {
    printf(" %s (%.1f", className_[T->Leaf].c_str(), T->Items);
    if ( T->Errors > 0 ) printf("/%.1f", T->Errors);
      printf(")");
  }
}

int c45_classifier::MaxLine(Tree* St)
{
  int a;
  int v, MaxV, Next;
  int Ll, MaxLl=0;

  a = St->Tested;
  MaxV = St->Forks;
  for(v=1; v<=MaxV; ++v)
  {
    Ll = 5;//( St->NodeType == 2 ? 4 : strlen(attValName_[a][v]) ) + 1;
    /*  Find the appropriate branch  */
    Next = v;
    if ( ! St->Branch[Next]->NodeType )
      Ll += strlen(className_[St->Branch[Next]->Leaf].c_str()) + 6;
    MaxLl = ((MaxLl)>(Ll) ? MaxLl : Ll);
  }
  return strlen(attName_[a].c_str()) + 4 + MaxLl;
}

void c45_classifier::showBranch(int sh, Tree* tree, int v)
{
  printf("\n");
  int ssh = sh;
  while ( ssh-- ) 
    printf("|   ");
  int att = tree->Tested;
  printf("%s %s %g ", attName_[att].c_str(), ( v == 1 ? "<=" : ">" ), tree->Cut);
  if ( tree->Lower != tree->Upper )
    printf("[%g,%g]", tree->Lower, tree->Upper);
  printf(":");
  show(tree->Branch[v], sh+1);
}

void c45_classifier::showTree( Tree* tree, int sh)
{
  int v, maxV;
    
  if ( tree->NodeType )
  {
    /*  See whether separate subtree needed  */
    if ( tree != 0 && sh && sh * TabSize + MaxLine(tree) > Width )
    {
      if ( subTree_ < 99 )
      {
        subdef_[++subTree_] = tree;
        printf("[S%d]", subTree_);
      }
      else
        printf("[S??]");
    }
    else
    {
      maxV = tree->Forks;
      /*  Print simple cases first */
      for(v = 1; v <= maxV; ++v)
      {
        if ( !tree->Branch[v]->NodeType )
          showBranch(sh, tree, v);
      }
      /*  Print subtrees  */
      for(v=1; v<=maxV; ++v)
      {
        if ( tree->Branch[v]->NodeType )
          showBranch(sh, tree, v);
      }
    }
  }
  else
  {
    printf(" %s (%.1f", className_[tree->Leaf].c_str(), tree->Items);
    if ( tree->Errors > 0 ) 
      printf("/%.1f", tree->Errors);
    printf(")");
  }
}

void c45_classifier::makeFiles()
{
// Monta o arquivo names
  ofstream namesFile;
  string namesFileName( "output_c45" ); // rasterIn_->params().fileName_ );
  namesFileName.append( ".names" );
  namesFile.open( namesFileName.c_str() );
  if( !namesFile.is_open() )
  {
    cout << "Não foi possivel criar arquivo .name." << endl;
    //return 1;
  }
  TeClassSampleSet::const_iterator interator;
  interator = sampleSet_->begin();
  namesFile << interator->first.c_str();
  ++interator;
  for(; interator != sampleSet_->end(); ++interator)
  {
    namesFile << ", ";
    namesFile << interator->first.c_str();
  }
  namesFile << endl << endl;
  int i;
  for(i=0; i<=maxAtt_; ++i)
  {
    namesFile << "banda" << i << ": continuous" << endl;
  }
  namesFile.close();

  // Monta o arquivo de treinamento
  ofstream dataFile;
  string dataFileName( "output_c45" ); // rasterIn_->params().fileName_ );
  dataFileName.append( ".data" );
  dataFile.open( dataFileName.c_str() );

  if( !dataFile.is_open() )
  {
    cout << "Não foi possivel criar arquivo .data." << endl;
    //return 1;
  }
  int k;
  interator = sampleSet_->begin();
  for(; interator != sampleSet_->end(); ++interator)
  {
    int total = interator->second.size();
    for(i = 0; i < total; ++i)
    {
      for(k=0; k < maxAtt_-1; ++k)
        dataFile << interator->second[i][k] << ", ";
      dataFile << interator->first << endl;
    }
  }
  dataFile.close();
}

string c45_classifier::var_show(Tree* T, int Sh)
{
  int v, MaxV;
  string ret;
  if ( T->NodeType )
  {
    /*  See whether separate subtree needed  */
    if ( T != 0 && Sh && Sh * TabSize + MaxLine(T) > Width )
    {
      if ( subTree_ < 99 )
      {
        subdef_[++subTree_] = T;
        ret += "[S" + format_number(subTree_) + "]";
      }
      else
        ret += "[S??]";
    }
    else
    {
      MaxV = T->Forks;
      /*  Print simple cases first */
      for(v=1; v<=MaxV; ++v)
      {
        if ( ! T->Branch[v]->NodeType )
          ret += var_showBranch(Sh, T, v);
      }
      /*  Print subtrees  */
      for(v=1; v<=MaxV; ++v)
      {
        if ( T->Branch[v]->NodeType )
          ret += var_showBranch(Sh, T, v);
      }
    }
  }
  else
  {
    ret += " -> " + className_[T->Leaf]; // + "(" + format_number(T->Items);
//     if ( T->Errors > 0 ) 
//       ret += " /" + format_number(T->Errors);
//     ret += ")";
  }

  return ret;
}

string c45_classifier::var_showBranch(int sh, Tree* tree, int v)
{
  string ret = "\n";

  int ssh = sh;
  while ( ssh-- ) 
    ret += "|   ";

  int att = tree->Tested;
  ret += attName_[att] + ( v == 1 ? " <= " : " > " ) + " " + format_number(tree->Cut) + " ";
  if ( tree->Lower != tree->Upper )
    ret += "[" + format_number(tree->Lower) + "," + format_number(tree->Upper) + "]";
//  ret += ":";
  return ret + var_show(tree->Branch[v], sh+1);
}

string c45_classifier::get_tree()
{
  return var_show(tree_, 0);
}

string c45_classifier::format_number(double n)
{
  char char_n[100];
  sprintf(char_n, "%.10f", n);
  return string(char_n);
}

void c45_classifier::print_test( TeClassSampleSet::iterator input, TeClassSampleSet::iterator input_end, vector<int> vetor )
{
  cout << "input.size()" << endl;
}

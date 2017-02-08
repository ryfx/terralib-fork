
/*! \file TePRAbstractClassifier.h
	\brief This file contains an abstract interface of a classifier
	\author Karine Reis Ferreira <karine@dpi.inpe.br>
	\author Emiliano <castejon@dpi.inpe.br>
*/
#ifndef  __PR_INTERNAL_ABSTRACTCLASSIFIER_H
#define  __PR_INTERNAL_ABSTRACTCLASSIFIER_H

#include "TePRDefines.h"

#include <vector>
#include <list>
#include <string>

using namespace std; 

/*! \class TePRAbstractClassifier
    \brief This class represents a abstract interface of a classifier
*/
class PR_DLL TePRAbstractClassifier
{

protected:
	
	std::string				errorMessage_;   //!< Error message 
	TePRClassifierStatus	status_;

	//! Construct
	TePRAbstractClassifier() : errorMessage_(""), status_(TePRStartedClassifier)
	{ }

public:

	//! Destructor
	virtual ~TePRAbstractClassifier()
	{ }


	//! Sets error message
	void setErrorMessage(const std::string& eMess)
	{	errorMessage_ = eMess; }

	//! Gets error message
	std::string& getErrorMessage()
	{	return errorMessage_; }

	
	//! Sets status
	void setStatus(const TePRClassifierStatus& st)
	{	status_ = st; }

	//! Gets status
	TePRClassifierStatus& getStatus()
	{	return status_; }

	//! Trains the classifier based on training pattern set
	/*!
      \param itBegin		iterator that points to the first training pattern
	  \param itEnd		iterator that points to the last training pattern
	  \param dimentions	dimentions of the training patterns which must be considered in the training 
	*/
	
		template<typename trainingPatternSetIterator> 
		bool train(	trainingPatternSetIterator& itBegin, trainingPatternSetIterator& itEnd, 
					const std::vector<int>& dimentions);

	//! Classifies the a pattern set
	/*!
      \param itBegin			iterator that points to the first pattern that will be classified
	  \param itEnd			iterator that points to the last pattern that will be classified
	  \param dimentions		dimentions of the patterns which must be considered in the classification 
	  \param classifyResult	the result class associated to each pattern
	*/
	template<typename patternSetIterator>
		bool classify(	patternSetIterator& itBegin, patternSetIterator& itEnd, 
						const std::vector<int>& dimentions, list<int>& classifyResult);
};


#endif



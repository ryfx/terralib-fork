/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroOverlay.h
*
*******************************************************************************
*
* $Rev: 8367 $:
*
* $Author: eric.abreu $:
*
* $Date: 2010-03-13 15:16:22 +0200 (la, 13 maalis 2010) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Classification Group.
	@defgroup hidroClassification The Group for Plugin Classification Classes.
*/

#ifndef HIDRO_OVERLAY_H_
#define HIDRO_OVERLAY_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <string>

#include <HidroOverlayRule.h>

class TeDatabase;
class TeRaster;
class TeProjection;
class TeCoord2D;

class HidroOverlayClassGroup;


/** 
  * \file HidroOverlay.h
  *
  * \class HidroOverlay
  *
  * \brief This file is a base class to definy the overlay operation
  *
  * Overlay proccess is a complex operation with many components. This components
  * can be defined as a regular grid. Applying logical operations over this grids
  * result in a output regular grid.
  * 
  * Overlay is defined by class group where each class is a definition of several
  * rules that defines how this new class will be generated.
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroClassification
 */

class HidroOverlay
{

public:
		
    /** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Constructor with parameters
	  * \param ruleId		Input parameter used to identify a new rule
	*/
	HidroOverlay(TeDatabase* db);

	/** \brief Virtual destructor.
	*/
	~HidroOverlay();

	//@}

	/** @name Behavioral Methods
		* Methods related to hidro overlay rule behavior.
		*/
	//@{

public:

	/** \brief Function used to set the reference raster used as params to create the output raster
        \param raster			Input parameter, valid pointer to a raster
      */
	void setReferenceRaster(TeRaster* raster);

	/** \brief Function used to get the reference raster pointer
        \return					A valid raster pointer
      */
	TeRaster* getReferenceRaster();

    /** \brief Function used to set a the classes and rules used in overlay operation
        \param classGroup		Input parameter, struct that defines the classes and rules
      */
	void setClassGroup(HidroOverlayClassGroup* classGroup);

	/** \brief Function used to get the classes and rules
        \return					A pointer to struct that has the classes and rules for overlay operation
      */
	HidroOverlayClassGroup* getClassGroup();

	/** \brief Main Function used to run the overlay process
        \return					A valid raster pointer if the proccess run correctly and false in other case
      */
	TeRaster* runOverlayOperation();

	/** \brief Function used to get the internal error message
        \return					A string that has the error message
      */
	std::string getErrorMessage();
		
	//@}


protected:
	int getPixelValue(const TeCoord2D& coord, TeProjection* proj);

	bool executeRule(HidroOverlayRule* rule, const TeCoord2D& coord, TeProjection* proj);

	bool compareValue(const double& valueWanted, const double& valueFound, const HidroOverlayRule::OverlayComparisonOperator& op);

	bool compareValue(const int& valueWanted, const int& valueFound, const HidroOverlayRule::OverlayComparisonOperator& op);

	bool compareValue(const std::string& valueWanted, const std::string& valueFound, const HidroOverlayRule::OverlayComparisonOperator& op);

protected:

	TeDatabase*					_db;					//!< Attribute used to define a valid database pointer

	TeRaster*					_referenceRaster;		//!< Attribute used to define the output raster params

	HidroOverlayClassGroup*		_classGroup;			//!< Attribute used to define the class group used in overlay op

	std::string					_errorMessage;			//!< Attribute used to inform about a error message in overlay proccess
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_OVERLAY_H_
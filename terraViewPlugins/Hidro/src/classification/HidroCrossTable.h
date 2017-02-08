/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroCrossTable.h
*
*******************************************************************************
*
* $Rev: 8052 $:
*
* $Author: eric $:
*
* $Date: 2010-02-08 21:12:37 -0300 (ter, 08 fev 2010) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Classification Group.
	@defgroup hidroClassification The Group for Plugin Classification Classes.
*/

#ifndef HIDRO_CROSSTABLE_H_
#define HIDRO_CROSSTABLE_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <string>
#include <vector>


class TeDatabase;
class TeRaster;

class HidroOverlayClassGroup;

/** 
  * \file HidroCrossTable.h
  *
  * \class HidroCrossTable
  *
  * \brief This file is a base class to definy the overlay cross table operation
  *
  * Cross table operation is an automatic overlay operation over a raster group.
  * A set of rules was generated using the lut information from each raster,
  * and a output raster was generated with the overlay classification.
  * 
  * Only raster with LUT can be used in this functions, and each LUT entry has to
  * be a class defined.
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroClassification
 */

class HidroCrossTable
{

public:
		
    /** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Constructor with parameters
	  * \param db					Input parameter used set current database
	  * \param refRaster			Input parameter used to define the reference raster
	*/
	HidroCrossTable(TeDatabase* db, TeRaster* refRaster);

	/** \brief Virtual destructor.
	*/
	~HidroCrossTable();

	//@}

	/** @name Behavioral Methods
		* Methods related to hidro overlay rule behavior.
		*/
	//@{

public:

	/** \brief Function used execute the overlay cross table using a set of layers
        \param layerVec			Input parameter, vector with layer names to be used in overlay cross table op
		\return					True if the operation was ok and false in other case
      */
	bool executeCrossTable(const std::vector<std::string>& layerVec);

	/** \brief Function used to get the raster result from overlay operation
		\return					A valid raster point if the operation was executed correctly and a NULL pointer in other case
      */
	TeRaster* getOutputRaster();

	/** \brief Function used to get the classes and rules
        \return					A pointer to struct that has the classes and rules for overlay operation
      */
	HidroOverlayClassGroup* getClassGroup();

protected:

	bool checkInputLayer();

	bool generateRules();

	//@}

public:

	TeDatabase*					_db;				//!< Attribute used to define a valid database pointer

	TeRaster*					_referenceRaster;	//!< Attribute used to define the output raster params

	std::vector<std::string>	_layerVec;			//!< Attribute used to define the input layers used in operation

	TeRaster*					_outputRaster;		//!< Attribute used to return the result from overlay operation

	HidroOverlayClassGroup*		_classGroup;			//!< Attribute used to define the class group used in overlay op
};


/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_CROSSTABLE_H_
/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroOverlayClassGroup.h
*
*******************************************************************************
*
* $Rev: 8053 $:
*
* $Author: eric $:
*
* $Date: 2009-10-16 03:29:19 +0300 (pe, 16 loka 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Classification Group.
	@defgroup hidroClassification The Group for Plugin Classification Classes.
*/

#ifndef HIDRO_OVERLAYCLASSGROUP_H_
#define HIDRO_OVERLAYCLASSGROUP_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <string>
#include <map>

#include <TeVisual.h>

class HidroOverlayClass;

/** 
  * \file HidroOverlayClassGroup.h
  *
  * \class HidroOverlayClassGroup
  *
  * \brief This file is a base class to definy a class group for overlay operation
  *
  * Overlay proccess is a complex operation with many components. This components
  * can be defined as a regular grid. Applying logical operations over this grids
  * result in a output regular grid.
  * 
  * Class group is a definition for a set of classes that defines the ouput grid.
  * Any of each class is a result of operation over selecteds grids. A class group
  * is defined by:
  * 	- a set of classes
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroClassification
 */

class HidroOverlayClassGroup
{

public:
		
    /** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor
	*/
	HidroOverlayClassGroup();

	/** \brief Virtual destructor.
	*/
	~HidroOverlayClassGroup();

	//@}

	/** @name Behavioral Methods
		* Methods related to hidro overlay rule behavior.
		*/
	//@{

public:

	/** \brief Function used to get the class set
        \return Return the map with the class set information
      */
	std::map<std::string, HidroOverlayClass*>& getClasses();

	/** \brief Function used to add a new class into the map
		\param class		Input parameter, valid pointer to a class
        \return Return true if the class was added correctly and false in other case
      */
	bool addNewClass(HidroOverlayClass* overlayClass);

	/** \brief Function used to remove a class from the map
        \param className		Input parameter, identification from class to be deleted
      */
	void removeClass(const std::string& className);

	HidroOverlayClass* getClass(const std::string& className);

	std::vector<TeColor> getClassGroupLut();

	int getNewIndex();

	//@}

protected:

	std::map<std::string, HidroOverlayClass*>	_classMap;	//!< Attribute used to save class information (key is the class name)

	int _count;
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_OVERLAYCLASSGROUP_H_
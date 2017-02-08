/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroGroupLUTController.h
*
*******************************************************************************
*
* $Rev: 7994 $:
*
* $Author: eric $:
*
* $Date: 2009-08-31 16:03:40 +0300 (ma, 31 elo 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Functions Group.
	@defgroup hidroFunctions The Group for Plugin Functions Classes.
*/

#ifndef HIDRO_GROUPLUT_CONTROLLER_H
#define HIDRO_GROUPLUT_CONTROLLER_H

/*
** ----------------------------------------------------------------------------
** Includes:
*/

#include <TeLayer.h>

#include <set>


struct HidroGroupLutInfo
{
	std::string _index;
	std::string _className;
	std::string _r;
	std::string _g;
	std::string _b;

	bool _hasGrouping;

	std::string _groupingName;
};

struct HidroGroupInfo
{
	std::string _index;
	std::string _className;
	std::string _r;
	std::string _g;
	std::string _b;
};

/** 
  * \file HidroGroupLUTController.h
  *
  * \class HidroGroupLUTController
  *
  * \brief This file is class to give support for the management
  * of a lut from a raster pallete to creating a grouping of classes
  * into a new raster pallete.
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroFunctions
 */

class HidroGroupLUTController
{

public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor with parameters
	  * \param database		Input parameter - current database pointer from app
	*/
	HidroGroupLUTController(TeLayer* layer);

	/** \brief Virtual destructor.
	*/
	~HidroGroupLUTController();

	//@}

	/** @name Behavioral Methods
		* Methods related to lut controller.
		*/
	//@{

public:
	std::map<std::string, HidroGroupLutInfo>& getLutInfo();

	bool lutEntryHasGroup(const std::string& lutIdx, std::string& groupName);

	void setLutEntryGroup(const std::string& lutIdx, const std::string& groupName);

	void removeLutEntryGroup(const std::string& lutIdx);

	bool addGroup(const std::string& groupName, const int& r, const int& g, const int& b);

	bool changeGroupColor(const std::string& groupName, const int& r, const int& g, const int& b);

	void removeGroup(const std::string& groupName);

	int getNumGroups();

	bool hasGrouping();

	bool generatingGrouping(const std::string& layerName);

protected:

	void updateLutInfo();


	//@}

protected:

	TeLayer*									_layer;		//!< Attribute used to access a layer pointer

	std::map<std::string, HidroGroupLutInfo>	_lutInfo;

	std::vector<HidroGroupInfo>					_groupInfo;

};

/*
** ---------------------------------------------------------------
** End:
*/


#endif //HIDRO_GROUPLUT_CONTROLLER_H
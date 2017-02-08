/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroOverlayClass.h
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

#ifndef HIDRO_OVERLAYCLASS_H_
#define HIDRO_OVERLAYCLASS_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <string>

#include <TeVisual.h>

class HidroOverlayRuleGroup;

/** 
  * \file HidroOverlayClass.h
  *
  * \class HidroOverlayClass
  *
  * \brief This file is a base class to definy a class for overlay operation
  *
  * Overlay proccess is a complex operation with many components. This components
  * can be defined as a regular grid. Applying logical operations over this grids
  * result in a output regular grid.
  * 
  * Class is a definition to a new component over the output grid generated from
  * the overlay proccess. A class is defined by:
  *		- a class name
  *		- a class color
  *		- a set of rule groups
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroClassification
 */

class HidroOverlayClass
{

public:
		
    /** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Constructor with parameters
	  * \param idx				Input parameter used to define the new class index (lut index)
	  * \param className		Input parameter used to identify a new class
	*/
	HidroOverlayClass(const int& idx , const std::string& className);

	/** \brief Virtual destructor.
	*/
	~HidroOverlayClass();

	//@}

	/** @name Behavioral Methods
		* Methods related to hidro overlay rule behavior.
		*/
	//@{

public:

	/** \brief Function used to set the rule identification
        \param id	Input parameter, value used to set the rule identification
      */
	void setClassName(const std::string& className);

	/** \brief Function used to get the rule identification
        \return String values with the rule identification
      */
	std::string getClassName();

	int getClassIdx();

    /** \brief Function used to set the rule identification
        \param id				Input parameter, value used to set the rule identification
      */
	void setClassColor(const TeColor& color);

	/** \brief Function used to get the rule identification
        \return String values with the rule identification
      */
	TeColor getClassColor();

	/** \brief Function used to inform if the rule group is valid
        \return Return true if the rule group is valid
      */
	bool ruleGroupIsValid();

	/** \brief Function used to get the rule group set
        \return Return the map with the rule group set information
      */
	std::map<std::string, HidroOverlayRuleGroup*>& getRuleGroups();

	/** \brief Function used to add a new rule group into the map
		\param ruleGroup		Input parameter, valid pointer to a ruleGroup
        \return Return true if the ruleGroup was added correctly and false in other case
      */
	bool addNewRuleGroup(HidroOverlayRuleGroup* ruleGroup);

	/** \brief Function used to remove a ruleGroup from the map
        \param ruleGroupId		Input parameter, identification from rule group to be deleted
      */
	void removeRuleGroup(const std::string& ruleGroupId);

	std::string getNewGroupName();

	HidroOverlayRuleGroup* getRuleGroup(const std::string& ruleGroupId);

	//@}

protected:

	int												_idx;			//!< Attribute used to define the class index (lut index)

	std::string										_className;		//!< Attribute used to define the class identification

	TeColor											_color;			//!< Attribute used to define the class color

	bool											_isValid;		//!< Attribute used to define if the class rule group is valid

	std::map<std::string, HidroOverlayRuleGroup*>	_ruleGroupMap;	//!< Attribute used to save the rule group information (key is the rule group id)

	int												_count;
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_OVERLAYCLASS_H_
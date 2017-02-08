/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroOverlayRuleGroup.h
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

#ifndef HIDRO_OVERLAYRULEGROUP_H_
#define HIDRO_OVERLAYRULEGROUP_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <string>
#include <map>

class HidroOverlayRule;

/** 
  * \file HidroOverlayRuleGroup.h
  *
  * \class HidroOverlayRuleGroup
  *
  * \brief This file is a base class to definy a rule group for overlay operation
  *
  * Overlay proccess is a complex operation with many components. This components
  * can be defined as a regular grid. Applying logical operations over this grids
  * result in a output regular grid.
  * 
  * Group Rule is a definition for the logical operation OR, its possible define a set of group rules
  * to define a condition in overlay operation. One group rule is represented by:
  *		- a set of rules
  *		- a valid condition
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroClassification
 */

class HidroOverlayRuleGroup
{

public:
		
    /** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Constructor with parameters
	  * \param groupId		Input parameter used to identify a new rule group
	*/
	HidroOverlayRuleGroup(const std::string& groupId);

	/** \brief Virtual destructor.
	*/
	~HidroOverlayRuleGroup();

	//@}

	/** @name Behavioral Methods
		* Methods related to hidro overlay rule group behavior.
		*/
	//@{

public:

	/** \brief Function used to set the rule group identification
        \param id	Input parameter, value used to set the rule group identification
      */
	void setGroupRuleId(const std::string& groupId);

	/** \brief Function used to get the rule group identification
        \return String values with the rule group identification
      */
	std::string getRuleGroupId();

	/** \brief Function used to inform if the rule is valid
        \return Return true if the rule is valid
      */
	bool ruleIsValid();

	/** \brief Function used to get the rule group
        \return Return the map with the rule group information
      */
	std::map<std::string, HidroOverlayRule*>& getRuleGroup();

	/** \brief Function used to add a new rule into the group
		\param rule		Input parameter, valid pointer to a rule
        \return Return true if the rule was added correctly and false in other case
      */
	bool addNewRule(HidroOverlayRule* rule);

	/** \brief Function used to remove a rule from the map
        \param ruleId	Input parameter, identification from rule to be deleted
      */
	void removeRule(const std::string& ruleId);

	HidroOverlayRule* getRule(const std::string& ruleId);

	std::string getNewRuleName();
    		
	//@}

protected:

	std::string									_groupId;		//!< Attribute used to define the rule group identification

	bool										_isValid;		//!< Attribute used to define if the rule group is valid

	std::map<std::string, HidroOverlayRule*>	_ruleMap;		//!< Attribute used to save the rule information (key is the rule id)

	int												_count;
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_OVERLAYRULEGROUP_H_
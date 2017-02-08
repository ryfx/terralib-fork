/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroOverlayRule.h
*
*******************************************************************************
*
* $Rev: 8052 $:
*
* $Author: eric $:
*
* $Date: 2009-10-16 03:12:37 +0300 (pe, 16 loka 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Classification Group.
	@defgroup hidroClassification The Group for Plugin Classification Classes.
*/

#ifndef HIDRO_OVERLAYRULE_H_
#define HIDRO_OVERLAYRULE_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <string>

class TeLayer;

class HidroAttr;

/** 
  * \file HidroOverlayRule.h
  *
  * \class HidroOverlayRule
  *
  * \brief This file is a base class to definy a rule for overlay operation
  *
  * Overlay proccess is a complex operation with many components. This components
  * can be defined as a regular grid. Applying logical operations over this grids
  * result in a output regular grid.
  * 
  * Rule is a definition for the logical operation AND, its possible define a set of rules
  * to define a condition in overlay operation. One rule is represented by:
  *		- one regular grid
  *		- one comparison operator
  *		- a attribute value (hidro attribute)
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroClassification
 */

class HidroOverlayRule
{
public:
	enum OverlayComparisonOperator
	{
		None,
		Equal,
		NotEqual,
		LessThan,
		GreaterThan,
		LessThanOrEqualTo,
		GreaterThanOrEqualTo
	};


public:	
    /** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Constructor with parameters
	  * \param ruleId		Input parameter used to identify a new rule
	*/
	HidroOverlayRule(const std::string& ruleId);

	/** \brief Virtual destructor.
	*/
	~HidroOverlayRule();

	//@}

	/** @name Behavioral Methods
		* Methods related to hidro overlay rule behavior.
		*/
	//@{

public:

	/** \brief Function used to set the rule identification
        \param id	Input parameter, value used to set the rule identification
      */
	void setRuleId(const std::string& ruleId);

	/** \brief Function used to get the rule identification
        \return String values with the rule identification
      */
	std::string getRuleId();

    /** \brief Function used to set a pointer to a layer that has a regular grid
        \param layer		Input parameter used to identify a regular grid in a rule
      */
	void setLayer(TeLayer* layer);

	/** \brief Function used to regular grid component from a rule
        \return A pointer to a regular grid layer
      */
	TeLayer* getLayer();

	/** \brief Function used to set te comparison operator in a rule
        \param op		Input parameter that defines a comparison operator
      */
	void setComparisonOperator(const OverlayComparisonOperator& op);

	/** \brief Function used to get the comparison operator for this rule
        \return A enum value that defines a comparison for this rule
      */
	HidroOverlayRule::OverlayComparisonOperator getComparisonOperator();

	/** \brief Function used to set an attribute to this rule
        \param attr		Input parameter, hidro attribute value
      */
	void setRuleAttr(HidroAttr* attr);

	/** \brief Function used to get the rule attribute
		\return			Return a valid pointer to this rule attribute
      */
	HidroAttr* getRuleAttr();

	OverlayComparisonOperator getRuleComparisonOp(const std::string& value);

	std::string getRuleStrComparisonOp(const OverlayComparisonOperator& op);
		
	//@}

protected:

	std::string					_ruleId;		//!< Attribute used to define the rule identification

	TeLayer*					_layer;			//!< Attribute used to define the layer component in this rule

	OverlayComparisonOperator	_op;			//!< Attribute used to define the comparison operator in this rule

	HidroAttr*					_attr;			//!< Attribute used to define the attribute in this rule
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_OVERLAYRULE_H_
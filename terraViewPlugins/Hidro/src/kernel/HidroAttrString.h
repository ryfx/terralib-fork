/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroAttrString.h
*
*******************************************************************************
*
* $Rev: 7694 $:
*
* $Author: eric $:
*
* $Date: 2009-03-23 14:37:58 +0200 (ma, 23 maalis 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Kernel Group.
	@defgroup hidroKernel The Group for Plugin Base Classes.
*/

#ifndef HIDRO_ATTR_STRING_H_
#define HIDRO_ATTR_STRING_H_

/** 
  * \file HidroAttrString.h
  *
  * \class HidroAttrString
  *
  * \brief This file is a extend class to define a graph attribute type string
  *
  * To do hydrological modeling is necessary associate attributes to
  * graph components. This attributes can be a integer, double or
  * string attribute. A vertex or edge can has a vector of attributes.
  * The attribute defined in this class is the STRING Type;
  *
  * \sa HidroAttr.h
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroKernel
 */

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <HidroAttr.h>

class HidroAttrString : public HidroAttr
{

public:

    /** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor.
	*/
	HidroAttrString();

	/** \brief Constructor with parameters.
	  * \param attrName		Input parameter, used to set the attribute name
	  * \param value		Input parameter, used to set the attribute value
	*/
	HidroAttrString(const std::string& attrName, const std::string& value);
	
	/** \brief Virtual destructor.
	*/
	~HidroAttrString();

	//@}

public:

	/** @name Behavioral Methods
		* Methods related to hidro attr behavior.
		*/
	//@{

    /** \brief Function used to get the attribute value
      *
      * \return String value that defined the attribute value
      */
	std::string getAttrValue() const;

	/** \brief Function used to set the attribute value
      *
      * \param attrValue	Input parameter, string value to set the attribute value
      */
	void setAttrValue(const std::string& attrValue);

	//@}

protected:

	std::string	_attrValue;		//!< Attribute used to define the attribute string value

};

/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_ATTR_STRING_H_
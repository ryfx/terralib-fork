/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroAttr.h
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

#ifndef HIDRO_ATTR_H_
#define HIDRO_ATTR_H_

/** 
  * \file HidroAttr.h
  *
  * \class HidroAttr
  *
  * \brief This file is a base class to define a graph attribute
  *
  * To do hydrological modeling is necessary associate attributes to
  * graph components. This attributes can be a integer, double or
  * string attribute. A vertex or edge can has a vector of attributes.
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
#include <TeDataTypes.h>

class HidroAttr
{
public:

    /** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor.
	*/
	HidroAttr();
	
	/** \brief Virtual destructor.
	*/
	~HidroAttr();

	//@}

public:

	/** @name Behavioral Methods
		* Methods related to hidro attr behavior.
		*/
	//@{

    /** \brief Function used to get the attribute data type { TeSTRING, TeREAL, TeINT }
      *
      * \return The attribute data type, native from TerraLib.
      */
	TeAttrDataType getAttrType() const;

	/** \brief Function used to get the attribute name
      *
      * \return String that defined the attribute name
      */
	std::string getAttrName() const;

	/** \brief Function used to set the attribute name
      *
      * \param attrName	Input parameter, string value to set the attribute name
      */
	void setAttrName(const std::string& attrName);

	//@}

protected:

	std::string _attrName;		//!< Attribute used to define the attribute name

	TeAttrDataType _attrType;	//!< Attribute used to set the attribute data type
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_ATTR_H_
/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroAttrDouble.h
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

#ifndef HIDRO_ATTR_DOUBLE_H_
#define HIDRO_ATTR_DOUBLE_H_

/** 
  * \file HidroAttrDouble.h
  *
  * \class HidroAttrDouble
  *
  * \brief This file is a extend class to define a graph attribute type double
  *
  * To do hydrological modeling is necessary associate attributes to
  * graph components. This attributes can be a integer, double or
  * string attribute. A vertex or edge can has a vector of attributes.
  * The attribute defined in this class is the DOUBLE Type;
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

class HidroAttrDouble : public HidroAttr
{

public:

    /** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor.
	*/
	HidroAttrDouble();

	/** \brief Constructor with parameters.
	  * \param attrName		Input parameter, used to set the attribute name
	  * \param value		Input parameter, used to set the attribute value
	*/
	HidroAttrDouble(const std::string& attrName, const double& value);
	
	/** \brief Virtual destructor.
	*/
	~HidroAttrDouble();

	//@}

public:

	/** @name Behavioral Methods
		* Methods related to hidro attr behavior.
		*/
	//@{

    /** \brief Function used to get the attribute value
      *
      * \return Double value that defined the attribute value
      */
	double getAttrValue() const;

	/** \brief Function used to set the attribute value
      *
      * \param attrValue	Input parameter, double value to set the attribute value
      */
	void setAttrValue(const double& attrValue);

	//@}

protected:

	double	_attrValue;		//!< Attribute used to define the attribute double value

};

/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_ATTR_DOUBLE_H_
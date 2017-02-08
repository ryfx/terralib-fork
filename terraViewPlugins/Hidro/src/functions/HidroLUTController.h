/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroLUTController.h
*
*******************************************************************************
*
* $Rev: 8049 $:
*
* $Author: eric $:
*
* $Date: 2009-10-15 16:23:53 +0300 (to, 15 loka 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Functions Group.
	@defgroup hidroFunctions The Group for Plugin Functions Classes.
*/

#ifndef HIDRO_LUT_CONTROLLER_H
#define HIDRO_LUT_CONTROLLER_H

/*
** ----------------------------------------------------------------------------
** Includes:
*/

#include <TeLayer.h>

class TeTheme;
class TeQtThemeItem;

struct HidroLutInfo
{
	std::string		_index;			//!< attribute used to identify the class
	std::string		_className;		//!< attribute used to describe the class
	std::string		_r;				//!< r component from color of class
	std::string		_g;				//!< g component from color of class
	std::string		_b;				//!< b component from color of class
};

/** 
  * \file HidroLUTController.h
  *
  * \class HidroLUTController
  *
  * \brief This file is class to give support for the management
  * of a lut from a raster pallete, used for insertion values in lut table,
  * or just research informations.
  *
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroFunctions
 */

class HidroLUTController
{

public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor with parameters
	  * \param layer		Input parameter - layer pointer with raster lut representation
	*/
	HidroLUTController(TeLayer* layer);

	/** \brief Virtual destructor.
	*/
	~HidroLUTController();

	//@}

	/** @name Behavioral Methods
		* Methods related to lut controller.
		*/
	//@{
	/** \brief Function used to get list of classes from a lut table
	  * \return Vector with the lut class
      */
	std::vector<std::string> listLUTClasses() const;

	/** \brief Function used to get a color from a class
	  * \param className	Attribute used to identify a class
	  * \param r			Attribute r color component
	  * \param g			Attribute g color component
	  * \param b			Attribute b color component
      */
	void getClassColor(const std::string& className, int& r, int& g, int& b);

	/** \brief Function used to get a index for a class
	  * \param className	Attribute used to identify a class
	  * \return				Integer with the class index
      */
	int getClassIndex(const std::string& className);

	/** \brief Function used to get name for a class
	  * \param index	Integer with the class index
	  * \return			Attribute used to identify a class 
      */
	std::string getClassName(const int& index);

	/** \brief Function used to get a index for a color
	  * \param r	Attribute r color component
	  * \param g	Attribute g color component
	  * \param b	Attribute b color component
	  * \return		Integer with the class index
      */
	int getClassIndex(const int& r, const int& g, const int& b);

	/** \brief Function used to add a new class in lut table
	  * \param className	Attribute used to identify a class
	  * \param r			Attribute r color component
	  * \param g			Attribute g color component
	  * \param b			Attribute b color component
	  * \param setAsDummy	Attribute used to inform if this new class is dummy class
	  * \return				True if the class was inserted correctly and false in other case
      */
	bool addNewClass(const std::string& className, const int& r, const int& g, const int& b, const bool& setAsDummy);

	/** \brief Function used to
	  * \param index		Integer with the class index
	  * \param className	Attribute used to identify a class
	  * \param r			Attribute r color component
	  * \param g			Attribute g color component
	  * \param b			Attribute b color component
	  * \param setAsDummy	Attribute used to inform if this new class is dummy class
	  * \return				True if the class was changed correctly and false in other case
      */
	bool changeClass(const int& index, const std::string& className, const int& r, const int& g, const int& b, const bool& setAsDummy);

	/** \brief Function used to get the lut class information from a lut table
	  * \param classVec		Vector with struct to describe a lut table
      */
	void getClassList(std::vector<HidroLutInfo>& classVec);

	/** \brief Function used to update a theme
	  * \param theme		Pointer to a theme to be recreated new legend
	  * \return				True it the theme was updated correctly and false in other case
      */
	bool updateThemeLegend(TeTheme* theme, TeQtThemeItem* themeItem);

	/** \brief Function used to create the attr lut table in database
	  * \return	True if the table was build correctly and false in other case
      */
	bool createAttrLUTTable();

	bool hasAttrLUTTable();

	/** \brief Function used to export the lut information for a file disk
	  * \param fileName		Attribute used to indicate the file name
      */
	void exportLutInfoToFile(const std::string& fileName);

	/** \brief Function used to get information about message error
	  * \return		String with the error message.
      */
	std::string	getErrorMessage() const;

	//@}

protected:

	TeLayer*	_layer;			//!< Attribute used to access a layer pointer
	std::string	_errorMessage;	//!< Attribute used to inform about a error in some operation

};

/*
** ---------------------------------------------------------------
** End:
*/


#endif //HIDRO_LUT_CONTROLLER_H
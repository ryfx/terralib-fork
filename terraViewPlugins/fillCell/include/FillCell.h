/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright  2001-2004 INPE and Tecgraf/PUC-Rio.
This file is part of TerraView. TerraView is free software; you can redistribute it 
and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

You should have received a copy of the GNU General Public License
along with TerraView.
The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The software provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use of
this program and its documentation.
*************************************************************************************/

#ifndef FILLCELL_H
#define FILLCELL_H

#include <string>

#include <TeDataTypes.h>

#include <FillCellOp.h>
#include <TeTimeInterval.h>

class TeDatabase;
class TeTheme;

/*
	Input Theme
			- Polygons
				- Table
					- Attribute
						- Ignore Value
			- Lines
				- Table
					- Attribute
						- Ignore Value
			- Points
				- Table
					- Attribute
						- Ignore Value
			- Cell
				- Table
					- Attribute
						- Ignore Value
				
			- Raster			
				- Dummy

	Layer Cell
			- Create Temporal Table
				- New Attribute
					- New Time

			- Select Table
				- Temporal Table
					- Create Attr
						- New Time

					- Select Attr
						- New Time
						- Select Time

				- Non Temporal Table
					- Create Attr
*/

/**
* @brief A class for fill cell execution.
* @author Eric Silva Abreu <eric.abreu@funcate.org.br>
*/   
class FillCell
{

public:

	/**
	* Default constructor.
	*/    
	FillCell(TeDatabase* db);

	/**
	* Default destructor.
	*/  
	~FillCell();

public:

	/**
	* Function used to run the fill cell operation
	* All parameters must be defined before call this function
	*
	* @return	True if the operation was executed correctly and false in other case,
	*			in this case check the error message
	*/    
	bool executeOperation();

	/**
	* Function used to get the error message
	*
	* @return	String with the error message
	*/
	std::string getErrorMessage();

protected:

	/**
	* Function used to check and validate all parameters
	*
	* @return	True if all parameters is ok and false in other case
	*/
	bool checkParameters();

	/**
	* Function used to execute the Minimum Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeMinimumStrategy();

	/**
	* Function used to execute the Maximum Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeMaximumStrategy();

	/**
	* Function used to execute the Average Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeAverageStrategy();

	/**
	* Function used to execute the Sum Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeSumStrategy();

	/**
	* Function used to execute the Standard Deviation Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeSTDevStrategy();

	/**
	* Function used to execute the Majority Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeMajorityStrategy();

	/**
	* Function used to execute the Category Percentage Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeCategoryPercentageStrategy();

	/**
	* Function used to execute the Average weight by Area Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeAverageWeighByAreaStrategy();

	/**
	* Function used to execute the Sum weight by Area Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeSumWeighByAreaStrategy();

	/**
	* Function used to execute the Category Majority Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeCategoryMajorityStrategy();

	/**
	* Function used to execute the Category Area Percentage Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeCategoryAreaPercentageStrategy();

	/**
	* Function used to execute the Minimum Distance Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeMinimumDistanceStrategy();

	/**
	* Function used to execute the Presence Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executePresenceStrategy();

	/**
	* Function used to execute the Count Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeCountObjectsStrategy();

	/**
	* Function used to execute the Total Area Percentage Strategy
	*
	* @return	True if the execution is ok and false in other case
	*/
	bool executeTotalAreaPercentageStrategy();


public:
	/**
	* String used to define the input theme name
	*/ 
	std::string _inputThemeName;

	/**
	* String used to define the table name from input theme
	*/
	std::string _dataTableName;

	/**
	* String used to define the attr table name from input theme
	*/
	std::string _dataAttrTableName;
	
	/**
	* String used to define the layer cell name
	*/ 
	std::string _layerCellName;

	/**
	* String used to define the table name from layer cell
	*/
	std::string _cellTableName;

	/**
	* String used to define the attr table name from layer cell
	*/
	std::string _cellAttrTableName;

	/**
	* Boolen used to define if a default value was defined
	*/
	bool _usecellDefaultValue;

	/**
	* String that defines the default value
	*/
	std::string _cellDefaultValue;

	/**
	* Boolen used to define if a default value was defined to be ignored, used in raster case
	*/
	bool _useDummy;

	/**
	* Attribute that defines the dummy value
	*/
	double _dummy;

	/**
	* Boolen used to define if a default value was defined to be ignored, used in vectorial case
	*/
	bool _ignoreValue;

	/**
	* String that defines the ignored value
	*/
	std::string _valueIgnored;
	
	/**
	* Geometry representation from input theme
	*/
	TeGeomRep _rep;

	/**
	* Fill Cell strategy
	*/
	CellOpStrategy _strategy;

	/**
	* Define if the selected table from layer cell is a temporal table
	*/
	bool _useTemporalTable;

	/**
	* If a temporal table was selected, a valid time interval must be defined
	*/
	TeTimeInterval _timeInterval;


	/**
	* String used to define the error message
	*/ 
	std::string _errorMessage;

	/**
	* Pointer to a current database
	*/ 
	TeDatabase* _db;

	/**
	* Pointer to a selected input theme
	*/ 
	TeTheme* _inputTheme;
	
	/**
	* Pointer to a selected layer cell
	*/
	TeLayer* _cellLayer;
};


#endif //FILLCELL_H
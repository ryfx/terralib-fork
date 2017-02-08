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

#ifndef FILLCELLDLG_H
#define FILLCELLDLG_H
  
#include "FillCellUI.h"
#include <FillCellUtils.h>
#include <FillCellOp.h>
  
#include <PluginParameters.h>

class Help;

using namespace std;

/**
* @brief A class for fill cell plugin form.
* @author Eric Silva Abreu <eric.abreu@funcate.org.br>
*/   
class FillCellDlg : public FillCellUI
{
	Q_OBJECT

public :

	/**
	* Default constructor.
	*/      
	FillCellDlg( QWidget* parent );

	/**
	* Default destructor.
	*/       
	~FillCellDlg();

	/**
	* Update the form input fields with new plugin parameters.
	*
	* @param pp Plugin parameters.
	*/       
	void UpdateForm( const PluginParameters& pp );

protected:

	/**
	* Turns the form to initial state
	*/ 
	void resetForm();

	/**
	* Fill all fields getting information from current database
	*/ 
	void fillForm();

	/**
	* Fill the layer cell list getting information from current database
	*/ 
	bool fillLayersCell();

	/**
	* Fill all static fields (time fields)
	*/ 
	void fillDateTimeFields();

	/**
	* Fill attribute table combo with all attributes for a selected table
	*/
	void fillAttrTable();

	/**
	* Fill temporal combo with all times for a selected temporal table
	*/
	void fillTemporalAttributes();

	/**
	* Calls a external class to execute the fill cell operation
	*/
	void execute(const bool& isTemporal);

protected :

	/**
	* The current plugin parameters.
	*/        
	PluginParameters plugin_params_;

	/**
	* Pointer to a class to do the extra job (terralib job).
	*/ 
	FillCellUtils* _fillCellUtils;

	/**
	* Pointer to a class that manager all cell operations.
	*/ 
	FillCellOp* _fillCellOp;

	/**
	* Attribute used to define if a default value was defined.
	*/ 
	bool _useDefaultValue;

	/**
	* Attribute used to show help from this interface
	*/ 
	Help* _help;
      
protected slots :

	/**
	* Select input theme (list of all themes in database)
	*/ 
	virtual void themeComboBox_activated(const QString& themeName);

	/**
	* Select geometry representation from selected theme
	*/ 
	virtual void geomRepButtonGroup_clicked(int value);

	/**
	* Select a fill cell operation, 
	* this operations can changed defined by geometry representation selected
	*/ 
	virtual void opComboBox_activated(const QString& operation);

	/**
	* Select table from selected theme
	*/ 
	virtual void tableVecComboBox_activated(const QString& tableName);

	/**
	* Check used if has to ignore a value from selected layer (used in raster case)
	*/ 
	virtual void ignoreCheckBox_clicked();

	/**
	* Changes to next page
	*/ 
	virtual void nextOnePushButton_clicked();

	/**
	* Select a cell layer (list of all cell layers from database)
	*/ 
	virtual void cellLayerComboBox_activated(const QString& cellLayerName);

	/**
	* Check to indicate that a table will be selected from a cell layer
	*/
	virtual void selectTableRadioButton_clicked();

	/**
	* Select a table from selected cell layer
	*/
	virtual void selectTableComboBox_activated(const QString& tableName);

	/**
	* Check to indicate that a table will be created in a cell layer
	*/
	virtual void createTableRadioButton_clicked();

	/**
	* Check to indicate that a attribute table will be created in a cell layer table
	*/
	virtual void createAttrRadioButton_clicked();

	/**
	* Check to indicate that a attribute table will be selected from a cell layer table
	*/
	virtual void useAttrRadioButton_clicked();

	/**
	* Changes to previous page
	*/ 
	virtual void previousTwoPushButton_clicked();

	/**
	* Changes to next page
	*/ 
	virtual void nextTwoPushButton_clicked();

	/**
	* Check to indicate that a new time will be created in a temporal cell layer table
	*/
	virtual void createTimeRadioButton_clicked();

	/**
	* Check to indicate that an exist time will be selected from a temporal cell layer table
	*/
	virtual void selectTimeRadioButton_clicked();

	/**
	* Defines if the hour will be defined in temporal table
	*/ 
	virtual void setHourCheckBox_clicked();

	/**
	* Changes to previous page
	*/ 
	virtual void previousThreePushButton_clicked();

	/**
	* Execute the fill cell operation in case a non temporal table selected
	*/ 
	virtual void okPushButton_clicked();

	/**
	* Execute the fill cell operation in case a temporal table selected
	*/ 
	virtual void ok2PushButton_clicked();

	/**
	* Call help interface
	*/ 
	virtual void helpPushButton_clicked();
};

#endif

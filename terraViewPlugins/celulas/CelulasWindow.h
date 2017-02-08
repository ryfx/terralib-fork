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

#ifndef PLUGINMAINWINDOW_HPP
#define PLUGINMAINWINDOW_HPP
  
#include "celulasform.h"
  
#include <PluginParameters.h>

using namespace std;

/**
* @brief A class for visual celulas plugin form.
* @author Priscila Gregati Ferrari
*/   
class CelulasWindow : public CelulasForm
{
	Q_OBJECT

	public :

		/**
		* Default constructor.
		*/      
		CelulasWindow( QWidget* parent );

		/**
		* Default destructor.
		*/       
		~CelulasWindow();
      
		/**
		* Update the form input fields with new plugin parameters.
		*
		* @param pp Plugin parameters.
		*/       
		void UpdateForm( const PluginParameters& pp );

    protected :

		/**
		* The current plugin parameters.
		*/        
		PluginParameters plugin_params_;
      
	protected slots :

		void okPushButton_clicked();
		void cancelButton_clicked();
		void layerComboBox_activated( const QString & layerName );
		void tableComboBox_activated( const QString & tableName );
		void themaDataComboBox_activated( const QString & themaName );
		void tableDataComboBox_activated();
		void NovaTabelaradioButton_clicked();
		void SelecionarTabelaradioButton_clicked();
		void NovaAtributoradioButton_clicked();
		void SelecionarAtributoradioButton_clicked();
		void GeomComboBox_activated();
		void NovoTemporadioButton_clicked();
		void SelecionarTemporadioButton_clicked();
		void helpPushButton_clicked();
		void aboutPushButton_clicked();
		void OperationComboBox_activated();
		void DummyCheckBox_clicked();
      
};

#endif

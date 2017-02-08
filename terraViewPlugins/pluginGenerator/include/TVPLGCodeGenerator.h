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

#ifndef __TVIEW_PLUGINCODEGENERATOR_H_
#define __TVIEW_PLUGINCODEGENERATOR_H_

#include <string>
#include <vector>

//Qt include files
#include <qfile.h>

using namespace std;

//Forward declarations
class QDir;

class TVPLGCodeGenerator
{
public:
	TVPLGCodeGenerator();
	
	~TVPLGCodeGenerator();

	void generatePlgCode(const string& name, const string& className, const string& description, const string& location, const string& vendorName, 
			const string& vendorMail, const string& vendorHP);

protected:

	void generateFolders(QDir& projectLocation);

	void generatePro(QDir& projectLocation, const string& name, const vector<string>& headers, const vector<string>& srcs);

	void generatePluginHeader(QDir projectLocation, const string& name, const string& className, vector<string>& headerNames);

	void generatePluginCodeHeader(QDir projectLocation, const string& name, const string& description, const string& vendorName, 
			const string& vendorMail, const string& vendorHP, vector<string>& headerNames);

	void generatePluginSource(QDir projectLocation, const string& name, const string& className, vector<string>& srcNames);

	void generatePluginCodeSrc(QDir projectLocation, const string& name, const string& className, vector<string>& srcNames);
};

#endif //__TVIEW_PLUGINCODEGENERATOR_H_

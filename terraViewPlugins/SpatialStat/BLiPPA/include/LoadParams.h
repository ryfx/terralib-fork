#ifndef LOADPARAMS_H
#define LOADPARAMS_H

#include <BLiPPAPluginWindow.h>
#include <BLiPPAForm.h>
#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>

#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h> 
#include <qradiobutton.h>
#include <qmessagebox.h>
#include <qbuttongroup.h> 
#include <qtabwidget.h>
#include <qtextbrowser.h> 
#include <qspinbox.h> 
#include <vector>

void LoadParams(TeTheme *theme, std::vector<double> &x, std::vector<double> &y, std::vector<string> &objIds);
//void BoxBiVariate(TeTheme *theme1,TeTheme *theme2, vector<double> &coordx1, vector<double> &coordy1,
//				  vector<double> &coordx2, vector<double> &coordy2, double &a, double &b);
//void BoxUniVariate(TeTheme *theme1, vector<double> &coordx1, vector<double> &coordy1, double &a, double &b);

#endif
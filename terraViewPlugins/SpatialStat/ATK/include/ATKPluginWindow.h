
#ifndef ATKPLUGINWINDOW_H
#define ATKPLUGINWINDOW_H

//Entrar com os .h das funcoes de ATK
#include <ATKForm.h>
//#include "ClusterAlg.h"
//#include "Clustering.h"
//#include "Event.h"
//#include "Geometry.h"
//#include "ProspectiveAnalysis.h"
//#include "Resource.h"
//#include "Scan.h"
//#include "ScanInt.h"
//#include "util.h"

//STL include files 
#include <vector>

#include <PluginParameters.h>
#include <TeSharedPtr.h>

class ATKPluginWindow : public ATK
{

Q_OBJECT

public:
	ATKPluginWindow(QWidget* parent);
	~ATKPluginWindow();
	

protected:
	int ncols_;
	std::vector<QColor> _colors;
	PluginParameters	plugIngParams_;
	TeDatabase*			localDb_;
	vector<string>		result_;
	bool				control_;
	int					ViewId_;

public:	
	void InitializefunctionComboBox();
	void updateForm(PluginParameters* pluginParams);
	void hideEvent(QHideEvent* );


public slots:
	void temaComboBoxClickSlot(const QString&);
    void okButtonClickSlot();
	void helpButtonSlot();
    void table1ComboBoxClickSlot(const QString&);
	void enabletimecolumnSlot();
	
};

#endif 



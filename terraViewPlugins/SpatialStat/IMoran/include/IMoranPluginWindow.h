
#ifndef IMORANPLUGINWINDOW_H
#define IMORANPLUGINWINDOW_H

#include <IMoranForm.h>
//#include "EBI.h"
#include <PluginParameters.h>
#include <TeSharedPtr.h>

#include <vector>

class IMoranPluginWindow : public imoran
{

Q_OBJECT

public:
	IMoranPluginWindow(QWidget* parent);
	~IMoranPluginWindow();
	bool fillProxMatrixCombo(const QString& temaName);
	void setDefaultMatrix(const QString& temaName);

protected:
	PluginParameters	plugIngParams_;
	TeDatabase*			localDb_;
	vector<string>		result_;
	map<int, int>		comboIndex_;
	bool				control_;
	int					ViewId_;

public slots:
	void temaComboBoxClickSlot(const QString&);
	void CloseSlot();
	void Enable_matrixSlot();
    void okButtonClickSlot();
	void helpButtonSlot();
    void tableComboBoxClickSlot(const QString&);
	void updateForm(PluginParameters* pluginParams);
	void hideEvent(QHideEvent* );
	
};

#endif 



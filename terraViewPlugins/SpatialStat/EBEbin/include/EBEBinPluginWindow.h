
#ifndef EBEBINPLUGINWINDOW_H
#define EBEBINPLUGINWINDOW_H

#include <EBEbinForm.h>
#include "EBEbinomial.h"
#include <PluginParameters.h>
#include <TeSharedPtr.h>

class EBEbinPluginWindow : public ebe
{

Q_OBJECT

public:
	EBEbinPluginWindow(QWidget* parent);
	~EBEbinPluginWindow();
	void init();


protected:
	PluginParameters	plugIngParams_;
	TeDatabase*			localDb_;
	vector<string>		result_;
	map<int, int>		comboIndex_;
	bool				control_;
	int					ViewId_;

public slots:
	void temaComboBoxClickSlot(const QString&);
	void Disable_advAttrSlot();
	void scanButtonClickSlot();
	void helpButtonSlot();
    void tableComboBoxClickSlot(const QString&);
	void updateForm(PluginParameters* pluginParams);
	void hideEvent(QHideEvent* );
	
};

#endif 



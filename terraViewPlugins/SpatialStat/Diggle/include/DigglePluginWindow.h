
#ifndef DIGGLEPLUGINWINDOW_H
#define DIGGLEPLUGINWINDOW_H


#include <DiggleForm.h>

#include <PluginParameters.h>
#include <TeSharedPtr.h>

#include <vector>

class DigglePluginWindow : public diggle
{

Q_OBJECT

public:
	DigglePluginWindow(QWidget* parent);
	~DigglePluginWindow();

protected:
	int ncols_;
	std::vector<QColor> _colors;
	PluginParameters	plugIngParams_;
	TeDatabase*			localDb_;
	vector<string>		result_;
	map<int, int>		comboIndex_;
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
	
};

#endif 




#ifndef CUZICKPLUGINWINDOW_H
#define CUZICKPLUGINWINDOW_H


#include <CuzickForm.h>

#include <PluginParameters.h>
#include <TeSharedPtr.h>

#include <vector>

class CuzickPluginWindow : public cuzick
{

Q_OBJECT

public:
	CuzickPluginWindow(QWidget* parent);
	~CuzickPluginWindow();

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



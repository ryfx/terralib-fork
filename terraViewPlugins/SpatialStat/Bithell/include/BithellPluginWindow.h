
#ifndef BITHELLPLUGINWINDOW_H
#define BITHELLPLUGINWINDOW_H


#include <BithellForm.h>

#include <PluginParameters.h>
#include <TeSharedPtr.h>

#include <vector>

class BithellPluginWindow : public bithell
{

Q_OBJECT

public:
	BithellPluginWindow(QWidget* parent);
	~BithellPluginWindow();

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
//	void helpPushButton_clicked();
	void helpPushButton();
    void table1ComboBoxClickSlot(const QString&);
	
};

#endif 



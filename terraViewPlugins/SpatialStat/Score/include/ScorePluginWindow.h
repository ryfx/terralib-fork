
#ifndef SCOREPLUGINWINDOW_H
#define SCOREPLUGINWINDOW_H


#include <ScoreForm.h>

#include <PluginParameters.h>
#include <TeSharedPtr.h>

#include <vector>

class ScorePluginWindow : public score
{

Q_OBJECT

public:
	ScorePluginWindow(QWidget* parent);
	~ScorePluginWindow();

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
    void Enable_pondSlot(const QString&);
	void table1ComboBoxClickSlot(const QString&);
	};

#endif 



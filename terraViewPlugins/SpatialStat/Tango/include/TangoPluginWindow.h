#ifndef TANGOPLUGINWINDOW_H
#define TANGOPLUGINWINDOW_H

#include <TangoForm.h>

#include <PluginParameters.h>
#include <TeSharedPtr.h>

//STL include files
#include <vector>

class TangoPluginWindow : public tango
{

Q_OBJECT

public:
	TangoPluginWindow(QWidget* parent);
	~TangoPluginWindow();

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
	void CloseSlot();
    void RunButtonClickSlot();
	void helpButtonSlot();
    void tableComboBoxClickSlot(const QString&);
	void Enable_StrataSlot();

	
};

#endif 



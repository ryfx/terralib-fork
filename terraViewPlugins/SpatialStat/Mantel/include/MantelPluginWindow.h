
#ifndef MANTELPLUGINWINDOW_H
#define MANTELPLUGINWINDOW_H

#include <MantelForm.h>


#include <PluginParameters.h>
#include <TeSharedPtr.h>

//STL include files
#include <vector>


class MantelPluginWindow : public mantel
{

Q_OBJECT

public:
	MantelPluginWindow(QWidget* parent);
	~MantelPluginWindow();

protected:
	PluginParameters	plugIngParams_;
	TeDatabase*			localDb_;
	vector<string>		result_;
	bool				control_;
	int					ViewId_;

public:
	void updateForm(PluginParameters* pluginParams);
	void hideEvent(QHideEvent* );

public slots:
	void temaComboBoxClickSlot(const QString&);
	void CloseSlot();
    void EnableSpaceSlot();
    void EnableTimeSlot();
    void enabletimecolumnSlot();
    void RunButtonClickSlot();
    void tableComboBoxClickSlot(const QString&);
	
};

#endif 



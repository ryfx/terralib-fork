
#ifndef SPATIALSCANPLUGINWINDOW_H
#define SPATIALSCANPLUGINWINDOW_H

#include <SpatialScanForm.h>
#include <PluginParameters.h>
#include <TeSharedPtr.h>

//STL include files
#include <vector>

class SpatialScanPluginWindow : public spatialscan
{

Q_OBJECT

public:
	SpatialScanPluginWindow(QWidget* parent);
	~SpatialScanPluginWindow();
	bool fillProxMatrixCombo(const QString& temaName);
	void setDefaultMatrix(const QString& temaName);

protected:
	PluginParameters	plugIngParams_;
	TeDatabase*			localDb_;
	std::vector<std::string>		result_;
	bool				control_;
	map<int, int>		comboIndex_;
	int					ViewId_;

public slots:
	void temaComboBoxClickSlot(const QString&);
	void CloseSlot();
	void Enable_matrixSlot();
    void scanButtonClickSlot();
	void helpButtonSlot();
    void tableComboBoxClickSlot(const QString&);
	void updateForm(PluginParameters* pluginParams);
	void hideEvent(QHideEvent* );
	
};

#endif 



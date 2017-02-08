
#ifndef GRIMSONPLUGINWINDOW_H
#define GRIMSONPLUGINWINDOW_H


#include <GrimsonForm.h>
#include <xyplotgraphic.h>

#include <PluginParameters.h>
#include <TeSharedPtr.h>

//STL include files
#include <vector>

struct estrutura
     {
     int regiao;
     double variavel;
     vector<int> vizinhos;
     bool highrisk;
     };


class GrimsonPluginWindow : public grimson
{

Q_OBJECT

public:
	GrimsonPluginWindow(QWidget* parent);
	~GrimsonPluginWindow();
	XYPlotGraphic		*gra_;

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
	bool fillProxMatrixCombo(const QString& temaName);
	void setDefaultMatrix(const QString& temaName);
	void Enable_matrixSlot();
    void okButtonClickSlot();
	void helpButtonSlot();
    void table1ComboBoxClickSlot(const QString&);
	
};

#endif 



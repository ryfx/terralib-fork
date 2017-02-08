
#ifndef SRPLUGINWINDOW_H
#define SRPLUGINWINDOW_H

#include <SRForm.h>
#include <xyplotgraphic.h>
#include <PluginParameters.h>
#include <TeSharedPtr.h>

//STL include files
#include <vector>
///Classe com informacao sobre os circulos de saida do plugin SR
class Elements
{
 public:
    int id;
	double x;
	double y;
	double dist;
	double pvalue;

 friend bool operator<(Elements &a,Elements &b){
		return(a.pvalue<b.pvalue);
	}
};

class SRPluginWindow : public sr
{

Q_OBJECT

public:
	SRPluginWindow(QWidget* parent);
	~SRPluginWindow();
	XYPlotGraphic		*gra_;

protected:
	int ncols_;
	std::vector<QColor> _colors;
	PluginParameters	plugIngParams_;
	TeDatabase*			localDb_;
	vector<string>		result_;
	bool				control_;
	int					ViewId_, layer_;

public:	
	void init();
	void updateForm(PluginParameters* pluginParams);
	void hideEvent(QHideEvent* );


public slots:
	void temaComboBoxClickSlot(const QString&);
	void Enable_timeSlot();
	void CloseSlot();
	void okButtonClickSlot();
	void helpButtonSlot();
    void tableComboBoxClickSlot(const QString&);
	
};

#endif 



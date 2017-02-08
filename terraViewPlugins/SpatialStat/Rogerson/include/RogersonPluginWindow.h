
#ifndef ROGERSONPLUGINWINDOW_H
#define ROGERSONPLUGINWINDOW_H


#include <PluginParameters.h>
#include <xyplotgraphic.h>
#include <TeSharedPtr.h>
#include <RogersonForm.h>

#include <vector>

class RogersonPluginWindow : public rogerson
{

Q_OBJECT

public:
	RogersonPluginWindow(QWidget* parent);
	~RogersonPluginWindow();
	XYPlotGraphic		*gra_;

protected:
	int ncols_;
	std::vector<QColor> _colors;
	PluginParameters	plugIngParams_;
	TeDatabase*			localDb_;
	vector<string>		result_;
	bool				control_;
	int					ViewId_;

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



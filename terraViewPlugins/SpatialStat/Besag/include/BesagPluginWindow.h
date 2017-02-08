
#ifndef BESAGPLUGINWINDOW_H
#define BESAGPLUGINWINDOW_H

#include <BesagForm.h>
#include <PluginParameters.h>
#include <TeSharedPtr.h>
//#include <matrixbesag.h>
#include <besagfunc.h>

///Classe com informacao sobre os circulos de saida do plugin Besag e Newell
class Elements
{
 public:
	int id;
	double dist;
	double pvalue;

	bool operator<(const Elements &b) const 
	{
		return(this->pvalue<b.pvalue);
	}
};


class BesagPluginWindow : public besag
{

Q_OBJECT

public:
	BesagPluginWindow(QWidget* parent);
	~BesagPluginWindow();
	void init();


protected:
	PluginParameters	plugIngParams_;
	TeDatabase*			localDb_;
	vector<string>		result_;
	bool				control_;
	int					ViewId_, layer_;

public slots:
	int temaComboBoxClickSlot(const QString&);
	void CloseSlot();
    void scanButtonClickSlot();
	void helpButtonSlot();
    void tableComboBoxClickSlot(const QString&);
	void updateForm(PluginParameters* pluginParams);
	void hideEvent(QHideEvent* );
	
};

#endif 




#ifndef KNOXPLUGINWINDOW_H
#define KNOXPLUGINWINDOW_H

//Entrar com os .h das funcoes de Knox
#include <KnoxForm.h>


#include <PluginParameters.h>
#include <TeSharedPtr.h>


//STL include files
#include <vector>


typedef long SIDStatId;

typedef double SIDStatTime; //long 

typedef struct stPoint
{
   double x;
   double y;      
}SIDStatPoint;

typedef struct stETP
{
   SIDStatId eId;
   SIDStatTime eTime;
   SIDStatPoint ePos;
}SIDStatEvETPoint;



class KnoxPluginWindow : public knox
{

Q_OBJECT

public:
	KnoxPluginWindow(QWidget* parent);
	~KnoxPluginWindow();
	

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
    void okButtonClickSlot();
    void table1ComboBoxClickSlot(const QString&);
	void enabletimecolumnSlot();
	void helpButtonSlot();
	
};

#endif 



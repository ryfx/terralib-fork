
#ifndef KNNPLUGINWINDOW_H
#define KNNPLUGINWINDOW_H

//Entrar com os .h das funcoes de kNN
#include <kNNForm.h>


#include <PluginParameters.h>
#include <TeSharedPtr.h>

//STL include files
#include <vector>

//class Help;

class kNNPluginWindow : public kNN
{

Q_OBJECT

public:
	kNNPluginWindow(QWidget* parent);
	~kNNPluginWindow();
	

protected:
	int ncols_;
	std::vector<QColor> _colors;
	PluginParameters	plugIngParams_;
	TeDatabase*			localDb_;
	vector<string>		result_;
	bool				control_;
	int					ViewId_;
//	Help*               help_;

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
	//void helpPushButton_clicked();
	
};

#endif 




#ifndef BLiPPAPLUGINWINDOW_H
#define BLiPPAPLUGINWINDOW_H

//Entrar com os .h das funcoes de BLiPPA
#include <BLiPPAForm.h>


#include <PluginParameters.h>
#include <TeSharedPtr.h>

//STL include files
#include <vector>

//class Help;

class BLiPPAPluginWindow : public BLiPPA
{

Q_OBJECT

public:
	BLiPPAPluginWindow(QWidget* parent);
	~BLiPPAPluginWindow();
	

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
	void init();
	void updateForm(PluginParameters* pluginParams);
	void hideEvent(QHideEvent* );


public slots:
    void okButtonClickSlot();
	void helpButtonSlot();
	
};

#endif 



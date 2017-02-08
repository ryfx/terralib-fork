#ifndef _WMS_CONFIGURER_H
#define _WMS_CONFIGURER_H

//QT include files
#include <qobject.h>

//STL include files
#include <vector>

//Foward declarations
class PluginParameters;
class QMainWindow;
class QPopupMenu;
class QAction;

class TeWMSConfigurer : public QObject
{
	Q_OBJECT

public:
	TeWMSConfigurer(PluginParameters* plgParams);
	virtual ~TeWMSConfigurer();

	virtual void updateTVInterface(const bool& addButtons = true);

public slots:

	void showPluginWindow();

protected:
 
	/* Função que encontra o menu de temas numa tela principal */
	QPopupMenu* findThemesMenu(QMainWindow* tvMainWin);

	std::vector<int> mnuIdxs_;
	PluginParameters* plgParams_;
	QAction*		  plgAction_;
};

#endif //_WMS_CONFIGURER_H
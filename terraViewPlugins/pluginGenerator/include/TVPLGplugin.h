#ifndef _TVIEWPLUGIN_PLUGIN_H_
#define _TVIEWPLUGIN_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>

//Forward declarations
class QAction;

class TVPLGplugin :	public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	TVPLGplugin(PluginParameters* params);

	~TVPLGplugin();

	void init();

	void end();

protected slots:

	void showWindow();

	QAction* showWinAction_;
};

#endif //_TVIEWPLUGIN_PLUGIN_H_

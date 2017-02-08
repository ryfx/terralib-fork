#ifndef __SCORE_PLUGIN_H_
#define __SCORE_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class ScorePluginWindow;

class ScorePlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	ScorePlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~ScorePlugin();

	/**	\brief Initializer method.
	 */
	void init();

	/**	\brief Finalizer method.
	 */
	void end();

protected slots:

	/**	\brief Slot used to present a window.
	 */
	void showWindow();

protected:

	QAction* showWinAction_;
	ScorePluginWindow* win_;
};

#endif //__SCORE_PLUGIN_H_
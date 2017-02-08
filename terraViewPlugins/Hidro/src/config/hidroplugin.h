#ifndef __HIDRO_PLUGIN_H_
#define __HIDRO_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>

class HidroPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	HidroPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~HidroPlugin();

	/**	\brief Initializer method.
	 */
	void init();

	/**	\brief Finalizer method.
	 */
	void end();
};

#endif //__HIDRO_PLUGIN_H_
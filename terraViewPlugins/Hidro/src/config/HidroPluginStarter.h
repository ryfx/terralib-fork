/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroPluginStarter.h
*
*******************************************************************************
*
* $Rev: 7969 $:
*
* $Author: eric $:
*
* $Date: 2009-08-06 15:58:50 +0300 (to, 06 elo 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Config Group.
	@defgroup hidroConfig The Group for Plugin Configuration.
*/

#ifndef HIDRO_PLUGIN_STARTER_H_
#define HIDRO_PLUGIN_STARTER_H_

/** 
  * \file HidroPluginStarter.h
  *
  * \class HidroPluginStarter
  *
  * \brief This file is a class to define the plugin starter
  *
  * This class is usefull to start terra hidro plugin, when plugin
  * was started on terraView application, a toolbar was insert in tv app
  * creating two new buttons. First button was used to open a hidro
  * modeling interface and a second button was used to open a hidro
  * edition interface.
  *
  * This class is a qt class dependency, (moc operation) because has
  * to receive qt events send by application.
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
  * \sa HidroPluginCode.h
/*!
  @ingroup hidroConfig
 */

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <qobject.h>

class QAction;

class PluginParameters;

class HidroMainWindow;
class HidroEditionWindow;
class HidroAttributeWindow;
class HidroClassWindow;


class HidroPluginStarter : public QObject
{
	Q_OBJECT

public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor.
	*/
	HidroPluginStarter(PluginParameters* params);

	/** \brief Virtual destructor.
	*/
	~HidroPluginStarter();

	//@}

public:

	/** @name Behavioral Methods
		* Methods related to hidro start plugin behavior.
		*/
	//@{

    /** \brief Function used to start hidro plugin in terraView app.
      *
      * \return Return true if the plugin was started correctly and false in other case.
	  *         Its necessary has a current database to start this plugin.
      */
	bool start();

	//@}

public slots:

	/** @name Slots Methods
		* Methods related to qt events.
		*/
	//@{

	/** \brief Function used when modeling button was pressed in tv app, a hidro modeling interface was openned
      */
	virtual void showMainWindow();

	/** \brief Function used when edition button was pressed in tv app, a hidro edition interface was openned
      */
	virtual void showEditionWindow();

	/** \brief Function used when attribute button was pressed in tv app, a hidro attributeinterface was openned
      */
	virtual void showAttributeWindow();

	/** \brief Function used when class button was pressed in tv app, a hidro class interface was openned
      */
	virtual void showClassWindow();

	//@}

protected:
	PluginParameters*		_params;			//!< Attribute used to define the parameters from application

	HidroMainWindow*		_mainWindow;		//!< Attribute used to define the hidro modeling interface

	HidroEditionWindow*		_editionWindow;		//!< Attribute used to define the hidro edition interface

	HidroAttributeWindow*	_attrWindow;		//!< Attribute used to define the hidro attribute interface

	HidroClassWindow*		_classWindow;		//!< Attribute used to define the hidro class interface

	bool					_started;			//!< Attribute used to define if hidro plugin is already started

	QAction*				_mainAction;		//!< Attribute used to define the qt action to start modeling interface

	QAction*				_editionAction;		//!< Attribute used to define the qt action to start edition interface

	QAction*				_attrAction;		//!< Attribute used to define the qt action to start attribute interface

	QAction*				_classAction;		//!< Attribute used to define the qt action to start class interface

};

/*
** ---------------------------------------------------------------
** End:
*/

#endif //HIDRO_PLUGIN_STARTER_H_
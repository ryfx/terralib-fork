#ifndef __TVPS_PLUGIN_SHORTCUT_H_
#define __TVPS_PLUGIN_SHORTCUT_H_

// STL
#include <string>
#include <vector>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;
class QAction;
class QString;
class QWidget;

class Shortcut : public QObject
{
    Q_OBJECT

public:

    Shortcut(TerraViewBase* tv);

	virtual ~Shortcut();

public:

    /** \brief Sets whether the shortcut is enabled.
      */
    virtual void setEnabled(const bool& e = true) = 0;

protected:

	/** \brief  Method used to create an action setting his properties
	  * \param  icon	 Defines the icon for this action
	  * \param  text	 Defines the text for this action
	  * \param  toolTip  Defines the tool tip for this action
      * \param  m	     Defines the method that will be called by this action.
      * \param  w    Defines the QWidget that will contains this action.
	  * \param  toggle	 Defines if this action will be toogled or not
      * \param  receiver Defines the object that will receive the action.
	  * \return Defined action if this function work correctly.
      */
	virtual QAction* createAction(const std::string& icon, const QString& text,
								  const QString& toolTip, 
								  const char* m, QWidget* w,
								  const bool& toggle = false,
                                  QObject* receiver = 0);

protected:

	TerraViewBase* _tv;             //!< A pointer to TerraView Main Window.
    std::vector<QAction*> _actions; //!< The set of actions to this shortcut.
};

#endif // __TVPS_PLUGIN_SHORTCUT_H_

// tvps
#include <Shortcut.h>

// TerraLib
#include <TerraViewBase.h>

// Qt
#include <qaction.h>
#include <qwidget.h>

Shortcut::Shortcut(TerraViewBase* tv) :
_tv(tv)
{}

Shortcut::~Shortcut()
{
    for(unsigned int i = 0; i < _actions.size(); ++i)
        delete _actions[i];
}

QAction* Shortcut::createAction(const std::string& icon,
							    const QString& text,
							    const QString& toolTip, 
							    const char* m,
                                QWidget* w,
							    const bool& toggle,
                                QObject* receiver)
{
	QAction* action = new QAction(0);
	action->setIconSet(QIconSet(QPixmap::fromMimeSource(icon.c_str())));
    action->setText(text);
	action->setToolTip(toolTip);
	action->setToggleAction(toggle);
    action->setOn(true);
    action->addTo(w);
    _actions.push_back(action);

    if(receiver == 0)
        connect(action, SIGNAL(activated()), this, m);
    else
        connect(action, SIGNAL(activated()), receiver, m);

	return action;
}

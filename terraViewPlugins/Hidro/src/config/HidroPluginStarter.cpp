#include <HidroPluginStarter.h>
#include <HidroMainWindow.h>
#include <HidroEditionWindow.h>
#include <HidroAttributeWindow.h>
#include <HidroClassWindow.h>

#include <HidroBoostAlgorithmFactory.h>

#include <PluginParameters.h>

#include <qaction.h>
#include <qtoolbar.h>
#include <qmainwindow.h>


HidroPluginStarter::HidroPluginStarter(PluginParameters* params):_params(params)
{
	_mainWindow		= NULL;
	_mainAction		= NULL;

	_editionWindow	= NULL;
	_editionAction  = NULL;

	_attrWindow		= NULL;
	_attrAction		= NULL;

	_classWindow	= NULL;
	_classAction	= NULL;

	_started = false;
}

HidroPluginStarter::~HidroPluginStarter()
{
//delete main window objects
	if(_mainWindow)
	{
		delete _mainWindow;
		_mainWindow = 0;
	}

	if(_mainAction)
	{
		delete _mainAction;
		_mainAction = 0;
	}

//delete edition window objects
	if(_editionWindow)
	{
		delete _editionWindow;
		_editionWindow = 0;
	}

	if(_editionAction)
	{
		delete _editionAction;
		_editionAction = 0;
	}

//delete attr window objects
	if(_attrWindow)
	{
		delete _attrWindow;
		_attrWindow = 0;
	}

	if(_attrAction)
	{
		delete _attrAction;
		_attrAction = 0;
	}

//delete class window objects
	if(_classWindow)
	{
		delete _classWindow;
		_classWindow = 0;
	}

	if(_classAction)
	{
		delete _classAction;
		_classAction = 0;
	}
	
	_started = false;
}

bool HidroPluginStarter::start()
{
	if(_started)
	{
		return true;
	}

	if(!_params)
	{
		return false;
	}

//Inserting the tool buttons into TerraView toolbar
	QPtrList< QToolBar > tvToolBarsList = _params->qtmain_widget_ptr_->toolBars( Qt::DockTop );

	if( tvToolBarsList.count() > 0 ) 
	{
//create main action
		_mainAction = new QAction(NULL, tr("Graph Manipulation"));

		_mainAction->setText(tr("Graph Modeling"));
		_mainAction->setMenuText(tr("Graph Modeling"));
		_mainAction->setToolTip(tr("Graph Modeling"));
		_mainAction->setIconSet( QIconSet( QPixmap::fromMimeSource("hidroIconMain.bmp")));
		_mainAction->addTo(_params->qtmain_widget_ptr_->toolBars(Qt::DockTop).at(0));

		connect( _mainAction, SIGNAL( activated() ), this, SLOT( showMainWindow() ) );

//create edition action
		_editionAction = new QAction(NULL, tr("Graph Edition"));

		_editionAction->setText(tr("Graph Edition"));
		_editionAction->setMenuText(tr("Graph Edition"));
		_editionAction->setToolTip(tr("Graph Edition"));
		_editionAction->setIconSet( QIconSet( QPixmap::fromMimeSource("hidroIconEdition.bmp")));
		_editionAction->addTo(_params->qtmain_widget_ptr_->toolBars(Qt::DockTop).at(0));

		connect( _editionAction, SIGNAL( activated() ), this, SLOT( showEditionWindow() ) );

//create attribute action
		_attrAction = new QAction(NULL, tr("Graph Attributes"));

		_attrAction->setText(tr("Graph Attributes"));
		_attrAction->setMenuText(tr("Graph Attributes"));
		_attrAction->setToolTip(tr("Graph Attributes"));
		_attrAction->setIconSet( QIconSet( QPixmap::fromMimeSource("hidroIconAttribute.bmp")));
		_attrAction->addTo(_params->qtmain_widget_ptr_->toolBars(Qt::DockTop).at(0));

		connect( _attrAction, SIGNAL( activated() ), this, SLOT( showAttributeWindow() ) );

//create attribute action
		_classAction = new QAction(NULL, tr("Graph Classes"));

		_classAction->setText(tr("Graph Classes"));
		_classAction->setMenuText(tr("Graph Classes"));
		_classAction->setToolTip(tr("Graph Classes"));
		_classAction->setIconSet( QIconSet( QPixmap::fromMimeSource("hidroIconClass.bmp")));
		_classAction->addTo(_params->qtmain_widget_ptr_->toolBars(Qt::DockTop).at(0));

		connect( _classAction, SIGNAL( activated() ), this, SLOT( showClassWindow() ) );

	}

	_started = true;

	return true;
}

void HidroPluginStarter::showMainWindow()
{
	if(_mainWindow)
	{
		delete _mainWindow;
	}

	_mainWindow = new HidroMainWindow(_params->qtmain_widget_ptr_, "Main Window", false);

	_mainWindow->init(_params);

	_mainWindow->show();
}

void HidroPluginStarter::showEditionWindow()
{
	if(_editionWindow)
	{
		delete _editionWindow;
	}

	_editionWindow = new HidroEditionWindow(_params->qtmain_widget_ptr_, "Edition Window", false);

	_editionWindow->init(_params);

	_editionWindow->show();
}

void HidroPluginStarter::showAttributeWindow()
{
	if(_attrWindow)
	{
		delete _attrWindow;
	}

	_attrWindow = new HidroAttributeWindow(_params->qtmain_widget_ptr_, "Attribute Window", false);

	_attrWindow->init(_params);

	_attrWindow->show();
}

void HidroPluginStarter::showClassWindow()
{
	if(_classWindow)
	{
		delete _classWindow;
	}

	_classWindow = new HidroClassWindow(_params->qtmain_widget_ptr_, "Class Window", false);

	_classWindow->init(_params);

	_classWindow->show();
}
// tvps
#include <ViewMenu.h>

// TerraLib
#include <TerraViewBase.h>

// Qt
#include <qaction.h>
#include <qpopupmenu.h>

ViewMenu::ViewMenu(TerraViewBase* tv, QPopupMenu* menu)
: ViewShortcut(tv),
_menu(menu)
{
	connect(_menu, SIGNAL(aboutToShow()), this, SLOT(buildMenu()));
}

ViewMenu::~ViewMenu()
{}

void ViewMenu::buildMenu()
{
	_tv->getViewMenu(_menu);
	_menu->connectItem(_menu->idAt(0), this, SLOT(renameView())); // Rename view
	_menu->connectItem(_menu->idAt(1), this, SLOT(removeView())); // Remove view
	_menu->connectItem(_menu->idAt(2), this, SLOT(exportThemes())); // Export themes
	_menu->connectItem(_menu->idAt(4), this, SLOT(viewProjection())); // View projection
	_menu->connectItem(_menu->idAt(6), this, SLOT(connectView())); // Connect view

	// GeoProcessing
	QMenuItem* geoPro = _menu->findItem(_menu->idAt(8));
	QPopupMenu* geoProPopup = geoPro->popup();
	geoProPopup->connectItem(geoProPopup->idAt(0), this, SLOT(aggreagtion()));  
	geoProPopup->connectItem(geoProPopup->idAt(1), this, SLOT(add()));  
	geoProPopup->connectItem(geoProPopup->idAt(2), this, SLOT(intersection()));  
	geoProPopup->connectItem(geoProPopup->idAt(3), this, SLOT(difference()));  
	QMenuItem* assing = geoProPopup->findItem(geoProPopup->idAt(4));
	QPopupMenu* assingPopup = assing->popup();
	assingPopup->connectItem(assingPopup->idAt(0), this, SLOT(assignDistribute()));  
	assingPopup->connectItem(assingPopup->idAt(1), this, SLOT(assignColect())); 
	geoProPopup->connectItem(geoProPopup->idAt(5), this, SLOT(buffer()));  

	_menu->connectItem(_menu->idAt(10), this, SLOT(viewInfo())); // View properties

	int id = _menu->insertItem(QIconSet(QPixmap::fromMimeSource("view.bmp")), tr("Add..."), this, SLOT(addView()), 0, -1, 0);
	if(!_tv->currentDatabase())
		_menu->setItemEnabled(id, false);
	_menu->insertSeparator(1);
}

void ViewMenu::setEnabled(const bool& e)
{
    _menu->setEnabled(e);
}

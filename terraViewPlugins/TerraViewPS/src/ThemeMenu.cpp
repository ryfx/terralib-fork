// tvps
#include <ThemeMenu.h>

// TerraLib
#include <TerraViewBase.h>

// Qt
#include <qaction.h>
#include <qpopupmenu.h>

ThemeMenu::ThemeMenu(TerraViewBase* tv, QPopupMenu* menu)
: ThemeShortcut(tv),
_menu(menu)
{
	connect(_menu, SIGNAL(aboutToShow()), this, SLOT(buildMenu()));
}

ThemeMenu::~ThemeMenu()
{}

void ThemeMenu::buildMenu()
{
	_tv->getThemeMenu(_menu);
	_menu->connectItem(_menu->idAt(0), this, SLOT(renameTheme())); // Rename theme
	_menu->connectItem(_menu->idAt(1), this, SLOT(removeTheme())); // Remove view

	 // Visual
	QMenuItem* visual = _menu->findItem(_menu->idAt(3));
	QPopupMenu* visualPopup = visual->popup();
	visualPopup->connectItem(visualPopup->idAt(0), this, SLOT(themeVisual()));  
	visualPopup->connectItem(visualPopup->idAt(1), this, SLOT(themeVisualPointing()));  
	visualPopup->connectItem(visualPopup->idAt(2), this, SLOT(themeVisualQuery()));  
	visualPopup->connectItem(visualPopup->idAt(3), this, SLOT(themeVisualPointingQuery()));  

	_menu->connectItem(_menu->idAt(4), this, SLOT(visRepresentations())); // Visibility of representations

	_menu->connectItem(_menu->idAt(6), this, SLOT(editLegend())); // Legend
	_menu->connectItem(_menu->idAt(7), this, SLOT(textRepresentation())); // Text representation

	_menu->connectItem(_menu->idAt(9), this, SLOT(createLayerFromTheme())); // Create Layer from Theme
	_menu->connectItem(_menu->idAt(10), this, SLOT(createThemeFromTheme())); // Create Theme from Theme
	_menu->connectItem(_menu->idAt(11), this, SLOT(saveThemeToFile())); // Save theme to file

	_menu->connectItem(_menu->idAt(13), this, SLOT(removeAliases())); // Remove aliases

	_menu->connectItem(_menu->idAt(15), this, SLOT(themeInfo()));    // Theme properties

	int id = _menu->insertItem(QIconSet(QPixmap::fromMimeSource("theme.bmp")), tr("Add..."), this, SLOT(addTheme()), 0, -1, 0);

	if(!_tv->currentDatabase() || 
		_tv->currentDatabase()->layerMap().empty() || 
		_tv->currentDatabase()->viewMap().empty())
	{
		_menu->setItemEnabled(id, false);
	}

	_menu->insertSeparator(1);
}

void ThemeMenu::setEnabled(const bool& e)
{
    _menu->setEnabled(e);
}

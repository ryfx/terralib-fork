// tvps
#include <LayerMenu.h>

// TerraLib
#include <TerraViewBase.h>

// Qt
#include <qaction.h>
#include <qpopupmenu.h>

LayerMenu::LayerMenu(TerraViewBase* tv, QPopupMenu* menu)
: LayerShortcut(tv),
_menu(menu)
{
	connect(_menu, SIGNAL(aboutToShow()), this, SLOT(buildMenu()));
}

LayerMenu::~LayerMenu()
{}

void LayerMenu::buildMenu()
{
    _tv->getLayerMenu(_menu);
    _menu->connectItem(_menu->idAt(0),  this, SLOT(renameLayer()));                  // Rename layer
    _menu->connectItem(_menu->idAt(1),  this, SLOT(removeLayer()));                  // Remove layer
    _menu->connectItem(_menu->idAt(3),  this, SLOT(createCells()));                  // Create cells
    _menu->connectItem(_menu->idAt(5),  this, SLOT(createLayerTable()));             // Create layer table
    _menu->connectItem(_menu->idAt(6),  this, SLOT(deleteLayerTable()));             // Delete layer table
    _menu->connectItem(_menu->idAt(8),  this, SLOT(addCentroidRepresentation()));    // Add centroid representation
    _menu->connectItem(_menu->idAt(9),  this, SLOT(createCentroidLayer()));          // Create centroid layer
    _menu->connectItem(_menu->idAt(10), this, SLOT(createCentroidCountLayer()));     // Create centroid count layer
    _menu->connectItem(_menu->idAt(12), this, SLOT(loadMemory()));                   // Load memory
    _menu->connectItem(_menu->idAt(13), this, SLOT(clearMemory()));                  // Clear memory
    _menu->connectItem(_menu->idAt(15), this, SLOT(layerProjection()));              // Layer Projection

    QMenuItem* generalInfo = _menu->findItem(_menu->idAt(17));
    QPopupMenu* insert = generalInfo->popup();
    QMenuItem* insertItens = insert->findItem(insert->idAt(0));
    QPopupMenu* insertPopup = insertItens->popup();
    insertPopup->connectItem(insertPopup->idAt(0), this, SLOT(insertFileInfo()));    // Insert File
    insertPopup->connectItem(insertPopup->idAt(1), this, SLOT(insertURLInfo()));     // Insert URL
    insert->connectItem(insert->idAt(1), this, SLOT(retrieveInfo()));                // Retrieve info
    insert->connectItem(insert->idAt(2), this, SLOT(removeInfo()));                  // Remove info

    _menu->connectItem(_menu->idAt(19), this, SLOT(layerInfo()));                    // Layer properties

    _menu->insertItem(tr("Update Metadata..."), this, SLOT(updateMetadata()), 0, -1, 0);
    _menu->insertSeparator(1);
}

void LayerMenu::setEnabled(const bool& e)
{
    _menu->setEnabled(e);
}

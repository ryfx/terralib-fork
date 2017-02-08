// tvps
#include <LayerToolBar.h>

// TerraLib
#include <TerraViewBase.h>

// Qt
#include <qtoolbar.h>

LayerToolBar::LayerToolBar(TerraViewBase* tv)
: LayerShortcut(tv),
_toolBar(new QToolBar(tv))
{
	createAction("layerRename.png", "", tr("Rename the layer"), SLOT(renameLayer()), _toolBar);
    createAction("layerRemove.png", "", tr("Remove the layer"), SLOT(removeLayer()), _toolBar);
    createAction("layerInfo.png", "", tr("Show the layer information"), SLOT(layerInfo()), _toolBar);
    createAction("layerProjection.png", "", tr("Define the layer projection"), SLOT(layerProjection()), _toolBar);
}

LayerToolBar::~LayerToolBar()
{
	delete _toolBar;
}

void LayerToolBar::setEnabled(const bool& e)
{
    _toolBar->setEnabled(e);
}

void LayerToolBar::setVisible(const bool& e)
{
    if(!e)
        _toolBar->hide();
    else
        _toolBar->show();
}

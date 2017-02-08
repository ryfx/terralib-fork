/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <qmessagebox.h>

#include <HidroUtils.h>
#include <HidroConvexHull.h>

#include <TeQTCanvas.h>
#include <TeAppTheme.h>

void HidroMaskWindow::init( PluginParameters * parameters )
{
    _parameters = parameters;
}

void HidroMaskWindow::destroy()
{
	_line.clear();
	_polygon.clear();
}

void HidroMaskWindow::updateTablePoly()
{
	tableMask->setNumRows(_line.size());

	for(unsigned int i = 0; i < _line.size(); ++i)
	{
		tableMask->setText(i, 0, Te2String(i).c_str());
		tableMask->setText(i, 1, Te2String(_line[i].x(), 6).c_str());
		tableMask->setText(i, 2, Te2String(_line[i].y(), 6).c_str());
	}
}

void HidroMaskWindow::selectPushButton_clicked()
{
	_line.clear();
	_polygon.clear();

	if(_parameters->teqtcanvas_ptr_->getCursorMode() != TeQtCanvas::Pointer)
	{
		selectPushButton->setOn(false);

		QMessageBox::warning(this, tr("Warning"), tr("Application tool is in use."));
		return;
	}	

	if(!_parameters->getCurrentThemeAppPtr())
	{
		selectPushButton->setOn(false);

		QMessageBox::warning(this, tr("Warning"), tr("Please, select a raster theme first."));
		return;
	}

	TeTheme* theme = dynamic_cast<TeTheme*>(_parameters->getCurrentThemeAppPtr()->getTheme());
	if(!theme || !theme->layer()->hasGeometry(TeRASTER))
	{
		selectPushButton->setOn(false);

		QMessageBox::warning(this, tr("Warning"), tr("Please, select a raster theme first."));
		return;
	}

	if(selectPushButton->isOn())
	{
		_parameters->teqtcanvas_ptr_->copyPixmap0ToPixmap1();

		connect(_parameters->teqtcanvas_ptr_, SIGNAL(mousePressed(TeCoord2D&, int, QPoint&)), this, SLOT(getMouseClicked(TeCoord2D&, int, QPoint&)));

		connect(_parameters->teqtcanvas_ptr_, SIGNAL(mouseDoublePressed(TeCoord2D&, int, QPoint&)), this, SLOT(getMouseDoubleClicked(TeCoord2D&, int, QPoint&)));
	}
	else
	{
		_parameters->teqtcanvas_ptr_->copyPixmap1ToPixmap0();
		_parameters->teqtcanvas_ptr_->copyPixmap0ToWindow();

		disconnect(_parameters->teqtcanvas_ptr_, SIGNAL(mousePressed(TeCoord2D&, int, QPoint&)), this, SLOT(getMouseClicked(TeCoord2D&, int, QPoint&)));

		disconnect(_parameters->teqtcanvas_ptr_, SIGNAL(mouseDoublePressed(TeCoord2D&, int, QPoint&)), this, SLOT(getMouseDoubleClicked(TeCoord2D&, int, QPoint&)));
	}
}


void HidroMaskWindow::undoPushButton_clicked()
{
	_parameters->teqtcanvas_ptr_->copyPixmap1ToPixmap0();
	_parameters->teqtcanvas_ptr_->copyPixmap0ToWindow();

	if(_line.empty())
	{
		return;
	}

	_line.erase(_line.last());

	_parameters->teqtcanvas_ptr_->setLineStyle(TeLnTypeDashed, 3);	//dashed and double width
	_parameters->teqtcanvas_ptr_->setLineColor(255, 0, 0);

	_parameters->teqtcanvas_ptr_->plotOnPixmap0();

	_parameters->teqtcanvas_ptr_->plotLine(_line);

	_parameters->teqtcanvas_ptr_->copyPixmap0ToWindow();

	updateTablePoly();
}


void HidroMaskWindow::deletePushButton_clicked()
{
	_parameters->teqtcanvas_ptr_->copyPixmap1ToPixmap0();
	_parameters->teqtcanvas_ptr_->copyPixmap0ToWindow();

	if(_line.empty() || tableMask->currentRow() == -1)
	{
		return;
	}

	_line.erase(tableMask->currentRow());

	_parameters->teqtcanvas_ptr_->setLineStyle(TeLnTypeDashed, 3);	//dashed and double width
	_parameters->teqtcanvas_ptr_->setLineColor(255, 0, 0);

	_parameters->teqtcanvas_ptr_->plotOnPixmap0();

	_parameters->teqtcanvas_ptr_->plotLine(_line);

	_parameters->teqtcanvas_ptr_->copyPixmap0ToWindow();

	updateTablePoly();
}


void HidroMaskWindow::clearPushButton_clicked()
{
	_line.clear();
	_polygon.clear();

	_parameters->teqtcanvas_ptr_->copyPixmap1ToPixmap0();
	_parameters->teqtcanvas_ptr_->copyPixmap0ToWindow();

	updateTablePoly();
}


void HidroMaskWindow::okPushButton_clicked()
{
	if(layerNameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Output layer name not defined."));
		return;
	}

	if(_polygon.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Polygon not defined."));
		return;
	}

//create output layer

	std::string layerPolsName = layerNameLineEdit->text().latin1();

	HidroUtils utils(_parameters->getCurrentDatabasePtr());

	std::string errorMessage = "";

	if(!utils.checkLayerName(layerPolsName, errorMessage))
	{
		QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
		return;
	}

	TeProjection* proj = TeProjectionFactory::make(_parameters->getCurrentViewPtr()->projection()->params());

	TeLayer* outputLayer = new TeLayer(layerPolsName, _parameters->getCurrentDatabasePtr(), proj);

	if(!outputLayer)
	{
		return;
	}

//set the polygon id

	_polygon.geomId(0);
	_polygon.objectId("0");

	TePolygonSet inputPolSet;
	inputPolSet.add(_polygon);


//save output layer

	HidroConvexHull ch(_parameters->getCurrentDatabasePtr());

	if(!ch.createOutputLayer(outputLayer, inputPolSet))
	{
		std::string strError = ch.errorMessage();
		QMessageBox::warning(this, tr("Warning"), strError.c_str());

		delete outputLayer;
		return;
	}

	_parameters->updateTVInterface();

	close();
}

void HidroMaskWindow::getMouseClicked( TeCoord2D &coord, int state, QPoint & point)
{
	_line.add(coord);

	_parameters->teqtcanvas_ptr_->setLineStyle(TeLnTypeDashed, 3);	//dashed and double width
	_parameters->teqtcanvas_ptr_->setLineColor(255, 0, 0);

	_parameters->teqtcanvas_ptr_->plotOnPixmap0();

	_parameters->teqtcanvas_ptr_->plotLine(_line);

	_parameters->teqtcanvas_ptr_->copyPixmap0ToWindow();

	updateTablePoly();
}

void HidroMaskWindow::getMouseDoubleClicked( TeCoord2D &coord, int state, QPoint & point)
{
	if(_line.empty())
	{
		return;
	}

	_polygon.clear();

	TeLinearRing ring;
	for(unsigned int i = 0; i < _line.size(); ++i)
	{
		ring.add(_line[i]);
	}

	ring.add(_line[0]);

	_polygon.add(ring);

	_parameters->teqtcanvas_ptr_->setPolygonColor(255, 0, 0);
	_parameters->teqtcanvas_ptr_->setPolygonStyle((int)TePolyTypeFill, 1);
	_parameters->teqtcanvas_ptr_->setPolygonLineColor (0, 0, 0);
	_parameters->teqtcanvas_ptr_->setPolygonLineStyle((int)TeLnTypeDashed, 3);

	_parameters->teqtcanvas_ptr_->plotOnPixmap0();

	_parameters->teqtcanvas_ptr_->plotPolygon(_polygon);

	_parameters->teqtcanvas_ptr_->copyPixmap0ToWindow();

//disable tool
	selectPushButton->setOn(false);

	disconnect(_parameters->teqtcanvas_ptr_, SIGNAL(mousePressed(TeCoord2D&, int, QPoint&)), this, SLOT(getMouseClicked(TeCoord2D&, int, QPoint&)));

	disconnect(_parameters->teqtcanvas_ptr_, SIGNAL(mouseDoublePressed(TeCoord2D&, int, QPoint&)), this, SLOT(getMouseDoubleClicked(TeCoord2D&, int, QPoint&)));
}

void HidroMaskWindow::closeEvent( QCloseEvent * ce)
{
	clearPushButton_clicked();

	selectPushButton->setOn(false);

	disconnect(_parameters->teqtcanvas_ptr_, SIGNAL(mousePressed(TeCoord2D&, int, QPoint&)), this, SLOT(getMouseClicked(TeCoord2D&, int, QPoint&)));

	disconnect(_parameters->teqtcanvas_ptr_, SIGNAL(mouseDoublePressed(TeCoord2D&, int, QPoint&)), this, SLOT(getMouseDoubleClicked(TeCoord2D&, int, QPoint&)));

	ce->accept();
}
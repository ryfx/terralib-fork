/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <HidroMetadata.h>
#include <HidroUtils.h>
#include <TeCoord2D.h>
#include <TeQtCanvas.h>

#include <qmessagebox.h>

void HidroOptimizedEditionWindow::init( PluginParameters * parameters )
{
    _params			= parameters;
	_graphName		= "";
	_baseName		= "";
	_count			= 0;
	_vertexTheme	= NULL;
	_edgeTheme		= NULL;
	_createAttr		= false;

	disableDirections();

	connect(_params->teqtcanvas_ptr_, SIGNAL(mousePressed(TeCoord2D&, int, QPoint&)), this, SLOT(getMouseClicked(TeCoord2D&, int, QPoint&)));    
}

void HidroOptimizedEditionWindow::setGraphName(string graphName )
{
	_graphName = graphName;
}

void HidroOptimizedEditionWindow::setBaseName(string baseName )
{
	_baseName = baseName;
}

void HidroOptimizedEditionWindow::setVertexTheme( TeAppTheme* vertexTheme )
{
	_vertexTheme = vertexTheme;

	_params->setCurrentThemeAppPtr(_vertexTheme);
}

 void HidroOptimizedEditionWindow::setEdgeTheme( TeAppTheme* edgeTheme )
 {
	_edgeTheme = edgeTheme;
 }

 void HidroOptimizedEditionWindow::createAutoAttributes( bool createAuto )
 {
	 _createAttr = createAuto;
 }

 void HidroOptimizedEditionWindow::checkDirections( )
 {
	 if(_currentPoint.empty())
	 {
		 return;
	 }

	HidroUtils utils(_params->getCurrentDatabasePtr());

	TeTheme* currentTheme		= (TeTheme*)_params->getCurrentThemeAppPtr()->getTheme();
	TeProjection* projCanvas	= _params->teqtcanvas_ptr_->projection();
	double pixelSize			= _params->teqtcanvas_ptr_->pixelSize();

	TeCoord2D originalCoord = utils.getPointCoord(_graphName, currentTheme, projCanvas, pixelSize, _currentPointCoord);

	HidroOptimizedEdition optEdition(_params->getCurrentDatabasePtr(), _graphName);

//verify valid directions
	_optDir.hasPtUpper = false;
	_optDir.hasPtUpperRight = false;
	_optDir.hasPtUpperLeft = false;
	_optDir.hasPtLower = false;
	_optDir.hasPtLowerRight = false;
	_optDir.hasPtLowerLeft = false;
	_optDir.hasPtRight = false;
	_optDir.hasPtLeft = false;


	if(!optEdition.getValidDirections(originalCoord, _optDir))
	{
		disableDirections();

		QMessageBox::warning(this ,tr("Warning"), optEdition.getErrorMessage().c_str());
		return;
	}

//upper direction
	if(_optDir.hasPtUpper)
	{
		HidroMetadata metadata(_params->getCurrentDatabasePtr());
		std::string vertexDestiny = metadata.getVertexName(_graphName, _optDir.ptUpper.objectId());

		if(!vertexDestiny.empty())
		{
			bool status = false;

			bool hasOutputEdges = false;
			if(optEdition.hasEdge(_currentPoint, vertexDestiny, hasOutputEdges))
			{
				if(hasOutputEdges)
				{
					upPushButton->setPixmap(QPixmap::fromMimeSource("arrow-up_yellow.bmp"));
					status = true;
				}
			}

			bool hasInputEdges = false;
			if(optEdition.hasEdge(vertexDestiny, _currentPoint, hasInputEdges))
			{
				if(hasInputEdges)
				{
					upPushButton->setPixmap(QPixmap::fromMimeSource("arrow-up_red.bmp"));
					status = true;
				}
			}

			if(!status)
			{
				upPushButton->setPixmap(QPixmap::fromMimeSource("arrow-up_green.bmp"));
			}

			upPushButton->setEnabled(true);	
		}
	}
	else
	{
		upPushButton->setPixmap(QPixmap::fromMimeSource(""));
		upPushButton->setEnabled(false);
	}

//upper left direction
	if(_optDir.hasPtUpperLeft)
	{
		HidroMetadata metadata(_params->getCurrentDatabasePtr());
		std::string vertexDestiny = metadata.getVertexName(_graphName, _optDir.ptUpperLeft.objectId());

		if(!vertexDestiny.empty())
		{
			bool status = false;

			bool hasOutputEdges = false;
			if(optEdition.hasEdge(_currentPoint, vertexDestiny, hasOutputEdges))
			{
				if(hasOutputEdges)
				{
					upLeftPushButton->setPixmap(QPixmap::fromMimeSource("arrow-upleft_yellow.bmp"));
					status = true;
				}
			}

			bool hasInputEdges = false;
			if(optEdition.hasEdge(vertexDestiny, _currentPoint, hasInputEdges))
			{
				if(hasInputEdges)
				{
					upLeftPushButton->setPixmap(QPixmap::fromMimeSource("arrow-upleft_red.bmp"));
					status = true;
				}
			}

			if(!status)
			{
				upLeftPushButton->setPixmap(QPixmap::fromMimeSource("arrow-upleft_green.bmp"));
			}

			upLeftPushButton->setEnabled(true);	
		}
	}
	else
	{
		upLeftPushButton->setPixmap(QPixmap::fromMimeSource(""));
		upLeftPushButton->setEnabled(false);
	}

//upper right direction
	if(_optDir.hasPtUpperRight)
	{
		HidroMetadata metadata(_params->getCurrentDatabasePtr());
		std::string vertexDestiny = metadata.getVertexName(_graphName, _optDir.ptUpperRight.objectId());

		if(!vertexDestiny.empty())
		{
			bool status = false;

			bool hasOutputEdges = false;
			if(optEdition.hasEdge(_currentPoint, vertexDestiny, hasOutputEdges))
			{
				if(hasOutputEdges)
				{
					upRightPushButton->setPixmap(QPixmap::fromMimeSource("arrow-upright_yellow.bmp"));
					status = true;
				}
			}

			bool hasInputEdges = false;
			if(optEdition.hasEdge(vertexDestiny, _currentPoint, hasInputEdges))
			{
				if(hasInputEdges)
				{
					upRightPushButton->setPixmap(QPixmap::fromMimeSource("arrow-upright_red.bmp"));
					status = true;
				}
			}

			if(!status)
			{
				upRightPushButton->setPixmap(QPixmap::fromMimeSource("arrow-upright_green.bmp"));
			}

			upRightPushButton->setEnabled(true);	
		}
	}
	else
	{
		upRightPushButton->setPixmap(QPixmap::fromMimeSource(""));
		upRightPushButton->setEnabled(false);
	}

//lower direction
	if(_optDir.hasPtLower)
	{
		HidroMetadata metadata(_params->getCurrentDatabasePtr());
		std::string vertexDestiny = metadata.getVertexName(_graphName, _optDir.ptLower.objectId());

		if(!vertexDestiny.empty())
		{
			bool status = false;

			bool hasOutputEdges = false;
			if(optEdition.hasEdge(_currentPoint, vertexDestiny, hasOutputEdges))
			{
				if(hasOutputEdges)
				{
					downPushButton->setPixmap(QPixmap::fromMimeSource("arrow-down_yellow.bmp"));
					status = true;
				}
			}

			bool hasInputEdges = false;
			if(optEdition.hasEdge(vertexDestiny, _currentPoint, hasInputEdges))
			{
				if(hasInputEdges)
				{
					downPushButton->setPixmap(QPixmap::fromMimeSource("arrow-down_red.bmp"));
					status = true;
				}
			}

			if(!status)
			{
				downPushButton->setPixmap(QPixmap::fromMimeSource("arrow-down_green.bmp"));
			}

			downPushButton->setEnabled(true);	
		}
	}
	else
	{
		downPushButton->setPixmap(QPixmap::fromMimeSource(""));
		downPushButton->setEnabled(false);
	}

//lower left direction
	if(_optDir.hasPtLowerLeft)
	{
		HidroMetadata metadata(_params->getCurrentDatabasePtr());
		std::string vertexDestiny = metadata.getVertexName(_graphName, _optDir.ptLowerLeft.objectId());

		if(!vertexDestiny.empty())
		{
			bool status = false;

			bool hasOutputEdges = false;
			if(optEdition.hasEdge(_currentPoint, vertexDestiny, hasOutputEdges))
			{
				if(hasOutputEdges)
				{
					downLeftPushButton->setPixmap(QPixmap::fromMimeSource("arrow-downleft_yellow.bmp"));
					status = true;
				}
			}

			bool hasInputEdges = false;
			if(optEdition.hasEdge(vertexDestiny, _currentPoint, hasInputEdges))
			{
				if(hasInputEdges)
				{
					downLeftPushButton->setPixmap(QPixmap::fromMimeSource("arrow-downleft_red.bmp"));
					status = true;
				}
			}

			if(!status)
			{
				downLeftPushButton->setPixmap(QPixmap::fromMimeSource("arrow-downleft_green.bmp"));
			}

			downLeftPushButton->setEnabled(true);	
		}
	}
	else
	{
		downLeftPushButton->setPixmap(QPixmap::fromMimeSource(""));
		downLeftPushButton->setEnabled(false);
	}

//lower right direction
	if(_optDir.hasPtLowerRight)
	{
		HidroMetadata metadata(_params->getCurrentDatabasePtr());
		std::string vertexDestiny = metadata.getVertexName(_graphName, _optDir.ptLowerRight.objectId());

		if(!vertexDestiny.empty())
		{
			bool status = false;

			bool hasOutputEdges = false;
			if(optEdition.hasEdge(_currentPoint, vertexDestiny, hasOutputEdges))
			{
				if(hasOutputEdges)
				{
					downRightPushButton->setPixmap(QPixmap::fromMimeSource("arrow-downright_yellow.bmp"));
					status = true;
				}
			}

			bool hasInputEdges = false;
			if(optEdition.hasEdge(vertexDestiny, _currentPoint, hasInputEdges))
			{
				if(hasInputEdges)
				{
					downRightPushButton->setPixmap(QPixmap::fromMimeSource("arrow-downright_red.bmp"));
					status = true;
				}
			}

			if(!status)
			{
				downRightPushButton->setPixmap(QPixmap::fromMimeSource("arrow-downright_green.bmp"));
			}

			downRightPushButton->setEnabled(true);	
		}
	}
	else
	{
		downRightPushButton->setPixmap(QPixmap::fromMimeSource(""));
		downRightPushButton->setEnabled(false);
	}

//right direction
	if(_optDir.hasPtRight)
	{
		HidroMetadata metadata(_params->getCurrentDatabasePtr());
		std::string vertexDestiny = metadata.getVertexName(_graphName, _optDir.ptRight.objectId());

		if(!vertexDestiny.empty())
		{
			bool status = false;

			bool hasOutputEdges = false;
			if(optEdition.hasEdge(_currentPoint, vertexDestiny, hasOutputEdges))
			{
				if(hasOutputEdges)
				{
					rightPushButton->setPixmap(QPixmap::fromMimeSource("arrow-right_yellow.bmp"));
					status = true;
				}
			}

			bool hasInputEdges = false;
			if(optEdition.hasEdge(vertexDestiny, _currentPoint, hasInputEdges))
			{
				if(hasInputEdges)
				{
					rightPushButton->setPixmap(QPixmap::fromMimeSource("arrow-right_red.bmp"));
					status = true;
				}
			}

			if(!status)
			{
				rightPushButton->setPixmap(QPixmap::fromMimeSource("arrow-right_green.bmp"));
			}

			rightPushButton->setEnabled(true);	
		}
	}
	else
	{
		rightPushButton->setPixmap(QPixmap::fromMimeSource(""));
		rightPushButton->setEnabled(false);
	}

//left direction
	if(_optDir.hasPtLeft)
	{
		HidroMetadata metadata(_params->getCurrentDatabasePtr());
		std::string vertexDestiny = metadata.getVertexName(_graphName, _optDir.ptLeft.objectId());

		if(!vertexDestiny.empty())
		{
			bool status = false;

			bool hasOutputEdges = false;
			if(optEdition.hasEdge(_currentPoint, vertexDestiny, hasOutputEdges))
			{
				if(hasOutputEdges)
				{
					leftPushButton->setPixmap(QPixmap::fromMimeSource("arrow-left_yellow.bmp"));
					status = true;
				}
			}

			bool hasInputEdges = false;
			if(optEdition.hasEdge(vertexDestiny, _currentPoint, hasInputEdges))
			{
				if(hasInputEdges)
				{
					leftPushButton->setPixmap(QPixmap::fromMimeSource("arrow-left_red.bmp"));
					status = true;
				}
			}

			if(!status)
			{
				leftPushButton->setPixmap(QPixmap::fromMimeSource("arrow-left_green.bmp"));
			}

			leftPushButton->setEnabled(true);	
		}
	}
	else
	{
		leftPushButton->setPixmap(QPixmap::fromMimeSource(""));
		leftPushButton->setEnabled(false);
	}
 }

void HidroOptimizedEditionWindow::disableDirections()
{
	upPushButton->setEnabled(false);
	upRightPushButton->setEnabled(false);
	upLeftPushButton->setEnabled(false);
	downPushButton->setEnabled(false);
	downRightPushButton->setEnabled(false);
	downLeftPushButton->setEnabled(false);
	rightPushButton->setEnabled(false);
	leftPushButton->setEnabled(false);
}

void HidroOptimizedEditionWindow::upPushButton_clicked()
{
    HidroMetadata metadata(_params->getCurrentDatabasePtr());
	std::string name = metadata.getVertexName(_graphName, _optDir.ptUpper.objectId());

	if(!name.empty())
	{
		resTextLabel->setPaletteForegroundColor(QColor(0, 0, 0));
		resTextLabel->setText("PROCESSING...");

		createObject(name);

		checkDirections();
	}
}


void HidroOptimizedEditionWindow::rightPushButton_clicked()
{
	HidroMetadata metadata(_params->getCurrentDatabasePtr());
	std::string name = metadata.getVertexName(_graphName, _optDir.ptRight.objectId());

	if(!name.empty())
	{
		resTextLabel->setPaletteForegroundColor(QColor(0, 0, 0));
		resTextLabel->setText("PROCESSING...");

		createObject(name);

		checkDirections();
	}
}


void HidroOptimizedEditionWindow::downPushButton_clicked()
{
	HidroMetadata metadata(_params->getCurrentDatabasePtr());
	std::string name = metadata.getVertexName(_graphName, _optDir.ptLower.objectId());

	if(!name.empty())
	{
		resTextLabel->setPaletteForegroundColor(QColor(0, 0, 0));
		resTextLabel->setText("PROCESSING...");

		createObject(name);

		checkDirections();
	}
}


void HidroOptimizedEditionWindow::leftPushButton_clicked()
{
    HidroMetadata metadata(_params->getCurrentDatabasePtr());
	std::string name = metadata.getVertexName(_graphName, _optDir.ptLeft.objectId());

	if(!name.empty())
	{
		resTextLabel->setPaletteForegroundColor(QColor(0, 0, 0));
		resTextLabel->setText("PROCESSING...");

		createObject(name);

		checkDirections();
	}
}

void HidroOptimizedEditionWindow::upRightPushButton_clicked()
{
    HidroMetadata metadata(_params->getCurrentDatabasePtr());
	std::string name = metadata.getVertexName(_graphName, _optDir.ptUpperRight.objectId());

	if(!name.empty())
	{
		resTextLabel->setPaletteForegroundColor(QColor(0, 0, 0));
		resTextLabel->setText("PROCESSING...");

		createObject(name);

		checkDirections();
	}
}


void HidroOptimizedEditionWindow::downRightPushButton_clicked()
{
    HidroMetadata metadata(_params->getCurrentDatabasePtr());
	std::string name = metadata.getVertexName(_graphName, _optDir.ptLowerRight.objectId());

	if(!name.empty())
	{
		resTextLabel->setPaletteForegroundColor(QColor(0, 0, 0));
		resTextLabel->setText("PROCESSING...");

		createObject(name);

		checkDirections();
	}
}


void HidroOptimizedEditionWindow::downLeftPushButton_clicked()
{
    HidroMetadata metadata(_params->getCurrentDatabasePtr());
	std::string name = metadata.getVertexName(_graphName, _optDir.ptLowerLeft.objectId());

	if(!name.empty())
	{
		resTextLabel->setPaletteForegroundColor(QColor(0, 0, 0));
		resTextLabel->setText("PROCESSING...");

		createObject(name);

		checkDirections();
	}
}


void HidroOptimizedEditionWindow::upLeftPushButton_clicked()
{
    HidroMetadata metadata(_params->getCurrentDatabasePtr());
	std::string name = metadata.getVertexName(_graphName, _optDir.ptUpperLeft.objectId());

	if(!name.empty())
	{
		resTextLabel->setPaletteForegroundColor(QColor(0, 0, 0));
		resTextLabel->setText("PROCESSING...");

		createObject(name);

		checkDirections();
	}
}

/** \brief HidroOptimizedEditionWindow getMouseClicked function.
    * Function used to get the mouse clicked over the canvas.
	* This signal is emit from application, so we have to connect this
	* signal with this function.
	* \param coord		Input parameter, used to define the geo coord from click (this coord is in canvas projection)
	* \param state		Input parameter, used to indicate the mouse button state
	* \param point		Input parameter, used to define the canvas coord from click
    */
void HidroOptimizedEditionWindow::getMouseClicked( TeCoord2D & coord, int /*state*/, QPoint & /*point*/ )
{
	if(_graphName.empty() || _baseName.empty())
	{
		return;
	}

	HidroUtils utils(_params->getCurrentDatabasePtr());

	TeTheme* currentTheme		= (TeTheme*)_params->getCurrentThemeAppPtr()->getTheme();
	TeProjection* projCanvas	= _params->teqtcanvas_ptr_->projection();
	double pixelSize			= _params->teqtcanvas_ptr_->pixelSize();

//get the name of pointed vertex
	_currentPoint = utils.getPointName(_graphName, currentTheme, projCanvas, pixelSize, coord);

	_currentPointCoord = coord;
	
	objSelectedLineEdit->setText(_currentPoint.c_str());

	resTextLabel->setText("");

	if(_currentPoint.empty())
	{
		disableDirections();
		return;
	}

	checkDirections();
}

void HidroOptimizedEditionWindow::createObject( string objName )
{
	if(objName.empty())
	{
		return;
	}

	if(_currentPoint.empty())
	{
		return;
	}

//define a new name for the new object
	_newName = _baseName + "_" +Te2String(_count);

	TeTheme* vertexTheme = dynamic_cast<TeTheme*>(_vertexTheme->getTheme());
	TeTheme* edgeTheme	 = dynamic_cast<TeTheme*>(_edgeTheme->getTheme());

	_params->setCurrentThemeAppPtr(_edgeTheme);

	HidroOptimizedEdition optEdition(_params->getCurrentDatabasePtr(), _graphName);

	if(!optEdition.createEdge(edgeTheme, vertexTheme, _params->teqtgrid_ptr_, _currentPoint, objName, _newName, _createAttr))
	{
		resTextLabel->setPaletteForegroundColor(QColor(255, 0, 0));
		resTextLabel->setText("FAILED");
	}
	else
	{
		resTextLabel->setPaletteForegroundColor(QColor(0, 255, 0));
		resTextLabel->setText("SUCCESSFULLY");

		++_count;
	}
	
	_params->plotTVData();

	_params->setCurrentThemeAppPtr(_vertexTheme);
}

void HidroOptimizedEditionWindow::closeEvent( QCloseEvent *ce )
{
	disconnect(_params->teqtcanvas_ptr_, SIGNAL(mousePressed(TeCoord2D&, int, QPoint&)), this, SLOT(getMouseClicked(TeCoord2D&, int, QPoint&)));
	
	ce->accept();
}
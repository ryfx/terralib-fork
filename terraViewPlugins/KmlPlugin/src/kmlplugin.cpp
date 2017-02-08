#include <kmlplugin.h>

#include <ImportKmlWindow.h>
#include <ExportKmlWindow.h>
#include <KmlThemeWindow.h>
#include <KmlPopupWidget.h>

#include <qaction.h>
#include <qmessagebox.h>
#include <qtoolbar.h>
#include <qpainter.h>
#include <qinputdialog.h>
#include <qdir.h>
#include <qcursor.h>

#include <TeKmlTheme.h>

#include <TeQtThemeItem.h>
#include <TeQtCanvas.h>
#include <TeQtGrid.h>

#include <KmlIconRequest.h>

#include <PluginsSignal.h>

#include <kmlmanager.h>
#include <kmlinterpreter.h>

#include <TeSpatialOperations.h>

#include <KmlInformationBalloon.h>

KmlPlugin::KmlPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("kmlplugin_");

	comm_.setHostObj( this, signalHandler );

	_balloon = 0;
}

KmlPlugin::~KmlPlugin()
{
	delete _balloon;

	end();
}

void KmlPlugin::init()
{
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			_importAction = new QAction(0, "ImportAction", false);
			_importAction->setMenuText(tr("Import KML..."));
			_importAction->setText( tr("Import KML") );
			_importAction->setToolTip( tr("Import KML") );
			_importAction->setIconSet(QPixmap::fromMimeSource("ImportKml.bmp"));

			_exportAction = new QAction(0, "ExportAction", false);
			_exportAction->setMenuText(tr("Export KML..."));
			_exportAction->setText( tr("Export KML") );
			_exportAction->setToolTip( tr("Export KML") );
			_exportAction->setIconSet(QPixmap::fromMimeSource("ExportKml.bmp"));
			
			_kmlThemeAction = new QAction(0, "KmlThemeAction", false);
			_kmlThemeAction->setText( tr("Create KML Theme...") ); 
			_kmlThemeAction->setToolTip( tr("Create KML Theme") );
			_kmlThemeAction->setIconSet(QPixmap::fromMimeSource("KmlTheme.bmp"));			
						
			QPopupMenu* mnu = getPluginsMenu("TerraViewPlugin.KML Manager");
			
			if(mnu != 0)
			{

				if(!_importAction->addTo(mnu))
					throw;
				if(!_exportAction->addTo(mnu))
					throw;
				if(!_kmlThemeAction->addTo(mnu))
					throw;
			}

			QPtrList< QToolBar > tv_tool_bars_list = tview->toolBars( Qt::DockTop );

			if( tv_tool_bars_list.count() > 0 )
			{
				QToolBar* tToolBar = dynamic_cast<QToolBar*>(tview->toolBars(Qt::DockTop).at(0));
							
				tToolBar->addSeparator();
				_importAction->addTo(tToolBar);
				_exportAction->addTo(tToolBar);
				_kmlThemeAction->addTo(tToolBar);
				tToolBar->addSeparator();
				
			}				
		
			connect(_importAction, SIGNAL(activated()), this, SLOT(showImportWindow()));
			connect(_exportAction, SIGNAL(activated()), this, SLOT(showExportWindow()));
			connect(_kmlThemeAction, SIGNAL(activated()), this, SLOT(showKmlThemeWindow()));
		}
	}
	catch(...)
	{
		QMessageBox::critical(tview, tr("TerraView plug-in error"), tr("Can't create plug-in menu."));

		delete _exportAction;
		delete _importAction;
		delete _kmlThemeAction;
		_importAction = 0;
		_exportAction = 0;
		_kmlThemeAction = 0;
	}
}

void KmlPlugin::end()
{
	delete _exportAction;
	delete _importAction;
	delete _kmlThemeAction;
}

void KmlPlugin::showImportWindow()
{
	TerraViewBase* tview = getMainWindow();
		
	TeDatabase* db = params_->getCurrentDatabasePtr();
	
	if(db == 0)
	{
		return;
	}	
	
	QMainWindow* mainWin = getMainWindow();
			
	ImportKmlWindow imp(mainWin, 0, true);

	imp.init(db, tview);	

	if(imp.exec() == QDialog::Accepted)
	{
			
	}
}

void KmlPlugin::showExportWindow()
{
		
	TeDatabase* db = params_->getCurrentDatabasePtr();
	
	if(db == 0)
	{		
		return;
	}
	if(db->themeMap().empty())
	{
		QMessageBox::warning(0, tr("Warning"), tr("You do not have any theme!"));
		return;
	}

	int count = 0;
	TeLayerMap layerMap = db->layerMap();
	TeLayerMap::iterator it = layerMap.begin();

	while(it != layerMap.end())
	{
		if(!it->second->hasGeometry(TeRASTER) && !it->second->hasGeometry(TeRASTERFILE))
		{
			if(it->second->hasGeometry(TePOLYGONS) || it->second->hasGeometry(TeLINES) || it->second->hasGeometry(TePOINTS))
			{
				count++;
			}
		}
		
		++it;
	}

	if(count < 1)
	{
		QMessageBox::warning(0, tr("Warning"), tr("You do not have valid information to export!"));
		return;
	}
	
	QMainWindow* mainWin = getMainWindow();
			
	ExportKmlWindow exp(mainWin, 0, true);

	exp.init(db);	

	if(exp.exec() == QDialog::Accepted)
	{
		
	}
}


void KmlPlugin::showKmlThemeWindow()
{		
		
	TerraViewBase* tview = getMainWindow();
	TeDatabase* db = params_->getCurrentDatabasePtr();
	
	if(db == 0)
	{		
		return;
	}		
	if(db->viewMap().empty())
	{
		QMessageBox::warning(0, tr("Warning"), tr("You need a view to create a Kml Theme!"));
		return;
	}
	
	QMainWindow* mainWin = getMainWindow();
			
	KmlThemeWindow kml(mainWin, 0, true);
	
	kml.init(db, tview);	

	if(kml.exec() == QDialog::Accepted)
	{
			
	}
	

}

void KmlPlugin::signalHandler( const PluginsSignal& sig, void* receiver)
{
	((KmlPlugin*)receiver)->signalHandler(sig);
}

void KmlPlugin::signalHandler( const PluginsSignal& x)
{
	if (x.signalType_ == PluginsSignal::S_DRAW_THEME)
		drawEventHandler(x);
	else if (x.signalType_ == PluginsSignal::S_POINT_QUERY)
		pointingEventHandler(x);
	else if (x.signalType_ == PluginsSignal::S_SHOW_THEME_POPUP)
		showThemePopup(x);	
	else if(x.signalType_ == PluginsSignal::S_SET_QTTHEMEICON)
		setThemeIcon(x);
	else if(x.signalType_ == PluginsSignal::S_POINT_QUERY)
		pointingEventHandler(x);	
}

void KmlPlugin::drawEventHandler(const PluginsSignal& x )
{
	TeAbstractTheme* theme = 0;
	if(!x.signalData_.retrive(PluginsSignal::D_ABSTRACT_THEME, theme))
		return;
	
	if(theme->type() != TeKMLTHEME)
		return;

	TeKmlTheme* kmlTheme = dynamic_cast<TeKmlTheme*>(theme);
	if(!kmlTheme)
		return;
	
	/* retrieve canvas */
	TeQtCanvas* canvas = 0;
	if(!x.signalData_.retrive(PluginsSignal::D_QT_CANVAS, canvas))
		return;

	TePointSet pointSet;
	TeLineSet lineSet;
	TePolygonSet polygonSet;
	TeTable attrTable;
	TeTable styleTable;
	
	kmlTheme->getData(pointSet, lineSet, polygonSet, attrTable, styleTable);	
		
	//Contador de tentativas mal sucedidas de requisição de imagem
	int count = 0;

	for(unsigned int i = 0; i < kmlTheme->getPolygonSet().size(); i++)
	{				
		for(unsigned int j = 0; j < attrTable.size(); j++)
		{
			if(atoi(attrTable(j, 0).c_str()) == i)
			{
				canvas->setPolygonColor(atoi(styleTable(i, 2).c_str()), atoi(styleTable(i, 3).c_str()), atoi(styleTable(i, 4).c_str()));
				canvas->setPolygonStyle(1, atoi(styleTable(i, 1).c_str()));
				canvas->setPolygonLineColor(atoi(styleTable(i, 6).c_str()), atoi(styleTable(i, 7).c_str()), atoi(styleTable(i, 8).c_str()));
				canvas->setPolygonLineStyle(0, atoi(styleTable(i, 9).c_str()));
				break;
			}
		}	

		canvas->plotPolygon(kmlTheme->getPolygonSet()[i]);		
	}

	for(unsigned int i = 0; i < kmlTheme->getLineSet().size(); i++)
	{	
		for(unsigned int j = 0; j < attrTable.size(); j++)
		{
			if(atoi(attrTable(j, 0).c_str()) == i)
			{
				canvas->setLineColor(atoi(styleTable(i, 6).c_str()), atoi(styleTable(i, 7).c_str()), atoi(styleTable(i, 8).c_str()));
				canvas->setLineStyle(0, atoi(styleTable(i, 9).c_str()));
				break;
			}
		}	

		canvas->plotLine(kmlTheme->getLineSet()[i]);		
	}

	for(unsigned int i = 0; i < kmlTheme->getPointSet().size(); i++)
	{
		for(unsigned int j = 0; j < attrTable.size(); j++)
		{
			if(atoi(attrTable(j, 0).c_str()) == i)
			{
				KmlIconRequest* iconRequest = new KmlIconRequest();
				QImage img;
				img = _images[styleTable(i, 10)];
				
				
				if(styleTable(i, 10) == "")
					count = 0;

				//Para evitar demora, se a requisição de imagens não tiver sucesso duas vezes srguidas, o sistema não requisitará mais e desenhara a imagem padrão
				if(img == 0 && count < 2)
				{
					_images[styleTable(i, 10)] = iconRequest->requestImage(styleTable(i, 10));
					img = _images[styleTable(i, 10)];
				}
				
				canvas->setPointColor(0, 0, 0);
				canvas->setPointStyle(2, 3);
					
				if(img != 0)
				{
					count = 0;
					TeCoord2D imgCoord;
					imgCoord.setXY(kmlTheme->getPointSet()[i].box().x1(), kmlTheme->getPointSet()[i].box().y1());

					QPoint q = canvas->mapCWtoV(imgCoord);
					
					QPainter* painter = canvas->getPainter();
					
					//Img Scale control
					
					QImage imgResizable;
					int scale = (int)(atof(styleTable(i, 13).c_str())*32);
					if(scale > 0)	
					{						
						if(img.height() == img.width())
							imgResizable = img.smoothScale(scale, scale);
						else if(img.height() > img.width())
							imgResizable = img.smoothScale(scale, (scale*img.height()/img.width()));						
						else
							imgResizable = img.smoothScale(scale, (scale*img.width()/img.height()));
					}
					else
						imgResizable = img;

					//HotSpot control
					if((atoi(styleTable(i, 11).c_str())) == 0 && (atoi(styleTable(i, 12).c_str())) == 0)
					{						
						q.setX(q.x()-imgResizable.width()/2-1);
						q.setY(q.y()-imgResizable.height()/2);
					}
					else
					{
						int hotW = (int)(imgResizable.width()*atoi(styleTable(i, 11).c_str()))/img.width();
						int hotH = (int)(imgResizable.height()*atoi(styleTable(i, 12).c_str()))/img.height();

						q.setX(q.x()-hotW);
						q.setY(q.y()-imgResizable.height()+hotH);				
					}					
					
					painter->drawImage(q, imgResizable);
					
				}
				else
				{
					img = QPixmap::fromMimeSource("Point.png");
					TeCoord2D imgCoord;
					imgCoord.setXY(kmlTheme->getPointSet()[i].box().x1(), kmlTheme->getPointSet()[i].box().y1());

					QPoint q = canvas->mapCWtoV(imgCoord);
					
					QPainter* painter = canvas->getPainter();

					img = img.smoothScale(41, 41);

					q.setX(q.x()-12);
					q.setY(q.y()-img.height()+1);

					painter->drawImage(q, img);
					count++;

				}

				break;
			}			
		}

		canvas->plotPoint(kmlTheme->getPointSet()[i]);
				
		TeCoord2D textCoord;		
		textCoord.setXY(kmlTheme->getPointSet()[i].box().x1(), kmlTheme->getPointSet()[i].box().y1());
		QPoint txPoint = canvas->mapCWtoV(textCoord);
		txPoint.setX(txPoint.x()+kmlTheme->getPointSet()[i].size()+15);
		txPoint.setY(txPoint.y()+15);
		textCoord = canvas->mapVtoCW(txPoint);
		canvas->plotText(textCoord, attrTable(i, 1));
	}

	canvas->copyPixmapToWindow();
	
}

void KmlPlugin::pointingEventHandler(const PluginsSignal& x )
{	
	QPoint cursorLocation = QCursor::pos();
	
	TeAbstractTheme* theme = 0;
	if(!x.signalData_.retrive(PluginsSignal::D_ABSTRACT_THEME, theme))
		return;
	
	if(theme->type() != TeKMLTHEME)
		return;

	TeKmlTheme* kmlTheme = dynamic_cast<TeKmlTheme*>(theme);
	if(!kmlTheme)
		return;


	TeQtCanvas* canvas = 0;
	if(!x.signalData_.retrive(PluginsSignal::D_QT_CANVAS, canvas))
		return;

	
	TeCoord2D pt;
	if(!x.signalData_.retrive(PluginsSignal::D_COORD2D, pt))
		return;

	if(_balloon != 0)
		delete _balloon;
	
	_balloon = new KmlInformationBalloon(0, "Balloon");
	

	std::map<std::string, int> geomPointeds;
	QStringList pointedList;
	int count = 0;
	std::string infoName;

	for(unsigned int i = 0; i < kmlTheme->getPolygonSet().size(); i++)
	{			
		if(TeWithin(pt, kmlTheme->getPolygonSet()[i]))
		{
			for(unsigned int j = 0; j < kmlTheme->getAttrTable().size(); j++)
			{			
				if(atoi(kmlTheme->getAttrTable()(j, 0).c_str()) == i)
				{
					pointedList.append(kmlTheme->getAttrTable()(j, 1).c_str());
					geomPointeds[kmlTheme->getAttrTable()(j, 1).c_str()] = atoi(kmlTheme->getAttrTable()(j, 0).c_str());
					infoName = kmlTheme->getAttrTable()(j, 1);
					count++;
				}
			}
		}		
		
	}
	for(unsigned int i = 0; i < kmlTheme->getPointSet().size(); i++)
	{			

		TeCoord2D point;
		point.setXY(kmlTheme->getPointSet()[i].box().x1(), kmlTheme->getPointSet()[i].box().y1());
		QPoint viewP = canvas->mapCWtoV(point);
				
		//Aplicando uma magem de erro para o click
		TeBox viewBox;
		viewBox.x1_ = viewP.x()-15;
		viewBox.y1_ = viewP.y()-15;
		viewBox.x2_ = viewP.x()+15;
		viewBox.y2_ = viewP.y()+15;

		canvas->mapVtoCW(viewBox);

		if(TeWithin(pt, viewBox))
		{
			for(unsigned int j = 0; j < kmlTheme->getAttrTable().size(); j++)
			{			
				if(atoi(kmlTheme->getAttrTable()(j, 0).c_str()) == i)
				{
					pointedList.append(kmlTheme->getAttrTable()(j, 1).c_str());
					geomPointeds[kmlTheme->getAttrTable()(j, 1).c_str()] = atoi(kmlTheme->getAttrTable()(j, 0).c_str());					
					infoName = kmlTheme->getAttrTable()(j, 1);
					count++;
				}
			}
		}
		
	}
	for(unsigned int i = 0; i < kmlTheme->getLineSet().size(); i++)
	{
		TeCoord2D point = pt;
		QPoint viewP = canvas->mapCWtoV(point);
				
		TeBox viewBox;
		viewBox.x1_ = viewP.x()-15;
		viewBox.y1_ = viewP.y()-15;
		viewBox.x2_ = viewP.x()+15;
		viewBox.y2_ = viewP.y()+15;	
		
		canvas->mapVtoCW(viewBox);
		
		TeLinearRing ring;
		ring.add(TeCoord2D(viewBox.x1(), viewBox.y1()));
		ring.add(TeCoord2D(viewBox.x1(), viewBox.y2()));
		ring.add(TeCoord2D(viewBox.x2(), viewBox.y2()));
		ring.add(TeCoord2D(viewBox.x2(), viewBox.y1()));
		ring.add(TeCoord2D(viewBox.x1(), viewBox.y1()));

		TePolygon polygon;
		polygon.add(ring);

		for(int j = 0; j < kmlTheme->getLineSet()[i].size(); j++)
		{
			bool inter1 = false;
			bool inter2 = false;
			
			inter1 = TeIntersects(kmlTheme->getLineSet()[i][j], polygon);
			inter2 = TeWithin(kmlTheme->getLineSet()[i][j], polygon);

			if(inter1 || inter2)
			{				
				for(unsigned int j = 0; j < kmlTheme->getAttrTable().size(); j++)
				{			
					if(atoi(kmlTheme->getAttrTable()(j, 0).c_str()) == i)
					{
						pointedList.append(kmlTheme->getAttrTable()(j, 1).c_str());
						geomPointeds[kmlTheme->getAttrTable()(j, 1).c_str()] = atoi(kmlTheme->getAttrTable()(j, 0).c_str());						
						infoName = kmlTheme->getAttrTable()(j, 1);
						count++;
					}
				}
				break;
			}
			
		}	
		
	}

	

	//Caso exista mais de uma geometria apontada
	if(count == 1)
	{
		if(hasHtml(kmlTheme->getAttrTable()(geomPointeds[infoName], 2)))
		{			
			createTempFile(kmlTheme->getAttrTable()(geomPointeds[infoName], 2).c_str());
			callBrowser();
			
		}
		else
		{						
			_balloon->init(canvas->size(), cursorLocation, kmlTheme->getAttrTable()(geomPointeds[infoName], 1), kmlTheme->getAttrTable()(geomPointeds[infoName], 2));
			_balloon->show();			
		}
	}
	else
	{
		if(!pointedList.empty())
		{			
			bool okClicked =  false;
			QString qItem = QInputDialog::getItem("Kml Plugin", tr("Select the item:"), pointedList, 0, false, &okClicked);

			if(okClicked)
			{
				if(hasHtml(kmlTheme->getAttrTable()(geomPointeds[qItem.latin1()], 2)))
				{					
					createTempFile(kmlTheme->getAttrTable()(geomPointeds[qItem.latin1()], 2));
					callBrowser();
				}
				else
				{
					_balloon->init(canvas->size(), cursorLocation, kmlTheme->getAttrTable()(geomPointeds[qItem.latin1()], 1), kmlTheme->getAttrTable()(geomPointeds[qItem.latin1()], 2));
					_balloon->show();					
				}
			}
		}		
	}

}

void KmlPlugin::setThemeIcon(const PluginsSignal &x)
{
	/* retrieve Qt theme */
	TeQtThemeItem* qtThemeItem = 0;

	if(!x.signalData_.retrive(PluginsSignal::D_QT_THEMEITEM, qtThemeItem))
	{
		QMessageBox::critical(0, tr("Error"), tr("The signal received is not about a Qt Theme Item!"));
		return;
	}

	if(!qtThemeItem)
	{
		QMessageBox::critical(0, tr("Error"), tr("Didn't receive a valid Qt Theme Item!"));
		return;
	}

	if(qtThemeItem->getAppTheme()->getTheme()->type() == TeKMLTHEME)
	{
		string fileName = ((TeKmlTheme*)qtThemeItem->getAppTheme()->getTheme())->getFileName();
		bool fileExist = TeCheckFileExistence(fileName);

		if(fileExist)
		{
			qtThemeItem->setPixmap(0, QPixmap::fromMimeSource("KmlTheme.bmp"));
		}
		else
		{
			qtThemeItem->setPixmap(0, QPixmap::fromMimeSource("KmlThemeInvalid.bmp"));
			qtThemeItem->setThemeItemAsInvalid();
		}

	}
	


        
}

void KmlPlugin::showThemePopup(const PluginsSignal& x )
{

	int themeType = 0;

	if(!x.signalData_.retrive(PluginsSignal::D_THEMETYPE, themeType))
	{		
		return;
	}

	if(themeType != TeKMLTHEME)
		return;
	
	TeQtThemeItem* qtThemeItem = 0;

	if(!x.signalData_.retrive(PluginsSignal::D_QT_THEMEITEM, qtThemeItem))
	{
		QMessageBox::critical(0, tr("Error"), tr("The signal received is not about a Qt Theme Item!"));
		return;
	}

	if(!qtThemeItem)
	{
		QMessageBox::critical(0, tr("Error"), tr("Didn't receive a valid Qt Theme Item!"));
		return;
	}	

	TeCoord2D location;

	if(!x.signalData_.retrive(PluginsSignal::D_COORD2D, location))
	{
		QMessageBox::critical(0, tr("Error"), tr("The signal received is not about a location!"));
		return;
	}
	
	KmlPopupWidget* popupmenu = new KmlPopupWidget(0, "", true);
	popupmenu->init(getMainWindow(), qtThemeItem, static_cast<int>(location.x()), static_cast<int>(location.y()));
	delete popupmenu;


}

bool KmlPlugin::hasHtml(const std::string & description)
{
	QString desc = description.c_str();
	
	if(desc.find("<html>", 0, false) == 0 || 
		desc.find("<body", 0, false) || 
		desc.find("<script", 0, false) == 0)
		return true;
	else
		return false;
		
}

void KmlPlugin::callBrowser()
{
/*
	LPCSTR dName = "";
	LPCSTR fName = "infoIcon_temp.html";
		
	SHELLEXECUTEINFO shExecInfo;
	shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shExecInfo.fMask = NULL;
	shExecInfo.hwnd = NULL;
	shExecInfo.lpVerb = "open";
	shExecInfo.lpFile = fName;
	shExecInfo.lpParameters = "/Open";
	shExecInfo.lpDirectory = dName;
	shExecInfo.nShow = SW_SHOWNORMAL;
	shExecInfo.hInstApp = NULL;

	ShellExecuteEx(&shExecInfo);
*/
}

void KmlPlugin::createTempFile(std::string code)
{
	QFileInfo fInfo("infoIcon_temp.html");

	QFile file(fInfo.filePath());

	QStringList lines(code.c_str());
					
	if (!file.open(IO_WriteOnly| IO_Translate))
		return;

	QTextStream stream( &file );
	for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
		stream << *it << "\n";
	file.close();
	file.flush();
}

//Terralib Includes
#include <TeDatabase.h>
#include <TeDataTypes.h>
#include <TeKmlTheme.h>

#include <KmlPluginUtils.h>

//TerraView Includes
#include <terraViewBase.h>
#include <TeQtViewsListView.h>

//QT Includes
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qfiledialog.h>

#include <TeQtCanvas.h>
#include <TeQtThemeItem.h>
#include <TeQtViewItem.h>

void KmlThemeWindow::filePushButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName("", tr("KML Files (*.kml;*.KML)"), this, tr("Open KML File"), tr("Choose a file"));

	if(fileName.isEmpty())
		return;

	std::string name = TeGetBaseName(fileName);
	
	if(!KmlPluginUtils::correctName(name))
		themeNameLineEdit->setText("");
	else
		themeNameLineEdit->setText(name.c_str());		
		
	fileLineEdit->setText(fileName);	

}


void KmlThemeWindow::createThemePushButton_clicked()
{		
	setCursor(Qt::waitCursor);

	TeView* currentView = KmlPluginUtils::getView(_database, string(viewComboBox->currentText().latin1()));
	if(!currentView)
		return;
	
	std::string fileName = fileLineEdit->text().latin1();
	std::string themeName = themeNameLineEdit->text().latin1();


	//Checking texts
	
	//Check File Name
	if(fileName.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Choose a file to create theme!"));
		setCursor(Qt::ArrowCursor);
		return;
	}

	std::string extension = TeGetExtension(fileName.c_str());

	if(extension != "kml" && extension != "KML")
	{
		QMessageBox::warning(this, tr("Warning"), tr("The file is not a kml file!"));
		setCursor(Qt::ArrowCursor);
		return;
	}

	//Check Theme Name
	if(themeName.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Choose a name for the Theme!"));
		setCursor(Qt::ArrowCursor);
		return;
	}	
	if(!KmlPluginUtils::correctName(themeName))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Theme name invalid!"));
		setCursor(Qt::ArrowCursor);
		return;
	}
	if(themeName.length() > 30)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please choose a theme name under 30 characters!"));
		setCursor(Qt::ArrowCursor);
		return;
	}
	if(hasThemeWithName(themeName, currentView))
	{
		QMessageBox::warning(this, tr("Warning"), tr("There is already a theme with this name!"));
		setCursor(Qt::ArrowCursor);
		return;
	}

	//End Checking texts
	
	
	//Theme Projection
	TeDatum pDatum = TeDatumFactory::make("WGS84");
	TeProjection* proj = new TeLatLong(pDatum);
	
	
	//Creating Theme
	_kmlTheme = new TeKmlTheme(fileName, 0, currentView->id(), 0);

	_kmlTheme->setThemeProjection(proj);
	_kmlTheme->setFileName(fileName);
		

	//Getting Theme Informations
	TePointSet pointSet;
	TeLineSet lineSet;
	TePolygonSet polygonSet;
	TeTable attrTable;
	TeTable styleTable;
	
	if(!_kmlTheme->getData(pointSet, lineSet, polygonSet, attrTable, styleTable))
	{
		QString msg = tr("Error inserting theme!") + "\n";
		QMessageBox::critical(this, tr("Error"), msg);
		delete _kmlTheme;
		setCursor(Qt::ArrowCursor);
		return;
	}
	//End Getting Theme Informations

	//Setting Theme Box
	TeBox themeBox;
	if(pointSet.box().isValid())
	{
		updateBox(themeBox, pointSet.box());
	}
	if(lineSet.box().isValid())
	{
		updateBox(themeBox, lineSet.box());
	}
	if(polygonSet.box().isValid())
	{
		updateBox(themeBox, polygonSet.box());
	}
	if(themeBox.x1() == themeBox.x2())
	{
		themeBox.x1_ -= 0.0000001;
		themeBox.x2_ += 0.0000001;
	}
	if(themeBox.y1() == themeBox.y2())
	{
		themeBox.y1_ -= 0.0000001;
		themeBox.y2_ += 0.0000001;
	}
	//End Setting Theme Box

	//Getting Theme Representations
	int rep = 0;
	if(!pointSet.empty())
		rep += TePOINTS;
	if(!lineSet.empty())
		rep += TeLINES;
	if(!polygonSet.empty())
		rep += TePOLYGONS;
	//End Getting Theme Representations

	_kmlTheme->visibleRep(rep);	
	_kmlTheme->createKmlThemeTable(_database);
	_kmlTheme->view(currentView->id());
	_kmlTheme->setLocalDatabase(_database);
	_kmlTheme->setThemeBox(themeBox);
	_kmlTheme->name(themeName);	
	_kmlTheme->parentId(_kmlTheme->id());
	_kmlTheme->setAttrTable(attrTable);
	_kmlTheme->setPointSet(pointSet);
	_kmlTheme->setPolygonSet(polygonSet);
	_kmlTheme->setLineSet(lineSet);

	if (!_kmlTheme->save())
	{
		QString msg = tr("Error inserting theme!") + "\n";
		QMessageBox::critical(this, tr("Error"), msg);
		delete _kmlTheme;
		setCursor(Qt::ArrowCursor);
		return;
	}

	currentView->add(_kmlTheme);		
	if(currentView->size() == 1)
	{
		delete currentView->projection();
		TeProjection *proj = _kmlTheme->getThemeProjection();
		currentView->projection(proj);
		currentView->setCurrentBox(themeBox);
		_database->updateProjection(proj);
		_database->updateView(currentView);
	}

	_database->updateTheme(_kmlTheme);

	TeQtViewItem* viewItem = _tViewBase->getViewsListView()->getViewItem(currentView);
		
	TeAppTheme* appTheme = new TeAppTheme(_kmlTheme);
	TeQtThemeItem* themeItem = new TeQtThemeItem(viewItem, _kmlTheme->name().c_str(), appTheme);
	themeItem->setPixmap(0, QPixmap::fromMimeSource("KmlTheme.bmp"));
		
	setCursor(Qt::ArrowCursor);

	QMessageBox::warning(this, tr("Warning"), tr("Theme created successfully!"));
	accept();
}


void KmlThemeWindow::closePushButton_clicked()
{
	close();
}


void KmlThemeWindow::init( TeDatabase * database, TerraViewBase* tViewBase )
{
	_database = database;
	_tViewBase = tViewBase;
	_kmlTheme = 0;	

	loadViews();
}


void KmlThemeWindow::loadViews()
{
	std::vector<TeView*> views = KmlPluginUtils::getViews(_database);

	if(views.empty())
		return;
	
	for(unsigned int i = 0; i < views.size(); i++)
	{
		viewComboBox->insertItem(views[i]->name().c_str());
	}	

	if(_tViewBase->currentView())
		viewComboBox->setCurrentText(_tViewBase->currentView()->name().c_str());
}	



void KmlThemeWindow::viewComboBox_activated( const QString & )
{

}


void KmlThemeWindow::helpPushButton_clicked()
{

}


bool KmlThemeWindow::hasThemeWithName( const std::string & themeName, TeView * view )
{
	std::vector<TeAbstractTheme*> absThemes = KmlPluginUtils::getThemes(_database, view);

	for(unsigned int i = 0; i < absThemes.size(); i++)
	{
		if(themeName == absThemes[i]->name())
		{
			return true;
		}
	}

	return false;
}



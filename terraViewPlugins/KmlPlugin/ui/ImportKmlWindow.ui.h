
//Terralib Includes
#include <TeDatabase.h>

#include <KmlPluginUtils.h>

//TerraView Includes
#include <terraViewBase.h>

//QT Includes
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qfiledialog.h>

#include <kmlmanager.h>
#include <kmlimporter.h>

void ImportKmlWindow::filePushButton_clicked()
{
	
	QString fileName = QFileDialog::getOpenFileName("", tr("KML Files (*.kml;*.KML)"), this, tr("Open Import File"), tr("Choose a file"));

	if(fileName.isEmpty())
		return;

	std::string name = TeGetBaseName(fileName);
	
	if(!KmlPluginUtils::correctName(name))
		layerNameLineEdit->setText("");
	else
		layerNameLineEdit->setText(name.c_str());
		
	fileLineEdit->setText(fileName);
	
}


void ImportKmlWindow::importPushButton_clicked()
{
	setCursor(Qt::WaitCursor);
	
	std::string layerName = layerNameLineEdit->text().latin1();
	std::string fileName = fileLineEdit->text().latin1();


	//Checking texts
	
	//Check File Name
	if(fileName.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Choose a file to import!"));
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

	//Check Layer Name
	if(layerName.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Choose a name for the Layer!"));
		setCursor(Qt::ArrowCursor);
		return;
	}	
	if(!KmlPluginUtils::correctName(layerName))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Layer name invalid!"));
		setCursor(Qt::ArrowCursor);
		return;
	}
	if(layerName.length() > 30)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please choose a layer name under 30 characters!"));
		setCursor(Qt::ArrowCursor);
		return;
	}	
	if (_database->layerExist(layerName))
	{
		QMessageBox::warning(this, tr("Warning"), tr("There is already a Layer with this name in the database."));
		setCursor(Qt::ArrowCursor);
		return;
	}

	//End Checking texts


	TeLayer* layer = 0;
	tdk::KMLManager* mger = new tdk::KMLManager();
	mger->parse(fileName);

	if(mger->hasMultiPolygon())
	{
		QMessageBox::warning(this, tr("Warning"), tr("The Kml has multipolygons, what is not suported."));
		setCursor(Qt::ArrowCursor);
		return;
	}
	else
	{
		try
		{		
			tdk::KMLImporter importer;
			layer = importer.import(mger->getObjects(), layerName, _database);
		}
		catch(TeException& e)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error importing kml file"));
			setCursor(Qt::ArrowCursor);
			return;
		}
	}

	delete mger;

	_tViewBase->popupDatabaseRefreshSlot();

    setCursor(Qt::ArrowCursor);

	QMessageBox::warning(this, tr("Warning"), tr("The import was successful!"));

	accept();

}


void ImportKmlWindow::closePushButton_clicked()
{
	close();
}


void ImportKmlWindow::init( TeDatabase * database, TerraViewBase* TViewBase )
{
	_database = database;
	_tViewBase = TViewBase;
}


void ImportKmlWindow::helpPushButton_clicked()
{

}

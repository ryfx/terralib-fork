//Terralib Includes
#include <TeDatabase.h>

#include <KmlPluginUtils.h>

//QT Includes
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qfiledialog.h>

#include <kmlmanager.h>
#include <kmlexporter.h>


void ExportKmlWindow::pathPushButton_clicked()
{
		
	QString fileName = QFileDialog::getSaveFileName(QString::null, tr("KML Files (*.kml;*.KML)"), this, tr("Open Import File"), tr("Choose a file"));
	
	if(fileName.isEmpty())
		return;
	
	int res;
	if(QFile::exists(fileName))
	{
		res = QMessageBox::question(this, tr("Kml Plugin"), tr("Do you want to replace de archive?"), QMessageBox::Yes, QMessageBox::No);
	
		if(res == QMessageBox::No)
		{
			pathPushButton_clicked();
		}
	}	
	
	pathLineEdit->setText(fileName);
	
}


void ExportKmlWindow::exportPushButton_clicked()
{
	setCursor(Qt::WaitCursor);

	std::string path = pathLineEdit->text().latin1();	
	
	if(path.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Choose a path to export!"));
		setCursor(Qt::ArrowCursor);
		return;
	}

	if(comboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Choose a layer or theme!"));
		setCursor(Qt::ArrowCursor);
		return;
	}
	
	std::string aux = TeGetExtension(path.c_str());
	if(aux.empty())
		path += ".kml";
	
	TeTheme* theme = 0;
	TeLayer* layer = 0;
	TeView* view = 0;
	std::string descTable;
	std::string nameColumn;
	std::string descColumn;

	if(layerRadioButton->isChecked())
	{		
		layer = KmlPluginUtils::getLayer(_database, std::string(comboBox->currentText().latin1()));
		descTable = layer->attrTables()[0].name();
		nameColumn = nameComboBox->currentText().latin1();
		descColumn = descComboBox->currentText().latin1();
	}
	else
	{
		view = KmlPluginUtils::getView(_database, viewComboBox->currentText().latin1()); 

		theme = KmlPluginUtils::getTeTheme(_database, std::string(comboBox->currentText().latin1()), view);	
		if(theme == 0)
		{
			QMessageBox::information(this, tr("Information"), tr("A theme with the given representation cannot be exported to Kml!"));
			return;
		}


		descTable = theme->layer()->attrTables()[0].name();
		nameColumn = nameComboBox->currentText().latin1();
		descColumn = descComboBox->currentText().latin1();
	}

	std::vector<KMLFeature*> docs;
	
	if(layerRadioButton->isChecked())
		docs = layer2KML(layer, descTable, nameColumn, descColumn);
	else
		docs = theme2KML(theme, descTable, nameColumn, descColumn);

	tdk::KMLManager* mger = new tdk::KMLManager();

	mger->saveKML(docs, path);

	setCursor(Qt::ArrowCursor);

	QMessageBox::warning(this, tr("Warning"), tr("The export was successful!"));

	accept();

}


void ExportKmlWindow::closePushButton_clicked()
{

	close();

}


void ExportKmlWindow::init( TeDatabase * database )
{
	_database = database;

	loadLayers();
	loadLayerColumns();
}


void ExportKmlWindow::loadThemes()
{
	comboBox->clear();
	
	if(viewComboBox->count() < 1)
		return;

	TeView* view = 0;
	view = KmlPluginUtils::getView(_database, viewComboBox->currentText().latin1());

	if(!view)
		return;

	std::vector<TeAbstractTheme*> themes = KmlPluginUtils::getThemes(_database, view);

	if(themes.empty())
		return;

	for(unsigned int i = 0; i < themes.size(); i++)
	{
		if(themes[i]->type() == TeTHEME)
		{
			TeTheme* theme = dynamic_cast<TeTheme*>(themes[i]);
			if(theme->layer()->hasGeometry(TeRASTER) || theme->layer()->hasGeometry(TeRASTERFILE))
			{
				continue;
			}

			comboBox->insertItem(themes[i]->name().c_str());
		}
	}	
	
}

void ExportKmlWindow::loadLayers()
{
	comboBox->clear();
	
	std::vector<TeLayer*> layers = KmlPluginUtils::getLayers(_database);
	
	for(unsigned int i = 0; i < layers.size(); i++)
	{
		comboBox->insertItem(layers[i]->name().c_str());
	}
	
}

void ExportKmlWindow::loadThemeColumns()
{
	nameComboBox->clear();
	descComboBox->clear();

	if(viewComboBox->count() < 1)
		return;

	if(comboBox->count() < 1)
		return;

	TeView* view = 0;
	view = KmlPluginUtils::getView(_database, viewComboBox->currentText().latin1());

	std::string themeName = comboBox->currentText().latin1();
	TeTheme* theme = KmlPluginUtils::getTeTheme(_database, themeName, view);

	if(theme == 0)
	{
		return;
	}

	TeAttributeList& attrList = theme->attrTables()[0].attributeList();

	nameComboBox->insertItem("");
	descComboBox->insertItem("");

	for(unsigned int i = 0; i < attrList.size(); i++)
	{
		nameComboBox->insertItem(attrList[i].rep_.name_.c_str());
		descComboBox->insertItem(attrList[i].rep_.name_.c_str());
	}
}

void ExportKmlWindow::loadLayerColumns()
{
	nameComboBox->clear();
	descComboBox->clear();

	if(comboBox->currentText().isEmpty())
		return;

	TeLayer* layer = KmlPluginUtils::getLayer(_database, string(comboBox->currentText().latin1()));

	if(!layer)
		return;

	TeAttributeList& attrList = layer->attrTables()[0].attributeList();

	nameComboBox->insertItem("");
	descComboBox->insertItem("");

	for(unsigned int i = 0; i < attrList.size(); i++)
	{
		nameComboBox->insertItem(attrList[i].rep_.name_.c_str());
		descComboBox->insertItem(attrList[i].rep_.name_.c_str());
	}
}

void ExportKmlWindow::buttonGroup_clicked( int id)
{
	nameComboBox->clear();
	descComboBox->clear();

	if(id == 0)
	{
		textLabel->setText(tr("Layer:"));
		viewTextLabel->setEnabled(false);
		viewComboBox->setEnabled(false);
		loadLayers();
	}
	else
	{
		textLabel->setText(tr("Theme:"));
		viewTextLabel->setEnabled(true);
		viewComboBox->setEnabled(true);
		loadViews();
		loadViewThemes();

		if(comboBox->count() < 1)
		{
			QMessageBox::warning(0, tr("Warning"), tr("You do not have a valid theme to export!"));
		}

		loadThemeColumns();

		
	}
	
	comboBox_activated("");
}


void ExportKmlWindow::comboBox_activated( const QString & )
{
	pathLineEdit->clear();
	nameComboBox->clear();
	descComboBox->clear();
	
	if(layerRadioButton->isChecked())
		loadLayerColumns();
	else
	{		
		loadThemeColumns();
	}
}


void ExportKmlWindow::helpPushButton_clicked()
{

}


void ExportKmlWindow::viewComboBox_activated( const QString & )
{
	loadViewThemes();
	loadThemeColumns();
}


void ExportKmlWindow::loadViews()
{
	viewComboBox->clear();

	std::vector<TeView*> views = KmlPluginUtils::getViews(_database);

	for(unsigned int i = 0; i < views.size(); i++)
	{
		viewComboBox->insertItem(views[i]->name().c_str());
	}
}


void ExportKmlWindow::loadViewThemes()
{
	comboBox->clear();

	if(viewComboBox->count() < 1)
		return;

	TeView* view = 0;
	view = KmlPluginUtils::getView(_database, viewComboBox->currentText().latin1());
	
	std::vector<TeAbstractTheme*> themes = KmlPluginUtils::getThemes(_database, view);

	if(themes.empty())
		return;

	for(unsigned int i = 0; i < themes.size(); i++)
	{
		if(themes[i]->type() == TeTHEME)
		{
			TeTheme* theme = dynamic_cast<TeTheme*>(themes[i]);
			if(theme->layer()->hasGeometry(TeRASTER) || theme->layer()->hasGeometry(TeRASTERFILE))
			{
				continue;
			}
			comboBox->insertItem(themes[i]->name().c_str());
		}
	}
}

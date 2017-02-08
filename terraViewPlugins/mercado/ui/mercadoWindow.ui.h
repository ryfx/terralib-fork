/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

/*! \file mercadoWindow.ui.h
    This file contains interface implementation used to create mercado.
	\author Eric Silva Abreu
*/
#include <qmessagebox.h>
#include <qcolordialog.h>
#include <qpainter.h>
#include <qbitmap.h>
#include <qvalidator.h>
#include <qfiledialog.h>

#include <TeImportRaster.h>
#include <TeRasterParams.h>
#include <TeRasterRemap.h>
#include <TeAppTheme.h>
#include <TeApplicationUtils.h>

#include "TeMercado.h"
#include "TeKernelInterpolation.h"
#include <TeGroupingAlgorithms.h>
#include <TeColorUtils.h>
#include <TeQtGrid.h>

//! Implementacao da interface que auxilia no processo de geracao de mercados
/*!
    Essa classe eh a implementacao da interface gerada em Qt para geracao de mercados
	permitindo a importacao do dados externos referentes ao eventos, permite tambem a
	selecao de objetos distintos para utilizacao no mercado e determinacao de suas legendas,
	tambem permite a selecao do algoritmo de interpolacao a ser utilizado.

 \sa
	TeMercado
 */


//! Constructor with parameters 
/*!
	\param currentTheme (input) - pointer to a current theme
*/
bool mercadoWindow::init(PluginParameters * plug_pars_ptr)
{
	QDoubleValidator* doubleValidator = new QDoubleValidator(radiusLineEdit);
	radiusLineEdit->setValidator(doubleValidator);

	plug_pars_ptr_ = plug_pars_ptr;
	currentTheme_ = (TeTheme*)plug_pars_ptr->getCurrentThemeAppPtr()->getTheme();
	currentDatabase_ = currentTheme_->layer()->database();;

	//lista todas as tabelas externas contidas neste banco
	if(!listExternalTables(string("")))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Error listing external tables!"));
		return false;
	}

	//lista todos os layers contidos neste banco
	if(!listLayers())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Error listing layers!"));
		return false;
	}

	objVec_.clear();

	//desabilitar todos os campos
	themeTableGroupBox->setEnabled(false);
	tabWinGroupBox->setEnabled(false);
	externalTableGroupBox->setEnabled(false);
	layerRefGroupBox->setEnabled(false);

	return true;
}

//! Destructor
/*!

*/
void mercadoWindow::destroy()
{
    
}

//! Slot next push button, used to load next page from interface
/*!

*/
void mercadoWindow::nextOnePushButton_clicked()
{
	if(externalTableCheckBox->isChecked())
	{
		//recuperar todos os objetos disintos da tabela externa;
		if(!getExternalTableObjects())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error getting external table objects!"));
			return;
		}
	}
	else if(themeTableCheckBox->isChecked())
	{
		//recupera as colunas da tabela local menos a coluna de ligacao
		if(!getLocalTableObjescts())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error getting local table objects!"));
			return;
		}
	}
	else if(tabWinTableCheckBox->isChecked())
	{
		TeMercado* merc = new TeMercado(currentDatabase_);

		std::string tableName = tableNameLineEdit->text().latin1();
		std::string layerName = currentTheme_->layer()->name();
		std::string layerTableName = currentTheme_->layer()->attrTables()[0].name();
		std::string columnName = linkColumnComboBox->currentText().latin1();
		std::string columnLabelName = linkNameColumnComboBox->currentText().latin1();

		vector<string> labels = merc->getDistinctTableObjects(tableName, layerName, layerTableName, columnName, columnLabelName);

		for (unsigned int i = 0; i < labels.size(); i++)
		{
			objectListBox->insertItem(labels[i].c_str(), i);
		}

		delete merc;
	}

	//inicializa a tabela de legenda
	legendTable->setNumRows(0);

	widgetStack->raiseWidget(1);
}

//! Slot previous push button, used to load previous page from interface
/*!

*/
void mercadoWindow::previousTwoPushButton_clicked()
{
	//limpa o campo contendo os objetos listados
	objectListBox->clear();

	//limpa o vetor contendo os objetos listados
	objVec_.clear();

	//limpa a tabela de legenda
	legendTable->setNumRows(0);

	widgetStack->raiseWidget(0);
}

//! Slot next push button, used to load next page from interface
/*!

*/
void mercadoWindow::nextTwoPushButton_clicked()
{
	//verifica se foi criada legenda para os objetos selecionados
	if(legendTable->numRows() == 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Legend is empty!"));
		return;
	}

	//desabilatado a opção do tamanho do raio
	adaptiveCheckBox->setChecked(false);
	adaptiveCheckBox->setDisabled(true);
	neighboursLineEdit->setDisabled(true);
	
    widgetStack->raiseWidget(2);
}

//! Slot previous push button, used to load previous page from interface
/*!

*/
void mercadoWindow::previousThreePushButton_clicked()
{
	radiusSlider->setEnabled(false);
	radiusLineEdit->setDisabled(true);
	radiusSlider->setValue(0);
	radiusLineEdit->clear();

    widgetStack->raiseWidget(1);
}

//! Slot next push button, used to load next page from interface
/*!

*/
void mercadoWindow::nextThreePushButton_clicked()
{
	if(kernelRadioButton->isChecked())
	{
		if(resXLineEdit->text().isEmpty() || resYLineEdit->text().isEmpty() || 
			numColsLineEdit->text().isEmpty() || numLinesLineEdit->text().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Raster Dimensions not defined!"));
			return;
		}

		if(radiusTextLabel->text().toDouble() <= 0. ||
		   radiusTextLabel->text().toDouble() > widthLineEdit->text().toDouble())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Valor de raio inválido!"));
			return;
		}
	}

    widgetStack->raiseWidget(3);
	mercTeQtColorBar->setVerticalBar(false);

	if(vecRadioButton->isChecked())
	{
		mercObjGroupBox->setEnabled(false);
	}
}

//! Slot previous push button, used to load previous page from interface
/*!

*/
void mercadoWindow::previousFourPushButton_clicked()
{
    widgetStack->raiseWidget(2);
}

//! Slot ok push button clicked, run  mercado process
/*!
	
*/
void mercadoWindow::okPushButton_clicked()
{
	if(minDomLineEdit->text().latin1() == string("") ||
	   maxDomLineEdit->text().latin1() == string("") ||
	   minSecLineEdit->text().latin1() == string("") ||
	   maxSecLineEdit->text().latin1() == string("") ||
	   minTerLineEdit->text().latin1() == string("") ||
	   maxTerLineEdit->text().latin1() == string(""))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Some dominance value is empty!"));
		return;
	}

	int valueMinDom = minDomLineEdit->text().toInt();
	int valueMaxDom = maxDomLineEdit->text().toInt();

	int valueMinSec = minSecLineEdit->text().toInt();
	int valueMaxSec = maxSecLineEdit->text().toInt();

	int valueMinTer = minTerLineEdit->text().toInt();
	int valueMaxTer = maxTerLineEdit->text().toInt();

	if(valueMinDom >= valueMaxDom)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Primary Dominance range is invalid!"));
		return;
	}

	if(valueMinSec >= valueMaxSec)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Secondary Dominance range is invalid!"));
		return;
	}

	if(valueMinTer >= valueMaxTer)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Tertiary Dominance range is invalid!"));
		return;
	}

	if(valueMaxSec > valueMinDom)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Mininum primary dominance value can not be major than Maximun second dominance value!"));
		return;
	}

	if(valueMaxTer > valueMinSec)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Mininum second dominance value can not be major than Maximun tertiary dominance value!"));
		return;
	}

	if(layerNameLineEdit->text().latin1() == string(""))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Layer name is empty!"));
		return;
	}

	string layerName = layerNameLineEdit->text().latin1();
	if(currentDatabase_->layerExist(layerName))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Layer already exist!"));
		return;
	}

	TeQtGrid* grid = plug_pars_ptr_->teqtgrid_ptr_;
	
	if(grid != 0)
	{
		grid->clearPortal();
	}

	string mercadoTableName;
	std::string mercadoLabelTableName;
	if(externalTableCheckBox->isChecked())
	{
		//gera a tabela de mercado e faz o link com a tabela do tema corrente;
		TeMercado* merc = new TeMercado(currentDatabase_);

		string externalTable = externalTableComboBox->currentText().latin1();
		string destinyColumnName = destinyComboBox->currentText().latin1();
		string originColumnName = originComboBox->currentText().latin1();
		string labelColumnName = destinyLabelComboBox->currentText().latin1();
		string linkTableName = currentTheme_->layer()->attrTables()[0].name();
		string linkColumnName = linkColumnComboBox->currentText().latin1();

		TeTable mercadoTable;

		if(!merc->createMercadoTable(externalTable, originColumnName, destinyColumnName, linkTableName, linkColumnName, mercadoTable))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado Table!"));
			delete merc;
			return;
		}

		string merTableName = "mercado_" + layerNameLineEdit->text();
		mercadoTable.name(merTableName);
		mercadoTable.setTableType(TeAttrStatic, currentTheme_->layer()->attrTables()[0].id(), linkColumnName);
		mercadoTable.setLinkName(originColumnName);
		mercadoTable.relatedTableName(currentTheme_->layer()->attrTables()[0].name());

		if(!currentDatabase_->createTable(mercadoTable.name(), mercadoTable.attributeList()))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado Table in database!"));
			return;
		}

		if(!currentTheme_->layer()->saveAttributeTable(mercadoTable))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error saving Mercado Table attributes!"));
			return;
		}

		if(!currentDatabase_->insertThemeTable(currentTheme_, mercadoTable))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error inserting Mercado Table in theme!"));
			return;
		}

		if(!currentTheme_->addThemeTable(mercadoTable))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error add theme Mercado Table!"));
			return;
		}

		if (!currentTheme_->createCollectionAuxTable() || !currentTheme_->populateCollectionAux())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Table Aux!"));
			return;
		}

		mercadoTableName = mercadoTable.name();

		mercadoLabelTableName = mercadoTableName + "_label";

		TeTable mercadoLabelTable;

		if(!merc->createMercadoLabelTable(externalTable, destinyColumnName, labelColumnName, mercadoLabelTable))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado label Table!"));
			delete merc;
			return;
		}

		mercadoLabelTable.name(mercadoLabelTableName);

		if(!currentDatabase_->createTable(mercadoLabelTable.name(), mercadoLabelTable.attributeList()))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado label Table in database!"));
			return;
		}

		if(!currentDatabase_->insertTable(mercadoLabelTable))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error saving Mercado label Table in database!"));
			return;
		}

		delete merc;
	}
	else if(tabWinTableCheckBox->isChecked())
	{
		//gera a tabela de mercado e faz o link com a tabela do tema corrente;
		TeMercado* merc = new TeMercado(currentDatabase_);

		string fileName = tableNameLineEdit->text().latin1();

		string labelColumnName = linkNameColumnComboBox->currentText().latin1();
		string linkTableName = currentTheme_->layer()->attrTables()[0].name();
		string linkColumnName = linkColumnComboBox->currentText().latin1();

		TeTable mercadoTable;

		std::string originColumnName = "Name";

		if(!merc->createMercadoTable(fileName, originColumnName, linkTableName, linkColumnName, mercadoTable))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado Table!"));
			delete merc;
			return;
		}
		
		string merTableName = "mercado_" + layerNameLineEdit->text();
		mercadoTable.name(merTableName);
		mercadoTable.setTableType(TeAttrStatic, currentTheme_->layer()->attrTables()[0].id(), linkColumnName);
		mercadoTable.setLinkName(originColumnName);
		mercadoTable.relatedTableName(currentTheme_->layer()->attrTables()[0].name());

		if(!currentDatabase_->createTable(mercadoTable.name(), mercadoTable.attributeList()))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado Table in database!"));
			return;
		}

		if(!currentTheme_->layer()->saveAttributeTable(mercadoTable))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error saving Mercado Table attributes!"));
			return;
		}

		if(!currentDatabase_->insertThemeTable(currentTheme_, mercadoTable))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error inserting Mercado Table in theme!"));
			return;
		}

		if(!currentTheme_->addThemeTable(mercadoTable))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error add theme Mercado Table!"));
			return;
		}

		if (!currentTheme_->createCollectionAuxTable() || !currentTheme_->populateCollectionAux())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Table Aux!"));
			return;
		}

		mercadoTableName = mercadoTable.name();

		mercadoLabelTableName = mercadoTableName + "_label";

		std::string layerName = currentTheme_->layer()->name();
		std::string layerTableName = currentTheme_->layer()->attrTables()[0].name();

		TeTable mercadoLabelTable;

		if(!merc->createMercadoLabelTable(fileName, layerName, layerTableName, linkColumnName, labelColumnName, mercadoLabelTable))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado label Table!"));
			delete merc;
			return;
		}

		mercadoLabelTable.name(mercadoLabelTableName);

		if(!currentDatabase_->createTable(mercadoLabelTable.name(), mercadoLabelTable.attributeList()))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado label Table in database!"));
			return;
		}

		if(!currentDatabase_->insertTable(mercadoLabelTable))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error saving Mercado label Table in database!"));
			return;
		}

		delete merc;

		
	}
	else if(themeTableCheckBox->isChecked())
	{
        //utiliza a tabela ja existente
		mercadoTableName = themeTableComboBox->currentText().latin1();

		mercadoLabelTableName = mercadoTableName + "_label";
	}

	vector<string> objVec;

	vector<string> objVecLabel;

	vector<string> allObjVec;

	QListBoxItem* item = objectListBox->firstItem();

	TeMercado* mercado = new TeMercado(currentDatabase_);
	
	while( item != 0 )
	{
		string objLabel = item->text().latin1();
		string objId = mercado->getIdFromLabel(mercadoLabelTableName, objLabel);
		string objIdLabel = "obj_" + objId;

		if( item->isSelected() )
		{
			objVec.push_back(objIdLabel);
			objVecLabel.push_back(item->text().latin1());
		}

		allObjVec.push_back(objIdLabel);
	
		item = item->next();
	}

	bool useOnlySelectedObjs = selObjCheckBox->isChecked();

	delete mercado;
	
	if(kernelRadioButton->isChecked())
	{
		if(!currentTheme_->layer()->hasGeometry(TePOINTS))
		{

			TeMercado* merc = new TeMercado(currentDatabase_);

			if(!merc->createPointRepresentation(currentTheme_->layer()))
			{
				QMessageBox::warning(this, tr("Warning"), tr("Error creating pointing representation!"));
				return;
			}

			delete merc;
			
			currentTheme_->visibleRep(currentTheme_->visibleRep() + 4);
		}
		
		vector<TeRaster*> rasterVec;
		
		TePolygonSet ps;
		currentTheme_->layer()->getPolygons(ps);

		
		double ratio = radiusTextLabel->text().toDouble();
		double resX = atof(resXLineEdit->text().latin1());
		double resY = atof(resYLineEdit->text().latin1());

		TeKernelInterpolationAlgorithm iA = TeDistWeightAvgInBoxInterpolation;
		int neighbours = 0;

		if(adaptiveCheckBox->isChecked())
		{
			iA = TeDistWeightAvgInterpolation;
			neighbours = neighboursLineEdit->text().toInt();
		}
		else
		{
			iA = TeDistWeightAvgInBoxInterpolation;
		}

		TeKernelInterpolationMethod iM = TeQuarticKernelMethod;

		std::string method = functionComboBox->currentText().latin1();

		if(method == "Quartic")
		{
			iM = TeQuarticKernelMethod;
		}
		else if(method == "Normal")
		{
			iM = TeNormalKernelMethod;
		}
		else if(method == "Triangular")
		{
			iM = TeUniformKernelMethod;
		}
		else if(method == "Uniform")
		{
			iM = TeTriangularKernelMethod;
		}
		else if(method == "Negative Exp")
		{
			iM = TeNegExpKernelMethod;
		}

		TeBox boxTheme = currentTheme_->box();

		TeRasterParams params;
		params.nBands(1);
		params.decoderIdentifier_ = "MEM";
		params.boundingBoxResolution(boxTheme.x1(), boxTheme.y1(), boxTheme.x2(), boxTheme.y2(),resX, resY);
		params.mode_ = 'c';
		params.projection(currentTheme_->layer()->projection());
		params.setDataType(TeDOUBLE);
		params.setDummy(-TeMAXFLOAT);
		params.setPhotometric(TeRasterParams::TeMultiBand);

		TeMercado* merc = new TeMercado(currentDatabase_);

		if (TeProgress::instance())
		{
			TeProgress::instance()->setCaption("Mercado");
			TeProgress::instance()->setMessage("Running. Please, wait!");
			TeProgress::instance()->setTotalSteps(objVec.size());
		}

		int count = 0;

		for(unsigned int i = 0; i < objVec.size(); i++)
		{
			TeRaster* raster = new TeRaster(params);
			if(!raster->init())
			{
				rasterVec.clear();
				return;
			}

			bool res = TeRasterInterpolate(currentTheme_, mercadoTableName, objVec[i], raster, 0, iA, iM, neighbours, ratio);

			if(res)
			{
				TeRaster* rasterClip = merc->mercadoRasterClipping(raster, ps, currentTheme_->layer()->projection(), objVec[i], 0, "SMARTMEM");

				raster->clear();
				delete raster;

				rasterVec.push_back(rasterClip);	
			}
			else
			{
				rasterVec.clear();
				return;
			}

			//progress bar
			if(TeProgress::instance())
			{
				if(TeProgress::instance()->wasCancelled())
				{
					TeProgress::instance()->reset();
					break;
				}
				TeProgress::instance()->setProgress(count);
			}

			count ++;
		}

		if (TeProgress::instance())
			TeProgress::instance()->reset();

		if(objLayersCheckBox->isChecked())
		{
			if(!createThemeRasters(rasterVec, objVecLabel))
			{
				QMessageBox::warning(this, tr("Warning"), tr("Error creating theme Rasters!"));
				rasterVec.clear();
				reject();
			}			
		}

//create output raster with mercado information
		std::vector<TeRaster*> mercRasterVec = merc->createMercadoRaster(rasterVec);

		std::vector<std::string> mercRasterVecNames;

		for(unsigned int i = 0; i < objVecLabel.size(); ++i)
		{
			std::string str = objVecLabel[i];
			str += "_merc";

			mercRasterVecNames.push_back(str);
		}

		if(!createThemeRasters(mercRasterVec, mercRasterVecNames))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating theme Rasters!"));
			rasterVec.clear();
			reject();
		}
/////////////////////////////////////

		TeRaster* rasterDom = merc->createMercadoRaster(rasterVec, objVec, colorVec_, valueMinDom, valueMaxDom);
		if(!rasterDom)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado Raster Dominance!"));
			rasterVec.clear();
			delete merc;
			delete rasterDom;
			reject();
		}
		
		if(!createMercadoThemeDom(rasterDom, objVecLabel))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado Theme Raster Dominance!"));
			rasterVec.clear();
			delete rasterDom;
			reject();
		}

		TeRaster* rasterSec = merc->createMercadoRaster(rasterVec, objVec, colorVec_, valueMinSec, valueMaxSec);
		if(!rasterSec)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado Raster Secondary!"));
			rasterVec.clear();
			delete merc;
			delete rasterSec;
			delete rasterDom;
			reject();
		}
		
		if(!createMercadoThemeSec(rasterSec, objVecLabel))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado Theme Raster Dominance!"));
			rasterVec.clear();
			delete rasterSec;
			delete rasterDom;
			reject();
		}

		TeRaster* rasterTer = merc->createMercadoRaster(rasterVec, objVec, colorVec_, valueMinTer, valueMaxTer);
		if(!rasterTer)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado Raster Tertiary!"));
			rasterVec.clear();
			delete merc;
			delete rasterTer;
			delete rasterSec;
			delete rasterDom;
			reject();
		}
		
		if(!createMercadoThemeTer(rasterTer, objVecLabel))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado Theme Raster Dominance!"));
			rasterVec.clear();
			delete rasterTer;
			delete rasterSec;
			delete rasterDom;
			reject();
		}

		std::string columnName = "primaryDom_" + layerName;

		merc->createMercadoRelation(rasterDom, currentTheme_->layer(), columnName, objVecLabel, colorVec_);

		std::string columnName2 = "secundaryDom_" + layerName;

		merc->createMercadoRelation(rasterSec, currentTheme_->layer(), columnName2, objVecLabel, colorVec_);

		std::string columnName3 = "tertiaryDom_" + layerName;

		merc->createMercadoRelation(rasterTer, currentTheme_->layer(), columnName3, objVecLabel, colorVec_);

		delete merc;

		rasterVec.clear();

		delete rasterDom;
		delete rasterSec;
		delete rasterTer;
	}
	else
	{
		TeMercado* merc = new TeMercado(currentDatabase_);

		//create theme dominance
		std::string themeDomName = "themeDom_" + layerName;

		TeTheme* themeDom = 0;

		bool res = false;

		if(useOnlySelectedObjs)
		{
			res = merc->createMercadoVetorial(currentTheme_, mercadoTableName, themeDomName, themeDom, objVec, objVecLabel, colorVec_, valueMinDom, valueMaxDom);
		}
		else
		{
			res = merc->createFullMercadoVetorial(currentTheme_, mercadoTableName, themeDomName, themeDom, objVec, objVecLabel, colorVec_, valueMinDom, valueMaxDom, allObjVec);
		}

		if(!res)
		{
			if(themeDom)
			{
				currentDatabase_->deleteTheme(themeDom->id());
				QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado Theme Dom!"));
				return;
			}
		}

		//create theme secondary
		std::string themeSecName = "themeSec_" + layerName;

		TeTheme* themeSec = 0;

		res = false;

		if(useOnlySelectedObjs)
		{
			res = merc->createMercadoVetorial(currentTheme_, mercadoTableName, themeSecName, themeSec, objVec, objVecLabel, colorVec_, valueMinSec, valueMaxSec);
		}
		else
		{
			res = merc->createFullMercadoVetorial(currentTheme_, mercadoTableName, themeSecName, themeSec, objVec, objVecLabel, colorVec_, valueMinSec, valueMaxSec, allObjVec);
		}

		if(!res)
		{
			if(themeSec)
			{
				currentDatabase_->deleteTheme(themeSec->id());
				QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado Theme Sec!"));
				return;
			}
		}

		//create theme tertiary
		std::string themeTerName = "themeTer_" + layerName;

		TeTheme* themeTer = 0;

		res = false;

		if(useOnlySelectedObjs)
		{
			res = merc->createMercadoVetorial(currentTheme_, mercadoTableName, themeTerName, themeTer, objVec, objVecLabel, colorVec_, valueMinTer, valueMaxTer);
		}
		else
		{
			res = merc->createFullMercadoVetorial(currentTheme_, mercadoTableName, themeTerName, themeTer, objVec, objVecLabel, colorVec_, valueMinTer, valueMaxTer, allObjVec);
		}

		if(!res)
		{
			if(themeTer)
			{
				currentDatabase_->deleteTheme(themeTer->id());
				QMessageBox::warning(this, tr("Warning"), tr("Error creating Mercado Theme Ter!"));
				return;
			}
		}
	}
	
	accept();
}

//! Function listExternalTables, used to get all external tables in database
/*!

*/
bool mercadoWindow::listExternalTables( string& extTable )
{
	externalTableComboBox->clear();	

	vector<string> tables;

	//lista todas as tabelas externas do banco corrente
	TeMercado utils(currentDatabase_);

	if(!utils.listCandidateFlowDataTables(tables))
	{
		return false;
	}

	unsigned int current = 0;
	
	for(unsigned int i = 0; i < tables.size(); ++i)
	{
		externalTableComboBox->insertItem(tables[i].c_str());

		if(tables[i] == extTable)
			current = i;
	}

	if(externalTableComboBox->count() > 0)
	{
        externalTableComboBox->setEnabled(true);	
		externalTableComboBox->setCurrentItem(current);	
		externalTableComboBox_activated(current);
	}
	else
	{
		externalTableComboBox->setEnabled(false);
	}

    return true;
}

//! Slot externalTableComboBox activated, used to get the attributes from a external table
/*!

*/
void mercadoWindow::externalTableComboBox_activated( int idxTable )
{
	string tableName = externalTableComboBox->text(idxTable).latin1();
	
	TeAttributeList attList;

	if(!currentDatabase_->getAttributeList(tableName, attList))
	{
		return;
	}
	

	unsigned int i = 0;

	originComboBox->clear();
	destinyLabelComboBox->clear();
	destinyComboBox->clear();

	for(i = 0; i < attList.size(); ++i)
	{
		originComboBox->insertItem(attList[i].rep_.name_.c_str());
		destinyLabelComboBox->insertItem(attList[i].rep_.name_.c_str());
		destinyComboBox->insertItem(attList[i].rep_.name_.c_str());
	}

	if(i > 0)
	{
		originComboBox->setEnabled(true);
		destinyLabelComboBox->setEnabled(true);
		destinyComboBox->setEnabled(true);
	}
	else
	{
		originComboBox->setEnabled(false);
		destinyLabelComboBox->setEnabled(false);
		destinyComboBox->setEnabled(false);
	}

	return;	
}

//! Function listLayers, used to get all layers in database
/*!

*/
bool mercadoWindow::listLayers()
{
	TeLayerMap& layerMap = currentDatabase_->layerMap();
	TeLayerMap::iterator it;

	unsigned int current = 0;
	unsigned int count = 0;
	for (it = layerMap.begin(); it != layerMap.end(); ++it)
	{
		layerNameComboBox->insertItem(it->second->name().c_str());

		if(it->second->name() == currentTheme_->layer()->name())
		{
			current = count;
		}

		++count;
	}

	if(count > 0)
	{
        layerNameComboBox->setEnabled(false);	
		layerNameComboBox->setCurrentItem(current);	
		layerNameComboBox_activated(current);
	}
	else
	{
		layerNameComboBox->setEnabled(false);
		return false;
	}

    return true;
}

//! Function layerNameComboBox activated, used to get the attribute tables from a specif layer
/*!

*/
void mercadoWindow::layerNameComboBox_activated( int idxLayerName )
{
    string layerName = layerNameComboBox->text(idxLayerName).latin1();

	//por enquanto apenas o layer do tema corrente podera ser utilizado

	TeAttrTableVector layerTablesVector;

	if(!currentTheme_->layer()->getAttrTables(layerTablesVector))
	{
		return;
	}

	linkColumnComboBox->clear();
	linkNameColumnComboBox->clear();

	unsigned int count = 0;
	for(unsigned int i = 0; i < layerTablesVector.size(); ++i)
	{
		TeAttributeList attList = layerTablesVector[i].attributeList();

		for(unsigned int j = 0; j < attList.size(); ++j)
		{
			linkColumnComboBox->insertItem(attList[j].rep_.name_.c_str());
			linkNameColumnComboBox->insertItem(attList[j].rep_.name_.c_str());
			++count;
		}
	}

	if(count > 0)
	{
		linkColumnComboBox->setEnabled(true);
	}
	else
	{
		linkColumnComboBox->setEnabled(false);
	}

	return;
}



//! Function getExternalTableObjects, used to get all distinct objecs from a external table
/*!

*/
bool mercadoWindow::getExternalTableObjects()
{
	if(externalTableComboBox->currentText().isEmpty())
	{
		return false;
	}

	if(destinyComboBox->currentText().isEmpty())
	{
		return false;
	}

	if(destinyLabelComboBox->currentText().isEmpty())
	{
		return false;
	}

	string externalTable = externalTableComboBox->currentText().latin1();
	string columnName = destinyComboBox->currentText().latin1();
	string labelColumnName = destinyLabelComboBox->currentText().latin1();


	//recupera todos os objetos distintos da coluna referente a tabela externa selecionada
	TeMercado* merc = new TeMercado(currentDatabase_);

	vector<string> allObjVec = merc->getDistinctLabelObjects(externalTable, columnName, labelColumnName);

	delete merc;

	if(allObjVec.size() == 0)
	{
		return false;
	}

	for(unsigned int i = 0; i < allObjVec.size(); i++)
	{
		objectListBox->insertItem(allObjVec[i].c_str(), i);
	}
	
	return true;
}

//! Slot allObjCheckBox clicked, used to select all objects to use in mercado process
/*!

*/
void mercadoWindow::allObjCheckBox_clicked()
{
	objectListBox->selectAll(allObjCheckBox->isChecked());
}

//! Slot color apply button clicked, used to generate the legend for distinct objects selected to mercado
/*!

*/
void mercadoWindow::applyPushButton_clicked()
{
    if (strColor_.empty())
	{
		redCheckBox->setChecked(true);
		strColor_ = "R";
	}

	//recupera os objetos selecionados pelo usuario para o algoritmo de mercado
	getSelectedObjects();

	if(objVec_.size() == 0)
	{
		legendTable->setNumRows(0);
		return;
	}

	vector<TeSlice> slices;
	TeGroupByUniqueValue(objVec_, /*group attribute type*/ TeSTRING, slices, /*group precision*/ 6);

	if(slices.size() > 200)
		return;

	vector<string> colorNameVec;
	getColorNameVector(strColor_, colorNameVec);
	
	colorVec_.clear();
	getColors(colorNameVec, objVec_.size(), colorVec_);

	TeVisual visual;
	if((currentTheme_->visibleRep() & TePOLYGONS) || (currentTheme_->visibleRep() & TeCELLS))
		visual = *(currentTheme_->defaultLegend().visual(TePOLYGONS));
	else if(currentTheme_->visibleRep() & TeLINES)
		visual = *(currentTheme_->defaultLegend().visual(TeLINES));
	else if(currentTheme_->visibleRep() & TePOINTS)
		visual = *(currentTheme_->defaultLegend().visual(TePOINTS));


	TeLegendEntryVector legendTheme; 
	
	for(unsigned int i=0; i<slices.size(); ++i)
	{
		TeColor cor = colorVec_[i];
		visual.color(cor);

		TeLegendEntry leg;
		leg.from(slices[i].from_);
		leg.to(slices[i].to_);

		if((currentTheme_->visibleRep() & TePOLYGONS) || (currentTheme_->visibleRep() & TeCELLS))
			leg.setVisual(visual.copy(), TePOLYGONS);
		else if(currentTheme_->visibleRep() & TeLINES)
			leg.setVisual(visual.copy(), TeLINES);
		else if(currentTheme_->visibleRep() & TePOINTS)
			leg.setVisual(visual.copy(), TePOINTS);

		int count = 0;
		unsigned int j = 0;
		while(j < objVec_.size())
		{
			if(objVec_[j] == leg.from())
			{
				count++;
				j++;
			}
			else
				break;
		}

		leg.count(count);
		leg.group(i);
		string label = leg.from();
		leg.label(label);
		legendTheme.push_back(leg);
	}
	legendTable->setNumRows(legendTheme.size());

	legVec_.clear();

	for(unsigned int i = 0; i<legendTheme.size(); i++)
	{
		legVec_.push_back(legendTheme[i]);
	}
	
	//putColorOnLegend();
	showLegend();
}

//! Slot color button clicked, used to set the color components from group color
/*!

*/
void mercadoWindow::colorButtonGroup_clicked( int i )
{
	int idx;
	QString colors = strColor_.c_str();
	
	int size = colors.length();

	switch(i)
	{
		case 0:
			if (redCheckBox->isChecked() == false)
			{
				idx = colors.find(QString("R"));
				if (idx == 0)
				{
					if (size == 1)
						colors.remove(idx, 1);
					else
						colors.remove(idx, 2);
				}
				else if (idx > 0)
					colors.remove(idx-1, 2);
			}
			else
			{
				if (colors.isEmpty())
					colors.append("R");
				else
					colors.append("-R");
			}
			break;

		case 1:
			if (greenCheckBox->isChecked() == false)
			{
				idx = colors.find(QString("G"));
				if (idx == 0)
				{
					if (size == 1)
						colors.remove(idx, 1);
					else
						colors.remove(idx, 2);
				}
				else if (idx > 0)
					colors.remove(idx-1, 2);
			}
			else
			{
				if (colors.isEmpty())
					colors.append("G");
				else
					colors.append("-G");
			}
			break;

		case 2:
			if (blueCheckBox->isChecked() == false)
			{
				idx = colors.find(QString("B"));
				if (idx == 0)
				{
					if (size == 1)
						colors.remove(idx, 1);
					else
						colors.remove(idx, 2);
				}
				else if (idx > 0)
					colors.remove(idx-1, 2);
			}
			else
			{
				if (colors.isEmpty())
					colors.append("B");
				else
					colors.append("-B");
			}
			break;

		case 3:
			if (yellowCheckBox->isChecked() == false)
			{
				idx = colors.find(QString("Y"));
				if (idx == 0)
				{
					if (size == 1)
						colors.remove(idx, 1);
					else
						colors.remove(idx, 2);
				}
				else if (idx > 0)
					colors.remove(idx-1, 2);
			}
			else
			{
				if (colors.isEmpty())
					colors.append("Y");
				else
					colors.append("-Y");
			}
			break;

		case 4:
			if (orangeCheckBox->isChecked() == false)
			{
				idx = colors.find(QString("Or"));
				if (idx == 0)
				{
					if (size == 2)
						colors.remove(idx, 2);
					else
						colors.remove(idx, 3);
				}
				else if (idx > 0)
					colors.remove(idx-1, 3);
			}
			else
			{
				if (colors.isEmpty())
					colors.append("Or");
				else
					colors.append("-Or");
			}
			break;

		case 5:
			if (magentaCheckBox->isChecked() == false)
			{
				idx = colors.find(QString("Mg"));
				if (idx == 0)
				{
					if (size == 2)
						colors.remove(idx, 2);
					else
						colors.remove(idx, 3);
				}
				else if (idx > 0)
					colors.remove(idx-1, 3);
			}
			else
			{
				if (colors.isEmpty())
					colors.append("Mg");
				else
					colors.append("-Mg");
			}
			break;

		case 6:
			if (cyanCheckBox->isChecked() == false)
			{
				idx = colors.find(QString("Cy"));
				if (idx == 0)
				{
					if (size == 2)
						colors.remove(idx, 2);
					else
						colors.remove(idx, 3);
				}
				else if (idx > 0)
					colors.remove(idx-1, 3);
			}
			else
			{
				if (colors.isEmpty())
					colors.append("Cy");
				else
					colors.append("-Cy");
			}
			break;

		case 7:
			if (grayCheckBox->isChecked() == false)
			{
				idx = colors.find(QString("Gr"));
				if (idx == 0)
				{
					if (size == 2)
						colors.remove(idx, 2);
					else
						colors.remove(idx, 3);
				}
				else if (idx > 0)
					colors.remove(idx-1, 3);
			}
			else
			{
				if (colors.isEmpty())
					colors.append("Gr");
				else
					colors.append("-Gr");
			}
			break;

		default:
			break;
	}
	strColor_ = colors.latin1();
	//putColorOnLegend();
	//showLegend();   
}


//! Slot table double clicked, used to select any item from table legend
/*!
	\param row		Current row from table legend
	\param col		Current col from table legend
	\param Qpoint	Specific point from table legend
*/
void mercadoWindow::legendTable_doubleClicked( int row, int col, int, const QPoint & )
{
	if(legVec_.empty())
		return;

	if(col != 0 || row >= (int)legVec_.size())
		return;

	if (col == 0)
	{

		QColor	qc;
		TeVisual visual;
		if((currentTheme_->visibleRep() & TePOLYGONS) || (currentTheme_->visibleRep() & TeCELLS))
		{
			visual = *(legVec_[row].visual(TePOLYGONS));
			qc = QColorDialog::getColor((QColor(visual.color().red_, visual.color().green_, visual.color().blue_)));
			visual.color(TeColor(qc.red(), qc.green(), qc.blue()));
			legVec_[row].setVisual(visual.copy(), TePOLYGONS);
		} 
		else if(currentTheme_->visibleRep() & TePOINTS)
		{
			visual = *(legVec_[row].visual(TePOLYGONS));
			qc = QColorDialog::getColor((QColor(visual.color().red_, visual.color().green_, visual.color().blue_)));
			visual.color(TeColor(qc.red(), qc.green(), qc.blue()));
			legVec_[row].setVisual(visual.copy(), TePOINTS);
		}
		else if(currentTheme_->visibleRep() & TeLINES)
		{
			visual = *(legVec_[row].visual(TePOLYGONS));
			qc = QColorDialog::getColor((QColor(visual.color().red_, visual.color().green_, visual.color().blue_)));
			visual.color(TeColor(qc.red(), qc.green(), qc.blue()));
			legVec_[row].setVisual(visual.copy(), TeLINES);
		}
		else
		{
			return;
		}


		colorVec_[row].red_ = qc.red();
		colorVec_[row].green_ = qc.green();
		colorVec_[row].blue_ = qc.blue();
	}

	showLegend(); 
}

//! Function to set the visual legend from a specific group color
/*!

*/
void mercadoWindow::putColorOnLegend()
{
     if(legVec_.size() == 0)
		return;

	vector<string> colorNameVec;
	getColorNameVector(strColor_, colorNameVec);
	
	colorVec_.clear();
	getColors(colorNameVec, legVec_.size(), colorVec_);

	TeVisual visual;
	if((currentTheme_->visibleRep() & TePOLYGONS) || (currentTheme_->visibleRep() & TeCELLS))
		visual = *(currentTheme_->defaultLegend().visual(TePOLYGONS));
	else if(currentTheme_->visibleRep() & TeLINES)
		visual = *(currentTheme_->defaultLegend().visual(TeLINES));
	else if(currentTheme_->visibleRep() & TePOINTS)
		visual = *(currentTheme_->defaultLegend().visual(TePOINTS));

	for(unsigned int i = 0; i < legVec_.size(); i++)
	{
		TeColor	cor = colorVec_[i];
		if(currentTheme_->visibleRep() & TePOINTS)
		{
			TeVisual* v = currentTheme_->defaultLegend().visual(TePOINTS);	
			v->color(cor);
			legVec_[i].getVisualMap()[TePOINTS] = v;
		}
		if(currentTheme_->visibleRep() & TeLINES)
		{
			TeVisual* v = currentTheme_->defaultLegend().visual(TeLINES);	
			v->color(cor);
			legVec_[i].getVisualMap()[TeLINES] = v;
		}
		if((currentTheme_->visibleRep() & TePOLYGONS) || (currentTheme_->visibleRep() & TeCELLS))
		{
			TeVisual* v = currentTheme_->defaultLegend().visual(TePOLYGONS);	
			v->color(cor);
			legVec_[i].getVisualMap()[TePOLYGONS] = v;
		}
	}
}

//! Function to show in legend table the legend information
/*!

*/
void mercadoWindow::showLegend()
{
    if(legVec_.size() == 0)
		return;

	legendTable->setNumRows(0);
	legendTable->setNumRows(legVec_.size());

	for (unsigned int i = 0; i < legVec_.size(); ++i)
	{
		TeGeomRepVisualMap& vm = legVec_[i].getVisualMap();
		createPixmap(i, vm);
		legendTable->setText(i, 1, objVec_[i].c_str());
	}

	legendTable->adjustColumn(0);
	legendTable->adjustColumn(1);
}

//! Function to generate the string group color
/*!

*/
void mercadoWindow::getColorNameVector( string & scor, vector<string> & colorNameVec )
{
     if(scor.empty())
		scor = tr("R").latin1();

	QString s = scor.c_str();
	QStringList ss = QStringList::split("-",s,true);

	for(unsigned int i = 0; i < ss.size(); i++)
	{
		QString a = ss[i];
		if(tr("R") == a)
			colorNameVec.push_back("RED");
		else if(tr("G") == a)
			colorNameVec.push_back("GREEN");
		else if(tr("B") == a)
			colorNameVec.push_back("BLUE");
		else if(tr("Cy") == a)
			colorNameVec.push_back("CYAN");
		else if(tr("Or") == a)
			colorNameVec.push_back("ORANGE");
		else if(tr("Mg") == a)
			colorNameVec.push_back("MAGENTA");
		else if(tr("Y") == a)
			colorNameVec.push_back("YELLOW");
		else
			colorNameVec.push_back("GRAY");
	} 
}

//! Function used to create a pixmap in table legend
/*!
	\param row		Current row from table legend where the pixmap will be created
	\param vm		Visual Information about the pixmap
*/
void mercadoWindow::createPixmap( int row, TeGeomRepVisualMap & vm )
{
   	int		pixh = 16;		
	int		pixw = 21;

	QPixmap	pixmap(pixw, pixh);
	pixmap.fill();

	drawPolygonRep(pixw, pixh, 0, vm, &pixmap);
	
	legendTable->setPixmap(row, 0, pixmap);  
}

//! Function used to draw a polygon in table legend
/*!
	\param w, h			Parameters to set the height and width for polygon
	\param offset		Parameter to set the offset 
	\param visual		Visual Information about the pixmap
	\param pixmap		Area used to draw the polygon
*/
void mercadoWindow::drawPolygonRep( int w, int h, int offset, TeGeomRepVisualMap & visual, QPixmap * pixmap )
{
    QPainter p(pixmap);
	QBrush	 brush;
	QColor	 cor;

	if((currentTheme_->visibleRep() & TePOLYGONS) || (currentTheme_->visibleRep() & TeCELLS))
	{
		cor.setRgb(visual[TePOLYGONS]->color().red_, visual[TePOLYGONS]->color().green_, visual[TePOLYGONS]->color().blue_);
	}
	else if(currentTheme_->visibleRep() & TePOINTS)
	{
		cor.setRgb(visual[TePOINTS]->color().red_, visual[TePOINTS]->color().green_, visual[TePOINTS]->color().blue_);
	}
	else if(currentTheme_->visibleRep() & TeLINES)
	{
		cor.setRgb(visual[TeLINES]->color().red_, visual[TeLINES]->color().green_, visual[TeLINES]->color().blue_);
	}
	else
	{
		return;
	}
	

	brush.setStyle(Qt::SolidPattern);
	brush.setColor(cor);

	QRect trect(offset+1, 1, w-2, h-2);
	QRect rect(0, 0, offset+w-1, h-1);

	int width = rect.width();
	int height = rect.height();

	int r = width%8;
	if(r)
		width += (8-r);
	r = height%8;
	if(r)
		height += (8-r);

	if(width == 0)
		width = 8;
	if(height == 0)
		height = 8;

	QBitmap	bm;
	bm.resize(width, height);
	//Fill bitmap with 0-bits: clipping region
	bm.fill(Qt::color0);
	QPainter maskPainter(&bm);

	// Draw bitmap with 1-bits: drawing region
	QBrush bs(Qt::color1, Qt::SolidPattern);
	QPen pen(Qt::color1, 1);
	maskPainter.setPen(pen);
 	maskPainter.fillRect(trect, bs);
	maskPainter.end();

	QRegion clipRegion(bm);
	p.setClipRegion(clipRegion);
	p.fillRect(trect, brush);
	p.setClipping(false);
	uint		pwidth;
	pen.setColor(cor);
	pen.setStyle(Qt::SolidLine);

	if((currentTheme_->visibleRep() & TePOLYGONS) || (currentTheme_->visibleRep() & TeCELLS))
	{
		pwidth = (Qt::PenStyle) visual[TePOLYGONS]->contourWidth();
	}
	else if(currentTheme_->visibleRep() & TePOINTS)
	{
		pwidth = (Qt::PenStyle) visual[TePOINTS]->contourWidth();
	}
	else if(currentTheme_->visibleRep() & TeLINES)
	{
		pwidth = (Qt::PenStyle) visual[TeLINES]->contourWidth();
	}
	else
	{
		return;
	}

	


	pen.setWidth (pwidth);
	p.setPen(pen);
	p.drawRect (offset+1+pwidth/2,1+pwidth/2,w-2-pwidth/2,h-2-pwidth/2);
	p.end();
}

//! Function getSelectedObjects,  used to get all objects selected by user from mercado process
/*!
	\return				
*/
void mercadoWindow::getSelectedObjects()
{
	objVec_.clear();

	int count = objectListBox->numRows();

    for(int i = 0; i < count; i++)
	{
		if(objectListBox->isSelected(i))
		{
			objVec_.push_back(objectListBox->text(i).latin1());
		}
	}
}

//! Slot adaptiveCheckBox toggled,  used to indicate if a will be used a radius value or a adaptative radius
/*!
	\return				
*/
void mercadoWindow::adaptiveCheckBox_toggled( bool state )
{
    if(!state)
	{
		radiusSlider->setEnabled(true);
		radiusLineEdit->setDisabled(false);
		radiusSlider->setValue(100);
		setRadiusForSlider(100);
	}
	else
	{
		radiusSlider->setEnabled(false);
		radiusLineEdit->setDisabled(true);
		radiusSlider->setValue(0);
		radiusLineEdit->clear();
	}
}

//! Function radiusSlider valueChanged,  used to set the value of radius defined in sliderRadius
/*!
	\return				
*/
void mercadoWindow::radiusSlider_valueChanged( int value )
{
	setRadiusForSlider(value);
}

//! Function setRadiusForSlider,  used to set the value of radius defined in sliderRadius in lineEdit
/*!
	\return				
*/
void mercadoWindow::setRadiusForSlider( int value )
{
    double maxDif = max(heightLineEdit->text().toDouble(), widthLineEdit->text().toDouble());

	radiusTextLabel->setText(Te2String(value * maxDif / 1000, 2).c_str());
}

//! Slot radiusLineEdit textChanged,  used to indicate if a value of radius is changed
/*!
	\return				
*/
void mercadoWindow::radiusLineEdit_textChanged( const QString & valueText )
{
    //setSliderForRadius(valueText.toDouble());

	radiusTextLabel->setText(valueText);
}

//! Slot setSliderForRadius,  used to set a value used for radius in mercado process
/*!
	\return				
*/
void mercadoWindow::setSliderForRadius( double radius )
{
	double maxDif = max(heightLineEdit->text().toDouble(), widthLineEdit->text().toDouble());
	radiusSlider->setValue((int)(radius*1000/maxDif));
}

//! Slot themeTableCheckBox toggled, used indicate if a mercado table will be used from a current theme or not
/*!
	\return				
*/
void mercadoWindow::themeTableCheckBox_toggled( bool status )
{
    if(status)
	{
		tabWinTableCheckBox->setChecked(false);
		externalTableCheckBox->setChecked(false);

		themeTableGroupBox->setEnabled(true);
		tabWinGroupBox->setEnabled(false);
		externalTableGroupBox->setEnabled(false);
		layerRefGroupBox->setEnabled(false);

		listThemeTables();
	}
}

void mercadoWindow::tabWinTableCheckBox_toggled( bool status)
{
	if(status)
	{
		themeTableCheckBox->setChecked(false);
		externalTableCheckBox->setChecked(false);

		themeTableGroupBox->setEnabled(false);
		tabWinGroupBox->setEnabled(true);
		externalTableGroupBox->setEnabled(false);
		layerRefGroupBox->setEnabled(true);
	}
}


void mercadoWindow::externalTableCheckBox_toggled( bool status)
{
	if(status)
	{

		themeTableCheckBox->setChecked(false);
		tabWinTableCheckBox->setChecked(false);

		themeTableGroupBox->setEnabled(false);
		tabWinGroupBox->setEnabled(false);
		externalTableGroupBox->setEnabled(true);
		layerRefGroupBox->setEnabled(true);
	}
}

//! Function listThemeTables, used to list all tables in current theme
/*!
	\return				
*/
void mercadoWindow::listThemeTables()
{
	TeAttrTableVector attrs;
	currentTheme_->getAttTables(attrs);

	if(attrs.size() == 0)
		return;

	themeTableComboBox->clear();

	for (unsigned int i = 0; i < attrs.size(); i++)
	{
		themeTableComboBox->insertItem(attrs[i].name().c_str());
	}
}

//! Function getLocalTableObjescts, used to get distinct objects from a mercado table in database
/*!
	\return				
*/
bool mercadoWindow::getLocalTableObjescts()
{
	string mercadoLabelTableName = themeTableComboBox->currentText().latin1();
		   mercadoLabelTableName += "_label";

	TeMercado* mercado = new TeMercado(currentDatabase_);

	vector<string> labels = mercado->getLabelObjects(mercadoLabelTableName);

	for (unsigned int i = 0; i < labels.size(); i++)
	{
		objectListBox->insertItem(labels[i].c_str(), i);
	}

    return true;
}

//! Function createThemeRasters, used to create a legend for all raster generated by distincts objects in mercado
/*!
	\param rasterVec	pointer to a vector of rasters
	\param objVec		list of all distincts objects in mercado
	\return				Return true if the legend create correctly and false in other case
*/
bool mercadoWindow::createThemeRasters( vector<TeRaster *> rasterVec, vector<string> objVec )
{	
	int nSlices = atoi(slicesComboBox->currentText().latin1());

	TeViewMap& viewMap = currentDatabase_->viewMap();
	TeView* currentView = viewMap[currentTheme_->view()];

	if (TeProgress::instance())
	{
		TeProgress::instance()->setCaption("Mercado");
		TeProgress::instance()->setMessage("Importing Rasters. Please, wait!");
		TeProgress::instance()->setTotalSteps(rasterVec.size());
	}

	for(unsigned int i = 0; i < rasterVec.size(); i++)
	{
		//cria novo layer
		string mercadoLayerName = layerNameLineEdit->text().latin1();
		string layerName = mercadoLayerName + "_objLayer_" + objVec[i];
		
		TeLayer* layer = new TeLayer(layerName, currentDatabase_, currentTheme_->layer()->projection());
		
		int res = TeImportRaster(layer, rasterVec[i], 512, 512, TeRasterParams::TeZLib, "O1", 0, true, TeRasterParams::TeNoExpansible);
		
		if(!res)
		{
			return false;
		}

		//cria novo tema
		string themeName = mercadoLayerName + "_objTheme_" + objVec[i];

		TeTheme *theme = new TeTheme(themeName, layer);
		theme->layer (layer);
		theme->visibleRep(layer->geomRep());
		currentView->add(theme);
		theme->visibility(true);

		TeAppTheme* themeApp = new TeAppTheme(theme);

		if(!insertAppTheme(currentDatabase_,themeApp))
		{
			return false;
		}

		//cria legenda para o raster
		TeAttributeRep rep;
		rep.type_ = TeREAL;
		rep.name_ = "0";

		TeGrouping groupKernel;
		groupKernel.groupNumSlices_ = nSlices;
		groupKernel.groupPrecision_ = 15;

		groupKernel.groupMode_ = TeRasterSlicing;
		groupKernel.groupAttribute_ = rep;

		if (!theme->buildGrouping(groupKernel))
		{
			return false;
		}
			
		vector<TeColor> colorGrad = getColors(mercTeQtColorBar->getInputColorVec(), nSlices);
		vector<ColorBar> cbVec;
		vector<ColorBar> inputColorVec = mercTeQtColorBar->getInputColorVec();
		string colors = getColors(inputColorVec, cbVec, TeEqualSteps);
		themeApp->groupColor(colors);	
		
		for (int i = 0; i < nSlices; i++)
		{

			TeVisual* visual = new TeVisual(TePOLYGONS);            
			visual->color(colorGrad[i]);
			visual->transparency(theme->defaultLegend().visual(TePOLYGONS)->transparency());
			visual->contourStyle(theme->defaultLegend().visual(TePOLYGONS)->contourStyle());
			visual->contourWidth(theme->defaultLegend().visual(TePOLYGONS)->contourWidth());
			visual->contourColor(theme->defaultLegend().visual(TePOLYGONS)->contourColor());	
			theme->setGroupingVisual(i+1,visual,TePOLYGONS);
		}

		themeApp->countObj(false);
		themeApp->mixColor(true);
		themeApp->groupColorDir(TeColorAscSatEnd);
		
		if (!theme->saveGrouping())
		{
			return false;
		}

		theme->visibleRep(theme->visibleRep() | 0x40000000);


		if(!updateAppTheme(currentDatabase_, themeApp))
		{
			return false;
		}

		themeApp->setAlias("0",tr("Object Mercado").latin1());

		TeProgress::instance()->setProgress(i);
		
	}

	if (TeProgress::instance())
		TeProgress::instance()->reset();

    return true;
}

//! Function createMercadoThemeDom, used to create a legend for raster dominance from mercado
/*!
	\param rasterDom	pointer to a dominance raster
	\param vecObj		list of all dominance objects that appears in raster
	\return				Return true if the legend create correctly and false in other case
*/
bool mercadoWindow::createMercadoThemeDom( TeRaster * rasterDom, vector<string> vecObj )
{
	TeViewMap& viewMap = currentDatabase_->viewMap();
	TeView* currentView = viewMap[currentTheme_->view()];

	//cria novo layer
	string layerName = layerNameLineEdit->text().latin1();
	
	TeLayer* layer = new TeLayer(layerName, currentDatabase_, currentTheme_->layer()->projection());
	
	int res = TeImportRaster(layer, rasterDom, 512, 512, TeRasterParams::TeZLib, "O1", 0, true, TeRasterParams::TeNoExpansible);
	
	if(!res)
	{
		return false;
	}

	//cria novo tema
	string themeName = "theme_" + layerName;

	TeTheme *theme = new TeTheme(themeName, layer);
	theme->layer (layer);
	theme->visibleRep(layer->geomRep());
	currentView->add(theme);
	theme->visibility(true);

	TeAppTheme* themeApp = new TeAppTheme(theme);

	if(!insertAppTheme(currentDatabase_,themeApp))
	{
		return false;
	}

	TeVisual* visual = new TeVisual(TePOLYGONS);            
	visual->color(TeColor(255,255,255));
	visual->transparency(theme->defaultLegend().visual(TePOLYGONS)->transparency());
	visual->contourStyle(theme->defaultLegend().visual(TePOLYGONS)->contourStyle());
	visual->contourWidth(theme->defaultLegend().visual(TePOLYGONS)->contourWidth());
	visual->contourColor(theme->defaultLegend().visual(TePOLYGONS)->contourColor());	

	TeLegendEntry legend;
	legend.label(string("No Dominance"));
	legend.setVisual(visual, TePOLYGONS);
	legend.group(0);
	legend.theme(theme->id());
	theme->legend().push_back(legend);


	vector<TeSlice> slices;
	TeGroupByUniqueValue(vecObj, TeSTRING, slices, 6);

	for(unsigned int i=0; i<slices.size(); ++i)
	{
		TeVisual* visual = new TeVisual(TePOLYGONS);            
		visual->color(colorVec_[i]);
		visual->transparency(theme->defaultLegend().visual(TePOLYGONS)->transparency());
		visual->contourStyle(theme->defaultLegend().visual(TePOLYGONS)->contourStyle());
		visual->contourWidth(theme->defaultLegend().visual(TePOLYGONS)->contourWidth());
		visual->contourColor(theme->defaultLegend().visual(TePOLYGONS)->contourColor());	

		TeLegendEntry legend(slices[i]);
		legend.label(vecObj[i]);
		legend.setVisual(visual, TePOLYGONS);
		legend.group(i+1);
		legend.theme(theme->id());
		theme->legend().push_back(legend);
	}

	themeApp->setAlias("0",tr("Dominance Mercado").latin1());

    return true;
}

bool mercadoWindow::createMercadoThemeSec( TeRaster * rasterSec, vector<string> vecObj )
{
	TeViewMap& viewMap = currentDatabase_->viewMap();
	TeView* currentView = viewMap[currentTheme_->view()];

	//cria novo layer
	string layerName = layerNameLineEdit->text().latin1();
	layerName+= "_sec";
	
	TeLayer* layer = new TeLayer(layerName, currentDatabase_, currentTheme_->layer()->projection());
	
	int res = TeImportRaster(layer, rasterSec, 512, 512, TeRasterParams::TeZLib, "O1", 0, true, TeRasterParams::TeNoExpansible);
	
	if(!res)
	{
		return false;
	}

	//cria novo tema
	string themeName = "theme_" + layerName;

	TeTheme *theme = new TeTheme(themeName, layer);
	theme->layer (layer);
	theme->visibleRep(layer->geomRep());
	currentView->add(theme);
	theme->visibility(true);

	TeAppTheme* themeApp = new TeAppTheme(theme);

	if(!insertAppTheme(currentDatabase_,themeApp))
	{
		return false;
	}

	TeVisual* visual = new TeVisual(TePOLYGONS);            
	visual->color(TeColor(255,255,255));
	visual->transparency(theme->defaultLegend().visual(TePOLYGONS)->transparency());
	visual->contourStyle(theme->defaultLegend().visual(TePOLYGONS)->contourStyle());
	visual->contourWidth(theme->defaultLegend().visual(TePOLYGONS)->contourWidth());
	visual->contourColor(theme->defaultLegend().visual(TePOLYGONS)->contourColor());	

	TeLegendEntry legend;
	legend.label(string("No Dominance"));
	legend.setVisual(visual, TePOLYGONS);
	legend.group(0);
	legend.theme(theme->id());
	theme->legend().push_back(legend);


	vector<TeSlice> slices;
	TeGroupByUniqueValue(vecObj, TeSTRING, slices, 6);

	for(unsigned int i=0; i<slices.size(); ++i)
	{
		TeVisual* visual = new TeVisual(TePOLYGONS);            
		visual->color(colorVec_[i]);
		visual->transparency(theme->defaultLegend().visual(TePOLYGONS)->transparency());
		visual->contourStyle(theme->defaultLegend().visual(TePOLYGONS)->contourStyle());
		visual->contourWidth(theme->defaultLegend().visual(TePOLYGONS)->contourWidth());
		visual->contourColor(theme->defaultLegend().visual(TePOLYGONS)->contourColor());	

		TeLegendEntry legend(slices[i]);
		legend.label(vecObj[i]);
		legend.setVisual(visual, TePOLYGONS);
		legend.group(i+1);
		legend.theme(theme->id());
		theme->legend().push_back(legend);
	}

	themeApp->setAlias("0",tr("Sec Dominance Mercado").latin1());

    return true;
}


bool mercadoWindow::createMercadoThemeTer( TeRaster * rasterTer, vector<string> vecObj )
{
	TeViewMap& viewMap = currentDatabase_->viewMap();
	TeView* currentView = viewMap[currentTheme_->view()];

	//cria novo layer
	string layerName = layerNameLineEdit->text().latin1();
	layerName+= "_ter";
	
	TeLayer* layer = new TeLayer(layerName, currentDatabase_, currentTheme_->layer()->projection());
	
	int res = TeImportRaster(layer, rasterTer, 512, 512, TeRasterParams::TeZLib, "O1", 0, true, TeRasterParams::TeNoExpansible);
	
	if(!res)
	{
		return false;
	}

	//cria novo tema
	string themeName = "theme_" + layerName;

	TeTheme *theme = new TeTheme(themeName, layer);
	theme->layer (layer);
	theme->visibleRep(layer->geomRep());
	currentView->add(theme);
	theme->visibility(true);

	TeAppTheme* themeApp = new TeAppTheme(theme);

	if(!insertAppTheme(currentDatabase_,themeApp))
	{
		return false;
	}

	TeVisual* visual = new TeVisual(TePOLYGONS);            
	visual->color(TeColor(255,255,255));
	visual->transparency(theme->defaultLegend().visual(TePOLYGONS)->transparency());
	visual->contourStyle(theme->defaultLegend().visual(TePOLYGONS)->contourStyle());
	visual->contourWidth(theme->defaultLegend().visual(TePOLYGONS)->contourWidth());
	visual->contourColor(theme->defaultLegend().visual(TePOLYGONS)->contourColor());	

	TeLegendEntry legend;
	legend.label(string("No Dominance"));
	legend.setVisual(visual, TePOLYGONS);
	legend.group(0);
	legend.theme(theme->id());
	theme->legend().push_back(legend);


	vector<TeSlice> slices;
	TeGroupByUniqueValue(vecObj, TeSTRING, slices, 6);

	for(unsigned int i=0; i<slices.size(); ++i)
	{
		TeVisual* visual = new TeVisual(TePOLYGONS);            
		visual->color(colorVec_[i]);
		visual->transparency(theme->defaultLegend().visual(TePOLYGONS)->transparency());
		visual->contourStyle(theme->defaultLegend().visual(TePOLYGONS)->contourStyle());
		visual->contourWidth(theme->defaultLegend().visual(TePOLYGONS)->contourWidth());
		visual->contourColor(theme->defaultLegend().visual(TePOLYGONS)->contourColor());	

		TeLegendEntry legend(slices[i]);
		legend.label(vecObj[i]);
		legend.setVisual(visual, TePOLYGONS);
		legend.group(i+1);
		legend.theme(theme->id());
		theme->legend().push_back(legend);
	}

	themeApp->setAlias("0",tr("Ter Dominance Mercado").latin1());

    return true;
}


//! Slot clear push button clicked, used to clear the legend
/*!
	
*/
void mercadoWindow::clearPushButton_clicked()
{
    mercTeQtColorBar->clearColorBar();
}

//! Slot invert push button clicked, used to invert the legend
/*!
	
*/
void mercadoWindow::invertPushButton_clicked()
{
    mercTeQtColorBar->invertColorBar();
}

//! Slot equal push button clicked, used to make legend steps equals
/*!
	
*/
void mercadoWindow::equalPushButton_clicked()
{
	mercTeQtColorBar->setEqualSpace();
}


void mercadoWindow::kernelRadioButton_clicked()
{
    gridGroupBox->setEnabled(true);
	set_1AlgorithmGroupBox->setEnabled(true);
}


void mercadoWindow::vecRadioButton_clicked()
{
	gridGroupBox->setEnabled(false);
	set_1AlgorithmGroupBox->setEnabled(false);
}

//! Slot numColsLineEdit_textChanged,  used to indicate that a number of colluns was changed
/*!
	\return				
*/
void mercadoWindow::numColsLineEdit_textChanged( const QString & nCols )
{
	if(nCols.isEmpty() || numLinesLineEdit->text().isEmpty())
	{
		return;
	}

	int numCols = nCols.toInt();
	int numLines = numLinesLineEdit->text().toInt();

	TeBox boxTheme = currentTheme_->box();

	heightLineEdit->setText(Te2String(boxTheme.height(), 2).c_str());
	widthLineEdit->setText(Te2String(boxTheme.width(), 2).c_str());

	TeRasterParams params;
	params.boundingBoxLinesColumns(boxTheme.x1(), boxTheme.y1(), boxTheme.x2(), boxTheme.y2(), numLines, numCols);

	resXLineEdit->setText(Te2String(params.resx_, 2).c_str());
	resYLineEdit->setText(Te2String(params.resy_, 2).c_str());

	return;
}

void mercadoWindow::numLinesLineEdit_textChanged( const QString & nLines)
{
	if(nLines.isEmpty() || numColsLineEdit->text().isEmpty())
	{
		return;
	}

	int numCols =  numColsLineEdit->text().toInt();
	int numLines = nLines.toInt();

	TeBox boxTheme = currentTheme_->box();

	heightLineEdit->setText(Te2String(boxTheme.height(), 2).c_str());
	widthLineEdit->setText(Te2String(boxTheme.width(), 2).c_str());

	TeRasterParams params;
	params.boundingBoxLinesColumns(boxTheme.x1(), boxTheme.y1(), boxTheme.x2(), boxTheme.y2(), numLines, numCols);

	resXLineEdit->setText(Te2String(params.resx_, 2).c_str());
	resYLineEdit->setText(Te2String(params.resy_, 2).c_str());
}


void mercadoWindow::resYLineEdit_textChanged( const QString & resY )
{
	if(resY.isEmpty() || resXLineEdit->text().isEmpty())
	{
		return;
	}

	double resYValue = resY.toDouble();
	double resXValue = resXLineEdit->text().toDouble();

	TeBox boxTheme = currentTheme_->box();

	heightLineEdit->setText(Te2String(boxTheme.height(), 2).c_str());
	widthLineEdit->setText(Te2String(boxTheme.width(), 2).c_str());

	TeRasterParams params;
	params.boundingBoxResolution(boxTheme.x1(), boxTheme.y1(), boxTheme.x2(), boxTheme.y2(), resXValue, resYValue);

	numColsLineEdit->setText(Te2String(params.ncols_, 0).c_str());
	numLinesLineEdit->setText(Te2String(params.nlines_, 0).c_str());
}


void mercadoWindow::resXLineEdit_textChanged( const QString & resX )
{
	if(resX.isEmpty() || resYLineEdit->text().isEmpty())
	{
		return;
	}

	double resYValue = resYLineEdit->text().toDouble();
	double resXValue = resX.toDouble();

	TeBox boxTheme = currentTheme_->box();

	heightLineEdit->setText(Te2String(boxTheme.height(), 2).c_str());
	widthLineEdit->setText(Te2String(boxTheme.width(), 2).c_str());

	TeRasterParams params;
	params.boundingBoxResolution(boxTheme.x1(), boxTheme.y1(), boxTheme.x2(), boxTheme.y2(), resXValue, resYValue);

	numColsLineEdit->setText(Te2String(params.ncols_, 0).c_str());
	numLinesLineEdit->setText(Te2String(params.nlines_, 0).c_str());
}


void mercadoWindow::manualRadioButton_clicked()
{
    radiusSlider->setEnabled(false);
	radiusLineEdit->setEnabled(true);
}


void mercadoWindow::semiAutoRadioButton_clicked()
{
    radiusSlider->setEnabled(true);
	radiusLineEdit->setEnabled(false);
}


void mercadoWindow::selObjCheckBox_clicked()
{
	if(selObjCheckBox->isChecked())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Apenas os dados referentes aos objetos selecionados serão utilizados nos calculos."));
	}
}


void mercadoWindow::tableFilePushButton_clicked()
{
	QString file = QFileDialog::getOpenFileName(QString::null, "CSV File (*.csv)", this);

	tableNameLineEdit->setText(file);
}

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
#include <HidroEdgeFunctions.h>
#include <HidroVertexFunctions.h>

#include <qmessagebox.h>

void HidroAttributeWindow::init(PluginParameters * parameters)
{
	_parameters = parameters;

    attrGroupBox->setEnabled(false);

	//list all graphs from current database
	listGraphs();
}


void HidroAttributeWindow::graphComboBox_activated( const QString & value )
{
	if(value.isEmpty())
	{
		attrGroupBox->setEnabled(false);
		return;
	}

	attrGroupBox->setEnabled(true);
}


void HidroAttributeWindow::vertexInfoRadioButton_clicked()
{
	if(vertexInfoRadioButton->isChecked())
	{
		listVertexAttributes();
	}
}


void HidroAttributeWindow::edgeInfoRadioButton_clicked()
{
	if(edgeInfoRadioButton->isChecked())
	{
		listEdgeAttributes();
	}
}


void HidroAttributeWindow::fromPolygonsRadioButton_clicked()
{
	addAttrClassComboBox->setEnabled(true);

	listLayers();
}


void HidroAttributeWindow::fromRasterRadioButton_clicked()
{
	addAttrClassComboBox->setEnabled(false);

	listLayers();
}


void HidroAttributeWindow::addAttrLayerComboBox_activated( const QString & value )
{
	if(value.isEmpty())
	{
		return;
	}

	if(fromPolygonsRadioButton->isChecked())
	{
		std::string layerName = value.latin1();

		HidroUtils utils(_parameters->getCurrentDatabasePtr());

		TeLayer* layer = utils.getLayerByName(layerName);

		if(layer)
		{
			std::vector<std::string> attrList;
			layer->attrTables()[0].attributeNames(attrList);

			addAttrClassComboBox->clear();
			addAttrClassComboBox->insertItem("");

			for(unsigned int i = 0; i < attrList.size(); ++i)
			{
				addAttrClassComboBox->insertItem(attrList[i].c_str());
			}
		}
	}
}


void HidroAttributeWindow::addAttrPushButton_clicked()
{
//verify input parameters
	if(addAttrNameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Attribute name not defined."));
		return;
	}

	if(addAttrLayerComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Layer not defined."));
		return;
	}

	if(fromPolygonsRadioButton->isChecked())
	{
		if(addAttrClassComboBox->currentText().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Attribute not defined from selected layer."));
			return;
		}
	}

//get parameters
	std::string graphName	= graphComboBox->currentText().latin1();
	std::string attrName	= addAttrNameLineEdit->text().latin1();
	std::string layerName	= addAttrLayerComboBox->currentText().latin1();
	std::string layerAttrName = "";

	if(fromPolygonsRadioButton->isChecked())
	{
		layerAttrName = addAttrClassComboBox->currentText().latin1();
	}

	HidroUtils utils(_parameters->getCurrentDatabasePtr());
	
	TeLayer* layer = utils.getLayerByName(layerName);

	if(!layer)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Error getting input layer."));
		return;
	}

//add new attribute in graph
	HidroVertexFunctions vertexFunc(_parameters->getCurrentDatabasePtr());

	if(fromPolygonsRadioButton->isChecked())
	{
		if(!vertexFunc.createAttributeFromPolygons(graphName, layer, layerAttrName, attrName))
		{
			QMessageBox::warning(this, tr("Warning"), vertexFunc.getErrorMessage().c_str());
			return;
		}
	}
	else
	{
		if(!vertexFunc.createAttributeFromRaster(graphName, layer, attrName))
		{
			QMessageBox::warning(this, tr("Warning"), vertexFunc.getErrorMessage().c_str());
			return;
		}
	}

//add new entry in attr attribute table
	HidroMetadata metadata(_parameters->getCurrentDatabasePtr());

	if(!metadata.addGraphVertexAttrEntry(graphName, attrName, layerName, layerAttrName))
	{
		QMessageBox::warning(this, tr("Warning"), metadata.getErrorMessage().c_str());
		return;
	}

	QMessageBox::warning(this, tr("Warning"), tr("Attribute added."));
}

void HidroAttributeWindow::vertexRadioButton_clicked()
{
    listGraphAttributes(true);
}


void HidroAttributeWindow::edgeRadioButton_clicked()
{
    listGraphAttributes(false);
}


void HidroAttributeWindow::removeAttrPushButton_clicked()
{
	if(removeAttrComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Attribute not selected."));
		return;
	}

	std::string graphName	= graphComboBox->currentText().latin1();
	std::string attrName	= removeAttrComboBox->currentText().latin1();
	bool isVertexAttribute	= vertexRadioButton->isChecked();

	HidroMetadata metadata(_parameters->getCurrentDatabasePtr());

//remove entry from attr table
	if(!metadata.removeGraphAttrEntry(graphName, attrName, isVertexAttribute))
	{
		QMessageBox::warning(this, tr("Warning"), metadata.getErrorMessage().c_str());
		return;
	}

//remove attribute from table
	if(!metadata.removeGraphAttr(graphName, attrName, isVertexAttribute))
	{
		QMessageBox::warning(this, tr("Warning"), metadata.getErrorMessage().c_str());
		return;
	}

//refresh interface and application
	listGraphAttributes(isVertexAttribute);

	_parameters->updateTVInterface();

	QMessageBox::warning(this, tr("Warning"), tr("Attribute removed."));
}


void HidroAttributeWindow::edgeDistCheckBox_clicked()
{
    if(edgeDistCheckBox->isChecked())
	{
		edgeDistAttrNameLineEdit->setEnabled(true);
	}
	else 
	{
		edgeDistAttrNameLineEdit->setEnabled(false);
	}
}


void HidroAttributeWindow::execPushButton_clicked()
{
	std::string graphName	= graphComboBox->currentText().latin1();

	HidroMetadata metadata(_parameters->getCurrentDatabasePtr());

//execute distance operation
	if(edgeDistCheckBox->isChecked())
	{
		if(edgeDistAttrNameLineEdit->text().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Attribute name for Edge distance is not defined."));
			return;
		}

		//verify if already exist this attribute
		std::string attrName = edgeDistAttrNameLineEdit->text().latin1();

		if(metadata.existGraphAttrEntry(graphName, attrName, false))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Attribute name for Edge distance already exist."));
			return;
		}

		//create edge distance attribute
		HidroEdgeFunctions edgeFunc(_parameters->getCurrentDatabasePtr());

		if(!edgeFunc.createDistanceAttribute(graphName, attrName))
		{
			QMessageBox::warning(this, tr("Warning"), edgeFunc.getErrorMessage().c_str());
			return;
		}

		//add edge distance attribute in edge attribute table
		if(!metadata.addGraphEdgeAttrEntry(graphName, attrName))
		{
			QMessageBox::warning(this, tr("Warning"), metadata.getErrorMessage().c_str());
			return;
		}
	}

	QMessageBox::warning(this, tr("Warning"), tr("Operations Done."));
}

/** \brief HidroAttributeWindow listGraphs function.
    * Used to list all graph names from current database
	* inside a combo box in interface.
    */
void HidroAttributeWindow::listGraphs()
{
	graphComboBox->clear();

	HidroMetadata metadata(_parameters->getCurrentDatabasePtr());

	std::vector<std::string> graphList = metadata.getGraphNameListFromDB();

	graphComboBox->insertItem("");

	for(unsigned int i =0; i < graphList.size(); ++i)
	{
		graphComboBox->insertItem(graphList[i].c_str());
	}
}

void HidroAttributeWindow::listGraphAttributes(bool vertexAttrs)
{
	if(graphComboBox->currentText().isEmpty())
	{
		return;
	}

	std::string graphName = graphComboBox->currentText().latin1();

	HidroMetadata metadata(_parameters->getCurrentDatabasePtr());

	std::vector<GraphAttr> attr = metadata.getGraphAttrList(graphName, vertexAttrs);
	
	removeAttrComboBox->clear();
	removeAttrComboBox->insertItem("");

	for(unsigned int i = 0; i < attr.size(); ++i)
	{
		removeAttrComboBox->insertItem(attr[i].attrName.c_str());
	}
}

void HidroAttributeWindow::listVertexAttributes()
{
	infoTable->setNumRows(0);

	std::string graphName = graphComboBox->currentText().latin1();

	HidroMetadata metadata(_parameters->getCurrentDatabasePtr());

	std::vector<GraphAttr> attr = metadata.getGraphAttrList(graphName, true);
	
	infoTable->setNumRows(attr.size());

	for(unsigned int i = 0; i < attr.size(); ++i)
	{
		infoTable->setText(i, 0, attr[i].attrName.c_str());
		infoTable->setText(i, 1, attr[i].layerName.c_str());

		if(!attr[i].layerAttrName.empty())
		{
			infoTable->setText(i, 2, attr[i].layerAttrName.c_str());
		}
	}
}

void HidroAttributeWindow::listEdgeAttributes()
{
	infoTable->setNumRows(0);

	std::string graphName = graphComboBox->currentText().latin1();

	HidroMetadata metadata(_parameters->getCurrentDatabasePtr());

	std::vector<GraphAttr> attr = metadata.getGraphAttrList(graphName, false);
	
	infoTable->setNumRows(attr.size());

	for(unsigned int i = 0; i < attr.size(); ++i)
	{
		infoTable->setText(i, 0, attr[i].attrName.c_str());
		infoTable->setText(i, 1, attr[i].layerName.c_str());

		if(!attr[i].layerAttrName.empty())
		{
			infoTable->setText(i, 2, attr[i].layerAttrName.c_str());
		}
	}
}

/** \brief HidroAttributeWindow listLayers function.
    * Used to list all layer names from current database
	* that has TeRASTER or TePOLYGONS representation inside a combo box
	* in interface.
    */
void HidroAttributeWindow::listLayers()
{
	if(!_parameters->getCurrentDatabasePtr())
	{
		return;
	}

//clear all layers names inside the combo box
    addAttrLayerComboBox->clear();
	addAttrLayerComboBox->insertItem("");

	HidroUtils utils(_parameters->getCurrentDatabasePtr());

	std::vector<std::string> layerNames;

	if(fromRasterRadioButton->isChecked())
	{
		layerNames = utils.listLayers(true, false);
	}
	else if(fromPolygonsRadioButton->isChecked())
	{
		layerNames = utils.listLayers(false, true);
	}

	for(unsigned int i = 0; i < layerNames.size(); ++i)
	{
		addAttrLayerComboBox->insertItem(layerNames[i].c_str());
	}
}
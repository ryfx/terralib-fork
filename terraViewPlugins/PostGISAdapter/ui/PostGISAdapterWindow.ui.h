#include <TeDatabase.h>
#include <TeDatabaseIndex.h>
#include <PluginParameters.h>
#include <TeWaitCursor.h>
#include <help.h>

//Qt include files
#include <qmainwindow.h>
#include <qlistview.h>
#include <qmessagebox.h>
	
std::string getSRID(TeDatabase* database, const std::string& tableName)
{
	std::string sql = "select srid from geometry_columns where f_table_name = '" + tableName + "'";

	TeDatabasePortal* portal = database->getPortal();
	if(!portal->query(sql) || !portal->fetchRow())
	{
		delete portal;
		return "";
	}
	
	std::string srid = portal->getData(0);

	delete portal;
	portal = NULL;

	return srid;
}

std::string getRepresentation(TeDatabase* database, const std::string& tableName)
{
	std::string sql = "select type from geometry_columns where f_table_name = '" + tableName + "'";

	TeDatabasePortal* portal = database->getPortal();
	if(!portal->query(sql) || !portal->fetchRow())
	{
		delete portal;
		return "";
	}
	
	std::string type = portal->getData(0);

	delete portal;
	portal = NULL;

	return type;
}

void PostGISAdapterWindow::init()
{
	layerCreated_ = false;
	help_ = NULL;
}


void PostGISAdapterWindow::destroy()
{

}

void PostGISAdapterWindow::initFromPluginParameters()
{
	if(plugPars_ == NULL)
	{
		return;
	}
}

void PostGISAdapterWindow::updateForm( PluginParameters * pparams )
{
	layerCreated_ = false;
	layersListBox->clear();

	plugPars_ = pparams;
	if(plugPars_ == NULL)
	{
		return;
	}

	TeDatabase* database = plugPars_->getCurrentDatabasePtr();
	if(database == NULL)
	{
		QMessageBox::critical(plugPars_->qtmain_widget_ptr_, tr("Error"), tr("There is no database connected!"));
		return;
	}
	if(database->dbmsName() != "PostGIS")
	{
		QMessageBox::information(plugPars_->qtmain_widget_ptr_, tr("Error"), tr("Plugin available only for an PostGIS Database!"));
		return;
	}

	TeWaitCursor waitCursor;
	waitCursor.setWaitCursor();

	TeDatabasePortal* portal = database->getPortal();

	std::string sql = "select f_table_name from geometry_columns";
	sql += " where upper(f_table_name) NOT IN(select upper(geom_table) from te_representation)";
	if(!portal->query(sql))
	{
		delete portal;
		waitCursor.resetWaitCursor();
		return;
	}
	while(portal->fetchRow())
	{
		layersListBox->insertItem(portal->getData("f_table_name"));
	}

	delete portal;
	portal = NULL;

	waitCursor.resetWaitCursor();

	exec();
}

void PostGISAdapterWindow::addPushButton_clicked()
{
	if(layersListBox->count() == 0)
	{
		return;
	}

	TeDatabase* database = plugPars_->getCurrentDatabasePtr();
	if(database == NULL)
	{
		return;
	}	

	TeWaitCursor waitCursor;
	waitCursor.setWaitCursor();

	unsigned int i = 0;
	while(i < layersListBox->count())
	{
		if(layersListBox->item(i)->isSelected())
		{
			std::string rep = getRepresentation(database, layersListBox->item(i)->text().latin1());
			if(rep != "POLYGON" && rep != "MULTIPOLYGON" && rep != "LINE" && rep != "POINT")
			{
				waitCursor.resetWaitCursor();
				QMessageBox::information(this, tr("PostGIS Adapter Plugin"), tr("The table must have POINT, LINE or POLYGON representation!"));
				return;
			}
			std::string srid = getSRID(database, layersListBox->item(i)->text().latin1());
			if(srid.empty())
			{
				waitCursor.resetWaitCursor();
				QMessageBox::information(this, tr("PostGIS Adapter Plugin"), tr("The data '") + layersListBox->item(i)->text() + tr("' has no information about its projection. Choose another one!"));
				return;
			}
			TeProjection* proj = TeProjectionFactory::make(atoi(srid.c_str()));
			if(proj == 0)
			{
				waitCursor.resetWaitCursor();
				QMessageBox::information(this, tr("PostGIS Adapter Plugin"), tr("The data '") + layersListBox->item(i)->text() + tr("' has an incompatible projection. Choose another one!"));
				return;
			}
			delete proj;
			if(database->layerExist(layersListBox->item(i)->text().latin1()))
			{
				waitCursor.resetWaitCursor();
				QMessageBox::information(this, tr("PostGIS Adapter Plugin"), tr("There is already a layer if the same name as '") + layersListBox->item(i)->text() + tr("'."));
				return;
			}

			selectedLayersListBox->insertItem(layersListBox->item(i)->text());
			layersListBox->removeItem(i);
			break;
		}
		else
		{
			i++;
		}
	}

	waitCursor.resetWaitCursor();
}


void PostGISAdapterWindow::addAllPushButton_clicked()
{	
	if(layersListBox->count() == 0)
		return;

	TeDatabase* database = plugPars_->getCurrentDatabasePtr();
	if(database == NULL)
	{
		return;
	}

	TeWaitCursor waitCursor;
	waitCursor.setWaitCursor();

	for (unsigned int i=0; i < layersListBox->count();i++)
	{
		if (selectedLayersListBox->findItem(layersListBox->text(i)) == 0)
		{
			std::string rep = getRepresentation(database, layersListBox->item(i)->text().latin1());
			if(rep != "POLYGON" && rep != "MULTIPOLYGON" && rep != "LINE" && rep != "MULTILINE" && rep != "POINT" && rep != "MULTIPOINT")
			{
				waitCursor.resetWaitCursor();
				QMessageBox::information(this, tr("PostGIS Adapter Plugin"), tr("The table must have POINT, LINE or POLYGON representation!"));
				return;
			}
			std::string srid = getSRID(database, layersListBox->item(i)->text().latin1());
			if(srid.empty())
			{
				waitCursor.resetWaitCursor();
				QMessageBox::information(this, tr("PostGIS Adapter Plugin"), tr("The data '") + layersListBox->item(i)->text() + tr("' has an incompatible projection. Choose another one!"));
				continue;
			}
			TeProjection* proj = TeProjectionFactory::make(atoi(srid.c_str()));
			if(proj == 0)
			{
				waitCursor.resetWaitCursor();
				QMessageBox::information(this, tr("PostGIS Adapter Plugin"), tr("The data '") + layersListBox->item(i)->text() + tr("' has an incompatible projection. Choose another one!"));
				return;
			}			
			delete proj;

			if(database->layerExist(layersListBox->item(i)->text().latin1()))
			{
				waitCursor.resetWaitCursor();
				QMessageBox::information(this, tr("PostGIS Adapter Plugin"), tr("There is already a layer if the same name as '") + layersListBox->item(i)->text() + tr("'."));
				continue;
			}

			selectedLayersListBox->insertItem(layersListBox->text(i));
			layersListBox->removeItem(i);
		}
	}
	waitCursor.resetWaitCursor();
}


void PostGISAdapterWindow::removePushButton_clicked()
{
	for(unsigned int i=0; i < selectedLayersListBox->count(); i++)
	{
		if(selectedLayersListBox->item(i)->isSelected())
		{
			layersListBox->insertItem(selectedLayersListBox->item(i)->text());
			selectedLayersListBox->removeItem(i);
			break;
		}
	}
}


void PostGISAdapterWindow::removeAllPushButton_clicked()
{
	while(selectedLayersListBox->count()>0)
	{
		layersListBox->insertItem(selectedLayersListBox->item(0)->text());
		selectedLayersListBox->removeItem(0);
	}
}


void PostGISAdapterWindow::createPushButton_clicked()
{
	TeWaitCursor waitCursor;	
	waitCursor.setWaitCursor();

	TeDatabase* database = plugPars_->getCurrentDatabasePtr();

//monta a lista de tabelas espaciais a serem inseridas no modelo terralib
	unsigned int success = 0;
	unsigned int errors = 0;
	for(unsigned int i=0; i < selectedLayersListBox->count(); i++)
	{
		std::string tableName = selectedLayersListBox->text(i).latin1();
		std::string errorMessage = "";

		if(!database->beginTransaction())
		{
		}

		if(!spatialToTerralib(tableName, errorMessage))
		{
			database->rollbackTransaction();
			waitCursor.resetWaitCursor();
			QMessageBox::critical(this, tr("PostGIS Adapter Plugin"), tr("Error creating model: ") + errorMessage.c_str());
			waitCursor.setWaitCursor();
			++errors;
			continue;
		}
		
		if(database->commitTransaction())
		{
			database->rollbackTransaction();
		}

		++success;
	}

	if(success > 0)
	{
		layerCreated_ = true;
	}

	QString message = tr("Finished with ") + QString(Te2String(success).c_str()) + tr(" table(s) imported successfully and ") + QString(Te2String(errors).c_str()) + tr(" errors!");

	QMessageBox::information(this, tr("PostGIS Adapter Plugin"), message);

	waitCursor.resetWaitCursor();	
}


void PostGISAdapterWindow::exitPushButton_clicked()
{
	if (help_)
	{
		delete help_;
		help_ = 0;
	}

	if(layerCreated_ == true)
	{
		if(plugPars_ != NULL)
		{
			plugPars_->updateTVInterface();
		}
	}
	accept();
}


void PostGISAdapterWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("PluginPostGISAdapter.htm");
	if(help_->erro_ == false)
	{
		help_->show();
		help_->raise();
	}
	else
	{
		delete help_;
		help_ = 0;
	}
}

int PostGISAdapterWindow::getMaxValueFromColumn(const std::string& tableName, const std::string& columnName)
{
	if(plugPars_ == NULL)
	{
		return -1;
	}

	TeDatabase* database = plugPars_->getCurrentDatabasePtr();
	if(database == NULL)	
	{
		return -1;
	}

	TeDatabasePortal* portal = database->getPortal();
	if(portal == NULL)
	{
		return -1;
	}

	std::string sqlMax = "select max(" + columnName + ") from " + tableName;
    if(!portal->query(sqlMax) || !portal->fetchRow())
	{
		delete portal;
		return -1;
	}

	int value = portal->getInt(0);

	delete portal;

	return value;

}

bool PostGISAdapterWindow::spatialToTerralib(const std::string& tableName, std::string& errorMessage)
{
	errorMessage = "";

	std::string geomIdColumnName = "te_geom_id";
	std::string oidColumnName = "te_object_id";
	std::string seqName = "te_"+tableName+"_seq";


	if(plugPars_ == NULL)
	{
		return false;
	}

	TeDatabase* database = plugPars_->getCurrentDatabasePtr();
	if(database == NULL)	
	{
		return false;
	}

	TeDatabasePortal* portal = database->getPortal();

	TeLayerMap& layerMap = database->layerMap();
	
	//se o layer ja existir, remove-o
	bool goToNextData = false;

	TeLayerMap::iterator it = layerMap.begin();
	while(it != layerMap.end())
	{
		if(TeConvertToUpperCase(it->second->name()) == TeConvertToUpperCase(tableName))
		{				
			goToNextData = true;
			break;
		}
		++it;
	}

	if(goToNextData == true)
	{
		errorMessage = tr("There is a layer with the same name as the spatial table.").latin1();
		return false;
	}
	

	TeAttribute aux;
	
	bool geomIdColumnExists = database->columnExist(tableName, geomIdColumnName, aux);	
	
	bool oidColumnExists = database->columnExist(tableName, oidColumnName, aux);	


	//Criando a sequencia
	std::string sqlCreateSeq = "CREATE SEQUENCE "+seqName+";";

	if(!database->tableExist(seqName))
	{
		if(!database->execute(sqlCreateSeq))
		{
			delete portal;
			errorMessage = tr("Error Creating the Sequence.").latin1();
			return false;
		}
	}
	else
	{
		std::string initSeq = "select setval('"+seqName+"', 1)";

		if(!database->execute(initSeq))
		{
			delete portal;
			errorMessage = tr("Error Starting the Sequence.").latin1();
			return false;
		}
	}	


	if(geomIdColumnExists == false)
	{
		TeAttributeRep rep;
		rep.name_ = geomIdColumnName;
		rep.type_ = TeINT;
		rep.defaultValue_ = "nextval('" + seqName + "')";
				
		if(!database->addColumn(tableName, rep))
		{
			delete portal;
			errorMessage = tr("Error adding column.").latin1();
			return false;
		}	
	}

	if(oidColumnExists == false)
	{
		TeAttributeRep rep;
		rep.name_ = oidColumnName;
		rep.type_ = TeSTRING;
		rep.defaultValue_ = "(currval('" + seqName + "'::regclass))::character varying(255)";
		rep.numChar_ = 255;

		if(!database->addColumn(tableName, rep))
		{
			delete portal;
			errorMessage = tr("Error adding column.").latin1();
			return false;
		}		

		TeDatabaseIndex idx;
		idx.setIndexName(tableName+"idx");
		std::vector<std::string> idxCol;
		idxCol.push_back(oidColumnName);
		idx.setColumns(idxCol);
		idx.setIsPrimaryKey(false);

		if(!database->createIndex(tableName, idx.getIndexName(), idxCol[0]))
		{
			delete portal;
			errorMessage = tr("Error creating index.").latin1();
			return false;
		}		
	}	

	
	std::string updateTable = "update " + tableName + " set "+oidColumnName+" = "+geomIdColumnName+"::character varying(255)";
	if(!database->execute(updateTable))
	{
		delete portal;
		errorMessage = tr("Error updating table.").latin1();
		return false;
	}
	
	std::string sqlProjection = "select srtext, f_geometry_column from geometry_columns m left join spatial_ref_sys c ON c.srid = m.srid WHERE m.f_table_name = '" + tableName + "'";
	if(!portal->query(sqlProjection))
	{
		delete portal;
		errorMessage = tr("Error queryng projection information.").latin1();
		return false;
	}
	std::string projectionWkt = "";
	std::string geomColumnName = "";
	if(portal->fetchRow())
	{
		projectionWkt = portal->getData(0);
		geomColumnName = portal->getData(1);
	}		

	portal->freeResult();


	std::string sqlAttrColums = "select a.*, t.* from pg_class c "; 
	sqlAttrColums += "join pg_attribute a on c.oid = a.attrelid ";
	sqlAttrColums += "join pg_type t on t.oid = a.atttypid ";
	sqlAttrColums += "where " + database->toUpper("c.relname") + " = " + database->toUpper("'" + tableName + "'"); 
	sqlAttrColums += "and a.attnum >= 0 ";
	sqlAttrColums += "and t.typname <> 'geometry'";
	
	if(!portal->query(sqlAttrColums))
	{
		delete portal;
		errorMessage = tr("Error querying non spatial columns.").latin1();
		return false;
	}

	std::string columnNames = "";
	while(portal->fetchRow())
	{
		if(!columnNames.empty())
		{
			columnNames += ",";
		}
		columnNames += portal->getData("attname");
	}

	if(columnNames.empty())
	{
		delete portal;
		errorMessage = tr("There must be at least one non spatial column.").latin1();
		return false;
	}

	portal->freeResult();

	//agora criamos a vista do banco para representar a tabela de atributos
    std::string createAttrViewSql = "create or replace view  " + tableName + "_ATTR as";
	createAttrViewSql += " select " + columnNames + " from " + tableName;
	if(!database->execute(createAttrViewSql))
	{
		delete portal;
		errorMessage = tr("Error creating attribute view.").latin1();
		return false;
	}
	
	
	std::string sqlGeomColums = "select a.*, t.* from pg_class c "; 
	sqlGeomColums += "join pg_attribute a on c.oid = a.attrelid ";
	sqlGeomColums += "join pg_type t on t.oid = a.atttypid ";
	sqlGeomColums += "where " + database->toUpper("c.relname") + " = " + database->toUpper("'" + tableName + "'"); 
	sqlGeomColums += "and a.attnum >= 0 ";
	sqlGeomColums += "and t.typname = 'geometry'";

	if(!portal->query(sqlGeomColums))
	{
		delete portal;
		errorMessage = tr("Error querying spatial columns.").latin1();
		return false;
	}

	columnNames = "";
	while(portal->fetchRow())
	{
		if(!columnNames.empty())
		{
			columnNames += ",";
		}
		columnNames += portal->getData("attname");
	}

	if(columnNames.empty())
	{
		delete portal;
		errorMessage = tr("There must be at least one spatial column.").latin1();
		return false;
	}

	portal->freeResult();

	std::string createGeomViewSql = "create or replace view  " + tableName + "_GEOM (GEOM_ID, OBJECT_ID, SPATIAL_DATA) as";
	createGeomViewSql += " select " + geomIdColumnName + ", " + oidColumnName + "," + columnNames + " from " + tableName;
	if(!database->execute(createGeomViewSql))
	{
		delete portal;
		errorMessage = tr("Error creating geometry view.").latin1();
		return false;
	}

	//recupera o tipo da geometria	
	std::string sqlGeomType = "select type from geometry_columns where f_table_name = '" + tableName + "' and f_geometry_column = '" + geomColumnName + "'";


	TeGeomRep geomType = TePOLYGONS;
	if(portal->query(sqlGeomType) && portal->fetchRow())
	{
		std::string spatialType = portal->getData("type");
		if(TeConvertToUpperCase(spatialType) == "POLYGON" || TeConvertToUpperCase(spatialType) == "MULTIPOLYGON")
		{
			geomType = TePOLYGONS;
		}
		else if(TeConvertToUpperCase(spatialType) == "LINE")
		{
			geomType = TeLINES;
		}
		else if(TeConvertToUpperCase(spatialType) == "POINT")
		{
			geomType = TePOINTS;
		}
	}

	portal->freeResult();


	//recuperamos o box do plano de informacao
	TeBox box;
	database->getMBRSelectedObjects(tableName, columnNames, tableName, "", "", geomType, box);

	std::string lx = Te2String(box.x1());
	std::string ly = Te2String(box.y1());
	std::string ux = Te2String(box.x2());
	std::string uy = Te2String(box.y2());

	//agora vamos inserir informacoes nas tabelas terralib
	//primeiro inserimos na tabela te_projection
	TeProjection* proj = TeGetTeProjectionFromWKT(projectionWkt);
	if(proj == NULL)
	{
		delete portal; 
		return false;
	}

	if(!database->insertProjection(proj))
	{
		delete portal;
		return false;
	}

	int projId = proj->id();


	if(projId == -1)
	{
		delete portal;
		errorMessage = tr("Error querying projection id.").latin1();
		return false;
	}

	//em seguida na tabela te_layer
	std::string insertLayer = "insert into te_layer (projection_id, name, lower_x, lower_y, upper_x, upper_y)";
    insertLayer += " values (" + Te2String(projId) + ", '" + tableName + "', " + lx + ", " + ly + ", " + ux + ", " + uy + ")";
	if(!database->execute(insertLayer))
	{
		delete portal;
		errorMessage = tr("Error inserting layer.").latin1();
		return false;
	}

	int layerId = getMaxValueFromColumn("te_layer", "layer_id");
	if(layerId == -1)
	{
		delete portal;
		errorMessage = tr("Error querying layer id.").latin1();
		return false;
	}

	//em seguida na tabela te_layer_table
	std::string insertLayerTable = "insert into te_layer_table  (layer_id, attr_table, unique_id, attr_link, attr_table_type)";
	insertLayerTable += " values (" + Te2String(layerId) + ", '" + tableName + "_ATTR','" + geomIdColumnName + "','" + oidColumnName + "',1)";
	if(!database->execute(insertLayerTable))
	{
		delete portal;
		errorMessage = tr("Error inserting layer table.").latin1();
		return false;
	}

	//em seguida na tabela te_representation
	std::string insertRepresentation = "insert into te_representation (layer_id, geom_type, geom_table,  lower_x, lower_y, upper_x, upper_y)";
	insertRepresentation += " values (" + Te2String(layerId) + ", " + Te2String(geomType) + ", '" + tableName + "_GEOM', " + lx + ", " + ly + ", " + ux + ", " + uy + ")";
	if(!database->execute(insertRepresentation))
	{
		delete portal;
		errorMessage = tr("Error inserting representation.").latin1();
		return false;
	}
	delete portal;

	return true;
}


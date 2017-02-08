/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <TeDatabase.h>
#include <PluginParameters.h>
#include <TeWaitCursor.h>
#include <help.h>

//Qt include files
#include <qmainwindow.h>
#include <qlistview.h>
#include <qmessagebox.h>

std::string getSRID(TeDatabase* database, const std::string& tableName)
{
	std::string sql = "select srid from user_sdo_geom_metadata where table_name = '" + tableName + "'";

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

void OracleSpatialAdapterWindow::init()
{
	layerCreated_ = false;
	help_ = NULL;
}


void OracleSpatialAdapterWindow::destroy()
{

}

void OracleSpatialAdapterWindow::initFromPluginParameters()
{
	if(plugPars_ == NULL)
	{
		return;
	}
}

void OracleSpatialAdapterWindow::updateForm( PluginParameters * pparams )
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
	if(database->dbmsName() != "OracleSpatial")
	{
		QMessageBox::information(plugPars_->qtmain_widget_ptr_, tr("Error"), tr("Plugin available only for an Oracle Spatial Database!"));
		return;
	}

	TeWaitCursor waitCursor;
	waitCursor.setWaitCursor();

	TeDatabasePortal* portal = database->getPortal();

	std::string sql = "select table_name from user_sdo_geom_metadata";
	sql += " where upper(table_name) NOT IN(select upper(geom_table) from te_representation)";
	if(!portal->query(sql))
	{
		delete portal;
		waitCursor.resetWaitCursor();
		return;
	}
	while(portal->fetchRow())
	{
		layersListBox->insertItem(portal->getData("table_name"));
	}

	delete portal;
	portal = NULL;

	waitCursor.resetWaitCursor();

	exec();
}


void OracleSpatialAdapterWindow::addPushButton_clicked()
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
			std::string srid = getSRID(database, layersListBox->item(i)->text().latin1());
			if(!srid.empty())
			{
				waitCursor.resetWaitCursor();
				QMessageBox::information(this, tr("Oracle Spatial Adapter Plugin"), tr("The data '") + layersListBox->item(i)->text() + tr("' has an incompatible projection. Choose another one!"));
				return;
			}
			if(database->layerExist(layersListBox->item(i)->text().latin1()))
			{
				waitCursor.resetWaitCursor();
				QMessageBox::information(this, tr("Oracle Spatial Adapter Plugin"), tr("There is already a layer if the same name as '") + layersListBox->item(i)->text() + tr("'."));
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


void OracleSpatialAdapterWindow::addAllPushButton_clicked()
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
			std::string srid = getSRID(database, layersListBox->item(i)->text().latin1());

			if(!srid.empty())
			{
				waitCursor.resetWaitCursor();
				QMessageBox::information(this, tr("Oracle Spatial Adapter Plugin"), tr("The data '") + layersListBox->item(i)->text() + tr("' has an incompatible projection. Choose another one!"));
				continue;
			}

			if(database->layerExist(layersListBox->item(i)->text().latin1()))
			{
				waitCursor.resetWaitCursor();
				QMessageBox::information(this, tr("Oracle Spatial Adapter Plugin"), tr("There is already a layer if the same name as '") + layersListBox->item(i)->text() + tr("'."));
				continue;
			}

			selectedLayersListBox->insertItem(layersListBox->text(i));
			layersListBox->removeItem(i);
		}
	}
	waitCursor.resetWaitCursor();
}


void OracleSpatialAdapterWindow::removePushButton_clicked()
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


void OracleSpatialAdapterWindow::removeAllPushButton_clicked()
{
	while(selectedLayersListBox->count()>0)
	{
		layersListBox->insertItem(selectedLayersListBox->item(0)->text());
		selectedLayersListBox->removeItem(0);
	}
}


void OracleSpatialAdapterWindow::createPushButton_clicked()
{
	TeWaitCursor waitCursor;	
	waitCursor.setWaitCursor();

//monta a lista de tabelas espaciais a serem inseridas no modelo terralib
	unsigned int success = 0;
	unsigned int errors = 0;
	for(unsigned int i=0; i < selectedLayersListBox->count(); i++)
	{
		std::string tableName = selectedLayersListBox->text(i).latin1();
		std::string errorMessage = "";
		if(!spatialToTerralib(tableName, errorMessage))
		{
			waitCursor.resetWaitCursor();
			QMessageBox::critical(this, tr("Oracle Spatial Adapter Plugin"), tr("Error creating model: ") + errorMessage.c_str());
			waitCursor.setWaitCursor();
			++errors;
			continue;
		}
		++success;
	}

	if(success > 0)
	{
		layerCreated_ = true;
	}

	QString message = tr("Finished with ") + QString(Te2String(success).c_str()) + tr(" table(s) imported successfully and ") + QString(Te2String(errors).c_str()) + tr(" errors!");

	QMessageBox::information(this, tr("Oracle Spatial Adapter Plugin"), message);

	waitCursor.resetWaitCursor();	
}


void OracleSpatialAdapterWindow::exitPushButton_clicked()
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


void OracleSpatialAdapterWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("PluginOracleSpatialAdapter.htm");
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

int OracleSpatialAdapterWindow::getMaxValueFromColumn(const std::string& tableName, const std::string& columnName)
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

bool OracleSpatialAdapterWindow::spatialToTerralib(const std::string& tableName, std::string& errorMessage)
{
	errorMessage = "";

	std::string geomIdColumnName = "GEOM_ID_";
	std::string oidColumnName = "OBJECT_ID_";


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
	
//verificamos se a tabela do oracle spatial jah possui uma coluna para representar o geom_id
	std::string sqlGeomId = "select column_name from user_tab_columns where " + database->toUpper("column_name") + " = '" + TeConvertToUpperCase(geomIdColumnName) + "' and table_name = '" + tableName + "'";
	if(!portal->query(sqlGeomId))
	{
		delete portal;
		tr("Unable to check if column exists.");
		return false;
	}
//verifica se a tabela espacial do oracle possui a coluna para representar o geom_id
	bool geomIdColumnExists = portal->fetchRow();

	portal->freeResult();

	std::string sqlOid = "select column_name from user_tab_columns where " + database->toUpper("column_name") + " = '" + TeConvertToUpperCase(oidColumnName) + "' and table_name = '" + tableName + "'";
	if(!portal->query(sqlOid))
	{
		delete portal;
		errorMessage = tr("Error checking if column exists.").latin1();
		return false;
	}
//verifica se a tabela espacial do oracle possui a coluna para representar o object_id
	bool oidColumnExists = portal->fetchRow();

	portal->freeResult();

	if(geomIdColumnExists == false)
	{
//se a tabela do oracle spatial nao possuir a coluna para representar o geom_id, criamos e populamos esta coluna
//criamos da coluna para representar o geom_id
		std::string sqlAlter = "alter table " + tableName + " add " + geomIdColumnName + " number";
		if(!database->execute(sqlAlter))
		{
			delete portal;
			errorMessage = tr("Error adding column.").latin1();
			return false;
		}
	}

	if(oidColumnExists == false)
	{
//se a tabela do oracle spatial nao possuir a coluna para representar o geom_id, criamos e populamos esta coluna
//criamos da coluna para representar o object_id
		std::string sqlAlter = "alter table " + tableName + " add " + oidColumnName + " varchar2(20)";
		if(!database->execute(sqlAlter))
		{
			delete portal;
			errorMessage = tr("Error adding column.").latin1();
			return false;
		}

		std::string sqlOidIndex = "CREATE INDEX " + tableName + "idx ON " + tableName + "(" + oidColumnName + ")";
		if(!database->execute(sqlOidIndex))
		{
			delete portal;
			errorMessage = tr("Error creating index.").latin1();
			return false;
		}
	}

//verificamos se a sequence que vai gerar os valores desta coluna de geom_id ja existe
	std::string sqlSequence = "select *  from user_sequences where sequence_name = '" + tableName + "_SEQ'";
	if(!portal->query(sqlSequence))
	{
		delete portal;
		errorMessage = tr("Error checking if sequence exists.").latin1();
		return false;
	}

	bool sequenceExists = portal->fetchRow();

	portal->freeResult();

//caso a sequence exista, removemos ela da base de dados
	if(sequenceExists == true)
	{
		std::string dropSequence = "drop sequence " + tableName + "_SEQ";
		if(!database->execute(dropSequence))
		{
			delete portal;
			errorMessage = tr("Error dropping sequence.").latin1();
			return false;
		}
	}

//verificamos se a trigger para gerar os ids automaticamente ja existe
	std::string sqlTrigger = " select * from user_triggers where trigger_name = '" + tableName + "_TRI'";
	if(!portal->query(sqlTrigger))
	{
		delete portal;
		errorMessage = tr("Error checking if trigger exists.").latin1();
		return false;
	}

	bool triggerExists = portal->fetchRow();

	portal->freeResult();

//caso a trigger exista, removemos ela da base de dados
	if(triggerExists == true)
	{
		std::string dropTrigger = "drop trigger " + tableName + "_TRI";
		if(!database->execute(dropTrigger))
		{
			delete portal;
			errorMessage = tr("Error dropping trigger.").latin1();
			return false;
		}
	}
//agora criamos a sequence que ira gerar os valores do geom_id
	std::string createSequence = "create sequence " + tableName + "_SEQ start with 1 nocache";
	if(!database->execute(createSequence))
	{
		delete portal;
		errorMessage = tr("Error creating sequence.").latin1();
		return false;
	}

//criamos a trigger para que preencha a tabela com os geom_ids gerados pela sequence quando a tabela for atualizada
	std::string createUpdateTrigger = "create or replace trigger " + tableName + "_TRI before update on " + tableName + " for each row";
	createUpdateTrigger += " begin";
	createUpdateTrigger += " select " + tableName + "_SEQ.nextval into :new.GEOM_ID_ from dual;";
    createUpdateTrigger += " end;";
	if(!database->execute(createUpdateTrigger))
	{
		delete portal;
		errorMessage = tr("Error creating trigger.").latin1();
		return false;
	}	

//atualizamos a tabela para que a trigger preencha os valores dos geom_ids
	std::string updateTable = "update " + tableName + " set geom_id_ = 1";
	if(!database->execute(updateTable))
	{
		delete portal;
		errorMessage = tr("Error updating table.").latin1();
		return false;
	}
//remove a trigger que atualiza os identificadores
	std::string dropTrigger = "drop trigger " + tableName + "_TRI";
	if(!database->execute(dropTrigger))
	{
		delete portal;
		errorMessage = tr("Error dropping trigger.").latin1();
		return false;
	}
	
//gera os object_ids das geometrias
	std::string updateTableOid = "update " + tableName + " set object_id_ = to_char(geom_id_)";
	if(!database->execute(updateTableOid))
	{
		delete portal;
		errorMessage = tr("Error updating table.").latin1();
		return false;
	}

//agora criamos a trigger para gerar os geom_ids caso algum dado seja inserido posteriomente na tabela espacial do oracle
	std::string createInsertTrigger = "create or replace trigger " + tableName + "_TRI before insert on " + tableName + " for each row";
	createInsertTrigger += " begin";
	createInsertTrigger += " select " + tableName + "_SEQ.nextval into :new.GEOM_ID_ from dual;";
    createInsertTrigger += " end;";
	if(!database->execute(createInsertTrigger))
	{
		delete portal;
		errorMessage = tr("Error creating trigger.").latin1();
		return false;
	}
//agora criamos a vista do banco que ira representar a tabela de atributos do plano de informacao
//recuperamos a projecao em foramto WKT
	std::string sqlProjection = "select wktext, column_name from user_sdo_geom_metadata m left join CS_SRS c ON c.srid = m.srid WHERE m.table_name = '" + tableName + "'";
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

//primeiramente	buscamos as colunas que NAO sao do tipo 'SDO_GEOMETRY', pois esta representa a geometria e estamos criando a view de atributos
	std::string sqlAttrColums = "select column_name from user_tab_columns where table_name = '" + tableName + "' and data_type <> 'SDO_GEOMETRY'";
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
		columnNames += portal->getData("column_name");
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
//agora criamos a vista do banco que ira representar a tabela de geometrias do plano de informacao
//primeiramente	buscamos as colunas que SAO do tipo 'SDO_GEOMETRY', pois esta representa a geometria e estamos criando a view de geoemtrias
	std::string sqlGeomColums = "select column_name from user_tab_columns where table_name = '" + tableName + "' and data_type = 'SDO_GEOMETRY'";
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
		columnNames += portal->getData("column_name");
	}

	if(columnNames.empty())
	{
		delete portal;
		errorMessage = tr("There must be at least one spatial column.").latin1();
		return false;
	}

	portal->freeResult();

	std::string createGeomViewSql = "create or replace view  " + tableName + "_GEOM (GEOM_ID, OBJECT_ID, SPATIAL_DATA) as";
	createGeomViewSql += " select GEOM_ID_, OBJECT_ID_," + columnNames + " from " + tableName;
	if(!database->execute(createGeomViewSql))
	{
		delete portal;
		errorMessage = tr("Error creating geometry view.").latin1();
		return false;
	}

//recupera o tipo da geometria
	std::string sqlGeomType = "select sdo_geometry.GET_GTYPE( " + geomColumnName + ") from " + tableName + " where rownum = 1";

	unsigned int geomType = 4;
	if(portal->query(sqlGeomType) && portal->fetchRow())
	{
		unsigned int spatialType = portal->getInt(0);
		if(spatialType == 1)
		{
			geomType = 4;
		}
		else if(spatialType == 2)
		{
			geomType = 2;
		}
		else if(spatialType == 3)
		{
			geomType = 1;
		}
	}

	portal->freeResult();


//recuperamos o box do plano de informacao
	std::string sqlBox = "select";
    sqlBox += " (SELECT SDO_LB FROM  THE(SELECT DIMINFO FROM user_sdo_geom_metadata b where a.table_name=b.table_name )  WHERE sdo_dimname='X' ) as lx";
    sqlBox += " ,(SELECT SDO_LB FROM  THE(SELECT DIMINFO FROM user_sdo_geom_metadata b where a.table_name=b.table_name ) WHERE sdo_dimname='Y' ) as ly";
    sqlBox += " ,(SELECT SDO_UB FROM  THE(SELECT DIMINFO FROM user_sdo_geom_metadata b where a.table_name=b.table_name ) WHERE sdo_dimname='X' ) as ux";
    sqlBox += " ,(SELECT SDO_UB FROM  THE(SELECT DIMINFO FROM user_sdo_geom_metadata b where a.table_name=b.table_name ) WHERE sdo_dimname='Y' ) as uy";
    sqlBox += " from user_sdo_geom_metadata a where a.table_name = '" + tableName + "'";

	if(!portal->query(sqlBox) || !portal->fetchRow())
	{
		delete portal;
		errorMessage = tr("Error querying bounding box .").latin1();
		return false;
	}

	std::string lx = portal->getData("lx");
	std::string ly = portal->getData("ly");
	std::string ux = portal->getData("ux");
	std::string uy = portal->getData("uy");

	portal->freeResult();

//agora vamos inserir informacoes nas tabelas terralib
//primeiro inserimos na tabela te_projection
	/*TeProjection* proj = TeGetTeProjectionFromWKT(projectionWkt);
	if(proj == NULL)
	{
		delete portal; 
		return;
	}

	if(!database->insertProjection(proj))
	{
		delete portal;
		return;
	}

	int projId = proj->id();*/

	std::string insertProjection = " insert into te_projection (name, long0, lat0, offx, offy, stlat1, stlat2, unit, scale, hemis, datum, radius, flattening, dx, dy, dz)";
	insertProjection += " values ('NoProjection',0,0,0,0,0,0,'UNITS',1,1,'SPHERICAL',6371000,0,0,0,0)";
	if(!database->execute(insertProjection))
	{
		delete portal;
		errorMessage = tr("Error inserting projection.").latin1();
		return false;
	}

	int projId = getMaxValueFromColumn("te_projection", "projection_id");
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
	insertLayerTable += " values (" + Te2String(layerId) + ", '" + tableName + "_ATTR','OBJECT_ID_','OBJECT_ID_',1)";
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

void OracleSpatialAdapterWindow::addAsPushButton_clicked()
{

}

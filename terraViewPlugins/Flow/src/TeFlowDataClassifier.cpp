  /************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright © 2001-2004 INPE and Tecgraf/PUC-Rio.

This code is part of the TerraLib library.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

You should have received a copy of the GNU Lesser General Public
License along with this library.

The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The library provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use
of this library and its documentation.
*************************************************************************************/

#include <TeFlowDataClassifier.h>
#include <TeFlowMetadata.h>
#include <TeFlowUtils.h>
#include <TeDatabase.h>
#include <TeQuerierParams.h>
#include <TeQuerier.h>

TeDominantFlowNetwork::TeDominantFlowNetwork(TeTheme* flowTheme, TeTheme* refTheme)
							: flowTheme_(flowTheme), referenceTheme_(refTheme), flowMetadata_(0)
{
	errorMessage_ = "";

	flowMetadata_ = new TeFlowMetadata(flowTheme_->layer()->database());

	if(!flowMetadata_->loadMetadata(flowTheme_->layer()->id()))
		throw;

	flowTableName_ = flowTheme_->layer()->database()->getTableName(flowMetadata_->flowTableId_);

	referenceTableName_ = referenceTheme_->layer()->database()->getTableName(flowMetadata_->referenceTableId_);

	
		flowType_.push_back("zero");										//0
		flowType_.push_back("Hierarquico Ascendente Direto");				//1
		flowType_.push_back("Hierarquico Ascendente CurtoCircuito");		//2
		flowType_.push_back("Hierarquico Descendente Direto");				//3
		flowType_.push_back("Hierarquico Descendente CurtoCircuito");		//4
		flowType_.push_back("Transversal Ascendente Mesma Rede");			//5
		flowType_.push_back("Transversal Ascendente Entre redes");			//6
		flowType_.push_back("Transversal Descendente Mesma Rede");			//7
		flowType_.push_back("Transversal Descendente Entre redes");			//8
        flowType_.push_back("Transversal Horizontal Mesma rede");			//9
		flowType_.push_back("Transversal Horizontal Entre redes");			//10
		flowType_.push_back("Transversal Ascendente Mesma rede");			//11
		flowType_.push_back("Transversal Descendente Mesma rede");			//12
		flowType_.push_back("Transversal Horizontal Entre redes");			//13
		flowType_.push_back("Exo Ascendente Mesma Rede");					//14
		flowType_.push_back("Exo Ascendente Entre Redes");					//15
		flowType_.push_back("Endo Descendente Mesma Rede");					//16
		flowType_.push_back("Endo Descendente Entre Redes");				//17
		flowType_.push_back("Ligacao Local");								//18
}

TeDominantFlowNetwork::~TeDominantFlowNetwork()
{
	if(flowMetadata_)
		delete flowMetadata_;
}

bool TeDominantFlowNetwork::buildNetwork(const string& weightColumnName, const string& dominanceColumnName, const bool& dominaceRefersToOriginCol, const bool& calculateDominance, const bool& tot1, const double& dominanceRelation,
	const bool& checkLocalDominance, const double& localDominanceValue)
{
// checa se o tema de refencia e o de fluxo estao carregados
	errorMessage_ = "";

	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return false;
	}
	
	if(!flowTheme_)
	{
		errorMessage_ = "No flow theme defined!";
		return false;
	}
	
// limpa a estrutura da rede
	isolateds_.clear();
	roots_.clear();
	flow_.clear();

// Determina os nomes das colunas e tabelas que serao utilizadas para geracao da rede
	if(!fillColumnAndTableNames(weightColumnName, dominanceColumnName, calculateDominance))
		return false;

// Carregar as informacoes de dominancia para os objetos de destino/origem
	map<string, double> dominanceMap;

	if(!calculateDominance)
	{
		if(!loadDominanceTable(dominanceMap, dominaceRefersToOriginCol))
			return false;
	}
	
// Pega o portal com os registros de fluxo e peso de fluxo
	TeDatabasePortal* portal = getFlowPortal();

    if(!portal)
		return false;

// navega pelos registros do portal, inserindo os fluxos na rede.... montagem da rede
	std::string origin = "";
	std::string destiny = "";
	string flowId;
	double flowWeightValue = 0.0;
	bool isDoubleWeight = (portal->getAttribute(3).rep_.type_ == TeREAL) ? true : false;

	while(portal->fetchRow())
	{
		origin  = portal->getData(0);
		destiny = portal->getData(1);
		flowId  = portal->getData(2);

		if(isDoubleWeight)
			flowWeightValue = portal->getDouble(3);
		else
			flowWeightValue = static_cast<double>(portal->getInt(3));

		if(!insertIntoNetwork(origin, destiny, flowId, flowWeightValue, dominanceMap, calculateDominance))
		{
			portal->freeResult();
			delete portal;
			return false;
		}
	}

	portal->freeResult();
	delete portal;

	if(calculateDominance)
	{
		calculateDominanceValue(tot1);
	}

	if(!setMainFlow(dominanceRelation, checkLocalDominance, localDominanceValue))
	{
		return false;
	}

	findIsolatedRoots();

	return true;
}

bool TeDominantFlowNetwork::saveNetwork(const string& columnName, const bool& sumColumns, const bool& treeColumns)
{
//----- This is used for progress bar
	int dt = CLOCKS_PER_SEC/4;
	int dt2 = CLOCKS_PER_SEC * 5;
	clock_t	t0, t1, t2;
	t2 = clock();
	t0 = t1 = t2;
//-----	

		
// checa se o tema de refencia e o de fluxo estao carregados
	errorMessage_ = "";
	
	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return false;
	}

	if(!flowTheme_)
	{
		errorMessage_ = "No flow theme defined!";
		return false;
	}

// nomes das colunas de saida, com a informacoes da rede
	string columnMainFlow = columnName + "_main_flow";
	string columnLevel = columnName + "_net_level";
	string columnIn = columnName + "_in";
	string columnOut = columnName + "_out";
	string columnSumIn = columnName + "_sum_in";
	string columnSumOut = columnName + "_sum_out";
	string columnNoSup = columnName + "_no_sup";

// nomes das colunas de saida referentes aos roots
	std::string columnNetNodes = columnName + "_net_nodes";
	std::string columnRootChild = columnName + "_root_childs";
		
//verificacao das colunas no tema com o fluxo
	TeDatabase* db = referenceTheme_->layer()->database();	
	TeAttribute at;

	if(db->columnExist(flowTableName_, columnMainFlow, at))
	{
		errorMessage_ = "Column: " + columnMainFlow + " already exist in flow attribute table!";

		return false;
	}

	if(db->columnExist(flowTableName_, columnLevel, at))
	{
		errorMessage_ = "Column: " + columnLevel + " already exist in reference attribute table theme!";

		return false;
	}

	if(db->columnExist(flowTableName_, columnIn, at))
	{
		errorMessage_ = "Column: " + columnIn + " already exist in reference attribute table theme!";

		return false;
	}
	if(db->columnExist(flowTableName_, columnOut, at))
	{
		errorMessage_ = "Column: " + columnOut + " already exist in reference attribute table theme!";

		return false;
	}

	at.rep_.name_ = columnMainFlow;
	at.rep_.type_ = TeINT;
	at.rep_.isPrimaryKey_ = false;
	at.rep_.isAutoNumber_ = false;
	
// Adiciona a coluna de saída na tabela de fluxo
	if(!db->addColumn(flowTableName_, at.rep_))
	{
		errorMessage_ = "Couldn't add column: " +  columnMainFlow + " to flow table!";

		return false;
	}

// Adiciona o campo de saída na tabela de atributos do layer de referência
	TeAttrTableVector attrs;
	db->layerMap()[flowMetadata_->referenceLayerId_]->getAttrTables(attrs);
	
	unsigned int i = 0;

	for(i = 0; i < attrs.size(); ++i)
	{
		if(attrs[i].id() == flowMetadata_->referenceTableId_)
			break;
	}

	if(i >= attrs.size())
	{
		errorMessage_ = "Couldn't find attribute table of reference theme!";

		return false;
	}
	
	TeTable& attRefTable = attrs[i];

	at.rep_.name_ = columnLevel;

	if(!db->addColumn(attRefTable.name(), at.rep_))
	{
		errorMessage_ = "Couldn't add column: " +  columnName + " to reference table!";

		return false;
	}

	if( sumColumns)
	{
		// Adiciona o campo com o número de fluxos de entrada na tabela de atributos do layer de referência
		at.rep_.name_ = columnIn;

		if(!db->addColumn(attRefTable.name(), at.rep_))
		{
			errorMessage_ = "Couldn't add column: " +  columnName + "to reference table!";

			return false;
		}

		// Adiciona o campo com o número de fluxos de saída na tabela de atributos do layer de referência
		at.rep_.name_ = columnOut;

		if(!db->addColumn(attRefTable.name(), at.rep_))
		{
			errorMessage_ = "Couldn't add column: " +  columnName + "to reference table!";

			return false;
		}

		// Adiciona o campo com o sumatório dos valores dos fluxos de entrada na tabela de atributos do layer de referência
		at.rep_.name_ = columnSumIn;
		at.rep_.type_ = TeREAL;

		if(!db->addColumn(attRefTable.name(), at.rep_))
		{
			errorMessage_ = "Couldn't add column: " +  columnName + "to reference table!";

			return false;
		}

		
		// Adiciona o campo com o sumatório dos valores dos fluxos de saída na tabela de atributos do layer de referência
		at.rep_.name_ = columnSumOut;
		at.rep_.type_ = TeREAL;

		if(!db->addColumn(attRefTable.name(), at.rep_))
		{
			errorMessage_ = "Couldn't add column: " +  columnName + "to reference table!";

			return false;
		}

		// Adiciona o campo com a informacao de quem e o no superior imediato do no corrente
		at.rep_.name_ = columnNoSup;
		at.rep_.type_ = TeINT;
		if(!db->addColumn(attRefTable.name(), at.rep_))
		{
			errorMessage_ = "Couldn't add column: " +  columnName + "to reference table!";

			return false;
		}

		// Adiciona o campo com a informacao de quantos nohs possuem na rede deste root
		at.rep_.name_ = columnNetNodes;
		if(!db->addColumn(attRefTable.name(), at.rep_))
		{
			errorMessage_ = "Couldn't add column: " +  columnNetNodes + "to reference table!";

			return false;
		}

		// Adiciona o campo com a informacao de quantos filhos este root possue
		at.rep_.name_ = columnRootChild;
		if(!db->addColumn(attRefTable.name(), at.rep_))
		{
			errorMessage_ = "Couldn't add column: " +  columnRootChild + "to reference table!";

			return false;
		}
	}

// Marca os níveis dos nós da rede
	if(!buildLevel())
		errorMessage_ = "Couldn't build levels";

// Marca os fluxos na tabela de fluxo
	string	sql  = "UPDATE ";
			sql += flowTableName_;
			sql += " SET ";
			sql += columnMainFlow;
			sql += " = -1 ";

	if(!db->execute(sql))
	{
		errorMessage_ = "Couldn't update column: " +  columnName + " of flow table with default values!";

		return false;
	}

	
	TeDominantFlowTree::iterator itFlow = flow_.begin();

	TeTable flowTable(flowTableName_);
	db->loadTableInfo(flowTable);
	//flowTable.linkName();


	unsigned int steps_aux = 0;
	if(TeProgress::instance())
	{
		TeProgress::instance()->setTotalSteps(NULL);
		TeProgress::instance()->setCaption("Update flow table...");
		TeProgress::instance()->setMessage("Processing...      ");
	}

	while(itFlow != flow_.end())
	{
		if(TeProgress::instance()->wasCancelled())
		{	
			TeProgress::instance()->reset();
			errorMessage_ = "The process was canceled by user!";

			return false;
		}

		sql  = "UPDATE ";
		sql += flowTableName_;
		sql += " SET ";
		sql += columnMainFlow;
		sql += " = ";
		sql += " 1 ";
		sql += " WHERE ";
		sql += "unique_id_";
		sql += " = '";
		sql += itFlow->second.flowId_;
		sql += "'";
		
		if(!db->execute(sql))
		{
			errorMessage_ = "Couldn't update column: " +  columnName + "!";

			return false;
		}

		if(TeProgress::instance())
		{
			t2 = clock();
			if(int(t2-t1) > dt)
			{
				t1 = t2;
				
				if((int)(t2-t0) > dt2)
					TeProgress::instance()->setProgress(steps_aux);
			}
		}

		++itFlow;
		++steps_aux;
	}

	if(TeProgress::instance())
		TeProgress::instance()->reset();

// Marca os nos na tabela de referência
	sql  = "UPDATE ";
	sql += attRefTable.name();
	sql += " SET ";
	sql += columnLevel;
	sql += " = -1";
	
	if(sumColumns)
	{
		sql += ",  ";
		sql += columnIn;
		sql += " = 0,  ";
		sql += columnOut;
		sql += " = 0  ";
	}

	if(!db->execute(sql))
	{
		errorMessage_ = "Couldn't update column: " +  columnName + " of reference table with default values!";

		return false;
	}

	itFlow = flow_.begin();

	t2 = clock();
	t0 = t1 = t2;
	steps_aux = 0;

	if(TeProgress::instance())
	{
		TeProgress::instance()->setTotalSteps(NULL);
		TeProgress::instance()->setCaption("Update reference Table");
		TeProgress::instance()->setMessage("Processing...      ");
	}
	
	while(itFlow != flow_.end())
	{
		
		double sumWeight = 0.0;
		vector<double>::iterator itFlowWeight = itFlow->second.weightValues_.begin();
		while(itFlowWeight != itFlow->second.weightValues_.end())
		{
			sumWeight += *itFlowWeight;
			++itFlowWeight;
		}
		if(TeProgress::instance()->wasCancelled())
		{
			TeProgress::instance()->reset();
			errorMessage_ = "The process was canceled by user!";

			return false;
		}

		sql  = "UPDATE ";
		sql += attRefTable.name();
		sql += " SET ";
		sql += columnLevel;
		sql += " = ";
		sql += Te2String(itFlow->second.level_);
		if(sumColumns)
		{
			sql += ", ";
			sql += columnIn;
			sql += " = ";
			sql += Te2String(itFlow->second.flowInTot_);
			sql += ", ";
			sql += columnOut;
			sql += " = ";
			sql += Te2String(itFlow->second.destinyNodes_.size());
			sql += ", ";
			sql += columnSumIn;
			sql += " = ";
			sql += Te2String(itFlow->second.flowWeightSumIn_);
			sql += ", ";
			sql += columnSumOut;
			sql += " = ";
			sql += Te2String(sumWeight);
			sql += ", ";
			sql += columnNoSup;
			sql += " = ";
			if(itFlow->second.destinyId_ == "")
			{
				sql += "0";
			}
			else if(itFlow->second.level_ == 0)
			{
				sql += itFlow->second.originId_;
			}
			else
			{
				sql += itFlow->second.destinyId_;
			}

			if(itFlow->second.level_ == 0)
			{
				sql += ", ";
				sql += columnNetNodes;
				sql += " = ";
				sql += Te2String(getNetNodesSize(itFlow->second.originId_));;
				sql += ", ";
				sql += columnRootChild;
				sql += " = ";
				sql += Te2String(itFlow->second.dominatedNodes_.size());
			}
		}
		sql += " WHERE ";
		sql += attRefTable.linkName();
		sql += " = '";
		sql += itFlow->second.originId_;
		sql += "'";

		if(!db->execute(sql))
		{
			errorMessage_ = "Couldn't update row: " +  itFlow->second.originId_ + "!";

			return false;
		}
		if(TeProgress::instance())
		{
			t2 = clock();
			if(int(t2-t1) > dt)
			{
				t1 = t2;
				
				if((int)(t2-t0) > dt2)
					TeProgress::instance()->setProgress(steps_aux);
			}
		}
		++steps_aux;

		++itFlow;
	}

	if(TeProgress::instance())
		TeProgress::instance()->reset();


	if(treeColumns)
	{
	/*insercao da informacao a respeito do fluxo na linha da tabela de atributo*/

	//descobre o maior nivel da rede
		int maxLevel = getMaxLevel();

	//insere a coluna para codigo do nivel
		std::string columnNameLC = columnName +  "_Level_Code";

		TeAttribute atCL;
		atCL.rep_.name_ = columnNameLC;
		atCL.rep_.type_ = TeSTRING;
		atCL.rep_.isPrimaryKey_ = false;
		atCL.rep_.isAutoNumber_ = false;
		atCL.rep_.numChar_ = 255;

		if(!db->addColumn(attRefTable.name(), atCL.rep_))
		{
			errorMessage_ = "Couldn't add column: " +  columnNameLC + " to reference table!";
			return false;
		}

	//insere as colunas para todos os niveis da rede
		for(int i = 0; i <= maxLevel; ++i)
		{
			std::string colName = columnName + "_N" + Te2String(i);

			TeAttribute at;
			at.rep_.name_ = colName;
			at.rep_.type_ = TeSTRING;
			at.rep_.isPrimaryKey_ = false;
			at.rep_.isAutoNumber_ = false;
			at.rep_.numChar_ = 255;

			if(!db->addColumn(attRefTable.name(), at.rep_))
			{
				errorMessage_ = "Couldn't add column: " +  colName + " to reference table!";
				return false;
			}
		}

	//percorre a rede inserindo os valores na tabela
		itFlow = flow_.begin();

		while(itFlow != flow_.end())
		{
	//atualiza o nivel do proprio noh na tabela
			int currentLevel = itFlow->second.level_;

			if(currentLevel < 0)
			{
				++itFlow;
				continue;
			}

			std::string colName = columnName + "_N" + Te2String(currentLevel);

			std::string idOrigin = itFlow->second.originId_;

			std::string sqlUpdate = "UPDATE " + attRefTable.name() + " SET " + colName + " = '" + idOrigin + "' WHERE " + attRefTable.linkName() + " = '" + idOrigin + "'";

			if(!db->execute(sqlUpdate))
			{
				errorMessage_ = "Couldn't update: " +  colName + " to reference table!";
				return false;
			}

	//atualiza o codigo do nivel na tabela
			bool hasDominatedNodes = true;

			if(itFlow->second.dominatedNodes_.empty())
			{
				hasDominatedNodes = false;
			}

			std::string code = Te2String(currentLevel);
			if(!hasDominatedNodes)
			{
				code += Te2String(currentLevel);
			}

			std::string sqlUpdateCodeLevel = "UPDATE " + attRefTable.name() + " SET " + columnNameLC + " = '" + code + "' WHERE " + attRefTable.linkName() + " = '" + idOrigin + "'";

			if(!db->execute(sqlUpdateCodeLevel))
			{
				errorMessage_ = "Couldn't update code level in reference table!";
				return false;
			}

	//atualiza os niveis dos codigos superiores de noh corrente na tabela
			TeDominantFlowTree::iterator it = flow_.find(itFlow->second.originId_);

			if(it != flow_.end())
			{
				for(int i = currentLevel - 1; i >= 0; i--)
				{
					std::string colName = columnName + "_N" + Te2String(i);

					std::string id = it->second.destinyId_;

					std::string sqlUpdate = "UPDATE " + attRefTable.name() + " SET " + colName + " = '" + id + "' WHERE " + attRefTable.linkName() + " = '" + idOrigin + "'";

					if(!db->execute(sqlUpdate))
					{
						errorMessage_ = "Couldn't update: " +  colName + " to reference table!";
						return false;
					}

					it = flow_.find(it->second.destinyId_);

					if(it == flow_.end())
					{
						return false;
					}
				}
			}

			++itFlow;
		}
	}

// Update all the themes that uses these tables
	TeViewMap& viewMap = db->viewMap();
	TeViewMap::iterator it;
	set<TeLayer*> layerSet;
	for (it = viewMap.begin(); it != viewMap.end(); ++it)
	{
		TeView *view = it->second;
		vector<TeViewNode*>& themesVector = view->themes();
		for (unsigned int i = 0; i < themesVector.size(); ++i)
		{
			TeTheme *theme = (TeTheme*)themesVector[i];
			if(theme->isThemeTable(attRefTable.name()) || theme->isThemeTable(flowTableName_))
			{
				theme->loadThemeTables();
				layerSet.insert(theme->layer());
			}
		}
	}

// Update the layer tables affected
	set<TeLayer*>::iterator setIt;
	for (setIt = layerSet.begin(); setIt != layerSet.end(); ++setIt)
		(*setIt)->loadLayerTables();

	return true;
}

bool TeDominantFlowNetwork::fillColumnAndTableNames(const string& weightColumnName, const string& dominanceColumnName, const bool& calculateDominance)
{
	weightColumnTableName_ = TeGetName(weightColumnName.c_str());
	weightColumnName_ = TeGetExtension(weightColumnName.c_str());

	if(!calculateDominance)
	{
		dominanceColumnTable_ = TeGetName(dominanceColumnName.c_str());
		dominanceColumn_ = TeGetExtension(dominanceColumnName.c_str());
	}

	if(weightColumnTableName_.empty() || weightColumnName_.empty())
	{
		errorMessage_ = "The weight column name and flow column name must be in the format: TABLE_NAME.COLUMN_NAME!";

		return false;
	}

	if(!calculateDominance)
	{
		if( dominanceColumn_.empty() || dominanceColumnTable_.empty())
		{
			errorMessage_ = "The dominance column name and flow column name must be in the format: TABLE_NAME.COLUMN_NAME!";

			return false;
		}
	}

	return true;
}

bool TeDominantFlowNetwork::loadJoinMap(map<string, string>& joinMap)
{
//----- This is used for progress bar
	int dt = CLOCKS_PER_SEC/4;
	int dt2 = CLOCKS_PER_SEC * 5;
	clock_t	t0, t1, t2;
	t2 = clock();
	t0 = t1 = t2;
//-----
	
	
// checa se o tema de refencia e o de fluxo estao carregados
	errorMessage_ = "";

	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return false;
	}

	joinMap.clear();

// descobre se a informacao de dominancia esta no tema de referencia ou no tema de fluxo
	string keyCol = referenceTableName_ + "." + flowMetadata_->referenceColumnName_;
			    	
	vector<string> attrVec;
	
	attrVec.push_back(keyCol);
	
	TeQuerierParams qParam(false, attrVec);

	qParam.setParams(referenceTheme_);

	TeQuerier q(qParam);
		
	if(!q.loadInstances())
	{
		errorMessage_ = "Couldn't load join map!";
		return false;
	}

	unsigned int steps = q.numElemInstances();
	unsigned int steps_aux = 0;
		
	if(TeProgress::instance())
	{
		TeProgress::instance()->setTotalSteps(steps);
		TeProgress::instance()->setCaption("Loading dominance information");
		TeProgress::instance()->setMessage("Processing...      ");
	}

	TeSTInstance sti;

	while(q.fetchInstance(sti))
	{
		if(TeProgress::instance()->wasCancelled())
		{
			TeProgress::instance()->reset();
			errorMessage_ = "The process was canceled by user!";

			return false;
		}

		string keyValue = "";
		sti.getPropertyValue(keyCol, keyValue);

		string objId = sti.objectId();
		
		joinMap[keyValue] = objId;

		if(TeProgress::instance())
		{
			t2 = clock();
			if(int(t2-t1) > dt)
			{
				t1 = t2;
				
				if((int)(t2-t0) > dt2)
					TeProgress::instance()->setProgress(steps_aux);
			}
		}
		
		steps_aux++;
	}

   	q.clear();

	if(TeProgress::instance())
		TeProgress::instance()->reset();

	if(joinMap.empty())
	{
		errorMessage_ = "There isn't information on reference table!";
		return false;
	}

	return true;
}

bool TeDominantFlowNetwork::loadDominanceTable(map<string, double>& dominanceTable, const bool& dominaceRefersToOriginCol)
{
//----- This is used for progress bar
	int dt = CLOCKS_PER_SEC/4;
	int dt2 = CLOCKS_PER_SEC * 5;
	clock_t	t0, t1, t2;
	t2 = clock();
	t0 = t1 = t2;
//-----

// checa se o tema de refencia e o de fluxo estao carregados
	errorMessage_ = "";

	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return false;
	}

	if(!flowTheme_)
	{
		errorMessage_ = "No flow theme defined!";
		return false;
	}

	dominanceTable.clear();

// descobre se a informacao de dominancia esta no tema de referencia ou no tema de fluxo
	if(flowTheme_->isThemeTable(dominanceColumnTable_))
	{
		string keyCol = "";

		if(dominaceRefersToOriginCol)
            keyCol = flowTableName_ + "." + flowMetadata_->flowTableOriginColumnName_;
		else
            keyCol = flowTableName_ + "." + flowMetadata_->flowTableDestinyColumnName_;

		string dominanceCol = dominanceColumnTable_ + "." + dominanceColumn_;

		vector<string> attrVec;
		attrVec.push_back(keyCol);
		attrVec.push_back(dominanceCol);

		TeQuerierParams qParam(false, attrVec);

		qParam.setParams(flowTheme_);

		TeQuerier q(qParam);
		
		if(!q.loadInstances())
		{
			errorMessage_ = "Couldn't load dominance information based on flow table!";
			return false;
		}
		
		unsigned int steps = q.numElemInstances();
		unsigned int steps_aux = 0;
		
		if(TeProgress::instance())
		{
			TeProgress::instance()->setTotalSteps(steps);
			TeProgress::instance()->setCaption("Loading reference theme centroids");
			TeProgress::instance()->setMessage("Processing...      ");
		}

		TeSTInstance sti;

		while(q.fetchInstance(sti))
		{
			if(TeProgress::instance()->wasCancelled())
			{
				TeProgress::instance()->reset();
				errorMessage_ = "The process was canceled by user!";

				return false;
			}

			string keyValue = "";

			sti.getPropertyValue(keyCol, keyValue);

			double dominanceValue = sti[1];

			dominanceTable[keyValue] = dominanceValue;

			if(TeProgress::instance())
			{
				t2 = clock();
				if(int(t2-t1) > dt)
				{
					t1 = t2;
					
					if((int)(t2-t0) > dt2)
						TeProgress::instance()->setProgress(steps_aux);
				}
			}
			steps_aux++;
		}

		if(TeProgress::instance())
			TeProgress::instance()->reset();

        q.clear();

		if(dominanceTable.empty())
		{
			errorMessage_ = "There isn't dominance information on flow table!";
			return false;
		}		
	}
	else if(referenceTheme_->isThemeTable(dominanceColumnTable_))
	{
		string keyCol = referenceTableName_ + "." + flowMetadata_->referenceColumnName_;

		string dominanceCol = dominanceColumnTable_ + "." + dominanceColumn_;

		vector<string> attrVec;
		attrVec.push_back(keyCol);
		attrVec.push_back(dominanceCol);

		TeQuerierParams qParam(false, attrVec);

		qParam.setParams(referenceTheme_);

		TeQuerier q(qParam);
		
		if(!q.loadInstances())
		{
			errorMessage_ = "Couldn't load dominance information based on reference theme!";
			return false;
		}

		TeSTInstance sti;

		while(q.fetchInstance(sti))
		{
			string keyValue = "";

			sti.getPropertyValue(keyCol, keyValue);

			double dominanceValue = sti[1];

			dominanceTable[keyValue] = dominanceValue;
		}

        q.clear();

		if(dominanceTable.empty())
		{
			errorMessage_ = "There isn't dominance information on reference table!";
			return false;
		}		

	}
	else
	{
		errorMessage_ = "Dominance/impedance column doen't belong to flow or reference theme!";
		return false;
	}

	return true;
}

TeDatabasePortal* TeDominantFlowNetwork::getFlowPortal()
{
	errorMessage_ = "";

	TeDatabase* db = referenceTheme_->layer()->database();

//SQL for flow network creation

	std::string fields = "T_FLOW.";
      			fields += flowMetadata_->flowTableOriginColumnName_;
				fields += ", T_FLOW.";
				fields += flowMetadata_->flowTableDestinyColumnName_;
				fields += ", T_FLOW.";
				fields += "unique_id_";


	std::string from  = " FROM ";
				from += flowTableName_;
				from += " AS T_FLOW";
				from += ", ";
				from += flowTheme_->collectionTable();

	std::string where  = " WHERE T_FLOW.";
				where += "unique_id_";
				where += " = ";
				where += flowTheme_->collectionTable();
				where += ".c_object_id ";


	std::string orderby  = " ORDER BY T_FLOW.";
				orderby += flowMetadata_->flowTableOriginColumnName_;

//weight information
		fields += ", T_FLOW.";
        fields += weightColumnName_; 

		orderby += ", T_FLOW." + weightColumnName_;


	std::string sql = "SELECT " + fields + from + where + orderby;

	TeDatabasePortal* portal = db->getPortal();
	
	if(!portal->query(sql))
	{
		errorMessage_ = "Couldn't read flow data information!";

		if(!portal->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += portal->errorMessage();
		}

		delete portal;
		
		return 0;
	}

	return portal;	
}

TeDatabasePortal* TeDominantFlowNetwork::getMainFlowPortal()
{
	errorMessage_ = "";

	TeDatabase* db = referenceTheme_->layer()->database();

	std::string sql = "SELECT ";
				sql += flowTableName_ + "." + flowMetadata_->flowTableOriginColumnName_;
				sql += ", " + flowTableName_ + "." + flowMetadata_->flowTableDestinyColumnName_;
				sql += ", " + flowTableName_ + ".unique_id_";
				sql += ", " + flowTableName_ + "." + weightColumnName_;
				sql += " FROM (" + flowTableName_;
				sql += " INNER JOIN " + flowTheme_->collectionTable() + " ON ";
				sql += flowTheme_->collectionTable() + ".c_object_id " + " = " + flowTableName_ + ".unique_id_)";
				sql += " ORDER BY " + flowTableName_ + "." + flowMetadata_->flowTableOriginColumnName_ + ", " + flowTableName_ + "." + weightColumnName_ + " DESC";

	//TeWriteToFile("sql.txt", sql, "a+");

	TeDatabasePortal* portal = db->getPortal();
	
	if(!portal->query(sql))
	{
		errorMessage_ = "Couldn't read flow data information!";

		if(!portal->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += portal->errorMessage();
		}

		delete portal;
		
		return 0;
	}

	return portal;	
}

bool TeDominantFlowNetwork::setMainFlow(const double& dominanceRelation, const bool& checkLocalDominance, const double& localDominanceValue)
{
	TeDatabasePortal* portal = getMainFlowPortal();

    if(!portal)
		return false;

	std::string origin = "";
	std::string destiny = "";
	std::string flowId = "";
	double weight = 0.;
	bool isDoubleWeight = (portal->getAttribute(3).rep_.type_ == TeREAL) ? true : false;
	double dominanceValue = -1.;

	int count = 0;

	bool linked = false;

	while(portal->fetchRow())
	{
		std::string newOrigin = portal->getData(0);

		if(newOrigin == "320140")
		{
			int a = 0;
		}

		if(newOrigin != origin && !origin.empty())
		{
			if(!linkNetwork(origin, destiny, flowId, weight, dominanceRelation, checkLocalDominance, localDominanceValue))
			{
				portal->freeResult();
				delete portal;
				return false;
			}

			dominanceValue = -1.;
			weight = 0;

			linked = false;

			count++;
		}

		std::string checkDestiny = portal->getData(1);

		TeDominantFlowTree::iterator itFlowCheck = flow_.find(checkDestiny);

		if(itFlowCheck == flow_.end())
		{	
			errorMessage_ = "Could not find an specified node destiny (" + checkDestiny + ") in the network.";
			return false;
		}

		double checkDominance = itFlowCheck->second.dominanceValue_;

		double weightCheck;

		if(isDoubleWeight)
			weightCheck = portal->getDouble(3);
		else
			weightCheck = static_cast<double>(portal->getInt(3));


		if(checkLocalDominance && checkDestiny == newOrigin)
		{
			TeDominantFlowTree::iterator itFlowCheck = flow_.find(newOrigin);

			if(itFlowCheck != flow_.end())
			{	
				double totOut = 0.;

				for(unsigned int k = 0; k < itFlowCheck->second.weightValues_.size(); ++k)
				{
					totOut += itFlowCheck->second.weightValues_[k];
				}
					
				//verificar a relacao de dominancia
				if(((weightCheck * 100) / totOut) >= localDominanceValue)
				{
					origin = newOrigin;
					destiny = portal->getData(1);
					flowId = portal->getData(2);

					linked = true;
				}
			}
		}
		else if(!checkLocalDominance || (checkLocalDominance && !linked))
		{
			if(checkDominance > dominanceValue  && weightCheck >= weight)
			{
				bool isValid = true;
				if(destiny == newOrigin)
				{
					TeDominantFlowTree::iterator itFlowOrigin = flow_.find(newOrigin);

					double totOut = 0.;

					for(unsigned int i = 0; i < itFlowOrigin->second.weightValues_.size(); ++i)
					{
						double value = itFlowOrigin->second.weightValues_[i];
						totOut += value;
					}

					if(((weightCheck * 100) / totOut) < dominanceRelation)
					{
						isValid = false;
					}
				}

				if(isValid)
				{
					origin = newOrigin;
					destiny = portal->getData(1);
					flowId = portal->getData(2);

					dominanceValue = checkDominance;
				
					if(isDoubleWeight)
						weight = portal->getDouble(3);
					else
						weight = static_cast<double>(portal->getInt(3));
				}
			}
		}
		
	}

	portal->freeResult();
	delete portal;

	if(!linkNetwork(origin, destiny, flowId, weight, dominanceRelation, checkLocalDominance, localDominanceValue))
	{
		return false;
	}

	return true;
}

bool TeDominantFlowNetwork::linkNetwork(const string& origin, const string& destiny, const string& flowId, const double& weight, const double& dominanceRelation, const bool& checkLocalDominance, const double& localDominanceValue)
{
// verifica se os nos ja estao na rede
	errorMessage_ = "";

	std::string destAux = destiny;
	std::string origAux = origin;

	TeDominantFlowTree::iterator itFlowOrigin = flow_.find(origin);

	if(itFlowOrigin == flow_.end())
	{	
		errorMessage_ = "Could not find an specified node origin (" + origin + ") in the network. Please, before doing a link, insert the specified node.";
		return false;
	}

	TeDominantFlowTree::iterator itFlowDest = flow_.find(destiny);

	if(itFlowDest == flow_.end())
	{	
		errorMessage_ = "Could not find an specified node destiny (" + destiny + ") in the network. Please, before doing a link, insert the specified node.";
		return false;
	}

	bool linked = false;

//verifica se o fluxo dominante é o fluxo interno ou nao, caso seja o fluxo interno (e se ele nao for o unico fluxo) altera o fluxo dominante para outro fluxo
// (itFlowOrigin == itFlowDest)   <==> (origin == destiny)
	if((itFlowOrigin == itFlowDest) && checkLocalDominance)
	{
		itFlowOrigin->second.flowId_      = flowId;
		itFlowOrigin->second.destinyId_   = destiny;
		itFlowOrigin->second.weightValue_ = weight;
		roots_.insert(origin);

		linked = true;
	}
	else if((itFlowOrigin == itFlowDest) && (itFlowOrigin->second.destinyNodes_.size() >= 2))
	{
		bool rootFlag = false;
		bool isDominated = false;
		int i = 0; 


// se estamos aqui eh porque se trata de um fluxo local!

		if(!linked)
		{
//verificar o maior fluxo que nao seja o fluxo interno
			for(i = (int)(itFlowOrigin->second.destinyNodes_.size()) - 1; i > 0; --i)
			{
				if(itFlowOrigin->second.destinyNodes_[i-1] != origin)
				{
					TeDominantFlowTree::iterator itFlowDestAux = flow_.find(itFlowOrigin->second.destinyNodes_[i-1]);

					if(itFlowDestAux == flow_.end())
					{	
						errorMessage_ = "Could not find an specified node destiny (" + destiny + ") in the network. Please, before doing a link, insert the specified node.";
						return false;
					}

					double totOut = 0.;

					for(unsigned int k = 0; k < itFlowOrigin->second.weightValues_.size(); ++k)
					{
						totOut += itFlowOrigin->second.weightValues_[k];
					}
	//verificar a relacao de dominancia
					double firstNodeWeight = itFlowOrigin->second.weightValues_[i-1];

					if(((firstNodeWeight * 100) / totOut) >= dominanceRelation)
					{
								
	// achei o primeiro noh (vindo do maior fluxo para o menor) que nao eh igual ao noh de origem
	// vamos fazer a ligacao baseado neste no destino encontrado
				
	// acrescenta as informacoes do fluxo dominante para o no de origem
						itFlowOrigin->second.flowId_      = itFlowOrigin->second.flows_[i-1];
						itFlowOrigin->second.destinyId_   = itFlowOrigin->second.destinyNodes_[i-1];
						itFlowOrigin->second.weightValue_ = itFlowOrigin->second.weightValues_[i-1];

	// determina se o no de origem e dominante ou dominado
					//if(itFlowOrigin->second.dominanceValue_ >= itFlowDestAux->second.dominanceValue_)
						if(itFlowOrigin->second.dominanceValue_ > itFlowDestAux->second.dominanceValue_)
						{
	// o noh eh dominante

	// entao basta marcar que ele faz parte da raiz da rede: os outros atributos deste noh jah foram atualizados corretamente
							set<string>::iterator findRoot = roots_.find(origin);
							if (findRoot == roots_.end())
							{
								roots_.insert(origin);
								rootFlag = true;
								linked = true;
							}
						}
						else
						{
							if (rootFlag)
							{
								set<string>::iterator findRoot = roots_.find(origin);
								roots_.erase(findRoot);
							}
	// o noh eh dominado

	// ajustar o atributo dominatedNodes_ do noh de destino (que domina este noh de origem) para apontar para este cara		
						itFlowDestAux->second.dominatedNodes_.push_back(origin);
						linked = true;
						isDominated = true;
						}
						if (isDominated)
							break;
					}
				}
			}
		}

// nao achei um fluxo sem ser o interno, entao vamos ligar o anterior mesmo.
		if(i < 0)
			linked = false;
    }

	if(!linked)
	{
		if(itFlowOrigin != itFlowDest)
		{
			vector<string>::iterator itVecDes = itFlowOrigin->second.destinyNodes_.begin();
			vector<double>::iterator itVecWei = itFlowOrigin->second.weightValues_.begin();
			double domValueOrig = itFlowOrigin->second.dominanceValue_;
			double domValue;
			string newDestiny = destAux;
			bool first = true;
			bool dominated = false;

	//percorre todos os destinos desta origem
			while(itVecDes != itFlowOrigin->second.destinyNodes_.end())
			{
	//caso este destino tenha o maior peso verifica seu valor de dominancia
				if(*itVecWei == weight)
				{
					std::string dest = *itVecDes;
					TeDominantFlowTree::iterator itFlowDom = flow_.find(dest);
	//se este destino tiver dominancia menor a origem domina este destino caso contrario entra no IF
					if(domValueOrig <= itFlowDom->second.dominanceValue_)
					{
						if (first)
						{
							domValue = itFlowDom->second.dominanceValue_;
							newDestiny = dest;
							first = false;
							dominated = true;
						}
	//verifica entre todos os destinos qual tem a menor dominancia maior que a da origem
						else if (itFlowDom->second.dominanceValue_ >= domValue)
						{
							domValue = itFlowDom->second.dominanceValue_;
							newDestiny = dest;
							dominated = true;
						}
					
					}
				}
				itVecDes++;
				itVecWei++;

			}
			
	// acrescenta as informacoes do fluxo dominante para o no de origem
			itFlowOrigin->second.flowId_      = flowId;
			itFlowOrigin->second.destinyId_   = newDestiny;
			itFlowOrigin->second.weightValue_ = weight;

	// determina se o no de origem e dominante ou dominado
	//if(itFlowOrigin->second.dominanceValue_ >= itFlowDest->second.dominanceValue_)
	//para comparacao de quem eh dominante ou dominado o valor de dominancia da
	//origem tem q ser obrigatoriamente maior que o do destino

			if(dominated == false)
			{
	// o noh eh dominante

	// entao basta marcar que ele faz parte da raiz da rede: os outros atributos deste noh jah foram atualizados corretamente
				roots_.insert(origin);
			}
			else
			{
	// o noh eh dominado
	// ajustar o atributo dominatedNodes_ do noh de destino (que domina este noh de origem) para apontar para este cara
				TeDominantFlowTree::iterator itFlowDest = flow_.find(newDestiny);

				if(itFlowDest != flow_.end())
				{
					itFlowDest->second.dominatedNodes_.push_back(origAux);
				}
			}
		}
		else
		{
			itFlowOrigin->second.flowId_      = flowId;
			itFlowOrigin->second.destinyId_   = destiny;
			itFlowOrigin->second.weightValue_ = weight;
			roots_.insert(origin);
		}
	}

	return true;
}

bool TeDominantFlowNetwork::insertIntoNetwork(const string& origin, const string& destiny, const string& flowId, const double& weight, map<string, double>& dominanceMap, const bool& calculateDom)
{
// Verificar se existe um noh com o valor da origem
	TeDominantFlowTree::iterator itFlowOrigin = flow_.find(origin);

	if(itFlowOrigin != flow_.end())
	{
// o noh ja existe
		itFlowOrigin->second.flows_.push_back(flowId);
		itFlowOrigin->second.destinyNodes_.push_back(destiny);
		itFlowOrigin->second.weightValues_.push_back(weight);
	}
	else
	{
// o noh nao existe, vamos inseri-lo na rede
		TeFlowNode newNode;
		newNode.originId_ = origin;

		if(!calculateDom)
		{
			newNode.dominanceValue_ = dominanceMap[origin];
		}

		newNode.flows_.push_back(flowId);
		newNode.destinyNodes_.push_back(destiny);
		newNode.weightValues_.push_back(weight);

		flow_.insert(pair<string, TeFlowNode>(origin, newNode));
	}

// Verificar se existe um noh com o valor de destino
	TeDominantFlowTree::iterator itFlowDest = flow_.find(destiny);

	if(itFlowDest != flow_.end())
	{
// o noh ja existe
		itFlowDest->second.flowInTot_ = itFlowDest->second.flowInTot_ + 1;
		itFlowDest->second.flowWeightSumIn_ = itFlowDest->second.flowWeightSumIn_ + weight;
	}
	else
	{
// o noh nao existe, vamos inseri-lo na rede
		TeFlowNode newNode;
		newNode.originId_ = destiny;

		if(!calculateDom)
		{
			newNode.dominanceValue_ = dominanceMap[destiny];
		}
		
		newNode.flowInTot_ = 1;
		newNode.flowWeightSumIn_ = weight;

		flow_.insert(pair<string, TeFlowNode>(destiny, newNode));
	}

	return true;
}

bool TeDominantFlowNetwork::calculateDominanceValue(const bool& tot1)
{
	TeDominantFlowTree::iterator itFlow = flow_.begin();

	while(itFlow != flow_.end())
	{
		if(tot1)
		{
			double domValue = 0.;

			for(unsigned int i = 0; i < itFlow->second.weightValues_.size(); ++i)
			{
				domValue += itFlow->second.weightValues_[i];
			}

			itFlow->second.dominanceValue_ = domValue;
		}
		else
		{
			itFlow->second.dominanceValue_ = itFlow->second.flowWeightSumIn_;
		}
		
		++itFlow;
	}

	return true;
}

bool TeDominantFlowNetwork::buildLevel()
{
// checa se o tema de refencia e o de fluxo estao carregados
	errorMessage_ = "";

	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return false;
	}

	if(!flowTheme_)
	{
		errorMessage_ = "No flow theme defined!";
		return false;
	}

	TeDominantFlowRoots::iterator itRoots = roots_.begin();

//determina no niveis de root como sendo 0
	int level = 0;

	while(itRoots != roots_.end())
	{
		string objId = *itRoots;

		TeDominantFlowTree::iterator itTree = flow_.find(objId);

		if(itTree != flow_.end())
			buildLevel(level, itTree->second);

		itRoots++;
	}
    return true;
}

bool TeDominantFlowNetwork::buildLevel(int level, TeFlowNode& flowNode)
{
	flowNode.level_ = level;
	
	for(unsigned int i = 0; i < flowNode.dominatedNodes_.size(); ++i)
	{
		TeDominantFlowTree::iterator itTree = flow_.find(flowNode.dominatedNodes_[i]);

		if(itTree != flow_.end())
			buildLevel(level + 1, itTree->second);		
	}
	return true;
}

bool TeDominantFlowNetwork::classifyNetwork(const unsigned int& cutLevel, const string& outputColumnName)
{
	TeDatabase* db = referenceTheme_->layer()->database();

	errorMessage_ = "";

	if(flow_.empty() || roots_.empty())
	{
		errorMessage_ = "You should create the network first!";

		return false;
	}

//define os nomes das colunas de classificacao dos fluxos
	string COD = outputColumnName + "_COD";
	string NAME = outputColumnName + "_NAME";
	string RELATION = outputColumnName + "_RELATION";

// Adiciona a coluna de saída
	TeAttributeRep at1;
	at1.type_ = TeINT;
	at1.isPrimaryKey_ = false;
	at1.name_ = COD;

//Adiciona coluna com nome dos fluxos
	TeAttributeRep at2;
	at2.type_ = TeSTRING;
	at2.name_ = NAME;
	at2.numChar_ = 255;

//Adiciona coluna com a relacao entre o fluxo interno/total de fluxos externos
	TeAttributeRep at3;
	at3.type_ = TeREAL;
	at3.name_ = RELATION;

	if(!db->addColumn(flowTableName_, at1))
	{
		errorMessage_ = "Couldn't add column to table!";

		return false;
	}

	if(!db->addColumn(flowTableName_, at2))
	{
		errorMessage_ = "Couldn't add column to table!";

		return false;
	}

	if(!db->addColumn(flowTableName_, at3))
	{
		errorMessage_ = "Couldn't add column to table!";

		return false;
	}
	

	string sql  = "UPDATE ";
	sql += flowTableName_;
	sql += " SET ";
	sql += at1.name_;
	sql += " = -1 ";
	sql += ", ";
	sql += at2.name_;
	sql += " = '' ";
	sql += ", ";
	sql += at3.name_;
	sql += " = 0 ";

	if(!db->execute(sql))
	{
		errorMessage_ = "Couldn't update table!";

		return false;
	}

	buildLevel();

	vector<string> newRoots;

	set<string>::iterator itRoots = roots_.begin();

	while(itRoots != roots_.end())
	{
		newRoots.push_back(*itRoots);
		++itRoots;
	}

//determina os ramos da rede a partir da dos nohs raizes
	labelBranchs(newRoots);

//Classificação
	vector<string>::iterator it = newRoots.begin();

	map<string, int> result;
	
	while(it != newRoots.end())
	{

		TeDominantFlowTree::iterator itFlow = flow_.find(*it);

		if(itFlow != flow_.end())
			classify(cutLevel, itFlow->second, result);

		++it;

	}

	TeDominantFlowTree::iterator itFlow = flow_.begin();

	if (TeProgress::instance())
		TeProgress::instance()->reset();
		

//Gravando o resultado da classificação

	map<string, int>::iterator itRes = result.begin();
	
	int dt = CLOCKS_PER_SEC/4;
	int dt2 = CLOCKS_PER_SEC * 5;
	clock_t	t0, t1, t2;
	t2 = clock();
	t0 = t1 = t2;

	unsigned int steps = result.size();
	unsigned int steps_aux = steps_aux = 0;

	if(TeProgress::instance())
	{
		TeProgress::instance()->setTotalSteps(steps);
		TeProgress::instance()->setCaption("Building MAP");
		TeProgress::instance()->setMessage("Processing...      ");
	}
	

	TeTable flowTable(flowTableName_);
	db->loadTableInfo(flowTable);
	//flowTable.linkName();
	
	while(itRes != result.end())
	{
		if(TeProgress::instance())
		{
			t2 = clock();
			if(int(t2-t1) > dt)
			{
				t1 = t2;
				
				if((int)(t2-t0) > dt2)
					TeProgress::instance()->setProgress(steps_aux);
			}
		}
		string sql = "UPDATE ";
		       sql += flowTableName_;
			   sql += " SET ";
			   sql += NAME;
			   sql += " = '";
			   sql += flowType_[itRes->second];
			   sql += "'";
			   sql += ", ";
			   sql += COD;
			   sql += " = ";
			   sql += Te2String(itRes->second);
			   sql += " WHERE ";
			   sql += "unique_id_";
			   sql += " = '";
			   sql += itRes->first;
			   sql += "'";

		db->execute(sql);

		if(TeProgress::instance()->wasCancelled())
		{
			TeProgress::instance()->reset();
			errorMessage_ = "The process was canceled by user!";

			return false;
		}

		++itRes;
		++steps_aux;
				
	}

	if (TeProgress::instance())
	TeProgress::instance()->reset();

//Gravando o resultado da relacao entre o fluxo interno/fluxo total externo
	double relation;
	string flowId;
	
	itFlow = flow_.begin();

	for (itFlow = flow_.begin(); itFlow != flow_.end(); ++itFlow)
	{
		vector<string>::iterator itDestNodes = itFlow->second.destinyNodes_.begin();
		vector<string>::iterator itFlows = itFlow->second.flows_.begin();
		vector<double>::iterator itWeightNode = itFlow->second.weightValues_.begin();
		
		double nodeWeight = 0.0;
		for(itDestNodes = itFlow->second.destinyNodes_.begin(); itDestNodes != itFlow->second.destinyNodes_.end(); ++itDestNodes)
		{
			double sumWeight = 0.0;
			vector<double>::iterator itWeightTotal = itFlow->second.weightValues_.begin();
			while(itWeightTotal != itFlow->second.weightValues_.end())
			{
				sumWeight += *itWeightTotal;
				++itWeightTotal;
			}
		
			nodeWeight = *itWeightNode;
			if (sumWeight == 0.0)
			{
				relation = 0.0;
			}
			else
			{
				relation = (nodeWeight / sumWeight);
			}
			
			flowId = *itFlows;
            
			sql  = "UPDATE ";
			sql += flowTableName_;
			sql += " SET ";
			sql += at3.name_;
			sql += " = ";
			sql += Te2String(relation, 2);
			sql += " WHERE ";
			sql += "unique_id_";
			sql += " = '";
			sql += flowId;
			sql += "'";
	
			if(!db->execute(sql))
			{
				errorMessage_ = "Couldn't update row: " +  itFlow->second.originId_ + "!" + sql + "vals " + Te2String(nodeWeight) + " "+ Te2String(nodeWeight);
				return false;
			}

			++itWeightNode;
			++itFlows;
		}

	}

// Update all the themes that uses these tables
	TeViewMap& viewMap = db->viewMap();
	TeViewMap::iterator it2;
	set<TeLayer*> layerSet;
	for (it2 = viewMap.begin(); it2 != viewMap.end(); ++it2)
	{
		TeView *view = it2->second;
		vector<TeViewNode*>& themesVector = view->themes();
		for (unsigned int i = 0; i < themesVector.size(); ++i)
		{
			TeTheme *theme = (TeTheme*)themesVector[i];
			if(theme->isThemeTable(flowTableName_))
			{
				theme->loadThemeTables();
				layerSet.insert(theme->layer());
			}
		}
	}

// Update the layer tables affected
	set<TeLayer*>::iterator setIt;
	for (setIt = layerSet.begin(); setIt != layerSet.end(); ++setIt)
		(*setIt)->loadLayerTables();

	return true;	
}

bool TeDominantFlowNetwork::checkConnections(vector<string>& disconnecteds)
{
//----- This is used for progress bar
	int dt = CLOCKS_PER_SEC/4;
	int dt2 = CLOCKS_PER_SEC * 5;
	clock_t	t0, t1, t2;
	t2 = clock();
	t0 = t1 = t2;
//-----

	bool check = false;

	TeDominantFlowTree::iterator itFlow = flow_.begin();

	unsigned int steps = flow_.size();
	unsigned int steps_aux = 0;
	
	if(TeProgress::instance())
	{
		TeProgress::instance()->setTotalSteps(steps);
		TeProgress::instance()->setCaption("Checking Destiny Nodes");
		TeProgress::instance()->setMessage("Processing...      ");
	}

	while(itFlow != flow_.end())
	{
		if(TeProgress::instance()->wasCancelled())
		{
			TeProgress::instance()->reset();
			errorMessage_ = "The process was canceled by user!";

			return false;
		}

		if (itFlow->second.destinyId_.empty())
		{
			disconnecteds.push_back(itFlow->second.originId_);
			check = true;
		}

		if(TeProgress::instance())
		{
			t2 = clock();
			if(int(t2-t1) > dt)
			{
				t1 = t2;
				
				if((int)(t2-t0) > dt2)
					TeProgress::instance()->setProgress(steps_aux);
			}
		}
		steps_aux++;
		++itFlow;
	}

	if(TeProgress::instance())
			TeProgress::instance()->reset();

	return check;
}

bool TeDominantFlowNetwork::setAsRootLevel(vector<string>& disconnecteds)
{
// verifica se os nos ja estao na rede
	errorMessage_ = "";

	for(unsigned int i = 0; i < disconnecteds.size(); ++i)
	{
        TeDominantFlowTree::iterator itNode = flow_.find(disconnecteds[i]);

        if(itNode == flow_.end())
		{	
			errorMessage_ = "Could not find an specified node (" + disconnecteds[i] + ") in the network. Please, before doing a link, insert the specified node.";
			return false;
		}

		roots_.insert(disconnecteds[i]);
	}

	return true;
}

void TeDominantFlowNetwork::labelBranchs(const int& branchId, TeFlowNode& node)
{
	node.branchId_ = branchId;

	for(unsigned int i = 0; i < node.dominatedNodes_.size(); ++i)
	{
		TeDominantFlowTree::iterator it2 = flow_.find(node.dominatedNodes_[i]);

		if(it2 != flow_.end())
			labelBranchs(branchId, it2->second);
	}

}

void TeDominantFlowNetwork::labelBranchs(vector<string>& newRoots)
{
	for(unsigned int i = 0; i < newRoots.size(); ++i)
	{
		TeDominantFlowTree::iterator it2 = flow_.find(newRoots[i]);
	
		if(it2 != flow_.end())
			labelBranchs(i, it2->second);
	}
}

void TeDominantFlowNetwork::classify(int cutLevel, map<string, int>& result,
									TeFlowNode& node_origin, TeFlowNode& node_destiny,
									const string& flowId)
{
	int aidLevel = node_origin.level_ - node_destiny.level_;
	int aidBranch = node_origin.branchId_ - node_destiny.branchId_;
	
	//INFERIOR origem e destino abaixo do cut level (area de interesse)
	if((node_origin.level_ >= cutLevel) && (node_destiny.level_ >= cutLevel))
	{
		//mesmo nivel
		if(aidLevel == 0)
		{
			// mesma rede
			if(aidBranch == 0) 
			{
				result[flowId] = 9; 
				
			} //Transversal Horizontal Mesma rede
			else 
			{
				result[flowId] = 10; 
				
			} //Transversal Horizontal Entre redes
		}
		else
		{
			// um nível acima
			if(aidLevel == 1)
			{
				if(aidBranch == 0) 
				{
					result[flowId] = 1; 
				
				} //Hierarquico Ascendente Direto
				else 
				{
					result[flowId] = 6; 
				
				} //Transversal Ascendente Entre redes
			}
			else
			{
				// um nível abaixo
				if(aidLevel == -1)
				{
					if(aidBranch == 0)  
					{
						result[flowId] = 3; 
				
					} //Hierarquico Descendente Direto
					else  
					{
						result[flowId] = 8; 
				
					} //Transversal Descendente Entre redes
				}
				else
				{
					//mais que um nivel acima
					if(aidLevel > 1)
					{
						//verifica se origem e destino pertencem ao mesmo ramo
						if(!checkNode(node_origin, node_destiny))
						{
							if(aidBranch == 0) 
							{
								result[flowId] = 5; 
								//Transversal Ascendente Mesma Rede
							} 
							else 
							{
								result[flowId] = 6;
								//Transversal Ascendente Entre redes
							} 
						}
						else
						{
							if(aidBranch == 0) 
							{
								result[flowId] = 2; 
								//Hierarquico Ascendente CurtoCircuito
							} 
							else 
							{
								result[flowId] = 6; 
								//Transversal Ascendente Entre redes
							} 
						}
					}
					else
					{
						//mais que um nivel abaixo
						if(aidLevel < 1)
						{
							//verifica se origem e destino pertencem ao mesmo ramo
							if(!checkNode(node_origin, node_destiny))
							{
								if(aidBranch == 0) 
								{
									result[flowId] = 7; 
									//Transversal Descendente Mesma Rede
				
								} 
								else 
								{
									result[flowId] = 8; 
									//Transversal Descendente Entre redes
								} 
							}
							else
							{
								if(aidBranch == 0) 
								{
									result[flowId] = 4;
									//Hierarquico Descendente CurtoCircuito
								} 
								else 
								{
									result[flowId] = 8; 
									//Transversal Descendente Entre redes
								} 
							}
						}
					}
				}
			}
		}
	}
	else 
	{
		//Exo (Ascendente)
		if((node_origin.level_ >= cutLevel) && (node_destiny.level_ <= cutLevel))
		{
			if(aidLevel >= 1)
			{
				if(aidBranch == 0) 
				{
					result[flowId] = 14; 
				
				} //Exo Ascendente Mesma Rede
				else 
				{
					result[flowId] = 15; 
				
				} //Exo Ascendente Entre Redes
			}
		}
		else
		{
			//Endo (Descendente)
			if((node_origin.level_ <= cutLevel) && (node_destiny.level_ >= cutLevel))
			{
				if(aidLevel <= 1)
				{
					if(aidBranch == 0) 
					{
						result[flowId] = 16; 
				
					} //Endo Descendente Mesma Rede
					else 
					{
						result[flowId] = 17; 
				
					} //Endo Descendente Entre Redes
				}
			}
			else
			{
				//SUPER origem e destino acima do cut level
				if((node_origin.level_ < cutLevel) && (node_destiny.level_ < cutLevel))
				{
					//mesmo nivel(horizontal)
					if(aidLevel == 0)
					{
						if(aidBranch == 0) 
						{
							result[flowId] = 13; 
				
						}//Transversal Horizontal Mesma rede
						else 
						{
							result[flowId] = 13; 
				
						}//Transversal Horizontal Entre redes
					}
					else
					{
						//ascendente
						if(aidLevel >= 1)
						{
							if(aidBranch == 0) 
							{
								result[flowId] = 11; 
				
							}//Transversal Ascendente Mesma rede
							else 
							{
								result[flowId] = 11; 
				
							}//Transversal Ascendente Entre redes
						}
						else
						{
							//descendente
							if(aidLevel < 1)
							{
								if(aidBranch == 0) 
								{
									result[flowId] = 12; 
				
								} //Transversal Descendente Mesma rede
								else 
								{
									result[flowId] = 12; 
				
								}//Transversal Descendente Entre redes
							}
						}
					}
				}
			}
		}
	}

	
	
}

void TeDominantFlowNetwork::classify(int cutLevel, TeFlowNode& node, map<string, int>& result)
{
	unsigned int i = 0;
	for(i = 0 ; i < node.destinyNodes_.size() ; i++)
	{
		if(node.originId_ != node.destinyNodes_[i])
		{
			TeDominantFlowTree::iterator itDestiny = flow_.find(node.destinyNodes_[i]);
			classify(cutLevel, result, node,  itDestiny->second, node.flows_[i]);
		}
		else
		{
			result[node.flows_[i]] = 18;//ligacao local
		}

	}


	for(i = 0; i < node.dominatedNodes_.size(); ++i)
	{
		string objId = node.dominatedNodes_[i];

		TeDominantFlowTree::iterator it = flow_.find(objId);

		if(it != flow_.end())
		{
			classify(cutLevel, it->second, result);
		}		
	}

}

bool TeDominantFlowNetwork::checkNode(TeFlowNode& node_origin, TeFlowNode& node_destiny)
{
	if(node_destiny.destinyNodes_.size() == 0)
		return false;

	vector<string>::iterator it = find(node_destiny.destinyNodes_.begin(), (node_destiny.destinyNodes_.end() - 1),node_origin.originId_);
	if (it == node_destiny.destinyNodes_.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void TeDominantFlowNetwork::findIsolatedRoots()
{
	std::vector<std::string> removeRoots;

	TeDominantFlowRoots::iterator it = roots_.begin();

	while(it != roots_.end())
	{
		std::string root = (*it);

		TeDominantFlowTree::iterator itFlow = flow_.find(root);

		if(itFlow != flow_.end() && itFlow->second.destinyId_ == root)
		{
			if((itFlow->second.dominatedNodes_.size() == 1 && itFlow->second.dominatedNodes_[0] == root) || itFlow->second.dominatedNodes_.empty())
			{
				isolateds_.insert(root);
				removeRoots.push_back(root);
			}
		}

		++it;
	}

	for(unsigned int i = 0; i < removeRoots.size(); ++i)
	{
		TeDominantFlowRoots::iterator it = roots_.find(removeRoots[i]);

		if(it != roots_.end())
		{
			roots_.erase(it);
		}
	}
}

int TeDominantFlowNetwork::getMaxLevel()
{
	int maxLevel = 0;

	TeDominantFlowTree::iterator itFlow = flow_.begin();

	while(itFlow != flow_.end())
	{
		if(itFlow->second.level_ > maxLevel)
		{
			maxLevel = itFlow->second.level_;
		}

		++itFlow;
	}

	return maxLevel;
}

int TeDominantFlowNetwork::getNetNodesSize(const std::string& objId)
{
	int value = 0;

	TeDominantFlowTree::iterator itFlow = flow_.find(objId);

	if(itFlow != flow_.end())
	{
		value = itFlow->second.dominatedNodes_.size();

		for(unsigned int i = 0; i < itFlow->second.dominatedNodes_.size(); ++i)
		{
			std::string destiny = itFlow->second.dominatedNodes_[i];

			if(destiny != objId)
			{
				getDomiantedNodesSize(itFlow->second.dominatedNodes_[i], value);
			}
			else
			{
				--value;
			}
		}
	}

	return value;
}

void TeDominantFlowNetwork::getDomiantedNodesSize(const std::string& objId, int& value)
{
	TeDominantFlowTree::iterator itFlow = flow_.find(objId);

	if(itFlow != flow_.end())
	{
		value += itFlow->second.dominatedNodes_.size();

		for(unsigned int i = 0; i < itFlow->second.dominatedNodes_.size(); ++i)
		{
			std::string destiny = itFlow->second.dominatedNodes_[i];

			if(destiny != objId)
			{
				getDomiantedNodesSize(itFlow->second.dominatedNodes_[i], value);
			}
			else
			{
				--value;
			}
		}
	}
}
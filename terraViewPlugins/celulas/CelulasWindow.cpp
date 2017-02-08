/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright © 2001-2004 INPE and Tecgraf/PUC-Rio.     

This file is part of TerraView. TerraView is free software; you can redistribute it 
and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

You should have received a copy of the GNU General Public License
along with TerraView.   
The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The software provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use of
this program and its documentation.
*************************************************************************************/

// Plugin's includes
#include "CelulasWindow.h"
#include "help.h"

// TerraLib's includes
#include <TeCellAlgorithms.h>
#include <TeComputeAttributeStrategies.h>

// TerraView's includes
#include <TeDatabaseFactory.h>
#include <TeWaitCursor.h>
#include <TeQtViewsListView.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeQtDatabaseItem.h>
#include <TeAgnostic.h>

#include "TeGeometryAlgorithms.h"
#include "TeOverlay.h"
#include <TeSTEFunctionsDB.h>
using namespace TeOVERLAY;

// Qt's includes
#include <qlineedit.h>
#include <qstring.h>
#include <qlistbox.h>
#include <qcombobox.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qradiobutton.h>

#define MAXINT 2147483647

using namespace std;

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif


void TeFillCellStepLoadingData2()
{
	if(TeProgress::instance())
	{
		TeProgress::instance()->setTotalSteps(2);
		TeProgress::instance()->setMessage("Step 1/3: Loading data");
		TeProgress::instance()->setProgress(1);
	}
	return;
}

void TeFillCellStepStartProcessing2(int steps)
{
	if (TeProgress::instance())
	{
		TeProgress::instance()->reset();
		TeProgress::instance()->setTotalSteps(steps);
		TeProgress::instance()->setMessage("Step 2/3: Processing");
	}
}

bool TeFillCellStepNextStep2(int step)
{
	if(TeProgress::instance())
	{
		if (TeProgress::instance()->wasCancelled())
		{
			TeProgress::instance()->reset();
			return false;
		}
		else
			TeProgress::instance()->setProgress(step);
	}
	return true;
}

void TeFillCellStepWriting2()
{
	if (TeProgress::instance())
	{
		TeProgress::instance()->reset();
		TeProgress::instance()->setMessage("Step 3/3: Writing results");
	}
}


struct cellDist2
{
	double distance;
	TeCellSet::iterator ref;
	bool done;
	bool valid;
};


vector<vector<cellDist2> > BuildNeighbors2(TeCellSet cells)
{
	vector<vector<cellDist2> > result;
	int line = 0, column = 0;

	// look for the number of lines and columns of the set
	for(TeCellSet::iterator cell_it = cells.begin(); cell_it != cells.end(); cell_it++)
	{
		if( (*cell_it).column() > column) column = (*cell_it).column();
		if( (*cell_it).line  () > line)   line   = (*cell_it).line  ();
	} 
	
	line++;
	column++;

	// initialize the neighbors	
	result.resize(line);
	for(int i = 0; i < line; i++)
	{
		result[i].resize(column);
		for(int j = 0; j < column; j++)
		{
			result[i][j].valid    = false;
			result[i][j].done     = false;
		    result[i][j].distance = 0;
		}
	}

	// set the values for the ones that have a cell
	for(TeCellSet::iterator cell_it = cells.begin(); cell_it != cells.end(); cell_it++)
	{
		result[ (*cell_it).line() ][ (*cell_it).column() ].ref   = cell_it;
		result[ (*cell_it).line() ][ (*cell_it).column() ].valid = true;
	} 
	
	return result;
}

struct procDist2
{
	int line;
	int column;
	TeBox box;
};


bool CalculateDistance2(unsigned int line, unsigned int column, TeBox box, TeTheme* theme, double r, TeGeomRep rep, vector<vector<cellDist2> >& Neighbors, list<procDist2>& procList)
{
	if( Neighbors[line][column].done || !Neighbors[line][column].valid )
	{
		return false;
	}

	TeCellSet::iterator cell_it = Neighbors[line][column].ref; 
	
	TeQuerierParams params(true, false);
	params.setParams (theme);
    params.setSpatialRest(box, TeWITHIN, rep);

	TeQuerier querier(params);

    querier.loadInstances();
	int quant = querier.numElemInstances();
	TeCoord2D p1 = (*cell_it).box().center();

	double dist = TeMAXFLOAT;

	if(rep == TePOINTS)
	{
		for(int i = 0; i < quant; i++)
		{
		    TeSTInstance sti;
		    querier.fetchInstance(sti);
			TeMultiGeometry mg = sti.geometries();
	
			double x = TeDistance(p1, mg.getPoints()[0].location());
			if (dist > x) dist = x;
		}
	}
	if(rep == TeLINES)
	{
		for(int i = 0; i < quant; i++)
		{
		    TeSTInstance sti;
		    querier.fetchInstance(sti);
			TeMultiGeometry mg = sti.geometries();

			TeLineSet ls = mg.getLines();

    		int line_index;
		    TeCoord2D pinter;
			double x;
    		TeNearest (p1, ls, line_index, pinter, x);
            if (dist > x) dist = x;
		}
	}
	if(rep == TePOLYGONS)
	{
		for(int i = 0; i < quant; i++)
		{
		    TeSTInstance sti;
		    querier.fetchInstance(sti);
			TeMultiGeometry mg = sti.geometries();
			TePolygonSet ps = mg.getPolygons();

		    TePolygonSet box_ps, intersect;
		    TePolygon pol = polygonFromBox((*cell_it).box());
		    box_ps.add(pol);

		    TeOverlay(box_ps, ps, intersect, TeINTERSECTION);
			unsigned int kk = 0;
		    if(!intersect.empty()) { dist = 0; kk = ps.size(); }
		   
			for (; kk < ps.size(); kk++)
			{
				TePolygon p = ps[kk];
				for (unsigned int j = 0; j < p.size(); j++)
				{
					TeLinearRing l = p[j];
					for (unsigned int k = 0; k < l.size(); k++)
					{
						double d = 	TeDistance (p1, l[k]);
						if (dist > d) dist = d;
					}
				}
			}
		}
	}

	Neighbors[line][column].done     = true; 
	Neighbors[line][column].distance = dist; 

	TeBox searchbox(p1.x() - dist - 1.001*r, p1.y() - dist - 1.001*r,
                    p1.x() + dist + 1.001*r, p1.y() + dist + 1.001*r);

	procDist2 pd;
	pd.box = searchbox;
	if(line > 0)                         { pd.line = line-1; pd.column = column;   procList.push_back(pd); }
	if(column > 0)                       { pd.line = line;   pd.column = column-1; procList.push_back(pd); }
    if(line < Neighbors.size() - 1)      { pd.line = line+1; pd.column = column;   procList.push_back(pd); }
	if(column < Neighbors[0].size() - 1) { pd.line = line;   pd.column = column+1; procList.push_back(pd); }
	
	return true;
}


bool TeFillCellDistanceOperation2(TeTheme* theme,
								 TeGeomRep rep,
								 TeLayer* cell_layer,
								 const string& cell_tablename,
								 const string& output_columnName,
								 TeTimeInterval t)
{
	TeFillCellStepLoadingData2();

	TeCellSet cells;
	if (!TeFillCellInitLoad (cell_layer, cell_tablename, cells)) return false;

	list<procDist2> procList;
	vector<vector<cellDist2> > Neighbors = BuildNeighbors2(cells);

	TeFillCellStepStartProcessing2(cells.size()*2);

	TePropertyVector result;
	TeCellSet::iterator cell_it = cells.begin();

	double res = cells.resX();
	if( res < cells.resY () ) res = cells.resY();
	
	procDist2 pd;

	pd.line   = (*cell_it).line();
	pd.column = (*cell_it).column();
	pd.box    = theme->getThemeBox();

	procList.push_back(pd);
	int count = 0; // counting the progress

	while(!procList.empty())
	{
		pd = procList.front();
		procList.pop_front();
		if(CalculateDistance2(pd.line, pd.column, pd.box, theme, res, rep, Neighbors, procList))
			if(!TeFillCellStepNextStep2(++count)) return false;
	}

	TeTable table(cell_tablename);
	cell_layer->database()->loadTableInfo(table);

	TeSTElementSet cellObjSet (cell_layer);
	//include the attribute description
	TeAttribute attr;
	attr.rep_.type_ = TeREAL;
	attr.rep_.decimals_ = 10;
	attr.rep_.name_ = output_columnName;
	cellObjSet.addProperty(attr);

	while (cell_it != cells.end())
	{
	    string value_ = Te2String(Neighbors[(*cell_it).line()][(*cell_it).column()].distance);
		vector<string> attrValues;
		attrValues.push_back(value_);
	    
		TeFillCellInitSTO ((*cell_it), attrValues, cellObjSet, table.tableType(), t);
		cell_it++;

		if(!TeFillCellStepNextStep2(++count)) return false;
	}
	
	TeFillCellStepWriting2();

	if (!TeUpdateDBFromSet(&cellObjSet, cell_tablename))
			return  false;

	return true;
}

bool TeFillCellCategoryAreaPercentageOperation2(TeTheme* theme,
											   const string attrName,
								 			   TeLayer* cell_layer,
											   const string& cell_tablename,
											   map<string, string>& classesMap,
											   TeTimeInterval t)
{
	TeFillCellStepLoadingData2();

	TeCellSet cells;
	if (!TeFillCellInitLoad (cell_layer, cell_tablename, cells)) return false;

	TeSTElementSet cellObjSet (cell_layer);

	TeTable table(cell_tablename);
	cell_layer->database()->loadTableInfo(table);

	TeCellSet::iterator cell_it = cells.begin();

	TeFillCellStepStartProcessing2(cells.size());
	int count = 0;
	while (cell_it != cells.end())
	{
	    TeProperty  prop;

		// COMPUTE
    // convert input box to a polygonset
	    TePolygonSet box_ps;
	    TePolygon pol = polygonFromBox((*cell_it).box());
	    box_ps.add(pol);
	    map<string, double>  areaMap;
	    double partial_area = 0.0;

	    // Build stoset
	    TeSTElementSet stos (theme);
	    vector<string> attrNames;
	    attrNames.push_back (attrName); 
	    if (TeSTOSetBuildDB (&stos, true, false, attrNames))
        {   
        
            // initialize areMap
            map<string, string>::iterator itMap = classesMap.begin();
            while (itMap != classesMap.end())
            {
                areaMap[(*itMap).second] = 0;
                ++itMap;
            }
            partial_area = 0.0;
            // iterate over all objects that intercept the box
            TeSTElementSet::iterator it = stos.begin();
            while (it != stos.end())
            {
                string category;
                if ((*it).getPropertyValue (attrName, category))
                {   
                    TePolygonSet objGeom;  
                    if ((*it).getGeometry(objGeom))
                    {
                        TePolygonSet intersect;
                        TeOverlay(box_ps, objGeom, intersect, TeINTERSECTION);
                        double intersectArea = TeGeometryArea (intersect);
                        partial_area += intersectArea;
                        areaMap[classesMap[category]] +=  intersectArea; // suppose objects are disjoint
                    }
                }
                ++it;
            }
        }

	    double areaTot = TeGeometryArea (box_ps);
	    double perc_total = 0.0;
	    double corrected_perc_total = 0.0;
	    vector<double> vec_percent;

		map <string, double>:: iterator areaIt = areaMap.begin();
	    while(areaIt != areaMap.end())  
	    {
	        double percent = 0.0;
	        if (areaTot != 0) percent = (*areaIt).second/areaTot;
	        perc_total += percent;
	        vec_percent.push_back (percent);
			
			//attribute list
			TeAttribute at;
			at.rep_.name_ = attrName + (*areaIt).first;
			at.rep_.type_ = TeREAL;
			at.rep_.decimals_ = 10;
			cellObjSet.addProperty(at);
	        ++areaIt;
	    }

		//fill the object set
		int i= 0;
	    areaIt = areaMap.begin();
	    while(areaIt != areaMap.end())  
	    {
	        vector<string> values;
			double percent = vec_percent[i];
	        corrected_perc_total += percent;
	        values.push_back(Te2String (percent));
	        ++areaIt;
	        i++;
			TeFillCellInitSTO ((*cell_it), values, cellObjSet, table.tableType(), t);
	    }

		cell_it++;
		if(!TeFillCellStepNextStep2(++count)) return false;
	} 

	TeFillCellStepWriting2();
	bool result = TeUpdateDBFromSet(&cellObjSet, cell_tablename);
	cell_layer->loadLayerTables();

	return result;
}

bool TeFillCellCategoryAreaPercentageOperation2(TeTheme* theme,
											   const string attrName,
								 			   TeLayer* cell_layer,
											   const string& cell_tablename,
											   TeTimeInterval t)
{
	map<string, string> classesMap;
	vector< string > atts;
	string value;
	TeQuerier* querier;
	TeSTInstance sti;

	atts.push_back(attrName);

 	TeQuerierParams params (false, atts);
	params.setParams (theme);

	querier = new TeQuerier(params);

	querier->loadInstances();

	while( querier->fetchInstance(sti) )
	{
		value = sti.getPropertyVector()[0].value_;
		classesMap[value] = value;

	}
	delete querier;

	return TeFillCellCategoryAreaPercentageOperation2(theme, attrName, cell_layer, cell_tablename, classesMap, t);
}

bool TeFillCellNonSpatialRasterOperation2 (TeDatabase* db,
										  const string& input_raster_layername, 
										  TeComputeAttrStrategy<TeBoxRasterIterator>* operation,
										  const string& cell_layername, 
										  const string& cell_tablename, 
										  const string& output_columnName,
										  TeTimeInterval t)
{
	TeFillCellStepLoadingData2();

	if (!db) return false;

	// Load input layers
	TeLayer* input_layer = new TeLayer (input_raster_layername);
	if (!db->loadLayer (input_layer))
	{
		 db->close();
		 return false;
	}

	TeTable table(cell_tablename);
	db->loadTableInfo(table);

	TeRaster* raster = input_layer->raster();
	if (!raster) return false;

	// Load output cells layer with geometry previously created
	TeLayer* cell_layer = new TeLayer (cell_layername);
	if (!db->loadLayer (cell_layer))
	{
		 db->close();
		 return false;
	}

	// Initialize cell set
	TeCellSet cells;
	if (!TeFillCellInitLoad (cell_layer, cell_tablename, cells)) return false;

	// Initialize object set to store cell properties
	TeSTElementSet cellObjSet (cell_layer);
	//include the attribute description
	TeAttribute attr;
	attr.rep_.type_ = TeREAL;
	attr.rep_.decimals_ = 10;
	attr.rep_.name_ = output_columnName;
	cellObjSet.addProperty(attr);

	TeFillCellStepStartProcessing2(cells.size());

	// Process
	TePropertyVector result;
	TeCellSet::iterator cell_it = cells.begin();

	int count = 0;
	while (cell_it != cells.end())
	{
		TeBoxRasterIterator rasterItBegin(raster, (*cell_it).box());
		rasterItBegin.begin();
		TeBoxRasterIterator rasterItEnd(raster, (*cell_it).box());
		rasterItEnd.end();
		result = operation->compute (rasterItBegin, rasterItEnd, output_columnName);										
		TeFillCellInitSTO ((*cell_it), result, cellObjSet, table.tableType(), t);
		cell_it++;

		if(!TeFillCellStepNextStep2(++count)) return false;
	} 

	TeFillCellStepWriting2();
	// Update DB
	if (!TeUpdateDBFromSet (&cellObjSet, cell_tablename))
		return  false;
	return  true;
}

bool TeFillCellCategoryCountPercentageRasterOperation2(TeDatabase* db,
													  string raster_layername,
													  string cell_layername,
													  const string& cell_tablename,
													  const string attrName,
													  TeTimeInterval t)
{
	TeFillCellStepLoadingData2();

	map<string, string> classesMap;
	vector< string > atts;
	string value;

	TeLayer* input_layer = new TeLayer (raster_layername);
	if (!db->loadLayer (input_layer))
	{
		 db->close();
		 return false;
	}

	atts.push_back(attrName);

	vector<double> values;
	bool found;
	unsigned int i;
	TeRaster* raster = input_layer->raster();
	TeRaster::iterator rasterItBegin = raster->begin();
	TeRaster::iterator rasterItEnd   = raster->end();

	TeTable table(cell_tablename);
	db->loadTableInfo(table);

	TeLayer* cell_layer = new TeLayer (cell_layername);
	if (!db->loadLayer (cell_layer))
	{
		 db->close();
		 return false;
	}

	// Initialize cell set
	TeCellSet cells;
	if (!TeFillCellInitLoad (cell_layer, cell_tablename, cells)) return false;

	// Initialize object set to store cell properties
	TeSTElementSet cellObjSet (cell_layer);

	// looking for the unique values in all the raster
	TeFillCellStepStartProcessing2(raster->params().nlines_ * raster->params().ncols_ + cells.size());

	int count = 0;
	while(rasterItBegin != rasterItEnd)
	{
		found = false;
		for(i = 0; i < values.size(); ++i)
			if(values[i] == (*rasterItBegin)[0])
				found = true;
		if(!found) 
			values.push_back((*rasterItBegin)[0]);
		++rasterItBegin;

		if(!TeFillCellStepNextStep2(++count)) return false;
	}

	// Process
	TePropertyVector result;
	map<double, int> counter;

	// set the propertyvector of the cell object set
	for(i = 0; i < values.size(); i++)
	{
		stringstream str;

		str << values[i];
        string value_ =  str.str();
        for (unsigned j = 0; j < value_.size(); j++)
            if(value_[j] == '.' || value_[j] == '+')
                value_[j] = '_';

        TeProperty prop;
        prop.attr_.rep_.name_ = attrName + value_;
        prop.attr_.rep_.type_ = TeREAL;
        prop.attr_.rep_.numChar_ = 48;
		prop.attr_.rep_.decimals_ = 10;

        cellObjSet.addProperty(prop.attr_);
	}

	TeCellSet::iterator cell_it = cells.begin();
	vector<string> vprop;

	while (cell_it != cells.end())
	{
		double total = 0.0;
		TePolygon p = polygonFromBox((*cell_it).box());
		TeRaster::iteratorPoly rasterItBegin(raster);
		rasterItBegin = raster->begin(p, TeBBoxPixelInters, 0);
		TeRaster::iteratorPoly rasterItEnd(raster);
		rasterItEnd = raster->end(p, TeBBoxPixelInters, 0);

		for(i = 0; i < values.size(); i++)
			counter[values[i]] = 0;

		while(rasterItBegin != rasterItEnd)
		{
			double pixel = (*rasterItBegin);
			counter[pixel]++;
			++rasterItBegin;
			total++;
		}		

		//add the attribute values in the object set
		vprop.clear();
		for(i = 0; i < values.size(); i++)
		{
			stringstream str;
			if (total > 0)
				str << counter[values[i]] / total;
			else
				str << "0.0";

			vprop.push_back(str.str());
		}

		TeFillCellInitSTO ((*cell_it), vprop, cellObjSet, table.tableType(), t);
		cell_it++;

		if(!TeFillCellStepNextStep2(++count)) return false;
	} 

	TeFillCellStepWriting2();
	// Update DB
	if (!TeUpdateDBFromSet (&cellObjSet, cell_tablename))
		return  false;
	return  true;
}

CelulasWindow::CelulasWindow( QWidget* parent )
: CelulasForm( parent, "CelulasWindow" , Qt::WType_TopLevel | Qt::WShowModal )
{   
}


CelulasWindow::~CelulasWindow()
{
}

void CelulasWindow::UpdateForm( const PluginParameters& pp )
{
	// Atualiza parametros do plugin
	plugin_params_ = pp;

	//Limpa grid do Tema atual
	if(plugin_params_.teqtgrid_ptr_)
		plugin_params_.teqtgrid_ptr_->clear();

	//Verifica se existe um BD conectado
	if(!plugin_params_.current_database_ptr_)
	{
		QMessageBox::critical( this, "Error", tr( "Database not connected!" ) );
		hide();
		return;
	}

	//Verifica se existe pelo menos um PI Celular no BD corrente
	if(plugin_params_.current_database_ptr_->layerMap().size()==0)
	{
		QMessageBox::critical( this, "Error", tr( "Nenhum Plano de Informação Celular foi encontrado!" ) );
		hide();
		return;
	}

	//Se tiver conexao, exibe janela
	show();

	// Limpa combo de layer celulares
	layerComboBox->clear();

	//Preenche o combo de PIs para o Layer de Celulas com a lista de PIs do BD corrente
	TeLayerMap& layerMap = plugin_params_.current_database_ptr_->layerMap();
	TeLayerMap::iterator layerIt;
	for (layerIt = layerMap.begin(); layerIt != layerMap.end(); ++layerIt)
	{
		//Verifica se o PI e do tipo de Celulas p/ preencher combo
		if ((layerIt->second) && (layerIt->second->geomRep() & TeCELLS))
			layerComboBox->insertItem(QString(layerIt->second->name().c_str()));
	}

	//Se existir um PI corrente(setado na arvore de PIs), seta ele como item corrente no combo de PI para o Layer de Celulas
	if (plugin_params_.current_layer_ptr_ != 0)
	{
		int i = 0;
		for (layerIt = layerMap.begin(); layerIt != layerMap.end(); ++layerIt)
		{
			if(layerIt->second->id() == plugin_params_.current_layer_ptr_->id())
			{
				layerComboBox->setCurrentItem(i);
				break;
			}
			++i;
		}
	}
	else
		layerComboBox->setCurrentItem(0);

	if(layerComboBox->count()==1)
		layerComboBox->setEnabled(false);
	if(layerComboBox->count()>1)
		layerComboBox->setEnabled(true);


	layerComboBox_activated(layerComboBox->currentText());

	// Limpa combo de temas de entrada
	themaDataComboBox->clear();

	// Preenche o combo de temas com os temas disponíveis
	TeThemeMap& themeMap = plugin_params_.current_database_ptr_->themeMap();
	TeThemeMap::iterator themeIt;
	for (themeIt = themeMap.begin(); themeIt != themeMap.end(); ++themeIt)
		themaDataComboBox->insertItem(QString(themeIt->second->name().c_str()));

	themaDataComboBox->setFocus();
	themaDataComboBox_activated(themaDataComboBox->currentText());
	tableComboBox_activated(tableComboBox->currentText());
}

void CelulasWindow::NovaTabelaradioButton_clicked()
{
	NomeNovaTableTextLabel->setEnabled(true);
	NovoNomeTabelaLineText->setEnabled(true);
	tableTextLabel->setEnabled(false);
	tableComboBox->setEnabled(false);

	atributoTextLabel->setEnabled(false);
	atributoComboBox->setEnabled(false);
	SelecionarTemporadioButton->setEnabled(false);
	tempoComboBox->setEnabled(false);
	NovoTemporadioButton->setEnabled(true);

	//Se vou criar nova tabela temporal, nao posso utilizar atributo existente
	SelecionarAtributoradioButton->setEnabled(false);
	//Preciso criar um novo tempo
	NovoTemporadioButton->setChecked(true);
	NovoTemporadioButton_clicked();
}

void CelulasWindow::SelecionarTabelaradioButton_clicked()
{
	NomeNovaTableTextLabel->setEnabled(false);
	NovoNomeTabelaLineText->setEnabled(false);
	tableTextLabel->setEnabled(true);
	tableComboBox->setEnabled(true);

	tempoInicialLineText->setEnabled(false);
	tempoFinalLineText->setEnabled(false);
	dateFormatComboBox->setEnabled(false);
	timeFormatComboBox->setEnabled(false);
	PMComboBox->setEnabled(false);
	dateSepLineEdit->setEnabled(false);
	timeSepLineEdit->setEnabled(false);
	tempoComboBox->setEnabled(false);
	NovoTemporadioButton->setEnabled(false);

	SelecionarAtributoradioButton->setEnabled(true);

	if(tableComboBox->count()==1)
		tableComboBox->setEnabled(false);
	if(tableComboBox->count()>1)
		tableComboBox->setEnabled(true);
}

void CelulasWindow::NovaAtributoradioButton_clicked()
{
	NomeNovoAtributoTextLabel->setEnabled(true);
	NovoNomeAtributoLineText->setEnabled(true);
	atributoTextLabel->setEnabled(false);
	atributoComboBox->setEnabled(false);
}

void CelulasWindow::SelecionarAtributoradioButton_clicked()
{
	NomeNovoAtributoTextLabel->setEnabled(false);
	NovoNomeAtributoLineText->setEnabled(false);
	atributoTextLabel->setEnabled(true);
	atributoComboBox->setEnabled(true);
}

void CelulasWindow::okPushButton_clicked()
{
	//Recupera a base de dados
	TeDatabase* db = plugin_params_.current_database_ptr_;	

	//Recupera tabela de dados de entrada
	string tabelaData = "";
	string atributoData = "";

	//Recupera Representacao (pontos, linhas, poligonos ou celulas)
	string representacao = GeomComboBox->currentText().latin1();

	//Recupera tipo de operacao
	string operacao = OperationComboBox->currentText().latin1();
	
	//Recupera layer escolhido
	string layerCelula = layerComboBox->currentText().latin1();

	TeLayerMap& layerMap = plugin_params_.current_database_ptr_->layerMap();
	TeLayerMap::iterator layerIt;

	TeLayer* layerCel;

	for(layerIt = layerMap.begin(); layerIt != layerMap.end(); ++layerIt)
	{
		
		if(layerCelula == layerIt->second->name().c_str())
		{
			layerCel = layerIt->second;

			TeAttrTableVector attVec;
			layerCel->getAttrTables(attVec, TeAttrStatic);
		}
	}

	//Recupera o Tema de Dados de Entrada
	string temaDados = themaDataComboBox->currentText().latin1();

	TeThemeMap& themes = plugin_params_.current_database_ptr_->themeMap();
	TeThemeMap::iterator it = themes.begin();
	TeTheme* temaData = 0;

	while(it != themes.end())
	{
		if(it->second->name() == temaDados)
		{
			temaData = (TeTheme*)it->second;

			TeAttrTableVector attVec;
			temaData->getAttTables(attVec, TeAttrStatic);
			break;
		}

		++it;
	}

	if((!NovaTabelaradioButton->isChecked())&&(!SelecionarTabelaradioButton->isChecked()))
	{
		QMessageBox::critical( this, "Error", tr( "Você precisa informar ou criar uma tabela para ser utilizada!" ) );
		return;
	}

	if((!NovaAtributoradioButton->isChecked())&&(!SelecionarAtributoradioButton->isChecked()))
	{
		QMessageBox::critical( this, "Error", tr( "Você precisa informar ou criar um atributo para ser utilizado!" ) );
		return;
	}

	if(SelecionarTemporadioButton->isEnabled())
	{
		if((!NovoTemporadioButton->isChecked())&&(!SelecionarTemporadioButton->isChecked()))
		{
			QMessageBox::critical( this, "Error", tr( "Você precisa informar ou criar um tempo para ser utilizado!" ) );
			return;
		}
	}

	//Se usuario vai criar nova tabela
	if(NovaTabelaradioButton->isChecked())
	{
		string novoNome = NovoNomeTabelaLineText->text();
		
		if (novoNome.empty())
		{
			QMessageBox::critical( this, "Error", tr( "Você precisa informar um nome para a nova tabela!" ) );
			NovoNomeTabelaLineText->setFocus();
			return;
		}
	}

	//Se usuario vai criar novo atributo
	if(NovaAtributoradioButton->isChecked())
	{
		string novoNome = NovoNomeAtributoLineText->text();
		
		if (novoNome.empty())
		{
			QMessageBox::critical( this, "Error", tr( "Você precisa informar um nome para o novo atributo!" ) );
			NovoNomeAtributoLineText->setFocus();
			return;
		}
	}

	//Se usuario vai criar novo tempo
	if((NovoTemporadioButton->isEnabled())&&(NovoTemporadioButton->isChecked()))
	{
		string tempoInicial = tempoInicialLineText->text();
		string tempoFinal = tempoFinalLineText->text();
		
		if (tempoInicial.empty())
		{
			QMessageBox::critical( this, "Error", tr( "Você precisa informar um tempo inicial para o atributo!" ) );
			tempoInicialLineText->setFocus();
			return;
		}

		if (tempoFinal.empty())
		{
			QMessageBox::critical( this, "Error", tr( "Você precisa informar um tempo final para o atributo!" ) );
			tempoFinalLineText->setFocus();
			return;
		}
	}

	if(themaDataComboBox->currentText().latin1()=="")
	{
		QMessageBox::critical( this, "Error", tr( "Selecione um tema para dados de entrada!" ) );
		return;	
	}

	if(representacao != "RASTER")
	{
		if((operacao != "Distância Mínima")&&
		   (operacao != "Contagem")&&
		   (operacao != "Porcentagem Total de Interseção")&&
		   (operacao != "Presença"))
		{
			if(tableDataComboBox->count()==0)
			{
				QMessageBox::critical( this, "Error", tr( "Selecione uma tabela para dados de entrada!" ) );
				return;	
			}
			else
				tabelaData = tableDataComboBox->currentText().latin1();

			if(atributoDataComboBox->count()==0)
			{
				QMessageBox::critical( this, "Error", tr( "Selecione um atributo para dados de entrada!" ) );
				return;	
			}
			else
				atributoData = atributoDataComboBox->currentText().latin1();
		}
	}

	if(OperationComboBox->count()==0)
	{
		QMessageBox::critical( this, "Error", tr( "Selecione uma operação!" ) );
		return;	
	}

	QString msg = tr("Deseja realmente executar operação?");
	int response = QMessageBox::question(this, tr("Question"), msg, tr("Yes"), tr("No"));
	if (response != 0)
	{
		return;	
	}

	//Altera cursor do mouse para ampulheta
	TeWaitCursor wait;
	wait.setWaitCursor();

	string tableCelula;

	//Se usuario vai utilizar tabela existente
	if(SelecionarTabelaradioButton->isChecked())
	{
		//Recupera tabela escolhida
		tableCelula = tableComboBox->currentText().latin1();
	}

	//Se usuario vai criar nova tabela
	if(NovaTabelaradioButton->isChecked())
	{
		//Cria nova tabela
		tableCelula = NovoNomeTabelaLineText->text().latin1();
	}

	string atributoCelula;

	//Se usuario vai utilizar atributo existente
	if(SelecionarAtributoradioButton->isChecked())
	{
		//Recupera tabela escolhida
		atributoCelula = atributoComboBox->currentText().latin1();
	}

	//Se usuario vai criar novo atributo
	if(NovaAtributoradioButton->isChecked())
	{
		//Cria nova tabela
		atributoCelula = NovoNomeAtributoLineText->text().latin1();
	}

	string valorDef = DefaultValueLineText->text().latin1();
	string valorDummy = DummyLineEdit->text().latin1();

	TeAttrDataType type = (TeAttrDataType)TeSTRING;

	if (valorDef.empty())
	{
		valorDef = "0";
	}

	//Se usuario vai criar novo atributo
	if(DummyCheckBox->isChecked())
	{
		if (valorDummy.empty())
		{
			QMessageBox::critical( this, "Error", tr( "Você precisa informar um valor Dummy ou desabilitá-lo!" ) );
			DummyLineEdit->setFocus();
			return;
		}
	}
	else
	{		  
		if(temaData->layer()->hasGeometry(TeRASTER))
			valorDummy = temaData->layer()->raster()->params().dummy_[0];
		else
		{
			TeAttributeList columns;
			TeAttrTableVector atvec;
  			temaData->getAttTables(atvec);
			
			for (unsigned int t = 0; t < atvec.size(); t++) 
			{
				TeTable ttable = atvec[t];
				
				if(ttable.name() == tabelaData)
				{
					columns = ttable.attributeList();		
					//string tableName = ttable.name();
						
					for(unsigned int i = 0; i < columns.size(); i++)
					{
						if(columns[i].rep_.name_.c_str() == atributoData)
						{
							//Recupera o tipo do atributo
							type = columns[i].rep_.type_;

							if(type == (TeAttrDataType)TeSTRING)
								valorDummy = "";
							else	
								valorDummy = "2147483647";
						}
					}
				}	
			}
		}
	}

	//declara tempo (necessario estar aqui para tabelas que sao estaticas)
	TeTimeInterval t;

	//verifica se usuario vai criar novo tempo
	if((NovoTemporadioButton->isEnabled())&&(NovoTemporadioButton->isChecked()))
	{
		//declaracao das variaveis utilizadas no TeTimeInterval
		string tempoInicial, tempoFinal, formatoData, formatoTempo, mascara, amPM, separadorData, separadorTempo;
		//inicializacao das variaveis utilizadas no TeTimeInterval
		tempoInicial = tempoFinal = formatoData = formatoTempo = mascara = amPM = separadorData = separadorTempo = "";

		//Recupera os tempos inicias e finais
		if(!tempoInicialLineText->text().isEmpty())
			tempoInicial = tempoInicialLineText->text().latin1();
		if(!tempoFinalLineText->text().isEmpty())
			tempoFinal = tempoFinalLineText->text().latin1();

		//declaracao das variaveis que seram utilizadas nos indices do formato da data e hora 
		int dataFormatIndex, timeFormatIndex;
		//inicializa variaveis
		dataFormatIndex = timeFormatIndex = -1;

		if(!dateFormatComboBox->currentText().isEmpty())
		{
			formatoData = dateFormatComboBox->currentText().latin1();
			mascara = formatoData;
			//Recupera o indice do formato da data
			dataFormatIndex = dateFormatComboBox->currentItem();
		}
		if(!timeFormatComboBox->currentText().isEmpty())
		{
			formatoTempo = timeFormatComboBox->currentText().latin1();
			mascara += " "+ formatoTempo;
			//Recupera o indice do formato da hora 
			timeFormatIndex = timeFormatComboBox->currentItem();
		}
		
		if(!PMComboBox->currentText().isEmpty())
			amPM = PMComboBox->currentText().latin1();
		
		if(!dateSepLineEdit->text().isEmpty())
			separadorData = dateSepLineEdit->text().latin1();
		
		if(!timeSepLineEdit->text().isEmpty())
			separadorTempo = timeSepLineEdit->text().latin1();

		//declara novo chronon
		TeChronon c;

		//se formato da data < 0, ou usuario esta trabalhando com hora ou nao preencheu nada
		if(dataFormatIndex<0)
		{
			if(timeFormatIndex>0)
			{
				//exibe mensagem de erro
				QString aux = tr( "Não é possível criar um tempo sem informação de data!" );
				aux += "/n" + tr( "Informe uma data padrão!" );
				QMessageBox::critical( this, "Error", aux );
				return;
			}
			
			QMessageBox::critical( this, "Error", tr( "Selecione o formato do tempo de entrada (data e/ou hora)!" ) );
			return;
		}
		//senao, ou seja, usuario informou formato da data
		else 
		{
			//verifica se usuario informou formato da hora. Se sim, chronon sera segundos
			if(timeFormatIndex>0)
				c = TeSECOND;
			//senao, ou seja, usuario nao informou formato de hora, esta trabalhando somente com data
			else
			{
				if(dataFormatIndex==4)
					c = TeMONTH;
				else if(dataFormatIndex==7)
					c = TeYEAR;
				else
					c = TeDAY;
			}
		}
		
		//cria o tempo auxiliar com os dados informados
		TeTimeInterval aux(tempoInicial, tempoFinal,c,mascara,separadorData,separadorTempo,amPM);
		
		//verifica se este tempo e valido
		if (!aux.isValid())
		{
			//Se nao e, exibe mensagem de erro
			QString aux = tr( "O formato do tempo de entrada não está correto!" );
			aux += "/n" + tr( "Consulte o Ajuda sobre os formatos válidos!" );
			QMessageBox::critical( this, "Error", aux );
			return;
		}

		//Atribui o tempo auxiliar ao tempo que sera utilizado
		t=aux;
	}

	if(SelecionarTemporadioButton->isChecked())
	{
		int index = tempoComboBox->currentItem();
		TeTimeInterval taux = times[index];
		t=taux;
	}

	TeGeomRep rep;

	bool result = false;

	if(representacao=="RASTER")
	{
		//****************************RASTER*************************************
		TeComputeAttrStrategy<TeBoxRasterIterator>* operador;

		if(operacao == "Classe Majoritária (número de elementos)")
			operador = new TeMajorityStrategy<TeBoxRasterIterator>(type,valorDef,valorDummy);
		if(operacao == "Desvio Padrão")
			operador = new TeSTDevStrategy<TeBoxRasterIterator>(atof(valorDummy.c_str()),atof(valorDef.c_str()));
		if(operacao == "Soma")
			operador = new TeSumStrategy<TeBoxRasterIterator>(atof(valorDummy.c_str()),atof(valorDef.c_str()));
		if(operacao == "Valor Máximo")
			operador = new TeMaximumStrategy<TeBoxRasterIterator>(atof(valorDummy.c_str()),atof(valorDef.c_str()));
		if(operacao == "Valor Médio")
			operador = new TeAverageStrategy<TeBoxRasterIterator>(atof(valorDummy.c_str()),atof(valorDef.c_str()));
		if(operacao == "Valor Mínimo")
			operador = new TeMinimumStrategy<TeBoxRasterIterator>(atof(valorDummy.c_str()),atof(valorDef.c_str()));

		if(operacao == "Porcentagem de Cada Classe")
			result = TeFillCellCategoryCountPercentageRasterOperation2(db,temaData->layer()->name(),layerCelula,tableCelula,atributoCelula,t);
		else
			result = TeFillCellNonSpatialRasterOperation2(db,temaDados,operador,layerCelula,tableCelula,atributoCelula,t);	
	}
	else
	{
		if(representacao=="PONTOS")		{	rep=TePOINTS;	}
		if(representacao=="LINHAS")		{	rep=TeLINES;	}
		if(representacao=="POLÍGONOS")	{	rep=TePOLYGONS;	}
		if(representacao=="CÉLULAS")	{	rep=TeCELLS;	}

		TeComputeSpatialStrategy* op;

		//****************************OPERADORES PARA VETOR*************************************

		if(operacao == "Contagem")
		{
			switch(rep)
			{
				case TePOLYGONS: op = new TeCountPolygonalObjectsStrategy(temaData); break;
				case TeLINES:    op = new TeCountLineObjectsStrategy(temaData);      break;
				case TePOINTS:   op = new TeCountPointObjectsStrategy(temaData);     break;
				default : { TEAGN_LOG_AND_THROW( "Invalid representation" ); break; };    
			}

			result = TeFillCellSpatialOperation(db,op,layerCelula,tableCelula,atributoCelula,t);
		}

		if(operacao == "Distância Mínima")
		{
			result = TeFillCellDistanceOperation2(temaData,rep,layerCel,tableCelula,atributoCelula,t);
		}

		if(operacao == "Porcentagem Total de Interseção")
		{
			op = new TeTotalAreaPercentageStrategy(temaData);

			result = TeFillCellSpatialOperation(db,op,layerCelula,tableCelula,atributoCelula,t);
		}

		if(operacao == "Presença")
		{			
			TeAttrTableVector& attrs = temaData->attrTables();
			string attrName = attrs[0].uniqueName();
			TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* operador = new TePresenceStrategy<TeSTElementSet::propertyIterator>;

			result = TeFillCellNonSpatialOperation(temaData,rep,attrName,operador,layerCel,tableCelula,atributoCelula,t);
		}

		/*if(operacao == "Categoria Majoritária Por Área")
		{
			switch(rep)
			{
				case TePOLYGONS: op = new TeMajoritySpatialStrategy(temaData); break; 
			}

			result = TeFillCellSpatialOperation(db,op,layerCelula,tableCelula,atributoCelula,t);
		}*/

		//****************************OPERADORES PARA VETOR COM ATRIBUTO*************************************

		if(operacao == "Soma Ponderada Pela Área da Célula")
		{
			string atributoData = atributoDataComboBox->currentText().latin1();
			op = new TeAverageWeighByAreaStrategy(temaData,atributoData,rep);

			result = TeFillCellSpatialOperation(db,op,layerCelula,tableCelula,atributoCelula,t);
		}

		if(operacao == "Soma dos Valores")
		{
			string tabelaData = tableDataComboBox->currentText().latin1();
			string atributoData = atributoDataComboBox->currentText().latin1();

			TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* operador = new TeSumStrategy<TeSTElementSet::propertyIterator>(atof(valorDummy.c_str()),atof(valorDef.c_str()));

			string attrName = tabelaData +"."+ atributoData;
			result = TeFillCellNonSpatialOperation(temaData,rep,attrName,operador,layerCel,tableCelula,atributoCelula,t);
		}

		if(operacao == "Valor Máximo")
		{
			string tabelaData = tableDataComboBox->currentText().latin1();
			string atributoData = atributoDataComboBox->currentText().latin1();

			TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* operador = new TeMaximumStrategy<TeSTElementSet::propertyIterator>(atof(valorDummy.c_str()),atof(valorDef.c_str()));

			string attrName = tabelaData +"."+ atributoData;
			result = TeFillCellNonSpatialOperation(temaData,rep,attrName,operador,layerCel,tableCelula,atributoCelula,t);
		}

		if(operacao == "Valor Médio")
		{
			string tabelaData = tableDataComboBox->currentText().latin1();
			string atributoData = atributoDataComboBox->currentText().latin1();

			TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* operador = new TeAverageStrategy<TeSTElementSet::propertyIterator>(atof(valorDummy.c_str()),atof(valorDef.c_str()));

			string attrName = tabelaData +"."+ atributoData;
			result = TeFillCellNonSpatialOperation(temaData,rep,attrName,operador,layerCel,tableCelula,atributoCelula,t);
		}

		if(operacao == "Valor Mínimo")
		{
			string tabelaData = tableDataComboBox->currentText().latin1();
			string atributoData = atributoDataComboBox->currentText().latin1();

			TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* operador = new TeMinimumStrategy<TeSTElementSet::propertyIterator>(atof(valorDummy.c_str()),atof(valorDef.c_str()));

			string attrName = tabelaData +"."+ atributoData;
			result = TeFillCellNonSpatialOperation(temaData,rep,attrName,operador,layerCel,tableCelula,atributoCelula,t);
		}

		if(operacao == "Soma Ponderada Pela Área do Polígono")
		{
			string tabelaData = tableDataComboBox->currentText().latin1();
			string atributoData = atributoDataComboBox->currentText().latin1();

			switch(rep)
			{
				case TePOLYGONS: op = new TeSumWeighByAreaStrategy(temaData, atributoData,rep); break;
				default : { TEAGN_LOG_AND_THROW( "Invalid representation" ); break; };
			}

			result = TeFillCellSpatialOperation(db,op,layerCelula,tableCelula,atributoCelula,t);
		}

		if(operacao == "Classe Majoritária (número de elementos)")
		{
			string tabelaData = tableDataComboBox->currentText().latin1();
			string atributoData = atributoDataComboBox->currentText().latin1();

			TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* operador = new TeMajorityStrategy<TeSTElementSet::propertyIterator>(type,valorDef,valorDummy);
			
			string attrName = tabelaData +"."+ atributoData;
			result = TeFillCellNonSpatialOperation(temaData,rep,attrName,operador,layerCel,tableCelula,atributoCelula,t);
		}

		/*if(operacao == "Classe Majoritária (área)")
		{

			string tabelaData = tableDataComboBox->currentText().latin1();
			string atributoData = atributoDataComboBox->currentText().latin1();

			switch(rep)
			{
				case TePOLYGONS: op = new TeCategoryMajorityStrategy(temaData, atributoCelula,rep); break;
			}

			result = TeFillCellSpatialOperation(db,op,layerCelula,tableCelula,atributoData,t);
		}*/
		

		if(operacao == "Porcentagem de Cada Classe")
		{
			string tabelaData = tableDataComboBox->currentText().latin1();
			string atributoData = atributoDataComboBox->currentText().latin1();

			TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* operador = new TeCategoryPercentageStrategy<TeSTElementSet::propertyIterator>;
			
			string attrName = tabelaData +"."+ atributoData;
			result = TeFillCellNonSpatialOperation(temaData,rep,attrName,operador,layerCel,tableCelula,atributoCelula,t);
		}

		if(operacao == "Porcentagem da Classe Majoritária")
		{
			string tabelaData = tableDataComboBox->currentText().latin1();
			string atributoData = atributoDataComboBox->currentText().latin1();

			result = TeFillCellCategoryAreaPercentageOperation2(temaData,atributoData,layerCel,tableCelula,t);
		}

		if(operacao == "Desvio Padrão")
		{
			string tabelaData = tableDataComboBox->currentText().latin1();
			string atributoData = atributoDataComboBox->currentText().latin1();

			TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* operador = new TeSTDevStrategy<TeSTElementSet::propertyIterator>(atof(valorDummy.c_str()),atof(valorDef.c_str()));
			
			string attrName = tabelaData +"."+ atributoData;
			result = TeFillCellNonSpatialOperation(temaData,rep,attrName,operador,layerCel,tableCelula,atributoCelula,t);
		}
	}

	wait.resetWaitCursor();

	QString mensagem;

	if(result)
	{
		mensagem = tr("Os dados foram gerados com sucesso!");
		if(NovoTemporadioButton->isChecked())
		{
			TeThemeMap& themes = db->themeMap();

			TeThemeMap::iterator it = themes.begin();
			//TeTheme* theme = 0;

			while(it != themes.end())
			{

				TeTheme *theme = (TeTheme*)it->second;
				
				if (theme->layer()->id() == layerCel->id())
				//if(it->second->layerId() == layerCel->id())
				{
					theme->createCollectionAuxTable();
					//themeIt->second->createCollectionAuxTable();
					theme->populateCollectionAux();
					//themeIt->second->populateCollectionAux();
					break;
				}
				++it;
			}				
		}
		close();
	}
	else
	{
		mensagem = tr("Os dados não puderam ser gerados!");
	}
	QMessageBox::information(this, tr("Aviso"), mensagem);	
}

void CelulasWindow::cancelButton_clicked()
{
	hide();
}

void CelulasWindow::tableComboBox_activated( const QString & tableName )
{
	//Limpa combo de atributos
	atributoComboBox->clear();
	
	//Se nome da tabela for vazio retorna
	if (tableName.isEmpty())
		return;

	//Cria Tabela do tipo TeTable
	TeTable table(tableName.latin1());

	//Carrega informacoes para a tabela
	plugin_params_.current_database_ptr_->loadTable(table.name(), table);

	//Recupera atributos da tabela
	TeAttributeList attList_;	
	attList_.clear();
	attList_ = table.attributeList();
	TeAttributeList::iterator it = attList_.begin();
	while (it != attList_.end())
	{
		//Finalmente atribui atributos ao combo de atributos
		atributoComboBox->insertItem(QString((*it).rep_.name_.c_str()));
		++it;
	}
	
	//Se a tabela escolhida for do tipo dinamica(temporal), recupera tempos ja existentes
	//na tabela para atribuir ao combo
	if(table.tableType()==TeFixedGeomDynAttr)
	{
		NovoTemporadioButton->setEnabled(true);
		SelecionarTemporadioButton->setEnabled(true);

		//Se a tabela e temporal, o padrao e utilizar um atributo ja existente
		SelecionarAtributoradioButton->setChecked(true);
		SelecionarAtributoradioButton_clicked();

		//Se a tabela e temporal, o padrao e criar novo tempo
		NovoTemporadioButton->setChecked(true);
		NovoTemporadioButton_clicked();
	
		//descobrir quais são os campos que tem a data
		string t1 = table.attInitialTime();
		string t2 = table.attFinalTime();

		//montar a sql
		string sql = " SELECT DISTINCT "+ t1 +", "+ t2;
		sql += " FROM "+ table.name();

		//inicia portal
		TeDatabasePortal* portal = plugin_params_.current_database_ptr_->getPortal();

		//verifica se sql esta correta
		if(!portal->query(sql))
		{
			//error  m essage: erro ao carregar os tempos do banco
		}

		//enquanto existir registros, adiciona tempos
		while(portal->fetchRow())
		{		
			TeTime t1, t2;
			t1 = portal->getDate(0);
			t2 = portal->getDate(1);

			string s1 = t1.getDateTime();
			string s2 = t2.getDateTime(); 

			string aux = s1 + "    " + s2;
			
			//adiciona tempos no combo
			tempoComboBox->insertItem(QString(aux.c_str()));		
			
			TeTimeInterval tInterval (t1, t2);
			//Adiciona tempos no vetor
			times.push_back(tInterval);
		}

		//limpa portal
		portal->freeResult();
		//exclui portal
		delete portal;
	}
	//Se a tabela escolhida nao for do tipo dinamica(temporal), desabilita campos para
	//trabalhar com tempos
	else
	{
		tempoInicialLineText->setEnabled(false);
		tempoFinalLineText->setEnabled(false);
		dateFormatComboBox->setEnabled(false);
		timeFormatComboBox->setEnabled(false);
		PMComboBox->setEnabled(false);
		dateSepLineEdit->setEnabled(false);
		timeSepLineEdit->setEnabled(false);	
		tempoComboBox->setEnabled(false);
		NovoTemporadioButton->setEnabled(false);
		SelecionarTemporadioButton->setEnabled(false);

		NovaAtributoradioButton->setChecked(true);
		NovaAtributoradioButton_clicked();
	}
}


void CelulasWindow::themaDataComboBox_activated( const QString & )
{
	GeomComboBox->clear();

	//Recupera o Tema de Dados de Entrada
	string temaDados = themaDataComboBox->currentText().latin1();

	TeThemeMap& themes = plugin_params_.current_database_ptr_->themeMap();
	TeThemeMap::iterator it = themes.begin();
	TeTheme* temaData = 0;

	while(it != themes.end())
	{
		if(it->second->name().c_str() == temaDados)
		{
			temaData = (TeTheme*)it->second;

			TeAttrTableVector attVec;
			temaData->getAttTables(attVec, TeAttrStatic);
		}
		++it;
	}

	if(temaData->layer()->hasGeometry(TeRASTER))
	{
		GeomComboBox->insertItem(QString("RASTER"));
		GeomComboBox->setEnabled(false);
	}
	else
	{
		if(temaData->layer()->hasGeometry(TePOLYGONS))
		{
			GeomComboBox->insertItem(QString("POLÍGONOS"));
		}

		if(temaData->layer()->hasGeometry(TeLINES))
		{
			GeomComboBox->insertItem(QString("LINHAS"));
		}

		if(temaData->layer()->hasGeometry(TePOINTS))
		{
			GeomComboBox->insertItem(QString("PONTOS"));
		}

		if(temaData->layer()->hasGeometry(TeCELLS))
		{
			GeomComboBox->insertItem(QString("CÉLULAS"));
		}

		if (GeomComboBox->count() == 1)
			GeomComboBox->setEnabled(false);

		if (GeomComboBox->count() > 1)
			GeomComboBox->setEnabled(true);
	}

	GeomComboBox_activated();
	OperationComboBox_activated();
}

void CelulasWindow::layerComboBox_activated( const QString & layerName )
{
	if (layerName.isEmpty())
	{
		QMessageBox::information(this, tr("Information"),tr("Você precisa de um Plano de Informação do tipo Células!"));
		return;
	}

	//Recupera PI selecionado no combo de PI
	TeLayerMap& layerMap = plugin_params_.current_database_ptr_->layerMap();
	TeLayerMap::iterator it;
	TeLayer *layer;
	for (it = layerMap.begin(); it != layerMap.end(); ++it)
	{
		layer = it->second;
		if (layer->name() == layerName.latin1())
			break;
	}
	TeLayer * themeLayer_ = layer;


	//Verifica se o PI selecionado e do tipo de celulas
	//Se nao for, nao faz nada
	if (!themeLayer_->geomRep() & TeCELLS)
	{
		QMessageBox::information(this, tr("Information"),tr("Você precisa de um Plano de Informação do tipo Células!"));
		return;
	}

	//Recupera as tabelas referentes ao PI selecionado
	TeAttrTableVector layerTablesVector;
	//Primeiro, verifica se e possivel recuperar as tabelas
	if(themeLayer_->getAttrTables(layerTablesVector) == false)
	{
		QMessageBox::critical(this, tr("Error"),
			tr("Fail to get the infolayer tables!"));
		hide();
	}

	//Se for possivel, limpa o que ja existe no combo de tabelas
	tableComboBox->clear();
	for(unsigned int i = 0; i < layerTablesVector.size(); ++i)
	{
		//Finalmente, atribui as tabelas ao combo
		if(layerTablesVector[i].tableType() != TeAttrMedia)
			tableComboBox->insertItem(QString(layerTablesVector[i].name().c_str()));
	}

	SelecionarTabelaradioButton->setChecked(true);
	SelecionarTabelaradioButton_clicked();

	NovaAtributoradioButton->setChecked(true);
	NovaAtributoradioButton_clicked();

	if(tableComboBox->count()==1)
		tableComboBox->setEnabled(false);
	if(tableComboBox->count()>1)
		tableComboBox->setEnabled(true);

	tableComboBox_activated(tableComboBox->currentText());

}

void CelulasWindow::GeomComboBox_activated()
{
	string geometria = GeomComboBox->currentText().latin1();

	//Limpa combo de operacoes
	OperationComboBox->clear();

	if(geometria=="RASTER")
	{
		OperationComboBox->insertItem(QString("Classe Majoritária (número de elementos)"));
		OperationComboBox->insertItem(QString("Desvio Padrão"));		
		OperationComboBox->insertItem(QString("Porcentagem de Cada Classe"));
		OperationComboBox->insertItem(QString("Soma"));
		OperationComboBox->insertItem(QString("Valor Máximo"));
		OperationComboBox->insertItem(QString("Valor Médio"));
		OperationComboBox->insertItem(QString("Valor Mínimo"));
	}

	if(geometria=="PONTOS")
	{		
		OperationComboBox->insertItem(QString("Classe Majoritária (número de elementos)"));
		OperationComboBox->insertItem(QString("Contagem"));
		OperationComboBox->insertItem(QString("Desvio Padrão"));
		OperationComboBox->insertItem(QString("Distância Mínima"));
		OperationComboBox->insertItem(QString("Presença"));		
		OperationComboBox->insertItem(QString("Soma dos Valores"));
		OperationComboBox->insertItem(QString("Valor Máximo"));
		OperationComboBox->insertItem(QString("Valor Médio"));
		OperationComboBox->insertItem(QString("Valor Mínimo"));		
	}
	if(geometria=="LINHAS")
	{
		OperationComboBox->insertItem(QString("Classe Majoritária (número de elementos)"));
		OperationComboBox->insertItem(QString("Contagem"));
		OperationComboBox->insertItem(QString("Desvio Padrão"));		
		OperationComboBox->insertItem(QString("Distância Mínima"));
		OperationComboBox->insertItem(QString("Presença"));		
		OperationComboBox->insertItem(QString("Soma dos Valores"));
		OperationComboBox->insertItem(QString("Valor Máximo"));
		OperationComboBox->insertItem(QString("Valor Médio"));
		OperationComboBox->insertItem(QString("Valor Mínimo"));
	}
	if(geometria=="POLÍGONOS")
	{
		//OperationComboBox->insertItem(QString("Classe Majoritária (área)"));
		OperationComboBox->insertItem(QString("Classe Majoritária (número de elementos)"));
		OperationComboBox->insertItem(QString("Contagem"));
		OperationComboBox->insertItem(QString("Desvio Padrão"));		
		OperationComboBox->insertItem(QString("Distância Mínima"));	
		OperationComboBox->insertItem(QString("Presença"));		
		OperationComboBox->insertItem(QString("Porcentagem de Cada Classe"));
		OperationComboBox->insertItem(QString("Porcentagem Total de Interseção"));
		OperationComboBox->insertItem(QString("Porcentagem da Classe Majoritária"));
		OperationComboBox->insertItem(QString("Soma dos Valores"));
		OperationComboBox->insertItem(QString("Soma Ponderada Pela Área da Célula"));
		OperationComboBox->insertItem(QString("Soma Ponderada Pela Área do Polígono"));
		OperationComboBox->insertItem(QString("Valor Máximo"));
		OperationComboBox->insertItem(QString("Valor Médio"));
		OperationComboBox->insertItem(QString("Valor Mínimo"));
	}

	if(geometria=="CÉLULAS")
	{
		OperationComboBox->insertItem(QString("Classe Majoritária (número de elementos)"));
		OperationComboBox->insertItem(QString("Porcentagem de Cada Classe"));
		OperationComboBox->insertItem(QString("Valor Máximo"));
		OperationComboBox->insertItem(QString("Valor Médio"));
		OperationComboBox->insertItem(QString("Valor Mínimo"));
	}
}

void CelulasWindow::NovoTemporadioButton_clicked()
{
	tempoInicialLineText->setEnabled(true);
	tempoFinalLineText->setEnabled(true);
	dateFormatComboBox->setEnabled(true);
	timeFormatComboBox->setEnabled(true);
	PMComboBox->setEnabled(true);
	dateSepLineEdit->setEnabled(true);
	timeSepLineEdit->setEnabled(true);
	tempoComboBox->setEnabled(false);
}


void CelulasWindow::SelecionarTemporadioButton_clicked()
{
	tempoComboBox->setEnabled(true);
	tempoInicialLineText->setEnabled(false);
	tempoFinalLineText->setEnabled(false);
	dateFormatComboBox->setEnabled(false);
	timeFormatComboBox->setEnabled(false);
	PMComboBox->setEnabled(false);
	dateSepLineEdit->setEnabled(false);
	timeSepLineEdit->setEnabled(false);	
}

void CelulasWindow::helpPushButton_clicked()
{
	Help* help_;

	help_ = new Help(this, "help", false);
	help_->init("fillCellPlugin.html");
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


void CelulasWindow::aboutPushButton_clicked()
{
	QMessageBox::about(this, tr("About TerraView Plugin Fill Cell"),
		tr("<h2>TerraView Plugin Fill Cell (28/02/2008)</h2>" )
		+ QString( "<p>" ) + tr( "Product based on the TerraLib" ) + QString( " " ) 
		+ QString( " 3.2.0 and TerraView 3.2.0<BR>" )
		+ tr( "Database Model version 3.2.0<BR>" )
		+ tr( "Build using QT" ) + QString( " " ) + QString( QT_VERSION_STR ) 
		+ QString( "<BR>" )
		+ tr( "Spatial Statistics Algorithms - TerraStat<BR>"
		   "<p>Copyright &copy; 2001-2007 INPE, Tecgraf PUC-Rio, and FUNCATE<BR>"
		   "Partner: LESTE/UFMG"
		   "<p>TerraView Page: http://www.dpi.inpe.br/terraview<BR>"
		   "TerraLib Page: http://www.terralib.org" ) );
}

void CelulasWindow::tableDataComboBox_activated()
{
	string operacao = OperationComboBox->currentText().latin1();
	string tabelaData = tableDataComboBox->currentText().latin1();

	//Limpa combo de atributos
	atributoDataComboBox->clear();

	//Cria Tabela do tipo TeTable
	TeTable table(tabelaData);

	//Carrega informacoes para a tabela
	plugin_params_.current_database_ptr_->loadTable(table.name(), table);

	TeAttributeList attList_;	
	attList_.clear();
	attList_ = table.attributeList();
	TeAttributeList::iterator it = attList_.begin();
	int type;
	while (it != attList_.end())
	{
		//Recupera o tipo do atributo
		type = (*it).rep_.type_;
		//Finalmente atribui atributos ao combo de atributos
		if((operacao == "Classe Majoritária") || (operacao == "Porcentagem de Cada Classe") || (operacao == "Classe Majoritária") || (operacao == "Porcentagem da Classe Majoritária"))
		{
			if(type == TeSTRING || type == TeINT)
			{
				atributoDataComboBox->insertItem(QString((*it).rep_.name_.c_str()));
			}
		}
		else
		{
			if(type == TeREAL || type == TeINT)
			{
				atributoDataComboBox->insertItem(QString((*it).rep_.name_.c_str()));
			}
		}
		++it;
	}
}

void CelulasWindow::OperationComboBox_activated()
{
	//Recupera tipo de operacao
	string operacao = OperationComboBox->currentText().latin1();

	//Recupera o Tema de Dados de Entrada
	string temaDados = themaDataComboBox->currentText().latin1();

	string geometria = GeomComboBox->currentText().latin1();

	//Limpa combo de tabelas
	tableDataComboBox->clear();

	TeThemeMap& themeMap = plugin_params_.current_database_ptr_->themeMap();
	TeThemeMap::iterator themeIt;

	TeTheme* temaData;

	for(themeIt = themeMap.begin(); themeIt != themeMap.end(); ++themeIt)
	{		
		if(temaDados == themeIt->second->name().c_str())
		{
			temaData = (TeTheme*)themeIt->second;

			TeAttrTableVector attVec;
			temaData->getAttTables(attVec, TeAttrStatic);
		}
	}
	if(geometria == "RASTER")
	{
		tableDataComboBox->setEnabled(false);			
		atributoDataComboBox->setEnabled(false);
		tableDataComboBox->clear();
		atributoDataComboBox->clear();
		DefaultValueLineText->clear();

		bool enable = (operacao != "Porcentagem de Cada Classe");

		DummyCheckBox->setEnabled(enable);
		DummyCheckBox->setChecked(enable);
		DummyLineEdit->setEnabled(enable);
		DefaultValueTextLabel->setEnabled(enable);
		DefaultValueLineText->setEnabled(enable);

		if(!enable)
			DummyLineEdit->clear();
		else
		{
			//Recupera valor Dummy do raster
			QString valorDummy;
			valorDummy.setNum( temaData->layer()->raster()->params().dummy_[0] );
			DummyLineEdit->setText(valorDummy);
		}
	}
	else
	{
		//Recupera as tabelas referentes ao Tema selecionado
		TeLayer * themeLayer_ = temaData->layer();
		TeAttrTableVector layerTablesVector;
		themeLayer_->getAttrTables(layerTablesVector);
		for(unsigned int i = 0; i < layerTablesVector.size(); ++i)
		{
			//Finalmente, atribui as tabelas ao combo
			tableDataComboBox->insertItem(QString(layerTablesVector[i].name().c_str()));
		}

		tableDataComboBox_activated();		
		
		if((operacao == "Distância Mínima")||
		   (operacao == "Contagem")||
		   (operacao == "Porcentagem Total de Interseção")||
		   (operacao == "Presença"))
		{
			tableDataComboBox->clear();
			tableDataComboBox->setEnabled(false);
			atributoDataComboBox->clear();
			atributoDataComboBox->setEnabled(false);

			DummyCheckBox->setEnabled(false);
			DummyCheckBox->setChecked(false);
			DummyLineEdit->setEnabled(false);
			DummyLineEdit->clear();
			DefaultValueTextLabel->setEnabled(false);
			DefaultValueLineText->setEnabled(false);
			DefaultValueLineText->clear();
		}
		else
		{
			if(tableDataComboBox->count()==1) tableDataComboBox->setEnabled(false);
			if(tableDataComboBox->count()>1)  tableDataComboBox->setEnabled(true);

			atributoDataComboBox->setEnabled(true);

			if((operacao == "Porcentagem de Cada Classe")||
			   (operacao == "Soma Ponderada Pela Área da Célula")||
			   (operacao == "Soma Ponderada Pela Área do Polígono"))
			{
				DummyCheckBox->setEnabled(false);
				DummyCheckBox->setChecked(false);
				DummyLineEdit->setEnabled(false);
				DummyLineEdit->clear();
				DefaultValueTextLabel->setEnabled(false);
				DefaultValueLineText->setEnabled(false);
				DefaultValueLineText->clear();
			}
			else
			{
				DummyCheckBox->setEnabled(true);
				DummyCheckBox->setChecked(false);
				DummyLineEdit->setEnabled(false);
				DummyLineEdit->clear();
				DefaultValueTextLabel->setEnabled(true);
				DefaultValueLineText->setEnabled(true);
				DefaultValueLineText->clear();
			}
		}
	}
}

void CelulasWindow::DummyCheckBox_clicked()
{
	DummyLineEdit->setEnabled(DummyCheckBox->isChecked());
}
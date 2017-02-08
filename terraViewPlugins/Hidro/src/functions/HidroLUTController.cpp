#include <HidroLUTController.h>

#include <TeDatabase.h>
#include <TeGroupingAlgorithms.h>

#include <TeQtThemeItem.h>
#include <TeQtLegendItem.h>


HidroLUTController::HidroLUTController(TeLayer* layer)
{
	_layer = layer;
}

HidroLUTController::~HidroLUTController()
{

}

std::vector<std::string> HidroLUTController::listLUTClasses() const
{
	std::vector<std::string> classes;

	std::string attrTableName = _layer->raster()->params().lutName() + "_Attr";

	TeDatabasePortal* portal = _layer->database()->getPortal();

	for(unsigned int i = 0; i < _layer->raster()->params().lutr_.size(); ++i)
	{
		std::string sql = "SELECT class FROM " + attrTableName + " WHERE index_id = " + Te2String(i);

		if(!portal->query(sql))
		{
			delete portal;
			return classes;
		}

		while(portal->fetchRow())
		{
			std::string lutClass = portal->getData(0);

			classes.push_back(lutClass);
		}

		portal->freeResult();
	}

	delete portal;

	return classes;
}

void HidroLUTController::getClassColor(const std::string& className, int& r, int& g, int& b)
{
	int index = getClassIndex(className);

	r = _layer->raster()->params().lutr_[index];
	g = _layer->raster()->params().lutg_[index];
	b = _layer->raster()->params().lutb_[index];
}

int HidroLUTController::getClassIndex(const std::string& className)
{
	std::string attrTableName = _layer->raster()->params().lutName() + "_Attr";

	TeDatabasePortal* portal = _layer->database()->getPortal();

	std::string sql = "SELECT index_id FROM " + attrTableName + " WHERE class = '" + className + "'";

	if(!portal->query(sql))
	{
		delete portal;
		return -1;
	}

	int index;

	while(portal->fetchRow())
	{
		index = portal->getInt(0);
	}

	portal->freeResult();
	
	delete portal;

	return index;
}

int HidroLUTController::getClassIndex(const int& r, const int& g, const int& b)
{
	int value = -1;

	for(unsigned int i = 0; i < _layer->raster()->params().lutr_.size(); ++i)
	{
		if(	
			_layer->raster()->params().lutr_[i] == r &&
			_layer->raster()->params().lutg_[i] == g &&
			_layer->raster()->params().lutb_[i] == b
		  )
		{
			value = i;
			break;
		}
	}

	return value;
}

std::string HidroLUTController::getClassName(const int& index)
{
	std::string attrTableName = _layer->raster()->params().lutName() + "_Attr";

	TeDatabasePortal* portal = _layer->database()->getPortal();

	std::string sql = "SELECT class  FROM " + attrTableName + " WHERE index_id = " + Te2String(index);

	if(!portal->query(sql))
	{
		delete portal;
		return "";
	}

	std::string className;

	while(portal->fetchRow())
	{
		className = portal->getData(0);
	}

	portal->freeResult();
	
	delete portal;

	return className;
}

bool HidroLUTController::addNewClass(const std::string& className, const int& r, const int& g, const int& b, const bool& setAsDummy)
{
	_errorMessage = "";

	int index = _layer->raster()->params().lutr_.size();

	if(index >= 256)
	{
		_errorMessage = "LUT Table is full.";
		return false;
	}

	_layer->raster()->params().lutr_.push_back(r);
	_layer->raster()->params().lutg_.push_back(g);
	_layer->raster()->params().lutb_.push_back(b);
	
	_layer->raster()->decoder()->params().lutr_.push_back(r);
	_layer->raster()->decoder()->params().lutg_.push_back(g);
	_layer->raster()->decoder()->params().lutb_.push_back(b);
	
	std::string sqlLUT = "INSERT INTO " + _layer->raster()->params().lutName() + " (index_id, r_val, g_val, b_val) VALUES (";
				sqlLUT+= Te2String(index) + ", ";
				sqlLUT+= Te2String(r) + ", ";
				sqlLUT+= Te2String(g) + ", ";
				sqlLUT+= Te2String(b) + ")";

	if(!_layer->database()->execute(sqlLUT))
	{
		_errorMessage = "Error insert class into LUT Table.";
		return false;
	}

	std::string sqlATTR = "INSERT INTO " + _layer->raster()->params().lutName() + "_Attr" + " (index_id, class) VALUES (";
				sqlATTR+= Te2String(index) + ", '";
				sqlATTR+= className + "')";

	if(!_layer->database()->execute(sqlATTR))
	{
		_errorMessage = "Error insert class into LUT Attr Table.";
		return false;
	}

	if(setAsDummy)
	{
		_layer->raster()->params().setDummy(index);
		_layer->raster()->decoder()->params().setDummy(index);

		std::string rasterTableMetadata = "rasterLayer" + Te2String(_layer->id()) + "_metadata";

		std::string updateLUT = "UPDATE " + rasterTableMetadata + " SET dummy = " + Te2String(index);

		if(!_layer->database()->execute(updateLUT))
		{
			_errorMessage = "Error updating dummy value in LUT Table.";
			return false;
		}
	}

	return true;
}

bool HidroLUTController::changeClass(const int& index, const std::string& className, const int& r, const int& g, const int& b, const bool& setAsDummy)
{
	_errorMessage = "";

	_layer->raster()->params().lutr_[index] = r;
	_layer->raster()->params().lutg_[index] = g;
	_layer->raster()->params().lutb_[index] = b;
	
	_layer->raster()->decoder()->params().lutr_[index] = r;
	_layer->raster()->decoder()->params().lutg_[index] = g;
	_layer->raster()->decoder()->params().lutb_[index] = b;
	
	std::string sqlLUT  = "UPDATE " + _layer->raster()->params().lutName() + " SET ";
				sqlLUT += "r_val = " + Te2String(r);
				sqlLUT += ", g_val = " + Te2String(g);
				sqlLUT += ", b_val = " + Te2String(b);
				sqlLUT += " where index_id = " + Te2String(index);
				

	if(!_layer->database()->execute(sqlLUT))
	{
		_errorMessage = "Error updating class entry in LUT Table.";
		return false;
	}

	bool update = false;

	std::string selectSQL = "SELECT * from " + _layer->raster()->params().lutName() + "_Attr" + " where index_id = " + Te2String(index);

	TeDatabasePortal* portal = _layer->database()->getPortal();

	if(!portal->query(selectSQL))
	{
		delete portal;
		_errorMessage = "Error getting class entry information.";
		return false;
	}

	if(portal->fetchRow())
	{
		update = true;
	}
	
	delete portal;
	
	if(update)
	{
		std::string sqlUpdate = "UPDATE " + _layer->raster()->params().lutName() +  "_Attr" + " SET class = '" + className + "' where index_id = " + Te2String(index);

		if(!_layer->database()->execute(sqlUpdate))
		{
			_errorMessage = "Error updating class into LUT Attr Table.";
			return false;
		}
	}
	else
	{
		std::string sqlATTR = "INSERT INTO " + _layer->raster()->params().lutName() + "_Attr" + " (index_id, class) VALUES (";
					sqlATTR+= Te2String(index) + ", '";
					sqlATTR+= className + "')";

		if(!_layer->database()->execute(sqlATTR))
		{
			_errorMessage = "Error insert class into LUT Attr Table.";
			return false;
		}
	}

	if(setAsDummy)
	{
		_layer->raster()->params().setDummy(index);
		_layer->raster()->decoder()->params().setDummy(index);

		std::string rasterTableMetadata = "rasterLayer" + Te2String(_layer->id()) + "_metadata";

		std::string updateLUT = "UPDATE " + rasterTableMetadata + " SET dummy = " + Te2String(index);

		if(!_layer->database()->execute(updateLUT))
		{
			_errorMessage = "Error updating dummy value in LUT Table.";
			return false;
		}
	}

	return true;
}

void HidroLUTController::getClassList(std::vector<HidroLutInfo>& classVec)
{
	std::string lutTableName = _layer->raster()->params().lutName();
	std::string attrTableName = _layer->raster()->params().lutName() + "_Attr";

	bool hasClassInfo = false;

	if(_layer->database()->tableExist(attrTableName))
	{
		hasClassInfo = true;
	}

	std::string sql = "";
	
	if(hasClassInfo)
	{
		sql += "SELECT A.*, B.class FROM " + lutTableName;
		sql +=" A LEFT JOIN " + attrTableName + " B ON A.index_id = B.index_id";
		sql += " ORDER BY index_id";
	}
	else
	{
		sql += "SELECT * FROM " + lutTableName;
		sql += " ORDER BY index_id";
	}

	TeDatabasePortal* portal = _layer->database()->getPortal();

	if(!portal->query(sql))
	{
		delete portal;
		return;
	}

	while(portal->fetchRow())
	{
		HidroLutInfo ci;
		ci._index = portal->getData(0);
		ci._r = portal->getData(1);
		ci._g = portal->getData(2);
		ci._b = portal->getData(3);

		if(hasClassInfo)
		{
			ci._className = portal->getData(4);
		}
		else
		{
			ci._className = "";
		}

		classVec.push_back(ci);
	}

	portal->freeResult();
	
	delete portal;
}

bool HidroLUTController::updateThemeLegend(TeTheme* theme, TeQtThemeItem* themeItem)
{
	std::vector<HidroLutInfo> classVec;
	
	getClassList(classVec);

	if(classVec.empty())
	{
		return false;
	}

	//std::vector<std::string> classes;

	//for(unsigned int i = 0; i < classVec.size(); ++i)
	//{
	//	classes.push_back(classVec[i]._className);
	//}

	//vector<TeSlice> slices;
	//TeGroupByUniqueValue(classes, TeSTRING, slices, 1);

	if(theme)
	{
		theme->cleanLegend();

		theme->layer()->database()->deleteLegend(theme->id());

		for(unsigned int i=0; i<classVec.size(); ++i)
		{
			TeColor color(atoi(classVec[i]._r.c_str()), atoi(classVec[i]._g.c_str()), atoi(classVec[i]._b.c_str()));

			TeVisual* visual = new TeVisual(TePOLYGONS);            

			visual->color(color);
			visual->transparency(theme->defaultLegend().visual(TePOLYGONS)->transparency());
			visual->contourStyle(theme->defaultLegend().visual(TePOLYGONS)->contourStyle());
			visual->contourWidth(theme->defaultLegend().visual(TePOLYGONS)->contourWidth());
			visual->contourColor(TeColor(0, 0, 0));	

			TeLegendEntry legend;
			legend.label(classVec[i]._className);
			legend.setVisual(visual, TePOLYGONS);
			legend.group(i);
			legend.theme(theme->id());
			theme->legend().push_back(legend);
		}

		theme->layer()->database()->updateTheme(theme);

		themeItem->removeLegends();

		TeLegendEntryVector& legendVector = theme->legend();
		if (legendVector.size() > 0)
		{
			std::string text = "LUT Class";
			
			new TeQtLegendTitleItem(themeItem, text.c_str());				

			for (unsigned j = 0; j < legendVector.size(); ++j)
			{
				TeLegendEntry& legendEntry = legendVector[j];
				std::string txt = legendEntry.label();
				new TeQtLegendItem(themeItem, txt.c_str(), &legendEntry);
			}
		}
	}

	return true;
}

bool HidroLUTController::createAttrLUTTable()
{
	_errorMessage = "";

	if(hasAttrLUTTable())
	{
		return true;
	}

	TeAttributeList list;

	{
		TeAttribute attr;

		attr.rep_.name_ = "index_id";
		attr.rep_.type_ = TeINT;
		attr.rep_.isPrimaryKey_ = false;
		attr.rep_.isAutoNumber_ = false;
		
		list.push_back(attr);
	}

	{
		TeAttribute attr;

		attr.rep_.name_ = "class";
		attr.rep_.type_ = TeSTRING;
		attr.rep_.isPrimaryKey_ = false;
		attr.rep_.isAutoNumber_ = false;
		attr.rep_.numChar_ = 255;
		
		list.push_back(attr);
	}

	std::string attrTableName = _layer->raster()->params().lutName() + "_Attr";

	if(!_layer->database()->createTable(attrTableName, list))
	{
		_errorMessage = "Error adding LUT Attr Table.";
		return false;
	}

	return true;
}

bool HidroLUTController::hasAttrLUTTable()
{
	std::string attrTableName = _layer->raster()->params().lutName() + "_Attr";

	if(_layer->database()->tableExist(attrTableName))
	{
		return true;
	}

	return false;
}

void HidroLUTController::exportLutInfoToFile(const std::string& fileName)
{
	if(fileName.empty())
	{
		return;
	}

	std::vector<HidroLutInfo> classVec;
	
	getClassList(classVec);

	std::vector<HidroLutInfo>::iterator it = classVec.begin();

	TeWriteToFile(fileName, "Lut Information \n", "w");
	TeWriteToFile(fileName, "Layer: " + _layer->name() +" \n", "w");

	TeWriteToFile(fileName, "Index, Label, cR, cG, cB\n", "a+");

	while(it != classVec.end())
	{
		std::string classInfo = "";

		classInfo += it->_index + ", ";
		classInfo += it->_className + ", ";
		classInfo += it->_r + ", ";
		classInfo += it->_g + ", ";
		classInfo += it->_b;

		classInfo+= "\n";

		TeWriteToFile(fileName, classInfo, "a+");

		++it;
	}
}

std::string	HidroLUTController::getErrorMessage() const
{
	return _errorMessage;
}

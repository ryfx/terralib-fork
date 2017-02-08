#include <HidroGroupLUTController.h>

#include <HidroLUTController.h>

#include <TeDatabase.h>


HidroGroupLUTController::HidroGroupLUTController(TeLayer* layer)
{
	if(layer && layer->hasGeometry(TeRASTER))
	{
		if(layer->raster()->params().photometric_[0] == TeRasterParams::TePallete)
		{
			_layer = layer;

			updateLutInfo();
		}
	}
}

HidroGroupLUTController::~HidroGroupLUTController()
{
	_lutInfo.clear();

	_groupInfo.clear();
}

std::map<std::string, HidroGroupLutInfo>& HidroGroupLUTController::getLutInfo()
{
	return _lutInfo;
}

bool HidroGroupLUTController::lutEntryHasGroup(const std::string& lutIdx, std::string& groupName)
{
	if(lutIdx.empty())
	{
		return false;
	}

	std::map<std::string, HidroGroupLutInfo>::iterator it = _lutInfo.find(lutIdx);

	if(it != _lutInfo.end())
	{
		if(it->second._hasGrouping)
		{
			groupName = it->second._groupingName;
		}

		return it->second._hasGrouping;
	}

	return false;
}

void HidroGroupLUTController::setLutEntryGroup(const std::string& lutIdx, const std::string& groupName)
{
	if(lutIdx.empty() || groupName.empty())
	{
		return;
	}

	std::map<std::string, HidroGroupLutInfo>::iterator it = _lutInfo.find(lutIdx);

	if(it != _lutInfo.end())
	{
		it->second._hasGrouping = true;
		it->second._groupingName = groupName;
	}
}

void HidroGroupLUTController::removeLutEntryGroup(const std::string& lutIdx)
{
	if(lutIdx.empty())
	{
		return;
	}

	std::map<std::string, HidroGroupLutInfo>::iterator it = _lutInfo.find(lutIdx);

	if(it != _lutInfo.end())
	{
		it->second._hasGrouping = false;
		it->second._groupingName = "";
	}
}

bool HidroGroupLUTController::addGroup(const std::string& groupName, const int& r, const int& g, const int& b)
{
	if(groupName.empty())
	{
		return false;
	}

	for(unsigned int i = 0; i < _groupInfo.size(); ++i)
	{
		if(_groupInfo[i]._className == groupName)
		{
			return false;
		}
	}

	HidroGroupInfo group;

	group._className = groupName;
	group._r = Te2String(r);
	group._g = Te2String(g);
	group._b = Te2String(b);
	
	_groupInfo.push_back(group);
	
	return true;
}

bool HidroGroupLUTController::changeGroupColor(const std::string& groupName, const int& r, const int& g, const int& b)
{
	if(groupName.empty())
	{
		return false;
	}

	for(unsigned int i = 0; i < _groupInfo.size(); ++i)
	{
		if(_groupInfo[i]._className == groupName)
		{
			_groupInfo[i]._r = Te2String(r);
			_groupInfo[i]._g = Te2String(g);
			_groupInfo[i]._b = Te2String(b);

			return true;
		}
	}

	return false;
}

void HidroGroupLUTController::removeGroup(const std::string& groupName)
{
	if(groupName.empty())
	{
		return;
	}

	std::map<std::string, HidroGroupLutInfo>::iterator it = _lutInfo.begin();

	while(it != _lutInfo.end())
	{
		if(it->second._hasGrouping && it->second._groupingName == groupName)
		{
			it->second._hasGrouping = false;
			it->second._groupingName = "";
		}

		++it;
	}

	std::vector<HidroGroupInfo>::iterator itGroup = _groupInfo.begin();

	while(itGroup != _groupInfo.end())
	{
		if(itGroup->_className == groupName)
		{
			_groupInfo.erase(itGroup);
			break;
		}

		++it;
	}
}

int HidroGroupLUTController::getNumGroups()
{
	return _groupInfo.size();
}

bool HidroGroupLUTController::hasGrouping()
{
	std::map<std::string, HidroGroupLutInfo>::iterator it = _lutInfo.begin();

	while(it != _lutInfo.end())
	{
		if(it->second._hasGrouping)
		{
			return true;
		}

		++it;
	}
	
	return false;
}

bool HidroGroupLUTController::generatingGrouping(const std::string& layerName)
{
	TeRaster* inputRaster = _layer->raster();

	TeLayer* layer = new TeLayer(layerName, _layer->database(), _layer->projection());

	std::string objId = "O1";

	TeRasterParams params = inputRaster->params();
	params.nBands(1);
	params.setDataType(TeUNSIGNEDCHAR);
	params.mode_ = 'c';
	params.fileName_ = "RasterLayer" + Te2String(layer->id()) + "_R_" + objId;
	params.decoderIdentifier_ = "DB";
	params.database_ = _layer->database();
	params.lutName_ = params.fileName_ + "_LUT";
	params.blockWidth_ = 512;
	params.blockHeight_ = 512;
	params.projection(layer->projection());
	params.setPhotometric(TeRasterParams::TePallete);
	params.useDummy_ = false;
	params.boundingBoxResolution(_layer->box().x1(),
								 _layer->box().y1(),
								 _layer->box().x2(),
								 _layer->box().y2(),
								 inputRaster->params().resx_,
								 inputRaster->params().resy_); 

	params.lutr_.clear();
	params.lutg_.clear();
	params.lutb_.clear();

	TeRaster* outputRaster = new TeRaster(params);

	if(!outputRaster->init())
	{
		delete outputRaster;
		layer->database()->deleteLayer(layer->id());
		
		return false;
	}

	layer->addRasterGeometry(outputRaster, objId);

//create legend
	HidroLUTController lutControl(layer);

	if(lutControl.createAttrLUTTable())
	{
		for(unsigned int i = 0; i < _groupInfo.size(); ++i)
		{
			int r = atoi(_groupInfo[i]._r.c_str());
			int g = atoi(_groupInfo[i]._g.c_str());
			int b = atoi(_groupInfo[i]._b.c_str());

			lutControl.addNewClass(_groupInfo[i]._className, r, g, b, false);

			_groupInfo[i]._index = Te2String(i);
		}
	}

//generate map for color transform
	std::map<int, int> lut2GroupLut;

	std::map<std::string, HidroGroupLutInfo>::iterator it = _lutInfo.begin();

	while(it != _lutInfo.end())
	{
		if(it->second._hasGrouping)
		{
			std::string groupName = it->second._groupingName;

			int idxOrigin = atoi(it->second._index.c_str());

			for(unsigned int i = 0; i < _groupInfo.size(); ++i)
			{
				if(_groupInfo[i]._className == groupName)
				{
					std::string idx = _groupInfo[i]._index;

					int idxDestiny= atoi(idx.c_str());

					lut2GroupLut.insert(std::map<int, int>::value_type(idxOrigin, idxDestiny));
				}
			}
		}

		++it;
	}


//fill output raster
	for(unsigned int l = 0; l < inputRaster->params().nlines_; ++l)
	{
		for(unsigned int c = 0; c < inputRaster->params().ncols_; ++c)
		{
			double value = 0;

			if(inputRaster->getElement(c, l, value))
			{
				int val = value;

				std::map<int, int>::iterator it = lut2GroupLut.find(val);

				if(it != lut2GroupLut.end())
				{
					outputRaster->setElement(c, l, it->second);
				}
				else
				{
					outputRaster->setElement(c, l, 0);
				}
			}
		}
	}

	outputRaster->clear();

	std::string tableR = layer->database()->getRasterTable(layer->id(), objId);
	layer->database()->insertMetadata(tableR, layer->database()->getSpatialIdxColumn(TeRASTER), 0.000005,0.000005, layer->box());		
	layer->database()->createSpatialIndex(tableR, layer->database()->getSpatialIdxColumn(TeRASTER), (TeSpatialIndexType)TeRTREE);


	return true;
}

void HidroGroupLUTController::updateLutInfo()
{
	_lutInfo.clear();

	if(_layer)
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
			HidroGroupLutInfo lutEntry;
			
			lutEntry._index = portal->getData(0);
			lutEntry._r = portal->getData(1);
			lutEntry._g = portal->getData(2);
			lutEntry._b = portal->getData(3);
			
			if(hasClassInfo)
			{
				lutEntry._className = portal->getData(4);
			}
			else
			{
				lutEntry._className = "";
			}

			lutEntry._hasGrouping = false;
			lutEntry._groupingName = "";

			_lutInfo.insert(std::map<std::string, HidroGroupLutInfo>::value_type(lutEntry._index, lutEntry));
		}

		portal->freeResult();

		delete portal;
	}
}
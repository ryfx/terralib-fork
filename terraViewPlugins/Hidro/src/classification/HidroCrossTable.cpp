#include <HidroCrossTable.h>

#include <HidroUtils.h>
#include <HidroLUTController.h>

#include <HidroOverlay.h>
#include <HidroOverlayClassGroup.h>
#include <HidroOverlayClass.h>
#include <HidroOverlayRuleGroup.h>
#include <HidroOverlayRule.h>

#include <HidroAttrString.h>

#include <TeRaster.h>
#include <TeLayer.h>
#include <TeColorUtils.h>

#include <map>

HidroCrossTable::HidroCrossTable(TeDatabase* db, TeRaster* refRaster): 
_db(db)
, _referenceRaster(refRaster)
, _outputRaster(NULL)
, _classGroup(NULL)
{
}

HidroCrossTable::~HidroCrossTable()
{
	_layerVec.clear();

	if(_outputRaster)
	{
		delete _outputRaster;
	}

	if(_classGroup)
	{
		delete _classGroup;
	}
}

bool HidroCrossTable::executeCrossTable(const std::vector<std::string>& layerVec)
{
//validation
	_layerVec = layerVec;

	if(!_db)
	{
		return false;
	}

	if(!_referenceRaster)
	{
		return false;
	}

	if(!checkInputLayer())
	{
		return false;
	}

//clear variables

	if(_outputRaster)
	{
		delete _outputRaster;
	}

	if(_classGroup)
	{
		delete _classGroup;
	}

//generate rules

	_classGroup = new HidroOverlayClassGroup();

	if(!generateRules())
	{
		return false;
	}

//run overlay
	HidroOverlay overlay(_db);

	overlay.setClassGroup(_classGroup);
	overlay.setReferenceRaster(_referenceRaster);

	_outputRaster = overlay.runOverlayOperation();

	return true;
}

TeRaster* HidroCrossTable::getOutputRaster()
{
	return _outputRaster;
}

HidroOverlayClassGroup* HidroCrossTable::getClassGroup()
{
	return _classGroup;
}

bool HidroCrossTable::checkInputLayer()
{
	if(_layerVec.empty())
	{
		return false;
	}

	if(_layerVec.size() != 2)
	{
		return false;
	}

	HidroUtils util(_db);

	for(unsigned int i = 0; i < _layerVec.size(); ++i)
	{
		TeLayer* layer = util.getLayerByName(_layerVec[i]);

		if(!layer || !layer->hasGeometry(TeRASTER))
		{
			return false;
		}

		TeRaster* raster = layer->raster();

		if(raster->params().photometric_[0] != TeRasterParams::TePallete)
		{
			return false;
		}

		HidroLUTController lutControl(layer);

		if(!lutControl.hasAttrLUTTable())
		{
			return false;
		}
	}

	return true;
}

bool HidroCrossTable::generateRules()
{
	HidroUtils util(_db);

	std::map<TeLayer*, std::vector<std::string> > mapClass;

	for(unsigned int i = 0; i < _layerVec.size(); ++i)
	{
		std::string layerName = _layerVec[i];

		TeLayer* layer = util.getLayerByName(layerName);

		HidroLUTController lutControl(layer);

		std::vector<std::string> lutClassList = lutControl.listLUTClasses();

		mapClass.insert(std::map<TeLayer*, std::vector<std::string> >::value_type(layer, lutClassList));
	}

	std::map<TeLayer*, std::vector<std::string> >::iterator it = mapClass.begin();

//check the number of generated classes
	int val = 1;

	while(it != mapClass.end())
	{
		val = val * it->second.size();

		++it;
	}

	if(val > 255)
	{
		return false;
	}

//generate color vector
	std::vector<std::string> colorsVec;
	colorsVec.push_back("RED");
	colorsVec.push_back("GREEN");
	colorsVec.push_back("BLUE");

	std::vector<TeColor> colors;

	getColors(colorsVec, val, colors);

//1st layer
	it = mapClass.begin();

	TeLayer* layerFirst = it->first;
	std::vector<std::string> classVecFirst = it->second;

//2nd layer
	++it;

	TeLayer* layerSecond = it->first;
	std::vector<std::string> classVecSecond = it->second;


//generate classes
	
	if(_classGroup)
	{
		delete _classGroup;
	}

	_classGroup = new HidroOverlayClassGroup();

	int count = 1;
	
	for(unsigned int i = 0; i < classVecFirst.size(); ++i)
	{
		std::string classNameFirst = classVecFirst[i];

		HidroAttrString* attrFirst = new HidroAttrString("class", classNameFirst);

		HidroOverlayRule* rule1 = new HidroOverlayRule(classNameFirst);
		rule1->setRuleAttr(attrFirst);
		rule1->setLayer(layerFirst);
		rule1->setComparisonOperator(HidroOverlayRule::Equal);

		for(unsigned int j = 0; j < classVecSecond.size(); ++j)
		{
			std::string classNameSecond = classVecSecond[j];

			HidroAttrString* attrSecond = new HidroAttrString("class", classNameSecond);

			HidroOverlayRule* rule2 = new HidroOverlayRule(classNameSecond);
			rule2->setRuleAttr(attrSecond);
			rule2->setLayer(layerSecond);
			rule2->setComparisonOperator(HidroOverlayRule::Equal);

			std::string className = classNameFirst + "_" + classNameSecond;

			HidroOverlayRuleGroup* ruleGroup = new HidroOverlayRuleGroup(className);
			ruleGroup->addNewRule(rule1);
			ruleGroup->addNewRule(rule2);

			HidroOverlayClass* classItem = new HidroOverlayClass(count, className);
			classItem->addNewRuleGroup(ruleGroup);
			classItem->setClassColor(colors[count - 1]);

			_classGroup->addNewClass(classItem);
			
			++count;
		}
	}

	return true;
}
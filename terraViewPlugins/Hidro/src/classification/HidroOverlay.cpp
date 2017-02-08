#include <HidroOverlay.h>

#include <HidroOverlayClassGroup.h>
#include <HidroOverlayClass.h>
#include <HidroOverlayRuleGroup.h>

#include <HidroAttrString.h>
#include <HidroAttrInt.h>
#include <HidroAttrDouble.h>

#include <HidroLUTController.h>

#include <TeRaster.h>
#include <TeRasterParams.h>
#include <TeVisual.h>
#include <TeVectorRemap.h>
#include <TeLayer.h>

HidroOverlay::HidroOverlay(TeDatabase* db): _db(db)
{
	_referenceRaster	= NULL;
	_classGroup			= NULL;
	_errorMessage		= "";
}

HidroOverlay::~HidroOverlay()
{
}

void HidroOverlay::setReferenceRaster(TeRaster* raster)
{
	_referenceRaster = raster;
}

TeRaster* HidroOverlay::getReferenceRaster()
{
	return _referenceRaster;
}

void HidroOverlay::setClassGroup(HidroOverlayClassGroup* classGroup)
{
	_classGroup = classGroup;
}

HidroOverlayClassGroup* HidroOverlay::getClassGroup()
{
	return _classGroup;
}

TeRaster* HidroOverlay::runOverlayOperation()
{
	TeRaster* raster = NULL;

	if(!_referenceRaster)
	{
		return false;
	}

	if(!_classGroup)
	{
		return false;
	}

//create output raster
	TeRasterParams params = _referenceRaster->params();
	params.mode_ = 'c';
	params.decoderIdentifier_ = "SMARTMEM";
	params.useDummy_ = true;
	params.setDummy(0);
	params.setPhotometric(TeRasterParams::TePallete);
	params.nBands(1);

//create output lut
	//clear
	params.lutr_.clear();
	params.lutg_.clear();
	params.lutb_.clear();
	//dummy value
	params.lutr_.push_back(255);
	params.lutg_.push_back(255);
	params.lutb_.push_back(255);
	//class lut
	std::vector<TeColor> vecColor = _classGroup->getClassGroupLut();
	for(unsigned int i = 0; i < vecColor.size(); ++i)
	{
		params.lutr_.push_back(vecColor[i].red_);
		params.lutg_.push_back(vecColor[i].green_);
		params.lutb_.push_back(vecColor[i].blue_);
	}

//start raster
	raster = new TeRaster(params);

	if(!raster->init())
	{
		delete raster;
		return NULL;
	}


//overlay operation
	TeProjection* proj = params.projection();

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Generating Overlay");
		TeProgress::instance()->setTotalSteps(params.nlines_);
	}

	for(int i = 0; i < params.nlines_; ++i)
	{
		for(int j = 0; j < params.ncols_; ++j)
		{
			TeCoord2D coordOrigin = raster->index2Coord(TeCoord2D(j, i));

			int value = getPixelValue(coordOrigin, proj);

			raster->setElement(j, i, (double)value);
		}
	
		if(TeProgress::instance()->wasCancelled())
		{
			break;
		}

		if( TeProgress::instance() )
		{
			TeProgress::instance()->setProgress( i );
		}
	}

	if(TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}

	return raster;
}

std::string HidroOverlay::getErrorMessage()
{
	return _errorMessage;
}

int HidroOverlay::getPixelValue(const TeCoord2D& coord, TeProjection* proj)
{
//get the class group
	std::map<std::string, HidroOverlayClass*>::iterator itClassGroup = _classGroup->getClasses().begin();

	while(itClassGroup != _classGroup->getClasses().end())
	{
		bool classTrue = false;

//get class
		HidroOverlayClass* classItem = itClassGroup->second;

		int val = classItem->getClassIdx();

//get the rule group
		std::map<std::string, HidroOverlayRuleGroup*>::iterator itRuleGroup = classItem->getRuleGroups().begin();

		while(itRuleGroup != classItem->getRuleGroups().end())
		{
			bool ruleGroupTrue = true;

			HidroOverlayRuleGroup* ruleGroup = itRuleGroup->second;

//get rules
			std::map<std::string, HidroOverlayRule*>::iterator itRule = ruleGroup->getRuleGroup().begin();

			while(itRule != ruleGroup->getRuleGroup().end())
			{
				bool ruleTrue = true;

				HidroOverlayRule* rule = itRule->second;

				ruleTrue = executeRule(rule, coord, proj);

				if(!ruleTrue)
				{
					ruleGroupTrue = false;
					break;
				}

				++itRule;
			}

			if(ruleGroupTrue)
			{
				classTrue = true;
				break;
			}

			++itRuleGroup;
		}

		if(classTrue)
		{
			return val;
		}

		++itClassGroup;
	}

	return 0;
}

bool HidroOverlay::executeRule(HidroOverlayRule* rule, const TeCoord2D& coord, TeProjection* proj)
{
//verify params
	if(!rule->getLayer() && !rule->getLayer()->hasGeometry(TeRASTER))
	{
		return false;
	}

	TeProjection* projOut = rule->getLayer()->projection();

	TeRaster* raster = rule->getLayer()->raster();

    TeCoord2D coordOut = coord;


//reproject coord if necessary
	if(proj && projOut && !(*proj == *projOut))
	{
		TeCoord2D coordIn = coord;
		TeVectorRemap(coordIn, proj, coordOut, projOut);
	}

	TeCoord2D coordMatrix = raster->coord2Index(coordOut);

	double value;

//get element
	if(!raster->getElement(coordMatrix.x(), coordMatrix.y(), value))
	{
		return false;
	}

//check attribute
	HidroAttr* attr = rule->getRuleAttr();

	bool res = false;

	if(attr->getAttrType() == TeSTRING)
	{
		HidroAttrString* attrString = (HidroAttrString*)rule->getRuleAttr();
		std::string classNameWanted = attrString->getAttrValue();

		//get the className from value
		HidroLUTController lut(rule->getLayer());
		std::string classNameFounded = lut.getClassName((int)value);

//compare value
		res = compareValue(classNameWanted, classNameFounded, rule->getComparisonOperator());
	}
	else if(attr->getAttrType() == TeINT)
	{
		HidroAttrInt* attrInt = (HidroAttrInt*)rule->getRuleAttr();
		int valueWanted = attrInt->getAttrValue();
		int valueFounded = (int)value;

//compare value
		res = compareValue(valueWanted, valueFounded, rule->getComparisonOperator());

	}
	else if(attr->getAttrType() == TeREAL)
	{
		HidroAttrDouble* attrDouble = (HidroAttrDouble*)rule->getRuleAttr();
		double valueWanted = attrDouble->getAttrValue();
		double valueFounded = value;

//compare value
		res = compareValue(valueWanted, valueFounded, rule->getComparisonOperator());
	}

	return res;
}

bool HidroOverlay::compareValue(const double& valueWanted, const double& valueFound, const HidroOverlayRule::OverlayComparisonOperator& op)
{

	if(op == HidroOverlayRule::Equal)
	{
		if(valueFound == valueWanted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(op == HidroOverlayRule::NotEqual)
	{
		if(valueFound != valueWanted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(op == HidroOverlayRule::GreaterThan)
	{
		if(valueFound > valueWanted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(op == HidroOverlayRule::LessThan)
	{
		if(valueFound < valueWanted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(op == HidroOverlayRule::GreaterThanOrEqualTo)
	{
		if(valueFound >= valueWanted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(op == HidroOverlayRule::LessThanOrEqualTo)
	{
		if(valueFound <= valueWanted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

bool HidroOverlay::compareValue(const int& valueWanted, const int& valueFound, const HidroOverlayRule::OverlayComparisonOperator& op)
{
	if(op == HidroOverlayRule::Equal)
	{
		if(valueFound == valueWanted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(op == HidroOverlayRule::NotEqual)
	{
		if(valueFound != valueWanted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(op == HidroOverlayRule::GreaterThan)
	{
		if(valueFound > valueWanted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(op == HidroOverlayRule::LessThan)
	{
		if(valueFound < valueWanted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(op == HidroOverlayRule::GreaterThanOrEqualTo)
	{
		if(valueFound >= valueWanted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(op == HidroOverlayRule::LessThanOrEqualTo)
	{
		if(valueFound <= valueWanted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

bool HidroOverlay::compareValue(const std::string& valueWanted, const std::string& valueFound, const HidroOverlayRule::OverlayComparisonOperator& op)
{
	if(op == HidroOverlayRule::Equal)
	{
		if(valueWanted == valueFound)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(op == HidroOverlayRule::NotEqual)
	{
		if(valueWanted != valueFound)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}
#include <HidroAttr.h>
#include <HidroVertex.h>
#include <HidroEdge.h>

HidroVertex::HidroVertex(): _vertexId(""), _vertexCoord(TeCoord2D())
{
}

HidroVertex::HidroVertex(const std::string& id, const TeCoord2D& coord)
{
	_vertexId		= id;
	_vertexCoord	= coord;
}

HidroVertex::~HidroVertex()
{
	std::vector<HidroAttr*>::iterator it = _vertexAttrVec.begin();

	while(it != _vertexAttrVec.end())
	{
		delete *it;

		++it;
	}

	_vertexAttrVec.clear();

	_mapEdgeOut.clear();

	_mapEdgeIn.clear();
}

void HidroVertex::setVertexId(const std::string& id)
{
	_vertexId = id;
}

std::string HidroVertex::getVertexId() const
{
	return _vertexId;
}

void HidroVertex::setVertexIdx(const int& idx)
{
	_vertexIdx = idx;
}

int HidroVertex::getVertexIdx() const
{
	return _vertexIdx;
}

void HidroVertex::setVertexCoord(const TeCoord2D& coord)
{
	_vertexCoord = coord;
}

TeCoord2D HidroVertex::getVertexCoord() const
{
	return _vertexCoord;
}

bool HidroVertex::addVertexAttr(HidroAttr* attr)
{
	//verify if the input attribute has a valid name
	if(attr->getAttrName().empty())
	{
		return false;
	}

	//check if already exist an attribute with this name
	std::vector<HidroAttr*>::iterator it = getVertexAttrItem(attr->getAttrName());

	if(it != _vertexAttrVec.end())
	{
		return false;
	}

	//add this attribute
	_vertexAttrVec.push_back(attr);

	return true;
}

bool HidroVertex::removeVertexAttr(const std::string& attrName)
{
	//check if exist an attribute with this name
	std::vector<HidroAttr*>::iterator it = getVertexAttrItem(attrName);

	if(it == _vertexAttrVec.end())
	{
		return false;	
	}
	
	//remove attribute from vertex attr vector
	delete *it;

	_vertexAttrVec.erase(it);

	return true;
}

HidroAttr* HidroVertex::getVertexAttr(const std::string& attrName)
{
	//check if exist an attribute with this name
	std::vector<HidroAttr*>::iterator it = getVertexAttrItem(attrName);

	//if the attribute was not found return false
	if(it == _vertexAttrVec.end())
	{
		return NULL;
	}

	return *it;
}

std::vector<HidroAttr*>& HidroVertex::getVertexVecAttr()
{
	return _vertexAttrVec;
}

std::map<std::string, HidroEdge*>& HidroVertex::getMapEdgeOut()
{
	return _mapEdgeOut;
}

void HidroVertex::addEdgeOut(HidroEdge* edge)
{
	std::map<std::string, HidroEdge*>::iterator it = _mapEdgeOut.find(edge->getEdgeId());

	if(it == _mapEdgeOut.end())
	{
		_mapEdgeOut.insert(std::map<std::string, HidroEdge*>::value_type(edge->getEdgeId(), edge));
	}
}

std::map<std::string, HidroEdge*>& HidroVertex::getMapEdgeIn()
{
	return _mapEdgeIn;
}

void HidroVertex::addEdgeIn(HidroEdge* edge)
{
	std::map<std::string, HidroEdge*>::iterator it = _mapEdgeIn.find(edge->getEdgeId());

	if(it == _mapEdgeIn.end())
	{
		_mapEdgeIn.insert(std::map<std::string, HidroEdge*>::value_type(edge->getEdgeId(), edge));
	}
}

std::vector<HidroAttr*>::iterator HidroVertex::getVertexAttrItem(const std::string& attrName)
{
	//verify if input attr name is empty
	if(!attrName.empty())
	{
		std::vector<HidroAttr*>::iterator it = _vertexAttrVec.begin();

		//find an attribute with this name
		while(it != _vertexAttrVec.end())
		{
			if((*it)->getAttrName() == attrName)
			{
				//attribute found
				return it;
			}
			++it;
		}	
	}

	return _vertexAttrVec.end();
}
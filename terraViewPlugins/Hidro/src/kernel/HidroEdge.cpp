#include <HidroEdge.h>
#include <HidroVertex.h>
#include <HidroAttr.h>

HidroEdge::HidroEdge(): _edgeId(""), _edgeVertexFrom(NULL), _edgeVertexTo(NULL)
{
}

HidroEdge::HidroEdge(const std::string& id) : _edgeVertexFrom(NULL), _edgeVertexTo(NULL)
{
	_edgeId		= id;
}

HidroEdge::~HidroEdge()
{
	std::vector<HidroAttr*>::iterator it = _edgeAttrVec.begin();

	while(it != _edgeAttrVec.end())
	{
		delete *it;

		++it;
	}

	_edgeAttrVec.clear();
}

void HidroEdge::setEdgeId(const std::string& id)
{
	_edgeId = id;
}

std::string HidroEdge::getEdgeId() const
{
	return _edgeId;
}

TeLine2D HidroEdge::getEdgeLine() const
{
	TeLine2D line;

	if(this->getEdgeVertexFrom() && this->getEdgeVertexTo())
	{
		line.add(this->getEdgeVertexFrom()->getVertexCoord());
		line.add(this->getEdgeVertexTo()->getVertexCoord());
	}

	return line;
}

void HidroEdge::setEdgeVertexFrom(HidroVertex* vertex)
{
	_edgeVertexFrom = vertex;

//add this edge into vertex map edge out
	_edgeVertexFrom->addEdgeOut(this);
}

HidroVertex* HidroEdge::getEdgeVertexFrom() const
{
	return _edgeVertexFrom;
}

void HidroEdge::setEdgeVertexTo(HidroVertex* vertex)
{
	_edgeVertexTo = vertex;

//add this edge into vertex map edge in
	_edgeVertexTo->addEdgeIn(this);
}

HidroVertex* HidroEdge::getEdgeVertexTo() const
{
	return _edgeVertexTo;
}

bool HidroEdge::addEdgeAttr(HidroAttr* attr)
{
	//verify if the input attribute has a valid name
	if(attr->getAttrName().empty())
	{
		return false;
	}

	//check if already exist an attribute with this name
	std::vector<HidroAttr*>::iterator it = getEdgeAttrItem(attr->getAttrName());

	if(it != _edgeAttrVec.end())
	{
		return false;
	}

	//add this attribute
	_edgeAttrVec.push_back(attr);

	return true;
}

bool HidroEdge::removeEdgeAttr(const std::string& attrName)
{
	//check if exist an attribute with this name
	std::vector<HidroAttr*>::iterator it = getEdgeAttrItem(attrName);

	if(it == _edgeAttrVec.end())
	{
		return false;	
	}
	
	//remove attribute from vertex attr vector
	delete *it;

	_edgeAttrVec.erase(it);

	return true;
}

HidroAttr* HidroEdge::getEdgeAttr(const std::string& attrName)
{
	//check if exist an attribute with this name
	std::vector<HidroAttr*>::iterator it = getEdgeAttrItem(attrName);

	//if the attribute was not found return false
	if(it == _edgeAttrVec.end())
	{
		return NULL;
	}

	return *it;
}

std::vector<HidroAttr*>& HidroEdge::getEdgeVecAttr()
{
	return _edgeAttrVec;
}

std::vector<HidroAttr*>::iterator HidroEdge::getEdgeAttrItem(const std::string& attrName)
{
	//verify if input attr name is empty
	if(!attrName.empty())
	{
		std::vector<HidroAttr*>::iterator it = _edgeAttrVec.begin();

		//find an attribute with this name
		while(it != _edgeAttrVec.end())
		{
			if((*it)->getAttrName() == attrName)
			{
				//attribute found
				return it;
			}
			++it;
		}	
	}

	return _edgeAttrVec.end();
}
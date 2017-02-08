#include <HidroBoostUtils.h>

#include <HidroGraph.h>
#include <HidroEdge.h>
#include <HidroVertex.h>

#include <HidroBoostParams.h>
#include <HidroBoostAlgorithmFactory.h>

#include <HidroAttrInt.h>
#include <HidroAttrDouble.h>
#include <HidroAttrString.h>

std::vector<std::string> getBoostAlgorithmsNames()
{
	std::vector<std::string> boostAlgorithmsNames;

	HidroBoostAlgorithmFactory::TeFactoryMap::iterator it = HidroBoostAlgorithmFactory::instance().begin();

	while( it != HidroBoostAlgorithmFactory::instance().end() ) 
	{
		HidroBoostParams params;
		params._boostAlgorithmName = it->first.c_str();			//boost algorithm name

		HidroBoostAlgorithms* boostAlgorithm = HidroBoostAlgorithmFactory::make(params);

		if(boostAlgorithm)
		{
			boostAlgorithmsNames.push_back(params._boostAlgorithmName);
		}

		delete boostAlgorithm;

		++it;
	}

	return boostAlgorithmsNames;
}

bool createGraphComponent(HidroGraph* inputGraph, HidroGraph* outputGraph, const int& startVertexIdx, const int& endVertexIdx, int& idx)
{
//check if exist input and output vertex from input graph
	HidroVertex* fromVertex = inputGraph->getGraphVertexByIdx(startVertexIdx);
	HidroVertex* toVertex = inputGraph->getGraphVertexByIdx(endVertexIdx);

	if(!fromVertex || !toVertex)
	{
		return false;
	}

//copy input vertex
	HidroVertex* vertexFrom = new HidroVertex(fromVertex->getVertexId(), fromVertex->getVertexCoord());
	vertexFrom->setVertexIdx(idx);
	++idx;

	if(!copyVertexAttributes(fromVertex, vertexFrom))
	{
		return false;
	}
	
//copy output vertex
	HidroVertex* vertexTo = new HidroVertex(toVertex->getVertexId(), toVertex->getVertexCoord());
	vertexTo->setVertexIdx(idx);
	++idx;

	if(!copyVertexAttributes(toVertex, vertexTo))
	{
		return false;
	}

//create new edge
	HidroEdge* edge = new HidroEdge();

	std::string edgeName = "edge_" + vertexFrom->getVertexId() + "_" + vertexTo->getVertexId();
	edge->setEdgeId(edgeName);

	edge->setEdgeVertexFrom(vertexFrom);
	edge->setEdgeVertexTo(vertexTo);

//insert new components into output graph
	outputGraph->addGraphVertex(vertexFrom);
	outputGraph->addGraphVertex(vertexTo);
	outputGraph->addGraphEdge(edge);

	return true;
}

bool copyVertexAttributes(HidroVertex* input, HidroVertex* output)
{
	for(unsigned int i = 0; i < input->getVertexVecAttr().size(); ++i)
	{
		HidroAttr* inputAttr = input->getVertexVecAttr()[i];

		TeAttrDataType type = inputAttr->getAttrType();

		if(type == TeSTRING)
		{
			HidroAttrString* attr = new HidroAttrString();
			attr->setAttrName(((HidroAttrString*)inputAttr)->getAttrName());
			attr->setAttrValue(((HidroAttrString*)inputAttr)->getAttrValue());

			if(!output->addVertexAttr(attr))
			{
				return false;
			}
		}
		else if(type == TeINT)
		{
			HidroAttrInt* attr = new HidroAttrInt();
			attr->setAttrName(((HidroAttrInt*)inputAttr)->getAttrName());
			attr->setAttrValue(((HidroAttrInt*)inputAttr)->getAttrValue());

			if(!output->addVertexAttr(attr))
			{
				return false;
			}
		}
		else if(type == TeREAL)
		{
			HidroAttrDouble* attr = new HidroAttrDouble();
			attr->setAttrName(((HidroAttrDouble*)inputAttr)->getAttrName());
			attr->setAttrValue(((HidroAttrDouble*)inputAttr)->getAttrValue());

			if(!output->addVertexAttr(attr))
			{
				return false;
			}
		}
	}

	return true;
}

bool copyEdgeAttributes(HidroEdge* input, HidroEdge* output)
{
	for(unsigned int i = 0; i < input->getEdgeVecAttr().size(); ++i)
	{
		HidroAttr* inputAttr = input->getEdgeVecAttr()[i];

		TeAttrDataType type = inputAttr->getAttrType();

		if(type == TeSTRING)
		{
			HidroAttrString* attr = new HidroAttrString();
			attr->setAttrName(((HidroAttrString*)inputAttr)->getAttrName());
			attr->setAttrValue(((HidroAttrString*)inputAttr)->getAttrValue());

			if(!output->addEdgeAttr(attr))
			{
				return false;
			}
		}
		else if(type == TeINT)
		{
			HidroAttrInt* attr = new HidroAttrInt();
			attr->setAttrName(((HidroAttrInt*)inputAttr)->getAttrName());
			attr->setAttrValue(((HidroAttrInt*)inputAttr)->getAttrValue());

			if(!output->addEdgeAttr(attr))
			{
				return false;
			}
		}
		else if(type == TeREAL)
		{
			HidroAttrDouble* attr = new HidroAttrDouble();
			attr->setAttrName(((HidroAttrDouble*)inputAttr)->getAttrName());
			attr->setAttrValue(((HidroAttrDouble*)inputAttr)->getAttrValue());

			if(!output->addEdgeAttr(attr))
			{
				return false;
			}
		}
	}

	return true;
}
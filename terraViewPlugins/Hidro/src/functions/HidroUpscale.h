#ifndef HIDRO_UPSCALE_H
#define HIDRO_UPSCALE_H

#include <string>
#include <map>
#include <set>

#include <TeBox.h>

class TeDatabase;
class TeRaster;

class HidroGraph;
class HidroVertex;
class HidroEdge;


struct DeepEdgeVertex
{
	std::string vertexFromId;
	std::string vertexToId;
	int	deepValue;
};

struct OrderByVal
{
	bool operator() (const DeepEdgeVertex& devA, DeepEdgeVertex& devB) const
	{
		if( devA.deepValue < devB.deepValue)
		{
			return true;
		}

		return false;
	}
};

class HidroUpscale
{
public:
	HidroUpscale(TeDatabase* db);

	~HidroUpscale();

public:
	bool generateUpscale(const std::string& inputGraphName, const std::string& gridLayerName, const int& COTAT, const int& MUFP);

	HidroGraph* getOutputGraph();

	std::string errorMessage();

protected:
	bool calculateDeepAttribute(HidroGraph& graph, const std::string& attrName, const bool& showProgress);

	void calculateDeepValue(const std::string& vertexId, HidroGraph* graph, int& deepValue, std::set<std::string>& vertexIdSet);

	bool getIntersectGrid(TeRaster* inputGrid, HidroGraph& inputGraph);

	TeBox getCellBox(TeRaster* raster, const int& lineCur, const int& colCur);

	HidroGraph* getSubGraph(const std::vector<HidroVertex*>& vecVertex, std::map<std::string, HidroEdge*>& edgeOutMap);

	std::string findOutputVertex(HidroGraph* subGraph, std::map<std::string, HidroEdge*>& edgeOutMap, const int& MUFP);

	void getDistValue(HidroVertex* vertex, double& dist, const int& MUFP);

	double calculateEdgeDistValue(HidroVertex* vA, HidroVertex* vB);

	double theta(const TeCoord2D& c1, const TeCoord2D& c2);

	int getCellDirection(HidroVertex* vertex, TeRaster* raster, const int& colCur, const int& lineCur, const int& COTAT);

	bool checkCrossEdges(HidroGraph* graph);

	void fixCrossEdge(HidroEdge* edge, HidroVertex* vertex);

protected:
	TeDatabase* _db;

	HidroGraph*	_outputGraph;

	std::string _errorMessage;

	std::string	_attributeGraphDeep;
	std::string	_attributeSubGraphDeep;

	int _countSubGraph;
};

#endif //HIDRO_UPSCALE_H
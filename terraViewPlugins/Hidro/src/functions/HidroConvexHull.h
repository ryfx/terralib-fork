#ifndef HIDRO_CONVEXHULL_H
#define HIDRO_CONVEXHULL_H

#include <string>

#include <TeDataTypes.h>

class TeDatabase;
class TeLayer;
class TePolygonSet;
class TeTable;
class TeRaster;

class HidroGraph;

class HidroConvexHull
{
public:
	HidroConvexHull(TeDatabase* database);
	~HidroConvexHull();


public:
	TeLayer* generateConvexHull(HidroGraph* graph, const double& bufferDistance = 0.);

	TeLayer* generateMask(HidroGraph* graph, TeRaster* raster, const std::string& layerName);

	TeLayer* generatePols(TeRaster* raster, const std::string& layerName);

	std::string errorMessage();

	bool createOutputLayer(TeLayer* layer, TePolygonSet& polSet);

protected:

	bool createTable(const std::string& tableName, TeTable& table);

protected:
	std::string _errorMessage;
	TeDatabase* _database;

};

#endif //HIDRO_CONVEXHULL_H
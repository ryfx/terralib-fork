#include "TeDatabase.h"
#include "TeAdoDB.h"
#include "TeBlockLoader.h"
#include "TeCoverage.h"
#include "TeCoverageDecoder.h"
#include "TeCoverageImport.h"

void printGeom(TePoint& point)
{
    std::cout << "(" << point.location().x() << "," << point.location().y() << ")";
}

void printGeom(TeLine2D& line)
{
    std::cout << "(line id = " << line.geomId() << ")";
}

void printGeom(TePolygon& poly)
{
    std::cout << "(polygon id = " << poly.geomId() << ")";
}

template <class T>
void testCoverage(std::string& databaseFile, std::string& coverageLayerName, std::string& coverageId, std::string& srcLayerName, std::vector<std::string>& columns)
{
    std::cout << "Entered testCoverage." << std::endl;

    // Connect to the database
	TeDatabase* myDB = new TeAdo();
	bool connected = myDB->connect("", "", "", databaseFile, 0);
    myDB->loadLayerSet(true);

    // Setup coverage layer
    TeLayer* layer = NULL;
    try
    {
        // Check if coverage layer exists
        if (myDB->layerExist(coverageLayerName))
        {
            std::cout << "> Layer '" << coverageLayerName << "' found on database." << std::endl;

            std::vector<std::string> coverageIds;
            layer = new TeLayer(coverageLayerName, myDB);
            TeRetrieveCoverageIds(layer, coverageIds);

            // Check if coverage is in the coverage layer
            if (find(coverageIds.begin(), coverageIds.end(), coverageId) != coverageIds.end())
            {
                std::cout << "> Coverage '" << coverageId << "' found on layer '" << layer->name() << "'." << std::endl;
            }
            else
            {
                std::string errorMsg = "Coverage '" + coverageId + "' not found on layer '" + layer->name() + "'.";
                throw TeException(UNKNOWN_ERROR_TYPE, errorMsg, false);
            }
        }
        else
        {
            std::cout << "> Layer '" << coverageLayerName << "' not found on database." << std::endl;

            // Check if source layer exists
            if (myDB->layerExist(srcLayerName))
            {
                std::cout << "> Source layer '" << srcLayerName << "' found on database." << std::endl;

                // Import from source layer
                TeLayer* srcLayer = new TeLayer(srcLayerName, myDB);
                layer = TeCoverageImportLayer<T>(myDB, coverageLayerName, srcLayer, columns, coverageId);
                
                if (layer)
                {
                    std::cout << "> Imported coverage data from source layer '" << srcLayerName << "'." << std::endl;
                }
                else
                {
                    std::string errorMsg = "Couldn't import coverage data from source layer '" + srcLayerName + "'.";
                    throw TeException(UNKNOWN_ERROR_TYPE, errorMsg, false);
                }
            }
            else
            {
                std::string errorMsg = "Source layer '" + srcLayerName + "' not found on database.";
                throw TeException(UNKNOWN_ERROR_TYPE, errorMsg, false);
            }
        }
    }
    catch(TeException e)
    {
        std::cout << "Error: " << e.message() << std::endl;
        return;
    }

    // Setup coverage parameters
    TeCoverageParams coverageParams = TeCoverageParams(myDB, layer->id(), coverageId);
    coverageParams.setPersistenceType(TePERSISTENCE_DATABASE_CACHELRU);

    // Create and initialize coverage
    TeCoverage<T> coverage = TeCoverage<T>(coverageParams);
    coverage.init();
    std::cout << "> Coverage '" << coverageParams.getCoverageId() << "' created and initialized." << std::endl;
    std::cout << "Coverage bounding box: (" << coverage.box().x1() << ", " << coverage.box().y1() << ", " << coverage.box().x2() << ", " << coverage.box().y2() << ") " << std::endl;
    std::vector<TeCoverageDimension> dimensions = coverage.getParameters().getDimensions();
    std::cout << "Coverage dimensions: ";
    for (std::vector<TeCoverageDimension>::iterator it = dimensions.begin(); it != dimensions.end(); ++it)
    {
        std::cout << "{" << it->dimension_id << ": " << it->name << "}, ";
    }
    std::cout << std::endl;

    unsigned int numDimensions = dimensions.size();

    // ============================= TEST 1 ===================================

    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "> (Test 1: Spatial selection)" << std::endl;

    // Ask the user for a selection box
    std::cout << "Enter box coordinates for the spatial query" << std::endl;

    double x1, y1, x2, y2;
    std::cout << "lower x: ";
    std::cin >> x1;
    std::cout << "lower y: ";
    std::cin >> y1;
    std::cout << "upper x: ";
    std::cin >> x2;
    std::cout << "upper y: ";
    std::cin >> y2;
    TeBox box(x1, y1, x2, y2);
    TePolygon poly = TeMakePolygon(box);

    TeCoverage<T>::iterator itSamples = coverage.begin(poly);
    TeCoverage<T>::iterator endSamples = coverage.end(poly);

    std::cout << "> Result of spatial query:" << std::endl;

    // Iterate and print geometries and values
    while (itSamples != endSamples) {

        printGeom(*itSamples);

        std::cout << ": {";
        for (unsigned int i = 0; i < numDimensions; ++i)
        {
            std::cout << itSamples[i] << ", ";
        }
        std::cout << "}" << std::endl;
        itSamples++;
    }

    // ============================= TEST 2 ===================================

    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "> (Test 2: Interpolated value)" << std::endl;

    // Ask the user for an arbitrary position
    std::cout << "Enter arbitrary coordinate for interpolating" << std::endl;
    double x, y;
    std::cout << "x: ";
    std::cin >> x;
    std::cout << "y: ";
    std::cin >> y;

    // Get interpolated value
    TeCoord2D location = TeCoord2D(x, y);
    std::vector<double> values;
    coverage.evaluate(location, values);
    std::cout << "> Result of interpolation:" << std::endl;
    std::cout << "(" << x << ", " << y << ")";
    std::cout << ": {";
    for (unsigned int i = 0; i < numDimensions; ++i)
    {
        std::cout << values[i] << ", ";
    }
    std::cout << "}" << std::endl;

    // ============================= TEST 3 ===================================
    
    // Create raster and fill with interpolated coverage data
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "> (Test 3: Fill Raster with interpolated data)" << std::endl;

    std::cout << "Enter parameters for filling raster" << std::endl;
    int nRows, nCols, srcDimension;
    std::cout << "Number of rows: ";
    std::cin >> nRows;
    std::cout << "Number of columns: ";
    std::cin >> nCols;
    std::cout << "ID of coverage dimension to be read: ";
    std::cin >> srcDimension;

    // Create raster projection and get coverage projection
    TeProjection* rasterProj = new TeLatLong(TeDatumFactory::make("SAD69"));
    TeProjection* coverageProj = coverage.getParameters().getProjection();
    coverageProj->setDestinationProjection(rasterProj);

    // Get coverage box coordinates
    TeCoord2D lowerLeft = TeCoord2D(coverage.box().x1(), coverage.box().y1());
    TeCoord2D upperRight = TeCoord2D(coverage.box().x2(), coverage.box().y2());

    // Convert to intermediate projection
    lowerLeft = coverageProj->PC2LL(lowerLeft);
    upperRight = coverageProj->PC2LL(upperRight);

    // Convert to raster projection
    lowerLeft = rasterProj->LL2PC(lowerLeft);
    upperRight = rasterProj->LL2PC(upperRight);

    TeBox rasterBox = TeBox(lowerLeft, upperRight); // Raster bounding box

    // Set raster params and create raster in memory
    TeRasterParams rasterParams;
    rasterParams.nBands(1);
    rasterParams.projection(rasterProj);
    rasterParams.setDataType(TeDOUBLE);
    rasterParams.decoderIdentifier_ = "MEM";
    rasterParams.mode_ = 'c';
    rasterParams.boundingBoxLinesColumns(rasterBox.x1(),
                                         rasterBox.y1(),
                                         rasterBox.x2(),
                                         rasterBox.y2(),
                                         nRows, nCols);
    TeRaster* rasterMem = new TeRaster(rasterParams);
    rasterMem->init();
    if (!rasterMem->status())
    {
        cout << "Falha ao criar raster em memoria." << endl;
    }

    // Fill coverage with interpolated values from the coverage, counting time
    time_t when;
    int beginTime = time(&when);
    coverage.evaluate(*rasterMem, 0, srcDimension);
    int endTime = time(&when);

    // Show contents of the raster
    for (TeRaster::iterator it = rasterMem->begin(); it != rasterMem->end(); it++)
    {
        TeCoord2D pixelCoord = rasterMem->index2Coord(TeCoord2D(it.currentColumn(), it.currentLine()));
        double val = 0;
        rasterMem->getElement(it.currentColumn(), it.currentLine(), val);
        std::cout << "coord(" << it.currentColumn() << "," << it.currentLine() << ") = (" << pixelCoord.x() << "," << pixelCoord.y() << "): value = [" << val << "]" << std::endl;
    }

    // Show time spent in raster interpolation
    std::cout << "Completed interpolation of " << nRows << "x" << nCols << " raster in " << (endTime - beginTime) << "s." << std::endl;

	// Close database connection
	myDB->close();
}

void main(int argc, char* argv[])
{
    std::cout << "This application will test some coverage functions.\n"
            << "What support representation do you want for the coverage?\n"
            << "(1=Point, 2=Line, 3=Polygon): ";

    int coverageType = -1;
    std::cin >> coverageType;

    std::string databaseFile, coverageLayerName, coverageId, srcLayerName;
    std::vector<std::string> columns;

    switch (coverageType)
    {
    case 1:
        // Database location
        databaseFile = ".\\cidades.mdb";

        // Information about coverage to be created/accessed
        coverageLayerName = "CidadesCoverageLayer";
        coverageId = "CidadesCoverage";

        // Information about source for importing
        srcLayerName = "CIDADES_point";
        columns.push_back("MD_LONGITU");
        columns.push_back("MD_LATITUD");

        testCoverage<TePoint>(databaseFile, coverageLayerName, coverageId, srcLayerName, columns);
        break;

    case 2:
        // Database location
        databaseFile = ".\\sergipe.mdb";

        // Information about coverage to be created/accessed
        coverageLayerName = "AltimetriaCoverageLayer";
        coverageId = "AltimetriaCoverage";

        // Information about source for importing
        srcLayerName = "Altimetria_lin";
        columns.push_back("COTA");
        columns.push_back("LENGTH");

        testCoverage<TeLine2D>(databaseFile, coverageLayerName, coverageId, srcLayerName, columns);
        break;

    case 3:
        // Database location
        databaseFile = ".\\sergipe.mdb";

        // Information about coverage to be created/accessed
        coverageLayerName = "UsoDoSoloCoverageLayer";
        coverageId = "UsoDoSoloCoverage";

        // Information about source for importing
        srcLayerName = "UsodoSolo_pol";
        columns.push_back("AREA");
        columns.push_back("PERIMETER");

        testCoverage<TePolygon>(databaseFile, coverageLayerName, coverageId, srcLayerName, columns);
        break;
    }
}

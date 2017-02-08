#ifndef HIDRO_FLOW_D8_1_H_
#define HIDRO_FLOW_D8_1_H_

#include <HidroMatrix.hpp>
class HidroGraph;
class HidroVertex;
class HidroEdge;

#include <TeSingleton.h>
class TeRaster;
class TeDatabase;
class TeView;
class TeLayer;

#include <TeUtils.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qstring.h>
#include <string>

/** \brief Function used to generate a LDD raster from a input raster using the eight neighbors method (D8)
                            with selection of multiples directions
    \param	inputRaster		Input parameter, terralib raster attribute, this raster could be any regular grid 
	\param	outputRaster	Output parameter, terralib raster attribute, this raster is a flow grid
	\return					True if the output raster was generated correctly and false in other case
    */
bool generateFlowD8(TeDatabase* db, TeView* view, TeLayer* demRasterLayer, std::string layerName, std::string postProcName, int treatmentMax, double boost,  std::string lddName);

/** \brief Generates a LDD raster from a input raster using the eight neighbors method (D8)
                            with selection of multiples directions
	\param inputRaster		Input parameter, terralib raster attribute, this raster could be any regular grid
	\param outputRaster		Output parameter, terralib raster attribute, this raster is a flow grid
	\param col				Current column from inputRaster
	\param lin				Current line from inputRaster
	\param lddDirectionsMap			Output raster values
	*/
bool generateCommonLDD( HidroMatrix<double>& demRaster, HidroMatrix<unsigned char>& outputRaster, int col, int lin, unsigned char lddDirectionsMap[8]);

/**
  \brief Check if slopes draw was solved.
  \param altimetriaMeans vector of 8 neighbors medias.
  \param minMedia minimum media valor.
*/
bool drawWasSolved( double altimetriaMeans[], double& minMedia );

/**
  \brief This class is used to save the statistics of flow generation.
*/
class HidroGenerateFlowStatistics : public TeSingleton< HidroGenerateFlowStatistics >
{
public :
  
  HidroGenerateFlowStatistics();

  /**
    \brief Reset the statistics values.  
  */
  void reset();
  
  /**
    \brief Record statistics in a file.
    \param fileName File Name.
  */
  void print( std::string fileName );

  /**
    \brief Generate the strem with statistics values.
  */
  void generateString();

  // types of draw
  unsigned int drawsNumber_;  
  unsigned int drawsSolvedNumber_;

  std::string text_;
};

#endif
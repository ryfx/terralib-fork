/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroFlowUtils.h
*
*******************************************************************************
*
* $Rev: 8883 $:
*
* $Author: alexcj $:
*
* $Date: 2010-08-30 23:16:16 +0300 (ma, 30 elo 2010) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Flow Group.
	@defgroup hidroFlow The Group for Plugin Flow Classes.
*/

#ifndef HIDRO_FLOW_UTILS_H_
#define HIDRO_FLOW_UTILS_H_

/** 
  * \file HidroFlowUtils.h
  *
  *
  * \brief This file has commung functions used by extract flow algorithms
  *
  * This file gives support to flow methods implemented in hidro plugin
  * Flow Methods:
  *                 DEM 8 Connected Single Flow
  *                 DEM 8 Connected Multi Flow
  *
  * \sa HidroFlowDEM8Connected.h
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroFlow
 */


/*
** ----------------------------------------------------------------------------
** Includes:
*/

#include <string>
#include <vector>

#define HIDRO_FLOW_DEM_8D_COMMON_FLOW_NEW	"8 Connected Flow Common"
#define HIDRO_FLOW_DEM_8D_RANDOM_FLOW_NEW	"8 Connected Flow Random"

class TeRaster;
class TeView;
class TeDatabase;
class TeLayer;
class TeTable;
class TePointSet;
#include <HidroMatrix.hpp>

#define LDD_DUMMY 255
#define M_SQRT2 1.41421356237309504880 // I don't know why I cant use math.h <alexandre - alexcj@dpi.inpe.br>

/** \brief Function used to get all flow algorithms from hidro plugin
	\return				Return a string vector with all algorithms names
    */
std::vector<std::string> getHidroFlowAlgorithms();

/** \brief Find the minimun slope 
    \params altimetriaNeighbors[8]		Grid values
    \params lddDirectionsMap[8]	      	Output raster values
    \params maxSlopeDirection			    Output raster lddDirectionsMap. Auxiliar variable used if exist only one direction.
    \return			                  		Lowest lddDirectionsMap in the grid
    */
double minValue(double altimetriaNeighbors[8], unsigned char lddDirectionsMap[8], unsigned char& minSlopeDirection);

double maxValue(double altimetriaNeighbors[8], unsigned char lddDirectionsMap[8], unsigned char& maxSlopeDirection);
	 
/** \brief Finds the number of existing directions in the grid
    \params altimetriaNeighbors[8]		Grid values
    \params vmin			Lowest lddDirectionsMap
    \return					Number of directions in the grid
    */
int ndirectionCompute(double altimetriaNeighbors[8], double vmin);

/** \brief Compute the altimetria mean in a 3x3 region of center cell
	\param raster 			Input raster
	\param gridcol			Column of the center cell
	\param gridlin			Line of the center cell
	\return					    Returns the altimetria mean in a 3x3 region of center cell
	*/
double altimetriaMeanCompute ( HidroMatrix<double>& raster, int gridlin, int gridcol);//(TeRaster* raster, int gridcol, int gridlin, int grid, int i);

double mediana(std::vector<double> vals, int grid);

/** \brief Generates a LDD raster from a input raster using the eight neighbors method (D8)
                            with ramdom selection of multiples directions
	\param inputRaster		Input parameter, terralib raster attribute, this raster could be any regular grid
	\param outputRaster		Output parameter, terralib raster attribute, this raster is a flow grid
	\param col				Current column from inputRaster
	\param lin				Current line from inputRaster
	\param lddDirectionsMap			Output raster values
	*/
bool generateRandLDD1(TeRaster* inputRaster, TeRaster*& outputRaster, int col, int lin, unsigned char lddDirectionsMap[8]);


/** \brief Function used to correct cross flows from LDD input raster.
	\param	outputRaster	Output parameter, terralib raster attribute, this raster is a flow grid
	\return					True if the output raster was generated correctly and false in other case
    */
bool D8FlowCorrection(TeRaster*& outputRaster);

/** \brief Function used to correct pits from LDD input raster.
    \param	inputRaster		Input parameter, terralib raster attribute, this raster could be any regular grid
	\param	outputRaster	Output parameter, terralib raster attribute, this raster is a flow grid
	\param  lddDirectionsMap			Output raster values
	\param  ldd				Algorithm type for extract flow
	\return					True if the output raster was generated correctly and false in other case
    */
bool D8PitCorrection(TeDatabase* db, TeRaster* inputRaster, TeRaster*& outputRaster, TeLayer*& outputLayer, unsigned char lddDirectionsMap[8], std::string ldd,
					 std::string layerName);

bool D8PitCorrectionMedia(TeDatabase* db, TeRaster* inputRaster, TeRaster*& outputRaster, TeLayer*& outputLayer, unsigned char lddDirectionsMap[8],
						  std::string ldd, std::string layerName);

/** \brief Function used to generate a layer with points of pits.
  */
bool generatePtsPits(TeDatabase* db, TeView* view, TeRaster* outputRaster, std::string layerName, std::string lddName);

/** \brief Creates attributes table.
  */
bool createTable(TeDatabase* db, TeTable& pointTable, std::string layerName);

/** \brief Insert values in the table.
  */
bool valuesTable(TeLayer* layer, TeTable& pointTable, TeRaster* raster, TePointSet ptset);

/** \brief Saves layer in database.
  */
bool saveLayer(TeDatabase* db, TeLayer* layer, TeTable& pointTable);

/** \brief Compute slope angle values for neighborhood
	\param inputRaster	      	Input parameter, terralib raster attribute, this raster could be any regular grid
	\param col	    		      	Current column from inputRaster
	\param lin			          	Current line from inputRaster
  \param slopeAngle           slope angle values for neighborhood
  \return				            	True if center cell is not dummy and False if center cell is dummy (can't compute the slope).
	*/
bool slopeCompute( HidroMatrix<double>& inputRaster, int col, int lin, double slopeAngle[8] );

/** \brief Check if its a border cell.
  \param demMatrix DEM
  \param col Cell X coordinate.
  \param lin Cell Y coordinate.
 */
bool isBorder( HidroMatrix<double>& demMatrix, int col, int lin );

/**
  \brief Convert LDD.
  \param lddInputName Input LDD name (layer).
  \param inputLDDmap Input LDD code.
  \param lddOutputName Output LDD name (layer).
  \param outputLDDmap Output LDD code.
 */
bool convertLDD( TeDatabase* db, std::string lddInputName, int inputLDDmap[10], std::string lddOutputName, int outputLDDmap[10], int dataType,bool useDummy );

/**
  \brief This class is used calculate processing time.
*/
class Time : public TeSingleton<Time>
{
public:
  /**
    \brief reset the timer (same that start).
  */
  void reset();

  /**
    \brief reset the timer (same that reset).
  */
  void start();

  /**
    \brief Show how many times passed before start or reset.
  */
  std::string partialString();

  /**
    \brief Show the system time and date.
  */
  std::string actualTimeString();

  /**
    \brief Return true if passed one houer by start (reset) or last call to this function.
  */
  bool hasPassedOneHouer();

protected:
  clock_t start_;
  clock_t partial_;
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif //#define HIDRO_FLOW_UTILS_H_
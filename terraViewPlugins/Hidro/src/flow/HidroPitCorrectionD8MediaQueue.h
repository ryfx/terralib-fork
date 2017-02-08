/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroPitCorrectionD8MediaQueue.h
*
*******************************************************************************
*
* $Rev: 8750 $:
*
* $Author: 
*
* $Date:
*
******************************************************************************/

/*!
	\brief This is the Pit Correction method based on D8 Media and Queue optimization.
	@defgroup hidroFlow The Group for generate flow functions.
*/

#ifndef HIDRO_PIT_CORRECTION_D8_MEDIA_QUEUE_H
#define HIDRO_PIT_CORRECTION_D8_MEDIA_QUEUE_H

#include <string>
#include <vector>

#include <HidroMatrix.hpp>

// Foreword declarations Te
class TeRaster;
class TeDatabase;
class HidroSmartQueue;
class TeRasterParams;

// Foreword declarations Qt
#include <qfile.h>
#include <qtextStream.h>

/** \brief Function used to correct pits from LDD input raster.
  * \param inputRaster    Input parameter, terralib raster attribute, this raster could be any regular grid
  * \param outputRaster   Output parameter, terralib raster attribute, this raster is a flow grid
  * \param lddDirectionsMap  Output raster values
  * \param ldd            Algorithm type for extract flow
  * \return       				True if the output raster was generated correctly and false in other case
*/
bool pitCorrectionD8MediaQueue( TeDatabase* db, TeRasterParams demRasterParams, HidroMatrix<double>& demMatrix, HidroMatrix<unsigned char>& lddMatrix, unsigned char lddDirectionsMap[8], std::string layerName, unsigned int treatmentMax, double boost,  std::string lddName);

// Amplify Vertical Contrast
void amplifyVerticalContrast( HidroMatrix<double>& demMatrix, double verticalContrast = 100.0 );

void growUpRegions( HidroMatrix<double>& demMatrix, HidroMatrix<unsigned char>& lddMatrix );

// Save Partial Results
bool saveParcialResults( TeDatabase* db, TeRasterParams demRasterParams, HidroMatrix<double>& demMatrix, HidroMatrix<unsigned char>& lddMatrix, double verticalContrast, std::string layerName,  std::string lddName,  std::string& demPartialName );

// Find all pits and store in pitQueue excluding boudary region
void initializePitQueue( HidroMatrix<double>& demMatrix, HidroMatrix<unsigned char>& lddMatrix, HidroSmartQueue &pitQueue );

// re-compute the LDD for this cell and neighbors
// and look for new pits
void recomputeNeighborsLDD( HidroMatrix<double>& demMatrix, HidroMatrix<unsigned char>& lddMatrix, HidroSmartQueue &pitQueue, int *pitCoord, unsigned char lddDirectionsMap[8] );

// Compute the altimetria media of neighborhoods
double computeAltimetriaMedia( HidroMatrix<double>& demRaster, int *pitCoord, double altimetriaPit, int maxMediaWindowSize, int &windowSize );

// Compute the new altimetria in the pit with percent method
double percentMethod( HidroMatrix<double>& demRaster, int c, int l, double percent );

// Resolve the hole problem - all neighbor has the same altmetria.
// Only rest holes.
// ___     ___
//    |___|
void holeProblem( HidroMatrix<double>& demRaster, HidroSmartQueue& pitQueue );

/** \brief check if any neighbor is dummy, in other words, if the pit is in boundary region.
  * \param demRaster  Altimetria raster.
  * \param pitCoord   Pit location.
  * \return True if the pit is in boundary region.
*/
bool isBoundaryRegion( HidroMatrix<double>& demMatrix, unsigned int col, unsigned int lin );

/** \brief // check how many neighbors are pit.
  * \param demRaster  Altimetria raster.
  * \param pitCoord   Pit location.
  * \return True if the pit is in boundary region.
*/
bool pitsNeighbors( HidroMatrix<unsigned char>& lddMatrix, unsigned int col, unsigned int lin, int threshold );


/** \brief check if the pit is in the peak.
  * \param demRaster  Altimetria raster.
  * \param pitCoord   Pit location.
  * \return True if the pit is in the peak.
*/
bool isPeak( HidroMatrix<unsigned char>& lddRaster, int *pitCoord, unsigned char lddDirectionsMap[8] );

/** \brief This class is used to save the statistics of queue sizes.
*/
class HidroGenerateQueueStatistics : public TeSingleton<HidroGenerateQueueStatistics>
{
public:
  /** \brief Start the Queue Statistics.
   */
  void start( std::string queueStatisticsFileName, unsigned int initialQueueSize );
  
  /** \brief Update the Queue Statistics file.
   */
  void increment( unsigned int newQueueSize );

  /** \brief Update the Queue Statistics file add a Hole Problem information.
   */
  void informHoleProblem( unsigned int holeProblemNumber, unsigned int newQueueSize );

  /** \brief Finalize the Queue Statistics file.
   */
  void finsh( unsigned int newQueueSize );
  
protected:
  QFile queueStatisticsFile_;
  QTextStream queueStatisticsStream_;
};


class HidroPitCorrectionStatistics : public TeSingleton<HidroPitCorrectionStatistics>
{
public:

  void start( int maxMediaWindowSize, unsigned int treatmentMax, double boost, std::string layerName, unsigned int demRasterColumns, unsigned int demRasterLines  );

  void updateWindowsMeansSizes( int windowSize );

  void print( std::string queueStatisticsFileName );

  // Pits Numbers  
  unsigned int pitsAddNumber_;
  unsigned int pitsResolvedNumber_;
  unsigned int pitQueueResolvedNumber_;
  unsigned int pitUnresolvedNumber_;
  unsigned int lastPitSolvedNumber_;
  unsigned int minPitNumberNumber_;
  unsigned int minPitTreatmentNumber_;
  unsigned int holeProblemNumber_;
  unsigned int interactionsNumber_;
  unsigned int initialQueueSize_;

  unsigned int meanLesserPit_;
  unsigned int meanEqualPit_;  
  unsigned int peakNumber_; 
  unsigned int peakSolvedNumber_;
  unsigned int erroMeanLesserPit_;
  unsigned int errorAny_;
  unsigned int outBoundaryRegionNumber_;

  double boost_;

  std::string layerName_;
  unsigned int demRasterColumns_;
  unsigned int demRasterLines_;
  unsigned int treatmentMax_;

  std::vector<unsigned int> windowsMeansSizes_;
  std::vector<unsigned int> pitsUnsolvedByTreatments_;

protected:  
  QFile pitCorrectionStatisticsFile_;
  QTextStream pitCorrectionStatisticsStream_;
  std::string sartTimeString_;
};

#endif // HIDRO_PIT_CORRECTION_D8_MEDIA_QUEUE_H
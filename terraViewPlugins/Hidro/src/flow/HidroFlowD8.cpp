#include <HidroFlowD8.h>
#include <HidroFlowUtils.h>
#include <HidroVertex.h>
#include <HidroEdge.h>
#include <HidroGraph.h>
#include <HidroPersister.h>
#include <HidroPitCorrectionD8MediaQueue.h>
#include <HidroUtils.h>
#include <HidroMatrix.hpp>
//#include <HidroFix.h>
#include <HidroFixPFS.h>

#include <TePDIMatrix.hpp>
#include <TeDatabase.h>
#include <TeRaster.h>
#include <TeDatabase.h>
#include <TeRasterRemap.h>
#include <TeDecoderMemoryMap.h>
#include <TeLayer.h>
#include <TeImportRaster.h>
#include <TeProjection.h>

#include <TeProgress.h>
#include <TeUtils.h>

#include <qstring.h>
#include <qmessageBox.h>
#include <qfile.h>

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

// Generate statistics of the flow generation
// Comment this to final version of the software
// #define GENERATE_FLOW_STATISTICS

// Geral algorithm
bool generateFlowD8(TeDatabase* db, TeView* view, TeLayer* demRasterLayer, std::string layerName, std::string postProcName, int treatmentMax, double boost, std::string lddName)
{

#ifdef GENERATE_FLOW_STATISTICS
  // Reset statistics of the flow generation
  HidroGenerateFlowStatistics::instance().reset();
#endif

  //  LDD Directions Map
  //  032 | 064 | 128
  //  ----+-----+----
  //  016 | 000 | 001
  //  ----+-----+----
  //  008 | 004 | 002
  // the max value if was multi direction is 1+2+4+8+16+32+64+128 = 255 
  // the value 0 is a pit
  // the value 255 is dummy ??? the same of max value, we will need to change it if want to work if multi-directions
  unsigned char lddDirectionsMap[8];
  lddDirectionsMap[0] = 32;
  lddDirectionsMap[1] = 64;
  lddDirectionsMap[2] = 128;
  lddDirectionsMap[3] = 16;
  lddDirectionsMap[4] = 1;
  lddDirectionsMap[5] = 8;
  lddDirectionsMap[6] = 4;
  lddDirectionsMap[7] = 2;

  // Copy the DEM raster parameters to delete raster from memory.  
  TeRaster* demRaster = demRasterLayer->raster();
  TeRasterParams demRasterParams = demRaster->params();

  int demColumns = demRaster->params().ncols_; // number of columns from input raster
  int demLines = demRaster->params().nlines_; // number of rows from input raster  

  // Use the HidroMatrix to best performance
  // LDD
  HidroMatrix<unsigned char> lddMatrix = HidroMatrix<unsigned char>( demLines, demColumns, demRaster, TeUNSIGNEDCHAR );
      
  // DEM  
  HidroMatrix<double> demMatrix = HidroMatrix<double>( demLines, demColumns, demRaster, TeDOUBLE );  
  
  // Delete DEM raster from memory (free memory).
  //demRasterLayer->deleteRaster();
  //demRaster = 0;  
  
  // Initialize the progress windows
  if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Generating Raster Flow");
		TeProgress::instance()->setTotalSteps(demLines);    
	}

	// For each cell compute the direction,
  // excluding first and last lines and columns
	for (int lin = 1; lin < demLines - 1; lin++)
	{
		for (int col = 1; col < demColumns - 1; col++)
		{      
			if ( !generateCommonLDD( demMatrix, lddMatrix, col, lin, lddDirectionsMap ) )
			{
        QMessageBox::warning(0, "Warning", "Error generate LDD raster.");
				
				return false;
			}      
		}

    // Update the progress windows
		if( TeProgress::instance() )
		{
			TeProgress::instance()->setProgress( lin );
      
      if( TeProgress::instance()->wasCancelled() )
      {
        QMessageBox::warning(0, "Warning", "The operation create LDD was cancelled by the user.");
			  
			  return false;
      }
		}
	}

#ifdef GENERATE_FLOW_STATISTICS
  // Record the statistics of the flow generation in a file before correct the pits.
  std::string generateFlowStatisticsFileName = layerName + "_" + lddName + "_Moving_Means_Stats.txt";
  HidroGenerateFlowStatistics::instance().print( generateFlowStatisticsFileName.c_str() );
#endif

  // Reset the progress windows
	if( TeProgress::instance() )
	{
		TeProgress::instance()->reset();
	}
  
  // Save original LDD in the Database.
  /*
  std::string originalLayerName = layerName + "_" + lddName + "_LDD_original";
  lddMatrix.save( db, originalLayerName, TeUNSIGNEDCHAR );
	/**/

  // Fix the pits!
  HidroFixPFS hidroFixPFS( db, demRasterParams, demMatrix, lddMatrix, lddDirectionsMap, layerName, lddName, treatmentMax, boost );  
  if( !hidroFixPFS.pfs() )
	{
    QMessageBox::warning(0, "Warning", "Error while correct LDD raster.");
		return false;
	}

  // Copy lddMatrix to database
  std::string lddLayerName = layerName + "_" + lddName + "_LDD_revised";
  lddMatrix.save( db, lddLayerName, TeUNSIGNEDCHAR );
  lddMatrix.freeMemory();

  // Copy demMatrix to database
  std::string DEMcorrName = layerName + "_" + lddName + "_DEM_revised";
  demMatrix.save( db, DEMcorrName, TeFLOAT );
  demMatrix.freeMemory();

  // Create pits table
  /*TeLayer* inputLayer = NULL;
	HidroUtils utils( db );
	inputLayer = utils.getLayerByName( lddLayerName );
  if( inputLayer->raster()->init() )
    generatePtsPits( db, view, inputLayer->raster(), layerName, lddName );/**/
    
  return true;
}


// Deterministic 8 (D8) Unidirectional choose by Moving Means algorithm for each cell.
bool generateCommonLDD( HidroMatrix<double>& demMatrix, HidroMatrix<unsigned char>& lddMatrix, int col, int lin, unsigned char lddDirectionsMap[8] )
{
  if( col == 70 && lin == 115 )
    int s = 0;

  // Compute the neighborhood slopes
  double slopeNeighbors[8];
  if( !slopeCompute( demMatrix, col, lin, slopeNeighbors ) ) 
	{
    lddMatrix[lin][col] = LDD_DUMMY; // LDD Dummy value
    return true;
  }

  // find the maximum slope and maximum slope direction (last if there are mutiple directions).
	unsigned char maxSlopeDirection;
	double maxSlope = maxValue( slopeNeighbors , lddDirectionsMap, maxSlopeDirection );

  // if there is a pit
	if( maxSlope <= 0 )
  {
		lddMatrix[lin][col] = 0;
    return true;
  }

  // find direction(s) number of max slope(s)	
	int ndirections = ndirectionCompute( slopeNeighbors, maxSlope );

  // if there are single direction
	if( ndirections == 1 )
  {
		lddMatrix[lin][col] = maxSlopeDirection;
    return true;
  }
  
  //if there are multiples directions choose by Moving Means
  if( ndirections > 1 )
  {
#ifdef GENERATE_FLOW_STATISTICS
    // One more draw
    HidroGenerateFlowStatistics::instance().drawsNumber_++;
#endif

    // check if the cell is in boudary region of LDD ( -2 lines and columns of DEM )
		int currentNeighbor = 0; // used to know what neighbor we are looking			
    double altimetriaMean[8], minmedia;

    // Compute the start and end of window (3x3)
    int startColumn = col - 1;
    int endColumn = col + 2;
    int startLine = lin - 1;
    int endLine = lin + 2;

    // For each neighbor of center cell
		for (int currentNeighborLine = startLine; currentNeighborLine < endLine; currentNeighborLine++)
		{			
      for (int currentNeighborColumn = startColumn; currentNeighborColumn < endColumn; currentNeighborColumn++)
			{
        // its is need because the relation of slopeNeighbors[currentNeighbor] and currentNeighbor.
        if (currentNeighborLine != lin || currentNeighborColumn != col)
        {          
          if( slopeNeighbors[currentNeighbor] == maxSlope )  // only for candidate cells to be maximum slope
					{
            // if the neighbor is boder it's the direction
            if( isBorder( demMatrix, currentNeighborColumn, currentNeighborLine) )
            {
              maxSlopeDirection = lddDirectionsMap[currentNeighbor];
              lddMatrix[lin][col] = maxSlopeDirection;

              #ifdef GENERATE_FLOW_STATISTICS
              // Verify if the draw was resolved
              if( !drawWasSolved( altimetriaMean, minmedia ) )
                HidroGenerateFlowStatistics::instance().drawsSolvedNumber_++;
              #endif

              return true;
            }

            // Compute the altimetria mean 3x3 for each neighbor and store in altimetriaMean
						altimetriaMean[currentNeighbor] = altimetriaMeanCompute(demMatrix, currentNeighborLine, currentNeighborColumn);
					}
					else
          {
						altimetriaMean[currentNeighbor] = TeMAXFLOAT; // dummy value for mean.
          }
					currentNeighbor++;
        }
			}
    }
			
    // find the minimum altimetria mean and this direction (that will be the maxSlopeDirection).
		minmedia = minValue(altimetriaMean, lddDirectionsMap, maxSlopeDirection);

#ifdef GENERATE_FLOW_STATISTICS
    // Verify if the draw was resolved
    if( !drawWasSolved( altimetriaMean, minmedia ) )
      HidroGenerateFlowStatistics::instance().drawsSolvedNumber_++;
#endif
		
    lddMatrix[lin][col] = maxSlopeDirection;
    return true;
	}
  
  return false;
}

bool drawWasSolved( double altimetriaMeans[], double& minMedia )
{
  char equals = 0;

  for( char i=0; i<8; i++ )
  {
    if( altimetriaMeans[i] == minMedia )
      equals++;
  }

  if( equals > 1 )
    return false;

  return false;
}


// Classes Methods

HidroGenerateFlowStatistics::HidroGenerateFlowStatistics()
{}

/**
  \brief Reset the statistics values.  
*/
void HidroGenerateFlowStatistics::reset()
{
  drawsNumber_ = 0;  
  drawsSolvedNumber_ = 0;
}

/**
  \brief Record statistics in a file.
  \param fileName File Name.
*/
void HidroGenerateFlowStatistics::print( std::string fileName )
{
  generateString();

  QFile file( fileName.c_str() );  
  if ( file.open( IO_WriteOnly ) )
  {
    QTextStream stream;    
    stream.setEncoding( QTextStream::Encoding::UnicodeUTF8 );
    stream.setDevice( &file );
    stream << text_.c_str();
    file.close();
  }
}

/**
  \brief Generate the strem with statistics values.
*/
void HidroGenerateFlowStatistics::generateString()
{
  text_.clear();
  text_.append( "Moving Means draws Statistics: \n" );
  text_.append( "Draws Number: " + Te2String( drawsNumber_ ) + "\n" );  
  text_.append( "Draws Solved Number: " + Te2String( drawsSolvedNumber_ ) + "\n" );  
  text_.append( "Draws UnSolved Number: " + Te2String( drawsNumber_ - drawsSolvedNumber_ ) + "\n" );
}

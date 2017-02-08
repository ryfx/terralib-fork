#include <HidroPitCorrectionD8MediaQueue.h>

#include <HidroFlowD8.h>
#include <HidroFlowUtils.h>
#include <HidroMatrix.hpp>
#include <HidroSmartQueue.h>
#include <HidroUtils.h>

#include <TePDIMatrix.hpp>
#include <TeDatabase.h>
#include <TeRaster.h>
#include <TeRasterRemap.h>
#include <TeImportRaster.h>
#include <TeProgress.h>
#include <TeUtils.h>

#include <qstring.h>
#include <qmessageBox.h>
#include <qfile.h>

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

// Generate statistics of the queue size
// Comment this to final version of the software
#define GENERATE_QUEUE_STATISTICS

// Generate statistics of the pit correction
// Comment this to final version of the software
#define GENERATE_PIT_CORRECTION_STATISTICS

// Generate statistics of the flow generation
// Comment this to final version of the software
#define GENERATE_FLOW_STATISTICS

// Generate Windows Sizes Raster
// Comment this to final version of the software
// #define GENERATE_WINDOWS_SIZES_RASTER


bool pitCorrectionD8MediaQueue(TeDatabase* db, TeRasterParams demRasterParams, HidroMatrix<double>& demMatrix, HidroMatrix<unsigned char>& lddMatrix, unsigned char lddDirectionsMap[8], std::string layerName, unsigned int treatmentMax, double boost,  std::string lddName)
{
  int maxMediaWindowSize = 3;

  /*for(int up=0; up<10; up++)
  {
    growUpRegions( demMatrix, lddMatrix );
  }
  /**/

#ifdef GENERATE_WINDOWS_SIZES_RASTER
  HidroMatrix<unsigned char> windowsSizesMatrix = HidroMatrix<unsigned char>( lddMatrix.GetLines(), lddMatrix.GetColumns() );  
  windowsSizesMatrix.setDummy( LDD_DUMMY );
  for (int lin = 0; lin < lddMatrix.GetLines(); lin++)
	{
		for (int col = 0; col < lddMatrix.GetColumns(); col++)
    {
      windowsSizesMatrix[lin][col] = LDD_DUMMY;
    }
  }
#endif // GENERATE_WINDOWS_SIZES_RASTER

  // Time Statistics
  Time::instance().start();

#ifdef GENERATE_FLOW_STATISTICS
  // Reset statistics of the flow generation (Moving Means).
  HidroGenerateFlowStatistics::instance().reset();
#endif // GENERATE_FLOW_STATISTICS

#ifdef GENERATE_PIT_CORRECTION_STATISTICS
  // Initialize the pit correction statistics.  
  HidroPitCorrectionStatistics::instance().start( maxMediaWindowSize, treatmentMax, boost, layerName, demMatrix.GetColumns(), demMatrix.GetLines()  );
#endif // GENERATE_PIT_CORRECTION_STATISTICS
  
  // Amplify Vertical Contrast
  float verticalContrast = 1000.0;
  demMatrix.setVerticalContrast( verticalContrast );  

  // Pit Queue.  
  HidroSmartQueue pitQueue;
       
	// Find all pits and store in pitQueue excluding boudary region
  initializePitQueue( demMatrix, lddMatrix, pitQueue );
  unsigned int queueSize = pitQueue.size(); // progres windows
  unsigned int queueCount = 0; // progres windows
  unsigned int initialQueueSize = queueSize; // progres windows

#ifdef GENERATE_PIT_CORRECTION_STATISTICS
  // Update the pit correction statistics.
  HidroPitCorrectionStatistics::instance().initialQueueSize_ = queueSize;
#endif // GENERATE_PIT_CORRECTION_STATISTICS

#ifdef GENERATE_QUEUE_STATISTICS
  // Initialize the Queue Statistics File
  std::string queueStatisticsFileName = layerName + "_" + lddName + "_Queue_Size_Stats.txt";
  HidroGenerateQueueStatistics::instance().start( queueStatisticsFileName, queueSize );
#endif // GENERATE_QUEUE_STATISTICS

  // Initialize the progress windows  
  if( TeProgress::instance() )
	{
    TeProgress::instance()->reset();
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Correcting pits fom a raster flow");
		TeProgress::instance()->setTotalSteps( queueSize );     
	}  

  // TODO: This layer have to be deleted after a normal execution.
  std::string demPartialName;

  // Restore especial pits (9) to normal pits (0).
  /*unsigned char lddVal;
  for (int lin = 0; lin < lddMatrix.GetLines(); lin++)
	{
		for (int col = 0; col < lddMatrix.GetColumns(); col++)
    {
      if( lddMatrix.getElement( col, lin, lddVal ) )
      {
        if( lddVal == 9 )
        {
          lddMatrix[lin][col] = 0;
        }
      }
    }
  }/**/
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////// Main Loop //////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  while( pitQueue.size() > 0 )  
  {
    // retrieve the line and column of first pit 
    int *pitCoord = pitQueue.front(); // pit Column and Line coordinates.    

    // Increase the treatment number
    pitCoord[2]++;
       
    // get altimetria in the pit
    double altimetriaPit = -1;
    demMatrix.getElement( pitCoord[0],  pitCoord[1], altimetriaPit ); // always have a valid valor.

    // Compute the neighborhood media altimetria.
    int windowSize = 1;
    double altimetriaMean = computeAltimetriaMedia(demMatrix, pitCoord, altimetriaPit, maxMediaWindowSize, windowSize);

#ifdef GENERATE_WINDOWS_SIZES_RASTER
    unsigned char aux;
    if( !windowsSizesMatrix.getElement( pitCoord[0], pitCoord[1], aux) )
      windowsSizesMatrix[pitCoord[1]] [pitCoord[0]] = (unsigned char) windowSize;  
#endif // GENERATE_WINDOWS_SIZES_RASTER

#ifdef GENERATE_PIT_CORRECTION_STATISTICS
    // Update the pit correction statistics.
    HidroPitCorrectionStatistics::instance().updateWindowsMeansSizes( windowSize );

    if( windowSize == 3 && altimetriaMean < altimetriaPit )
    {
      HidroPitCorrectionStatistics::instance().erroMeanLesserPit_++;
    }
#endif // GENERATE_PIT_CORRECTION_STATISTICS
    
    // If the media altimetria is bigger than pit altimetria
    // change the DEM and re-compute the LDD for this cell
    if( altimetriaMean > altimetriaPit )
    {
      // boost the diference bettewn altimetriaMean and altimetriaPit
      double diff = altimetriaMean - altimetriaPit;
      diff = diff * boost;
      altimetriaMean = altimetriaPit + diff;

      // update the DEM Matrix
      demMatrix [pitCoord[1]] [pitCoord[0]] = altimetriaMean ;      
            
      // re-compute the LDD for the neighbors and look for new pits
      recomputeNeighborsLDD( demMatrix, lddMatrix, pitQueue, pitCoord, lddDirectionsMap );

      // re-compute the LDD for this pit
      generateCommonLDD( demMatrix, lddMatrix, pitCoord[0], pitCoord[1], lddDirectionsMap );

      // Check if it is still a pit. Its is necessary because all the neighboor can have the same altimetria.
      // TODO: This justify the treatment number?
      unsigned char lddVal = 0;
      lddMatrix.getElement( pitCoord[0], pitCoord[1], lddVal );
      if( lddVal == 0 )
      {
         pitQueue.sendFront2Back(); // TODO: increase the treatment number?        
      }
      else
      {
        // Pit Resolved
        pitQueue.pop();

#ifdef GENERATE_PIT_CORRECTION_STATISTICS
        HidroPitCorrectionStatistics::instance().pitsResolvedNumber_++;
#endif // GENERATE_PIT_CORRECTION_STATISTICS
      }

#ifdef GENERATE_PIT_CORRECTION_STATISTICS
      // Check any error.
      if( lddVal == LDD_DUMMY )
      {
         HidroPitCorrectionStatistics::instance().errorAny_++;
      }
#endif // GENERATE_PIT_CORRECTION_STATISTICS
    }
    // if !( altimetriaMean >= altimetriaPit ) ...
    else 
    {
#ifdef GENERATE_PIT_CORRECTION_STATISTICS
      // Update the pit correction statistics.
      if( altimetriaMean == altimetriaPit )
        HidroPitCorrectionStatistics::instance().meanEqualPit_++;
      else
        HidroPitCorrectionStatistics::instance().meanLesserPit_++;
#endif // GENERATE_PIT_CORRECTION_STATISTICS

      // resolve pits on peak
      if( isPeak( lddMatrix, pitCoord, lddDirectionsMap) )
      {
#ifdef GENERATE_PIT_CORRECTION_STATISTICS
        HidroPitCorrectionStatistics::instance().peakNumber_++;
#endif // GENERATE_PIT_CORRECTION_STATISTICS

        double altimetriaMean[8], minMedia;
        unsigned char minMediaDirection = 0;
        int currentNeighbor = 0; // used to know what neighbor we are looking

        int startColumn = pitCoord[0] - 1;
        int endColumn = pitCoord[0] +  2;
        int startLine = pitCoord[1] - 1;
        int endLine = pitCoord[1] + 2;

        for (int l = startLine; l < endLine; l++)
			  {				
				  for (int c = startColumn; c < endColumn; c++)
				  {
            // if is not center cell
            if( !(pitCoord[0] == c && pitCoord[1] == l) )
            {
              // Compute the altimetria mean 3x3 for each neighbor and store in altimetriaMean
							altimetriaMean[currentNeighbor] = altimetriaMeanCompute( demMatrix, l, c );
              currentNeighbor++;
            }
          }
        }
        // find the minimum altimetria mean and this direction.
			  minMedia = minValue(altimetriaMean, lddDirectionsMap, minMediaDirection);

        lddMatrix.setElement(pitCoord[0], pitCoord[1], minMediaDirection);

        pitQueue.pop();

#ifdef GENERATE_PIT_CORRECTION_STATISTICS
        HidroPitCorrectionStatistics::instance().pitsResolvedNumber_++;
#endif // GENERATE_PIT_CORRECTION_STATISTICS        
      } 
      else       
      {// window 15x15 (or max window size) and Altimetri Mean lesser than Altimetria Pit and is not Peak (recebe agua) 

        // increase treatment number and move to the end of Queue
        if( pitCoord[2] < treatmentMax ) 
        {
          pitQueue.sendFront2Back(); // send to the end of queue
#ifdef GENERATE_PIT_CORRECTION_STATISTICS
          // Update the pit correction statistics.
          if( pitCoord[2] > pitCoord[3] )
          {              
            HidroPitCorrectionStatistics::instance().pitsUnsolvedByTreatments_[ pitCoord[3] ];
            pitCoord[3]++;
          }
#endif // GENERATE_PIT_CORRECTION_STATISTICS
        }
        else // don't try to resolve this pit nevermore
        {
#ifdef GENERATE_PIT_CORRECTION_STATISTICS
          // Update the pit correction statistics.
          if( pitCoord[2] > pitCoord[3] )
          {              
            HidroPitCorrectionStatistics::instance().pitsUnsolvedByTreatments_[ pitCoord[3] ];
            pitCoord[3]++;
          }
          HidroPitCorrectionStatistics::instance().pitUnresolvedNumber_++;
#endif // GENERATE_PIT_CORRECTION_STATISTICS
          
          pitQueue.pop();
        }
      }
    }
    
#ifdef GENERATE_PIT_CORRECTION_STATISTICS
    // Increase the interaction count
    HidroPitCorrectionStatistics::instance().interactionsNumber_++;
#endif // GENERATE_PIT_CORRECTION_STATISTICS 
    
    queueCount++;

    // One Queue was solved (not included the news pit)
    if( queueCount >= queueSize )
    {
#ifdef GENERATE_PIT_CORRECTION_STATISTICS
      HidroPitCorrectionStatistics::instance().pitQueueResolvedNumber_++;
#endif // GENERATE_PIT_CORRECTION_STATISTICS 

      // Storage the partial results hour by hour
      if( Time::instance().hasPassedOneHouer() )
      {
        // Record the parcial results
        saveParcialResults( db, demRasterParams, demMatrix, lddMatrix, verticalContrast, layerName,  lddName, demPartialName );

#ifdef GENERATE_QUEUE_STATISTICS
        // Update the Queue Statistics File.
        HidroGenerateQueueStatistics::instance().increment( pitQueue.size() );
#endif // GENERATE_QUEUE_STATISTICS
      }     
            
      queueCount = 0;
      queueSize = pitQueue.size();      

      if( TeProgress::instance() )
		  {
        TeProgress::instance()->setProgress( initialQueueSize -  queueSize );
        
        // The user cancel the proccess
        if( TeProgress::instance()->wasCancelled() )
        {          
#ifdef GENERATE_PIT_CORRECTION_STATISTICS
           HidroPitCorrectionStatistics::instance().pitUnresolvedNumber_ += queueSize;
#endif // GENERATE_PIT_CORRECTION_STATISTICS
          QMessageBox::warning(0, "Warning", "The operation fix LDD was cancelled by the user.");
          break; // go out main loop while( pitQueue.size() > 0 )
        }
		  }
    }    
  }  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////// Main Loop End //////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GENERATE_QUEUE_STATISTICS
  // Finalize the Queue Statistis File.
  HidroGenerateQueueStatistics::instance().finsh( pitQueue.size() );
#endif // GENERATE_QUEUE_STATISTICS
  
  if(TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}

  // demMatrix columns and lines its is global to best proccess perform
  unsigned int demRasterColumns = demMatrix.GetColumns();
  unsigned int demRasterLines = demMatrix.GetLines();  

  // Remove Partial results
  remove( demPartialName.c_str() );  

#ifdef GENERATE_PIT_CORRECTION_STATISTICS
  // Save the pit correction statistics in a file.
  std::string pitCorrectionStatisticsFileName = layerName + "_" + lddName + "_Pit_Correction_Stats.txt";
  HidroPitCorrectionStatistics::instance().print( pitCorrectionStatisticsFileName );  
#endif // GENERATE_PIT_CORRECTION_STATISTICS

#ifdef GENERATE_WINDOWS_SIZES_RASTER
  // Back LDD HidroMatrix to TeRaster
  TeRasterParams params2( demRasterParams );
  params2.decoderIdentifier_ = "SMARTMEM";
  params2.mode_ = 'w';
  params2.useDummy_ = true;
  params2.setDummy( LDD_DUMMY ); // Dummy value for unidirection LDD
  params2.setDataType(TeUNSIGNEDCHAR); // we will need to change it if want to work if multi-directions
  params2.vmax_[0] = 17;
  params2.vmin_[0] = 0;
  TeRaster* lddRaster = new TeRaster( params2 );
  // verify if output raster created is valid
  if( !lddRaster->init() )
  {
    QMessageBox::warning(0, "Warning", "Error create LDD raster.");
  	delete lddRaster;
	  return false;
  }
  // Copy the values
  unsigned char lddVal;
  for( int lin = 0; lin < demRasterLines; lin++ )
	{
		for( int col = 0; col < demRasterColumns; col++ )
    {
      if( windowsSizesMatrix.getElement( col, lin, lddVal ) )
      {
        lddRaster->setElement( col, lin, lddVal );
      }
      else
      {
        lddRaster->setElement( col, lin, LDD_DUMMY );
      }
      
    }
  }

  // Save the revised LDD in the Database.
  HidroUtils utils(db);
  std::string lddLayerName = layerName + "_" + lddName + "_Windows_Sizes";
	if(!utils.saveOutputRasterCreatingTheme(lddLayerName, lddRaster, 0))
	{
		QMessageBox::warning(0, "Warning", "Error importing LDD revised raster.");
    lddRaster->clear();
		delete lddRaster;
		return false;
	}
#endif // GENERATE_WINDOWS_SIZES_RASTER
/**/
  return true;
}



bool pitsNeighbors( HidroMatrix<unsigned char>& lddMatrix, unsigned int col, unsigned int lin, int threshold )
{
  // Compute the start and end of window (3x3)
  unsigned int startColumn = col - 1;
  unsigned int endColumn = col +  2;
  unsigned int startLine = lin - 1;
  unsigned int endLine = lin + 2;
  	
  unsigned char lddVal;
  // becouse the center cell is pit
  int pitsNumber = -1;

	for (unsigned int l = startLine; l < endLine; l++)
	{				
		for (unsigned int c = startColumn; c < endColumn; c++)
		{      
      if( lddMatrix.getElement(c, l, lddVal) )
      {
        if( lddVal == 0 || lddVal == 9 )
          pitsNumber++;          
      }      
    }
  }  

  if( pitsNumber < threshold )
    return true;
  else
    return false;
}

// Compute the altimetria media of neighborhoods
double computeAltimetriaMedia( HidroMatrix<double>& demMatrix, int *pitCoord, double altimetriaPit, int maxMediaWindowSize, int &windowSize)
{
  // First try 3x3 window until 15x15 window in +2 step 
  double altimetriaMean = -1;

  // Start to compute the means until "altimetria Mean" > "altimetria Pit" or Windows Size > 15x15    
  do
  {
    // Incremente the next windows size step = +2
    windowSize += 2;    

    double altimetriaSum = 0;
    int naltimetrias = 0; // number of computed altimetrias    

    // Compute the start and end of window
    unsigned int startColumn = pitCoord[0] - (int)( windowSize/2 );
    unsigned int endColumn = pitCoord[0] + (int)( windowSize/2 ) +1;
    unsigned int startLine = pitCoord[1] - (int)( windowSize/2 );
    unsigned int endLine = pitCoord[1] + (int)( windowSize/2 ) + 1;

    // Verify Borders the matrix dont do it
    if( startColumn < 0 )
      startColumn = 0;
    if( endColumn > demMatrix.GetColumns() )
      endColumn = demMatrix.GetColumns();
    if( startLine < 0 )
      startLine = 0;
    if( endLine > demMatrix.GetLines() )
      endLine = demMatrix.GetLines();

    double altimetria;
    for( unsigned int l = startLine; l < endLine; ++l )
    {
      for( unsigned int c = startColumn; c < endColumn; ++c )
      {          
        if( demMatrix.getElement( c, l, altimetria ) )
        {        
          altimetriaSum += altimetria; 
          naltimetrias++;         
        }          
      }
    }

    // Remove the pit altimetria
    // altimetria in the pit is not used to compute the altimetria mean
    altimetriaSum = altimetriaSum - altimetriaPit;
    naltimetrias--;

    // Compute altimetria mean
    altimetriaMean = altimetriaSum / naltimetrias;       
  }
  while( (windowSize < maxMediaWindowSize) && (altimetriaMean <= altimetriaPit) );
  // End to compute the means until "altimetria Mean" < "altimetria Pit" or Windows Size > 15x15

  return altimetriaMean;
}

/** \brief check if any neighbor is dummy, in other words, if the pit is in boundary region.
  * \param demMatrix  Altimetria raster.
  * \param pitCoord   Pit location.
  * \return True if the pit is in boundary region.
*/
bool isBoundaryRegion( HidroMatrix<double>& demMatrix, unsigned int col, unsigned int lin )
{
  if( (col < 2) ||
    (col > (demMatrix.GetColumns()-3)) ||
    (lin < 2) ||
    (lin > (demMatrix.GetLines()-3)) )
  {
    return true;
  }

  if( demMatrix.hasDummy() )
  {
    // Compute the start and end of window (3x3)
    unsigned int startColumn = col - 1;
    unsigned int endColumn = col +  2;
    unsigned int startLine = lin - 1;
    unsigned int endLine = lin + 2;
  	
    double altimetria;

	  for (unsigned int l = startLine; l < endLine; l++)
	  {				
		  for (unsigned int c = startColumn; c < endColumn; c++)
		  {
        if( !demMatrix.getElement(c, l, altimetria) )
        {
          // is dummy.
          return true;
        }
      }
    }
  }

  return false;
}

/** \brief check if the pit is in the peak.
  * \param demMatrix  Altimetria raster.
  * \param pitCoord   Pit location.
  * \return True if the pit is in the peak.
*/
bool isPeak( HidroMatrix<unsigned char>& lddMatrix, int *pitCoord, unsigned char lddDirectionsMap[8] )
{
  unsigned char direction = LDD_DUMMY;

  //right
  lddMatrix.getElement(pitCoord[0] + 1, pitCoord[1], direction);
  if( direction == lddDirectionsMap[3]) // 16
    return false;
  direction = LDD_DUMMY;

  //lower right
  lddMatrix.getElement(pitCoord[0] + 1, pitCoord[1] + 1, direction);
  if( direction == lddDirectionsMap[0]) // 32
    return false;
  direction = LDD_DUMMY;

  //down
  lddMatrix.getElement(pitCoord[0], pitCoord[1] + 1, direction);
  if( direction == lddDirectionsMap[1]) // 64
    return false;
  direction = LDD_DUMMY;

  //lower left
  lddMatrix.getElement(pitCoord[0] - 1, pitCoord[1] + 1, direction);
  if( direction == lddDirectionsMap[2] ) // 128
    return false;
  direction = LDD_DUMMY;

  //left
  lddMatrix.getElement(pitCoord[0] - 1, pitCoord[1], direction);
  if( direction == lddDirectionsMap[4]) // 1
    return false;
  direction = LDD_DUMMY;

  //upper left
  lddMatrix.getElement(pitCoord[0] - 1, pitCoord[1] - 1, direction);
  if( direction == lddDirectionsMap[7]) // 2
    return false;
  direction = LDD_DUMMY;

  //up
  lddMatrix.getElement(pitCoord[0] , pitCoord[1] - 1, direction);
  if( direction == lddDirectionsMap[6]) // 4
    return false;
  direction = LDD_DUMMY;

  //upper right
  lddMatrix.getElement(pitCoord[0] + 1, pitCoord[1] - 1, direction);
  if( direction == lddDirectionsMap[5]) // 8
    return false;

  return true;
}

void holeProblem( HidroMatrix<double>& demMatrix, HidroSmartQueue& pitQueue )
{
  // Only rest holes
  unsigned int holesSize = pitQueue.size();

  for( unsigned int holesCount=0; holesCount<holesSize; holesCount++ )
  {
    // retrieve the line and column of first pit 
    int *pitCoord2 = pitQueue.front(); // pit Column and Line coordinates    

    // Higher Neighbor of the pit
    double higherNeighbor = 0.0;

    // Compute the start and end of window
    int startColumn = pitCoord2[0] - 1;
    int endColumn = pitCoord2[0] + 2;
    int startLine = pitCoord2[1] - 1;
    int endLine = pitCoord2[1] + 2;

    double altimetria;

    for( int l = startLine; l < endLine; ++l )
    {
      for( int c = startColumn; c < endColumn; ++c )
      {        
        if( demMatrix.getElement(c, l, altimetria) )
        {             
          // find the higher neighbor
          if( altimetria > higherNeighbor )
          {
            higherNeighbor = altimetria;
          }
        }          
      }
    }          
    demMatrix[pitCoord2[1]][pitCoord2[0]] = higherNeighbor;
    pitQueue.sendFront2Back(); 
  }      
}

// Compute the new altimetria in the pit with percent method
double percentMethod( HidroMatrix<double>& demMatrix, int c, int l, double percent )
{
  double altimetriaPit;
  demMatrix.getElement( c, l, altimetriaPit );

  // Compute the start and end of window
  int startColumn = c - 1;
  int endColumn = c + 2;
  int startLine = l - 1;
  int endLine = l + 2;

  double maxAltimetria = -TeMAXFLOAT;
  double minAltimetria = TeMAXFLOAT;

  double altimetria;
  for( int l = startLine; l < endLine; ++l )
  {
    for( int c = startColumn; c < endColumn; ++c )
    {        
      if( demMatrix.getElement(c, l, altimetria) )
      {
        if( altimetria > maxAltimetria )
        {
          maxAltimetria = altimetria;
        }
        if( altimetria < minAltimetria && altimetria != altimetriaPit )
        {
          minAltimetria = altimetria;
        }
      }
    }
  }

  /*double diff = ( maxAltimetria - minAltimetria ) / 2;
  return diff*percent + minAltimetria;*/
  return maxAltimetria*percent;
}

void initializePitQueue( HidroMatrix<double>& demMatrix, HidroMatrix<unsigned char>& lddMatrix, HidroSmartQueue &pitQueue )
{
  // demMatrix columns and lines its is global to best proccess perform
  unsigned int demRasterColumns = demMatrix.GetColumns();
  unsigned int demRasterLines = demMatrix.GetLines();

  unsigned char lddVal;

  for( unsigned int lin = 2; lin < demRasterLines-2; ++lin )
	{
		for( unsigned int col = 2; col < demRasterColumns-2; ++col )
    {			
			if( lddMatrix.getElement( col, lin, lddVal ) )
      {      
        // Is a pit
		    if( lddVal == 0 )
        {
          // check if any neighbor is dummy, in other words, the pit is in boundary region.
          if( !isBoundaryRegion( demMatrix, col, lin ) )
          {
            // check how many neighbors are pit. If all neighbors are pit don't add.
            //if( pitsNeighbors( lddMatrix, col, lin, 4 ) )
            //{
              int *pitCoord = pitQueue.newPit();
              pitCoord[0] = col;
              pitCoord[1] = lin;
              pitCoord[2] = 0; // treatment number          
              pitCoord[3] = 0; // Qual tratamento me add?                      
              pitQueue.push( pitCoord );
            //}
          }          
        }
      }
		}
	}
}

void recomputeNeighborsLDD( HidroMatrix<double>& demMatrix, HidroMatrix<unsigned char>& lddMatrix, HidroSmartQueue &pitQueue, int *pitCoord, unsigned char lddDirectionsMap[8] )
{
  unsigned char lddVal;
  unsigned char oldLDDVal = 0;

  // Compute the start and end of window (3x3)
  unsigned int startColumn = pitCoord[0] - 1;
  unsigned int endColumn = pitCoord[0] +  2;
  unsigned int startLine = pitCoord[1] - 1;
  unsigned int endLine = pitCoord[1] + 2;  
	
	for (unsigned int l = startLine; l < endLine; l++)
	{				
		for (unsigned int c = startColumn; c < endColumn; c++)
		{
      // Used to now if it was a pit
      lddMatrix.getElement( c, l, oldLDDVal );      
      
      // Recompute the LDD for cell
      if( oldLDDVal != 0 )
      {
			  generateCommonLDD( demMatrix, lddMatrix, c, l, lddDirectionsMap );
  			
        // Check if it's a new pit          
			  if( lddMatrix.getElement( c, l, lddVal ) ) 
        {
          if( lddVal == 0 )
          {
            // check if any neighbor is dummy, in other words, the pit is in boundary region.
            if( !isBoundaryRegion( demMatrix, c, l ) )
            {
              // New Pit
              int *pitCoordNew = pitQueue.newPit();
              pitCoordNew[0] = c;
              pitCoordNew[1] = l;
              pitCoordNew[2] = 0;
              pitCoordNew[3] = pitCoord[3]; // Qual tratamento me add?            
              pitQueue.push( pitCoordNew );

#ifdef GENERATE_PIT_CORRECTION_STATISTICS
              HidroPitCorrectionStatistics::instance().pitsAddNumber_++;
#endif // GENERATE_PIT_CORRECTION_STATISTICS              
            }
#ifdef GENERATE_PIT_CORRECTION_STATISTICS
            else
              HidroPitCorrectionStatistics::instance().outBoundaryRegionNumber_++;
#endif // GENERATE_PIT_CORRECTION_STATISTICS                          
          }
        }
      }
		}				
	}
}

bool saveParcialResults( TeDatabase* db, TeRasterParams demRasterParams, HidroMatrix<double>& demMatrix, HidroMatrix<unsigned char>& lddMatrix, double verticalContrast, std::string layerName,  std::string lddName,  std::string& demPartialName )
{
  // DEM Raster Partial File Name
  demPartialName = layerName + "_" + lddName + "_DEM_partial.tif";

  // Create a TeRaster
  TeRasterParams params = demRasterParams;
  params.fileName_ = demPartialName;
  params.mode_ = 'c';
  params.decoderIdentifier_ = "TIF";  
  params.setDataType( TeFLOAT );
  TeRaster* demRasterPartial = new TeRaster(params);
  if( !demRasterPartial->init() )
  {
    QMessageBox::warning(0, "Warning", "Error while create DEM partial raster.");

    delete demRasterPartial;
    return NULL;
  }
  // back the amplify vertical contrast and recomput min and max
  double altimetriaVal;
  unsigned int demRasterColumns = demMatrix.GetColumns();
  unsigned int demRasterLines = demMatrix.GetLines();  
  double vmin = TeMAXFLOAT;
  double vmax = -TeMAXFLOAT;  
  for( unsigned int lin = 0; lin < demRasterLines; ++lin )
	{
		for( unsigned int col = 0; col < demRasterColumns; ++col )
    {			
			if( demMatrix.getElement(col, lin, altimetriaVal) )
      {
        double val = altimetriaVal / verticalContrast;
        if(val > vmax) vmax = val;
        if(val < vmin) vmin = val;
        demRasterPartial->setElement( col, lin, val );
      }
    }
  }
  demRasterPartial->params().vmax_[0] = vmax;
  demRasterPartial->params().vmin_[0] = vmin;

  delete demRasterPartial;
  return true;

  /*
  // demMatrix columns and lines its is global to best proccess perform
  unsigned int demRasterColumns = demMatrix.GetColumns();
  unsigned int demRasterLines = demMatrix.GetLines();  

  // First the DEM
  double altimetriaVal;

  // Create a TeRaster
  TeRasterParams params = demRaster->params();
  params.mode_ = 'w';
  params.decoderIdentifier_ = "SMARTMEM";  
  params.setDataType( TeFLOAT );
  TeRaster* demRasterPartial = new TeRaster(params);
  if(!demRasterPartial->init())
  {
    QMessageBox::warning(0, "Warning", "Error while create DEM revised raster.");

    delete demRasterPartial;
    return NULL;
  }
  // back the amplify vertical contrast and recomput min and max
  double vmin = TeMAXFLOAT;
  double vmax = -TeMAXFLOAT;  
  for( unsigned int lin = 0; lin < demRasterLines; ++lin )
	{
		for( unsigned int col = 0; col < demRasterColumns; ++col )
    {			
			if( demMatrix.getElement(col, lin, altimetriaVal) )
      {
        double val = altimetriaVal / verticalContrast;
        if(val > vmax) vmax = val;
        if(val < vmin) vmin = val;
        demRasterPartial->setElement( col, lin, val );
      }
    }
  }
  demRasterPartial->params().vmax_[0] = vmax;
  demRasterPartial->params().vmin_[0] = vmin;

  // Save the Partial Raster in the Database
	std::string DEMPartialName = layerName + "_" + lddName + "_DEM_partial";

  // Check if Layer existis
  if( db->layerExist(DEMPartialName) )
  {    
  }

	TeLayer* layer = new TeLayer( DEMPartialName, db, demRasterPartial->projection() );
	if (layer->id() <= 0)
  {
		delete layer;
		delete demRasterPartial;
		return false;
	}

  unsigned int blockW = demRasterPartial->params().ncols_;
	unsigned int blockH = demRasterPartial->params().nlines_;
  if (demRasterPartial->params().ncols_ > 512)
	{
		blockW = 512;	
	}
	if (demRasterPartial->params().nlines_ > 512)
  {
    blockH = 512;
  }  
  
	if (!TeImportRaster(layer, demRasterPartial, blockW, blockH, demRasterPartial->params().compression_[0], 
		"", demRasterPartial->params().dummy_[0], demRasterPartial->params().useDummy_, demRasterPartial->params().tiling_type_))
  {
		delete layer;
		delete demRasterPartial;
		return false;
	}

	delete demRasterPartial;  
  return true;*/
}


void amplifyVerticalContrast( HidroMatrix<double>& demMatrix, double verticalContrast )
{
  // demMatrix columns and lines its is global to best proccess perform
  unsigned int demMatrixColumns = demMatrix.GetColumns();
  unsigned int demMatrixLines = demMatrix.GetLines();  
  
  double altimetriaVal = 0;
  for( unsigned int lin = 0; lin < demMatrixLines; ++lin )
	{
		for( unsigned int col = 0; col < demMatrixColumns; ++col )
    {			
			if( demMatrix.getElement(col, lin, altimetriaVal) )
      {        
        demMatrix[lin][col] = ( altimetriaVal * verticalContrast ) ;
      }
    }
  }
}



// Class HidroGenerateQueueStatistics
void HidroGenerateQueueStatistics::start( std::string queueStatisticsFileName, unsigned int initialQueueSize )
{
  queueStatisticsStream_.reset();

  queueStatisticsFile_.setName( queueStatisticsFileName.c_str() );
  if( queueStatisticsFile_.open( IO_ReadWrite  ) )
  {
    queueStatisticsStream_.setDevice( &queueStatisticsFile_ );

    queueStatisticsStream_ << "Start Time: " << Time::instance().actualTimeString().c_str();
    queueStatisticsStream_ << "Queue size: " << Te2String( initialQueueSize ).c_str() << "\n\n";  
    queueStatisticsFile_.flush();
  }
}

void HidroGenerateQueueStatistics::increment( unsigned int newQueueSize )
{
  if( queueStatisticsFile_.isOpen() )
  {
    queueStatisticsStream_ << "Actual Time: " << Time::instance().actualTimeString().c_str();
    queueStatisticsStream_ << "Queue size: " << Te2String( newQueueSize ).c_str() << "\n";
    queueStatisticsStream_ << "Processing time: " << Time::instance().partialString().c_str() << "\n\n";
    queueStatisticsFile_.flush();
  }
}

void HidroGenerateQueueStatistics::finsh( unsigned int newQueueSize )
{
  if( queueStatisticsFile_.isOpen() )
  {
    queueStatisticsStream_ << "End Time: " << Time::instance().actualTimeString().c_str();
    queueStatisticsStream_ << "Queue size: " << Te2String( newQueueSize ).c_str() << "\n";
    queueStatisticsStream_ << "Total Processing time: " << Time::instance().partialString().c_str();
    queueStatisticsFile_.close();
  }
}

void HidroGenerateQueueStatistics::informHoleProblem( unsigned int holeProblemNumber, unsigned int newQueueSize )
{
  if( queueStatisticsFile_.isOpen() )
  {
    queueStatisticsStream_ << "Actual Time: " << Time::instance().actualTimeString().c_str();
    queueStatisticsStream_ << "hole Problem : " << Te2String( holeProblemNumber ).c_str() << "\n";
    queueStatisticsStream_ << "Queue size: " << Te2String( newQueueSize ).c_str() << "\n";
    queueStatisticsStream_ << "Processing time: " << Time::instance().partialString().c_str() << "\n\n";
    queueStatisticsFile_.flush();
  }
}




void
HidroPitCorrectionStatistics::start( int maxMediaWindowSize, unsigned int treatmentMax, double boost, std::string layerName, unsigned int demRasterColumns, unsigned int demRasterLines  )
{
  boost_ = boost;
  pitsAddNumber_ = 0;
  pitsResolvedNumber_ = 0;
  pitQueueResolvedNumber_ = 0;
  pitUnresolvedNumber_ = 0;
  lastPitSolvedNumber_ = 0;
  minPitNumberNumber_ = 0;
  minPitTreatmentNumber_ = 0;
  holeProblemNumber_ = 0;
  interactionsNumber_ = 0;
  initialQueueSize_ = 0;

  meanLesserPit_ = 0;
  meanEqualPit_ = 0;
  erroMeanLesserPit_ = 0;
  errorAny_ = 0;
  outBoundaryRegionNumber_ = 0;

  peakNumber_ = 0; 
  peakSolvedNumber_ = 0;

  windowsMeansSizes_.clear();
  for( int i=0; i<(maxMediaWindowSize/2) + 1 ; i++)
  {
    windowsMeansSizes_.push_back(0);
  }

  // Remain pits for each treatment number  
  pitsUnsolvedByTreatments_.clear();
  for( unsigned int ii=0; ii<treatmentMax; ii++)
  {
    pitsUnsolvedByTreatments_.push_back( 0 );
  }

  layerName_ = layerName;

  demRasterColumns_ = demRasterColumns;
  demRasterLines_ = demRasterLines;
  treatmentMax_ = treatmentMax;  

  sartTimeString_ = Time::instance().actualTimeString();
}

  void HidroPitCorrectionStatistics::updateWindowsMeansSizes( int windowSize )
  {
    windowsMeansSizes_[ ( windowSize/2 ) -1 ]++;
  }

  void HidroPitCorrectionStatistics::print( std::string queueStatisticsFileName )
  {
    pitCorrectionStatisticsStream_.reset();
    pitCorrectionStatisticsFile_.setName( queueStatisticsFileName.c_str() );
    if( pitCorrectionStatisticsFile_.open( IO_ReadWrite  ) )
    {
      pitCorrectionStatisticsStream_.setDevice( &pitCorrectionStatisticsFile_ );
      pitCorrectionStatisticsStream_ << "Start Time: " << sartTimeString_.c_str();
      pitCorrectionStatisticsStream_ << "End Time: " << Time::instance().actualTimeString().c_str() << "\n";

      std::string information;
      information.append( layerName_.c_str() );
      information.append("\n");
      information.append("Image size: ");
      information.append( Te2String(demRasterColumns_).c_str() );
      information.append(", ");
      information.append( Te2String(demRasterLines_).c_str() );
      information.append("\n");
      
      /*information.append("LDD name: ");
      information.append( lddName_.c_str() );
      information.append("\n");
      information.append("\n");*/

      information.append("Initial pit number: ");
      information.append( Te2String(initialQueueSize_).c_str() );
      information.append("\n");

      information.append("Pit unsolved number (outside border): ");
      information.append( Te2String(pitUnresolvedNumber_).c_str() );
      information.append("\n");  

      information.append("Pit Max Treatment: ");  
      information.append( Te2String(treatmentMax_).c_str() );
      information.append("\n");

      information.append("Boost: ");  
      information.append( Te2String(boost_, 4).c_str() );
      information.append("\n");

      /*information.append("SmarQueue Max Memory: ");  
      information.append( Te2String(pitQueue.getMaxMem()/1024/1024).c_str() );  
      information.append(" Mb.");
      information.append("\n");*/  

      information.append("Processing time: ");  
      information.append( Time::instance().partialString().c_str() );
      information.append("\n");
      information.append("\n");

      information.append("Max Media Window Size: ");  
      information.append( Te2String(windowsMeansSizes_.size()*2-1).c_str() );  
      information.append("\n");

      information.append("Pit added number: ");
      information.append( Te2String(pitsAddNumber_).c_str() );
      information.append("\n");

      information.append("Pit solved number: ");
      information.append( Te2String(pitsResolvedNumber_).c_str() );
      information.append("\n");

      information.append("PitQueue solved number: ");
      information.append( Te2String(pitQueueResolvedNumber_).c_str() );
      information.append("\n");

      information.append("Total Steps: ");
      information.append( Te2String(interactionsNumber_).c_str() );
      information.append("\n");

      information.append("holes Problems: ");
      information.append( Te2String(holeProblemNumber_).c_str() );
      information.append("\n");

      information.append("Altimetria Mean Equals than Altimetria Pit: ");
      information.append( Te2String(meanEqualPit_).c_str() );
      information.append("\n");

      information.append("Altimetria Mean Lesser than Altimetria Pit: ");
      information.append( Te2String(meanLesserPit_).c_str() );
      information.append("\n");      

      information.append("Peaks Number: ");
      information.append( Te2String(peakNumber_).c_str() );
      information.append("\n");

      /*information.append("Peaks Number Solved: ");
      information.append( Te2String(peakSolvedNumber_).c_str() );
      information.append("\n");*/

      information.append("Window 3x3 and Mean lesser than Pit errors: ");
      information.append( Te2String(erroMeanLesserPit_).c_str() );
      information.append("\n");

      information.append("Any errors: ");
      information.append( Te2String(errorAny_).c_str() );
      information.append("\n");

      information.append("New pit On Boundary Region Number: ");
      information.append( Te2String(outBoundaryRegionNumber_).c_str() );
      information.append("\n");      

      information.append("\n");      
      int aux = 3;
      for( int ii=0; ii<windowsMeansSizes_.size() ; ii++)
      {
        information.append("Media Windows Size ");
        information.append( Te2String(aux).c_str() );
        information.append( " : ");  
        information.append( Te2String(windowsMeansSizes_[ii]).c_str() );  
        information.append("\n");
        aux+=2;
      }
      information.append("\n");

      // Moving Means Statistics
      HidroGenerateFlowStatistics::instance().generateString();
      information.append( HidroGenerateFlowStatistics::instance().text_.c_str() );
      information.append("\n");

      minPitNumberNumber_ = initialQueueSize_;
      for( unsigned int jj=0; jj<pitsUnsolvedByTreatments_.size(); jj++)
      {
        information.append("Pits unsolved after ");
        information.append( Te2String(jj+1).c_str() );
        information.append(" treatment: ");
        information.append( Te2String(pitsUnsolvedByTreatments_[jj]).c_str() );  
        information.append("\n");

        if( minPitNumberNumber_ > pitsUnsolvedByTreatments_[jj] )
        {
          minPitNumberNumber_ = pitsUnsolvedByTreatments_[jj];
          minPitTreatmentNumber_ = jj+1;
        }
      }

      information.append("\n");
      information.append("Minimum pits number: ");  
      information.append( Te2String(minPitNumberNumber_).c_str() );
      information.append(" with ");
      information.append( Te2String(minPitTreatmentNumber_).c_str() );
      information.append(" treatments.");
      information.append("\n");

      /*information.append("No more pits solved after: ");  
      information.append( Te2String(lastPitSolved).c_str() );
      information.append(" treatments.");
      information.append("\n");*/

      pitCorrectionStatisticsStream_ << information.c_str();

      pitCorrectionStatisticsFile_.close();
    }
  }

  void growUpRegions( HidroMatrix<double>& demMatrix, HidroMatrix<unsigned char>& lddMatrix )
{
  unsigned char lddVal;
  unsigned char lddNeighborVal;
  double altimetria;
  double altimetriaPit;  

  for (int lin = 2; lin < lddMatrix.GetLines()-2; lin++)
	{
		for (int col = 2; col < lddMatrix.GetColumns()-2; col++)
    {
      if( lddMatrix.getElement( col, lin, lddVal ) )
      {
        if( lddVal == 0 || lddVal == 9 )
        {
          if( !pitsNeighbors( lddMatrix, col, lin, 7 ) )
          {
            demMatrix.getElement( col, lin, altimetriaPit );

            // Compute the start and end of window (5x5)
            unsigned int startColumn = col - 2;
            unsigned int endColumn = col +  3;
            unsigned int startLine = lin - 2;
            unsigned int endLine = lin + 3;          

            for( unsigned int l = startLine; l < endLine; l++ )
            {				
	            for( unsigned int c = startColumn; c < endColumn; c++ )
	            {      
                if( demMatrix.getElement(c, l, altimetria) )
                {
                  if( altimetriaPit == altimetria )
                  {                  
                    lddMatrix[l][c] = 9;
                  }                  
                }
                /*if( lddMatrix.getElement(c, l, lddNeighborVal) )
                {
                  if( lddNeighborVal == 0 )
                  {                  
                    lddMatrix[l][c] = 9;
                  }                  
                }*/
              }
            }
          }
        }
      }
    }
  }
}





  /*/ Resolve the hole problem - all neighbor has the same altmetria.
    // Only rest holes.
    // ___     ___
    //    |___|
    if( queuePitSolved == false )
    {
      holeProblemNumber++;
#ifdef GENERATE_QUEUE_STATISTICS
      // Update the Queue Statistics File informing a Hole Problem.
      HidroGenerateQueueStatistics::instance().informHoleProblem( holeProblemNumber, pitQueue.size() );
#endif // GENERATE_QUEUE_STATISTICS

      holeProblem( demMatrix, pitQueue );
      queuePitSolved = true;
      pitSolved = false;  
    }/* Hole Problem End */
/*
  // This variables are used to find the last pit solved  
  bool pitSolved = false;
  bool queuePitSolved = true; 
  unsigned int lastPitSolved = 0;

  if(pitSolved == false)
      {
        queuePitSolved = false;
      }

      pitSolved = false;

  */
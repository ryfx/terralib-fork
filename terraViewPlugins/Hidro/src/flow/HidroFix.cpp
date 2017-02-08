#include <HidroFix.h>

// Hidro Includes
#include <HidroFlowD8.h>
#include <HidroFlowUtils.h>

// TODO: create new .h and .cpp to HidroGenerateQueueStatistics
#include <HidroPitCorrectionD8MediaQueue.h> 

// TerraLib Includes
#include <TeAttribute.h>
#include <TeUtils.h>

// Qt Includes
#include <QmessageBox.h>

int
HidroFix::ldd()
{
  int pitsNumber = 0;
  unsigned char lddVal;

  for( unsigned int line = 1; line < rasterLines_-1; ++line )
	{    
		for( unsigned int column = 1; column < rasterColumns_-1; ++column )
    {
      generateCommonLDD( demMatrix_, lddMatrix_, column, line, lddDirectionsMap_ );
      if( lddMatrix_.getElement( column, line, lddVal ) )
      {
        if( lddVal == 0 )
        {
          ++pitsNumber;
        }
      }
    }
  }

  return pitsNumber;
}

void
HidroFix::backLDD( unsigned int x, unsigned int y, unsigned char reLDDVal )
{
  // Pit
  if( reLDDVal == 0 )
  {
    lddMatrix_[y-1][x-1] = 2;
    lddMatrix_[y-1][x] = 4;
    lddMatrix_[y-1][x+1] = 8;
    
    lddMatrix_[y][x-1] = 1;
    lddMatrix_[y][x] = 0;
    lddMatrix_[y][x+1] = 16;

    lddMatrix_[y+1][x-1] = 128;
    lddMatrix_[y+1][x] = 64;
    lddMatrix_[y+1][x+1] = 32;
  }


  // right (1)
  if( reLDDVal == 1 )
  {
    lddMatrix_[y-1][x-1] = 2;
    lddMatrix_[y-1][x] = 2;
    lddMatrix_[y-1][x+1] = 2;
    
    lddMatrix_[y][x-1] = 1;
    lddMatrix_[y][x] = 1;
    lddMatrix_[y][x+1] = 1;

    lddMatrix_[y+1][x-1] = 128;
    lddMatrix_[y+1][x] = 128;
    lddMatrix_[y+1][x+1] = 128;
  }

  // down-right
  if( reLDDVal == 2 )
  {
    lddMatrix_[y-1][x-1] = 2;
    lddMatrix_[y-1][x] = 4;
    lddMatrix_[y-1][x+1] = 4;
    
    lddMatrix_[y][x-1] = 1;
    lddMatrix_[y][x] = 2;
    lddMatrix_[y][x+1] = 4;

    lddMatrix_[y+1][x-1] = 1;
    lddMatrix_[y+1][x] = 1;
    lddMatrix_[y+1][x+1] = 2;
  }

  // down
  if( reLDDVal == 4 )
  {
    lddMatrix_[y-1][x-1] = 2;
    lddMatrix_[y-1][x] = 4;
    lddMatrix_[y-1][x+1] = 8;
    
    lddMatrix_[y][x-1] = 2;
    lddMatrix_[y][x] = 4;
    lddMatrix_[y][x+1] = 8;

    lddMatrix_[y+1][x-1] = 2;
    lddMatrix_[y+1][x] = 4;
    lddMatrix_[y+1][x+1] = 8;
  }

  // down-left
  if( reLDDVal == 8 )
  {
    lddMatrix_[y-1][x-1] = 4;
    lddMatrix_[y-1][x] = 4;
    lddMatrix_[y-1][x+1] = 8;
    
    lddMatrix_[y][x-1] = 4;
    lddMatrix_[y][x] = 8;
    lddMatrix_[y][x+1] = 16;

    lddMatrix_[y+1][x-1] = 8;
    lddMatrix_[y+1][x] = 16;
    lddMatrix_[y+1][x+1] = 16;
  }

  // left
  if( reLDDVal == 16 )
  {
    lddMatrix_[y-1][x-1] = 8;
    lddMatrix_[y-1][x] = 8;
    lddMatrix_[y-1][x+1] = 8;
    
    lddMatrix_[y][x-1] = 16;
    lddMatrix_[y][x] = 16;
    lddMatrix_[y][x+1] = 16;

    lddMatrix_[y+1][x-1] = 32;
    lddMatrix_[y+1][x] = 32;
    lddMatrix_[y+1][x+1] = 32;
  }

  // up-left
  if( reLDDVal == 32 )
  {
    lddMatrix_[y-1][x-1] = 32;
    lddMatrix_[y-1][x] = 16;
    lddMatrix_[y-1][x+1] = 16;
    
    lddMatrix_[y][x-1] = 64;
    lddMatrix_[y][x] = 32;
    lddMatrix_[y][x+1] = 16;

    lddMatrix_[y+1][x-1] = 64;
    lddMatrix_[y+1][x] = 64;
    lddMatrix_[y+1][x+1] = 32;
  }

  // up
  if( reLDDVal == 64 )
  {
    lddMatrix_[y-1][x-1] = 128;
    lddMatrix_[y-1][x] = 64;
    lddMatrix_[y-1][x+1] = 32;
    
    lddMatrix_[y][x-1] = 128;
    lddMatrix_[y][x] = 64;
    lddMatrix_[y][x+1] = 32;

    lddMatrix_[y+1][x-1] = 128;
    lddMatrix_[y+1][x] = 64;
    lddMatrix_[y+1][x+1] = 32;
  }

  // up-right
  if( reLDDVal == 128 )
  {
    lddMatrix_[y-1][x-1] = 1;
    lddMatrix_[y-1][x] = 1;
    lddMatrix_[y-1][x+1] = 128;
    
    lddMatrix_[y][x-1] = 1;
    lddMatrix_[y][x] = 128;
    lddMatrix_[y][x+1] = 64;

    lddMatrix_[y+1][x-1] = 128;
    lddMatrix_[y+1][x] = 64;
    lddMatrix_[y+1][x+1] = 64;
  }
}

void
HidroFix::resampleLDD( int level )
{
  // Restart original LDD matrix
  lddMatrix_.Reset( rasterLines_, rasterColumns_, TePDIMatrix<unsigned char>::AutoMemPol );
  lddMatrix_.rasterParams = rasterParams_;  
  lddMatrix_.rasterParams.setDataType( TeUNSIGNEDCHAR );
  lddMatrix_.setDummy( LDD_DUMMY );
  for( unsigned int line = 0; line < rasterLines_; ++line )
	{    
		for( unsigned int column = 0; column < rasterColumns_; ++column )
    {
      lddMatrix_[line][column] = LDD_DUMMY;
    }
  }

  unsigned int reLines = resampleLDD_.GetLines();
  unsigned int reColumns = resampleLDD_.GetColumns();
  unsigned char reLDDVal;

  unsigned int l = 4;
  unsigned int c = 4;
  for( unsigned int line = 1; line < reLines-1; ++line )
	{
    unsigned int c = 4;
		for( unsigned int column = 1; column < reColumns-1; ++column )
    {
      if( resampleLDD_.getElement( column, line, reLDDVal ) )
      {
        backLDD( c, l, reLDDVal );
      }
      c+=3;
    }
    l+=3;
  }
}

void
HidroFix::resample( int level )
{
  // DEM
  unsigned int windowSize = (2 * level) + 1;

  unsigned int newLines;
  unsigned int newColumns;
  newLines = ((float)rasterLines_ / (float)windowSize) + 0.9999; 
  newColumns = ((float)rasterColumns_ / (float)windowSize)  + 0.9999;

  resampleDEM_.Reset( newLines, newColumns, TePDIMatrix<double>::AutoMemPol );

  double demDummy = -TeMAXFLOAT;
  if( rasterParams_.useDummy_ )
  {
    demDummy = rasterParams_.dummy_[0];
    resampleDEM_.setDummy( demDummy );
  }
  unsigned int l = level;
  unsigned int c = level;
  for( unsigned int line = 0; line < newLines; ++line )
	{
    c = level;
		for( unsigned int column = 0; column < newColumns; ++column )
    {
      double mean = meanNeigbhors( c, l, windowSize, true );
      if( mean > 0 )
      {
        resampleDEM_[line][column] = mean;
      }
      else
      {
        resampleDEM_[line][column] = demDummy;
      }
      c+=windowSize;
    }
    l+=windowSize;
  }
  resampleDEM_.rasterParams = rasterParams_;  
  double x1 = rasterParams_.box().x1();
  double x2 = rasterParams_.box().x2();
  double y1 = rasterParams_.box().y1();
  double y2 = rasterParams_.box().y2();

  double dx = (x2-x1)/rasterColumns_;
  double dy = (y2-y1)/rasterLines_;

  x1 = x1 + dx;
  y2 = y2 - dy;

  y1 = y1 - dy * ((newLines * windowSize) - rasterLines_ );
  //x2 = x2 + dx * (newColumns * windowSize - rasterColumns_ );  

  resampleDEM_.rasterParams.boxLinesColumns( x1, y1, x2, y2, newLines, newColumns );
  resampleDEM_.rasterParams.setDataType( TeFLOAT );    

  // LDD
  resampleLDD_.Reset( newLines, newColumns, TePDIMatrix<unsigned char>::AutoMemPol );
  resampleLDD_.rasterParams = resampleDEM_.rasterParams;  
  resampleLDD_.rasterParams.setDataType( TeUNSIGNEDCHAR );
  resampleLDD_.setDummy( LDD_DUMMY );
  for( unsigned int line = 0; line < newLines; ++line )
	{    
		for( unsigned int column = 0; column < newColumns; ++column )
    {
      resampleLDD_[line][column] = LDD_DUMMY;
    }
  }

  // Generate LDD
  for( unsigned int line = 1; line < newLines-1; ++line )
	{    
		for( unsigned int column = 1; column < newColumns-1; ++column )
    {
      generateCommonLDD( resampleDEM_, resampleLDD_, column, line, lddDirectionsMap_ );
    }
  }
}

bool
HidroFix::run3()
{
  recursive( pitMaxTreatment_, 0 );

  std::string lddLayerName = demName_ + "_" + lddName_ + "_LDD";
  lddMatrix_.save( database_, lddLayerName, TeUNSIGNEDCHAR );
  lddMatrix_.freeMemory();

  return true;
}

bool
HidroFix::recursive( int maxLevel, int level )
{
  int pitRemain = ldd();

  if( pitRemain > 0 && level < maxLevel )
  {
    level++;

    lddMatrix_.freeMemory();
    resample( 1 );
    demMatrix_.freeMemory();

    HidroFix hidroFix( database_, resampleDEM_.rasterParams, resampleDEM_, resampleLDD_, lddDirectionsMap_, demName_, lddName_, pitMaxTreatment_, boost_ );
    hidroFix.recursive( maxLevel, level );

    resampleLDD( 1 );

    // Copy lddMatrix to database
    //std::string lddLayerName = demName_ + "_" + lddName_ + "_LDD_resample_" + Te2String(level);
    //resampleLDD_.save( database_, lddLayerName, TeUNSIGNEDCHAR );
    //resampleLDD_.freeMemory();
  }

  return true;
}

bool
HidroFix::run2()
{
  lddMatrix_.freeMemory();

  resample( 1 );

  demMatrix_.freeMemory();  

  HidroFix hidroFix( database_, resampleDEM_.rasterParams, resampleDEM_, resampleLDD_, lddDirectionsMap_, demName_, lddName_, pitMaxTreatment_, boost_ );
  //hidroFix.run();
  hidroFix.ldd();

  // Copy demMatrix to database
  std::string DEMcorrName = demName_ + "_" + lddName_ + "_DEM_resample";
  resampleDEM_.save( database_, DEMcorrName, TeFLOAT );
  resampleDEM_.freeMemory();

  resampleLDD( 1 );

  // Copy lddMatrix to database
  std::string lddLayerName = demName_ + "_" + lddName_ + "_LDD_resample";
  resampleLDD_.save( database_, lddLayerName, TeUNSIGNEDCHAR );
  resampleLDD_.freeMemory();

  lddLayerName = demName_ + "_" + lddName_ + "_LDD";
  lddMatrix_.save( database_, lddLayerName, TeUNSIGNEDCHAR );
  lddMatrix_.freeMemory();  

  return true;
}

bool
HidroFix::run()
{
  // Time Statistics
  Time::instance().start();
  starTime_ = Time::instance().actualTimeString();

  // Initialize the pits queue  
  initPitQueue();
  //initPitQueueOrdered();
  HidroGenerateQueueStatistics::instance().start( queueStatisticsFileName_, initialSize_ );  
  
  // Amplify the vertical contrasct for the DEM
  demMatrix_.setVerticalContrast( 100 );  

  // Progress Bar
  TeProgress::instance()->reset();
	TeProgress::instance()->setCaption("TerraHidro");
	TeProgress::instance()->setMessage("Correcting pits fom a raster flow.");
	TeProgress::instance()->setTotalSteps( initialSize_ ); 
  
  // Solve the pits queue. Main Loop.
  solveQueue();
  
  // Finishi progress bar and statistics
  TeProgress::instance()->reset();

  // Time Statistics
  endTime_ = Time::instance().actualTimeString();
  partialTime_ = Time::instance().partialString();
  print();

  // Queue Statistics
  HidroGenerateQueueStatistics::instance().finsh( mainQueue_.size() );

  return true;
}

bool
HidroFix::solveQueue()
{
  // Used to update progress bar queue by queue.
  unsigned int queueCount = 0;
  unsigned int queueSize = mainQueue_.size();

  while( mainQueue_.size() > 0 )  
  {
    // retrieve the line and column of first pit 
    int *pitCoord = mainQueue_.front();
    unsigned int x = pitCoord[0];
    unsigned int y = pitCoord[1];

    // Altimetria of pit
    double altimetriaPit;
    demMatrix_.getElement( x, y, altimetriaPit );

    // Compute altimetria mean of neighbors    
    double mean = meanNeigbhors( x, y, 3, false );    

    if( mean > altimetriaPit )
    {
      // update the DEM
      demMatrix_[y][x] = mean;

      // Recompute the LDD for the neighbors
      recomputeNeighborhoodLDD( x, y );      

      // Recompute the LDD for this pit
      generateCommonLDD( demMatrix_, lddMatrix_, x, y, lddDirectionsMap_ );

      // Check if it is still a pit.
      // Its is necessary because all the neighboor can have the same altimetria.
      // Next try its will be a peak.
      unsigned char lddVal = 0;
      lddMatrix_.getElement( x,y, lddVal );
      if( lddVal == 0 )
      {
        mainQueue_.sendFront2Back();
      }
      else
      {
        ++solvedPitsNumber_;
        mainQueue_.pop();
      }
    }
    else // if( mean > altimetriaPit )
    {
      // Solve Peaks
      solvePeak( x, y );      
    }

    ++queueCount;    
    // Update progress bar
    if( queueCount < queueSize )
    {
      queueCount = 0;
      queueSize = mainQueue_.size();

      TeProgress::instance()->setProgress( initialSize_ -  queueSize );

      // Storage the partial results hour by hour
      if( Time::instance().hasPassedOneHouer() )
      {
        // Record the parcial results
        //saveParcialResults( db, demRasterParams, demMatrix, lddMatrix, verticalContrast, layerName,  lddName, demPartialName );
        // Update the Queue Statistics File.
        HidroGenerateQueueStatistics::instance().increment( queueSize );
      }

      // The user cancel the proccess
      if( TeProgress::instance()->wasCancelled() )
      {
        unsolvedPitsNumber_ += queueSize;        
        QMessageBox::warning(0, "Warning", "The operation fix LDD was cancelled by the user.");
        return false;
      }
    }
  }

  return true;
}

void
HidroFix::solvePeak( unsigned int x, unsigned int y )
{
  // TODO: review this method

  double altimetriaMean[8], minMedia;
  unsigned char minMediaDirection = 0;
  int currentNeighbor = 0; // used to know what neighbor we are looking

  int startColumn = x - 1;
  int endColumn = x +  2;
  int startLine = y - 1;
  int endLine = y + 2;

  for (int l = startLine; l < endLine; l++)
	{				
		for (int c = startColumn; c < endColumn; c++)
		{
      // if is not center cell
      if( !(x == c && y == l) )
      {
        // Compute the altimetria mean 3x3 for each neighbor and store in altimetriaMean
				altimetriaMean[currentNeighbor] = altimetriaMeanCompute( demMatrix_, l, c );
        currentNeighbor++;
      }
    }
  }
  // find the minimum altimetria mean and this direction.
	minMedia = minValue(altimetriaMean, lddDirectionsMap_, minMediaDirection);

  lddMatrix_.setElement(x, y, minMediaDirection);

  ++solvedPitsNumber_;        
  mainQueue_.pop();
}

void
HidroFix::recomputeNeighborhoodLDD( unsigned int x, unsigned int y )
{
  // Recompute the LDD for the neighbors
  // Compute the start and end of window (3x3)
  unsigned int startColumn = x - 1;
  unsigned int endColumn = x +  2;
  unsigned int startLine = y - 1;
  unsigned int endLine = y + 2;

  unsigned char lddVal;
  unsigned char oldLDDVal;

	for( unsigned int line = startLine; line < endLine; ++line )
	{
		for( unsigned int column = startColumn; column < endColumn; ++column )
    {      
      if( lddMatrix_.getElement( column, line, oldLDDVal ) )
      {
        // Its wasn't pit.
        if( oldLDDVal != 0 )
        {
          generateCommonLDD( demMatrix_, lddMatrix_, column, line, lddDirectionsMap_ );

          // Check for new pit
          lddMatrix_.getElement( column, line, lddVal );
          if( lddVal == 0 )
          {
            // Don't add new pit if its relay on border.
            if( !boundaryFixRegion( column, line ) )
            {
              mainQueue_.newPit( column, line );                  
            }
          }
        }
      }
    }
  }
}

bool
HidroFix::recursiveMethod()
{
  // Initialize the pits queue
  initPitQueueOrdered();

  // Initialize the progress window
  TeProgress::instance()->reset();
	TeProgress::instance()->setCaption("TerraHidro");
  TeProgress::instance()->setMessage("Correcting pits fom a raster flow");
  TeProgress::instance()->setTotalSteps( initialSize_ );
  TeProgress::instance()->setProgress( 0 );
  unsigned int interaction = 0;

  // Amplify the vertical contrasct for the DEM
  demMatrix_.setVerticalContrast( 100 );

  while( mainQueue_.size() > 0 )  
  {
    // retrieve the line and column of first pit 
    int *pitCoord = mainQueue_.front();

    if( solvePit( pitCoord[0], pitCoord[1] ) )
    {
      ++solvedPitsNumber_;
    }
    else
    {
      ++unsolvedPitsNumber_;
    }
    
    // pop the first pit of queue
    mainQueue_.pop();

    // Update the progress window
    TeProgress::instance()->setProgress( interaction );
    // Check if the user cancel the proccess
    if( TeProgress::instance()->wasCancelled() )
    {          
      QMessageBox::warning(0, "Warning", "The operation fix LDD was cancelled by the user.");
      return false;
    }
		
    interaction++;
  }

  // Finalize progress window
  TeProgress::instance()->reset();

  return true;
}

bool
HidroFix::solvePit( unsigned int x, unsigned int y )
{
  // verify if it already was solved
  unsigned char lddVal;
  if( lddMatrix_.getElement( x, y, lddVal ) )
  {
    if( lddVal != 0 )
    {      
      return true;
    }
  }

  // Compute altimetria mean of neighbors
  double mean = meanNeigbhors( x, y, 3, false );

  double altimetriaPit;
  demMatrix_.getElement( x, y, altimetriaPit );

  if( mean > altimetriaPit )
  {
    // update the DEM
    demMatrix_[y][x] = mean;        

    // Recompute the LDD for the pit and the neighbors
    // Compute the start and end of window (3x3)
    unsigned int startColumn = x - 1;
    unsigned int endColumn = x +  2;
    unsigned int startLine = y - 1;
    unsigned int endLine = y + 2;

	  for( unsigned int line = startLine; line < endLine; ++line )
	  {				
		  for( unsigned int column = startColumn; column < endColumn; ++column )
      {        
        generateCommonLDD( demMatrix_, lddMatrix_, column, line, lddDirectionsMap_ );              
      }
    }

    // Solve all neighbors pit starting from higher neighbors pit.
    unsigned int highPitNeighborX;
    unsigned int highPitNeighborY;
    while( highNeighborPit( x, y, highPitNeighborX, highPitNeighborY ) )
    {
      solvePit( highPitNeighborX, highPitNeighborY );
    }
    // Obs.: Recursividade da stak overflow :(

    return true;
  }
  else // if( mean > altimetriaPit )
  {
    int *pitCoord = unsolvedQueue_.newPit();
    pitCoord[0] = x;
    pitCoord[1] = y;      
    unsolvedQueue_.push(pitCoord);    
    return false;
  }  

  return true;
}

bool 
HidroFix::highNeighborPit( unsigned int x, unsigned int y,
    unsigned int& highPitNeighborX, unsigned int& highPitNeighborY )
{
  // Compute the start and end of window (3x3)
  unsigned int startColumn = x - 1;
  unsigned int endColumn = x +  2;
  unsigned int startLine = y - 1;
  unsigned int endLine = y + 2;
  
  double mean;
  unsigned char lddVal;

  double highAltimetria = - TeFLOAT;
  bool hasNeighborPit = false;
	for( unsigned int line = startLine; line < endLine; ++line )
	{				
		for( unsigned int column = startColumn; column < endColumn; ++column )
    {
      if( !boundaryFixRegion( column, line ) )
      {
        if( column!=x || line!=y )
        {
          if( lddMatrix_.getElement( column, line, lddVal ) )
          {
            if( lddVal == 0 )
            {
              mean = meanNeigbhors( column, line, 3, true );
              if( mean > highAltimetria )
              {
                highAltimetria = mean;
                highPitNeighborX = column;
                highPitNeighborY = line;
                hasNeighborPit = true;
              }
            }
          }
        }
      }
    }
  }

  return hasNeighborPit;
}

HidroFix::HidroFix( TeDatabase* database,
    TeRasterParams rasterParams,
    HidroMatrix<double>& demMatrix,
    HidroMatrix<unsigned char>& lddMatrix,
    unsigned char lddDirectionsMap[8], 
    std::string demName,
    std::string lddName,
    int pitMaxTreatment,
    double boost ) :
      database_(database),
      rasterParams_(rasterParams),
      demMatrix_(demMatrix),
      lddMatrix_(lddMatrix),
      lddDirectionsMap_(lddDirectionsMap),
      solvedPitsNumber_(0),
      unsolvedPitsNumber_(0),
      demName_(demName),
      lddName_(lddName),
      pitMaxTreatment_(pitMaxTreatment),
      boost_(boost)
{
  rasterLines_ = rasterParams.nlines_;
  rasterColumns_ = rasterParams.ncols_;
  
  queueStatisticsFileName_ = demName_ + "_" + lddName_ + "_Queue_Size_Stats.txt";
  pitCorrectionStatisticsFileName_ = demName_ + "_" + lddName_ + "_Pit_Correction_Stats.txt";
}

bool
HidroFix::initPitQueueOrdered()
{  
  // Create temporary table to order the pits
  if( database_->tableExist( "hidro_pits" ) )
  {
    database_->deleteTable( "hidro_pits" );
  }
  TeAttributeList attList;
  {TeAttribute attPitId;
	attPitId.rep_.name_ = "pit_id";
	attPitId.rep_.type_ = TeUNSIGNEDINT;
	attPitId.rep_.isPrimaryKey_ = true;
	attPitId.rep_.isAutoNumber_ = true;
	attPitId.rep_.null_ = false;
  attList.push_back(attPitId);}
  {TeAttribute attPitX;
	attPitX.rep_.name_ = "pit_x";
	attPitX.rep_.type_ = TeINT;	
	attPitX.rep_.defaultValue_ = "0";
	attList.push_back(attPitX);}
  {TeAttribute attPitY;
	attPitY.rep_.name_ = "pit_y";
	attPitY.rep_.type_ = TeINT;	
	attPitY.rep_.defaultValue_ = "0";
	attList.push_back(attPitY);}
  {TeAttribute attPitAlt;
	attPitAlt.rep_.name_ = "pit_alt";
	attPitAlt.rep_.type_ = TeREAL;
  attPitAlt.rep_.decimals_ = 15;
	attPitAlt.rep_.defaultValue_ = "0.0";
	attList.push_back(attPitAlt);}
  if( !database_->createTable("hidro_pits", attList) )
  {
		return false;
  }
  // Create index  
  {std::string sql = "CREATE INDEX alt_index ON hidro_pits (pit_alt);";  
  if( !database_->execute( sql ) )
  {
    QMessageBox::critical( 0, "HidroFix", database_->errorMessage().c_str() );
    return false;
  }}

  // Initialize the progress window
  TeProgress::instance()->reset();
	TeProgress::instance()->setCaption("TerraHidro");
  TeProgress::instance()->setMessage("Initialize main pit queue.");
  TeProgress::instance()->setTotalSteps( rasterLines_-4 );
  TeProgress::instance()->setProgress( 0 );
  unsigned int interaction = 0;

  // Populate temporary table
  for( unsigned int line = 2; line < rasterLines_-2; ++line )
	{
		for( unsigned int column = 2; column < rasterColumns_-2; ++column )
    {      
      unsigned char lddVal;
      lddMatrix_.getElement( column, line, lddVal );
      if( lddVal == 0 )
      {
        if( !boundaryFixRegion( column, line ) )
        {
          //if( pitsNeighbors( column, line ) < 5 )
          //{
            double val = meanNeigbhors( column, line, 3, true );
            std::string sql = "INSERT INTO hidro_pits ( pit_x, pit_y, pit_alt ) VALUES ( " + Te2String(column) + ", " + Te2String(line) + ", " + Te2String(val, 15) + " );" ;
            if( !database_->execute( sql ) )
            {
              QMessageBox::critical( 0, "HidroFix", database_->errorMessage().c_str() );
              return false;
            }
          //}
          //else
          //{
            //lddMatrix_[line][column] = 0;
          //}
        }
      }
      TeProgress::instance()->setProgress( line );
    }
  }

  // Finalize the progress window
  TeProgress::instance()->reset();

  // Populate queue pits
  std::string sql = "SELECT * FROM hidro_pits ORDER BY pit_alt DESC;";
  TeDatabasePortal* portal = database_->getPortal();
	if( !portal )
  {
		return false;
  }
	if( !portal->query(sql) )
	{    
    QMessageBox::critical( 0, "HidroFix", portal->errorMessage().c_str() );
		delete portal;
		return false;
	}
	while( portal->fetchRow() ) 
	{    
    unsigned int x = portal->getInt( "pit_x" );
    unsigned int y = portal->getInt( "pit_y" );

    int *pitCoord = mainQueue_.newPit();
    pitCoord[0] = x;
    pitCoord[1] = y;
    mainQueue_.push( pitCoord );
  }
  // remove temporary table  
  database_->deleteTable( "hidro_pits" );

  initialSize_ = mainQueue_.size();

  return true;
}

double
HidroFix::meanNeigbhors( unsigned int x, unsigned int y, int windowSize = 3, bool includeCenter = true )
{
  // Compute the start and end of window
  unsigned int startColumn = x - (int)( windowSize/2 );
  unsigned int endColumn = x + (int)( windowSize/2 ) +1;
  unsigned int startLine = y - (int)( windowSize/2 );
  unsigned int endLine = y + (int)( windowSize/2 ) + 1;

  // Check the borders
  if( startColumn < 0 )
    startColumn = 0;
  if( endColumn > rasterColumns_ )
    endColumn = rasterColumns_;
  if( startLine < 0 )
    startLine = 0;
  if( endLine > rasterLines_ )
    endLine = rasterLines_;

  double total = 0;
  int elementsNumber = 0; // number of computed altimetrias  
  double altimetria;

  for( unsigned int line = startLine; line < endLine; ++line )
	{
		for( unsigned int column = startColumn; column < endColumn; ++column )
    {
      if( demMatrix_.getElement( column, line, altimetria ) )
      {
        total = total + altimetria;
        ++elementsNumber;
      }
    }
  }

  if( !includeCenter )
  {
    demMatrix_.getElement( x, y, altimetria );
    total =  total - altimetria;
    --elementsNumber;
  }

  double mean;

  if( elementsNumber > 0 )
  {
    mean = total / elementsNumber;
  }
  else
  {
    mean = -9999;
  }

  return mean;  
}

bool
HidroFix::boundaryFixRegion( unsigned int x, unsigned int y )
{
  if( (x < 2) ||  (x > (rasterColumns_-3)) ||
    (y < 2) || (y > (rasterLines_-3)) )
  {
    return true;
  }

  if( demMatrix_.hasDummy() )
  {
    // Compute the start and end of window (3x3)
    unsigned int startColumn = x - 1;
    unsigned int endColumn = x +  2;
    unsigned int startLine = y - 1;
    unsigned int endLine = y + 2;
  	
    double altimetria;

	  for( unsigned int line = startLine; line < endLine; ++line )
	  {				
		  for( unsigned int column = startColumn; column < endColumn; ++column )
		  {
        if( !demMatrix_.getElement(column, line, altimetria) )
        {
          // is dummy.
          return true;
        }
      }
    }
  }

  return false;
}

int 
HidroFix::pitsNeighbors( unsigned int x, unsigned int y )
{
  // Compute the start and end of window (3x3)
  unsigned int startColumn = x - 1;
  unsigned int endColumn = x + 2;
  unsigned int startLine = y - 1;
  unsigned int endLine = y + 2;  
  
  int pitsNumber = 0;
  unsigned char lddVal;

	for( unsigned int line = startLine; line < endLine; ++line )
	{				
		for( unsigned int column = startColumn; column < endColumn; ++column )
		{
      if( column!=x || line!=y )
      {
        if( lddMatrix_.getElement(column, line, lddVal) )
        {
          if( lddVal == 0 )
          {
            ++pitsNumber;
          }
        }
      }
    }
  }

  return pitsNumber;
}


void
HidroFix::growUpPitRegions()
{
  for( unsigned int line = 1; line < rasterLines_-1; ++line )
	{
		for( unsigned int column = 1; column < rasterColumns_-1; ++column )
    {
      // All the neighbors are pit.
      if( pitsNeighbors( column, line ) > 7 )
      {
        double altimetria;
        if( demMatrix_.getElement( column, line, altimetria ) )
        {          
          growUpAltimetriaRegions( column, line, altimetria );
        }
      }
    }
  }
}

void 
HidroFix::growUpAltimetriaRegions( unsigned int x, unsigned int y, double altimetria )
{
  // Compute the start and end of window
  int startColumn = x - 1;
  int endColumn = x + 2;
  int startLine = y - 1;
  int endLine = y + 2;

  // Check the borders
  if( startColumn < 0 )
    startColumn = 0;
  if( endColumn > rasterColumns_ )
    endColumn = rasterColumns_;
  if( startLine < 0 )
    startLine = 0;
  if( endLine > rasterLines_ )
    endLine = rasterLines_;

  for( unsigned int line = startLine; line < endLine; ++line )
	{
		for( unsigned int column = startColumn; column < endColumn; ++column )
    {
      double altimetria_;
      if( demMatrix_.getElement( column, line, altimetria_ ) )
      {
        unsigned char lddVal;
        lddMatrix_.getElement( column, line, lddVal );
        if( altimetria == altimetria_ && lddVal != 9 )
        {
          lddMatrix_[line][column] = 9;
          growUpAltimetriaRegions( column, line, altimetria_ );
        }
      }
    }
  }
}

bool
HidroFix::initPitQueue()
{
  // Initialize the progress window
  TeProgress::instance()->reset();
	TeProgress::instance()->setCaption("TerraHidro");
  TeProgress::instance()->setMessage("Initialize main pit queue.");
  TeProgress::instance()->setTotalSteps( rasterLines_-4 );
  TeProgress::instance()->setProgress( 0 );
  unsigned int interaction = 0;
  
  for( unsigned int line = 2; line < rasterLines_-2; ++line )
	{
		for( unsigned int column = 2; column < rasterColumns_-2; ++column )
    {      
      unsigned char lddVal;
      lddMatrix_.getElement( column, line, lddVal );
      if( lddVal == 0 )
      {
        if( !boundaryFixRegion( column, line ) )
        {
          mainQueue_.newPit( column, line );          
        }
      }
      TeProgress::instance()->setProgress( line );
    }
  }

  // Finalize the progress window
  TeProgress::instance()->reset();
  
  initialSize_ = mainQueue_.size();

  return true;
}

void
HidroFix::print()
{
  QFile pitCorrectionStatisticsFile( pitCorrectionStatisticsFileName_.c_str() );  

  if( pitCorrectionStatisticsFile.open( IO_ReadWrite  ) )
  {
    QTextStream pitCorrectionStatisticsStream;
    pitCorrectionStatisticsStream.setDevice( &pitCorrectionStatisticsFile );

    pitCorrectionStatisticsStream << demName_.c_str() << "\n";
    pitCorrectionStatisticsStream << "Image size: " << Te2String(rasterLines_*rasterColumns_).c_str() << "\n";
    pitCorrectionStatisticsStream << "Lines: " << Te2String(rasterLines_).c_str() << "\n";
    pitCorrectionStatisticsStream << "Columns: " << Te2String(rasterColumns_).c_str() << "\n \n";

    pitCorrectionStatisticsStream << "Start Time: " << starTime_.c_str();
    pitCorrectionStatisticsStream << "End Time: " << endTime_.c_str();
    pitCorrectionStatisticsStream << "Processing time: " << partialTime_.c_str() << "\n \n";

    pitCorrectionStatisticsStream << "Initial pit number: " << Te2String(initialSize_).c_str() << "\n";
    pitCorrectionStatisticsStream << "Solved pit number: " << Te2String(solvedPitsNumber_).c_str() << "\n";
    pitCorrectionStatisticsStream << "Unsolved pit number: " << Te2String(unsolvedPitsNumber_).c_str() << "\n";    
  }
}
#include <HidroFixPFS.h>

// Hidro Includes
#include <HidroPFS.h>
#include <HidroFlowD8.h>
#include <HidroFlowUtils.h>

// TODO: create new .h and .cpp to HidroGenerateQueueStatistics
#include <HidroPitCorrectionD8MediaQueue.h> 

// TerraLib Includes
#include <TeUtils.h>

// Qt Includes
#include <QmessageBox.h>

bool
HidroFixPFS::pfs()
{
  // Time Statistics
  Time::instance().start();
  starTime_ = Time::instance().actualTimeString();

  // Solve plane areas (pits river)
  fillPlaneAreas();
 
  // Initialize the pits queue
  initPitQueue();  

  // Solve the simple pits
  solveSimplePits();
  
  // Queue Statistics
  HidroGenerateQueueStatistics::instance().start( queueStatisticsFileName_, initialSize_ );  

  // Progress Bar
  TeProgress::instance()->reset();
	TeProgress::instance()->setCaption("TerraHidro");
	TeProgress::instance()->setMessage("Correcting pits fom a raster flow.");
  TeProgress::instance()->setTotalSteps( initialSize_ );

  // Initialize the PFS algorithm
  HidroPFS pfs( demMatrix_ );
  
  // Para cada fosso
  while( !pitQueue_.empty() )
  {
    // retrieve the line and column of first pit 
    int *pitCoord = pitQueue_.front();
    unsigned int x = pitCoord[0];
    unsigned int y = pitCoord[1];    
    pitQueue_.pop();

    unsigned char lddVal;
    lddMatrix_.getElement( x, y, lddVal );
    
    // while pit wans't solved
    while( lddVal == 0 )
    {
      // sove the pit using the PFS algorithm.
      pfs.solvePit( x, y );
            
      // recomput the LDD for changed cells and neighboors      
      for( unsigned int i=0; i < pfs.solution_path_.size(); i++ )
      {
        recomputeNeighborhoodLDD( pfs.solution_path_[i].first, pfs.solution_path_[i].second );      
      }

      // check if it keep a pit
      lddMatrix_.getElement( x, y, lddVal );
      if( lddVal == 0 )
      {
        // probably a precission error (zero computational)         
        demMatrix_[y][x] = demMatrix_[y][x] - double_problem_increment_;            
        double_problem_++;          
      }
      else
      {
        // FPS Statistics
        solvedPitsNumber_++;
        path_length_mean_ += pfs.solution_path_.size();        

        if( pfs.solution_path_.size() > big_path_ )
          big_path_ = pfs.solution_path_.size();
      }
      
      // check pits in the path
      for( unsigned int i=0; i<pfs.solution_path_.size(); i++ )
      {          
        // Compute the start and end of window (3x3)
        unsigned int startColumn = pfs.solution_path_[i].first - 1;
        unsigned int endColumn = pfs.solution_path_[i].first + 2;
        unsigned int startLine = pfs.solution_path_[i].second - 1;
        unsigned int endLine = pfs.solution_path_[i].second + 2;

        if( pfs.solution_path_[i].first < 2 )
          startColumn = 1;
        if( endColumn > rasterColumns_-1 )
          endColumn = rasterColumns_-1;
        if( pfs.solution_path_[i].second < 2 )
          startLine = 1;
        if( endLine > rasterLines_-1 )
          endLine = rasterLines_-1;          

	      for( unsigned int line = startLine; line < endLine; ++line )
	      {
		      for( unsigned int column = startColumn; column < endColumn; ++column )
		      {
            if( lddMatrix_[line][column] == 0 )
            {
              if( !boundaryFixRegion( column, line) )
                pitQueue_.newPit( column, line );
                pits_in_path_++;
            }
          }          
        }
      }      
    }    

    // Update Progress
    TeProgress::instance()->setProgress( initialSize_ - pitQueue_.size() );

    // Storage the partial results hour by hour
    if( Time::instance().hasPassedOneHouer() )
    {
      // Record the parcial results
      //saveParcialResults( db, demRasterParams, demMatrix, lddMatrix, verticalContrast, layerName,  lddName, demPartialName );
      // Update the Queue Statistics File.
      HidroGenerateQueueStatistics::instance().increment( pitQueue_.size() );
    }

    // The user cancel the proccess
    if( TeProgress::instance()->wasCancelled() )
    {
      unsolvedPitsNumber_ += pitQueue_.size();        
      QMessageBox::warning(0, "Warning", "The operation fix LDD was cancelled by the user.");
      // Use insted return to save the results.
      break;
    }   
  }

  // FPS Statistics  
  path_length_mean_ = path_length_mean_ / solvedPitsNumber_;
  visited_number_mean_ = visited_number_mean_ / solvedPitsNumber_;
  border_path_gradient_ = BORDER_PATH_GRADIENT_;  

  // Finishi progress bar and statistics
  TeProgress::instance()->reset();

  // Time Statistics
  endTime_ = Time::instance().actualTimeString();
  partialTime_ = Time::instance().partialString();
  print();

  // Queue Statistics
  HidroGenerateQueueStatistics::instance().finsh( pitQueue_.size() );

  return true;
}

void
HidroFixPFS::fillPlaneAreas()
{
  // Fila das celulas planas
  HidroSmartQueue planeQueue_;

  // Initialize the progress window
  TeProgress::instance()->reset();
	TeProgress::instance()->setCaption("TerraHidro");
  TeProgress::instance()->setMessage("Initialize plane areas.");
  TeProgress::instance()->setTotalSteps( rasterLines_ - 2 );
  TeProgress::instance()->setProgress( 0 );
  double centerAltimetria;
  double neighboorAltimetria;
  for( unsigned int line = 1; line < rasterLines_-1; ++line )
	{
		for( unsigned int column = 1; column < rasterColumns_-1; ++column )
    {
      if( pitsNeighbors( column, line ) > 7 )
      {
        lddMatrix_[line][column] = 9;
        
        centerAltimetria = demMatrix_[line][column];
        // Janela 5x5        
        unsigned int startColumn = column - 2;
        unsigned int endColumn = column + 3;
        unsigned int startLine = line - 2;
        unsigned int endLine = line + 3;
        // Check the borders
        if( column < 2 )
          startColumn = 0;
        if( endColumn > rasterColumns_ )
          endColumn = rasterColumns_;
        if( startLine < 2 )
          startLine = 0;
        if( endLine > rasterLines_ )
          endLine = rasterLines_;
	      for( unsigned int l = startLine; l < endLine; ++l )
	      {
		      for( unsigned int c = startColumn; c < endColumn; ++c )
          {
            neighboorAltimetria = demMatrix_[l][c];
            if( neighboorAltimetria == centerAltimetria )
            {
              lddMatrix_[l][c] = 9;              
              //if( c!=0 && l!=0 && c!=rasterColumns_-1 && l!=rasterLines_-1 )
              if( !boundaryRegion( c, l ) )
                planeQueue_.newPit( c, l );
            }
          }
        }
      }
    }
    TeProgress::instance()->setProgress( line );
  }
  // Finalize the progress window
  TeProgress::instance()->reset();

  // Initialize the progress window
  TeProgress::instance()->reset();
	TeProgress::instance()->setCaption("TerraHidro");
  TeProgress::instance()->setMessage("Caving plane Areas.");
  TeProgress::instance()->setTotalSteps( planeQueue_.size() );
  TeProgress::instance()->setProgress( 0 );

  // Fila da borda das areas planas  
  std::vector<BreachingNode> borderQueue_;

  unsigned int counter = 0;  
  double decrease = 0;

  while( !planeQueue_.empty() )
  {
    // Varro a fila inteira procurando pelas bordas
    // Acho as bordas e calculo a nova altimetria
    unsigned int planeQueueSize = planeQueue_.size();
    for( unsigned int count=0; count<planeQueueSize; count++ )
    {
      // retrieve the line and column of first plane cell 
      int *pitCoord = planeQueue_.front();
      unsigned int x = pitCoord[0];
      unsigned int y = pitCoord[1];
      planeQueue_.pop();

      if( isPlaneBorder( x, y ) )
      {
        // Se for borda adiciona na fila de borda
        BreachingNode border;
        border.x_ = x;
        border.y_ = y;        
        border.altimetria_ = demMatrix_[y][x] - decrease;
        borderQueue_.push_back( border );
        
        counter++;
        TeProgress::instance()->setProgress( counter );
      }
      else
      {
        // Se nao adiciona na fila de planos novamente
        planeQueue_.newPit( x, y );
      }
    }
    decrease = decrease + caving_increment_;

    // Altero o DEM e recalculo o LDD da vizinhança
    while( !borderQueue_.empty() )
    {
      // retrieve the line and column of first border cell 
      BreachingNode border = borderQueue_.back();
      unsigned int x = border.x_;
      unsigned int y = border.y_;
      borderQueue_.pop_back();
      
      // atualizo o DEM com a media das altimetrias (3x3) excluindo o centro
      demMatrix_[y][x] = border.altimetria_;

      // Marco para calcular novamente o LDD      
      lddMatrix_[y][x] = 11;
    }    
  }
  // Finalize the progress window
  TeProgress::instance()->reset();

  //*/ zera o ldd dnovo
  for( unsigned int line = 1; line < rasterLines_-1; ++line )
	{
		for( unsigned int column = 1; column < rasterColumns_-1; ++column )
    {
      if( lddMatrix_[line][column] == 11 )
        recomputeNeighborhoodLDD( column, line );        
    }
  }
  // Bordas
  for( unsigned int column = 0; column < rasterColumns_; ++column )
  {
    lddMatrix_[0][column] = 255;
    lddMatrix_[rasterLines_-1][column] = 255;    
  }
  for( unsigned int line = 0; line < rasterLines_; ++line )
  {
    lddMatrix_[line][0] = 255;
    lddMatrix_[line][rasterColumns_-1] = 255;    
  }
  /**/
}

void
HidroFixPFS::fillPlaneAreas2()
{
  // Fila das celulas planas
  HidroSmartQueue planeQueue_;

  // Initialize the progress window
  TeProgress::instance()->reset();
	TeProgress::instance()->setCaption("TerraHidro");
  TeProgress::instance()->setMessage("Initialize plane areas.");
  TeProgress::instance()->setTotalSteps( rasterLines_ -4 );
  TeProgress::instance()->setProgress( 0 );
  double centerAltimetria;
  double neighboorAltimetria;
  for( unsigned int line = 2; line < rasterLines_-2; ++line )
	{
		for( unsigned int column = 2; column < rasterColumns_-2; ++column )
    {
      if( pitsNeighbors( column, line ) > 7 )
      {
        lddMatrix_[line][column] = 9;
        
        centerAltimetria = demMatrix_[line][column];
        // Janela 5x5        
        unsigned int startColumn = column - 2;
        unsigned int endColumn = column + 3;
        unsigned int startLine = line - 2;
        unsigned int endLine = line + 3;
        // Check the borders
        if( startColumn < 0 )
          startColumn = 0;
        if( endColumn > rasterColumns_ )
          endColumn = rasterColumns_;
        if( startLine < 0 )
          startLine = 0;
        if( endLine > rasterLines_ )
          endLine = rasterLines_;
	      for( unsigned int l = startLine; l < endLine; ++l )
	      {
		      for( unsigned int c = startColumn; c < endColumn; ++c )
          {
            neighboorAltimetria = demMatrix_[l][c];
            if( neighboorAltimetria == centerAltimetria )
            {
              lddMatrix_[l][c] = 9;

              if( c!=0 && l!=0 && c!=rasterColumns_-1 && l!=rasterLines_-1 )
                planeQueue_.newPit( c, l );
            }
          }
        }
      }
    }
    TeProgress::instance()->setProgress( line );
  }
  // Finalize the progress window
  TeProgress::instance()->reset();

  // Initialize the progress window
  TeProgress::instance()->reset();
	TeProgress::instance()->setCaption("TerraHidro");
  TeProgress::instance()->setMessage("Fill plane Areas.");
  TeProgress::instance()->setTotalSteps( planeQueue_.size() );
  TeProgress::instance()->setProgress( 0 );

  // Fila da borda das areas planas
  //HidroSmartQueue borderQueue_;
  std::vector<BreachingNode> borderQueue_;

  unsigned int counter = 0;
  while( !planeQueue_.empty() )
  {
    // Varro a fila inteira procurando pelas bordas
    // Acho as bordas e calculo a nova altimetria
    unsigned int planeQueueSize = planeQueue_.size();
    for( unsigned int count=0; count<planeQueueSize; count++ )
    {
      // retrieve the line and column of first plane cell 
      int *pitCoord = planeQueue_.front();
      unsigned int x = pitCoord[0];
      unsigned int y = pitCoord[1];
      planeQueue_.pop();

      if( isPlaneBorder( x, y ) )
      {
        // Se for borda adiciona na fila de borda
        BreachingNode border;
        border.x_ = x;
        border.y_ = y;        
        border.altimetria_ = meanNeigbhors( x, y, 3, false ); // media das altimetrias (3x3) excluindo o centro
        
        borderQueue_.push_back( border );        
        
        counter++;
        TeProgress::instance()->setProgress( counter );
      }
      else
      {
        // Se nao adiciona na fila de planos novamente
        planeQueue_.newPit( x, y );
      }
    }    

    // Altero o DEM e recalculo o LDD da vizinhança
    while( !borderQueue_.empty() )
    {
      // retrieve the line and column of first border cell 
      BreachingNode border = borderQueue_.back();
      unsigned int x = border.x_;
      unsigned int y = border.y_;
      borderQueue_.pop_back();
      
      // atualizo o DEM com a media das altimetrias (3x3) excluindo o centro
      demMatrix_[y][x] = border.altimetria_;

      // Calculo novamente o LDD
      lddMatrix_[y][x] = 11;      
    }   
  }
  // Finalize the progress window
  TeProgress::instance()->reset();

  //*/ zera o ldd dnovo
  for( unsigned int line = 0; line < rasterLines_; ++line )
	{
		for( unsigned int column = 0; column < rasterColumns_; ++column )
    {
      if( lddMatrix_[line][column] == 11 )
        recomputeNeighborhoodLDD( column, line );        
    }
  }/**/
}

void
HidroFixPFS::solveSimplePits()
{
  // Initialize the progress window
  TeProgress::instance()->reset();
	TeProgress::instance()->setCaption("TerraHidro");
  TeProgress::instance()->setMessage("Simple Pits.");
  TeProgress::instance()->setTotalSteps( initialSize_ );
  TeProgress::instance()->setProgress( 0 );

  for( unsigned int counter = 0; counter < initialSize_; counter++ )
  {
    // retrieve the line and column of first pit 
    int *pitCoord = pitQueue_.front();
    unsigned int x = pitCoord[0];
    unsigned int y = pitCoord[1];    
    pitQueue_.pop();

    // Guardo a altimetria antiga
    double oldAltimetria = demMatrix_[y][x];

    // change the DEM altimetria with mean altimetria
    double meanAltimetria = meanNeigbhors( x, y, 3, false );
    demMatrix_[y][x] = meanAltimetria;

    recomputeNeighborhoodLDD( x, y );

    // verifico se algum vizinho é fosso
    bool anyPit = false;
    // Compute the start and end of window (3x3)
    unsigned int startColumn = x - 1;
    unsigned int endColumn = x + 2;
    unsigned int startLine = y - 1;
    unsigned int endLine = y + 2;
	  for( unsigned int line = startLine; line < endLine; ++line )
	  {
		  for( unsigned int column = startColumn; column < endColumn; ++column )
      {
        unsigned char lddVal;
        if( lddMatrix_.getElement( column, line, lddVal ) )
        {
          if( lddVal == 0 )
          {
            anyPit = true;
          }
        }
      }
    }

    if( anyPit )
    {
      // volto ao que era antes
      demMatrix_[y][x] = oldAltimetria;
      recomputeNeighborhoodLDD( x, y );
      pitQueue_.newPit( x, y );
    }
    else
    {
      // pit solved
      simplePits_++;
    }

    TeProgress::instance()->setProgress( counter );
  }

  initialSize_ = pitQueue_.size();
}

HidroFixPFS::HidroFixPFS( TeDatabase* database,
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
      caving_increment_(0.0001),
      double_problem_increment_(0.001),
      solvedPitsNumber_(0),
      addPitsNumber_(0),
      unsolvedPitsNumber_(0),
      simplePits_(0),
      demName_(demName),
      lddName_(lddName),
      pitMaxTreatment_(pitMaxTreatment),
      boost_(boost),
      path_length_mean_(0.0),
      visited_number_mean_(0.0),
      pits_solved_(0),
      pits_solved_order_(0),
      big_path_(0),
      big_path_number_(0),
      double_problem_(0),
      pits_in_path_(0),
      unsolved_double_problem_(0)
{
  rasterLines_ = rasterParams.nlines_;
  rasterColumns_ = rasterParams.ncols_;
  
  queueStatisticsFileName_ = demName_ + "_" + lddName_ + "_Queue_Size_Stats.txt";
  pitCorrectionStatisticsFileName_ = demName_ + "_" + lddName_ + "_Pit_Correction_Stats.txt";
}

HidroFixPFS::~HidroFixPFS()
{
}

bool
HidroFixPFS::initPitQueue()
{
  // Initialize the progress window
  TeProgress::instance()->reset();
	TeProgress::instance()->setCaption("TerraHidro");
  TeProgress::instance()->setMessage("Initialize main pit queue.");
  TeProgress::instance()->setTotalSteps( rasterLines_-4 );
  TeProgress::instance()->setProgress( 0 );

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
          pitQueue_.newPit( column, line );         
        }
      }
      TeProgress::instance()->setProgress( line );
    }
  }  

  // Finalize the progress window
  TeProgress::instance()->reset();
  
  initialSize_ = pitQueue_.size();

  return true;
}

int 
HidroFixPFS::pitsNeighbors( unsigned int x, unsigned int y )
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
      if( lddMatrix_.getElement(column, line, lddVal) )
      {
        if( lddVal == 0 || lddVal == 9 )
        {
          ++pitsNumber;
        }
      }
    }
  }

  lddMatrix_.getElement(x, y, lddVal);
  if( lddVal == 0 || lddVal == 9 )
    --pitsNumber;

  return pitsNumber;
}

bool
HidroFixPFS::initPitQueueOrdered()
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
            //double val = meanNeigbhors( column, line, 3, true );
            double val = demMatrix_[line][column];
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

    int *pitCoord = pitQueue_.newPit();
    pitCoord[0] = x;
    pitCoord[1] = y;
    pitQueue_.push( pitCoord );
  }
  // remove temporary table  
  database_->deleteTable( "hidro_pits" );

  initialSize_ = pitQueue_.size();

  return true;
}

double
HidroFixPFS::meanNeigbhors( unsigned int x, unsigned int y, int windowSize = 3, bool includeCenter = true )
{
  // Compute the start and end of window
  unsigned int startColumn = x - (int)( windowSize/2 );
  unsigned int endColumn = x + (int)( windowSize/2 ) +1;
  unsigned int startLine = y - (int)( windowSize/2 );
  unsigned int endLine = y + (int)( windowSize/2 ) + 1;

  // Check the borders
  if( (int)x < windowSize/2 )
    startColumn = 0;
  if( endColumn > rasterColumns_ )
    endColumn = rasterColumns_;
  if( (int)y < windowSize/2 )
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

void
HidroFixPFS::recomputeNeighborhoodLDD( unsigned int x, unsigned int y )
{
  // Recompute the LDD for the neighbors
  // Compute the start and end of window (3x3)
  unsigned int startColumn = x - 1;
  unsigned int endColumn = x + 2;
  unsigned int startLine = y - 1;
  unsigned int endLine = y + 2;

  // Check the borders
  if( x < 2 )
    startColumn = 1;
  if( endColumn > rasterColumns_-1 )
    endColumn = rasterColumns_-1;
  if( y < 2 )
    startLine = 1;
  if( endLine > rasterLines_-1 )
    endLine = rasterLines_-1;

	for( unsigned int line = startLine; line < endLine; ++line )
	{
		for( unsigned int column = startColumn; column < endColumn; ++column )
    {
      generateCommonLDD( demMatrix_, lddMatrix_, column, line, lddDirectionsMap_ );      
    }
  }
}

void
HidroFixPFS::print()
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
    pitCorrectionStatisticsStream << "Simple pit number: " << Te2String(simplePits_).c_str() << "\n \n";

    pitCorrectionStatisticsStream << "-- Priority First Search Parameters --" << "\n";
    pitCorrectionStatisticsStream << "border_path_gradient_: " << Te2String(border_path_gradient_, 9).c_str() << "\n";    
    pitCorrectionStatisticsStream << "caving_increment_: " << Te2String(caving_increment_,9).c_str() << "\n";
    pitCorrectionStatisticsStream << "double_problem_increment_: " << Te2String(double_problem_increment_,9).c_str() << "\n \n";

    pitCorrectionStatisticsStream << "-- Priority First Search numbers --" << "\n";
    pitCorrectionStatisticsStream << "Path mean: " << Te2String(path_length_mean_, 0).c_str() << "\n";    
    pitCorrectionStatisticsStream << "Big Path: " << Te2String(big_path_).c_str() << "\n";    
    pitCorrectionStatisticsStream << "Double Problem: " << Te2String(double_problem_).c_str() << "\n \n";    
  }
}

bool
HidroFixPFS::boundaryFixRegion( unsigned int x, unsigned int y )
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

bool
HidroFixPFS::boundaryRegion( unsigned int x, unsigned int y )
{

  //if( c!=0 && l!=0 && c!=rasterColumns_-1 && l!=rasterLines_-1 )
  
  if( (x == 0) ||  (x == rasterColumns_-1) ||
    (y == 0) || (y == (rasterLines_-1) ) )
  {
    return true;
  }
  if( demMatrix_.hasDummy() )
  {
    // Compute the start and end of window (3x3)
    unsigned int startColumn = x - 1;
    unsigned int endColumn = x + 2;
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

bool
HidroFixPFS::isPlane( unsigned int x, unsigned int y )
{
  double centerAltimetria;

  // if its not dummy
  if( !demMatrix_.getElement( x, y, centerAltimetria ) )
    return false;

  double neighboorAltimetria;

  // Recompute the LDD for the neighbors
  // Compute the start and end of window (3x3)
  unsigned int startColumn = x - 1;
  unsigned int endColumn = x +  2;
  unsigned int startLine = y - 1;
  unsigned int endLine = y + 2;

  // Check the borders
  /*if( startColumn < 0 )
    startColumn = 0;
  if( endColumn > rasterColumns_ )
    endColumn = rasterColumns_;
  if( startLine < 0 )
    startLine = 0;
  if( endLine > rasterLines_ )
    endLine = rasterLines_;*/

  for( unsigned int line = startLine; line < endLine; ++line )
	{
		for( unsigned int column = startColumn; column < endColumn; ++column )
    {
      if( !demMatrix_.getElement( column, line, neighboorAltimetria ) )
        return false;

      if( centerAltimetria != neighboorAltimetria )
        return false;
    }
  }

  return true;
}

bool
HidroFixPFS::isPlaneBorder( unsigned int x, unsigned int y )
{
  // Tratar dados com Dummy também.
  if( !demMatrix_.hasDummy() )
  {
    // up
    if( y > 0)
    if( lddMatrix_[y-1][x] != 9 )
      return true;
    // right
    if( lddMatrix_[y][x+1] != 9 )
      return true;
    // down
    if( lddMatrix_[y+1][x] != 9 )
      return true;
    // left
    if( x > 0 )
    if( lddMatrix_[y][x-1] != 9 )
      return true;
  }
  else
  {
    unsigned char lddVal;
    
    // up
    if( y > 0)
    {
      if( lddMatrix_.getElement( x, y-1, lddVal ) )
      {
        if( lddVal != 9 )
          return true;
      }
    }
    
    // right
    if( lddMatrix_.getElement( x+1, y, lddVal ) )
    {
      if( lddVal != 9 )
        return true;
    }
    
    // down
    if( lddMatrix_.getElement( x, y+1, lddVal ) )
    {
      if( lddVal != 9 )
        return true;
    }    
    
    
    // left
    if( x > 0 )
    {
      if( lddMatrix_.getElement( x-1, y, lddVal ) )
      {
        if( lddVal != 9 )
          return true;
      }
    }    
  }  

  return false;
}


int
HidroFixPFS::sameAlimetriaNeighboors( unsigned int x, unsigned int y )
{
  double centerAltimetria;

  // if its not dummy
  if( !demMatrix_.getElement( x, y, centerAltimetria ) )
    return false;

  double neighboorAltimetria;

  // Recompute the LDD for the neighbors
  // Compute the start and end of window (3x3)
  unsigned int startColumn = x - 1;
  unsigned int endColumn = x +  2;
  unsigned int startLine = y - 1;
  unsigned int endLine = y + 2;

  // Check the borders
  if( startColumn < 0 )
    startColumn = 0;
  if( endColumn > rasterColumns_ )
    endColumn = rasterColumns_;
  if( startLine < 0 )
    startLine = 0;
  if( endLine > rasterLines_ )
    endLine = rasterLines_;

  int sameAltimetria = 0;

  for( unsigned int line = startLine; line < endLine; ++line )
	{
		for( unsigned int column = startColumn; column < endColumn; ++column )
    {
      if( demMatrix_.getElement( column, line, neighboorAltimetria ) )
        if( centerAltimetria == neighboorAltimetria )
          sameAltimetria++;        
    }
  }

  return sameAltimetria;
}
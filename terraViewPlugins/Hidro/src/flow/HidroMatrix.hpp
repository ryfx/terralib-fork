#ifndef HIDRO_MATRIX_H_
  #define HIDRO_MATRIX_H_

  #include <TePDIMatrix.hpp>

  #include <TeDatabase.h>
  #include <TeRaster.h>
  #include <TeRasterParams.h>
  #include <TeImportRaster.h>
  
  #define LDD_DUMMY 255

  /**
   * @class HidroMatrix
   * @brief This is the template class to deal with a generic matrix extends the TePDIMatrix.
   * @author Alexandre Copertino Jardim <alexcj@dpi.inpe.br>
   * @ingroup hidroFlow The Group for generate flow functions.
   */
  template< class T >
  class HidroMatrix : public TePDIMatrix<T>
  {
  public:
      /**
       * @brief Default Constructor.
       * @note The default mamory policy is RAMMemPol.
       */
      HidroMatrix();    

      /**
       * @brief Default Destructor
       */
      virtual ~HidroMatrix();

      /**
       * @brief Alternative Constructor.
       *
       * @param lines Number of lines.
       * @param columns Number of columns.
       * @param raster TeRaster that will be converted in HidroMatrix.
       * @param dataType TeUNSIGNEDCHAR for LDD TeFLOAT for DEM.
       * @note The default mamory policy is RAMMemPol.
       */
      HidroMatrix( unsigned int lines, unsigned int columns, TeRaster* raster, TeDataType dataType, float verticalContrast = 1 );     

      void setDummy( T dummy );

      void setVerticalContrast( float verticalContrast );

      T getDummy();

      bool hasDummy();

      bool getElement( unsigned int col, unsigned int line, T &val );

      void setElement( unsigned int col, unsigned int line, T val );

      //! Close and Save the matrix in database as TeRaster.
      bool save( TeDatabase* db, std::string layerName, TeDataType dataType );

      void freeMemory();

      TeRasterParams rasterParams;

  protected:
    T dummy_;
    bool hasDummy_;
    double verticalContrast_;    
  };

  template< class T >
  HidroMatrix< T >::HidroMatrix() : TePDIMatrix<T>(), verticalContrast_(1)
  {    
  } 
  

  // Main Constructor used in TeHidro
  template< class T >
    HidroMatrix< T >::HidroMatrix( unsigned int lines, unsigned int columns, TeRaster* raster, TeDataType dataType, float verticalContrast = 1 ) : verticalContrast_( verticalContrast )
  {
    Reset( lines, columns, TePDIMatrix<T>::AutoMemPol, maxTmpFileSize_/10, 5 );    

    // Copy the raster parameters because the raster will be deleted
    rasterParams = raster->params();

    int demColumns = rasterParams.ncols_; // number of columns from input raster
    int demLines = rasterParams.nlines_; // number of rows from input raster

    // LDD
    if( dataType == TeUNSIGNEDCHAR )
    {    
      setDummy( LDD_DUMMY );
      for (int lin = 0; lin < demLines; lin++)
	    {
		    for (int col = 0; col < demColumns; col++)
        {          
          getScanLine( lin )[ col ] = LDD_DUMMY;          
        }
      }
    }
    else // DEM
    {
      hasDummy_ = false;
      double demDummy = -TeMAXFLOAT;
      if( rasterParams.useDummy_ )
      {
        demDummy = rasterParams.dummy_[0];
        setDummy( demDummy );
        hasDummy_ = true;
      }
      double val = 0;
      for(int lin = 0; lin < demLines; lin++)
	    {
		    for(int col = 0; col < demColumns; col++)
        {
          if( raster->getElement( col, lin, val ) )
          {
            getScanLine( lin )[ col ] =  val;
          }
          else
          {
            getScanLine( lin )[ col ] = demDummy;
          }
        }
      }
    }
  }


  template< class T >
    HidroMatrix< T >::~HidroMatrix()
  {    
  }

  template< class T >
  void HidroMatrix< T >::setDummy( T dummy )
  {
    dummy_ = dummy;
    hasDummy_ = true;
  }

  template< class T >
  T HidroMatrix< T >::getDummy()
  {
    return dummy_;
  }

  template< class T >
  bool HidroMatrix< T >::hasDummy()
  {
    return hasDummy_;
  }

  template< class T >
  bool HidroMatrix< T >::getElement( unsigned int column, unsigned int line, T &val )
  {
    val = getScanLine( line )[ column ];

    if( val == dummy_ )
      return false;

    return true;
  }

  template< class T >
  void HidroMatrix< T >::setElement( unsigned int column, unsigned int line, T val )
  {
    getScanLine( line )[ column ] = val;
  }

  template< class T >
  void HidroMatrix< T >::freeMemory()
  {
    clear();
  }

  template< class T >
  void HidroMatrix< T >::setVerticalContrast( float verticalContrast )
  {
    verticalContrast_ = verticalContrast;

    int demColumns = rasterParams.ncols_; // number of columns from input raster
    int demLines = rasterParams.nlines_; // number of rows from input raster

    T altimetriaVal = 0;
    for( unsigned int lin = 0; lin < demLines; ++lin )
	  {
		  for( unsigned int col = 0; col < demColumns; ++col )
      {			
			  if( getElement(col, lin, altimetriaVal) )
        {        
          getScanLine( lin )[ col ] = ( altimetriaVal * verticalContrast_ ) ;
        }
      }
    }
  }

  template< class T >
  bool HidroMatrix< T >::save( TeDatabase* db, std::string layerName, TeDataType dataType )
  {
    // check if layer name already exits in the database
    int i = 0;
	  while( db->layerExist(layerName) )
	  {
		  i++;
		  layerName = layerName + "_" + Te2String(i);
	  }

    // raster params
    TeRasterParams params( rasterParams );
    params.decoderIdentifier_ = "SMARTMEM";
    params.mode_ = 'w';

    //if( T == unsigned char )
    if( dataType == TeUNSIGNEDCHAR )
    {
      params.useDummy_ = true;
      params.setDummy( LDD_DUMMY ); // Dummy value for unidirection LDD      
      params.setDataType( TeUNSIGNEDCHAR ); 
    }
    else
    {    
      params.setDataType( TeFLOAT );
    }

    TeRaster* raster = new TeRaster( params );
    
    // verify if output raster created is valid
    if( !raster->init() )
    {
      QMessageBox::warning(0, "Warning", "Error create TeRaster.");
  	  delete raster;
	    return false;
    }

    // transform in TeRaster
    unsigned int rasterColumns = rasterParams.ncols_; // number of columns from input raster
    unsigned int rasterLines = rasterParams.nlines_; // number of rows from input raster

    T rasterValue;
    T vmax = -TeMAXFLOAT;
    T vmin =  TeMAXFLOAT;
    
    for( unsigned int lin = 0; lin < rasterLines; ++lin )
	  {
		  for( unsigned int col = 0; col < rasterColumns; ++col )
      {			
        if( getElement(col, lin, rasterValue) )
        {
          T val = rasterValue / verticalContrast_;
          if(val > vmax) vmax = val;
          if(val < vmin) vmin = val;
          raster->setElement( col, lin, val );
        }
        else
        {
          raster->setElement( col, lin, params.dummy_[0] );
        }
      }
    }
    raster->params().vmax_[0] = vmax;
    raster->params().vmin_[0] = vmin;

    TeLayer* layer = new TeLayer(layerName, db, params.projection() );
	  if (layer->id() <= 0)
    {
		  delete layer;
		  delete raster;
		  return false;
	  }

    unsigned int blockW = rasterColumns;
	  unsigned int blockH = rasterLines;
    if( raster->params().ncols_ > 512 )
	  {
		  blockW = 512;	
	  }
	  if( raster->params().nlines_ > 512 )
    {
      blockH = 512;
    }  
    
	  if (!TeImportRaster(layer, raster, blockW, blockH, raster->params().compression_[0], 
		  "", raster->params().dummy_[0], raster->params().useDummy_, raster->params().tiling_type_))
    {
		  delete layer;
		  delete raster;
		  return false;
	  }
    	  
    delete raster;
    raster = 0;

    //this->clear();

    return true;
  }
  

#endif // HIDRO_MATRIX_H_
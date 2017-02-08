#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIParameters.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>
#include <TePDIRaster2Vector.hpp>
#include <TeInitRasterDecoders.h>

#include <TeProgress.h>
#include <TeStdIOProgress.h>

#include <shapefil.h> // Needed by exportPS2SHP

bool exportPS2SHP( const TePolygonSet& ps, 
    const std::string& base_file_name  )
{
    // creating files names
    std::string dbfFilename = base_file_name + ".dbf";
    std::string shpFilename = base_file_name + ".shp";

    // creating polygons attribute list ( max attribute size == 12 )
    TeAttributeList attList;
    
    TeAttribute at;
    at.rep_.type_ = TeSTRING;               //the id of the cell
    at.rep_.numChar_ = 10;
    at.rep_.name_ = "object_id_";
    at.rep_.isPrimaryKey_ = true;
    
    attList.push_back(at);
    
    /* DBF output file handle creation */

    DBFHandle hDBF = DBFCreate( dbfFilename.c_str() );
    TEAGN_TRUE_OR_RETURN( ( hDBF != 0 ), "DBF file creation error" );
    
    /* Writing attributes */

    TeAttributeList::iterator it=attList.begin();
    while ( it != attList.end() )
    {
      TeAttribute at = (*it);
      string atName = at.rep_.name_;

      // *OBS****atributos podem ter no maximo 12 caracteres
      // max attribute size == 12
      if (at.rep_.type_ == TeSTRING )
      {
        TEAGN_TRUE_OR_THROW( 
          ( DBFAddField( hDBF, atName.c_str(), FTString, at.rep_.numChar_, 0 ) 
           != -1 ), "Error writing TeSTRING attribute" );
      }
      else if (at.rep_.type_ == TeINT)
      {
        TEAGN_TRUE_OR_THROW( 
          ( DBFAddField( hDBF, atName.c_str(), FTInteger, 10, 0 ) != -1 ), 
          "Error writing TeINT attribute" );
      }
      else if (at.rep_.type_ == TeREAL)
      {
        TEAGN_TRUE_OR_THROW( 
          ( DBFAddField( hDBF, atName.c_str(), FTDouble, 10, 5 ) != -1 ), 
          "Error writing TeREAL attribute" );
          
      }
      else if (at.rep_.type_ == TeDATETIME)
      {
        TEAGN_TRUE_OR_THROW( 
          ( DBFAddField( hDBF, atName.c_str(), FTDate, 8, 0 ) != -1 ), 
          "Error writing TeDATETIME attribute" );
      }
                
      ++it;
    }
    
    /* SHP output file handle creation */

    SHPHandle hSHP = SHPCreate( shpFilename.c_str(), SHPT_POLYGON );
    if( hSHP == 0 ) {
      TEAGN_LOGERR( "DBF file creation error" );
      DBFClose( hDBF );
      return false;
    }
    
    /* Writing polygons */

    int iRecord = 0;
    int totpoints = 0;
    double  *padfX, *padfY;
    SHPObject       *psObject;
    int posXY, npoints, nelem;
    int nVertices;
    int* panParts;

    TePolygonSet::iterator itps;
    TePolygon poly;

    for (itps = ps.begin() ; itps != ps.end() ; itps++ ) {
      poly=(*itps);
      totpoints = 0;
      nVertices = poly.size();
      for (unsigned int n=0; n<poly.size();n++) {
        totpoints += poly[n].size();
      }

      panParts = (int *) malloc(sizeof(int) * nVertices);
      padfX = (double *) malloc(sizeof(double) * totpoints);
      padfY = (double *) malloc(sizeof(double) * totpoints);
      posXY = 0;
      nelem = 0;
      
      for (unsigned int l=0; l<poly.size(); ++l) {
        if (l==0) {
          if (TeOrientation(poly[l]) == TeCOUNTERCLOCKWISE) {
            TeReverseLine(poly[l]);
          }
        } else {
          if (TeOrientation(poly[l]) == TeCLOCKWISE) {
            TeReverseLine(poly[l]);
          }
        }
        
        npoints = poly[l].size();
        panParts[nelem]=posXY;
        
        for (int m=0; m<npoints; m++ ) {
          padfX[posXY] = poly[l][m].x_;
          padfY[posXY] = poly[l][m].y_;
          posXY++;
        }
        
        nelem++;
      }
                
      psObject = SHPCreateObject( SHPT_POLYGON, -1, nelem, panParts, NULL,
        posXY, padfX, padfY, NULL, NULL );
        
      int shpRes = SHPWriteObject( hSHP, -1, psObject );
      TEAGN_TRUE_OR_THROW( ( shpRes != -1 ), 
        "Unable to create a shape write object" )
        
      SHPDestroyObject( psObject );
      free( panParts );
      free( padfX );
      free( padfY );

      // writing attributes - same creation order
      for (unsigned int j=0; j<attList.size();j++) {
        if ( attList[j].rep_.type_ == TeSTRING ) {
          DBFWriteStringAttribute(hDBF, iRecord, j, poly.objectId().c_str() );
        } /*else if ( attList[j].rep_.type_ == TeINT) {
          DBFWriteIntegerAttribute(hDBF, iRecord, j,  VALOR INT );        
        } else if ( attList[j].rep_.type_ == TeREAL) {
          DBFWriteDoubleAttribute(hDBF, iRecord, j,  VALOR DOUBLE);
        } else if ( attList[j].rep_.type_ == TeDATETIME) {
          TeTime time =  VALOR DATA;
          char dd[8];
          sprintf(dd,"%04d%02d%02d",time.year(),time.month(),time.day());
          DBFWriteDateAttribute(hDBF, iRecord, j, dd );
        }*/
      }
                
      iRecord++;
    }
        
    DBFClose( hDBF );
    SHPClose( hSHP );

    return true;  
}

TePDITypes::TePDIRasterPtrType generateTestRaster()
{
  TeRasterParams RAMRaster_params;
  RAMRaster_params.nBands( 1 );
  RAMRaster_params.setDataType( TeUNSIGNEDLONG );
  RAMRaster_params.setDummy( 0, -1 );
  RAMRaster_params.setNLinesNColumns( 100, 100 );
  
  TePDITypes::TePDIRasterPtrType RAMRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( RAMRaster,
    RAMRaster_params, false ), "RAM Raster Alloc error" );
    
  int line = 0;
  int col = 0;
  
  /* Area 1 */
    
  for( line = 10 ; line < 20; ++line ) {
    for( col = 10 ; col < 20 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 10.0, 0 ), "Raster fill error" );
    }
  }
  
  /* Area 1 dummy hole */
    
  for( line = 13 ; line < 18; ++line ) {
    for( col = 13 ; col < 18 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 0.0, 0 ), "Raster fill error" );
    }
  }  
  
  /* Area 2 */
  
  for( line = 10 ; line < 20; ++line ) {
    for( col = 30 ; col < 40 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 20.0, 0 ), "Raster fill error" );
    }
  }  
  
  /* Area 2 hole */
    
  for( line = 13 ; line < 18; ++line ) {
    for( col = 33 ; col < 38 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 10.0, 0 ), "Raster fill error" );
    }
  }  
  
  /* Area 3 */
  
  for( line = 10 ; line < 20; ++line ) {
    for( col = 50 ; col < 60 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 30.0, 0 ), "Raster fill error" );
    }
  } 
  
  /* Area 4 */
  
  for( line = 0 ; line < 9; ++line ) {
    for( col = 70 ; col < 100 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 40.0, 0 ), "Raster fill error" );
    }
  } 
  
  for( line = 0 ; line < 40; ++line ) {
    for( col = 90 ; col < 100 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 40.0, 0 ), "Raster fill error" );
    }
  } 
  
  for( line = 30 ; line < 40; ++line ) {
    for( col = 70 ; col < 100 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 40.0, 0 ), "Raster fill error" );
    }
  } 
  
  for( line = 20 ; line < 30; ++line ) {
    for( col = 70 ; col < 80 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 40.0, 0 ), "Raster fill error" );
    }
  }   
  
  /* Area 5 */
  
  for( line = 50 ; line < 90; ++line ) {
    for( col = 10 ; col < 90 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 50.0, 0 ), "Raster fill error" );
    }
  }
  
  /* Area 6 */
    
  for( line = 60 ; line < 70; ++line ) {
    for( col = 20 ; col < 30 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 10.0, 0 ), "Raster fill error" );
    }
  }    
  
  /* Area 7 */
    
  for( line = 60 ; line < 70; ++line ) {
    for( col = 40 ; col < 50 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 20.0, 0 ), "Raster fill error" );
    }
  }    

  /* Area 8 */
    
  for( line = 60 ; line < 70; ++line ) {
    for( col = 60 ; col < 70 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 30.0, 0 ), "Raster fill error" );
    }
  }    

  /* Area 9 */
    
  for( line = 70 ; line < 80; ++line ) {
    for( col = 20 ; col < 70 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 0.0, 0 ), "Raster fill error" );
    }
  }  
  
  /* Area 10 */
    
  for( line = 95 ; line < 100; ++line ) {
    for( col = 95 ; col < 100 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 60.0, 0 ), "Raster fill error" );
    }
  }     
  
  /* Area 11 */
    
  for( line = 0 ; line < 5; ++line ) {
    for( col = 0 ; col < 5 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 60.0, 0 ), "Raster fill error" );
    }
  }  
  
  /* Area 12 */
    
  for( line = 95 ; line < 100; ++line ) {
    for( col = 0 ; col < 5 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 60.0, 0 ), "Raster fill error" );
    }
  }      
  
  /* Area 13 */
    
  for( line = 40 ; line < 43; ++line ) {
    for( col = 10 ; col < 13 ; ++col ) {
      TEAGN_TRUE_OR_THROW( 
        RAMRaster->setElement( col, line, 60.0, 0 ), "Raster fill error" );
    }
  }   
  
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 11, 39, 60.0, 0 ), "Raster fill error" );   
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 13, 41, 60.0, 0 ), "Raster fill error" );   
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 11, 43, 60.0, 0 ), "Raster fill error" );   
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 9, 41, 60.0, 0 ), "Raster fill error" );   
    
  /* Area 14 */
    
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 31, 39, 60.0, 0 ), "Raster fill error" );   
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 31, 40, 60.0, 0 ), "Raster fill error" );   
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 31, 41, 60.0, 0 ), "Raster fill error" );   
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 31, 42, 60.0, 0 ), "Raster fill error" );   
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 31, 43, 60.0, 0 ), "Raster fill error" );   

  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 29, 41, 60.0, 0 ), "Raster fill error" );   
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 30, 41, 60.0, 0 ), "Raster fill error" );   
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 31, 41, 60.0, 0 ), "Raster fill error" );   
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 32, 41, 60.0, 0 ), "Raster fill error" );   
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 33, 41, 60.0, 0 ), "Raster fill error" );   
        
  /* Area 15 */
  
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 42, 39, 60.0, 0 ), "Raster fill error" );  
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 46, 39, 60.0, 0 ), "Raster fill error" );  
    
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 43, 40, 60.0, 0 ), "Raster fill error" );
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 45, 40, 60.0, 0 ), "Raster fill error" );
    
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 44, 41, 60.0, 0 ), "Raster fill error" );
    
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 43, 42, 60.0, 0 ), "Raster fill error" );
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 45, 42, 60.0, 0 ), "Raster fill error" );
    
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 42, 43, 60.0, 0 ), "Raster fill error" );
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 46, 43, 60.0, 0 ), "Raster fill error" );
    
  /* Area 16 */
    
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 55, 39, 60.0, 0 ), "Raster fill error" );  
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 56, 40, 60.0, 0 ), "Raster fill error" );      
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 57, 41, 60.0, 0 ), "Raster fill error" );      
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 58, 42, 60.0, 0 ), "Raster fill error" );      
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 59, 43, 60.0, 0 ), "Raster fill error" );      
    
  /* Area 17 */
    
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 10, 30, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 11, 29, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 12, 28, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 13, 27, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 14, 26, 60.0, 0 ), "Raster fill error" );     
    
  /* Area 18 */
    
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 10, 2, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 11, 2, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 9, 3, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 10, 3, 60.0, 0 ), "Raster fill error" ); 

  /* Area 19 */
    
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 15, 2, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 16, 2, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 16, 3, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 17, 3, 60.0, 0 ), "Raster fill error" ); 
    
  /* Area 20 */
    
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 20, 2, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 20, 3, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 21, 3, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 21, 4, 60.0, 0 ), "Raster fill error" );    
    
  /* Area 20 */
    
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 25, 3, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 25, 4, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 26, 3, 60.0, 0 ), "Raster fill error" ); 
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 26, 2, 60.0, 0 ), "Raster fill error" );     
    
                      
  /* Line 1 */
    
  for( line = 35 ; line < 55; ++line ) {
    TEAGN_TRUE_OR_THROW( 
      RAMRaster->setElement( 85, line, 200.0, 0 ), "Raster fill error" );
  } 
  
  /* Line 2 */
    
  for( col = 25 ; col < 45; ++col ) {
    TEAGN_TRUE_OR_THROW( 
      RAMRaster->setElement( col, 65, 10.0, 0 ), "Raster fill error" );
  }      
  
  /* Points */

  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 55, 15, 250.0, 0 ), "Raster fill error" );  
    
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 55, 5, 251.0, 0 ), "Raster fill error" );      
    
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 15, 15, 251.0, 0 ), "Raster fill error" );     

  TEAGN_TRUE_OR_THROW( 
    RAMRaster->setElement( 35, 15, 0, 0 ), "Raster fill error" );     
      
  return RAMRaster; 
}


void Raster2Vector_test()
{
  TePDITypes::TePDIRasterPtrType RAMRaster = generateTestRaster();
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( RAMRaster,
    TEPDIEXAMPLESBINPATH "Raster2Vector_test.tif" ), "GeoTIF generation error" );
    
  TePDITypes::TePDIPolSetMapPtrType output_polsets( 
    new TePDITypes::TePDIPolSetMapType );

  TePDIParameters params2;
  params2.SetParameter( "rotulated_image", RAMRaster );
  params2.SetParameter( "channel", (unsigned int)0 );
  params2.SetParameter( "output_polsets", output_polsets );
  params2.SetParameter( "max_pols", (unsigned long int)200 );
  
  TePDIRaster2Vector raster2Vector;  

  TEAGN_TRUE_OR_THROW( raster2Vector.Reset( params2 ),
    "Invalid Parameters for raster2Vector" );

  TEAGN_TRUE_OR_THROW( raster2Vector.Apply(),
    "Apply error" );
    
  TePDITypes::TePDIPolSetMapType::iterator it = output_polsets->begin();
  TePDITypes::TePDIPolSetMapType::iterator it_end = output_polsets->end();
  
  unsigned int pols_number = 0;
    
  while( it != it_end ) {
    TEAGN_TRUE_OR_THROW( exportPS2SHP( it->second, TEPDIEXAMPLESBINPATH "Raster2Vector_test_ps" +
      Te2String( (int)it->first ) ),  "Polygonset export error" )
      
    pols_number += it->second.size();
      
    ++it;
  }
  
  TEAGN_CHECK_EPS( output_polsets->size(), 9, 0.0,
    "Invalid generated polygon set size" );
  TEAGN_CHECK_EPS( pols_number, 41, 0.0,
    "Invalid generated polygon set size" );
    
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );  
  
    TeInitRasterDecoders();

    Raster2Vector_test();
  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

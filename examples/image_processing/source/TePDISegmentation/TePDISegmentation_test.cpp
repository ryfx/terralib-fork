#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TeAgnostic.h>
#include <TePDIUtils.hpp>
#include <TePDIRegGrowSeg.hpp>
#include <TePDIBaatz.hpp>
#include <TeInitRasterDecoders.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>

#include <shapefil.h> // Needed by exportPS2SHP

#include <time.h>

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


bool checkNumberCells(TePDITypes::TePDIRasterPtrType outRaster, long nbCell)
{
        double val;
        long max=0;
        for (int i=0 ; i<outRaster->params().nlines_ ; i++)
                for (int j=0 ; j<outRaster->params().ncols_ ; j++){
                        outRaster->getElement(j,i,val,0);
                        if (val>max)
                                max=(long)val;
                }
        if (max!=nbCell)
                return false;

        return true;
}


void TePDIRegGrowSeg_test()
{
  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, true, TeUNSIGNEDLONG, 0 ), "RAM Raster Alloc error" );    
  params.SetParameter( "output_image", outRaster );

  TePDITypes::TePDIRasterPtrType dummy_ptr;
  params.SetParameter( "exclusion_image", dummy_ptr );
  params.SetParameter( "euc_treshold", (double)20 );
  params.SetParameter( "area_min", (int)15 );
  
  TePDITypes::TePDIPolSetMapPtrType output_polsets( 
    new TePDITypes::TePDIPolSetMapType );
  params.SetParameter( "output_polsets", output_polsets );
  
  TePDIRegGrowSeg segmenter;
  
  TEAGN_TRUE_OR_THROW( segmenter.Reset(params), "Reset failed" );
  
  double initTime = ((double)clock()) / ((double)CLOCKS_PER_SEC) ;
  
  TEAGN_TRUE_OR_THROW( segmenter.Apply(), "Apply error" ); 
   
  double endTime = ((double)clock()) / ((double)CLOCKS_PER_SEC) ;
  
  TEAGN_WATCH( endTime - initTime );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Segmentation_RegionGrowing_test.tif", TeUNSIGNEDCHAR ), 
    "GeoTIF generation error" );   
  
  TePDITypes::TePDIPolSetMapType::iterator it = output_polsets->begin();
  TePDITypes::TePDIPolSetMapType::iterator it_end = output_polsets->end();  
  
  unsigned int pols_number = 0;
    
  while( it != it_end ) {
    TEAGN_TRUE_OR_THROW( exportPS2SHP( it->second, 
      TEPDIEXAMPLESBINPATH "Segmentation_RegionGrowingPols_ps" +
      Te2String( (int)it->first ) ),  "Polygonset export error" )
      
    pols_number += it->second.size();
      
    ++it;
  }
  
  TEAGN_CHECK_EPS( output_polsets->size(), 23, 0.0,
    "Invalid generated polygon set size" );
  TEAGN_CHECK_EPS( pols_number, 23, 0.0,
    "Invalid generated polygon set size" );    

  //with an euclidian treshold of 20 and an area min of 15
  //you are suppose to find 23 cells
  //(you can use the spring software to check that number with
  // others images and/or paramaters)
  TEAGN_TRUE_OR_THROW( checkNumberCells(outRaster, 23),
    "Check number of cell Error" );
}

void TePDIBaatz_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  std::vector<unsigned> input_channels;
  input_channels.push_back( 0 );
  input_channels.push_back( 1 );
  input_channels.push_back( 2 );
  params.SetParameter( "input_channels", input_channels );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
  1, 1, 1, true, TeUNSIGNEDLONG, 0 ), "RAM Raster Alloc error" );    
  params.SetParameter( "output_image", outRaster );
  params.SetParameter( "scale", (float) 30 );
  params.SetParameter( "compactness", (float) 0.4 );
  params.SetParameter( "color", (float) 0.5 );

  vector<float> input_weights;
  input_weights.push_back( (float) 0.7 );
  input_weights.push_back( (float) 0.2 );
  input_weights.push_back( (float) 0.1 );
  params.SetParameter( "input_weights", input_weights );

  TePDITypes::TePDIPolSetMapPtrType output_polsets( 
  new TePDITypes::TePDIPolSetMapType );
  params.SetParameter( "output_polsets", output_polsets );

  TePDIBaatz segmenter;

  TEAGN_TRUE_OR_THROW( segmenter.Reset(params), "Reset failed" );
  
  double initTime = ((double)clock()) / ((double)CLOCKS_PER_SEC) ;

  TEAGN_TRUE_OR_THROW( segmenter.Apply(), "Apply error" );
    
  double endTime = ((double)clock()) / ((double)CLOCKS_PER_SEC) ;
  
  TEAGN_WATCH( endTime - initTime );    

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Segmentation_Baatz_test.tif", TeUNSIGNEDCHAR ), 
    "GeoTIF generation error" );   

  TePDITypes::TePDIPolSetMapType::iterator it = output_polsets->begin();
  TePDITypes::TePDIPolSetMapType::iterator it_end = output_polsets->end();  
  TePolygonSet pols;
  while( it != it_end ) 
  {
    pols.copyElements(it->second);
    ++it;
  }
  exportPS2SHP( pols, TEPDIEXAMPLESBINPATH "Segmentation_BaatzPols_ps");
}


int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );     
  
    TeInitRasterDecoders();
    
    TePDIBaatz_test();
    TePDIRegGrowSeg_test();
  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}


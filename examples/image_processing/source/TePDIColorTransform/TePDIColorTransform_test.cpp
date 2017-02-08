#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIColorTransform.hpp>
#include <TePDIParameters.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>

#include <TeRaster.h>
#include <TeInitRasterDecoders.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>


void RGB2IHS_IHS2RGB_image_test()
{
  TePDITypes::TePDIRasterPtrType IHSRasterPtr;
  
  /* Testing RGB -> IHS */
  {
    TePDIParameters params;
    
    TePDITypes::TePDIRasterPtrType rgb_raster( new TeRaster(
      std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
    TEAGN_TRUE_OR_THROW( rgb_raster->init(), 
      "Unable to init input_raster1" );  
    
    TePDITypes::TePDIRasterVectorType input_rasters;
    input_rasters.push_back( rgb_raster );
    input_rasters.push_back( rgb_raster );
    input_rasters.push_back( rgb_raster );
    params.SetParameter( "input_rasters", input_rasters );
      
    std::vector< int > input_channels;
    input_channels.push_back( 0 );
    input_channels.push_back( 1 );
    input_channels.push_back( 2 );
    params.SetParameter( "input_channels", input_channels );   
    
    TeRasterParams outRaster_params;
    outRaster_params.nBands( 3 );
    outRaster_params.setNLinesNColumns( 1, 1 );
    outRaster_params.setDataType( TeFLOAT, -1 );
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( IHSRasterPtr,
      outRaster_params, false ), "output_raster Alloc error" );  
      
    TePDITypes::TePDIRasterVectorType output_rasters;
    output_rasters.push_back( IHSRasterPtr );
    params.SetParameter( "output_rasters", output_rasters );    
      
    params.SetParameter( "transf_type", TePDIColorTransform::Rgb2Ihs ); 
    params.SetParameter( "rgb_channels_min", (double)0.0 );
    params.SetParameter( "rgb_channels_max", (double)255.0 );    
    
    TePDIColorTransform ct;

    TEAGN_TRUE_OR_THROW( ct.Reset( params ),
      "Invalid Parameters" );

    TEAGN_TRUE_OR_THROW( ct.Apply(),
      "Apply error" );   

    TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( IHSRasterPtr,
      TEPDIEXAMPLESBINPATH 
      "TePDIColorTransform_RGB2IHS_IHS2RGB_image_test_ihs.tif" ), 
      "GeoTIF generation error" );
  }
  
  TePDITypes::TePDIRasterPtrType RGBRasterPtr;

  /* Testing IHS -> RGB */
  {
    TePDIParameters params;
    
    TePDITypes::TePDIRasterVectorType input_rasters;
    input_rasters.push_back( IHSRasterPtr );
    input_rasters.push_back( IHSRasterPtr );
    input_rasters.push_back( IHSRasterPtr );
    params.SetParameter( "input_rasters", input_rasters );
      
    std::vector< int > input_channels;
    input_channels.push_back( 0 );
    input_channels.push_back( 1 );
    input_channels.push_back( 2 );
    params.SetParameter( "input_channels", input_channels );   
    
    TeRasterParams outRaster_params;
    outRaster_params.nBands( 3 );
    outRaster_params.setNLinesNColumns( 1, 1 );
    outRaster_params.setDataType( TeUNSIGNEDCHAR, -1 );
    outRaster_params.setPhotometric( TeRasterParams::TeRGB, -1 );
    
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( RGBRasterPtr,
      outRaster_params, false ), "output_raster Alloc error" );  
      
    TePDITypes::TePDIRasterVectorType output_rasters;
    output_rasters.push_back( RGBRasterPtr );
    params.SetParameter( "output_rasters", output_rasters );    
      
    params.SetParameter( "transf_type", TePDIColorTransform::Ihs2Rgb ); 
    
    params.SetParameter( "rgb_channels_min", (double)0.0 );
    params.SetParameter( "rgb_channels_max", (double)255.0 );    
    
    TePDIColorTransform ct;

    TEAGN_TRUE_OR_THROW( ct.Reset( params ),
      "Invalid Parameters" );

    TEAGN_TRUE_OR_THROW( ct.Apply(),
      "Apply error" );   

    TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( RGBRasterPtr,
      TEPDIEXAMPLESBINPATH 
      "TePDIColorTransform_RGB2IHS_IHS2RGB_image_test_rgb.tif" ), 
      "GeoTIF generation error" );
  }
}

void RGB2IHS_IHS2RGB_numeric_test()
{
  /* Testing RGB -> IHS */

  TeRasterParams inRaster_params;
  inRaster_params.mode_ = 'c';
  inRaster_params.nBands( 3 );
  inRaster_params.setNLinesNColumns( 1, 4 );
  inRaster_params.setDataType( TeUNSIGNEDCHAR, -1 );
  inRaster_params.decoderIdentifier_ = "SMARTMEM";
  inRaster_params.setPhotometric( TeRasterParams::TeMultiBand, -1 );
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster( inRaster_params ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), 
    "inRaster allocation error" );
    
  TEAGN_TRUE_OR_THROW( inRaster->setElement( 0, 0, 50., 0 ), "value set" );
  TEAGN_TRUE_OR_THROW( inRaster->setElement( 0, 0, 50., 1 ), "value set" );
  TEAGN_TRUE_OR_THROW( inRaster->setElement( 0, 0, 50., 2 ), "value set" );
  TEAGN_TRUE_OR_THROW( inRaster->setElement( 1, 0, 0., 0 ), "value set" );
  TEAGN_TRUE_OR_THROW( inRaster->setElement( 1, 0, 200., 1 ), "value set" );
  TEAGN_TRUE_OR_THROW( inRaster->setElement( 1, 0, 255., 2 ), "value set" );
  TEAGN_TRUE_OR_THROW( inRaster->setElement( 2, 0, 0.0, 0 ), "value set" );
  TEAGN_TRUE_OR_THROW( inRaster->setElement( 2, 0, 0.0, 1 ), "value set" );
  TEAGN_TRUE_OR_THROW( inRaster->setElement( 2, 0, 0.0, 2 ), "value set" );
  TEAGN_TRUE_OR_THROW( inRaster->setElement( 3, 0, 255.0, 0 ), "value set" );
  TEAGN_TRUE_OR_THROW( inRaster->setElement( 3, 0, 255.0, 1 ), "value set" );
  TEAGN_TRUE_OR_THROW( inRaster->setElement( 3, 0, 255.0, 2 ), "value set" );

  TeRasterParams outRaster_params;
  outRaster_params.mode_ = 'c';
  outRaster_params.nBands( 3 );
  outRaster_params.setNLinesNColumns( 1, 1 );
  outRaster_params.setDataType( TeDOUBLE, -1 );
  outRaster_params.decoderIdentifier_ = "SMARTMEM";
  TePDITypes::TePDIRasterPtrType outRaster( new TeRaster( outRaster_params ) );
  TEAGN_TRUE_OR_THROW( outRaster->init(), 
    "outRaster allocation error" );
    
  TePDIParameters params;
  
  TePDITypes::TePDIRasterVectorType input_rasters;
  input_rasters.push_back( inRaster );
  input_rasters.push_back( inRaster );
  input_rasters.push_back( inRaster );
  params.SetParameter( "input_rasters", input_rasters );
  
  std::vector< int > input_channels;
  input_channels.push_back( 0 );
  input_channels.push_back( 1 );
  input_channels.push_back( 2 );
  params.SetParameter( "input_channels", input_channels );
  
  TePDITypes::TePDIRasterVectorType output_rasters;
  output_rasters.push_back( outRaster );
  params.SetParameter( "output_rasters", output_rasters );
  
  params.SetParameter( "transf_type", TePDIColorTransform::Rgb2Ihs );
  params.SetParameter( "rgb_channels_min", (double)0.0 );
  params.SetParameter( "rgb_channels_max", (double)255.0 );   

  TePDIColorTransform ct;

  TEAGN_TRUE_OR_THROW( ct.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( ct.Apply(),
    "Apply error" );

  double value;
  TEAGN_TRUE_OR_THROW( outRaster->getElement( 0, 0, value, 0 ), "Value get" );
  TEAGN_CHECK_EPS( value, 0.196078, 0.0001,
    "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster->getElement( 0, 0, value, 1 ), "Value get" );
  TEAGN_CHECK_EPS( value, 0.0, 0.00001,
    "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster->getElement( 0, 0, value, 2 ), "Value get" );
  TEAGN_CHECK_EPS( value, 0.0, 0.0001,
    "Invalid value" );
    
  TEAGN_TRUE_OR_THROW( outRaster->getElement( 1, 0, value, 0 ), "Value get" );
  TEAGN_CHECK_EPS( value, 0.594771, 0.0001,
    "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster->getElement( 1, 0, value, 1 ), "Value get" );
  TEAGN_CHECK_EPS( value, 3.347980, 0.00001,
    "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster->getElement( 1, 0, value, 2 ), "Value get" );
  TEAGN_CHECK_EPS( value, 1.0, 0.0001,
    "Invalid value" );   
     
  TEAGN_TRUE_OR_THROW( outRaster->getElement( 2, 0, value, 0 ), "Value get" );
  TEAGN_CHECK_EPS( value, 0.0, 0.0001,
    "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster->getElement( 2, 0, value, 1 ), "Value get" );
  TEAGN_CHECK_EPS( value, 0.0, 0.00001,
    "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster->getElement( 2, 0, value, 2 ), "Value get" );
  TEAGN_CHECK_EPS( value, 0.0, 0.0001,
    "Invalid value" );     
    
  TEAGN_TRUE_OR_THROW( outRaster->getElement( 3, 0, value, 0 ), "Value get" );
  TEAGN_CHECK_EPS( value, 1.0, 0.0001,
    "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster->getElement( 3, 0, value, 1 ), "Value get" );
  TEAGN_CHECK_EPS( value, 0.0, 0.00001,
    "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster->getElement( 3, 0, value, 2 ), "Value get" );
  TEAGN_CHECK_EPS( value, 0.0, 0.0001,
    "Invalid value" );        

  /* Testing IHS -> RGB */
  
  params.Clear();

  TeRasterParams outRaster2_params;
  outRaster2_params.mode_ = 'c';
  outRaster2_params.nBands( 3 );
  outRaster2_params.setNLinesNColumns( 1, 1 );
  outRaster2_params.setDataType( TeDOUBLE, -1 );
  outRaster2_params.decoderIdentifier_ = "SMARTMEM";
  outRaster2_params.setPhotometric( TeRasterParams::TeMultiBand, -1 );
  TePDITypes::TePDIRasterPtrType outRaster2( new TeRaster( 
    outRaster2_params ) );
  TEAGN_TRUE_OR_THROW( outRaster2->init(), 
    "outRaster2 allocation error" );  
    
  TePDITypes::TePDIRasterVectorType input_rasters2;
  input_rasters2.push_back( outRaster );
  input_rasters2.push_back( outRaster );
  input_rasters2.push_back( outRaster );
  params.SetParameter( "input_rasters", input_rasters2 );
  
  std::vector< int > input_channels2;
  input_channels2.push_back( 0 );
  input_channels2.push_back( 1 );
  input_channels2.push_back( 2 );
  params.SetParameter( "input_channels", input_channels2 );    
  
  TePDITypes::TePDIRasterVectorType output_rasters2;
  output_rasters2.push_back( outRaster2 );
  params.SetParameter( "output_rasters", output_rasters2 );  

  params.SetParameter( "transf_type", TePDIColorTransform::Ihs2Rgb );
  params.SetParameter( "rgb_channels_min", (double)0.0 );
  params.SetParameter( "rgb_channels_max", (double)255.0 );

  TEAGN_TRUE_OR_THROW( ct.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( ct.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( outRaster2->getElement( 0, 0, value, 0 ), "Value get" );
  TEAGN_CHECK_EPS( value, 50.0, 0.000001, "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster2->getElement( 0, 0, value, 1 ), "Value get" );
  TEAGN_CHECK_EPS( value, 50.0, 0.000001, "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster2->getElement( 0, 0, value, 2 ), "Value get" );
  TEAGN_CHECK_EPS( value, 50.0, 0.000001, "Invalid value" );
  
  TEAGN_TRUE_OR_THROW( outRaster2->getElement( 1, 0, value, 0 ), "Value get" );
  TEAGN_CHECK_EPS( value, 0.0, 0.000001, "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster2->getElement( 1, 0, value, 1 ), "Value get" );
  TEAGN_CHECK_EPS( value, 200.0, 0.000001, "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster2->getElement( 1, 0, value, 2 ), "Value get" );
  TEAGN_CHECK_EPS( value, 255.0, 0.000001, "Invalid value" );  
  
  TEAGN_TRUE_OR_THROW( outRaster2->getElement( 2, 0, value, 0 ), "Value get" );
  TEAGN_CHECK_EPS( value, 0.0, 0.000001, "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster2->getElement( 2, 0, value, 1 ), "Value get" );
  TEAGN_CHECK_EPS( value, 0.0, 0.000001, "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster2->getElement( 2, 0, value, 2 ), "Value get" );
  TEAGN_CHECK_EPS( value, 0.0, 0.000001, "Invalid value" );    
  
  TEAGN_TRUE_OR_THROW( outRaster2->getElement( 3, 0, value, 0 ), "Value get" );
  TEAGN_CHECK_EPS( value, 255.0, 0.000001, "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster2->getElement( 3, 0, value, 1 ), "Value get" );
  TEAGN_CHECK_EPS( value, 255.0, 0.000001, "Invalid value" );
  TEAGN_TRUE_OR_THROW( outRaster2->getElement( 3, 0, value, 2 ), "Value get" );
  TEAGN_CHECK_EPS( value, 255.0, 0.000001, "Invalid value" );      
}


int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );
      
    TeInitRasterDecoders();
    
    RGB2IHS_IHS2RGB_image_test();
    RGB2IHS_IHS2RGB_numeric_test();
  }
  catch( const TeException& excpt ){
    TEAGN_LOGERR( excpt.message() )
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

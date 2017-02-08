#include "EDA_SlopePercent.h"                                                  
                                                                               
bool slopeInPercent(TeRaster& dbAltitudeRaster, TeRaster& slopePercentRaster,  
                    string& msg)                                               
{                                                                              
   char* fT = "EDA Plugin: slope in percent";                                  
   //TeRaster& dbAltituteRaster = *altitudeLayer.raster("", 'r');//in database 
   TeRaster altitudeRaster;//local copy of the database Raster used for speed  
   TeRasterParams altitudeParams =  dbAltitudeRaster.params();                 
   TeRasterParams slopePercentParams= altitudeParams;                          
   //set local memory values for both Rasters                                  
   slopePercentParams.setDataType(TeDOUBLE, -1);                               
   altitudeParams.setDataType(TeDOUBLE, -1);                                   
   slopePercentParams.useDummy_ = true;                                        
   altitudeParams.useDummy_ = true;                                            
   slopePercentParams.setDummy(-TeMAXFLOAT, -1 );                              
   altitudeParams.setDummy(-TeMAXFLOAT, -1 );                                  
   slopePercentParams.mode_ = 'c';//read and write                             
   altitudeParams.mode_ = 'c';//read and write                                 
   slopePercentParams.decoderIdentifier_="SMARTMEM";//this matters for speed   
   altitudeParams.decoderIdentifier_ =   "SMARTMEM";//this matters for speed   
   slopePercentParams.vmin_.clear();                                           
   slopePercentParams.vmin_.push_back( 0.0 );  //general minimum               
   slopePercentParams.vmax_.clear();                                           
   slopePercentParams.vmax_.push_back(100000000);//Percent especific maximum?  
   //make new rasters ready to hold and alter data.                            
   slopePercentRaster.updateParams( slopePercentParams );//set params in Raster
   altitudeRaster.updateParams( altitudeParams );//pass values to new Raster   
   TeDecoder* slpDec = new TeDecoderSmartMem( slopePercentParams );            
   TeDecoder* altDec = new TeDecoderSmartMem( altitudeParams );                
   slopePercentRaster.setDecoder(slpDec);                                      
   altitudeRaster.setDecoder(altDec);                                          
   if( ! slopePercentRaster.init())// make new Raster ready to use             
   {                                                                           
       msg=err(fT, "unable to init output raster");                            
       return false;                                                           
   }                                                                           
   if( ! altitudeRaster.init() )// make new Raster ready to use                
   {                                                                           
       msg=err(fT, "unable to init local input raster");                       
       return false;                                                           
   }                                                                           
   //copy from database Raster to input Raster.                                
   //copy raster contents indiscriminately                                     
   int nBands = dbAltitudeRaster.params().nBands();//number of bands           
   int nRows  = dbAltitudeRaster.params().nlines_;//number of lines            
   int nCols  = dbAltitudeRaster.params().ncols_;//number of colunms           
   double value = 0.0;//value to be copied                                     
   for(int b = 0; b < nBands; b++)//for all bands                              
   {                                                                           
       for(int r = 0; r < nRows; r++)//for all lines                           
       {                                                                       
           for(int c = 0; c < nCols; c++)//for all colunms                     
           {                                                                   
               dbAltitudeRaster.getElement(c, r, value, b);//copy value        
               altitudeRaster.setElement(  c, r, value, b);//get value         
           }//for all cols                                                     
       }//for all line                                                         
   }//for all bands                                                            
   msg=inf(fT, "Raster objects obtained");                                     
   if(!slopeGradientInPercent(altitudeRaster, slopePercentRaster, 0, msg))     
   {                                                                           
       msg=msg+err(fT, "Raster completion failed");                            
       return false;                                                           
   }                                                                           
   return true;                                                                
}                                                                              
                                                                               
bool slopeGradientInPercent(TeRaster& inGrid, TeRaster& outGrid, int band,     
                            string& msg)                                       
{                                                                              
   char* fT = "EDA Plugin: SlopePercent:Gradient";                             
   //Definitions and initializations                                           
   int rows = inGrid.params().nlines_;                                         
   int cols = inGrid.params().ncols_;                                          
   double dzdy, dzdx;//parcial first derivatives of Z                          
   double zvalue; //holder used for all individual values of Z, one at a time  
   //double pi180 = 180./3.1415927;//not necessary with percent                
   //outGrid is used to read the max min and dummy values because they are not 
   // neccessarely the same as for the input inGrid.                           
   double zvaluemin = outGrid.params().vmin_.at(band); // minimum zvalue       
   double zvaluemax = outGrid.params().vmax_.at(band); // maximum zvalue       
   double zvaluedum = outGrid.params().dummy_.at(band);//invalid zvalue        
   string error;                                                               
   //For each point of the input grid except those in the edges                
   int line,column;//SSL0296                                                   
   for(line = 1; line < rows-1; line++)//SSL0296                               
   {                                                                           
       for( column = 1; column < cols-1; column++)                             
       {                                                                       
           //Calculate slope in percent                                        
           if(calcLocalGradient(inGrid, band, line, column, dzdx, dzdy, error))
           {                                                                   
               //zvalue =  (double)(sqrt(((double)dzdx*(double)dzdx)           
               //       + ((double)dzdy*(double)dzdy))*(double)100.);          
               zvalue =  sqrt( (dzdx*dzdx) + (dzdy*dzdy) ) * 100.;             
           }//if(CalcGradientLocal(grid, band, line, column, dzdx, dzdy))      
           else                                                                
               zvalue = zvaluedum;                                             
           //check the minimum and maximum gradient values                     
           if((zvalue < zvaluemin)                                             
           || (zvalue > zvaluemax))//if zvalue is invalid                      
               zvalue = zvaluedum;                                             
           outGrid.setElement(column, line, zvalue, band);                     
       }//for all columns except the first and last columns                    
                                                                               
       //get second column and copy it to first column                         
       if((!outGrid.getElement(1, line, zvalue, band))&&(zvalue != zvaluedum)) 
       {                                                                       
           msg=err(fT,"outGrid.getElement (001, %03d) = %G failed",line,       
                   zvalue);                                                    
           return false;                                                       
       }                                                                       
       if((!outGrid.setElement(0, line, zvalue, band))&&(zvalue != zvaluedum)) 
       {                                                                       
           msg=err(fT,"outGrid.getElement (000, %03d) = %G failed",line,       
                   zvalue);                                                    
           return false;                                                       
       }                                                                       
       //get second to last column and copy it to last column                  
       if((!outGrid.getElement(cols-2, line, zvalue, band))                    
         &&(zvalue != zvaluedum))                                              
       {                                                                       
           msg=err(fT,"outGrid.getElement (%03d, %03d) = %G failed",cols-2,    
                   line, zvalue);                                              
           return false;                                                       
       }                                                                       
       if((!outGrid.setElement(cols-1, line, zvalue, band))                    
                            &&(zvalue != zvaluedum))                           
       {                                                                       
           msg=err(fT,"outGrid.getElement (%03d, %03d) = %G failed",cols-1,    
                   line, zvalue);                                              
           return false;                                                       
       }                                                                       
   }//for all lines except the first and last lines                            
                                                                               
   for( column = 1; column < cols; column++)                                   
   {                                                                           
       //get second line and copy it to first                                  
       if((!outGrid.getElement(column, 1, zvalue, band))                       
                            &&(zvalue != zvaluedum))                           
       {                                                                       
           msg=err(fT,"outGrid.getElement (%03d, 001) = %G failed",column,     
                   zvalue);                                                    
           return false;                                                       
       }                                                                       
       if((!outGrid.setElement(column, 0, zvalue, band))                       
                            &&(zvalue != zvaluedum))                           
       {                                                                       
           msg=err(fT,"outGrid.getElement (%03d, 000) = %G failed",column,     
                   zvalue);                                                    
           return false;                                                       
       }                                                                       
                                                                               
       //get second to last line and copy it to last line                      
       if((!outGrid.getElement(column, rows-2, zvalue, band))                  
        &&(zvalue != zvaluedum))                                               
       {                                                                       
           msg=err(fT,"outGrid.getElement (%03d, %03d) = %G failed",column,    
                   rows-2, zvalue);                                            
           return false;                                                       
       }                                                                       
                                                                               
       if((!outGrid.setElement(column, rows-1, zvalue, band))                  
                            &&(zvalue != zvaluedum))                           
       {                                                                       
           msg=err(fT,"outGrid.getElement (%03d, %03d) = %G failed",column,    
                   rows-1, zvalue);                                            
           return false;                                                       
       }                                                                       
   }//for the first and last lines                                             
   return true;                                                                
}//short slopeGradientInPercent(TeRaster* inGrid, TeRaster* outGrid, int band) 

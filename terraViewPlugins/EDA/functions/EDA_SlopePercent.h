/***************************************************************/              
/* Functions that are especific to calculating lope in percent */              
/***************************************************************/              
                                                                               
#ifndef EDA_SLOPE_PERCENT_H                                                    
#define EDA_SLOPE_PERCENT_H                                                    
                                                                               
#include "EDA_sharedFunctions.h"                                               
                                                                               
//pre-process function                                                         
//The next functoninitalizes "SMARTMEM" vertions of both input and output      
//Rasters. Input Raster is copied completely to memory for faster access during
//proccessment. Coping only takes one access per element O(Rows * Cols), while 
//any gradient processing requires that for every element except the edge ones,
//the client code access all of its 8 surrounding elements.                    
bool slopeInPercent(TeRaster& dbAltitudeRaster, TeRaster& slopePercentRaster,  
                    string& error);                                            
                                                                               
    //! Calculate the the slope gradient of an entire raster object in percent.
    // Each call to this method handles one band only and outGrid must         
    // hold a copy of grid's RasterParam object with its own apropriate        
    // max min and dummy values.                                               
    /*                                                                         
       \Param TeHaster grid: Raster holding the cells                          
       \Param int band: which band of the Raster object that will be used      
       \Param double& outGrid: The resulting Haster                            
    */                                                                         
bool slopeGradientInPercent(TeRaster& grid, TeRaster& outGrid, int band,       
                            string& msg);                                      
                                                                               
#endif                                                                         
                                                                               

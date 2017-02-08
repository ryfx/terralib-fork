/***************************************************************/              
/* Functions that are especific to calculating lope in degrees */              
/***************************************************************/              
                                                                               
#ifndef EDA_SLOPE_DEGREES_H                                                    
#define EDA_SLOPE_DEGREES_H                                                    
                                                                               
#include "EDA_sharedFunctions.h"                                               
                                                                               
//pre-process function                                                         
//The next functoninitalizes "SMARTMEM" vertions of both input and output      
//Rasters. Input Raster is copied completely to memory for faster access during
//proccessment. Coping only takes one access per element O(Rows * Cols), while 
//any gradient processing requires that for every element except the edge ones,
//the client code access all of its 8 surrounding elements.                    
bool slopeInDegrees(TeRaster& dbAltitudeRaster, TeRaster& slopeDegreesRaster,  
                    string& error);                                            
                                                                               
    //! Calculate the the slope gradient of an entire raster object in degrees.
    // Each call to this method handles one band only and outGrid must         
    // hold a copy of grid's RasterParam object with its own apropriate        
    // max min and dummy values.                                               
    /*                                                                         
       \Param TeHaster grid: Raster holding the cells                          
       \Param int band: which band of the Raster object that will be used      
       \Param double& outGrid: The resulting Haster                            
    */                                                                         
bool slopeGradientInDegrees(TeRaster& grid, TeRaster& outGrid, int band,       
                            string& msg);                                      
                                                                               
#endif                                                                         
                                                                               

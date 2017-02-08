/***********************************************************/                  
/*                                                         */                  
/* tardem.h                                                */                  
/*                                                         */                  
/* TARDEM callable functions -- header file                */                  
/*                                                         */                  
/*                                                         */                  
/* David Tarboton                                          */                  
/* Utah Water Research Laboratory                          */                  
/* Utah State University                                   */                  
/* Logan, UT 84322-8200                                    */                  
/* http://www.engineering.usu.edu/dtarb/                   */                  
/*                                                         */                  
/***********************************************************/                  
                                                                               
#ifndef  __TE_TAR_DEM_H                                                        
#define  __TE_TAR_DEM_H                                                        
                                                                               
//Terra Lib libraries ;//retundant comment                                     

//standard C/C++ includes                                                      
#include <string>                                                              
#include <time.h>                                                              
#include <stdio.h>                                                             
//Terra View includes                                                          
#include <TeLayer.h>                                                           
#include <TeRaster.h>                                                          
#include <TeRasterParams.h>                                                    
#include <TeSharedPtr.h>                                                       
#include <TeLayerFunctions.h>                                                  
#include <TeRasterRemap.h>                                                     
#include <TeImportRaster.h>                                                    
#include <TeInitRasterDecoders.h>                                              
#include <TeDecoderSmartMem.h>                                                 
#include <TePDIUtils.hpp>                                                      

//check the following files for the internal functions of TeTarDEM:            
//- TeTarDEM_Flood.h                                                           
//- TeTarDEM_SetDir.h                                                          
//- TeTarDEM_SharedSubRotines.h                                                
                                                                               
                                                                               
//  Constants    //                                                            
#define MAXLN 4096                                                             
//Global data types and constants                                              
#define PI2 1.5707963268                                                       
#ifndef PI                                                                     
#define PI 3.14159265359                                                       
#endif                                                                         
                                                                               
#define LINELEN 40                                                             
#ifndef MAXLN                                                                  
#define MAXLN 4096                                                             
#endif                                                                         
                                                                               
#define MISSINGSHORT -32767                                                    
#define MISSINGINT -2147483647                                                 
                                                                               
//data types                                                                   
#define RPSHRDTYPE  1                                                          
#define RPINTDTYPE  2                                                          
#define RPFLTDTYPE  3                                                          
#define RPDBLDTYPE  4                                                          
                                                                               
//byte sizes corresponding to data types above                                 
#define RPSHRSIZE (sizeof(short))                                              
#define RPINTSIZE (sizeof(int))                                                
#define RPFLTSIZE (sizeof(float))                                              
#define RPDBLSIZE (sizeof(double))                                             
                                                                               
//error constants                                                              
#define TD_NO_ERROR                  0                                         
#define TD_FAILED_GRID_OPEN          1                                         
#define TD_FAILED_GRID_SAVE          2                                         
#define TD_FAILED_MEMORY_ALLOC       3                                         
#define TD_STACK_LARGER_THAN_GRID    4                                         
#define TD_FAILED_TO_ENLARGE_STACK   5                                         
#define TD_DROPAN_ENCOUNTERED_ZERO   6                                         
#define TD_COORDINATE_OUTSIDE_GRID   7                                         
#define TD_CHANNEL_NETWORK_MISMATCH  8                                         
#define TD_FAILED_TREEFILE_OPEN      9                                         
#define TD_FAILED_COORDFILE_OPEN    10                                         
#define TD_NEGATIVE_SA_VALUE        11                                         
#define TD_NEGATIVE_OVERLAND_DIST   12                                         
#define TD_INVALID_TABLE_INDEX      13                                         
#define TD_INVALID_TABLE_ROW_INDEX  14                                         
#define TD_TABLE_ROW_MISSING_PARAM  15                                         
#define TD_INVALID_TABLE_COL_INDEX  16                                         
#define TD_PARAM_GRID_NO_DATA       17                                         
#define TD_FAILED_WTR_SHED_GRD_OPEN 18                                         
#define TD_FAILED_CONNECT_GRD_OPEN  19                                         
#define TD_FAILED_CREATE_SHP_FILE   20                                         
#define TD_INVALID_RAINFALL_SHPFILE 21                                         
#define TD_PARAM_COUNT_INCONSISTENT 22                                         
#define TD_INVALID_VARIANT_TYPE     23                                         
#define TD_UNEXPECTED_NODATA_IN_GRD 24                                         
#define TD_FAILED_SAVE_SHPFILE      25                                         
#define TD_FAILED_GRD_INIT          26                                         
#define TD_GRID_SIZE_MISMATCH       27                                         
#define TD_FAILED_SHPFILE_OPEN      28                                         
#define TD_FAILED_GET_HEADER        29                                         
#define TD_INVALID_ARRAY_SIZE       30                                         
#define TD_FAILED_FILE_OPEN         31                                         
#define TD_FLATROUT_NO_CONVERGENCE  32                                         
#define TD_Fortran_Allocate_Error   33                                         
#define TD_Topsetup_Zero_Area_Catchment  34                                    
                                                                               
/*                                                                             
//define directions                                                            
vector_J[1]=  0; vector_J[2]= -1; vector_J[3]= -1; vector_J[4]= -1;            
vector_J[5]=  0; vector_J[6]=  1; vector_J[7]=  1; vector_J[8]=  1;            
                                                                               
vector_I[1]=  1; vector_I[2]=  1; vector_I[3]=  0; vector_I[4]= -1;            
vector_I[5]= -1; vector_I[6]= -1; vector_I[7]=  0; vector_I[8]=  1;            
*/                                                                             
//constant array of possible components of the direction vector < i->, j-> >.  
//direction consts       0   1   2   3   4   5   6   7   8                     
const int vector_J[] = { 0,  0, -1, -1, -1,  0,  1,  1,  1};//J-> component    
const int vector_I[] = { 0,  1,  1,  0, -1, -1, -1,  0,  1};//Î-> component    
/*the vector in the comment abovve is not the extencible templated array from  
  the ANSI C++ speccification. It is the 2 dimension vector used to define     
  direction in a plane.                                                        
                                                                               
(vector_I, vector_J) |  (vector_J, vector_I)                                   
   |-1 | 0 | 1 |     |     |-1 | 0 | 1 |                                       
   |---|---|---|     |     |---|---|---|                                       
-1 | 4 | 3 | 2 |     |  -1 | 4 | 5 | 6 |                                       
   |---|---|---|     |     |---|---|---|                                       
 0 | 5 | 0 | 1 |     |   0 | 3 | 0 | 7 |                                       
   |---|---|---|     |     |---|---|---|                                       
 1 | 6 | 7 | 8 |     |   1 | 2 | 1 | 8 |                                       
   |---|---|---|     |     |---|---|---|                                       
                     |                                                         
   */                                                                          
                                                                               
typedef struct WorldRecord                                                     
{                                                                              
   bool useflowfile;                                                           
   int ccheck;     //Global contamination check flag                           
   int useww;      //Global use weights flag                                   
   int err;//hols the number of the last error fond (see below for convention) 
   int recursedepth; //ALL                                                     
   //the next 8 variables are related to the pools of cell and the stack of    
   //unresolved cells that that is currently making the while loop in setdf()  
   //unsesonably long. It does end in a resonable time with an 8 X 10 Raster,  
   //thus it may only be taking too long with the 748 X 920 Raster I used to   
   //test flood.                                                               
   int *ipool;  //                                                             
   int *jpool;  //                                                             
   int npool;   //                                                             
   int pooln;   //                                                             
   int pstack;  //                                                             
   int *dn;     //Array of linear indices used to access is and js             
   int *is;     //Array of X-coordinates                                       
   int *js;     //Array of Y-coordinates                                       
   int nis;     //loop control variable for the while loop that ends setdf()   
   int istack;  //                                                             
   //All Rasters used in flood() of setdir() simplt cary diferent data with the
   //same resolution over the same geographical area, therefore all Rasters    
   //uised in one run of either flood() or setdir(), have the same dimencions  
   int minX;      //Minimum X coordinate of Raster (this is is often 0)        
   int minY;      //Minimum Y coordinate of Raster (this is is often 0)        
   int numX;      //Number of X coordinates or number of Raster columns(a size)
   int numY;      //Number of Y coordinates or number of Raster lines (a size) 
   int nf;      //flood, setdir //Flood_SetDirRecord floodDirRec               
   double dx;   //Real world x-dimencion of a raster cell                      
   double dy;   //Real world y-dimencion of a raster cell                      
   double csize;//resolution of a cell, if dx = dy, czise = dx = dx            
   double mval; //Grid bound variable, or dummy                                
   double ndv;  //                                                             
   double emin; //                                                             
   double et;   //                                                             
   double fdmval;                                                              
   TeBox bndbox;//Grid bound Box                                               
} WorldRecord;                                                                 
                                                                               
                                                                               
/*---------------------------------------                                      
/--- Function Prototypes ----------------                                      
*/                                                                             
                                                                               
//*****************************************************************************
//***** Main Functions OR Tardem API functions: flood() and setdir() **********
//*****************************************************************************
                                                                               
                                                                               
    //! Removes pits using the flooding algorithm.                             
    /*!                                                                        
     \Param demfile : TeRaster with original altitude data                     
     \Param newfile : TeRaster used to output pitt filled altutude data        
     \Param flowfile: TeRaster used for the optional input of pre-existing flow
     \Param newflowfile : TeRater used to output verified flow.                
     \Param use flowfile: bool used to toggle the use of a flow file           
   */                                                                          
int flood(TeRaster& demfile, TeRaster& newfile, TeRaster& flowfile,            
          TeRaster& newflowfile, bool useflowfile);                            
                                                                               
    //!Sets Dinf flow directions. The output of flood() is the input of        
    //setdir()                                                                 
    /*!                                                                        
      \Param demfile: TeRaster with (pitt filled) Elevation data               
      \Param angfile: TeRaster used to output Dinf flow angles.                
      \Param slopefile: TeRaster to output Dinf slopes.                        
      \Param flowfile:  TeRaster with optional verified flow file              
      \Param useflowfile:  bool used to toggle the use of a flow file          
    */                                                                         
int setdir(TeRaster& demfile,  TeRaster& angfile, TeRaster& slopefile,         
           TeRaster& flowfile, bool useflowfile);                              
                                                                               
 //!Overloaded function that takes the following extra TeRaster references:    
 /*!                                                                           
    This funtion does exactly the same as the original except that internally  
 used raster can be accessed for debugging. It is highly recomented that the   
 user do nothing with the extra TRaster PArameters exceptdefining them         
 immediately before calling this function.                                     
   \Param fweight: TeRaster use not yet clear to me                            
   \Param larr   : TeRaster use not yet clear to me                            
   \Param fareaw : TeRaster use not yet clear to me                            
   \Param sdir   : TeRaster used to store directions during proccessment       
   \Param fangle : TeRaster used to store angles during proccement             
   \Param felevg : TeRaster used to store elevations during proccessment       
   \Param fslopeg: TeRaster used to store slopes during proccessment           
   \Param sapoolg: TeRaster used related to the pool of unresolved cells       
   note: the actual pool of unresolved cells is in the arrays world.is,        
   world.js, and world.dn                                                      
    */                                                                         
int setdir(TeRaster& demfile,  TeRaster& angfile, TeRaster& slopefile,         
           TeRaster& flowfile, bool useflowfile,  TeRaster& fweight,           
           TeRaster& larr,     TeRaster& fareaw,  TeRaster& sdir,              
           TeRaster& fangle,   TeRaster& felevg,  TeRaster& fslopeg,           
           TeRaster& sapoolg);                                                 
                                                                               
#endif                                                                         

/***************************************************************************/
/*   The functions bellow are used both by setdir() and by flood(). Most   */
/* of them are taken Tarbodon's grid class but and the functions used to   */
/* handle the interface between drid struct and hard disk file. However,   */
/* the disk access and the grid objects have all been replaced by TeRaster */
/* objects and its interface. TeRaster allows disk access but is mostly    */
/* geared towards database access. Because they are mostly adapters, and   */
/* all dependancies of public API functions, none of the functions defined */
/* here is meant to be called directly by the general public. The last 3   */
/* are from TarDEM and just happened to be shared by foold() and setdir()  */
/***************************************************************************/

#ifndef  __TE_TAR_DEM_SHARED_SUB_ROTINES_H
#define  __TE_TAR_DEM_SHARED_SUB_ROTINES_H

#include "TeTarDEM.h"
//#include "TeTarDEM_Flood.h"
//#include "TeTarDEM_SetDir.h"

//*****************************************************************************
//*****Adapter Functions: from file to TeRaster ********************************
//*****************************************************************************

//the next two functions set max or min repectively for one or all bands much 
//like the TeRasterParam method setDummy().
void setMax(TeRasterParams& params, double max, int band);
//*****************************************************************************
void setMin(TeRasterParams& params, double min, int band);
//*****************************************************************************
//init Raster from a TeRasterParams Object
bool rasterInit(TeRaster& newRaster, TeRasterParams& newRasterParams);
                                                                            
//*****************************************************************************
//set Raster params object.
void paramInit(TeRasterParams& newRasterParams, bool useDummy, int band, 
               int row, int col, TeDataType generalType, double resx, 
               double resy, double generalMax, double generalMin, 
               double generalDummy, double x1, double y1, double x2, double y2, 
               char mode, char* decoderId);
// The names of the methods bellow have been chosen to minimize the changes 
//needed to add the code bellow to terra lib.

//gives a raster a set of params and sets all cells to one value
//This allocate inittializes Raster, assumes the given params are correct.
bool allocategrid(TeRaster &grid, TeRasterParams &params, double value);
//*****************************************************************************
//uses a raster's original params and sets all cells to value
//This allocate does NOT inittialize Raster! use a method with init first.
bool allocategrid(TeRaster &grid, double value);
//*****************************************************************************
//local adapters for TeRaster.getElement() and TeRaster.setElement()
//ALL 3 functions below take parameters dimencion parameters in the order (i,j)
//or (x,y) as do the function calls in the adapted code, not the matrix standard
//[j][i], in retrospect the matrix standard migth have been more practical since
//those functions are subtituted matrix accesses, but by now the majority of 
//the doce has been change to use send parameters to them in cartesian
//coordinates(x, y) or (i, j). Througthout the code i is alwasy used for the X
//dimencion or the horizontal and j is always used for the Y dimencion or the 
//vertical.
//The first adapter of the 3 simply uses the function return value to return
//the raster value and a boolean reference for the success status, allowing 
//the client code to use the Returned value as part of another line of code, 
//when testing for success is not too important, for example, when the value
//can be a dummy, which causes TeRaster.getElement(in, int, duble&, int) to 
//return false even though the cordinates and band are all valid
double getElement(TeRaster & grid, int i, int j, bool& itWorked);
//*****************************************************************************
//The name could be clearer, this is an adpter for  grid.d[j][i] used to return 
//a value. if the name were the same as a bove it would tale longer to change 
//the code that uses this below.
double d(TeRaster & grid, int i, int j);
//*****************************************************************************
//purely convenience setter method. The name could be clearer, this is an 
//adpter for d(grid , i, j) used to change a value. The boolean return is given
//to make the adapter complete, but it is not used in the code below because
//original d[j][i] did had no error reporting functionality. see comment above
//double getElement(TeRaster & grid, int i, int j, bool& itWorked) for parameter
//order!
bool d(TeRaster& grid,int i,int j,double val);

//*****************************************************************************
bool gridread(TeLayer& iLayer, TeRaster& grid);
//*****************************************************************************
bool gridread(TeRaster& iRaster, TeRaster& grid);
//*****************************************************************************
//**NOTE: despite similar names the 2 functions below have different purposes  
//the first one takes pointer to a database and to an unintialized layer, 
//whitch it uses to return the layer created TeLayer* TeIm,portRaster(char*, 
//TeRaster*, TeDatabase*). The second function imply makes a Raster object in 
//memory
bool gridwrite(char* oLayerName, TeRaster& grid, TeDatabase* db_ptr,           
               TeLayer* oLayer_ptr);
bool gridwrite(TeRaster& oRaster, TeRaster& grid);
//*****************************************************************************
//*****Auxilliary functions original to TauDEM.cpp*****************************
//*****************************************************************************

void darea(int i, int j, WorldRecord& world, TeRaster& sdir, TeRaster& larr, 
           TeRaster& fareaw, TeRaster& fweight);
//*****************************************************************************
void set1(int i,int j,double *fact,double mval,bool useflowfile,               
          TeRaster& sdir, TeRaster& larr, TeRaster& felevg);
//*****************************************************************************
//check if the 2 cells that form a 2X2 square with cells sdir(i,j) and         
//sdir(i+vector_[k], j+vector_J[k]) do not drain from one to the other and     
//vice-versa 
int dontcross(int k,int i,int j, TeRaster& sdir);

#endif
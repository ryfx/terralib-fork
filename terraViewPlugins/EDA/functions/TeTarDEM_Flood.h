/***************************************************************************/
/*   The functions bellow are used exclusively by flood(), and its         */
/* dependancies with the only exception of flood() itself                  */
/***************************************************************************/

#ifndef  __TE_TAR_DEM_FLOOD_H
#define  __TE_TAR_DEM_FLOOD_H

#include "TeTarDEM.h"
#include "TeTarDEM_SharedSubRotines.h"
//#include "TeTarDEM_SetDir.h"

int flood(TeRaster& demfile, TeRaster& newfile, TeRaster& flowfile, 
          TeRaster& newflowfile, bool useflowfile);
//*****************************************************************************
//convinience method used for raster initialization
//WARNIG! This method assumes that world has all suitable parameters
bool initRasterFromWorld(TeRaster& newRaster, WorldRecord& world, 
                         TeDataType dataType,double max,double min,double dummy);
//*****************************************************************************
// Function to climb recursively flow direction grid for min elevation to burn 
//down
double climb(int i,int j, TeRaster& felevg, TeRaster& sdir);
//*****************************************************************************
// Function to burn down flow elevations for given flow directions. This is a 
//placeholder for what will eventually be simplex optimization
int fdsimplex(WorldRecord& world, TeRaster& felevg, TeRaster& sdir);
//*****************************************************************************
// Function to clean given flow directions.
int dirclean(TeRaster& newflowfile, bool writeflowfile, WorldRecord& world, 
             TeRaster& felevg, TeRaster& sdir, TeRaster& larr, 
             TeRaster& fareaw, TeRaster& sapoolg, TeRaster& fweight);
//*****************************************************************************
int addstack(int i, int j, WorldRecord& world);
//*****************************************************************************
//that function should be doing more to bring about the end condition of the 
//while loop in setdf(), but after a few loop iterations it seems to stop 
//decrementing world.nis.
int vdn(int n, WorldRecord& world, TeRaster& felevg, TeRaster& sdir);

void pool(int i,int j, WorldRecord& world, TeRaster& sapoolg, TeRaster& sdir, 
          TeRaster& felevg);
//*****************************************************************************
double max2(double e1,double e2);
//*****************************************************************************
bool notfdr(bool useflowfile,int i,int j,TeRaster& larr);

int setdf(double fdmval, bool useflowfile, TeRaster& newflowfile, 
          bool writeflowfile, WorldRecord& world, TeRaster& larr, 
          TeRaster& felevg, TeRaster& sapoolg,TeRaster& sdir, TeRaster& fareaw, 
          TeRaster& fweight);

#endif
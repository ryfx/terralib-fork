/*************************************************************************/
/*  The functions bellow are used exclusively by setdir() and its        */
/* dependancies, with the only exception of setdir() in its 2 vertions.  */
/*************************************************************************/

#ifndef  __TE_TAR_DEM_SET_DIR_H
#define  __TE_TAR_DEM_SET_SIR_H

#include "TeTarDEM.h"
#include "TeTarDEM_SharedSubRotines.h"

int setdir(TeRaster& demfile, TeRaster& angfile, TeRaster& slopefile, 
           TeRaster& flowfile, bool useflowfile);

int setdir(TeRaster& demfile,  TeRaster& angfile, TeRaster& slopefile, 
           TeRaster& flowfile, bool useflowfile,  TeRaster& fweight,
           TeRaster& larr,     TeRaster& fareaw,  TeRaster& sdir, 
           TeRaster& fangle,   TeRaster& felevg,  TeRaster& fslopeg, 
           TeRaster& sapoolg);
bool rasterInitWithDummy(TeRaster& newRaster, TeRasterParams newRasterParams,  
                          double dummy);
/*
  matalloc(...) allocates memory for matrix navigation pointer
  and for matrix data, then returns matrix navigation pointer
  Modification of matrixalloc by DGT to not use so many pointers
  7/1/97
  This is a further modification of the function to support doubles
  11/13/07
*/

void** matalloc(int nx,int ny,int datatype);
//*****************************************************************************
void VSLOPE(double E0, double E1, double E2, double D1, double D2, double DD,
            double *S, double *A);
//*****************************************************************************
// SUBROUTINE TO RETURN SLOPE AND ANGLE OF STEEPEST DECENT IF POSITIVE 
void SET2(int I, int J, double *DXX, double DD, TeRaster& fslopeg, 
          TeRaster& felevg, TeRaster& fangle);
//*****************************************************************************
void set2(int i,int j,double *fact,double *elev1, double *elev2, int iter, 
          int **spos,  int *s, bool useflowfile, TeRaster& sdir, 
          TeRaster& felevg);
//*****************************************************************************
void incfall(int n, double *elev1,  int *s1,int **spos, int iter, int *sloc,
             WorldRecord& world, TeRaster& sdir, TeRaster& felevg);
//*****************************************************************************
void incrise(int n, double *elev1,  int *s2,int **spos, int iter, int *sloc, 
             WorldRecord& world, TeRaster& felevg);
//*****************************************************************************
int flatrout(int n,int *sloc, int *s, int **spos,int iter,double *elev1,
             double *elev2, double *fact, int ns, WorldRecord& world, 
             TeRaster& sdir, TeRaster& felevg);
//*****************************************************************************
int setdfnoflood(WorldRecord& world, TeRaster& sdir, TeRaster& felevg, 
                 TeRaster& larr, TeRaster& fareaw,  TeRaster& sapoolg, 
                 TeRaster& fweight);
//*****************************************************************************
/*
Converted from FORTRAN July 05, 1997  K. Tarbet

C
C---SUBROUTINE TO SET VARIABLE DIRECTIONS.
CDIRECTIONS MEASURED ANTICLOCKWISE FROM EAST IN RADIANS.
C
*/

void setdf2(WorldRecord& world, TeRaster& felevg, TeRaster& fangle, 
            TeRaster& fslopeg, TeRaster& sdir, TeRaster& sapoolg);

#endif
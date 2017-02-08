/**
 * @file sv.cpp
 * @author Danilo Lourenco Lopes.
 * @date July 03, 2007
 */

//includes
#include "algoritmos.h"
#include "sv.h"
#include <cmath>

int OpenData(const char *ArqO, const char *ArqD, const char *ArqP,
			 std::vector<double> &xp, double &xmin, double &xmax,
             std::vector<double> &yp, double &ymin, double &ymax,              
             std::vector<double> &x1, std::vector<double> &y1, 
             std::vector<double> &x2, std::vector<double> &y2)
{

 FILE *orig, *dest, *poly;
 double xaux, yaux;
 int r;



 //try to open the Origin Point Pattern Data File
 if( (orig = fopen(ArqO, "r")) == NULL ) {
  return 2;
 }
 
 //save the Origin Point Pattern Data File in memory
 while( !feof(orig) ) {
  r = fscanf(orig, "%lf%lf", &xaux, &yaux);
  if( (r >= 0) && (r != 2) )
   return 3;
  if( r == 2 )
  {
	  x1.push_back(xaux);
	  y1.push_back(yaux);
  }
 }



  //try to open the Destination Point Pattern Data File
 if( (dest = fopen(ArqD, "r")) == NULL ) {
  return 2;
 }
 
 //save the Destination Point Pattern Data File in memory
 while( !feof(dest) ) {
  int r = fscanf(dest, "%lf%lf", &xaux, &yaux);
  if( (r >= 0) && (r != 2) )
   return 4;
  if( r == 2 )
  {
	  x2.push_back(xaux);
	  y2.push_back(yaux);
  }
 }
   
if(x1.size() != x2.size())
	return 5;


 //try to open the Polygon Data File
 if( (poly = fopen(ArqP, "r")) == NULL ) {
  return 2;
 }
 r = fscanf(poly, "%lf%lf", &xaux, &yaux);
 if( (r >= 0) && (r != 2) )
   return 6;
 if( r == 2 )
 {
	xp.push_back(xaux);
	xmin = xaux;
	xmax = xaux;
	yp.push_back(yaux);
	ymin = yaux;
	ymax = yaux;
 }

 //save the Polygon Data File in memory
 while( !feof(poly) ) {
  int r = fscanf(poly, "%lf%lf", &xaux, &yaux);
  if( (r >= 0) && (r != 2) )
   return 6;
  if( r == 2 )
  {
	  xp.push_back(xaux);
      xmin = MIN(xmin,xaux);
      xmax = MAX(xmax,xaux);
	  yp.push_back(yaux);
      ymin = MIN(ymin,yaux);
      ymax = MAX(ymax,yaux);
  }
 }

 return 0;
}


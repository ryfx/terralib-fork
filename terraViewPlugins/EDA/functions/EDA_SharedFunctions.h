/*****************************************************************************/
/* Functions that are general to Elevation Data Analizer and not meant to be */
/*art of a public user API                                                   */
/*****************************************************************************/

#ifndef EDA_SHARED_FUNCTIONS_H
#define EDA_SHARED_FUNCTIONS_H

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

#define BUFFER_SIZE 65530                                                      
typedef double*    Y_D_Ptr_T;//Y-Axis dimension pointer type                   
typedef Y_D_Ptr_T* X_D_Ptr_T;//X-Axis dimension pointer type                   
typedef X_D_Ptr_T* Z_D_Ptr_T;//Z-Axis dimension pointer type (band)            
typedef vector<string> NameList_T;                                             

//function to take start in time_t and the time it begun running to to return  
// a report on both times and their difference.                                
string timeCheck(time_t startTime);                                            
/*TeDB methods that will be used to solved unique name problem once and for all
TeLayerMap  & layerMap ()//Returns the map of layers in the database.          
TeProjectMap& projectMap ()//Returns the map of projects in the database.      
TeThemeMap  & themeMap ()//Returns the map of themes in the database.          
TeViewMap   & viewMap ()//Returns the map of views in the database.            
*/                                                                             
                                                                               
//Overloaded functions with ther over all effect6 of allowing client code use  
// string& uniqueNewName = makeUnique(inputName, mapToList(map)); where map    
// can be either the layer map or the or the project map or the theme map or   
// the view map.                                                               
NameList_T mapToList(TeProjectMap& map);                                       
NameList_T mapToList(TeLayerMap&   map);                                       
NameList_T mapToList(TeThemeMap&   map);                                       
NameList_T mapToList(TeViewMap&    map);                                       
//NOTE: this function is based on the 4 above and it takes a vector<string>,   
// not a map of any kind. This function assumes that the map was recently read,
// and the name list is complete, including all names currently in the DB. If a
// not in the DB is on the list it should not cause major problems, but it can 
// make code take longer since it has a greater set of names to avoid.         
string makeUnique(string& name, NameList_T& list);                             
                                                                               
//Two convinience functions to reduce typing and make the code look simpler.   
//They are not meant to make code more efficient. Their variable argument      
//works the same way as printf()                                               
string inf(char* title, const char* text, ...);                                
string err(char* title, const char* text, ...);                                
                                                                               
//Mostly for debug function that tests a raster object for usefull data. It    
//will output a warming if the raster sent tu it has all dummy values or its   
//values are equal                                                             
bool rasterInputCheck(TeRaster& iGrid, string& msg);                           
                                                                               
//mostly for debug function that takes one raster and reports its params       
string reportRasterParams(TeRaster& grid, int band);                           
//Mostly for debug function that takes any two Rasters and uses                
//QMessageBox::information to display key Raster parameters side by side       
string reportRasterParams(TeRaster& iGrid, TeRaster& oGrid, int band);         
                                                                               
//Mostly for debug function that takes a raster and writes a .asc file allowing
//the user to personally read what information in in the raster.               
//void displayRaster(TeRaster& grid, int band, char* rasterName);              
                                                                               
//Mostly for debug function that takes a raster and writes a .asc file allowing
//the user to personally read what information in in the raster. The diference 
//this and the function above is that this does not rely un dynamic memory     
//allocation and writes the .asc file directly as it reads the Raster.         
void displayRasterASC(TeRaster& grid, int band, char* rasterName, string& msg);
                                                                               
//Make a shared pointer copy of a TeRaster object. So far only makeGeoTiff()   
//uses this, but it could pibliy have other uses in the future. Rastes is deep 
// copied to a locatrion in memory;                                            
TePDITypes::TePDIRasterPtrType makeSharedPtrMemCopy(TeRaster& outputRaster,    
                                                    string& msg);              
                                                                               
//Mostly for debug fuction that follows a similar logic to the one above, but  
//produces a .tiff file. NOTE, this requires the shared pointer copy of raster.
//See makeSharedPtrMemCopy() above.                                            
bool makeGeoTiff(TeRaster& outputRaster, string outLayerName, string& msg);    
                                                                               
    // CALCULATE GRADIENT VECTOR COMPONENTS IN THE POINT L,C OF THE GRID       
    //! Find the first derivative of a user-defined cell inside this raster    
    //object                                                                   
    /*                                                                         
       \Param TeHaster grid: Raster holding the cell around wich we will       
derivate                                                                       
       \Param int band: which band of the Raster object that will be used      
       \Param int row: Row of the cell used to find the area derivative        
       \Param int col: Colomn of the cell used to find the area derivative     
       \Param double$ dzdx: Reference used to derivative of X with respect to Z
       \Param double& dzdy: Reference used to derivative of Y with respect to Z
       \Param char* errorReport: pointer to string containing an error message 
    //Hopefully the last parameter will not be very much used.                 
   */                                                                          
bool calcLocalGradient(TeRaster& grid, int band, int row,int col, double& dzdx,
                       double& dzdy, string& msg);                             
                                                                               
#endif

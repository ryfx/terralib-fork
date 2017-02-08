/*************************************************************************/    
/*  This is the Elevation Data Analizer or EDA plugin to Terra View      */    
/*  It mostly uses TerraLib underneath the user interrface               */    
/*************************************************************************/    
                                                                               
#ifndef  __EDA_PLUNGIN_CODE_H                                                  
#define  __EDA_PLUNGIN_CODE_H                                                  
                                                                               
#include <spl.h>                                                               
                                                                               
#if SPL_PLATFORM == SPL_PLATFORM_WIN32                                         
  // Include windows.h - Needed for Windows projects.                          
  #include <windows.h>                                                         
#endif                                                                         
                                                                               
//standard C/C++ includes                                                      
#include <string>                                                              
#include <time.h>                                                              
#include <stdio.h>                                                             
#include <stdarg.h>                                                            
//Terra View includes                                                          
#include <TeLayer.h>                                                           
#include <TeRaster.h>                                                          
#include <TeRasterParams.h>                                                    
#include <PluginBase.h>                                                        
#include <TeSharedPtr.h>                                                       
#include <TeLayerFunctions.h>                                                  
#include <TeRasterRemap.h>                                                     
#include <TeImportRaster.h>                                                    
#include <TeInitRasterDecoders.h>                                              
#include <TeDecoderSmartMem.h>                                                 
#include <TePDIUtils.hpp>                                                      
#include <TeQtThemeItem.h>                                                     
#include <TeQtViewItem.h>                                                      
//QT includes                                                                  
#include <qmessagebox.h>                                                       
#include <qtextedit.h>                                                         
#include <qwidget.h>                                                           
#include <qtextcodec.h>                                                        
#include <qcombobox.h> //for the Qt combo box.                                 
#include <qradiobutton.h> //for the Qt radio button.                           
#include <qlineedit.h> //for the input line.                                   
#include <qmessagebox.h>  //for the messages                                   
//internal includes from this plugin                                           
#include <PluginParameters.h>                                                  
#include "EDAPluginMainForm.h"                                                 
#include "functions/EDA_SharedFunctions.h" //used by aspect and slope          
#include "functions/EDA_Aspect.h"                                              
#include "functions/EDA_SlopeDegrees.h"                                        
#include "functions/EDA_SlopePercent.h"                                        
#include "functions/TeTarDEM.h"                                                
#include "functions/TeTarDEM_SharedSubRotines.h" //used by flood and set dir   
#include "functions/TeTarDEM_Flood.h"                                          
#include "functions/TeTarDEM_SetDir.h"                                         
                                                                               
/* A pointer to the plugin main window instance */                             
EDAPluginMainForm* main_window_instance_ptr = 0;                               
                                                                               
#define BUFFER_SIZE 65530                                                      
typedef double*    Y_D_Ptr_T;//Y-Axis dimension pointer type                   
typedef Y_D_Ptr_T* X_D_Ptr_T;//X-Axis dimension pointer type                   
typedef X_D_Ptr_T* Z_D_Ptr_T;//Z-Axis dimension pointer type (band)            
typedef vector<string> NameList_T;                                             
//                                                                             
// Define standard export symbols for Win32.                                   
// Note: You don't have to remove this macro for other platforms.              
//                                                                             
SPL_DEFINE_PLUGIN_EXPORTS();                                                   
                                                                               
//                                                                             
// Define all plugin info (this info is used by TerraView).                    
//                                                                             
SPL_DEFINE_PLUGIN_INFO(                                                        
  1,		// The build number (e.g. xx.1.0)..                                
  0,		// The major version (e.g. 1.xx.0).                                
  1,		// The minor version (e.g. 0.1.xx).                                
  true,		// Always "true".                                                  
  "EDA",	// The plugin's name.                                              
  "INPE-DPI",	///< The plugin's vendor.                                      
  "A TerraView plugin to analyse elevation data", //< The plugin's description 
  PLUGINPARAMETERS_VERSION,	///< Always "PLUGINPARAMETERS_VERSION".            
  "http://none",	///< The plugin's homepage.                                
  "luizcarlosriosabreu@gmail.com",	///< The plugin author's email address.    
  "TerraViewPlugin" );	// The plugin's UUID (The first string must            
                        // always be "TerraViewPlugin". The next               
                        // strings will define the plugin                      
                        // sub-menus. All strings must be separated            
                        // by the a "." (dot).                                 
                                                                               
//                                                                             
// Define a standard DllMain() function for Win32 plugin DLLs.                 
// Note: You haven't to delete or comment out this function for other platforms
// SPL automatically detects the current platform and removes unused/unwanted  
//code behind the macros.                                                      
//                                                                             
SPL_DEFINE_PLUGIN_DLLMAIN();                                                   
                                                                               
//                                                                             
// We don't want to change the default get information method, so simply       
// use the default implementation.                                             
//                                                                             
SPL_IMPLEMENT_PLUGIN_GETINFO();                                                
// Functions specified by the SPL library for interation with the              
//interface of the plugin.                                                     
SPL_PLUGIN_API bool SPL_INIT_NAME_CODE( slcPluginArgs* );                      
SPL_PLUGIN_API bool SPL_SHUTDOWN_NAME_CODE( slcPluginArgs* );                  
SPL_PLUGIN_API bool SPL_RUN_NAME_CODE( slcPluginArgs* a_pPluginArgs );         
                                                                               
//Two convinience functions for stactic Members of QMEssageBox. They are meant 
//to reduce typing and make the code using QMessageBox::information or         
//QMessageBox::critical look simpler. They are not meant to make code more     
//efficient.                                                                   
void infStr(char* title, string message);                                      
void errStr(char* title, string message);                                      
                                                                               
//pre EDA main run tests, They are a sequence of for tests to determine if the 
//plugin can really run this time.                                             
// - Test database avilability                                                 
// - Test presence of layers in the database                                   
// - Test presence of Rasters in the layers                                    
//A faliure in any such tests makes the method sotp and return false. Because  
//EDA only handles layers with hasters, a failure in any of the tests cited    
//above forces it to quit this run. There is always next time                  
bool testPlugPars(PluginParameters* plug_pars_ptr,TeLayerMap& layerMap,        
                  vector<int>& layerIdList);                                   
                                                                               
//post output Raster proccess used to make a new view and a new theme, a new   
//.asc file and .tif files out of the output raster.                           
bool layerOutput(PluginParameters* plug_pars_ptr, TeRaster& outputRaster,      
                 string outLayerName);                                         
                                                                               
///////////////////////////////////////////////////////////////////// / / /    
///////////////////////////heavy processment functions.///////////// / / /     
/////////////////////////////////////////////////////////////////// / / /      
                                                                               

#endif                                                                         

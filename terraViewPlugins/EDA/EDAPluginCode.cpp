#include "EDAPluginCode.h"                                                     
                                                                               
///////////////////////////////////////////////////////////                    
// This function will be executed at plugin's loading time.                    
SPL_PLUGIN_API bool SPL_INIT_NAME_CODE(slcPluginArgs* a_pPluginArgs)           
{                                                                              
  /* Extracting plugin parameters */                                           
  void* arg_ptrs[ 1 ];                                                         
  if(!(a_pPluginArgs->GetArg( 0, arg_ptrs ) == 0) ) return false;              
                                                                               
   PluginParameters* plug_pars_ptr = ( PluginParameters* ) arg_ptrs[0];        
   //This is needed for windows - TeSingletons doesn't work with DLLs          
   TeProgress::setProgressInterf(plug_pars_ptr->teprogressbase_ptr_ );         
   //Creating the plugin main window                                           
   main_window_instance_ptr = new EDAPluginMainForm();                         
   //free that memory in shutdown!                                             
   return true;                                                                
}//SPL_PLUGIN_API bool SPL_INIT_NAME_CODE(slcPluginArgs* a_pPluginArgs)        
                                                                               
///////////////////////////////////////////////////////////                    
// This function will be executed at plugin's unloading time.                  
SPL_PLUGIN_API bool SPL_SHUTDOWN_NAME_CODE(slcPluginArgs* a_pPluginArgs)       
{                                                                              
  //Extracting plugin parameters                                               
  void* arg_ptrs[ 1 ];                                                         
  if( a_pPluginArgs->GetArg( 0, arg_ptrs ) != 0) return false;                 
                                                                               
   PluginParameters* plug_pars_ptr = 0;                                        
   plug_pars_ptr = ( PluginParameters* ) arg_ptrs[ 0 ];                        
   //Destroying the plugin main window                                         
   delete main_window_instance_ptr;                                            
   return true;                                                                
}//SPL_PLUGIN_API bool SPL_SHUTDOWN_NAME_CODE( slcPluginArgs* a_pPluginArgs )  
                                                                               
///////////////////////////////////////////////////////////                    
// This function will be executed when the user activates                      
//the plugin menu by clicking on it.                                           
SPL_PLUGIN_API bool SPL_RUN_NAME_CODE( slcPluginArgs* a_pPluginArgs )          
{                                                                              
   // - Test plugin parameter retrival                                         
   TeInitRasterDecoders();//From Terrra Lib tutorial                           
   char* fT = "EDA Plugin";//function title to all info and error windows      
   string msg;//hold messages from other functions                             
   //from this function. Because this is a constant, it is defined here and    
   //reused whenever needed                                                    
   time_t startTime = time(0);//from now on this run time will be recorded     
   //do this before anything else to measure all run time of this function     
   //Extracting plugin parameters                                              
   void* arg_ptrs[1];                                                          
   if( a_pPluginArgs->GetArg( 0, arg_ptrs ) != 0 )                             
   {                                                                           
       errStr(fT,err(fT,                                                       
                    "Error getting plugin parameters! No suggestions so far"));
       return false;//no parameters, lets get out of here                      
   }//if we did not get the plugin parameters!                                 
   // Updating the plugin window with the new parameters                       
   //and getting a local direct access to the DB                               
   PluginParameters* plug_pars_ptr = (PluginParameters*) arg_ptrs[0];          
   TeDatabase* currentDB_ptr = plug_pars_ptr->getCurrentDatabasePtr();         
   vector<int> layerIdList;                                                    
   TeLayerMap layerMap;                                                        
   if(!testPlugPars(plug_pars_ptr, layerMap, layerIdList))                     
       return(false);//user already got an error message                       
   //This is were the main part of the EDA plugin starts                       
   if(main_window_instance_ptr->exec() == QDialog::Accepted)                   
   {                                                                           
       //luckly, we need to retrive the Id based on the array index.           
       //because the inverse would be a search operation.                      
       int inputLayerKey = layerIdList.at(main_window_instance_ptr->           
                                          inputLayerCombo->currentItem());     
       TeLayer* inputLayer_ptr = layerMap[inputLayerKey];                      
       //DEM = Digital Elevation Model                                         
       TeRaster& demRaster = *(inputLayer_ptr->raster("",'r'));//general input 
       TeRaster outputRaster;                                                  
       const char* outName;                                                    
       outName = (main_window_instance_ptr->outputLayerLine->text()).ascii();  
       string outLayerName(outName);                                           
       outLayerName = makeUnique(outLayerName, mapToList(layerMap));           
       outName = outLayerName.c_str();                                         
       //Rasters before proccess                                               
       if(!rasterInputCheck(demRaster, msg)){  errStr(fT,msg);  return false;} 
       //if raster is empty, quit                                              
       infStr(fT, reportRasterParams(demRaster,0));//original input params     
       displayRasterASC(demRaster, 0,"DEM_Raster",msg);//contents of demRaster 
       if(!msg.empty())errStr(fT,msg);                                         
       bool useFlowRaster = false;//used in flood() and setFlowDirection()     
       if(main_window_instance_ptr->aspect->isOn())                            
       {//if user chose aspect                                                 
           inf(fT, "Layer named: %s built for output of Aspect", outName);     
           if(!aspect(demRaster, outputRaster, msg))                           
           {   errStr(fT,msg);     return false;   }                           
           inf(fT, "Layer named: %s built for output of Aspect", outName);     
           displayRasterASC(outputRaster, 0, "AspectRaster", msg);             
           if(!msg.empty())errStr(fT,msg);                                     
       }//if user chose aspect                                                 
       else if(main_window_instance_ptr->slope->isOn())                        
       {//if user chose slope                                                  
           if(main_window_instance_ptr->degrees->isOn())                       
           {//slope in degrees                                                 
               inf(fT, "Layer named: %s will hold slope degrees", outName);    
               if(!slopeInDegrees(demRaster, outputRaster,msg))                
               {   errStr(fT,msg);     return false;   }                       
               inf(fT, "Layer made: %s holds slope degrees", outName);         
               displayRasterASC(outputRaster, 0, "SlopeDegreesRaster", msg);   
               if(!msg.empty())errStr(fT,msg);                                 
           }//slope in degrees                                                 
           else if(main_window_instance_ptr->percent->isOn())                  
           {//declivity in percent                                             
               inf(fT, "Layer named: %s will hold slope percent", outName);    
               if(!slopeInPercent(demRaster,outputRaster,msg))                 
               {   errStr(fT,msg);     return false;   }                       
               inf(fT, "Layer made: %s holds slope percent", outName);         
               displayRasterASC(outputRaster, 0, "SlopePercentRaster", msg);   
               if(!msg.empty())errStr(fT,msg);                                 
           }//slope in percent                                                 
       }//if user chose slope                                                  
       else if(main_window_instance_ptr->flood->isOn())                        
       {//if user chose flood()                                                
           int inputFlowKey = layerIdList.at(main_window_instance_ptr->        
               inputFlowLayerCombo->currentItem());                            
           TeLayer* inputFlowLayer_ptr = layerMap[inputFlowKey];               
           TeRaster& flowRaster = *inputFlowLayer_ptr->raster("", 'c');        
           TeRaster newFlowRaster;                                             
           if(main_window_instance_ptr->yes->isOn())                           
           {   //in this case, we do to flow the same pre-proccess we do to DEM
               //in all cases.                                                 
               if(!rasterInputCheck(flowRaster, msg))                          
               {   errStr(fT,msg);     return false;}//if empty. quit          
               infStr(fT,reportRasterParams(flowRaster,0));//flow input params 
               displayRasterASC(flowRaster, 0, "DEM_Raster",msg);              
               if(!msg.empty())errStr(fT, msg);                                
               useFlowRaster = true;                                           
           }                                                                   
           int error = flood(demRaster, outputRaster, flowRaster,              
                             newFlowRaster, useFlowRaster); //KEY LINE         
           if(error)                                                           
           {                                                                   
               errStr(fT,err(fT, " flood returned error %03d ", error));       
               return false;                                                   
           }                                                                   
           infStr(fT, inf(fT, "Raster created for layer:%s %s %3d",            
                          outLayerName.c_str(),                                
                          "filled with output of flood error from flood: ",    
                          error));                                             
           displayRasterASC(outputRaster, 0, "floodRaster", msg);              
           if(!msg.empty())errStr(fT,msg);                                     
       }//if user chose flood()                                                
       else if(main_window_instance_ptr->setFlowDirection->isOn())             
       {//if user chose setFlowDirection()                                     
           int inputFlowKey = layerIdList.at(main_window_instance_ptr->        
               inputFlowLayerCombo->currentItem());                            
           TeLayer* inputFlowLayer_ptr = layerMap[inputFlowKey];               
           TeRaster& flowRaster = *inputFlowLayer_ptr->raster("", 'c');        
           TeRaster slopeRaster;                                               
           //debug variables: in a release they would be defined in setdir()   
           TeRaster fweight;//aread8 & area //double                           
           TeRaster larr;   //aread8 & area  //int                             
           TeRaster fareaw; //aread8 & area //double                           
           TeRaster sdir;   //aread8 & area // int                             
           TeRaster fangle; //area //double                                    
           TeRaster felevg; //source //double                                  
           TeRaster fslopeg;//source //double                                  
           TeRaster sapoolg;//sdir function//                                  
           if(main_window_instance_ptr->yes->isOn())                           
           {   //in this case, we do to flow the same pre-proccess we do to DEM
               //in all cases.                                                 
               if(!rasterInputCheck(flowRaster, msg))return false;//if empty q 
               infStr(fT,reportRasterParams(flowRaster,0));//flow input params 
               displayRasterASC(flowRaster, 0, "DEM_Raster",msg);              
               if(!msg.empty())errStr(fT, msg);                                
               useFlowRaster = true;                                           
           }                                                                   
           //int errorCode = setdir(demRaster, outputRaster, slopeRaster,      
           //                       flowRaster, useFlowRaster); //KEY LINE     
           //  Overloaded function that takes the extra TeRaster references    
           int error = setdir(demRaster, outputRaster, slopeRaster, flowRaster,
                              useFlowRaster, fweight, larr, fareaw, sdir,      
                              fangle, felevg, fslopeg, sapoolg);               
           if(error)                                                           
           {                                                                   
               errStr(fT, err(fT, "setdir returned error %03d", error));       
               return false;                                                   
           }                                                                   
           msg=inf(fT, "Raster created for layer: %s %s %3d",                  
                   outLayerName.c_str(),                                       
                  "filled with output of Set Dir error from sert dir: ",error);
           infStr(fT,msg);                                                     
           string slopeLayerName = outLayerName.replace(outLayerName.size()-5, 
                                                        4, "_Slp");            
           msg = reportRasterParams(slopeRaster, 0);                           
           infStr(fT, msg);                                                    
           displayRasterASC(outputRaster, 0, "angleRaster", msg);              
           if(!msg.empty())errStr(fT,msg);                                     
           displayRasterASC(slopeRaster, 0, "slopeRaster", msg);               
           if(!msg.empty())errStr(fT,msg);                                     
           //debug variables                                                   
           displayRasterASC(sdir, 0, "sdir", msg);                             
           if(!msg.empty())errStr(fT,msg);                                     
           displayRasterASC(fweight,0, "fweight", msg);                        
           if(!msg.empty())errStr(fT,msg);                                     
           displayRasterASC(larr, 0, "larr", msg);                             
           if(!msg.empty())errStr(fT,msg);                                     
           displayRasterASC(fareaw, 0, "fareaw", msg);                         
           if(!msg.empty())errStr(fT,msg);                                     
           displayRasterASC(fangle, 0, "fangle", msg);                         
           if(!msg.empty())errStr(fT,msg);                                     
           displayRasterASC(felevg, 0, "feleveg", msg);                        
           if(!msg.empty())errStr(fT,msg);                                     
           displayRasterASC(fslopeg,0,"fslopeg",msg);                          
           if(!msg.empty())errStr(fT,msg);                                     
           displayRasterASC(sapoolg, 0, "sapoolg", msg);                       
           if(!msg.empty())errStr(fT,msg);                                     
           //extra Raster imports used only for this option                    
           if(!layerOutput(plug_pars_ptr, slopeRaster, slopeLayerName))        
           return(false);                                                      
       }//if user chose setFlowDirection()                                     
       infStr(fT,reportRasterParams(demRaster, outputRaster, 0));              
                                                                               
       if(!layerOutput(plug_pars_ptr, outputRaster, outLayerName))             
           return(false);                                                      
   }//if plugin window returned from modal execution                           
   infStr(fT, timeCheck(startTime));                                           
   //For a reason I do not know a char* is only read by the %s format of a     
   //var_arg list if the string is either a static buffer with a compile time  
   //constant size or a char* pointing to dynamicaly allocated buffer, but a   
   //char* pointing to a static buffer with a compile time constant size will  
   //not have its contents read through the var_ar list, at least not read by  
   //int vsprintf(char*, const char*, va_args)                                 
   //                                                                          
   //int tStrSize = strlen(timeR_ptr);                                         
   //char* timeReport = (char*)malloc(tStrSize+2);//secure space for NULL char 
   //memset(timeReport, 0, sizeof(timeReport));// set what we allocated to grow
   //for(int t = 0; t <= tStrSize; t++)//write the NULL termination as well    
   //    timeReport[t] = timeR_ptr[t];//copy from static array to dynamic pointer
   //inf(fT,"%s\n", timeReport);//send dynamic pointer rather than static array
   //free(timeReport);//free dynamic pointer.                                  
   return true;//SUCCESS!!!! hopefully, this line will excute many times yet   
}//SPL_PLUGIN_API bool SPL_RUN_NAME_CODE( slcPluginArgs* a_pPluginArgs )       
                                                                               
void infStr(char* title, string message)//call from TV plugin. Requires Qt     
{                                                                              
   QMessageBox::information(main_window_instance_ptr, title,                   
                            QString(message.c_str() ) );                       
}                                                                              
                                                                               
void errStr(char* title, string message)//call from TV plugin. Requires Qt     
{                                                                              
   QMessageBox::critical(main_window_instance_ptr, title,                      
                         QString(message.c_str() ) );                          
}                                                                              
                                                                               
bool testPlugPars(PluginParameters* plug_pars_ptr,TeLayerMap& layerMap,        
                  vector<int>& layerIdList)                                    
{                                                                              
   char* fT = "EDA Plugin:TestPlugPars";//function title to all info and error 
   //windows from this function. Because this is a constant, it is defined     
   //here and reused whenever needed                                           
   main_window_instance_ptr->inputLayerCombo->clear();                         
   main_window_instance_ptr->inputFlowLayerCombo->clear();                     
   TeDatabase* currentDB_ptr = plug_pars_ptr->getCurrentDatabasePtr();         
   //get BD                                                                    
   if(!currentDB_ptr)//if current Database is NULL!                            
   {                                                                           
       errStr(fT,                                                              
              err(fT,                                                          
                  "No Database found! Select or create a database to use EDA." 
                 )                                                             
             );                                                                
       return false;//no DB, lets get out of here!                             
   }//if current Database is NULL!                                             
   //get layers from DB                                                        
   layerMap = currentDB_ptr->layerMap();                                       
   //we have a DB, get the layers                                              
   if(layerMap.empty())//if current database has no layer                      
   {                                                                           
       errStr(fT,                                                              
              err(fT,                                                          
                  "Selected database has no layers! Create one or more %s\n",  
                  "layers or select a different data base before using EDA."   
                 )                                                             
             );                                                                
       return false;//no layers, lets get out of here                          
   }//if current database has no layer                                         
   //finnally, get layers that have rasters from layer map                     
   TeLayerMap::iterator layerIter;//iterate through layers testing for Rasters 
   //vector<int> layerIdList;                                                  
   for(layerIter= layerMap.begin(); layerIter!= layerMap.end(); layerIter++)   
   {                                                                           
       if(layerIter->second->hasGeometry(TeRASTER)                             
       || layerIter->second->hasGeometry(TeRASTERFILE))                        
       {//if layer has a haster!                                               
           layerIdList.push_back(layerIter->first);//relate Id to index        
           string layerName = layerIter->second->name();//get layer name       
           //write names of layers in the combo box. Note that the combo box   
           //requires sequencial indexes instead of the layer Ids              
           main_window_instance_ptr->inputLayerCombo->                         
               insertItem(QString(layerName.c_str()), -1);                     
           main_window_instance_ptr->inputFlowLayerCombo->                     
               insertItem(QString(layerName.c_str()), -1);                     
       }//if layer has a haster!                                               
   }//for all layers in the map                                                
   //if(main_window_instance_ptr->CombinacaoDePlanosDeEntrada->count() == 0)   
   if(layerIdList.empty())                                                     
   {//if no layers have Rasters                                                
       errStr(fT,                                                              
              err(fT,"Selected database has no Rasters! Import one or more %s", 
                     "Rasters or Select a different data base before using EDA."
                 )                                                             
             );                                                                
       return false;//no Rasters in layers, lets get out of here!              
   }//if no layers have rasters                                                
   //copy first layer listed in the input combo box and paste it in output     
   //line edit, so the output field should never be empty unless the user      
   //makes it empty.                                                           
   main_window_instance_ptr->outputLayerLine->                                 
       insert(main_window_instance_ptr->inputFlowLayerCombo->currentText());   
   return(true);                                                               
}                                                                              
bool layerOutput(PluginParameters* plug_pars_ptr, TeRaster& outputRaster,      
                 string outLayerName)                                          
{                                                                              
   TeDatabase* currentDB_ptr = plug_pars_ptr->getCurrentDatabasePtr();         
   char* fT = "EDA Plugin: layer output";//title to info and error windows     
   string msg;                                                                 
   if(!makeGeoTiff(outputRaster, outLayerName, msg))                           
   {                                                                           
       errStr(fT, err(fT, "Raster to geotiff failed! \s", msg.c_str()));       
       return false;                                                           
   }                                                                           
   TeRaster* outputRaster_ptr = &outputRaster;                                 
   if(outputRaster_ptr == 0)                                                   
   {                                                                           
       errStr(fT, err(fT, "Raster referencing failed!"));                      
       return false;                                                           
   }//if output layer was not successfully produced                            
   if(QMessageBox::question(main_window_instance_ptr, fT,                      
                           QString("Make new layer?"),QString("No thanks"),    
                           QString("yes, do it"), QString::null, 0, 1)         
                           )                                                   
   {                                                                           
       TeLayer* outputLayer_ptr = TeImportRaster(outLayerName,                 
                                                 outputRaster_ptr,             
                                                 currentDB_ptr);               
       if(outputLayer_ptr == 0)                                                
       {                                                                       
           errStr(fT, err(fT, "Layer importation failed!"));                   
           return false;                                                       
       }//if output layer was not successfully produced                        
       int outputLayerId = outputLayer_ptr->id();//use to retrive layer        
       //WARMING: next command makes layer pointers invalid by closing the     
       //database connection. Database pointer must be renewed from plugin     
       //parameters.                                                           
       plug_pars_ptr->updateTVInterface();//destructive line                   
       //reconect to database and retrive layer                                
       currentDB_ptr = plug_pars_ptr->getCurrentDatabasePtr();                 
       TeLayerMap layerMap = currentDB_ptr->layerMap();                        
       outputLayer_ptr = layerMap[outputLayerId];                              
       if(QMessageBox::question(main_window_instance_ptr, fT,                  
                               QString("View layer?"),QString("Not now"),      
                               QString("yes, show me"),QString::null, 0, 1))   
       {                                                                       
           //Make a new theme and display output                               
           //Make unique view name                                             
           string outViewName =                                                
           makeUnique(outLayerName, mapToList(currentDB_ptr->viewMap()));      
           //create a projection to the view that of the output layer          
           TeProjection* outProjection =                                       
            TeProjectionFactory::make(outputLayer_ptr->projection()->params());
           //create a view and a theme on the main window for visualization add
           //the new view to the database and in the map of Views              
           TeView *outputView_ptr=new TeView(outViewName,currentDB_ptr->user());
           if(!outputView_ptr)                                                 
           {                                                                   
               errStr(fT, err(fT,"View creation failed!"));                    
               return(false);                                                  
           }                                                                   
           outputView_ptr->setCurrentBox(outputLayer_ptr->box());              
           outputView_ptr->setCurrentTheme(-1);                                
           outputView_ptr->projection(outProjection);                          
           if(!currentDB_ptr->insertView(outputView_ptr))                      
           {                                                                   
               delete outputView_ptr;//get rid of the useless view             
               errStr(fT, err(fT,"inserting view in database failed!"));       
               return(false);                                                  
           }                                                                   
       //plug_pars_ptr->teqtviewslistview_ptr_->selectViewItem(outputView_ptr);
           // create the theme that will be associated to this view            
           string outThemeName =                                               
             makeUnique(outLayerName,mapToList(currentDB_ptr->themeMap()));    
           TeTheme* outputTheme_ptr=new TeTheme(outThemeName,outputLayer_ptr); 
           if(!outputTheme_ptr)                                                
           {                                                                   
               delete outputTheme_ptr;//delete theme that cannot be built      
               errStr(fT, err(fT,"theme creation failed!"));                   
               return(false);                                                  
           }                                                                   
           outputView_ptr->add(outputTheme_ptr);                               
           outputView_ptr->setCurrentTheme(outputTheme_ptr->id());             
           outputTheme_ptr->visibility(true);                                  
           outputTheme_ptr->draw();                                            
           if(!outputTheme_ptr->save())                                        
           {                                                                   
               delete outputTheme_ptr;//delete theme that cannot be saved      
               errStr(fT, err(fT,"theme save failed!"));                       
               return(false);                                                  
           }                                                                   
                                                                               
           //pointers to DB and its contents become outdated and invalid       
           plug_pars_ptr->updateTVInterface();                                 
       }//if user chose to display new layer                                   
   }//if user chose to make a new layer                                        
   return(true);                                                               
}                                                                              

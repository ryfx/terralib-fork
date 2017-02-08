#include "EDA_SharedFunctions.h"                                               
                                                                               
string timeCheck(time_t startTime)                                             
{                                                                              
    char* fT = "EDA Plugin: TimeCheck";                                        
   //get current time in 2 usefull forms. time_t, and char[30]                 
   time_t finishTime = time(0);//do this after all relevant proccessement      
   char startString[30];//hold human readable time stamp in its own buffer     
   memset(startString, '\0', sizeof(startString));//clear buffer               
   strcpy(startString, ctime(&startTime));//deep copy the static return to it  
   char finishString[30]; memset(finishString, '\0', sizeof(finishString));    
   strcpy(finishString, ctime(&finishTime));                                   
   double runTime = difftime(finishTime, startTime);                           
   char timeReport[1000];  memset( timeReport,  '\0', sizeof(timeReport) );    
   char beginReport[300];  memset( beginReport, '\0', sizeof(beginReport) );   
   char end__Report[300];  memset( end__Report, '\0', sizeof(end__Report) );   
   char diff_Report[300];  memset( diff_Report, '\0', sizeof(diff_Report) );   
   int b, e, d, t;//begin, end difference, total.                              
   b = sprintf(beginReport,"Start     :%ld or %s \n", startTime, startString); 
   e = sprintf(end__Report,"Finish    :%ld or %s \n", finishTime,finishString);
   d = sprintf(diff_Report,"Difference: %G", runTime);                         
   t = sprintf(timeReport, "Time report \n \n %s%s%s", beginReport,            
               end__Report, diff_Report);                                      
   return(inf(fT,"%s\n", timeReport));                                         
}                                                                              
NameList_T mapToList(TeProjectMap& map)                                        
{                                                                              
   TeProjectMap::iterator mit;//get an iterator for that map                   
   NameList_T list;//declare vector of strings                                 
   for(mit = map.begin(); mit!= map.end(); ++mit)//for all pairs in map        
       list.push_back(mit->second->name());//add TeObject name to list         
   return(list);//return(&list);??                                             
}                                                                              
NameList_T mapToList(TeLayerMap&   map)                                        
{                                                                              
   TeLayerMap::iterator mit;//get an iterator for that map                     
   NameList_T list;//declare vector of strings                                 
   for(mit = map.begin(); mit!= map.end(); ++mit)//for all pairs in map        
       list.push_back(mit->second->name());//add TeObject name to list         
   return(list);//return(&list);??                                             
}                                                                              
NameList_T mapToList(TeThemeMap&   map)                                        
{                                                                              
   TeThemeMap::iterator mit;//get an iterator for that map                     
   NameList_T list;//declare vector of strings                                 
   for(mit = map.begin(); mit!= map.end(); ++mit)//for all pairs in map        
       list.push_back(mit->second->name());//add TeObject name to list         
   return(list);//return(&list);??                                             
}                                                                              
NameList_T mapToList(TeViewMap&    map)                                        
{                                                                              
   TeViewMap::iterator mit;//get an iterator for that map                      
   NameList_T list;//declare vector of strings                                 
   for(mit = map.begin(); mit!= map.end(); ++mit)//for all pairs in map        
       list.push_back(mit->second->name());//add TeObject name to list         
   return(list);//return(&list);??                                             
}                                                                              
string makeUnique(string& name, NameList_T& list)                              
{                                                                              
   int size = list.size();//used to avoid uneccessary method calls             
   int index = 0;//used to control inner for loop                              
   int extNum = 0;//used to make a new name if need.                           
   //bool flag = true;//used to conmtrol outer while loop                      
   string temp = name;//used to record the unique name once found several      
                      // extentions to the original name                       
   while(true)                                                                 
   {                                                                           
       for(index = 0; index < size; ++index)                                   
       {//this is basecally seaching for name in the list                      
           if(temp == list[index])// temp = name + latest extention            
               break;//exit inner for loop with a name known not to be unique  
       }//for all names in list                                                
       if(index >= size)//if we searched the whole list and did not find name  
           return(temp);//temp(temp = name + ext) did not mach anme in te list,
                        //thus it is unique and we are done.                   
       else                                                                    
       {                                                                       
           char ext[10]; memset(ext, '\0', sizeof(ext));//hold likely extention
           sprintf(ext, "_%d", extNum);//ready extention for a string          
           extNum++;//increment num in case we have make new extention to name 
           temp = name + ext;//name itself is never changed in the while loop  
       }//else or if name was found in the list, therefore it is not unique.   
   }//while no unique name was found                                           
   return(name);//unconditional return line used just to avoid a warning       
}                                                                              
string inf(char* title, const char* text, ...)//adapter to information window  
{                                                                              
   string message(title);//start with function identification title.           
   va_list args;//pointer -> variable size list of arguments                   
   va_start(args,text);                                                        
   char fullText[BUFFER_SIZE]; memset(fullText, '\0',BUFFER_SIZE);             
   //int c = sprintf(fullText, text, ...  );                                   
   if( vsprintf(fullText, text, args ) < 0)                                    
       message= message +": how did manage to overflow the 65530 bytes buffer?";
   va_end(args);                                                               
   message = message + ": " + fullText;                                        
   return(message);                                                            
}                                                                              
                                                                               
string err(char* title, const char* text, ...)//adapter to critical error      
{                                                                              
   string message(title);//start with function identification title.           
   va_list args;                                                               
   char fullText[BUFFER_SIZE]; memset(fullText, '\0',BUFFER_SIZE);             
   va_start(args,text);                                                        
   if( vsprintf(fullText, text, args ) < 0)                                    
       message= message +"how did manage to overflow the 65530 bytes buffer?"; 
   va_end(args);                                                               
   message = message + ": " + fullText;                                        
   return(message);                                                            
}                                                                              
                                                                               
bool rasterInputCheck(TeRaster& iGrid, string& msg)                            
{                                                                              
   char* fT = "EDA Plugin: Raster Input Check";//function title to all info    
   //and error windows                                                         
   bool isDummy = true;                                                        
   bool isFirst = true;                                                        
   int band = iGrid.params().nBands();                                         
   int cols = iGrid.params().ncols_;                                           
   int rows = iGrid.params().nlines_;                                          
   int b, c, r;                                                                
   double dummy = 0.0;                                                         
   double first = 0.0;                                                         
   double value = 0.0;                                                         
   //search entire raster for at least one valid element                       
   for(b = 0; b < band; b++)                                                   
   {                                                                           
       dummy = iGrid.params().dummy_[b];//get dummy for band                   
       iGrid.getElement( 0, 0, first, b);//get first element for band          
       for(r = 0; r < rows; r++)                                               
       {                                                                       
           for(c = 0; c< cols; c++)                                            
           {                                                                   
               iGrid.getElement(c, r, value, b);//get individual value         
               isDummy = (value == dummy);//check if it is dummy               
               isFirst = (value == first);//check if it is == first value      
               //exit if  element is not dummy and it is not first, we have a  
               //valid element,; Exit inner-loop                               
               if(!isDummy && !isFirst)break;                                  
           }//for all lines                                                    
           //exit if  element is not dummy and it is not first, we have a      
           //valid element,; Exit mid-loop                                     
           if(!isDummy && !isFirst)break;                                      
       }//for all rows                                                         
       //exit if  element is not dummy and it is not first, we have a valid    
       //element,; Exit outter-loop                                            
       if(!isDummy && !isFirst)break;                                          
   }//for all bands                                                            
   //report if needed                                                          
   if(isDummy)                                                                 
   {                                                                           
       msg = err(fT, "all elements of input raster are dummies!");             
       return(false);                                                          
   }                                                                           
   if(isFirst)                                                                 
   {                                                                           
       msg = err(fT, "all elements of input raster are the same!");            
       return(false);                                                          
   }                                                                           
//when the line immediately below executes the user will have no idea but the  
//entire raster has been checked and found to contain more than just one value 
//least of all more than just dummies.                                         
   return(true);//silence is golden                                            
}//bool rasterInputCheck(TeRaster& iGrid)                                      
                                                                               
string reportRasterParams(TeRaster& grid, int band)                            
{                                                                              
   char* fT = "EDA Plugin: Raster. params() ";                                 
   TeRasterParams iP = grid.params();                                          
   char ba[101], co[101], li[101], bp[101], ud[101], sv[101], rm[101], du[101],
        mi[101], ma[101], rx[101], ry[101], di[101], pn[101], pd[101],         
        iTemp[101], em[201], iErr[101], all[2021];                             
   //reserving memory                                                          
   memset( ba, '\0', sizeof(ba) );       memset( co, '\0', sizeof(co) );       
   memset( li, '\0', sizeof(li) );       memset( bp, '\0', sizeof(bp) );       
   memset( ud, '\0', sizeof(ud) );       memset( sv, '\0', sizeof(sv) );       
   memset( rm, '\0', sizeof(rm) );       memset( du, '\0', sizeof(du) );       
   memset( mi, '\0', sizeof(mi) );       memset( ma, '\0', sizeof(ma) );       
   memset( rx, '\0', sizeof(rx) );       memset( ry, '\0', sizeof(ry) );       
   memset( di, '\0', sizeof(di) );       memset( pn, '\0', sizeof(pn) );       
   memset( pd, '\0', sizeof(pd) );       memset( em, '\0', sizeof(em) );       
   memset( iTemp, '\0', sizeof(iTemp) ); memset( iErr, '\0', sizeof(iErr) );   
   memset( all, '\0', sizeof(all));                                            
   char* iTemp_ptr = iTemp;                                                    
   //char* iErr_ptr = iErr;                                                    
   // Trivial params: number bands, number of colunms, number of lines, number 
   //of bits per pixel, use dummy(bool), memory swap(bool), raster mode, dummy,
   // value, maximum value, minimum value, X resolution, and Y resolution, are 
   // primitive variables and easy to get if the TeRasterParams Object has     
   //been initialized.                                                         
   sprintf(ba, " Number of Bands_____: in = %04d \n", grid.nBands());          
   sprintf(co, " Number of columns___: in = %04d \n", iP.ncols_);              
   sprintf(li, " Number of lines_____: in = %04d \n", iP.nlines_);             
   sprintf(bp, " Number of Bits/Pixel: in = %08d \n", iP.nbitsperPixel_);      
   sprintf(ud, " uses dummy values___: in = %08d \n", iP.useDummy_);           
   sprintf(sv, " Swaps values________: in = %08d \n", iP.swap_);               
   sprintf(rm, " Raster Mode_________: in = %04c \n", iP.mode_);               
   sprintf(du, " Dummy values: in = %G \n", iP.dummy_);                        
   sprintf(mi, " Minimum values: in = %G \n", iP.vmin_[band]);                 
   sprintf(ma, " Maximum values: in = %G \n", iP.vmax_[band]);                 
   sprintf(rx, " Resolution: in = %G \n", iP.resx_);                           
   sprintf(ry, " Resolution: in = %G \n", iP.resy_);                           
   // Less trivial parameters: Decoder and Projection are objects with some one
   //interesting attribute each and they may not always be initialized, thus   
   //output about their state is less trivial to get.                          
   iTemp_ptr = const_cast<char *>(iP.decName().c_str());                       
   sprintf(di, " Decoder: in = %s \n", iTemp_ptr);                             
   if(iP.projection())                                                         
       iTemp_ptr = const_cast<char *>(iP.projection()->name().c_str());        
   else                                                                        
       iTemp_ptr = " N/A ";                                                    
   sprintf(pn, " projection: in = %s \n", iTemp_ptr);                          
   if(iP.projection())                                                         
       iTemp_ptr = " see above ";                                              
   else                                                                        
       iTemp_ptr = " N/A ";                                                    
   sprintf(pd, " projection: in = %s \n", iTemp_ptr);                          
   sprintf(iErr, " iRasterError: %s \n", grid.errorMessage().c_str());         
   sprintf(em, "%s", iErr);                                                    
   sprintf(all,"%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s",
                ba, co, li, bp, ud, sv, rm, du, mi, ma, rx, ry, di, pn, pd, em);
   return(inf(fT, "Raster object params: \n%s", all));                         
}                                                                              
string reportRasterParams(TeRaster& iGrid, TeRaster& oGrid, int band)          
{                                                                              
   char* fT = "EDA Plugin: Input Raster | Output Raster";                      
   TeRasterParams iP = iGrid.params();                                         
   TeRasterParams oP = oGrid.params();                                         
   char ba[201], co[201], li[201], bp[201], ud[201], sv[201], rm[201], du[201],
        mi[201], ma[201], rx[201], ry[201], di[201], pn[201], pd[201],         
        iTemp[201], oTemp[201], em[401], iErr[201], oErr[201], all[4201];      
                                                                               
   memset( ba, '\0', sizeof(ba) );       memset( co, '\0', sizeof(co) );       
   memset( li, '\0', sizeof(li) );       memset( bp, '\0', sizeof(bp) );       
   memset( ud, '\0', sizeof(ud) );       memset( sv, '\0', sizeof(sv) );       
   memset( rm, '\0', sizeof(rm) );       memset( du, '\0', sizeof(du) );       
   memset( mi, '\0', sizeof(mi) );       memset( ma, '\0', sizeof(ma) );       
   memset( rx, '\0', sizeof(rx) );       memset( ry, '\0', sizeof(ry) );       
   memset( di, '\0', sizeof(di) );       memset( pn, '\0', sizeof(pn) );       
   memset( pd, '\0', sizeof(pd) );       memset( em, '\0', sizeof(em) );       
   memset( iTemp, '\0', sizeof(iTemp) ); memset( oTemp, '\0', sizeof(oTemp) ); 
   memset( iErr, '\0', sizeof(iErr) );   memset( oErr, '\0', sizeof(oErr) );   
   memset( all, '\0', sizeof(all));                                            
   char* iTemp_ptr = iTemp;                                                    
   char* oTemp_ptr = oTemp;                                                    
   //char* iErr_ptr = iErr;                                                    
   //char* oErr_ptr = oErr;                                                    
   // Trivial params: number bands, number of colunms, number of lines, number 
   //of bits per pixel, use dummy(bool), memory swap(bool), raster mode, dummy,
   // value, maximum value, minimum value, X resolution, and Y resolution, are 
   // primitive variables and easy to get if the TeRasterParams Object has     
   //been initialized.                                                         
   sprintf(ba, " Number of Bands_____: in = %04d | out = %04d \n",             
           iGrid.nBands(), oGrid.nBands());                                    
   sprintf(co, " Number of columns___: in = %04d | out = %04d \n",             
           iP.ncols_, oP.ncols_);                                              
   sprintf(li, " Number of lines_____: in = %04d | out = %04d \n",             
           iP.nlines_, oP.nlines_);                                            
   sprintf(bp, " Number of Bits/Pixel: in = %08d | out = %08d \n",             
           iP.nbitsperPixel_, oP.nbitsperPixel_);                              
   sprintf(ud, " uses dummy values___: in = %08d | out = %08d \n",             
           iP.useDummy_, oP.useDummy_);                                        
   sprintf(sv, " Swaps values________: in = %08d | out = %08d \n",             
           iP.swap_, oP.swap_);                                                
   sprintf(rm, " Raster Mode_________: in = %04c | out = %04c \n",             
           iP.mode_, oP.mode_);                                                
   sprintf(du, " Dummy values: in = %G out = %G \n",                           
           iP.dummy_, oP.dummy_);                                              
   sprintf(mi, " Minimum values: in = %G out = %G \n",                         
           iP.vmin_[band], oP.vmin_[band]);                                    
   sprintf(ma, " Maximum values: in = %G out = %G \n",                         
           iP.vmax_[band], oP.vmax_[band]);                                    
   sprintf(rx, " Resolution: in = %G out = %G \n",                             
           iP.resx_, oP.resx_);                                                
   sprintf(ry, " Resolution: in = %G out = %G \n",                             
           iP.resy_, oP.resy_);                                                
   // Less trivial parameters: Decoder and Projection are objects with some one
   //interesting attribute each and they may not always be initialized, thus   
   //output about their state is less trivial to get.                          
   iTemp_ptr = const_cast<char *>(iP.decName().c_str());                       
   oTemp_ptr = const_cast<char *>(oP.decName().c_str());                       
   sprintf(di, " Decoder: in = %s out = %s \n", iTemp_ptr, oTemp_ptr);         
                                                                               
   if(iP.projection())                                                         
       iTemp_ptr = const_cast<char *>(iP.projection()->name().c_str());        
   else                                                                        
       iTemp_ptr = " N/A ";                                                    
   if(oP.projection())                                                         
       oTemp_ptr = const_cast<char *>(oP.projection()->name().c_str());        
   else                                                                        
       oTemp_ptr = " N/A ";                                                    
                                                                               
   sprintf(pn, " projection: in = %s out = %s \n", iTemp_ptr, oTemp_ptr);      
                                                                               
   if(iP.projection())                                                         
       iTemp_ptr = " see above ";                                              
   else                                                                        
       iTemp_ptr = " N/A ";                                                    
   if(oP.projection())                                                         
       oTemp_ptr = " see above ";                                              
   else                                                                        
       oTemp_ptr = " N/A ";                                                    
                                                                               
   sprintf(pd, " projection: in = %s out = %s \n", iTemp_ptr, oTemp_ptr);      
                                                                               
   sprintf(iErr, " iRasterError: %s \n", iGrid.errorMessage().c_str());        
   sprintf(oErr, " oRasterError: %s \n", oGrid.errorMessage().c_str());        
   sprintf(em, "%s%s", iErr, oErr);                                            
   sprintf(all,"%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s",
                ba, co, li, bp, ud, sv, rm, du, mi, ma, rx, ry, di, pn, pd, em);
                                                                               
   return(inf(fT, "Raster object params: \n%s", all));                         
}                                                                              
void displayRasterASC(TeRaster& grid, int band, char* rasterName, string& msg) 
{                                                                              
   //function title to all info and error windows                              
   char* fT = "EDA Plugin: Display Raster.asc";                                
   int rows = grid.params().nlines_;                                           
   int cols = grid.params().ncols_;//get columns NOT lines!                    
   //For each point of the input grid                                          
   int line,column;                                                            
   int c = 0;                                                                  
   //the name of the output file is the given <rasterName>.asc                 
   char fileName[256];//just to make sure we have space                        
   char* ext = ".asc";                                                         
   int baseLen = strlen(rasterName);                                           
   int extLen  = strlen(ext);                                                  
   memset(fileName, '\0', baseLen + extLen + 1);                               
   for(int ch = 0; ch < baseLen; ch++)                                         
       fileName[ch] = rasterName[ch];                                          
   for(int ch = 0; ch < extLen; ch++)                                          
       fileName[ch + baseLen] = ext[ch];                                       
                                                                               
   FILE* ascOut = fopen(fileName, "w");                                        
   if(ascOut == NULL) { msg=err(fT, "failed to open file!"); return; }         
   fprintf(ascOut,"ncols         %d\n", cols);                                 
   fprintf(ascOut,"nrows         %d\n", rows);                                 
   fprintf(ascOut,"xllcorner     %f\n", grid.params().boundingBox().x1_);      
   fprintf(ascOut,"yllcorner     %f\n", grid.params().boundingBox().y2_);      
   fprintf(ascOut,"cellsize      %f\n", grid.params().resx_);                  
   fprintf(ascOut,"NODATA_value  %3.6f\n", grid.params().dummy_[band]);        
   for(line = 0; line < rows; line++)//SSL0296                                 
   {                                                                           
       //if((rows/5*2 <= line)&&(line < rows/5 *3))//for central 1/5 of Raster 
       //if(line %100 ==0)//for 1% of raster (presision reduced)               
       for( column = 0; column < cols; column++)                               
       {                                                                       
           double v = 0;                                                       
           grid.getElement(column, line, v, band);                             
           //if(( cols/5*2 <= column)&&(column < cols/5 *3))//for central 1/5  
           //if(column % 100 ==0)//for 1% of raster (presision reduced)        
           c = fprintf(ascOut,"%010.6f ", v);                                  
       }//for all columns in Raster                                            
       //if((rows/5*2 <= line)&&(line < rows/5 *3))//for central 1/5 of Raster 
       //if(line % 100 ==0)//for 1% of raster (presision reduced)              
       c = fprintf(ascOut," \n"); //cout<<endl;                                
   }//for all lines in Raster                                                  
   fclose(ascOut);                                                             
}                                                                              
TePDITypes::TePDIRasterPtrType makeSharedPtrMemCopy(TeRaster& outputRaster,    
                                                    string& msg)               
{                                                                              
   char* fT = "EDA Plugin:makeSharedPtrMemCopy";                               
   TePDITypes::TePDIRasterPtrType RAM_out_Raster(new TeRaster);                
   TeRasterParams RAM_out_Params = outputRaster.params();                      
   RAM_out_Params.decoderIdentifier_ ="SMARTMEM";//now, this matters           
   RAM_out_Raster->updateParams(RAM_out_Params);//pass params to new Raster    
   TeDecoder* dec_ptr = new TeDecoderSmartMem( RAM_out_Params );               
   RAM_out_Raster->setDecoder(dec_ptr);                                        
   if(!RAM_out_Raster->init())//stop if init FAILS!                            
   {                                                                           
       msg=err(fT,"RAM Raster init failed!");                                  
       return TePDITypes::TePDIRasterPtrType(0);                               
   }                                                                           
   //copy raster contents indiscriminately                                     
   int nBands = RAM_out_Params.nBands();//number of bands                      
   int nRows  = RAM_out_Params.nlines_;//number of lines                       
   int nCols  = RAM_out_Params.ncols_;//number of colunms                      
   double value = 0.0;//value to be copied                                     
   for(int b = 0; b < nBands; b++)//for all bands                              
       for(int r = 0; r < nRows; r++)//for all lines                           
           for(int c = 0; c < nCols; c++)//for all colunms                     
           {                                                                   
               outputRaster.getElement(c, r, value, b);//get value             
               RAM_out_Raster->setElement(c, r, value, b);//copy value         
           }                                                                   
   return(RAM_out_Raster);                                                     
}                                                                              
                                                                               
bool makeGeoTiff(TeRaster& outputRaster, string outLayerName, string& msg)     
{                                                                              
   //Save the raster to a GeoTIFF file.                                        
   char* fT = "EDA Plugin:MakeGeoTiff";                                        
   char timeStamp[64]; memset(timeStamp, '\0', sizeof(timeStamp));             
   sprintf(timeStamp, "%ld", time(0));//use timeStamp to make a filename       
   string fileName = outLayerName + timeStamp +".tif";//need comment here?     
   TePDITypes::TePDIRasterPtrType RAM_out_Raster =                             
                                  makeSharedPtrMemCopy(outputRaster, msg);     
   TeDataType tdt = outputRaster.params().dataType_[0];                        
   if(!TePDIUtils::TeRaster2Geotiff(RAM_out_Raster, fileName.c_str(),tdt))     
   {                                                                           
       msg=err(fT, "Raster file write failed!");                               
       return false;                                                           
   }                                                                           
   return true;                                                                
}                                                                              
bool calcLocalGradient(TeRaster& grid, int band, int row,int col, double& dzdx,
                       double& dzdy, string& msg)                              
{                                                                              
   char* fT = "EDA Plugin: calcLocalGradient";                                 
   bool gradient = true;                                                       
   double subGrid[3][3];//the grid of cells surronding the center cell         
   double dx = grid.params().resx_;                                            
   double dy = grid.params().resy_;                                            
   double maxValue = grid.params().vmax_.at(band);                             
   // I assume the vmax_ vector holds max element values indexed by the band.  
   //Note that the local matrix is stored in row major order despite           
   //getElement()s requiring (col,row) in the order of its parameters. Row,    
   //major order was chosen to require less modifications in code below.       
   //Still, indices after the next 9 lines should refer to the local subGrid,  
   //instead of the given Raster Object. quit if any of the 9 calls to         
   //getElement() fails, for all 9 are neccessary.                             
   if(!grid.getElement(col-1, row-1, subGrid[0][0], band))                     
   //)&&(zvalue != zvaluedum)                                                  
   {                                                                           
       msg=err(fT,"getElement(col-1, row-1) (%03d, %03d) = %G failed",         
               col-1, row-1, subGrid[0][0]);                                   
       return false;                                                           
   }                                                                           
   if(!grid.getElement(col  , row-1, subGrid[0][1], band))                     
   {                                                                           
       msg=err(fT,"getElement(col  , row-1) (%03d, %03d) = %G failed",         
               col  , row-1, subGrid[0][1]);                                   
       return false;                                                           
   }                                                                           
   if(!grid.getElement(col+1, row-1, subGrid[0][2], band))                     
   {                                                                           
       msg=err(fT,"getElement(col+1, row-1) (%03d, %03d) = %G failed",         
               col+1, row-1, subGrid[0][2]);                                   
       return false;                                                           
   }                                                                           
                                                                               
   if(!grid.getElement(col-1, row  , subGrid[1][0], band))                     
   {                                                                           
       msg=err(fT,"getElement(col-1, row-1) (%03d, %03d) = %G failed",         
               col-1, row  , subGrid[1][0]);                                   
       return false;                                                           
   }                                                                           
   if(!grid.getElement(col  , row  , subGrid[1][1], band))                     
   {                                                                           
       msg=err(fT,"getElement(col  , row  ) (%03d, %03d) = %G failed",         
               col  , row  , subGrid[1][1]);                                   
       return false;                                                           
   }                                                                           
   if(!grid.getElement(col+1, row  , subGrid[1][2], band))                     
   {                                                                           
       msg=err(fT,"getElement(col+1, row  ) (%03d, %03d) = %G failed",         
               col+1, row  , subGrid[1][2]);                                   
       return false;                                                           
   }                                                                           
                                                                               
   if(!grid.getElement(col-1, row+1, subGrid[2][0], band))                     
   {                                                                           
       msg=err(fT,"getElement(col-1, row+1) (%03d, %03d) = %G failed",         
               col-1, row+1, subGrid[2][0]);                                   
       return false;                                                           
   }                                                                           
   if(!grid.getElement(col  , row+1, subGrid[2][1], band))                     
   {                                                                           
       msg=err(fT,"getElement(col  , row+1) (%03d, %03d) = %G failed",         
               col  , row+1, subGrid[2][1]);                                   
       return false;                                                           
   }                                                                           
   if(!grid.getElement(col+1, row+1, subGrid[2][2], band))                     
   {                                                                           
       msg=err(fT,"getElement(col+1, row+1) (%03d, %03d) = %G failed",         
               col+1, row+1, subGrid[2][2]);                                   
       return false;                                                           
   }                                                                           
                                                                               
   if(   (subGrid[0][1]<=maxValue)&&(subGrid[1][0]<=maxValue)                  
       &&(subGrid[2][1]<=maxValue)&&(subGrid[1][2]<=maxValue)                  
     )//if + <= maxValue ('+' = horizontal and vertical center lines)          
       {                                                                       
           if(  (subGrid[0][0]<=maxValue)&&(subGrid[0][2]<=maxValue)           
              &&(subGrid[2][0]<=maxValue)&&(subGrid[2][2]<=maxValue)           
             )//if X <= maxValue ('X' = both diagonals)                        
           {                                                                   
               //dz/dx and dz/dy derivative values with neighbourhood 8        
               dzdx  = ( (                                                     
                             ( subGrid[2][2] + 2.*subGrid[1][2]                
                             + subGrid[0][2])/*right col*/                     
                             -( subGrid[2][0] + 2.*subGrid[1][0]               
                              + subGrid[0][0])/*left col*/                     
                       )/(8.*dx) );                                            
                                                                               
               dzdy  = ( (                                                     
                             ( subGrid[2][2] + 2.*subGrid[2][1]                
                             + subGrid[2][0])/*botton row*/                    
                            -( subGrid[0][2] + 2.*subGrid[0][1]                
                             + subGrid[0][0])/*top row*/                       
                       )/(8.*dy) );                                            
           }//if X <= maxValue                                                 
           else//if X > maxValue                                               
           {                                                                   
               //dz/dx and dz/dy derivative values with the neighbourhood 4    
               dzdx  = (double)( (subGrid[1][2]                                
                     - subGrid[1][0])/(2.*dx) );/*middle row*/                 
               dzdy  = (double)( (subGrid[2][1]                                
                     - subGrid[0][1])/(2.*dy) );/*middle col*/                 
           }//else-if X > maxValue                                             
       }//if + <= maxValue                                                     
       else//if + > maxValue                                                   
       {//neighbourhood 4 using the corners                                    
           if(  (subGrid[0][0]<=maxValue)                                      
              &&(subGrid[0][2]<=maxValue)/*top row*/                           
              &&(subGrid[2][0]<=maxValue)                                      
              &&(subGrid[2][2]<=maxValue)/*botton row*/                        
             )//if = <=maxValue ('=' = top and botton rows)                    
           {                                                                   
               //dz/dx and dz/dy derivative values with neighbourhood 8        
               dzdx  = (double)                                                
                       ( (                                                     
                           (subGrid[2][2] + subGrid[0][2])/*right col*/        
                          -(subGrid[2][0] + subGrid[0][0])/*left col*/         
                       )/(4.*dx) );                                            
                                                                               
               dzdy  = (double)                                                
                       ( (                                                     
                           (subGrid[2][2] + subGrid[2][0])/*botton row*/       
                          -(subGrid[0][2] + subGrid[0][0])/*top row*/          
                       )/(4.*dy) );                                            
           }//if = <=maxValue                                                  
           else//else-if = > maxValue                                          
               gradient = false;                                               
       }//else-if + > maxValue                                                 
                                                                               
   return gradient;                                                            
}//short calcLocalGradient(TeRaster&, int, int, int, double&, double&, char*)  

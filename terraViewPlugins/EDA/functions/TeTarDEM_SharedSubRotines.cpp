
#include "TeTarDEM_SharedSubRotines.h"

void setMax(TeRasterParams& params, double max, int band)
{
   int nBands = params.nBands();//why call it multiple times? 
   if((band < 0) || (band >= nBands))//set max for all bands
   {   
       params.vmax_.clear();//avoid the chance of inconsistency
       for(int b = 0; b < nBands; b++)//for all bands
           params.vmax_.push_back(max);
   }//if band < 0 or band >= nBands
   else 
       params.vmax_[band] = max;
}//void setMax(TeRasterParams& params, double max, int band)

void setMin(TeRasterParams& params, double min, int band)
{
   int nBands = params.nBands();//why call it multiple times? 
   if((band < 0) || (band >= nBands))//set max for all bands
   {   
       params.vmin_.clear();//avoid the chance of inconsistency
       for(int b = 0; b < nBands; b++)//for all bands
           params.vmin_.push_back(min);
   }//if band < 0
   else 
       params.vmin_[band] = min;
}//void setMax(TeRasterParams& params, double max, int band)

bool rasterInit(TeRaster& newRaster, TeRasterParams& newRasterParams)
{  //pass new raster parameters to new raster.
   newRaster.updateParams(newRasterParams);//pass values to new Raster
   TeDecoder* dec = new TeDecoderSmartMem(newRasterParams);
   newRaster.setDecoder(dec);
   if(!newRaster.init())// make new Raster ready to use
       return false;
   return true;
}
                                                                            
void paramInit(TeRasterParams& newRasterParams, bool useDummy, int band, 
               int row, int col, TeDataType generalType, double resx, 
               double resy, double generalMax, double generalMin, 
               double generalDummy, double x1, double y1, double x2, double y2, 
               char mode, char* decoderId)
{  //save the client the trouble of writing several lines to set params, 
   //at the expance of providing a 17 parameters method that limits the client 
   //to one to one max one min and one dummy for all bands. Never the less,
   //the client can still write all lines below and hope to not forget any.
   newRasterParams.setNLinesNColumns(row, col);
   newRasterParams.nBands(band);
   newRasterParams.setDataType(generalType, -1);
   newRasterParams.useDummy_ = useDummy;
   newRasterParams.setDummy( generalDummy, -1 );
   newRasterParams.resx_ = resx; 
   newRasterParams.resy_ = resy;
   newRasterParams.mode_ = mode;
   newRasterParams.decoderIdentifier_ = decoderId;
   TeBox tempBox = newRasterParams.boundingBox();
   tempBox.x1_ = x1;
   tempBox.y1_ = y1;
   tempBox.x2_ = x2;
   tempBox.y2_ = y2;

   setMax(newRasterParams, generalMax, -1);
   setMin(newRasterParams, generalMin, -1);
}


bool allocategrid(TeRaster &grid, TeRasterParams &params, double value)
{
   if(!rasterInit(grid, params)) return false;
   int band = grid.params().nBands();
   int row  = grid.params().nlines_;
   int col  = grid.params().ncols_;
   double dum = grid.params().dummy_[0];
   for(int b = 0; b < band; b++)
       for(int r = 0; r < row; r++) 
           for(int c = 0; c < col; c++)
               if(!grid.setElement(c, r, value, b) && (value != dum))
                   return false;
   return true;
}

bool allocategrid(TeRaster &grid, double value)
{
   int band = grid.params().nBands();
   int row  = grid.params().nlines_;
   int col  = grid.params().ncols_;
   double dum = grid.params().dummy_[0];
   for(int b = 0; b < band; b++)
       for(int r = 0; r < row; r++) 
           for(int c = 0; c < col; c++)
               if(!grid.setElement( c, r, value, b) && (value != dum))
                   return false;
   return true;
}

double getElement(TeRaster & grid, int i, int j, bool& itWorked)
{
   double val = grid.params().dummy_[0]; 
   itWorked = grid.getElement(i, j, val, 0);
   return(val);
}

double d(TeRaster & grid, int i, int j)
{
   double val = grid.params().dummy_[0]; 
   grid.getElement(i, j, val, 0);
   return(val);
}

bool d(TeRaster& grid,int i,int j,double val){return(grid.setElement(i,j,val,0));}


bool gridread(TeLayer& iLayer, TeRaster& grid)                                 
{   //Make sure layer has a raster geometry before copying it to a Raster      
    if(!(iLayer.hasGeometry(TeRASTER) || iLayer.hasGeometry(TeRASTERFILE)))    
        return false;//no raster in layer                                      
   TeRaster& iRaster = *iLayer.raster("", 'r');                                
   return(gridread(iRaster, grid));//see below and do not re-invent the whell  
}//bool gridread(TeLayer& iLayer, TeRaster& grid)                              

bool gridread(TeRaster& iRaster, TeRaster& grid)                               
{                                                                              
    TeRasterParams iParams = iRaster.params();                                 
   //record raster in memory for fast proccess                                 
   //The Params of the new Raster bust be exact copy of the original one except
   //for the decoder that must be SMARTMEM for faster memory proccessing and   
   //the mode that must be 'c' to allow the raster´s client code to get and    
   //set data.                                                                 
   //iParams.setDataType(-TeDOUBLE, -1);                                       
                                                                               
   // It wouild be nice if we could simply copy the parameters from the DB     
   //raster to the smart mem raster, but this functions require a dummy which  
   //the DB raster migth not have. TeMAXFLOAT is the default dummy used unless 
   //the DB raster has something bigger. TeMAXFLOAT == 3.4E37 while the max    
   //double is over 300 digits long. TerraView does not  use Scientific        
   //notation in its parameter display                                         
   iParams.setDataType(TeDOUBLE, -1);//make sure -TeMAXFLOAT will not oveflow
   if(iParams.useDummy_ == false)                                              
   {                                                                           
       iParams.useDummy_ = true;                                               
       iParams.setDummy( -TeMAXFLOAT, -1);                                     
   }                                                                           
   else //if iRaster has dummy and dummy(for band 0)not in [-TeMAXFLOAT to -1] 
       if(iParams.dummy_[0] < -TeMAXFLOAT || iParams.dummy_[0] > -1)           
           iParams.setDummy(-TeMAXFLOAT, -1);                                  
   iParams.mode_ = 'c';//read and write                                        
   iParams.decoderIdentifier_ = "SMARTMEM";//now, this matters                 
   grid.updateParams(iParams);//pass params to new Raster                      
   TeDecoder* dec_ptr = new TeDecoderSmartMem( iParams );//make new decoder    
   grid.setDecoder(dec_ptr);//pass decoder to new Raster                       
   //Use the new params and the new decoder to init grid for the data          
   if(! grid.init())// make new Raster ready to use                            
   {                                                                           
       return false;                                                           
   }                                                                           
   // Copy iRaster contents  to grid indiscriminately                          
   // Before the lines bellow, gridread did all that Tarbodon's original did   
   //except actually reading data into the grid!                               
   int band = grid.params().nBands();                                          
   int rows = grid.params().nlines_;                                           
   int cols = grid.params().ncols_;                                            
   double value = 0.0;//just to avoid a non-initialized warning                
   for(int b = 0; b < band; b++)                                               
       for(int r = 0; r < rows; r++)                                           
           for(int c = 0; c < cols; c++)                                       
           {//it does not matter if value is a dummy or valid data             
               iRaster.getElement(c, r, value, b);                             
               grid.setElement(   c, r, value, b);                             
           }                                                                   
   return true;                                                                
}//bool gridread(TeRaster& iRaster, TeRaster& grid)                            

bool gridwrite(char* oLayerName, TeRaster& grid, TeDatabase* db_ptr,           
               TeLayer* oLayer_ptr)                                            
{                                                                              
   oLayer_ptr = TeImportRaster(oLayerName, &grid, db_ptr);                     
   if(oLayer_ptr == 0)                                                         
   {                                                                           
       return false;                                                           
   }//if output layer was not successfully produced                            
   return true;                                                                
}//bool gridwrite(TeLayer& iLayer, TeRaster& grid, oLayer_ptr)                 

bool gridwrite(TeRaster& oRaster, TeRaster& grid)                              
{                                                                              
   TeRasterParams oParams = grid.params();                                     
   //  Set the specific parameters whose value we need to be sure. For the     
   //others, we only need make than eqaul their input grid counter-parts       
   oParams.mode_ = 'c';//read and write                                        
   //record raster in memory for fast proccess                                 
   oParams.decoderIdentifier_ = "SMARTMEM";                                    
   oRaster.updateParams(oParams);//values to new Raster                        
   TeDecoder* dec = new TeDecoderSmartMem( oParams );                          
   oRaster.setDecoder(dec);  //pass values to new Raster                       
                                                                               
   if(! oRaster.init())// make new Raster ready to use                         
   {                                                                           
       return false;                                                           
   }                                                                           
   //copy raster contents indiscriminately                                     
   int nBands = grid.params().nBands();//number of bands                       
   int nRows  = grid.params().nlines_;//number of lines                        
   int nCols  = grid.params().ncols_;//number of colunms                       
   double value = 0.0;//value to be copied                                     
   for(int b = 0; b < nBands; b++)//for all bands                              
   {                                                                           
       for(int r = 0; r < nRows; r++)//for all lines                           
       {                                                                       
           for(int c = 0; c < nCols; c++)//for all colunms                     
           {                                                                   
               grid.getElement(   c, r, value, b);//copy value                 
               oRaster.setElement(c, r, value, b);//set value                  
           }//for all cols                                                     
       }//for all line                                                         
    }//for all bands                                                           
   return true;                                                                
}//bool gridread(TeRaster& iRaster, TeRaster& grid)                            

void darea(int i, int j, WorldRecord& world, TeRaster& sdir, TeRaster& larr, 
           TeRaster& fareaw, TeRaster& fweight)
{ 
   int in,jn,k,con=0;
   //int ccheck;     //local contamination check flag  
   // con is a flag that signifies possible contaminatin of area
   //due to edge effects  
   if(i!= world.minX && i!= world.numX-1 
   && j!= world.minY && j!= world.numY-1 
   && d(sdir, i, j) > -1)
   {                 //not on boundary  
       if( d(larr , i, j)==0)  // not touched yet
       {
           d(larr , i, j, 1);
           if(world.useww == 1) 
               d(fareaw , i, j, d(fweight , i, j));
           for(k=1; k<=8; k++)
           {                 
               in=i+vector_I[k];
               jn=j+vector_J[k];
               //test if neighbor drains towards cell excluding boundaryies 
               if(d(sdir, in, jn)>0 && 
                ((d(sdir, in, jn)-k)==4 ||(d(sdir, in, jn)-k)==-4))
               {
                   darea(in,jn, world, sdir, larr, fareaw, fweight);
                   if(   d(larr , in, jn) < 0) con = -1;
                   else  d(larr , i , j , (d(larr , i, j)+ d(larr , in, jn)) );
                   if(world.useww == 1)
                   {
                       if( d(fareaw, in, jn) <= world.ndv  
                        || d(fareaw, i , j ) <= world.ndv) 
                       {                                   
                           d(fareaw, i, j, world.ndv);     
                       }                                   
                       else 
                       {//fareaw(in, jn)> ndv || fareaw(i , j ) > ndv)
                           d(fareaw, i, j, d(fareaw, i, j)+ d(fareaw, in, jn));
                           //fixed(j,i)to(i,j)
                       }//fareaw(in, jn)> ndv || fareaw(i , j ) > ndv)
                   }//if(world.useww == 1)
               }//if sdir(in,jn) > 0 && (sdir(in,jn) == (+-)4 + k)
               if( d(sdir , in, jn) < 0) con = -1;
           }//for k in (1 to 9)
           if(con == -1 && world.ccheck == 1)
           {
               d(larr, i, j, -1);
               if(world.useww == 1) d(fareaw, i, j, world.ndv);
           }//if(con == -1 && world.ccheck == 1)
       }//if( d(larr , i, j)==0)  // not touched yet
   }//if(i = not edge && j = not edge && d(sdir, i, j) > -1)
   else d(larr, i, j, -1);
}

void set1(int i,int j,double *fact,double mval,bool useflowfile,               
          TeRaster& sdir, TeRaster& larr, TeRaster& felevg)                    
{                                                                              
   double slope,smax;
   int k,amax,in,jn,aneigh=-1;
   int dirnb;
   d(sdir, i, j, 0); //This necessary for repeat passes after level raised */
   smax = 0.;
   amax = 0;
   for(k=1; k<=8; k=k+2)   // examine adjacent cells first
   {
       in=i+vector_I[k];
       jn=j+vector_J[k];
       //if an adjacent cell= no data, dirction = no data
       if(d(felevg, in, jn) <= mval)  d(sdir, i, j, MISSINGINT);
       if(d(sdir, i, j) != MISSINGINT)//if direction is still valid
       {
           slope=fact[k]*(d(felevg, i, j) -  d(felevg , in, jn)); 
           if(useflowfile)aneigh= (int) d(larr, in, jn); 
           if(aneigh > amax && slope >= 0.)  
           {
               amax = aneigh;
               dirnb = ( int) d(sdir, in, jn);
               if(dirnb > 0 && abs(dirnb-k) != 4)
               {
                   d(sdir, i, j, k);// Dont set opposing pointers
               }
           }//if(aneigh > amax && slope >= 0.)  
           else if(slope > smax) // && amax <=0)
           {
               smax=slope;
               dirnb= ( int) d(sdir , in, jn);
               if(dirnb > 0 && abs(dirnb-k) ==4)//Dont set an opposing pointer.
               //  Here we have a definitive positive slope but a neighbor that 
               //drains uphill This occurs with imposed flow directions Resolve 
               //by setting the direction of the burned in direction to no data 
               {
                   d(sdir, in, jn, MISSINGINT); //-32767
               }
               d(sdir , i, j, k); 
           }//else if(slope > smax) // && amax <=0)
        }//if(d(sdir, i, j) != MISSINGINT)
    }//for(k=1; k<=8; k=k+2)   // examine adjacent cells first
   for(k=2; k<=8; k=k+2)   // examine diagonal cells 
    {   
       in=i+vector_I[k];
       jn=j+vector_J[k];
       if(d(felevg , in, jn) <= mval) d(sdir , i, j, MISSINGINT);//fixed(i,i)to(i,j)
       if( d(sdir , i, j) != MISSINGINT)//fixed(j,i)to(i,j)
       {
           slope=fact[k]* ( d(felevg , i, j)- d(felevg , in, jn));
           //fixed(jn,in)to(in,jn)//fixed(j,i)to(i,j)//after long search
           if(slope > smax && dontcross(k,i,j, sdir) ==0)//Dont cross pointers
           {
               smax=slope;
               dirnb = ( int) d(sdir, in, jn);
               if(dirnb > 0 && abs(dirnb-k) ==4)//Dont set an opposing pointer. 
               //  Here we have a definitive positive slope but a neighbor that 
               //drains uphill This occurs with imposed flow directions Resolve 
               //by setting the direction of the burned in direction to no data 
               {
                   d(sdir , in, jn, MISSINGINT);    //-32767 
               }
               d(sdir , i, j, k); 
           }//if(slope > smax && dontcross(k,i,j,sdir)==0)//Dont cross pointers
       }//if( d(sdir , i, j) != MISSINGINT) //-32767
   }//for(k=2; k<=8; k=k+2)   // examine diagonal cells 
}

                                                                
int dontcross(int k, int i, int j, TeRaster& sdir)                             
{                                                                              
   int n1, n2, c1, c2, ans=0;                                                  
/*                                                                             
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
   switch(k)//select the diagonal neighbor we were told to access by index k   
   {                                                                           
       case 2: //upper right                                                   
           n1=1; c1=4;                                                         
           n2=3; c2=8;                                                         
           if(d(sdir, i + vector_I[n1], j + vector_J[n1])== c1                 
           || d(sdir, i + vector_I[n2], j + vector_J[n2])== c2)                
               ans=1;                                                          
       break;                                                                  
       case 4: //upper left                                                    
           n1=3; c1=6;                                                         
           n2=5; c2=2;                                                         
           if(d(sdir, i + vector_I[n1], j + vector_J[n1])== c1                 
           || d(sdir, i + vector_I[n2], j + vector_J[n2])== c2)                
               ans=1;                                                          
       break;                                                                  
       case 6: //lower left                                                    
           n1=7; c1=4;                                                         
           n2=5; c2=8;                                                         
           if(d(sdir, i + vector_I[n1], j + vector_J[n1])== c1                 
           || d(sdir, i + vector_I[n2], j + vector_J[n2])== c2)                
               ans=1;                                                          
       break;                                                                  
       case 8: //lower right                                                   
           n1=1; c1=6;                                                         
           n2=7; c2=2;                                                         
           if(d(sdir, i + vector_I[n1], j + vector_J[n1])== c1                 
           || d(sdir, i + vector_I[n2], j + vector_J[n2])== c2)                
               ans=1;                                                          
       break;                                                                  
   }                                                                           
   return(ans);                                                                
}                                                                              

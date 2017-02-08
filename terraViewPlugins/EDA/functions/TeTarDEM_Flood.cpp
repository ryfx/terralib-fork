
#include "TeTarDEM_Flood.h"

int flood(TeRaster& demfile, TeRaster& newfile, TeRaster& flowfile, 
          TeRaster& newflowfile, bool useflowfile)
{   
   WorldRecord world;
   TeRaster fweight;//aread8 & area //double
   TeRaster larr;   //aread8 & area  //int
   TeRaster fareaw; //aread8 & area //double
   TeRaster sdir;   //aread8 & area // int
   TeRaster felevg; //source //double
   TeRaster sapoolg;//sdir function// int
   
   world.err = TD_NO_ERROR;//start assuming no errors
   double fdmval = 0.0;//avoid complains of lack of intialization
   char* ctempFile = NULL;
    
   if(!gridread(demfile,felevg))//get elevation raster
   {   
       world.err=TD_FAILED_GRID_OPEN;
       return(world.err);
   }
   //recording params from elevation raster
   world.minX= 0; 
   world.minY= 0;
   world.numX = felevg.params().ncols_;
   world.numY = felevg.params().nlines_;
   world.dx = felevg.params().resx_;
   world.dy = felevg.params().resy_;
   world.csize = world.dx;
   world.mval = felevg.params().dummy_[0];
   world.bndbox = felevg.params().boundingBox();
   if(useflowfile)
   {
       if(!gridread(flowfile, sapoolg))
       {
           world.err = TD_FAILED_GRID_OPEN;
           return(world.err);
       }//if initializing sapoolg from flowfile failed
       fdmval =sapoolg.params().dummy_[0];
       if(world.numX != sapoolg.params().ncols_  
       || world.numY != sapoolg.params().nlines_)
       {    
           world.err = TD_GRID_SIZE_MISMATCH;
           return world.err;
       }//if dimensions of sapoolg do not mach those of felevg
       initRasterFromWorld(larr, world, TeINTEGER, 10000000, 0, -1);
       if(!allocategrid(larr, larr.params().dummy_[0]))
       {
           world.err = TD_FAILED_GRID_OPEN; //;lArrGrid.LastErrorCode();
           return world.err;
       }//if allocating larr failed
   }//if using flow file
   else
   {
       initRasterFromWorld(sapoolg, world, TeINTEGER, MISSINGINT, 0, -1);
       if(!allocategrid(sapoolg,sapoolg.params().dummy_[0]))
       {
           world.err =TD_FAILED_GRID_OPEN; // lArrGrid.LastErrorCode();
           return world.err;
       }//
   }//else OR if not using flow file
   /* allocate dir and stack arrays  */
   initRasterFromWorld(sdir, world, TeINTEGER, 9, 0, MISSINGINT);
   if(!allocategrid(sdir, sdir.params().dummy_[0]))
   {
       world.err = TD_FAILED_GRID_OPEN;
       return world.err;
   }//if sdir initialization failed
   world.istack = (int) (world.numX * world.numY * 0.1);
   world.pstack = world.istack;
   world.dn = (int*)malloc(sizeof(int) * world.istack);
   world.is = (int*)malloc(sizeof(int) * world.istack);
   world.js = (int*)malloc(sizeof(int) * world.istack);
   world.ipool = (int*)malloc(sizeof(int) * world.pstack);
   world.jpool = (int*)malloc(sizeof(int) * world.pstack);
   if(world.dn == NULL 
   || world.is == NULL || world.js == NULL 
   || world.ipool == NULL || world.jpool == NULL)
   {
       world.err = TD_FAILED_MEMORY_ALLOC;
       return(world.err);
   }
    //x (or i) = [i1 to n1)
    //y (or j) = [i2 to n2)
    //world.minX = 0; 
    //world.minY = 0; 
    //world.numX = world.numX; 
    //world.numY = world.numY;  /*  full grid  */ 
    //  printf("Grid size: %d x %d\n",world.numX,world.numY);
    bool writeflowfile=1;
    
    world.err = setdf(fdmval, useflowfile, newflowfile, writeflowfile, world, 
                      larr, felevg, sapoolg, sdir, fareaw, fweight);
   if(world.err != TD_NO_ERROR) return(world.err);
   //free stacks
   free(world.dn);
   free(world.is);
   free(world.js);
   free(world.ipool);
   free(world.jpool);
   //write Raster
   if(!gridwrite(newfile, felevg))
   {
       world.err=TD_FAILED_GRID_SAVE;
       return(world.err);
   }
    if(world.err != TD_NO_ERROR) return(world.err);
    return(0);  /*  ALL OK return from flood  */
}//flood()

//*****************************************************************************
//convinience method used for raster initialization
//WARNIG! This method assumes that world has all suitable parameters
bool initRasterFromWorld(TeRaster& newRaster, WorldRecord& world, 
                         TeDataType dataType,double max,double min,double dummy)
{
   TeRasterParams params = newRaster.params();
   paramInit(params, true, 1, world.numY, world.numX, dataType, world.dx, world.dy, 
             max, min, dummy, world.bndbox.x1_, world.bndbox.y1_, 
             world.bndbox.x2_, world.bndbox.y2_, 'c', "SMARTMEM");
   TeDecoder* dec = new TeDecoderSmartMem(params);
   newRaster.setDecoder(dec);
   //if(!newRaster.init())// make new Raster ready to use
   //    return false;
   //return true;
   return(newRaster.init());// make new Raster ready to use and return result
}

//*****************************************************************************
// Function to climb recursively flow direction grid for min elevation to burn 
//down
double climb(int i,int j, TeRaster& felevg, TeRaster& sdir)
{
   double emin,eneigh;
   int in,jn,k;
   emin= d(felevg, i, j);
   for(k=1; k<=8; k++)
   {
       in=i+vector_I[k];
       jn=j+vector_J[k];
       /* test if neighbor drains towards cell excluding boundaries */
       if(d(sdir , in, jn)>0 && 
         (d(sdir , in, jn)-k==4 ||  d(sdir , in, jn)-k ==-4))
       {
           eneigh = climb(in,jn, felevg, sdir);
           if(eneigh < emin)
               emin=eneigh;
       }
   }//for(k=1; k<=8; k++)
   d(felevg, i, j, emin);  // burn down elevation
   return(emin);
}

//*****************************************************************************
// Function to burn down flow elevations for given flow directions. This is a 
//placeholder for what will eventually be simplex optimization
int fdsimplex(WorldRecord& world, TeRaster& felevg, TeRaster& sdir)
{
   int in,jn,i,j;
   world.err = TD_NO_ERROR;
   double emin;
   for(j = world.minY + 1; j < world.numY - 1; j++)
        for(i = world.minX + 1; i < world.numX - 1; i++)
        {
            if( d(sdir, i, j) > 0)
            {
               in=i+vector_I[ (int) d(sdir, i, j)];
               jn=j+vector_J[ (int) d(sdir, i, j)];
               if( d(sdir , in, jn) <= 0) // Neighbour is out
                   emin = climb(i,j, felevg, sdir);
           }//if sdir(i,j) > 0
       }//for all cols except edges
   return(world.err);
}

//*****************************************************************************
// Function to clean given flow directions.
int dirclean(TeRaster& newflowfile, bool writeflowfile, WorldRecord& world, 
             TeRaster& felevg, TeRaster& sdir, TeRaster& larr, 
             TeRaster& fareaw, TeRaster& sapoolg, TeRaster& fweight)
{
/* Find all locations that have an imposed flow direction that drain to a location 
   that does not have imposed flow directions.
   Evaluate contrib area for these locations not doing contamination checking.
   At the end we have contrib area evaluated for the skeleton of cells that drain 
   off the imposed raster.  Any other cells in the imposed raster are "cleaned"
   from it because they do not drain off the raster, i.e. have inconsistent directions
   */
    
   int in,jn,i,j;
   world.err = TD_NO_ERROR;
   world.ccheck=0;  // These are globals to remove contamination checking
   world.useww=0;  //  for the darea function
   for(j = world.minY + 1; j < world.numY - 1; j++)
       for(i = world.minX + 1; i < world.numX - 1; i++)
       {
           if( d(sdir, i, j) > 0)
           {
               in=i+vector_I[ (int) d(sdir, i, j)];
               jn=j+vector_J[ (int) d(sdir, i, j)];
               if( d(sdir, in, jn) <= 0)  // Neighbour is out
                   darea(i,j, world, sdir, larr, fareaw, fweight);
           }
       }
   //  At this point contrib area has been evaluated for all grid cells that drain to 
   //    an edge or internal end.  Set direction to no data for the rest.
   for(j = world.minY + 1; j < world.numY - 1; j++)
       for(i = world.minX + 1; i < world.numX - 1; i++)
       {
           if( d(larr , i, j) <= 0)
           {   // reinitialize   10/5/02.  DGT modified below to include 
               //elevation check
               if( d(felevg , i, j) <= world.mval)  
                   d(sdir , i, j, MISSINGINT); //-32767
               else  
                   d(sdir , i, j, 0);   
           }
       }  
   if(writeflowfile == 1)
   {// Use pool array as placeholder to write directions
       for(j = world.minY; j < world.numY; j++)
           for(i = world.minX; i < world.numX; i++)
           {
               if(d(sdir, i, j) >0)  d(sapoolg, i, j, d(sdir, i, j));
               else  d(sapoolg , i, j, MISSINGINT); //-32767
           } 
           
           sapoolg.params().dummy_[0]=MISSINGINT; 
           
           //sApoolGrid.setHeader(sApoolHeader);
           if (gridwrite(newflowfile, sapoolg)==0 )
               world.err=TD_NO_ERROR;
           else
           {
               world.err=TD_FAILED_GRID_SAVE;
               return world.err;
           //    if (newflowfile)
           //        AfxMessageBox( LPCTSTR(strcat( "Failed to save file: ", 
           //                       newflowfile)) );
           }
   }
   return(world.err);
}

//*****************************************************************************
int addstack(int i, int j, WorldRecord& world)
{
   /*  Routine to add entry to world.is, world.js stack, enlarging if necessary   */
   world.nis++;
   if(world.nis >= world.istack )//needed number in stack > stack size
   {/*  Try enlarging   */
       world.istack += (int) (world.numX * world.numY * 0.1);//add area/10
       if(world.istack > world.numX * world.numY)
       {//if stack can no longer be enlarged
           world.err = TD_STACK_LARGER_THAN_GRID;
           return(world.err);//is, js, stack too large
       }
       //Enlarging world.is,world.js stack
       world.is = ( int *)realloc(world.is, sizeof( int) * world.istack);
       world.js = ( int *)realloc(world.js, sizeof( int) * world.istack);
       world.dn = ( int *)realloc(world.dn, sizeof( int) * world.istack);
       if(world.is == NULL || world.js == NULL || world.dn == NULL)
       {//if any realocation failed
           world.err = TD_FAILED_TO_ENLARGE_STACK; 
           return(world.err);
       }
   }//if(world.nis >= world.istack )//needed number in stack > stack size
   world.is[world.nis]=i;
   world.js[world.nis]=j;
   world.err = TD_NO_ERROR;   
   return(world.err);
}
//*****************************************************************************
int vdn(int n, WorldRecord& world, TeRaster& felevg, TeRaster& sdir)
{    
   int ip,k,imin;
   double ed;//Elevation Difference
   world.nis = n;// set world.nis from parameter n
   do//while(world.nis < n);
   {
       n = world.nis;//set n = number of unset cells in dn array;
       world.nis = 0;//reset world.nis. n still has its original value
       for(ip=1; ip <=n; ip++) world.dn[ip]=0;//init dn array to all == 0
        
       //for all vertical and horizontal neighbors to all cells in the stack
       for(k=1; k<=7; k=k+2) for(ip=1; ip<=n; ip++)
       {
           ed = d(felevg, world.is[ip],             world.js[ip]) 
              - d(felevg, world.is[ip]+vector_I[k], world.js[ip]+vector_J[k]);
           if(   ed >= 0.  /*if cell in stack is higher than or == to neighbor*/
               && d(sdir, world.is[ip]+vector_I[k], world.js[ip]+vector_J[k])!=0 
               && world.dn[ip] == 0 /*and dn[ip] has not been set*/
             )
               world.dn[ip]=k;//record ´direction of neighbor in dn
       }//for ( k in [1 to 7]inc 2 ) X ( ip in [1 to n] )
        
       //for all diagonal neighbors to all cells in the stack
       for(k=2; k<=8; k=k+2) for(ip=1; ip<=n; ip++)
       {
           ed = d(felevg, world.is[ip],             world.js[ip]) 
              - d(felevg, world.is[ip]+vector_I[k], world.js[ip]+vector_J[k]);
           if(  ed >= 0.  /*if cell in stack is higher than or == to neighbor*/
              && d(sdir,  world.is[ip]+vector_I[k], world.js[ip]+vector_J[k])!=0 
              && world.dn[ip] == 0 /*and dn[ip] has not been set*/
              && dontcross(k, world.is[ip], world.js[ip], sdir)==0
             )
               world.dn[ip]=k;//record ´direction of neighbor
       }//for ( k in [2 to 8]inc2 ) X ( ip in [1 to n] )
        
       imin=1;  /*  location of point on stack with lowest elevation  */
       for(ip=1; ip <= n; ip++)//for all dn indices from 1 to n
       {
           if(world.dn[ip] > 0)/*if dn[ip] has been set*/
               d(sdir, world.is[ip], world.js[ip], world.dn[ip]);//record dir
           else/*if dn[ip] has not been set*/
           {
               world.nis++;//nis = number of unset cells in dn array;
               world.is[world.nis]=world.is[ip];
               world.js[world.nis]=world.js[ip];
               if(  d(felevg, world.is[world.nis], world.js[world.nis]) 
                  < d(felevg, world.is[imin],      world.js[imin]) )
                   imin = world.nis;
           }//else if dn[ip] has not been set
       }//for ip in [1 to n] OR for all dn indices from 1 to n
   }while(world.nis < n);//end when not all points are unset
   return(imin);//return index of lowest point in the "stack"
}//int vdn(int n, WorldRecord& world, TeRaster& felevg, TeRaster& sdir)

//*****************************************************************************
// Function to compute pool recursively and at the same time determine the 
//minimum elevation of the edge.
//void oldBody()
//{
//   
//   int in,jn,k;
//   world.recursedepth = world.recursedepth + 1;
//    
//   if(d(sapoolg, i, j) <= 0.0)//not already part of a pool
//   {    
//       //if((int)d(sdir , i, j)!= MISSINGINT)//check only dir since dir was initialized
//       if((int)d(sdir, i, j)!= -1)//check only dir since dir was initialized
//       {/* not on boundary  */
//           //only after the tests above the real procceing begins here
//           d(sapoolg, i, j, world.pooln);//apool assigned pool number
//           world.npool = world.npool+1;
//           if(world.npool >= world.pstack)
//           {    
//               if(world.pstack < (world.numX * world.numY))
//               {
//                   /*  Try enlarging pool stack*/
//                   //printf("\n Enlarging pool stack\n");
//                   world.pstack = (int) (world.pstack + world.numX * world.numY * 0.1);
//                   if(world.pstack > world.numX * world.numY)
//                   {
//                       //printf("\n Pool stack too large, exiting ...\n");
//                       world.err = TD_STACK_LARGER_THAN_GRID;
//                       return;
//                   }
//                   world.ipool=(int*)realloc(world.ipool,sizeof(int)*world.pstack);
//                   world.jpool=(int*)realloc(world.jpool,sizeof(int)*world.pstack);
//                   if(world.ipool == NULL || world.jpool == NULL)
//                   {
//                       //printf("\n Cannot reallocate pool stack, exiting ...\n");
//                       world.err = TD_FAILED_MEMORY_ALLOC;
//                       return;
//                   }
//               }//if(world.pstack < (world.numX * world.numY))
//               else//if(world.pstack >= (world.numX * world.numY))
//               {
//                   //printf("\n Could not enlarge Pool stack\n");
//                   world.err = TD_FAILED_TO_ENLARGE_STACK;
//                   return;
//               }
//           }//if(world.npool >= world.pstack)
//           world.ipool[world.npool]=i;
//           world.jpool[world.npool]=j;
//           /*printf("%d %d Pool %d\n",i,j,pooln);  */
//           for(k=1; k<=8; k++)
//           {  
//               in=i+vector_J[k];
//               jn=j+vector_I[k];
//               //test if neighbor drains towards cell excluding boundaries 
//               if(( (int)d(sdir, in, jn) > 0 
//                 && ( (int)d(sdir, in, jn)-k == 4 
//                   || (int)d(sdir, in, jn)-k == -4))
//               ||((int)d(sdir, in, jn) == 0 
//                 && (int)d(felevg, in, jn) >= (int)d(felevg , i, j) 
//                 && dontcross(k,i,j, sdir) == 0))
//               {/* so that adjacent flats get included */
//                   pool(in,jn, world, sapoolg, sdir, felevg);
//               }//if(see above)
//           }//for(k=1; k<=8; k++)
//
//       }//if( d(sdir, i, j)!= MISSINGINT)//check only dir since dir was initialized
//   }//if( d(sapoolg, i, j) <=0)//not already part of a pool
//}//pool()

void pool(int i,int j, WorldRecord& world, TeRaster& sapoolg, TeRaster& sdir, 
          TeRaster& felevg)
{
   
   int in,jn,k;
   world.recursedepth = world.recursedepth + 1;
    
   if(d(sapoolg, i, j) > 0.0)/*already part of a pool*/ return;
   //if((int)d(sdir, i, j)== MISSINGINT)//check only dir since dir was initialized
   if((int)d(sdir, i, j)== -1)/*check only dir since dir was initialized*/
       return;
   /* not on boundary  */
   //only after the tests above the real procceing begins here
   d(sapoolg, i, j, world.pooln);//apool assigned pool number
   world.npool = world.npool+1;
   if(world.npool >= world.pstack)
   {    
       if(world.pstack < (world.numX * world.numY))
       {
           /*  Try enlarging pool stack*/
           world.pstack = (int) (world.pstack + world.numX * world.numY * 0.1);
           if(world.pstack > world.numX * world.numY)
           {
               //Pool stack too large, exiting ...
               world.err = TD_STACK_LARGER_THAN_GRID;
               return;
           }
           world.ipool = (int*)realloc(world.ipool, sizeof(int) * world.pstack);
           world.jpool = (int*)realloc(world.jpool, sizeof(int) * world.pstack);
           if(world.ipool == NULL || world.jpool == NULL)
           {
               //Cannot reallocate pool stack, exiting ...
               world.err = TD_FAILED_MEMORY_ALLOC;
               return;
           }
       }//if(world.pstack < (world.numX * world.numY))
       else//if(world.pstack >= (world.numX * world.numY))
       {
           //Could not enlarge Pool stack
           world.err = TD_FAILED_TO_ENLARGE_STACK;
           return;
       }
   }//if(world.npool >= world.pstack)
   world.ipool[world.npool]=i;
   world.jpool[world.npool]=j;
   /*printf("%d %d Pool %d\n",i,j,pooln);  */
   for(k=1; k<=8; k++)
   {  
       in=i+vector_J[k];
       jn=j+vector_I[k];
       //test if neighbor drains towards cell excluding boundaries 
       if(( (int)d(sdir, in, jn) > 0 
           && ( (int)d(sdir, in, jn)-k == 4 || (int)d(sdir, in, jn)-k == -4))
       ||((int)d(sdir, in, jn) == 0 
           && (int)d(felevg, in, jn) >= (int)d(felevg , i, j) 
           && dontcross(k,i,j, sdir) == 0))
       {/* so that adjacent flats get included */
           pool(in,jn, world, sapoolg, sdir, felevg);
       }//if(see above)
   }//for(k=1; k<=8; k++)
}//pool()

//*****************************************************************************
double max2(double e1,double e2){if(e2 > e1)return(e2); /*dfault*/return(e1);} 
//*****************************************************************************
bool notfdr(bool useflowfile,int i,int j,TeRaster& larr)
{
   if( (!useflowfile) || (d(larr, i, j) <= 0) ) return(true);
   return(false);
}
//{
//   if(!useflowfile)
//       return(true);
//   else 
//       if(d(larr, i, j) <= 0)
//           return(true);
//   return(false);
//}
//*****************************************************************************
int setdf(double fdmval, bool useflowfile, TeRaster& newflowfile, 
          bool writeflowfile, WorldRecord& world, TeRaster& larr, 
          TeRaster& felevg, TeRaster& sapoolg,TeRaster& sdir, TeRaster& fareaw, 
          TeRaster& fweight)
{
   int i, j, k, n, nflat, ni, ip, imin, in, jn, np1, nt;
   world.err = TD_NO_ERROR;
   double fact[9],per=1.;
   //x (or i) = [i1 to n1)
   //y (or j) = [i2 to n2)
   /*initialize internal pointers*/
   for(j = world.minY+1; j < world.numY-1; j++)
   {
       for(i = world.minX+1; i < world.numX-1; i++)
       { 
           if(d(felevg, i, j) <= world.mval)//Dummy must be < minimum value   
               d(sdir, i, j, MISSINGINT); //-32767
           else d(sdir, i, j, 0);
       }
   }
    //Set stream overlay directions
   if(useflowfile)
   {
       for(j=world.minY+1; j< world.numY-1; j++)
       {
           for(i=world.minX+1; i<world.numX-1; i++)
           {            
               //DGT 7/22/02  
               //A problem occurs if direction is set adjacent to no data 
               //elevation. Guard against this
               if( d(sapoolg , i, j) > fdmval &&  d(sdir , i, j) >= 0 )
               {
                   int isAdjNodata = 0;
                   for(k=1; k<=8; k++)   // examine adjacent cells first
                   {
                       in=i+vector_I[k];
                       jn=j+vector_J[k];
                       if( d(felevg, in, jn) <= world.mval)isAdjNodata = 1;
                   }//for all directions
                   if(isAdjNodata==0)
                   {
                       d(sdir, i, j, d(sapoolg ,i, j));
                       d(larr, i, j, 0);
                   }//if
               }//if 0 <= sapoolg[j,i] < fdmval 
               //End DGT modifications. 
           }//for j in (i2 to n2)
       }//for i in (i1 to n1)
       //arr=flowdirection;  // to reuse space for area array for cleaning 
       //and in set
       world.err=dirclean(newflowfile, writeflowfile, world, felevg, sdir, 
                          larr, fareaw, sapoolg, fweight);
       //Call function to optimize elevation adjustments based on flow 
       //directions already given
       world.err=fdsimplex(world,felevg, sdir);            
    }//if flow file
    
    /*  Direction factors  */
    for(k=1; k<= 8; k++)
        fact[k]=(double)(1./
           sqrt( vector_J[k]*vector_J[k] * world.dy*world.dy
               + vector_I[k]*vector_I[k] * world.dx*world.dx));
    /*Set positive slope directions - store unresolved on stack */
   world.nis = 0;
   for(j = world.minY+1; j < world.numY-1; j++) 
   {
       for(i = world.minX+1; i < world.numX-1; i++)
       {
           if(d(felevg, i, j) > world.mval &&  d(sdir, i, j) == 0)
               set1(i, j, fact, world.mval, useflowfile, sdir, larr, felevg);
           /*Put unresolved pixels on stack*/
           if( d(sdir, i, j) == 0)
           {
               world.err = addstack(i,j,world);
           }
       }//for j in (i2 to n2)
   }//for i in (i1 to n1)
   nflat = world.nis;
   /* routine to drain flats to neighbors  */
     
   imin = vdn(nflat, world, felevg, sdir);
   n = world.nis;
    //   printf("Number of pits to resolve: %d\n",n); 
   np1=n;
   nt=np1*(1 - (int) per / 100);
    
   /*  initialize apool to zero  */
   for(j = world.minY; i < world.numY; j++)
       for(i = world.minX; i < world.numX; i++)
           d(sapoolg, i, j, 0);

    /*  store unresolved stack location in apool for easy deletion  */
    // debuggind

//    fdbg=fopen("c:\\dave\\scratch\\temp.txt","w");
   while(world.nis > 0)   /*  While AA */
   {
       i = world.is[imin];
       j = world.js[imin];
       world.pooln = 1;
       world.npool = 0;
       world.nf = 0;  /*  reset flag to that new min elev is found */
       world.recursedepth=0;
       //Recursive call on unresolved point with lowest elevation 
       pool(i,j, world, sapoolg, sdir, felevg);
       /*  Find the pour point of the pool  */
       for(ip = 1; ip <= world.npool; ip++)
       {
           i=world.ipool[ip];
           j=world.jpool[ip];
           for(k=1; k <=8; k++)    
           {
               jn=j+vector_J[k];
               in=i+vector_I[k];  
               /*  neighbor not in pool  */
               if( d(sapoolg, in, jn) != world.pooln && dontcross(k, i, j, sdir)==0)
               {
                   world.et= max2( d(felevg, i, j), d(felevg, in, jn));
                   if(world.nf == 0)  /*  this is the first edge found  */
                   {
                       world.emin = world.et;
                       world.nf = 1;
                   }
                   else
                   {
                       if(world.emin > world.et)
                       {
                           world.emin = world.et;
                       }//if
                   }//else
               }//if neighbor not in pool  
           }//for k = [1, 8]
       } //for ip in [1 to world.npool]//Find the pour point of the pool 
       /*  Fill the pool  */
       for(k = 1; k <= world.npool; k++)
       {
           i= world.ipool[k];
           j= world.jpool[k];
           if( d(felevg , i, j) <= world.emin)//fixed(j,i)to(i,j)
           {   /*Can be in pool, but not flat*/
               if( d(sdir, i, j) > 0 && notfdr(useflowfile, i, j, larr))
               {   // notfdr is used to avoid changing burned in directions
                   d(sdir, i, j, 0);
                   world.err=addstack(i,j,world);/*  Add to stack  */
               }
               for(ip=1; ip <=8; ip++)
               {
                   jn=j+vector_J[ip];
                   in=i+vector_I[ip];
                   if( d(felevg, in, jn) >  d(felevg, i, j) 
                   &&  d(sdir,   in, jn) > 0
                   && notfdr(useflowfile,in,jn, larr))  
                   // 7/29/04  DGT Fixed from notfdr(useflowfile,i,j)
                   /*Only zero direction of neighbors that are higher -because
                   lower  or equal may be a pour point in a pit that must not be
                   disrupted  */
                   {
                       d(sdir, in, jn, 0);
                       world.err=addstack(in,jn,world);
                   }
               }// ip in [i to 8]
               d(felevg, i, j, world.emin);                
           }// if fevelg(i,j) <= emin
           d(sapoolg, i, j, 0);/*Reinitialize for next time round*/
       }//for k in [1 to number of pools]
       /*reset unresolved stack*/
       ni=0;
       for(ip=1; ip <= world.nis; ip++)
       {
           set1(world.is[ip], world.js[ip], fact, world.mval, useflowfile, 
                sdir, larr, felevg);  
           if(d(sdir, world.is[ip], world.js[ip]) == 0)/*still on stack*/
           {
               ni++;
               world.is[ni] = world.is[ip];
               world.js[ni] = world.js[ip];
           }//ig sdir(is[ip], js[ip]) == 0, OR noninitialized
       }// for ip in [1 to nis]
       n = world.nis;////////////////////////////
       imin = vdn(ni, world,felevg, sdir);
       if(world.nis < nt)
       {
           //printf("Percentage done %f\n",per);
           //         ocxCtrl->FloodEvent(( int)per);
           //feedback("Percentage done %f\n",per,FN_FLOOD);
           per = per + 1;
           nt = np1 * (1 - (int) per / 100);
       }
   }/*end of while AA*/
   return(world.err);
}

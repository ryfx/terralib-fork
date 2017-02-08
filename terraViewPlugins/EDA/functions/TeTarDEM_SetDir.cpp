
#include "TeTarDEM_SetDir.h"

int setdir(TeRaster& demfile, TeRaster& angfile, TeRaster& slopefile, 
           TeRaster& flowfile, bool useflowfile) 
{                    
   TeRaster fweight;//aread8 & area //double
   TeRaster larr;   //aread8 & area  //int
   TeRaster fareaw; //aread8 & area //double
   TeRaster sdir;   //aread8 & area // int
   TeRaster fangle; //area //double
   TeRaster felevg; //source //double
   TeRaster fslopeg;//source //double
   TeRaster sapoolg;//sdir function// int
   
   return(setdir(demfile, angfile, slopefile, flowfile, useflowfile, fweight,
                 larr, fareaw, sdir, fangle, felevg, fslopeg, sapoolg));
}//release setdir()

int setdir(TeRaster& demfile,  TeRaster& angfile, TeRaster& slopefile, 
           TeRaster& flowfile, bool useflowfile,  TeRaster& fweight,
           TeRaster& larr,     TeRaster& fareaw,  TeRaster& sdir, 
           TeRaster& fangle,   TeRaster& felevg,  TeRaster& fslopeg, 
           TeRaster& sapoolg)
{
/*
   Overloaded function that takes the following extra TeRaster references:
   -TeRaster fweight;//aread8 & area //double
   -TeRaster larr;   //aread8 & area  //int
   -TeRaster fareaw; //aread8 & area //double
   -TeRaster sdir;   //aread8 & area // int
   -TeRaster fangle; //area //double
   -TeRaster felevg; //source //double
   -TeRaster fslopeg;//source //double
   -TeRaster sapoolg;//sdir function//
*/
//   char * tempFileName = NULL;
   WorldRecord world;
   world.err = TD_NO_ERROR;
   world.fdmval = 0;
   world.useflowfile = useflowfile;
                                                                               
   if(!gridread(demfile, felevg))
   {
       world.err=TD_FAILED_GRID_OPEN;
       return(world.err);
   }
   TeRasterParams elevPars = felevg.params();
   //record full grid dimensions
   world.minX= 0; 
   world.minY= 0;
   world.numX = elevPars.ncols_;
   world.numY = elevPars.nlines_;
   world.dx = elevPars.resx_; 
   world.dy = elevPars.resy_; 
   world.csize = world.dx;
   world.mval = elevPars.dummy_[0]; 
   world.bndbox = elevPars.boundingBox();
   
   if(world.useflowfile)
   {
       if(!gridread(flowfile, sapoolg))
       {
           world.err=TD_FAILED_GRID_OPEN;
           return(world.err);
       }
       world.fdmval = sapoolg.params().dummy_[0]; 
       if(!rasterInitWithDummy(larr, elevPars, -1.0))
       {
           world.err = TD_FAILED_GRD_INIT;
           return(world.err);
       }
       if(!allocategrid(larr, -1.0)) 
       {
           world.err = TD_FAILED_GRD_INIT;
           return(world.err);
       }
   }//if using flow file
   /*allocate dir and stack arrays*/
   if(!rasterInitWithDummy(sdir, elevPars, MISSINGINT))
   {
       world.err = TD_FAILED_GRD_INIT;
       return(world.err);
   }
   if(!allocategrid(sdir,sdir.params().dummy_[0]))
   {
       world.err = TD_FAILED_GRD_INIT;
       return(world.err);
   }  
   world.err= setdfnoflood(world, sdir, felevg, larr, fareaw, sapoolg, fweight);
   /*  allocate memory for slope and angle  */
   if(!rasterInitWithDummy(fslopeg,elevPars, -1))
   {
       world.err = TD_FAILED_GRD_INIT;
       return(world.err);
   }
   if(!allocategrid(fslopeg, fslopeg.params().dummy_[0])) 
   {
       world.err = TD_FAILED_GRD_INIT;
       return(world.err);
   }
   if(!rasterInitWithDummy(fangle, elevPars, -2))
   {   
       world.err = TD_FAILED_GRD_INIT;
       return(world.err);
   }
   if(!allocategrid(fangle,   fangle.params().dummy_[0]))
   {
       world.err = TD_FAILED_GRD_INIT;
       return(world.err);
   }
   setdf2(world, felevg, fangle, fslopeg, sdir, sapoolg);
   if(!gridwrite(slopefile, fslopeg)) 
   {
       world.err=TD_FAILED_GRID_SAVE;
       return world.err;
   }
   if(!gridwrite(angfile, fangle))
   {
       world.err=TD_FAILED_GRID_SAVE;
       return(world.err);
   }
   /*  Wrapping up  */
   world.err=TD_NO_ERROR;
   return(world.err);
}//debug setdir() 

bool rasterInitWithDummy(TeRaster& newRaster, TeRasterParams newRasterParams,  
                          double dummy)                                        
{  //pass new raster parameters to new raster.                                 
   newRasterParams.useDummy_ = true;//set all parameters we know we might need 
   newRasterParams.setDummy(dummy, -1);//make needed changes in new params     
   newRasterParams.mode_ = 'c';//read and write                                
   newRasterParams.decoderIdentifier_ = "SMARTMEM";//now, this matters         
   newRaster.updateParams(newRasterParams);//pass values to new Raster         
   TeDecoder* dec = new TeDecoderSmartMem(newRasterParams);                    
   newRaster.setDecoder(dec);                                                  
   if(!newRaster.init())// make new Raster ready to use                        
       return false;                                                           
   return true;                                                                
}                                                                              

/*
  matalloc(...) allocates memory for matrix navigation pointer
  and for matrix data, then returns matrix navigation pointer
  Modification of matrixalloc by DGT to not use so many pointers
  7/1/97
  This is a further modification of the function to support doubles
  11/13/07
*/

void** matalloc(int nx,int ny,int datatype)
{
    int i, arrsize;
    void **mat;
    void *data;
    int **imat;
     int **smat;
    double **fmat;
    double **dmat;

    //Allocate memory for array navigation pointers, or pointers to rows
    if(datatype == 1)
    {
      mat = (void **)malloc(sizeof( int *)*(nx));
      arrsize = sizeof( int)*(nx)*(ny);
    }
    else if(datatype == 2)
    {
      mat = (void **)malloc(sizeof(int *)*(nx));
      arrsize = sizeof(int)*(nx)*(ny);
    }
    else if(datatype == 3)
    {
      mat = (void **)malloc(sizeof(double *)*(nx));
      arrsize = sizeof(double)*(nx)*(ny);
    }
    else if(datatype == 4)
    {
      mat = (void **)malloc(sizeof(double *)*(nx));
      arrsize = sizeof(double)*(nx)*(ny);
    }

    if(mat == NULL)//if pointer allocation failed
    {
//      printf("\nError: Cannot allocate memory for matrix navigation pointers");
//      printf("\n       nx = %d, ny = %d\n\n",nx,ny);
//      exit(2);
        return NULL;
    }
    
    data = malloc(arrsize);//allocate data matrix
    
    if(data == NULL)//If matrix allofr failed
    {
//      printf("\nError: Cannot allocate memory for matrix of dimension");
//      printf("\n       nx = %d, ny = %d\n\n",nx,ny);
//      exit(3);
        return NULL;
    }
    
    switch(datatype)
    {
      case 1:
        smat = ( int **)mat;
        for(i=0; i<(nx); i++)
        {
          smat[i] = &((( int *)(data))[i*(ny)]);
        }
        break;
      case 2:
        imat = (int **)mat;
        for(i=0; i<(nx); i++)
        {
          imat[i] = &(((int *)(data))[i*(ny)]);
        }
        break;
      case 3:
        fmat = (double **)mat;
        for(i=0; i<(nx); i++)
        {
          fmat[i] = &(((double *)(data))[i*(ny)]);
        }
        break;
      case 4:
        dmat = (double **)mat;
        for(i=0; i<(nx); i++)
        {
          dmat[i] = &(((double *)(data))[i*(ny)]);
        }
        break;
      default:
        break;
    }
    return(mat);
}

//*****************************************************************************
void VSLOPE(double E0, double E1, double E2, double D1, double D2, double DD,
            double *S, double *A)
{  //SUBROUTINE TO RETURN THE SLOPE AND ANGLE ASSOCIATED WITH A DEM PANEL 
   double S1,S2,AD;
   
   if(D1!=0) S1=(E0-E1)/D1;
   if(D2!=0) S2=(E1-E2)/D2;
   
   if(S2==0 && S1==0) *A=0;
   else *A= (double) atan2(S2,S1);
   
   AD= (double) atan2(D2,D1);
   if(*A  <   0.)
   {
       *A=0.;
       *S=S1;
   }
   else if(*A > AD)
   {
       *A=AD;
       *S=(E0-E2)/DD;
   }
   else *S= (double) sqrt(S1*S1+S2*S2);
}

//*****************************************************************************
// SUBROUTINE TO RETURN SLOPE AND ANGLE OF STEEPEST DECENT IF POSITIVE 
void SET2(int I, int J, double *DXX, double DD, TeRaster& fslopeg, 
          TeRaster& felevg, TeRaster& fangle)
{
   double SK[9];
   double ANGLE[9];
   double SMAX;
   int K;
   int KD;
   //To maintain the original logic, the array names starting with 'I' and the
   //array names staring with 'J' have been swiched. J1 and J2 are now I1 and 
   //I2, and vice-versa. ID1 and ID2 have also been swiched.
   int ID1[]= { 0, 2, 1, 1, 2, 2, 1, 1, 2};
   int ID2[]= { 0, 1, 2, 2, 1, 1, 2, 2, 1};
   int I1[] = { 0, 1, 0, 0,-1,-1, 0, 0, 1};
   int I2[] = { 0, 1, 1,-1,-1,-1,-1, 1, 1};
   int J1[] = { 0, 0,-1,-1, 0, 0, 1, 1, 0};
   int J2[] = { 0,-1,-1,-1,-1, 1, 1, 1, 1};

   double  ANGC[]={0,0.,1.,1.,2.,2.,3.,3.,4.};
   double  ANGF[]={0,1.,-1.,1.,-1.,1.,-1.,1.,-1.};
   
   for(K=1; K<=8; K++)
   {
       VSLOPE(d(felevg,       I,       J),/*entered as E0 */
              d(felevg, I+I1[K], J+J1[K]),/*entered as E1 */
              d(felevg, I+I2[K], J+J2[K]),/*entered as E3 */
              DXX[ID1[K]], /*entered as D1 */
              DXX[ID2[K]], /*entered as D2 */
              DD, /*entered as DD */
              &SK[K], /*entered as *S */
              &ANGLE[K]);/*entered as *A */
   }
   
   SMAX=0.;
   KD=0;
   d(fangle, I, J, -1.);//USE -1 TO INDICATE DIRECTION NOT YET SET 
   for(K=1; K<=8; K++)
   {
       if(SK[K] >  SMAX)
       {
           SMAX=SK[K];
           KD=K;
       }
   }

   if(KD  > 0)
   {
       d(fangle, I, J, (double) (ANGC[KD]*PI2+ANGF[KD]*ANGLE[KD]) );
   }
   d(fslopeg, I, J, SMAX);
}

//*****************************************************************************
void set2(int i,int j,double *fact,double *elev1, double *elev2, int iter, 
          int **spos,  int *s, TeRaster& sdir, 
          TeRaster& felevg)
{
/*  This function sets directions based upon secondary elevations for
  assignment of flow directions across flats according to Garbrecht and Martz
  scheme.  There are two possibilities: 
   A.  The neighbor is outside the flat set
   B.  The neighbor is in the flat set.
   In the case of A the elevation of the neighbor is set to 0 for the purposes
   of computing slope.  Since the incremental elevations are all positive there
   is always a downwards slope to such neighbors, and if the previous elevation
   increment had 0 slope then a flow direction can be assigned.
*/
   double slope,slope2,smax,ed;
   int k,spn,sp; 
   //int kflat=0;
   int in,jn;
   smax=0.;
   sp=spos[i][j];
   for(k=1; k<=8; k++)
   {
       jn=j+vector_J[k];
       in=i+vector_I[k];
       spn=spos[in][jn];
       if(iter <= 1)
       {
           ed =  d(felevg , i, j) -  d(felevg , in, jn);
       }
       else//if iter > 1
       {
           ed = elev1[sp] - elev1[spn];
       }
       slope=fact[k]* ed;
       if(spn < 0 || s[spn] < 0)
       {
           /*  The neighbor is outside the flat set.  */
           ed=0.;
       }
       else //if spn >= 0 && s[spn] > 0
       {
           ed=elev2[spn];
       }
       slope2 =fact[k]*(elev2[sp]-ed);//Only if latest iteration slope is 
       //positive and previous iteration slope flat
       //9/13/03  add check for opposing directions to resove a bug reported by 
       //Anna Katarina Mahlau
        
       int adrdirnn=0;
       if( (int)d(sdir , in, jn) > 0 && (int) d(sdir, in, jn)<=8)
           adrdirnn=abs( (int) d(sdir , in, jn)-k);  
           //only calculate if sdir has been really set
           //if(adrdirnn == 4)
           //    in=in;
       if(slope2 > smax 
       && slope >= 0. 
       && ( d(felevg , i, j) - d(felevg , in, jn))>=0. 
       && adrdirnn!=4)
       //  Only if latest iteration slope is positive and previous iteration  
       //slope flat and real elevation difference not uphill The elevation    
       //check added 3/1/03 to resolve a bug reported by Anna Katarina Mahlau 
       //<au@ivu-umwelt.de>                                                   
       {
           smax=slope2;
           d(sdir, i, j, k);
       }
   }  /*  End of for  */

}

//*****************************************************************************
void incfall(int n, double *elev1,  int *s1,int **spos, int iter, int *sloc,
             WorldRecord& world, TeRaster& sdir, TeRaster& felevg)
{
    /* This routine implements drainage towards lower areas - stage 1 */
   int done=0,donothing,k,ip,ninc,nincold,spn;
   int st=1,i,j,in,jn;
   double ed;
   nincold= -1;
   while(done < 1)
   {
       done=1;
       ninc=0;
       for(ip=0; ip<n; ip++)
       {
           //if adjacent to same level or lower that drains or 
           //adjacent to pixel with s1 < st and dir not set
           //do nothing 
           donothing=0;
           j=world.js[sloc[ip]];
           i=world.is[sloc[ip]];
           for(k=1; k<=8; k++)
           {
               //7/28/04  DGT Dont cross check added
              if(dontcross(k,i,j, sdir) ==0)
              {    // Only examine neighbors that do not cross existing flow 
                   //directions, e.g. due to burn in
                   jn=j+vector_J[k];
                   in=i+vector_I[k];
                   spn=spos[in][jn];
                   if(iter <= 1)
                   {
                       ed=  d(felevg , i, j) - d(felevg , in, jn);
                   }
                   else
                   {
                       ed = elev1[sloc[ip]] - elev1[spn];
                   }
                   //9/13/03  DGT changed conditional below to >=1 <=8 rather 
                   //than !=0 to also capture 9 case decided to revert back to 
                   //old code. This did not succeed in flagging the entire pit
                   if(ed >= 0. &&  d(sdir , in, jn) != 0)//fixed(jn,in)to(in,jn)
                   {
                       donothing = 1;  /* If neighbor drains */
                   }
                   if(spn >= 0) /* if neighbor is in flat   */
                   {
                       if(s1[spn]>= 0 && s1[spn]< st && (d(sdir, in, jn) == 0))
                       {/*  If neighbor is not being  */
                           donothing = 1;   /*  Incremented  */
                       }
                   }//if spn >= 0 OR if meighbor is in flat
              }//if(dontcross(k,i,j, sdir) ==0)
           }//for k in (0 to 9) 9/13/03 GT added ==9 in the above for consistency
           if(donothing == 0)
           {
               s1[sloc[ip]]++;
               ninc++;
               done=0;
           }
       }//for(ip=0; ip<n; ip++) //End of loop over all flats
       st=st+1;
       //printf("Incfall %d %d \n",ninc,n);
       if(ninc == nincold)
       {
           done = 1;
           //printf("There are pits remaining, direction will not be set\n");
           //Set the direction of these pits to 9 to flag them
           for(ip=0; ip<n; ip++)  //loop 2 over all flats  
           {
               //if adjacent to same level or lower that drains or 
               //adjacent to pixel with s1 < st and dir not set
               //do nothing  
               donothing=0;
               j=world.js[sloc[ip]];
               i=world.is[sloc[ip]];
               for(k=1; k<=8; k++)
               {
                   jn=j+vector_J[k];
                   in=i+vector_I[k];
                   spn=spos[in][jn];
                   if(iter <= 1)
                   {
                       ed= d(felevg, i, j) -  d(felevg, in, jn);
                   }
                   else
                   {
                       ed= elev1[sloc[ip]]- elev1[spn];
                   }
                   //9/13/03 DGT changed conditional below to >=1 <=8 rather 
                   //than !=0 to also capture 9 case decided to revert back to 
                   //old code.  This did not succeed in flagging the entire pit
                   if(ed >= 0. &&  d(sdir , in, jn) != 0)//fixed(jn,in)to(in,jn)
                       donothing = 1;  /* If neighbor drains */
                   if(spn >= 0) /* if neighbor world.is in flat   */
                       if(s1[spn] >= 0 && s1[spn] < st/*If neighbor is not being*/
                       &&(d(sdir , in, jn)  == 0 ))
                           donothing = 1;   /*Incremented*/
               }  //9/13/03  DGT added == 9 in the above for consistency
               if(donothing == 0)
               {
                   d(sdir, i, j, 9);
                              
                }
           }   /*  End of loop 2 over all flats  */
       }//if(ninc == nincold)
       nincold=ninc;
   }  /*  End of while done loop  */
}

//*****************************************************************************
void incrise(int n, double *elev1,  int *s2,int **spos, int iter, int *sloc, 
             WorldRecord& world, TeRaster& felevg)
{
   /*  This routine implements stage 2 drainage away from higher ground 
   dn is used to flag pixels still being incremented */
   int done=0,ip,k,ninc,nincold,spn;
   double ed;
   int i,j,in,jn;
   nincold=0;
   while(done < 1)
   {
       done=1;
       ninc=0; 
       for(ip=0; ip<n; ip++)
       {
           for(k=1; k<=8; k++)
           {
               j=world.js[sloc[ip]];
               i=world.is[sloc[ip]];
               jn=j+vector_J[k];
               in=i+vector_I[k];
               spn=spos[in][jn];
               if(iter <= 1)
               {
                   ed =  d(felevg, i, j) -  d(felevg, in, jn);
               }
               else
               {
                   ed= elev1[sloc[ip]] - elev1[spn];
               }
               if(ed < 0.)world.dn[sloc[ip]] = 1;
                   if(spn >=0)
                       if(s2[spn] > 0)
                           world.dn[sloc[ip]] = 1;
           }//for k in (0 to 9)
       }//for ip in [0 to n)
       for(ip=0; ip<n; ip++)
       {
           s2[sloc[ip]]=s2[sloc[ip]]+world.dn[sloc[ip]];
           ninc=ninc+ (world.dn[sloc[ip]]>0 ? 1: 0);// DGT 9/13/03 changed to 
                                                   //because now dn is not 0 or 1
           if(world.dn[sloc[ip]] == 0)
               done=0;//if still some not being incremented continue looping
       }//for(ip=0; ip<n; ip++)
       //printf("incrworld.ise %d %d\n",ninc,n);   
       if(ninc == nincold)
           done=1;//If there are no new cells incremented stop this is the 
                 //case when a flat has no higher ground around it.
       nincold=ninc;
   }//while(done < 1)
}

//*****************************************************************************
int flatrout(int n,int *sloc, int *s, int **spos,int iter,double *elev1,
             double *elev2, double *fact, int ns, WorldRecord& world, 
             TeRaster& sdir, TeRaster& felevg)
{
   int ip,nu, *sloc2,ipp;
   world.err=TD_NO_ERROR;
   double *elev3;
   
   incfall(n,elev1,s,spos,iter,sloc, world, sdir, felevg);
   for(ip=0; ip < n; ip++)
   {
       elev2[sloc[ip]]=(double)(s[sloc[ip]]);
       s[sloc[ip]]=0;   /*  Initialize for pass 2  */
   }
    
   incrise(n,elev1,s,spos,iter,sloc, world, felevg);
   for(ip=0; ip < n; ip++)
   {
       elev2[sloc[ip]] += (double)(s[sloc[ip]]);
   }
   
   nu=0;
   for(ip=0; ip < n; ip++)
   {
       set2(world.is[sloc[ip]], world.js[sloc[ip]], fact, elev1, elev2, iter, 
            spos, s, sdir, felevg);
       if( d(sdir, world.is[sloc[ip]], world.js[sloc[ip]]) == 0)  nu++;
   }
   if(nu >= n) return(TD_NO_ERROR);//The recursion is not converging so bail
   //DGT 7/31/04 No error reported because this usually occurs at pits and it 
   //is OK to have pits be no data
   if(nu > 0)
   { 
       /*  Iterate Recursively   */
       /*  Now resolve flats following the Procedure of Garbrecht and Martz, 
         Journal of Hydrology, 1997.  */
       iter=iter+1;
       //printf("Resolving %d Flats, Iteration: %d \n",nu,iter);
       sloc2 = (int *)malloc(sizeof(int) * nu);
       elev3 = (double *)malloc(sizeof(double) *ns);
       
       if(sloc2 == NULL || elev3 == NULL)
       {
           // printf("Unable to allocate at iteration %d\n",iter);
       }
       /*  Initialize elev3  */
       for(ip=0; ip < ns; ip++)  elev3[ip]=0.;
       /*  Put unresolved pixels on new stacks - keeping in same positions  */
       ipp=0;
       for(ip=0; ip<n; ip++)
       {
           if(d(sdir, world.is[sloc[ip]], world.js[sloc[ip]]) == 0)
           {
               sloc2[ipp]=sloc[ip];
               /*   Initialize the stage 1 array for flat routing   */
               s[sloc[ip]] = 1;
               ipp++;
                
           }//if(d(sdir, world.is[sloc[ip]], world.js[sloc[ip]]) == 0)
           else
           {
               s[sloc[ip]] = -1;//Used to designate out of remaining flat on 
                                //higher iterations 
           }
           world.dn[sloc[ip]]=0;  /*  Reinitialize for next time round.  */
       }
       world.err=flatrout(nu,sloc2,s,spos,iter,elev2,elev3,fact,ns, world, sdir, 
                          felevg);
       //free(sloc2);
       //free(elev3);
       //printf("Done iteration %d\nFlats resolved %d\n",iter,n);
   } /*  end if nu > 0  */
   return(world.err);
}   /*  End flatrout  */

int setdfnoflood(WorldRecord& world, TeRaster& sdir, TeRaster& felevg, 
                 TeRaster& larr, TeRaster& fareaw,  TeRaster& sapoolg, 
                 TeRaster& fweight)
/*  This version is stripped of pit filling  */
{
   int i,j,k,ip, n, iter; 
   world.err=TD_NO_ERROR;
   double fact[9];
   int *s;  /*  variables for flat draining   */
   int **spos, *sloc;
   double *elev2;

/*  Initialize boundaries  */
  for(i = world.minX; i< world.numX; i++)
  {
     d(sdir, i,            0, MISSINGINT);
     d(sdir, i, world.numY-1, MISSINGINT);
  }
  for(j = world.minY; j< world.numY; j++)
  {
     d(sdir,            0, j, MISSINGINT);
     d(sdir, world.numX-1, j, MISSINGINT);
  }

   /*initialize internal pointers to 0 except where elevation is no data*/
    
   for(j=world.minY+1; j< world.numY-1; j++)//for all columns                  
   {                                                                           
        for(i=world.minX+1; i< world.numX-1; i++)//for all cols                
        {                                                                      
           if(d(felevg, i, j) <= world.mval)//Dummy must be < minimum value   
           {                                                                   
               d(sdir, i, j, MISSINGINT ); // -32767                           
           }                                                                   
           else                                                                
               d(sdir, i, j, 0);                                               
        }//for all cols                                                        
   }//for all lines                                            
    /*  Direction factors  */                                  
    for(k=1; k<= 8; k++)                                       
       fact[k]= (double) (1./
        sqrt(vector_J[k]*vector_J[k] * world.dy*world.dy 
           + vector_I[k]*vector_I[k] * world.dx*world.dx)); 

//  Set stream overlay directions
   if(world.useflowfile)                                             
   {                                                           
       for(j=world.minY+1; j<world.numY-1; j++)//for all lines     
       {                                                       
           for(i=world.minX+1; i< world.numX-1; i++)//for all cols 
           {                                                   
               if(d(sapoolg, i, j) > world.fdmval )                  
               {                                               
                   d(sdir, i, j, d(sapoolg, i, j));            
                   d(larr, i, j, 0);                           
               }//if                                           
           }//for i in(i1 to n1) OR for all cols               
       }//for j in(i2 to n2) OR for all lines                  
   }//if(useflowfile)
//   Compute contrib area using overlayed directions for direction setting
   world.ccheck=0;   // dont worry about edge contamination
   world.useww=0;// dont worry about weights
   for(j=world.minY+1; j<world.numY-1; j++)//for all lines         
   {                                                           
       for(i=world.minX+1; i< world.numX-1; i++)//for all cols     
       {                                                       
           //This allows for a stream overlay                  
           if(d(sdir , i, j) > 0)                              
               darea(i, j, world, sdir, larr, fareaw,fweight); 
       }//for i in(i1 to n1) OR for all cols                   
   }//for j in(i2 to n2) OR for all lines                      
/*  Set positive slope directions   */
   n=0;
   for(j=world.minY+1; j<world.numY-1; j++)//for all lines but edges           
   {                                                                           
       for(i=world.minX+1; i< world.numX-1; i++)//for all cols but edges       
       {                                                                       
           if(d(sdir, i, j) == 0 )                                             
           {                                                                   
               if(d(felevg, i, j)  > world.mval)                               
               {                                                               
                   set1(i, j, fact, world.mval, world.useflowfile, sdir, larr, 
                        felevg);                                               
                   if( d(sdir , i, j) == 0)//if sdir(i, j) still == 0          
                       n++;//n = number of directions not initialized above    
               }//if felevg[i][j] > mval                                       
           }//if sdir[i][j] == 0                                               
       }//for i in (i1 to n1) OR for all cols                                  
   }//for j in(i2 to n2) OR for all lines                                      
   
   if(n > 0)
   { 
   /* Now resolve flats following the Procedure of Garbrecht and Martz, Journal
   of Hydrology, 1997.  */

/*  Memory is utilized as follows
is, js, dn, s and elev2 are unidimensional arrays storing information for flats.
sloc is a indirect addressing array for accessing these - used during recursive 
iteration. spos is a grid of pointers for accessing these to facilitate finding 
neighbors

The routine flatrout is recursive and at each recursion allocates a new sloc for
addressing these arrays and a new elev for keeping track of the elevations
for that recursion level.  
  */
       iter=1;
//      printf("Resolving %d Flats, Iteration: %d \n",n,iter);
       spos = (int **) matalloc(world.numX, world.numY, RPINTDTYPE);
       world.dn = ( int *)malloc(sizeof( int) * n);
       world.is = ( int *)malloc(sizeof( int) * n);
       world.js = ( int *)malloc(sizeof( int) * n);
       s = ( int *)malloc(sizeof( int) * n);
       sloc = (int *)malloc(sizeof(int) * n);
       elev2 = (double *)malloc(sizeof(double) *n);
       
       if(world.dn == NULL || world.is == NULL || world.js == NULL 
       || s == NULL || spos == NULL || elev2 == NULL || sloc == NULL)
       {
           //printf("Unable to allocate at iteration %d\n",iter);
           world.err = TD_FAILED_MEMORY_ALLOC;
           return(world.err);
       }
       /*Put unresolved pixels on stack  */
       ip=0;
       for(i=world.minX; i< world.numX; i++)
       {
           for(j=world.minY; j<world.numY; j++)
           {
               spos[i][j]=-1;   /*  Initialize stack position  */
               if( (int) d(sdir, i, j) == 0)//if local flow is not initialized  
               {                                                                
                   world.is[ip]=i;// is[ip] = x-coordinate of cell of this flow 
                   world.js[ip]=j;// js[ip] = y-coordinate of cell of this flow 
                   world.dn[ip]=0;// dn[ip] = unresolved direction of this flow 
                   sloc[ip]=ip;//sloc[ip] = index of this cell in linear array  
                   s[ip] = 1;//Initialize the stage 1 array for flat routing    
                   spos[i][j]=ip;//  pointer for back tracking                  
                   ip++;//count a nother unresolved pixel                       
                   if(ip>n){world.err=TD_INVALID_ARRAY_SIZE; return(world.err);}
               }//if sdir(i,j) == 0                                             
           }//for j in[minY to numY) OR for all lines                           
       }//for i in(minX to numX) OR for all columns                             
       world.err= flatrout(n, sloc, s, spos, iter, elev2, elev2, fact, n,       
                           world, sdir, felevg);                                
       /*  The direction 9 was used to flag pits.  Set these to 0  */
       for(j=world.minY; j<world.numY; j++)//for all lines
       {
           for(i=world.minX; i< world.numX; i++)//for all columns
           {
               if( d(sdir , i, j) == 9) d(sdir , i, j, sdir.params().dummy_[0]);
               if( d(sdir , i, j) == 0) d(sdir , i, j, sdir.params().dummy_[0]);
           }//for i in (i1 to n1) OR for all columns
       }//for j in (i2 to n2) OR for all lines
       free(spos[0]); //free(spos);
       free(elev2);
       free(world.dn);
       free(world.is);
       free(world.js);
       free(s);
       free(sloc);
       //printf("Done iteration %d\nFlats resolved %d\n",iter,n);
   } /*  End if n > 0  */
   return(world.err);   /*  OK exit from setdir  */
}  /*  End setdfnoflood  */

//*****************************************************************************
/*
Converted from FORTRAN July 05, 1997  K. Tarbet

C
C---SUBROUTINE TO SET VARIABLE DIRECTIONS.
CDIRECTIONS MEASURED ANTICLOCKWISE FROM EAST IN RADIANS.
C
*/

void setdf2(WorldRecord& world, TeRaster& felevg, TeRaster& fangle, 
            TeRaster& fslopeg, TeRaster& sdir, TeRaster& sapoolg)
{

    double FANG[9];
    double dxx[3],slp;
    int i,j,k;
    double dd;

/*   INITIALISE ANGLES and slopes */

    for (i=0; i< world.numX; i++)//for all colunms
{
        d(fangle, i,          0, -2);//set top line
        d(fangle, i, world.numY-1, -2);//set botton line
        
        d(fslopeg, i,          0, -1);//set top line
        d(fslopeg, i, world.numY-1, -1);//set botton line

}

    for(j=0; j<world.numY; j++)//for all lines
    {
        d(fangle,          0, j, -2);//set left most column
        d(fangle, world.numX-1, j, -2);//set rigth most column
        
        d(fslopeg,          0, j, -1);//set left most column
        d(fslopeg, world.numX-1, j, -1);//set rigth most column

    }

 FANG[1]=0.;
 FANG[2]=(double)atan2(world.dy, world.dx);
 FANG[3]=(double) PI2; 
 FANG[4]=2*FANG[3]-FANG[2];
 FANG[5]=2*FANG[3];
 FANG[6]=2*FANG[3]+FANG[2];
 FANG[7]=3*FANG[3];
 FANG[8]=4*FANG[3]-FANG[2];

/* --INITIALISE INTERNAL POINTERS (-VE POINTER INDICATES OUTSIDE DOMAIN) */
   for(j=1; j<world.numY-1; j++)//for all lines
   {
       for(i=1; i<world.numX-1; i++)//for all columns
       {
           if( d(sdir , i, j) <  0)   
           {                 
               d(fangle , i, j, -2.0);
               d(fslopeg , i, j, -1.0);
               /*  !  -1 slope indicates no data */
           }//if dir element < 0
           else if( d(sdir , i, j) == 0)
           {                
               d(fangle , i, j, -1.0);
               /*  DGT 9/2/97 since without   */
               d(fslopeg , i, j, 0.0);        
               //ang = -1 designates unfilled pit, ang = -2 designates no data.
           }//else if dir element == 0
           else// if dir element > 0
               d(fangle , i, j, 0.0);
       }//for all columns
   }//for all lines
    
/*TEST ALL INTERNAL ELEVATIONS AND SET SLOPE AND ANGLE */
   dxx[1]=world.dx;
   dxx[2]=world.dy;
   dd=(double)sqrt(world.dx * world.dx  +  world.dy * world.dy);
   for(j= world.minY + 1; j<world.numY-1; j++)//for all lines
   {
       for(i= world.minX + 1; i< world.numX-1; i++)//for all columns
       {
           if( d(sdir , i, j) >0 )
           {
               SET2(i, j, dxx, dd, fslopeg, felevg, fangle);
               //9/14/02 DGT decided to set flow directions along exworld.
               //isting paths if they are given 
               if(world.useflowfile)
               {
                   k= (int) d(sapoolg , i, j);
                   if(k > 0)
                   {                       
                       d(fangle , i, j, FANG[ k ]);
                       slp=( d(felevg , i, j)- d(felevg,i+vector_I[k], j+vector_J[k]))
                            /sqrt(world.dx*world.dx*vector_I[k]*vector_I[k]
                                 +world.dy*world.dy*vector_J[k]*vector_J[k]);
                       d(fslopeg , i, j, slp);
                   }//if z > 0
               }//if using flow file
               if( d(fangle , i, j) < -0.5)
               {
                   d(fangle , i, j, FANG[ (int)d(sdir , i, j)]);
               }//if angle < -0.5
           }//if dir element > 0
       }//for all columns
   }//for all lines
}//sedf2


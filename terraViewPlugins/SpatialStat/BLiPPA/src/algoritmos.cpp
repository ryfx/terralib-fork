/**
 * @file algrotimos.cpp
 * @author Danilo Lourenco Lopes.
 * @date July 03, 2007
 */

#include <algoritmos.h>

/*-------------------------------------------------------------------------------

  ------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <CImg.h>
#include <math.h>
#include <vector>
#include <fstream>

#include <TeProgress.h>
#include <qobject.h>

//using namespace std;
//using namespace cimg_library;

/*
 * Figure out which quadrent pt is in with respect to orig
 */
int whichquad(double ptx, double pty, double origx, double origy)
//Adapted from: kenm@sci.UUCP (Ken McElvain)
//Organization: Silicon Compilers Systems Corp. San Jose, Ca
{
        if(ptx < origx) {
                if(pty < origy) return 2;
                else return 1;
        } else {
                if(pty < origy) return 3;
                else return 0;
        }
}


/*
 * Quadrants:
 *    1 | 0
 *    -----
 *    2 | 3
 */

bool within(double x, double y, const std::vector<double> &xp, const std::vector<double> &yp)
//Adapted from: kenm@sci.UUCP (Ken McElvain)
//Organization: Silicon Compilers Systems Corp. San Jose, Ca
{
		unsigned int i;
        int oldquad,newquad;
        double thisptx, thispty, lastptx, lastpty,a,b;
        int wind;       /* current winding number */

        wind = 0;
        lastptx = xp[xp.size()- 2];
        lastpty = yp[yp.size()- 2];
        oldquad = whichquad(lastptx, lastpty, x, y); /* get starting angle */
        for(i=0;i<xp.size()-1;i++) { /* for each point in the polygon */
                thisptx = xp[i];
                thispty = yp[i];
                newquad = whichquad(thisptx, thispty, x, y);
                if(oldquad!=newquad) { /* adjust wind */
                        /*
                         * use mod 4 comparsions to see if we have
                         * advanced or backed up one quadrant 
                         */
                        if(((oldquad+1)%4)==newquad) wind++;
                        else if(((newquad+1)%4)==oldquad) wind--;
                        else {
                                /*
                                 * upper left to lower right, or
                                 * upper right to lower left. Determine
                                 * direction of winding  by intersection
                                 *  with x==0.
                                 */
                                a = lastpty - thispty;
                                a *= (x - lastptx);
                                b = lastptx - thisptx;
                                a += lastpty * b;
                                b *= y;

                                if(a > b) wind += 2;
                                else wind -= 2;
                        }
                }
                lastptx = thisptx;
                lastpty = thispty;
                oldquad = newquad;
        }
        return( wind!=0 ); /* non zero means point in poly */
}

double distance(double x1, double y1, double x2, double y2)
{
       return (sqrt( pow(x1-x2,2.0) + pow(y1-y2,2.0) ));
}

double arzz(double d, double theta)
{
  double t = tan(theta);
  double arzz = t*pow(d,2.0)*(1-pow(d,2.0)+pow(d,4.0)/3)
	  + pow(t,3.0)*pow(d,4.0)*(pow(d,2.0)/3-1)/3 +(pow(t,5.0))*pow(d,6.0)/15;
 return (arzz*1.5/cimg_library::cimg::PI);
}

double ssarea( double d1t, double d2t, double h)
{
	double d1,d2, ssarea, theta1, theta2, theta3, theta4;
   d1 = MIN(1.0,d1t/h);
   d2 = MIN(1.0,d2t/h);
   ssarea = 1.0;
   if(d1*d2 != 1.0)
   {
	   theta1 = acos(d1);
	   theta2 = acos(d2);
	   theta3 = atan(d1/d2);
	   theta4 = 1.50796327-theta3;
	   ssarea = 0.75-(theta1+theta2)/(2*cimg_library::cimg::PI) + arzz(d1,theta1)
                          + arzz(d2,theta2) + arzz(d2,theta3) + arzz(d1,theta4);
  }
 return (ssarea);
}

int kernelink(unsigned int nx,unsigned  int ny, double h1, double h2, double x,
             double y, const std::vector<double> &xp, double xmin, double xmax,
             const std::vector<double> &yp, double ymin, double ymax,              
             const std::vector<double> &x1, const std::vector<double> &y1, 
             const std::vector<double> &x2, const std::vector<double> &y2, 
             std::vector<double> &xgrid, std::vector<double> &ygrid, 
             std::vector< std::vector<double> > &zgrid, double &MaxZ)
{

	double xinc, yinc, xaux, yaux, hx, hy, d, d1, d2, kaux, ksum = 0.0;
	unsigned int i,j,k,cont=0;
	std::vector<double> x2aux, y2aux, kernx;
	
	if(!within(x,y,xp,yp))
		return 1; // ERROR 1: Invalid conditional position argument

	 zgrid.resize(nx);  // Set nx lines for zgrid matrix
     xinc = (xmax - xmin)/(double)nx;   // Calculate increments for xgrid vector
     yinc = (ymax - ymin)/(double)ny;   // Calculate increments for zgrid vector
	 xaux = xmin + 0.5*xinc;            // First values for xgrid and ygrid vectors
	 yaux = ymin + 0.5*yinc;            // xgrid and ygrid are coordinates for rectangles' centers

	for(i=0;i<nx;i++)
	{
		xgrid.push_back(xaux);
		xaux += xinc;
		zgrid[i].resize(ny,-1.0);       // Set ny columns for each line
        // -1.0 means retangle outside polygon
	}

	for(i=0;i<ny;i++)
	{
		ygrid.push_back(yaux);
		yaux += yinc;
	}

	hx = h1*sqrt(2.0);        
	hy = h2*sqrt(2.0);

	for( i=0; i < x1.size(); i++)
	{
		d = distance(x, y, x1[i], y1[i]);
		if(d < hx)      // Find origin locations near to conditional location
		{
			x2aux.push_back(x2[i]);
			y2aux.push_back(y2[i]);
			cont++;

			kaux = 1.0-2.0*pow(d/hx,2.0)+pow(d/hx,4.0);  // Kernel value (less from a constant) for these points
			ksum += kaux;
			kernx.push_back(kaux);
		}
	}
	
	if(cont == 0)
		return 2; // ERROR 2: No points of type 1 near to the conditional position
		
    MaxZ = 0.0;
	for( i=0; i < nx; i++)
	{
		for( j=0; j < ny; j++)
		{
			if( within(xgrid[i], ygrid[j], xp, yp) )
			{
                // Grid points within polygon have value different from -1.0
				zgrid[i][j] = 0.0;
				d1 = MIN(xgrid[i] - xmin, xmax - xgrid[i]);        // Calculate minimum horizontal distance to bounding box
				d2 = MIN(ygrid[j] - ymin, ymax - ygrid[j]);        // Calculate minimum vertical distance to bounding box
					for( k=0; k < cont; k++)
					{
						d = distance(x2aux[k],y2aux[k],xgrid[i],ygrid[j]);
						if(d < hy)    // If selected destination location is near to grid point
							zgrid[i][j] += 3.0*kernx[k]*(1.0-2.0*pow(d/hy,2.0)+pow(d/hy,4.0));
					}
				zgrid[i][j] /= (ksum*hy*hy*ssarea(d1,d2,hy)*cimg_library::cimg::PI);
				// Edge correction and some constants
			    MaxZ = MAX(MaxZ, zgrid[i][j]); // maximum value of kernel function matrix
			}
		}
	}
	return 0;
}

int limited(int n)
{
      if(n >= 255) return 255;
      if(n <= 0) return 0;
      return n;
}

double rand0a1()
{
  double resultado= rand()/(double)(RAND_MAX+1);
  return resultado;
}

void Permut(std::vector<int> &A) 
{
     int i,j, b;
     for(i = A.size()-1; i>0; i --)
     {
           j = (int)((i+1)* rand0a1()) ;
           b = A[i];
           A[i] = A[j];
           A[j] = b;
     }
}

double Z(double x)
  {
  double retorno;

  retorno = 1/sqrt(2*cimg_library::cimg::PI)*exp(-x*x/2);

  return(retorno);
  }


double AcNormal(double x)
  {

  vector<double> b;
  double t, retorno, p, fator;
  bool xneg = false;

  b.push_back(0.31938);
  b.push_back(-0.35656);
  b.push_back(1.78147);
  b.push_back(-1.82125);
  b.push_back(1.33027);

  p = 0.23164;

  if(x<0)
   {
   x = x*(-1);
   xneg = true;
   }

  t = 1/(1+p*x);


  fator =0;

  for(unsigned int i=0; i<b.size(); i++)
   fator = fator + b[i]*pow(t, (double)(i+1));

  retorno = 1- Z(x)*(fator);

  if(xneg==true)
   retorno = 1 - retorno;


  return(retorno);

  }

void ScoreStat(const std::vector<double> &x1, const std::vector<double> &y1, 
               const std::vector<double> &x2, const std::vector<double> &y2,
               double h1, double h2, double &Obs, double &Exp, double &Var,
               std::vector<int> &i1, std::vector<int> &i2, bool & wasCancelled)
{
	wasCancelled = false;
	double N1 = 0.0, N2 = 0.0;
    unsigned int i, j, d1, d2, n1 = 0, n2 = 0, n12 = 0;
    std::vector<double> s1(x1.size(),0.0), s2(x1.size(),0.0);

	int progressAux = 0;
	TeProgress::instance()->reset();
	TeProgress::instance()->setTotalSteps(x1.size());
	TeProgress::instance()->setCaption("BLiPPA");
	TeProgress::instance()->setMessage(QObject::tr("Executing Step 1 of 2").latin1());
    
	for(i = 0; i < x1.size()-1; i++)
	{
		for(j = i+1; j < x1.size(); j++)
        {
			d1 = (int)(distance(x1[i],y1[i],x1[j],y1[j]) < h1);
            d2 = (int)(distance(x2[i],y2[i],x2[j],y2[j]) < h2);
            i1.push_back(d1);
            i2.push_back(d2);
            s1[i] += d1; s2[i] += d2;
            s1[j] += d1; s2[j] += d2;
            n1 += d1;
            n2 += d2;
            n12 += d1*d2;
        }
		
		TeProgress::instance()->setProgress(progressAux);
		progressAux++;

		if(TeProgress::instance()->wasCancelled())
		{
			wasCancelled = true;
			return;
		}

	}

	TeProgress::instance()->reset();
            
    for(i = 0; i < x1.size(); i++)
    {
		N1 += pow(s1[i],2.0);
        N2 += pow(s2[i],2.0);
    }
    
	N1 /= 2.0; N2 /= 2.0;
    N1 -= n1; N2 -= n2;
            
    Exp = 2.0*n1*n2/(double)(x1.size()*(x1.size()-1.0));
    Var = Exp + 4*N1*N2/(double)(x1.size()*(x1.size()-1.0)*(x1.size()-2.0));
    Var += 4*(n1*(n1-1.0)-N1)*(n2*(n2-1.0)-N2)/(double)(x1.size()*
    (x1.size()-1.0)*(x1.size()-2.0)*(x1.size()-3.0));
    Var -= pow(Exp,2.0);
    Obs = n12;
}

void ScoreTest(std::vector<int> i1, std::vector<int> i2, int n, int nsim,
               const double Obs, double &pvalue, bool & wasCancelled)
{
	wasCancelled = false;
	std::vector<int> index;
    int i, j, k, a, b;
    double Sim;
      
    pvalue = 1.0;
              
    for(i = 0; i < n; i++)
		index.push_back(i);
         
	int progressAux = 0;
	TeProgress::instance()->reset();
	TeProgress::instance()->setTotalSteps(nsim);
	TeProgress::instance()->setCaption("BLiPPA");
	TeProgress::instance()->setMessage(QObject::tr("Executing Step 2 of 2").latin1());

    for(i = 0; i < nsim; i++)
    {		
		Sim = 0.0;
        Permut(index);
        for(j = 0; j < n-1; j++)
			for(k = j+1; k < n; k++)
            {
				if( index[j] < index[k] )
                {
					a = index[k];
                    b = index[j];
                }
                else
                {
                    a = index[j];
                    b = index[k];
                }                            
                Sim += i1[(int)(k+j*n-1.5*j-j*j/2-1)]*i2[(int)(a+b*n-1.5*b-b*b/2-1)];
            }
		pvalue += (Sim >= Obs);

		TeProgress::instance()->setProgress(progressAux);
		progressAux++;

		if(TeProgress::instance()->wasCancelled())
		{
			wasCancelled = true;
			return;
		}
		
	}

	TeProgress::instance()->reset();
      
    pvalue /= (double)(nsim+1.0);
}


int ConditionalKernel(const unsigned int nx, const unsigned  int ny, 
					  const double h1, const double h2,
					  const std::vector<double> &xp, double xmin, double xmax,
					  const std::vector<double> &yp, double ymin, double ymax,              
					  const std::vector<double> &x1, const std::vector<double> &y1, 
					  const std::vector<double> &x2, const std::vector<double> &y2, 
					  const double rate, const bool MaxRegion)
{
    const unsigned char black[3] = { 0,0,0 }, purple[3] = { 255,0,255 }, blue[3] = { 0,0,255 }, red[3] = { 255,0,0 }, white[3] = { 255,255,255 };
    unsigned int i, dim1, dim2, a, b, bord1, bord2, wlegen, hlegen, wsca, hsca;
    unsigned int x, y, xini, yini, xfin, yfin, px, py, xm, ym, lxi, lxf, lyi, lyf, sxi, syi, cx, cy, cross, result;
    double scale, MaxZo, MaxZd, zmax, sinc;
    double  xcon, ycon, xincp, yincp;
	float  radiuso, radiusd;
    std::vector<double> xgrid, ygrid;
    std::vector< std::vector <double> > zgrid;
    std::vector<int> xpoly, ypoly;

     
    dim1 = 555;        // Window width
    dim2 = 555;        // Window height

    CImg<unsigned char> img(dim1,dim2,1,3),opts(dim1,dim2,1,3),dpts(dim1,dim2,1,3),img4(dim1,dim2,1,3),
                        img1o(dim1,dim2,1,3),img2o(dim1,dim2,1,3),
                        img1d(dim1,dim2,1,3),img2d(dim1,dim2,1,3);
    img.fill(255); opts.fill(255); dpts.fill(255);           // Set pixel values to 0 (color : white)
    CImgDisplay visu(opts,"Origin Point Pattern"),
                disp(dpts,"Destination Point Pattern");     //Creating Displays
    visu.move((CImgDisplay::screen_dimx()-disp.dimx()-visu.dimx())/2,(CImgDisplay::screen_dimy()-disp.dimy())/2);
    disp.move(visu.window_x + visu.window_width + 40,visu.window_y);   // Moving displays side by side
    b = (int)(0.8*dim1);       // Width of graphics device
    a = (int)(0.75*dim2);      // Height of graphics device
    bord1 = (int)(0.05*dim1);  // Width of right border
    bord2 = (int)(0.15*dim2);  // Height of lower border
    wlegen = (int)(0.1*dim1);  // Width of legend
    hlegen = (int)(0.168*dim2);// Height of legend
    lxi = (int)(0.05*dim1);    // Starting coordinate for legend
    lyi = (int)((dim2 - hlegen)/2); // Starting coordinate for legend
    lxf = lxi + wlegen;           // Ending coordinate for legend
    lyf = lyi + hlegen;           // Ending coordinate for legend
    wsca = 300;
    hsca = 2;
    syi = dim2 - hsca - (int)(bord2/2);
    sxi = (int)((dim1 - wsca)/2);
    cx = (int)((dim1-b-bord1)/2);
    cy = dim2 - bord2;
    xini = dim1-b-bord1;       // Starting coordinate for drawing 
    yini = dim2-a-bord2;       // Starting coordinate for drawing 
    
    
    if( (xmax - xmin)/(ymax - ymin) <= b/(double)a )  //Test for aspect ratio
    {
        scale = a/(ymax - ymin);    //Fit map to the height of graphics device
        xini += (int)((b - (xmax - xmin)*scale)/2);  // Centering the map
    }else
    {
        scale = b/(xmax - xmin);    //Fit map to the width of graphics device
        yini += (int)((a - (ymax - ymin)*scale)/2);  // Centering the map     
    }
    
    
    xfin = xini+(int)((xmax - xmin)*scale);          // Ending coordinate for drawing 
    yfin = yini+(int)((ymax - ymin)*scale);          // Ending coordinate for drawing 
    xincp = (xfin - xini)/(double)(nx-1);            // Width in pixels of colored rectangles 
    yincp = (yfin - yini)/(double)(ny-1);            // Height in pixels of colored rectangles 
    
    // Draw graphic scale rectangle
    opts.draw_rectangle(sxi + (int)(wsca/3), syi, sxi + (int)(2*wsca/3), syi + hsca, blue).
         draw_line(sxi,syi,sxi+wsca,syi,blue).draw_line(sxi,syi+hsca,sxi+wsca,syi+hsca,blue).
         draw_line(sxi,syi-hsca,sxi,syi+3*hsca,blue).draw_line(sxi + (int)(wsca/3),syi-hsca,sxi + (int)(wsca/3),syi+3*hsca,blue).
         draw_line(sxi + (int)(2*wsca/3),syi-hsca,sxi + (int)(2*wsca/3),syi+3*hsca,blue).draw_line(sxi+wsca,syi-hsca,sxi+wsca,syi+3*hsca,blue);
        
    sinc = wsca/(3*scale);
    // Draw graphic scale text
    opts.draw_text(sxi-1,syi-hsca-20,blue,0,17,1,"0").draw_text(sxi+(int)(wsca/3)-20,syi-hsca-20,blue,0,17,1,"%G",sinc).  
         draw_text(sxi+(int)(2*wsca/3)-20,syi-hsca-20,blue,0,17,1,"%G",2.0*sinc).draw_text(sxi+wsca-20,syi-hsca-20,blue,0,17,1,"%G",3.0*sinc);

    // Draw compass
    opts.draw_rectangle(cx -1,cy+10,cx +1,cy+35,black);
    opts.draw_triangle(cx, cy, cx-4, cy+15, cx, cy+10, black).draw_triangle(cx, cy, cx+4, cy+15, cx, cy+10, white);
    opts.draw_line(cx,cy,cx+4,cy+15,black).draw_line(cx+4,cy+15,cx,cy+10,black);
    opts.draw_text(cx-3,cy-13,black,0,10,1,"N");
     
    img = opts;    // Share graphic elements 
    
     // Conversion of polygon to pixels coordinates and its drawing
    xpoly.push_back(xini + (int)((xp[0] - xmin)*scale));
    ypoly.push_back(yfin - (int)((yp[0] - ymin)*scale));
    for (i = 1; i < xp.size(); i++)
    {
            xpoly.push_back(xini + (int)((xp[i] - xmin)*scale));
            ypoly.push_back(yfin - (int)((yp[i] - ymin)*scale));
            opts.draw_line(xpoly[i-1],ypoly[i-1],xpoly[i],ypoly[i],black);
    }

    dpts = opts;    // Share graphic elements
    
    // Converting origin and destination point patterns to pixels coordinates and its drawning
    for (i = 0; i < x1.size(); i++)
    {
            px = xini + (int)((x1[i] - xmin)*scale);
            py = yfin - (int)((y1[i] - ymin)*scale);
            opts.draw_point(px,py,black);

            px = xini + (int)((x2[i] - xmin)*scale);
            py = yfin - (int)((y2[i] - ymin)*scale);
            dpts.draw_point(px,py,black);
    }
    
    // Draw legend
    for (px=lxi; px<lxf; px++)
    {
        for (py=lyi; py<lyf; py++)
        {
       	      img(px,py,0)=(unsigned char)(255);
              img(px,py,1)=(unsigned char)(MIN((int)(255*(4.0 - 4*(lyf-py)/(double)(lyf -lyi) )/3.0),255));
              img(px,py,2)=(unsigned char)(MAX((int)(220*(1.0 - 4*(lyf-py)/(double)(lyf -lyi))),0));
        }
    }
    // Draw legend rectangle
    img.draw_line(lxi,lyi,lxi,lyf,black).draw_line(lxi,lyf,lxf,lyf,black).
        draw_line(lxf,lyf,lxf,lyi,black).draw_line(lxf,lyi,lxi,lyi,black);
           
    opts.display(visu);      // Display opts on visu window
    dpts.display(disp);      // Display dpts on disp window
    disp.resize(false);      // Avoid window resizing by user
    visu.resize(false);
    img2o = opts;             // img2 will represent polygon+points+cross

    img2d = dpts;             // img2 will represent polygon+points+cross
    
    img4 = img;
    
    cross = 5;               // height and width in pixels of the cross

    radiuso = (float)(h1*sqrt(2.0)*scale);  // Converting the radius to image scale
    radiusd = (float)(h2*sqrt(2.0)*scale);  // Converting the radius to image scale
    
    MaxZo = 0.0;    
    MaxZd = 0.0;    
    
    while (!disp.is_closed && !visu.is_closed && disp.key!=cimg::keyQ && visu.key!=cimg::keyQ &&
          disp.key!=cimg::keyESC && visu.key!=cimg::keyESC )
    {

        if (visu.mouse_x>=0 && visu.mouse_y>=0)
        { 
                img1o = img2o;             // Draw an image without circle
                x = visu.mouse_x; y = visu.mouse_y;       // Get mouse pointer coordinates
                img1o.draw_circle(x,y,radiuso,purple,(unsigned int)(RAND_MAX+1e9)).display(visu);
                // Draw a circle centered at mouse pointer
                if (visu.button)
                {
                    img2o = opts;          //Draw an image without cross
                    img = img4;
                    img2o.draw_line(x-cross,y-cross,x+cross,y+cross,red).draw_line(x-cross,y+cross,x+cross,y-cross,red).display(visu);
                    // Draw cross centered at mouse pointer
                    xcon = (x - xini)/scale + xmin;        //Convert mouse pointer coordinates to real ones
                    ycon = (yfin - y)/scale + ymin;
                    result =  kernelink(nx, ny, h1, h2, xcon, ycon, xp, xmin, xmax, yp, ymin, ymax, x1, y1, x2, y2, xgrid, ygrid, zgrid, zmax);
                   
                    if(MaxRegion){
                         MaxZo = MAX(MaxZo,zmax); // Maximum for legend is over region
                    }
                    else{
                         MaxZo = zmax;           // Maximum for legend is only for the current conditional location
                    }
                    
                    // Calculate the conditional kernel function matrix
                    for (px=xini; px<xfin; px++)
                    {
                        for (py=yini; py<yfin; py++)
                        {
                            xm = (int)((px-xini)/xincp);   //Finding the corresponding matrix cell
                            ym = (int) ((ny - 1) - (py-yini)/yincp);
                            if(zgrid[xm][ym] != -1.0)     //Change color only for pixels within polygon
                            {
                            // Heat colors: gradient from red to light yellow
                               	      img(px,py,0)=(unsigned char)(255);
	                                  img(px,py,1)=(unsigned char)(MIN((int)(255*(4.0 - 4*zgrid[xm][ym]/MaxZo)/3.0),255));
	                                  img(px,py,2)=(unsigned char)(MAX((int)(220*(1.0 - 4*zgrid[xm][ym]/MaxZo)),0));
                            }
                        }
                    }
                    // Draw legend values    
                    img.draw_text(lxi-15,lyi-20,black,0,17,1,"%G",MaxZo/(rate*rate));
                    img.draw_text(lxi-15,lyf+ 3,black,0,17,1,"0.000000000");
	
                    for (i = 1; i < xpoly.size(); i++)   // Redraw the polygon
                            img.draw_line(xpoly[i-1],ypoly[i-1],xpoly[i],ypoly[i],black);
                 img2d = img;
                 img.display(disp);           // Display the updated image                                                   
                }
        }
        
        if (disp.mouse_x>=0 && disp.mouse_y>=0)
        { 
                img1d = img2d;             // Draw an image without circle
                x = disp.mouse_x; y = disp.mouse_y;       // Get mouse pointer coordinates
                img1d.draw_circle(x,y,radiusd,purple,(unsigned int)(RAND_MAX+1e9)).display(disp);
                // Draw a circle centered at mouse pointer
                if (disp.button)
                {
                    img2d = dpts;          //Draw an image without cross
                    img = img4;
                    img2d.draw_line(x-cross,y-cross,x+cross,y+cross,red).draw_line(x-cross,y+cross,x+cross,y-cross,red).display(disp);
                    // Draw cross centered at mouse pointer
                    xcon = (x - xini)/scale + xmin;        //Convert mouse pointer coordinates to real ones
                    ycon = (yfin - y)/scale + ymin;
                    result =  kernelink(nx, ny, h2, h1, xcon, ycon, xp, xmin, xmax, yp, ymin, ymax, x2, y2, x1, y1, xgrid, ygrid, zgrid, zmax);
                   
                    if(MaxRegion){
                         MaxZd = MAX(MaxZd,zmax); // Maximum for legend is over region
                    }
                    else{
                         MaxZd = zmax;           // Maximum for legend is only for the current conditional location
                    }
                    
                    // Calculate the conditional kernel function matrix
                    for (px=xini; px<xfin; px++)
                    {
                        for (py=yini; py<yfin; py++)
                        {
                            xm = (int)((px-xini)/xincp);   //Finding the corresponding matrix cell
                            ym = (int) ((ny - 1) - (py-yini)/yincp);
                            if(zgrid[xm][ym] != -1.0)     //Change color only for pixels within polygon
                            {
                            // Heat colors: gradient from red to light yellow
                               	      img(px,py,0)=(unsigned char)(255);
	                                  img(px,py,1)=(unsigned char)(MIN((int)(255*(4.0 - 4*zgrid[xm][ym]/MaxZd)/3.0),255));
	                                  img(px,py,2)=(unsigned char)(MAX((int)(220*(1.0 - 4*zgrid[xm][ym]/MaxZd)),0));
                            }
                        }
                    }
                    // Draw legend values    
                    img.draw_text(lxi-15,lyi-20,black,0,17,1,"%G",MaxZd/(rate*rate));
                    img.draw_text(lxi-15,lyf+ 3,black,0,17,1,"0.000000000");
	
                    for (i = 1; i < xpoly.size(); i++)   // Redraw the polygon
                            img.draw_line(xpoly[i-1],ypoly[i-1],xpoly[i],ypoly[i],black);
                 img2o = img;
                 img.display(visu);           // Display the updated image                                                   
                }
        }
        
		//if (disp.is_typed(cimg::keyCTRLLEFT,cimg::keyS) || visu.is_typed(cimg::keyCTRLLEFT,cimg::keyS))
		//
		// AQUI DEVE SER COLOCADA ALGUMA FUNÇÃO QUE SALVE A MATRIZ zgrid COMO UM TEMA DE RASTER QUANDO 
		// REALIZADO UM CERTO PROCEDIMENTO, COMO APERTAR CTRL+S; INFORMAÇÕES COMO h1, h2, xcon, ycon, nx, ny,
		// QUAL TEMA É DE ORIGEM E QUAL É DE DESTINO DEVEM SER ARMAZENADAS NO NOVO TEMA; INFELIZMENTE A CRIAÇÃO
		// DESSE NOVO TEMA É EXTREMAMENTE COMPLEXA PARA SER IMPLEMENTADO EM VERSÕES INICIAIS DO PLUGIN E FOI 
		// DEIXADA PARA VERSÕES SEGUINTES

        CImgDisplay::wait(disp,visu);         // Wait for command while user do not quit routine
    }

    return(0);
}

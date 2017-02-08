/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/


//////////////////////////////////
void SemivarGraph::init()
{
	gplot_=0;
	vx_=0;
	vy_=0;
	vmx_=0;
	vmy_=0;
	vmsize_=0;
	vsize_=0;

	gplot_= new TePlotWidget(this);
}


///////////////////////////
void SemivarGraph::paintEvent( QPaintEvent * )
{
	if (gplot_)
		gplot_->copyPixmapToWindow();
}


//////////////////////////
void
SemivarGraph::resizeEvent( QResizeEvent * )
{
    int i;
	if (gplot_)
	{
		gplot_->resize(width(), height());
		gplot_->plotOnPixmap();

		gplot_->reset();
		gplot_->plotInit (5, 2, 2, 2);

		gplot_->setColorGrid (0,0,0);
		gplot_->setColorText (0,0,0);
		gplot_->setColorLine (0,0,0);
		gplot_->setColorCurve (0,0,0,0);

		gplot_->loadDirect ((int)XY_BINARY,0, vsize_, labels_, vx_, vy_);

		for(i=0; i < vsize_; i++)
		{
			gplot_->setColorSymbol (255,0,0);
			gplot_->plotSymbol(vx_[i],vy_[i],2, 5, 5);
		}

		if(vmsize_)
		{
			gplot_->setColorCurve (1,255,0,0);
			gplot_->loadDirect ((int)XY_BINARY,1, vmsize_, labels_, vmx_, vmy_);
		}

		for(i=0; i < vmsize_; i++)
		{
			gplot_->setColorSymbol (255,0,0);
			gplot_->plotSymbol(vmx_[i],vmy_[i],2, 5, 5);
		}

		gplot_->plotCurves ();
		
		gplot_->copyPixmapToWindow();
	}
}


//////////////////////////
void SemivarGraph::init( TeMatrix &mx, TeMatrix &mod )
{
	int		    i,j;

	vsize_ = mx.Nrow();

	if(vx_) { delete[] vx_; vx_=0;}
    vx_ = new double[mx.Nrow()];

	if(vy_) { delete[] vy_; vy_=0;}
    vy_ = new double[mx.Nrow()];

	if(vmx_) { delete[] vmx_; vmx_=0;}
    vmx_ = new double[mod.Nrow()];

	if(vmy_) { delete[] vmy_; vmy_=0;}
    vmy_ = new double[mod.Nrow()];

	xmin_ = ymin_ = 0;
	xmax_ = ymax_ = -TeMAXFLOAT;

	strcpy(labels_.xaxis,"h");
	strcpy(labels_.yaxis,"Y(h)");
	strcpy(labels_.title,"");
	labels_.curve[0][0] = '\0';
	labels_.curve[1][0] = '\0';

	j=0;
	for (i=0; i<mx.Nrow(); ++i)
	{
		vx_[i] = mx(i,j);
		vy_[i] = mx(i,j+1);

		xmax_=MAX(vx_[i],xmax_);
		ymax_=MAX(vy_[i],ymax_);
	}
	j=0;
	for (i=0; i<mod.Nrow(); ++i)
	{
		vmx_[i] = mod(i,j);
		vmy_[i] = mod(i,j+1);
	}


	QPainter p(gplot_->getPixmap());
	QBrush b (white);
	p.fillRect(0, 0, gplot_->getWidth(), gplot_->getHeight(), b ); 

	gplot_->plotOnPixmap();
	gplot_->reset();
	gplot_->plotInit (5, 2, 2, 2);

	adjustAxis();
	
	gplot_->setColorGrid (0,0,0);
	gplot_->setColorText (0,0,0);
	gplot_->setColorLine (0,0,0);
	gplot_->setColorCurve (0,0,0,0);

	gplot_->loadDirect ((int)XY_BINARY,0, mx.Nrow(), labels_, vx_, vy_);

	for(i=0; i < mx.Nrow(); i++)
	{
		gplot_->setColorSymbol (0,0,0);
		gplot_->plotSymbol(vx_[i],vy_[i],2, 5, 5);
	}

	if(vmsize_)
	{
		gplot_->setColorCurve (1,255,0,0);
		gplot_->loadDirect ((int)XY_BINARY,1, vmsize_, labels_, vmx_, vmy_);
	}

	for(i=0; i < vmsize_; i++)
	{
		gplot_->setColorSymbol (255,0,0);
		gplot_->plotSymbol(vmx_[i],vmy_[i],2, 5, 5);
	}

	gplot_->plotCurves ();

	gplot_->copyPixmapToWindow();
}


void SemivarGraph::adjustAxis()
{
	double ddr, dr, d, fator, delta, range;
	double xmin, xmax, ymin, ymax;
	double rx; // number of values displayed on x axis
	double ry; // number of values displayed on y axis

// calculate xmin, xmax, rx

	range = xmax_ - xmin_;
	ddr = 5.;
	dr = range / ddr;
	d = log10(dr);
	if(d >= 0)
		fator = pow(10., (double)((int) d));
	else
		fator = pow(10., (double)((int) (d - 1.)));

	delta = (double)((int)(dr / fator + 1.));
	delta *= fator;

	xmin = (double)((int)(xmin_ / fator)) * fator;
	xmax = xmin + ddr * delta;

	if(xmax < xmax_)
	{
		ddr = 6.;
		xmax = xmin + ddr * delta;
	}
	rx = ddr;

	range = ymax_ - ymin_;
	ddr = 5.;
	dr = range / ddr;
	d = log10(dr);
	if(d >= 0)
		fator = pow(10., (double)((int) d));
	else
		fator = pow(10., (double)((int) (d - 1.)));

	delta = (double)((int)(dr / fator + 1.));
	delta *= fator;

	ymin = (double)((int)(ymin_ / fator)) * fator;
	ymax = ymin + ddr * delta;

	if(ymax < ymax_)
	{
		ddr = 6.;
		ymax = ymin + ddr * delta;
	}
	ry = ddr;

	gplot_->setGridSize((int)rx, (int)ry, 5, 5);
	gplot_->setScale(xmin, xmax, ymin, ymax);
}


void SemivarGraph::setModSize( int size )
{
	vmsize_=size;
}

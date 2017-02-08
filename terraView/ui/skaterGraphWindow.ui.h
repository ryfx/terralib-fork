#include <qpopupmenu.h>

void SkaterGraphWindow::init()
{
	vecx_.clear();
	vecy_.clear();
	
	gplot_ = new TePlotWidget( this, "gplot" );
	
	QColor white(255, 255, 255);
	setMouseTracking(false);
}


void SkaterGraphWindow::show()
{	
	QMainWindow::show();	
	plotGraphic();
}


void SkaterGraphWindow::plotGraphic()
{
	int i;
	Labels label;

	try
	{
		xmin_ = ymin_ = TeMAXFLOAT;
		xmax_ = ymax_ = -TeMAXFLOAT;

		int tamX = vecx_.size();
		int tamY = vecy_.size();
	
		for(i=0; i<tamX; i++)
		{
			xmin_ = MIN(xmin_, vecx_[i]);
			xmax_ = MAX(xmax_, vecx_[i]);
		}

		for(i=0; i<tamY; i++)
		{
			ymin_ = MIN(ymin_, vecy_[i]);
			ymax_ = MAX(ymax_, vecy_[i]);
		}
		
		if(xmin_ > xmax_ && ymin_ > ymax_)
			return;
		if(tamX==0 || tamY==0)
			return;

		gplot_->reset();
		gplot_->setScale(xmin_, xmax_, ymin_, ymax_); //escala - mínimo x, max x, min y, max y
		gplot_->setGridSize(tamX-1, tamY-1, 1, 1);	 //num. divisões (maior x, maior y, menor x, menor y)

		resize(450, 450);
		gplot_->resize(width(), height());

		gplot_->plotInit(5, 2, 2, 2);
		gplot_->setColorText(0, 0, 160);
		gplot_->setColorGrid (128, 0, 128);
		gplot_->setColorLine (255, 0, 255);
		gplot_->setColorCurve (0, 50, 110, 230);
		gplot_->setColorSymbol (50, 110, 230);

		strcpy(label.title, "Queda de Desvios");
		strcpy(label.xaxis, "Numero de Grupos");
		strcpy(label.yaxis, "Desvio");

		strcpy(label.curve[0], "");

		Real_Vector nx = new Real[tamX];
		Real_Vector ny = new Real[tamY];

		if(nx == NULL || ny == NULL)
			return;

		for(i=0; i<tamX; i++)
			nx[i] = vecx_[i];
		
		for(i=0; i<tamY; i++)
			ny[i] = vecy_[i];
					
		gplot_->loadDirect(XY_BINARY, 0, tamX, label, nx, ny);
		gplot_->plot();
	}
	catch(...)
	{
		gplot_->reset();
	}
}


void SkaterGraphWindow::destroy()
{
	delete gplot_;
}


void SkaterGraphWindow::dataSet( vector<double> & setX, vector<double> & setY )
{
	vecx_ = setX;
	vecy_ = setY;
}



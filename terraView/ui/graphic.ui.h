/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright © 2001-2007 INPE and Tecgraf/PUC-Rio.
This file is part of TerraView. TerraView is free software; you can redistribute it 
and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

You should have received a copy of the GNU General Public License
along with TerraView.
The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The software provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use of
this program and its documentation.
*************************************************************************************/

#include <terraViewBase.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <TeQwtPlot.h>
#include <TeQwtPlotZoomer.h>
#include <TeQwtPlotCurve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_printfilter.h>
#include <qprinter.h>
#include <qregexp.h>
#include <qfont.h>
#include <help.h>
#include <graphicConfigurationWindow.h>
#include <TeExternalTheme.h>
#include <TeDatabaseUtils.h>
#include<vector>
#include<map>

#include <TeThemeFunctions.h>

using namespace std;

void GraphicWindow::init()
{
	help_ = 0;
	theme_ = 0;
	x_ = NULL;
	y_ = NULL;
	nx_ = NULL;
	ny_ = NULL;
	mx_ = NULL;
	my_ = NULL;
	ITMAX_ = 100;
	EPS_ = 3.0e-7;
	zoomInit_ = true;

	QWidget *ws = (QWidget*)parent();
	mainWindow_ = (TerraViewBase*)ws->parent();

	plot_ = new TeQwtPlot(this);
    plot_->setMargin(5);

    // grid 
    plotGrid_ = new QwtPlotGrid;
    plotGrid_->enableXMin(true);
    plotGrid_->enableYMin(true);
    plotGrid_->attach(plot_);

	marker_ = new QwtPlotMarker();
	markerLL_ = new QwtPlotMarker();
	markerLH_ = new QwtPlotMarker();
	markerHL_ = new QwtPlotMarker();
	markerHH_ = new QwtPlotMarker();
    marker_->attach(plot_);
    markerLL_->attach(plot_);
    markerLH_->attach(plot_);
    markerHL_->attach(plot_);
    markerHH_->attach(plot_);

	setCentralWidget(plot_);
	plot_->setCanvasBackground(QColor(Qt::white));

	QToolBar *toolBar = new QToolBar(this);
	QColor qc(220,220,220);
	toolBar->setPaletteBackgroundColor(qc);

    promoteToolButton_ = new QToolButton(toolBar);
	promoteToolButton_->setTextLabel(tr("Promote"));
    promoteToolButton_->setToggleButton(true);
    promoteToolButton_->setIconSet( QIconSet( QPixmap::fromMimeSource( "promote.bmp" ) ) );
    promoteToolButton_->setUsesTextLabel(true);

	bool statusGrid = mainWindow_->getGrid()->autoPromote_;
	promoteToolButton_->setOn(statusGrid);
    zoomToolButton_= new QToolButton(toolBar);
	zoomToolButton_->setTextLabel(tr("Zoom"));
    zoomToolButton_->setIconSet( QIconSet( QPixmap::fromMimeSource( "zoomCursor.bmp" ) ) );
    zoomToolButton_->setToggleButton(true);
    zoomToolButton_->setUsesTextLabel(true);

	previousToolButton_ = new QToolButton(toolBar);
	previousToolButton_->setTextLabel(tr("Previous"));
    previousToolButton_->setIconSet( QIconSet( QPixmap::fromMimeSource( "previousDisplay.bmp" ) ) );
    previousToolButton_->setUsesTextLabel(true);
	previousToolButton_->setEnabled(false);

    nextToolButton_ = new QToolButton(toolBar);
	nextToolButton_->setTextLabel(tr("Next"));
    nextToolButton_->setIconSet( QIconSet( QPixmap::fromMimeSource( "nextDisplay.bmp" ) ) );
    nextToolButton_->setUsesTextLabel(true);
	nextToolButton_->setEnabled(false);

    QToolButton* configurationButton = new QToolButton(toolBar);
	configurationButton->setTextLabel(tr("Config"));
    configurationButton->setIconSet( QIconSet( QPixmap::fromMimeSource( "graphicConfig.bmp" ) ) );
    configurationButton->setUsesTextLabel(true);

	QToolButton* printButton = new QToolButton(toolBar);
	printButton->setTextLabel(tr("Print"));
    printButton->setIconSet( QIconSet( QPixmap::fromMimeSource( "print.bmp" ) ) );
    printButton->setUsesTextLabel(true);

	QToolButton* helpButton = new QToolButton(toolBar);
	helpButton->setTextLabel(tr("Help"));
    helpButton->setIconSet( QIconSet( QPixmap::fromMimeSource( "help.bmp" ) ) );
    helpButton->setUsesTextLabel(true);

	plot_->replot();

    zoomer_ = new TeQwtPlotZoomer (QwtPlot::xBottom, QwtPlot::yLeft,
        QwtPicker::DragSelection, QwtPicker::AlwaysOff, plot_->canvas());
    zoomer_->setRubberBandPen(QColor(Qt::green));

	dataCurve_ = new TeQwtPlotCurve("");
   // Attach (don't copy) data. Both curves use the same x array.
    dataCurve_->attach(plot_);
 
	dataSelCurve_ = new TeQwtPlotCurve("");
   // Attach (don't copy) data. Both curves use the same x array.
    dataSelCurve_->attach(plot_);

	// Histogram
	histogramPen_.setColor(QColor(Qt::darkBlue));
	histogramPen_.setStyle(Qt::SolidLine);
	histogramPen_.setWidth(0);

	// Raster Histogram
	rasterHistogramPen_.setColor(QColor(Qt::darkBlue));
	rasterHistogramPen_.setStyle(Qt::SolidLine);
	rasterHistogramPen_.setWidth(0);

	rasterHistogramBrush_.setColor(QColor(0, 0, 150));
	rasterHistogramBrush_.setStyle(Qt::SolidPattern);

	// Dispersion
	dispersionSymbol_ = QwtSymbol(QwtSymbol::Triangle, QBrush(QColor(220, 0, 0)), QPen(QColor(220, 0, 0)), QSize(6,6));

	// Normal Probability
	normalCurvePen_.setColor(QColor(Qt::red));
	normalCurvePen_.setStyle(Qt::SolidLine);
	normalCurvePen_.setWidth(0);

	normalProbCurvePen_.setColor(QColor(Qt::green));
	normalProbCurvePen_.setStyle(Qt::SolidLine);
	normalProbCurvePen_.setWidth(0);

	// Semivariogram
	semivarCurvePen_.setColor(QColor(Qt::red));
	semivarCurvePen_.setStyle(Qt::SolidLine);
	semivarCurvePen_.setWidth(0);

	semivarAdjustCurvePen_.setColor(QColor(Qt::green));
	semivarAdjustCurvePen_.setStyle(Qt::SolidLine);
	semivarAdjustCurvePen_.setWidth(0);

	// Skater skaterPen_
	skaterPen_.setColor(QColor(Qt::red));
	skaterPen_.setStyle(Qt::SolidLine);
	skaterPen_.setWidth(0);

	// Grid
    horizontalMajorVisible_ = true;
    horizontalMinorVisible_ = true;
    verticalMajorVisible_ = true;
	verticalMinorVisible_ = true;
    majorLinePen_.setColor(QColor(Qt::magenta));
	majorLinePen_.setStyle(Qt::SolidLine);
	majorLinePen_.setWidth(0);
    minorLinePen_.setColor(QColor(Qt::blue));
	minorLinePen_.setStyle(Qt::DotLine);
	minorLinePen_.setWidth(0);
    backgroundColor_.setRgb(255, 255, 255);

    plotGrid_->setMajPen(majorLinePen_);
    plotGrid_->setMinPen(minorLinePen_);
	plotGrid_->enableY(horizontalMajorVisible_);
	plotGrid_->enableYMin(horizontalMinorVisible_);
	plotGrid_->enableX(verticalMajorVisible_);
	plotGrid_->enableXMin(verticalMinorVisible_);

	// Font
    horizontalAxisFont_ = plot_->axisFont(QwtPlot::xBottom);
    verticalAxisFont_ = plot_->axisFont(QwtPlot::yLeft);

	connect( mainWindow_, SIGNAL( graphicCursorActivated(bool) ), this, SLOT(graphicCursorActivatedSlot(bool)));
	connect( zoomToolButton_, SIGNAL( toggled(bool) ), this, SLOT(zoomToggledSlot(bool)));
	connect( plot_->cursor(), SIGNAL( mousePressedSignal(int,int,QwtDoubleRect) ),
		     this, SLOT(mousePressedSlot(int,int,QwtDoubleRect)));
	connect( previousToolButton_, SIGNAL( clicked() ), this, SLOT(previousZoomSlot()));
	connect( nextToolButton_, SIGNAL( clicked() ), this, SLOT(nextZoomSlot()));
	connect( zoomer_, SIGNAL( enablePreviousZoomSignal(bool) ), this, SLOT(previousZoomEnableSlot(bool)));
	connect( zoomer_, SIGNAL( enableNextZoomSignal(bool) ), this, SLOT(nextZoomEnableSlot(bool)));
	connect( promoteToolButton_, SIGNAL( toggled(bool) ), this, SLOT(promoteToggledSlot(bool)));
	connect( printButton, SIGNAL( clicked() ), this, SLOT(print()));
	connect( configurationButton, SIGNAL( clicked() ), this, SLOT(configurationSlot()));
	connect( helpButton, SIGNAL( clicked() ), this, SLOT(helpSlot()));

	zoomToggledSlot(zoomToolButton_->isOn());
}

float GraphicWindow::erf( float x )
{
	return x < 0.0 ? -gammp(0.5, x*x) : gammp(0.5,x*x);
}


float GraphicWindow::gammln( float xx )
{
	double x, tmp, ser;
	static double cof[6]= {76.18009173, -86.50532033, 24.01409822,
		-1.231739516, 0.120858003e-2, -0.536382e-5};
	int j;

	x = xx - 1.0;
	tmp = x + 5.5;
	tmp -= (x+0.5)*log(tmp);
	ser = 1.0;

	for (j=0; j<=5; j++)
	{
		x += 1.0;
		ser += cof[j]/x;
	}
	return (float)(-tmp + log(2.50662827465*ser));
}


float GraphicWindow::gammp( float a, float x )
{
	float gamser, gammcf, gln;

	if (x < 0.0 || a <= 0.0)
		printf ("Invalid arguments in GAMMP routine");

	if (x < (a+1.0))
	{
		gser (&gamser, a, x, &gln);
		return gamser;
	}
	else
	{
		gcf (&gammcf, a, x, &gln);
		return (float)(1.0 - gammcf);
	}
}


void GraphicWindow::gcf( float * gammcf, float a, float x, float * gln )
{
	int n;
	float gold=0.0, g, fac=1.0, b1=1.0;
	float b0=0.0, anf, ana, an, a1, a0=1.0;
	
	*gln=gammln(a);

	a1=x;

	for(n=1; n<= ITMAX_; n++)
	{
		an = (float) n;
		ana = an - a;
		a0 = (a1+a0*ana)*fac;
		b0 = (b1+b0*ana)*fac;
		anf = an*fac;
		a1 = x*a0+anf*a1;
		b1 = x*b0+anf*b1;
		if (a1)
		{
			fac = (float)(1.0/a1);
			g = b1*fac;
			if (fabs((g-gold)/g) < EPS_)
			{
				*gammcf = (float)(exp (-x+a*log(x)-(*gln))*g);
				return;
			}
			gold = g;
		}
	}
	printf ("Variable \"a\" too large, ITMAX too small in the GCF routine");
}


void GraphicWindow::getHistogramValues()
{
//	put the frequency into vy_
//  put the central value into vx_

	vx_.clear();
	vy_.clear();
	vmx_.clear(); // for pointing objects
	vmy_.clear(); // for pointing objects

	double	v;
	int		i;
	vector<double>	h;

	string	atr = attributeX_;
	int func = atr.find("(");

	if(theme_->getTheme()->getProductId() == TeTHEME)
	{
		string query = "SELECT " + atr + ((TeTheme*)theme_->getTheme())->sqlGridFrom();

		if(selectedObjects_ == TeAll) {}
		else if(selectedObjects_ == TeSelectedByQuery)
		{
			query += " WHERE (grid_status >= 2 OR (grid_status is null AND c_object_status >= 2))";
		}
		else if(selectedObjects_ == TeNotSelectedByQuery)
		{
			query += " WHERE (grid_status < 2  OR (grid_status is null AND c_object_status < 2))";
		}
		else if(selectedObjects_ == TeGrouped)
			query += " WHERE (c_legend_id <> 0)";
		else if(selectedObjects_ == TeNotGrouped)
			query += " WHERE (c_legend_id = 0)";

		if(func >= 0)
			query += " GROUP BY " + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_id";

		TeDatabasePortal* portal = db_->getPortal();
		if(portal->query(query) == false)
		{
			delete portal;
			return;
		}

		double min = TeMAXFLOAT;
		double max = -TeMAXFLOAT;
		long double	soma = 0.;
		long double sm2 = 0.;
		bool bb = portal->fetchRow(0);
		while(bb)
		{
			string sv = portal->getData(0);
			if(sv.empty() == false)
			{
				v = atof(sv.c_str());
				h.push_back(v);
				min = MIN(min, v);
				max = MAX(max, v);
				soma += v;
				sm2 += (v * v);
			}
			bb = portal->fetchRow();
		}
		if(h.size() == 0)
		{
			delete portal;
			return;
		}

		mean_ = soma /(double)h.size();
		double var = (sm2 /(double)h.size()) - (mean_ * mean_);
		stdev_ = sqrt(var);

		double step = (max - min) / (double)slices_;
		vector<double>::iterator it = h.begin();

		for(i=0; i<slices_; i++)
		{
			vy_.push_back(0);
			vx_.push_back(min + i*step + step/2.);
		}

		while(it != h.end())
		{
			double d = *it;
			int a = (int)((d - min)/step);
			if(a >= (int)vy_.size())
				a = vy_.size() - 1;
			vy_[a] += 1;
			it++;
		}
		xmin_ = min;
		xmax_ = max;

		ymin_ = TeMAXFLOAT;
		ymax_ = -TeMAXFLOAT;
		it = vy_.begin();
		while(it != vy_.end())
		{
			v = *it;
			ymax_ = MAX(ymax_, v);
			it++;
		}
		ymin_ = 0;
		getNormalValues();

		string q = "SELECT " + atr + ((TeTheme*)theme_->getTheme())->sqlGridFrom();

		if(selectedObjects_ == TeAll)
		{
			q += " WHERE (grid_status = 1 OR grid_status = 3";
			q += " OR (grid_status is null AND (c_object_status = 1 OR c_object_status = 3)))";
		}
		else if(selectedObjects_ == TeSelectedByQuery)
		{
			q += " WHERE (grid_status = 3  OR (grid_status is null AND c_object_status = 3))";
		}
		else if(selectedObjects_ == TeNotSelectedByQuery)
		{
			q += " WHERE (grid_status = 1  OR (grid_status is null AND c_object_status = 1))";
		}
		else if(selectedObjects_ == TeGrouped)
		{
			q += " WHERE (c_legend_id <> 0 AND (grid_status = 1 OR grid_status = 3";
			q += " OR (grid_status is null AND (c_object_status = 1 OR c_object_status = 3))))";
		}
		else if(selectedObjects_ == TeNotGrouped)
		{
			q += " WHERE (c_legend_id = 0 AND (grid_status = 1 OR grid_status = 3";
			q += " OR (grid_status is null AND (c_object_status = 1 OR c_object_status = 3))))";
		}
		if(func >= 0)
			q += " GROUP BY " + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_id";

		portal->freeResult();
		if(portal->query(q) == false)
		{
			delete portal;
			return;
		}

		h.clear();
		bb = portal->fetchRow(0);
		while(bb)
		{
			string sv = portal->getData(0);
			if(sv.empty() == false)
			{
				v = atof(sv.c_str());
				h.push_back(v);
			}
			bb = portal->fetchRow();
		}
		delete portal;

		for(i=0; i<slices_; i++)
		{
			vmy_.push_back(0);
			vmx_.push_back(min + i*step + step/2.);
		}

		it = h.begin();
		while(it != h.end())
		{
			double d = *it;
			int a = (int)((d - min)/step);
			if(a >= (int)vmy_.size())
				a = vmy_.size() - 1;
			vmy_[a] += 1;
			it++;
		}
	}
	else if(theme_->getTheme()->getProductId() == TeEXTERNALTHEME)
	{
		TeTheme* theme = (TeTheme*)theme_->getTheme();
		TeTheme* rTheme = ((TeExternalTheme*)theme)->getRemoteTheme();
		string query = "SELECT " + atr + rTheme->sqlGridFrom();
		string CT = rTheme->collectionTable();
		string CA = rTheme->collectionAuxTable();
		TeDatabasePortal* portal = rTheme->layer()->database()->getPortal();
		double min = TeMAXFLOAT;
		double max = -TeMAXFLOAT;
		long double	soma = 0.;
		long double sm2 = 0.;

		vector<string> itenVec = getItems(theme, selectedObjects_);
    
    std::vector< std::string >::iterator it_begin = itenVec.begin();
    std::vector< std::string >::iterator it_end = itenVec.end();
    
		vector<string> sv = generateItemsInClauses(it_begin, it_end, theme);

		vector<string>::iterator sit;
		for(sit=sv.begin(); sit!=sv.end(); ++sit)
		{
			string sel = query + " WHERE " + CA + ".unique_id IN " + *sit;
			if(func >= 0)
				sel += " GROUP BY " + CT + ".c_object_id";

			portal->freeResult();
			if(portal->query(sel) == false)
			{
				delete portal;
				return;
			}

			while(portal->fetchRow())
			{
				string sv = portal->getData(0);
				if(sv.empty() == false)
				{
					v = atof(sv.c_str());
					h.push_back(v);
					min = MIN(min, v);
					max = MAX(max, v);
					soma += v;
					sm2 += (v * v);
				}
			}
		}
		if(h.size() == 0)
		{
			delete portal;
			return;
		}

		mean_ = soma /(double)h.size();
		double var = (sm2 /(double)h.size()) - (mean_ * mean_);
		stdev_ = sqrt(var);

		double step = (max - min) / (double)slices_;
		vector<double>::iterator it = h.begin();

		for(i=0; i<slices_; i++)
		{
			vy_.push_back(0);
			vx_.push_back(min + i*step + step/2.);
		}

		while(it != h.end())
		{
			double d = *it;
			int a = (int)((d - min)/step);
			if(a >= (int)vy_.size())
				a = vy_.size() - 1;
			vy_[a] += 1;
			it++;
		}
		xmin_ = min;
		xmax_ = max;

		ymin_ = TeMAXFLOAT;
		ymax_ = -TeMAXFLOAT;
		it = vy_.begin();
		while(it != vy_.end())
		{
			v = *it;
			ymax_ = MAX(ymax_, v);
			it++;
		}
		ymin_ = 0;
		getNormalValues();

		string q = "SELECT " + atr + rTheme->sqlGridFrom();
		string where;

		if(selectedObjects_ == TeAll)
		{
			where = " WHERE (grid_status = 1 OR grid_status = 3";
			where += " OR (grid_status is null AND (c_object_status = 1 OR c_object_status = 3)))";
		}
		else if(selectedObjects_ == TeSelectedByQuery)
		{
			where = " WHERE (grid_status = 3  OR (grid_status is null AND c_object_status = 3))";
		}
		else if(selectedObjects_ == TeNotSelectedByQuery)
		{
			where = " WHERE (grid_status = 1  OR (grid_status is null AND c_object_status = 1))";
		}
		else if(selectedObjects_ == TeGrouped)
		{
			where = " WHERE (c_legend_id <> 0 AND (grid_status = 1 OR grid_status = 3";
			where += " OR (grid_status is null AND (c_object_status = 1 OR c_object_status = 3))))";
		}
		else if(selectedObjects_ == TeNotGrouped)
		{
			where = " WHERE (c_legend_id = 0 AND (grid_status = 1 OR grid_status = 3";
			where += " OR (grid_status is null AND (c_object_status = 1 OR c_object_status = 3))))";
		}

		sv = generateItemsInClauseVec(theme, where);

		h.clear();
		for(sit=sv.begin(); sit!=sv.end(); ++sit)
		{
			string sel = q + " WHERE " + CA + ".unique_id IN " + *sit;
			if(func >= 0)
				sel += " GROUP BY " + CT + ".c_object_id";

			portal->freeResult();
			if(portal->query(sel) == false)
			{
				delete portal;
				return;
			}

			while(portal->fetchRow())
			{
				string sv = portal->getData(0);
				if(sv.empty() == false)
				{
					v = atof(sv.c_str());
					h.push_back(v);
				}
			}
		}
		delete portal;

		for(i=0; i<slices_; i++)
		{
			vmy_.push_back(0);
			vmx_.push_back(min + i*step + step/2.);
		}

		it = h.begin();
		while(it != h.end())
		{
			double d = *it;
			int a = (int)((d - min)/step);
			if(a >= (int)vmy_.size())
				a = vmy_.size() - 1;
			vmy_[a] += 1;
			it++;
		}
	}
}


void GraphicWindow::getNormalValues()
{
	vnx_.clear();
	vny_.clear();

	double xdots = 300.;
	double k = (xmax_ - xmin_) / xdots;
	int i;
	for (i=1; i <= (int)xdots; i++)
	{
		double v = (double)i * k + xmin_;
		vnx_.push_back(v);
	}

	double classampl = (xmax_ - xmin_) / (double)slices_;
	double pi = 4.*atan((double)1.);
	double cont = (double)vx_.size();

	double b = (cont / (sqrt(2.*pi)*stdev_)) * classampl;

	for (i=0; i < xdots; i++)
	{
		double v = vnx_[i];
		double a = -0.5 * (v - mean_)*(v - mean_) / (stdev_ * stdev_);
		double c = b * exp(a);
		vny_.push_back(c);
	}
}


void GraphicWindow::getProbNormalValues()
{
	vx_.clear();
	vy_.clear();
	vnx_.clear();
	vny_.clear();
	long double	soma = 0.;
	long double sm2 = 0.;

	double	v;
	unsigned int i;

	string	atr = attributeX_;
	int func = atr.find("(");

	if(theme_->getTheme()->getProductId() == TeTHEME)
	{
		string query = "SELECT " + atr + ((TeTheme*)theme_->getTheme())->sqlGridFrom();

		if(selectedObjects_ == TeAll) {}
		else if(selectedObjects_ == TeSelectedByQuery)
		{
			query += " WHERE (grid_status >= 2  OR (grid_status is null AND c_object_status >= 2))";
		}
		else if(selectedObjects_ == TeNotSelectedByQuery)
		{
			query += " WHERE (grid_status < 2  OR (grid_status is null AND c_object_status < 2))";
		}
		else if(selectedObjects_ == TeGrouped)
			query += " WHERE (c_legend_id <> 0)";
		else if(selectedObjects_ == TeNotGrouped)
			query += " WHERE (c_legend_id = 0)";

		if(func >= 0)
			query += " GROUP BY " + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_id";
		TeDatabasePortal*	portal = db_->getPortal();
		if(portal->query(query) == false)
		{
			delete portal;
			return;
		}

		bool bb = portal->fetchRow(0);
		while(bb)
		{
			string sv = portal->getData(0);
			if(sv.empty() == false)
			{
				v = atof(sv.c_str());
				vx_.push_back(v);
				soma += v;
				sm2 += (v * v);
			}
			bb = portal->fetchRow();
		}
		delete portal;
	}
	else if(theme_->getTheme()->getProductId() == TeEXTERNALTHEME)
	{
		TeTheme* theme = (TeTheme*)theme_->getTheme();
		TeTheme* rTheme = ((TeExternalTheme*)theme)->getRemoteTheme();
		string CT = rTheme->collectionTable();
		string CA = rTheme->collectionAuxTable();
		TeDatabasePortal*	portal = rTheme->layer()->database()->getPortal();

		string query = "SELECT " + atr + rTheme->sqlGridFrom();

		string where;
		if(selectedObjects_ == TeAll) {}
		else if(selectedObjects_ == TeSelectedByQuery)
		{
			where = " WHERE (grid_status >= 2  OR (grid_status is null AND c_object_status >= 2))";
		}
		else if(selectedObjects_ == TeNotSelectedByQuery)
		{
			where = " WHERE (grid_status < 2  OR (grid_status is null AND c_object_status < 2))";
		}
		else if(selectedObjects_ == TeGrouped)
			where = " WHERE (c_legend_id <> 0)";
		else if(selectedObjects_ == TeNotGrouped)
			where = " WHERE (c_legend_id = 0)";

		vector<string> sv = generateItemsInClauseVec(theme, where);
		vector<string>::iterator sit;

		for(sit=sv.begin(); sit!=sv.end(); ++sit)
		{
			string sel = query + " WHERE " + CA + ".unique_id IN " + *sit;

			if(func >= 0)
				sel += " GROUP BY " + CT + ".c_object_id";

			if(portal->query(sel) == false)
			{
				delete portal;
				return;
			}

			while(portal->fetchRow())
			{
				string sv = portal->getData(0);
				if(sv.empty() == false)
				{
					v = atof(sv.c_str());
					vx_.push_back(v);
					soma += v;
					sm2 += (v * v);
				}
			}
			portal->freeResult();
		}
		delete portal;
	}
	unsigned int cont = vx_.size();
	mean_ = soma /(double)cont;
	double var = (sm2 /(double)cont) - (mean_ * mean_);
	stdev_ = sqrt(var);

	// Normalize the variable selected
	for(i=0; i<vx_.size(); i++)
	{
		v = vx_[i];
		v = (v - mean_) / stdev_;
		vx_[i] = v;
	}
	sort(vx_.begin(), vx_.end());

	// Fill the vector of the probability
	for(i = 0; i < cont; i++)
	{
		double v = vx_[i];
		if (v > 0.)
			v = 0.5 + (erf ((float)(v/sqrt(2.)))/2.);
		else
		{
			v = -v;
			v = 0.5 - (erf ((float)(v/sqrt(2.)))/2.);
		}
		vy_.push_back(v);
	}

	for (i = 0; i < cont; i++)
	{
		v = ((double)i+(double)1.0)/(double)cont;
		vx_[i] = v;
		vnx_.push_back(v);
		vny_.push_back(v);
	}
	xmin_ = 0.;
	ymin_ = 0.;
	xmax_ = 1.;
	ymax_ = 1.;
}


void GraphicWindow::getScatterValues()
{
	vx_.clear();
	vy_.clear();

	double	a, b;
	string	atrx = attributeX_;
	string	atry = attributeY_;
	string	atr = atrx + ", " + atry;
	int func = atrx.find("(");

	if(theme_->getTheme()->getProductId() == TeTHEME)
	{
		string query = "SELECT " + atr + ((TeTheme*)theme_->getTheme())->sqlGridFrom();

		if(selectedObjects_ == TeAll) {}
		else if(selectedObjects_ == TeSelectedByQuery)
		{
			query += " WHERE (grid_status >= 2  OR (grid_status is null AND c_object_status >= 2))";
		}
		else if(selectedObjects_ == TeNotSelectedByQuery)
		{
			query += " WHERE (grid_status < 2  OR (grid_status is null AND c_object_status < 2))";
		}
		else if(selectedObjects_ == TeGrouped)
			query += " WHERE (c_legend_id <> 0)";
		else if(selectedObjects_ == TeNotGrouped)
			query += " WHERE (c_legend_id = 0)";

		if(func >= 0)
			query += " GROUP BY " + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_id";
		TeDatabasePortal *portal = db_->getPortal();
		if(portal->query(query) == false)
		{
			delete portal;
			return;
		}

		xmin_ = ymin_ = TeMAXFLOAT;
		xmax_ = ymax_ = -TeMAXFLOAT;
		bool bb = portal->fetchRow(0);
		while(bb)
		{
			string sa = portal->getData(0);
			string sb = portal->getData(1);
			if(sa.empty()==false && sb.empty()==false)
			{
				a = atof(sa.c_str());
				b = atof(sb.c_str());
				vx_.push_back(a);
				vy_.push_back(b);
				xmin_ = MIN(xmin_, a);
				xmax_ = MAX(xmax_, a);
				ymin_ = MIN(ymin_, b);
				ymax_ = MAX(ymax_, b);
			}
			bb = portal->fetchRow();
		}
		delete portal;
	}
	else if(theme_->getTheme()->getProductId() == TeEXTERNALTHEME)
	{
		TeTheme* theme = (TeTheme*)theme_->getTheme();
		TeTheme* rTheme = ((TeExternalTheme*)theme)->getRemoteTheme();
		string CT = rTheme->collectionTable();
		string CA = rTheme->collectionAuxTable();
		TeDatabasePortal*	portal = rTheme->layer()->database()->getPortal();

		string query = "SELECT " + atr + rTheme->sqlGridFrom();

		string where;
		if(selectedObjects_ == TeAll) {}
		else if(selectedObjects_ == TeSelectedByQuery)
		{
			where = " WHERE (grid_status >= 2  OR (grid_status is null AND c_object_status >= 2))";
		}
		else if(selectedObjects_ == TeNotSelectedByQuery)
		{
			where = " WHERE (grid_status < 2  OR (grid_status is null AND c_object_status < 2))";
		}
		else if(selectedObjects_ == TeGrouped)
			where = " WHERE (c_legend_id <> 0)";
		else if(selectedObjects_ == TeNotGrouped)
			where = " WHERE (c_legend_id = 0)";

		vector<string> sv = generateItemsInClauseVec(theme, where);
		vector<string>::iterator sit;

		xmin_ = ymin_ = TeMAXFLOAT;
		xmax_ = ymax_ = -TeMAXFLOAT;
		for(sit=sv.begin(); sit!=sv.end(); ++sit)
		{
			string sel = query + " WHERE " + CA + ".unique_id IN " + *sit;

			if(func >= 0)
				sel += " GROUP BY " + CT + ".c_object_id";

			if(portal->query(sel) == false)
			{
				delete portal;
				return;
			}

			while(portal->fetchRow())
			{
				string sa = portal->getData(0);
				string sb = portal->getData(1);
				if(sa.empty()==false && sb.empty()==false)
				{
					a = atof(sa.c_str());
					b = atof(sb.c_str());
					vx_.push_back(a);
					vy_.push_back(b);
					xmin_ = MIN(xmin_, a);
					xmax_ = MAX(xmax_, a);
					ymin_ = MIN(ymin_, b);
					ymax_ = MAX(ymax_, b);
				}
			}
			portal->freeResult();
		}
		delete portal;	
	}
}


void GraphicWindow::getSelTabScatterValues()
{
	vnx_.clear();
	vny_.clear();

	double	a, b;
	string	atrx = attributeX_;
	string	atry = attributeY_;
	string	atr = atrx + ", " + atry;
	int func = atrx.find("(");

	if(theme_->getTheme()->getProductId() == TeTHEME)
	{
		string query = "SELECT " + atr + ((TeTheme*)theme_->getTheme())->sqlGridFrom();

		if(selectedObjects_ == TeAll)
		{
			query += " WHERE (grid_status = 1 OR grid_status = 3";
			query += " OR (grid_status is null AND (c_object_status = 1 OR c_object_status = 3)))";
		}
		else if(selectedObjects_ == TeSelectedByQuery)
		{
			query += " WHERE (grid_status = 3  OR (grid_status is null AND c_object_status = 3))";
		}
		else if(selectedObjects_ == TeNotSelectedByQuery)
		{
			query += " WHERE ((grid_status = 1  OR (grid_status is null AND c_object_status = 1)))";
		}
		else if(selectedObjects_ == TeGrouped)
		{
			query += " WHERE (c_legend_id <> 0)";
		}
		else if(selectedObjects_ == TeNotGrouped)
		{
			query += " WHERE (c_legend_id = 0)";
		}

		if(func >= 0)
			query += " GROUP BY " + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_id";
		TeDatabasePortal*	portal = db_->getPortal();
		if(portal->query(query) == false)
		{
			delete portal;
			return;
		}

		bool bb = portal->fetchRow(0);
		while(bb)
		{
			string sa = portal->getData(0);
			string sb = portal->getData(1);
			if(sa.empty()==false && sb.empty()==false)
			{
				a = atof(sa.c_str());
				b = atof(sb.c_str());
				vnx_.push_back(a);
				vny_.push_back(b);
			}
			bb = portal->fetchRow();
		}
		delete portal;
	}
	else if(theme_->getTheme()->getProductId() == TeEXTERNALTHEME)
	{
		TeTheme* theme = (TeTheme*)theme_->getTheme();
		TeTheme* rTheme = ((TeExternalTheme*)theme)->getRemoteTheme();
		string CT = rTheme->collectionTable();
		string CA = rTheme->collectionAuxTable();
		TeDatabasePortal*	portal = rTheme->layer()->database()->getPortal();

		string query = "SELECT " + atr + rTheme->sqlGridFrom();

		string where;
		if(selectedObjects_ == TeAll)
		{
			where += " WHERE (grid_status = 1 OR grid_status = 3";
			where += " OR (grid_status is null AND (c_object_status = 1 OR c_object_status = 3)))";
		}
		else if(selectedObjects_ == TeSelectedByQuery)
		{
			where += " WHERE (grid_status = 3  OR (grid_status is null AND c_object_status = 3))";
		}
		else if(selectedObjects_ == TeNotSelectedByQuery)
		{
			where += " WHERE ((grid_status = 1  OR (grid_status is null AND c_object_status = 1)))";
		}
		else if(selectedObjects_ == TeGrouped)
		{
			where += " WHERE (c_legend_id <> 0)";
		}
		else if(selectedObjects_ == TeNotGrouped)
		{
			where += " WHERE (c_legend_id = 0)";
		}

		vector<string> sv = generateItemsInClauseVec(theme, where);
		vector<string>::iterator sit;

		for(sit=sv.begin(); sit!=sv.end(); ++sit)
		{
			string sel = query + " WHERE " + CA + ".unique_id IN " + *sit;

			if(func >= 0)
				sel += " GROUP BY " + CT + ".c_object_id";

			if(portal->query(sel) == false)
			{
				delete portal;
				return;
			}

			while(portal->fetchRow())
			{
				string sa = portal->getData(0);
				string sb = portal->getData(1);
				if(sa.empty()==false && sb.empty()==false)
				{
					a = atof(sa.c_str());
					b = atof(sb.c_str());
					vnx_.push_back(a);
					vny_.push_back(b);
				}
			}
			portal->freeResult();
		}
		delete portal;	
	}
}


void GraphicWindow::gser( float * gamser, float a, float x, float * gln )
{
	int n;
	float sum, del,ap;

	*gln = gammln(a);

	if (x <= 0.0)
	{
		if ( x < 0.0)
			printf ("x less than 0 in the GSER routine");
	
		*gamser = 0.0;
		return;
	}

	else
	{
		ap = a;
		del= sum = (float)(1.0/a);
		for (n=1; n <= ITMAX_; n++)
		{
			ap += 1.0;
			del *= x/ap;
			sum += del;
			if (fabs (del) < fabs (sum)*EPS_)
			{
				*gamser = (float)(sum*exp(-x+a*log(x)-(*gln)));
				return;
			}
		}
		printf ("Variable \"a\" too large, ITMAX too small in the GSER routine");
		return;
	}
}



void GraphicWindow::plotGraphic()
{
	bool vazio = false;
	if (theme_ == 0)
	{
		zoomInit_ = true;
		return;
	}

	if(xmin_ > xmax_ && ymin_ > ymax_)
	{
//		zoomInit_ = true;
//		return;
		vx_.push_back(0.);
		vy_.push_back(0.);
//		vx_.push_back(1.);
//		vy_.push_back(0.);
		xmin_ = 0.;
		ymin_ = 0.;
		xmax_ = 0.;
		ymax_ = 0.;
		vazio = true;
	}
	if(vx_.size()==0 || vy_.size()==0)
	{
//		zoomInit_ = true;
//		return;
		vx_.push_back(0.);
		vy_.push_back(0.);
//		vx_.push_back(1.);
//		vy_.push_back(0.);
		vazio = true;
	}
	unsigned int i;
	if(x_)
		delete []x_;
	if(y_)
		delete []y_;
	if(mx_)
		delete []mx_;
	if(my_)
		delete []my_;
	if(nx_)
		delete []nx_;
	if(ny_)
		delete []ny_;
	x_ = y_ = mx_ = my_ = nx_ = ny_ = 0;

	x_ = new double[vx_.size()];
	y_ = new double[vy_.size()];

	if(x_== NULL || y_==NULL)
	{
//		zoomInit_ = true;
//		return;
	}
	TeLegendEntry& leg = ((TeTheme*)theme_->getTheme())->defaultLegend();
	TeColor cor = leg.visual(TePOLYGONS)->color();
	defaultColor_.setRgb(cor.red_, cor.green_, cor.blue_);

	TeLegendEntry& pleg = ((TeTheme*)theme_->getTheme())->pointingLegend();
	cor = pleg.visual(TePOLYGONS)->color();
	pointingColor_.setRgb(cor.red_, cor.green_, cor.blue_);


	gploted_ = graphicType_;

	for(i=0; i<vx_.size(); i++)
	{
		x_[i] = vx_[i];
		y_[i] = vy_[i];
	}

	if(gploted_ == Skater)
	{
		if(vazio)
			dataCurve_->setPen(Qt::NoPen);
		else
			dataCurve_->setPen(skaterPen_);

		dataCurve_->setStyle(QwtPlotCurve::Lines);
		dataCurve_->setBrush(Qt::NoBrush);
		dataCurve_->setRawData(x_, y_, vx_.size());

		plot_->setTitle(title_.c_str());
		plot_->setAxisTitle(QwtPlot::xBottom, labelBottom_.c_str());
		plot_->setAxisTitle(QwtPlot::yLeft, labelLeft_.c_str());
	}
	else if(gploted_ == NormalProbability)
	{
		if(vazio)
			dataCurve_->setPen(Qt::NoPen);
		else
			dataCurve_->setPen(normalProbCurvePen_);

		dataCurve_->setPen(normalProbCurvePen_);
		dataCurve_->setStyle(QwtPlotCurve::Lines);
		dataCurve_->setBrush(Qt::NoBrush);
		dataCurve_->setRawData(x_, y_, vx_.size());

		plot_->setTitle(title_.c_str());
		plot_->setAxisTitle(QwtPlot::xBottom, labelBottom_.c_str());
		plot_->setAxisTitle(QwtPlot::yLeft, labelLeft_.c_str());

		nx_ = new double[vnx_.size()];
		ny_ = new double[vny_.size()];

//		if(nx_== NULL || ny_==NULL)
//			return;
		for(i=0; i<vnx_.size(); i++)
		{
			nx_[i] = vnx_[i];
			ny_[i] = vny_[i];
		}
		if(vazio)
			dataSelCurve_->setPen(Qt::NoPen);
		else
			dataSelCurve_->setPen(normalCurvePen_);

		dataSelCurve_->setStyle(QwtPlotCurve::Lines);
		dataSelCurve_->setBrush(Qt::NoBrush);
		dataSelCurve_->setRawData(nx_, ny_, vnx_.size());
	}
	else if(gploted_ == Semivariogram)
	{
		if(vazio)
			dataCurve_->setPen(Qt::NoPen);
		else
			dataCurve_->setPen(semivarCurvePen_);

		dataCurve_->setStyle(QwtPlotCurve::Lines);
		dataCurve_->setBrush(Qt::NoBrush);
		dataCurve_->setRawData(x_, y_, vx_.size());

		plot_->setTitle(title_.c_str());
		plot_->setAxisTitle(QwtPlot::xBottom, labelBottom_.c_str());
		plot_->setAxisTitle(QwtPlot::yLeft, labelLeft_.c_str());

		nx_ = new double[vnx_.size()];
		ny_ = new double[vny_.size()];

//		if(nx_== NULL || ny_==NULL)
//			return;
		for(i=0; i<vnx_.size(); i++)
		{
			nx_[i] = vnx_[i];
			ny_[i] = vny_[i];
		}
		if(vazio)
			dataSelCurve_->setPen(Qt::NoPen);
		else
			dataSelCurve_->setPen(semivarAdjustCurvePen_);

		dataSelCurve_->setStyle(QwtPlotCurve::Lines);
		dataSelCurve_->setBrush(Qt::NoBrush);
		dataSelCurve_->setRawData(nx_, ny_, vnx_.size());
	}
	else if(gploted_ == Histogram)
	{
		plot_->setTitle(title_.c_str());
		plot_->setAxisTitle(QwtPlot::xBottom, labelBottom_.c_str());
		plot_->setAxisTitle(QwtPlot::yLeft, labelLeft_.c_str());

		dataCurve_->setSymbol(QwtSymbol((QwtSymbol::NoSymbol), QBrush(Qt::NoBrush), QPen(Qt::NoPen), QSize(1,1)));
		dataCurve_->setStyle(QwtPlotCurve::UserCurve); // histogram
		if(vazio)
		{
			dataCurve_->setPen(Qt::NoPen);
			dataCurve_->setBrush(Qt::NoBrush);
		}
		else
		{
			dataCurve_->setPen(histogramPen_);
			dataCurve_->setBrush(QBrush(defaultColor_));
		}
		dataCurve_->setRawData(x_, y_, vx_.size());

		if(mx_)
			delete []mx_;
		if(my_)
			delete []my_;
		mx_ = new double[vmx_.size()];
		my_ = new double[vmy_.size()];

//		if(mx_== NULL || my_==NULL)
//			return;
		for(i=0; i<vmx_.size(); i++)
		{
			mx_[i] = vmx_[i];
			my_[i] = vmy_[i];
		}
		dataSelCurve_->setSymbol(QwtSymbol((QwtSymbol::NoSymbol), QBrush(Qt::NoBrush), QPen(Qt::NoPen), QSize(1,1)));
		dataSelCurve_->setStyle(QwtPlotCurve::UserCurve); // histogram
		if(vazio)
		{
			dataSelCurve_->setPen(Qt::NoPen);
			dataSelCurve_->setBrush(Qt::NoBrush);
		}
		else
		{
			dataSelCurve_->setPen(histogramPen_);
			dataSelCurve_->setBrush(QBrush(pointingColor_));
		}
		dataSelCurve_->setRawData(mx_, my_, vmx_.size());
	}
	else if(gploted_ == RasterHistogram)
	{
		plot_->setTitle(title_.c_str());
		plot_->setAxisTitle(QwtPlot::xBottom, labelBottom_.c_str());
		plot_->setAxisTitle(QwtPlot::yLeft, labelLeft_.c_str());

		dataCurve_->setSymbol(QwtSymbol((QwtSymbol::NoSymbol), QBrush(Qt::NoBrush), QPen(Qt::NoPen), QSize(1,1)));
		dataCurve_->setStyle(QwtPlotCurve::UserCurve); // histogram
		if(vazio)
		{
			dataCurve_->setPen(Qt::NoPen);
			dataCurve_->setBrush(Qt::NoBrush);
		}
		else
		{
			dataCurve_->setPen(rasterHistogramPen_);
			dataCurve_->setBrush(rasterHistogramBrush_);
		}
		dataCurve_->setRawData(x_, y_, vx_.size());

		dataSelCurve_->setRawData(mx_, my_, 0);
	}
	else if(gploted_ == Dispersion)
	{
		if(nx_)
			delete []nx_;
		if(ny_)
			delete []ny_;
		nx_ = new double[vnx_.size()];
		ny_ = new double[vny_.size()];

//		if(nx_== NULL || ny_==NULL)
//			return;
		for(i=0; i<vnx_.size(); i++)
		{
			nx_[i] = vnx_[i];
			ny_[i] = vny_[i];
		}
		plot_->setTitle(title_.c_str());
		plot_->setAxisTitle(QwtPlot::xBottom, labelBottom_.c_str());
		plot_->setAxisTitle(QwtPlot::yLeft, labelLeft_.c_str());
		dataCurve_->setStyle(QwtPlotCurve::NoCurve); 
		if(vazio)
		{
			dispersionSymbol_.setPen(Qt::NoPen);
			dispersionSymbol_.setBrush(Qt::NoBrush);
		}
		else
		{
			dispersionSymbol_.setPen(defaultColor_);
			dispersionSymbol_.setBrush(defaultColor_);
		}

		dataCurve_->setSymbol(dispersionSymbol_);
		dataCurve_->setRawData(x_, y_, vx_.size());

		dataSelCurve_->setStyle(QwtPlotCurve::NoCurve); 
		QwtSymbol ssym(dispersionSymbol_);
		if(vazio)
		{
			ssym.setPen(Qt::NoPen);
			ssym.setBrush(Qt::NoBrush);
		}
		else
		{
			ssym.setPen(pointingColor_);
			ssym.setBrush(pointingColor_);
		}
		dataSelCurve_->setSymbol(ssym);
		dataSelCurve_->setRawData(nx_, ny_, vnx_.size());
	}

	if(zoomInit_ == true)
	{
		QwtDoubleRect adr, dr = dataCurve_->boundingRect();
		if(moran_ == true)
		{
			if(fabs(dr.left()) > fabs(dr.right()))
				dr.setRight(-(dr.left()));
			else
				dr.setLeft(-(dr.right()));

			if(fabs(dr.top()) > fabs(dr.bottom()))
				dr.setBottom(-(dr.top()));
			else
				dr.setTop(-(dr.bottom()));

			adr = zoomer_->adjustRect(dr);

			// marker
			marker_->setValue(0.0, 0.0);
			marker_->setLineStyle(QwtPlotMarker::Cross);
			marker_->setLinePen(QPen(Qt::darkBlue, 2, Qt::SolidLine));
			marker_->attach(plot_);

			QFont font;
			font.setBold(true);
			font.setPointSize(18);

			QwtText text;
			text.setFont(font);
			text.setColor(QColor(Qt::darkGray));

			text.setText("LL");
			markerLL_->setLineStyle(QwtPlotMarker::NoLine);
			markerLL_->setValue(adr.left()+adr.width()/4., adr.top()+adr.height()/4.);
			markerLL_->setLabel(text);

			text.setText("LH");
			markerLH_->setLineStyle(QwtPlotMarker::NoLine);
			markerLH_->setValue(adr.left()+adr.width()/4., adr.top()+3*adr.height()/4.);
			markerLH_->setLabel(text);

			text.setText("HL");
			markerHL_->setLineStyle(QwtPlotMarker::NoLine);
			markerHL_->setValue(adr.left()+3*adr.width()/4., adr.top()+adr.height()/4.);
			markerHL_->setLabel(text);

			text.setText("HH");
			markerHH_->setLineStyle(QwtPlotMarker::NoLine);
			markerHH_->setValue(adr.left()+3*adr.width()/4., adr.top()+3*adr.height()/4.);
			markerHH_->setLabel(text);
		}
		else
		{
			adr = zoomer_->adjustRect(dr);

			marker_->setLineStyle(QwtPlotMarker::NoLine);
			markerLL_->setLabel(QwtText(""));
			markerLH_->setLabel(QwtText(""));
			markerHL_->setLabel(QwtText(""));
			markerHH_->setLabel(QwtText(""));
		}

		zoomer_->setZoomBase(adr);
		zoomToolButton_->setOn(false);
		previousToolButton_->setEnabled(false);
		nextToolButton_->setEnabled(false);
	}
	zoomInit_ = true;
	
	zoomToggledSlot(false);

	if (graphicType_ == Histogram || graphicType_ == Dispersion)
		promoteToolButton_->setEnabled(true);
	else
		promoteToolButton_->setEnabled(false);


	plot_->replot();
}


void GraphicWindow::plotSelTable()
{
	if (mainWindow_ == 0)
		return;

	if (mainWindow_->graphicCursorAction->isOn() == false)
		return;

	if (theme_ == 0)
		return;

	if ((gploted_ == Histogram) && (graphicType_ == Histogram))
		applyGraphic();
	else if ((gploted_ == Dispersion) && (graphicType_ == Dispersion))
		applyGraphic();
}

void GraphicWindow::applyGraphic()
{
	if (mainWindow_ == 0)
	{
		zoomInit_ = true;
		return;
	}


	if(graphicType_ == Skater)
	{
		title_ = tr("Variance Decrease").latin1();
		labelLeft_ = tr("Variance").latin1();
		labelBottom_ = tr("Number of Clusters").latin1();
	}
	else if(graphicType_ == Semivariogram)
	{
		title_ = tr("Semivariogram").latin1();
		title_ += ": " + attributeX_;
		labelBottom_ = tr("h").latin1();
		labelLeft_ = tr("Y(h)").latin1();
	}
	else if(graphicType_ == RasterHistogram)
	{
		title_ = tr("Raster Histogram").latin1();
		labelLeft_ = tr("Frequency").latin1();
		labelBottom_ = tr("Gray Level").latin1();
	}
	else
	{
		if (theme_ == 0)
		{
			zoomInit_ = true;
			return;
		}
		QString tipo = graphicParams_->graphicTypeComboBox->currentText();
		string	atrx = attributeX_;
		string	atry = attributeY_;

		if(graphicType_ == Histogram)
		{
			title_ = tipo.latin1();
			title_ += ": " + atrx;
			labelLeft_ = tr("Frequency").latin1();
			labelBottom_ = atrx;
			getHistogramValues();
		}
		else if(graphicType_ == NormalProbability)
		{
			title_ = tipo.latin1();
			title_ += ": " + atrx;
			labelLeft_ = tr("Probability").latin1();
			labelBottom_ = atrx;
			getProbNormalValues();
		}
		else if(graphicType_ == Dispersion)
		{
			title_ = tr("Dispersion:").latin1();
			title_ += " " + atrx + " x " + atry;
			labelLeft_ = atry;
			labelBottom_ = atrx;
			getScatterValues();
			getSelTabScatterValues();
		}

		moran_ = false;
		string ax = attributeX_;
		string ay = attributeY_;
		if(ax=="Z" && ay=="Wz")
			moran_ = true;
		if(moran_ == false)
		{
			int iax = ax.find("Z");
			int iay = ay.find("Wz");
			if(iax == (int)(ax.size()-1) && iay == (int)(ay.size()-2))
			{
				string s1 = ax;
				s1 = s1.substr(0, iax);
				string s2 = ay;
				s2 = s2.substr(0, iay);
				if(s1 == s2)
					moran_ = true;
			}
		}
		
		if(moran_ == true)
			title_ = tr("Moran Diagram").latin1();

		xaxis_ = attributeX_;
		yaxis_ = attributeY_;
	}
	plotGraphic();
}


void GraphicWindow::closeEvent( QCloseEvent * )
{
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();	
	mainWindow_->graphicAction->setOn(false);
	hide();
}


void GraphicWindow::hide()
{
	QMainWindow::hide();
}


void GraphicWindow::show()
{
	getGraphicParamsSlot();
	QMainWindow::show();
}


void GraphicWindow::getGraphicParamsSlot()
{
	graphicParams_ = mainWindow_->getGraphicParams();
    db_ = mainWindow_->currentDatabase();

	if(graphicParams_ == 0)
		return;
	if(graphicParams_->graphicTypeComboBox->count() == 0)
		return;

    graphicType_ = graphicParams_->graphicTypeComboBox->currentItem();
	slices_ = atoi(graphicParams_->graphicSlicesComboBox->currentText().latin1());
    grid_ = mainWindow_->getGrid();
    theme_ = mainWindow_->currentTheme();
	int index=graphicParams_->inputObjectsComboBox->currentItem();
	switch(index)
	{
		case 0 :	selectedObjects_ = TeAll;
					break;
		case 1 :	selectedObjects_ = TeSelectedByQuery;
					break;
		case 2 :	selectedObjects_ = TeNotSelectedByQuery;
					break;
		case 3 :	selectedObjects_ = TeGrouped;
					break;
		case 4 :	selectedObjects_ = TeNotGrouped;
					break;
		default :	selectedObjects_ = TeAll;
	}

	if(graphicParams_->attributeXComboBox->count() == 0)
		return;
	if(graphicParams_->attributeYComboBox->count() == 0)
		return;
	attributeX_ = graphicParams_->attributeXComboBox->currentText().latin1();
	attributeY_ = graphicParams_->attributeYComboBox->currentText().latin1();

	if(graphicParams_->functionCheckBox->isChecked())
	{
		string xfunc = graphicParams_->xFunctionComboBox->currentText().latin1();
		attributeX_ = xfunc + "(" + attributeX_ + ")";

		if(graphicType_ == Dispersion)
		{
			string yfunc = graphicParams_->yFunctionComboBox->currentText().latin1();
			attributeY_ = yfunc + "(" + attributeY_ + ")";
		}
	}

	applyGraphic();
}


double GraphicWindow::ymaxaxis()
{
	return ymaxaxis_;
}


double GraphicWindow::xmaxaxis()
{
	return xmaxaxis_;
}


double GraphicWindow::xminaxis()
{
	return xminaxis_;
}


double GraphicWindow::yminaxis()
{
	return yminaxis_;
}


int GraphicWindow::rxaxis()
{
	return rxaxis_;
}


int GraphicWindow::ryaxis()
{
	return ryaxis_;
}


void GraphicWindow::print()
{
//	gplot_->copyPixmapToPrinter();
    QPrinter printer;

    QString docName = plot_->title().text();
    if ( docName.isEmpty() )
    {
		QString s = QString::fromLatin1 ("\n");
		QRegExp r(s);
		QString ss = tr (" -- ");
        docName.replace (r, ss);
        printer.setDocName (docName);
    }

    printer.setCreator("TerraView");
//    printer.setOrientation(QPrinter::Portrait);

    if (printer.setup())
    {
        QwtPlotPrintFilter filter;
        if ( printer.colorMode() == QPrinter::GrayScale )
        {
            filter.setOptions(QwtPlotPrintFilter::PrintAll 
                & ~QwtPlotPrintFilter::PrintWidgetBackground);
        }
        plot_->print(printer, filter);
    }
}


void GraphicWindow::mousePressedSlot( int buttom, int state, QwtDoubleRect dr)
{
	if (mainWindow_ == 0)
		return;

	if (theme_ == 0)
		return;

	if(buttom == Qt::LeftButton)
	{
		if (mainWindow_->graphicCursorAction->isOn() == false)
			return;

		if(gploted_ == NormalProbability || 
		   gploted_ == RasterHistogram ||
		   gploted_ == Skater)
			return;

		int func = xaxis_.find("(");
		string xat = xaxis_, yat = yaxis_;
		if(func >= 0)
		{
			xat = xat.substr(func+1);
			xat = xat.substr(0, xat.size()-1);
			int f = yat.find("(");
			yat = yat.substr(f+1);
			yat = yat.substr(0, yat.size()-1);
		}

		string xmin = Te2String(dr.left(), 6);
		string ymin = Te2String(dr.bottom(), 6);
		string xmax = Te2String(dr.right(), 6);
		string ymax = Te2String(dr.top(), 6);

		TeDatabase* db;
		string	from, CT, CTE;
		TeTheme* theme = (TeTheme*)theme_->getTheme();

		if(theme->getProductId() == TeTHEME)
		{
			db = theme->layer()->database();
			CT = theme->collectionTable();
			CTE = theme->collectionAuxTable();
			from = theme->sqlGridFrom();
		}
		else if(theme->getProductId() == TeEXTERNALTHEME)
		{
			db = ((TeExternalTheme*)theme)->getRemoteTheme()->layer()->database();
			CT = ((TeExternalTheme*)theme)->getRemoteTheme()->collectionTable();
			CTE = ((TeExternalTheme*)theme)->getRemoteTheme()->collectionAuxTable();
			from = ((TeExternalTheme*)theme)->getRemoteTheme()->sqlGridFrom();
		}
		
		string where;
		if(selectedObjects_ == TeAll) {}
		else if(selectedObjects_ == TeSelectedByQuery)
			where += " WHERE (grid_status >= 2  OR (grid_status is null AND c_object_status >= 2))";
		else if(selectedObjects_ == TeNotSelectedByQuery)
			where += " WHERE (grid_status < 2  OR (grid_status is null AND c_object_status < 2))";
		else if(selectedObjects_ == TeGrouped)
			where += " WHERE (c_legend_id <> 0)";
		else if(selectedObjects_ == TeNotGrouped)
			where += " WHERE (c_legend_id = 0)";

		string xas = xaxis_ , yas = yaxis_, obj = CT + ".c_object_id";
		string sobj = obj;
		if(func >= 0)
		{
			xas = xaxis_ + " as xas";
			yas = yaxis_ + " as yas";
			obj = "MIN(" + obj + ") as obj";
		}

		string sel;
		if(gploted_ == Dispersion)
		{
			if(func >= 0)
			{
				sel = "SELECT " + sobj + ", " + xat + ", " + yat + from;
				if(where.empty() == false)
					sel += where;

				sel.insert(0, "SELECT " + obj + ", " +  xas + ", " + yas + " FROM (");
				sel += ") GROUP BY " + CT + ".c_object_id";
				sel.insert(0, "SELECT obj, xas, yas FROM (");
				sel += ") WHERE ";
				sel += "xas > " + xmin + " AND ";
				sel += "xas < " + xmax + " AND ";
				sel += "yas > " + ymin + " AND ";
				sel += "yas < " + ymax;
			}
			else
			{
				sel = "SELECT " + CTE + ".unique_id, " + CT + ".c_object_id, " + xat + ", " + yat + from;
				if(where.empty())
					sel += " WHERE ";
				else
					sel += where + " AND ";

				sel += xat + " > " + xmin + " AND ";
				sel += xat + " < " + xmax + " AND ";
				sel += yat + " > " + ymin + " AND ";
				sel += yat + " < " + ymax;
			}

		}
		else if(gploted_ == Histogram)
		{
			if(func >= 0)
			{
				sel = "SELECT " + sobj + ", " + xat + from;
				if(where.empty() == false)
					sel += where;

				sel.insert(0, "SELECT " + obj + ", " + xas + " FROM (");
				sel += ") GROUP BY " + CT + ".c_object_id";
				sel.insert(0, "SELECT obj, xas FROM (");
				sel += ") WHERE ";
				sel += "xas > " + xmin + " AND ";
				sel += "xas < " + xmax;
			}
			else
			{
				sel = "SELECT " + CTE + ".unique_id, " + CT + ".c_object_id, " + xat + from;
				if(where.empty())
					sel += " WHERE ";
				else
					sel += where + " AND ";

				sel += xat + " > " + xmin + " AND ";
				sel += xat + " < " + xmax;
			}
		}

		set<string> uniqueIdSet, objectIdSet;
		string val;
		TeDatabasePortal *portal = db->getPortal();
		if(portal->query(sel))
		{
			while(portal->fetchRow())
			{
				if(func >= 0)
					objectIdSet.insert(portal->getData(0));
				else
				{
					val = portal->getData(0);
					if(val.empty())
						objectIdSet.insert(portal->getData(1));
					else
						uniqueIdSet.insert(val);
				}
			}
		}
		delete portal;

		if(state == 0)
			grid_->putColorOnGrid(uniqueIdSet, objectIdSet, "newPointing");
		else if(state == Qt::ShiftButton)
			grid_->putColorOnGrid(uniqueIdSet, objectIdSet, "addPointing");
	}
}


void GraphicWindow::zoomToggledSlot( bool b)
{
	zoomer_->setEnabledSlot(b);
	if (b)
	{
		plot_->showCursor(false);
		uint i = zoomer_->zoomRectIndex();
		nextToolButton_->setEnabled(true);
		previousToolButton_->setEnabled(true);
		if(i == (zoomer_->zoomStack().size() - 1))
			nextToolButton_->setEnabled(false);
		if(i == 0)
			previousToolButton_->setEnabled(false);
	}
	else
	{
		nextToolButton_->setEnabled(false);
		previousToolButton_->setEnabled(false);

		if (graphicType_ == Histogram)
			plot_->cursorMode(TeQwtPlotPicker::histog);
		else if (graphicType_ == Dispersion)
			plot_->cursorMode(TeQwtPlotPicker::scatter);
		
		if (graphicType_ == Histogram || graphicType_ == Dispersion)
		{
			if(mainWindow_->graphicCursorAction->isOn())
				plot_->showCursor(true);
			else
				plot_->showCursor(false);
		}
		else
			plot_->showCursor(false);

		plot_->canvas()->setCursor(QCursor(Qt::arrowCursor));
	}
}


void GraphicWindow::previousZoomSlot()
{
	zoomer_->backward();
	nextToolButton_->setEnabled(true);
	uint i = zoomer_->zoomRectIndex();
	if(i == 0)
		previousToolButton_->setEnabled(false);
}


void GraphicWindow::nextZoomSlot()
{
	zoomer_->forward();
		previousToolButton_->setEnabled(true);
	uint i = zoomer_->zoomRectIndex();
	if(i == (zoomer_->zoomStack().size() - 1))
		nextToolButton_->setEnabled(false);
}


void GraphicWindow::previousZoomEnableSlot( bool b)
{
	previousToolButton_->setEnabled(b);
}


void GraphicWindow::nextZoomEnableSlot( bool b)
{
	nextToolButton_->setEnabled(b);
}


void GraphicWindow::graphicCursorActivatedSlot( bool b)
{
	if(zoomToolButton_->isOn() && zoomToolButton_->isEnabled())
	{
		plot_->showCursor(false);
		zoomToggledSlot(true);
	}
	else
	{
		plot_->showCursor(b);
		zoomToggledSlot(false);
	}
}


void GraphicWindow::putColorOnObjectSlot( TeAppTheme *, set<string> & )
{
	zoomInit_ = false;
}


void GraphicWindow::promoteToggledSlot( bool b)
{
	mainWindow_->getGrid()->autoPromote_ = b;
}


void GraphicWindow::plotRasterHistogram(TePDIHistogram hist)
{
	vx_.clear();
	vy_.clear();
	theme_ = mainWindow_->currentTheme();

	if(hist.size() > 0)
	{
		TePDIHistogram::iterator it = hist.begin();
		while(it != hist.end())
		{
			vx_.push_back(it->first);
			vy_.push_back((double)it->second);
			it++;
		}

		xmin_ = vx_[0];
		ymin_ = vy_[0];
		xmax_ = vx_[vx_.size()-1];
		ymax_ = vy_[vy_.size()-1];
	}

	graphicType_ = RasterHistogram;
	applyGraphic();
}


void GraphicWindow::configurationSlot()
{
	GraphicConfigurationWindow w(this, "Graphic Configuration Window");
	if(w.exec() == QDialog::Accepted)
	{
		plotGrid_->setMajPen(majorLinePen_);
		plotGrid_->setMinPen(minorLinePen_);
		plotGrid_->enableY(horizontalMajorVisible_);
		plotGrid_->enableYMin(horizontalMinorVisible_);
		plotGrid_->enableX(verticalMajorVisible_);
		plotGrid_->enableXMin(verticalMinorVisible_);
		plot_->setCanvasBackground(backgroundColor_);
		plot_->setAxisFont(QwtPlot::xBottom, horizontalAxisFont_);
		plot_->setAxisFont(QwtPlot::yLeft, verticalAxisFont_);
		plot_->replot();
		plotGraphic();
	}
}


void GraphicWindow::helpSlot()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("graphic.htm");

	if(help_->erro_ == false)
	{
		help_->show();
		help_->raise();
	}
	else
	{
		delete help_;
		help_ = 0;
	}
}


void GraphicWindow::setSkaterValues( vector<double> & setX, vector<double> & setY )
{
	if (setX.empty() || setY.empty())
		return;

	vx_.clear();
	vy_.clear();

    theme_ = mainWindow_->currentTheme();
	for (unsigned int i=0; i<setX.size(); ++i)
	{
		vx_.push_back(setX[i]);
		vy_.push_back(setY[i]);
	}
	graphicType_ = Skater;
	gploted_ = Skater;
}

void GraphicWindow::setSemiVariogramValues( TeMatrix &mx, TeMatrix &mod )
{
	int i,j;

	int nvalues = mx.Nrow();
	if (nvalues <= 0)
		return;

	vx_.clear();
	vy_.clear();
	vnx_.clear();
	vny_.clear();

	j=0;
	for (i=0; i<nvalues; ++i)
	{
		vx_.push_back(mx(i,j));
		vy_.push_back(mx(i,j+1));
	}
	j=0;
	for (i=0; i<nvalues; ++i)
	{
		vnx_.push_back(mod(i,j));
		vny_.push_back(mod(i,j+1));
	}
    theme_ = mainWindow_->currentTheme();
	graphicType_ = Semivariogram;
	gploted_ = Semivariogram;
}


void GraphicWindow::setAttributeTitle( const string & name )
{
	attributeX_ = name;
}

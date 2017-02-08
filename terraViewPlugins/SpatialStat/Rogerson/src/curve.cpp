/*
 * $Id: curve.cpp,v 1.2 2007/07/31 13:01:34 leste Exp $
 * (C) COPYRIGHT 2006, Joao Luiz Elias Campos.
 * All Rights Reserved
 * Duplication of this program or any part thereof without the express
 * written consent of the author is prohibited
 */

/**
 * @file curve.cpp
 * @author Joao Luiz Elias Campos.
 * @date February 21, 2006
 */

/*
 * Global variables and symbols:
 */
#include "curve.h"
#include "xyplot.h"

Curve::Curve()
{
}

Curve::Curve(const std::valarray<double> &r)
{
 _vals.resize(r.size());
 _vals = r;
}

Curve::Curve(const std::valarray<double> &r,const std::valarray<double> &x)
{
 _vals.resize(r.size());
 _vals = r;
 _xaxis.resize(x.size());
 _xaxis = x;
}


Curve::~Curve()
{
}

void Curve::addValues(const std::valarray<double> &r)
{
 _vals.resize(r.size());
 _vals = r;
}


void Curve::addValues(const std::valarray<double> &r, const std::valarray<double> &x)
{
 _vals.resize(r.size());
 _vals = r;
 _xaxis.resize(x.size());
 _xaxis = x;
}

Curve &Curve::operator=(const std::valarray<double> &r)
{
 _vals.resize(r.size());
 _vals = r;
 return *this;
}

void Curve::draw(QPainter *p, XYPlot *xyp)
{
 p->setPen(_color);
 int x1, y1, x2, y2;
 xyp->transformX(0.0, x1);
 xyp->transformY(_vals[0], y1);
 for( int i = 1; i < (int)_vals.size(); i++ ) {
  xyp->transformX(_xaxis[i], x2);
  xyp->transformY(_vals[i], y2);
  p->drawLine(x1, y1, x2, y2);
  x1 = x2;
  y1 = y2;
 }
}

/*
 * End of File.
 */


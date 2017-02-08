/*
 * $Id: xyplot.cpp,v 1.2 2007/07/31 13:01:34 leste Exp $
 * (C) COPYRIGHT 2005, Joao Luiz Elias Campos.
 * All Rights Reserved
 * Duplication of this program or any part thereof without the express
 * written consent of the author is prohibited
 */

/**
 * @file xyplot.cpp
 * @author Joao Luiz Elias Campos.
 * @date May 28, 2005
 */

/*
 * Global variables and symbols:
 */
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qobjectlist.h>
#include "xyplot.h"
#include "xyseries.h"

/**
 *
 */
XYPlot::XYPlot(QWidget *parent, const char *name, WFlags f)
       :QFrame(parent, name, f)
{
 _xmin = 0.0;
 _xmax = 1500.0;
 _xstep = 150.0;
 _xtick = XYPlot::LeftTick;

 _ymin = 0.0;
 _ymax = 1500.0;
 _ystep = 150.0;
 _ytick = XYPlot::RightTick;

 _fh = fontMetrics().ascent();
 QString v = QString::number(_xmax, 'f', 0);
 _fwx = fontMetrics().width(v);

 setMinimumSize(200, 200);
}

XYPlot::~XYPlot()
{
}

void XYPlot::setMinXValue(double m)
{
 _xmin = m;
 if( (int)_xmin != 0 ) {
  if( (_xmin/(int)_xmin) > 1.0 ) {
   _xmin = (double)((int)(_xmin - 0.5));
  }
 }
 _xstep = 0.1 * (_xmax - _xmin);
 if( (_xstep/(int)_xstep) > 1.0 ) {
  _xstep = (double)((int)(_xstep + 0.5));
 }
 _xmax = _xmin + (10.0 * _xstep);
 frameChanged();
}

void XYPlot::setMaxXValue(double m)
{
 _xmax = m;
 if( (_xmax/(int)_xmax) > 1.0 ) {
  _xmax = (double)((int)(_xmax + 0.5));
 }
 _xstep = 0.1 * (_xmax - _xmin);
 if( (_xstep/(int)_xstep) > 1.0 ) {
  _xstep = (double)((int)(_xstep + 0.5));
 }
 _xmin = _xmax - (10.0 * _xstep);
 frameChanged();
}

void XYPlot::setStepXValue(double s)
{
 _xstep = s;
 frameChanged();
}

void XYPlot::setTicksXAxis(TicksMark side)
{
 _xtick = side;
 frameChanged();
}

void XYPlot::setMinYValue(double m)
{
 _ymin = m;
 if( (int)_ymin != 0 ) {
  if( (_ymin/(int)_ymin) > 1.0 ) {
   _ymin = (double)((int)(_ymin - 0.5));
  }
 }
 _ystep = 0.1 * (_ymax - _ymin);
 if( (_ystep/(int)_ystep) > 1.0 ) {
  _ystep = (double)((int)(_ystep + 0.5));
 }
 _ymax = _ymin + (10.0 * _ystep);
 frameChanged();
}

void XYPlot::setMaxYValue(double m)
{
 _ymax = m;
 if( (_ymax/(int)_ymax) > 1.0 ) {
  _ymax = (double)((int)(_ymax + 0.5));
 }
 _ystep = 0.1 * (_ymax - _ymin);
 if( (_ystep/(int)_ystep) > 1.0 ) {
  _ystep = (double)((int)(_ystep + 0.5));
 }
 _ymin = _ymax - (10.0 * _ystep);
 frameChanged();
}

void XYPlot::setStepYValue(double s)
{
 _ystep = s;
 frameChanged();
}

void XYPlot::setTicksYAxis(TicksMark side)
{
 _ytick = side;
 frameChanged();
}

void XYPlot::drawAxis(QPainter *p)
{
 QRect box = contentsRect();
 p->drawLine(box.left()+55, box.bottom()-55, box.right()-_fwx, box.bottom()-55);
 p->drawLine(box.left()+55, box.bottom()-55, box.left()+55, box.top()+_fh);
}

void XYPlot::drawMinTicks(QPainter *p)
{
 int dc;
 double v;
 double ls = _xstep / 5.0;
 QRect box = contentsRect();
 for( v = _xmin; v <= _xmax; v += ls ) {
  transformX(v, dc);
  p->drawLine(dc+55, box.bottom()-50, dc+55, box.bottom()-55);
 }

 ls = _ystep / 5.0;
 for( v = _ymin; v <= _ymax; v += ls ) {
  transformY(v, dc);
  p->drawLine(box.left()+50, dc+55, box.left()+55, dc+55);
 }
}

void XYPlot::drawMaxTicks(QPainter *p)
{
 QRect box = contentsRect();
 QFontMetrics finfo = fontMetrics();
 QString sv;
 int dc, fw;
 double v;

 for( v = _xmin; v <= _xmax; v += _xstep ) {
  sv = QString::number(v, 'f', 0);
  fw = finfo.width(sv);
  transformX(v, dc);
  p->drawLine(dc+55, box.bottom()-40, dc+55, box.bottom()-55);
  p->drawText(dc+55-(fw/2), box.bottom()-38+_fh, sv);
 }

 for( v = _ymin; v <= _ymax; v += _ystep ) {
  sv = QString::number(v, 'f', 0);
  fw = finfo.width(sv);
  transformY(v, dc);
  p->drawLine(box.left()+40, dc+55, box.left()+55, dc+55);
  p->drawText(box.left()+40-fw, dc+55+(_fh/2), sv);
 }
}

void XYPlot::drawTitles(QPainter *p)
{
 QRect box = contentsRect();
 QFontMetrics finfo = fontMetrics();
 int length;
 int fwidth;
 int x, y;
 if( !_xtitle.isEmpty() ) {
  length = box.right() - box.left() - 55 - _fwx;
  fwidth = finfo.width(_xtitle);
  x = 55 + (length / 2) - (fwidth / 2);
  y = box.bottom() - _fh + 4;
  p->drawText(x, y, _xtitle);
 }

 if( !_ytitle.isEmpty() ) {
  length = box.bottom() - box.top() - 55 - _fh;
  fwidth = finfo.width(_ytitle);
  y = 55 + (length / 2) - (fwidth / 2);
  x = box.left() + _fh + 2;
  p->rotate(-90.0);
  p->translate(-y-fwidth, -y+x-4);
  p->drawText(x, y, _ytitle);
  p->resetXForm();
 }
}

void XYPlot::drawContents(QPainter *p)
{
 p->setPen(Qt::black);
 drawAxis(p);
 drawMinTicks(p);
 drawMaxTicks(p);
 drawTitles(p);

 if( _series.isEmpty() )
  return;

 QRect box = contentsRect();
 int lx = box.right() - box.left() - 55 - _fwx;
 int ly = box.bottom() - box.top() - 55;
 p->setClipRect(55, 55, lx, ly);
 p->translate(55, 55);
 for( XYSeries *crv = _series.first(); crv != NULL; crv = _series.next() ) {
  crv->draw(p, this);
 }
 p->resetXForm();
 p->setClipping(FALSE);
}

void XYPlot::transformX(double wc, int &dc)
{
 QRect box = contentsRect();
 int length = box.right() - box.left() - 55 - _fwx;
 dc = (int)(((wc - _xmin) * length) / (_xmax - _xmin));
}

void XYPlot::transformX(int dc, double &wc)
{
 QRect box = contentsRect();
 int length = box.right() - box.left() - 55 - _fwx;
 wc = ((dc * (_xmax - _xmin)) / length) + _xmin;
}

void XYPlot::transformY(double wc, int &dc)
{
 QRect box = contentsRect();
 int length = box.bottom() - box.top() - 55 - _fh;
 dc = (int)(((_ymax - wc) * length) / (_ymax - _ymin)) - 55 + _fh;
}

void XYPlot::transformY(int dc, double &wc)
{
 QRect box = contentsRect();
 int length = box.bottom() - box.top() - 55 - _fh;
 wc = ((dc * (_ymax - _ymin)) / length) + _ymin;
}

void XYPlot::setTitleXAxis(const QString &t)
{
 _xtitle = t;
}

void XYPlot::setTitleYAxis(const QString &t)
{
 _ytitle = t;
}

void XYPlot::append(XYSeries *c)
{
 if( _series.isEmpty() ) {
  this->setMaxXValue(c->maxX());
  this->setMinXValue(c->minX());
  this->setMaxYValue(c->maxY());
  this->setMinYValue(c->minY());
 }
 else {
  if( this->maxXValue() < c->maxX() )
   this->setMaxXValue(c->maxX());
  if( this->minXValue() > c->minX() )
   this->setMinXValue(c->minX());
  if( this->maxYValue() < c->maxY() )
   this->setMaxYValue(c->maxY());
  if( this->minYValue() > c->minY() )
   this->setMinYValue(c->minY());
 }
 _series.append(c);
}

/*
 * End of File.
 */


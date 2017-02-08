/*
 * $Id: xyseries.h 6243 2007-07-31 12:52:42Z leste $
 * (C) COPYRIGHT 2005, Joao Luiz Elias Campos.
 * All Rights Reserved
 * Duplication of this program or any part thereof without the express
 * written consent of the author is prohibited
 */

/**
 * @file xyseries.h
 * @author Joao Luiz Elias Campos.
 * @date May 28, 2005
 */

#ifndef __xyseries_h
#define __xyseries_h

#include <qobject.h>
#include <qpainter.h>
#include <qstring.h>

class XYPlot;

/**
 *
 */
class XYSeries : public QObject
{
 Q_OBJECT
 Q_PROPERTY(QColor color READ color WRITE setColor)

 protected:
  QColor _color;

 public:
  XYSeries();
  ~XYSeries();

  virtual double minX() const = 0;
  virtual double maxX() const = 0;
  virtual double minY() const = 0;
  virtual double maxY() const = 0;

  QColor color() const;
  void setColor(const QColor &);

  virtual void draw(QPainter *, XYPlot *) = 0;
};

inline QColor XYSeries::color() const {
 return _color;
}

inline void XYSeries::setColor(const QColor &color) {
 _color = color;
}

#endif


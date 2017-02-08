/*
 * $Id: curve.h 6243 2007-07-31 12:52:42Z leste $
 * (C) COPYRIGHT 2006, Joao Luiz Elias Campos.
 * All Rights Reserved
 * Duplication of this program or any part thereof without the express
 * written consent of the author is prohibited
 */

/**
 * @file curve.h
 * @author Joao Luiz Elias Campos.
 * @date February 21, 2006
 */

#ifndef __curve_h
#define __curve_h

#undef min
#undef max

#include <valarray>
#include "xyseries.h"

class XYPlot;

class Limiar : public XYSeries
{
 protected:
  double _val;
  double _xmax;

 public:
  Limiar();
  Limiar(double, double);
  virtual ~Limiar();

  double minX() const;
  double maxX() const;
  double minY() const;
  double maxY() const;

  void setValue(double);
  void setLimit(double);
  Limiar &operator=(double);

  void draw(QPainter *, XYPlot *);
};

inline double Limiar::minX() const {
 return 0.0;
}

inline double Limiar::maxX() const {
 return _xmax;
}

inline double Limiar::minY() const {
 return (_val * 0.85);
}

inline double Limiar::maxY() const {
 return (_val * 1.15);
}

class Curve : public XYSeries
{
 protected:
  std::valarray<double> _vals;
  std::valarray<double> _xaxis;

 public:
  Curve();
  Curve(const std::valarray<double> &);
  Curve(const std::valarray<double> &, const std::valarray<double> &);
  virtual ~Curve();

  double minX() const;
  double maxX() const;
  double minY() const;
  double maxY() const;

  void addValues(const std::valarray<double> &);
  void addValues(const std::valarray<double> &, const std::valarray<double> &);
  Curve &operator=(const std::valarray<double> &);

  void draw(QPainter *, XYPlot *);
};

inline double Curve::minX() const {
	return (double)_xaxis.min();
}

inline double Curve::maxX() const {
	return (double)_xaxis.max();
}

inline double Curve::minY() const {
 return (double)_vals.min();
}

inline double Curve::maxY() const {
 return (double)_vals.max();
}

#endif



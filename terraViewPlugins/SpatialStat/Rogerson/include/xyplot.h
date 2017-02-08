/*
 * $Id: xyplot.h,v 1.2 2007/07/31 13:01:34 leste Exp $
 * (C) COPYRIGHT 2005, Joao Luiz Elias Campos.
 * All Rights Reserved
 * Duplication of this program or any part thereof without the express
 * written consent of the author is prohibited
 */

/**
 * @file xyplot.h
 * @author Joao Luiz Elias Campos.
 * @date May 28, 2005
 */

#ifndef __xyplot_h
#define __xyplot_h

#include <qframe.h>
#include <qstring.h>
#include <qptrlist.h>
#include "xyseries.h"

/**
 *
 */
class XYPlot : public QFrame
{
 Q_OBJECT
 Q_ENUMS(TicksMark)
 Q_PROPERTY(TicksMark ticksXAxis READ ticksXAxis WRITE setTicksXAxis)
 Q_PROPERTY(TicksMark ticksYAxis READ ticksYAxis WRITE setTicksXAxis)
 Q_PROPERTY(double minXValue READ minXValue WRITE setMinXValue)
 Q_PROPERTY(double maxXValue READ maxXValue WRITE setMaxXValue)
 Q_PROPERTY(double stepXValue READ stepXValue WRITE setStepXValue)
 Q_PROPERTY(double minYValue READ minYValue WRITE setMinYValue)
 Q_PROPERTY(double maxYValue READ maxYValue WRITE setMaxYValue)
 Q_PROPERTY(double stepYValue READ stepYValue WRITE setStepYValue)
 Q_PROPERTY(QString titleXAxis READ titleXAxis WRITE setTitleXAxis)
 Q_PROPERTY(QString titleYAxis READ titleYAxis WRITE setTitleYAxis)

 public:
  enum TicksMark { LeftTick, RightTick };

 protected:
  TicksMark _xtick;
  TicksMark _ytick;
  double _xmin;
  double _xmax;
  double _xstep;
  double _ymin;
  double _ymax;
  double _ystep;
  int _fwx;
  int _fh;
  QString _xtitle;
  QString _ytitle;
  QPtrList<XYSeries> _series;

 protected:
  void drawAxis(QPainter *);
  void drawMinTicks(QPainter *);
  void drawMaxTicks(QPainter *);
  void drawTitles(QPainter *);
  void drawContents(QPainter *);
 
 public:
  XYPlot(QWidget * = 0, const char * = 0, WFlags = 0);
  ~XYPlot();

  double minXValue() const;
  double maxXValue() const;
  double stepXValue() const;
  TicksMark ticksXAxis() const;

  double minYValue() const;
  double maxYValue() const;
  double stepYValue() const;
  TicksMark ticksYAxis() const;

  QString titleXAxis() const;
  QString titleYAxis() const;

  void setMinXValue(double);
  void setMaxXValue(double);
  void setStepXValue(double);
  void setTicksXAxis(TicksMark);

  void setMinYValue(double);
  void setMaxYValue(double);
  void setStepYValue(double);
  void setTicksYAxis(TicksMark);

  void setTitleXAxis(const QString &);
  void setTitleYAxis(const QString &);

  void transformX(double, int &);
  void transformX(int, double &);

  void transformY(double, int &);
  void transformY(int, double &);
 
  void append(XYSeries *);
  void clear();
  bool empty() const;
};

inline double XYPlot::minXValue() const {
 return _xmin;
}

inline double XYPlot::maxXValue() const {
 return _xmax;
}

inline double XYPlot::stepXValue() const {
 return _xstep;
}

inline XYPlot::TicksMark XYPlot::ticksXAxis() const {
 return _xtick;
}

inline double XYPlot::minYValue() const {
 return _ymin;
}

inline double XYPlot::maxYValue() const {
 return _ymax;
}

inline double XYPlot::stepYValue() const {
 return _ystep;
}

inline XYPlot::TicksMark XYPlot::ticksYAxis() const {
 return _ytick;
}

inline QString XYPlot::titleXAxis() const {
 return _xtitle;
}

inline QString XYPlot::titleYAxis() const {
 return _ytitle;
}

inline void XYPlot::clear() {
 _series.clear();
}

inline bool XYPlot::empty() const {
 return _series.isEmpty();
}

#endif


/****************************************************************************
** Form interface generated from reading ui file 'xyplotgraphic.ui'
**
** Created: Tue Feb 21 21:29:31 2006
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef XYPLOTGRAPHIC_H
#define XYPLOTGRAPHIC_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>
#include "xyplot.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class XYPlot;

class XYPlotGraphic : public QDialog
{
    Q_OBJECT

public:
    XYPlotGraphic( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~XYPlotGraphic();

    XYPlot *plot;

protected:

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // XYPLOTGRAPHIC_H

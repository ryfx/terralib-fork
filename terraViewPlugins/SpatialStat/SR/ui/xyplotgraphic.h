/****************************************************************************
** Form interface generated from reading ui file 'ui\xyplotgraphic.ui'
**
** Created: seg 25. out 10:56:04 2010
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef XYPLOTGRAPHIC_H
#define XYPLOTGRAPHIC_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QFrame;

class XYPlotGraphic : public QDialog
{
    Q_OBJECT

public:
    XYPlotGraphic( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~XYPlotGraphic();

    QFrame* frame3;

protected:

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // XYPLOTGRAPHIC_H

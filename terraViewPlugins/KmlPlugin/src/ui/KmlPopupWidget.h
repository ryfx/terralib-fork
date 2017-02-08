/****************************************************************************
** Form interface generated from reading ui file 'ui/KmlPopupWidget.ui'
**
** Created: Ter Jun 7 11:19:54 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef KMLPOPUPWIDGET_H
#define KMLPOPUPWIDGET_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class TeQtThemeItem;
class TerraViewBase;

class KmlPopupWidget : public QWidget
{
    Q_OBJECT

public:
    KmlPopupWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~KmlPopupWidget();


public slots:
    virtual void init( TerraViewBase * tViewBase, TeQtThemeItem * themeItem, const int & x, const int & y );
    virtual void remove();
    virtual void rename();

protected:
    TerraViewBase* _tViewBase;
    TeQtThemeItem* _themeItem;


protected slots:
    virtual void languageChange();

};

#endif // KMLPOPUPWIDGET_H

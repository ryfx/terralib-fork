/****************************************************************************
** Form interface generated from reading ui file 'ui/ImportKmlWindow.ui'
**
** Created: Ter Jun 7 11:19:54 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef IMPORTKMLWINDOW_H
#define IMPORTKMLWINDOW_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QPushButton;
class QLineEdit;
class QLabel;
class QFrame;
class TeDatabase;
class TerraViewBase;

class ImportKmlWindow : public QDialog
{
    Q_OBJECT

public:
    ImportKmlWindow( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ImportKmlWindow();

    QGroupBox* groupBox1;
    QPushButton* filePushButton;
    QLineEdit* fileLineEdit;
    QLabel* textLabel1;
    QLineEdit* layerNameLineEdit;
    QFrame* line1;
    QPushButton* helpPushButton;
    QPushButton* importPushButton;
    QPushButton* closePushButton;

    TeDatabase* _database;
    TerraViewBase* _tViewBase;

public slots:
    virtual void filePushButton_clicked();
    virtual void importPushButton_clicked();
    virtual void closePushButton_clicked();
    virtual void init( TeDatabase * database, TerraViewBase * TViewBase );
    virtual void helpPushButton_clicked();

protected:
    QGridLayout* ImportKmlWindowLayout;
    QVBoxLayout* layout16;
    QGridLayout* groupBox1Layout;
    QHBoxLayout* layout1;
    QHBoxLayout* layout2;
    QHBoxLayout* layout15;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

};

#endif // IMPORTKMLWINDOW_H

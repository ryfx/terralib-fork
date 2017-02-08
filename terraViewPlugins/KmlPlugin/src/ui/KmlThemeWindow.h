/****************************************************************************
** Form interface generated from reading ui file 'ui/KmlThemeWindow.ui'
**
** Created: Ter Jun 7 11:19:54 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef KMLTHEMEWINDOW_H
#define KMLTHEMEWINDOW_H

#include <qvariant.h>
#include <qdialog.h>
#include <string>
#include <vector>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QPushButton;
class QLineEdit;
class QLabel;
class QComboBox;
class QFrame;
class TeDatabase;
class TerraViewBase;
class TeView;
class TeKmlTheme;
class TeTheme;
class TeAbstractTheme;

class KmlThemeWindow : public QDialog
{
    Q_OBJECT

public:
    KmlThemeWindow( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~KmlThemeWindow();

    QGroupBox* groupBox1;
    QPushButton* filePushButton;
    QLineEdit* fileLineEdit;
    QGroupBox* groupBox2;
    QLabel* textLabel1_2;
    QLabel* textLabel1;
    QComboBox* viewComboBox;
    QLineEdit* themeNameLineEdit;
    QFrame* line1;
    QPushButton* helpPushButton;
    QPushButton* createThemePushButton;
    QPushButton* closePushButton;

public slots:
    virtual void filePushButton_clicked();
    virtual void createThemePushButton_clicked();
    virtual void closePushButton_clicked();
    virtual void init( TeDatabase * database, TerraViewBase * tViewBase );
    virtual void viewComboBox_activated( const QString & );
    virtual void helpPushButton_clicked();

protected:
    TeKmlTheme* _kmlTheme;
    TeDatabase* _database;
    TerraViewBase* _tViewBase;

    virtual void loadViews();
    virtual bool hasThemeWithName( const std::string & themeName, TeView * view );

    QGridLayout* KmlThemeWindowLayout;
    QVBoxLayout* layout33;
    QGridLayout* groupBox1Layout;
    QHBoxLayout* layout1;
    QGridLayout* groupBox2Layout;
    QHBoxLayout* layout8;
    QVBoxLayout* layout7;
    QVBoxLayout* layout6;
    QHBoxLayout* layout32;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

};

#endif // KMLTHEMEWINDOW_H

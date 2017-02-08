/****************************************************************************
** Form interface generated from reading ui file 'ui/ExportKmlWindow.ui'
**
** Created: Ter Jun 7 11:19:54 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef EXPORTKMLWINDOW_H
#define EXPORTKMLWINDOW_H

#include <qvariant.h>
#include <qdialog.h>
#include <string>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QButtonGroup;
class QRadioButton;
class QPushButton;
class QGroupBox;
class QLabel;
class QComboBox;
class QLineEdit;
class TeDatabase;
class TeTheme;
class TeLayer;

class ExportKmlWindow : public QDialog
{
    Q_OBJECT

public:
    ExportKmlWindow( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ExportKmlWindow();

    QButtonGroup* buttonGroup;
    QRadioButton* layerRadioButton;
    QRadioButton* themeRadioButton;
    QPushButton* helpPushButton;
    QPushButton* exportPushButton;
    QPushButton* closePushButton;
    QGroupBox* groupBox2_2;
    QLabel* textLabel1_2;
    QLabel* textLabel2;
    QComboBox* nameComboBox;
    QLabel* textLabel2_2;
    QComboBox* descComboBox;
    QGroupBox* groupBox11;
    QPushButton* pathPushButton;
    QLineEdit* pathLineEdit;
    QLabel* viewTextLabel;
    QLabel* textLabel;
    QComboBox* viewComboBox;
    QComboBox* comboBox;

    virtual void init( TeDatabase * database );

public slots:
    virtual void pathPushButton_clicked();
    virtual void exportPushButton_clicked();
    virtual void closePushButton_clicked();
    virtual void buttonGroup_clicked( int id );
    virtual void comboBox_activated( const QString & );
    virtual void helpPushButton_clicked();
    virtual void viewComboBox_activated( const QString & );

protected:
    TeDatabase* _database;

    QGridLayout* ExportKmlWindowLayout;
    QGridLayout* buttonGroupLayout;
    QHBoxLayout* layout10;
    QHBoxLayout* layout9;
    QSpacerItem* spacer2;
    QGridLayout* groupBox2_2Layout;
    QVBoxLayout* layout15;
    QHBoxLayout* layout13;
    QHBoxLayout* layout14;
    QGridLayout* groupBox11Layout;
    QHBoxLayout* layout5;
    QHBoxLayout* layout12;
    QVBoxLayout* layout10_2;
    QVBoxLayout* layout11;

protected slots:
    virtual void languageChange();

private:
    virtual void loadThemes();
    virtual void loadLayers();
    virtual void loadThemeColumns();
    virtual void loadLayerColumns();
    virtual void loadViews();
    virtual void loadViewThemes();

};

#endif // EXPORTKMLWINDOW_H

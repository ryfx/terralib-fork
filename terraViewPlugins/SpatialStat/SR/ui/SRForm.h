/****************************************************************************
** Form interface generated from reading ui file 'ui\SRForm.ui'
**
** Created: seg 25. out 10:56:05 2010
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SR_H
#define SR_H

#include <qvariant.h>
#include <qmainwindow.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QGroupBox;
class QTextBrowser;
class QLabel;
class QCheckBox;
class QComboBox;
class QPushButton;
class QLineEdit;

class sr : public QMainWindow
{
    Q_OBJECT

public:
    sr( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~sr();

    QGroupBox* Output;
    QTextBrowser* Output_text;
    QGroupBox* timegroupBox;
    QLabel* textLabel1_2;
    QLabel* casesLabel;
    QLabel* textLabel1_3;
    QLabel* popLabel_2;
    QCheckBox* timecheckBox;
    QComboBox* timeUnitComboBox;
    QComboBox* tableComboBox;
    QComboBox* timeComboBox;
    QPushButton* okButton;
    QPushButton* quitButton;
    QGroupBox* InputgroupBox;
    QLabel* textLabel1;
    QLabel* textLabel1_4;
    QLabel* textLabel1_4_2;
    QLabel* textLabel1_4_3;
    QLineEdit* TresholdlineEdit;
    QLineEdit* RadiuslineEdit;
    QComboBox* themeComboBox;
    QLineEdit* EpsilonlineEdit;

public slots:
    virtual void temaComboBoxClickSlot(const QString&);
    virtual void okButtonClickSlot();
    virtual void tableComboBoxClickSlot(const QString&);
    virtual void destroy();
    virtual void CloseSlot();
    virtual void Enable_timeSlot();

protected:

protected slots:
    virtual void languageChange();

};

#endif // SR_H

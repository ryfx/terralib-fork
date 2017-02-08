/****************************************************************************
** Form interface generated from reading ui file 'ui\EBEbinForm.ui'
**
** Created: ter 26. out 14:35:57 2010
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef EBE_H
#define EBE_H

#include <qvariant.h>
#include <qmainwindow.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QLabel;
class QButtonGroup;
class QRadioButton;
class QGroupBox;
class QLineEdit;
class QComboBox;
class QCheckBox;
class QTextEdit;
class QPushButton;

class ebe : public QMainWindow
{
    Q_OBJECT

public:
    ebe( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~ebe();

    QLabel* AttibLabel;
    QLabel* themeLabel;
    QLabel* casesLabel;
    QButtonGroup* functypeButtonGroup;
    QRadioButton* localradioButton;
    QRadioButton* globalradioButton;
    QLabel* popLabel;
    QGroupBox* advancedgroupBox;
    QLabel* radiusLabel;
    QLineEdit* radiuslineEdit;
    QComboBox* themeComboBox;
    QComboBox* tableComboBox;
    QComboBox* populationComboBox;
    QComboBox* caseComboBox;
    QLabel* ConvertLabel;
    QCheckBox* ConvertCheckBox;
    QButtonGroup* OutpubuttonGroup;
    QTextEdit* Output_text;
    QPushButton* OKButton;
    QPushButton* quitButton;
    QPushButton* helpPushButton;

public slots:
    virtual void temaComboBoxClickSlot(const QString&);
    virtual void scanButtonClickSlot();
    virtual void tableComboBoxClickSlot(const QString&);
    virtual void destroy();
    virtual void Disable_advAttrSlot();
    virtual void Enable_matrixSlot();
    virtual void helpButtonSlot();

protected:

protected slots:
    virtual void languageChange();

};

#endif // EBE_H

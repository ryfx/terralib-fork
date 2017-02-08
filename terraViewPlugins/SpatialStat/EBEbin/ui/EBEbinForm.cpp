/****************************************************************************
** Form implementation generated from reading ui file 'ui\EBEbinForm.ui'
**
** Created: ter 26. out 14:35:57 2010
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "EBEbinForm.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a ebe as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
ebe::ebe( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    if ( !name )
	setName( "ebe" );
    setEnabled( TRUE );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 21, 21 ) );
    setMaximumSize( QSize( 1000, 1000 ) );
    setProperty( "modal", QVariant( TRUE, 0 ) );
    setProperty( "sizeGripEnabled", QVariant( FALSE, 0 ) );
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );

    AttibLabel = new QLabel( centralWidget(), "AttibLabel" );
    AttibLabel->setGeometry( QRect( 13, 66, 90, 30 ) );

    themeLabel = new QLabel( centralWidget(), "themeLabel" );
    themeLabel->setGeometry( QRect( 65, 30, 40, 30 ) );

    casesLabel = new QLabel( centralWidget(), "casesLabel" );
    casesLabel->setGeometry( QRect( 56, 150, 46, 26 ) );
    QFont casesLabel_font(  casesLabel->font() );
    casesLabel_font.setPointSize( 10 );
    casesLabel->setFont( casesLabel_font ); 

    functypeButtonGroup = new QButtonGroup( centralWidget(), "functypeButtonGroup" );
    functypeButtonGroup->setGeometry( QRect( 70, 199, 290, 50 ) );
    functypeButtonGroup->setExclusive( TRUE );

    localradioButton = new QRadioButton( functypeButtonGroup, "localradioButton" );
    localradioButton->setGeometry( QRect( 40, 20, 85, 20 ) );
    localradioButton->setChecked( TRUE );

    globalradioButton = new QRadioButton( functypeButtonGroup, "globalradioButton" );
    globalradioButton->setGeometry( QRect( 170, 20, 85, 20 ) );
    globalradioButton->setAcceptDrops( FALSE );

    popLabel = new QLabel( centralWidget(), "popLabel" );
    popLabel->setGeometry( QRect( 32, 110, 71, 26 ) );
    QFont popLabel_font(  popLabel->font() );
    popLabel_font.setPointSize( 10 );
    popLabel->setFont( popLabel_font ); 

    advancedgroupBox = new QGroupBox( centralWidget(), "advancedgroupBox" );
    advancedgroupBox->setEnabled( TRUE );
    advancedgroupBox->setGeometry( QRect( 70, 270, 290, 70 ) );
    advancedgroupBox->setAcceptDrops( FALSE );

    radiusLabel = new QLabel( advancedgroupBox, "radiusLabel" );
    radiusLabel->setGeometry( QRect( 100, 30, 30, 20 ) );

    radiuslineEdit = new QLineEdit( advancedgroupBox, "radiuslineEdit" );
    radiuslineEdit->setGeometry( QRect( 140, 30, 61, 20 ) );
    radiuslineEdit->setFrameShape( QLineEdit::LineEditPanel );
    radiuslineEdit->setFrameShadow( QLineEdit::Sunken );

    themeComboBox = new QComboBox( FALSE, centralWidget(), "themeComboBox" );
    themeComboBox->setGeometry( QRect( 112, 30, 270, 20 ) );

    tableComboBox = new QComboBox( FALSE, centralWidget(), "tableComboBox" );
    tableComboBox->setGeometry( QRect( 112, 70, 270, 20 ) );

    populationComboBox = new QComboBox( FALSE, centralWidget(), "populationComboBox" );
    populationComboBox->setGeometry( QRect( 111, 110, 270, 20 ) );

    caseComboBox = new QComboBox( FALSE, centralWidget(), "caseComboBox" );
    caseComboBox->setGeometry( QRect( 111, 150, 270, 20 ) );

    ConvertLabel = new QLabel( centralWidget(), "ConvertLabel" );
    ConvertLabel->setGeometry( QRect( 82, 390, 220, 26 ) );
    QFont ConvertLabel_font(  ConvertLabel->font() );
    ConvertLabel_font.setPointSize( 10 );
    ConvertLabel->setFont( ConvertLabel_font ); 

    ConvertCheckBox = new QCheckBox( centralWidget(), "ConvertCheckBox" );
    ConvertCheckBox->setGeometry( QRect( 306, 393, 20, 20 ) );

    OutpubuttonGroup = new QButtonGroup( centralWidget(), "OutpubuttonGroup" );
    OutpubuttonGroup->setGeometry( QRect( 400, 39, 270, 331 ) );

    Output_text = new QTextEdit( OutpubuttonGroup, "Output_text" );
    Output_text->setGeometry( QRect( 10, 21, 250, 300 ) );

    OKButton = new QPushButton( centralWidget(), "OKButton" );
    OKButton->setEnabled( TRUE );
    OKButton->setGeometry( QRect( 370, 410, 82, 29 ) );
    QFont OKButton_font(  OKButton->font() );
    OKButton_font.setPointSize( 10 );
    OKButton->setFont( OKButton_font ); 

    quitButton = new QPushButton( centralWidget(), "quitButton" );
    quitButton->setEnabled( TRUE );
    quitButton->setGeometry( QRect( 480, 410, 80, 30 ) );
    QFont quitButton_font(  quitButton->font() );
    quitButton_font.setPointSize( 10 );
    quitButton->setFont( quitButton_font ); 

    helpPushButton = new QPushButton( centralWidget(), "helpPushButton" );
    helpPushButton->setGeometry( QRect( 590, 410, 80, 30 ) );

    // toolbars

    languageChange();
    resize( QSize(690, 485).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( themeComboBox, SIGNAL( activated(const QString&) ), this, SLOT( temaComboBoxClickSlot(const QString&) ) );
    connect( tableComboBox, SIGNAL( activated(const QString&) ), this, SLOT( tableComboBoxClickSlot(const QString&) ) );
    connect( OKButton, SIGNAL( clicked() ), this, SLOT( scanButtonClickSlot() ) );
    connect( globalradioButton, SIGNAL( stateChanged(int) ), this, SLOT( Disable_advAttrSlot() ) );
    connect( quitButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( helpPushButton, SIGNAL( clicked() ), this, SLOT( helpButtonSlot() ) );

    // tab order
    setTabOrder( OKButton, quitButton );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ebe::~ebe()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ebe::languageChange()
{
    setCaption( tr( "Binomial Empirical Bayes " ) );
    AttibLabel->setText( tr( "Table of Attributes" ) );
    themeLabel->setText( tr( "Theme" ) );
    casesLabel->setText( tr( "Cases :" ) );
    functypeButtonGroup->setTitle( tr( "Function type" ) );
    localradioButton->setText( tr( "local EBE" ) );
    globalradioButton->setText( tr( "Global EBE" ) );
    popLabel->setText( tr( "Population :" ) );
    advancedgroupBox->setTitle( tr( "Local EBE setup" ) );
    radiusLabel->setText( tr( "radius" ) );
    ConvertLabel->setText( tr( "Convert Lat/Long coordinates to Km" ) );
    ConvertCheckBox->setText( QString::null );
    OutpubuttonGroup->setTitle( tr( "Output" ) );
    OKButton->setText( tr( "&OK" ) );
    OKButton->setAccel( QKeySequence( tr( "Alt+O" ) ) );
    quitButton->setText( tr( "&Quit" ) );
    quitButton->setAccel( QKeySequence( tr( "Alt+Q" ) ) );
    helpPushButton->setText( tr( "&Help" ) );
    helpPushButton->setAccel( QKeySequence( tr( "Alt+H" ) ) );
}

void ebe::temaComboBoxClickSlot(const QString&)
{
    qWarning( "ebe::temaComboBoxClickSlot(const QString&): Not implemented yet" );
}

void ebe::scanButtonClickSlot()
{
    qWarning( "ebe::scanButtonClickSlot(): Not implemented yet" );
}

void ebe::tableComboBoxClickSlot(const QString&)
{
    qWarning( "ebe::tableComboBoxClickSlot(const QString&): Not implemented yet" );
}

void ebe::destroy()
{
}

void ebe::Disable_advAttrSlot()
{
    qWarning( "ebe::Disable_advAttrSlot(): Not implemented yet" );
}

void ebe::Enable_matrixSlot()
{
    qWarning( "ebe::Enable_matrixSlot(): Not implemented yet" );
}

void ebe::helpButtonSlot()
{
    qWarning( "ebe::helpButtonSlot(): Not implemented yet" );
}


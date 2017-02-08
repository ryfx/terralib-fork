/****************************************************************************
** Form implementation generated from reading ui file 'ui\SRForm.ui'
**
** Created: seg 25. out 10:56:05 2010
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "SRForm.h"

#include <qvariant.h>
#include <qgroupbox.h>
#include <qtextbrowser.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
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
 *  Constructs a sr as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
sr::sr( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    if ( !name )
	setName( "sr" );
    setEnabled( TRUE );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 21, 21 ) );
    setMaximumSize( QSize( 1000, 1000 ) );
    setProperty( "modal", QVariant( TRUE, 0 ) );
    setProperty( "sizeGripEnabled", QVariant( FALSE, 0 ) );
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );

    Output = new QGroupBox( centralWidget(), "Output" );
    Output->setGeometry( QRect( 10, 120, 200, 170 ) );

    Output_text = new QTextBrowser( Output, "Output_text" );
    Output_text->setEnabled( TRUE );
    Output_text->setGeometry( QRect( 10, 20, 180, 140 ) );
    QFont Output_text_font(  Output_text->font() );
    Output_text_font.setPointSize( 10 );
    Output_text->setFont( Output_text_font ); 

    timegroupBox = new QGroupBox( centralWidget(), "timegroupBox" );
    timegroupBox->setGeometry( QRect( 220, 120, 280, 170 ) );

    textLabel1_2 = new QLabel( timegroupBox, "textLabel1_2" );
    textLabel1_2->setGeometry( QRect( 15, 80, 91, 30 ) );
    QFont textLabel1_2_font(  textLabel1_2->font() );
    textLabel1_2_font.setPointSize( 10 );
    textLabel1_2->setFont( textLabel1_2_font ); 

    casesLabel = new QLabel( timegroupBox, "casesLabel" );
    casesLabel->setGeometry( QRect( 67, 110, 40, 20 ) );
    QFont casesLabel_font(  casesLabel->font() );
    casesLabel_font.setPointSize( 10 );
    casesLabel->setFont( casesLabel_font ); 

    textLabel1_3 = new QLabel( timegroupBox, "textLabel1_3" );
    textLabel1_3->setGeometry( QRect( 10, 19, 60, 30 ) );
    QFont textLabel1_3_font(  textLabel1_3->font() );
    textLabel1_3_font.setPointSize( 9 );
    textLabel1_3->setFont( textLabel1_3_font ); 

    popLabel_2 = new QLabel( timegroupBox, "popLabel_2" );
    popLabel_2->setGeometry( QRect( 30, 60, 137, 17 ) );
    QFont popLabel_2_font(  popLabel_2->font() );
    popLabel_2_font.setPointSize( 10 );
    popLabel_2->setFont( popLabel_2_font ); 

    timecheckBox = new QCheckBox( timegroupBox, "timecheckBox" );
    timecheckBox->setGeometry( QRect( 10, 59, 20, 20 ) );

    timeUnitComboBox = new QComboBox( FALSE, timegroupBox, "timeUnitComboBox" );
    timeUnitComboBox->setGeometry( QRect( 80, 25, 70, 20 ) );

    tableComboBox = new QComboBox( FALSE, timegroupBox, "tableComboBox" );
    tableComboBox->setEnabled( FALSE );
    tableComboBox->setGeometry( QRect( 110, 84, 160, 20 ) );

    timeComboBox = new QComboBox( FALSE, timegroupBox, "timeComboBox" );
    timeComboBox->setEnabled( FALSE );
    timeComboBox->setGeometry( QRect( 110, 110, 160, 20 ) );

    okButton = new QPushButton( centralWidget(), "okButton" );
    okButton->setEnabled( TRUE );
    okButton->setGeometry( QRect( 130, 310, 82, 29 ) );
    QFont okButton_font(  okButton->font() );
    okButton_font.setPointSize( 10 );
    okButton->setFont( okButton_font ); 

    quitButton = new QPushButton( centralWidget(), "quitButton" );
    quitButton->setEnabled( TRUE );
    quitButton->setGeometry( QRect( 280, 310, 82, 29 ) );
    QFont quitButton_font(  quitButton->font() );
    quitButton_font.setPointSize( 10 );
    quitButton->setFont( quitButton_font ); 

    InputgroupBox = new QGroupBox( centralWidget(), "InputgroupBox" );
    InputgroupBox->setGeometry( QRect( 80, 10, 360, 100 ) );

    textLabel1 = new QLabel( InputgroupBox, "textLabel1" );
    textLabel1->setGeometry( QRect( 20, 20, 53, 30 ) );
    QFont textLabel1_font(  textLabel1->font() );
    textLabel1_font.setPointSize( 9 );
    textLabel1->setFont( textLabel1_font ); 

    textLabel1_4 = new QLabel( InputgroupBox, "textLabel1_4" );
    textLabel1_4->setGeometry( QRect( 20, 50, 53, 30 ) );
    QFont textLabel1_4_font(  textLabel1_4->font() );
    textLabel1_4_font.setPointSize( 9 );
    textLabel1_4->setFont( textLabel1_4_font ); 

    textLabel1_4_2 = new QLabel( InputgroupBox, "textLabel1_4_2" );
    textLabel1_4_2->setGeometry( QRect( 131, 50, 53, 30 ) );
    QFont textLabel1_4_2_font(  textLabel1_4_2->font() );
    textLabel1_4_2_font.setPointSize( 9 );
    textLabel1_4_2->setFont( textLabel1_4_2_font ); 

    textLabel1_4_3 = new QLabel( InputgroupBox, "textLabel1_4_3" );
    textLabel1_4_3->setGeometry( QRect( 240, 50, 64, 30 ) );
    QFont textLabel1_4_3_font(  textLabel1_4_3->font() );
    textLabel1_4_3_font.setPointSize( 9 );
    textLabel1_4_3->setFont( textLabel1_4_3_font ); 

    TresholdlineEdit = new QLineEdit( InputgroupBox, "TresholdlineEdit" );
    TresholdlineEdit->setGeometry( QRect( 302, 58, 40, 18 ) );
    TresholdlineEdit->setFrameShape( QLineEdit::LineEditPanel );
    TresholdlineEdit->setFrameShadow( QLineEdit::Sunken );

    RadiuslineEdit = new QLineEdit( InputgroupBox, "RadiuslineEdit" );
    RadiuslineEdit->setGeometry( QRect( 71, 56, 40, 18 ) );

    themeComboBox = new QComboBox( FALSE, InputgroupBox, "themeComboBox" );
    themeComboBox->setGeometry( QRect( 72, 26, 270, 20 ) );

    EpsilonlineEdit = new QLineEdit( InputgroupBox, "EpsilonlineEdit" );
    EpsilonlineEdit->setGeometry( QRect( 183, 57, 40, 18 ) );

    // toolbars

    languageChange();
    resize( QSize(523, 374).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( themeComboBox, SIGNAL( activated(const QString&) ), this, SLOT( temaComboBoxClickSlot(const QString&) ) );
    connect( okButton, SIGNAL( clicked() ), this, SLOT( okButtonClickSlot() ) );
    connect( quitButton, SIGNAL( clicked() ), this, SLOT( CloseSlot() ) );
    connect( timecheckBox, SIGNAL( clicked() ), this, SLOT( Enable_timeSlot() ) );

    // tab order
    setTabOrder( themeComboBox, RadiuslineEdit );
    setTabOrder( RadiuslineEdit, EpsilonlineEdit );
    setTabOrder( EpsilonlineEdit, TresholdlineEdit );
    setTabOrder( TresholdlineEdit, timecheckBox );
    setTabOrder( timecheckBox, timeUnitComboBox );
    setTabOrder( timeUnitComboBox, tableComboBox );
    setTabOrder( tableComboBox, timeComboBox );
    setTabOrder( timeComboBox, Output_text );
    setTabOrder( Output_text, okButton );
    setTabOrder( okButton, quitButton );
}

/*
 *  Destroys the object and frees any allocated resources
 */
sr::~sr()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void sr::languageChange()
{
    setCaption( tr( "Surveillance System SR" ) );
    Output->setTitle( tr( "Output" ) );
    timegroupBox->setTitle( tr( "Time Selection" ) );
    textLabel1_2->setText( tr( "Attribute Table:" ) );
    casesLabel->setText( tr( "Time :" ) );
    textLabel1_3->setText( tr( "<font size=\"+1\"><font>Time Unit:</font></font>" ) );
    popLabel_2->setText( tr( "Enable Time Selection" ) );
    timecheckBox->setText( tr( "checkBox2" ) );
    timeUnitComboBox->clear();
    timeUnitComboBox->insertItem( tr( "Hour" ) );
    timeUnitComboBox->insertItem( tr( "Day" ) );
    timeUnitComboBox->insertItem( tr( "Month" ) );
    timeUnitComboBox->insertItem( tr( "Year" ) );
    timeUnitComboBox->setCurrentItem( 0 );
    okButton->setText( tr( "&OK" ) );
    okButton->setAccel( QKeySequence( tr( "Alt+O" ) ) );
    quitButton->setText( tr( "&Quit" ) );
    quitButton->setAccel( QKeySequence( tr( "Alt+Q" ) ) );
    InputgroupBox->setTitle( tr( "Input" ) );
    textLabel1->setText( tr( "<font size=\"+1\"><font>Theme:</font></font>" ) );
    textLabel1_4->setText( tr( "<font size=\"+1\"><font>Radius:</font></font>" ) );
    textLabel1_4_2->setText( tr( "<font size=\"+1\"><font>Epsilon:</font></font>" ) );
    textLabel1_4_3->setText( tr( "<font size=\"+1\"><font>Treshold:</font></font>" ) );
}

void sr::temaComboBoxClickSlot(const QString&)
{
    qWarning( "sr::temaComboBoxClickSlot(const QString&): Not implemented yet" );
}

void sr::okButtonClickSlot()
{
    qWarning( "sr::okButtonClickSlot(): Not implemented yet" );
}

void sr::tableComboBoxClickSlot(const QString&)
{
    qWarning( "sr::tableComboBoxClickSlot(const QString&): Not implemented yet" );
}

void sr::destroy()
{
}

void sr::CloseSlot()
{
    qWarning( "sr::CloseSlot(): Not implemented yet" );
}

void sr::Enable_timeSlot()
{
    qWarning( "sr::Enable_timeSlot(): Not implemented yet" );
}


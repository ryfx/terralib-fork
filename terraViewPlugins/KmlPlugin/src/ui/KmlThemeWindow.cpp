/****************************************************************************
** Form implementation generated from reading ui file 'ui/KmlThemeWindow.ui'
**
** Created: Ter Jun 7 11:20:35 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "KmlThemeWindow.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../../ui/KmlThemeWindow.ui.h"
/*
 *  Constructs a KmlThemeWindow as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
KmlThemeWindow::KmlThemeWindow( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "KmlThemeWindow" );
    KmlThemeWindowLayout = new QGridLayout( this, 1, 1, 11, 6, "KmlThemeWindowLayout"); 

    layout33 = new QVBoxLayout( 0, 0, 6, "layout33"); 

    groupBox1 = new QGroupBox( this, "groupBox1" );
    groupBox1->setColumnLayout(0, Qt::Vertical );
    groupBox1->layout()->setSpacing( 6 );
    groupBox1->layout()->setMargin( 11 );
    groupBox1Layout = new QGridLayout( groupBox1->layout() );
    groupBox1Layout->setAlignment( Qt::AlignTop );

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 

    filePushButton = new QPushButton( groupBox1, "filePushButton" );
    layout1->addWidget( filePushButton );

    fileLineEdit = new QLineEdit( groupBox1, "fileLineEdit" );
    fileLineEdit->setFrameShape( QLineEdit::LineEditPanel );
    fileLineEdit->setFrameShadow( QLineEdit::Sunken );
    fileLineEdit->setReadOnly( TRUE );
    layout1->addWidget( fileLineEdit );

    groupBox1Layout->addLayout( layout1, 0, 0 );
    layout33->addWidget( groupBox1 );

    groupBox2 = new QGroupBox( this, "groupBox2" );
    groupBox2->setColumnLayout(0, Qt::Vertical );
    groupBox2->layout()->setSpacing( 6 );
    groupBox2->layout()->setMargin( 11 );
    groupBox2Layout = new QGridLayout( groupBox2->layout() );
    groupBox2Layout->setAlignment( Qt::AlignTop );

    layout8 = new QHBoxLayout( 0, 0, 6, "layout8"); 

    layout7 = new QVBoxLayout( 0, 0, 6, "layout7"); 

    textLabel1_2 = new QLabel( groupBox2, "textLabel1_2" );
    layout7->addWidget( textLabel1_2 );

    textLabel1 = new QLabel( groupBox2, "textLabel1" );
    textLabel1->setMaximumSize( QSize( 80, 32767 ) );
    layout7->addWidget( textLabel1 );
    layout8->addLayout( layout7 );

    layout6 = new QVBoxLayout( 0, 0, 6, "layout6"); 

    viewComboBox = new QComboBox( FALSE, groupBox2, "viewComboBox" );
    layout6->addWidget( viewComboBox );

    themeNameLineEdit = new QLineEdit( groupBox2, "themeNameLineEdit" );
    layout6->addWidget( themeNameLineEdit );
    layout8->addLayout( layout6 );

    groupBox2Layout->addLayout( layout8, 0, 0 );
    layout33->addWidget( groupBox2 );

    line1 = new QFrame( this, "line1" );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );
    layout33->addWidget( line1 );

    layout32 = new QHBoxLayout( 0, 0, 6, "layout32"); 

    helpPushButton = new QPushButton( this, "helpPushButton" );
    layout32->addWidget( helpPushButton );
    spacer1 = new QSpacerItem( 82, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout32->addItem( spacer1 );

    createThemePushButton = new QPushButton( this, "createThemePushButton" );
    layout32->addWidget( createThemePushButton );

    closePushButton = new QPushButton( this, "closePushButton" );
    layout32->addWidget( closePushButton );
    layout33->addLayout( layout32 );

    KmlThemeWindowLayout->addLayout( layout33, 0, 0 );
    languageChange();
    resize( QSize(315, 211).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( filePushButton, SIGNAL( clicked() ), this, SLOT( filePushButton_clicked() ) );
    connect( createThemePushButton, SIGNAL( clicked() ), this, SLOT( createThemePushButton_clicked() ) );
    connect( closePushButton, SIGNAL( clicked() ), this, SLOT( closePushButton_clicked() ) );
    connect( helpPushButton, SIGNAL( clicked() ), this, SLOT( helpPushButton_clicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
KmlThemeWindow::~KmlThemeWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KmlThemeWindow::languageChange()
{
    setCaption( tr( "KML Theme" ) );
    groupBox1->setTitle( tr( "KML File" ) );
    filePushButton->setText( tr( "&File..." ) );
    filePushButton->setAccel( QKeySequence( tr( "Alt+F" ) ) );
    groupBox2->setTitle( tr( "New Theme" ) );
    textLabel1_2->setText( tr( "View:" ) );
    textLabel1->setText( tr( "Theme Name:" ) );
    helpPushButton->setText( tr( "&Help" ) );
    helpPushButton->setAccel( QKeySequence( tr( "Alt+H" ) ) );
    createThemePushButton->setText( tr( "Create &Theme" ) );
    createThemePushButton->setAccel( QKeySequence( tr( "Alt+T" ) ) );
    closePushButton->setText( tr( "&Close" ) );
    closePushButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}


/****************************************************************************
** Form implementation generated from reading ui file 'ui/ImportKmlWindow.ui'
**
** Created: Ter Jun 7 11:20:31 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "ImportKmlWindow.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../../ui/ImportKmlWindow.ui.h"
/*
 *  Constructs a ImportKmlWindow as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ImportKmlWindow::ImportKmlWindow( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ImportKmlWindow" );
    ImportKmlWindowLayout = new QGridLayout( this, 1, 1, 11, 6, "ImportKmlWindowLayout"); 
    ImportKmlWindowLayout->setResizeMode( QLayout::Fixed );

    layout16 = new QVBoxLayout( 0, 0, 6, "layout16"); 

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
    layout16->addWidget( groupBox1 );

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 

    textLabel1 = new QLabel( this, "textLabel1" );
    layout2->addWidget( textLabel1 );

    layerNameLineEdit = new QLineEdit( this, "layerNameLineEdit" );
    layout2->addWidget( layerNameLineEdit );
    layout16->addLayout( layout2 );

    line1 = new QFrame( this, "line1" );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );
    layout16->addWidget( line1 );

    layout15 = new QHBoxLayout( 0, 0, 6, "layout15"); 

    helpPushButton = new QPushButton( this, "helpPushButton" );
    layout15->addWidget( helpPushButton );
    spacer1 = new QSpacerItem( 62, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout15->addItem( spacer1 );

    importPushButton = new QPushButton( this, "importPushButton" );
    layout15->addWidget( importPushButton );

    closePushButton = new QPushButton( this, "closePushButton" );
    layout15->addWidget( closePushButton );
    layout16->addLayout( layout15 );

    ImportKmlWindowLayout->addLayout( layout16, 0, 0 );
    languageChange();
    resize( QSize(313, 154).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( filePushButton, SIGNAL( clicked() ), this, SLOT( filePushButton_clicked() ) );
    connect( importPushButton, SIGNAL( clicked() ), this, SLOT( importPushButton_clicked() ) );
    connect( closePushButton, SIGNAL( clicked() ), this, SLOT( closePushButton_clicked() ) );
    connect( helpPushButton, SIGNAL( clicked() ), this, SLOT( helpPushButton_clicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ImportKmlWindow::~ImportKmlWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ImportKmlWindow::languageChange()
{
    setCaption( tr( "Import Kml" ) );
    groupBox1->setTitle( tr( "File" ) );
    filePushButton->setText( tr( "&File..." ) );
    filePushButton->setAccel( QKeySequence( tr( "Alt+F" ) ) );
    fileLineEdit->setText( QString::null );
    textLabel1->setText( tr( "Layer Name:" ) );
    helpPushButton->setText( tr( "&Help" ) );
    helpPushButton->setAccel( QKeySequence( tr( "Alt+H" ) ) );
    importPushButton->setText( tr( "&Import" ) );
    importPushButton->setAccel( QKeySequence( tr( "Alt+I" ) ) );
    closePushButton->setText( tr( "&Close" ) );
    closePushButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}


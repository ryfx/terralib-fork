/****************************************************************************
** Form implementation generated from reading ui file 'ui/ExportKmlWindow.ui'
**
** Created: Ter Jun 7 11:20:33 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "ExportKmlWindow.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../../ui/ExportKmlWindow.ui.h"
/*
 *  Constructs a ExportKmlWindow as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ExportKmlWindow::ExportKmlWindow( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ExportKmlWindow" );
    ExportKmlWindowLayout = new QGridLayout( this, 1, 1, 11, 6, "ExportKmlWindowLayout"); 

    buttonGroup = new QButtonGroup( this, "buttonGroup" );
    buttonGroup->setColumnLayout(0, Qt::Vertical );
    buttonGroup->layout()->setSpacing( 6 );
    buttonGroup->layout()->setMargin( 11 );
    buttonGroupLayout = new QGridLayout( buttonGroup->layout() );
    buttonGroupLayout->setAlignment( Qt::AlignTop );

    layout10 = new QHBoxLayout( 0, 0, 6, "layout10"); 

    layerRadioButton = new QRadioButton( buttonGroup, "layerRadioButton" );
    layerRadioButton->setChecked( TRUE );
    layout10->addWidget( layerRadioButton );

    themeRadioButton = new QRadioButton( buttonGroup, "themeRadioButton" );
    layout10->addWidget( themeRadioButton );

    buttonGroupLayout->addLayout( layout10, 0, 0 );

    ExportKmlWindowLayout->addWidget( buttonGroup, 0, 0 );

    layout9 = new QHBoxLayout( 0, 0, 6, "layout9"); 

    helpPushButton = new QPushButton( this, "helpPushButton" );
    layout9->addWidget( helpPushButton );
    spacer2 = new QSpacerItem( 72, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout9->addItem( spacer2 );

    exportPushButton = new QPushButton( this, "exportPushButton" );
    layout9->addWidget( exportPushButton );

    closePushButton = new QPushButton( this, "closePushButton" );
    layout9->addWidget( closePushButton );

    ExportKmlWindowLayout->addLayout( layout9, 3, 0 );

    groupBox2_2 = new QGroupBox( this, "groupBox2_2" );
    groupBox2_2->setColumnLayout(0, Qt::Vertical );
    groupBox2_2->layout()->setSpacing( 6 );
    groupBox2_2->layout()->setMargin( 11 );
    groupBox2_2Layout = new QGridLayout( groupBox2_2->layout() );
    groupBox2_2Layout->setAlignment( Qt::AlignTop );

    layout15 = new QVBoxLayout( 0, 0, 6, "layout15"); 

    textLabel1_2 = new QLabel( groupBox2_2, "textLabel1_2" );
    layout15->addWidget( textLabel1_2 );

    layout13 = new QHBoxLayout( 0, 0, 6, "layout13"); 

    textLabel2 = new QLabel( groupBox2_2, "textLabel2" );
    textLabel2->setMinimumSize( QSize( 70, 0 ) );
    textLabel2->setMaximumSize( QSize( 70, 32767 ) );
    layout13->addWidget( textLabel2 );

    nameComboBox = new QComboBox( FALSE, groupBox2_2, "nameComboBox" );
    layout13->addWidget( nameComboBox );
    layout15->addLayout( layout13 );

    layout14 = new QHBoxLayout( 0, 0, 6, "layout14"); 

    textLabel2_2 = new QLabel( groupBox2_2, "textLabel2_2" );
    textLabel2_2->setMinimumSize( QSize( 70, 0 ) );
    textLabel2_2->setMaximumSize( QSize( 70, 32767 ) );
    layout14->addWidget( textLabel2_2 );

    descComboBox = new QComboBox( FALSE, groupBox2_2, "descComboBox" );
    layout14->addWidget( descComboBox );
    layout15->addLayout( layout14 );

    groupBox2_2Layout->addLayout( layout15, 0, 0 );

    ExportKmlWindowLayout->addWidget( groupBox2_2, 2, 0 );

    groupBox11 = new QGroupBox( this, "groupBox11" );
    groupBox11->setColumnLayout(0, Qt::Vertical );
    groupBox11->layout()->setSpacing( 6 );
    groupBox11->layout()->setMargin( 11 );
    groupBox11Layout = new QGridLayout( groupBox11->layout() );
    groupBox11Layout->setAlignment( Qt::AlignTop );

    layout5 = new QHBoxLayout( 0, 0, 6, "layout5"); 

    pathPushButton = new QPushButton( groupBox11, "pathPushButton" );
    layout5->addWidget( pathPushButton );

    pathLineEdit = new QLineEdit( groupBox11, "pathLineEdit" );
    pathLineEdit->setFrameShape( QLineEdit::LineEditPanel );
    pathLineEdit->setFrameShadow( QLineEdit::Sunken );
    pathLineEdit->setReadOnly( TRUE );
    layout5->addWidget( pathLineEdit );

    groupBox11Layout->addLayout( layout5, 1, 0 );

    layout12 = new QHBoxLayout( 0, 0, 6, "layout12"); 

    layout10_2 = new QVBoxLayout( 0, 0, 6, "layout10_2"); 

    viewTextLabel = new QLabel( groupBox11, "viewTextLabel" );
    viewTextLabel->setEnabled( FALSE );
    viewTextLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, viewTextLabel->sizePolicy().hasHeightForWidth() ) );
    viewTextLabel->setMaximumSize( QSize( 80, 32767 ) );
    layout10_2->addWidget( viewTextLabel );

    textLabel = new QLabel( groupBox11, "textLabel" );
    textLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, textLabel->sizePolicy().hasHeightForWidth() ) );
    textLabel->setMaximumSize( QSize( 80, 32767 ) );
    layout10_2->addWidget( textLabel );
    layout12->addLayout( layout10_2 );

    layout11 = new QVBoxLayout( 0, 0, 6, "layout11"); 

    viewComboBox = new QComboBox( FALSE, groupBox11, "viewComboBox" );
    viewComboBox->setEnabled( FALSE );
    viewComboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, viewComboBox->sizePolicy().hasHeightForWidth() ) );
    viewComboBox->setMaximumSize( QSize( 32767, 20 ) );
    layout11->addWidget( viewComboBox );

    comboBox = new QComboBox( FALSE, groupBox11, "comboBox" );
    comboBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, comboBox->sizePolicy().hasHeightForWidth() ) );
    comboBox->setMaximumSize( QSize( 32767, 20 ) );
    layout11->addWidget( comboBox );
    layout12->addLayout( layout11 );

    groupBox11Layout->addLayout( layout12, 0, 0 );

    ExportKmlWindowLayout->addWidget( groupBox11, 1, 0 );
    languageChange();
    resize( QSize(289, 315).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pathPushButton, SIGNAL( clicked() ), this, SLOT( pathPushButton_clicked() ) );
    connect( exportPushButton, SIGNAL( clicked() ), this, SLOT( exportPushButton_clicked() ) );
    connect( closePushButton, SIGNAL( clicked() ), this, SLOT( closePushButton_clicked() ) );
    connect( buttonGroup, SIGNAL( clicked(int) ), this, SLOT( buttonGroup_clicked(int) ) );
    connect( comboBox, SIGNAL( activated(const QString&) ), this, SLOT( comboBox_activated(const QString&) ) );
    connect( helpPushButton, SIGNAL( clicked() ), this, SLOT( helpPushButton_clicked() ) );
    connect( viewComboBox, SIGNAL( activated(const QString&) ), this, SLOT( viewComboBox_activated(const QString&) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ExportKmlWindow::~ExportKmlWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ExportKmlWindow::languageChange()
{
    setCaption( tr( "Export Kml" ) );
    buttonGroup->setTitle( tr( "Options" ) );
    layerRadioButton->setText( tr( "Layer" ) );
    themeRadioButton->setText( tr( "Theme" ) );
    helpPushButton->setText( tr( "&Help" ) );
    helpPushButton->setAccel( QKeySequence( tr( "Alt+H" ) ) );
    exportPushButton->setText( tr( "&Export" ) );
    exportPushButton->setAccel( QKeySequence( tr( "Alt+E" ) ) );
    closePushButton->setText( tr( "&Close" ) );
    closePushButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
    groupBox2_2->setTitle( tr( "Attribute Column" ) );
    textLabel1_2->setText( tr( "Select the column for:" ) );
    textLabel2->setText( tr( "Name:" ) );
    nameComboBox->clear();
    nameComboBox->insertItem( QString::null );
    textLabel2_2->setText( tr( "Description:" ) );
    descComboBox->clear();
    descComboBox->insertItem( QString::null );
    groupBox11->setTitle( QString::null );
    pathPushButton->setText( tr( "&Directory..." ) );
    pathPushButton->setAccel( QKeySequence( tr( "Alt+D" ) ) );
    pathLineEdit->setText( QString::null );
    viewTextLabel->setText( tr( "View:" ) );
    textLabel->setText( tr( "Layer:" ) );
}


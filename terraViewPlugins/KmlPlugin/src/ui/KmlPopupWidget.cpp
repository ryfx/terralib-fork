/****************************************************************************
** Form implementation generated from reading ui file 'ui/KmlPopupWidget.ui'
**
** Created: Ter Jun 7 11:20:38 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "KmlPopupWidget.h"

#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../../ui/KmlPopupWidget.ui.h"
/*
 *  Constructs a KmlPopupWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
KmlPopupWidget::KmlPopupWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "KmlPopupWidget" );
    languageChange();
    resize( QSize(600, 480).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
KmlPopupWidget::~KmlPopupWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KmlPopupWidget::languageChange()
{
    setCaption( tr( "Form1" ) );
}


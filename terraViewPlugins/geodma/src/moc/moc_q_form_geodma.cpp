/****************************************************************************
** q_form_geodma meta object code from reading C++ file 'q_form_geodma.h'
**
** Created: Mon Dec 13 16:01:43 2010
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../ui/q_form_geodma.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *q_form_geodma::className() const
{
    return "q_form_geodma";
}

QMetaObject *q_form_geodma::metaObj = 0;
static QMetaObjectCleanUp cleanUp_q_form_geodma( "q_form_geodma", &q_form_geodma::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString q_form_geodma::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "q_form_geodma", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString q_form_geodma::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "q_form_geodma", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* q_form_geodma::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "languageChange()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"q_form_geodma", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_q_form_geodma.setMetaObject( metaObj );
    return metaObj;
}

void* q_form_geodma::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "q_form_geodma" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool q_form_geodma::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: languageChange(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool q_form_geodma::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool q_form_geodma::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool q_form_geodma::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

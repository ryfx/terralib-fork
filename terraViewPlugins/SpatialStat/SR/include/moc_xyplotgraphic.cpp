/****************************************************************************
** XYPlotGraphic meta object code from reading C++ file 'xyplotgraphic.h'
**
** Created: seg 25. out 10:56:05 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "xyplotgraphic.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *XYPlotGraphic::className() const
{
    return "XYPlotGraphic";
}

QMetaObject *XYPlotGraphic::metaObj = 0;
static QMetaObjectCleanUp cleanUp_XYPlotGraphic( "XYPlotGraphic", &XYPlotGraphic::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString XYPlotGraphic::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "XYPlotGraphic", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString XYPlotGraphic::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "XYPlotGraphic", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* XYPlotGraphic::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "languageChange()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"XYPlotGraphic", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_XYPlotGraphic.setMetaObject( metaObj );
    return metaObj;
}

void* XYPlotGraphic::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "XYPlotGraphic" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool XYPlotGraphic::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool XYPlotGraphic::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool XYPlotGraphic::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool XYPlotGraphic::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

/****************************************************************************
** XYSeries meta object code from reading C++ file 'xyseries.h'
**
** Created: seg 25. out 10:56:05 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "xyseries.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#include <qvariant.h>
const char *XYSeries::className() const
{
    return "XYSeries";
}

QMetaObject *XYSeries::metaObj = 0;
static QMetaObjectCleanUp cleanUp_XYSeries( "XYSeries", &XYSeries::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString XYSeries::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "XYSeries", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString XYSeries::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "XYSeries", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* XYSeries::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
#ifndef QT_NO_PROPERTIES
    static const QMetaProperty props_tbl[1] = {
 	{ "QColor","color", 0xa000103, &XYSeries::metaObj, 0, -1 }
    };
#endif // QT_NO_PROPERTIES
    metaObj = QMetaObject::new_metaobject(
	"XYSeries", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	props_tbl, 1,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_XYSeries.setMetaObject( metaObj );
    return metaObj;
}

void* XYSeries::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "XYSeries" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool XYSeries::qt_invoke( int _id, QUObject* _o )
{
    return QObject::qt_invoke(_id,_o);
}

bool XYSeries::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool XYSeries::qt_property( int id, int f, QVariant* v)
{
    switch ( id - staticMetaObject()->propertyOffset() ) {
    case 0: switch( f ) {
	case 0: setColor(v->asColor()); break;
	case 1: *v = QVariant( this->color() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    default:
	return QObject::qt_property( id, f, v );
    }
    return TRUE;
}

bool XYSeries::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

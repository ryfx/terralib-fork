/****************************************************************************
** geodma_plugin meta object code from reading C++ file 'geodma_plugin.h'
**
** Created: seg 7. nov 09:31:02 2011
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../include/geodma_plugin.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *geodma_plugin::className() const
{
    return "geodma_plugin";
}

QMetaObject *geodma_plugin::metaObj = 0;
static QMetaObjectCleanUp cleanUp_geodma_plugin( "geodma_plugin", &geodma_plugin::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString geodma_plugin::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "geodma_plugin", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString geodma_plugin::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "geodma_plugin", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* geodma_plugin::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"execute_plugin", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "execute_plugin()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"geodma_plugin", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_geodma_plugin.setMetaObject( metaObj );
    return metaObj;
}

void* geodma_plugin::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "geodma_plugin" ) )
	return this;
    if ( !qstrcmp( clname, "TViewAbstractPlugin" ) )
	return (TViewAbstractPlugin*)this;
    return QObject::qt_cast( clname );
}

bool geodma_plugin::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: execute_plugin(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool geodma_plugin::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool geodma_plugin::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool geodma_plugin::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

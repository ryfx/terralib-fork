/****************************************************************************
** EBEbinPlugin meta object code from reading C++ file 'ebebinplugin.h'
**
** Created: ter 26. out 14:35:57 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ebebinplugin.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *EBEbinPlugin::className() const
{
    return "EBEbinPlugin";
}

QMetaObject *EBEbinPlugin::metaObj = 0;
static QMetaObjectCleanUp cleanUp_EBEbinPlugin( "EBEbinPlugin", &EBEbinPlugin::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString EBEbinPlugin::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EBEbinPlugin", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString EBEbinPlugin::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EBEbinPlugin", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* EBEbinPlugin::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"showWindow", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "showWindow()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"EBEbinPlugin", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_EBEbinPlugin.setMetaObject( metaObj );
    return metaObj;
}

void* EBEbinPlugin::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "EBEbinPlugin" ) )
	return this;
    if ( !qstrcmp( clname, "TViewAbstractPlugin" ) )
	return (TViewAbstractPlugin*)this;
    return QObject::qt_cast( clname );
}

bool EBEbinPlugin::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: showWindow(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool EBEbinPlugin::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool EBEbinPlugin::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool EBEbinPlugin::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

/****************************************************************************
** KmlPlugin meta object code from reading C++ file 'kmlplugin.h'
**
** Created: Tue Jun 7 11:20:41 2011
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../include/kmlplugin.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *KmlPlugin::className() const
{
    return "KmlPlugin";
}

QMetaObject *KmlPlugin::metaObj = 0;
static QMetaObjectCleanUp cleanUp_KmlPlugin( "KmlPlugin", &KmlPlugin::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString KmlPlugin::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KmlPlugin", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString KmlPlugin::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KmlPlugin", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* KmlPlugin::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"showImportWindow", 0, 0 };
    static const QUMethod slot_1 = {"showExportWindow", 0, 0 };
    static const QUMethod slot_2 = {"showKmlThemeWindow", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "showImportWindow()", &slot_0, QMetaData::Protected },
	{ "showExportWindow()", &slot_1, QMetaData::Protected },
	{ "showKmlThemeWindow()", &slot_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"KmlPlugin", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_KmlPlugin.setMetaObject( metaObj );
    return metaObj;
}

void* KmlPlugin::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "KmlPlugin" ) )
	return this;
    if ( !qstrcmp( clname, "TViewAbstractPlugin" ) )
	return (TViewAbstractPlugin*)this;
    return QObject::qt_cast( clname );
}

bool KmlPlugin::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: showImportWindow(); break;
    case 1: showExportWindow(); break;
    case 2: showKmlThemeWindow(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool KmlPlugin::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool KmlPlugin::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool KmlPlugin::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

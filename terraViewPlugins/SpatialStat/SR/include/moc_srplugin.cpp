/****************************************************************************
** SRPlugin meta object code from reading C++ file 'srplugin.h'
**
** Created: seg 25. out 10:56:05 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "srplugin.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SRPlugin::className() const
{
    return "SRPlugin";
}

QMetaObject *SRPlugin::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SRPlugin( "SRPlugin", &SRPlugin::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SRPlugin::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SRPlugin", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SRPlugin::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SRPlugin", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SRPlugin::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"SRPlugin", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SRPlugin.setMetaObject( metaObj );
    return metaObj;
}

void* SRPlugin::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SRPlugin" ) )
	return this;
    if ( !qstrcmp( clname, "TViewAbstractPlugin" ) )
	return (TViewAbstractPlugin*)this;
    return QObject::qt_cast( clname );
}

bool SRPlugin::qt_invoke( int _id, QUObject* _o )
{
    return QObject::qt_invoke(_id,_o);
}

bool SRPlugin::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SRPlugin::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool SRPlugin::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

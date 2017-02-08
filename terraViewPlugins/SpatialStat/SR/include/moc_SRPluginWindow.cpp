/****************************************************************************
** SRPluginWindow meta object code from reading C++ file 'SRPluginWindow.h'
**
** Created: seg 25. out 10:56:05 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "SRPluginWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SRPluginWindow::className() const
{
    return "SRPluginWindow";
}

QMetaObject *SRPluginWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SRPluginWindow( "SRPluginWindow", &SRPluginWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SRPluginWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SRPluginWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SRPluginWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SRPluginWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SRPluginWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = sr::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"temaComboBoxClickSlot", 1, param_slot_0 };
    static const QUMethod slot_1 = {"Enable_timeSlot", 0, 0 };
    static const QUMethod slot_2 = {"CloseSlot", 0, 0 };
    static const QUMethod slot_3 = {"okButtonClickSlot", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"tableComboBoxClickSlot", 1, param_slot_4 };
    static const QMetaData slot_tbl[] = {
	{ "temaComboBoxClickSlot(const QString&)", &slot_0, QMetaData::Public },
	{ "Enable_timeSlot()", &slot_1, QMetaData::Public },
	{ "CloseSlot()", &slot_2, QMetaData::Public },
	{ "okButtonClickSlot()", &slot_3, QMetaData::Public },
	{ "tableComboBoxClickSlot(const QString&)", &slot_4, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"SRPluginWindow", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SRPluginWindow.setMetaObject( metaObj );
    return metaObj;
}

void* SRPluginWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SRPluginWindow" ) )
	return this;
    return sr::qt_cast( clname );
}

bool SRPluginWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: temaComboBoxClickSlot((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: Enable_timeSlot(); break;
    case 2: CloseSlot(); break;
    case 3: okButtonClickSlot(); break;
    case 4: tableComboBoxClickSlot((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return sr::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SRPluginWindow::qt_emit( int _id, QUObject* _o )
{
    return sr::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SRPluginWindow::qt_property( int id, int f, QVariant* v)
{
    return sr::qt_property( id, f, v);
}

bool SRPluginWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

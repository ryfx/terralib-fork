/****************************************************************************
** EBEbinPluginWindow meta object code from reading C++ file 'EBEBinPluginWindow.h'
**
** Created: ter 26. out 14:35:57 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "EBEBinPluginWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *EBEbinPluginWindow::className() const
{
    return "EBEbinPluginWindow";
}

QMetaObject *EBEbinPluginWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_EBEbinPluginWindow( "EBEbinPluginWindow", &EBEbinPluginWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString EBEbinPluginWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EBEbinPluginWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString EBEbinPluginWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EBEbinPluginWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* EBEbinPluginWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ebe::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"temaComboBoxClickSlot", 1, param_slot_0 };
    static const QUMethod slot_1 = {"Disable_advAttrSlot", 0, 0 };
    static const QUMethod slot_2 = {"scanButtonClickSlot", 0, 0 };
    static const QUMethod slot_3 = {"helpButtonSlot", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"tableComboBoxClickSlot", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "pluginParams", &static_QUType_ptr, "PluginParameters", QUParameter::In }
    };
    static const QUMethod slot_5 = {"updateForm", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ 0, &static_QUType_ptr, "QHideEvent", QUParameter::In }
    };
    static const QUMethod slot_6 = {"hideEvent", 1, param_slot_6 };
    static const QMetaData slot_tbl[] = {
	{ "temaComboBoxClickSlot(const QString&)", &slot_0, QMetaData::Public },
	{ "Disable_advAttrSlot()", &slot_1, QMetaData::Public },
	{ "scanButtonClickSlot()", &slot_2, QMetaData::Public },
	{ "helpButtonSlot()", &slot_3, QMetaData::Public },
	{ "tableComboBoxClickSlot(const QString&)", &slot_4, QMetaData::Public },
	{ "updateForm(PluginParameters*)", &slot_5, QMetaData::Public },
	{ "hideEvent(QHideEvent*)", &slot_6, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"EBEbinPluginWindow", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_EBEbinPluginWindow.setMetaObject( metaObj );
    return metaObj;
}

void* EBEbinPluginWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "EBEbinPluginWindow" ) )
	return this;
    return ebe::qt_cast( clname );
}

bool EBEbinPluginWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: temaComboBoxClickSlot((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: Disable_advAttrSlot(); break;
    case 2: scanButtonClickSlot(); break;
    case 3: helpButtonSlot(); break;
    case 4: tableComboBoxClickSlot((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: updateForm((PluginParameters*)static_QUType_ptr.get(_o+1)); break;
    case 6: hideEvent((QHideEvent*)static_QUType_ptr.get(_o+1)); break;
    default:
	return ebe::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool EBEbinPluginWindow::qt_emit( int _id, QUObject* _o )
{
    return ebe::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool EBEbinPluginWindow::qt_property( int id, int f, QVariant* v)
{
    return ebe::qt_property( id, f, v);
}

bool EBEbinPluginWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

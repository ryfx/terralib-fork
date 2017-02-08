/****************************************************************************
** KmlThemeWindow meta object code from reading C++ file 'KmlThemeWindow.h'
**
** Created: Tue Jun 7 11:20:44 2011
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../ui/KmlThemeWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *KmlThemeWindow::className() const
{
    return "KmlThemeWindow";
}

QMetaObject *KmlThemeWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_KmlThemeWindow( "KmlThemeWindow", &KmlThemeWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString KmlThemeWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KmlThemeWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString KmlThemeWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KmlThemeWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* KmlThemeWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"filePushButton_clicked", 0, 0 };
    static const QUMethod slot_1 = {"createThemePushButton_clicked", 0, 0 };
    static const QUMethod slot_2 = {"closePushButton_clicked", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "database", &static_QUType_ptr, "TeDatabase", QUParameter::In },
	{ "tViewBase", &static_QUType_ptr, "TerraViewBase", QUParameter::In }
    };
    static const QUMethod slot_3 = {"init", 2, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"viewComboBox_activated", 1, param_slot_4 };
    static const QUMethod slot_5 = {"helpPushButton_clicked", 0, 0 };
    static const QUMethod slot_6 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "filePushButton_clicked()", &slot_0, QMetaData::Public },
	{ "createThemePushButton_clicked()", &slot_1, QMetaData::Public },
	{ "closePushButton_clicked()", &slot_2, QMetaData::Public },
	{ "init(TeDatabase*,TerraViewBase*)", &slot_3, QMetaData::Public },
	{ "viewComboBox_activated(const QString&)", &slot_4, QMetaData::Public },
	{ "helpPushButton_clicked()", &slot_5, QMetaData::Public },
	{ "languageChange()", &slot_6, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"KmlThemeWindow", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_KmlThemeWindow.setMetaObject( metaObj );
    return metaObj;
}

void* KmlThemeWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "KmlThemeWindow" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool KmlThemeWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: filePushButton_clicked(); break;
    case 1: createThemePushButton_clicked(); break;
    case 2: closePushButton_clicked(); break;
    case 3: init((TeDatabase*)static_QUType_ptr.get(_o+1),(TerraViewBase*)static_QUType_ptr.get(_o+2)); break;
    case 4: viewComboBox_activated((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: helpPushButton_clicked(); break;
    case 6: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool KmlThemeWindow::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool KmlThemeWindow::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool KmlThemeWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

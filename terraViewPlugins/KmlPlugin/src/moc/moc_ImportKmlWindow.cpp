/****************************************************************************
** ImportKmlWindow meta object code from reading C++ file 'ImportKmlWindow.h'
**
** Created: Tue Jun 7 11:20:43 2011
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../ui/ImportKmlWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ImportKmlWindow::className() const
{
    return "ImportKmlWindow";
}

QMetaObject *ImportKmlWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ImportKmlWindow( "ImportKmlWindow", &ImportKmlWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ImportKmlWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ImportKmlWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ImportKmlWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ImportKmlWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ImportKmlWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"filePushButton_clicked", 0, 0 };
    static const QUMethod slot_1 = {"importPushButton_clicked", 0, 0 };
    static const QUMethod slot_2 = {"closePushButton_clicked", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "database", &static_QUType_ptr, "TeDatabase", QUParameter::In },
	{ "TViewBase", &static_QUType_ptr, "TerraViewBase", QUParameter::In }
    };
    static const QUMethod slot_3 = {"init", 2, param_slot_3 };
    static const QUMethod slot_4 = {"helpPushButton_clicked", 0, 0 };
    static const QUMethod slot_5 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "filePushButton_clicked()", &slot_0, QMetaData::Public },
	{ "importPushButton_clicked()", &slot_1, QMetaData::Public },
	{ "closePushButton_clicked()", &slot_2, QMetaData::Public },
	{ "init(TeDatabase*,TerraViewBase*)", &slot_3, QMetaData::Public },
	{ "helpPushButton_clicked()", &slot_4, QMetaData::Public },
	{ "languageChange()", &slot_5, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ImportKmlWindow", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ImportKmlWindow.setMetaObject( metaObj );
    return metaObj;
}

void* ImportKmlWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ImportKmlWindow" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool ImportKmlWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: filePushButton_clicked(); break;
    case 1: importPushButton_clicked(); break;
    case 2: closePushButton_clicked(); break;
    case 3: init((TeDatabase*)static_QUType_ptr.get(_o+1),(TerraViewBase*)static_QUType_ptr.get(_o+2)); break;
    case 4: helpPushButton_clicked(); break;
    case 5: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ImportKmlWindow::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ImportKmlWindow::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool ImportKmlWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

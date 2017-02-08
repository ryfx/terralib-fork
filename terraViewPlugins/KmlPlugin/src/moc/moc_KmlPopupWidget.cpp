/****************************************************************************
** KmlPopupWidget meta object code from reading C++ file 'KmlPopupWidget.h'
**
** Created: Tue Jun 7 11:20:45 2011
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../ui/KmlPopupWidget.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *KmlPopupWidget::className() const
{
    return "KmlPopupWidget";
}

QMetaObject *KmlPopupWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_KmlPopupWidget( "KmlPopupWidget", &KmlPopupWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString KmlPopupWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KmlPopupWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString KmlPopupWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KmlPopupWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* KmlPopupWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "tViewBase", &static_QUType_ptr, "TerraViewBase", QUParameter::In },
	{ "themeItem", &static_QUType_ptr, "TeQtThemeItem", QUParameter::In },
	{ "x", &static_QUType_int, 0, QUParameter::In },
	{ "y", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"init", 4, param_slot_0 };
    static const QUMethod slot_1 = {"remove", 0, 0 };
    static const QUMethod slot_2 = {"rename", 0, 0 };
    static const QUMethod slot_3 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "init(TerraViewBase*,TeQtThemeItem*,const int&,const int&)", &slot_0, QMetaData::Public },
	{ "remove()", &slot_1, QMetaData::Public },
	{ "rename()", &slot_2, QMetaData::Public },
	{ "languageChange()", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"KmlPopupWidget", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_KmlPopupWidget.setMetaObject( metaObj );
    return metaObj;
}

void* KmlPopupWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "KmlPopupWidget" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool KmlPopupWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: init((TerraViewBase*)static_QUType_ptr.get(_o+1),(TeQtThemeItem*)static_QUType_ptr.get(_o+2),(const int&)static_QUType_int.get(_o+3),(const int&)static_QUType_int.get(_o+4)); break;
    case 1: remove(); break;
    case 2: rename(); break;
    case 3: languageChange(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool KmlPopupWidget::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool KmlPopupWidget::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool KmlPopupWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

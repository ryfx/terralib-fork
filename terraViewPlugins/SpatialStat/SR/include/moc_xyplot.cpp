/****************************************************************************
** XYPlot meta object code from reading C++ file 'xyplot.h'
**
** Created: seg 25. out 10:56:05 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "xyplot.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#include <qvariant.h>
const char *XYPlot::className() const
{
    return "XYPlot";
}

QMetaObject *XYPlot::metaObj = 0;
static QMetaObjectCleanUp cleanUp_XYPlot( "XYPlot", &XYPlot::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString XYPlot::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "XYPlot", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString XYPlot::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "XYPlot", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* XYPlot::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
#ifndef QT_NO_PROPERTIES
    static const QMetaEnum::Item enum_0[] = {
	{ "LeftTick",  (int) XYPlot::LeftTick },
	{ "RightTick",  (int) XYPlot::RightTick }
    };
    static const QMetaEnum enum_tbl[] = {
	{ "TicksMark", 2, enum_0, FALSE }
    };
#endif // QT_NO_PROPERTIES
#ifndef QT_NO_PROPERTIES
    static const QMetaProperty props_tbl[10] = {
 	{ "TicksMark","ticksXAxis", 0x0107, &XYPlot::metaObj, &enum_tbl[0], -1 },
	{ "TicksMark","ticksYAxis", 0x0007, &XYPlot::metaObj, &enum_tbl[0], -1 },
	{ "double","minXValue", 0x13000103, &XYPlot::metaObj, 0, -1 },
	{ "double","maxXValue", 0x13000103, &XYPlot::metaObj, 0, -1 },
	{ "double","stepXValue", 0x13000103, &XYPlot::metaObj, 0, -1 },
	{ "double","minYValue", 0x13000103, &XYPlot::metaObj, 0, -1 },
	{ "double","maxYValue", 0x13000103, &XYPlot::metaObj, 0, -1 },
	{ "double","stepYValue", 0x13000103, &XYPlot::metaObj, 0, -1 },
	{ "QString","titleXAxis", 0x3000103, &XYPlot::metaObj, 0, -1 },
	{ "QString","titleYAxis", 0x3000103, &XYPlot::metaObj, 0, -1 }
    };
#endif // QT_NO_PROPERTIES
    metaObj = QMetaObject::new_metaobject(
	"XYPlot", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	props_tbl, 10,
	enum_tbl, 1,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_XYPlot.setMetaObject( metaObj );
    return metaObj;
}

void* XYPlot::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "XYPlot" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool XYPlot::qt_invoke( int _id, QUObject* _o )
{
    return QFrame::qt_invoke(_id,_o);
}

bool XYPlot::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool XYPlot::qt_property( int id, int f, QVariant* v)
{
    switch ( id - staticMetaObject()->propertyOffset() ) {
    case 0: switch( f ) {
	case 0: setTicksXAxis((TicksMark&)v->asInt()); break;
	case 1: *v = QVariant( (int)this->ticksXAxis() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    case 1: switch( f ) {
	case 0: setTicksXAxis((TicksMark&)v->asInt()); break;
	case 1: *v = QVariant( (int)this->ticksYAxis() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    case 2: switch( f ) {
	case 0: setMinXValue(v->asDouble()); break;
	case 1: *v = QVariant( this->minXValue() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    case 3: switch( f ) {
	case 0: setMaxXValue(v->asDouble()); break;
	case 1: *v = QVariant( this->maxXValue() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    case 4: switch( f ) {
	case 0: setStepXValue(v->asDouble()); break;
	case 1: *v = QVariant( this->stepXValue() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    case 5: switch( f ) {
	case 0: setMinYValue(v->asDouble()); break;
	case 1: *v = QVariant( this->minYValue() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    case 6: switch( f ) {
	case 0: setMaxYValue(v->asDouble()); break;
	case 1: *v = QVariant( this->maxYValue() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    case 7: switch( f ) {
	case 0: setStepYValue(v->asDouble()); break;
	case 1: *v = QVariant( this->stepYValue() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    case 8: switch( f ) {
	case 0: setTitleXAxis(v->asString()); break;
	case 1: *v = QVariant( this->titleXAxis() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    case 9: switch( f ) {
	case 0: setTitleYAxis(v->asString()); break;
	case 1: *v = QVariant( this->titleYAxis() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    default:
	return QFrame::qt_property( id, f, v );
    }
    return TRUE;
}

bool XYPlot::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

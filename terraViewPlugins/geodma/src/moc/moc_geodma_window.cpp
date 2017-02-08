/****************************************************************************
** geodma_window meta object code from reading C++ file 'geodma_window.h'
**
** Created: seg 7. nov 09:31:03 2011
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../ui/geodma_window.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *geodma_window::className() const
{
    return "geodma_window";
}

QMetaObject *geodma_window::metaObj = 0;
static QMetaObjectCleanUp cleanUp_geodma_window( "geodma_window", &geodma_window::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString geodma_window::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "geodma_window", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString geodma_window::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "geodma_window", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* geodma_window::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "e", &static_QUType_ptr, "QCloseEvent", QUParameter::In }
    };
    static const QUMethod slot_0 = {"closeEvent", 1, param_slot_0 };
    static const QUMethod slot_1 = {"q_tb_select_attribute_norm_clicked", 0, 0 };
    static const QUMethod slot_2 = {"q_tb_select_attribute_normalize_all_clicked", 0, 0 };
    static const QUMethod slot_3 = {"q_tb_unselect_attribute_norm_clicked", 0, 0 };
    static const QUMethod slot_4 = {"q_tb_select_attribute_extract_all_clicked", 0, 0 };
    static const QUMethod slot_5 = {"q_tb_select_attribute_extract_clicked", 0, 0 };
    static const QUMethod slot_6 = {"q_tb_unselect_attribute_extract_clicked", 0, 0 };
    static const QUMethod slot_7 = {"q_tb_select_attribute_classify_clicked", 0, 0 };
    static const QUMethod slot_8 = {"q_tb_select_attribute_classify_all_released", 0, 0 };
    static const QUMethod slot_9 = {"q_tb_unselect_attribute_classify_clicked", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"q_tab_selected", 1, param_slot_10 };
    static const QUMethod slot_11 = {"q_pb_reset_geodma_clicked", 0, 0 };
    static const QUMethod slot_12 = {"clear_inputs", 0, 0 };
    static const QUMethod slot_13 = {"refresh_tables", 0, 0 };
    static const QUMethod slot_14 = {"create_training_table", 0, 0 };
    static const QUMethod slot_15 = {"q_pb_select_layers_clicked", 0, 0 };
    static const QUParameter param_slot_16[] = {
	{ "item", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_16 = {"q_cb_polygon_layers_highlighted", 1, param_slot_16 };
    static const QUParameter param_slot_17[] = {
	{ "item", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_17 = {"q_cb_cell_layers_highlighted", 1, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ "item", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"q_cb_raster_layers_highlighted", 1, param_slot_18 };
    static const QUParameter param_slot_19[] = {
	{ "item", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_19 = {"q_cb_visualizing_what_highlighted", 1, param_slot_19 };
    static const QUParameter param_slot_20[] = {
	{ "item", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_20 = {"q_cb_classifying_what_highlighted", 1, param_slot_20 };
    static const QUParameter param_slot_21[] = {
	{ "item", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_21 = {"q_cb_validating_what_highlighted", 1, param_slot_21 };
    static const QUParameter param_slot_22[] = {
	{ "item", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_22 = {"q_cb_segmentation_algorithm_highlighted", 1, param_slot_22 };
    static const QUParameter param_slot_23[] = {
	{ 0, &static_QUType_ptr, "QShowEvent", QUParameter::In }
    };
    static const QUMethod slot_23 = {"showEvent", 1, param_slot_23 };
    static const QUMethod slot_24 = {"q_pb_run_segmentation_clicked", 0, 0 };
    static const QUMethod slot_25 = {"q_pb_extract_attributes_clicked", 0, 0 };
    static const QUMethod slot_26 = {"q_pb_normalize_clicked", 0, 0 };
    static const QUMethod slot_27 = {"q_pb_create_class_clicked", 0, 0 };
    static const QUMethod slot_28 = {"q_pb_remove_class_clicked", 0, 0 };
    static const QUMethod slot_29 = {"q_pb_edit_class_clicked", 0, 0 };
    static const QUMethod slot_30 = {"q_pb_associate_class_clicked", 0, 0 };
    static const QUMethod slot_31 = {"clear_frame", 0, 0 };
    static const QUMethod slot_32 = {"q_pb_plot_clicked", 0, 0 };
    static const QUMethod slot_33 = {"q_pb_spatialize_attribute_y_clicked", 0, 0 };
    static const QUMethod slot_34 = {"q_pb_load_c45_clicked", 0, 0 };
    static const QUMethod slot_35 = {"q_pb_classify_clicked", 0, 0 };
    static const QUMethod slot_36 = {"q_pb_create_confusion_matrix_clicked", 0, 0 };
    static const QUMethod slot_37 = {"q_pb_start_monte_carlo_clicked", 0, 0 };
    static const QUMethod slot_38 = {"fill_about", 0, 0 };
    static const QUMethod slot_39 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "closeEvent(QCloseEvent*)", &slot_0, QMetaData::Public },
	{ "q_tb_select_attribute_norm_clicked()", &slot_1, QMetaData::Public },
	{ "q_tb_select_attribute_normalize_all_clicked()", &slot_2, QMetaData::Public },
	{ "q_tb_unselect_attribute_norm_clicked()", &slot_3, QMetaData::Public },
	{ "q_tb_select_attribute_extract_all_clicked()", &slot_4, QMetaData::Public },
	{ "q_tb_select_attribute_extract_clicked()", &slot_5, QMetaData::Public },
	{ "q_tb_unselect_attribute_extract_clicked()", &slot_6, QMetaData::Public },
	{ "q_tb_select_attribute_classify_clicked()", &slot_7, QMetaData::Public },
	{ "q_tb_select_attribute_classify_all_released()", &slot_8, QMetaData::Public },
	{ "q_tb_unselect_attribute_classify_clicked()", &slot_9, QMetaData::Public },
	{ "q_tab_selected(const QString&)", &slot_10, QMetaData::Public },
	{ "q_pb_reset_geodma_clicked()", &slot_11, QMetaData::Public },
	{ "clear_inputs()", &slot_12, QMetaData::Public },
	{ "refresh_tables()", &slot_13, QMetaData::Public },
	{ "create_training_table()", &slot_14, QMetaData::Public },
	{ "q_pb_select_layers_clicked()", &slot_15, QMetaData::Public },
	{ "q_cb_polygon_layers_highlighted(int)", &slot_16, QMetaData::Public },
	{ "q_cb_cell_layers_highlighted(int)", &slot_17, QMetaData::Public },
	{ "q_cb_raster_layers_highlighted(int)", &slot_18, QMetaData::Public },
	{ "q_cb_visualizing_what_highlighted(int)", &slot_19, QMetaData::Public },
	{ "q_cb_classifying_what_highlighted(int)", &slot_20, QMetaData::Public },
	{ "q_cb_validating_what_highlighted(int)", &slot_21, QMetaData::Public },
	{ "q_cb_segmentation_algorithm_highlighted(int)", &slot_22, QMetaData::Public },
	{ "showEvent(QShowEvent*)", &slot_23, QMetaData::Public },
	{ "q_pb_run_segmentation_clicked()", &slot_24, QMetaData::Public },
	{ "q_pb_extract_attributes_clicked()", &slot_25, QMetaData::Public },
	{ "q_pb_normalize_clicked()", &slot_26, QMetaData::Public },
	{ "q_pb_create_class_clicked()", &slot_27, QMetaData::Public },
	{ "q_pb_remove_class_clicked()", &slot_28, QMetaData::Public },
	{ "q_pb_edit_class_clicked()", &slot_29, QMetaData::Public },
	{ "q_pb_associate_class_clicked()", &slot_30, QMetaData::Public },
	{ "clear_frame()", &slot_31, QMetaData::Public },
	{ "q_pb_plot_clicked()", &slot_32, QMetaData::Public },
	{ "q_pb_spatialize_attribute_y_clicked()", &slot_33, QMetaData::Public },
	{ "q_pb_load_c45_clicked()", &slot_34, QMetaData::Public },
	{ "q_pb_classify_clicked()", &slot_35, QMetaData::Public },
	{ "q_pb_create_confusion_matrix_clicked()", &slot_36, QMetaData::Public },
	{ "q_pb_start_monte_carlo_clicked()", &slot_37, QMetaData::Public },
	{ "fill_about()", &slot_38, QMetaData::Public },
	{ "languageChange()", &slot_39, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"geodma_window", parentObject,
	slot_tbl, 40,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_geodma_window.setMetaObject( metaObj );
    return metaObj;
}

void* geodma_window::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "geodma_window" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool geodma_window::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: closeEvent((QCloseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 1: q_tb_select_attribute_norm_clicked(); break;
    case 2: q_tb_select_attribute_normalize_all_clicked(); break;
    case 3: q_tb_unselect_attribute_norm_clicked(); break;
    case 4: q_tb_select_attribute_extract_all_clicked(); break;
    case 5: q_tb_select_attribute_extract_clicked(); break;
    case 6: q_tb_unselect_attribute_extract_clicked(); break;
    case 7: q_tb_select_attribute_classify_clicked(); break;
    case 8: q_tb_select_attribute_classify_all_released(); break;
    case 9: q_tb_unselect_attribute_classify_clicked(); break;
    case 10: q_tab_selected((const QString&)static_QUType_QString.get(_o+1)); break;
    case 11: q_pb_reset_geodma_clicked(); break;
    case 12: clear_inputs(); break;
    case 13: refresh_tables(); break;
    case 14: create_training_table(); break;
    case 15: q_pb_select_layers_clicked(); break;
    case 16: q_cb_polygon_layers_highlighted((int)static_QUType_int.get(_o+1)); break;
    case 17: q_cb_cell_layers_highlighted((int)static_QUType_int.get(_o+1)); break;
    case 18: q_cb_raster_layers_highlighted((int)static_QUType_int.get(_o+1)); break;
    case 19: q_cb_visualizing_what_highlighted((int)static_QUType_int.get(_o+1)); break;
    case 20: q_cb_classifying_what_highlighted((int)static_QUType_int.get(_o+1)); break;
    case 21: q_cb_validating_what_highlighted((int)static_QUType_int.get(_o+1)); break;
    case 22: q_cb_segmentation_algorithm_highlighted((int)static_QUType_int.get(_o+1)); break;
    case 23: showEvent((QShowEvent*)static_QUType_ptr.get(_o+1)); break;
    case 24: q_pb_run_segmentation_clicked(); break;
    case 25: q_pb_extract_attributes_clicked(); break;
    case 26: q_pb_normalize_clicked(); break;
    case 27: q_pb_create_class_clicked(); break;
    case 28: q_pb_remove_class_clicked(); break;
    case 29: q_pb_edit_class_clicked(); break;
    case 30: q_pb_associate_class_clicked(); break;
    case 31: clear_frame(); break;
    case 32: q_pb_plot_clicked(); break;
    case 33: q_pb_spatialize_attribute_y_clicked(); break;
    case 34: q_pb_load_c45_clicked(); break;
    case 35: q_pb_classify_clicked(); break;
    case 36: q_pb_create_confusion_matrix_clicked(); break;
    case 37: q_pb_start_monte_carlo_clicked(); break;
    case 38: fill_about(); break;
    case 39: languageChange(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool geodma_window::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool geodma_window::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool geodma_window::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

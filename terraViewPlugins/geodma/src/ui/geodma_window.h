/****************************************************************************
** Form interface generated from reading ui file 'ui\geodma_window.ui'
**
** Created: seg 7. nov 09:31:02 2011
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef GEODMA_WINDOW_H
#define GEODMA_WINDOW_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qmainwindow.h>
#include <PluginParameters.h>
#include <TePDITypes.hpp>
#include <qgrid.h>
#include <geodma_debug.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QTabWidget;
class QWidget;
class QLabel;
class QPushButton;
class QGroupBox;
class QFrame;
class QSpinBox;
class QLineEdit;
class QComboBox;
class QTextEdit;
class QToolButton;
class QListBox;
class QListBoxItem;
class QButtonGroup;
class QRadioButton;
class QToolBox;
class QCheckBox;

class geodma_window : public QMainWindow
{
    Q_OBJECT

public:
    geodma_window( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~geodma_window();

    QTabWidget* q_tab;
    QWidget* TabPage;
    QLabel* textLabel1_11_2_2;
    QPushButton* q_pb_run_segmentation;
    QGroupBox* groupBox16;
    QFrame* q_fr_baatz;
    QLabel* textLabel2_3_5;
    QLabel* textLabel2_3_2_3;
    QLabel* textLabel2_3_3_2;
    QSpinBox* q_sb_baatz_scale;
    QSpinBox* q_sb_baatz_color;
    QSpinBox* q_sb_baatz_compactness;
    QLabel* textLabel2_3_4_2_2_2;
    QFrame* q_fr_chessboard;
    QLabel* textLabel2_3_4_2_2_2_2;
    QLabel* textLabel2_3_5_2;
    QLabel* textLabel2_3_2_3_2;
    QSpinBox* q_sb_chessboard_lines;
    QSpinBox* q_sb_chessboard_columns;
    QFrame* q_fr_regiongrowing;
    QLabel* textLabel2_3_2_2_2;
    QLineEdit* q_le_regiongrowing_minimumarea;
    QLineEdit* q_le_regiongrowing_euclideandistance;
    QLabel* textLabel2_3_4_3;
    QLabel* textLabel2_3_4_2_3;
    QComboBox* q_cb_available_raster_layers_segmentation;
    QComboBox* q_cb_segmentation_algorithm;
    QLabel* textLabel1_11_2;
    QWidget* TabPage_2;
    QLabel* textLabel1_6;
    QLabel* textLabel2_4;
    QLabel* textLabel1_5_2;
    QLabel* textLabel2;
    QLabel* textLabel1_5;
    QComboBox* q_cb_polygon_layers;
    QComboBox* q_cb_polygon_tables;
    QComboBox* q_cb_cell_layers;
    QComboBox* q_cb_cell_tables;
    QComboBox* q_cb_raster_layers;
    QGroupBox* groupBox10_2_2;
    QTextEdit* textEdit6_2_2;
    QPushButton* q_pb_select_layers;
    QPushButton* q_pb_reset_geodma;
    QWidget* tab;
    QToolButton* q_tb_unselect_attribute_extract;
    QToolButton* q_tb_select_attribute_extract;
    QToolButton* q_tb_select_attribute_extract_all;
    QListBox* q_lb_attributes_extract;
    QGroupBox* groupBox10_2;
    QTextEdit* textEdit6_2;
    QPushButton* q_pb_extract_attributes;
    QListBox* q_lb_attributes_available;
    QLabel* textLabel1_3_2;
    QWidget* tab_2;
    QToolButton* q_tb_unselect_attribute_norm;
    QPushButton* q_pb_normalize;
    QButtonGroup* q_br_methods;
    QRadioButton* q_rb_mean;
    QTextEdit* textEdit6_3;
    QRadioButton* q_rb_normal;
    QToolButton* q_tb_select_attribute_normalize_all;
    QToolButton* q_tb_select_attribute_norm;
    QListBox* q_lb_attributes_norm;
    QListBox* q_lb_attributes;
    QLabel* textLabel1_3;
    QWidget* TabPage_3;
    QPushButton* q_pb_associate_class;
    QLabel* textLabel1_9;
    QGroupBox* groupBox10;
    QTextEdit* textEdit6;
    QListBox* q_lb_training_classes;
    QLabel* textLabel1_9_2;
    QPushButton* q_pb_create_class;
    QLabel* textLabel2_2;
    QPushButton* q_pb_remove_class;
    QPushButton* q_pb_edit_class;
    QComboBox* q_cb_new_class_color;
    QLineEdit* q_le_new_class_name;
    QWidget* TabPage_4;
    QLabel* textLabel1_4;
    QPushButton* q_pb_plot;
    QFrame* q_frame;
    QGroupBox* groupBox1;
    QLabel* textLabel1_2_2;
    QLabel* textLabel1_2_3;
    QLabel* textLabel1_2;
    QLabel* textLabel1;
    QLabel* textLabel1_12;
    QPushButton* q_pb_spatialize_attribute_y;
    QComboBox* q_cb_visualizing_what;
    QSpinBox* q_sb_srate;
    QComboBox* q_cb_xaxis;
    QComboBox* q_cb_labels;
    QComboBox* q_cb_yaxis;
    QWidget* TabPage_5;
    QLabel* textLabel1_10;
    QToolButton* q_tb_unselect_attribute_classify;
    QToolButton* q_tb_select_attribute_classify_all;
    QToolButton* q_tb_select_attribute_classify;
    QListBox* q_lb_attributes_classify;
    QPushButton* q_pb_classify;
    QListBox* q_lb_attributes_candidate;
    QLabel* textLabel1_3_3;
    QButtonGroup* buttonGroup9;
    QLabel* textLabel1_2_3_2_2;
    QComboBox* q_cb_classifying_what;
    QLabel* textLabel1_2_3_2;
    QLabel* textLabel1_2_3_2_3;
    QRadioButton* q_rb_create_legends;
    QRadioButton* q_rb_create_themes;
    QLabel* textLabel1_8;
    QLabel* textLabel1_8_2;
    QLineEdit* q_le_new_view;
    QListBox* q_lb_ancillary_training_sets;
    QComboBox* q_cb_training_labels;
    QToolBox* q_tb_classifiers;
    QWidget* page2;
    QLabel* textLabel1_7_3;
    QLabel* textLabel1_7_4;
    QLabel* textLabel1_7;
    QLabel* textLabel1_7_6;
    QLabel* textLabel1_7_5;
    QLabel* textLabel1_7_2;
    QSpinBox* q_sb_max_epochs;
    QLineEdit* q_le_radius;
    QLineEdit* q_le_decreasing;
    QLineEdit* q_le_alpha;
    QSpinBox* q_sb_map_height;
    QSpinBox* q_sb_map_width;
    QWidget* page;
    QTextEdit* textEdit5;
    QCheckBox* q_cb_save_c45;
    QPushButton* q_pb_load_c45;
    QWidget* page_2;
    QLabel* textLabel1_7_6_2;
    QLabel* textLabel1_7_3_2;
    QLabel* textLabel1_7_2_2;
    QSpinBox* q_sb_max_epochs_ann;
    QLineEdit* q_le_learning_rate;
    QSpinBox* q_sb_hidden_neurons;
    QComboBox* q_cb_activation_function;
    QLabel* textLabel1_7_7;
    QTextEdit* textEdit5_2;
    QWidget* TabPage_6;
    QButtonGroup* buttonGroup8;
    QLabel* textLabel1_13_5;
    QLabel* textLabel1_13_2;
    QTextEdit* q_te_confusion_matrix;
    QPushButton* q_pb_create_confusion_matrix;
    QLabel* textLabel1_13;
    QComboBox* q_cb_classification_column;
    QComboBox* q_cb_reference_column;
    QLabel* textLabel1_13_4;
    QButtonGroup* buttonGroup8_2;
    QPushButton* q_pb_start_monte_carlo;
    QListBox* q_lb_monte_carlo_attributes;
    QTextEdit* q_te_monte_carlo_output;
    QLabel* textLabel1_13_5_2;
    QLabel* textLabel1_13_3_2;
    QSpinBox* q_sb_monte_carlo_iterations;
    QComboBox* q_cb_monte_carlo_labels;
    QLabel* textLabel1_13_3;
    QLabel* textLabel1_13_2_2;
    QCheckBox* q_cb_show_only_kappas;
    QComboBox* q_cb_validating_what;
    QWidget* TabPage_7;
    QButtonGroup* buttonGroup5;
    QTextEdit* q_te_about;
    QFrame* frame6;
    QLabel* pixmapLabel4;
    QLabel* pixmapLabel3;
    QLabel* pixmapLabel5;
    QLabel* pixmapLabel2;
    QLabel* q_pm_xmas;
    QLabel* q_pm_flag;
    QTextEdit* q_te_version;

    bool started;
    geodma_debug debug;
    int rasters_layer_index;
    vector<TeLayer*> polygons_layers;
    vector<TeLayer*> cells_layers;
    vector<TeLayer*> rasters_layers;
    PluginParameters* plug_params_ptr_;
    string polygons_attribute_table_name;
    string cells_attribute_table_name;
    TePolygonSet polygons;
    TeCellSet cells;
    int polygons_layer_index;
    int cells_layer_index;
    bool using_polygons;
    bool using_cells;
    bool using_rasters;
    string loaded_c45;

    void init();
    void destroy();
    virtual void unlock_grid();
    virtual QString function_name( QString name );
    virtual vector<string> get_all_polygons_layers();
    virtual vector<string> get_all_cells_layers();
    virtual TeLayer * get_polygons_layer();
    virtual TeLayer * get_cells_layer();
    virtual TeLayer * get_rasters_layer();
    virtual TeLayer * get_layer( string layer_name );
    virtual bool execute_query( string query );
    virtual int find_index( string name, TeLayer * layer, string table_name );
    virtual list<string> get_columns_names( TeLayer * layer, string attribute_table_name, TeTable table, TeAttrDataType restriction_type );
    virtual double get_kappa( TeMatrix confusion_matrix );
    virtual bool update_first_window();
    virtual string get_valid_layer_name( string needle );
    virtual bool create_polygons_theme( TeLayer * polygons_layer, string theme_name, TeLayer * raster_layer );
    virtual bool create_polygons_layer( TePolygonSet pols, string polygons_layer_name, TeProjection * layer_projection );

public slots:
    virtual void closeEvent( QCloseEvent * e );
    virtual void q_tb_select_attribute_norm_clicked();
    virtual void q_tb_select_attribute_normalize_all_clicked();
    virtual void q_tb_unselect_attribute_norm_clicked();
    virtual void q_tb_select_attribute_extract_all_clicked();
    virtual void q_tb_select_attribute_extract_clicked();
    virtual void q_tb_unselect_attribute_extract_clicked();
    virtual void q_tb_select_attribute_classify_clicked();
    virtual void q_tb_select_attribute_classify_all_released();
    virtual void q_tb_unselect_attribute_classify_clicked();
    virtual void q_tab_selected( const QString & );
    virtual void q_pb_reset_geodma_clicked();
    virtual void clear_inputs();
    virtual void refresh_tables();
    virtual void create_training_table();
    virtual void q_pb_select_layers_clicked();
    virtual void q_cb_polygon_layers_highlighted( int item );
    virtual void q_cb_cell_layers_highlighted( int item );
    virtual void q_cb_raster_layers_highlighted( int item );
    virtual void q_cb_visualizing_what_highlighted( int item );
    virtual void q_cb_classifying_what_highlighted( int item );
    virtual void q_cb_validating_what_highlighted( int item );
    virtual void q_cb_segmentation_algorithm_highlighted( int item );
    virtual void showEvent( QShowEvent * );
    virtual void q_pb_run_segmentation_clicked();
    virtual void q_pb_extract_attributes_clicked();
    virtual void q_pb_normalize_clicked();
    virtual void q_pb_create_class_clicked();
    virtual void q_pb_remove_class_clicked();
    virtual void q_pb_edit_class_clicked();
    virtual void q_pb_associate_class_clicked();
    virtual void clear_frame();
    virtual void q_pb_plot_clicked();
    virtual void q_pb_spatialize_attribute_y_clicked();
    virtual void q_pb_load_c45_clicked();
    virtual void q_pb_classify_clicked();
    virtual void q_pb_create_confusion_matrix_clicked();
    virtual void q_pb_start_monte_carlo_clicked();
    virtual void fill_about();

protected:
    map<string, QGrid*> grids;


protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;
    QPixmap image4;
    QPixmap image5;
    QPixmap image6;
    QPixmap image7;

};

#endif // GEODMA_WINDOW_H

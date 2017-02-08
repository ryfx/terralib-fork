#include "WMSConfigurer.h"

#include "PluginParameters.h"
#include "WMSClientPluginMainForm.h"
#include <TeQtViewsListView.h>

//QT include files
#include <qtoolbar.h>
#include <qpopupmenu.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qmainwindow.h>
#include <qimage.h>

// XPM image include files
#include "wmsTheme.xpm"

#define TVWMSPLG_WIN_TITLE	QObject::tr("TerraView WMS Client Plugin")

TeWMSConfigurer::TeWMSConfigurer(PluginParameters* plgParams) :
QObject(),
plgParams_(plgParams)
{
}

TeWMSConfigurer::~TeWMSConfigurer()
{
//	updateTVInterface(false);
}

void TeWMSConfigurer::updateTVInterface(const bool& addButtons)
{
	QPopupMenu* themeMnu = findThemesMenu(plgParams_->qtmain_widget_ptr_);

	if(themeMnu == NULL)
	{
		return;
	}

	if(addButtons)
	{
	  /* Inserting the tool button into TerraView toolbar */
  
		QPtrList< QToolBar > tv_tool_bars_list = 
			plgParams_->qtmain_widget_ptr_->toolBars( Qt::DockTop );
			
		if( tv_tool_bars_list.count() > 0 ) 
		{
			plgAction_ = new QAction(NULL, "WMS plugin action");
		//    plugin_action_ptr_.reset( new QAction( tv_tool_bars_list.at( 0 ) ) );
			
			plgAction_->setText( tr("WMS Theme") );
			plgAction_->setMenuText( tr("WMS Theme") );
			plgAction_->setToolTip( tr("WMS Theme") );
			plgAction_->setIconSet( QIconSet( QPixmap(wmsTheme_xpm)));
			
			plgAction_->addTo(plgParams_->qtmain_widget_ptr_->toolBars(Qt::DockTop).at(0));
			
			connect( plgAction_, SIGNAL( activated() ), this, SLOT( showPluginWindow() ) );
		}

		if(themeMnu == NULL)
		{
			return;
		}

		mnuIdxs_.push_back(themeMnu->insertSeparator());

		mnuIdxs_.push_back(themeMnu->insertItem(tr("Add WMS Theme..."), this, 
			SLOT(showPluginWindow())));
	}
	else
	{
		delete plgAction_;

		std::vector<int>::iterator it;

		for(it = mnuIdxs_.begin(); it != mnuIdxs_.end(); ++it)
		{
			themeMnu->removeItem((*it));
		}
	}
}

void TeWMSConfigurer::showPluginWindow()
{
	/* Checking the TerraView plugin interface compatibility */
    if(plgParams_->getVersion() == PLUGINPARAMETERS_VERSION)
	{
		if(!(plgParams_->getCurrentDatabasePtr()))
		{
			QString warningMessage(tr("There is no database connected!"));
			QMessageBox::critical(plgParams_->qtmain_widget_ptr_, tr("Error"), 
				warningMessage, 0, 0, 0);
			return;
		}

		if(!(plgParams_->getCurrentViewPtr()))
		{
			QString warningMessage(tr("In order to run WMS Plugin you should activate a view."));
			QMessageBox::warning(plgParams_->qtmain_widget_ptr_, 
				TVWMSPLG_WIN_TITLE, warningMessage, 0, 0, 0);
			return;
		}

/* create WMS Client window with the new parameters */
		WMSClientPluginMainForm* wmsDialog = new WMSClientPluginMainForm(plgParams_->qtmain_widget_ptr_, "WMS Client Plugin", true);
		wmsDialog->init(plgParams_->getCurrentDatabasePtr(),
				        plgParams_->getCurrentViewPtr(),
						plgParams_->signal_emitter_,
						plgParams_->teqtviewslistview_ptr_->getViewItem(plgParams_->getCurrentViewPtr()));
		wmsDialog->exec();
		delete wmsDialog;
	}
	else
	{
		QString warningMessage(tr("WMS Plugin's parameter version is different from TerraView's!"));
		QMessageBox::warning(plgParams_->qtmain_widget_ptr_, 
			TVWMSPLG_WIN_TITLE, warningMessage, 0, 0, 0);
		return;
	}
}

QPopupMenu* TeWMSConfigurer::findThemesMenu(QMainWindow* tvMainWin)
{
	QMenuBar* mnu = tvMainWin->menuBar();

    int idx = -1;
    
    for(unsigned int i = 0; i < mnu->count() -1; i++)
    {
        idx = mnu->idAt(i);
		if(mnu->text(idx).isEmpty())
		{
			continue;
		}

		if(mnu->text(idx) == tr("&Theme"))
        {
            break;
        }
    }

    if(idx == -1)
    {
        return NULL;
    }

    return mnu->findItem(idx)->popup();
}

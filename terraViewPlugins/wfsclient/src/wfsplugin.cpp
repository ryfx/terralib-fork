#include <wfsplugin.h>

#include <wfsconfig.h>
#include <wfsPluginUtils.h>
#include <wfsClientTerraViewUtils.h>

//TerraOGC include files
#include <wfsclient/TeWFSClient.h>

#include <wfstheme/TeWFSThemeFactory.h>
#include <wfstheme/TeWFSTheme.h>

//TerraView include files
#include <TeQtViewsListView.h>
#include <TeQtThemeItem.h>
#include <TeQtViewItem.h>

//TerraLib include files
#include <TeVectorRemap.h>

//Images
#include <images/wmsTheme.xpm>

//Qt include files 
#include <qaction.h>
#include <qmessagebox.h>
#include <qimage.h>

static TeOGC::TeWFSThemeFactory wfsThemeFactory;

void setDefaultPointVisual(TeAbstractTheme* th)
{
	//Polygon
	TeVisual* vis = th->pointingLegend().getVisualMap()[TePOLYGONS];
	vis->color(TeColor(0, 255, 0));
	vis->transparency(50);
	vis->style(TePolyTypeTransparent);
	vis->contourWidth(3);
	vis->contourColor(TeColor(0,255,0));
	vis->contourStyle(TeLnTypeDashDotDot);

	//Line
	vis = th->pointingLegend().getVisualMap()[TeLINES];
	vis->color(TeColor(0, 255, 0));
	vis->transparency(50);
	vis->width(3);
	vis->style(TeLnTypeDashDotDot);

	//Point
	vis = th->pointingLegend().getVisualMap()[TePOINTS];
	vis->color(TeColor(0, 255, 0));
	vis->transparency(50);
	vis->size(5);
	vis->style(TePtTypeStar);
}

void saveViewBox(TeView* view, TeDatabase* db)
{
    TeBox viewBox = view->getCurrentBox();

    //salva o novo box no banco de dados.
	std::string sql("UPDATE te_view SET ");
	sql += "lower_x = " + Te2String(viewBox.x1()) + ", lower_y = " + Te2String(viewBox.y1());
	sql += ", upper_x = " + Te2String(viewBox.x2()) + ", upper_y = " + Te2String(viewBox.y2());
	sql += " WHERE view_id = " + Te2String(view->id());

	if(!db->execute(sql))
	{
		std::string msg = QObject::tr("Could not save theme box in database: ");

		throw TeException(UNKNOWN_ERROR_TYPE, msg + db->errorMessage());
	}
}

void changeViewProjection(TeView* view, TeProjection* newProjection, TeDatabase* db)
{
    if((view == 0) || (newProjection == 0))
		return;

    TeProjection* oldProj = view->projection();
    TeProjection* newProj = 0;

    newProj = TeProjectionFactory::make(newProjection->params());
    
    TeBox viewBox = view->getCurrentBox();

    if((newProj != 0) && (!(*oldProj == *newProj)))
    {
		viewBox = TeRemapBox(viewBox, oldProj, newProj);

        int id = oldProj->id();
        delete oldProj;

        newProj->id(id);

        view->setCurrentBox(viewBox);
        view->projection(newProj);

        if(db != 0)
        {
        	if(!db->updateProjection(newProj))
				throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Error trying to update view projection.").latin1());

        	string sql = "UPDATE te_view SET lower_x=" + Te2String(viewBox.x1())  + ",lower_y=" + Te2String(viewBox.y1()) +
        			",upper_x=" + Te2String(viewBox.x2()) + ",upper_y=" + Te2String(viewBox.y2()) + " WHERE view_id=" + Te2String(view->id());

        	if(!db->execute(sql))
				throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Error trying to update view box.").latin1());
        }
    }
}


WFSPlugin::WFSPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("wfsclient_");

	WFSSetPluginParameters(params);
}

WFSPlugin::~WFSPlugin()
{
	end();
}

void WFSPlugin::init()
{
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			createTheme_ = new QAction(0, "", false);
			createTheme_->setMenuText(tr("Add WFS Theme"));
			createTheme_->setText( tr("WFS Theme") );
			createTheme_->setToolTip( tr("WFS Theme") );
			createTheme_->setIconSet( QIconSet( QPixmap(wmsTheme_xpm)));

			QPopupMenu* mnu = getPluginsMenu(tr("TerraViewPlugins.Themes").latin1());

			if(mnu != 0)
			{
				if(!createTheme_->addTo(mnu))
					throw;
			}

			QPtrList< QToolBar > tv_tool_bars_list = tview->toolBars( Qt::DockTop );

			if( tv_tool_bars_list.count() > 0 )
				createTheme_->addTo(tview->toolBars(Qt::DockTop).at(0));

			connect(createTheme_, SIGNAL(activated()), this, SLOT(showCreateThemeWindow()));

			initOWSPlatform();
		}
	}
	catch(...)
	{
		QMessageBox::critical(tview, tr("TerraView plug-in error"), tr("Can't create plug-in menu."));

		delete createTheme_;
		createTheme_ = 0;
	}
}

void WFSPlugin::end()
{
	TeOGC::TeWFSClient::finalizePlatform();
	cleanAttrDialogPtr();
}

void WFSPlugin::showCreateThemeWindow()
{
	TerraViewBase* tview = getMainWindow();

	if(params_->getCurrentDatabasePtr() == 0)
	{
		QMessageBox::warning(tview, tr("WFS Client Plugin"), tr("There's no database connected. Connect one first."));
		return;
	}

	if(params_->getCurrentViewPtr() == 0)
	{
		QMessageBox::warning(tview, tr("WFS Client Plugin"), tr("There's no current view. Select a view first."));
		return;
	}

	WFSConfig conf(tview);
	conf.setDatabase(params_->getCurrentDatabasePtr());

	connect(&conf, SIGNAL(saveWFSTheme(const WFSParams&)), this, SLOT(createTheme(const WFSParams&)));

	conf.exec();
}

void WFSPlugin::createTheme(const WFSParams& params)
{
	TeOGC::TeWFSTheme* theme = 0;
	TeDatabase* db = params_->getCurrentDatabasePtr();
	TeView* view = params_->getCurrentViewPtr();

	try
	{
		TeOGC::TeWFSThemeParams p(params.themeName_, params.wfsLayerName_, params.projection_);

		p.namespaceURI_ = params.uri_;
		p.serviceId_ = params.serviceId_;

		theme = dynamic_cast<TeOGC::TeWFSTheme*>(TeViewNodeFactory::make(&p));

		if(theme == 0)
			throw TeException(UNKNOWN_ERROR_TYPE, 
			tr("Fail to create WFSTheme object: error on theme Factory").latin1());

		if(*theme->getThemeProjection() == TeNoProjection())
			QMessageBox::warning(getMainWindow(), tr("WFS creation error"), tr("Projection was not found. Theme was created without projection defined."));

		setDefaultPointVisual(theme);

		db->updateLegend(&theme->pointingLegend());


		theme->view(view->id());
		theme->priority(view->size());

		theme->setThemeBox(params.bx_);

		if(!db->insertTheme(theme))
		{
			string msg = tr("Fail to insert WFSTheme in database: ").latin1() + db->errorMessage() + ".";
			throw TeException(UNKNOWN_ERROR_TYPE, msg);
		}

		view->add(theme);

		// Update view box
		TeBox vBox;

		bool has2Change = !((*view->projection()) == (*theme->getThemeProjection()));

		if(view->size() == 1 && has2Change)
			changeViewProjection(view, theme->getThemeProjection(), db);
		//else
		
		vBox = view->box(false);

		view->setCurrentBox(vBox);
		saveViewBox(view, db);

		TeAppTheme* appTheme = new TeAppTheme(theme);
		std::string themename = appTheme->getTheme()->name();
		
		QString qthemename(themename.c_str());

/* put the new theme on the interface and set it as the current theme */
		TeQtThemeItem* themeItem = new TeQtThemeItem(params_->teqtviewslistview_ptr_->getViewItem(params_->getCurrentViewPtr()),
													 qthemename,
													 appTheme);
		
		themeItem->setPixmap(0, QPixmap(wmsTheme_xpm));
		themeItem->setOn(true);

		params_->teqtviewslistview_ptr_->getViewItem(params_->getCurrentViewPtr())->setOpen(true);


		QMessageBox::information(getMainWindow(), tr("Theme created"), tr("Theme created successfully!"));
	}
	catch(TeException& e)
	{
		if(theme != 0)
			delete theme;

		QMessageBox::warning(getMainWindow(), tr("WFS creation error"), e.message().c_str());
	}
}

void WFSPlugin::initOWSPlatform()
{
/* Checking the TerraView plugin interface compatibility */
    if(params_->getVersion() == PLUGINPARAMETERS_VERSION)
	{
/* Initializes WFS theme support */
		TeOGC::TeWFSClient::initializePlatform();

/* connects to TerraView signal emitter instance */
		wmsPluginSignalReceiver_.setHostObj(0, WFSClientPluginSignalHandler);
		wmsPluginSignalReceiver_.connect(*(params_->signal_emitter_));
	}
	else
	{
		QString warningMessage(tr("WFS Plugin's parameter version is different from TerraView's!"));
		QMessageBox::warning(params_->qtmain_widget_ptr_, "TerraView WFS Client Plugin", warningMessage, 0, 0, 0);
	}
}
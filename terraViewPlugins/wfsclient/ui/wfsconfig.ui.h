/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <common/TeOGCProjection.h>

#include <xml/TeXMLException.h>

#include <ows/TeOWSException.h>
#include <ows/TeOWSWGS84BoundingBox.h>

#include <owstheme/TeOWSThemeServiceInfo.h>
#include <owstheme/TeOWSThemeServiceInfoManager.h>

#include <wfs/TeWFSException.h>
#include <wfs/TeWFSGetCapabilities.h>
#include <wfs/TeWFSFeatureTypeList.h>
#include <wfs/TeWFSFeatureType.h>
#include <wfs/TeWFSCapabilities.h>
#include <wfs/TeWFSLatLongBoundingBox.h>
#include <wfs/TeWFSGMLObjectType.h>

#include <wfstheme/TeWFSThemeFactory.h>
#include <wfstheme/TeWFSTheme.h>

#include <wfsclient/TeWFSClient.h>

#include <wfsPluginUtils.h>
#include <wfsClientTerraViewUtils.h>

//TerraLib include files
#include <TeBox.h>
#include <TeSTElementSet.h>

//Qt include files
#include <qmessagebox.h>
#include <qcursor.h>
#include <qinputdialog.h>

template <typename T>
void cleanPtVector(std::vector<T*>& vec)
{
    typename std::vector<T*>::iterator it;
for(int i=0; vec.size()<0; i++) {
		delete *vec[i];
	}
	vec.clear();
}

void WFSConfig::init()
{
	cap_ = 0;
	gml_vrs_cbb_->setEnabled(false);
	op_lst_->setEnabled(false);
	db_ = 0;
	currentServiceId_ = -1;

	canvas_->setBackgroundColor(Qt::white);

	cleanThemesConfig();

	updateServicesTable(serv_tbl_);
}

void WFSConfig::destroy()
{
	delete cap_;
}

void WFSConfig::refresh_themes_clicked()
{
	QCursor cursor(Qt::WaitCursor);

	try
	{
		setCursor(cursor);

		QString serv = server_lne_->text();

		if(serv.isEmpty())
			throw std::string(tr("The service URL must not be empty!").latin1());

		cleanThemesConfig();

		TeOGC::TeWFSClient client;
		client.setServiceURL(serv.latin1());

		//getting server info
		cap_ = client.getCapabilities(TeOGC::TeWFSGetCapabilities());

		if(cap_ == 0)
			return;

		//Updating the gml format
		gml_vrs_cbb_->clear();
		wfs_vrs_cbb_->clear();

		wfs_vrs_cbb_->insertItem(cap_->getVersion().c_str());

		std::vector<TeOGC::TeWFSGMLObjectType*> gmlFs = cap_->getServesGMLObjectTypeList();
		std::vector<TeOGC::TeWFSGMLObjectType*>::iterator git;

		for(git = gmlFs.begin(); git != gmlFs.end(); ++git)
		{
			TeOGC::TeWFSGMLObjectType* gmlF = *git;
			std::vector<std::string> formats = gmlF->getOutputFormats();

			for(size_t i=0; i<formats.size(); i++)
				gml_vrs_cbb_->insertItem(formats[i].c_str());
		}

		TeOGC::TeWFSFeatureTypeList* featList = cap_->getFeatureTypeList();
		feats_ = featList->getFeatureTypes();

		themes_lst_->setNumRows(feats_.size());
		

		std::vector<TeOGC::TeWFSFeatureType*>::iterator it;

		if(feats_.empty())
			throw std::string(tr("Themes not loaded! Try to refresh themes list.").latin1());

		configParams_->page(1)->setEnabled(true);

		for(it = feats_.begin(); it != feats_.end(); ++it)
		{
			int row = it - feats_.begin();

			TeOGC::TeWFSFeatureType* feat = *it;
			themes_lst_->setText(row, 0, feat->getTitle().c_str());
		}

		themes_lst_->adjustColumn(0);
		themes_lst_->sortColumn(0,true);

		currentServiceId_ = saveService(db_, serv.latin1(), this);
		updateServicesTable(serv_tbl_);

		setCursor(QCursor());
	}
	catch(TeOGC::TeXMLException& e)
	{
		setCursor(QCursor());
		QMessageBox::warning(this, tr("XML error"), e.getErrorMessage().c_str());
	}
	catch(TeOGC::TeWFSException& e)
	{
		setCursor(QCursor());
		QMessageBox::warning(this, tr("WFS error"), e.getErrorMessage().c_str());
	}
	catch(TeOGC::TeOWSException& e)
	{
		setCursor(QCursor());
		QMessageBox::warning(this, tr("OWS error"), e.getErrorMessage().c_str());
	}
	catch(std::string& e)
	{
		setCursor(QCursor());
		QMessageBox::warning(this, tr("Server information error"), e.c_str());
	}
}


void WFSConfig::serv_selectionChanged()
{
	int idx = serv_tbl_->currentRow();
	if(idx < 0 || idx >= serv_tbl_->numRows())	return;

	QString serv = serv_tbl_->text(idx, 1);

	if(serv.isEmpty())		return;

	TeOGC::TeOWSThemeServiceInfo* info = TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceInfoByOperationURL(serv.latin1());
	currentServiceId_ = info->getId();
	server_lne_->setText(serv);
}


void WFSConfig::themes_selectionChanged()
{
	cleanThemeArgs();

	TeOGC::TeWFSFeatureType* feat = getCurrentFeature();

	if(feat == 0)
		return;

	std::vector<std::string>::iterator it;

	//Output format
	std::vector<std::string> res = feat->getOutputFormats();

	for(it = res.begin(); it != res.end(); ++it)
		gml_vrs_cbb_->insertItem((*it).c_str());

	gml_vrs_cbb_->insertItem("3.3.1");

	//Projections available
	proj_cbb_->insertItem(feat->getDefaultSRS().c_str());
	res = feat->getOtherSRS();

	for(it = res.begin(); it != res.end(); ++it)
		proj_cbb_->insertItem((*it).c_str());

	//Bounding rectangle
	TeBox bx;
	feat->getLatLongBoundingBox()->getTeBox(bx);

	x1_lne_->setText(QString::number(bx.x1()));
	y1_lne_->setText(QString::number(bx.y1()));
	x2_lne_->setText(QString::number(bx.x2()));
	y2_lne_->setText(QString::number(bx.y2()));

	//Operations
	res = feat->getOperations();

	for(it = res.begin(); it != res.end(); ++it)
		op_lst_->insertItem((*it).c_str());

	save_btn_->setEnabled(true);
	preview_btn_->setEnabled(true);
	configParams_->page(2)->setEnabled(true);
}

void WFSConfig::save_clicked()
{
	bool ok;

	try
	{
		QString tName = QInputDialog::getText(tr("Enter theme name"), tr("Theme name: "), QLineEdit::Normal, QString::null, &ok, this);

		if(ok)
		{
			if(tName.isEmpty())
				throw tr("Theme name must not be empty.");

			TeOGC::TeWFSFeatureType* feat = getCurrentFeature();

			if(feat == 0)
				throw tr("There's no selected theme.");

			int projId = getProjectionId();
			TeBox bx;

			if(feat->getWGS84BoundingBox().empty())
				feat->getLatLongBoundingBox()->getTeBox(bx);
			else
				bx = feat->getWGS84BoundingBox()[0]->getBBox(projId);


			WFSParams p(tName.latin1(), feat->getName().getFullName(), feat->getName().getURI(), currentServiceId_, proj_cbb_->currentText().latin1(), bx);

			emit saveWFSTheme(p);
		}
	}
	catch(QString& e)
	{
		QMessageBox::warning(this, tr("Theme creation failed"), e.latin1());
	}
}

void WFSConfig::preview_clicked()
{
	TeSTElementSet* eSet = 0;

	QCursor cur(Qt::WaitCursor);

	setCursor(cur);

	try
	{
		TeOGC::TeWFSFeatureType* feat = getCurrentFeature();

		TeOGC::TeWFSThemeParams par(feat->getName().getFullName(), feat->getName().getFullName(),  proj_cbb_->currentText().latin1());

		par.serviceId_ = currentServiceId_;
		par.namespaceURI_ = feat->getName().getURI();

		TeOGC::TeWFSTheme theme(par);

		TeBox bx = feat->getWGS84BoundingBox()[0]->getBBox(getProjectionId());

		theme.setThemeBox(bx);

		eSet = theme.getDataSet(bx, 1000, false);

		canvas_->setWorld(theme.getThemeBox(), canvas_->width(), canvas_->height());
		canvas_->setProjection(theme.getThemeProjection());

		canvas_->clear();

		drawObjects(eSet, canvas_, theme.defaultLegend(), 0);
	}
	catch(TeException& e)
	{

	}

	setCursor(QCursor());
}

void WFSConfig::server_textChanged( const QString & txt)
{
	bool enable = !txt.isEmpty();

	refresh_themes_btn_->setEnabled(enable);
}

void WFSConfig::setDatabase(TeDatabase* db)
{
	db_ = db;
}

void WFSConfig::cleanThemeArgs()
{
	gml_vrs_cbb_->clear();
	proj_cbb_->clear();
	x1_lne_->clear();
	y1_lne_->clear();
	x2_lne_->clear();
	y2_lne_->clear();
	op_lst_->clear();

	canvas_->clear();

	canvas_->copyPixmapToWindow();

	save_btn_->setEnabled(false);

	preview_btn_->setEnabled(false);

	configParams_->page(2)->setEnabled(false);
}

void WFSConfig::cleanThemesConfig()
{
	themes_lst_->setNumRows(0);
	cleanThemeArgs();

	delete cap_;
	cap_ = 0;

	feats_.clear();

	configParams_->page(1)->setEnabled(false);
}

TeOGC::TeWFSFeatureType* WFSConfig::getCurrentFeature() const
{
	TeOGC::TeWFSFeatureType* feat = 0;

	size_t idx = themes_lst_->currentRow();

	if(idx >= 0 && idx < feats_.size())
		feat = feats_[idx];

	return feat;
}

int WFSConfig::getProjectionId() const
{
	int proj = TeOGC::TeGetEPSGFromText(proj_cbb_->currentText().latin1());

	return proj;
}

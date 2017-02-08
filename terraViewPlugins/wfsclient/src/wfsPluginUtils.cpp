#include <wfsPluginUtils.h>

//TerraOGC include files
#include <common/TeOGCException.h>

#include <owstheme/TeOWSThemeServiceInfo.h>
#include <owstheme/TeOWSThemeServiceInfoManager.h>
#include <owstheme/TeOWSThemeServiceType.h>
#include <owstheme/TeOWSThemeServiceOperation.h>

#include <wfs/TeWFSService.h>
#include <wfs/TeWFSCapabilities.h>
#include <wfs/TeWFSGetCapabilities.h>
#include <wfstheme/TeWFSTheme.h>

#include <wfsclient/TeWFSClient.h>

//TerraLib include files
#include <TeDatabase.h>
//#include <TeQtCanvas.h>
#include <TeVectorRemap.h>
#include <TeSTElementSet.h>
#include <TeUtils.h>
#include <TeProgress.h>

//Qt3 include files
#include <qmessagebox.h>
#include <qtable.h>

//STL include files
#include <string>
#include <vector>

#define MESSAGEBOXTITLE QObject::tr("WFS Client Plugin")

TeAttribute getPkeyName(const TeAttributeList& lst, const int& layerId)
{
	std::string objectIdAtt = "object_id_" + Te2String(layerId),
				s2 = TeConvertToUpperCase(objectIdAtt);

	size_t j;
	int count = 0;

	while (true)
	{
		for (j=0; j<lst.size(); j++)
		{
			std::string s0 = lst[j].rep_.name_,
						s1 = TeConvertToUpperCase(s0);
			if ( s1 == s2)
				break;
		}
		if (j < lst.size())
		{
			++count;
			objectIdAtt = "object_id_" + Te2String(count);
		}
		else
			break;
	}
	TeAttribute at;
	at.rep_.type_ = TeSTRING;
	at.rep_.numChar_ = 16;
	at.rep_.name_ = objectIdAtt;
	at.rep_.isPrimaryKey_ = true;
//	attList.push_back(at);
//	linkCol = attList.size()-1;
//	autoIndex = true;

	return at;
}

//returns the service identifier, or -1 if the operation fails
int saveService(TeDatabase* db, const std::string& url, QWidget* parent)
{
	std::string title;
	std::string version;

	TeOGC::TeWFSClient client;
	client.setServiceURL(url);

	TeOGC::TeWFSCapabilities* cap = client.getCapabilities(TeOGC::TeWFSGetCapabilities("1.0.0"));

	if(cap->getService() == 0)	
	{
		QMessageBox::critical(parent, MESSAGEBOXTITLE, QObject::tr("The capabilities version is higher than 1.0.0"));
		return -1;
	}
	title = cap->getService()->getTitle();
	version = cap->getVersion();

	delete cap;

	TeOGC::TeOWSThemeServiceInfo* serviceInfo = TeOGC::TeOWSThemeServiceInfoManager::instance().findServiceInfoByOperationURL(url);

	if(serviceInfo)
		return serviceInfo->getId();

/* otherwise we save its information */
	serviceInfo = new TeOGC::TeOWSThemeServiceInfo;
	serviceInfo->setTitle(title);
	serviceInfo->setType(TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceType("WFS"));

	TeOGC::TeOWSThemeServiceInfoOperation op;
	op.setServiceOperation(TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceOperation("GetCapabilities"));
	op.setServiceInfo(serviceInfo);
	op.setOperationURL(url);
	serviceInfo->insert(op);

	TeOGC::TeOWSThemeServiceInfoOperation op1;
	op1.setServiceOperation(TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceOperation("GetFeature"));
	op1.setServiceInfo(serviceInfo);
	op1.setOperationURL(url);
	serviceInfo->insert(op1);

	TeOGC::TeOWSThemeServiceInfoOperation op2;
	op2.setServiceOperation(TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceOperation("DescribeFeature"));
	op2.setServiceInfo(serviceInfo);
	op2.setOperationURL(url);
	serviceInfo->insert(op2);

	serviceInfo->setVersion(version);

	try
	{
		TeOGC::TeOWSThemeServiceInfo::save(db, serviceInfo);
	}
	catch(const TeOGC::TeOGCException& e)
	{
		QMessageBox::critical(parent, MESSAGEBOXTITLE, e.getErrorMessage().c_str());
		delete serviceInfo;

		return -1;
	}
	catch(...)
	{
		QMessageBox::critical(parent, MESSAGEBOXTITLE, QObject::tr("Unexpected error..."));
		delete serviceInfo;

		return -1;
	}

	return serviceInfo->getId();
}

void updateServicesTable(QTable* table)
{
	table->setNumRows(0);

	TeOGC::TeOWSThemeServiceType* serviceType = TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceType("WFS");

	std::vector<TeOGC::TeOWSThemeServiceInfo*> services;
	TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().getServicesInfo(services);

	table->setNumRows(0);
	table->setColumnStretchable(0, true);
	table->setColumnStretchable(1, true);

    for(size_t i = 0; i < services.size(); ++i)
    {
		if(services[i]->getType()->getId() != serviceType->getId())
			continue;

		table->setNumRows(table->numRows()+1);

        std::string title = services[i]->getTitle();
		std::string url("");

		for(size_t j = 0; j < services[i]->getOperations().size(); ++j)
		{
			if(TeConvertToUpperCase(services[i]->getOperations()[j].getServiceOperation()->getName()) == std::string("GETCAPABILITIES"))
			{
				url = services[i]->getOperations()[j].getOperationURL();
				break;
			}
		}

		table->setText(table->numRows()-1, 0, title.c_str());
		table->setText(table->numRows()-1, 1, url.c_str());
    }
}

TeBox getWorldInThemeProjection(TeView* view, TeAbstractTheme* theme)
{
	TeBox bx = view->getCurrentBox();
	TeProjection* vProj = view->projection();
	TeProjection* thProj = theme->getThemeProjection();

	if(!(*vProj == *thProj))
		bx = TeRemapBox(bx, vProj, thProj);

	return bx;
}

TeLayer* importWFS(TeOGC::TeWFSTheme* theme, TeDatabase* db, const TeBox& box, const std::vector<std::string>& attrs, const std::string& layerName)
{
	TeSTElementSet* eSet = 0;
	TeLayer* layer = 0;
	int hits = 0;

	try
	{
		if(TeProgress::instance() != 0)
		{
			TeProgress::instance()->reset();
			TeProgress::instance()->setCaption(QObject::tr("Get information from server").latin1());
			TeProgress::instance()->setMessage(QObject::tr("Please wait. This operation may take a few minutes...").latin1());
//			TeProgress::instance()->;
		}

		hits = theme->getCountFeatures(box);

		eSet = theme->getDataSet(box, attrs, 5000);

		if(TeProgress::instance() != 0)
		{
			TeProgress::instance()->reset();
			TeProgress::instance()->setCaption(QObject::tr("Importing WFS data").latin1());
			TeProgress::instance()->setMessage(QObject::tr("Please wait. This operation may take a few minutes...").latin1());
			TeProgress::instance()->setTotalSteps(hits);
		}

		if(eSet == 0)
			throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to get element set from WFS theme.").latin1());

		TeProjection* lProj = TeProjectionFactory::make(theme->getThemeProjection()->params());

		TeBox bx = box;

		layer = new TeLayer(layerName, db, bx, lProj);
		TeAttributeList lst = eSet->getAttributeList();

		TeAttribute pkey = getPkeyName(lst, layer->id());
		lst.push_back(pkey);

		TeTable tab(layer->name(), lst, pkey.rep_.name_, pkey.rep_.name_, TeAttrStatic);

		if(!layer->createAttributeTable(tab))
			throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to insert attribute table in layer.").latin1());

		TeSTElementSet::iterator it = eSet->begin();
		TeSTElementSet::iterator itEnd = eSet->end();

		TePolygonSet polSet;
		TeLineSet lSet;
		TePointSet ptSet;

		int offSet = layer->getNewObjectId();
		int x = 0;

		while(it != itEnd)
		{
			TeSTInstance inst = *it;

			std::string value;
			TeTableRow row;

			for(size_t i=0; i<lst.size()-1; i++)
			{
				inst.getPropertyValue(lst[i].rep_.name_, value);
				row.push_back(value);
			}

			std::string oId = Te2String(x+offSet);

			row.push_back(oId);

			tab.add(row);

			if(it->hasPolygons())
			{
				it->getPolygons().objectId(oId);
				polSet.copyElements(it->getPolygons());
			}
			if(it->hasLines())
			{
				it->getLines().objectId(oId);
				lSet.copyElements(it->getLines());
			}
			if(it->hasPoints())
			{
				it->getPoints().objectId(oId);
				ptSet.copyElements(it->getPoints());
			}

			if((x % 100) == 0)
			{
				if(!polSet.empty())
					if(!layer->addPolygons(polSet))
						throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to insert polygons in layer.").latin1());

				if(!lSet.empty())
					if(!layer->addLines(lSet))
						throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to insert lines in layer.").latin1());

				if(!ptSet.empty())
					if(!layer->addPoints(ptSet))
						throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to insert points in layer.").latin1());

				if(!layer->saveAttributeTable(tab))
					throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to save attributes in layer.").latin1());

				tab.clear();
				polSet.clear();
				lSet.clear();
				ptSet.clear();

				if(TeProgress::instance() != 0)
				{
					TeProgress::instance()->setProgress(x);

					if(TeProgress::instance()->wasCancelled())
					{
						TeProgress::instance()->cancel();
						throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Import cancelled.").latin1());
					}
				}
			}

			x++;
			++it;
		}

		if(tab.size() > 0)
		{
			if(!polSet.empty())
				if(!layer->addPolygons(polSet))
					throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to insert polygons in layer.").latin1());

			if(!lSet.empty())
				if(!layer->addLines(lSet))
					throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to insert lines in layer.").latin1());

			if(!ptSet.empty())
				if(!layer->addPoints(ptSet))
					throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to insert points in layer.").latin1());

			if(!layer->saveAttributeTable(tab))
				throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to save attributes in layer.").latin1());
		}

		if(TeProgress::instance() != 0)
			TeProgress::instance()->reset();
	}
	catch(TeException& e)
	{
		if(TeProgress::instance() != 0)
			TeProgress::instance()->cancel();

		delete eSet;

		if(layer != 0)
			db->deleteLayer(layer->id());

//		delete layer;

		throw e;
	}

	delete eSet;

	return layer;
}

bool layerExists(const std::string& layerName, TeDatabase* db)
{
	TeLayerMap lmap = db->layerMap();
	TeLayerMap::iterator it;

	for(it = lmap.begin(); it != lmap.end(); ++it)
		if(TeStringCompare(it->second->name(), layerName))
			return true;

	return false;
}
/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

// TerraView WMS Plugin include files
#include "WMSClientPluginUtils.h"
#include "WMSPropertiesWindow.h"
#include "WMSClientTerraViewUtils.h"

// TerraOGC WMS Theme include files
#include <wmstheme/TeWMSTheme.h>

// TerraOGC WMS include files
#include <wms/TeWMSResponse.h>

// TerraOGC OWS include files
#include <ows/TeOWSMimeType.h>

// TerraOGC Common include files
#include <common/TeOGCException.h>

// TerraOGC XML include files
#include <xml/TeXMLDecoder.h>

// TerraOGC SLD include files
#include <sld/TeSLDStyledLayerDescriptor.h>

// TerraView Support include files
#include <TeQtThemeItem.h>

// TerraView include files
#include <TeAppTheme.h>
#include <TeApplicationUtils.h>
#include <TeQtViewsListView.h>
#include <TeWaitCursor.h>
#include <PluginParameters.h>
#include <help.h>

// Qt include files
#include <qlistview.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qinputdialog.h>
#include <qfiledialog.h>

#define MESSAGEBOXTITLE "WMS Client Plugin"

void WMSPopupWidget::showProperties( )
{
	WMSPropertiesWindow* pWindow = new WMSPropertiesWindow(this, "WMS Plugin Properties Window", true);
	pWindow->init(themeItem_);
	pWindow->exec();
	delete pWindow;
}

void WMSPopupWidget::removeItem( )
{
	QString question = tr("Do you really want to remove theme \"") + themeItem_->text() + "\" ?";

	int response = QMessageBox::question(this, tr("Remove Item"), question, tr("Yes"), tr("No"));

	if(response != 0)
		return;

/* get theme pointer */	
	TeAppTheme* appTheme = themeItem_->getAppTheme();
	TeOGC::TeWMSTheme* baseTheme = dynamic_cast<TeOGC::TeWMSTheme*>(appTheme->getTheme());

	if(!baseTheme)
		return;

//	emit themeChanged(baseTheme->id());

	if(WMSGetPluginParameters()->getCurrentThemeAppPtr() == appTheme)
	{
		
//		WMSGetPluginParameters()->teqtgrid_ptr_->clear();
//		WMSGetPluginParameters()->teqtcanvas_ptr_->clearAll();
		WMSGetPluginParameters()->setCurrentThemeAppPtr(0);

		if(WMSGetPluginParameters()->getCurrentViewPtr()->getCurrentTheme() == baseTheme->id())
		{
			WMSGetPluginParameters()->getCurrentViewPtr()->setCurrentTheme(-1);
			WMSGetPluginParameters()->getCurrentDatabasePtr()->updateView(WMSGetPluginParameters()->getCurrentViewPtr());
		}
	}

	WMSGetPluginParameters()->teqtviewslistview_ptr_->removeThemeItem(appTheme);
	deleteAppTheme(WMSGetPluginParameters()->getCurrentDatabasePtr(), baseTheme);
}

void WMSPopupWidget::showLegend(  )
{
	TeWaitCursor waitcursor;
/* get WMS theme pointer */
	TeOGC::TeWMSTheme* theme = dynamic_cast<TeOGC::TeWMSTheme*>(themeItem_->getAppTheme()->getTheme());

	if(!theme)
		return;

/* get legend */
	std::vector<TeOGC::TeWMSResponse*> legends;

	try
	{
		theme->getLegendURL(legends);
	}
	catch(const TeOGC::TeOGCException& e)
	{
		waitcursor.resetWaitCursor();
		std::string msg  = "Could not get legend for this theme: ";
					msg += e.getErrorMessage();

		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), msg.c_str());
		return;
	}

	if(legends.empty())
		return;

/* put legend on theme item */
	for(unsigned int i = 0; i < legends.size(); ++i)
	{
		std::string imgtype = TeConvertToUpperCase(legends[i]->getMimeType()->getSubType());
	    
		QPixmap* legendimage = new QPixmap();
		legendimage->fill(QColor(255, 255, 255));
		bool result = legendimage->loadFromData((const unsigned char*)(legends[i]->getData()), legends[i]->getDataSize(), imgtype.c_str());

		if(result)
		{
			themeItem_->setOpen(true);
			QListViewItem* vitem = new QListViewItem(themeItem_);
			vitem->setPixmap(0, *legendimage);
			vitem->setSelectable(false);
		}
		else
		{
			waitcursor.resetWaitCursor();
			std::string imgNotSupported = "The image type format for the legend is not supported.";
			QMessageBox::warning(this, tr(MESSAGEBOXTITLE), imgNotSupported .c_str());
			for(unsigned int i = 0; i < legends.size(); ++i)
				delete (legends[i]);

			delete legendimage;
			return;
		}

		delete legendimage;
	}

	for(unsigned int i = 0; i < legends.size(); ++i)
		delete (legends[i]);
}

void WMSPopupWidget::init(TeQtThemeItem* themeItem, const int& x, const int& y)
{
	themeItem_ = themeItem;
	help_ = 0;

/* get WMS theme pointer */
	TeOGC::TeWMSTheme* theme = dynamic_cast<TeOGC::TeWMSTheme*>(themeItem_->getAppTheme()->getTheme());

	if(!theme)
		return;
/* build popup */

	QPopupMenu* popupMenu = new QPopupMenu(themeItem->listView());
	popupMenu->move(x, y);
	
	int menuIdx = 0;

	menuIdx = popupMenu->insertItem(tr("Hide Legend"), this, SLOT(hideLegend()));
	if(!themeItem_->firstChild() || theme->getLayerStyleName().empty())
		popupMenu->setItemEnabled(menuIdx, false);

	menuIdx = popupMenu->insertItem(tr("Show Legend"), this, SLOT(showLegend()));
	if(themeItem_->firstChild() || theme->getLayerStyleName().empty())
		popupMenu->setItemEnabled(menuIdx, false);

	popupMenu->insertSeparator();
	popupMenu->insertItem(tr("Remove"), this, SLOT(removeItem()));

	popupMenu->insertSeparator();
	popupMenu->insertItem(tr("Properties..."), this, SLOT(showProperties()));

	popupMenu->insertSeparator();
	
	// SLD Menu
	QPopupMenu* sldMenu = new QPopupMenu(themeItem->listView());
	popupMenu->insertItem(tr("SLD"), sldMenu);
	
	QPopupMenu* associateMenu = new QPopupMenu(themeItem->listView());
	
	associateMenu->insertItem(tr("URL"), this, SLOT(associateSLDURL()));
	associateMenu->insertItem(tr("File..."), this, SLOT(associateSLDFile()));

	sldMenu->insertItem(tr("Associate"), associateMenu);
	menuIdx = sldMenu->insertItem(tr("Clear"), this, SLOT(clearSLD()));
	if(theme->getSLD() == 0 && theme->getSLDUrl().empty())
		sldMenu->setItemEnabled(menuIdx, false);

	popupMenu->insertSeparator();
	popupMenu->insertItem(tr("Help"), this, SLOT(showHelp()));
	
	popupMenu->exec();

	delete associateMenu;
	delete sldMenu;
    delete popupMenu;
}

void WMSPopupWidget::hideLegend()
{
	TeWaitCursor waitcursor;

	QListViewItem* vitem = 0;
		
	while(vitem = themeItem_->firstChild())
		themeItem_->removeItem(vitem);
}

void WMSPopupWidget::showHelp()
{
	delete help_;
	help_ = new Help(0, tr(MESSAGEBOXTITLE), true);
	QString helpFile = "wmsclient.htm";
	help_->init(helpFile);
	help_->exec();
	delete help_;
	help_ = 0;
}

void WMSPopupWidget::destroy()
{
	delete help_;
	help_ = 0;
}

void WMSPopupWidget::associateSLDURL()
{
	/* get WMS theme pointer */
	TeOGC::TeWMSTheme* theme = dynamic_cast<TeOGC::TeWMSTheme*>(themeItem_->getAppTheme()->getTheme());

	if(!theme)
		return;

	bool ok;
    QString url = QInputDialog::getText(tr("SLD URL"), tr("Enter the Styled Layer Descriptor XML Document URL:"), 
									    QLineEdit::Normal, theme->getSLDUrl().c_str(), &ok, this);
    if(ok && !url.isEmpty())
	{	
		TeOGC::TeXMLDecoder* xmlDecoder = 0;
		try
		{
			xmlDecoder = OpenXML(url.latin1());
		}
		catch(const TeOGC::TeOGCException& e)
		{
			QString msg = tr("Error accessing the Styled Layer Descriptor XML Document.") + "\n";
			msg += e.getErrorMessage().c_str();
			QMessageBox::warning(this, tr(MESSAGEBOXTITLE), msg);
			return;
		}

		theme->setSLDUrl(url.latin1());

		delete xmlDecoder;

		WMSGetPluginParameters()->plotTVData();
	}
}

void WMSPopupWidget::associateSLDFile()
{
	/* get WMS theme pointer */
	TeOGC::TeWMSTheme* theme = dynamic_cast<TeOGC::TeWMSTheme*>(themeItem_->getAppTheme()->getTheme());

	if(!theme)
		return;

	QString path = QFileDialog::getOpenFileName("", "eXtensible Markup Language file (*.xml)", 
												this, 0, tr("Open"));
	if(path.isEmpty())
		return;

	TeOGC::TeXMLDecoder* xmlDecoder = 0;
	try
    {
		 xmlDecoder = OpenXML(path.latin1());
    }
	catch(const TeOGC::TeOGCException& e)
	{
		QString msg = tr("Error opening the Styled Layer Descriptor XML Document.") + "\n";
		msg += e.getErrorMessage().c_str();
		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), msg);
		return;
	}

	TeOGC::TeSLDStyledLayerDescriptor* sld = new TeOGC::TeSLDStyledLayerDescriptor();
	sld->readFromXML(xmlDecoder);
	theme->setSLD(sld);
	
	delete xmlDecoder;
	
	WMSGetPluginParameters()->plotTVData();
}


void WMSPopupWidget::clearSLD()
{
	/* get WMS theme pointer */
	TeOGC::TeWMSTheme* theme = dynamic_cast<TeOGC::TeWMSTheme*>(themeItem_->getAppTheme()->getTheme());

	if(!theme)
		return;

	theme->setSLD(0);

	WMSGetPluginParameters()->plotTVData();
}

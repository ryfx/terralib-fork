/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

//TeOGC include files
#include <wfstheme/TeWFSTheme.h>

//TerraLib include files
#include <TeAttribute.h>
#include <TeProjection.h>

#include <wfsPluginUtils.h>

//Qt3 include files
#include <qlistview.h>
#include <qmessagebox.h>

void WFSImport::init()
{
	db_ = 0;
}

void WFSImport::saveClicked()
{
	if(getThemeName().isEmpty())
	{
		QMessageBox::warning(this, tr("Fail import data"), tr("Layer name must not be empty"));
		return;
	}

	if(layerExists(getThemeName().latin1(), db_))
	{
		QMessageBox::warning(this, tr("Fail import data"), tr("Layer name already exists in the database. Try another name"));
		return;
	}

	accept();
}

void WFSImport::setTheme( TeOGC::TeWFSTheme * theme)
{
	if(theme == 0)
		return;

	QString projName = theme->getThemeProjection()->name().c_str();
	projName += " / ";
	projName += theme->getThemeProjection()->datum().name().c_str();

	proj_lne_->setText(projName);

	tname_lne_->setText(theme->name().c_str());

	setThemeAttributes(theme->getAttributes());
}

void WFSImport::setDatabase(TeDatabase* db)
{
	db_ = db;
}

std::vector<std::string> WFSImport::getAttributes() const 
{
	std::vector<std::string> res;

	if (atts_lst_->childCount() <= 0)
		return res;

	QCheckListItem* item = (QCheckListItem*)atts_lst_->firstChild();

	do
	{
		if(item->isOn())
			res.push_back(item->text().latin1());

		item = (QCheckListItem*)item->nextSibling();

	} while(item != 0);

	return res;
}


QString WFSImport::getThemeName() const
{
	return tname_lne_->text();
}

void WFSImport::setThemeAttributes( const TeAttributeList& attrs )
{
	TeAttributeList::const_iterator it;

	for(it = attrs.begin(); it != attrs.end(); ++it)
	{
		QCheckListItem* item = new QCheckListItem(atts_lst_, (*it).rep_.name_.c_str(), QCheckListItem::CheckBox);
		item->setOn(true);
	}
}
/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

//TerraLib include files
#include <TeSTElementSet.h>

//Qt3 include files 
#include <qtable.h>

void getHeaders(QTable* table, const TeAttributeList& lst)
{
	TeAttributeList::const_iterator it;

	table->setNumCols(2);

	table->horizontalHeader()->setLabel(0, QObject::tr("Attributes"));
	table->horizontalHeader()->setLabel(1, QObject::tr("Values"));

	for(it = lst.begin(); it != lst.end(); ++it)
		table->setText(it-lst.begin(), 0, (*it).rep_.name_.c_str());
}

void WFSAttr::init()
{
	attr_tbl_->setNumRows(0);
	attr_tbl_->setNumCols(0);
}

void WFSAttr::setElementSet(TeSTElementSet* set)
{
	if(set == 0)
		return;

	TeSTElementSet::iterator it = set->begin(),
							itEnd = set->end();

	TeAttributeList lst = set->getAttributeList();

	attr_tbl_->setNumRows(lst.size());

	getHeaders(attr_tbl_, lst);

	while(it != itEnd)
	{
		for(size_t row=0; row<lst.size(); row++)
		{
			TeAttribute att = lst[row];
			std::string value;

			(*it).getPropertyValue(value, row);

			if(!value.empty())
				attr_tbl_->setText(row, 1, QString::fromLatin1(value.c_str()));
		}

		++it;
	}
}

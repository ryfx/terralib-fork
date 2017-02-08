/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <TeDatabase.h>
#include <qmessagebox.h>
#include <qlabel.h>

void dlgDropDatabase::pbClose_clicked()
{

}


void dlgDropDatabase::pbDelete_clicked()
{
	QListViewItem				*item;
	std::string					databaseName;
	bool						bIsChecked=false;

	if(_update == true)
	{
		loadDatabase(_database);
		pbDelete->setText(tr("Delete"));
		return;
	}

	item=lvDatabase->firstChild();
	QListViewItemIterator it(item);	
	this->setCursor(Qt::waitCursor);
	pbDelete->setEnabled(false);
try
{
	for ( ; it.current(); ++it )
	{
		if ( it.current()->parent() )
		{
			if ( ( (QCheckListItem*)it.current() )->isOn() )
			{
				bIsChecked=true;
				databaseName=it.current()->text(0).latin1();

				if( (databaseName.empty() == true) || (_database == 0) )
				{
					it.current()->setPixmap(0,QPixmap::fromMimeSource("ticket_error.png"));
					it.current()->setText(1,tr("Connection or Invalid Database Name Error"));
				}else
				{
					if(_database->dropDatabase(databaseName) == true )
						it.current()->setPixmap(0,QPixmap::fromMimeSource("tick_circle.png"));
					else
					{
						it.current()->setPixmap(0,QPixmap::fromMimeSource("ticket_error.png"));
						it.current()->setText(1,_database->errorMessage().c_str());
					}
				}	
				this->update();
				_update=true;
				pbDelete->setText(tr("Update"));
			}
		}
	}
}catch(...)
{
	
}
	this->setCursor(Qt::arrowCursor);
	pbDelete->setEnabled(true);
	if(bIsChecked == false)
	{
		 QMessageBox::warning(this, "TerraView",tr("Select one or more databases to drop!"));
	}

}


void dlgDropDatabase::init()
{
	_database=0;
	_update=false;
}


void dlgDropDatabase::loadDatabase( TeDatabase *db )
{
	std::string								dbName;
	std::vector<std::string>				databaseList;
	std::vector<std::string>::iterator		it;
	QListViewItem							*newItem;

	_update=false;
	lvDatabase->clear();
	_database=db;
	if(db == 0 ) return;
	newItem=new QListViewItem( lvDatabase, tr("Select Database"));
	newItem->setOpen(TRUE);
	if(db->listDatabases(databaseList) == false) return;
  	for(it=databaseList.begin();it!=databaseList.end();it++)
		{	
			dbName=(*it);
			(void)new QCheckListItem( newItem, dbName.c_str(), QCheckListItem::CheckBox );
		}
}

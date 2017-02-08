/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <qapplication.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <TeQtGrid.h>
#include <terraViewBase.h>
#include <string>
#include <TeDriverSHPDBF.h>

using namespace std;

void SaveTableAsTxt::init()
{
	help_ = 0;

	mainWindow_= (TerraViewBase*)qApp->mainWidget();
	TeAppTheme* curAppTheme = mainWindow_->currentTheme();
	TeTheme* curTheme = (TeTheme*)curAppTheme->getTheme();
	if(curTheme->type() != TeTHEME)
		return;

	grid_ = mainWindow_->getGrid();

	linesComboBox->clear();
	linesComboBox->insertItem(tr("All"));
	linesComboBox->insertItem(tr("Pointed"));
	linesComboBox->insertItem(tr("Not Pointed"));
	linesComboBox->insertItem(tr("Queried"));
	linesComboBox->insertItem(tr("Not Queried"));
	linesComboBox->insertItem(tr("Pointed And Queried"));
	linesComboBox->insertItem(tr("Pointed Or Queried"));
	linesComboBox->setCurrentItem(0);

	columnsComboBox->clear();
	columnsComboBox->insertItem(tr("All"));
	columnsComboBox->insertItem(tr("Visible"));
	columnsComboBox->insertItem(tr("Selected"));
	columnsComboBox->setCurrentItem(0);

  groupBox3_2->setEnabled(true);

}

void SaveTableAsTxt::browsePushButton_clicked()
{
    QString s = QFileDialog::getSaveFileName(
                    "",
                    tr("File (*.txt *.dbf)"),
                    this,
                    tr("save file dialog"),
                    tr("Choose a filename to save under"));

	if(s.isNull() || s.isEmpty())
		return;

	if(s.findRev(".txt", -1, false) == -1 && s.findRev(".dbf", -1, false) == -1)
		s.append(".txt");

	string file = s.latin1();
	FILE* fp = fopen(file.c_str(), "r");
	if(fp)
	{
		fclose(fp);
		int response = QMessageBox::question(this, tr("File already exists"),
				 tr("Do you want to overwrite the file?"),
				 tr("Yes"), tr("No"));

		if (response != 0)
			return;
	}
	fileLineEdit->setText(s);
  string ext = TeGetExtension(s.latin1());
  if (ext == "dbf" || ext == "DBF")
    groupBox3_2->setEnabled(false);
}


void SaveTableAsTxt::okPushButton_clicked()
{
	otherCheckBox_toggled(otherCheckBox->isChecked());

	if(fileLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Put file name and try again!"));
		return;
	}

  FILE* fp;
  string ext = TeGetExtension(fileLineEdit->text().latin1());
  if (ext == "txt" || ext == "TXT")
  {
  	fp = fopen(fileLineEdit->text().latin1(), "w");
  	if(fp == 0)
	  {
		  QMessageBox::critical(this, tr("Error"), tr("The file cannot be opened!"));
		  return;
	  }
	  //fclose(fp);
  }

	int item = linesComboBox->currentItem();
	QString columnSel = columnsComboBox->currentText();

	int nfields = grid_->numCols();
	TeAttributeList& atl = grid_->getPortal()->getAttributeList();
	int st_pos = atl.size() - 1;
	int i, j;

  TeAttributeList dbfAttList;

	string ss;
	for(j=0; j<nfields; j++)
	{
		i = grid_->getColumn(j);
		if(columnSel == tr("Visible"))
		{
			if(grid_->getVisColumn(j) == false)
				continue;
		}
		else if(columnSel == tr("Selected"))
		{
			//if(colSet.find(i) == colSet.end())
			if(grid_->isColumnSelected(j, true) == false)
				continue;
		}

		string type;
		if(atl[i].rep_.type_ == TeINT)
			type = "INT";
		else if(atl[i].rep_.type_ == TeREAL)
			type = "REAL";
		else if(atl[i].rep_.type_ == TeSTRING)
			type = "STRING";
		else if(atl[i].rep_.type_ == TeDATETIME)
			type = "DATETIME";
		else
			type = "UNKNOWN";
		ss += atl[i].rep_.name_ + "_" + type + delimiter_;

    dbfAttList.push_back(atl[i]);
	}

  DBFHandle dbfH = 0;
  if (ext == "txt" || ext == "TXT")
  {
  	if(ss.size() == 0)
    {
 		  QMessageBox::critical(this, tr("Error"), tr("There is no column to export!"));
	  	return;
    }
	  ss.replace(ss.size()-1, 1, "\n");

	  if(fwrite(ss.c_str(), sizeof( char ), ss.size(), fp) < ss.size())
	  {
		  fclose(fp);
		  QMessageBox::critical(this, tr("Error"), tr("Fail to save the table to TXT!"));
		  return;
	  }
  }
  else
  {
    if (dbfAttList.empty())
    {
 		  QMessageBox::critical(this, tr("Error"), tr("There is no column to export!"));
	  	return;
    }
    dbfH =  TeCreateDBFFile(fileLineEdit->text().latin1(), dbfAttList);
    if (!dbfH)
    {
 		  QMessageBox::critical(this, tr("Error"), tr("Fail to save the table to DBF!"));
		  return;
    }
  }

  unsigned int nr=0;
	if(grid_->getPortal()->fetchRow(0))
	{
		do
		{
			int status = atoi(grid_->getPortal()->getData(st_pos));

			bool readLine = false;
			if(item == 0)
				readLine = true;
			else if(item == 1 && (status & 1) == 1)
				readLine = true;
			else if(item == 2 && (status & 1) == 0)
				readLine = true;
			else if(item == 3 && (status & 2) == 2)
				readLine = true;
			else if(item == 4 && (status & 2) == 0)
				readLine = true;
			else if(item == 5 && (status & 1) == 1 && (status & 2) == 2)
				readLine = true;
			else if(item == 6 && ((status & 1) == 1 || (status & 2) == 2))
				readLine = true;

			if(readLine == false)
				continue;

			ss.clear();

			for(j=0; j<nfields; j++)
			{
				i = grid_->getColumn(j);
				if(columnSel == tr("Visible"))
				{
					if(grid_->getVisColumn(j) == false)
						continue;
				}
				else if(columnSel == tr("Selected"))
				{
					//if(colSet.find(i) == colSet.end())
					if(grid_->isColumnSelected(j, true) == false)
						continue;
				}
        if (ext == "txt" || ext == "TXT")
        {
				  ss += grid_->getPortal()->getData(i);
          ss += delimiter_;
        }
        else
        {
          if (dbfAttList[j].rep_.type_ == TeSTRING || dbfAttList[j].rep_.type_ == TeDATETIME)
					{
						DBFWriteStringAttribute(dbfH, nr, j, grid_->getPortal()->getData(i));
					}
					else if (dbfAttList[j].rep_.type_ == TeINT)
					{
						DBFWriteIntegerAttribute(dbfH, nr, j, atoi(grid_->getPortal()->getData(i)));
					}
					else if (dbfAttList[j].rep_.type_ == TeREAL)
					{
            double ff = atof(grid_->getPortal()->getData(i));
						DBFWriteDoubleAttribute(dbfH, nr, j, ff);
					}
        }
			}
      if (ext == "txt" || ext == "TXT")
      {
			  ss.replace(ss.size()-1, 1, "\n");
			  if(fwrite(ss.c_str(), sizeof( char ), ss.size(), fp) < ss.size())
			  {
				  fclose(fp);
				  QMessageBox::critical(this, tr("Error"), tr("Fail to save the table!"));
				  return;
			  }
      }
      ++nr;
		}
		while(grid_->getPortal()->fetchRow());
	}
  if (ext == "txt" || ext == "TXT")
  {
    fclose(fp);
  }
  else
  {
 		DBFClose(dbfH);
  }

	QMessageBox::information(this, tr("Save Table As"),
		tr("The save operation was executed successfully!"));
	accept();
	hide();
}


void SaveTableAsTxt::cancelPushButton_clicked()
{
	reject();
	hide();
}


void SaveTableAsTxt::helpPushButton_clicked()
{

}


void SaveTableAsTxt::otherCheckBox_toggled( bool b)
{
	if(tabRadioButton->isOn())
		delimiter_ = "\t";
	else if(pointRadioButton->isOn())
		delimiter_ = ".";
	else if(semicolonRadioButton->isOn())
		delimiter_ = ";";
	else if(colonRadioButton->isOn())
		delimiter_ = ":";
	else if(closebracketRadioButton->isOn())
		delimiter_ = "]";
	else if(openbracketRadioButton->isOn())
		delimiter_ = "[";

	if(b)
	{
		delimitersButtonGroup->setEnabled(false);
		delimiterLineEdit->setEnabled(true);
		if(delimiterLineEdit->text().isEmpty() ==  false)
		{
			QString t = delimiterLineEdit->text();
			if(t.length() > 1)
			{
				t.remove(1, t.length() - 1);
				delimiterLineEdit->setText(t);
			}
			delimiter_ = delimiterLineEdit->text().latin1();
		}
	}
	else
	{
		delimitersButtonGroup->setEnabled(true);
		delimiterLineEdit->setEnabled(false);
	}
}



void SaveTableAsTxt::delimiterLineEdit_textChanged( const QString & )
{
	QString t = delimiterLineEdit->text();
	if(t.length() > 1)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Delimiter has only one character!"));
		t.remove(1, t.length() - 1);
		delimiterLineEdit->setText(t);
	}
}


void SaveTableAsTxt::show()
{
	TeAppTheme* curAppTheme = mainWindow_->currentTheme();
	TeTheme* curTheme = (TeTheme*)curAppTheme->getTheme();
	if(curTheme->type() == TeTHEME)
		QDialog::show();
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
	}
}

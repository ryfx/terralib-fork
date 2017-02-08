/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright © 2001-2007 INPE and Tecgraf/PUC-Rio.
This file is part of TerraView. TerraView is free software; you can redistribute it 
and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

You should have received a copy of the GNU General Public License
along with TerraView.
The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The software provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use of
this program and its documentation.
*************************************************************************************/

#include <qstringlist.h>
#include <qcolordialog.h>
#include <qfontdatabase.h>
#include <qvariant.h>
#include <TeWaitCursor.h>
#include <terraViewBase.h>
#include <createTextRepresentation.h>
#include <selectTextRepresentation.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <TeAppTheme.h>


void InsertSymbol::init()
{
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	TeAppTheme* curAppTheme = mainWindow_->currentTheme();
	TeTheme* curTheme = (TeTheme*)curAppTheme->getTheme();

	if(curTheme->type() != TeTHEME)
	{
		erro_ = false;
		return;
	}

	if((curTheme->layer()->geomRep() & TeTEXT) == 0)
	{
		int response = QMessageBox::question(mainWindow_, tr("Create Text Representation"),
				tr("To insert text you need to create a text representation. Continue?"),
				tr("Yes"), tr("No"));

		if (response != 0)
		{
			reject();
			return;
		}

		CreateTextRepresentation w(mainWindow_, "create text representation", true);
		w.exec();
	}
	else if(curAppTheme->textTable().empty())
	{
		int response = QMessageBox::question(mainWindow_, tr("Select Text Representation"),
				tr("To insert text you need to select a text representation. Continue?"),
				tr("Yes"), tr("No"));

		if (response != 0)
		{
			reject();
			return;
		}

		SelectTextRepresentation w(mainWindow_, "select text representation", true);
		w.exec();

		if(curAppTheme->textTable().empty())
		{
			reject();
			return;
		}
	}

	if((curTheme->visibleRep() & TeTEXT) == 0)
		curTheme->visibleRep(curTheme->visibleRep() | TeTEXT);

	help_ = 0;

	erro_ = false;
	color_ = QColor(black);
	colorPushButton->setPaletteBackgroundColor(color_);
	colorPushButton->erase();

	table->setBackgroundColor(white);
	table->setReadOnly(true);

	QFontDatabase fontDB;
	QStringList slist = fontDB.families();
	QValueListIterator<QString> it;
	for(it=slist.begin(); it != slist.end(); ++it)
	{
		QString s = (*it);
		fontComboBox->insertItem(s);
	}

	fontSizeComboBox->setEditable(true);
	for(int i=8; i<31; ++i)
	{
		QVariant v(i);
		fontSizeComboBox->insertItem(v.toString());
	}
	fontSizeComboBox->setCurrentItem(4);
	fontSizeComboBox_activated(fontSizeComboBox->currentText());
}


void InsertSymbol::okPushButton_clicked()
{
	accept();
}


void InsertSymbol::reject()
{
	hide();
}


void InsertSymbol::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("insertSymbol.htm");
	if(help_->erro_ == false)
	{
		help_->show();
		help_->raise();
	}
	else
	{
		delete help_;
		help_ = 0;
	}
}


void InsertSymbol::closeEvent( QCloseEvent * )
{
	reject();
}


void InsertSymbol::fontComboBox_activated( const QString& family)
{
	TeWaitCursor wait;
	QString s;
	int	i, j;

	for(i=0; i<(int)fontComboBox->count(); ++i)
	{
		QString s = fontComboBox->text(i);
		if(s == family)
		{
			fontComboBox->setCurrentItem(i);
			break;
		}
	}

	table->setNumRows(0);
	table->setNumCols(0);
	QString f = fontComboBox->currentText();
	font_ = QFont(f);
	QFontMetrics fontMetrics(font_);

	int size = fontSizeComboBox->currentText().toInt();
	if(size == 0)
		size = 12;
	font_.setPointSize(size);
	unsigned short	a = 0;
	i = j = 0;
	while(a<255)
	{
		QChar ch(++a);
		unsigned short un = ch.unicode();
		if(ch.isNull()==false && fontMetrics.inFont(ch) && ch.isSpace()==false && un > 0)
		{
			if(ch.isPrint() || ch.isPunct() || ch.isMark() || ch.isLetterOrNumber() || ch.isDigit() || ch.isSymbol())
					j++;
		}
	}

	int tot = j;
	int rows = tot / 16;
	if(tot%16)
		rows++;

	table->setFont(font_);
	table->setNumRows(rows);
	table->setNumCols(16);
	table->setLeftMargin(size*2);
	table->setTopMargin(size*2);

	a = 0;
	for(i=0; i<table->numRows() && (int)a<255; ++i)
	{
		j = 0;
		while(j<table->numCols() && (int)a<255)
		{
			QChar ch(++a);
			if(ch.isNull()==false && fontMetrics.inFont(ch) && ch.isSpace()==false)
			{
				if(ch.isPrint() || ch.isPunct() || ch.isMark() || ch.isLetterOrNumber() || ch.isDigit() || ch.isSymbol())
				{
					QString s(ch);
					table->setText(i, j++, s);
				}
			}
		}
	}
	for(i=0; i<table->numCols(); ++i)
	{
		QVariant v(i+1);
		table->horizontalHeader()->setLabel(i, v.toString());
	}
	for(i=0; i<table->numRows(); ++i)
	{
		QVariant v(i+1);
		table->verticalHeader()->setLabel(i, v.toString());
	}

	for(i=0; i<table->numRows(); ++i)
		table->adjustRow(i);
	for(i=0; i<table->numCols(); ++i)
		table->adjustColumn(i);
}


void InsertSymbol::fontSizeComboBox_activated( const QString & )
{
	QString family = fontComboBox->currentText();
	fontComboBox_activated(family);
}

void InsertSymbol::table_clicked( int row, int col, int /* button */, const QPoint &  /* p */ )
{
	selString_ = table->text(row, col);
}


void InsertSymbol::table_doubleClicked( int row, int col, int /* button */, const QPoint & /* p */ )
{
	selString_ = table->text(row, col);
	okPushButton_clicked();
}


void InsertSymbol::colorPushButton_clicked()
{
}




void InsertSymbol::colorPushButton_released()
{
	bool isOK = false;

	QColor	qc = QColorDialog::getRgba (colorPushButton->paletteBackgroundColor().rgb(), &isOK, this );
	if (isOK)
		setColor(qc);
}


void InsertSymbol::setColor( QColor qc)
{
	color_ = qc;
	colorPushButton->setPaletteBackgroundColor(qc);
	colorPushButton->erase();
}


void InsertSymbol::setFixedSize( bool b)
{
	fixedSizeCheckBox->setChecked(b);
}


void InsertSymbol::show()
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

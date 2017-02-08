/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright � 2001-2007 INPE and Tecgraf/PUC-Rio.
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

#include <TeLayer.h>
#include <terraViewBase.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <TeWaitCursor.h>
#include <TeDatabase.h>
#include <TeDatabaseUtils.h>
#include <mediaDescription.h>
#include <qtextedit.h>
#include <qpopupmenu.h>
#include <qfiledialog.h>

#include <TeApplicationUtils.h>


void LayerProperties::init()
{
	edit_ = false;
	row_ = -1;
	col_ = -1;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();    
	connect(layerPropertiesTable,SIGNAL(pressed(int, int, int, const QPoint&)), this, SLOT(mousePressedSlot(int, int, int, const QPoint&)));	
	connect(layerPropertiesTable,SIGNAL(doubleClicked(int, int, int, const QPoint&)), this, SLOT(mouseDoubleClickedSlot(int, int, int, const QPoint&)));	
	connect(layerPropertiesTable,SIGNAL(valueChanged(int, int)), this, SLOT(valueChangedSlot(int, int)));	
	connect(layerPropertiesTable,SIGNAL(contextMenuRequested (int, int, const QPoint&)), this, SLOT(contextMenuSlot(int, int, const QPoint&)));	
}


void LayerProperties::init( TeQtLayerItem * layerItem )
{
	layerPropertiesTable->setNumCols(2);
	layerPropertiesTable->horizontalHeader()->setLabel(0, tr("Parameter"));
	layerPropertiesTable->horizontalHeader()->setLabel(1, tr("Value"));

	vector<QString> param;
	vector<QString> val;
	string s;

	layer_ = layerItem->getLayer();
	TeDatabase *db = layer_->database();
	//TeDatabase *db = mainWindow_->currentDatabase();

	if(db->tableExist("te_layer_metadata") == false)
	{
		int ret = createTeLayerMetadata(db);
		if(ret == 1)
		{
			QMessageBox::critical(this, "Error", 
				tr("Fail to create the table \"te_layer_metadata\" !"));
			return;
		}
		else if (ret == 2)
		{
			QMessageBox::warning(this, "Warning", 
				tr("Fail to create the integrity between the tables \"te_layer_metadata\" and \"te_layer\" !"));
			return;
		}
	}

	TeDatabasePortal* portal = db->getPortal();

	string selm = "SELECT * FROM te_layer_metadata WHERE layer_id = " + Te2String(layer_->id());
	portal->freeResult();
	if(portal->query(selm))
	{
		param.push_back(tr("Name"));
		param.push_back(tr("Author"));
		param.push_back(tr("Source"));
		param.push_back(tr("Quality"));
		param.push_back(tr("Description"));
		param.push_back(tr("Date"));
		param.push_back(tr("Hour"));
		param.push_back(tr("Transf"));
		if(portal->fetchRow())
		{
			string name = portal->getData(1);			
			val.push_back(name.c_str());
			string author = portal->getData(2);			
			val.push_back(author.c_str());
			string source = portal->getData(3);			
			val.push_back(source.c_str());
			string quality = portal->getData(4);			
			val.push_back(quality.c_str());
			string description = portal->getData(5);
			val.push_back(description.c_str());
			string date = portal->getData(6);			
			val.push_back(date.c_str());
			string hour = portal->getData(7);			
			val.push_back(hour.c_str());
			string transf = portal->getData(8);			
			val.push_back(transf.c_str());
		}
		else
		{
			portal->freeResult();
			string ins = "INSERT INTO te_layer_metadata (layer_id) VALUES (" + Te2String(layer_->id()) + ")";
			db->execute(ins);
			val.push_back("");
			val.push_back("");
			val.push_back("");
			val.push_back("");
			val.push_back("");
			val.push_back("");
			val.push_back("");
			val.push_back("");
		}
	}

	param.push_back(tr("Infolayer"));
	val.push_back(layer_->name().c_str());
	param.push_back(tr("Infolayer Identifier"));
	val.push_back(QString("%1").arg(layer_->id()));

	char fmt[100];
	if (layer_->projection()->name() == "NoProjection" ||
		layer_->projection()->name() == "LatLong")
		strcpy(fmt,"%.10f");
	else
		strcpy(fmt,"%.3f");

	QString myValue;
	param.push_back(tr("Initial Coordinate(X)"));
	myValue.sprintf(fmt,layer_->box().x1_);
	val.push_back(myValue);
    param.push_back(tr("Initial Coordinate(Y)"));
	myValue.sprintf(fmt,layer_->box().y1_);
	val.push_back(myValue);
	param.push_back(tr("Final Coordinate(X)"));
	myValue.sprintf(fmt,layer_->box().x2_);
	val.push_back(myValue);
	param.push_back(tr("Final Coordinate(Y)"));
	myValue.sprintf(fmt,layer_->box().y2_);
	val.push_back(myValue);

	param.push_back(tr("Last Edition Date"));
	val.push_back(layer_->getEditionTime().getDateTime().c_str());

	param.push_back(tr("Projection"));
	val.push_back(layer_->projection()->name().c_str());
	param.push_back(tr("Projection Identifier"));
	val.push_back(QString("%1").arg(layer_->projection()->id()));
	TeDatum datum = layer_->projection()->datum();
	param.push_back("Datum");
	val.push_back(datum.name().c_str());

	TeProjInfo pjInfo = TeProjectionInfo(layer_->projection()->name());
	if (pjInfo.hasUnits)
	{
		param.push_back(tr("Unit"));
		val.push_back(layer_->projection()->units().c_str());
	}
	if (pjInfo.hasLon0)
	{
		param.push_back(tr("Origin Longitude"));
		myValue.sprintf("%.6f",layer_->projection()->lon0()*TeCRD);
		val.push_back(myValue);
	}
	if (pjInfo.hasLat0)
	{
		param.push_back(tr("Origin Latitude"));
		myValue.sprintf("%.6f",layer_->projection()->lat0()*TeCRD);
		val.push_back(myValue);
	}
	if (pjInfo.hasOffx)
	{
		param.push_back(tr("Offset X"));
		myValue.sprintf("%.0f",layer_->projection()->offX());
		val.push_back(myValue);
	}
	if (pjInfo.hasOffy)
	{
		param.push_back(tr("Offset Y"));
		myValue.sprintf("%.0f",layer_->projection()->offY());
		val.push_back(myValue);
	}
	if (pjInfo.hasStlat1)
	{
		param.push_back(tr("Standard Parallel 1"));
		myValue.sprintf("%.6f",layer_->projection()->stLat1()*TeCRD);
		val.push_back(myValue);
	}
	if (pjInfo.hasStlat2)
	{
		param.push_back(tr("Standard Parallel 2"));
		myValue.sprintf("%.6f",layer_->projection()->stLat2()*TeCRD);
		val.push_back(myValue);
	}
	if (pjInfo.hasScale)
	{
		param.push_back(tr("Scale"));
		myValue.sprintf("%.6f",layer_->projection()->scale());
		val.push_back(myValue);
	}
	param.push_back(tr("Hemisphere"));
	if(layer_->projection()->hemisphere() == TeNORTH_HEM)
		val.push_back(tr("North"));
	else
		val.push_back(tr("South"));

//	TePOLYGONS = 1, TeLINES = 2, TePOINTS = 4, TeSAMPLES = 8, TeCONTOURS = 16,
//	TeARCS = 32, TeNODES = 64 , TeRASTER = 128 , TeTEXT = 256, TeCELLS = 512

	portal->freeResult();
	if(layer_->hasGeometry(TePOLYGONS))
	{
		param.push_back(tr("Polygons Number"));
		string conta = "SELECT COUNT(*) FROM " + layer_->tableName(TePOLYGONS);
		if (portal->query(conta) && portal->fetchRow())
				val.push_back(QString("%1").arg(portal->getInt(0)));
		else
			val.push_back("0");		
	}
	
	portal->freeResult();
	if(layer_->hasGeometry(TeLINES))
	{
		param.push_back(tr("Lines Number"));
		string conta = "SELECT COUNT(*) FROM " + layer_->tableName(TeLINES);
		if (portal->query(conta) && portal->fetchRow())
				val.push_back(QString("%1").arg(portal->getInt(0)));
		else
			val.push_back("0");
	}

	portal->freeResult();
	if(layer_->hasGeometry(TePOINTS))
	{
		param.push_back(tr("Points Number"));
		string conta = "SELECT COUNT(*) FROM " + layer_->tableName(TePOINTS);
		if (portal->query(conta) && portal->fetchRow())
				val.push_back(QString("%1").arg(portal->getInt(0)));
		else
			val.push_back("0");
	}
	
	portal->freeResult();
	if(layer_->hasGeometry(TeCELLS))
	{
		param.push_back(tr("Cells Number"));
		string conta = "SELECT COUNT(*) FROM " + layer_->tableName(TeCELLS);
		if (portal->query(conta) && portal->fetchRow())
		{
			val.push_back(QString("%1").arg(portal->getInt(0)));
			param.push_back(tr("Horizontal Resolution"));
			param.push_back(tr("Vertical Resolution"));
			conta = "SELECT res_x, res_y FROM te_representation WHERE";
			conta += " geom_table = '" + layer_->tableName(TeCELLS) + "'";
			portal->freeResult();
			if (portal->query(conta) && portal->fetchRow())
			{
				val.push_back(QString("%1").arg(portal->getData(0)));
				val.push_back(QString("%1").arg(portal->getData(1)));
			}
			else
			{
				val.push_back("Unknown");
				val.push_back("Unknown");
			}
		}
		else
			val.push_back("Unknown");
	}
	portal->freeResult();
	if (layer_->hasGeometry(TeRASTER) || layer_->hasGeometry(TeRASTERFILE))
	{
		TeRasterParams& RP = layer_->raster()->params();
		param.push_back(tr("Photometric interpretation"));
		switch(RP.photometric_[0])
		{
			case TeRasterParams::TePallete:
				val.push_back(tr("Pallete"));
				if (RP.decName() == "DB")
				{	
					param.push_back(tr("LUT Name"));
					val.push_back(RP.lutName_.c_str());
				}
  				break;
			case TeRasterParams::TeMultiBand:
				val.push_back(tr("Multi-band"));
  				break;
			case TeRasterParams::TeRGB:
				val.push_back(tr("RGB"));
  				break;
			default:
				val.push_back(tr("Unknown"));
		}
		param.push_back(tr("Lines number"));
		val.push_back(QString("%1").arg(RP.nlines_));
		param.push_back(tr("Columns number"));
		val.push_back(QString("%1").arg(RP.ncols_));
		param.push_back(tr("Bands number"));
		val.push_back(QString("%1").arg(RP.nBands()));

		param.push_back(tr("Horizontal Resolution"));
		myValue.sprintf(fmt,RP.resx_);
		val.push_back(myValue);
		param.push_back(tr("Vertical Resolution"));
		myValue.sprintf(fmt,RP.resy_);
		val.push_back(myValue);

		if (RP.dataType_[0] == TeDOUBLE)
			strcpy(fmt,"%.15f");
		else if (RP.dataType_[0] == TeFLOAT)
			strcpy(fmt,"%.10f");
		else
			strcpy(fmt,"%.0f");

		int i;
		if (RP.useDummy_)
		{
			param.push_back(tr("Has dummy"));
			val.push_back(tr("Yes"));
			for(i=0; i<RP.nBands(); ++i)
			{
				param.push_back(tr("Dummy value of band")+  QString(" %1").arg(i));
				myValue.sprintf(fmt,RP.dummy_[i]);
				val.push_back(myValue);
			}
		}
		for(i=0; i<RP.nBands(); ++i)
		{
			QString signal;
			if (RP.dataType_[i] == TeUNSIGNEDCHAR || 
				RP.dataType_[i] == TeUNSIGNEDSHORT || 
				RP.dataType_[i] == TeUNSIGNEDLONG)
				signal = tr("Unsigned");
			else
				signal = tr("Signed");

			param.push_back(tr("Number of bits per pixel of the band") + QString(" %1").arg(i));
			val.push_back(QString("%1 (%2)").arg(RP.nbitsperPixel_[i]).arg(signal));

			param.push_back(tr("Minimum value of the band") + QString(" %1").arg(i));
			myValue.sprintf(fmt,RP.vmin_[i]);
			val.push_back(myValue);

			param.push_back(tr("Maximum value of the band") + QString(" %1").arg(i));
			myValue.sprintf(fmt,RP.vmax_[i]);
			val.push_back(myValue);
		}
		param.push_back(tr("Expansible"));
		if (RP.tiling_type_ == TeRasterParams::TeExpansible)
			val.push_back(tr("Yes"));
		else
			val.push_back(tr("No"));
		if (RP.decName() == "DB")
		{
			param.push_back(tr("Block Height"));
			val.push_back(QString("%1").arg(RP.blockHeight_));
			param.push_back(tr("Block Width"));
			val.push_back(QString("%1").arg(RP.blockWidth_));
			param.push_back(tr("Table"));
		}
		else
			param.push_back(tr("Original File"));
		val.push_back(RP.fileName_.c_str());
		if (RP.compression_[0] != TeRasterParams::TeNoCompression)
		{
			param.push_back(tr("Compression Type"));
			if (RP.compression_[0] == TeRasterParams::TeZLib)
				val.push_back("Zlib");
			else
				val.push_back("JPEG");
		}
		param.push_back(tr("Decoder"));
		val.push_back(RP.decName().c_str());

		if (RP.decName()== "MEMMAP" || RP.decName()== "MEM" || RP.decName()== "RAW")
		{
			param.push_back(tr("Interleaving Mode"));
			switch(RP.interleaving_)
			{
				case TeRasterParams::TePerPixel:
					val.push_back(tr("By Pixel"));
  					break;
				case TeRasterParams::TePerLine:
					val.push_back(tr("By Line"));
  					break;
				case TeRasterParams::TePerBand:
					val.push_back(tr("By Band"));
					break;
				default:
					val.push_back(tr("Unknown"));
			}
		}
	}

	if(!layer_->mediaTable().empty())
	{
		param.push_back(tr("Media Table"));
		val.push_back(layer_->mediaTable().c_str());
	}
	string sel = "SELECT table_id, attr_table, attr_link, attr_table_type FROM te_layer_table";
	sel += " WHERE layer_id = " + Te2String(layer_->id());
	portal->freeResult();
	if(portal->query(sel))
	{
		while(portal->fetchRow())
		{
			param.push_back(tr("Attributes Table"));
			val.push_back(portal->getData(1));
			param.push_back(tr("Column Used for Link"));
			val.push_back(portal->getData(2));
			param.push_back(tr("Table Identifier"));
			val.push_back(portal->getData(0));
			param.push_back(tr("Table Type"));
			TeAttrTableType tt = static_cast<TeAttrTableType>(portal->getInt(3));
			switch (tt)
			{
			case TeAttrStatic:
				val.push_back(tr("Static"));
				break;
			case TeAttrMedia:
				val.push_back(tr("Media"));
				break;
			case TeAttrEvent:
				val.push_back(tr("Events"));
				break;
			case TeFixedGeomDynAttr:
			case TeDynGeomDynAttr:
				val.push_back(tr("Temporal"));
				break;
			case TeGeomAttrLinkTime:
				val.push_back(tr("Status"));
				break;
			case TeGeocodingData:
				val.push_back(tr("Geocoding data"));
				break;
			default:
				val.push_back(tr("Unknown"));
			}
		}
	}
	sel = "SELECT te_theme.name, te_view.name";
	sel += " FROM te_theme INNER JOIN te_view ON te_theme.view_id = te_view.view_id";
	sel += " WHERE layer_id = " + Te2String(layer_->id());
	sel += " ORDER BY te_view.view_id";
	portal->freeResult();
	if(portal->query(sel))
	{
		while(portal->fetchRow())
		{
			param.push_back(tr("View/Theme"));
			string vt = portal->getData(1);
			vt += " / ";
			vt += portal->getData(0);
			val.push_back(vt.c_str());
		}
	}
	delete portal;

	layerPropertiesTable->setNumRows(param.size());
	unsigned int i;
	for(i=0; i<param.size(); ++i)
	{
		layerPropertiesTable->setText(i, 0, param[i]);
		layerPropertiesTable->setText(i, 1, val[i]);
		if(i == 2 || i == 4 || i >= 8)
			layerPropertiesTable->setRowReadOnly(i, true);
	}
	layerPropertiesTable->adjustColumn(0);
	layerPropertiesTable->adjustColumn(1);
	layerPropertiesTable->setColumnStretchable(0, false);
	layerPropertiesTable->setColumnReadOnly(0, true);

	layerPropertiesTable->adjustRow(2);
	layerPropertiesTable->adjustRow(4);
}


void LayerProperties::mousePressedSlot( int row, int col, int /* button */, const QPoint&)
{
	if(edit_)
	{
		if(row == row_ && col == col_)
			return;
		layerPropertiesTable->endEdit(row_, col_, true, true);
		row_ = -1;
		col_ = -1;
	}
	else
	{
		if(row == 2 || row == 4)
		{
			row_ = -1;
			col_ = -1;
		}
		else if(row <= 7 && col == 1)
		{
			row_ = row;
			col_ = col;
		}
	}
	edit_ = false;
}


void LayerProperties::mouseDoubleClickedSlot( int row, int col, int /* button */, const QPoint&)
{
	if(col == 0)
		return;

	if(edit_)
	{
		layerPropertiesTable->endEdit(row_, col_, true, true);
		updateMetadata(row_, col_);
		row_ = -1;
		col_ = -1;
		edit_ = false;
	}
	if(row == 2 || row == 4)
	{
		MediaDescription* editWidget;
		if(row == 2)
		{
			editWidget= new MediaDescription(this, tr("Source"), true);
			editWidget->setCaption(tr("Source"));
		}
		else
		{
			editWidget= new MediaDescription(this, tr("Description"), true);
			editWidget->setCaption(tr("Description"));
		}

		string val = layerPropertiesTable->text(row, col).latin1();
		editWidget->descriptionTextEdit->setText(val.c_str());
		editWidget->exec();

		string newVal = editWidget->descriptionTextEdit->text().latin1();
		delete editWidget;
		if(newVal.size() > 255)
		{
			QMessageBox::warning(this,
			tr("Warning"),
			tr("The text is longer than 255 characters!\nIt will be truncated!"));
			newVal = newVal.substr(0, 255);
		}
		if(newVal.empty())
			newVal = " ";

		layerPropertiesTable->setText(row, col, newVal.c_str());
		layerPropertiesTable->adjustRow(row);
		raise();

		updateMetadata(row, col);
		row_ = -1;
		col_ = -1;
		edit_ = false;
	}
	else if(row <= 7)
	{
		row_ = row;
		col_ = col;
		edit_ = true;
	}
}


void LayerProperties::valueChangedSlot( int /* row */, int /* col */ )
{
	updateMetadata(row_, col_);
	edit_ = false;
}

void LayerProperties::updateMetadata( int row, int col )
{
	string up = "UPDATE te_layer_metadata SET ";
	string val = layerPropertiesTable->text(row, col).latin1();
	TeDatabase *db = mainWindow_->currentDatabase();

	if(row == 0)
		up += "name = '" + val + "'";
	else if(row == 1)
		up += "author = '" + val + "'";
	else if(row == 2)
		up += "source = '" + val + "'";
	else if(row == 3)
		up += "quality = '" + val + "'";
	else if(row == 4)
		up += "description = '" + val + "'";
	else if(row == 5)
		up += "date_ = '" + val + "'";
	else if(row == 6)
		up += "hour_ = '" + val + "'";
	else if(row == 7)
	{
		QString v = val.c_str();
		bool ok;
		v.toLong(&ok);
		if(ok == false)
		{
			QString msg = tr("transf should be numeric data!");
			msg += "\n" + tr("you typed") + " :" + v;
			QMessageBox::information(this, tr("Error"), msg);
			layerPropertiesTable->clearCell(7, 1);
			return;
		}
		up += "transf = " + val;
	}

	up += " WHERE layer_id = " + Te2String(layer_->id());
	if(db->execute(up) == false)
	{
		QMessageBox::critical(this, tr("Error"),
		tr("Fail to update the table of the layer metadata!"));
	}
}


void LayerProperties::contextMenuSlot( int, int, const QPoint & p )
{
	QPopupMenu* popup = new QPopupMenu;
	popup->insertItem(tr("&Save as txt..."), this, SLOT(saveTxtSlot()));
	popup->move(p.x(), p.y());
	popup->exec();
}


void LayerProperties::saveTxtSlot()
{
    QString s = QFileDialog::getSaveFileName(
                    "",
                    tr("File (*.txt)"),
                    this,
                    tr("save file dialog"),
                    tr("Choose a filename to save under"));

	if(s.isNull() || s.isEmpty())
		return;

	if(s.findRev(".txt", -1, false) == -1)
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
	if((fp = fopen(file.c_str(), "w")) == 0)
	{
		QMessageBox::critical(this, tr("Error"), tr("The file cannot be opened!"));
		return;
	}

	string ss = tr("Properties of the Layer:").latin1();
	ss += " ";
	ss += layer_->name() + "\n\n";
	if(fwrite(ss.c_str(), sizeof( char ), ss.size(), fp) < ss.size())
	{
		fclose(fp);
		QMessageBox::critical(this, tr("Error"), tr("Fail to save the table properties!"));
		return;
	}

	int	i;
	for(i=0; i<layerPropertiesTable->numRows(); i++)
	{
		ss = layerPropertiesTable->text(i, 0).latin1();
		while(ss.size() < 24)
			ss += " ";
		string sa = layerPropertiesTable->text(i, 1).latin1();
		int f = sa.find("\n");
		while(f >= 0)
		{
			sa.insert(f+1, "                        "); // 24 spaces
			f += 24;
			f = sa.find("\n", f);
		}
		ss += sa;
		ss += "\n";
		if(i < 8)
			ss += "\n";

		if(fwrite(ss.c_str(), sizeof( char ), ss.size(), fp) < ss.size())
		{
			fclose(fp);
			QMessageBox::critical(this, tr("Error"), tr("Fail to save the table properties!"));
			return;
		}
	}
	fclose(fp);

	QMessageBox::information(this, tr("Save As txt"), tr("The save operation was executed successfully!"));
}

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

#include <TeLayer.h>
#include <qmessagebox.h>
#include <qimage.h>
#include <qdir.h>
#include <TeQtGrid.h>
#include <qbitmap.h>
#include <visual.xpm>
#include "TeAbstractTheme.h"

void VisualWindow::init()
{
	help_ = 0;

	visualTabWidget->setTabEnabled(polygonPage, false);
	visualTabWidget->setTabEnabled(linePage, false);
	visualTabWidget->setTabEnabled(pointPage, false);
	visualTabWidget->setTabEnabled(textPage, false);

	// Set the points that will form the resulting polygon
	QPointArray polPointsArray(18);
	polPointsArray.setPoint(0,10,30);
	polPointsArray.setPoint(1,40,10);
	polPointsArray.setPoint(2,80,20);
	polPointsArray.setPoint(3,100,10);
	polPointsArray.setPoint(4,110,40);
	polPointsArray.setPoint(5,130,20);
	polPointsArray.setPoint(6,120,80);
	polPointsArray.setPoint(7,130,110);
	polPointsArray.setPoint(8,120,150);
	polPointsArray.setPoint(9,100,130);
	polPointsArray.setPoint(10,80,160);
	polPointsArray.setPoint(11,60,140);
	polPointsArray.setPoint(12,40,160);
	polPointsArray.setPoint(13,10,140);
	polPointsArray.setPoint(14,30,130);
	polPointsArray.setPoint(15,10,110);
	polPointsArray.setPoint(16,30,60);
	polPointsArray.setPoint(17,10,30);

	polygonPoints_ = polPointsArray;

	// Set the points that will form the resulting lines
	QPointArray linePointsArray(10);
	linePointsArray.setPoint(0,10,25);
	linePointsArray.setPoint(1,130,25);
	linePointsArray.setPoint(2,10,55);
	linePointsArray.setPoint(3,130,55);
	linePointsArray.setPoint(4,10,85);
	linePointsArray.setPoint(5,130,85);
	linePointsArray.setPoint(6,10,115);
	linePointsArray.setPoint(7,130,115);
	linePointsArray.setPoint(8,10,145);
	linePointsArray.setPoint(9,130,145);

	linePoints_ = linePointsArray;

	//Mounting the brushStyleMap
	brushStyleMap_[TePolyTypeTransparent] = Qt::NoBrush;
	brushStyleMap_[TePolyTypeFill] = Qt::SolidPattern;
	brushStyleMap_[TePolyTypeHorizontal] = Qt::HorPattern;
	brushStyleMap_[TePolyTypeVertical] = Qt::VerPattern;
	brushStyleMap_[TePolyTypeFDiagonal] = Qt::FDiagPattern;
	brushStyleMap_[TePolyTypeBDiagonal] = Qt::BDiagPattern;
	brushStyleMap_[TePolyTypeCross] = Qt::CrossPattern;
	brushStyleMap_[TePolyTypeDiagonalCross] = Qt::DiagCrossPattern;

	//Mounting the penStyleMap
	penStyleMap_[TeLnTypeContinuous] = Qt::SolidLine;
	penStyleMap_[TeLnTypeDashed] = Qt::DashLine;
	penStyleMap_[TeLnTypeDotted] = Qt::DotLine;
	penStyleMap_[TeLnTypeDashDot] = Qt::DashDotLine;
	penStyleMap_[TeLnTypeDashDotDot] = Qt::DashDotDotLine;
	penStyleMap_[TeLnTypeNone] = Qt::NoPen;
}




void VisualWindow::paintEvent( QPaintEvent * )
{
	QPainter p(resultVisual);

	int width = resultVisual->width();
	int height = resultVisual->height();

	p.eraseRect(0, 0, width, height);

	if (visualTabWidget->currentPage() == polygonPage)
	{
		QImage ima(visual);
			
		TePolyBasicType polygonBrushStyle = (TePolyBasicType)polygonBrushStyleComboBox->currentItem();
		TeLnBasicType polygonPenStyle = (TeLnBasicType)polygonPenStyleComboBox->currentItem();

		int polygonContourWidth = atoi(polygonPenWidthComboBox->currentText().latin1());
		int polygonTransparency = atoi(polygonTransparencyComboBox->currentText().latin1());

		p.drawImage(0, 0, ima, 0, 0, width, height);
		QBrush brush (polygonBrushColor_, brushStyleMap_[polygonBrushStyle]);
		p.setBrush(brush);

		QPen pen(polygonPenColor_, polygonContourWidth, penStyleMap_[polygonPenStyle]);
		p.setPen(pen);

		if(polygonTransparency == 0 && polygonBrushStyle != TePolyTypeTransparent)
			p.drawPolygon(polygonPoints_);
		else
		{
			QBitmap bm;
			bm.resize(width, height);

			//Fill bitmap with 0-bits: clipping region
			bm.fill(Qt::color0);

			// Draw polygon with 1-bits: drawing region
			QPainter maskPainter(&bm);
			QBrush bmBrush(Qt::color1, brushStyleMap_[polygonBrushStyle]);
			maskPainter.setBrush(bmBrush);
			QPen bmPen(Qt::color1, polygonContourWidth);
			maskPainter.setPen(bmPen);
			maskPainter.drawPolygon(polygonPoints_);
			maskPainter.end();

			QRegion clipRegion(bm);

			// set alpha buffer and color
			QImage img(width, height, 32);

			int transp = 255 - (polygonTransparency * 255 / 100);
			unsigned int val = (transp << 24) | (polygonBrushColor_.red() << 16) | (polygonBrushColor_.green() << 8) | polygonBrushColor_.blue();
			img.fill(val);

			img.setAlphaBuffer(true);

			// plot polygon with transparency
			p.setClipRegion(clipRegion);
			p.drawImage(0,0,img);

			//draw polygon's contour
			p.drawPolyline( polygonPoints_);
		}
	}
	else if (visualTabWidget->currentPage() == linePage)
	{
		QPen pen;
		int linePenWidth = atoi(linePenWidthComboBox->currentText().latin1());
		TeLnBasicType linePenStyle = (TeLnBasicType)linePenStyleComboBox->currentItem();
		if((linePenStyle != TeLnTypeArrow) && (linePenStyle != TeLnTypeMiddleArrow))	pen=QPen(linePenColor_, linePenWidth, penStyleMap_[linePenStyle]);
		else																			pen=QPen(linePenColor_, linePenWidth, Qt::SolidLine);

		p.setPen(pen);
		p.drawLineSegments(linePoints_);
		if(linePenStyle == TeLnTypeArrow)
		{
			plotArrow(&p,linePoints_);
		}else if(linePenStyle == TeLnTypeMiddleArrow)
		{
			plotMiddleLineArrow(&p,linePoints_);
		}
	}
	else if (visualTabWidget->currentPage() == pointPage)
	{
		int pointSize = atoi(pointSizeComboBox->currentText().latin1());
		int pointStyle = pointStyleComboBox->currentItem();

		int px = width/2;
		int py = height/2;
	
		p.setPen(pointPenColor_);

		if (pointStyle == TePtTypePlus)
		{
			p.drawLine (px - pointSize/2, py, px + pointSize/2, py);
			p.drawLine (px, py - pointSize/2, px, py + pointSize/2);
		}
		else if (pointStyle == TePtTypeStar)
		{
			p.save();
			p.translate(px, py);
			p.drawLine(0, -pointSize/2, 0, pointSize/2);
			p.rotate(45);
			p.drawLine(0,- pointSize/2, 0,pointSize/2);
			p.rotate(-90);
			p.drawLine(0, -pointSize/2, 0, pointSize/2);
			p.restore();
		}
		else if (pointStyle == TePtTypeCircle)
		{
			p.setBrush(pointPenColor_);
			p.drawChord(px - pointSize/2, py - pointSize/2, pointSize, pointSize, 0, 360*16);
		}
		else if (pointStyle == TePtTypeX)
		{
			p.drawLine(px-pointSize/2,py-pointSize/2,px+pointSize/2,py+pointSize/2);
			p.drawLine(px-pointSize/2,py+pointSize/2,px+pointSize/2,py-pointSize/2);
		}
		else if (pointStyle == TePtTypeBox)
		{
			p.fillRect(px - pointSize/2, py - pointSize/2, pointSize, pointSize, pointPenColor_);
		}
		else if (pointStyle == TePtTypeDiamond)
		{
			QPointArray pa(5);
			pa.setPoint(0, px - pointSize/2, py);
			pa.setPoint(1, px, py - pointSize/2);
			pa.setPoint(2, px + pointSize/2, py);
			pa.setPoint(3, px, py + pointSize/2);
 			pa.setPoint(4, px - pointSize/2, py);
			p.setBrush(pointPenColor_);
			p.drawPolygon(pa);
		}
		else if (pointStyle == TePtTypeHollowCircle)
		{
			p.drawArc(px - pointSize/2, py - pointSize/2, pointSize, pointSize, 0, 360*16);
		}
		else if (pointStyle == TePtTypeHollowBox)
		{
			p.drawRect(px - pointSize/2, py - pointSize/2, pointSize, pointSize);
		}
		else if (pointStyle == TePtTypeHollowDiamond)
		{
			p.drawLine (px - pointSize/2, py, px, py - pointSize/2);
			p.drawLine (px, py - pointSize/2, px + pointSize/2, py);
			p.drawLine (px + pointSize/2, py, px, py + pointSize/2);
			p.drawLine (px, py + pointSize/2, px - pointSize/2, py);
		}
	}
	else if (visualTabWidget->currentPage() == textPage)
	{
		bool bold = textBoldStyleCheckBox->isChecked();
		bool italic = textItalicStyleCheckBox->isChecked();
		QString s = textSizeLineEdit->text();
		int textSize;
		QString qs = textSizeLineEdit->text();
		if (qs.length() == 0)
			textSize = 0;
		else
			textSize = atoi(qs);

		QString textFamily = textFamilyLineEdit->text();
		if (textFamily.length() == 0)
			textFamily = "";

		QString lowCase = "ab";
		QString upperCase = "AB";

		int weight;
		if (bold == true)
			weight = QFont::Bold;
		else
			weight = QFont::Normal;

		QFont font(textFamily, textSize, weight, italic);
		p.setFont(font);
		p.setPen(textPenColor_);
		QFontMetrics fm = p.fontMetrics();

		p.drawText((width - fm.width(lowCase))/2, height/2 - fm.descent() , lowCase);
		p.drawText((width - fm.width(upperCase))/2, height - fm.descent() , upperCase);
	}

}


void VisualWindow::textFontPushButton_clicked()
{
	bool bold = textBoldStyleCheckBox->isChecked();
	bool italic = textItalicStyleCheckBox->isChecked();
	int textSize = (atoi)(textSizeLineEdit->text().latin1());
	QString textFont = textFamilyLineEdit->text();

	bool ok;

	QFont oldFont;
	oldFont.setFamily(textFont);
	oldFont.setPointSize(textSize);
	oldFont.setBold(bold);
	oldFont.setItalic(italic);
	QFont newFont = QFontDialog::getFont(&ok,oldFont,this);
	if (ok)
	{
		textFamilyLineEdit->setText(newFont.family());
		bold = newFont.bold();
		textBoldStyleCheckBox->setChecked(bold);
		italic = newFont.italic();
		textItalicStyleCheckBox->setChecked(italic);
		textSize = newFont.pointSize();
		textSizeLineEdit->setText(Te2String(textSize).c_str());
		repaint();
		textVisual_.family(newFont.family().latin1());
		textVisual_.bold(bold);
		textVisual_.italic(italic);
		textVisual_.size(textSize);
	}
}



void VisualWindow::okPushButton_clicked()
{
	TeGeomRepVisualMap& repVisualMap = legendEntry_->getVisualMap();
	TeGeomRep rep;

	TeGeomRepVisualMap::iterator it;
	for (it = repVisualMap.begin(); it != repVisualMap.end(); ++it)
	{
		rep = it->first;
		if (rep == TePOLYGONS || rep == TeCELLS)
			legendEntry_->setVisual(polygonVisual_.copy(), rep);
		else if (rep == TeLINES)
			legendEntry_->setVisual(lineVisual_.copy(), rep);
		else if (rep == TePOINTS)
			legendEntry_->setVisual(pointVisual_.copy(), rep);
		else if (rep == TeTEXT)
		{
			textVisual_.fixedSize(textFixedSizeCheckBox->isChecked());
			legendEntry_->setVisual(textVisual_.copy(), rep);
		}
	}
	accept();
}



void VisualWindow::polygonBrushColorPushButton_clicked()
{
	TeColor color;
	bool isOK = false;

	QColor	qc = QColorDialog::getRgba (polygonBrushColor_.rgb(), &isOK, this );
	if (isOK)
	{
		polygonBrushColor_ = qc;
		color.init(polygonBrushColor_.red(), polygonBrushColor_.green(), polygonBrushColor_.blue());
		polygonVisual_.color(color);
		repaint();
	}
}	


void VisualWindow::linePenColorPushButton_clicked()
{
	TeColor color;
	bool isOK = false;

	QColor	qc = QColorDialog::getRgba (linePenColor_.rgb(), &isOK, this );
	if (isOK)
	{
		linePenColor_ = qc;
		color.init(linePenColor_.red(), linePenColor_.green(), linePenColor_.blue());
		lineVisual_.color(color);
		repaint();
	}
}


void VisualWindow::pointPenColorPushButton_clicked()
{
	TeColor color;
	bool isOK = false;

	QColor	qc = QColorDialog::getRgba (pointPenColor_.rgb(), &isOK, this );
	if (isOK)
	{
		pointPenColor_ = qc;
		color.init(pointPenColor_.red(), pointPenColor_.green(), pointPenColor_.blue());
		pointVisual_.color(color);
		repaint();
	}
}


void VisualWindow::textPenColorPushButton_clicked()
{
	TeColor color;
	bool isOK = false;

	QColor	qc = QColorDialog::getRgba (textPenColor_.rgb(), &isOK, this );
	if (isOK)
	{
		textPenColor_ = qc;
		color.init(textPenColor_.red(), textPenColor_.green(), textPenColor_.blue());
		textVisual_.color(color);
		repaint();
	}
}

void VisualWindow::polygonTransparencyComboBox_activated(const QString& qtransp)
{
	int transp = atoi(qtransp.latin1());
	polygonVisual_.transparency(transp);
	repaint();
}


void VisualWindow::polygonBrushStyleComboBox_activated(int bstyle)
{
	polygonVisual_.style(bstyle);
	repaint();
}


void VisualWindow::polygonPenColorPushButton_clicked()
{
	bool isOK = false;
	QColor	qc = QColorDialog::getRgba(polygonPenColor_.rgb(), &isOK, this );
	if(isOK)
	{
		TeColor color;
		polygonPenColor_ = qc;
		color.init(qc.red(),qc.green(),qc.blue());
		polygonVisual_.contourColor(color);
		repaint();
	}
}

void VisualWindow::polygonPenStyleComboBox_activated(int pstyle)
{
	polygonVisual_.contourStyle(pstyle);
	repaint();
}



void VisualWindow::polygonPenWidthComboBox_activated(const QString& qw )
{
	int width = atoi(qw.latin1());
	polygonVisual_.contourWidth(width);
	repaint();
}


void VisualWindow::linePenWidthComboBox_activated(const QString& qwidth)
{
	int width = atoi(qwidth.latin1());
	lineVisual_.width(width);
	repaint();
}


void VisualWindow::linePenStyleComboBox_activated(int pstyle)
{
	lineVisual_.style(pstyle);
	repaint();
}


void VisualWindow::pointSizeComboBox_activated(const QString& qsize)
{
	int size = atoi(qsize.latin1());
	pointVisual_.size(size);	
}


void VisualWindow::pointStyleComboBox_activated( int style)
{
	pointVisual_.style(style);   
}


void VisualWindow::visualTabWidget_currentChanged( QWidget * /* qw */)
{
	repaint();
}


void VisualWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("visual.htm");
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



void VisualWindow::loadLegend( TeLegendEntry & legendEntry, TeAbstractTheme * theme )
{
	legendEntry_ = &legendEntry;
	TeGeomRepVisualMap& repVisualMap = legendEntry_->getVisualMap();
	TeGeomRep rep;
	TeColor color;

	TeGeomRepVisualMap::iterator it;
	for (it = repVisualMap.begin(); it != repVisualMap.end(); ++it)
	{
		rep = it->first;
		if (rep == TePOLYGONS || rep == TeCELLS || rep == TeRASTER)
			polygonVisual_ = *(legendEntry_->visual(rep));
		else if (rep == TeLINES)
			lineVisual_ = *(legendEntry_->visual(rep));
		else if (rep == TePOINTS)
			pointVisual_ = *(legendEntry_->visual(rep));
		else if (rep == TeTEXT)
			textVisual_ = *(legendEntry_->visual(rep));
	}

	if(repVisualMap.size() == 1 && rep == TeTEXT)
	{
		visualTabWidget->setTabEnabled(textPage, true);

		color = textVisual_.color();
		textPenColor_.setRgb(color.red_, color.green_, color.blue_);

		int textSize = textVisual_.size();
		textSizeLineEdit->setText(Te2String(textSize).c_str());

		string textFamily = textVisual_.family();
		textFamilyLineEdit->setText(textFamily.c_str());

		bool bold = textVisual_.bold();
		textBoldStyleCheckBox->setChecked(bold);

		bool italic = textVisual_.italic();
		textItalicStyleCheckBox->setChecked(italic);

		bool textFixedSize = textVisual_.fixedSize();
		textFixedSizeCheckBox->setChecked(textFixedSize);

		visualTabWidget->setCurrentPage(3);
	}
	else
	{
		if (!theme)
			return;

		unsigned long possibleVis = theme->visibleGeoRep();
		if ((possibleVis & TePOLYGONS) || (possibleVis & TeCELLS))
		{
			visualTabWidget->setTabEnabled(polygonPage, true);

			color = polygonVisual_.color();
			polygonBrushColor_.setRgb(color.red_, color.green_, color.blue_);

			int tr = polygonVisual_.transparency();
			polygonTransparencyComboBox->setCurrentText(Te2String(tr).c_str());

			BrushStyle polygonBrushStyle = (BrushStyle)polygonVisual_.style();
			polygonBrushStyleComboBox->setCurrentItem((int)polygonBrushStyle);

			color = polygonVisual_.contourColor();
			polygonPenColor_.setRgb(color.red_, color.green_, color.blue_);

//			int ctr = polygonVisual_.contourTransparency();
//			polygonContourLineTransparencyComboBox->setCurrentText(Te2String(ctr).c_str());

			int polygonContourWidth = polygonVisual_.contourWidth();
			polygonPenWidthComboBox->setCurrentText(Te2String(polygonContourWidth).c_str());

			PenStyle polygonPenStyle = (PenStyle)polygonVisual_.contourStyle();
			polygonPenStyleComboBox->setCurrentItem((int)polygonPenStyle);
		}

		if ((possibleVis & TeLINES))
		{
			visualTabWidget->setTabEnabled(linePage, true);

			color = lineVisual_.color();
			linePenColor_.setRgb(color.red_, color.green_, color.blue_);

			int lineWidth = lineVisual_.width();
			linePenWidthComboBox->setCurrentText(Te2String(lineWidth).c_str());

			PenStyle linePenStyle = (PenStyle)lineVisual_.style();
			linePenStyleComboBox->setCurrentItem((int)linePenStyle);
		}

		if ((possibleVis & TePOINTS))
		{
			visualTabWidget->setTabEnabled(pointPage, true);

			color = pointVisual_.color();
			pointPenColor_.setRgb(color.red_, color.green_, color.blue_);

			int pointSize = pointVisual_.size();
			pointSizeComboBox->setCurrentText(Te2String(pointSize).c_str());

			int pointStyle = pointVisual_.style();
			pointStyleComboBox->setCurrentItem(pointStyle);
		}

		if ((possibleVis & TeTEXT))
		{
			visualTabWidget->setTabEnabled(textPage, true);

			color = textVisual_.color();
			textPenColor_.setRgb(color.red_, color.green_, color.blue_);

			int textSize = textVisual_.size();
			textSizeLineEdit->setText(Te2String(textSize).c_str());

			string textFamily = textVisual_.family();
			textFamilyLineEdit->setText(textFamily.c_str());

			bool bold = textVisual_.bold();
			textBoldStyleCheckBox->setChecked(bold);

			bool italic = textVisual_.italic();
			textItalicStyleCheckBox->setChecked(italic);

			bool textFixedSize = textVisual_.fixedSize();
			textFixedSizeCheckBox->setChecked(textFixedSize);
		}

		if ((possibleVis & TePOLYGONS) || (possibleVis & TeCELLS))
			visualTabWidget->setCurrentPage(0);
		else if ((possibleVis & TeLINES))
			visualTabWidget->setCurrentPage(1);
		else if ((possibleVis & TePOINTS))
			visualTabWidget->setCurrentPage(2);
		else 
			visualTabWidget->setCurrentPage(3);
	}
}


QPointArray VisualWindow::getArrow(QPoint ptBegin, QPoint ptEnd, double size)
{
  QPointArray	points(4);
  double	slopy , cosy , siny;
	
  
  slopy = atan2((double)( ptBegin.y() - ptEnd.y() ),(double)( ptBegin.x() - ptEnd.x() ) );
  cosy = cos( slopy );
  siny = sin( slopy ); 

  points[0]=QPoint(ptEnd.x(),ptEnd.y());
  points[1]=QPoint(ptEnd.x() - int(-size*cosy-(size/2.0*siny)),ptEnd.y() - int(-size*siny+(size/2.0*cosy )));
  points[2]=QPoint(ptEnd.x() - int(-size*cosy+(size/2.0*siny)),ptEnd.y() + int(size/2.0*cosy+size*siny));
  points[3]=QPoint(ptEnd.x(),ptEnd.y());
  return points;

}




void VisualWindow::plotArrow( QPainter *painter, QPointArray lines )
{
	unsigned int i;

	painter->setBrush(Qt::SolidPattern);
	painter->setBrush(painter->pen().color());
	for(i=0;i<lines.size();i++)
	{
		painter->drawLine(lines.point(i),lines.point(i+1));

		QPointArray arrow=getArrow(lines.point(i),lines.point(i+1),10);
		painter->drawPolygon(arrow);
		i++;
	}
}


void VisualWindow::plotMiddleLineArrow( QPainter *painter, QPointArray lines )
{
	unsigned int	i;
	double			centerX;

	painter->setBrush(Qt::SolidPattern);
	painter->setBrush(painter->pen().color());
	for(i=0;i<lines.size();i++)
	{
		painter->drawLine(lines.point(i),lines.point(i+1));
		centerX=(lines.point(i).x() + lines.point(i+1).x())/2.0;
		

		QPointArray arrow=getArrow(lines.point(i),QPoint(centerX,lines.point(i+1).y()),10);
		painter->drawPolygon(arrow);
		i++;
	}
}

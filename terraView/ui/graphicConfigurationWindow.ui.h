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
#include <graphic.h>
#include <help.h>
#include <qcolordialog.h>
#include <qfontdialog.h>
#include <qwt_symbol.h>


void GraphicConfigurationWindow::init()
{
	int item;
	help_ = 0;
	timerId_ = 0;

	graphWindow_ = (GraphicWindow*)parent();
	if(graphWindow_->gploted_ == Histogram)
		GraphicConfigurationTabWidget->setCurrentPage(0);
	else if(graphWindow_->gploted_ == RasterHistogram)
		GraphicConfigurationTabWidget->setCurrentPage(1);
	else if(graphWindow_->gploted_ == Dispersion)
		GraphicConfigurationTabWidget->setCurrentPage(2);
	else if(graphWindow_->gploted_ == NormalProbability)
		GraphicConfigurationTabWidget->setCurrentPage(3);
	else if(graphWindow_->gploted_ == Skater)
		GraphicConfigurationTabWidget->setCurrentPage(4);
	else if(graphWindow_->gploted_ == Semivariogram)
		GraphicConfigurationTabWidget->setCurrentPage(5);

    horizontalMajorVisible_ = graphWindow_->horizontalMajorVisible_;
    horizontalMinorVisible_ = graphWindow_->horizontalMinorVisible_;
    verticalMajorVisible_ = graphWindow_->verticalMajorVisible_;
    normalProbCurvePen_ = graphWindow_->normalProbCurvePen_;
    normalCurvePen_ = graphWindow_->normalCurvePen_;
	skaterCurvePen_ = graphWindow_->skaterPen_;
    rasterHistogramBrush_ = graphWindow_->rasterHistogramBrush_;
    rasterHistogramPen_ = graphWindow_->rasterHistogramPen_;
    histogramPen_ = graphWindow_->histogramPen_;
    dispersionSymbol_ = graphWindow_->dispersionSymbol_;
    verticalMinorVisible_ = graphWindow_->verticalMinorVisible_;
    majorLinePen_ = graphWindow_->majorLinePen_;
    minorLinePen_ = graphWindow_->minorLinePen_;
    backgroundColor_ = graphWindow_->backgroundColor_;
    horizontalAxisFont_ = graphWindow_->horizontalAxisFont_;
    verticalAxisFont_ = graphWindow_->verticalAxisFont_;
	semivarCurvePen_ = graphWindow_->semivarCurvePen_;
	semivarAdjustCurvePen_ = graphWindow_->semivarAdjustCurvePen_;

	histogramLineStyleComboBox->clear();
	histogramLineStyleComboBox->insertItem(tr("NoPen"));
	histogramLineStyleComboBox->insertItem(tr("SolidLine"));
	histogramLineStyleComboBox->insertItem(tr("DashLine"));
	histogramLineStyleComboBox->insertItem(tr("DotLine"));
	histogramLineStyleComboBox->insertItem(tr("DashDotLine"));
	histogramLineStyleComboBox->insertItem(tr("DashDotDotLine"));
	item = (int)histogramPen_.style();
	histogramLineStyleComboBox->setCurrentItem(item);
	histogramLineStyleComboBox_activated(histogramLineStyleComboBox->currentText());

	rasterHistogramLineStyleComboBox->clear();
	rasterHistogramLineStyleComboBox->insertItem(tr("NoPen"));
	rasterHistogramLineStyleComboBox->insertItem(tr("SolidLine"));
	rasterHistogramLineStyleComboBox->insertItem(tr("DashLine"));
	rasterHistogramLineStyleComboBox->insertItem(tr("DotLine"));
	rasterHistogramLineStyleComboBox->insertItem(tr("DashDotLine"));
	rasterHistogramLineStyleComboBox->insertItem(tr("DashDotDotLine"));
	item = (int)rasterHistogramPen_.style();
	rasterHistogramLineStyleComboBox->setCurrentItem(item);
	rasterHistogramLineStyleComboBox_activated(rasterHistogramLineStyleComboBox->currentText());

	dispersionSymbolSizeComboBox->clear();
	dispersionSymbolSizeComboBox->insertItem("1");
	dispersionSymbolSizeComboBox->insertItem("2");
	dispersionSymbolSizeComboBox->insertItem("3");
	dispersionSymbolSizeComboBox->insertItem("4");
	dispersionSymbolSizeComboBox->insertItem("5");
	dispersionSymbolSizeComboBox->insertItem("6");
	dispersionSymbolSizeComboBox->insertItem("7");
	dispersionSymbolSizeComboBox->insertItem("8");
	dispersionSymbolSizeComboBox->insertItem("9");
	dispersionSymbolSizeComboBox->insertItem("10");
	dispersionSymbolSizeComboBox->insertItem("11");
	dispersionSymbolSizeComboBox->insertItem("12");
	dispersionSymbolSizeComboBox->insertItem("13");
	dispersionSymbolSizeComboBox->insertItem("14");
	dispersionSymbolSizeComboBox->insertItem("15");
	QSize qsize = dispersionSymbol_.size();
	item = qsize.width() - 1;
	dispersionSymbolSizeComboBox->setCurrentItem(item);
	dispersionSymbolSizeComboBox_activated(dispersionSymbolSizeComboBox->currentText());

	dispersionSymbolStyleComboBox->clear();
	dispersionSymbolStyleComboBox->insertItem(tr("Ellipse"));
	dispersionSymbolStyleComboBox->insertItem(tr("Rect"));
	dispersionSymbolStyleComboBox->insertItem(tr("Diamond"));
	dispersionSymbolStyleComboBox->insertItem(tr("Triangle"));
	dispersionSymbolStyleComboBox->insertItem(tr("DTriangle"));
	dispersionSymbolStyleComboBox->insertItem(tr("UTriangle"));
	dispersionSymbolStyleComboBox->insertItem(tr("LTriangle"));
	dispersionSymbolStyleComboBox->insertItem(tr("RTriangle"));
	dispersionSymbolStyleComboBox->insertItem(tr("Cross"));
	dispersionSymbolStyleComboBox->insertItem(tr("XCross"));
	item = (int)dispersionSymbol_.style() - 1;
	dispersionSymbolStyleComboBox->setCurrentItem(item);
	dispersionSymbolStyleComboBox_activated(dispersionSymbolStyleComboBox->currentText());

	skaterLineStyleComboBox->clear();
	skaterLineStyleComboBox->insertItem(tr("NoPen"));
	skaterLineStyleComboBox->insertItem(tr("SolidLine"));
	skaterLineStyleComboBox->insertItem(tr("DashLine"));
	skaterLineStyleComboBox->insertItem(tr("DotLine"));
	skaterLineStyleComboBox->insertItem(tr("DashDotLine"));
	skaterLineStyleComboBox->insertItem(tr("DashDotDotLine"));
	item = (int)skaterCurvePen_.style();
	skaterLineStyleComboBox->setCurrentItem(item);
	skaterLineStyleComboBox_activated(skaterLineStyleComboBox->currentText());

	semivarLineStyleComboBox->clear();
	semivarLineStyleComboBox->insertItem(tr("NoPen"));
	semivarLineStyleComboBox->insertItem(tr("SolidLine"));
	semivarLineStyleComboBox->insertItem(tr("DashLine"));
	semivarLineStyleComboBox->insertItem(tr("DotLine"));
	semivarLineStyleComboBox->insertItem(tr("DashDotLine"));
	semivarLineStyleComboBox->insertItem(tr("DashDotDotLine"));
	item = (int)semivarCurvePen_.style();
	semivarLineStyleComboBox->setCurrentItem(item);
	semivarLineStyleComboBox_activated(semivarLineStyleComboBox->currentText());

	adjustLineStyleComboBox->clear();
	adjustLineStyleComboBox->insertItem(tr("NoPen"));
	adjustLineStyleComboBox->insertItem(tr("SolidLine"));
	adjustLineStyleComboBox->insertItem(tr("DashLine"));
	adjustLineStyleComboBox->insertItem(tr("DotLine"));
	adjustLineStyleComboBox->insertItem(tr("DashDotLine"));
	adjustLineStyleComboBox->insertItem(tr("DashDotDotLine"));
	item = (int)semivarAdjustCurvePen_.style();
	adjustLineStyleComboBox->setCurrentItem(item);
	adjustLineStyleComboBox_activated(adjustLineStyleComboBox->currentText());


	normalLineStyleComboBox->clear();
	normalLineStyleComboBox->insertItem(tr("NoPen"));
	normalLineStyleComboBox->insertItem(tr("SolidLine"));
	normalLineStyleComboBox->insertItem(tr("DashLine"));
	normalLineStyleComboBox->insertItem(tr("DotLine"));
	normalLineStyleComboBox->insertItem(tr("DashDotLine"));
	normalLineStyleComboBox->insertItem(tr("DashDotDotLine"));
	item = (int)normalCurvePen_.style();
	normalLineStyleComboBox->setCurrentItem(item);
	normalLineStyleComboBox_activated(normalLineStyleComboBox->currentText());

	normalProbLineStyleComboBox->clear();
	normalProbLineStyleComboBox->insertItem(tr("NoPen"));
	normalProbLineStyleComboBox->insertItem(tr("SolidLine"));
	normalProbLineStyleComboBox->insertItem(tr("DashLine"));
	normalProbLineStyleComboBox->insertItem(tr("DotLine"));
	normalProbLineStyleComboBox->insertItem(tr("DashDotLine"));
	normalProbLineStyleComboBox->insertItem(tr("DashDotDotLine"));
	item = (int)normalProbCurvePen_.style();
	normalProbLineStyleComboBox->setCurrentItem(item);
	normalProbLineStyleComboBox_activated(normalProbLineStyleComboBox->currentText());

	horizontalMajorLinesVisibleCheckBox->setChecked(horizontalMajorVisible_);
	horizontalMajorLinesVisibleCheckBox_toggled(horizontalMajorVisible_);
	horizontalMinorLinesVisibleCheckBox->setChecked(horizontalMinorVisible_);
	horizontalMinorLinesVisibleCheckBox_toggled(horizontalMinorVisible_);
	verticalMajorLinesVisibleCheckBox->setChecked(verticalMajorVisible_);
	verticalMajorLinesVisibleCheckBox_toggled(verticalMajorVisible_);
	verticalMinorLinesVisibleCheckBox->setChecked(verticalMinorVisible_);
	verticalMinorLinesVisibleCheckBox_toggled(verticalMinorVisible_);

	majorLinesStyleComboBox->clear();
	majorLinesStyleComboBox->insertItem(tr("SolidLine"));
	majorLinesStyleComboBox->insertItem(tr("DashLine"));
	majorLinesStyleComboBox->insertItem(tr("DotLine"));
	majorLinesStyleComboBox->insertItem(tr("DashDotLine"));
	majorLinesStyleComboBox->insertItem(tr("DashDotDotLine"));
	item = (int)majorLinePen_.style() - 1;
	majorLinesStyleComboBox->setCurrentItem(item);
	majorLinesStyleComboBox_activated(majorLinesStyleComboBox->currentText());

	minorLinesStyleComboBox->clear();
	minorLinesStyleComboBox->insertItem(tr("SolidLine"));
	minorLinesStyleComboBox->insertItem(tr("DashLine"));
	minorLinesStyleComboBox->insertItem(tr("DotLine"));
	minorLinesStyleComboBox->insertItem(tr("DashDotLine"));
	minorLinesStyleComboBox->insertItem(tr("DashDotDotLine"));
	item = (int)minorLinePen_.style() - 1;
	minorLinesStyleComboBox->setCurrentItem(item);
	minorLinesStyleComboBox_activated(minorLinesStyleComboBox->currentText());

	majorLinesWidthComboBox->clear();
	majorLinesWidthComboBox->insertItem("1");
	majorLinesWidthComboBox->insertItem("2");
	majorLinesWidthComboBox->insertItem("3");
	majorLinesWidthComboBox->insertItem("4");
	item = (int)majorLinePen_.width() - 1;
	majorLinesWidthComboBox->setCurrentItem(item);
	majorLinesWidthComboBox_activated(majorLinesWidthComboBox->currentText());

	minorLinesWidthComboBox->clear();
	minorLinesWidthComboBox->insertItem("1");
	minorLinesWidthComboBox->insertItem("2");
	minorLinesWidthComboBox->insertItem("3");
	minorLinesWidthComboBox->insertItem("4");
	item = (int)minorLinePen_.width() - 1;
	minorLinesWidthComboBox->setCurrentItem(item);
	minorLinesWidthComboBox_activated(minorLinesWidthComboBox->currentText());

	horizontalFontLineEdit->setText(horizontalAxisFont_.family());
	verticalFontLineEdit->setText(verticalAxisFont_.family());

	paint();
}


void GraphicConfigurationWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	if(GraphicConfigurationTabWidget->currentPageIndex() == 0) // histogram
		help_->init("graphicConfigurationWindow_Histogram.htm");
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 1) // raster histogram
		help_->init("graphicConfigurationWindow_RasterHistogram.htm");
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 2) // dispersion
		help_->init("graphicConfigurationWindow_Dispersion.htm");
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 3) // normal probability
		help_->init("graphicConfigurationWindow_NormalProbability.htm");
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 4) // skater
		help_->init("graphicConfigurationWindow_Skater.htm");
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 5) // semivariogram
		help_->init("graphicConfigurationWindow_Semivariogram.htm");
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 6) // grid
		help_->init("graphicConfigurationWindow_Grid.htm");
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 7) // font
		help_->init("graphicConfigurationWindow_Fonts.htm");
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


void GraphicConfigurationWindow::histogramLineColorPushButton_clicked()
{
	bool isOK = false;

	QColor inputColor = histogramPen_.color();
	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		histogramPen_.setColor(outputColor);
		paint();
	}
}


void GraphicConfigurationWindow::histogramLineStyleComboBox_activated( const QString & )
{
	Qt::PenStyle style = (Qt::PenStyle)histogramLineStyleComboBox->currentItem();
	histogramPen_.setStyle(style);
	paint();
}


void GraphicConfigurationWindow::rasterHistogramLineColorPushButton_clicked()
{
	bool isOK = false;

	QColor inputColor = rasterHistogramPen_.color();
	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		rasterHistogramPen_.setColor(outputColor);
		paint();
	}
}


void GraphicConfigurationWindow::rasterHistogramBrushColorPushButton_clicked()
{
	bool isOK = false;

	QColor inputColor = rasterHistogramBrush_.color();
	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		rasterHistogramBrush_.setColor(outputColor);
		paint();
	}
}


void GraphicConfigurationWindow::rasterHistogramLineStyleComboBox_activated( const QString & )
{
	Qt::PenStyle style = (Qt::PenStyle)rasterHistogramLineStyleComboBox->currentItem();
	rasterHistogramPen_.setStyle(style);
	paint();
}


void GraphicConfigurationWindow::dispersionSymbolSizeComboBox_activated( const QString & )
{
	int size = dispersionSymbolSizeComboBox->currentItem() + 1;
	QSize qsize(size, size);
	dispersionSymbol_.setSize(qsize);
	paint();
}


void GraphicConfigurationWindow::dispersionSymbolStyleComboBox_activated( const QString & )
{
	QwtSymbol::Style style = (QwtSymbol::Style)(dispersionSymbolStyleComboBox->currentItem() + 1);
	dispersionSymbol_.setStyle(style);
	paint();
}

void GraphicConfigurationWindow::normalLineColorPushButton_clicked()
{
	bool isOK = false;

	QColor inputColor = normalCurvePen_.color();
	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		normalCurvePen_.setColor(outputColor);
		paint();
	}
}

void GraphicConfigurationWindow::semivarLineColorPushButton_clicked()
{
	bool isOK = false;

	QColor inputColor = semivarCurvePen_.color();
	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		semivarCurvePen_.setColor(outputColor);
		paint();
	}
}


void GraphicConfigurationWindow::normalProbLineColorPushButton_clicked()
{
	bool isOK = false;

	QColor inputColor = normalProbCurvePen_.color();
	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		normalProbCurvePen_.setColor(outputColor);
		paint();
	}
}



void GraphicConfigurationWindow::normalLineStyleComboBox_activated( const QString & )
{
	Qt::PenStyle style = (Qt::PenStyle)normalLineStyleComboBox->currentItem();
	normalCurvePen_.setStyle(style);
	paint();
}


void GraphicConfigurationWindow::normalProbLineStyleComboBox_activated( const QString & )
{
	Qt::PenStyle style = (Qt::PenStyle)normalProbLineStyleComboBox->currentItem();
	normalProbCurvePen_.setStyle(style);
	paint();
}


void GraphicConfigurationWindow::horizontalMajorLinesVisibleCheckBox_toggled( bool b)
{
	horizontalMajorVisible_ = b;
	paint();
}


void GraphicConfigurationWindow::horizontalMinorLinesVisibleCheckBox_toggled( bool b)
{
	horizontalMinorVisible_ = b;
	paint();
}


void GraphicConfigurationWindow::verticalMajorLinesVisibleCheckBox_toggled( bool b)
{
	verticalMajorVisible_ = b;
	paint();
}


void GraphicConfigurationWindow::verticalMinorLinesVisibleCheckBox_toggled( bool b)
{
	verticalMinorVisible_ = b;
	paint();
}


void GraphicConfigurationWindow::majorLinesColorPushButton_clicked()
{
	bool isOK = false;

	QColor inputColor = majorLinePen_.color();
	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		majorLinePen_.setColor(outputColor);
		paint();
	}
}


void GraphicConfigurationWindow::minorLinesColorPushButton_clicked()
{
	bool isOK = false;

	QColor inputColor = minorLinePen_.color();
	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		minorLinePen_.setColor(outputColor);
		paint();
	}
}


void GraphicConfigurationWindow::majorLinesStyleComboBox_activated( const QString & )
{
	Qt::PenStyle style = (Qt::PenStyle)(majorLinesStyleComboBox->currentItem() + 1);
	majorLinePen_.setStyle(style);
	paint();
}


void GraphicConfigurationWindow::minorLinesStyleComboBox_activated( const QString & )
{
	Qt::PenStyle style = (Qt::PenStyle)(minorLinesStyleComboBox->currentItem() + 1);
	minorLinePen_.setStyle(style);
	paint();
}


void GraphicConfigurationWindow::majorLinesWidthComboBox_activated( const QString & )
{
	int item = majorLinesWidthComboBox->currentItem() + 1;
	majorLinePen_.setWidth(item);
	paint();
}


void GraphicConfigurationWindow::minorLinesWidthComboBox_activated( const QString & )
{
	int item = minorLinesWidthComboBox->currentItem() + 1;
	minorLinePen_.setWidth(item);
	paint();
}


void GraphicConfigurationWindow::backgroundColorPushButton_clicked()
{
	bool isOK = false;

	QColor inputColor = backgroundColor_;
	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		backgroundColor_ = outputColor;
		paint();
	}
}


void GraphicConfigurationWindow::horizontalFontPushButton_clicked()
{
	bool ok;

	QFont newFont = QFontDialog::getFont(&ok, horizontalAxisFont_, this);
	if (ok)
		horizontalAxisFont_ = newFont;
	horizontalFontLineEdit->setText(horizontalAxisFont_.family());
	paint();
}


void GraphicConfigurationWindow::verticalFontPushButton_clicked()
{
	bool ok;

	QFont newFont = QFontDialog::getFont(&ok, verticalAxisFont_, this);
	if (ok)
		verticalAxisFont_ = newFont;
	verticalFontLineEdit->setText(verticalAxisFont_.family());
	paint();
}


void GraphicConfigurationWindow::paintEvent( QPaintEvent *e )
{
	QDialog::paintEvent(e);
	
	if(timerId_ == 0)
		timerId_ = startTimer(50);
}

void GraphicConfigurationWindow::timerEvent( QTimerEvent * )
{
	if(GraphicConfigurationTabWidget->currentPageIndex() == 0) // histogram
		drawHistogram();
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 1) // raster histogram
		drawRasterHistogram();
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 2) // dispersion
		drawDispersion();
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 3) // normal probability
		drawNormalProbability();
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 4) // skater
		drawSkater();
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 5) // semivariogram
		drawSemivar();
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 6) // grid
		drawGrid();
	else if(GraphicConfigurationTabWidget->currentPageIndex() == 7) // font
		drawFonts();

	killTimer(timerId_);
	timerId_ = 0;
}


void GraphicConfigurationWindow::currentChangedSlot( QWidget * )
{
	QPaintEvent e(rect());
	paintEvent(&e);
}

void GraphicConfigurationWindow::drawHistogram()
{
	histogramFrame->erase();
	QPainter painter(histogramFrame);
	painter.setPen(histogramPen_);
	painter.setBrush(graphWindow_->defaultColor_);

	QRect rect0(20, 90, 20, 50);
	QRect rect1(40, 100, 20, 40);
	QRect rect2(60, 60, 20, 80);
	QRect rect3(80, 50, 20, 90);
	QRect rect4(100, 30, 20, 110);
	QRect rect5(120, 50, 20, 90);
	QRect rect6(140, 80, 20, 60);
	QRect rect7(160, 60, 20, 80);

	painter.drawRect(rect0);
	painter.drawRect(rect3);
	painter.drawRect(rect4);
	painter.drawRect(rect5);
	painter.drawRect(rect6);
	painter.drawRect(rect7);

	painter.setBrush(graphWindow_->pointingColor_);
	painter.drawRect(rect1);
	painter.drawRect(rect2);

	histogramLineColorFrame->setPaletteBackgroundColor(histogramPen_.color());
	histogramLineColorFrame->erase();

	int style = histogramPen_.style();
    histogramLineStyleComboBox->setCurrentItem(style);
}


void GraphicConfigurationWindow::drawRasterHistogram()
{
	rasterHistogramFrame->erase();
	QPainter painter(rasterHistogramFrame);
	painter.setPen(rasterHistogramPen_);
	painter.setBrush(rasterHistogramBrush_);

	QRect rect0(20, 70, 20, 90);
	QRect rect1(40, 80, 20, 80);
	QRect rect2(60, 40, 20, 120);
	QRect rect3(80, 30, 20, 130);
	QRect rect4(100, 10, 20, 150);
	QRect rect5(120, 30, 20, 130);
	QRect rect6(140, 60, 20, 100);
	QRect rect7(160, 40, 20, 120);

	painter.drawRect(rect0);
	painter.drawRect(rect1);
	painter.drawRect(rect2);
	painter.drawRect(rect3);
	painter.drawRect(rect4);
	painter.drawRect(rect5);
	painter.drawRect(rect6);
	painter.drawRect(rect7);

	rasterHistogramBrushColorFrame->setPaletteBackgroundColor(rasterHistogramBrush_.color());
	rasterHistogramBrushColorFrame->erase();

	rasterHistogramLineColorFrame->setPaletteBackgroundColor(rasterHistogramPen_.color());
	rasterHistogramLineColorFrame->erase();

	int style = rasterHistogramPen_.style();
    rasterHistogramLineStyleComboBox->setCurrentItem(style);
}


void GraphicConfigurationWindow::drawDispersion()
{
	QPointArray pa(10);
	pa.setPoint(0, 100, 30);
	pa.setPoint(1, 150, 70);
	pa.setPoint(2, 180, 120);
	pa.setPoint(3, 38, 100);
	pa.setPoint(4, 94, 75);
	pa.setPoint(5, 127, 35);
	pa.setPoint(6, 39, 89);
	pa.setPoint(7, 104, 115);
	pa.setPoint(8, 79, 68);
	pa.setPoint(9, 57, 56);

	dispersionSymbol_.setPen(QPen(graphWindow_->defaultColor_));
	dispersionSymbol_.setBrush(QBrush(graphWindow_->defaultColor_));

	dispersionSymbolFrame->erase();
	QPainter painter(dispersionSymbolFrame);

	unsigned int i;
	for(i=0; i<pa.count()-3; ++i)
	{
		QPoint p = pa.point(i);
		dispersionSymbol_.draw(&painter, p);
	}

	dispersionSymbol_.setPen(QPen(graphWindow_->pointingColor_));
	dispersionSymbol_.setBrush(QBrush(graphWindow_->pointingColor_));
	for(i=pa.count()-3; i<pa.count(); ++i)
	{
		QPoint p = pa.point(i);
		dispersionSymbol_.draw(&painter, p);
	}
}


void GraphicConfigurationWindow::drawNormalProbability()
{
	normalFrame->erase();
	QPainter painter(normalFrame);
	painter.setPen(normalCurvePen_);
	painter.setBrush(Qt::NoBrush);

	painter.moveTo(1, 78);
	painter.lineTo(158, 1);

	normalCurveColorFrame->setPaletteBackgroundColor(normalCurvePen_.color());
	normalCurveColorFrame->erase();

	int style = normalCurvePen_.style();
    normalLineStyleComboBox->setCurrentItem(style);

	normalProbFrame->erase();
	painter.end();
	painter.begin(normalProbFrame);
	painter.setPen(normalProbCurvePen_);
	painter.setBrush(Qt::NoBrush);

	painter.moveTo(1, 79);
	painter.lineTo(10, 73);
	painter.lineTo(20, 66);
	painter.lineTo(30, 61);
	painter.lineTo(40, 55);
	painter.lineTo(50, 50);
	painter.lineTo(60, 44);
	painter.lineTo(70, 41);
	painter.lineTo(80, 36);
	painter.lineTo(90, 32);
	painter.lineTo(100, 31);
	painter.lineTo(110, 27);
	painter.lineTo(120, 27);
	painter.lineTo(130, 21);
	painter.lineTo(140, 13);
	painter.lineTo(150, 10);
	painter.lineTo(158, 3);

	normalProbCurveFame->setPaletteBackgroundColor(normalProbCurvePen_.color());
	normalProbCurveFame->erase();

	style = normalProbCurvePen_.style();
    normalProbLineStyleComboBox->setCurrentItem(style);
}


void GraphicConfigurationWindow::drawGrid()
{
	int i;

	minorLineColorFrame->setPaletteBackgroundColor(minorLinePen_.color());
	minorLineColorFrame->erase();

	majorLineColorFrame->setPaletteBackgroundColor(majorLinePen_.color());
	majorLineColorFrame->erase();

	backgroundColorFrame->setPaletteBackgroundColor(backgroundColor_);
	backgroundColorFrame->erase();

//	gridFrame->setPaletteBackgroundColor(backgroundColor_); // delay problem...
//	gridFrame->erase(); // delay problem...
	QPainter painter(gridFrame);
	painter.fillRect(gridFrame->rect(), QBrush(backgroundColor_));
	painter.setBrush(Qt::NoBrush);

	if(horizontalMajorVisible_)
	{
		if(horizontalMinorVisible_)
		{
			painter.setPen(minorLinePen_);
			for(i=30; i<170;i+=20)
			{
				painter.moveTo(10, i);
				painter.lineTo(250, i);
			}
		}

		painter.setPen(majorLinePen_);
		for(i=10; i<180;i+=60)
		{
			painter.moveTo(10, i);
			painter.lineTo(250, i);
		}
	}

	if(verticalMajorVisible_)
	{
		if(verticalMinorVisible_)
		{
			painter.setPen(minorLinePen_);
			for(i=30; i<250;i+=20)
			{
				painter.moveTo(i, 10);
				painter.lineTo(i, 170);
			}
		}

		painter.setPen(majorLinePen_);
		for(i=10; i<260;i+=60)
		{
			painter.moveTo(i, 10);
			painter.lineTo(i, 170);
		}
	}
}


void GraphicConfigurationWindow::drawFonts()
{
	QPainter painter;
	painter.begin(horizontalFontFrame);
	painter.fillRect(horizontalFontFrame->rect(), horizontalFontFrame->backgroundColor());
	painter.setFont(horizontalAxisFont_);
	QPen pen(Qt::black);
	painter.setPen(pen);

	QString s = "123456789";
	
	QFontMetrics fm(horizontalAxisFont_);
	QRect rec = fm.boundingRect(s);

	int a = (horizontalFontFrame->height() - rec.height()) / 2;
	int y = a + rec.height();
	int x = (horizontalFontFrame->width() - rec.width()) / 2;

	painter.drawText(x, y, s);
	painter.end();

	painter.begin(verticalFontFrame);
	painter.fillRect(verticalFontFrame->rect(), verticalFontFrame->backgroundColor());
	painter.setFont(verticalAxisFont_);

	QFontMetrics fm1(verticalAxisFont_);
	QRect rec1 = fm.boundingRect(s);

	a = (verticalFontFrame->height() - rec1.height()) / 2;
	y = a + rec1.height();
	x = (verticalFontFrame->width() - rec1.width()) / 2;

	painter.drawText(x, y, s);
}


void GraphicConfigurationWindow::paint()
{
	QPaintEvent e(rect());
	paintEvent(&e);
}



void GraphicConfigurationWindow::accept()
{
    graphWindow_->horizontalMajorVisible_ = horizontalMajorVisible_;
    graphWindow_->horizontalMinorVisible_ = horizontalMinorVisible_;
    graphWindow_->verticalMajorVisible_ = verticalMajorVisible_;
    graphWindow_->normalProbCurvePen_ = normalProbCurvePen_;
    graphWindow_->normalCurvePen_ = normalCurvePen_;
    graphWindow_->rasterHistogramBrush_ = rasterHistogramBrush_;
    graphWindow_->rasterHistogramPen_ = rasterHistogramPen_;
    graphWindow_->histogramPen_ = histogramPen_;
    graphWindow_->dispersionSymbol_ = dispersionSymbol_;
    graphWindow_->verticalMinorVisible_ = verticalMinorVisible_;
    graphWindow_->majorLinePen_ = majorLinePen_;
    graphWindow_->minorLinePen_ = minorLinePen_;
    graphWindow_->backgroundColor_ = backgroundColor_;
    graphWindow_->horizontalAxisFont_ = horizontalAxisFont_;
    graphWindow_->verticalAxisFont_ = verticalAxisFont_;
	graphWindow_->skaterPen_ = skaterCurvePen_;
	graphWindow_->semivarCurvePen_ = semivarCurvePen_;
	graphWindow_->semivarAdjustCurvePen_ = semivarAdjustCurvePen_;

	QDialog::accept();
}


void GraphicConfigurationWindow::reject()
{
	QDialog::reject();
}


void GraphicConfigurationWindow::drawSkater()
{
	skaterFrame->erase();
	QPainter painter(skaterFrame);
	painter.setPen(skaterCurvePen_);
	painter.setBrush(Qt::NoBrush);

	painter.moveTo(1, 78);
	painter.lineTo(158, 1);

	skaterCurveColorFrame->setPaletteBackgroundColor(skaterCurvePen_.color());
	skaterCurveColorFrame->erase();

	int style = skaterCurvePen_.style();
    skaterLineStyleComboBox->setCurrentItem(style);
}


void GraphicConfigurationWindow::skaterLineStyleComboBox_activated( const QString & )
{
	Qt::PenStyle style = (Qt::PenStyle)skaterLineStyleComboBox->currentItem();
	skaterCurvePen_.setStyle(style);
	paint();
}


void GraphicConfigurationWindow::skaterLineColorPushButton_clicked()
{
	bool isOK = false;

	QColor inputColor = skaterCurvePen_.color();
	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		skaterCurvePen_.setColor(outputColor);
		paint();
	}
}


void GraphicConfigurationWindow::semivarLineStyleComboBox_activated( const QString & )
{
	Qt::PenStyle style = (Qt::PenStyle)semivarLineStyleComboBox->currentItem();
	semivarCurvePen_.setStyle(style);
	paint();
}


void GraphicConfigurationWindow::adjustLineColorPushButton_clicked()
{
	bool isOK = false;

	QColor inputColor = semivarAdjustCurvePen_.color();
	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		semivarAdjustCurvePen_.setColor(outputColor);
		paint();
	}
}


void GraphicConfigurationWindow::adjustLineStyleComboBox_activated( const QString & )
{
	Qt::PenStyle style = (Qt::PenStyle)adjustLineStyleComboBox->currentItem();
	semivarAdjustCurvePen_.setStyle(style);
	paint();
}


void GraphicConfigurationWindow::drawSemivar()
{
	semivarFrame->erase();
	QPainter painter(semivarFrame);
	painter.setPen(semivarCurvePen_);
	painter.setBrush(Qt::NoBrush);

	painter.moveTo(1, 78);
	painter.lineTo(158, 1);

	semivarCurveColorFrame->setPaletteBackgroundColor(semivarCurvePen_.color());
	semivarCurveColorFrame->erase();

	int style = semivarCurvePen_.style();
    semivarLineStyleComboBox->setCurrentItem(style);

	semivarAdjustFrame->erase();
	painter.end();
	painter.begin(semivarAdjustFrame);
	painter.setPen(semivarAdjustCurvePen_);
	painter.setBrush(Qt::NoBrush);

	painter.moveTo(1, 79);
	painter.lineTo(10, 73);
	painter.lineTo(20, 66);
	painter.lineTo(30, 61);
	painter.lineTo(40, 55);
	painter.lineTo(50, 50);
	painter.lineTo(60, 44);
	painter.lineTo(70, 41);
	painter.lineTo(80, 36);
	painter.lineTo(90, 32);
	painter.lineTo(100, 31);
	painter.lineTo(110, 27);
	painter.lineTo(120, 27);
	painter.lineTo(130, 21);
	painter.lineTo(140, 13);
	painter.lineTo(150, 10);
	painter.lineTo(158, 3);

	semivarAdjustCurveColorFame->setPaletteBackgroundColor(semivarAdjustCurvePen_.color());
	semivarAdjustCurveColorFame->erase();

	style = semivarAdjustCurvePen_.style();
    adjustLineStyleComboBox->setCurrentItem(style);
}

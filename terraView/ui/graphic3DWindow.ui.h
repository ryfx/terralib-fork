
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

#include <qmessagebox.h>
#include <qapplication.h>
#include <TeAppTheme.h>
#include <terraViewBase.h>
#include <TeQtGrid.h>
#include <TeWaitCursor.h>
#include <TeDatabase.h>
#include <TeQtGLWidget.h>
#include <TeExternalTheme.h>
#include <help.h>

void Graphic3DWindow::init()
{
	help_ = 0;

	mainWindow_= (TerraViewBase*)qApp->mainWidget();
	TeQtGrid* grid = mainWindow_->getGrid();
	TeAppTheme* appTheme = grid->getTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();
	TeAttributeList attList;
	if(theme->getProductId() == TeTHEME)
		attList = theme->sqlAttList();
	else if(theme->getProductId() == TeEXTERNALTHEME)
		attList = ((TeExternalTheme*)theme)->getRemoteTheme()->sqlAttList();

	int n, nc = grid->numCols();
	vector<int>	cols;

	for(n = 0; n < nc; ++n)
	{
		if (grid->isColumnSelected(n, true) == true)
		{
			int col = grid->getColumn(n);
			if(attList[col].rep_.type_ == TeREAL ||
				attList[col].rep_.type_ == TeINT)
				cols.push_back(col);
		}
	}

	int x = cols[0];
	int y = cols[1];
	int z = cols[2];
	string xField = attList[x].rep_.name_;
	string yField = attList[y].rep_.name_;
	string zField = attList[z].rep_.name_;
	xLineEdit->setText(xField.c_str());
	yLineEdit->setText(yField.c_str());
	zLineEdit->setText(zField.c_str());
	
	functionComboBox->insertItem("NONE");
	functionComboBox->insertItem("MIN");
	functionComboBox->insertItem("MAX");
	functionComboBox->insertItem("AVG");
	functionComboBox->insertItem("SUM");
	functionComboBox->insertItem("COUNT");
	functionComboBox->setCurrentItem(0);
	string function = functionComboBox->currentText().latin1();

	inputComboBox->insertItem(tr("All"));
	inputComboBox->insertItem(tr("Queried"));
	inputComboBox->insertItem(tr("Not Queried"));
	inputComboBox->insertItem(tr("Grouped"));
	inputComboBox->insertItem(tr("Not Grouped"));
	inputComboBox->setCurrentItem(0);

    // Create OpenGL widget
    glWidget_ = new TeQtGLWidget( frameGroupBox, "glbox", grid, xField, yField, zField, xyTeQtFrame, zxTeQtFrame, yzTeQtFrame, TeAll, function);

	// Rotation
    QObject::connect( xRotationSlider, SIGNAL(valueChanged(int)), glWidget_, SLOT(setXRotation(int)) );
    QObject::connect( yRotationSlider, SIGNAL(valueChanged(int)), glWidget_, SLOT(setYRotation(int)) );
    QObject::connect( zRotationSlider, SIGNAL(valueChanged(int)), glWidget_, SLOT(setZRotation(int)) );

	QObject::connect( glWidget_, SIGNAL(setXRotationSignal(int)), this, SLOT(setXRotationSlot(int)) );
    QObject::connect( glWidget_, SIGNAL(setYRotationSignal(int)), this, SLOT(setYRotationSlot(int)) );
    QObject::connect( glWidget_, SIGNAL(setZRotationSignal(int)), this, SLOT(setZRotationSlot(int)) );

	// Lighting
    QObject::connect( lightingButtonGroup, SIGNAL(clicked(int)), glWidget_, SLOT(setLightingMode(int)) );

	// Zoom
    QObject::connect( zoomSlider, SIGNAL(valueChanged(int)), glWidget_, SLOT(setViewAngle(int)) );

	QObject::connect( glWidget_, SIGNAL(setZoomSignal(int)), this, SLOT(setZoomSlot(int)) );

	// Offset
	QObject::connect( xOffsetSlider, SIGNAL(valueChanged(int)), glWidget_, SLOT(setXOffset(int)) );
    QObject::connect( yOffsetSlider, SIGNAL(valueChanged(int)), glWidget_, SLOT(setYOffset(int)) );
    QObject::connect( zOffsetSlider, SIGNAL(valueChanged(int)), glWidget_, SLOT(setZOffset(int)) );

	QObject::connect( xOffsetSlider, SIGNAL(sliderReleased()), glWidget_, SLOT(xOffsetReleased()) );
    QObject::connect( yOffsetSlider, SIGNAL(sliderReleased()), glWidget_, SLOT(yOffsetReleased()) );
    QObject::connect( zOffsetSlider, SIGNAL(sliderReleased()), glWidget_, SLOT(zOffsetReleased()) );

	QObject::connect( glWidget_, SIGNAL(setXOffsetSignal(int)), this, SLOT(setXOffsetSlot(int)) );
    QObject::connect( glWidget_, SIGNAL(setYOffsetSignal(int)), this, SLOT(setYOffsetSlot(int)) );
    QObject::connect( glWidget_, SIGNAL(setZOffsetSignal(int)), this, SLOT(setZOffsetSlot(int)) );

	// Size
	QObject::connect( xSizeSlider, SIGNAL(valueChanged(int)), glWidget_, SLOT(setXSize(int)) );
    QObject::connect( ySizeSlider, SIGNAL(valueChanged(int)), glWidget_, SLOT(setYSize(int)) );
    QObject::connect( zSizeSlider, SIGNAL(valueChanged(int)), glWidget_, SLOT(setZSize(int)) );

	QObject::connect( xSizeSlider, SIGNAL(sliderReleased()), glWidget_, SLOT(xSizeReleased()) );
    QObject::connect( ySizeSlider, SIGNAL(sliderReleased()), glWidget_, SLOT(ySizeReleased()) );
    QObject::connect( zSizeSlider, SIGNAL(sliderReleased()), glWidget_, SLOT(zSizeReleased()) );

	QObject::connect( glWidget_, SIGNAL(setXSizeSignal(int)), this, SLOT(setXSizeSlot(int)) );
    QObject::connect( glWidget_, SIGNAL(setYSizeSignal(int)), this, SLOT(setYSizeSlot(int)) );
    QObject::connect( glWidget_, SIGNAL(setZSizeSignal(int)), this, SLOT(setZSizeSlot(int)) );

	// Paint 2D 
	QObject::connect( xyTeQtFrame, SIGNAL(paint2DSignal()), this, SLOT(paint2DSlot()) );
	QObject::connect( zxTeQtFrame, SIGNAL(paint2DSignal()), this, SLOT(paint2DSlot()) );
	QObject::connect( yzTeQtFrame, SIGNAL(paint2DSignal()), this, SLOT(paint2DSlot()) );

	// mouse 2D 
	QObject::connect( xyTeQtFrame, SIGNAL(setCursorSignal(const QPoint&)), this, SLOT(xySetCursorSlot(const QPoint&)) );
	QObject::connect( zxTeQtFrame, SIGNAL(setCursorSignal(const QPoint&)), this, SLOT(zxSetCursorSlot(const QPoint&)) );
	QObject::connect( yzTeQtFrame, SIGNAL(setCursorSignal(const QPoint&)), this, SLOT(yzSetCursorSlot(const QPoint&)) );

	QObject::connect( xyTeQtFrame, SIGNAL(mouseMoveSignal(QMouseEvent*)), this, SLOT(xyMouseMoveSlot(QMouseEvent*)) );
	QObject::connect( zxTeQtFrame, SIGNAL(mouseMoveSignal(QMouseEvent*)), this, SLOT(zxMouseMoveSlot(QMouseEvent*)) );
	QObject::connect( yzTeQtFrame, SIGNAL(mouseMoveSignal(QMouseEvent*)), this, SLOT(yzMouseMoveSlot(QMouseEvent*)) );

	QObject::connect( xyTeQtFrame, SIGNAL(mouseReleaseSignal(QMouseEvent*)), this, SLOT(xyMouseReleaseSlot(QMouseEvent*)) );
	QObject::connect( zxTeQtFrame, SIGNAL(mouseReleaseSignal(QMouseEvent*)), this, SLOT(zxMouseReleaseSlot(QMouseEvent*)) );
	QObject::connect( yzTeQtFrame, SIGNAL(mouseReleaseSignal(QMouseEvent*)), this, SLOT(yzMouseReleaseSlot(QMouseEvent*)) );

	QObject::connect( mainWindow_, SIGNAL(queryChangedSignal()), this, SLOT(queryChangedSlot()) );
	QObject::connect( mainWindow_, SIGNAL(groupChangedSignal()), this, SLOT(groupChangedSlot()) );

	xRotationSlider->setValue((int)(glWidget_->xRot_ * 99 / 360));
	yRotationSlider->setValue((int)(glWidget_->yRot_ * 99 / 360));
	zRotationSlider->setValue((int)(glWidget_->zRot_ * 99 / 360));

	int angle = 120 - (int)glWidget_->viewAngle_;
	zoomSlider->setValue(angle);

	qualitySlider->setValue(20);

	xOffsetSlider->setValue((int)((glWidget_->xOff_ + 1) * 99 / 2));
	yOffsetSlider->setValue((int)((glWidget_->yOff_ + 1) * 99 / 2));
	zOffsetSlider->setValue((int)((glWidget_->zOff_ + 1) * 99 / 2));

	xSizeSlider->setValue((int)(glWidget_->xSize_ * 99 / 2));
	ySizeSlider->setValue((int)(glWidget_->ySize_ * 99 / 2));
	zSizeSlider->setValue((int)(glWidget_->zSize_ * 99 / 2));
}



void Graphic3DWindow::resizeEvent( QResizeEvent * )
{
	int w = frameGroupBox->width();
	int h = frameGroupBox->height();
	glWidget_->setMinimumWidth(w);
	glWidget_->setMaximumWidth(w);
	glWidget_->setMinimumHeight(h);
	glWidget_->setMaximumHeight(h);
}


void Graphic3DWindow::setXOffsetSlot( int v)
{
	xOffsetSlider->setValue(v);
}


void Graphic3DWindow::setYOffsetSlot( int v)
{
	yOffsetSlider->setValue(v);
}


void Graphic3DWindow::setZOffsetSlot( int v)
{
	zOffsetSlider->setValue(v);
}


void Graphic3DWindow::setXSizeSlot( int v)
{
	xSizeSlider->setValue(v);
}


void Graphic3DWindow::setYSizeSlot( int v)
{
	ySizeSlider->setValue(v);
}


void Graphic3DWindow::setZSizeSlot( int v)
{
	zSizeSlider->setValue(v);
}

void Graphic3DWindow::setXRotationSlot( int r)
{
	xRotationSlider->setValue(r * 99 / 360);
}

void Graphic3DWindow::setYRotationSlot( int r)
{
	yRotationSlider->setValue(r * 99 / 360);
}

void Graphic3DWindow::setZRotationSlot( int r)
{
	zRotationSlider->setValue(r * 99 / 360);
}

void Graphic3DWindow::setZoomSlot( int z)
{
	zoomSlider->setValue(z);
}

void Graphic3DWindow::showAxisCheckBox_toggled( bool b)
{
	glWidget_->drawAxis(b);
}

void Graphic3DWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("graphic3DWindow.htm");
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

void Graphic3DWindow::paint2DSlot()
{
	glWidget_->paint2D();
}

void Graphic3DWindow::xyMouseMoveSlot( QMouseEvent *e )
{
	if(!(e->state() == Qt::LeftButton || e->state() == (Qt::LeftButton | Qt::ShiftButton) || e->state() == (Qt::LeftButton | Qt::ControlButton)))
		return;

	int	val;
	GLfloat w = (GLfloat)xyTeQtFrame->width();
	QPoint pp = xyTeQtFrame->p_;
	QPoint p = e->pos();
	xyTeQtFrame->p_ = p;
	GLfloat dx = (GLfloat)(p.x() - pp.x());
	dx = dx * 2. / w;
	GLfloat dy = (GLfloat)(pp.y() - p.y());
	dy = dy * 2. / w;

	if(dragMode_ == cMode) // move
	{
		if(dx != 0.)
		{
			glWidget_->xOff_ += dx;
			if(glWidget_->xOff_ < -1.)
				glWidget_->xOff_ = -1.;
			if((glWidget_->xSize_ + glWidget_->xOff_) > 1.)
				glWidget_->xOff_ = 1. - glWidget_->xSize_;

			val = TeRound((glWidget_->xOff_ + 1.) * 99. / 2.);
			setXOffsetSlot(val);
		}
		if(dy != 0.)
		{
			glWidget_->yOff_ += dy;
			if(glWidget_->yOff_ < -1.)
				glWidget_->yOff_ = -1.;
			if((glWidget_->ySize_ + glWidget_->yOff_) > 1.)
				glWidget_->yOff_ = 1. - glWidget_->ySize_;

			val = TeRound((glWidget_->yOff_ + 1.) * 99. / 2.);
			setYOffsetSlot(val);
		}
	}
	else if(dragMode_ == x1y1Mode)
	{
		if(dx != 0.)
		{
			GLfloat xf =  glWidget_->xOff_ + glWidget_->xSize_;
			glWidget_->xOff_ += dx;
			if(glWidget_->xOff_ < -1.)
				glWidget_->xOff_ = -1.;
			if(glWidget_->xOff_ >= xf-.05)
				glWidget_->xOff_ = xf-.05;
			glWidget_->xSize_ = xf - glWidget_->xOff_;

			val = TeRound((glWidget_->xOff_ + 1.) * 99. / 2.);
			setXOffsetSlot(val);
		}
		if(dy != 0.)
		{
			GLfloat yf =  glWidget_->yOff_ + glWidget_->ySize_;
			glWidget_->yOff_ += dy;
			if(glWidget_->yOff_ < -1.)
				glWidget_->yOff_ = -1.;
			if(glWidget_->yOff_ >= yf-.05)
				glWidget_->yOff_ = yf-.05;
			glWidget_->ySize_ = yf - glWidget_->yOff_;

			val = TeRound((glWidget_->yOff_ + 1.) * 99. / 2.);
			setYOffsetSlot(val);
		}
	}
	else if(dragMode_ == x1y2Mode)
	{
		if(dx != 0.)
		{
			GLfloat xf =  glWidget_->xOff_ + glWidget_->xSize_;
			glWidget_->xOff_ += dx;
			if(glWidget_->xOff_ < -1.)
				glWidget_->xOff_ = -1.;
			if(glWidget_->xOff_ >= xf-.05)
				glWidget_->xOff_ = xf-.05;
			glWidget_->xSize_ = xf - glWidget_->xOff_;

			val = TeRound((glWidget_->xOff_ + 1.) * 99. / 2.);
			setXOffsetSlot(val);
		}
		if(dy != 0.)
		{
			glWidget_->ySize_ += dy;
			if(glWidget_->ySize_ < .05)
				glWidget_->ySize_ = (GLfloat).05;
			if(glWidget_->yOff_ + glWidget_->ySize_ > 1.)
				glWidget_->ySize_ = (GLfloat)1. - glWidget_->yOff_;

			val = TeRound((glWidget_->ySize_) * 99. / 2.);
			setYSizeSlot(val);
		}
	}
	else if(dragMode_ == x2y1Mode)
	{
		if(dx != 0.)
		{
			glWidget_->xSize_ += dx;
			if(glWidget_->xSize_ < .05)
				glWidget_->xSize_ = (GLfloat).05;
			if(glWidget_->xOff_ + glWidget_->xSize_ > 1.)
				glWidget_->xSize_ = (GLfloat)1. - glWidget_->xOff_;

			val = TeRound((glWidget_->xSize_) * 99. / 2.);
			setXSizeSlot(val);
		}
		if(dy != 0.)
		{
			GLfloat yf =  glWidget_->yOff_ + glWidget_->ySize_;
			glWidget_->yOff_ += dy;
			if(glWidget_->yOff_ < -1.)
				glWidget_->yOff_ = -1.;
			if(glWidget_->yOff_ >= yf-.05)
				glWidget_->yOff_ = yf-.05;
			glWidget_->ySize_ = yf - glWidget_->yOff_;

			val = TeRound((glWidget_->yOff_ + 1.) * 99. / 2.);
			setYOffsetSlot(val);
		}
	}
	else if(dragMode_ == x2y2Mode)
	{
		if(dx != 0.)
		{
			glWidget_->xSize_ += dx;
			if(glWidget_->xSize_ < .05)
				glWidget_->xSize_ = (GLfloat).05;
			if(glWidget_->xOff_ + glWidget_->xSize_ > 1.)
				glWidget_->xSize_ = (GLfloat)1. - glWidget_->xOff_;

			val = TeRound((glWidget_->xSize_) * 99. / 2.);
			setXSizeSlot(val);
		}
		if(dy != 0.)
		{
			glWidget_->ySize_ += dy;
			if(glWidget_->ySize_ < .05)
				glWidget_->ySize_ = (GLfloat).05;
			if(glWidget_->yOff_ + glWidget_->ySize_ > 1.)
				glWidget_->ySize_ = (GLfloat)1. - glWidget_->yOff_;

			val = TeRound((glWidget_->ySize_) * 99. / 2.);
			setYSizeSlot(val);
		}
	}
	else if(dragMode_ == x1Mode)
	{
		if(dx != 0.)
		{
			GLfloat xf =  glWidget_->xOff_ + glWidget_->xSize_;
			glWidget_->xOff_ += dx;
			if(glWidget_->xOff_ < -1.)
				glWidget_->xOff_ = -1.;
			if(glWidget_->xOff_ >= xf-.05)
				glWidget_->xOff_ = xf-.05;
			glWidget_->xSize_ = xf - glWidget_->xOff_;

			val = TeRound((glWidget_->xOff_ + 1.) * 99. / 2.);
			setXOffsetSlot(val);
		}
	}
	else if(dragMode_ == y1Mode)
	{
		if(dy != 0.)
		{
			GLfloat yf =  glWidget_->yOff_ + glWidget_->ySize_;
			glWidget_->yOff_ += dy;
			if(glWidget_->yOff_ < -1.)
				glWidget_->yOff_ = -1.;
			if(glWidget_->yOff_ >= yf-.05)
				glWidget_->yOff_ = yf-.05;
			glWidget_->ySize_ = yf - glWidget_->yOff_;

			val = TeRound((glWidget_->yOff_ + 1.) * 99. / 2.);
			setYOffsetSlot(val);
		}
	}
	else if(dragMode_ == x2Mode)
	{
		if(dx != 0.)
		{
			glWidget_->xSize_ += dx;
			if(glWidget_->xSize_ < .05)
				glWidget_->xSize_ = (GLfloat).05;
			if(glWidget_->xOff_ + glWidget_->xSize_ > 1.)
				glWidget_->xSize_ = (GLfloat)1. - glWidget_->xOff_;

			val = TeRound((glWidget_->xSize_) * 99. / 2.);
			setXSizeSlot(val);
		}
	}
	else if(dragMode_ == y2Mode)
	{
		if(dy != 0.)
		{
			glWidget_->ySize_ += dy;
			if(glWidget_->ySize_ < .05)
				glWidget_->ySize_ = (GLfloat).05;
			if(glWidget_->yOff_ + glWidget_->ySize_ > 1.)
				glWidget_->ySize_ = (GLfloat)1. - glWidget_->yOff_;

			val = TeRound((glWidget_->ySize_) * 99. / 2.);
			setYSizeSlot(val);
		}
	}

	if(xyTeQtFrame->cursor().shape() != Qt::ArrowCursor)
	{
		glWidget_->paint3D();
		glWidget_->paint2D();
	}
}


void Graphic3DWindow::zxMouseMoveSlot( QMouseEvent *e )
{
	if(!(e->state() == Qt::LeftButton || e->state() == (Qt::LeftButton | Qt::ShiftButton) || e->state() == (Qt::LeftButton | Qt::ControlButton)))
		return;

	int	val;
	GLfloat w = (GLfloat)zxTeQtFrame->width();
	QPoint pp = zxTeQtFrame->p_;
	QPoint p = e->pos();
	zxTeQtFrame->p_ = p;
	GLfloat dx = (GLfloat)(p.x() - pp.x());
	dx = dx * 2. / w;
	GLfloat dy = (GLfloat)(pp.y() - p.y());
	dy = dy * 2. / w;

	if(dragMode_ == cMode) // move
	{
		if(dx != 0.)
		{
			glWidget_->zOff_ += dx;
			if(glWidget_->zOff_ < -1.)
				glWidget_->zOff_ = -1.;
			if((glWidget_->zSize_ + glWidget_->zOff_) > 1.)
				glWidget_->zOff_ = 1. - glWidget_->zSize_;

			val = TeRound((glWidget_->zOff_ + 1.) * 99. / 2.);
			setZOffsetSlot(val);
		}
		if(dy != 0.)
		{
			glWidget_->xOff_ += dy;
			if(glWidget_->xOff_ < -1.)
				glWidget_->xOff_ = -1.;
			if((glWidget_->xSize_ + glWidget_->xOff_) > 1.)
				glWidget_->xOff_ = 1. - glWidget_->xSize_;

			val = TeRound((glWidget_->xOff_ + 1.) * 99. / 2.);
			setXOffsetSlot(val);
		}
	}
	else if(dragMode_ == x1y1Mode)
	{
		if(dx != 0.)
		{
			GLfloat xf =  glWidget_->zOff_ + glWidget_->zSize_;
			glWidget_->zOff_ += dx;
			if(glWidget_->zOff_ < -1.)
				glWidget_->zOff_ = -1.;
			if(glWidget_->zOff_ >= xf-.05)
				glWidget_->zOff_ = xf-.05;
			glWidget_->zSize_ = xf - glWidget_->zOff_;

			val = TeRound((glWidget_->zOff_ + 1.) * 99. / 2.);
			setZOffsetSlot(val);
		}
		if(dy != 0.)
		{
			GLfloat yf =  glWidget_->xOff_ + glWidget_->xSize_;
			glWidget_->xOff_ += dy;
			if(glWidget_->xOff_ < -1.)
				glWidget_->xOff_ = -1.;
			if(glWidget_->xOff_ >= yf-.05)
				glWidget_->xOff_ = yf-.05;
			glWidget_->xSize_ = yf - glWidget_->xOff_;

			val = TeRound((glWidget_->xOff_ + 1.) * 99. / 2.);
			setXOffsetSlot(val);
		}
	}
	else if(dragMode_ == x1y2Mode)
	{
		if(dx != 0.)
		{
			GLfloat xf =  glWidget_->zOff_ + glWidget_->zSize_;
			glWidget_->zOff_ += dx;
			if(glWidget_->zOff_ < -1.)
				glWidget_->zOff_ = -1.;
			if(glWidget_->zOff_ >= xf-.05)
				glWidget_->zOff_ = xf-.05;
			glWidget_->zSize_ = xf - glWidget_->zOff_;

			val = TeRound((glWidget_->zOff_ + 1.) * 99. / 2.);
			setZOffsetSlot(val);
		}
		if(dy != 0.)
		{
			glWidget_->xSize_ += dy;
			if(glWidget_->xSize_ < .05)
				glWidget_->xSize_ = (GLfloat).05;
			if(glWidget_->xOff_ + glWidget_->xSize_ > 1.)
				glWidget_->xSize_ = (GLfloat)1. - glWidget_->xOff_;

			val = TeRound((glWidget_->xSize_) * 99. / 2.);
			setXSizeSlot(val);
		}
	}
	else if(dragMode_ == x2y1Mode)
	{
		if(dx != 0.)
		{
			glWidget_->zSize_ += dx;
			if(glWidget_->zSize_ < .05)
				glWidget_->zSize_ = (GLfloat).05;
			if(glWidget_->zOff_ + glWidget_->zSize_ > 1.)
				glWidget_->zSize_ = (GLfloat)1. - glWidget_->zOff_;

			val = TeRound((glWidget_->zSize_) * 99. / 2.);
			setZSizeSlot(val);
		}
		if(dy != 0.)
		{
			GLfloat yf =  glWidget_->xOff_ + glWidget_->xSize_;
			glWidget_->xOff_ += dy;
			if(glWidget_->xOff_ < -1.)
				glWidget_->xOff_ = -1.;
			if(glWidget_->xOff_ >= yf-.05)
				glWidget_->xOff_ = yf-.05;
			glWidget_->xSize_ = yf - glWidget_->xOff_;

			val = TeRound((glWidget_->xOff_ + 1.) * 99. / 2.);
			setXOffsetSlot(val);
		}
	}
	else if(dragMode_ == x2y2Mode)
	{
		if(dx != 0.)
		{
			glWidget_->zSize_ += dx;
			if(glWidget_->zSize_ < .05)
				glWidget_->zSize_ = (GLfloat).05;
			if(glWidget_->zOff_ + glWidget_->zSize_ > 1.)
				glWidget_->zSize_ = (GLfloat)1. - glWidget_->zOff_;

			val = TeRound((glWidget_->zSize_) * 99. / 2.);
			setZSizeSlot(val);
		}
		if(dy != 0.)
		{
			glWidget_->xSize_ += dy;
			if(glWidget_->xSize_ < .05)
				glWidget_->xSize_ = (GLfloat).05;
			if(glWidget_->xOff_ + glWidget_->xSize_ > 1.)
				glWidget_->xSize_ = (GLfloat)1. - glWidget_->xOff_;

			val = TeRound((glWidget_->xSize_) * 99. / 2.);
			setXSizeSlot(val);
		}
	}
	else if(dragMode_ == x1Mode)
	{
		if(dx != 0.)
		{
			GLfloat xf =  glWidget_->zOff_ + glWidget_->zSize_;
			glWidget_->zOff_ += dx;
			if(glWidget_->zOff_ < -1.)
				glWidget_->zOff_ = -1.;
			if(glWidget_->zOff_ >= xf-.05)
				glWidget_->zOff_ = xf-.05;
			glWidget_->zSize_ = xf - glWidget_->zOff_;

			val = TeRound((glWidget_->zOff_ + 1.) * 99. / 2.);
			setZOffsetSlot(val);
		}
	}
	else if(dragMode_ == y1Mode)
	{
		if(dy != 0.)
		{
			GLfloat yf =  glWidget_->xOff_ + glWidget_->xSize_;
			glWidget_->xOff_ += dy;
			if(glWidget_->xOff_ < -1.)
				glWidget_->xOff_ = -1.;
			if(glWidget_->xOff_ >= yf-.05)
				glWidget_->xOff_ = yf-.05;
			glWidget_->xSize_ = yf - glWidget_->xOff_;

			val = TeRound((glWidget_->xOff_ + 1.) * 99. / 2.);
			setXOffsetSlot(val);
		}
	}
	else if(dragMode_ == x2Mode)
	{
		if(dx != 0.)
		{
			glWidget_->zSize_ += dx;
			if(glWidget_->zSize_ < .05)
				glWidget_->zSize_ = (GLfloat).05;
			if(glWidget_->zOff_ + glWidget_->zSize_ > 1.)
				glWidget_->zSize_ = (GLfloat)1. - glWidget_->zOff_;

			val = TeRound((glWidget_->zSize_) * 99. / 2.);
			setZSizeSlot(val);
		}
	}
	else if(dragMode_ == y2Mode)
	{
		if(dy != 0.)
		{
			glWidget_->xSize_ += dy;
			if(glWidget_->xSize_ < .05)
				glWidget_->xSize_ = (GLfloat).05;
			if(glWidget_->xOff_ + glWidget_->xSize_ > 1.)
				glWidget_->xSize_ = (GLfloat)1. - glWidget_->xOff_;

			val = TeRound((glWidget_->xSize_) * 99. / 2.);
			setXSizeSlot(val);
		}
	}

	if(zxTeQtFrame->cursor().shape() != Qt::ArrowCursor)
	{
		glWidget_->paint3D();
		glWidget_->paint2D();
	}
}


void Graphic3DWindow::yzMouseMoveSlot( QMouseEvent *e )
{
	if(!(e->state() == Qt::LeftButton || e->state() == (Qt::LeftButton | Qt::ShiftButton) || e->state() == (Qt::LeftButton | Qt::ControlButton)))
		return;

	int	val;
	GLfloat w = (GLfloat)yzTeQtFrame->width();
	QPoint pp = yzTeQtFrame->p_;
	QPoint p = e->pos();
	yzTeQtFrame->p_ = p;
	GLfloat dx = (GLfloat)(p.x() - pp.x());
	dx = dx * 2. / w;
	GLfloat dy = (GLfloat)(pp.y() - p.y());
	dy = dy * 2. / w;

	if(dragMode_ == cMode) // move
	{
		if(dx != 0.)
		{
			glWidget_->yOff_ += dx;
			if(glWidget_->yOff_ < -1.)
				glWidget_->yOff_ = -1.;
			if((glWidget_->ySize_ + glWidget_->yOff_) > 1.)
				glWidget_->yOff_ = 1. - glWidget_->ySize_;

			val = TeRound((glWidget_->yOff_ + 1.) * 99. / 2.);
			setYOffsetSlot(val);
		}
		if(dy != 0.)
		{
			glWidget_->zOff_ += dy;
			if(glWidget_->zOff_ < -1.)
				glWidget_->zOff_ = -1.;
			if((glWidget_->zSize_ + glWidget_->zOff_) > 1.)
				glWidget_->zOff_ = 1. - glWidget_->zSize_;

			val = TeRound((glWidget_->zOff_ + 1.) * 99. / 2.);
			setZOffsetSlot(val);
		}
	}
	else if(dragMode_ == x1y1Mode)
	{
		if(dx != 0.)
		{
			GLfloat xf =  glWidget_->yOff_ + glWidget_->ySize_;
			glWidget_->yOff_ += dx;
			if(glWidget_->yOff_ < -1.)
				glWidget_->yOff_ = -1.;
			if(glWidget_->yOff_ >= xf-.05)
				glWidget_->yOff_ = xf-.05;
			glWidget_->ySize_ = xf - glWidget_->yOff_;

			val = TeRound((glWidget_->yOff_ + 1.) * 99. / 2.);
			setYOffsetSlot(val);
		}
		if(dy != 0.)
		{
			GLfloat yf =  glWidget_->zOff_ + glWidget_->zSize_;
			glWidget_->zOff_ += dy;
			if(glWidget_->zOff_ < -1.)
				glWidget_->zOff_ = -1.;
			if(glWidget_->zOff_ >= yf-.05)
				glWidget_->zOff_ = yf-.05;
			glWidget_->zSize_ = yf - glWidget_->zOff_;

			val = TeRound((glWidget_->zOff_ + 1.) * 99. / 2.);
			setZOffsetSlot(val);
		}
	}
	else if(dragMode_ == x1y2Mode)
	{
		if(dx != 0.)
		{
			GLfloat xf =  glWidget_->yOff_ + glWidget_->ySize_;
			glWidget_->yOff_ += dx;
			if(glWidget_->yOff_ < -1.)
				glWidget_->yOff_ = -1.;
			if(glWidget_->yOff_ >= xf-.05)
				glWidget_->yOff_ = xf-.05;
			glWidget_->ySize_ = xf - glWidget_->yOff_;

			val = TeRound((glWidget_->yOff_ + 1.) * 99. / 2.);
			setYOffsetSlot(val);
		}
		if(dy != 0.)
		{
			glWidget_->zSize_ += dy;
			if(glWidget_->zSize_ < .05)
				glWidget_->zSize_ = (GLfloat).05;
			if(glWidget_->zOff_ + glWidget_->zSize_ > 1.)
				glWidget_->zSize_ = (GLfloat)1. - glWidget_->zOff_;

			val = TeRound((glWidget_->zSize_) * 99. / 2.);
			setZSizeSlot(val);
		}
	}
	else if(dragMode_ == x2y1Mode)
	{
		if(dx != 0.)
		{
			glWidget_->ySize_ += dx;
			if(glWidget_->ySize_ < .05)
				glWidget_->ySize_ = (GLfloat).05;
			if(glWidget_->yOff_ + glWidget_->ySize_ > 1.)
				glWidget_->ySize_ = (GLfloat)1. - glWidget_->yOff_;

			val = TeRound((glWidget_->ySize_) * 99. / 2.);
			setYSizeSlot(val);
		}
		if(dy != 0.)
		{
			GLfloat yf =  glWidget_->zOff_ + glWidget_->zSize_;
			glWidget_->zOff_ += dy;
			if(glWidget_->zOff_ < -1.)
				glWidget_->zOff_ = -1.;
			if(glWidget_->zOff_ >= yf-.05)
				glWidget_->zOff_ = yf-.05;
			glWidget_->zSize_ = yf - glWidget_->zOff_;

			val = TeRound((glWidget_->zOff_ + 1.) * 99. / 2.);
			setZOffsetSlot(val);
		}
	}
	else if(dragMode_ == x2y2Mode)
	{
		if(dx != 0.)
		{
			glWidget_->ySize_ += dx;
			if(glWidget_->ySize_ < .05)
				glWidget_->ySize_ = (GLfloat).05;
			if(glWidget_->yOff_ + glWidget_->ySize_ > 1.)
				glWidget_->ySize_ = (GLfloat)1. - glWidget_->yOff_;

			val = TeRound((glWidget_->ySize_) * 99. / 2.);
			setYSizeSlot(val);
		}
		if(dy != 0.)
		{
			glWidget_->zSize_ += dy;
			if(glWidget_->zSize_ < .05)
				glWidget_->zSize_ = (GLfloat).05;
			if(glWidget_->zOff_ + glWidget_->zSize_ > 1.)
				glWidget_->zSize_ = (GLfloat)1. - glWidget_->zOff_;

			val = TeRound((glWidget_->zSize_) * 99. / 2.);
			setZSizeSlot(val);
		}
	}
	else if(dragMode_ == x1Mode)
	{
		if(dx != 0.)
		{
			GLfloat xf =  glWidget_->yOff_ + glWidget_->ySize_;
			glWidget_->yOff_ += dx;
			if(glWidget_->yOff_ < -1.)
				glWidget_->yOff_ = -1.;
			if(glWidget_->yOff_ >= xf-.05)
				glWidget_->yOff_ = xf-.05;
			glWidget_->ySize_ = xf - glWidget_->yOff_;

			val = TeRound((glWidget_->yOff_ + 1.) * 99. / 2.);
			setYOffsetSlot(val);
		}
	}
	else if(dragMode_ == y1Mode)
	{
		if(dy != 0.)
		{
			GLfloat yf =  glWidget_->zOff_ + glWidget_->zSize_;
			glWidget_->zOff_ += dy;
			if(glWidget_->zOff_ < -1.)
				glWidget_->zOff_ = -1.;
			if(glWidget_->zOff_ >= yf-.05)
				glWidget_->zOff_ = yf-.05;
			glWidget_->zSize_ = yf - glWidget_->zOff_;

			val = TeRound((glWidget_->zOff_ + 1.) * 99. / 2.);
			setZOffsetSlot(val);
		}
	}
	else if(dragMode_ == x2Mode)
	{
		if(dx != 0.)
		{
			glWidget_->ySize_ += dx;
			if(glWidget_->ySize_ < .05)
				glWidget_->ySize_ = (GLfloat).05;
			if(glWidget_->yOff_ + glWidget_->ySize_ > 1.)
				glWidget_->ySize_ = (GLfloat)1. - glWidget_->yOff_;

			val = TeRound((glWidget_->ySize_) * 99. / 2.);
			setYSizeSlot(val);
		}
	}
	else if(dragMode_ == y2Mode)
	{
		if(dy != 0.)
		{
			glWidget_->zSize_ += dy;
			if(glWidget_->zSize_ < .05)
				glWidget_->zSize_ = (GLfloat).05;
			if(glWidget_->zOff_ + glWidget_->zSize_ > 1.)
				glWidget_->zSize_ = (GLfloat)1. - glWidget_->zOff_;

			val = TeRound((glWidget_->zSize_) * 99. / 2.);
			setZSizeSlot(val);
		}
	}

	if(yzTeQtFrame->cursor().shape() != Qt::ArrowCursor)
	{
		glWidget_->paint3D();
		glWidget_->paint2D();
	}
}


void Graphic3DWindow::xyMouseReleaseSlot( QMouseEvent *e )
{
	if(e->state() == (Qt::LeftButton | Qt::ControlButton))
		return;

	glWidget_->addSelection_ = false;
	if(e->state() == (Qt::LeftButton | Qt::ShiftButton))
		glWidget_->addSelection_ = true;
	glWidget_->drawSelecteds();
}


void Graphic3DWindow::zxMouseReleaseSlot( QMouseEvent *e )
{
	if(e->state() == (Qt::LeftButton | Qt::ControlButton))
		return;

	glWidget_->addSelection_ = false;
	if(e->state() == (Qt::LeftButton | Qt::ShiftButton))
		glWidget_->addSelection_ = true;
	glWidget_->drawSelecteds();
}


void Graphic3DWindow::yzMouseReleaseSlot( QMouseEvent *e )
{
	if(e->state() == (Qt::LeftButton | Qt::ControlButton))
		return;

	glWidget_->addSelection_ = false;
	if(e->state() == (Qt::LeftButton | Qt::ShiftButton))
		glWidget_->addSelection_ = true;
	glWidget_->drawSelecteds();
}




void Graphic3DWindow::xySetCursorSlot( const QPoint &p )
{
	GLfloat w = (GLfloat)xyTeQtFrame->width();
	int ctype = Qt::ArrowCursor;
	int x1 = TeRound((glWidget_->xOff_ + 1.) * w / 2.);
	int y1 = (int)(w - TeRound((glWidget_->yOff_ + 1.) * w / 2.));
	int x2 = TeRound((glWidget_->xOff_ + 1. + glWidget_->xSize_) * w / 2.);
	int y2 = (int)(w - TeRound((glWidget_->yOff_ + 1. + glWidget_->ySize_) * w / 2.));

	QRect x1y1(0, 0, 9, 9);
	x1y1.moveCenter(QPoint(x1, y1));
	QRect x1y2(0, 0, 9, 9);
	x1y2.moveCenter(QPoint(x1, y2));
	QRect x2y1(0, 0, 9, 9);
	x2y1.moveCenter(QPoint(x2, y1));
	QRect x2y2(0, 0, 9, 9);
	x2y2.moveCenter(QPoint(x2, y2));
	QRect xx1(0, 0, 9, y1-y2);
	xx1.moveCenter(QPoint(x1, TeRound(((double)y1-(double)y2)/2.)+y2));
	QRect xx2(0, 0, 9, y1-y2);
	xx2.moveCenter(QPoint(x2, TeRound(((double)y1-(double)y2)/2.)+y2));
	QRect yy1(0, 0, x2-x1, 9);
	yy1.moveCenter(QPoint(TeRound(((double)x2-(double)x1)/2.)+x1, y1));
	QRect yy2(0, 0, x2-x1, 9);
	yy2.moveCenter(QPoint(TeRound(((double)x2-(double)x1)/2.)+x1, y2));
	int cw = x2 - x1;
	int ch = y1 - y2;
	if(ch > 15)
		ch -= 9;
	if(cw > 15)
		cw -= 9;
	QRect c(0, 0, cw, ch);
	c.moveCenter(QPoint(TeRound(((double)x2-(double)x1)/2.)+x1, TeRound(((double)y1-(double)y2)/2.)+y2));

	dragMode_ = noneMode;
	if(c.contains(p))
	{
		ctype = Qt::PointingHandCursor;
		dragMode_ = cMode;
	}
	else if(x1y1.contains(p))
	{
		ctype = Qt::SizeBDiagCursor;
		dragMode_ = x1y1Mode;
	}
	else if(x1y2.contains(p))
	{
		ctype = Qt::SizeFDiagCursor;
		dragMode_ = x1y2Mode;
	}
	else if(x2y1.contains(p))
	{
		ctype = Qt::SizeFDiagCursor;
		dragMode_ = x2y1Mode;
	}
	else if(x2y2.contains(p))
	{
		ctype = Qt::SizeBDiagCursor;
		dragMode_ = x2y2Mode;
	}
	else if(xx1.contains(p))
	{
		ctype = Qt::SizeHorCursor;
		dragMode_ = x1Mode;
	}
	else if(xx2.contains(p))
	{
		ctype = Qt::SizeHorCursor;
		dragMode_ = x2Mode;
	}
	else if(yy1.contains(p))
	{
		ctype = Qt::SizeVerCursor;
		dragMode_ = y1Mode;
	}
	else if(yy2.contains(p))
	{
		ctype = Qt::SizeVerCursor;
		dragMode_ = y2Mode;
	}

	QCursor cursor(ctype);
	xyTeQtFrame->setCursor(cursor);
}


void Graphic3DWindow::zxSetCursorSlot( const QPoint &p )
{
	double w = (double)zxTeQtFrame->width();
	int ctype = Qt::ArrowCursor;
	int x1 = TeRound((glWidget_->zOff_ + 1.) * w / 2.);
	int y1 = (int)(w - TeRound((glWidget_->xOff_ + 1.) * w / 2.));
	int x2 = TeRound((glWidget_->zOff_ + 1. + glWidget_->zSize_) * w / 2.);
	int y2 = (int)(w - TeRound((glWidget_->xOff_ + 1. + glWidget_->xSize_) * w / 2.));

	QRect x1y1(0, 0, 9, 9);
	x1y1.moveCenter(QPoint(x1, y1));
	QRect x1y2(0, 0, 9, 9);
	x1y2.moveCenter(QPoint(x1, y2));
	QRect x2y1(0, 0, 9, 9);
	x2y1.moveCenter(QPoint(x2, y1));
	QRect x2y2(0, 0, 9, 9);
	x2y2.moveCenter(QPoint(x2, y2));
	QRect xx1(0, 0, 9, y1-y2);
	xx1.moveCenter(QPoint(x1, TeRound(((double)y1-(double)y2)/2.)+y2));
	QRect xx2(0, 0, 9, y1-y2);
	xx2.moveCenter(QPoint(x2, TeRound(((double)y1-(double)y2)/2.)+y2));
	QRect yy1(0, 0, x2-x1, 9);
	yy1.moveCenter(QPoint(TeRound(((double)x2-(double)x1)/2.)+x1, y1));
	QRect yy2(0, 0, x2-x1, 9);
	yy2.moveCenter(QPoint(TeRound(((double)x2-(double)x1)/2.)+x1, y2));
	int cw = x2 - x1;
	int ch = y1 - y2;
	if(ch > 15)
		ch -= 9;
	if(cw > 15)
		cw -= 9;
	QRect c(0, 0, cw, ch);
	c.moveCenter(QPoint(TeRound(((double)x2-(double)x1)/2.)+x1, TeRound(((double)y1-(double)y2)/2.)+y2));

	dragMode_ = noneMode;
	if(c.contains(p))
	{
		ctype = Qt::PointingHandCursor;
		dragMode_ = cMode;
	}
	else if(x1y1.contains(p))
	{
		ctype = Qt::SizeBDiagCursor;
		dragMode_ = x1y1Mode;
	}
	else if(x1y2.contains(p))
	{
		ctype = Qt::SizeFDiagCursor;
		dragMode_ = x1y2Mode;
	}
	else if(x2y1.contains(p))
	{
		ctype = Qt::SizeFDiagCursor;
		dragMode_ = x2y1Mode;
	}
	else if(x2y2.contains(p))
	{
		ctype = Qt::SizeBDiagCursor;
		dragMode_ = x2y2Mode;
	}
	else if(xx1.contains(p))
	{
		ctype = Qt::SizeHorCursor;
		dragMode_ = x1Mode;
	}
	else if(xx2.contains(p))
	{
		ctype = Qt::SizeHorCursor;
		dragMode_ = x2Mode;
	}
	else if(yy1.contains(p))
	{
		ctype = Qt::SizeVerCursor;
		dragMode_ = y1Mode;
	}
	else if(yy2.contains(p))
	{
		ctype = Qt::SizeVerCursor;
		dragMode_ = y2Mode;
	}

	QCursor cursor(ctype);
	zxTeQtFrame->setCursor(cursor);
}


void Graphic3DWindow::yzSetCursorSlot( const QPoint &p )
{
	double w = (double)yzTeQtFrame->width();
	int ctype = Qt::ArrowCursor;
	int x1 = TeRound((glWidget_->yOff_ + 1.) * w / 2.);
	int y1 = (int)(w - TeRound((glWidget_->zOff_ + 1.) * w / 2.));
	int x2 = TeRound((glWidget_->yOff_ + 1. + glWidget_->ySize_) * w / 2.);
	int y2 = (int)(w - TeRound((glWidget_->zOff_ + 1. + glWidget_->zSize_) * w / 2.));

	QRect x1y1(0, 0, 9, 9);
	x1y1.moveCenter(QPoint(x1, y1));
	QRect x1y2(0, 0, 9, 9);
	x1y2.moveCenter(QPoint(x1, y2));
	QRect x2y1(0, 0, 9, 9);
	x2y1.moveCenter(QPoint(x2, y1));
	QRect x2y2(0, 0, 9, 9);
	x2y2.moveCenter(QPoint(x2, y2));
	QRect xx1(0, 0, 9, y1-y2);
	xx1.moveCenter(QPoint(x1, TeRound(((double)y1-(double)y2)/2.)+y2));
	QRect xx2(0, 0, 9, y1-y2);
	xx2.moveCenter(QPoint(x2, TeRound(((double)y1-(double)y2)/2.)+y2));
	QRect yy1(0, 0, x2-x1, 9);
	yy1.moveCenter(QPoint(TeRound(((double)x2-(double)x1)/2.)+x1, y1));
	QRect yy2(0, 0, x2-x1, 9);
	yy2.moveCenter(QPoint(TeRound(((double)x2-(double)x1)/2.)+x1, y2));
	int cw = x2 - x1;
	int ch = y1 - y2;
	if(ch > 15)
		ch -= 9;
	if(cw > 15)
		cw -= 9;
	QRect c(0, 0, cw, ch);
	c.moveCenter(QPoint(TeRound(((double)x2-(double)x1)/2.)+x1, TeRound(((double)y1-(double)y2)/2.)+y2));

	dragMode_ = noneMode;
	if(c.contains(p))
	{
		ctype = Qt::PointingHandCursor;
		dragMode_ = cMode;
	}
	else if(x1y1.contains(p))
	{
		ctype = Qt::SizeBDiagCursor;
		dragMode_ = x1y1Mode;
	}
	else if(x1y2.contains(p))
	{
		ctype = Qt::SizeFDiagCursor;
		dragMode_ = x1y2Mode;
	}
	else if(x2y1.contains(p))
	{
		ctype = Qt::SizeFDiagCursor;
		dragMode_ = x2y1Mode;
	}
	else if(x2y2.contains(p))
	{
		ctype = Qt::SizeBDiagCursor;
		dragMode_ = x2y2Mode;
	}
	else if(xx1.contains(p))
	{
		ctype = Qt::SizeHorCursor;
		dragMode_ = x1Mode;
	}
	else if(xx2.contains(p))
	{
		ctype = Qt::SizeHorCursor;
		dragMode_ = x2Mode;
	}
	else if(yy1.contains(p))
	{
		ctype = Qt::SizeVerCursor;
		dragMode_ = y1Mode;
	}
	else if(yy2.contains(p))
	{
		ctype = Qt::SizeVerCursor;
		dragMode_ = y2Mode;
	}

	QCursor cursor(ctype);
	yzTeQtFrame->setCursor(cursor);
}

void Graphic3DWindow::functionComboBox_activated( const QString &t )
{
	string function = t.latin1();
	glWidget_->changeFunction(function);
	glWidget_->setFocus();
}

void Graphic3DWindow::inputComboBox_activated( const QString &t )
{
	int input = TeAll;

	if(t == tr("Queried"))
		input = TeSelectedByQuery;
	else if(t == tr("Not Queried"))
		input = TeNotSelectedByQuery;
	else if(t == tr("Grouped"))
		input = TeGrouped;
	else if(t == tr("Not Grouped"))
		input = TeNotGrouped;

	glWidget_->changeInputObjects(input);
	glWidget_->setFocus();
}


void Graphic3DWindow::updateSelection()
{
	glWidget_->updateSelection();
}


void Graphic3DWindow::closeEvent( QCloseEvent * )
{
	glWidget_->deleteGLObject();
	hide();
}

void Graphic3DWindow::initData()
{
	TeQtGrid* grid = mainWindow_->getGrid();
	TeAppTheme* appTheme = grid->getTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();
	TeAttributeList attList;
	if(theme->getProductId() == TeTHEME)
		attList = theme->sqlAttList();
	else if(theme->getProductId() == TeEXTERNALTHEME)
		attList = ((TeExternalTheme*)theme)->getRemoteTheme()->sqlAttList();

	int n, nc = grid->numCols();
	vector<int>	cols;

	for(n = 0; n < nc; ++n)
	{
		if (grid->isColumnSelected(n, true) == true)
		{
			int col = grid->getColumn(n);
			if(attList[col].rep_.type_ == TeREAL ||
				attList[col].rep_.type_ == TeINT)
				cols.push_back(col);
		}
	}

	int x = cols[0];
	int y = cols[1];
	int z = cols[2];
	string xField = attList[x].rep_.name_;
	string yField = attList[y].rep_.name_;
	string zField = attList[z].rep_.name_;
	xLineEdit->setText(xField.c_str());
	yLineEdit->setText(yField.c_str());
	zLineEdit->setText(zField.c_str());
	
    // Create OpenGL widget
    glWidget_->xField_ = xField;
    glWidget_->yField_ = yField;
    glWidget_->zField_ = zField;
    glWidget_->theme_ = theme;
	glWidget_->db_ = theme->layer()->database();

	glWidget_->initGLObject();
	//updateSelection();
}


void Graphic3DWindow::qualitySlider_valueChanged( int v)
{
	glWidget_->changeQuality(v);
}


void Graphic3DWindow::mouseMoveEvent( QMouseEvent * )
{

}


void Graphic3DWindow::queryChangedSlot()
{
	QString s = inputComboBox->currentText();
	if(s == tr("Queried") || s == tr("Not Queried"))
		glWidget_->initGLObject();
}


void Graphic3DWindow::groupChangedSlot()
{
	QString s = inputComboBox->currentText();
	if(s == tr("Grouped") || s == tr("Not Grouped"))
		glWidget_->initGLObject();
}

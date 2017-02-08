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

#ifndef TEQWTPLOT_H
#define TEQWTPLOT_H

#include "../../kernel/TeDefines.h"

#include <qwt_plot.h>
#include <qevent.h>
#include <qpainter.h>
#include "TeQwtPlotPicker.h"

class TL_DLL TeQwtPlot:public QwtPlot
{
    Q_OBJECT

public:

	TeQwtPlot(QWidget *p = 0);
	TeQwtPlot(const QwtText &title, QWidget *p = NULL);

	virtual ~TeQwtPlot() {delete cursor_;}

	void showCursor(bool b) {cursor_->show(b);}
	void zoomActive(bool b) {cursor_->zoomActive(b);}
	void cursorMode(int m) {cursor_->mode(m);}
	TeQwtPlotPicker* cursor() {return cursor_;}

private:
	TeQwtPlotPicker* cursor_;
};

#endif

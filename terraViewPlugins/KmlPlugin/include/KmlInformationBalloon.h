#ifndef KMLINFORMATIONBALLOON_H
#define KMLINFORMATIONBALLOON_H

#include <qapplication.h>
#include <qwidget.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qfile.h>
#include <string>
#include <stdlib.h>
#include <qsize.h>

class KmlInformationBalloon : public QWidget
{
public:
    KmlInformationBalloon( QWidget *parent=0, const char *name=0, WFlags f = 0);
	
	void init(const QSize & canvasSize, const QPoint & local, const std::string & name, const std::string & description);

protected:
    void mousePressEvent( QMouseEvent *);
    void mouseMoveEvent( QMouseEvent *);
	void paintEvent(QPaintEvent* e);
	QImage checkImagePosition(const QImage & img, QPoint & position);
	void setComponentsPosition(const int & quad);
private:
    QPoint _local;
	QSize _canvasSize;
	QSize _imgSize;
	int _currentQuad;
	QPoint _exitPosition;
	QPoint _nameTextPosition;
	QPoint _descriptionTextPosition;
};

#endif // KMLINFORMATIONBALLOON_H
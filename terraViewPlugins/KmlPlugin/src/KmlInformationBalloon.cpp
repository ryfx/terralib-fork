#include <KmlInformationBalloon.h>

#include <qtextedit.h>
#include <qpainter.h>
#include <qcursor.h>

KmlInformationBalloon::KmlInformationBalloon( QWidget *parent, const char *name, WFlags f)	
		:QWidget(parent,name, Qt::WStyle_Customize|Qt::WStyle_NoBorder)
{
	this->setMouseTracking(true);
}

void KmlInformationBalloon::mousePressEvent( QMouseEvent *e )
{   
	if(e->pos().x() >= _exitPosition.x() && e->pos().x() <= _exitPosition.x()+15 
		&& e->pos().y() >= _exitPosition.y() && e->pos().y() <= _exitPosition.y()+15)
		this->hide();
}

void KmlInformationBalloon::mouseMoveEvent( QMouseEvent *e )
{
	
	QCursor cur;
    if(e->pos().x() >= _exitPosition.x() && e->pos().x() <= _exitPosition.x()+15 
		&& e->pos().y() >= _exitPosition.y() && e->pos().y() <= _exitPosition.y()+15)
	{
		cur.setShape(Qt::PointingHandCursor);
		this->setCursor(cur);
	}		
	else
	{
		cur.setShape(Qt::ArrowCursor);
		this->setCursor(cur);
	}
}


void KmlInformationBalloon::init(const QSize & canvasSize, const QPoint & local, const std::string & name, const std::string & description)
{
	_canvasSize = canvasSize;
	_local = local;	
	
	QImage img = QImage::fromMimeSource("Balloon.png");
				
	QPoint newPosition = _local;
	img = checkImagePosition(img, newPosition);
	move(newPosition);



    QPixmap p;
    p.convertFromImage( img );
    if ( !p.mask() )
	if ( img.hasAlphaBuffer() ) {
	    QBitmap bm;
	    bm = img.createAlphaMask();
	    p.setMask( bm );
	} else {
	    QBitmap bm;
	    bm = img.createHeuristicMask();
	    p.setMask( bm );
	}		

    setBackgroundPixmap( p );
    setFixedSize( p.size() );
    if ( p.mask() )
		setMask( *p.mask() );	
	
	
	//Name Text Edit
	QTextEdit* nameTextEdit = new QTextEdit(this);
	nameTextEdit->move(_nameTextPosition);
	nameTextEdit->resize(255,35);
	QFont nameFont("Arial Black", 12);
	nameTextEdit->setFont(nameFont);
	nameTextEdit->setFrameShape(QFrame::NoFrame);	
	nameTextEdit->setAlignment(Qt::AlignCenter);
	nameTextEdit->setReadOnly(true);
	nameTextEdit->setText(name.c_str());
	//----------------------------------------------

	//Description Text Edit
	QTextEdit* descriptionTextEdit = new QTextEdit(this);
	descriptionTextEdit->move(_descriptionTextPosition);
	descriptionTextEdit->resize(288, 190);
	QFont descriptionFont("Arial", 10);
	descriptionTextEdit->setFont(descriptionFont);
	descriptionTextEdit->setFrameShape(QFrame::NoFrame);
	descriptionTextEdit->setReadOnly(true);
	descriptionTextEdit->setText(description.c_str());
	//----------------------------------------------
	
	
	
		
}

void KmlInformationBalloon::paintEvent(QPaintEvent* e)
{

	QImage exitImg = QImage::fromMimeSource("ExitButton.png");

	QPainter* painter = new QPainter(this);
	painter->drawImage(_exitPosition.x(), _exitPosition.y(), exitImg);	
	
}

QImage KmlInformationBalloon::checkImagePosition(const QImage & img, QPoint & position)
{
	//Quadrantes da tela
	//-------------
	//| QD1 | QD2 |
	//-------------
	//| QD4 | QD3 |
	//-------------
	
	QSize scSize = QApplication::desktop()->size();
	QImage temp = img;

	if(_local.x() < scSize.width()/2 && _local.y() < scSize.height()/2)//QD 1
	{
		temp = temp.mirror(true, true);
		setComponentsPosition(1);
	}
	if(_local.x() > scSize.width()/2 && _local.y() < scSize.height()/2)//QD 2
	{
		temp = temp.mirror(false, true);
		position.setX(position.x()-img.width());
		setComponentsPosition(2);
	}
	if(_local.x() > scSize.width()/2 && _local.y() > scSize.height()/2)//QG 3
	{
		position.setX(position.x()-img.width());
		position.setY(position.y()-img.height());
		setComponentsPosition(3);
	}

	if(_local.x() < scSize.width()/2 && _local.y() > scSize.height()/2)//QG 4
	{
		temp = temp.mirror(true, false);		
		position.setY(position.y()-img.height());
		setComponentsPosition(4);
	}

	return temp;

}

void KmlInformationBalloon::setComponentsPosition(const int & quad)
{
	if(quad <= 2)//Quads 1 e 2
	{
		_exitPosition.setX(272);
		_exitPosition.setY(70);
		
		_nameTextPosition.setX(12);
		_nameTextPosition.setY(73);
		
		_descriptionTextPosition.setX(12);
		_descriptionTextPosition.setY(107);		

	}
	else if(quad >= 3)//Quads 3 e 4
	{
		_exitPosition.setX(272);
		_exitPosition.setY(20);

		_nameTextPosition.setX(12);
		_nameTextPosition.setY(23);
		
		_descriptionTextPosition.setX(12);
		_descriptionTextPosition.setY(57);
	}	
}
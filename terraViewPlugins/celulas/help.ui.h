/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <qdir.h>
#include <TeWaitCursor.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qsimplerichtext.h>
#include <qmessagebox.h>

void Help::init( QString helpFile )
{
	erro_ = false;
	QDir dir;
	QString absFile = dir.absPath() + "/doc/port/";
	absFile += helpFile;
	QFileInfo info(absFile);
	bool exist = info.exists();
	bool isReadable = info.isReadable();
	if(exist==false || isReadable==false)
	{
		if(dir.cdUp())
		{
			if(dir.cdUp())
			{
				absFile = dir.absPath() + "/doc/port/";
				absFile += helpFile;
				QFileInfo info(absFile);
				exist = info.exists();
				isReadable = info.isReadable();
				if(exist==false || isReadable==false)
					erro_ = true;;
			}
			else
				erro_ = true;;
		}
		else
			erro_ = true;;
	}

	if(erro_)
	{
		QMessageBox::information(this, "Help", tr("The help of this interface is not available yet!")); 
		return;
	}

    connect(homePushButton, SIGNAL(clicked()), textBrowser, SLOT(home()));
    connect(backwardPushButton, SIGNAL(clicked()), this, SLOT(backward()));
    connect(forwardPushButton, SIGNAL(clicked()), this, SLOT(forward()));
    connect(textBrowser, SIGNAL(linkClicked(const QString&)), this, SLOT(linkClicked(const QString&)));
    connect(textBrowser, SIGNAL(anchorClicked(const QString&, const QString&)), this, SLOT(anchorClicked(const QString&, const QString&)));

	textBrowser->setSource(absFile);
	erro_ = false;
}

void Help::printPushButton_clicked()
{
    QPrinter printer( QPrinter::HighResolution );
    printer.setFullPage(true);
    printer.setOptionEnabled(QPrinter::PrintSelection, false);
    printer.setOptionEnabled(QPrinter::PrintPageRange, false);

    if ( printer.setup( this ) )
	{
        QPainter p( &printer );
        if( !p.isActive() ) // starting printing failed
            return;
        QPaintDeviceMetrics metrics(p.device());
        int dpiy = metrics.logicalDpiY();
        int margin = (int) ( (2/2.54)*dpiy ); // 2 cm margins
        QRect body( margin, margin, metrics.width() - 2*margin, metrics.height() - 2*margin );
        QSimpleRichText richText( textBrowser->text(),
                                  textBrowser->font(),
                                  textBrowser->context(),
                                  textBrowser->styleSheet(),
                                  textBrowser->mimeSourceFactory(),
                                  body.height() );
        richText.setWidth( &p, body.width() );
        QRect view( body );

		int page = 1;
		do
		{
			richText.draw( &p, body.left(), body.top(), view, colorGroup() );
			view.moveBy( 0, body.height() );
			p.translate( 0 , -body.height() );
			p.drawText( view.right() - p.fontMetrics().width( QString::number(page) ),
						view.bottom() + p.fontMetrics().ascent() + 5, QString::number(page) );
			if ( view.top()  >= richText.height() )
				break;
			printer.newPage();
			page++;
		} while (TRUE);
	}
}


void Help::backward()
{
	if(paragVec_.size() == 0)
	{
		textBrowser->home();
		return;
	}

	textBrowser->backward();

	int para = paragVec_[paragVec_.size()-1];
	paragVec_.pop_back();
	int index = indexVec_[indexVec_.size()-1];
	indexVec_.pop_back();

	textBrowser->setCursorPosition(para, index);
	textBrowser->ensureCursorVisible();
}


void Help::forward()
{  
	int para, index;

	textBrowser->getCursorPosition(&para, &index);

	paragVec_.push_back(para);
	indexVec_.push_back(index);

	textBrowser->forward();
}


void Help::reject()
{
	hide();
}


void Help::anchorClicked( const QString &, const QString & )
{

}


void Help::boldCheckBox_clicked()
{
//	fontSizeComboBox_activated(fontSizeComboBox->currentText());
}


void Help::fontSizeComboBox_activated( const QString & )
{
//	QFont font = textBrowser->font();
//	font.setPointSize(size.toInt());
//	font.setBold(boldCheckBox->isChecked());
//	textBrowser->setFont(font);
//	textBrowser->update();
}


void Help::linkClicked( const QString & )
{
	int para, index;

	textBrowser->getCursorPosition(&para, &index);

	paragVec_.push_back(para);
	indexVec_.push_back(index);
}

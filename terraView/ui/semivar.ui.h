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

#include <terraViewBase.h>
#include <TeAppTheme.h>
#include <TeWaitCursor.h>
#include <TeSemivarModelFactory.h>

#include "TeDatabase.h"
#include "TeSTElementSet.h"
#include "TeSTEFunctionsDB.h"
#include "TeMatrix.h"

#include <qpainter.h>
#include <graphic.h>

static const char* Model_items[] = { "Esferic", "Exponential", "Gaussian", 0 };
static const char* Sel_items[] = { "Semivariogram", 0 };

void Semivar::init()
{
	help_ = 0;
	elementSet_ = 0;
	erro_ = false;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	theme_ = mainWindow_->currentTheme();
	if(theme_->getTheme()->type() != TeTHEME)
		return;

	if(theme_ == 0)
	{
		erro_ = true;
		QString msg = tr("You must select, before, a theme !") + "\n";
		msg += tr("Select a theme and try again.");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}
	TeTheme* curBaseTheme = (TeTheme*)theme_->getTheme();
	layer_ = curBaseTheme->layer();
	db_ = layer_->database();
	TeGeomRep gType = (TeGeomRep)(curBaseTheme->visibleGeoRep() & ~TeTEXT); // TeTEXT not is considered;
	if(gType != TePOINTS)
	{
		erro_ = true;
		QString msg = tr("Selected theme should have POINTS as the only visible representation!") + "\n";
		msg += tr("Select another theme and try again.");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	TeAttributeList attrList = curBaseTheme->sqlNumAttList();
	if (attrList.empty())
	{
		erro_ = true;
		QString msg = tr("Selected theme should have at least one numeric attribute!") + "\n";
		msg += tr("Select another theme and try again.");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	vector<string> attrVec;
	for(int i = 0; i<(int)attrList.size(); ++i)
		attrVec.push_back(attrList[i].rep_.name_);

	elementSet_ = new TeSTElementSet(curBaseTheme);
	if(!TeSTOSetBuildDB (elementSet_, true, false, attrVec))
	{
		erro_ = true;
		QString msg = tr("Fail to Build TeSTElementSet!");
		QMessageBox::warning(this, tr("Warning"), msg);
		delete elementSet_;
		return;
	}
	if(elementSet_->numElements() > 500)
	{
		QString msg1 = tr("Question!");
		QString msg2 = tr("The theme has many data for dinamic display!") + "\n";
		msg2 +=	tr("Do you want continue?");
		int response = QMessageBox::question(this, msg1, msg2, tr("Yes"), tr("No"));

		if (response != 0)
		{
			erro_ = true;
			return;
		}
	}
	
	// Incializar o combo box de variaveis
	TeAttributeList list = elementSet_->getAttributeList();
	for(unsigned int i=0; i<list.size(); ++i)
	{
		TeAttrDataType type = list[i].rep_.type_;
		string name = list[i].rep_.name_;
		if(type == TeREAL || type == TeINT)
			variableComboBox->insertItem( name.c_str() );
	}

	fill_MVEmatrix();

	// Inicializar o combo box de estatisticas
	choiceComboBox->insertStrList( Sel_items );

	// Inicializar os campos de textos de parametros da estatistica
	nLagLineEdit->setText("6");
	incrementLineEdit->setText("968.0");
	directionLineEdit->setText("0.0");
	angularTolLineEdit->setText("90.0");

	// Inicializar o combo box de modelos
	modelComboBox->insertStrList( Model_items );

	// Inicializar os campos de textos de parametros do modelo
	pepitaEffectLineEdit->setText(0);
	contributionLineEdit->setText(0);
	rangeLineEdit->setText(0);

//	if(!variableComboBox->currentText().isEmpty())
//		okPushButton_clicked();
}

///////////////////////////
void Semivar::fill_MVEmatrix()
{
	TePointSet		pts, pts1;
	TePoint			pt;
	TeCoord2D		c2d;
	double			x1, y1, x2, y2, dist, angle;
	int				i, j, svarm_size;
	string			valor;
	TeWaitCursor	wait;

	// Define tamanho da matriz auxiliar = Número de pontos 
	svarm_size = elementSet_->numElements();

	// Inicializa matriz auxiliar para calculo do Semivariograma
	MSVParams_.MSVPmatrix_.Clear();
	MSVParams_.MSVPmatrix_.Init (svarm_size, svarm_size);

	// Define iterador it1 para o conjunto de STO
	TeSTElementSet::iterator it1 = elementSet_->begin();

	// Define iterador it2 para o conjunto de STO
	TeSTElementSet::iterator it2 = elementSet_->begin();

	// Calcula a distância e o angulo entre todos os pares de pontos
	for (i=0; i<svarm_size; ++i)
	{
		// Ler o valor do atributo selecionado
		string val;
		string s = variableComboBox->currentText().latin1();
		(*it1).getPropertyValue (s, val);

		// Escrever na diagonal da matriz o valor do atributo
		MSVParams_.MSVPmatrix_(i,i)= atof(val.c_str());
		
		// Obtem a geometria do objeto
		pts.clear();
		(*it1).getGeometry (pts);

		// Obtem a localização X Y do Ponto de referencia
		x1 = pts[0].location().x();
		y1 = pts[0].location().y();
		
	    // Incrementa Iterador para próximo STO
		++it1;

		for (j=i+1; j<svarm_size; ++j)
		{
			// Incrementa o iterador
			++it2;

			// Obtem a geometria do objeto
			pts1.clear();
			(*it2).getGeometry (pts1);

			// Obtem a localização X Y do Ponto
			x2 = pts1[0].location().x();
			y2 = pts1[0].location().y();
		
			//Calcular distancia
			dist = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
			
			//Escreve o valor da distancia na parte superior da matrix
			MSVParams_.MSVPmatrix_(i,j)=dist;

			// Calcular angulo segundo padrao geoestatistica (Norte=0 graus e Leste=90 graus)
			angle = -( atan ((y1-y2)/(x1-x2)) * (180/3.14) ) + 90.0;
			
			//Escreve o valor do angulo na parte inferior da matrix
			MSVParams_.MSVPmatrix_(j,i) = angle;
		}
		it2 = it1;		
	}
	calculaVariancia();
}

void Semivar::fill_MVEmatrixDiag()
{
	int				i, svarm_size;
	string			val;
	TeWaitCursor	wait;
	string s = variableComboBox->currentText().latin1();

	// Define tamanho da matriz auxiliar = Número de pontos 
	svarm_size = elementSet_->numElements();

	// Define iterador it1 para o conjunto de STO
	TeSTElementSet::iterator it1 = elementSet_->begin();

	// Calcula a distância e o angulo entre todos os pares de pontos
	for (i=0; i<svarm_size; ++i)
	{
		// Ler o valor do atributo selecionado
		(*it1).getPropertyValue (s, val);

		// Escrever na diagonal da matriz o valor do atributo
		MSVParams_.MSVPmatrix_(i,i)= atof(val.c_str());
		
	    // Incrementa Iterador para próximo STO
		++it1;
	}
	calculaVariancia();
}


//////////////////////////////
void Semivar::calculaVariancia()
{
	int		i;
	double	sm2, sm1, media;

	sm1=sm2=0.;
	media=0.;

	for (i=0; i<MSVParams_.MSVPmatrix_.Nrow(); ++i)
	{
		sm1 = sm1 + MSVParams_.MSVPmatrix_(i,i);
		sm2 = sm2 + (MSVParams_.MSVPmatrix_(i,i)*MSVParams_.MSVPmatrix_(i,i));
	}

	media = sm1/MSVParams_.MSVPmatrix_.Nrow();
	MVEvariancia_ = (sm2/MSVParams_.MSVPmatrix_.Nrow()) - media*media;
}



void Semivar::variableComboBox_activated( const QString &variavel )
{
	fill_MVEmatrixDiag();
	if(!variavel.isEmpty())
		okPushButton_clicked();
}


void Semivar::choiceComboBox_activated( const QString & )
{
	// Preenche a estrutura de parametros
	MSVParams_.MSVPnlag_ = nLagLineEdit->text().toFloat();		
	MSVParams_.MSVPincr_ = incrementLineEdit->text().toFloat();
	MSVParams_.MSVPdir_ = directionLineEdit->text().toFloat();
	MSVParams_.MSVPtoldir_ = angularTolLineEdit->text().toFloat();

	// Constroi fabrica de medidas de variabilidade espacial
	string s = choiceComboBox->currentText().latin1(); 
	MVEgraph_.Clear();
	TeMSV* msv = TeMSVFactory::make(s, MSVParams_);
	msv->setMatrix(&(MSVParams_.MSVPmatrix_));
	MVEgraph_ = msv->calculate();
	delete (msv);
}


void Semivar::okPushButton_clicked()
{
	QString	val;
	int		min, max, j;
	TeWaitCursor	wait;

	clearPushButton_clicked();
	MVEpressexec_ = true;
	choiceComboBox_activated("");

//--------------------------
	min=0;
	max=(int)MVEvariancia_;

	pepitaEffectSlider->setValue(min);
	j = int(double(max-min)/74. + 0.5);
	pepitaEffectSlider->setRange(min-j, max+j);

	val.setNum(min);
	pepitaEffectLineEdit->setText(val);

//--------------------------
	min=0;
	max=(int)MVEvariancia_;

	contributionSlider->setValue(min);
	j = int(double(max-min)/74. + 0.5);
	contributionSlider->setRange(min-j, max+j);

	val.setNum(min);
	contributionLineEdit->setText(val);

//--------------------------
	min=atoi(incrementLineEdit->text().latin1());
	j=atoi(nLagLineEdit->text().latin1());

	max=min*j;

	rangeSlider->setValue(min);
	j = int(double(max-min)/74. + 0.5);
	rangeSlider->setRange(min-j, max+j);

	val.setNum(min);
	rangeLineEdit->setText(val);

//--------------------------
	mainWindow_->getGraphicWindow();
	mainWindow_->getGraphicWindow()->setAttributeTitle(variableComboBox->currentText().latin1());
	mainWindow_->getGraphicWindow()->setSemiVariogramValues(MVEgraph_, MVEmodel_);
	mainWindow_->getGraphicWindow()->applyGraphic();
	mainWindow_->getGraphicWindow()->show();
	MVEpressexec_ = false;
}


void Semivar::modelComboBox_activated( const QString &modelo )
{
	// Preenche a estrutura de parametros
	TeSemivarModelParams	st;
	st.efeitopepita_ = pepitaEffectLineEdit->text().toFloat()+0.5;		
	st.contribuicao_ = contributionLineEdit->text().toFloat()+0.5;
	st.alcance_ = rangeLineEdit->text().toFloat()+0.5;

	// Constroi fabrica de modelos de ajuste
	string s = modelComboBox->currentText().latin1(); 
	if(MVEpressexec_ == false)
	{
		MVEmodel_.Clear();
		MVEmodel_ = TeSemivarModelFactory::make(s, st)->calculate(MVEgraph_);
	}

	if(modelo.isEmpty() == false)
	{
		mainWindow_->getGraphicWindow()->setSemiVariogramValues(MVEgraph_, MVEmodel_);
		mainWindow_->getGraphicWindow()->applyGraphic();
		mainWindow_->getGraphicWindow()->show();
	}
}


void Semivar::pepitaEffectSlider_valueChanged( int i)
{
	QString s;
	s.setNum(i);//pepitaEffectSlider->value());
	pepitaEffectLineEdit->setText(s);	
	modelComboBox_activated("");
	mainWindow_->getGraphicWindow()->setSemiVariogramValues(MVEgraph_, MVEmodel_);
	mainWindow_->getGraphicWindow()->applyGraphic();
	mainWindow_->getGraphicWindow()->show();
}


void Semivar::pepitaEffectLineEdit_returnPressed()
{
	QString x;
	pepitaEffectSlider->setValue(pepitaEffectLineEdit->text().toInt());
	modelComboBox_activated(x);
}

void Semivar::contributionLineEdit_returnPressed()
{
	QString x;
	contributionSlider->setValue(contributionLineEdit->text().toInt());
	modelComboBox_activated(x);
}


void Semivar::rangeSlider_valueChanged( int i)
{
	QString s;
	s.setNum(i);//rangeSlider->value());
	rangeLineEdit->setText(s);	
	MVEpressexec_= false;
	modelComboBox_activated("");
	mainWindow_->getGraphicWindow()->setSemiVariogramValues(MVEgraph_, MVEmodel_);
	mainWindow_->getGraphicWindow()->applyGraphic();
	mainWindow_->getGraphicWindow()->show();
}


void Semivar::rangeSlider_sliderMoved( int i)
{
	QString s;
	s.setNum(i);//rangeSlider->value());
	rangeLineEdit->setText(s);	
	modelComboBox_activated("");
	mainWindow_->getGraphicWindow()->setSemiVariogramValues(MVEgraph_, MVEmodel_);
	mainWindow_->getGraphicWindow()->applyGraphic();
	mainWindow_->getGraphicWindow()->show();
}


void Semivar::rangeLineEdit_returnPressed()
{
	QString x;
	rangeSlider->setValue(rangeLineEdit->text().toInt());
	modelComboBox_activated(x);
}


void Semivar::pepitaEffectSlider_sliderMoved( int i)
{
	QString s;
	s.setNum(i);//pepitaEffectSlider->value());
	pepitaEffectLineEdit->setText(s);	
	modelComboBox_activated("");
	mainWindow_->getGraphicWindow()->setSemiVariogramValues(MVEgraph_, MVEmodel_);
	mainWindow_->getGraphicWindow()->applyGraphic();
	mainWindow_->getGraphicWindow()->show();
}


void Semivar::clearPushButton_clicked()
{
}


void Semivar::contributionSlider_sliderMoved( int i)
{
	QString s;
	s.setNum(i);//contributionSlider->value());
	contributionLineEdit->setText(s);	
	modelComboBox_activated("");
	mainWindow_->getGraphicWindow()->setSemiVariogramValues(MVEgraph_, MVEmodel_);
	mainWindow_->getGraphicWindow()->applyGraphic();
	mainWindow_->getGraphicWindow()->show();
}


void Semivar::contributionSlider_valueChanged( int i)
{
	QString s;
	s.setNum(i);//contributionSlider->value());
	contributionLineEdit->setText(s);	
	modelComboBox_activated("");
	mainWindow_->getGraphicWindow()->setSemiVariogramValues(MVEgraph_, MVEmodel_);
	mainWindow_->getGraphicWindow()->applyGraphic();
	mainWindow_->getGraphicWindow()->show();
}


void Semivar::closeEvent( QCloseEvent * )
{
	if(elementSet_)
		delete elementSet_;
	hide();
}


void Semivar::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("semivar.htm");
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


void Semivar::show()
{
	if(theme_->getTheme()->type() == TeTHEME)
		QDialog::show();
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
	}

}

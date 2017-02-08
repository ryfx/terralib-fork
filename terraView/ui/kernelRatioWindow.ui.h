/************************************************************************************
Exploring and analysis of geographical data using TerraLib and TerraView

Copyright © 2003-2007 INPE and LESTE/UFMG.

Partially funded by CNPq - Project SAUDAVEL, under grant no. 552044/2002-4,
SENASP-MJ and INPE

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This program is distributed hoping it will be useful, however, WITHOUT ANY 
WARRANTIES; neither to the implicit warranty of MERCHANTABILITY OR SUITABILITY FOR
AN SPECIFIC FINALITY. Consult the GNU General Public License for more details.

You must have received a copy of the GNU General Public License with this program.
In negative case, write to the Free Software Foundation, Inc. in the following
address: 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.
***********************************************************************************/

#include <qapplication.h>
#include <TeUtils.h>
#include <TeQtGrid.h>
#include <qmessagebox.h>
#include <TeQtTerraStat.h>
#include <TeStatDataStructures.h>
#include <display.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeQtViewsListView.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <help.h>

void KernelRatioWindow::fillInformationGroup()
{
	TeTheme* dataSetTheme;
	if(setTabWidget->currentPageIndex() == 0) //Set 1
	{
		if (set_1ThemeComboBox->currentText().isEmpty())
			return;
		dataSetTheme = mainWindow_->currentView()->get(set_1ThemeComboBox->currentText().latin1());
	}
	else
	{
		if (set_2ThemeComboBox->currentText().isEmpty())
			return;
		dataSetTheme = mainWindow_->currentView()->get(set_2ThemeComboBox->currentText().latin1());
	}
	if (!dataSetTheme)
	{
		QMessageBox::warning(this, tr("Warning"), tr("There is no data set themes!"));
		return;
	}
	cLTotalEventsValueTextLabel->setText(QString("%1").arg(dataSetTheme->getNumberOfObjects()));

	cLHeightValueTextLabel->clear();
	cLWidthValueTextLabel->clear();
	resYTextLabel->clear();
	resXTextLabel->clear();

	TeBox themeBox;
	// Fill information about the dimensions of the output grid
	if (regionButtonGroup->selectedId() == 0)	// grid over events
		themeBox = dataSetTheme->getThemeBox();
	else if (regionButtonGroup->selectedId() == 1)	// grid over regions
	{
		if (!gridThemeComboBox->currentText().isEmpty())
			themeBox = mainWindow_->currentView()->get(gridThemeComboBox->currentText().latin1())->getThemeBox();
	}
	else
		return;

	cLHeightValueTextLabel->setText(QString("%1").arg(themeBox.height()));
	cLWidthValueTextLabel->setText(QString("%1").arg(themeBox.width()));
	resYTextLabel->setText(QString("%1").arg(themeBox.height()/gridColNumberLineEdit->text().toFloat()));
	resXTextLabel->setText(QString("%1").arg(themeBox.width()/gridColNumberLineEdit->text().toFloat()));
}

void KernelRatioWindow::setDefaultWindow(bool hasPoint, bool hasPolygon)
{	
	//Sets up window default options.
	//setTabWidget->setTabEnabled(tab_2, false);	
	setTabWidget->setCurrentPage(0);

	regionButtonGroup->setButton(0);
	gridColNumberLineEdit->setText("50");
	gridThemeComboBox->setEnabled(false);
	gridThemeTextLabel->setEnabled(false);
	gridTableComboBox->setEnabled(false);
	gridTableTextLabel->setEnabled(false);

	set_1PointRadioButton->setChecked(true);
	set_1AreaRadioButton->setChecked(false);
	set_2PointRadioButton->setChecked(true);
	set_2AreaRadioButton->setChecked(false);
	set_1AttributeCheckBox->setChecked(false);
	set_2AttributeCheckBox->setChecked(false);
	set_1FunctionComboBox->setCurrentItem(0);
	set_2FunctionComboBox->setCurrentItem(0);

	fillCalculateCombos(set_1CalculateComboBox);	//Adicionado. Procedimento que preenche o CalculateCombo
	fillCalculateCombos(set_2CalculateComboBox);
	
	set_1AdaptiveCheckBox->setChecked(true);
	set_2AdaptiveCheckBox->setChecked(true);	
	set_1RadiusGroupBox->setEnabled(false);
	set_1RadiusSlider->setValue(0); 
	set_2RadiusSlider->setValue(0);

	string currThemeName = "";
	if(mainWindow_->currentTheme())
		currThemeName = ((TeTheme*)mainWindow_->currentTheme()->getTheme())->name();
	if(hasPoint)
	{
		gridEventRadioButton->setEnabled(true);
		set_1PointRadioButton->setEnabled(true);
		set_1PointRadioButton->setChecked(true);
		fillThemeCombo(TePOINTS, mainWindow_->currentView(), set_1ThemeComboBox, currThemeName);
		set_2PointRadioButton->setEnabled(true);
		set_2PointRadioButton->setChecked(true);
		fillThemeCombo(TePOINTS, mainWindow_->currentView(), set_2ThemeComboBox, currThemeName);
	}
	else
	{
		gridEventRadioButton->setEnabled(false);
		regionButtonGroup->setButton(1);
		gridThemeComboBox->setEnabled(true);
		gridThemeTextLabel->setEnabled(true);
		set_1PointRadioButton->setEnabled(false);
		set_1PointRadioButton->setChecked(false);
		set_1AreaRadioButton->setChecked(true);
		set_2PointRadioButton->setEnabled(false);
		set_2PointRadioButton->setChecked(false);
		set_2AreaRadioButton->setChecked(true);
	}

	if(hasPolygon) 
	{
		set_1AreaRadioButton->setEnabled(true);
		set_2AreaRadioButton->setEnabled(true);
		if (!hasPoint) 
		{
			fillThemeCombo(TePOLYGONS, mainWindow_->currentView(),set_1ThemeComboBox, currThemeName);
			fillThemeCombo(TePOLYGONS, mainWindow_->currentView(),set_2ThemeComboBox, currThemeName);
		}
		gridRegionRadioButton->setEnabled(true);
		gridNoGridRadioButton->setEnabled(true);
	}
	else
	{
		set_1AreaRadioButton->setEnabled(false);
		set_2AreaRadioButton->setEnabled(false);
		gridRegionRadioButton->setEnabled(false);
		gridNoGridRadioButton->setEnabled(false);
	}
	regionButtonGroup_clicked(regionButtonGroup->selectedId());
	set_1RadiusLineEdit->setText(QString::null);
	set_2RadiusLineEdit->setText(QString::null);
}

void KernelRatioWindow::fillCalculateCombos(QComboBox *cBox)
{
	TeProjection* view_proj = mainWindow_->currentView()->projection();
	cBox->clear();
	string currProj = view_proj->units();
	if (currProj == "Meters")
	{
		cBox->insertItem(tr("Density"));
	}
	cBox->insertItem(tr("Spatial Moving Average"));
	cBox->insertItem(tr("Probability"));
	cBox->setCurrentItem(0);
}


void KernelRatioWindow::fillColumns(int which, string prevValue) 
{
	if (which == 1) 
	{
		TeTheme* theme = mainWindow_->currentView()->get(string(set_1ThemeComboBox->currentText().ascii()));
		if (theme == NULL) 
		{
			QMessageBox::warning(this, tr("Warning"), tr("The first data set theme is not selected!"));
			return;
		}
		fillColumnCombo(TeINT, theme ,set_1ColumnComboBox, set_1TableComboBox, prevValue);
		fillColumnCombo(TeREAL, theme ,set_1ColumnComboBox, set_1TableComboBox, prevValue, false);
	}
	else 
	{
		TeTheme* theme = mainWindow_->currentView()->get(string(set_2ThemeComboBox->currentText().ascii()));
		if (theme == NULL) 
		{
			QMessageBox::warning(this, tr("Warning"), tr("The second data set theme is not selected!"));
			return;
		}
		fillColumnCombo(TeINT, theme ,set_2ColumnComboBox, set_2TableComboBox, prevValue);
		fillColumnCombo(TeREAL, theme ,set_2ColumnComboBox, set_2TableComboBox, prevValue, false);
	}
}


//Maintains the same parameters -- for this enables selection for table updates
void KernelRatioWindow::setPreviousWindow( bool hasPoint, bool hasPolygon )
{
  //TEMPORARIO, depois tem que mudar
  setDefaultWindow(hasPoint, hasPolygon);
  return;

  int button = regionButtonGroup->selectedId();

  if ((!hasPolygon) && (button != 0)) {
    setDefaultWindow(hasPoint, hasPolygon);
    return;
  }

  QString prevValue;
  //Se eh grid 0, 1, 2
  switch(button)
	{
		case 0:
		{	
			break;
		}
		case 1:
		{
			prevValue = gridThemeComboBox->currentText();
			if (prevValue.isEmpty())
				break;
			fillThemeCombo(TePOLYGONS, mainWindow_->currentView(),
                     gridThemeComboBox, string(prevValue.ascii()));
			break;
		}
		case 2:
		{
			prevValue = gridThemeComboBox->currentText();
			if (prevValue.isEmpty())
				break;
			fillThemeCombo(TePOLYGONS, mainWindow_->currentView(),
                     gridThemeComboBox,string(prevValue.ascii()));
			prevValue = gridTableComboBox->currentText();
			fillTableCombo(mainWindow_->currentView(),
			gridTableComboBox, gridThemeComboBox,string(prevValue.ascii()));
			break;
		}
    default:
      setDefaultWindow(hasPoint,hasPolygon);
      return;
      break;
	}

  //Se eh area ou ponto
	prevValue = set_1ThemeComboBox->currentText();
	if (set_1PointRadioButton->isChecked()) {
		if (!hasPoint) {
			setDefaultWindow(hasPoint, hasPolygon);
			return;
		}	
		fillThemeCombo(TePOINTS, mainWindow_->currentView(),
      set_1ThemeComboBox,  string(prevValue.ascii()));
	}
	else
	{
		if (!hasPolygon) {
			setDefaultWindow(hasPoint, hasPolygon);
			return;
		}
		fillThemeCombo(TePOLYGONS, mainWindow_->currentView(),
      set_1ThemeComboBox, string(prevValue.ascii()));
	}

  //Se tem atributo
	if (set_1AttributeCheckBox->isChecked()) {
		prevValue = set_1TableComboBox->currentText();
		fillTableCombo(mainWindow_->currentView(), set_1TableComboBox, set_1ThemeComboBox, string(prevValue.ascii()));
		prevValue = set_1ColumnComboBox->currentText();
		fillColumns(1,string(prevValue.ascii()));
	}
}


bool KernelRatioWindow::findViableThemes(bool& hasPoint, bool& hasPolygon)
{
	bool enableWindow = false;

	/* Verifica se a vista atual contém temas viáveis para o kernel e controla habilitações
	   relacionadas, inclusive da janela principal 
		IMPLEMENTADO APENAS PARA PRIMEIRO CONJUNTO. GENERALIZAR PARA OS 2 CONJUNTOS */
  	vector<TeViewNode*>& themesVec = mainWindow_->currentView()->themes();
	for (unsigned int i = 0; i < themesVec.size(); ++i)
	{
		TeAbstractTheme* theme = (TeAbstractTheme*) themesVec[i];
		if (theme->type() != TeTHEME)
			continue;

		if(theme->visibleGeoRep() & TePOINTS)
		{
			hasPoint = true;
			enableWindow = true;
		}
		if(theme->visibleGeoRep() & TePOLYGONS)
		{
			hasPolygon = true;
			enableWindow = true;
		}
	}
	return enableWindow;
}

bool KernelRatioWindow::verifyProjections()
{
	/* Verifica as projeções dos diversos conjuntos relacionados a uma execução do kernel.
	Só executa o método se todos estiverem na mesma projeção. */

	QString s = set_1ThemeComboBox->currentText();
	if (s.isEmpty())
		return false;

	TeProjection* viewProj = mainWindow_->currentView()->projection();

	TeProjection* evts1Proj = mainWindow_->currentView()->get(s.latin1())->getThemeProjection();

	s = set_2ThemeComboBox->currentText();
	if (s.isEmpty())
		return false;
	TeProjection* evts2Proj = mainWindow_->currentView()->get(s.latin1())->getThemeProjection();

	if (!(*viewProj == *evts1Proj) || !(*viewProj == *evts2Proj))
	{		
		QMessageBox::warning(this, tr("Warning"), tr("Both view and events theme should be in the same projection!"));
		return false;
	}//garante que o tema de eventos está na mesma projeção da vista.

	if(!gridEventRadioButton->isOn() )
	{
		s = gridThemeComboBox->currentText();
		if (s.isEmpty())
			return false;

		TeProjection *supportRegionProj = mainWindow_->currentView()->get(s.latin1())->getThemeProjection();
		if(!(*evts1Proj == *supportRegionProj))
		{		
			QMessageBox::warning(this, tr("Warning"), tr("Both support region and events theme should be in the same projection!"));
			return false;
		}//garante que a região de suporte está na mesma projeção dos eventos.
	}
	return true;
}

void KernelRatioWindow::init()
{
	help_ = 0;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	lastView_ = NULL;
	DisplayWindow *displayWindow = mainWindow_->getDisplayWindow();
	TeQtGrid *ptrGrid = mainWindow_->getGrid();
	connect((const QObject*)displayWindow,SIGNAL(distanceMeterSignal(TeCoord2D, TeCoord2D)),
			this, SLOT(putDistanceMeterSlot(TeCoord2D, TeCoord2D)));
	connect((const QObject*)ptrGrid,SIGNAL(gridChangedSignal()),
	        this, SLOT(updateCombosSlot()));

	if(!mainWindow_->currentDatabase()->tableExist("te_kernel_metadata"))
		createKernelMetadataTable(mainWindow_->currentDatabase());
}


bool KernelRatioWindow::loadView(bool def)
{
	if(mainWindow_->currentView() == 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("There is no active view!"));
		return false;
	}

	bool hasPoints = false;
	bool hasPolygons = false;

	//Opens KernelRatioWindow only if there´s at least 1 viable theme.
	//(A viable theme is a theme with an associated geometry)
	if(!findViableThemes(hasPoints, hasPolygons)) {
		QMessageBox::warning(this, tr("Warning"),
		tr("The active view does not contain any themes with an associated geometry!"));
		return false;
	}

	//If it is first time, set default. Else do not change options -- may change themes...  
	if (def) 
	{
		setDefaultWindow(hasPoints, hasPolygons);
	}
	else 
	{
		if (lastView_ != mainWindow_->currentView()) 
		{
			setDefaultWindow(hasPoints, hasPolygons);
			lastView_ = mainWindow_->currentView();
		}
		else 
		{
			setPreviousWindow(hasPoints, hasPolygons);
		}
	}
	fillInformationGroup();
	return true;
}


void KernelRatioWindow::regionButtonGroup_clicked( int button )
{
	switch(button)
	{
		case 0:
		{	
			//disable theme selection
			gridThemeTextLabel->setEnabled(false);
			gridThemeComboBox->setEnabled(false);		
			gridThemeComboBox->clear();
			//disable table selection
			gridTableTextLabel->setEnabled(false);	
			gridTableComboBox->setEnabled(false);
			gridTableComboBox->clear();
			//enable column number selection
			gridColNumberLineEdit->setEnabled(true);
			gridColNumberTextLabel->setEnabled(true);
			gridDimGroupBox->setEnabled(true);
			resultsTextLabel->setText(tr("Layer name:"));
			break;
		}
		case 1:
		{
			//enable theme selection
			fillThemeCombo(TePOLYGONS, mainWindow_->currentView(), gridThemeComboBox,"");
			gridThemeTextLabel->setEnabled(true);
			gridThemeComboBox->setEnabled(true);
			//disable table selection
			gridTableTextLabel->setEnabled(false);	
			gridTableComboBox->setEnabled(false);
			gridTableComboBox->clear();
			//enable column number selection
			gridColNumberLineEdit->setEnabled(true);
			gridColNumberTextLabel->setEnabled(true);
			gridDimGroupBox->setEnabled(true);
			resultsTextLabel->setText(tr("Layer name:"));
			break;
		}
		case 2:
		{
			//enable theme selection
			fillThemeCombo(TePOLYGONS, mainWindow_->currentView(), gridThemeComboBox,""); 
			gridThemeTextLabel->setEnabled(true);
			gridThemeComboBox->setEnabled(true);
			//enable table selection
			fillTableCombo(mainWindow_->currentView(), gridTableComboBox, gridThemeComboBox, "");
			gridTableComboBox->setEnabled(true);
			gridTableTextLabel->setEnabled(true);
			//disable column number selection
			gridColNumberLineEdit->setEnabled(false);
			gridColNumberTextLabel->setEnabled(false);
			gridDimGroupBox->setEnabled(false);
			resultsTextLabel->setText(tr("Column name:"));
			break;
		}
	}
	fillInformationGroup();
}

void KernelRatioWindow::defaultPushButton_clicked()
{
	bool hasPoints, hasPolygons;

//Opens KernelRatioWindow only if there´s at least 1 viable theme.
//(A viable theme is a theme with an associated geometry)
	if(findViableThemes(hasPoints, hasPolygons))
		setDefaultWindow(hasPoints, hasPolygons);
}

void KernelRatioWindow::set_1AttributeCheckBox_toggled( bool )
{
  if(set_1AttributeCheckBox->isChecked())
  {
    set_1TableGroupBox->setEnabled(true);
    fillTableCombo(mainWindow_->currentView(), set_1TableComboBox, set_1ThemeComboBox, "");
    fillColumns(1,"");
  }
  else
  {
    set_1TableGroupBox->setEnabled(false);
    set_1TableComboBox->clear();
    set_1ColumnComboBox->clear();
  }
}

void KernelRatioWindow::set_1AdaptiveCheckBox_toggled( bool )
{
  if(set_1AdaptiveCheckBox->isChecked() == false)
  {
    set_1RadiusGroupBox->setEnabled(true);
    set_1RadiusSlider->setValue(100);
    setRadiusForSlider(100);
  }
  else
  {
    set_1RadiusGroupBox->setEnabled(false);
    set_1RadiusSlider->setValue(0);
    set_1RadiusLineEdit->setText(QString::null);
  }
  
}

void KernelRatioWindow::set_2AttributeCheckBox_toggled( bool )
{
  if(set_2AttributeCheckBox->isChecked())
  {
    set_2TableGroupBox->setEnabled(true);
    fillTableCombo(mainWindow_->currentView(), set_2TableComboBox, set_2ThemeComboBox, "");
    fillColumns(2, "");
  }
  else
  {
    set_2TableGroupBox->setEnabled(false);
    set_2TableComboBox->clear();
    set_2ColumnComboBox->clear();
  }
}


void KernelRatioWindow::set_2AdaptiveCheckBox_toggled( bool )
{
  if(set_2AdaptiveCheckBox->isChecked() == false)
  {
    set_2RadiusGroupBox->setEnabled(true);
    set_2RadiusSlider->setValue(100);
    setRadiusForSlider(100);
  }
  else
  {
    set_2RadiusGroupBox->setEnabled(false);
    set_2RadiusSlider->setValue(0);
    set_2RadiusLineEdit->setText(QString::null);
  }
}


void KernelRatioWindow::putDistanceMeterSlot(TeCoord2D pa, TeCoord2D pb)
{
	if (!this->isVisible())
		return; 
	DisplayWindow *displayWindow = mainWindow_->getDisplayWindow();
	TeQtCanvas *canvas = displayWindow->getCanvas();
	TeProjection *cproj = canvas->projection();

	//COLOCAR EM QUAL TEMA QUE VAI SELECIONAR--- Vai selecionar eh o tema de eventos

	TeTheme* theme = mainWindow_->currentView()->get(string(set_1ThemeComboBox->currentText().ascii()));
	TeProjection *dproj = theme->getThemeProjection();

	if(cproj && dproj && !(*cproj == *dproj))
	{
		cproj->setDestinationProjection(dproj);
		pa = cproj->PC2LL (pa);
		pa = dproj->LL2PC (pa);
		pb = cproj->PC2LL (pb);
		pb = dproj->LL2PC (pb);
	}
	double radius = sqrt((pa.x()-pb.x()) * (pa.x()-pb.x()) + (pa.y()-pb.y()) * (pa.y()-pb.y()));
	string s = Te2String(radius);
	if(setTabWidget->currentPageIndex() == 0)
	{
		if(set_1AdaptiveCheckBox->isChecked() == false)
		{
			set_1RadiusLineEdit->setText(s.c_str());
			setSliderForRadius(set_1RadiusLineEdit->text().toDouble());
		}
	}
	else
	{
		if(set_2AdaptiveCheckBox->isChecked() == false)
		{
			set_2RadiusLineEdit->setText(s.c_str());
			setSliderForRadius(set_1RadiusLineEdit->text().toDouble());
		}
	}
}


void KernelRatioWindow::set_2PointRadioButton_clicked()
{
	if(set_2PointRadioButton->isChecked())
	{
		set_2PointRadioButton->setChecked(true);
		set_2AreaRadioButton->setChecked(false);
		fillThemeCombo( TePOINTS, mainWindow_->currentView(), set_2ThemeComboBox, "");
	}
	else
	{
		set_2PointRadioButton->setChecked(false);
		set_2AreaRadioButton->setChecked(true);
		fillThemeCombo(TePOLYGONS, mainWindow_->currentView(), set_2ThemeComboBox, "");
	}
	if(set_2AttributeCheckBox->isChecked())
		fillTableCombo(mainWindow_->currentView(), set_2TableComboBox, set_2ThemeComboBox, "");
}

void KernelRatioWindow::set_2AreaRadioButton_clicked()
{
	if(set_2AreaRadioButton->isChecked())
	{
		set_2PointRadioButton->setChecked(false);
		set_2AreaRadioButton->setChecked(true);
		fillThemeCombo(TePOLYGONS, mainWindow_->currentView(), set_2ThemeComboBox, "");
	}
	else
	{
		set_2PointRadioButton->setChecked(true);
		set_2AreaRadioButton->setChecked(false);
		fillThemeCombo(TePOINTS, mainWindow_->currentView(), set_2ThemeComboBox, "");
	}

	if(set_2AttributeCheckBox->isChecked())
		fillTableCombo(mainWindow_->currentView(),set_2TableComboBox, set_2ThemeComboBox, "");
}

void KernelRatioWindow::set_1PointRadioButton_clicked()
{
	if(set_1PointRadioButton->isChecked())
	{
		set_1PointRadioButton->setChecked(true);
		set_1AreaRadioButton->setChecked(false);
		fillThemeCombo(TePOINTS, mainWindow_->currentView(), set_1ThemeComboBox, "");
	}
	else
	{
		if(set_1AreaRadioButton->isEnabled())
		{    
			set_1PointRadioButton->setChecked(false);
			set_1AreaRadioButton->setChecked(true);
			fillThemeCombo(TePOLYGONS, mainWindow_->currentView(), set_1ThemeComboBox, "");
		}
		else
		{
			set_1PointRadioButton->setChecked(true);
			return;
		}
	}
	if(set_1AttributeCheckBox->isChecked())
	{
		fillTableCombo(mainWindow_->currentView(), set_1TableComboBox, set_1ThemeComboBox, "");
		fillColumns(1,"");
	}
	fillInformationGroup();
}


void KernelRatioWindow::set_1AreaRadioButton_clicked()
{
	if(set_1AreaRadioButton->isChecked())
	{
		set_1PointRadioButton->setChecked(false);
		set_1AreaRadioButton->setChecked(true);
		fillThemeCombo(TePOLYGONS, mainWindow_->currentView(), set_1ThemeComboBox,  "");
	}
	else
	{
		if(set_1PointRadioButton->isEnabled())
		{
			set_1PointRadioButton->setChecked(true);
			set_1AreaRadioButton->setChecked(false);
			fillThemeCombo(TePOINTS, mainWindow_->currentView(),set_1ThemeComboBox, "");
		}
		else
		{
			set_1AreaRadioButton->setChecked(true);
			return;
		}
	}
	if(set_1AttributeCheckBox->isChecked())
	{
		fillTableCombo(mainWindow_->currentView(),set_1TableComboBox, set_1ThemeComboBox, "");
		fillColumns(1, "");
	}
	fillInformationGroup();
}

void KernelRatioWindow::setTabWidget_currentChanged( QWidget * )
{
	fillInformationGroup();
}

void KernelRatioWindow::set_1TableComboBox_activated( const QString & )
{
	fillColumns(1, "");
}


void KernelRatioWindow::set_2TableComboBox_activated( const QString & )
{
	fillColumns(2,  "");
}

void KernelRatioWindow::setRadiusForSlider( int pos )
{
	//Changes value of Radius boxes to meet Slider position
	double maxDif = max(cLHeightValueTextLabel->text().toDouble(),
	cLWidthValueTextLabel->text().toDouble());

	string text = Te2String(pos*maxDif/1000);
	if(setTabWidget->currentPageIndex() == 0)
		set_1RadiusLineEdit->setText(text.c_str());
	else
		set_2RadiusLineEdit->setText(text.c_str());
}


void KernelRatioWindow::setSliderForRadius( double radius )
{
	//Moves Slider to meet Radius value
	double maxDif = max(cLHeightValueTextLabel->text().toDouble(), 
	cLWidthValueTextLabel->text().toDouble());

	if(setTabWidget->currentPageIndex() == 0)
		set_1RadiusSlider->setValue((int)(radius*1000/maxDif));
	else
		set_2RadiusSlider->setValue((int)(radius*1000/maxDif));
}

void KernelRatioWindow::set_1RadiusSlider_valueChanged( int value)
{
	setRadiusForSlider(value);
}

void KernelRatioWindow::set_1RadiusLineEdit_returnPressed()
{
	setSliderForRadius(set_1RadiusLineEdit->text().toDouble());
}

void KernelRatioWindow::set_2RadiusLineEdit_returnPressed()
{
	setSliderForRadius(set_2RadiusLineEdit->text().toDouble());
}

void KernelRatioWindow::set_2RadiusSlider_valueChanged( int value )
{
	setRadiusForSlider(value);
}

void KernelRatioWindow::updateCombosSlot()
{
	if (!this->isVisible())
		return;
	if(set_1PointRadioButton->isChecked())
		fillThemeCombo(TePOINTS, mainWindow_->currentView(), set_1ThemeComboBox,  "");
	else
		fillThemeCombo(TePOLYGONS,mainWindow_->currentView(),set_1ThemeComboBox,  "");

	if(set_2PointRadioButton->isChecked())
		fillThemeCombo(TePOINTS, mainWindow_->currentView(),set_2ThemeComboBox,  "");
	else
		fillThemeCombo(TePOLYGONS, mainWindow_->currentView(),set_2ThemeComboBox,  "");

	if(set_1TableGroupBox->isEnabled())
	{
		fillTableCombo(mainWindow_->currentView(),set_1TableComboBox, set_1ThemeComboBox, "");
		fillColumns(1, "");
	}

	if(set_2TableGroupBox->isEnabled())
	{
		fillTableCombo(mainWindow_->currentView(),set_2TableComboBox, set_2ThemeComboBox, "");
		fillColumns(2,"");
	}	

	fillInformationGroup();
}

void KernelRatioWindow::set_1ThemeComboBox_activated( const QString & )
{
	if(set_1AttributeCheckBox->isChecked())
		fillTableCombo(mainWindow_->currentView(),set_1TableComboBox, set_1ThemeComboBox, "");
	fillInformationGroup();
}


void KernelRatioWindow::set_2ThemeComboBox_activated( const QString & )
{
	if(set_2AttributeCheckBox->isChecked())
		fillTableCombo(mainWindow_->currentView(),set_2TableComboBox, set_2ThemeComboBox, "");
	fillInformationGroup();
}


void KernelRatioWindow::gridThemeComboBox_activated( const QString & )
{
	fillTableCombo(mainWindow_->currentView(),gridTableComboBox, gridThemeComboBox, "");
	if (regionButtonGroup->selectedId() >= 1) //grid over regions or no grid
	{
		QString gridtheme = gridThemeComboBox->currentText();
		if (!gridtheme.isEmpty())
		{
			TeTheme* theme = mainWindow_->currentView()->get(gridtheme.latin1());
			TeBox& themeBox = theme->getThemeBox();
			cLHeightValueTextLabel->setText(QString("%1").arg(themeBox.height()));
			cLWidthValueTextLabel->setText(QString("%1").arg(themeBox.width()));
			resYTextLabel->setText(QString("%1").arg(themeBox.height()/gridColNumberLineEdit->text().toFloat()));
			resXTextLabel->setText(QString("%1").arg(themeBox.width()/gridColNumberLineEdit->text().toFloat()));
		}
	}
}


/***************************************************************
** Execucao do Kernel
***************************************************************/
void KernelRatioWindow::executeKernel()
{
	//Recupera parametros da interface e faz consistencia
	TeKernelMethod meth;

	QString dummy; //Used to process text
	meth.db_ = mainWindow_->currentDatabase();

	if(verifyProjections() == false)
		return;

  /***********************************************************************/
  /*** Inicializa parametros                                           ***/

	//First Event Set
	dummy = set_1ThemeComboBox->currentText();
	if (dummy.isNull() || dummy.isEmpty() ) {
		QMessageBox::warning(this, tr("Warning"), tr("Select a theme for the event set 1!"));
		return;
	}
	if (!meth.setTheme(string(dummy.ascii()), 1, mainWindow_->currentView())) {
		QMessageBox::critical(this, tr("Error"), tr("Unexpected error! Contact support."));
		return;
	}

	meth.isPoint1_ =  set_1PointRadioButton->isChecked();

	//If do not have attribute --> table name is empty
	if (set_1AttributeCheckBox->isChecked()) {
		dummy = set_1TableComboBox->currentText();
		if (dummy.isNull() || dummy.isEmpty() ) {
			QMessageBox::warning(this, tr("Warning"), tr("Select a table for the attribute 1!"));
			return;
		}
		meth.params_.intensityAttrTable1_ = string(dummy.ascii());

		dummy = set_1ColumnComboBox->currentText();
		if (dummy.isNull() || dummy.isEmpty() ) {
			QMessageBox::warning(this, tr("Warning"), tr("Select a column for the attribute 1!"));
			return;
		}
		meth.params_.intensityAttrName1_ = string(dummy.ascii());
	}
	else {
		meth.params_.intensityAttrTable1_ = "";
		meth.params_.intensityAttrName1_ = "";
	}
  
	if (set_1FunctionComboBox->currentItem() < 0) {
		QMessageBox::warning(this, tr("Warning"), tr("Select a kernel function 1!"));
		return;
	}
	meth.params_.kernelFunction1_ = (TeKernelFunctionType)set_1FunctionComboBox->currentItem();

	if (set_1CalculateComboBox->currentItem() < 0) {
	    QMessageBox::warning(this, tr("Warning"), tr("Select a kernel function 1!"));
		return;
	}
	meth.params_.computeType1_ = (TeKernelComputeType)set_1CalculateComboBox->currentItem();

	//If is adaptive, radius = 0
	if (set_1AdaptiveCheckBox->isChecked()) {
		meth.params_.radiusValue1_ = 0.0;
	}
	else {
	    dummy =set_1RadiusLineEdit->text();
		if (dummy.isNull() || dummy.isEmpty() ) {
			QMessageBox::warning(this, tr("Warning"), tr("Select a radius 1!"));
			return;
		}
		meth.params_.radiusValue1_ = atof(dummy.ascii());
		if (meth.params_.radiusValue1_ <= 0) {
			QMessageBox::warning(this, tr("Warning"), tr("Select a valid radius 1!"));
			return;
		}
	}

	meth.params_.combinationType_ = TeKRatio; //DEPOIS TEM QUE ALTERAR!!!!
    
	//Second Event Set
	dummy = set_2ThemeComboBox->currentText();
	if (dummy.isNull() || dummy.isEmpty() ) {
		QMessageBox::warning(this, tr("Warning"), tr("Select a theme for the event set 2!"));
		return;
	}
	if (!meth.setTheme(string(dummy.ascii()), 2, mainWindow_->currentView())) {
		QMessageBox::critical(this, tr("Error"), tr("Unexpected error! Contact support."));
		return;
	}
 
	meth.isPoint2_ =  set_2PointRadioButton->isChecked();
	//If do not have attribute --> table name is empty
 
	if (set_2AttributeCheckBox->isChecked()) {
		dummy = set_2TableComboBox->currentText();
		if (dummy.isNull() || dummy.isEmpty() ) {
			QMessageBox::warning(this, tr("Warning"), tr("Select a table for the attribute 2!"));
			return;
		}
		meth.params_.intensityAttrTable2_ = string(dummy.ascii());
   
		dummy = set_2ColumnComboBox->currentText();
		if (dummy.isNull() || dummy.isEmpty() ) {
			QMessageBox::warning(this, tr("Warning"), tr("Select a column for the attribute 2!"));
			return;
		}
		meth.params_.intensityAttrName2_ = string(dummy.ascii());
   
	}
	else {
		meth.params_.intensityAttrTable2_ = "";
		meth.params_.intensityAttrName2_ = "";
	}
 
	if (set_2FunctionComboBox->currentItem() < 0) {
		QMessageBox::warning(this, tr("Warning"), tr("Select a kernel function 2!"));
		return;
	}
	meth.params_.kernelFunction2_ = (TeKernelFunctionType)set_2FunctionComboBox->currentItem();
  
	if (set_2CalculateComboBox->currentItem() < 0) {
		QMessageBox::warning(this, tr("Warning"), tr("Select a kernel function 2!"));
		return;
	}
	meth.params_.computeType2_ = (TeKernelComputeType)set_2CalculateComboBox->currentItem();
 
	 //If is adaptive, radius = 0
 
	if (set_2AdaptiveCheckBox->isChecked()) {
		meth.params_.radiusValue2_ = 0.0;
	}
	else {
		dummy =set_2RadiusLineEdit->text();
		if (dummy.isNull() || dummy.isEmpty() ) {
			QMessageBox::warning(this, tr("Warning"), tr("Select a radius 2!"));
			return;
		}
		meth.params_.radiusValue2_ = atof(dummy.ascii());
		if (meth.params_.radiusValue2_ <= 0) {
			QMessageBox::warning(this, tr("Warning"), tr("Select a valid radius 2!"));
			return;
		}
	}
 
  /////////////////////////////////////////////////////////////
  //Support regions definition
	meth.isGridEv_= gridEventRadioButton->isChecked();
	meth.isGridReg_ = gridRegionRadioButton->isChecked();

  //Grid from events
	if ((meth.isGridEv_) || (meth.isGridReg_)) 
	{
		dummy = gridColNumberLineEdit->text();
		meth.gridCols_ = atoi(dummy.ascii());
		if (meth.gridCols_ <= 0) {
			QMessageBox::warning(this, tr("Warning"), 
			tr("Select a valid number of columns!"));
			return;
		}

		dummy = gridResultLineEdit->text();
		if (dummy.isNull() || dummy.isEmpty()) {
			QMessageBox::warning(this, tr("Warning"), 
			tr("Select the layer name!"));
			return;
		}

		//Testando se nome eh igual ao de algum dos temas utilizados
		if (meth.isGridReg_) {
			if (!dummy.compare(gridThemeComboBox->currentText())) {
				QMessageBox::warning(this, tr("Warning"), 
				tr("The layer name cannot have the same name of the region theme!"));
				return;
			}
		}

		if (!dummy.compare(set_1ThemeComboBox->currentText())) {
			QMessageBox::warning(this, tr("Warning"), 
			tr("The layer name cannot have the same name of the event theme!"));
			return;
		}
    
		if (!meth.setGridLayer(string(dummy.ascii()))) 
			return;

		if (!meth.isNewLayer()) {
			int response = QMessageBox::question(this, tr("Grid Layer"),
				tr("Do you really want to update the existing layer or theme?"),
				tr("Yes"), tr("No"));
			if (response != 0)
				return;
		}
	}
  
	if (!meth.isGridEv_) {
	    //has an associated theme
		dummy = gridThemeComboBox->currentText();
		if (dummy.isNull() || dummy.isEmpty()) {
			QMessageBox::warning(this, tr("Warning"), tr("Select a theme for support regions!"));
			return;
		}
		meth.setTheme(string(dummy.ascii()), 0, mainWindow_->currentView());

		//Irregular grid from polygons
		if (!meth.isGridReg_) {
			dummy = gridTableComboBox->currentText();
			if (dummy.isNull() || dummy.isEmpty()) {
				QMessageBox::warning(this, tr("Warning"), tr("Select an attribute table to store kernel values!"));
				return;
			}	
			meth.params_.supportThemeTable_ = string(dummy.ascii());

			dummy = gridResultLineEdit->text();
			if (dummy.isNull() || dummy.isEmpty()) {
				QMessageBox::warning(this, tr("Warning"), tr("Select a column to store kernel values!"));
				return;
			}	
			meth.params_.supportThemeAttr_ = string(dummy.ascii());
		}
	}
  
	/*****************************************************************************/
	/* Execucao do Kernel                                                        */
	/*****************************************************************************/
	void* supportSet=0;

	// TeWaitCursor wait;
	if (TeProgress::instance())
	{
		QString label = tr("Kernel");
		QString msg = tr("Calculating kernel. Please wait...");
		TeProgress::instance()->setCaption(label.latin1());
		TeProgress::instance()->setMessage(msg.latin1());
	}

	okPushButton->setEnabled(false);
	bool res = meth.apply(supportSet, this);
	
	if (TeProgress::instance())
		TeProgress::instance()->reset();

	if (!res) {
		QMessageBox::critical(this, tr("Error"), tr("Fail to evaluate the kernel function!"));
		okPushButton->setEnabled(true);
		return;
	}

	if (TeProgress::instance())
	{
		TeProgress::instance()->setTotalSteps(3);
		TeProgress::instance()->setMessage(tr("Storing Kernel values...").latin1());
		TeProgress::instance()->setProgress(1);
	}

	/*****************************************************************************/
	/* Armazena valores no BD                                                    */
	/*****************************************************************************/
	TeAppTheme* tempTheme = mainWindow_->currentTheme();
	TeTheme* kernelTheme;
	if (!((meth.isGridEv_) || (meth.isGridReg_))) //Se for por poligonos, precisa armazenar a coluna
	{
	  	if (!meth.createColumn(tempTheme, mainWindow_->getGrid())) {
			QMessageBox::critical(this, tr("Error"), 
			tr("Fail to store the column!"));
			okPushButton->setEnabled(true);
			return;
		}
		kernelTheme = meth.regTheme();
	}
	//se for raster, deve criar um layer novo e um tema com o resultado 
	else 
	{
		TeRaster* raster = ((TeKernelGridSupport *)supportSet)->raster();
		if (!meth.createLayer(raster, mainWindow_)) 
		{
			QMessageBox::critical(this, tr("Error"), 
			tr("Fail to create the layer!"));
			okPushButton->setEnabled(true);
			return;
		}
		kernelTheme = meth.rasterTheme();
	}

	/*****************************************************************************/
	/* Armazena parametros do kernel                                             */
	/*****************************************************************************/
	if (!updateKernelMetadata(meth.db_ ,meth.params_) ) 
	{
		QMessageBox::critical(this, tr("Error"), 
		tr("Fail to store the kernel parameters!"));
		okPushButton->setEnabled(true);
		return;
	}

	/*****************************************************************************/
	/* Agrupa tema de com resultado do kernel                                    */
	/*****************************************************************************/
	if (TeProgress::instance())
	{
		TeProgress::instance()->setTotalSteps(3);
		TeProgress::instance()->setMessage(tr("Building grouping...").latin1());
		TeProgress::instance()->setProgress(2);
	}

	TeQtThemeItem* themeItem = mainWindow_->getViewsListView()->getThemeItem(kernelTheme->id());

	if(themeItem == NULL)
	{
		return;
	}

	if (!meth.createKernelLegends( 10, true, themeItem->getAppTheme()))
	{
		QMessageBox::critical(this, tr("Error"), 
		tr("Fail to generate the display! The results are available."));
		okPushButton->setEnabled(true);
		return;
	}
	okPushButton->setEnabled(true);
	if (TeProgress::instance())
		TeProgress::instance()->reset();
   
    // update the user interface
	mainWindow_->updateThemeItemInterface(themeItem);
	if (themeItem)
	{
		TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->getViewItem(mainWindow_->currentView());
		curViewItem->resetAll();
		mainWindow_->getViewsListView()->setOn((QCheckListItem*)themeItem,true);
		mainWindow_->getViewsListView()->selectItem((QListViewItem*)themeItem);
   
	//show grid
		mainWindow_->getGrid()->clear();
		if(!((meth.isGridEv_) || (meth.isGridReg_)))
		{
			mainWindow_->getGrid()->init(themeItem->getAppTheme());
  			mainWindow_->getGrid()->refresh();
		}
		mainWindow_->drawAction_activated();
	}

	close();  
}

void KernelRatioWindow::closeEvent( QCloseEvent *)
{
	mainWindow_->kernelEnableControl(true);
	hide();
}

void KernelRatioWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("kernelRatioWindow.htm");
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


void KernelRatioWindow::gridColNumberLineEdit_textChanged( const QString & txt)
{
	if (txt.isEmpty())
	{
		resYTextLabel->clear();
		resXTextLabel->clear();
	}
	else
	{
		resYTextLabel->setText(QString("%1").arg(cLHeightValueTextLabel->text().toFloat()/txt.toFloat()));
		resXTextLabel->setText(QString("%1").arg(cLWidthValueTextLabel->text().toFloat()/txt.toFloat()));
	}
}

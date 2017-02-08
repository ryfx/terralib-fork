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

void KernelWindow::fillInformationGroup()
{
	// Fill information about the total number of events
	if (set_1ThemeComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("There is no data set theme!"));
		return;
	}

	TeTheme* dataSetTheme = mainWindow_->currentView()->get(set_1ThemeComboBox->currentText().latin1());
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


void KernelWindow::setDefaultWindow(bool hasPoint, bool hasPolygon)
{	
	regionButtonGroup->setButton(0);
	gridColNumberLineEdit->setText("50");
	gridThemeComboBox->setEnabled(false);
	gridThemeTextLabel->setEnabled(false);
	gridTableComboBox->setEnabled(false);
	gridTableTextLabel->setEnabled(false);

	set_1PointRadioButton->setChecked(true);
	set_1AreaRadioButton->setChecked(false);
	set_1AttributeCheckBox->setChecked(false);
	set_1FunctionComboBox->setCurrentItem(0);

	fillCalculateCombos(set_1CalculateComboBox);	

	set_1AdaptiveCheckBox->setChecked(true);
	set_1RadiusGroupBox->setEnabled(false);
	set_1RadiusSlider->setValue(0); 

	string currThemeName = "";
	if(mainWindow_->currentTheme())
		currThemeName = ((TeTheme*)mainWindow_->currentTheme()->getTheme())->name();
	if(hasPoint)
	{
		gridEventRadioButton->setEnabled(true);
		set_1PointRadioButton->setEnabled(true);
		set_1PointRadioButton->setChecked(true);
		fillThemeCombo(TePOINTS, mainWindow_->currentView(), set_1ThemeComboBox, currThemeName);
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
	}

	gridThemeComboBox->clear();
	if(hasPolygon) 
	{
		fillThemeCombo(TePOLYGONS, mainWindow_->currentView(), gridThemeComboBox,"");
		if (!hasPoint)
			fillThemeCombo(TePOLYGONS, mainWindow_->currentView(),set_1ThemeComboBox, currThemeName);
		set_1AreaRadioButton->setEnabled(true);
		gridRegionRadioButton->setEnabled(true);
		gridNoGridRadioButton->setEnabled(true);
	}
	else
	{
		set_1AreaRadioButton->setEnabled(false);
		gridRegionRadioButton->setEnabled(false);
		gridNoGridRadioButton->setEnabled(false);
	}
	regionButtonGroup_clicked(regionButtonGroup->selectedId());
	set_1RadiusLineEdit->setText(QString::null);
}

void KernelWindow::fillCalculateCombos(QComboBox *cBox)
{
	//TeProjection* view_proj = mainWindow_->currentView()->projection();
	cBox->clear();
	
	// "Density" is an option only projection has a Meters units
	//if (view_proj->units()== "Meters")
	//	cBox->insertItem(tr("Density"));

	cBox->insertItem(tr("Density"));
	cBox->insertItem(tr("Spatial Moving Average"));
	cBox->insertItem(tr("Probability"));
	cBox->setCurrentItem(0);
}

void KernelWindow::fillColumns(int which, string prevValue) 
{
	if (which != 1)
		return;

	TeTheme* theme = mainWindow_->currentView()->get(string(set_1ThemeComboBox->currentText().ascii()));
	if (theme == NULL) 
		return;
	fillColumnCombo(TeINT, theme ,set_1ColumnComboBox, set_1TableComboBox, prevValue);
	fillColumnCombo(TeREAL, theme ,set_1ColumnComboBox, set_1TableComboBox, prevValue, false);
}


//Maintains the same parameters -- for this enables selection for table updates
void KernelWindow::setPreviousWindow( bool hasPoint, bool hasPolygon )
{
	int button = regionButtonGroup->selectedId();
	if (!hasPolygon && (button != 0)) 
	{
		setDefaultWindow(hasPoint, hasPolygon);
		return;
	}

	QString prevValue;
	switch(button) {
	case 0:
		break;
	case 1:
		prevValue = gridThemeComboBox->currentText();
		if (prevValue.isEmpty())
			break;
		fillThemeCombo(TePOLYGONS, mainWindow_->currentView(),gridThemeComboBox, string(prevValue.ascii()));
		break;
	case 2:
		prevValue = gridThemeComboBox->currentText();
		if (prevValue.isEmpty())
			break;
		fillThemeCombo(TePOLYGONS, mainWindow_->currentView(),
		gridThemeComboBox,string(prevValue.ascii()));
		prevValue = gridTableComboBox->currentText();
		fillTableCombo(mainWindow_->currentView(),gridTableComboBox, gridThemeComboBox,string(prevValue.ascii()));
		break;
	default:
		setDefaultWindow(hasPoint,hasPolygon);
		return;
	}

	//Se eh area ou ponto
	prevValue = set_1ThemeComboBox->currentText();
	if (set_1PointRadioButton->isChecked()) 
	{
		if (!hasPoint) 
		{
			setDefaultWindow(hasPoint, hasPolygon);
			return;
		}	
		fillThemeCombo(TePOINTS, mainWindow_->currentView(),
		set_1ThemeComboBox,  string(prevValue.ascii()));
	}
	else
	{
		if (!hasPolygon) 
		{
			setDefaultWindow(hasPoint, hasPolygon);
			return;
		}
		fillThemeCombo(TePOLYGONS, mainWindow_->currentView(),
		set_1ThemeComboBox, string(prevValue.ascii()));
	}

	//Se tem atributo
	if (set_1AttributeCheckBox->isChecked()) 
	{
		prevValue = set_1TableComboBox->currentText();
		fillTableCombo(mainWindow_->currentView(), set_1TableComboBox, set_1ThemeComboBox, string(prevValue.ascii()));
		prevValue = set_1ColumnComboBox->currentText();
		fillColumns(1,string(prevValue.ascii()));
	}
}

bool KernelWindow::findViableThemes(bool& hasPoint, bool& hasPolygon)
{
	bool enableWindow = false;
	vector<TeViewNode*>& themesVec = mainWindow_->currentView()->themes();
	for (unsigned int i = 0; i < themesVec.size(); ++i)
	{
		if (themesVec[i]->type() != TeTHEME)
			continue;

		TeAbstractTheme* theme = (TeAbstractTheme*) themesVec[i];
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

bool KernelWindow::verifyProjections()
{
	// Check if the original projections of the themes are the same
	QString s = set_1ThemeComboBox->currentText();
	if (s.isEmpty())
		return false;
	
	TeTheme* currTheme = mainWindow_->currentView()->get(s.latin1());
	// Check if the event theme has the same projection and view
	TeProjection* viewProj = mainWindow_->currentView()->projection();
	TeProjection* evtsProj = currTheme->getThemeProjection();
	if(!(*viewProj == *evtsProj))
	{		
		QMessageBox::warning(this, tr("Warning"), tr("Both view and events theme should be in the same projection!"));
		return false;
	}

	// Check if the event theme and the support theme has the same projection
	if (!gridEventRadioButton->isOn())
	{
		s = gridThemeComboBox->currentText();
		if (s.isEmpty())
			return false;
		currTheme = mainWindow_->currentView()->get(s.latin1());
		TeProjection *supportRegionProj = currTheme->getThemeProjection();
		if(!(*evtsProj == *supportRegionProj))
		{		
			QMessageBox::warning(this, tr("Warning"), tr("Both support region and events theme should be in the same projection!"));
			return false;
		}
	}
	return true;
}

void KernelWindow::init()
{
	help_ = 0;
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	lastView_ = NULL;
	DisplayWindow *displayWindow = mainWindow_->getDisplayWindow();
	TeQtGrid *ptrGrid = mainWindow_->getGrid();
	connect((const QObject*)displayWindow,SIGNAL(distanceMeterSignal(TeCoord2D, TeCoord2D)),this, SLOT(putDistanceMeterSlot(TeCoord2D, TeCoord2D)));
	connect((const QObject*)ptrGrid,SIGNAL(gridChangedSignal()), this, SLOT(updateCombosSlot()));

	if(!mainWindow_->currentDatabase()->tableExist("te_kernel_metadata"))
		createKernelMetadataTable(mainWindow_->currentDatabase());
}

bool KernelWindow::loadView(bool def)
{
	if(mainWindow_->currentView() == 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("There is no active view!"));
		return false;
	}

	bool hasPoints = false;
	bool hasPolygons = false;

	//Opens kernelWindow only if there´s at least 1 viable theme.
	//(A viable theme is a theme with an associated geometry)
	if(!findViableThemes(hasPoints, hasPolygons)) 
	{
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
	return true;
}

void KernelWindow::regionButtonGroup_clicked( int button )
{
	switch(button)
	{
		case 0:
		{	
			//disable theme selection
			gridThemeTextLabel->setEnabled(false);
			gridThemeComboBox->setEnabled(false);		
			//disable table selection
			gridTableTextLabel->setEnabled(false);	
			gridTableComboBox->setEnabled(false);
			gridTableComboBox->clear();
			//enable column number selection
			gridDimGroupBox->setEnabled(true);
			resultsTextLabel->setText(tr("Layer name:"));
			break;
		}
		case 1:
		{
			//enable theme selection
			gridThemeTextLabel->setEnabled(true);
			gridThemeComboBox->setEnabled(true);
			//disable table selection
			gridTableTextLabel->setEnabled(false);	
			gridTableComboBox->setEnabled(false);
			gridTableComboBox->clear();
			//enable column number selection
			gridDimGroupBox->setEnabled(true);
			resultsTextLabel->setText(tr("Layer name:"));
			break;
		}
		case 2:	// no grid
		{
			//enable theme selection
			gridThemeTextLabel->setEnabled(true);
			gridThemeComboBox->setEnabled(true);
			//enable table selection
			fillTableCombo(mainWindow_->currentView(), gridTableComboBox, gridThemeComboBox, "");
			gridTableComboBox->setEnabled(true);
			gridTableTextLabel->setEnabled(true);
			gridDimGroupBox->setEnabled(false);
			resultsTextLabel->setText(tr("Column name:"));
			break;

		}
	}
	fillInformationGroup();
}



void KernelWindow::defaultPushButton_clicked()
{
	bool hasPoints = false, hasPolygons = false;

 //Opens kernelWindow only if there´s at least 1 viable theme.
//(A viable theme is a theme with an associated geometry)
	if(findViableThemes(hasPoints, hasPolygons))
		setDefaultWindow(hasPoints, hasPolygons);
}

void KernelWindow::set_1AttributeCheckBox_toggled( bool )
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


void KernelWindow::set_1AdaptiveCheckBox_toggled( bool )
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

void KernelWindow::putDistanceMeterSlot(TeCoord2D pa, TeCoord2D pb)
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
	if(set_1AdaptiveCheckBox->isChecked() == false)
	{
		set_1RadiusLineEdit->setText(s.c_str());
		setSliderForRadius(set_1RadiusLineEdit->text().toDouble());
	}
}

void KernelWindow::set_1PointRadioButton_clicked()
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


void KernelWindow::set_1AreaRadioButton_clicked()
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



void KernelWindow::set_1TableComboBox_activated( const QString & )
{
	fillColumns(1, "");
}

void KernelWindow::setRadiusForSlider( int pos )
{
	//Changes value of Radius boxes to meet Slider position
	double maxDif = max(cLHeightValueTextLabel->text().toDouble(), cLWidthValueTextLabel->text().toDouble());

	string text = Te2String(pos*maxDif/1000);
	set_1RadiusLineEdit->setText(text.c_str());
}

void KernelWindow::setSliderForRadius( double radius )
{
	//Moves Slider to meet Radius value

	double maxDif = max(cLHeightValueTextLabel->text().toDouble(), 
	cLWidthValueTextLabel->text().toDouble());
	set_1RadiusSlider->setValue((int)(radius*1000/maxDif));
}

void KernelWindow::set_1RadiusSlider_valueChanged( int value)
{
	setRadiusForSlider(value);
}


void KernelWindow::set_1RadiusLineEdit_returnPressed()
{
	setSliderForRadius(set_1RadiusLineEdit->text().toDouble());
}

void KernelWindow::updateCombosSlot()
{
	if (!this->isVisible())
		return;

	if(set_1PointRadioButton->isChecked())
		fillThemeCombo(TePOINTS, mainWindow_->currentView(), set_1ThemeComboBox, "");
	else
		fillThemeCombo(TePOLYGONS,mainWindow_->currentView(),set_1ThemeComboBox, "");

	if(set_1TableGroupBox->isEnabled())
	{
		fillTableCombo(mainWindow_->currentView(),set_1TableComboBox, set_1ThemeComboBox, "");
		fillColumns(1, "");
	}
	fillInformationGroup();
}


void KernelWindow::set_1ThemeComboBox_activated( const QString & )
{
	if(set_1AttributeCheckBox->isChecked())
		fillTableCombo(mainWindow_->currentView(),set_1TableComboBox, set_1ThemeComboBox, "");
	fillInformationGroup();
}



void KernelWindow::gridThemeComboBox_activated( const QString & )
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
void KernelWindow::executeKernel()
{
	TeProjection* view_proj = mainWindow_->currentView()->projection();
	if (view_proj->units() != "Meters")
	{
		if(set_1CalculateComboBox->currentText() == tr("Density"))
		{
			QMessageBox::information(this, tr("Information"), tr("Density cannot be used when the Unit of the View Projection is not in Meters!"));
			return;		
		}
	}

	//Recupera parametros da interface e faz consistencia
	TeKernelMethod meth;

	QString dummy; //Used to process text
	meth.db_ = mainWindow_->currentDatabase();

	if(verifyProjections() == false)
		return;

	/***********************************************************************/
	/*** Inicializa parametros                                           ***/

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
	if (set_1AttributeCheckBox->isChecked()) 
	{
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
	else 
	{
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
		if (meth.isGridReg_&& !dummy.compare(gridThemeComboBox->currentText())) {
				QMessageBox::warning(this, tr("Warning"), 
				tr("The layer name cannot have the same name of the region theme!"));
				return;
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
		if (!meth.db_->tableExist("te_kernel_metadata"))
		{
			createKernelMetadataTable(meth.db_);
			if (!updateKernelMetadata(meth.db_ ,meth.params_))
			{
				QMessageBox::critical(this, tr("Error"), 
				tr("Fail to store the kernel parameters!"));
				okPushButton->setEnabled(true);
				return;
			}
		}
		else
		{
			QMessageBox::critical(this, tr("Error"), 
			tr("Fail to store the kernel parameters!"));
			okPushButton->setEnabled(true);
			return;
		}
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

void KernelWindow::closeEvent( QCloseEvent *)
{
	mainWindow_->kernelEnableControl(true);
	hide();
}


void KernelWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("kernelWindow.htm");
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


void KernelWindow::gridColNumberLineEdit_textChanged( const QString & txt )
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

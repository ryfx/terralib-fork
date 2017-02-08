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

//#include <TeDatabaseUtils.h>
#include <display.h>
#include <TeWaitCursor.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeExternalTheme.h>

#include <TeThemeFunctions.h>

#include <iterator>

void SpatialQueryWindow::Init()
{
    help_ = 0;

    mainWindow_ =(TerraViewBase*)qApp->mainWidget();
    //current theme 
    curtheme_ = mainWindow_->currentTheme();
	TeTheme* baseTheme = (TeTheme*)curtheme_->getTheme();
	if(!(baseTheme->type() == TeTHEME || baseTheme->type() == TeEXTERNALTHEME))
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	db_ = mainWindow_->currentDatabase();
    if(!db_)
		return;
    
    TemaLineEdit->setText(baseTheme->name().c_str());

	TeLayer* layer;
	if(baseTheme->getProductId() == TeTHEME)
		layer = baseTheme->layer();
	else if(baseTheme->getProductId() == TeEXTERNALTHEME)
		layer = ((TeExternalTheme*)baseTheme)->getRemoteTheme()->layer();
    
    int numRepres = 0;
	TeGeomRep visRep = (TeGeomRep)baseTheme->visibleRep();

	if ((layer->hasGeometry (TePOLYGONS)) && ((visRep & TePOLYGONS) == TePOLYGONS))
	{
		++numRepres;
		repCur_ = TePOLYGONS;
	}
	if ((layer->hasGeometry (TeLINES)) && ((visRep & TeLINES) == TeLINES))
	{
		++numRepres;
		repCur_ = TeLINES;
	}
	if ((layer->hasGeometry (TePOINTS)) && ((visRep & TePOINTS) == TePOINTS))
	{
		++numRepres;
		repCur_ = TePOINTS;
	}
	if ((layer->hasGeometry (TeCELLS)) && ((visRep & TeCELLS) == TeCELLS))
	{
		++numRepres;
		repCur_ = TeCELLS;
	}

	if(numRepres>1)
	{
		QMessageBox::warning(this, tr("Warning"),
		tr("Select only a visible geometrical representation for the current theme!"));
		return;
	}
	
	enableRepSlot(repCur_, repCur_);
    
    //current view 
    curview_ = mainWindow_->currentView();
    
    //tema visível
    vistheme_ = 0;
	CheckthemaCor_2_2->setChecked(false);
	ComboBox1_2->setEnabled(false);
	
     //canvas
    canvas_ = mainWindow_->getCanvas();
    
    //box canvas
    box_= mainWindow_->getChangedBox();

	TemaLineEdit->setEnabled(false);
    repVis_ = TePOLYGONS;
    relate_ = 0;
    explicacaoLineEdit->clear();
   	newquery_ = false;
	addquery_ = false;
	refinequery_ = false;

	pointedCheckBox->setChecked(true);
	queriedCheckBox->setChecked(true);
		 
	this->show();
}


void  SpatialQueryWindow::enableLinPolButtons(bool en)
{
    DisjointLinPolButton->setEnabled(en);
    CrossLinPolButton->setEnabled(en);
    OverlapLinPolButton->setEnabled(en);
    TouchLinPolButton->setEnabled(en);
    WithinLinPolButton->setEnabled(en);
    CoveredByLinPolButton->setEnabled(en);
}

void  SpatialQueryWindow::enablePolPolButtons(bool en)
{
    DisjointPolButton->setEnabled(en);
    EqualPolButton->setEnabled(en);
    OverlapPolButton->setEnabled(en);
    TouchPolButton->setEnabled(en);
    WithinPolButton->setEnabled(en);
    ContainPolButton->setEnabled(en);
    CoveredByPolButton->setEnabled(en);
    CoversPolButton->setEnabled(en);
}

void  SpatialQueryWindow::enableLinLinButtons(bool en)
{
    DisjointLinButton->setEnabled(en);
    EqualLinButton->setEnabled(en);
    TouchLinButton->setEnabled(en);
    WithinLinButton->setEnabled(en);
    ContainLinButton->setEnabled(en);
    CrossLinButton->setEnabled(en);
    OverlapLinButton->setEnabled(en);
}

void  SpatialQueryWindow::enableLinPonButtons(bool en)
{
    DisjointLinPonButton->setEnabled(en);
    WithinLinPonButton->setEnabled(en);
    TouchLinPonButton->setEnabled(en);
}

void  SpatialQueryWindow::enablePonPolButtons(bool en)
{
    DisjointPonPolButton->setEnabled(en);
    ContainPonPolButton->setEnabled(en);
    TouchPonPolButton->setEnabled(en);
}

void SpatialQueryWindow::offButtonsSlot()
{
    DisjointPolButton->setOn(false);
    EqualPolButton->setOn(false);
    OverlapPolButton->setOn(false);
    TouchPolButton->setOn(false);
    WithinPolButton->setOn(false);
    ContainPolButton->setOn(false);
    CoveredByPolButton->setOn(false);
    CoversPolButton->setOn(false);
    
    DisjointLinPolButton->setOn(false); 
    CrossLinPolButton->setOn(false);
    OverlapLinPolButton->setOn(false);
    TouchLinPolButton->setOn(false);
    WithinLinPolButton->setOn(false);
    CoveredByLinPolButton->setOn(false);
     
    DisjointLinButton->setOn(false);
    EqualLinButton->setOn(false);
    TouchLinButton->setOn(false);
    WithinLinButton->setOn(false);
    ContainLinButton->setOn(false);
    CrossLinButton->setOn(false);
    OverlapLinButton->setOn(false);
    
    DisjointLinPonButton->setOn(false);
    WithinLinPonButton->setOn(false);
    TouchLinPonButton->setOn(false);
    DisjointPonPolButton->setOn(false);
    ContainPonPolButton->setOn(false);
    TouchPonPolButton->setOn(false);
    	    
 } 

void SpatialQueryWindow::changeVisThemeSlot(const QString& themeName)
{
	TeQtViewsListView* viewsListView = mainWindow_->getViewsListView();
	TeQtViewItem* curViewItem = viewsListView->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();

    string tName = themeName.latin1();
	if(tName.empty())
	{
		vistheme_= 0;
		return;
	}

	for (unsigned int i = 0; i < themeItemVec.size(); ++i)
	{
		TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
		TeTheme* theme = (TeTheme*)appTheme->getTheme();
		if (mainWindow_->getViewsListView()->getThemeItem(appTheme) && theme->name() == tName)
		{
			vistheme_ = appTheme;
			break;
		}
	}
	
	if(vistheme_)
	{
		int numRepres = 0;
		TeGeomRep visRep = (TeGeomRep)((TeTheme*)vistheme_->getTheme())->visibleRep();
		TeLayer* layer;
		if(((TeTheme*)vistheme_->getTheme())->getProductId() == TeTHEME)
			layer = ((TeTheme*)vistheme_->getTheme())->layer();
		else if(((TeTheme*)vistheme_->getTheme())->getProductId() == TeEXTERNALTHEME)
			layer = ((TeExternalTheme*)vistheme_->getTheme())->getRemoteTheme()->layer();

		if (layer->hasGeometry(TePOLYGONS) && ((visRep & TePOLYGONS) == TePOLYGONS))
		{
			++numRepres;
			repVis_ = TePOLYGONS;
		}
		if (layer->hasGeometry(TeLINES) && ((visRep & TeLINES) == TeLINES))
		{
			++numRepres;
			repVis_ = TeLINES;
		}
		if (layer->hasGeometry(TePOINTS) && ((visRep & TePOINTS) == TePOINTS))
		{
			++numRepres;
			repVis_ = TePOINTS;
		}
		if (layer->hasGeometry(TeCELLS) && ((visRep & TeCELLS) == TeCELLS))
		{
			++numRepres;
			repVis_ = TeCELLS;
		}

		if(numRepres>1)
		{
			QMessageBox::warning(this, tr("Warning"),
			tr("Select only a visible geometrical representation for the visible theme!"));
			return;
		}

		enableRepSlot(repCur_, repVis_);
		return;
	 }
	
	vistheme_ = 0;
	return;
}
   
    
void SpatialQueryWindow::enableVisThemeSlot(bool checked)
{
    ComboBox1_2->clear();
         
	if(!checked)
	{
		ComboBox1_2->setEnabled(false);
		enableRepSlot(repCur_, repCur_);
	}
	else
	{   
		ComboBox1_2->setEnabled(true);
		ComboBox1_2->setCurrentItem(0);
		vistheme_= 0;
		int cont = 0;

		//fill the visible themes 
		TeQtViewsListView* viewsListView = mainWindow_->getViewsListView();
		TeQtViewItem* curViewItem = viewsListView->currentViewItem();
		vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();

		for (unsigned int i = 0; i < themeItemVec.size(); ++i)
		{
			TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
			TeTheme* theme = (TeTheme*)appTheme->getTheme();
			if (theme->visibility()==1 && (theme->type() == TeTHEME || theme->type() == TeEXTERNALTHEME)) //TeTHEME OR TeEXTERNALTHEME
			{
				if(mainWindow_->getViewsListView()->getThemeItem(appTheme))
				{
					if(theme->name() != ((TeTheme*)curtheme_->getTheme())->name())
					{
						if(!cont)
							vistheme_= appTheme;
						ComboBox1_2->insertItem(QString(theme->name().c_str()));
						cont++;
					}
				}
			}
		}
		
		if(vistheme_)
		{
		    int numRepres = 0;
			TeGeomRep visRep = (TeGeomRep)((TeTheme*)vistheme_->getTheme())->visibleRep();
			TeLayer* layer;
			if(((TeTheme*)vistheme_->getTheme())->getProductId() == TeTHEME)
				layer = ((TeTheme*)vistheme_->getTheme())->layer();
			else if(((TeTheme*)vistheme_->getTheme())->getProductId() == TeEXTERNALTHEME)
				layer = ((TeExternalTheme*)vistheme_->getTheme())->getRemoteTheme()->layer();

			if (layer->hasGeometry(TePOLYGONS) && ((visRep & TePOLYGONS) == TePOLYGONS))
			{
				++numRepres;
				repVis_ = TePOLYGONS;
			}
			if (layer->hasGeometry(TeLINES) && ((visRep & TeLINES) == TeLINES))
			{
				++numRepres;
				repVis_ = TeLINES;
			}
			if (layer->hasGeometry (TePOINTS) && ((visRep & TePOINTS) == TePOINTS))
			{
				++numRepres;
				repVis_ = TePOINTS;
			}
			if (layer->hasGeometry(TeCELLS) && ((visRep & TeCELLS) == TeCELLS))
			{
				++numRepres;
				repVis_ = TeCELLS;
			}

			if(numRepres>1)
			{
				QMessageBox::warning(this, tr("Warning"),
				tr("Select only a visible geometrical representation for the visible theme!"));
				return;
			}
	
			enableRepSlot(repCur_, repVis_);
		 }
	}
}


void SpatialQueryWindow::DisjointPolButtonSlot() 
{
    offButtonsSlot();
    DisjointPolButton->setOn(true);

    QString rel =  "Disjoint (polygon, polygon)";
    QString mes = tr("Result: polygons which don´t present any intersection with the selected polygons.");
    nomeLineEdit->setText(rel);
    explicacaoLineEdit->setText(mes);

    relate_ = TeDISJOINT;
    return;
}
    
void SpatialQueryWindow::EqualPolButtonSlot()   
{
    offButtonsSlot();
    EqualPolButton->setOn(true);

    QString rel =  "Equal (polygon, polygon)";
    QString mes = tr("Result: polygons which are equal to selected polygons.");
    nomeLineEdit->setText(rel);
    explicacaoLineEdit->setText(mes);

     relate_ = TeEQUALS; 
     return;
}
    
void SpatialQueryWindow::OverlapPolButtonSlot() 
{
    offButtonsSlot();
    OverlapPolButton->setOn(true);
    
    QString rel =  "Overlap (polygon, polygon)";
    QString mes = tr("Result: polygons which overlap the selected polygons.");
    nomeLineEdit->setText(rel);
    explicacaoLineEdit->setText(mes);
	
    relate_ = TeOVERLAPS;
    return;
}

void SpatialQueryWindow::TouchPolButtonSlot() 
{
    offButtonsSlot();
    TouchPolButton->setOn(true);
    	
    QString rel =  "Touch (polygon, polygon)";
    QString mes = tr("Result: polygons which touch the selected polygons (intersection only between boundaries).");
     nomeLineEdit->setText(rel);
    explicacaoLineEdit->setText(mes);
	
    relate_ = TeTOUCHES;
    return;
}

void SpatialQueryWindow::WithinPolButtonSlot() 
{
    offButtonsSlot();
     WithinPolButton->setOn(true);
    	
    QString rel   =  "Within (polygon, polygon)";
    QString mes = tr("Result: polygons which are inside the selected polygons (without intersection between boundaries).");
     nomeLineEdit->setText(rel);
    explicacaoLineEdit->setText(mes);

    relate_ = TeWITHIN;
    return;
}

void SpatialQueryWindow::ContainPolButtonSlot()
{
    offButtonsSlot();
    ContainPolButton->setOn(true);
    	
    QString rel   =  "Contain (polygon, polygon)";
    QString mes = tr("Result: polygons which contain the selected polygons (without intersection between boundaries).");
     nomeLineEdit->setText(rel);
    explicacaoLineEdit->setText(mes);
	
    relate_ = TeCONTAINS;
    return;
}

void SpatialQueryWindow::CoveredByPolButtonSlot() 
{
    offButtonsSlot();
    CoveredByPolButton->setOn(true);
    
    QString rel   =  "CoveredBy (polygon, polygon)";
    QString mes = tr("Result: polygons which are inside the selected polygons (with intersection between boundaries).");
    nomeLineEdit->setText(rel);
    explicacaoLineEdit->setText(mes);

    relate_ = TeCOVEREDBY;
    return;
}

void SpatialQueryWindow::CoversPolButtonSlot() 
{
    offButtonsSlot();
    CoversPolButton->setOn(true);
    
    QString rel   =  "Cover (polygon, polygon)";
    QString mes = tr("Result: polygons which contain the selected polygons (with intersection between boundaries).");
     nomeLineEdit->setText(rel);
    explicacaoLineEdit->setText(mes);
	
    relate_ = TeCOVERS;
    return;
}

void SpatialQueryWindow::DisjointLinPolButtonSlot()  
{
    offButtonsSlot();
    DisjointLinPolButton->setOn(true);
    	
    QString mes, rel;
    if((repCur_== TePOLYGONS) )
    {
		rel =  " Disjoint (polygon, line)";
		mes = tr("Result: lines which don´t present any intersection with the selected polygons.");
    }
    else
    {
		rel =  "Disjoint (line, polygon)";
		mes = tr("Result: polygons which don´t present any intersection with the selected lines.");
    }
	
    nomeLineEdit->setText(rel);
    explicacaoLineEdit->setText(mes);

    relate_ = TeDISJOINT;	
    return;
}
 
void SpatialQueryWindow::CrossLinPolButtonSlot()   
{
    offButtonsSlot();
    CrossLinPolButton->setOn(true);
    
    QString mes, rel;
    if((repCur_== TePOLYGONS))
	{
		rel = "Cross  (polygon, line)";
		mes = tr("Result: lines which cross the boundaries of the selected polygons.");
	}
	else
	{
		rel = "Cross (line, polygon)";
		mes = tr("Result: polygons whose boundaries are crossed by the selected lines.");
    }
	
	nomeLineEdit->setText(rel);
	explicacaoLineEdit->setText(mes);

	relate_ = TeCROSSES;  	
    return;
}


void SpatialQueryWindow::OverlapLinPolButtonSlot()   
{
    offButtonsSlot();
    OverlapLinPolButton->setOn(true);
    
    QString mes, rel;
    if((repCur_== TePOLYGONS) )
	{
		rel = "Intersects  (polygon, line)";
		mes = tr("Result: lines which intersect the selected polygons.");
	}
	else
	{
		rel = "Intersects (line, polygon)";
		mes = tr("Result: polygons which Intersect the selected lines.");
    }
	
	nomeLineEdit->setText(rel);
	explicacaoLineEdit->setText(mes);
	relate_ = TeINTERSECTS;  	
    return;
}



void SpatialQueryWindow::TouchLinPolButtonSlot()  
{
    offButtonsSlot();
    TouchLinPolButton->setOn(true);
    
	QString mes, rel;
	if(repCur_==TeLINES)
	{
		rel = "Touch (line, polygon)";
		mes = tr("Result: polygons which touch the selected lines (intersection only between boundaries).");
	}
	else
	{
		rel = "Touch (polygon, line)";
		mes = tr("Result: lines which touch the selected polygons (intersection only between boundaries).");   
	}
	 nomeLineEdit->setText(rel);
	explicacaoLineEdit->setText(mes);

	relate_ = TeTOUCHES;
    return;
}
    
void SpatialQueryWindow::WithinLinPolButtonSlot() 
{
    offButtonsSlot();
    WithinLinPolButton->setOn(true);

	QString mes, rel;
	if(repCur_==TeLINES)
	{
		rel = "Contain (polygon, line)";
		mes = tr("Result: polygons which contain the selected lines (without intersection between boundaries).");  
		relate_ = TeCONTAINS;
	}
    else
	{
		rel = "Within (line, polygon)";
		mes = tr("Result: lines which are inside the selected polygons (without intersection between boundaries).");   
		relate_ = TeWITHIN;
	}

	 nomeLineEdit->setText(rel);
	 explicacaoLineEdit->setText(mes);
    return;
}
    

void SpatialQueryWindow::CoveredByLinPolButtonSlot()  
{
    offButtonsSlot();
    CoveredByLinPolButton->setOn(true);

	QString mes, rel;
	if(repCur_==TeLINES)
	{
		rel = "Cover (polygon, line)";
		mes = tr("Result: polygons which contain the selected lines (with intersection between boundaries)."); 
		relate_ = TeCOVERS;
	}
	else
	{
		rel = "CoveredBy (line, polygon)";
		mes = tr("Result: lines which are inside the selected polygons (with intersection between boundaries)");    
		relate_ = TeCOVEREDBY;
	}
	nomeLineEdit->setText(rel);
	explicacaoLineEdit->setText(mes);
    return;
}

    
 void SpatialQueryWindow::DisjointLinButtonSlot()  
 {
     offButtonsSlot();
     DisjointLinButton->setOn(true);

	 QString rel   =  "Disjoint (line, line)";
	 QString mes = tr("Result: lines which don´t present any intersection with the selected lines.");
	
	 nomeLineEdit->setText(rel);
	 explicacaoLineEdit->setText(mes);

	 relate_ = TeDISJOINT;
     return;
 }
    
 void SpatialQueryWindow::EqualLinButtonSlot()  
 {
     offButtonsSlot();
     EqualLinButton->setOn(true);

     QString rel   =  "Equal (line, line)";
     QString mes = tr("Result: lines which are equal to selected lines.");
     nomeLineEdit->setText(rel);
     explicacaoLineEdit->setText(mes);
     
     relate_ = TeEQUALS;
     return;
 }
  
 void SpatialQueryWindow::TouchLinButtonSlot()  
 {
     offButtonsSlot();
     TouchLinButton->setOn(true);
	 
     QString rel   =  "Touch (line, line)";
     QString mes = tr("Result: lines which touch the selected lines (intersection only between boundaries).");
     nomeLineEdit->setText(rel);
     explicacaoLineEdit->setText(mes);

     relate_ = TeTOUCHES;
     return;
 }
 
 void SpatialQueryWindow::WithinLinButtonSlot() 
 {
     offButtonsSlot();
    WithinLinButton->setOn(true);

	 QString rel   =  "Within (line, line)";
     QString mes = tr("Result: lines which are inside the selected lines."); 
     nomeLineEdit->setText(rel);
	 explicacaoLineEdit->setText(mes);

	 relate_ = TeWITHIN;
     return;
 }
    
 void SpatialQueryWindow::ContainLinButtonSlot() 
 {
     offButtonsSlot();
     ContainLinButton->setOn(true);

	 QString rel   =  "Contain (line, line)";
     QString mes = tr("Result: lines which contain the selected lines."); 
     nomeLineEdit->setText(rel);
     explicacaoLineEdit->setText(mes);

     relate_ = TeCONTAINS;
     return;
 }
 
 void SpatialQueryWindow::CrossLinButtonSlot() 
 {
     offButtonsSlot();
    CrossLinButton->setOn(true);

	 QString rel   =  "Cross (line, line)";
     QString mes = tr("Result: lines which cross the selected lines (without intersection between boundaries).");
     nomeLineEdit->setText(rel);
     explicacaoLineEdit->setText(mes);

	 relate_ = TeCROSSES;
     return;
 }
 
 void SpatialQueryWindow::OverlapLinButtonSlot() 
 {
     offButtonsSlot();
     OverlapLinButton->setOn(true);

	 QString rel   =  "Overlap (line, line)";
     QString mes = tr("Result: lines which overlap the selected lines (without intersection between boundaries).");
     nomeLineEdit->setText(rel);
     explicacaoLineEdit->setText(mes);

	 relate_ = TeOVERLAPS;
     return;
 }
 
 void SpatialQueryWindow::DisjointLinPonButtonSlot()  
 {
     offButtonsSlot();
     DisjointLinPonButton->setOn(true);
	
	 QString rel, mes;
	 if(repCur_ == TeLINES)
	 {
		rel = "Disjoint (line, point)";
		mes = tr("Result: points which don´t present any intersection with the selected lines.");
	 }
	 else
	 {
		rel = "Disjoint (point, line)";
		mes = tr("Result: lines which don´t present any intersection with the selected points.");
	 }
	 nomeLineEdit->setText(rel);
	 explicacaoLineEdit->setText(mes);

	 relate_ = TeDISJOINT;
     return;
 }
 
 void SpatialQueryWindow::WithinLinPonButtonSlot() 
 {
     offButtonsSlot();
     WithinLinPonButton->setOn(true);

	 QString rel, mes;
	 if(repCur_ == TeLINES)
	 {
		rel = "Within (point, line)";
		mes = tr("Result: points which are inside the selected lines."); 
		relate_ = TeWITHIN;
	 }
	 else
	 {
		rel = "Contain (line, point)";
		mes = tr("Result: lines which contain the selected points.");  
		relate_ = TeCONTAINS;
	 }
	 nomeLineEdit->setText(rel);
	 explicacaoLineEdit->setText(mes);
     return;
 }
  
 void SpatialQueryWindow::TouchLinPonButtonSlot() 
 {
     offButtonsSlot();
     TouchLinPonButton->setOn(true);

	 QString rel, mes;
	 if(repCur_ == TeLINES)
	 {
		rel =  "Touch (point, line)";
		mes = tr("Result: points which touch the selected lines."); 
	 }
	 else
	 {
		rel = "Touch (line, point)";
		mes = tr("Result: lines which touch the selected points.");  
	 }
	 relate_ = TeTOUCHES;
	 nomeLineEdit->setText(rel);
	 explicacaoLineEdit->setText(mes);
     return;
 }
 
 void SpatialQueryWindow::DisjointPonPolButtonSlot() 
 {
     offButtonsSlot();
     DisjointPonPolButton->setOn(true);

	 QString rel, mes;
	 if(repCur_==TePOLYGONS) 
	 {
		rel = "Disjoint (polygon, point)";
		mes = tr("Result: points which don´t present any intersection with the selected polygons.");
	 }
	 else
	 {
		rel = "Disjoint (point, polygon)";
		mes = tr("Result: polygons which don´t present any intersection with the selected points.");
	 }
	 nomeLineEdit->setText(rel);
	 explicacaoLineEdit->setText(mes);

	 relate_ = TeDISJOINT;
     return;
 }
  
 void SpatialQueryWindow::ContainPonPolButtonSlot()
 {
     offButtonsSlot();
     ContainPonPolButton->setOn(true);

	 QString rel, mes;
	 if(repCur_==TePOLYGONS)
	 {
		rel =  "Within (point, polygon)";
		mes = tr("Result: points which are inside the selected polygons.");  
		relate_ = TeWITHIN;
	 }
	 else
	 {
		rel = "Contain (polygon, point)";
		mes = tr("Result: polygons which contain the selected points.");   
		relate_ = TeCONTAINS;
	 }
	 nomeLineEdit->setText(rel); 
	 explicacaoLineEdit->setText(mes);
     return;
 }
 
 
 void SpatialQueryWindow::TouchPonPolButtonSlot() 
 {
     offButtonsSlot();
    TouchPonPolButton->setOn(true);

	 QString rel, mes;
	 if(repCur_==TePOLYGONS) 
	 {
		rel =  "Touch (polygon, point)";
		mes = tr("Result: points which touch the boundaries of the selected polygons."); 
	 }
	 else
	 {
		rel = "Touch (point, polygon)";
		mes = tr("Result: polygons touched by the selected points.");  
	 }
	 relate_ = TeTOUCHES;
	 nomeLineEdit->setText(rel);
	explicacaoLineEdit->setText(mes);
     return;
 }
    
 
TeDatabase* SpatialQueryWindow::getDatabaseSlot()
{
	return db_;
}

TeTheme* SpatialQueryWindow::getCurThemeSlot()
{
	return (TeTheme*)curtheme_->getTheme();
}

TeTheme* SpatialQueryWindow::getVisThemeSlot()
{
	return (TeTheme*)vistheme_->getTheme();
}

void  SpatialQueryWindow::sairSlot()
{
    hide();
}

void SpatialQueryWindow::NewQuerySlot( )
{
    newquery_ = true;
    addquery_ = false;
    refinequery_ = false;
    ExecuteSlot( );
}

void SpatialQueryWindow::AddQuerySlot( )
{
    newquery_ = false;
    refinequery_ = false;
    addquery_ = true;
    ExecuteSlot( );
}

void SpatialQueryWindow::RefineQuerySlot( )
{
    newquery_ = false;
    refinequery_ = true;
    addquery_ = false;
    ExecuteSlot( );
}

void SpatialQueryWindow::ExecuteSlot( )
{ 
	if (!pointedCheckBox->isChecked() && !queriedCheckBox->isChecked())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Define which objects in the active theme should be considered (pointed and/or queried!)"));
		return;
	}
	if(relate_==0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select the query that will be executed!"));
		return;
	}

	TeKeys IdsIn;
	TeKeys IdsOut;
	string collName = "spatial_data";

///	double tol = LineEdit3_2->text().toDouble();

	//active theme
	TeTheme* curBaseTheme = (TeTheme*)curtheme_->getTheme();
	string actColTable = curBaseTheme->collectionTable();

	TeLayer* layer;
	if(curBaseTheme->getProductId() == TeTHEME)
		layer = curBaseTheme->layer();
	else if(curBaseTheme->getProductId() == TeEXTERNALTHEME)
		layer = ((TeExternalTheme*)curBaseTheme)->getRemoteTheme()->layer();

	string actTable = layer->tableName((TeGeomRep)repCur_);

	string sql, visColTable;
 
	TeDatabasePortal* port = db_->getPortal();
	if(curBaseTheme->getProductId() == TeTHEME)
	{
		sql = "SELECT c_object_id FROM " + actColTable;
		
		if (pointedCheckBox->isChecked())
		{
			if (queriedCheckBox->isChecked())
			{
				sql += "  WHERE c_object_status = 2";
				sql += "  OR c_object_status = 1";
			}
			else
				sql += "  WHERE c_object_status = 1";
		}
		else if (queriedCheckBox->isChecked())
				sql += "  WHERE c_object_status = 2";
	    
		if(!port->query(sql))
		{
			delete port;
			return;
		}
	    
		while(port->fetchRow())
		{
			string Id = port->getData(0);
			IdsIn.push_back(Id);
		}
		delete port;
	}
	else if(curBaseTheme->getProductId() == TeEXTERNALTHEME)
	{
		actColTable = ((TeExternalTheme*)curBaseTheme)->getRemoteTheme()->collectionTable();
		if (pointedCheckBox->isChecked())
		{
			IdsIn = getObjects(curBaseTheme, TeSelectedByPointing);
			if (queriedCheckBox->isChecked())
			{
				vector<string> vaux = getObjects(curBaseTheme, TeSelectedByQuery);
				copy(vaux.begin(),vaux.end(),back_inserter(IdsIn));
			}
		}
		else if (queriedCheckBox->isChecked())
			IdsIn = getObjects(curBaseTheme, TeSelectedByPointing);
	}
	    
	if(IdsIn.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("The active theme doesn't have pointed or queried objects."));
		return;
	}

	TeWaitCursor wait;

	TePrecision::instance().setPrecision(TeGetPrecision(curBaseTheme->layer()->projection()));	

	canvas_->setLineStyle(3,3);

	//visible theme

	if(curBaseTheme->getProductId() == TeTHEME)
	{
		if(vistheme_)
		{
			int visRep = repVis_; 
			TeTheme* visBaseTheme = (TeTheme*)vistheme_->getTheme();
			if(visBaseTheme->getProductId() == TeTHEME)
			{
				visColTable = visBaseTheme->collectionTable();			
				string visTable = visBaseTheme->layer()->tableName((TeGeomRep)visRep);
				db_->spatialRelation(actTable, (TeGeomRep)repCur_, IdsIn, visTable, (TeGeomRep)visRep, IdsOut, relate_, visColTable);
			}
			else if(visBaseTheme->getProductId() == TeEXTERNALTHEME)
			{
				TeDatabase* db = ((TeExternalTheme*)visBaseTheme)->getRemoteTheme()->layer()->database();
				visColTable = ((TeExternalTheme*)visBaseTheme)->getRemoteTheme()->collectionTable();			
				string visTable = ((TeExternalTheme*)visBaseTheme)->getRemoteTheme()->layer()->tableName((TeGeomRep)visRep);
				db_->spatialRelation(actTable, (TeGeomRep)repCur_, IdsIn, visTable, (TeGeomRep)visRep, IdsOut, relate_, visColTable, db);
			}		
		}
		else
			db_->spatialRelation(actTable, (TeGeomRep)repCur_, IdsIn, IdsOut, relate_, actColTable);
	}
	else if (curBaseTheme->getProductId() == TeEXTERNALTHEME)
	{
		TeDatabase* db = ((TeExternalTheme*)curBaseTheme)->getRemoteTheme()->layer()->database();
		if(vistheme_)
		{
			int visRep = repVis_; 
			TeTheme* visBaseTheme = (TeTheme*)vistheme_->getTheme();
			if(visBaseTheme->getProductId() == TeTHEME)
			{
				visColTable = visBaseTheme->collectionTable();			
				string visTable = visBaseTheme->layer()->tableName((TeGeomRep)visRep);
				db->spatialRelation(actTable, (TeGeomRep)repCur_, IdsIn, visTable, (TeGeomRep)visRep, IdsOut, relate_, visColTable, db_);
			}
			else if(visBaseTheme->getProductId() == TeEXTERNALTHEME)
			{
				TeDatabase* dbVis = ((TeExternalTheme*)visBaseTheme)->getRemoteTheme()->layer()->database();
				visColTable = ((TeExternalTheme*)visBaseTheme)->getRemoteTheme()->collectionTable();			
				string visTable = ((TeExternalTheme*)visBaseTheme)->getRemoteTheme()->layer()->tableName((TeGeomRep)visRep);
				db->spatialRelation(actTable, (TeGeomRep)repCur_, IdsIn, visTable, (TeGeomRep)visRep, IdsOut, relate_, visColTable, dbVis);
			}		
		}
		else
			db->spatialRelation(actTable, (TeGeomRep)repCur_, IdsIn, IdsOut, relate_, actColTable);
	}
	

	if(IdsOut.empty())
	{
		wait.resetWaitCursor();
		QMessageBox::information(this, tr("Information"),
					 tr("This spatial query didn´t return any geometry!"));
		return;
	}
	
	if(refinequery_)
	{
		TeKeys idsQuery;
		string colTable;
		port = db_->getPortal();

		if(vistheme_)
			colTable = ((TeTheme*)vistheme_->getTheme())->collectionTable();			
		else
			colTable = curBaseTheme->collectionTable();	

		sql = "SELECT c_object_id FROM " + colTable;
		sql += " WHERE c_object_status = 3 OR c_object_status = 2 ORDER BY c_object_id";

		if(!port->query(sql))
		{
			delete port;
			wait.resetWaitCursor();
			return;
		}
    
		while(port->fetchRow())
		{
			string Id = port->getData(0);
			idsQuery.push_back(Id);
		}

		if(idsQuery.empty())
		{
			wait.resetWaitCursor();
			QMessageBox::information(this, tr("Information"),
					 tr("There are no queried objects to refine!"));
			delete port;
			return;
		}

		TeKeys temp(idsQuery.size());
		set_intersection(IdsOut.begin(), IdsOut.end(), idsQuery.begin(), idsQuery.end(), temp.begin());
		IdsOut = temp;
		delete port;
	}

	plotGeomResult(IdsOut);
	wait.resetWaitCursor();
	return;
 }


void SpatialQueryWindow::enableRepSlot( TeGeomRep rep1, TeGeomRep rep2 )
{
    offButtonsSlot();
	switch (rep1 | rep2)
    {
	case 1:  //polygon and polygon
	case 256: //cell and cell
	case 257: //cell and polygon
		enableLinPolButtons(false);
		enablePolPolButtons(true);
		enableLinLinButtons(false);
		enableLinPonButtons(false);
		enablePonPolButtons(false);
		break;
    
	case 2:  //line and line
		enableLinPolButtons(false);
		enablePolPolButtons(false);
		enableLinLinButtons(true);
		enableLinPonButtons(false);
		enablePonPolButtons(false);
		break;
	    
	case 3:  //line and polygon
	case 258://line and cell
	    enableLinPolButtons(true);
		enablePolPolButtons(false);
		enableLinLinButtons(false);
		enableLinPonButtons(false);
		enablePonPolButtons(false);
		break;
	    
	case 4:  //point and point
	    enableLinPolButtons(false);
		enablePolPolButtons(false);
		enableLinLinButtons(false);
		enableLinPonButtons(false);
		enablePonPolButtons(false);
		 break;
	
	case 5: //polygon and point
	case 260: //cell and point
	    enableLinPolButtons(false);
		enablePolPolButtons(false);
		enableLinLinButtons(false);
		enableLinPonButtons(false);
		enablePonPolButtons(true);
	    break;
	    
	case 6: //line and point
	     enableLinPolButtons(false);
		enablePolPolButtons(false);
		enableLinLinButtons(false);
		enableLinPonButtons(true);
		enablePonPolButtons(false);
	    break;
	    	    
	default:
	    break;	
	}
}

void SpatialQueryWindow::getGeomIdsSlot(const string& objId, const string& geomTable, TeKeys& geomIds)
{
	string sql;
	TeDatabasePortal* port = db_->getPortal();

	sql=  " SELECT geom_id ";
	sql+= " FROM "+ geomTable;
	sql+= " WHERE object_id = '"+ objId +"'";

	if(db_->dbmsName()!="OracleSpatial")
	{
		sql=  " SELECT MIN(geom_id) ";
		sql+= " FROM "+ geomTable;
		sql+= " WHERE object_id = '"+ objId +"'";
		sql+= " GROUP BY parent_id";
	}
	
	if(!port->query(sql))
	{
		delete port;
		return;
	}

	while(port->fetchRow())
	{
		string gId = port->getData(0);
		geomIds.push_back(gId);
	}

	delete port;
	return;
}


void SpatialQueryWindow::plotGeomResult(TeGeometryVect& geomVect)
{
	canvas_->setPolygonColor(0,0,0);
	canvas_->setLineColor (0,0,0);
	canvas_->setPointColor (0,0,0);
	
	TeGeometryVect::iterator it = geomVect.begin();
	while (it!= geomVect.end())
	{
		TeGeometry* geom = (*it);
					
		if(dynamic_cast<TePolygon*> (geom))
		{
			TePolygon poly, *pol;
			pol = new TePolygon();
			pol = (TePolygon*) geom;
			poly = *pol;
			canvas_->plotPolygon (poly, true);
		}
		else if (dynamic_cast<TePolygonSet*> (geom))
		{
			TePolygonSet polySet, *polSet;
			polSet = new TePolygonSet();
			polSet = (TePolygonSet*) geom;
			polySet = *polSet;
			int size = polySet.size();
			for(int c=0; c<size; c++)
			{
				TePolygon p = polySet[c];
				canvas_->plotPolygon (p, true);
			}
		}
		else if(dynamic_cast<TeLine2D*> (geom))
		{
			TeLine2D	line, *lin;
			lin = new TeLine2D();
			lin = (TeLine2D*) geom;
			line = *lin;
			canvas_->plotLine (line);
		}
		else if(dynamic_cast<TeLineSet*> (geom))
		{
			TeLineSet lineSet, *linSet;
			linSet = new TeLineSet();
			linSet = (TeLineSet*) geom;
			lineSet = *linSet;
			int size = lineSet.size();
			for(int c=0; c<size; c++)
			{
				TeLine2D l = lineSet[c];
				canvas_->plotLine (l);
			}
		}
		else if(dynamic_cast<TePoint*> (geom))
		{
			TePoint	 point, *pnt;
			pnt = new TePoint();
			pnt = (TePoint*) geom;
			point = *pnt;
			canvas_->plotPoint(point);
		}
		else if(dynamic_cast<TePointSet*> (geom))
		{
			TePointSet	pointSet, *ptSet;
			ptSet = new TePointSet();
			ptSet = (TePointSet*) geom;
			pointSet = *ptSet;
			int size = pointSet.size();
			for(int c=0; c<size; c++)
			{
				TePoint p = pointSet[c];
				canvas_->plotPoint(p);
			}
		}
		canvas_->copyPixmapToWindow();
		++it;
	}
	return;
}


void SpatialQueryWindow::plotGeomResult(TeKeys& IdsOut)
{
    vector<string>::iterator it = IdsOut.begin();
    set<string> objectIdSet, uniqueIdSet;
    TeQtGrid* grid = mainWindow_->getGrid();

	while(it != IdsOut.end())
    {
		objectIdSet.insert(*it);
		++it;
	}
	  
	if(objectIdSet.empty())
		return;

	if(vistheme_)	//change current theme
	{
		mainWindow_->changeCurrentThemeSlot(vistheme_);
		mainWindow_->getDisplayWindow()->plotData();
	}

	if(newquery_)
		grid->putColorOnGrid(uniqueIdSet, objectIdSet, "newQuery");
	else if(refinequery_)
		grid->putColorOnGrid(uniqueIdSet, objectIdSet, "filterQuery");
	else 
		grid->putColorOnGrid(uniqueIdSet, objectIdSet, "addQuery");

	mainWindow_->queryChanged();
}


void SpatialQueryWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("consulta_espacial.htm");
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


void SpatialQueryWindow::show()
{
	TeTheme* theme = (TeTheme*)mainWindow_->currentTheme()->getTheme();
	if(theme->type() == TeTHEME || theme->type() == TeEXTERNALTHEME)
		QDialog::show();
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
	}
}

/****************************************************************************  
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
******************************************************************************/
#include <String.h>
#include <qwidget.h> 
#include <qframe.h> 
#include <qcombobox.h>
#include <qlabel.h> 
#include <qlineedit.h>
#include <qbuttongroup.h> 
#include <qradiobutton.h> 
#include <qstring.h> 
#include <qdeepcopy.h> 


#define ASPECT__IS_ON 1
#define SLOPE_P_IS_ON 2
#define SLOPE_D_IS_ON 4
#define SLOPE_GEN_ON 6 //This is not a final option!
#define FLOOD_F_IS_ON 8
#define SET_DIR_IS_ON 16

const char* ASPECT__EXT = "_Asp";
const char* SLOPE_P_EXT = "_S_P";
const char* SLOPE_D_EXT = "_S_D";
const char* FLOOD_F_EXT = "_Fel";
const char* SET_DIR_EXT = "_Ang";
const char* DEFAULT_EXT = "What_do_you_want_ me_to_do";

//I am not huge fan of global rariables, but currently see no other way to keep
//treck of the status of the user input.
QString outLayerContent =  QDeepCopy<QString>("outputLayer");
int  operation = 0;

void EDAPluginMainForm::localHelp()
{

}

void EDAPluginMainForm::inToOutSlot(const QString& inLayer)
{   
   outLayerContent =  QDeepCopy<QString>(inLayer);
   //since outout Layer name has been completly reset, no checks for other 
   //extencions are needed
   switch(operation)
   {
       case ASPECT__IS_ON: optAspectSlot(true); break;
       case SLOPE_P_IS_ON: optSlopePercentSlot(true); break;
       case SLOPE_D_IS_ON: optSlopeDegreesSlot(true); break;
       case SLOPE_GEN_ON: optSlopeGenSlot(true); break;//not a final choice
       case FLOOD_F_IS_ON: optFloodSlot(true); break;
       case SET_DIR_IS_ON: optSetDirSlot(true); break;
       default : outLayerContent.append(DEFAULT_EXT);
   }
   outputLayerLine->setText(outLayerContent);
}

void EDAPluginMainForm::optAspectSlot( bool isOn )
{
   if(isOn) //use only with aspect chosen
   {
       //one and only one of those should be true at any time
       operation = ASPECT__IS_ON;
       
       //set other input widgets acording to this option
       
       //flow layer options
       inputFlowLayerCombo->setEnabled(false);
       inputFlowLayerCombo->setShown(false);
       inputFlowLayerLable->setEnabled(false);
       inputFlowLayerLable->setShown(false);
       inputFlowLayerPane->setEnabled(false);
       inputFlowLayerPane->setShown(false);
       yes->setEnabled(false);
       yes->setShown(false);
       no->setEnabled(false);
       no->setShown(false);
       useFlowLayer->setEnabled(false);
       useFlowLayer->setShown(false);
       //slope options
       degrees->setEnabled(false);
       degrees->setShown(false);
       percent->setEnabled(false);
       percent->setShown(false);
       slopeUnit->setEnabled(false);
       slopeUnit->setShown(false);
       
       //set output suggestion acoording to this option
       
       //remove the other 5 extentions if present
       outLayerContent.remove(SLOPE_P_EXT); 
       outLayerContent.remove(SLOPE_D_EXT); 
       outLayerContent.remove(FLOOD_F_EXT);
       outLayerContent.remove(SET_DIR_EXT);
       outLayerContent.remove(DEFAULT_EXT);
       //appent this operations extention if it is not already there
       if(!outLayerContent.contains(ASPECT__EXT, false)) 
           outLayerContent.append(ASPECT__EXT);
       outputLayerLine->setText(outLayerContent);
   }//if aspect changed to chosen
}


void EDAPluginMainForm::optSlopeDegreesSlot( bool isOn )
{
   if(isOn)//use only with slope degrees chosen
   {
       //one and only one of those should be true at any time    
       operation = SLOPE_D_IS_ON;
       
       //set other input widgets acording to this option
       
       //flow layer options
       inputFlowLayerCombo->setEnabled(false);
       inputFlowLayerCombo->setShown(false);
       inputFlowLayerLable->setEnabled(false);
       inputFlowLayerLable->setShown(false);
       inputFlowLayerPane->setEnabled(false);
       inputFlowLayerPane->setShown(false);
       yes->setEnabled(false);
       yes->setShown(false);
       no->setEnabled(false);
       no->setShown(false);
       useFlowLayer->setEnabled(false);
       useFlowLayer->setShown(false);
       
       //set output suggestion acoording to this option
       
       //remove the other 5 extentions if present  
       outLayerContent.remove(SLOPE_P_EXT); 
       outLayerContent.remove(ASPECT__EXT); 
       outLayerContent.remove(FLOOD_F_EXT);
       outLayerContent.remove(SET_DIR_EXT);
       outLayerContent.remove(DEFAULT_EXT);
       //appent this operations extention if it is not already there
       if(!outLayerContent.contains(SLOPE_D_EXT, false)) 
           outLayerContent.append(SLOPE_D_EXT);
       outputLayerLine->setText(outLayerContent);
   }//if user chose slope in degrees
}


void EDAPluginMainForm::optSetDirSlot( bool isOn )
{    
   if(isOn)//use only with set flow dir has been chosen
   {
       //one and only one of those should be true at any time
       operation = SET_DIR_IS_ON;
       
       //set other input widgets acording to this option
       
       //flow layer options
       inputFlowLayerCombo->setEnabled(false);
       inputFlowLayerCombo->setShown(false);
       inputFlowLayerLable->setEnabled(false);
       inputFlowLayerLable->setShown(false);
       inputFlowLayerPane->setEnabled(false);
       inputFlowLayerPane->setShown(false);
       useFlowLayer->setEnabled(true);
       useFlowLayer->setShown(true);
       yes->setEnabled(true);
       yes->setShown(true);
       no->setEnabled(true);
       no->setShown(true);

       //slope options
       degrees->setEnabled(false);
       degrees->setShown(false);
       percent->setEnabled(false);
       percent->setShown(false);
       slopeUnit->setEnabled(false);
       slopeUnit->setShown(false);
       
       //set output suggestion acoording to this option
       
       //remove the other 5 extentions if present
       outLayerContent.remove(SLOPE_P_EXT); 
       outLayerContent.remove(SLOPE_D_EXT); 
       outLayerContent.remove(FLOOD_F_EXT);
       outLayerContent.remove(ASPECT__EXT);
       outLayerContent.remove(DEFAULT_EXT);
       //appent this operations extention  if it is not already there
       if(!outLayerContent.contains(SET_DIR_EXT, false)) 
           outLayerContent.append(SET_DIR_EXT);
       outputLayerLine->setText(outLayerContent);
   }//if user chose set flow dir
}


void EDAPluginMainForm::optSlopePercentSlot( bool isOn )
{  
   if(isOn)//use only with slope percent chosen
   {
       //one and only one of those should be true at any time
       operation = SLOPE_P_IS_ON;
       
       //set other input widgets acording to this option
       
       //flow layer options
       inputFlowLayerCombo->setEnabled(false);
       inputFlowLayerCombo->setShown(false);
       inputFlowLayerLable->setEnabled(false);
       inputFlowLayerLable->setShown(false);
       inputFlowLayerPane->setEnabled(false);
       inputFlowLayerPane->setShown(false);
       yes->setEnabled(false);
       yes->setShown(false);
       no->setEnabled(false);
       no->setShown(false);
       useFlowLayer->setEnabled(false);
       useFlowLayer->setShown(false);
       
       //set output suggestion acoording to this option
         
       //remove the other 5 extentions if present
       outLayerContent.remove(ASPECT__EXT); 
       outLayerContent.remove(SLOPE_D_EXT); 
       outLayerContent.remove(FLOOD_F_EXT);
       outLayerContent.remove(SET_DIR_EXT); 
       outLayerContent.remove(DEFAULT_EXT);
       //appent this operations extention if it is not already there
       if(!outLayerContent.contains(SLOPE_P_EXT, false)) 
           outLayerContent.append(SLOPE_P_EXT);
       outputLayerLine->setText(outLayerContent);
   }//if user chose slope in percent
}


void EDAPluginMainForm::optFloodSlot( bool isOn )
{
   if(isOn)//use this only with flood has been chosen
   {
        //one and only one of those should be true at any time    
       operation = FLOOD_F_IS_ON; 
       
       //set other input widgets acording to this option
       
       //flow layer options
       inputFlowLayerCombo->setEnabled(false);
       inputFlowLayerCombo->setShown(false);
       inputFlowLayerLable->setEnabled(false);
       inputFlowLayerLable->setShown(false);
       inputFlowLayerPane->setEnabled(false);
       inputFlowLayerPane->setShown(false);
       useFlowLayer->setEnabled(true);
       useFlowLayer->setShown(true);
       yes->setEnabled(true);
       yes->setShown(true);
       no->setEnabled(true);
       no->setShown(true);

       //slope options
       degrees->setEnabled(false);
       degrees->setShown(false);
       percent->setEnabled(false);
       percent->setShown(false);
       slopeUnit->setEnabled(false);
       slopeUnit->setShown(false);
       
       //set output suggestion acoording to this option
       
       //remove the other 5 extentions if present
       outLayerContent.remove(SLOPE_P_EXT); 
       outLayerContent.remove(SLOPE_D_EXT); 
       outLayerContent.remove(ASPECT__EXT);
       outLayerContent.remove(SET_DIR_EXT);
       outLayerContent.remove(DEFAULT_EXT);
       //appent this operations extention if it is not already there
      if(!outLayerContent.contains(FLOOD_F_EXT, false)) 
           outLayerContent.append(FLOOD_F_EXT);
       outputLayerLine->setText(outLayerContent);
   }//if user chose flood
}


void EDAPluginMainForm::optSlopeGenSlot( bool isOn )
{
  if(isOn) //use only with aspect chosen
   {
       //one and only one of those should be true at any time
       operation = SLOPE_GEN_ON;
       
       //set other input widgets acording to this option
       
       //flow layer options
       inputFlowLayerCombo->setEnabled(false);
       inputFlowLayerCombo->setShown(false);
       inputFlowLayerLable->setEnabled(false);
       inputFlowLayerLable->setShown(false);
       inputFlowLayerPane->setEnabled(false);
       inputFlowLayerPane->setShown(false);
       yes->setEnabled(false);
       yes->setShown(false);
       no->setEnabled(false);
       no->setShown(false);
       useFlowLayer->setEnabled(false);
       useFlowLayer->setShown(false);
       //slope options
        slopeUnit->setEnabled(true);
       slopeUnit->setShown(true);
       degrees->setEnabled(true);
       degrees->setShown(true);
       percent->setEnabled(true);
       percent->setShown(true);
       
       //set output suggestion acoording to this option
       
       //remove the other 4 extentions if present
       outLayerContent.remove(ASPECT__EXT);
       outLayerContent.remove(FLOOD_F_EXT);
       outLayerContent.remove(SET_DIR_EXT);
       outLayerContent.remove(DEFAULT_EXT);
   }//if slope was changed to chosen
}


void EDAPluginMainForm::optYesFlowSlot( bool isOn )
{
  if(isOn) //use only with yes chosen
   {   
       //set other input widgets acording to this option
       
       //flow layer options
       inputFlowLayerPane->setEnabled(true);
       inputFlowLayerPane->setShown(true);
       inputFlowLayerCombo->setEnabled(true);
       inputFlowLayerCombo->setShown(true);
       inputFlowLayerLable->setEnabled(true);
       inputFlowLayerLable->setShown(true);

       //slope options
       degrees->setEnabled(false);
       degrees->setShown(false);
       percent->setEnabled(false);
       percent->setShown(false);
       slopeUnit->setEnabled(false);
       slopeUnit->setShown(false);
       
       //set output suggestion acoording to this option

   }//if user chose to use a flow file
}


void EDAPluginMainForm::optNoFlowSlot( bool isOn )
{
  if(isOn) //use only with no chosen
   {   
       //set other input widgets acording to this option
       
       //flow layer options
       inputFlowLayerCombo->setEnabled(false);
       inputFlowLayerCombo->setShown(false);
       inputFlowLayerLable->setEnabled(false);
       inputFlowLayerLable->setShown(false);
       inputFlowLayerPane->setEnabled(false);
       inputFlowLayerPane->setShown(false);

       //slope options
       degrees->setEnabled(false);
       degrees->setShown(false);
       percent->setEnabled(false);
       percent->setShown(false);
       slopeUnit->setEnabled(false);
       slopeUnit->setShown(false);
       
       //set output suggestion acoording to this option

   }//if user chose not to use a flow file
}

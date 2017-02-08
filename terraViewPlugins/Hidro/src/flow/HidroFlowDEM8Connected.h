/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroFlowDEM8Connected.h
*
*******************************************************************************
*
* $Rev: 7810 $:
*
* $Author: eric $:
*
* $Date: 2009-04-24 15:33:33 +0300 (pe, 24 huhti 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Flow Group.
	@defgroup hidroFlow The Group for Plugin Flow Classes.
*/

#ifndef HIDRO_FLOW_DEM_8CONNECTED_H_
#define HIDRO_FLOW_DEM_8CONNECTED_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/

class TeRaster;

class HidroGraph;
class HidroVertex;
class HidroEdge;


/** 
  * \file HidroFlowDEM8Connected.h
  *
  *
  * \brief This file is used to extract flow from DEM
           and extrat graph from flow;
  *
  * In this file has the implementation for methods to
  * extract a LDD from a regular grid, this grid can be
  * a raster from elevation, ...
  * The function generateFlow is used to get a input
  * raster (regular grid) is used a mask 3x3 to generate
  * the output raster flow.
  * The function generateGraph is used to generate a graph
  * in memory from a raster flow.
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroFlow
 */

/** \brief Function used to generate a LDD raster from a input raster
    \param	inputRaste		Input parameter, terralib raster attribute, this raster could be any regular grid 
	\param	outputRaster	Output parameter, terralib raster attribute, this raster is a flow grid
	\return					True if the output raster was generated correctly and false in other case
    */
bool generateFlow(TeRaster* inputRaster, TeRaster*& outputRaster);

/** \brief Function used to generate a LDD multi flow raster from a input raster
    \param	inputRaste		Input parameter, terralib raster attribute, this raster could be any regular grid 
	\param	outputRaster	Output parameter, terralib raster attribute, this raster is a flow grid
	\return					True if the output raster was generated correctly and false in other case
    */
bool generateMultiFlow(TeRaster* inputRaster, TeRaster*& outputRaster);


/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_FLOW_DEM_8CONNECTED_H_
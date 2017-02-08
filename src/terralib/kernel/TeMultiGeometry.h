/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright � 2001-2007 INPE and Tecgraf/PUC-Rio.

This code is part of the TerraLib library.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

You should have received a copy of the GNU Lesser General Public
License along with this library.

The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The library provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use
of this library and its documentation.
*************************************************************************************/
/*! \file TeMultiGeometry.h
    \brief This file contains structures and definitions to deal with a multigeometry. 
*/

#ifndef  __TERRALIB_MULTIGEOMETRY_H
#define  __TERRALIB_MULTIGEOMETRY_H

#include "TeGeometry.h"


class TL_DLL TeMultiGeometry
{

public:
	TePolygonSet		polygons_;
	TeLineSet			lines_;
	TePointSet			points_;
	TeCellSet			cells_;
	TeTextSet			texts_;
	
	//! Constructor
	TeMultiGeometry() {};

	//! Copy constructor
	TeMultiGeometry(const TeMultiGeometry& other);
	
	//! Assignment operator
	TeMultiGeometry& operator= (const TeMultiGeometry& other);
	
	//! Get a copy of polygon geometries. Return true if the geometries exit.  
	bool getGeometry(TePolygonSet& result) const; 
	
	//! Get a copy of line geometries. Return true if the geometries exit.  
	bool getGeometry(TeLineSet& result) const;
	
	//! Get a copy of point geometries. Return true if the geometries exit.  
	bool getGeometry(TePointSet& result) const;
	
	//! Get a copy of cell geometries. Return true if the geometries exit.  
	bool getGeometry(TeCellSet& result) const;

	//! Get a copy of text geometries. Return true if the geometries exit.  
	bool getGeometry(TeTextSet& result) const;
	
	//! Get a reference to the polygon geometry container  
	TePolygonSet&  getPolygons() 
	{	return polygons_; }
	
	//! Get a reference to the line geometry container  
	TeLineSet&  getLines() 
	{	return lines_; }

	//! Get a reference to the point geometry container  
	TePointSet&  getPoints() 
	{	return points_; }

	//! Get a reference to the cell geometry container  
	TeCellSet&  getCells() 
	{	return cells_; }

	//! Get a reference to the text geometry container  
	TeTextSet&  getTexts() 
	{	return texts_; }

	//! Get all geometries. Return true if the geometries exit.  
	bool getGeometry(vector<TeGeometry*>& result);

	//! Set polygon geometries
	void setGeometry(const TePolygonSet& result);
	
	//! Set line geometries
	void setGeometry(const TeLineSet& result);
	
	//! Set point geometries
	void setGeometry(const TePointSet& result);
	
	//! Set cell geometries
	void setGeometry(const TeCellSet& result);
	
	//! Set text geometries
	void setGeometry(const TeTextSet& result); 
	
	//! Set all geometries
	void setGeometry(vector<TeGeometry*>& result); 

	//! Add polygon geometry 
	void addGeometry(const TePolygon& poly); 
	
	//! Add line geometry
	void addGeometry(const TeLine2D& line); 
	
	//! Add point geometry
	void addGeometry(const TePoint& point); 
	
	//! Add cell geometry
	void addGeometry(const TeCell& cell); 

	//! Add cell geometry
	void addGeometry(const TeText& text); 

	//! Return true if this has polygons 
	bool hasPolygons()	const
	{ return (!polygons_.empty()); } 
	
	//! Return true if this has lines 
	bool hasLines() const		
	{ return (!lines_.empty()); } 
	
	//! Return true if this has points 
	bool hasPoints() const		
	{ return (!points_.empty()); } 
	
	//! Return true if this has cells 
	bool hasCells()	const
	{ return (!cells_.empty()); } 
	
	//! Return true if this has cells 
	bool hasTexts()	const	
	{ return (!texts_.empty()); } 
	
	//! Return true if there are not any geometry  
	bool empty()  
	{ 
		return ( polygons_.empty() &&  lines_.empty() &&
				 points_.empty()   &&  cells_.empty() &&  texts_.empty());
	}

	//! Clear geometries
	void clear();

	//! Get the total extension of the multi-geometry
	TeBox getBox();

	//! Get the total extension of the multi-geometry
	TeBox box()
	{ return this->getBox(); } 
};

#endif 

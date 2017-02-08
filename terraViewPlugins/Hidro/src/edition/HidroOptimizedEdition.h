/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroOptmizedEdition.h
*
*******************************************************************************
*
* $Rev: 7906 $:
*
* $Author: eric $:
*
* $Date: 2009-06-15 20:45:07 +0300 (ma, 15 kesä 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Edition Group.
	@defgroup hidroEdition The Group for Plugin Edition Classes.
*/

#ifndef HIDRO_OPTIMIZED_EDITION_H_
#define HIDRO_OPTIMIZED_EDITION_H_

/** 
  * \file HidroOptmizedEdition.h
  *
  * \class HidroOptmizedEdition
  *
  * \brief This file is a class for optmized graph edge edition
  *
  * The edition operations defined for hidro will be used from class HidroEdgeEdition
  * This class will be used to turn faster the edge creation process.
  *
  * \sa HidroGraphEdition.h, HidroEdgeEdition.h
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroEdition
 */



/*
** ----------------------------------------------------------------------------
** Includes:
*/

#include <string>

#include <TeCoord2D.h>
#include <TeGeometry.h>

class TeTheme;
class TeDatabase;
class TeQtGrid;

struct HidroOptEditionDirections
{
	TePoint ptUpper, ptUpperRight, ptRight, ptLowerRight, ptLower, ptLowerLeft, ptLeft, ptUpperLeft;
	bool hasPtUpper, hasPtUpperRight, hasPtRight, hasPtLowerRight, hasPtLower, hasPtLowerLeft, hasPtLeft, hasPtUpperLeft;

	HidroOptEditionDirections()
	{
		hasPtUpper = false;
		hasPtUpperRight = false;
		hasPtRight = false;
		hasPtLowerRight = false;
		hasPtLower = false;
		hasPtLowerLeft = false;
		hasPtLeft = false;
		hasPtUpperLeft = false;
	}
};

class HidroOptimizedEdition
{
public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor.
	*/
	HidroOptimizedEdition(TeDatabase* database, const std::string& graphName);

	/** \brief Virtual destructor.
	*/
	~HidroOptimizedEdition();

	//@}

public:

	/** @name Behavioral Methods
		* Methods related to hidro edge behavior.
		*/
	//@{

    /** \brief Function used to get the error messagem from edge edition
        \return		Return the string with the error message from edge edition operation
      */
	std::string getErrorMessage() const;

	bool getValidDirections(const TeCoord2D& coord, HidroOptEditionDirections& optEdDirections);

	bool createEdge(TeTheme* edgeTheme, TeTheme* vertexTheme, TeQtGrid* grid, 
					const std::string& vertexFromId, const std::string& vertexToId, const std::string& name, const bool& autoCreateAttrs);

	bool hasEdge(const std::string& vertexNameFrom, const std::string& vertexNameTo, bool& hasEdge);

protected:

	bool hasVertexOutputEdges(const std::string& vertexName, bool& hasEdges, std::string& edgeName);

	bool hasVertexInputEdges(const std::string& vertexNameFrom, const std::string& vertexNameTo, bool& hasEdges, std::string& edgeName);

	int getIndexForMaxCooordInY(TePointSet& ps, const double& yReference, const double& precision);
	
	int getIndexForMinCooordInY(TePointSet& ps, const double& yReference, const double& precision);

	int getIndexForMaxCooordInX(TePointSet& ps, const double& xReference, const double& precision);
	
	int getIndexForMinCooordInX(TePointSet& ps, const double& xReference, const double& precision);

	//@}

protected:

	std::string		_errorMessage;		//!< Attribute used to define a error message in edge edition

	std::string		_graphName;

	TeDatabase*		_db;				//!< Pointer to current database
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_OPTIMIZED_EDITION_H_
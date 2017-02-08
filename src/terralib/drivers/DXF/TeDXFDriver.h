/*********************************
 * FUNCATE - GIS development team
 *
 * TerraLib INPE
 *********************************/
/** @file TeDXFDriver.h
 *  @brief This files contains a definition for a driver for DXF files. 
 * The driver is used to read from a file.
 *  @version 1.0
 *  @author Frederico Augusto Bedê
 *  @date 11/2009
 */
#ifndef _TEDXFDRIVER_H
#define _TEDXFDRIVER_H

#include "tedxfdefines.h"
#include "TeGeoDataDriver.h"
#include <string>
using namespace std;
#include "TeBox.h"
#include "TeDataTypes.h"
#include "TeAttribute.h"

class TeProjection;
class TeSTElementSet;
class TeDXFLayer;
class TeDXFVisitor;

/** @class TeDXFDriver
 *  @brief Concrete implementation of a driver for DXF files. 
 */
class TEDXF_DLL TeDXFDriver : public TeGeoDataDriver 
{
  public:
    /** @brief 
         */
        TeDXFDriver(const string & fileName);

    /** @brief 
         */
        ~TeDXFDriver();

    //! Returns the complete access path associated to this driver
    virtual std::string getFileName();

    //! Returns true or false whether the data file is accessible
    virtual bool isDataAccessible();

    //! Returns the spatial reference associated to the data as an instance of TeProjection 
    virtual TeProjection * getDataProjection();

    //! Loads the minimum metadata information about the data
    //
    //		\param nObjects to return the number of objects accessible in the data set
    //		\param ext	to return the spatial extension of the geometries in the data set
    //		\param repres to return the type of geometries in the file
    //		\return true if or false whether it can retrieve the information or not
    //	
    virtual bool getDataInfo(unsigned int & nObjects, TeBox & ext, TeGeomRep & repres);

    //! Loads the list of descriptive attributes of the objetcts represented in the file
    virtual bool getDataAttributesList(TeAttributeList & attList);

    //! Loads the data into an TeSTElementSet structure in memory
    //
    //		\param dataSet to return data set
    //		\return true if or false whether it can retrieve the information or not
    //	
    virtual bool loadData(TeSTElementSet * dataSet);


  protected:
    string fileName_; //!< Complete file name, including teh complete path and its extension.    

    TeBox box_; //!< 

    unsigned int numObjs_; //!< 

    TeGeomRep geomType_; //!< 


  private:
    /** @brief No copy allowed.    
         */
        TeDXFDriver(const TeDXFDriver & source);

    /** @brief No copy allowed.    
         */
        TeDXFDriver & operator=(const TeDXFDriver & source);

};
#endif

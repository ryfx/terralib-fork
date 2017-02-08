#ifndef __TERRALIB_INTERNAL_GEOPROCESSING_H
#define __TERRALIB_INTERNAL_GEOPROCESSING_H 

#include <vector>
#include <string>

using std::vector;
using std::string;

#include <TeDataTypes.h>

class TeAsciiFile;
class TeTheme;
class TeLayer;


/** \param newLayer			pointer to a valid layer already created in the database to store the result of the operation
	\param theme			pointer to the inout theme
	\param themeTrim		pointer to the overlay theme
	\param selOb			a selection of the objects of the input theme
	\param selObTrim		a selection of the objects of the overlay theme
	\param logFile			Optional: a pointer to a text file where error messages will be saved
	\return true if it succeed and false otherwise
*/
bool TeGeoOpOverlayUnion(TeLayer* newLayer, TeTheme* theme, TeTheme* themeOverlay, TeSelectedObjects selObj, TeSelectedObjects selObjOverlay, TeAsciiFile* logFile);

#endif
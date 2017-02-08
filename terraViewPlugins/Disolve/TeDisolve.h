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

bool TeOpDisolve(TeLayer* newLayer, TeTheme* theme, TeSelectedObjects selObj);

#endif
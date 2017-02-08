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
/*! \file TeUpdateDBVersion.h
    \brief This file contains the routines to update a TerraLib database to its different versions
*/

#ifndef  __TERRALIB_INTERNAL_UPDATEDBVERSION_H
#define  __TERRALIB_INTERNAL_UPDATEDBVERSION_H

#include "../kernel/TeDatabase.h"
#include "TeUtilsDefines.h"

//! verify if a version is lower than othen version 
TLUTILS_DLL bool isLowerVersion(const std::string& version1, const std::string& version2);

//! verify if the database needs update 
TLUTILS_DLL bool needUpdateDB(TeDatabase* db, string& DBversion);

//! update database version 
TLUTILS_DLL bool updateDBVersion(TeDatabase* db, string& DBversion, string& errorMessage);

//! update database from release 2.0 to release 3.0
TLUTILS_DLL bool updateDB20To30(TeDatabase* db, string& errorMessage);

//! update database from release 3.0 to release 3.0.1
TLUTILS_DLL bool updateDB30To301(TeDatabase* db, string& errorMessage);

//! update database from release 3.0.1 to release 3.0.2 (3.0 Plus)
TLUTILS_DLL bool updateDB301To302(TeDatabase* db, string& errorMessage); 
 
//! update database from release 3.0.2 to release 3.1.0
TLUTILS_DLL bool updateDB302To310(TeDatabase* db, string& errorMessage); 

//! update database from release 3.1.0 to release 3.1.1
TLUTILS_DLL bool updateDB310To311(TeDatabase* db, string& errorMessage); 

//! update database from release 3.1.1 to release 3.2.0
TLUTILS_DLL bool updateDB311To320(TeDatabase* db, string& errorMessage); 

//! update database from release 3.2.0 to release 3.2.0.1
TLUTILS_DLL bool updateDB320To3201(TeDatabase* db, string& errorMessage);

//! update database from release 3.2.0.1 to release 3.3.1
TLUTILS_DLL bool updateDB3201To331(TeDatabase* db, string& errorMessage);

//! update database from release 3.3.1 to release 4.0.0
TLUTILS_DLL bool updateDB331To400(TeDatabase* db, string& errorMessage);

//! update database from release 4.0.0 to release 4.1.2
TLUTILS_DLL bool updateDB400To412(TeDatabase* db, string& errorMessage);

#endif


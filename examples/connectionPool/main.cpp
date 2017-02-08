/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright 2001-2007 INPE and Tecgraf/PUC-Rio.

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

/*! \file main.cpp
    \brief This file contains a set of examples that use the Connection Pool.
    \author Douglas Uba <douglas@dpi.inpe.br>
*/

// TerraLib
#include "../../src/terralib/kernel/TeConnection.h"
#include "../../src/terralib/drivers/PostgreSQL/TePostgreSQLConnection.h"
#include "../../src/terralib/kernel/TeConnectionPool.h"
#include "../../src/terralib/kernel/TeDatabase.h"
#include "../../src/terralib/drivers/PostgreSQL/TePostgreSQL.h"
#include "../../src/terralib/kernel/TeThread.h"

// STL
#include <iostream>

#include <omp.h>

int main()
{
    // Number of connections
    unsigned int nConn = 1;

    // Connection params
    std::string user = "postgres";
    std::string password = "mypassword";
    std::string host = "localhost";
    std::string dbName = "uba";
    std::string dbmns = "PostgreSQL";

    // Initializes the pool of connections
    TeConnectionPool* pool = new TeConnectionPool(nConn);
    if(!pool->initialize(user, password, host, dbName, dbmns, 5432))
    {
         std::cout << "Error initializing pool of connections." << std::endl;
         std::cin.get();
         return 0;
    }
    
    std::cout << "- Testing with threads..." << std::endl;
    // Test with threads
    #pragma omp parallel
    {
        // Obtain thread number
        int tid = omp_get_thread_num();
        TeConnection* c = pool->getConnection();
        TeDelayThread( 3 );
        pool->releaseConnection(c);
    }
 
    std::cout << "- Testing getConnection()..." << std::endl;
    TeConnection* c1 = pool->getConnection();
    TeConnection* c2 = pool->getConnection();
    // Here, c2 should be NULL
    pool->releaseConnection(c1);

    std::cout << "- Testing database connections..." << std::endl;

    // Number of test iterations
    unsigned int iterations = 100;
    TeDatabase* db = new TePostgreSQL;
    for(unsigned int i = 0; i < iterations; ++i)
    {
        TeConnection* c = pool->getConnection();
        if(c == 0)
            continue;

        db->setConnection(c);
        db->loadLayerSet();

        TeLayerMap& layerMap = db->layerMap();
        TeLayerMap::iterator it;
        for(it = layerMap.begin(); it != layerMap.end(); ++it)
            std::cout << "Layer: " << it->second->name() << std::endl;

        pool->releaseConnection(c);
    }

    delete db;
    delete pool;

    std::cout << "end!" << std::endl;
    std::cin.get();

	return 0;
}
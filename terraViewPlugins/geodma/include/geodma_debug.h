#ifndef __GEODMA_DEBUG_H_
#define __GEODMA_DEBUG_H_

#include <TeUtils.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#define DEBUG_PATH "c:/debug.txt"

using namespace std;

class geodma_debug
{
  private:

    map< string, time_t > execution_times;
    unsigned level;

  public:

    /* Constructor, opens the debug file, located at DEBUG_PATH */
    geodma_debug( bool append = false );
    ~geodma_debug();

    /* Prints some line in the debug file */
    void print( string line );

    /* Defines the start of a function */
    void start( string function );

    /* Defines the end of a function, print name and execution time */
    void finish( string function );
};

#endif //__GEODMA_DEBUG_H_
#include <geodma_debug.h>

geodma_debug::geodma_debug( bool append )
{
  level = 0;
  ofstream debug_file;
  if ( append )
    debug_file.open( DEBUG_PATH, ofstream::out | ofstream::app );
  else
    debug_file.open( DEBUG_PATH );
  debug_file << " " << endl;
  debug_file.close();
}

geodma_debug::~geodma_debug()
{
}

void geodma_debug::print( string line )
{
  ofstream debug_file;
  debug_file.open( DEBUG_PATH, ofstream::out | ofstream::app );
  debug_file << line << endl;
  debug_file.close();

  cout << line << endl;
}

void geodma_debug::start( string function )
{
  string function_index = Te2String( level ) + function;
  execution_times[ function_index ] = time( NULL );
  level++; 

  ofstream debug_file;
  if ( function == "init()" )
    debug_file.open( DEBUG_PATH, ofstream::out );
  else
    debug_file.open( DEBUG_PATH, ofstream::out | ofstream::app );
  debug_file << string( level * 3, ' ' ) << function << endl;
  debug_file.close();

  for( unsigned i = 0; i < level; i++ ) 
    cout << "   "; 
  cout << function << endl;

}

void geodma_debug::finish( string function )
{
  string function_index = Te2String( level ) + function;
  execution_times[ function_index ] -= time( NULL );

  ofstream debug_file;
  debug_file.open( DEBUG_PATH, ofstream::out | ofstream::app);
  debug_file << string( level * 3, ' ' ) << "end_" << function << " - " << -execution_times[ function ] << "s" << endl;
  debug_file.close();

  for( unsigned i = 0; i < level; i++ ) 
    cout << "   "; 
  level--; 
  cout << "end_" << function << " after " << -execution_times[ function_index ] << " seconds" << endl;
  execution_times[ function_index ] = 0;
  if ( level == 0 )
    cout << endl;
}

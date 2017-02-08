#include <HidroSmartQueue.h>

HidroSmartQueue::HidroSmartQueue()
{
  setMaxMemPercentUsage( MAXMEMPERCENTUSAGE );
}

HidroSmartQueue::~HidroSmartQueue()
{
  // libera memória dos arquivos
  while( !queue_file_.empty() )
  {
    char *name = queue_file_.front();
    remove( name );
    free( name );
    queue_file_.pop();    
  }
  // libera memória da Front
  while( !queue_front_.empty() )
  {
    int *v = queue_front_.front();
    delete[4] v;
    queue_front_.pop();    
  }

  // libera memória da Back
  while( !queue_back_.empty() )
  {
    int *v = queue_back_.front();
    delete[4] v;
    queue_back_.pop();    
  }
}

unsigned int 
HidroSmartQueue::setMaxMemPercentUsage( double maxMemPercentUsage )
{
  // calcula tamanho máximo da fila em memória
  unsigned long int free_vm = MIN( TeGetFreeVirtualMemory(), TeGetTotalPhysicalMemory() );

  max_ram_  = unsigned int( (maxMemPercentUsage *  double(free_vm) ) / 100.0 );

  max_ram_ -= 16000; // quanto que ocupa so instanciar a HidroSmartQueue
  
  // cada elemento da minha fila ocupa + ou - 100 bytes
  // divido por 2 porque vou ter 2 filas de tamanho max_queue_size_
  max_queue_size_ = (max_ram_ / 100) / 2;

  return max_ram_;
}

bool
HidroSmartQueue::empty()
{
  if( size() )
    return false;

  return true;
}

size_t
HidroSmartQueue::size()
{
  return queue_front_.size() + queue_back_.size() + queue_file_.size()*max_queue_size_;
}

int* HidroSmartQueue::newPit()
{
  int *v = new int[3];
  
  v[0] = 0;
  v[1] = 0;
  v[2] = 0; // número de tratamentos  
  
  return v;
}

void HidroSmartQueue::newPit( unsigned int column, unsigned int line )
{
  int *v = new int[3];
  
  v[0] = column;
  v[1] = line;
  v[2] = 0; // número de tratamentos  

  push( v );
}

void HidroSmartQueue::newPit( unsigned int column, unsigned int line, unsigned int treatment )
{
  
  int *v = new int[3];
  
  v[0] = column;
  v[1] = line;
  v[2] = treatment; // número de tratamentos  

  push( v );
}


void
HidroSmartQueue::push( int* v )
{
  // Se tem Back
  if( queue_back_.size() )
  {
    // Se Back não ta cheia
    if( queue_back_.size() < max_queue_size_ )
    {
      queue_back_.push( v );
    }
    // Se Back ta cheia
    else
    {
      copyBack2File();       
      queue_back_.push( v );
    }
  }
  else
  {
    // Se tem File
    if( !queue_file_.empty() )
    {
      queue_back_.push( v );
    }
    else
    {
      // Se Front não ta cheia
      if( queue_front_.size() < max_queue_size_ )
      {
        queue_front_.push( v );
      }
      // Se Front ta cheia
      else
      {       
        queue_back_.push( v );
      }
    }
  }
}

void
HidroSmartQueue::pop()
{
  int *v = front();
  delete[3] v;

  // Se tem Front
  if( !queue_front_.empty() )
  {    
    queue_front_.pop();    
  }
  // Se tem File
  else if ( !queue_file_.empty() )
  {
    copyFile2Front();    
    queue_front_.pop();    
  }
  else
  {
    // So sobrou o Back
    queue_back_.pop();
  }
}

int*
HidroSmartQueue::front()
{
  // Se tem Front
  if( !queue_front_.empty() )
  {
    return queue_front_.front();
  }
  // Se tem File
  else if ( !queue_file_.empty() )
  {
    copyFile2Front();
    return queue_front_.front();
  }
  // So sobrou o Back
  return queue_back_.front();
}

void
HidroSmartQueue::copyBack2File()
{
  char *name = tempnam( 0, "Hidro_SmartQueue" );
  FILE *stream = fopen( name, "wb");

  while( queue_back_.size() )
  {
    int *v = queue_back_.front();    

    _putw( v[0], stream );
    _putw( v[1], stream );
    _putw( v[2], stream );    

    delete[3] v;    
    queue_back_.pop();
  }

  fclose( stream );
  queue_file_.push( name );
}

void
HidroSmartQueue::copyFile2Front()
{
  char *name = queue_file_.front();
  FILE *stream = fopen( name, "rb");
  rewind( stream );

  for(unsigned int i=0; i<max_queue_size_; i++)
  {
    int *v = new int[3];
    v[0] = _getw(stream) ;
    v[1] = _getw(stream) ;
    v[2] = _getw(stream) ;    
    
    queue_front_.push( v );
  }

  fclose( stream );
  remove( name );
  free( name );

  queue_file_.pop();  
}

void
HidroSmartQueue::sendFront2Back()
{
  int *v = front();
  push( v );

  // Se tem Front
  if( !queue_front_.empty() )
  {    
    queue_front_.pop();    
  }
  // Se tem File
  else if ( !queue_file_.empty() )
  {
    copyFile2Front();    
    queue_front_.pop();    
  }
  else
  {
    // So sobrou o Back
    queue_back_.pop();
  }
}

unsigned int
HidroSmartQueue::getMaxMem()
{
  return max_ram_;
}
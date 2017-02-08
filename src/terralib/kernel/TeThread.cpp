#include "TeThread.h"
#include "TeAgnostic.h"

TeThread::TeThread()
{
  threadStatus_ = TeThreadStopped;

  threadCurrPriority_ = TeThreadNormalPriority;

  #if TePLATFORM == TePLATFORMCODE_MSWINDOWS
    threadHandler_ = 0;
    threadId_ = 0;
  #elif TePLATFORM == TePLATFORMCODE_LINUX || TePLATFORM == TePLATFORMCODE_AIX || TePLATFORM == TePLATFORMCODE_APPLE
    threadId_ = 0;
  #else
    #error "ERROR: Unsupported platform"
  #endif 
}

TeThread::~TeThread()
{
	TEAGN_TRUE_OR_LOG( ( threadStatus_ == TeThreadStopped ),
		"Trying to delete a running thread handler instance" )
    
  TEAGN_TRUE_OR_THROW( waitToFinish(), "Error waiting thread to finish" );
}

bool TeThread::start()
{
  if( threadStatus_ != TeThreadStopped )
	{
    return false;
  }
	else
	{
    TEAGN_TRUE_OR_THROW( waitToFinish(), "Error waiting thread to finish" );
    
    threadStatus_ = TeThreadRunning;
    
    #if TePLATFORM == TePLATFORMCODE_MSWINDOWS
      threadHandler_ = CreateThread(NULL, 0, 
        (LPTHREAD_START_ROUTINE)TeThread::startThread,
        (LPVOID)(this), 0, (LPDWORD) &threadId_);
  
      if( threadHandler_ == 0 )
      {
        TEAGN_LOGERR( "Thread creation failed" );
        threadStatus_ = TeThreadStopped;
        return false;
      }
    #elif TePLATFORM == TePLATFORMCODE_LINUX || TePLATFORM == TePLATFORMCODE_AIX || TePLATFORM == TePLATFORMCODE_APPLE
      if( pthread_attr_init(&threadAttr_) != 0 )
      {
        TEAGN_LOGERR( "Unable to init thread attributes" );
        threadStatus_ = TeThreadStopped;
        return false;
      }      
        
      if( pthread_attr_setdetachstate( &threadAttr_, 
        PTHREAD_CREATE_JOINABLE ) != 0 )
      {
        TEAGN_LOGERR( "Unable to set thread detach attribute" );
        
        TEAGN_TRUE_OR_THROW( ( pthread_attr_destroy( &threadAttr_ ) == 0 ),
          "Unable to destroy thread attributes" )         
          
        threadStatus_ = TeThreadStopped;
        
        return false;
      }       
  
      if( pthread_create( &threadId_, &threadAttr_, 
        TeThread::startThread, (void *) this ) != 0 )
      {
        TEAGN_LOGERR( "Thread creation failed" );
        
        TEAGN_TRUE_OR_THROW( ( pthread_attr_destroy( &threadAttr_ ) == 0 ),
          "Unable to destroy thread attributes" )         
          
        threadStatus_ = TeThreadStopped;
        
        return false;
      }       
    #else
      #error "ERROR: Unsupported platform"
    #endif

    if( threadCurrPriority_ != TeThreadNormalPriority )
		{
      TEAGN_TRUE_OR_LOG( setPriority(threadCurrPriority_ ), 
        "Unable to set thread priority" );
    }

    return true;
  }
}


TeThreadStatus TeThread::getStatus() const
{
  return threadStatus_;
}

TeThreadPriority TeThread::getPriority() const
{
  return threadCurrPriority_;
}


bool TeThread::setPriority(TeThreadPriority newPriority)
{
  if( threadStatus_ == TeThreadRunning ) {
    #if TePLATFORM == TePLATFORMCODE_MSWINDOWS
      int new_prio_value = THREAD_PRIORITY_NORMAL;
      
      switch( newPriority ) {
        case TeThreadIdlePriority :
        {
          new_prio_value = THREAD_PRIORITY_IDLE;
          break;
        }
        case TeThreadBelowNormalPriority :
        {
          new_prio_value = THREAD_PRIORITY_BELOW_NORMAL;
          break;
        }
        case TeThreadNormalPriority :
        {
          new_prio_value = THREAD_PRIORITY_NORMAL;
          break;
        }
        case TeThreadAboveNormalPriority :
        {
          new_prio_value = THREAD_PRIORITY_ABOVE_NORMAL;
          break;
        }
        case TeThreadTimeCriticalPriority :
        {
          new_prio_value = THREAD_PRIORITY_TIME_CRITICAL;
          break;
        }
        default :
        {
          TEAGN_LOG_AND_THROW( "Invalid thread priority" )
          break;
        }                                      
      }
      
      if( SetThreadPriority( threadHandler_, new_prio_value ) ) {
        threadCurrPriority_ = newPriority;
        return true;
      } else {
        return false;
      }      
    #elif TePLATFORM == TePLATFORMCODE_LINUX || TePLATFORM == TePLATFORMCODE_AIX || TePLATFORM == TePLATFORMCODE_APPLE
      int curr_policy = 0;
      struct sched_param curr_sched_param; 
      
      TEAGN_TRUE_OR_THROW( ( pthread_getschedparam( threadId_, &curr_policy,
        &curr_sched_param ) == 0 ),
        "Unable to get thread scheduler parameters" );

	    int min_prio = sched_get_priority_min( curr_policy );
      int max_prio = sched_get_priority_max( curr_policy );
      
      switch( newPriority ) {
        case TeThreadIdlePriority :
        {
          curr_sched_param.sched_priority = min_prio;
          break;
        }
        case TeThreadBelowNormalPriority :
        {
          curr_sched_param.sched_priority = (int)
            ( ( max_prio - min_prio ) / 4 );
          break;
        }
        case TeThreadNormalPriority :
        {
          curr_sched_param.sched_priority = (int)
            ( 2 * ( max_prio - min_prio ) / 4 );
          break;
        }
        case TeThreadAboveNormalPriority :
        {
          curr_sched_param.sched_priority = (int)
            ( 3 * ( max_prio - min_prio ) / 4 );
          break;
        }
        case TeThreadTimeCriticalPriority :
        {
          curr_sched_param.sched_priority = max_prio;
          break;
        }
        default :
        {
          TEAGN_LOG_AND_THROW( "Invalid thread priority" )
          break;
        }                                      
      }
      
      if( pthread_setschedparam( threadId_, curr_policy,
        &curr_sched_param ) == 0 ) {
        
        threadCurrPriority_ = newPriority;
        return true;        
      } else {
        return false;
      }  
    #else
      #error "ERROR: Unsupported platform"
    #endif    
  } else {
    /* Thread not running */
    
    threadCurrPriority_ = newPriority;
    
    return true;
  }
}

bool TeThread::waitToFinish()
{
  #if TePLATFORM == TePLATFORMCODE_MSWINDOWS
    if( threadHandler_ != 0 ) 
    {
      TEAGN_TRUE_OR_RETURN( ( WaitForSingleObjectEx( 
        threadHandler_, INFINITE, false ) == WAIT_OBJECT_0 ),
        "Thread joinning failed" );
              
      TEAGN_TRUE_OR_THROW( CloseHandle( threadHandler_ ),
        "Error closing thread handle" );
        
      threadHandler_ = 0;
      threadId_ = 0;
    }
  #elif TePLATFORM == TePLATFORMCODE_LINUX || TePLATFORM == TePLATFORMCODE_AIX || TePLATFORM == TePLATFORMCODE_APPLE
    if( threadId_ != 0 ) 
    {
      // The return value should not be checked since it may
      // represent a join call to a thread that already finished execution
      pthread_join( threadId_, 0 );
        
      threadId_ = 0;
      
      pthread_attr_destroy( &threadAttr_ );           
    }
  #else
    #error "ERROR: Unsupported platform"
  #endif
  
  return true;
}

void* TeThread::startThread( void* threadPtr )
{
  TEAGN_DEBUG_CONDITION( ( threadPtr != 0 ),
    "Invalid thread parameter pointer" )

	TeThread* thread = (TeThread*)threadPtr;

	thread->run();
	thread->threadStatus_ = TeThreadStopped;
  
  #if TePLATFORM == TePLATFORMCODE_MSWINDOWS
  #elif TePLATFORM == TePLATFORMCODE_LINUX || TePLATFORM == TePLATFORMCODE_AIX || TePLATFORM == TePLATFORMCODE_APPLE
    pthread_exit(0);
  #else
    #error "ERROR: Unsupported platform"
  #endif  

	return 0;
}





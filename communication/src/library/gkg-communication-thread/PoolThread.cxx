#include <gkg-communication-thread/PoolThread.h>
#include <gkg-communication-thread/ThreadPool.h>


gkg::PoolThread::PoolThread( gkg::ThreadPool* parent )
               : gkg::Thread(),
                 _parent( parent )
{
}


void gkg::PoolThread::doRun()
{

  if ( _parent )
  {

    _parent->execute( this );

  }

}

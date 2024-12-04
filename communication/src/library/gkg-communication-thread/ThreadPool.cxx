#include <gkg-communication-thread/ThreadPool.h>
#include <gkg-communication-thread/PoolThread.h>
#include <gkg-communication-thread/Task.h>
#include <gkg-communication-sysinfo/SystemInfo.h>
#include <gkg-core-cppext/CUniStd.h>


gkg::ThreadPool::ThreadPool()
               : gkg::Singleton< gkg::ThreadPool >(),
                 _running( true )
{

  int32_t maxThreadCount = gkg::SystemInfo::getInstance().getCpuCount();

  while ( maxThreadCount-- )
  {

    gkg::PoolThread* thread = new gkg::PoolThread( this );

    if ( thread )
    {

      _threads.push_back( thread );
      thread->launch();

    }

  }

}


gkg::ThreadPool::~ThreadPool()
{

  if ( _running )
  {

    stop();

  }

}


void gkg::ThreadPool::execute( gkg::PoolThread* thread )
{

  while ( 1 )
  {

    _mutex.lock();

    while ( _running && _tasks.empty() )
    {

      _condition.wait( _mutex );

    }

    if ( !_running )
    {

      _mutex.unlock();

      if ( thread )
      {

        thread->exit();

      }

    }

    gkg::Task* task = _tasks.front();

    _tasks.pop();
    _mutex.unlock();
    task->run();
    _mutex.lock();

    std::map< int32_t, std::pair< int32_t, int32_t > >::iterator
      d = _done.find( task->getId() );

    if ( d != _done.end() )
    {

      d->second.second++;

    }

    _mutex.unlock();

    delete task;    

  }

}


void gkg::ThreadPool::addTask( gkg::Task* task )
{

  if ( task )
  {

    _mutex.lock();
    _tasks.push( task );
    _condition.signal();
    _mutex.unlock();

  }

}


void gkg::ThreadPool::addTask( std::list< gkg::Task* >& tasks )
{

  std::list< gkg::Task* >::iterator
    t = tasks.begin(),
    te = tasks.end();

  _mutex.lock();

  while ( t != te )
  {

    if ( *t )
    {

      _tasks.push( *t );
      _condition.signal();

    }

    ++t;

  }

  _mutex.unlock();

}


void gkg::ThreadPool::launch( gkg::Task* task, uint32_t microsecond )
{

  if ( task )
  {

    int32_t id = getTaskId();

    task->setId( id );
    _done[ id ].first = 1;
    addTask( task ); 
    wait( id, microsecond );

  }

}


void gkg::ThreadPool::launch( std::list< gkg::Task* >& tasks,
                              uint32_t microsecond )
{

  int32_t n = 0;
  int32_t id = getTaskId();
  std::list< gkg::Task* >::iterator
    t = tasks.begin(),
    te = tasks.end();

  while ( t != te )
  {

    if ( *t )
    {

      (*t)->setId( id );
      n++;

    }

    ++t;

  }

  if ( n )
  {

    _done[ id ].first = n;
    addTask( tasks );
    wait( id, microsecond );

  }
  else
  {

    _done.erase( id );

  }

}


void gkg::ThreadPool::stop()
{

  _mutex.lock();
  _running = false;
  _mutex.unlock();

  _condition.broadcast();

  std::vector< gkg::PoolThread* >::iterator
    t = _threads.begin(),
    te = _threads.end();

  while ( t != te )
  {

    (*t)->join();
    _condition.broadcast();
    delete *t;
    ++t;

  }

  while ( _tasks.size() )
  {

    delete _tasks.front();
    _tasks.pop();

  }

}


void gkg::ThreadPool::wait( int32_t id, uint32_t microsecond )
{

  if ( id >= 0 )
  {

    std::map< int32_t, std::pair< int32_t, int32_t > >::iterator
      d = _done.find( id );

    if ( d != _done.end() )
    {

      while ( !_tasks.empty() || ( d->second.second != d->second.first ) )
      {

        usleep( microsecond );

      }

      _mutex.lock();
      _done.erase( id );
      _mutex.unlock();

    }

  }

}


int32_t gkg::ThreadPool::getTaskId()
{

  int32_t id = 0;
  std::map< int32_t, std::pair< int32_t, int32_t > >::iterator
    de = _done.end();

  while ( _done.find( id ) != de )
  {

    id++;

  }

  _done.insert( std::make_pair( id, std::make_pair( 0, 0 ) ) );

  return id;

}

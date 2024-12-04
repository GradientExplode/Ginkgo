#include <gkg-core-pattern/Observable.h>
#include <gkg-core-pattern/Observer.h>
#include <iostream>


gkg::Observable::Observable()
                : _changed( false ),
                  _updating( false )
{
}


gkg::Observable::~Observable()
{

  if ( !_observers.empty() )
  {

    std::set< gkg::Observer* >::iterator o = _observers.begin(),
                                         oe = _observers.end();
    while ( o != oe )
    {

      ( *o )->unregisterObservable( this );
      ++ o;

    }
    _observers.clear();

  }

}


void gkg::Observable::addObserver( gkg::Observer* observer )
{

  if ( _updating )
  {

    _toBeAdded.insert( observer );
    _toBeRemoved.erase( observer );

  }
  else
  {

    _observers.insert( observer );

  }

}


void gkg::Observable::removeObserver( gkg::Observer* observer )
{


  if ( _updating )
  {

    _toBeRemoved.insert( observer );
    _toBeAdded.erase( observer );

  }
  else
  {

    _observers.erase( observer );

  }

}


void gkg::Observable::removeObservers()
{

  _observers.clear();

}


int32_t gkg::Observable::getObserverCount() const
{

  return ( int32_t )_observers.size();

}


void gkg::Observable::notifyObservers( void* arg )
{

  if ( _changed && !_updating )
  {

    // lock
    _updating = true;
    std::set< gkg::Observer* >::iterator o = _observers.begin(),
                                         oe = _observers.end();
    while ( o != oe )
    {

      if ( _toBeRemoved.find( *o ) == _toBeRemoved.end() )
      {

        ( *o )->update( this, arg );

      }
      ++ o;

    }

    // handle delayed insertions/deletions
    o = _toBeAdded.begin();
    oe = _toBeAdded.end();
    while ( o != oe )
    {

      _observers.insert( *o );
      ++ o;

    }
    _toBeAdded.clear();

    o = _toBeRemoved.begin();
    oe = _toBeRemoved.end();
    while ( o != oe )
    {

      _observers.erase( *o );
      ++ o;

    }
    _toBeRemoved.clear();

    _changed = false;
    _updating = false;

  }

}


void gkg::Observable::notifyUnregisterObservers()
{

  std::set< gkg::Observer* >::iterator o, oe = _observers.end();

  while ( !_observers.empty() )
  {

    o = _observers.begin();
    ( *o )->unregisterObservable( this );
    if ( _observers.find( *o ) != oe )
    {

      _observers.erase( o );

    }

  }

}


bool gkg::Observable::hasChanged() const
{

  return _changed;

}


void gkg::Observable::setChanged()
{

  _changed = true;

}


void gkg::Observable::clearChanged()
{

  _changed = false;

}


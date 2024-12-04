#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <list>


//
// class RCGUIObjectImplementation
//

namespace gkg
{


class RCGUIObjectImplementation
{

  friend class RCGUIObject;

  static bool _deferred;
  static std::list< const RCGUIObject* > _toBeDeleted;

};


}


bool gkg::RCGUIObjectImplementation::_deferred = false;
std::list< const gkg::RCGUIObject* >
  gkg::RCGUIObjectImplementation::_toBeDeleted;


//
// class RCGUIObject
//


gkg::RCGUIObject::RCGUIObject()
                 : _count( 0 )
{
}


gkg::RCGUIObject::RCGUIObject( const gkg::RCGUIObject& )
                 : _count( 0 )
{
}


gkg::RCGUIObject::~RCGUIObject()
{
}



gkg::RCGUIObject& gkg::RCGUIObject::operator=( const gkg::RCGUIObject& )
{

  return *this;

}


void gkg::RCGUIObject::addReference() const
{

  ++ _count;

}


void gkg::RCGUIObject::removeReference() const
{

  if ( _count )
  {

    -- _count;

  }
  if ( _count == 0 )
  {

    // uggly hack to be suppressed
    ( ( gkg::RCGUIObject* )this )->cleanup();
    delete this;

  }

}


void gkg::RCGUIObject::removeReferenceDeferred() const
{

  if ( _count )
  {

    -- _count;

  }
  if ( _count == 0 )
  {

    // uggly hack to be suppressed
    ( ( gkg::RCGUIObject* )this )->cleanup();

    if ( gkg::RCGUIObjectImplementation::_deferred )
    {

      gkg::RCGUIObjectImplementation::_toBeDeleted.push_back( this );

    }
    else
    {

      delete this;

    }

  }

}


void gkg::RCGUIObject::cleanup()
{
}


int gkg::RCGUIObject::getCount() const
{

  return _count;

}


bool gkg::RCGUIObject::defer( bool value )
{

  bool previous = gkg::RCGUIObjectImplementation::_deferred;
  if ( value != previous )
  {

    flush();
    gkg::RCGUIObjectImplementation::_deferred = value;

  }
  return previous;

}


void gkg::RCGUIObject::flush()
{

  if ( !gkg::RCGUIObjectImplementation::_toBeDeleted.empty() )
  {

    bool previous = gkg::RCGUIObjectImplementation::_deferred;
    gkg::RCGUIObjectImplementation::_deferred = false;
    std::list< const gkg::RCGUIObject* >::iterator
      o = gkg::RCGUIObjectImplementation::_toBeDeleted.begin(),
      oe = gkg::RCGUIObjectImplementation::_toBeDeleted.end();
    while ( o != oe )
    {

      delete *o;
      ++ o;

    }
    gkg::RCGUIObjectImplementation::_toBeDeleted.clear();
    gkg::RCGUIObjectImplementation::_deferred = previous;

  }

}

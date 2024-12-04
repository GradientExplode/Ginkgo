#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/HitImplementation.h>
#include <iostream>

gkg::Hit::Hit( const gkg::Event* event )
{

  _hitImplementation = new gkg::HitImplementation( event );

}


gkg::Hit::Hit( gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  _hitImplementation = new gkg::HitImplementation( x, y, x, y );

}


gkg::Hit::Hit( gkg::FloatCoordinate left,
               gkg::FloatCoordinate bottom,
               gkg::FloatCoordinate right,
               gkg::FloatCoordinate top )
{

  _hitImplementation = new gkg::HitImplementation( left, bottom, right,top );

}


gkg::Hit::~Hit()
{

  delete _hitImplementation;

}


const gkg::Event* gkg::Hit::getEvent() const
{

  return _hitImplementation->getEvent();

}


gkg::FloatCoordinate gkg::Hit::getLeft() const
{

  return _hitImplementation->getLeft();

}


gkg::FloatCoordinate gkg::Hit::getBottom() const
{

  return _hitImplementation->getBottom();

}


gkg::FloatCoordinate gkg::Hit::getRight() const
{

  return _hitImplementation->getRight();

}


gkg::FloatCoordinate gkg::Hit::getTop() const
{

  return _hitImplementation->getTop();

}


void gkg::Hit::pushTransform()
{

  _hitImplementation->pushTransform();

}


void gkg::Hit::setTransform( const gkg::Transformer& transformer )
{

  _hitImplementation->setTransform( transformer );

}


void gkg::Hit::popTransform()
{

  _hitImplementation->popTransform();

}


void gkg::Hit::begin( int32_t depth, gkg::Glyph* glyph,
                      gkg::Glyph::Index index,
                      gkg::EventHandler* eventHandler )
{

  _hitImplementation->begin( depth, glyph, index, eventHandler );

}


void gkg::Hit::setTarget( int32_t depth, gkg::Glyph* glyph,
                          gkg::Glyph::Index index,
                          gkg::EventHandler* eventHandler )
{

  _hitImplementation->setTarget( depth, glyph, index, eventHandler );

}


void gkg::Hit::end()
{

  _hitImplementation->end();

}


void gkg::Hit::remove( int32_t depth, gkg::Glyph::Index target )
{

  _hitImplementation->remove( depth, target );

}


void gkg::Hit::retarget( int32_t depth, gkg::Glyph* glyph,
                         gkg::Glyph::Index index,
                         gkg::EventHandler* eventHandler,
                         gkg::Glyph::Index target )
{

  _hitImplementation->addItem( true, depth, glyph, index,
                               eventHandler, target );

}


bool gkg::Hit::hasAny() const
{

  return _hitImplementation->hasAny();

}


int32_t gkg::Hit::getCount() const
{

  return _hitImplementation->getCount();

}


int32_t gkg::Hit::getDepth( gkg::Glyph::Index target ) const
{

  return _hitImplementation->getDepth( target );

}


gkg::Glyph* gkg::Hit::getTarget( int32_t depth, 
                                  gkg::Glyph::Index target ) const
{

  return _hitImplementation->getTarget( depth, target );

}


gkg::Glyph::Index gkg::Hit::getIndex( int32_t depth,
                                       gkg::Glyph::Index target  ) const
{

  return _hitImplementation->getIndex( depth, target );

}


gkg::EventHandler* gkg::Hit::getEventHandler() const
{

  return _hitImplementation->getEventHandler();

}


gkg::HitImplementation* gkg::Hit::getImplementation() const
{

  return _hitImplementation;

}

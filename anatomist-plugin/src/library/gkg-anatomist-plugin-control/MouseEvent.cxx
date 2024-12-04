#include <gkg-anatomist-plugin-control/MouseEvent.h>
#include <gkg-anatomist-plugin-server/AnatomistServer.h>

#include <anatomist/reference/Transformation.h>
#include <anatomist/mobject/MObject.h>
#include <anatomist/config/version.h>


gkg::MouseEvent::MouseEvent( const std::string& object )
               : _object( 
                   gkg::AnatomistServer::getInstance().getAnatomistObject( 
                                                                      object ) )
{
}


gkg::MouseEvent::MouseEvent( anatomist::AObject* object )
               : _object( object )
{
}


gkg::MouseEvent::~MouseEvent()
{
}


void gkg::MouseEvent::press( anatomist::AWindow* win, 
                             int32_t x, 
                             int32_t y )
{

  if ( win && _object )
  {

    Point3d pos;

    getPosition( win, x, y, pos );
    press( pos[ 0 ], pos[ 1 ], pos[ 2 ] );

  }
  else
  {

    press( x, y, 0 );

  }

}


void gkg::MouseEvent::move( anatomist::AWindow* win, 
                            int32_t x, 
                            int32_t y )
{

  if ( win && _object )
  {

    Point3d pos;

    getPosition( win, x, y, pos );
    move( pos[ 0 ], pos[ 1 ], pos[ 2 ] );

  }
  else
  {

    move( x, y, 0 );

  }

}


void gkg::MouseEvent::release( anatomist::AWindow* win, 
                               int32_t x, 
                               int32_t y )
{

  if ( win && _object )
  {

    Point3d pos;

    getPosition( win, x, y, pos );
    release( pos[ 0 ], pos[ 1 ], pos[ 2 ] );

  }
  else
  {

    release( x, y, 0 );

  }

}


void gkg::MouseEvent::press( int32_t /* x */, 
                             int32_t /* y */,
                             int32_t /* z */ )
{
}


void gkg::MouseEvent::move( int32_t /* x */, 
                            int32_t /* y */,
                            int32_t /* z */ )
{
}


void gkg::MouseEvent::release( int32_t /* x */, 
                               int32_t /* y */,
                               int32_t /* z */ )
{
}


void gkg::MouseEvent::getPosition( anatomist::AWindow* win, 
                                   int32_t x, 
                                   int32_t y, 
                                   Point3d& pos )
{

  if ( win && _object )
  {

    bool valid = false;
    std::set< anatomist::AObject* > objects = win->Objects();
    std::set< anatomist::AObject* >::iterator
      o = objects.begin(),
      oe = objects.end();

    while ( o != oe )
    {

      valid |= validObject( *o );
      ++o;

    }

    if ( valid )
    {

      Point3df posf;

      if ( win->positionFromCursor( x, y, posf ) )
      {
#if ( ANATOMIST_VERSION_MAJOR >= 5 ) || \
    ( ( ANATOMIST_VERSION_MAJOR == 4 ) && \
      ( ANATOMIST_VERSION_MINOR >= 6 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )
    
        std::vector< float > sizes = _object->voxelSize();
        Point3df voxelSizes( sizes[ 0 ], sizes[ 1 ], sizes[ 2 ] );
#else
        Point3df voxelSizes = _object->VoxelSize();
#endif

        Point3df p = anatomist::Transformation::transform( 
                                                   posf, 
                                                   win->getReferential(),
                                                   _object->getReferential(),
                                                   Point3df( 1.0f, 1.0f, 1.0f ),
                                                   voxelSizes );
       pos[ 0 ] = (int16_t)rint( p[ 0 ] );
       pos[ 1 ] = (int16_t)rint( p[ 1 ] );
       pos[ 2 ] = (int16_t)rint( p[ 2 ] );

      }

    }

  }

}


bool gkg::MouseEvent::validObject( anatomist::AObject* obj )
{

  bool status = false;

  if ( obj->isMultiObject() )
  {

    const anatomist::MObject *mo = (const anatomist::MObject *)obj;
    anatomist::MObject::const_iterator 
      m = mo->begin(), 
      me = mo->end();

    while ( m != me )
    {

      status |= validObject( *m );
      ++m;

    }

  }
  else if ( obj == _object )
  {

    status = true;

  }

  return status;

}

#include <gkg-processing-mesh/RayBoxIntersection.h>
#include <gkg-core-exception/Exception.h>


gkg::RayBoxIntersection::RayBoxIntersection()
{
}


gkg::RayBoxIntersection::~RayBoxIntersection()
{
}


void 
gkg::RayBoxIntersection::setBox( const gkg::Vector3d< float >& lowerCorner,
                                 const gkg::Vector3d< float >& upperCorner )
{

  try
  {

    _corners[ 0 ] = lowerCorner;
    _corners[ 1 ] = upperCorner;

  }
  GKG_CATCH( "void "
             "gkg::RayBoxIntersection::setBox( "
             "const gkg::Vector3d< float >& lowerCorner, "
             "const gkg::Vector3d< float >& upperCorner )" );

}


void gkg::RayBoxIntersection::setRayWithDirection(
                                       const gkg::Vector3d< float >& origin,
                                       const gkg::Vector3d< float >& direction )
{

  try
  {

    _origin = origin;
    _direction = direction;
    _direction.normalize();
    _inverseDirection.x = 1.0 / _direction.x;
    _inverseDirection.y = 1.0 / _direction.y;
    _inverseDirection.z = 1.0 / _direction.z;

    _sign.x = ( _inverseDirection.x < 0 );
    _sign.y = ( _inverseDirection.y < 0 );
    _sign.z = ( _inverseDirection.z < 0 );

  }
  GKG_CATCH( "void "
             "gkg::RayBoxIntersection::setRayWithDirection( "
             "const gkg::Vector3d< float >& origin, "
             "const gkg::Vector3d< float >& direction )" );

}


void gkg::RayBoxIntersection::setRayWithDestination(
                                     const gkg::Vector3d< float >& origin,
                                     const gkg::Vector3d< float >& destination )
{

  try
  {

    gkg::Vector3d< float > direction = destination - origin;
    setRayWithDirection( origin, direction );

  }
  GKG_CATCH( "void "
             "gkg::RayBoxIntersection::setRayWithDestination( "
             "const gkg::Vector3d< float >& origin, "
             "const gkg::Vector3d< float >& destination )" );

}


bool 
gkg::RayBoxIntersection::getIntersection( gkg::Vector3d< float >& hit ) const
{

  try
  {

    float tmin = ( _corners[ _sign.x ].x - _origin.x ) *
                 _inverseDirection.x;
    float tmax = ( _corners[ 1 - _sign.x ].x - _origin.x ) *
                 _inverseDirection.x;

    float tymin = ( _corners[ _sign.y ].y - _origin.y ) *
                 _inverseDirection.y;
    float tymax = ( _corners[ 1 - _sign.y ].y - _origin.y ) *
                 _inverseDirection.y;

    if ( ( tmin > tymax ) || ( tymin > tmax ) )
    {

      return false;

    }

    if ( tymin > tmin )
    {

      tmin = tymin;

    }
    if ( tymax < tmax )
    {

      tmax = tymax;

    }

    float tzmin = ( _corners[ _sign.z ].z - _origin.z ) *
                 _inverseDirection.z;
    float tzmax = ( _corners[ 1 - _sign.z ].z - _origin.z ) *
                 _inverseDirection.z;

    if ( ( tmin > tzmax ) || ( tzmin > tmax ) )
    {

      return false;

    }

    if ( tzmin > tmin )
    {

      tmin = tzmin;

    }
    if ( tzmax < tmax )
    {

      tmax = tzmax;

    }

    hit = _origin + _direction * tmax;

    return true;

  }
  GKG_CATCH( "bool "
             "gkg::RayBoxIntersection::getIntersection( "
             "gkg::Vector3d< float >& hit ) const" );

}



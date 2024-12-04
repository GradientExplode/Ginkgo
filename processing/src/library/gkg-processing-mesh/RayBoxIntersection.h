#ifndef _gkg_processing_mesh_RayBoxIntersection_h_
#define _gkg_processing_mesh_RayBoxIntersection_h_


#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{



class RayBoxIntersection
{

  public:

    RayBoxIntersection();
    virtual ~RayBoxIntersection();

    void setBox( const Vector3d< float >& lowerCorner,
                  const Vector3d< float >& upperCorner );

    void setRayWithDirection( const Vector3d< float >& origin,
                               const Vector3d< float >& direction );

    void setRayWithDestination( const Vector3d< float >& origin,
                                 const Vector3d< float >& destination );

    bool getIntersection( Vector3d< float >& hit ) const;

  protected:

    // box parameters
    Vector3d< float > _corners[ 2 ];

    // ray parameters
    Vector3d< float > _origin;
    Vector3d< float > _direction;
    Vector3d< float > _inverseDirection;
    Vector3d< int32_t > _sign;

};


}


#endif



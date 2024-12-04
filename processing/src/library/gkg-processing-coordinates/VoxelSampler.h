#ifndef _gkg_processing_coordinates_VoxelSampler_h_
#define _gkg_processing_coordinates_VoxelSampler_h_


#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{


template < class T >
class VoxelSampler
{

  public:

    virtual ~VoxelSampler();

    const Vector3d< double >& getResolution() const;
    int32_t getPointCount() const;

    virtual void getPoint( const Vector3d< T >& origin,
                           int32_t pointIndex,
                           Vector3d< T >& destination ) const = 0;
               

  protected:

    VoxelSampler( const Vector3d< double >& resolution,
                  int32_t pointCount );

    Vector3d< double > _resolution;
    int32_t _pointCount;


};


}


#endif

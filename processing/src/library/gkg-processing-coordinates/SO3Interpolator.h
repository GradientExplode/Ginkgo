#ifndef _gkg_processing_coordinates_SO3Interpolator_h_
#define _gkg_processing_coordinates_SO3Interpolator_h_


#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <vector>


namespace gkg
{


class SO3Interpolator
{

  public:

    virtual ~SO3Interpolator();

    virtual std::vector< std::pair< int32_t, float > >
      getNeighborIndicesAndWeights(
                        const OrientationSet& orientationSet,
                        const Vector3d< float >& unitaryOrientation ) const = 0;

    int32_t getNeighborCount() const;

  protected:

    SO3Interpolator( int32_t neighborCount );

    int32_t _neighborCount;

};


}


#endif

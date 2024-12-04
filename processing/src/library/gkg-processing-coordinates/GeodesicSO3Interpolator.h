#ifndef _gkg_processing_coordinates_GeodesicSO3Interpolator_h_
#define _gkg_processing_coordinates_GeodesicSO3Interpolator_h_


#include <gkg-processing-coordinates/SO3Interpolator.h>


namespace gkg
{


class GeodesicSO3Interpolator : public SO3Interpolator
{

  public:

    GeodesicSO3Interpolator( int32_t neighborCount, float sigma );
    virtual ~GeodesicSO3Interpolator();

    std::vector< std::pair< int32_t, float > >
      getNeighborIndicesAndWeights(
                            const OrientationSet& orientationSet,
                            const Vector3d< float >& unitaryOrientation ) const;

    float getSigma() const;

  protected:

    float _sigma;

};


}


#endif


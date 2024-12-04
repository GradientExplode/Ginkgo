#ifndef _gkg_processing_coordinates_BarycentricSO3Interpolator_h_
#define _gkg_processing_coordinates_BarycentricSO3Interpolator_h_


#include <gkg-processing-coordinates/SO3Interpolator.h>


namespace gkg
{


class BarycentricSO3Interpolator : public SO3Interpolator
{

  public:

    enum Type
    {

      Unnormalized,
      Normalized

    };

    BarycentricSO3Interpolator( Type type );
    virtual ~BarycentricSO3Interpolator();

    std::vector< std::pair< int32_t, float > >
      getNeighborIndicesAndWeights(
                            const OrientationSet& orientationSet,
                            const Vector3d< float >& unitaryOrientation ) const;

  protected:

    Type _type;

};


}


#endif


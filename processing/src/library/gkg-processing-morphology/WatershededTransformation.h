#ifndef _gkg_processing_morphology_WatershededTransformation_h_
#define _gkg_processing_morphology_WatershededTransformation_h_


#include <gkg-processing-morphology/Neighborhood3d.h>
#include <list>
#include <map>


namespace gkg
{


template < class T > class Volume;
template < class T > class TestFunction;
template < class T > class BoundingBox;


template < class IN, class OUT >
class WatershededTransformation
{

  public:

    WatershededTransformation( Neighborhood3d::Type neighborhoodType,
                               const TestFunction< IN >& testFunction,
                               const OUT& foreground,
                               const OUT& background );
    virtual ~WatershededTransformation();

    void watershed( const Volume< IN >& inputVolume,
                    Volume< OUT >& watershededVolume,
                    bool verbose = false ) const;

  protected:

    void computeCompleteVolume(
      const Volume< IN >& inputVolume,
      const Vector3d< int32_t >& inputVolumeSize,
      int32_t t,
      const std::list< Vector3d< int32_t > >& neighborOffsets,
      const BoundingBox< int32_t >& boundingBox,
      const std::vector< float >& offsetDistances,
      Volume< float >& lowerCompleteVolume,
      std::vector< Vector3d< int32_t > >& voxels,
      std::vector< const float* >& completeVolumePointers,
      std::vector< float >& lowerSlopes,
      std::vector< std::vector< std::pair< int32_t, int32_t > > >&
                                               lutWithAllNeighbors,
      std::vector< std::vector< std::pair< int32_t, int32_t > > >&
                                               lutWithForwardNeighbors,
      std::vector< std::vector< std::pair< int32_t, int32_t > > >&
                                               lutWithBackwardNeighbors ) const;

    float getCost(
      const std::vector< const float* >& completeVolumePointers,
      const std::vector< float >& lowerSlopes,
      float offsetDistance,
      int32_t currentVoxelIndex,
      int32_t neighborVoxelIndex ) const;

    float getLowerSlope( 
      const std::vector< const float* >& completeVolumePointers,
      const std::vector< float >& offsetDistances,
      int32_t currentVoxelIndex,
      const std::vector< std::pair< int32_t, int32_t > >& neighbors ) const;

    Neighborhood3d::Type _neighborhoodType;
    const TestFunction< IN >& _testFunction;
    OUT _foreground;
    OUT _background;

};


}


#endif

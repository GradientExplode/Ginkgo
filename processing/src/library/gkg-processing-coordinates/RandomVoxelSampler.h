#ifndef _gkg_processing_coordinates_RandomVoxelSampler_h_
#define _gkg_processing_coordinates_RandomVoxelSampler_h_


#include <gkg-processing-coordinates/VoxelSampler.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <vector>


namespace gkg
{

//
// this class create a set of offset point(s) inside the voxel in order
// to get a uniform random sampling of point(s)
//

template < class T >
class RandomVoxelSampler : public VoxelSampler< T >
{

  public:

    RandomVoxelSampler( const Vector3d< double >& resolution,
                        int32_t pointCount );
    virtual ~RandomVoxelSampler();

    void getPoint( const Vector3d< T >& origin,
                   int32_t pointIndex,
                   Vector3d< T >& destination ) const;

  protected:

    double _halfResolutionX;
    double _halfResolutionY;
    double _halfResolutionZ;
    RandomGenerator _randomGenerator;
    NumericalAnalysisImplementationFactory* _factory;

};


}


#endif

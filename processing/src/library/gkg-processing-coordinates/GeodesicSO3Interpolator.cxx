#include <gkg-processing-coordinates/GeodesicSO3Interpolator.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::GeodesicSO3Interpolator::GeodesicSO3Interpolator( int32_t neighborCount,
                                                       float sigma )
                             : gkg::SO3Interpolator( neighborCount ),
                               _sigma( sigma )
{
}


gkg::GeodesicSO3Interpolator::~GeodesicSO3Interpolator()
{
}


std::vector< std::pair< int32_t, float > >
gkg::GeodesicSO3Interpolator::getNeighborIndicesAndWeights(
                        const gkg::OrientationSet& orientationSet,
                        const gkg::Vector3d< float >& unitaryOrientation ) const
{

  try
  {

    // allocating orientation indices and weights
    std::vector< std::pair< int32_t, float > >
      indicesAndWeights( _neighborCount );


    // collecting rounding triangle
    std::vector< int32_t > 
      bestIndices = orientationSet.getNearestOrientationIndices( 
                                                             unitaryOrientation,
                                                             _neighborCount );

    // processing normals to the plane defined by 0, o(n), o(n+1)
    float angle, weight, weightSum = 0.0, sigma2 = _sigma * _sigma;
    int32_t* bptr = &bestIndices[ 0 ];
    std::pair< int32_t, float >* iptr = &indicesAndWeights[ 0 ];
    int32_t n = _neighborCount;
    while ( n-- )
    {

      const gkg::Vector3d< float >& u = orientationSet.getOrientation( *bptr );
      angle = gkg::safeAcos( u.dot( unitaryOrientation ) );
      weight = std::exp( - ( angle * angle ) / sigma2 );
      iptr->first = *bptr++;
      iptr->second =  weight;         
      weightSum += weight;
      ++iptr;

    }

    for ( n = 0; n < _neighborCount; n++ )
    {

      indicesAndWeights[ n ].second /= weightSum;

    }
    return indicesAndWeights;

  }
  GKG_CATCH( "std::vector< std::pair< int32_t, float > > "
             "gkg::GeodesicSO3Interpolator::getNeighborIndicesAndWeights( "
             "const gkg::OrientationSet& orientationSet, "
             "const gkg::Vector3d< float >& unitaryOrientation ) const" );

}


float gkg::GeodesicSO3Interpolator::getSigma() const
{

  return _sigma;

}

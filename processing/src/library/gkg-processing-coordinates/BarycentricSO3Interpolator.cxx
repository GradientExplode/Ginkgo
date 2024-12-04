#include <gkg-processing-coordinates/BarycentricSO3Interpolator.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>


gkg::BarycentricSO3Interpolator::BarycentricSO3Interpolator(
                                    gkg::BarycentricSO3Interpolator::Type type )
                                : gkg::SO3Interpolator( 3 ),
                                  _type( type )
{
}


gkg::BarycentricSO3Interpolator::~BarycentricSO3Interpolator()
{
}


std::vector< std::pair< int32_t, float > >
gkg::BarycentricSO3Interpolator::getNeighborIndicesAndWeights(
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
    std::vector< Vector3d< float > > normals( _neighborCount );
    float weight, weightSum = 0.0;
    int32_t n;
    for ( n = 0; n < _neighborCount; n++ )
    {

      const gkg::Vector3d< float >&
        u = orientationSet.getOrientation(
                                    bestIndices[ ( n ) % _neighborCount ] );
      const gkg::Vector3d< float >&
        v = orientationSet.getOrientation(
                                    bestIndices[ ( n + 1 ) % _neighborCount ] );
      const gkg::Vector3d< float >&
        w = orientationSet.getOrientation(
                                    bestIndices[ ( n + 2 ) % _neighborCount ] );

      normals[ n ] = v.cross( w );
      float dotProduct = u.dot( normals[ n ] ) ;

      if ( dotProduct < 0 )
      {

        normals[ n ] *= -1;
        dotProduct *= -1;

      }

      weight = unitaryOrientation.dot( normals[ n ] ) / dotProduct;
      indicesAndWeights[ n ].first = bestIndices[ n ];
      indicesAndWeights[ n ].second =  weight;         
      if ( _type == gkg::BarycentricSO3Interpolator::Normalized )
      {

        weightSum += weight;

      }

    }

    if ( _type == gkg::BarycentricSO3Interpolator::Normalized )
    {

      for ( n = 0; n < _neighborCount; n++ )
      {

        indicesAndWeights[ n ].second /= weightSum;

      }

    }
    return indicesAndWeights;

  }
  GKG_CATCH( "std::vector< std::pair< int32_t, float > > "
             "gkg::BarycentricSO3Interpolator::getNeighborIndicesAndWeights( "
             "const gkg::OrientationSet& orientationSet, "
             "const gkg::Vector3d< float >& unitaryOrientation ) const" );

}

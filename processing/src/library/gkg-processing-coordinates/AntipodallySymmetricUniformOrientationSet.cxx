#include <gkg-processing-coordinates/AntipodallySymmetricUniformOrientationSet.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::AntipodallySymmetricUniformOrientationSet::
                      AntipodallySymmetricUniformOrientationSet( int32_t count )
                                               : gkg::OrientationSet( count )
{

  try
  {

    // check that the target orientation count is even

    if ( count % 2 )
    {

      throw std::runtime_error( "antipodally symmetric uniform orientation set"
                                " requires an even orientation count" );

    }

    // computing half count corresponding to the number of points within one
    // hemisphere
    int32_t K = count / 2;

    // computing the target number of circles
    int32_t N = std::lround( std::sqrt( ( double )K * M_PI / 8.0 ) );

    // looping over latitude circles
    int32_t i = 0;
    int32_t j = 0;
    double theta = 0.0;
    double phi = 0.0;
    int32_t ki5 = 0.0;
    int32_t sumOfKi5 = 0;
    int32_t index = 0;

    for ( i = 1; i <= N; i++ )
    {

      // computing the latitude
      theta = ( ( double )i - 0.5 ) * M_PI / ( 2.0 * ( double )N );

      if ( i == N )
      {

        ki5 = K - sumOfKi5;

      }
      else
      {

        ki5 = std::round( 2 * std::sin( theta ) * ( double )K * 
                          std::sin( M_PI / ( 4.0 * ( double )N ) ) );
        sumOfKi5 += ki5;

      }

      // looping over longitudes
      for ( j = 1; j <= ki5; j++ )
      {

        // computing the longitude
        phi = ( ( double )j - 0.5 ) * 2 * M_PI / ( double )ki5;

        // setting orientation using Cartesian coordinates
        _orientations[ index ].x = std::sin( theta ) * std::cos( phi );
        _orientations[ index ].y = std::sin( theta ) * std::sin( phi );
        _orientations[ index ].z = std::cos( theta );

        // adding antipodally symmetric orientation
        _orientations[ index + K ] = -_orientations[ index ];

        ++ index;

      }

    }

    // building orientation indices of nearest symmetrical orientations
    _nearestSymmetricalOrientationIndices.resize( _orientations.size() );

    std::vector< gkg::Vector3d< float > >::const_iterator
      o = _orientations.begin(),
      oe = _orientations.end();
    std::vector< int32_t >::iterator
      s = _nearestSymmetricalOrientationIndices.begin();
    while ( o != oe )
    {

      *s = getNearestOrientationIndex( -*o );
      ++ o;
      ++ s;

    }

  }
  GKG_CATCH( "gkg::AntipodallySymmetricUniformOrientationSet::"
             "AntipodallySymmetricUniformOrientationSet( "
             "int32_t count )" );

}


gkg::AntipodallySymmetricUniformOrientationSet::
                                    ~AntipodallySymmetricUniformOrientationSet()
{
}



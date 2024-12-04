#include <gkg-dmri-microstructure-axon-mapping/CanonicalWatsonSphericalHarmonicsCache.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>

gkg::CanonicalWatsonSphericalHarmonicsCache::
                                        CanonicalWatsonSphericalHarmonicsCache()
                                            : _factory(
     gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory() ),
                                              _orientations( 0 ),
                                              _orientationCount( 0 ),
                                              _maximumSphericalHarmonicsOrder(
                                                                            0 ),
                                              _lowerKappa( 0.0 ),
                                              _upperKappa( 0.0 ),
                                              _canonicalDecompositions(
                                CANONICAL_WATSON_SPHERICAL_HARMONICS_BIN_COUNT )

{
}


gkg::CanonicalWatsonSphericalHarmonicsCache::
                                       ~CanonicalWatsonSphericalHarmonicsCache()
{
}


void gkg::CanonicalWatsonSphericalHarmonicsCache::update(
                      const std::vector< gkg::Vector3d< float > >& orientations,
                      int32_t maximumSphericalHarmonicsOrder,
                      double lowerKappa,
                      double upperKappa )
{

  try
  {

    if ( ( &orientations != _orientations ) ||
         ( maximumSphericalHarmonicsOrder !=
           _maximumSphericalHarmonicsOrder ) ||
         ( lowerKappa != _lowerKappa ) ||
         ( upperKappa != _upperKappa ) )
    {

      _mutex.lock();

      // updating protected field(s)
      _orientations = &orientations;
      _orientationCount = ( int32_t )orientations.size();
      _maximumSphericalHarmonicsOrder = maximumSphericalHarmonicsOrder;
      _lowerKappa = lowerKappa;
      _upperKappa = upperKappa;

      // allocating real spherica harmonics B matrix
      gkg::Matrix B( _orientationCount, maximumSphericalHarmonicsOrder + 1 );

      // looping over orientation(s)
      int32_t o = 0;
      int32_t s = 0;
      double radial = 0.0;
      double theta = 0.0;
      double phi = 0.0;
      for ( o = 0; o < _orientationCount; o++ )
      {

        // obtaining a reference to the current orientation
        const gkg::Vector3d< float >& 
          orientation = ( *_orientations )[ o ];

        // converting cartesian coordinate(s) to spherical coordinate(s)
        _factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                      ( double )orientation.y,
                                                      ( double )orientation.z,
                                                      radial,
                                                      theta,
                                                      phi );

        // looping over spherical harmonic(s)
        for ( s = 0; s <= _maximumSphericalHarmonicsOrder; s++ )
        {

          B( o, s ) = 
                std::real( _factory->getSphericalHarmonic( s, 0, phi, theta ) );

        }

      }

      // computing the matrix for spherical harmonics decomposition
      gkg::Matrix Bplus;
      _factory->getMoorePenrosePseudoInverse( B, Bplus );

      // allocating the watson distribution vector
      gkg::Vector watsonDistribution( _orientationCount );

      // filling principal orientation vector
      gkg::Vector3d< float > principalOrientation( 0.0, 0.0, 1.0 );


      //  computing values of canonical Watson distribution along provided
      // orientations for all kappa(s)
      int32_t k = 0;
      for ( k = 0; k < CANONICAL_WATSON_SPHERICAL_HARMONICS_BIN_COUNT; k++ )
      {

        _canonicalDecompositions[ k ].resize(
                                           maximumSphericalHarmonicsOrder + 1 );

        double kappa = _lowerKappa + ( double )k *
                       ( ( _upperKappa - _lowerKappa ) / 
                         ( double )
                               CANONICAL_WATSON_SPHERICAL_HARMONICS_BIN_COUNT );

        const gkg::Vector3d< float >*
          orientationPtr = &( ( *_orientations )[ 0 ] );
        double* watsonDistributionPtr = &watsonDistribution( 0 );

        double sum = 0.0;
        int32_t o = 0;

        sum = 0.0;
        for ( o = 0; o < _orientationCount;
              o++, orientationPtr++, watsonDistributionPtr++ )
        {

          *watsonDistributionPtr = _factory->getWatsonDistribution(
                                                           kappa,
                                                           principalOrientation,
                                                           *orientationPtr );
          sum += *watsonDistributionPtr;

        }

        watsonDistributionPtr = &watsonDistribution( 0 );
        for ( o = 0; o < _orientationCount;
              o++, watsonDistributionPtr++ )
        {

          *watsonDistributionPtr /= sum;

        }


        gkg::Vector decomposition = Bplus.getComposition( watsonDistribution );

        int32_t d = 0;
        for ( d = 0; d <= _maximumSphericalHarmonicsOrder; d++ )
        {

          _canonicalDecompositions[ k ][ d ] = decomposition( d );

        }

      }

      _mutex.unlock();

    }

  }
  GKG_CATCH( "void gkg::CanonicalWatsonSphericalHarmonicsCache::update( "
             "const std::vector< gkg::Vector3d< float > >& orientations, "
             "int32_t maximumSphericalHarmonicsOrder, "
             "double lowerKappa, "
             "double upperKappa )" );

}


int32_t gkg::CanonicalWatsonSphericalHarmonicsCache::
                                       getMaximumSphericalHarmonicsOrder() const
{

  try
  {

    return _maximumSphericalHarmonicsOrder;

  }
  GKG_CATCH( "int32_t gkg::CanonicalWatsonSphericalHarmonicsCache::"
             "getMaximumSphericalHarmonicsOrder() const" );

}


const std::vector< double >& 
gkg::CanonicalWatsonSphericalHarmonicsCache::getDecomposition( 
                                                            double kappa ) const
{

  try
  {

    if ( ( kappa > _upperKappa ) || ( kappa < _lowerKappa ) )
    {


      throw std::runtime_error( 
                             "kappa must be between lower and upper bound(s)" );

    }

    int32_t index = ( int32_t )(
                      ( double )( CANONICAL_WATSON_SPHERICAL_HARMONICS_BIN_COUNT
                                  - 1 ) *
                      ( kappa - _lowerKappa ) /
                      ( _upperKappa - _lowerKappa ) + 0.5 );

    return _canonicalDecompositions[ index ];

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::CanonicalWatsonSphericalHarmonicsCache::getDecomposition( "
             "double kappa ) const" );

}


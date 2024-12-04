#include <gkg-dmri-sh-basis/SymmetricalSphericalHarmonicsCache.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>


gkg::SymmetricalSphericalHarmonicsCache::SymmetricalSphericalHarmonicsCache()
                                        : _orientationSet( 0 ),
                                          _orientationCount( 0 ),
                                          _symmetricalSHCoefficientCount( 0 )
{
}


gkg::SymmetricalSphericalHarmonicsCache::~SymmetricalSphericalHarmonicsCache()
{
}


void gkg::SymmetricalSphericalHarmonicsCache::update(
                                     const gkg::OrientationSet& orientationSet,
                                     int32_t symmetricalSHCoefficientCount )
{

  try
  {

    _mutex.lock();
    if ( ( orientationSet.getCount() != _orientationCount ) ||
         ( &orientationSet != _orientationSet ) ||
         ( symmetricalSHCoefficientCount != _symmetricalSHCoefficientCount ) )
    {

      // updating protected field(s)
      _orientationCount = orientationSet.getCount();
      _orientationSet = &orientationSet;
      _symmetricalSHCoefficientCount = symmetricalSHCoefficientCount;

      // getting a pointer to the numerical analysis factory
      gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

      // allocation coefficient double dimension vector
      _coefficients = std::vector< std::vector< float > >( _orientationCount );

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
          orientation = _orientationSet->getOrientation( o );

        // allocating coefficient(s) for current orientation
        _coefficients[ o ] =
          std::vector< float >( _symmetricalSHCoefficientCount );

        // converting cartesian coordinate(s) to spherical coordinate(s)
        factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                     ( double )orientation.y,
                                                     ( double )orientation.z,
                                                     radial,
                                                     theta,
                                                     phi );

        // looping over spherical harmonic(s)
        for ( s = 0; s < _symmetricalSHCoefficientCount; s++ )
        {

          _coefficients[ o ][ s ] =
            ( float )factory->getSymmetricalSphericalHarmonic( s, phi, theta );

        }

      }

    }
    _mutex.unlock();

  }
  GKG_CATCH( "void gkg::SymmetricalSphericalHarmonicsCache::update( "
             "const gkg::OrientationSet& orientationSet, "
             "int32_t symmetricalSHCoefficientCount )" );

}


const std::vector< std::vector< float > >& 
gkg::SymmetricalSphericalHarmonicsCache::getCoefficients() const
{

  try
  {

    return _coefficients;

  }
  GKG_CATCH( "const std::vector< std::vector< float > >& "
             "gkg::SymmetricalSphericalHarmonicsCache::getCoefficients() "
             "const" );

}


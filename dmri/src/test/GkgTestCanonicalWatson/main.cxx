#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <fstream>





int main( int32_t argc, char* argv[] )
{


  int result = EXIT_SUCCESS;

  try
  {

    float kappa = 0.0;
    int32_t maximumSphericalHarmonicsOrder = 12;
    int32_t outputOrientationCount = 60;
    bool verbose = false;
    
    // managing the command line options
    gkg::Application application( argc, argv,
                                  "Canonical Watson spherical harmonics "
                                  "decomposition" );
    application.addSingleOption( "-kappa",
                                 "Kappa value",
                                 kappa );
    application.addSingleOption( "-maximumSphericalHarmonicsOrder",
                                 "Maximum sphericalharmonics order "
                                 "(default=12)",
                                 maximumSphericalHarmonicsOrder,
                                 true );
    application.addSingleOption( "-outputOrientationCount",
                                 "Output orientation count (default=60)",
                                 outputOrientationCount,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();
    
    //////////////////////////////////////////////////////////////////////////
    // getting numerical analysis implementation factory
    //////////////////////////////////////////////////////////////////////////
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                 getImplementationFactory();

    gkg::ElectrostaticOrientationSet
      orientationSet( outputOrientationCount, false );
    

    gkg::Matrix B( outputOrientationCount,
                   maximumSphericalHarmonicsOrder );

    int32_t o = 0;
    int32_t l = 0;

    double radial = 0.0;
    double theta = 0.0;
    double phi = 0.0;
    for ( o = 0; o < outputOrientationCount; o++ )
    {

      const gkg::Vector3d< float >&
        orientation = orientationSet.getOrientation( o );
      factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                   ( double )orientation.y,
                                                   ( double )orientation.z,
                                                   radial,
                                                   theta,
                                                   phi );

      for ( l = 0; l < maximumSphericalHarmonicsOrder; l++ )
      {

        B( o, l ) =
          std::real( factory->getSphericalHarmonic( l, 0, phi, theta ) );


      }

    }

    std::cout << "B--------------------------------------------" << std::endl;
    std::cout << B << std::endl;

    gkg::Matrix Bplus;

    factory->getMoorePenrosePseudoInverse( B, Bplus );

    std::cout << "Bplus--------------------------------------------"
              << std::endl;
    std::cout << Bplus << std::endl;


    gkg::Vector watson( outputOrientationCount );
    gkg::Vector3d< float > principalOrientation( 0.0, 0.0, 1.0 );
    for ( o = 0; o < outputOrientationCount; o++ )
    {

      const gkg::Vector3d< float >&
        orientation = orientationSet.getOrientation( o );
      watson( o ) = factory->getWatsonDistribution( kappa,
                                                    principalOrientation,
                                                    orientation );

    }

    std::cout << "Watson--------------------------------------------"
              << std::endl;
    std::cout << watson << std::endl;


    gkg::Vector decomposition = Bplus.getComposition( watson );

    std::cout << "decomposition--------------------------------------------"
              << std::endl;
    std::cout << decomposition << std::endl;


    std::cout << "from NumAnalysisFactory----------------------------------"
              << std::endl;

    std::cout << factory->getCanonicalWatsonSphericalHarmonicsDecomposition(
                                          kappa,
                                          maximumSphericalHarmonicsOrder,
                                          orientationSet.getOrientations() )
              << std::endl;
  
  }
  GKG_CATCH_COMMAND( result );

  return result;

  
}

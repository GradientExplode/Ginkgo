#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelNelderMeadSimplexFunction.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cmath>
#include <cstdio>



#define SHELL_COUNT 10
#define ORIENTATION_COUNT 60


int main( int /*argc*/, char* argv[] )
{


  int result = EXIT_SUCCESS;

  try
  {

    gkg::PluginLoader::getInstance().load();

    // in fact, 'gsl' library is the default; so the following line is not
    // mandatory: it is only for didactic purpose
    gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );

    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    // initializing random generator 
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    // preparing an instance of two-pool model
    gkg::Vector coefficients( 13 );
    coefficients( 0 ) = 0.001;
    coefficients( 1 ) = 0.0;
    coefficients( 2 ) = 0.0;
    coefficients( 3 ) = 0.0002;
    coefficients( 4 ) = 0.0;
    coefficients( 5 ) = 0.0002;

    coefficients( 6 ) = 0.00002;
    coefficients( 7 ) = 0.0;
    coefficients( 8 ) = 0.0;
    coefficients( 9 ) = 0.0001;
    coefficients( 10 ) = 0.0;
    coefficients( 11 ) = 0.00002;

    coefficients( 12 ) = 0.5;

    gkg::TwoPoolModel twoPoolModel( 0, coefficients );

    // generating the instance of orientation set
    gkg::ElectrostaticOrientationSet orientationSet( ORIENTATION_COUNT );

    // generating samples from this two-pool model on two shells
    gkg::Matrix qSpaceSamples( SHELL_COUNT * ORIENTATION_COUNT, 4 );
    gkg::Vector measuredAttenuations( SHELL_COUNT * ORIENTATION_COUNT );
    int32_t s = 0;
    int32_t o = 0;

    for ( s = 0; s < SHELL_COUNT; s++ )
    {

      double bValue = 10000.0 * ( double )s / ( ( double )SHELL_COUNT - 1 );
      std::cout << "shell at b=" << bValue << " s/mm2 :" << std::endl;
      std::cout << "---------------------" << std::endl;
      for ( o = 0; o < ORIENTATION_COUNT; o++ )
      {

        gkg::Vector3d< float > orientation = orientationSet.getOrientation( o );

        qSpaceSamples( s * ORIENTATION_COUNT + o, 0 ) = ( double )orientation.x;
        qSpaceSamples( s * ORIENTATION_COUNT + o, 1 ) = ( double )orientation.y;
        qSpaceSamples( s * ORIENTATION_COUNT + o, 2 ) = ( double )orientation.z;
        qSpaceSamples( s * ORIENTATION_COUNT + o, 3 ) = bValue;

        measuredAttenuations( s * ORIENTATION_COUNT + o ) =
           twoPoolModel.getAttenuation( orientation, bValue ) ;//+
           //factory->getGaussianRandomNumber( randomGenerator, 0.0, 0.01 );

        std::cout << orientation << " -> " 
                  << "attenuation="
                  << measuredAttenuations( s * ORIENTATION_COUNT + o )
                  << std::endl;

      }
      std::cout << std::endl;

    }

    // launching Nelder-Mead simplex optimizer

    gkg::Vector parameters( 13 );
    parameters( 0 )  = 0.001;
    parameters( 1 )  = 0.0;
    parameters( 2 )  = 0.0;
    parameters( 3 )  = 0.001;
    parameters( 4 )  = 0.0;
    parameters( 5 )  = 0.001;

    parameters( 6 )  = 0.0001;
    parameters( 7 )  = 0.0;
    parameters( 8 )  = 0.0;
    parameters( 9 )  = 0.0001;
    parameters( 10 ) = 0.0;
    parameters( 11 ) = 0.0001;

    parameters( 12 ) = 0.5;


    gkg::Vector deltaParameters( 13 );
    deltaParameters( 0 )  = 0.0001;
    deltaParameters( 1 )  = 0.0001;
    deltaParameters( 2 )  = 0.0001;
    deltaParameters( 3 )  = 0.0001;
    deltaParameters( 4 )  = 0.0001;
    deltaParameters( 5 )  = 0.0001;

    deltaParameters( 6 )  = 0.00001;
    deltaParameters( 7 )  = 0.00001;
    deltaParameters( 8 )  = 0.00001;
    deltaParameters( 9 )  = 0.00001;
    deltaParameters( 10 ) = 0.00001;
    deltaParameters( 11 ) = 0.00001;

    deltaParameters( 12 ) = 0.00001;



    gkg::TwoPoolModelNelderMeadSimplexFunction
    twoPoolModelFunction( qSpaceSamples,
                          measuredAttenuations );

    double testSize = 0.0;
    int32_t maximumIterationCount = 100000;
    double maximumTestSize = 1e-9;


    factory->getNelderMeadSimplexOptimumParameters( parameters,
                                                    deltaParameters,
                                                    testSize,
                                                    twoPoolModelFunction,
                                                    maximumIterationCount,
                                                    maximumTestSize,
                                                    true );

    std::cout << "fitted parameters : " << std::endl;
    std::cout << "-----------------" << std::endl;

    int32_t p = 0;
    for ( p = 0; p < 13; p++ )
    {


      std::cout << "ideal: " << coefficients( p )
                << "  fitted: "
                << parameters( p ) 
                << std::endl;

      if ( ( p == 5 ) || ( p == 11 ) )
      {

        std::cout << std::endl;

      }

    }

    gkg::TwoPoolModel twoPoolModelOut( 0, parameters );
    std::cout << "eigen values : " 
              << twoPoolModelOut.getEigenValues() << std::endl;
    std::cout << "fast ADC : " 
              << twoPoolModelOut.getApparentDiffusionCoefficient(
                                         gkg::TwoPoolModel::Fast ) << std::endl;
    std::cout << "slow ADC : " 
              << twoPoolModelOut.getApparentDiffusionCoefficient(
                                         gkg::TwoPoolModel::Slow ) << std::endl;
    


  }
  GKG_CATCH_COMMAND( result );

  return result;


}

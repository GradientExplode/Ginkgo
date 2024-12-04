#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelLevenbergMarquardtFunction.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cmath>
#include <cstdio>



#define SHELL_COUNT 10
#define ORIENTATION_COUNT 20


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
    gkg::Vector weights( SHELL_COUNT * ORIENTATION_COUNT );
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
           twoPoolModel.getAttenuation( orientation, bValue ) +
           factory->getGaussianRandomNumber( randomGenerator, 0.0, 0.01 );

        std::cout << orientation << " -> " 
                  << "attenuation="
                  << measuredAttenuations( s * ORIENTATION_COUNT + o )
                  << std::endl;

        weights( s * ORIENTATION_COUNT + o ) = 1.0;

      }
      std::cout << std::endl;

    }

    // launching Levenberg-Marquardt optimizer

    gkg::Vector initialParameters( 13 );
    initialParameters( 0 )  = 0.001;
    initialParameters( 1 )  = 0.0;
    initialParameters( 2 )  = 0.0;
    initialParameters( 3 )  = 0.001;
    initialParameters( 4 )  = 0.0;
    initialParameters( 5 )  = 0.001;

    initialParameters( 6 )  = 0.001;
    initialParameters( 7 )  = 0.0;
    initialParameters( 8 )  = 0.0;
    initialParameters( 9 )  = 0.001;
    initialParameters( 10 ) = 0.0;
    initialParameters( 11 ) = 0.001;

    initialParameters( 12 ) = 0.0005;

    gkg::Matrix covariance;
    double chiSquarePerDegreeOfFreedom = 0.0;

    gkg::TwoPoolModelLevenbergMarquardtFunction
    twoPoolModelFunction( initialParameters );


    factory->getLevenbergMarquardtFit( qSpaceSamples,
                                       measuredAttenuations,
                                       weights,
                                       twoPoolModelFunction,
                                       covariance,
                                       chiSquarePerDegreeOfFreedom,
                                       50000,
                                       1e-7,
                                       1e-7 );

    std::cout << "fitted parameters : " << std::endl;
    std::cout << "-----------------" << std::endl;

    int32_t p = 0;
    for ( p = 0; p < 6; p++ )
    {


      std::cout << "ideal: " << coefficients( p )
                << "  fitted: "
                << twoPoolModelFunction.getParameters()( p ) << " +/- "
                << std::sqrt( covariance( p, p ) ) << "  "
                << std::endl;

    }
    std::cout << std::endl;
    for ( p = 6; p < 12; p++ )
    {


      std::cout << "ideal: " << coefficients( p )
                << "  fitted: "
                << twoPoolModelFunction.getParameters()( p ) / 10.0 << " +/- "
                << std::sqrt( covariance( p, p ) ) / 10.0 << "  "
                << std::endl;

    }

    std::cout << "ideal: " << coefficients( 12 )
              << "  fitted: "
              << twoPoolModelFunction.getParameters()( 12 ) * 1000.0 << " +/- "
              << std::sqrt( covariance( 12, 12 ) ) * 1000.0 << "  "
              << std::endl;


    std::cout << std::endl;
    std::cout << "chiq per degree of freedom : " << std::endl
              << "--------------------------" << std::endl
              << chiSquarePerDegreeOfFreedom
              << std::endl;


    gkg::Vector outParameters = twoPoolModelFunction.getParameters();
    for ( p = 6; p < 12; p++ )
    {

      outParameters( p ) /= 10.0;

    }
    outParameters( 12 ) *= 1000.0;

    gkg::TwoPoolModel twoPoolModelOut( 0, outParameters );
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

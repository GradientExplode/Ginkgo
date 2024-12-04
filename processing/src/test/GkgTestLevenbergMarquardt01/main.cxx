#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-processing-numericalanalysis/LevenbergMarquardtFunction.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cmath>


#define TRIAL_COUNT  40


class MyFunction : public gkg::LevenbergMarquardtFunction
{

  public:

    MyFunction( const gkg::Vector& parameters );
    ~MyFunction();

    double getValueWithParametersAt( const gkg::Vector& x,
                                     const gkg::Vector& p ) const;
    gkg::Vector getDerivativeWithParametersAt( const gkg::Vector& x,
                                               const gkg::Vector& p ) const;

};


MyFunction::MyFunction( const gkg::Vector& parameters )
           : gkg::LevenbergMarquardtFunction( parameters )
{
}


MyFunction::~MyFunction()
{
}


double MyFunction::getValueWithParametersAt( const gkg::Vector& x,
                                             const gkg::Vector& p ) const
{

  return p( 0 ) * std::exp( -p( 1 ) * x( 0 ) ) + p( 2 );

}


gkg::Vector MyFunction::getDerivativeWithParametersAt(
                                                    const gkg::Vector& x,
                                                    const gkg::Vector& p ) const
{

  gkg::Vector derivative( 3 );

  derivative( 0 ) = std::exp( -p( 1 ) * x( 0 ) );
  derivative( 1 ) = -x( 0 ) * p( 0 ) * std::exp( -p( 1 ) * x( 0 ) );
  derivative( 2 ) = 1.0;

  return derivative;

}


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

    // 
    gkg::Matrix x( TRIAL_COUNT, 1 );
    gkg::Vector y( TRIAL_COUNT );
    gkg::Vector w( TRIAL_COUNT );
    int32_t t;
    for ( t = 0; t < TRIAL_COUNT; t++ )
    {

      x( t, 0 ) = ( double )t;
      y( t ) = 5.0 * std::exp( -0.1 * x( t, 0 ) ) + 1.0 +
               factory->getGaussianRandomNumber( randomGenerator, 0.0, 0.1 );
      w( t ) = 0.1;

    }

    gkg::Vector initialParameters( 3 );
    initialParameters( 0 ) = 1.0;
    initialParameters( 1 ) = 0.0;
    initialParameters( 2 ) = 0.0;
    MyFunction myFunction( initialParameters );

    gkg::Matrix covariance;
    double chiSquarePerDegreeOfFreedom = 0.0;

    factory->getLevenbergMarquardtFit( x,
                                       y,
                                       w,
                                       myFunction,
                                       covariance,
                                       chiSquarePerDegreeOfFreedom,
                                       500,
                                       1e-4,
                                       1e-4 );

    std::cout << "fit of function p(O) * exp( -p(1) * t ) + p(2)" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "ideal parameters : ( 5.0,  0.1,  1.0 )" << std::endl;
    std::cout << "fitted parameters : ( "
              << myFunction.getParameters()( 0 ) << " +/- "
              << std::sqrt( covariance( 0, 0 ) ) << ", "
              << myFunction.getParameters()( 1 ) << " +/- "
              << std::sqrt( covariance( 1, 1 ) ) << ", "
              << myFunction.getParameters()( 2 ) << " +/- "
              << std::sqrt( covariance( 2, 2 ) ) << " )"
              << std::endl;
    std::cout << "chiq per degree of freedom : " << chiSquarePerDegreeOfFreedom
              << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;


}

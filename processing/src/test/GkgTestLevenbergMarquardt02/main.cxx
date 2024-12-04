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
#include <cstdio>

#define TRIAL_COUNT  1000


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

  return p( 0 ) / std::sqrt(2*M_PI*p(2)*p(2)) 
    * std::exp( -( x(0) - p( 1 ) ) * ( x( 0 ) - p( 1 ) )  
		/ (2 * p(2) * p(2) ) );

}


gkg::Vector MyFunction::getDerivativeWithParametersAt(
                                                    const gkg::Vector& x,
                                                    const gkg::Vector& p ) const
{

  gkg::Vector derivative( 3 );

  double exponential = std::exp(-0.5*( x(0)-p(1) ) * 
				( x(0)-p(1) ) / p(2)*p(2) );
  double Sqrt = 1/( std::sqrt( 2*M_PI*p(2)*p(2) ) );
  double Gauss = Sqrt*exponential;

  derivative( 0 ) = 1 / std::sqrt(2*M_PI*p(2)*p(2)) 
    * std::exp( -( x(0) - p( 1 ) ) * ( x( 0 ) - p( 1 ) )  
		/ (2 * p(2) * p(2)) );

  derivative( 1 ) = Gauss*p(0)*( x(0) - p(1) )/ ( p(2)*p(2) );
  derivative( 2 ) = p(0) / (std::sqrt( 2*M_PI )*p(2)*p(2))*exponential*
    ( (x(0) - p(1))*(x(0) - p(1))/ (p(2)*p(2)) - 1 ) ;

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

    double a = 2.0;
    double mu = 0.6;
    double sigma = 0.11;

    FILE *f;
    f  = fopen("plot.txt", "w");

    for ( t = 0; t < TRIAL_COUNT; t++ )
    {

      x( t, 0 ) = ( double )t/TRIAL_COUNT;
      y( t ) = a / std::sqrt(2*M_PI*sigma*sigma) 
	* std::exp( -( x(t, 0) - mu ) * ( x( t, 0 ) - mu )  
		    / (2 * sigma * sigma) ) 
	+ factory->getGaussianRandomNumber( randomGenerator, 0.0, 0.01 ); 
      
      w( t ) = 1.0;
      
      fprintf (f, "%f %f\n", x(t, 0), y(t));

    }

    fclose(f);
    //system ("gnuplot plot.plt");
    //system ("gv Rice.eps");

    gkg::Vector initialParameters( 3 );
    initialParameters( 0 ) = 1.0;  //2.0;
    initialParameters( 1 ) = 0.5;  //20.0;
    initialParameters( 2 ) = 0.1;  //5.0;
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

    std::cout << "fit of function a * Gaussienne(mu, sigma) " << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "ideal parameters : ( "
	      << a << ", " << mu << ", " << sigma << ")" << std::endl;
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
    

    FILE *f2;
    f2  = fopen("fit.txt", "w");
    gkg::Vector tempX( 1 );
    gkg::Vector tempP( 3 );
    tempP( 0 ) = myFunction.getParameters()( 0 );
    tempP( 1 ) = myFunction.getParameters()( 1 );
    tempP( 2 ) = myFunction.getParameters()( 2 );
    
      for ( t = 0; t < TRIAL_COUNT; t++ )
       {
	
	 tempX( 0 ) = x(t, 0);
	 fprintf ( f2, "%f %f\n", x(t, 0), 
		   myFunction.getValueWithParametersAt( 
						       tempX, tempP ) );
	      
       }	      
      
      fclose(f2);	    
      //system ("gnuplot plot_result.plt");
      //system ("gv fit.eps");



  }
  GKG_CATCH_COMMAND( result );

  return result;


}

#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/LevenbergMarquardtFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <cstdio>


//
//class SumOfGaussianFunction
//

class SumOfGaussianFunction : public gkg::LevenbergMarquardtFunction
{

  public:

  SumOfGaussianFunction( const gkg::Vector& parameters );
  ~SumOfGaussianFunction();

  double getValueWithParametersAt( const gkg::Vector& x,
				   const gkg::Vector& p ) const;
  gkg::Vector getDerivativeWithParametersAt( const gkg::Vector& x,
					     const gkg::Vector& p ) const;
  
};


SumOfGaussianFunction::SumOfGaussianFunction( const gkg::Vector& parameters )
                      : gkg::LevenbergMarquardtFunction( parameters )
{
}


SumOfGaussianFunction::~SumOfGaussianFunction()
{
}


double SumOfGaussianFunction::getValueWithParametersAt(
                                                    const gkg::Vector& x,
                                                    const gkg::Vector& p ) const
{
  try
  {
 
    // p(0) -> a1
    // p(1) -> mu1
    // p(2) -> sigma1

    // p(3) -> a2
    // p(4) -> mu2
    // p(5) -> sigma2

    // p(6) -> a2
    // p(7) -> mu2
    // p(8) -> sigma2

    // p(9) -> a2
    // p(10) -> mu2
    // p(11) -> sigma2  
    //   std::cout << "\na1, mu1, sigma1 : "<< p(0) << "," << p(1) << "," << p(2) << "\n";
    //     std::cout << "a2, mu2, sigma2 : "  << p(3) << "," << p(4) << "," << p(5) << "\n";
    //     std::cout << "a3, mu3, sigma3 : "  << p(6) << "," << p(7) << "," << p(8) << "\n"; 
    //     std::cout << "a4, mu4, sigma4 : "  << p(9) << "," << p(10) << ","<< p(11)<< "\n";

    double sumOfGaussian = 
      p(0) * 1/( std::sqrt( 2*M_PI*p(2)*p(2) ) ) 
      * std::exp( 
		 -0.5*( x(0)-p(1) ) * ( x(0)-p(1) ) / p(2)*p(2) ) ;

    return sumOfGaussian;

  }
  GKG_CATCH( "double SumOfGaussianFunction::getValueWithParametersAt( "
             "const gkg::Vector& x, "
             "const gkg::Vector& p ) const" );

}


gkg::Vector SumOfGaussianFunction::getDerivativeWithParametersAt(
                                                    const gkg::Vector& x,
                                                    const gkg::Vector& p ) const
{

  try
  {

  gkg::Vector derivative( 3 );
  
  double exponential = std::exp(-0.5*( x(0)-p(1) ) * 
				( x(0)-p(1) ) / p(2)*p(2) );
  double Sqrt = 1/( std::sqrt( 2*M_PI*p(2)*p(2) ) );
  double Gauss = Sqrt*exponential;
  
  // derivee par rapport a a1 
  derivative( 0 ) = Gauss;

  // derivee par rapport a mu1 		     
  derivative( 1 ) = Gauss*p(0)*( x(0) - p(1) )/ ( p(2)*p(2) );
  // p(0)/( p(2)*p(2)*p(2) * std::sqrt( 2*M_PI ) ) * ( x(0)-p(1) ) *
  //             std::exp(-0.5*( x(0)-p(1) ) * ( x(0)-p(1) ) / p(2)*p(2) );
   
  // derivee par rapport a sigma1 
  derivative( 2 ) = p(0) / (std::sqrt( 2*M_PI )*p(2)*p(2))*exponential*
    ( (x(0) - p(1))*(x(0) - p(1))/ (p(2)*p(2)) - 1 ) ;
  
  return derivative;

}

  GKG_CATCH( "gkg::Vector SumOfGaussianFunction:: " 
             "getDerivativeWithParametersAt( " 
             "const gkg::Vector& x, "
             "const gkg::Vector& p ) const" );

}

int main( int argc, char* argv[] )
{


  int result = EXIT_SUCCESS;

  try
  {

    // defining parameters of Rice distribution 
    double sigma = 0.11;
    double mu = 0.5;
    double a = 2.0;
    std::string riceFileName = "rice.eps";
    std::string fittedRiceFileName = "fittedRice.eps";
    int32_t trialCount = 40;
    double confidence = 1.0;
    int32_t maximumIterationCount = 500;
    double absoluteError = 1e-4;
    double relativeError = 1e-4;

    ////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////
    gkg::Application application( argc, 
				  argv, "Test Gaussian fit with 1 Gaussian \
using Levenberg-Marquart" );

    application.addSingleOption( "-a",
                                 "Weight of the Gaussian\n(default 2.0)",
                                 a, 2.0 );
    application.addSingleOption( "-mu",
                                 "Noiseless signal\n(default 0.5)",
                                 mu, 0.5 );
    application.addSingleOption( "-sigma",
                                 "Sigma of the Rician distribution \
\n(default 0.11)",
                                 sigma, 0.11 );
    application.addSingleOption( "-trialCount",
                                 "Trial count number\n(default 40)",
				 trialCount, 40 );
    application.addSingleOption( "-w",
                                 "Confidence interval\n(default 1.0)",
				 confidence, 1.0 );
    application.addSingleOption( "-maximumIterationCount",
                                 "Maximum iteration count for the LM algorithm \
\n(default 500)",
				 maximumIterationCount, 500 );
    application.addSingleOption( "-relativeError",
                                 "Relative error for the LM algorithm \
\n(default 1e-4)",
				 relativeError, 1e-4 );
    application.addSingleOption( "-absoluteError",
                                 "Absolute error for the LM algorithm \
\n(default 1e-4)",
				 absoluteError, 1e-4 );

//     application.addSingleOption( "-riceFileName",
//                                  "Output eps file for input Rice curve",
//                                  riceFileName, true );
//     application.addSingleOption( "-fittedRiceFileName",
//                                  "Output eps file for fitted Rice curve",
//                                  fittedRiceFileName, true );

    application.initialize();

    gkg::PluginLoader::getInstance().load();

    // in fact, 'gsl' library is the default; so the following line is not
    // mandatory: it is only for didactic purpose
    gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );
    
    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    gkg::Matrix x( trialCount, 1 );
    gkg::Vector y( trialCount );
    gkg::Vector w( trialCount ); //??????????????????????????
    int32_t t;
    
    FILE *f;
    f  = fopen("plot.txt", "w");
    
    for ( t = 0; t < trialCount; t++ )
    {

      x( t, 0 ) = ( double )t;// / trialCount;
//       y( t ) = x( t, 0 ) / ( sigma * sigma ) * 
// 	std::exp( -( x( t, 0 ) * 
// 		     x( t, 0 ) + v * v ) / ( 2 * sigma * sigma ) ) *
// 	factory->getRegularBesselFunctionI0( x( t, 0 ) * v / ( sigma * 
// 							       sigma ) );
      y(t) = a / std::sqrt( 2*M_PI*sigma * sigma ) * 
	std::exp( -( ( x( t, 0 ) -  mu ) * (x( t, 0) - mu )  
		     / ( 2 * sigma * sigma ) ) ) +
	+ factory->getGaussianRandomNumber( randomGenerator, 0.0, 0.01 ); 

      w( t ) = confidence; 
      //        std::cout << "x( t, 0 ) * v / ( sigma * sigma ) ="
      // 		 << x( t, 0 ) * v / ( sigma * sigma ) << std::endl; 
		
      fprintf (f, "%f %f\n", x(t, 0), y(t));

    }
    
    fclose(f);
    
    if ( system ("gnuplot plot.plt") < 0 )
    {
    
      throw std::runtime_error( "gnuplot command failed" );
    
    }
    if ( system ("gv Rice.eps") < 0 )
    {
    
      throw std::runtime_error( "gv command failed" );
    
    }
    gkg::Vector initialParameters( 3 );
    initialParameters( 0 ) = a;
    initialParameters( 1 ) = mu; 
    initialParameters( 2 ) = sigma;
//     initialParameters( 3 ) = 0.0;
//     initialParameters( 4 ) = 0.0;
//     initialParameters( 5 ) = 0.0;
//     initialParameters( 6 ) = 0.0;
//     initialParameters( 7 ) = 0.0;
//     initialParameters( 8 ) = 0.0; 
//     initialParameters( 9 ) = 0.0;
//     initialParameters( 10 ) = 0.0;
//     initialParameters( 11 ) = 0.0;           
    SumOfGaussianFunction sumOfGaussianFunction( initialParameters );

    gkg::Matrix covariance;
    double chiSquarePerDegreeOfFreedom = 10.0;
    
    factory->getLevenbergMarquardtFit( x,
                                       y,
                                       w,
                                       sumOfGaussianFunction,
                                       covariance,
                                       chiSquarePerDegreeOfFreedom,
                                       maximumIterationCount,
                                       absoluteError,
                                       relativeError );

    std::cout << "fit of Rician function with of weighted Gaussians \n"
              << "p(0)*Gauss(p(1), p(2)) \n" 
	      << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "Ideal parameters : " 
	      << "(" << a << "," << mu << "," << sigma << ")"
	      << std::endl;
    std::cout << "fitted parameters : " << std::endl;
    std::cout << sumOfGaussianFunction.getParameters()( 0 ) << " +/- "
              << std::sqrt( covariance( 0, 0 ) ) << ", "
              << sumOfGaussianFunction.getParameters()( 1 ) << " +/- "
              << std::sqrt( covariance( 1, 1 ) ) << ", "
              << sumOfGaussianFunction.getParameters()( 2 ) << " +/- "
              << std::sqrt( covariance( 2, 2 ) ) << " )"
              << std::endl;
	          
    std::cout << "chiSquare per degree of freedom : " 
	      << chiSquarePerDegreeOfFreedom
              << std::endl;

      FILE *f2 = fopen("fit.txt", "w");
      gkg::Vector tempX( 1 );
      gkg::Vector tempP( 3 );
      tempP( 0 ) = sumOfGaussianFunction.getParameters()( 0 );
      tempP( 1 ) = sumOfGaussianFunction.getParameters()( 1 );
      tempP( 2 ) = sumOfGaussianFunction.getParameters()( 2 );
            
      for ( t = 0; t < trialCount; t++ )
       {
	
	 tempX( 0 ) = x(t, 0);
	 fprintf ( f2, "%f %f\n", x(t, 0), 
		   sumOfGaussianFunction.getValueWithParametersAt( 
							     tempX, tempP ) );
	      
       }	      
      
      fclose(f2);	    
      if ( system ("gnuplot plot_result.plt") < 0 )
      {
    
        throw std::runtime_error( "gnuplot command failed" );
    
      }
      if ( system ("gv fit.eps") < 0 )
      {
    
        throw std::runtime_error( "gv command failed" );
    
      }

  }
  GKG_CATCH_COMMAND( result );

  return result;


}


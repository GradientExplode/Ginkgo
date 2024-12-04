#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/LevenbergMarquardtFunction.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/ConstrainedGaussianMixtureFunction.h>
#include <gkg-processing-numericalanalysis/NonCentralChiFunction.h>

#include <fstream>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    double sigma;
    double v;
    int32_t channelCount;
    int32_t gaussianCount = 4;
    int32_t trialCount = 80;
    double confidence = 0.1;
    int32_t maximumIterationCount = 500;
    double absoluteError = 1e-4;
    double relativeError = 1e-4;
    bool verbose = false;
    
    ////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////
    gkg::Application application( argc, 
				  argv, "Test fit of a Rician with a "
                                  "Gaussian mixture using Levenberg-Marquart" );
 
    application.addSingleOption( "-v",
                                 "Noiseless signal\n",
                                 v );
    application.addSingleOption( "-sigma",
                                 "Sigma of the Rician distribution",
                                 sigma );
    application.addSingleOption( "-n",
                                 "Number of channels",
                                 channelCount );
    application.addSingleOption( "-gaussianCount",
                                 "Number of Gaussians\n(default 4)",
				 gaussianCount, 
				 true );
    application.addSingleOption( "-trialCount",
                                 "Trial count number\n(default 80)",
				 trialCount, 
				 true );
    application.addSingleOption( "-w",
                                 "Confidence interval\n(default 1.0)",
				 confidence,
				 true );
    application.addSingleOption( "-maximumIterationCount",
                                 "Maximum iteration count for the LM algorithm"
                                 "\n(default 500)",
				 maximumIterationCount,
				 true );
    application.addSingleOption( "-relativeError",
                                 "Relative error for the LM algorithm"
                                 "\n(default 1e-4)",
				 relativeError,
				 true );
    application.addSingleOption( "-absoluteError",
                                 "Absolute error for the LM algorithm"
                                 "\n(default 1e-4)",
				 absoluteError,
				 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
				 
    application.initialize();

    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    int32_t t;
    gkg::Vector w( trialCount ); 
    
    for ( t = 0; t < trialCount; t++ )
    {
    
      w( t ) = confidence;

    }
    
    gkg::Matrix x( trialCount, 1 );
    gkg::Matrix y( trialCount, 1 );
    gkg::Vector nonCentralChiParameters( 3 );

    nonCentralChiParameters( 0 ) = v;
    nonCentralChiParameters( 1 ) = sigma;
    nonCentralChiParameters( 2 ) = (double)channelCount;

    gkg::NonCentralChiFunction nonCentralChi( nonCentralChiParameters );
    
    nonCentralChi.getValuesAt( x, y );
    
    std::cout << "v = " << v << " , sigma = " << sigma << " , ";
    std::cout << "n = " << channelCount << std::endl;

    std::ofstream of( "nonCentralChi.txt" );
    
    if ( of.good() )
    {
    
      for ( t = 0; t < trialCount; t++ )
      {
      
        of << x( t, 0 ) << " " << y( t, 0 ) << std::endl;
	
      }
    
      of.close();
      
    }
    
    double m = nonCentralChi.getApproximateMean();
    double s = nonCentralChi.getApproximateStandardDeviation();
    
    std::cout << "m = " << m << " , sigma = " << s << std::endl;
    
    int32_t parameterCount = 3 * gaussianCount;
    gkg::Vector initialParameters( parameterCount );
    double a = std::sqrt( 1.0 / (double)gaussianCount );
    
    for ( t = 0; t < parameterCount; t += 3 )
    {
    
      initialParameters( t ) = a;
      initialParameters( t + 1 ) = std::sqrt( m );
      initialParameters( t + 2 ) = std::sqrt( s );
    
    }
    
    gkg::ConstrainedGaussianMixtureFunction gaussianMixtureFunction( 
                                                            initialParameters );

    gkg::Matrix covariance;
    double chiSquarePerDegreeOfFreedom = 0.0;
    
    factory->getLevenbergMarquardtFit( x,
                                       y.getColumn( 0 ),
                                       w,
                                       gaussianMixtureFunction,
                                       covariance,
                                       chiSquarePerDegreeOfFreedom,
                                       maximumIterationCount,
                                       absoluteError,
                                       relativeError );

    std::cout << "fit a non central Chi with a Gaussian mixture" << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "fitted parameters : " << std::endl;
    
    double sumA = 0.0;
    
    for ( t = 0; t < parameterCount; t += 3 )
    {
    
      double a = gaussianMixtureFunction.getParameters()( t );
      double m = gaussianMixtureFunction.getParameters()( t + 1 );
      double s = gaussianMixtureFunction.getParameters()( t + 2 );
      double a2 = a * a;
      
      sumA += a2;
      std::cout << a2 << " +/- "
                << std::sqrt( covariance( t, t ) ) << " , "
                << m * m << " +/- "
                << std::sqrt( covariance( t + 1, t + 1 ) ) << " , "
                << s * s << " +/- "
                << std::sqrt( covariance( t + 2, t + 2 ) ) << " )"
                << std::endl;
			      
    }
    
    std::cout << "chi square per degree of freedom : " 
              << chiSquarePerDegreeOfFreedom
              << std::endl;
    std::cout << "sum(ai) = " << sumA << std::endl;

    of.open( "fittedNonCentralChi.txt" );
    
    if ( of.good() )
    {
    
      gkg::Vector tempX( 1 );
                           
      for ( t = 0; t < trialCount; t++ )
      {
         	
	tempX( 0 ) = x( t, 0 );
	of << tempX( 0 ) << " " 
	   << gaussianMixtureFunction.getValueWithParametersAt( 
	                               tempX, 
	                               gaussianMixtureFunction.getParameters() )
	   << std::endl;

      }	      
      
      of.close();
      
    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

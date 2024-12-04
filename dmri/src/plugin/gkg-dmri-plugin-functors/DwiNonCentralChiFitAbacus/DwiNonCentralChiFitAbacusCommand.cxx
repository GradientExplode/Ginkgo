#include <gkg-dmri-plugin-functors/DwiNonCentralChiFitAbacus/DwiNonCentralChiFitAbacusCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/LevenbergMarquardtFunction.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/GaussianMixtureFunction.h>
#include <gkg-processing-numericalanalysis/ConstrainedGaussianMixtureFunction.h>
#include <gkg-processing-numericalanalysis/SemiConstrainedGaussianMixtureFunction.h>
#include <gkg-processing-numericalanalysis/NonCentralChiFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>


gkg::DwiNonCentralChiFitAbacusCommand::DwiNonCentralChiFitAbacusCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                      : gkg::Command( argc, argv, loadPlugin,
                                                      removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiNonCentralChiFitAbacusCommand::"
             "DwiNonCentralChiFitAbacusCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiNonCentralChiFitAbacusCommand::DwiNonCentralChiFitAbacusCommand(
                                            const std::string& outputName,
                                            const std::vector< double >& vs,
                                            const std::vector< double >& sigmas,
                                            double channelCount,
                                            int32_t gaussianCount,
                                            int32_t trialCount,
                                            double confidence,
                                            int32_t maximumIterationCount,
                                            double absoluteError,
                                            double relativeError,
                                            const std::string& type,
                                            bool verbose )
                                      : gkg::Command()
{

  try
  {

    execute( outputName,
             vs,
             sigmas,
             channelCount,
             gaussianCount,
             trialCount,
             confidence,
             maximumIterationCount,
             absoluteError,
             relativeError,
             type,
             verbose );

  }
  GKG_CATCH( "gkg::DwiNonCentralChiFitAbacusCommand::DwiNonCentralChiFitAbacusCommand( "
             "const std::string& outputName, "
             "const std::vector< double >& vs, "
             "const std::vector< double >& sigmas, "
             "double channelCount, "
             "int32_t gaussianCount, "
             "int32_t trialCount, "
             "double confidence, "
             "int32_t maximumIterationCount, "
             "double absoluteError, "
             "double relativeError, "
             "const std::string& type, "
             "bool verbose )" );
 
}


gkg::DwiNonCentralChiFitAbacusCommand::DwiNonCentralChiFitAbacusCommand(
                                             const gkg::Dictionary& parameters )
                                      : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, outputName );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           vs );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           sigmas );
    DECLARE_FLOATING_PARAMETER( parameters, double, channelCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, gaussianCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, trialCount );
    DECLARE_FLOATING_PARAMETER( parameters, double, confidence );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, maximumIterationCount );
    DECLARE_FLOATING_PARAMETER( parameters, double, absoluteError );
    DECLARE_FLOATING_PARAMETER( parameters, double, relativeError );
    DECLARE_STRING_PARAMETER( parameters, std::string, type );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( outputName,
             vs,
             sigmas,
             channelCount,
             gaussianCount,
             trialCount,
             confidence,
             maximumIterationCount,
             absoluteError,
             relativeError,
             type,
             verbose );

  }
  GKG_CATCH( "gkg::DwiNonCentralChiFitAbacusCommand::"
             "DwiNonCentralChiFitAbacusCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiNonCentralChiFitAbacusCommand::~DwiNonCentralChiFitAbacusCommand()
{
}


std::string gkg::DwiNonCentralChiFitAbacusCommand::getStaticName()
{

  try
  {

    return "DwiNonCentralChiFitAbacus";

  }
  GKG_CATCH( "std::string gkg::DwiNonCentralChiFitAbacusCommand::getStaticName()" );

}


void gkg::DwiNonCentralChiFitAbacusCommand::parse()
{

  try
  {

    std::string outputName;
    std::vector< double > vs;
    std::vector< double > sigmas;
    double channelCount;
    int32_t gaussianCount = 4;
    int32_t trialCount = 80;
    double confidence = 0.1;
    int32_t maximumIterationCount = 500;
    double absoluteError = 1e-4;
    double relativeError = 1e-4;
    std::string type = "semiConstrained";
    bool verbose = false;
    
    ////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////
    gkg::Application application( _argc, 
				  _argv,
                                  "Build an abacus of fits of "
                                  "non central Chi distributions "
				  "with a Gaussian mixture",
                                   _loadPlugin );
 
    application.addSingleOption( "-o",
                                 "Output abacus file",
                                 outputName );
    application.addSeriesOption( "-v",
                                 "Noiseless signal range (start,end,step)",
                                 vs,
				 3,
				 3 );
    application.addSeriesOption( "-sigma",
                                 "Sigma range (start,end,step)",
                                 sigmas,
				 3,
				 3 );
    application.addSingleOption( "-channelCount",
                                 "Number of channels of the coil",
                                 channelCount );
    application.addSingleOption( "-gaussianCount",
                                 "Number of Gaussians\n(default 4)",
				 gaussianCount, 
				 true );
    application.addSingleOption( "-trialCount",
                                 "Trial count number\n(default 80)",
				 trialCount, 
				 true );
    application.addSingleOption( "-mixtureType",
                                 "Gaussian mixture type"
                                 "\n  - \"normal\" (no positive constraints)"
                                 "\n  - \"semiConstrained\" (positive "
                                 "constraints on a_i and sigma_i)"
                                 "\n  - \"constrained\" (positive constraints "
                                 "on all parameters)"
                                 "\n(default semiConstrained)",
				 type, 
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

    execute( outputName,
             vs,
             sigmas,
             channelCount,
             gaussianCount,
             trialCount,
             confidence,
             maximumIterationCount,
             absoluteError,
             relativeError,
             type,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiNonCentralChiFitAbacusCommand::parse()" );

}


void gkg::DwiNonCentralChiFitAbacusCommand::execute(
                                            const std::string& outputName,
                                            const std::vector< double >& vs,
                                            const std::vector< double >& sigmas,
                                            double channelCount,
                                            int32_t gaussianCount,
                                            int32_t trialCount,
                                            double confidence,
                                            int32_t maximumIterationCount,
                                            double absoluteError,
                                            double relativeError,
                                            const std::string& type,
                                            bool verbose )
{

  try
  {

    // sanity check
    if ( channelCount <= 0.0 )
    {

      throw std::runtime_error( "bad parameter channelCount" );

    }

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
        
    gkg::Matrix curveX( trialCount, 1 );
    gkg::Matrix curveY( trialCount, 1 );
    gkg::Vector nonCentralChiParameters( 3 );
    double v, sigma;
    double sigmaStart = sigmas[ 0 ];
    double sigmaEnd = sigmas[ 1 ];
    double sigmaStep = sigmas[ 2 ];
    double vStart = vs[ 0 ];
    double vEnd = vs[ 1 ];
    double vStep = vs[ 2 ];
    int32_t x, y, yStart = 0;
    int32_t sizeX = int32_t( ( vEnd - vStart ) / vStep + 0.5 );
    int32_t sizeY = int32_t( ( sigmaEnd - sigmaStart ) / sigmaStep + 0.5 );
    int32_t parameterCount = 3 * gaussianCount;
    gkg::Vector3d< double > resolution( vStep, sigmaStep, 1.0 );
    
    gkg::Volume< float > abacus( sizeX, sizeY, parameterCount );
    abacus.setResolution( resolution );
    abacus.fill( 0.0f );

    nonCentralChiParameters( 2 ) = channelCount;
    
    if ( sigmaStart <= 0.0 )
    {
    
      sigmaStart = sigmaStep;
      yStart = 1;
    
    }

    for ( y = yStart, sigma = sigmaStart; y < sizeY; sigma += sigmaStep, y++ )
    {

      for ( x = 0, v = vStart; x < sizeX; v += vStep, x++ )
      {
    
        nonCentralChiParameters( 0 ) = v;
	nonCentralChiParameters( 1 ) = sigma;
	
        gkg::NonCentralChiFunction nonCentralChi( nonCentralChiParameters );
        double m = nonCentralChi.getApproximateMean();
        double s = nonCentralChi.getApproximateStandardDeviation();
    
        nonCentralChi.getValuesAt( curveX, curveY );
    
        if ( verbose )
        {

          if ( ( sigma != sigmaStart ) || ( v != vStart ) )
          {

            std::cout << gkg::Eraser( 41 );

          }
	
          std::cout << "Sigma = " << std::setw( 6 ) << sigma
	            << " / " << std::setw( 6 ) << sigmaEnd - sigmaStep
                    << " ; v = " << std::setw( 4 ) << v
		    << " / " << std::setw( 4 ) << vEnd - vStep << std::flush;

        }    

        gkg::Vector initialParameters( parameterCount );
        double a = 1.0 / (double)gaussianCount;
    
        if ( ( v / sigma ) <= 8.0 )
        {
    
          gkg::LevenbergMarquardtFunction* gaussianMixtureFunction = 0;

          if ( type == "constrained" )
          {
        
            for ( t = 0; t < parameterCount; t += 3 )
            {
    
              initialParameters( t ) = std::sqrt( a );
              initialParameters( t + 1 ) = std::sqrt( m );
              initialParameters( t + 2 ) = std::sqrt( s );
    
            }

            gaussianMixtureFunction = 
               new gkg::ConstrainedGaussianMixtureFunction( initialParameters );

          }
          else if ( type == "semiConstrained" )
          {
        
            for ( t = 0; t < parameterCount; t += 3 )
            {
    
              initialParameters( t ) = std::sqrt( a );
              initialParameters( t + 1 ) = m;
              initialParameters( t + 2 ) = std::sqrt( s );
    
            }

            gaussianMixtureFunction = 
              new gkg::SemiConstrainedGaussianMixtureFunction( 
                                                            initialParameters );

          }
          else
          {
        
            for ( t = 0; t < parameterCount; t += 3 )
            {
    
              initialParameters( t ) = a;
              initialParameters( t + 1 ) = m;
              initialParameters( t + 2 ) = s;
    
            }

            gaussianMixtureFunction = 
                          new gkg::GaussianMixtureFunction( initialParameters );

          }

          if ( !gaussianMixtureFunction )
          {

            throw std::runtime_error( "unable to allocation mixture function" );

          }

          gkg::Matrix covariance;
          double chiSquarePerDegreeOfFreedom = 0.0;
    
      	  factory->getLevenbergMarquardtFit( curveX,
      	                                     curveY.getColumn( 0 ),
      	                                     w,
      	                                     *gaussianMixtureFunction,
      	                                     covariance,
      	                                     chiSquarePerDegreeOfFreedom,
      	                                     maximumIterationCount,
      	                                     absoluteError,
      	                                     relativeError );

          if ( type == "constrained" )
          {
        
            for ( t = 0; t < parameterCount; t++ )
  	    {
	   
	      double p = gaussianMixtureFunction->getParameters()( t );	    
	      abacus( x, y, t ) = (float)( p * p );
	
	    }

          }
          else if ( type == "semiConstrained" )
          {
        
            for ( t = 0; t < parameterCount; t += 3 )
  	    {
	   
	      double p1 = gaussianMixtureFunction->getParameters()( t );
	      double p3 = gaussianMixtureFunction->getParameters()( t + 2 );
	      abacus( x, y, t ) = (float)( p1 * p1 );
	      abacus( x, y, t + 1 ) = 
                       (float)gaussianMixtureFunction->getParameters()( t + 1 );
	      abacus( x, y, t + 2 ) = (float)( p3 * p3 );
	
	    }

          }
          else
          {
        
            for ( t = 0; t < parameterCount; t++ )
  	    {
	   
	      abacus( x, y, t ) = 
                           (float)gaussianMixtureFunction->getParameters()( t );
	
	    }

          }

          delete gaussianMixtureFunction;

        }
        else
        {

          for ( t = 0; t < parameterCount; t += 3 )
          {
    
            abacus( x, y, t ) = (float)a;
            abacus( x, y, t + 1 ) = (float)m;
            abacus( x, y, t + 2 ) = (float)s;
    
          }

        }

      }
    
    }

    abacus.getHeader().addAttribute( "abacus", 
                                     std::string( "non central chi" ) );
    abacus.getHeader().addAttribute( "signalStart", vStart );
    abacus.getHeader().addAttribute( "sigmaStart", sigmaStart );
    abacus.getHeader().addAttribute( "channelCount", channelCount );
    
    if ( verbose )
    {

      std::cout << std::endl;
      std::cout << "writing '" << outputName << "' volume : " << std::flush;
    
    }
    
    gkg::Writer::getInstance().write( outputName, abacus );
    
    if ( verbose )
    {
    
      std::cout << "done" << std::endl;
    
    }

  }
  GKG_CATCH( "void gkg::DwiNonCentralChiFitAbacusCommand::execute( "
             "const std::string& outputName, "
             "const std::vector< double >& vs, "
             "const std::vector< double >& sigmas, "
             "double channelCount, "
             "int32_t gaussianCount, "
             "int32_t trialCount, "
             "double confidence, "
             "int32_t maximumIterationCount, "
             "double absoluteError, "
             "double relativeError, "
             "const std::string& type, "
             "bool verbose )" );

}


RegisterCommandCreator(
    DwiNonCentralChiFitAbacusCommand,
    DECLARE_STRING_PARAMETER_HELP( outputName ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( vs ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( sigmas ) +
    DECLARE_FLOATING_PARAMETER_HELP( channelCount ) +
    DECLARE_INTEGER_PARAMETER_HELP( gaussianCount ) +
    DECLARE_INTEGER_PARAMETER_HELP( trialCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( confidence ) +
    DECLARE_INTEGER_PARAMETER_HELP( maximumIterationCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( absoluteError ) +
    DECLARE_FLOATING_PARAMETER_HELP( relativeError ) +
    DECLARE_STRING_PARAMETER_HELP( type ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );

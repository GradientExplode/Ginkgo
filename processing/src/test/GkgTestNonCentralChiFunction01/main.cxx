#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/NonCentralChiFunction.h>
#include <gkg-core-exception/Exception.h>

#include <fstream>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    double v;
    double sigma;
    int32_t channelCount;
    int32_t trialCount = 80;
    bool verbose = false;
    
    ////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////
    gkg::Application application( argc, 
				  argv, "Test Rice Function" );
 
    application.addSingleOption( "-v",
                                 "Noiseless signal value",
                                 v );
    application.addSingleOption( "-sigma",
                                 "Sigma value",
                                 sigma );
    application.addSingleOption( "-n",
                                 "Number of channels",
                                 channelCount );
    application.addSingleOption( "-trialCount",
                                 "Trial count number\n(default 80)",
				 trialCount, 
				 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
				 
    application.initialize();
        
    gkg::Matrix curveX( trialCount, 1 );
    gkg::Matrix curveY( trialCount, 1 );
    gkg::Vector nonCentralChiParameters( 3 );

    nonCentralChiParameters( 0 ) = v;
    nonCentralChiParameters( 1 ) = sigma;
    nonCentralChiParameters( 2 ) = (double)channelCount;

    gkg::NonCentralChiFunction nonCentralChi( nonCentralChiParameters );
    double m = nonCentralChi.getApproximateMean();
    
    if ( verbose )
    {

      std::cout << "Building non central Chi distribution curve" << std::endl;
      std::cout << "Approximated mean = " << m << std::endl;

    }

    nonCentralChi.getValuesAt( curveX, curveY );

    if ( verbose )
    {

      std::cout << "writing \"nonCentralChi.txt\" : " << std::flush;

    }

    std::ofstream of( "nonCentralChi.txt" );

    if ( of.good() )
    {

      int32_t t;

      for ( t = 0; t < trialCount; t++ )
      {

        of << curveX( t, 0 ) << " " << curveY( t, 0 ) << std::endl;

      }

      of.close();

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


  }
  GKG_CATCH_COMMAND( result );

  return result;

}

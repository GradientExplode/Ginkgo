#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-plot/CurvePlotter.h>
#include <gkg-processing-plot/PlotterFactory.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main( int argc, char* argv[] )
{


  int result = EXIT_SUCCESS;

  try
  {

    int32_t sampleCount = 0;
    double step = 0.1f;
    std::string outputName = "";
    double alpha = 1.0f;

    gkg::Application application( argc, argv,
                                  "Bessel's function J0 testing" );
    application.addSingleOption( "-c",
                                 "Sample count",
                                 sampleCount );
    application.addSingleOption( "-s",
                                 "step (default=0.1)",
                                 step );
    application.addSingleOption( "-o",
                                 "Output text file",
                                 outputName );
    application.addSingleOption( "-a",
                                 "Alpha scale for J(a.x) (default=1.0)",
                                 alpha,
                                 true );

    application.initialize();

    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    std::vector< double > x( sampleCount );
    std::vector< double > y( sampleCount );

    // testing Bessel's function J0
    int32_t s;
    for ( s = 0; s < sampleCount; s++ )
    {

      x[ s ] = s * step;
      y[ s ] = factory->getRegularBesselFunctionJ0( alpha * s * step );

    }

    // now we select Postscript plotter with Plotter factory
    gkg::PlotterFactory::getInstance().select( "PostscriptPlot" );
    gkg::CurvePlotter postscriptCurvePlotter;

    postscriptCurvePlotter.plot( x, y, "r", "J0(0.225r)",
                                 "Zeroth order Bessel function" );


    if ( !outputName.empty() )
    {

      std::ofstream os( outputName.c_str() );
      for ( s = 0; s < sampleCount; s++ )
      {

        os << x[ s ] << "  " << y[ s ] << std::endl;

      }
      os.close();

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;


}

#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-fmri-contrast/ContrastEffects.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{

  int32_t result = EXIT_SUCCESS;

  try
  {

    gkg::PluginLoader::getInstance().load();

    gkg::ContrastEffects contrastEffects;

    gkg::Reader::getInstance().read( "example.contrasts.minf",
                                     contrastEffects );

    std::cout << std::endl;

    int32_t index;

    for ( index = 0; index < contrastEffects.getContrastCount(); index++ )
    {

      const gkg::ContrastEffects::Information&
        information = contrastEffects.getInformation( index );

      std::cout << "------------------"
                << information.name
                << "------------------" << std::endl;

      std::cout << "regressor names : " << std::flush;
      int32_t r = 0;
      for ( r = 0; r < ( int32_t )information.regressorNames.size(); r++ )
      {

        std::cout << information.regressorNames[ r ] << "   ";

      }
      std::cout << std::endl;

      std::cout << "weights : " << std::flush;
      int32_t w = 0;
      for ( w = 0; w < information.weights.getSize(); w++ )
      {

        std::cout << information.weights( w ) << "   ";

      }
      std::cout << std::endl;

      std::cout << std::endl;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

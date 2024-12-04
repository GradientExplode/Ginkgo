#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-fmri-paradigm/Paradigm.h>
#include <gkg-fmri-stimulus/DiscreteStimulus.h>
#include <gkg-fmri-stimulus/ContinuousStimulus.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{

  int32_t result = EXIT_SUCCESS;

  try
  {

    gkg::PluginLoader::getInstance().load();

    gkg::Paradigm paradigm;

    gkg::Reader::getInstance().read( "example.paradigm.minf", paradigm );

    std::cout << std::endl;

    std::list< gkg::Stimulus* >::const_iterator
      s = paradigm.getStimuli().begin(),
      se = paradigm.getStimuli().end();
    while ( s != se )
    {

      if ( dynamic_cast< gkg::DiscreteStimulus* >( *s ) )
      {

        gkg::DiscreteStimulus* discreteStimulus = 
          dynamic_cast< gkg::DiscreteStimulus* >( *s );

        std::cout << "discrete stimulus : " << std::endl;
        std::cout << "----------------- " << std::endl;

        std::cout << "name = "
                  << discreteStimulus->getName()
                  << std::endl;
        std::cout << "times = "
                  << discreteStimulus->getTimes()
                  << std::endl;
        std::cout << "durations = "
                  << discreteStimulus->getDurations()
                  << std::endl;
        std::cout << "magnitudes = "
                  << discreteStimulus->getMagnitudes()
                  << std::endl;

      }
      else if ( dynamic_cast< gkg::ContinuousStimulus* >( *s ) )
      {

        gkg::ContinuousStimulus* continuousStimulus = 
          dynamic_cast< gkg::ContinuousStimulus* >( *s );

        std::cout << "continuous stimulus : " << std::endl;
        std::cout << "------------------- " << std::endl;

        std::cout << "name = "
                  << continuousStimulus->getName()
                  << std::endl;
        std::cout << "starting_time = "
                  << continuousStimulus->getStartingTime()
                  << std::endl;
        std::cout << "sampling_period = "
                  << continuousStimulus->getSamplingPeriod()
                  << std::endl;
        std::cout << "magnitudes = "
                  << continuousStimulus->getMagnitudes()
                  << std::endl;

      }

      std::cout << std::endl;

      ++ s;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

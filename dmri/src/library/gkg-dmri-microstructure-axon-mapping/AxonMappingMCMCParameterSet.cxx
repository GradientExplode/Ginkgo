#include <gkg-dmri-microstructure-axon-mapping/AxonMappingMCMCParameterSet.h>
#include <gkg-core-exception/Exception.h>


gkg::AxonMappingMCMCParameterSet::AxonMappingMCMCParameterSet( 
                                               const gkg::Vector& lowerBounds,
                                               const gkg::Vector& upperBounds,
                                               const std::vector< bool >& fixed,
                                               const gkg::Vector& steps )
                                 : gkg::MonteCarloMarkovChainParameterSet( 
                                                          lowerBounds.getSize(),
                                                          lowerBounds,
                                                          upperBounds,
                                                          fixed,
                                                          steps )                             
{

  try
  {

    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    _randomGenerator.reset(
                       new gkg::RandomGenerator( gkg::RandomGenerator::Taus ) );

  }
  GKG_CATCH( "gkg::AxonMappingMCMCParameterSet::AxonMappingMCMCParameterSet( "
             "const gkg::Vector& lowerBounds, "
             "const gkg::Vector& upperBounds, "
             "const std::vector< bool >& fixed, "
             "const gkg::Vector& steps )" );

}


gkg::AxonMappingMCMCParameterSet::~AxonMappingMCMCParameterSet()
{
}



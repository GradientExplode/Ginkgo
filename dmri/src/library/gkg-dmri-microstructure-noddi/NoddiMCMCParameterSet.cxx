#include <gkg-dmri-microstructure-noddi/NoddiMCMCParameterSet.h>
#include <gkg-core-exception/Exception.h>


gkg::NoddiMCMCParameterSet::NoddiMCMCParameterSet( 
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
  GKG_CATCH( "gkg::NoddiMCMCParameterSet::NoddiMCMCParameterSet( "
             "const gkg::Vector& lowerBounds, "
             "const gkg::Vector& upperBounds, "
             "const std::vector< bool >& fixed, "
             "const gkg::Vector& steps )" );

}


gkg::NoddiMCMCParameterSet::~NoddiMCMCParameterSet()
{
}



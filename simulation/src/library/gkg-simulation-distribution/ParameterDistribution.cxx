#include <gkg-simulation-distribution/ParameterDistribution.h>
#include <gkg-core-exception/Exception.h>


gkg::ParameterDistribution::ParameterDistribution()
                           : _randomGenerator( gkg::RandomGenerator::Taus )
{

  try
  {

    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

  }
  GKG_CATCH( "gkg::ParameterDistribution::ParameterDistribution()" );

}


gkg::ParameterDistribution::~ParameterDistribution()
{
}




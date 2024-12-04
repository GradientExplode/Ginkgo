#include <gkg-simulation-distribution/UniformDistribution.h>
#include <gkg-simulation-distribution/DistributionFactory.h>
#include <gkg-core-exception/Exception.h>


gkg::UniformDistribution::UniformDistribution( 
                                             const gkg::Dictionary& parameters )
                         : gkg::ParameterDistribution()
{

  try
  {

    gkg::Dictionary::const_iterator p = parameters.find( "lower_boundary" );

    if ( p != parameters.end() )
    {

      _lowerBoundary = p->second->getScalar();


      p = parameters.find( "upper_boundary" );
      if ( p == parameters.end() )
      {

        throw std::runtime_error( "a 'upper_boundary' item is required" );

      }
      _upperBoundary = p->second->getScalar();


    }
    else
    {

      throw std::runtime_error( "a 'lower_boundary' item is required" );

    }

  }
  GKG_CATCH( "gkg::UniformDistribution::UniformDistribution( "
             "const gkg::Dictionary& parameters )" );

}


gkg::UniformDistribution::~UniformDistribution()
{
}



float gkg::UniformDistribution::getRandomValue() const
{

  try
  {

    return ( float )_factory->getUniformRandomNumber( _randomGenerator,
                                                      _lowerBoundary,
                                                      _upperBoundary );

  }
  GKG_CATCH( "float gkg::UniformDistribution::getRandomValue() const" );
}


std::string gkg::UniformDistribution::getStaticName()
{

  try
  {

    return "uniform-distribution";

  }
  GKG_CATCH( "std::string gkg::UniformDistribution::getStaticName()" );

}


//
// adding the UniformDistribution creator to the distribution factory
//

RegisterDistributionCreator( gkg, UniformDistribution );

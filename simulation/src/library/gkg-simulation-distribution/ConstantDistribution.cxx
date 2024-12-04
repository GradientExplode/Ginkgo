#include <gkg-simulation-distribution/ConstantDistribution.h>
#include <gkg-simulation-distribution/DistributionFactory.h>
#include <gkg-core-exception/Exception.h>


gkg::ConstantDistribution::ConstantDistribution( 
                                             const gkg::Dictionary& parameters )
                          : gkg::ParameterDistribution()
{

  try
  {

    gkg::Dictionary::const_iterator p = parameters.find( "constant_value" );

    if ( p != parameters.end() )
    {

      _constantValue = ( float )p->second->getScalar();

    }
    else
    {

      throw std::runtime_error( "a 'constant_value' item is required" );

    }

  }
  GKG_CATCH( "gkg::ConstantDistribution::ConstantDistribution( "
             "const gkg::Dictionary& parameters )" );

}


gkg::ConstantDistribution::~ConstantDistribution()
{
}



float gkg::ConstantDistribution::getRandomValue() const
{

  try
  {

    return _constantValue;

  }
  GKG_CATCH( "float gkg::ConstantDistribution::getRandomValue() const" );
}


std::string gkg::ConstantDistribution::getStaticName()
{

  try
  {

    return "constant-distribution";

  }
  GKG_CATCH( "std::string gkg::ConstantDistribution::getStaticName()" );

}


//
// adding the ConstantDistribution creator to the distribution factory
//

RegisterDistributionCreator( gkg, ConstantDistribution );

#include <gkg-processing-algobase/NLMNoiseModelFactory.h>
#include <gkg-processing-algobase/NLMGaussianNoiseModel.h>
#include <gkg-processing-algobase/NLMRicianNoiseModel.h>
#include <gkg-processing-algobase/NLMNonCentralChiNoiseModel.h>
#include <gkg-core-exception/Exception.h>


gkg::NLMNoiseModelFactory::NLMNoiseModelFactory()
                         : gkg::Singleton< NLMNoiseModelFactory >()
{
}


gkg::NLMNoiseModelFactory::~NLMNoiseModelFactory()
{
}


gkg::NLMNoiseModel* gkg::NLMNoiseModelFactory::createNLMNoiseModelFunction( 
                                                      const std::string& name,
                                                      float sigma,
                                                      float channelCount ) const
{

  try
  {
  
    if ( name == "gaussian" )
    {

      return new gkg::NLMGaussianNoiseModel( sigma );

    }
    else if ( name == "rician" )
    {

      return new gkg::NLMRicianNoiseModel( sigma );

    }
    else if ( name == "nonCentralChi" )
    {

      return new gkg::NLMNonCentralChiNoiseModel( sigma, channelCount );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }
    
  }
  GKG_CATCH( "gkg::NLMNoiseModel* "
             "gkg::NLMNoiseModelFactory::createNLMNoiseModelFunction( "
             "const std::string& name, float sigma, "
             "float channelCount ) const" );

}

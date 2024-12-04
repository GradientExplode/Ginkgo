#include <gkg-simulation-distribution/DistributionFactory.h>
#include <gkg-core-exception/Exception.h>


gkg::DistributionFactory::DistributionFactory()
{
}


gkg::DistributionFactory::~DistributionFactory()
{
}


bool gkg::DistributionFactory::registerDistributionCreator(
                                     const std::string& name,
                                     gkg::DistributionFactory::Creator creator )
{

  try
  {

    if ( creator )
    {

      std::map< std::string, gkg::DistributionFactory::Creator >::const_iterator
        c = _creators.find( name );

      if ( c == _creators.end() )
      {

        _creators[ name ] = creator;
        return true;

      }
      else
      {

        std::cerr << "gkg::DistributionFactory::"
                  << "registerDistributionCreator: " << name
                  << " already exists"
                  << std::endl;
        return false;

      }

    }
    return false;


  }
  GKG_CATCH( "bool gkg::DistributionFactory::registerDistributionCreator( "
             "const std::string& name, "
             "gkg::DistributionFactory::Creator creator )" );

}


gkg::ParameterDistribution* 
gkg::DistributionFactory::create( const gkg::Dictionary& dictionary ) const
{

  try
  {

    gkg::Dictionary::const_iterator i = dictionary.find( "type" );
    if ( i == dictionary.end() )
    {

      throw std::runtime_error( "a 'type' item is required" );

    }
    std::string name = i->second->getString();

    i = dictionary.find( "parameters" );
    if ( i == dictionary.end() )
    {

      throw std::runtime_error( "a 'parameters' item is required" );

    }
    gkg::Dictionary parameters = i->second->getValue< gkg::Dictionary >();

    std::map< std::string, gkg::DistributionFactory::Creator >::const_iterator
      c = _creators.find( name );

    if ( c != _creators.end() )
    {

      return ( *c->second )( parameters );

    }


    throw std::runtime_error( std::string( "creator for '"  ) + name +
                              "' distribution type not found" );

    return 0;

  }
  GKG_CATCH( "gkg::ParameterDistribution* "
             "gkg::DistributionFactory::create( "
             "int32_t id, "
             "const gkg::Dictionary& dictionary ) const" );

}


std::set< std::string > 
gkg::DistributionFactory::getDistributionTypeNames() const
{

  try
  {

    std::set< std::string > distributionTypeNames;

    std::map< std::string, gkg::DistributionFactory::Creator >::const_iterator
      c = _creators.begin(),
      ce = _creators.end();

    while ( c != ce )
    {

      distributionTypeNames.insert( c->first );
      ++ c;

    }

    return distributionTypeNames;

  }
  GKG_CATCH( "std::set< std::string > "
             "gkg::DistributionFactory::getDistributionTypeNames() const" );

}

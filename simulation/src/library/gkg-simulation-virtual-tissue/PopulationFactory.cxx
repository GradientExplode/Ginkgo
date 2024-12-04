#include <gkg-simulation-virtual-tissue/PopulationFactory.h>
#include <gkg-core-exception/Exception.h>




gkg::PopulationFactory::PopulationFactory()
{
}


gkg::PopulationFactory::~PopulationFactory()
{
}


bool gkg::PopulationFactory::registerPopulationCreator(
                                       const std::string& name,
                                       gkg::PopulationFactory::Creator creator )
{

  try
  {

    if ( creator )
    {

      std::map< std::string, gkg::PopulationFactory::Creator >::const_iterator
        c = _creators.find( name );

      if ( c == _creators.end() )
      {

        _creators[ name ] = creator;
        return true;

      }
      else
      {

        std::cerr << "gkg::PopulationFactory::"
                  << "registerPopulationCreator: " << name
                  << " already exists"
                  << std::endl;
        return false;

      }

    }
    return false;


  }
  GKG_CATCH( "bool gkg::PopulationFactory::registerPopulationCreator( "
             "const std::string& name, "
             "gkg::PopulationFactory::Creator creator )" );

}


gkg::Population* 
gkg::PopulationFactory::create( int32_t id,
                                const gkg::VirtualTissue* parent,
                                const gkg::Dictionary& dictionary,
                                bool verbose,
                                std::ofstream* osLog ) const
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

    std::map< std::string, gkg::PopulationFactory::Creator >::const_iterator
      c = _creators.find( name );

    if ( c != _creators.end() )
    {

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "    found type : " << name << std::endl;
          *osLog << "    id : " << id << std::endl;

        }
        else
        {

          std::cout << "    found type : " << name << std::endl;
          std::cout << "    id : " << id << std::endl;

        }

      }

      return ( *c->second )( id, parent, parameters, verbose, osLog );

    }


    throw std::runtime_error( std::string( "creator for '"  ) + name +
                              "' population type not found" );

    return 0;

  }
  GKG_CATCH( "gkg::Population* "
             "gkg::PopulationFactory::create( "
             "int32_t id, "
             "const VirtualTissue* parent, "
             "const gkg::Dictionary& dictionary ) const" );

}


std::set< std::string > gkg::PopulationFactory::getPopulationTypeNames() const
{

  try
  {

    std::set< std::string > populationTypeNames;

    std::map< std::string, gkg::PopulationFactory::Creator >::const_iterator
      c = _creators.begin(),
      ce = _creators.end();

    while ( c != ce )
    {

      populationTypeNames.insert( c->first );
      ++ c;

    }

    return populationTypeNames;

  }
  GKG_CATCH( "std::set< std::string > "
             "gkg::PopulationFactory::getPopulationTypeNames() const" );

}

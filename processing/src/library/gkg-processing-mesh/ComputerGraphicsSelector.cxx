#include <gkg-processing-mesh/ComputerGraphicsSelector.h>
#include <gkg-core-exception/Exception.h>


#define DEFAULT_COMPUTER_GRAPHICS_IMPLEMENTATION_FACTORY  "libcgal"


gkg::ComputerGraphicsSelector::ComputerGraphicsSelector()
                              : gkg::Singleton<
                                              gkg::ComputerGraphicsSelector >(),
                                _implementationFactory( 0 ),
                                _ready( false )
{
}


gkg::ComputerGraphicsSelector::~ComputerGraphicsSelector()
{

  _accessors.clear();

}


gkg::ComputerGraphicsImplementationFactory* 
gkg::ComputerGraphicsSelector::getImplementationFactory()
{

  try
  {

    if ( !_ready )
    {

      select( DEFAULT_COMPUTER_GRAPHICS_IMPLEMENTATION_FACTORY );

    }

    return _implementationFactory;

  }
  GKG_CATCH( "gkg::ComputerGraphicsImplementationFactory* "
             "gkg::ComputerGraphicsSelector::getImplementationFactory()" );

}


void gkg::ComputerGraphicsSelector::registerFactory(
                              const std::string& name,
                              gkg::ComputerGraphicsSelector::Accessor accessor )
{

  try
  {

    if ( accessor )
    {

      std::map< std::string, 
                gkg::ComputerGraphicsSelector::Accessor >::const_iterator
        a = _accessors.find( name );

      if ( a == _accessors.end() )
      {

        _accessors[ name ] = accessor;

      }
      else
      {

        throw std::runtime_error(
                std::string( "computer graphics implementation factory" ) +
                             " '" + name + " already registered" );

      }

    }

  }
  GKG_CATCH( "void gkg::ComputerGraphicsSelector::registerFactory( "
             "const std::string& name, "
             "gkg::ComputerGraphicsSelector::Accessor accessor )" );

}


void gkg::ComputerGraphicsSelector::select( const std::string& name )
{

  try
  {

    std::map< std::string,
              gkg::ComputerGraphicsSelector::Accessor >::const_iterator
      a = _accessors.find( name );

    if ( a != _accessors.end() )
    {

      _implementationFactory = &( *a->second )();
      _ready = true;

    }
    else
    {

      throw std::runtime_error(
             std::string( "unable to find computer graphics implementation" ) +
             " factory '" + name + "'" );

    }

  }
  GKG_CATCH( "void gkg::ComputerGraphicsSelector::select( "
             "const std::string& name )" );

}


#undef DEFAULT_COMPUTER_GRAPHICS_IMPLEMENTATION_FACTORY

#include <gkg-communication-command/CommandFactory.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::CommandFactory::CommandFactory()
                   : gkg::Singleton< gkg::CommandFactory >()
{
}


bool gkg::CommandFactory::registerCommand(
const std::string& name, 
gkg::CommandFactory::CommandCreatorFromArgcArgv commandCreatorFromArgcArgv,
gkg::CommandFactory::CommandCreatorFromDictionary commandCreatorFromDictionary,
gkg::CommandFactory::CommandHelp commandHelp )
{

  try
  {

    if ( commandCreatorFromArgcArgv )
    {

      std::map< std::string, 
                gkg::CommandFactory::CommandCreatorFromArgcArgv,
                noCaseCompLess >::const_iterator
        c = _commandCreatorsFromArgcArgv.find( name );

      if ( c == _commandCreatorsFromArgcArgv.end() )
      {

        _commandCreatorsFromArgcArgv.insert(
                         std::make_pair( name, commandCreatorFromArgcArgv ) );
        _commandCreatorsFromDictionary.insert(
                         std::make_pair( name, commandCreatorFromDictionary ) );
        _commandHelps.insert(
                         std::make_pair( name, commandHelp ) );

        return true;

      }
      else
      {

        std::cerr << "gkg::CommandFactory::registerCommand: " << name
                  << " already exists" << std::endl;

      }

    }

    return false;

  }
  GKG_CATCH( "bool gkg::CommandFactory::registerCommand( "
             "const std::string& name, "
             "gkg::CommandFactory::CommandCreatorFromArgcArgv "
             "commandCreatorFromArgcArgv, "
             "gkg::CommandFactory::CommandCreatorFromDictionary "
             "commandCreatorFromDictionary, "
             "gkg::CommandFactory::CommandHelp commandHelp )" );

}


gkg::Command* gkg::CommandFactory::create( int32_t argc, char** argv ) const
{

  try
  {

    std::map< std::string, 
              gkg::CommandFactory::CommandCreatorFromArgcArgv,
              noCaseCompLess >::const_iterator
      c = _commandCreatorsFromArgcArgv.find( argv[ 1 ] );

    if ( c != _commandCreatorsFromArgcArgv.end() )
    {

      return ( *c->second )( argc, argv );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + argv[ 1 ] +
                                "' not found" );

    }

  }
  GKG_CATCH( "gkg::Command* gkg::CommandFactory::create( "
             "const std::string& name, int32_t argc, char** argv )" );

}


gkg::Command* gkg::CommandFactory::create(
                                       const std::string& name,
                                       const gkg::Dictionary& parameters ) const
{

  try
  {

    std::map< std::string, 
              gkg::CommandFactory::CommandCreatorFromDictionary,
              noCaseCompLess >::const_iterator
      c = _commandCreatorsFromDictionary.find( name );

    if ( c != _commandCreatorsFromDictionary.end() )
    {

      return ( *c->second )( parameters );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name +
                                "' not found" );

    }

  }
  GKG_CATCH( "gkg::Command* gkg::CommandFactory::create( "
             "const std::string& name, const gkg::Dictionary& parameters )" );

}


std::string gkg::CommandFactory::getHelp( const std::string& name ) const
{

  try
  {

    std::string help = name + " parameters:\n";
    std::map< std::string, 
              gkg::CommandFactory::CommandHelp,
              noCaseCompLess >::const_iterator
      c = _commandHelps.find( name );

    if ( c != _commandHelps.end() )
    {

      help += ( *c->second )(); 
      return help;

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name +
                                "' not found" );

    }

  }
  GKG_CATCH( "std::string gkg::CommandFactory::getHelp( "
             "const std::string& name )" );

}


std::vector< std::string > gkg::CommandFactory::getNames() const
{

  try
  {

    std::vector< std::string > names;
    std::map< std::string, 
              gkg::CommandFactory::CommandCreatorFromArgcArgv,
              noCaseCompLess >::const_iterator
      c = _commandCreatorsFromArgcArgv.begin(),
      ce = _commandCreatorsFromArgcArgv.end();

    while ( c != ce )
    {

      names.push_back( c->first );
      ++c;

    }

    return names;

  }
  GKG_CATCH( "std::vector< std::string > "
             "gkg::CommandFactory::getNames() const" );

}

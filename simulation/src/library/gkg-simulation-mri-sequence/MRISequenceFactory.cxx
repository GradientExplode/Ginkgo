#include <gkg-simulation-mri-sequence/MRISequenceFactory.h>
#include <gkg-core-exception/Exception.h>


gkg::MRISequenceFactory::MRISequenceFactory()
{
}


gkg::MRISequenceFactory::~MRISequenceFactory()
{
}


bool gkg::MRISequenceFactory::registerMRISequenceCreator(
                                     const std::string& name,
                                     gkg::MRISequenceFactory::Creator creator )
{

  try
  {

    if ( creator )
    {

      std::map< std::string, gkg::MRISequenceFactory::Creator >::const_iterator
        c = _creators.find( name );

      if ( c == _creators.end() )
      {

        _creators[ name ] = creator;
        return true;

      }
      else
      {

        std::cerr << "gkg::MRISequenceFactory::"
                  << "registerMRISequenceCreator: " << name
                  << " already exists"
                  << std::endl;
        return false;

      }

    }
    return false;


  }
  GKG_CATCH( "bool gkg::MRISequenceFactory::registerMRISequenceCreator( "
             "const std::string& name, "
             "gkg::MRISequenceFactory::Creator creator )" );

}


gkg::MRISequence* 
gkg::MRISequenceFactory::create( const gkg::Dictionary& dictionary,
                                 float timeStepInUs,
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

    std::map< std::string, gkg::MRISequenceFactory::Creator >::const_iterator
      c = _creators.find( name );

    if ( c != _creators.end() )
    {

      return ( *c->second )( parameters, timeStepInUs, verbose, osLog );

    }


    throw std::runtime_error( std::string( "creator for '"  ) + name +
                              "' distribution type not found" );

    return 0;

  }
  GKG_CATCH( "gkg::MRISequence* "
             "gkg::MRISequenceFactory::create( "
             "int32_t id, "
             "const gkg::Dictionary& dictionary, "
             "float timeStepInUs, "
             "bool verbose, "
             "std::ofstream* osLog ) const" );

}


std::set< std::string > 
gkg::MRISequenceFactory::getMRISequenceTypeNames() const
{

  try
  {

    std::set< std::string > mriSequenceTypeNames;

    std::map< std::string, gkg::MRISequenceFactory::Creator >::const_iterator
      c = _creators.begin(),
      ce = _creators.end();

    while ( c != ce )
    {

      mriSequenceTypeNames.insert( c->first );
      ++ c;

    }

    return mriSequenceTypeNames;

  }
  GKG_CATCH( "std::set< std::string > "
             "gkg::MRISequenceFactory::getMRISequenceTypeNames() const" );

}

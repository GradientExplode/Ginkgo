#include <gkg-dmri-bundle-distance/BundleDistanceFunctorFactory.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::BundleDistanceFunctorFactory::BundleDistanceFunctorFactory()
{
}


gkg::BundleDistanceFunctorFactory::~BundleDistanceFunctorFactory()
{
}


bool gkg::BundleDistanceFunctorFactory::registerBundleDistanceFunctor(
                 const std::string& name,
                 gkg::BundleDistanceFunctorFactory::BundleDistanceFunctorCreator 
                                                   bundleDistanceFunctorCreator,
                 const std::string& scalarParameterHelp )
{

  try
  {

    if ( bundleDistanceFunctorCreator )
    {

      std::map< std::string,
                gkg::BundleDistanceFunctorFactory::
                     BundleDistanceFunctorCreator >::const_iterator
        c = _bundleDistanceFunctorCreators.find( name );

      if ( c == _bundleDistanceFunctorCreators.end() )
      {

        _bundleDistanceFunctorCreators[ name ] = bundleDistanceFunctorCreator;
        _scalarParameterHelps[ name ] = scalarParameterHelp;
        return true;

      }
      else
      {

        std::cerr << "gkg::BundleDistanceFunctorFactory::"
                  << "registerBundleDistanceFunctor: " << name
                  << " already exists"
                  << std::endl;
        return false;

      }

    }
    return false;


  }
  GKG_CATCH( "bool gkg::BundleDistanceFunctorFactory::"
             "registerBundleDistanceFunctor( "
             "const std::string& name, "
             "gkg::BundleDistanceFunctorFactory::BundleDistanceFunctorCreator "
             "bundleDistanceFunctorCreator, "
             "const std::string& scalarParameterHelp )" );

}



gkg::BundleDistanceFunctor*
gkg::BundleDistanceFunctorFactory::create(
                           const std::string& name,
                           const std::vector< double >& scalarParameters ) const
{

  try
  {

    typename std::map< std::string,
                       gkg::BundleDistanceFunctorFactory::
                            BundleDistanceFunctorCreator >::const_iterator
      c = _bundleDistanceFunctorCreators.find( name );
    if ( c != _bundleDistanceFunctorCreators.end() )
    {

      return ( *c->second )( scalarParameters );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "gkg::BundleDistanceFunctor* "
             "gkg::BundleDistanceFunctorFactory::create( "
             "const std::string& name, "
             "const std::vector< double >& scalarParameters ) const" );

}


bool gkg::BundleDistanceFunctorFactory::hasBundleDistanceFunctor(
                                                 const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       gkg::BundleDistanceFunctorFactory::
                       BundleDistanceFunctorCreator >::const_iterator
      c = _bundleDistanceFunctorCreators.find( name );
    if ( c != _bundleDistanceFunctorCreators.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "bool gkg::BundleDistanceFunctorFactory::hasBundleDistanceFunctor( "
             "const std::string& name ) const" );

}


std::set< std::string > gkg::BundleDistanceFunctorFactory::getNames() const
{

  try
  {

    std::set< std::string > names;
    typename std::map< std::string,
                       gkg::BundleDistanceFunctorFactory::
                       BundleDistanceFunctorCreator >::const_iterator
      c = _bundleDistanceFunctorCreators.begin(),
      ce = _bundleDistanceFunctorCreators.end();

    while ( c != ce )
    {

      names.insert( c->first );
      ++ c;

    }
    return names;

  }
  GKG_CATCH( "std::set< std::string > "
             "gkg::BundleDistanceFunctorFactory::getNames() const" );

}


std::string gkg::BundleDistanceFunctorFactory::getNameListHelp() const
{

  try
  {

    std::string help;

    std::set< std::string > names = this->getNames();

    std::set< std::string >::const_iterator
      n = names.begin(),
      ne = names.end();
    while ( n != ne )
    {

      help += "- " + *n + "\n";
      ++ n;

    }
    return help;

  }
  GKG_CATCH( "std::string "
             "gkg::BundleDistanceFunctorFactory::getNameListHelp() const" );

}


std::string
gkg::BundleDistanceFunctorFactory::getScalarParameterHelp(
                                                 const std::string& name ) const
{

  try
  {

    std::map< std::string, std::string >::const_iterator
      h = _scalarParameterHelps.find( name );
    if ( h == _scalarParameterHelps.end() )
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }
    return h->second;


  }
  GKG_CATCH( "std::string "
             "gkg::BundleDistanceFunctorFactory::"
             "getScalarParameterHelp( "
             "const std::string& name ) const" );

}

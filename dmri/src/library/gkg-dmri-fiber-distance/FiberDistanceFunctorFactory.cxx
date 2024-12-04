#include <gkg-dmri-fiber-distance/FiberDistanceFunctorFactory.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::FiberDistanceFunctorFactory::FiberDistanceFunctorFactory()
{
}


gkg::FiberDistanceFunctorFactory::~FiberDistanceFunctorFactory()
{
}


bool gkg::FiberDistanceFunctorFactory::registerFiberDistanceFunctor(
                 const std::string& name,
                 gkg::FiberDistanceFunctorFactory::FiberDistanceFunctorCreator 
                                                   fiberDistanceFunctorCreator,
                 const std::string& scalarParameterHelp )
{

  try
  {

    if ( fiberDistanceFunctorCreator )
    {

      std::map< std::string,
                gkg::FiberDistanceFunctorFactory::
                     FiberDistanceFunctorCreator >::const_iterator
        c = _fiberDistanceFunctorCreators.find( name );

      if ( c == _fiberDistanceFunctorCreators.end() )
      {

        _fiberDistanceFunctorCreators[ name ] = fiberDistanceFunctorCreator;
        _scalarParameterHelps[ name ] = scalarParameterHelp;
        return true;

      }
      else
      {

        std::cerr << "gkg::FiberDistanceFunctorFactory::"
                  << "registerFiberDistanceFunctor: " << name
                  << " already exists"
                  << std::endl;
        return false;

      }

    }
    return false;


  }
  GKG_CATCH( "bool gkg::FiberDistanceFunctorFactory::"
             "registerFiberDistanceFunctor( "
             "const std::string& name, "
             "gkg::FiberDistanceFunctorFactory::FiberDistanceFunctorCreator "
             "fiberDistanceFunctorCreator, "
             "const std::string& scalarParameterHelp )" );

}



gkg::FiberDistanceFunctor*
gkg::FiberDistanceFunctorFactory::create(
                           const std::string& name,
                           const std::vector< double >& scalarParameters ) const
{

  try
  {

    typename std::map< std::string,
                       gkg::FiberDistanceFunctorFactory::
                            FiberDistanceFunctorCreator >::const_iterator
      c = _fiberDistanceFunctorCreators.find( name );
    if ( c != _fiberDistanceFunctorCreators.end() )
    {

      return ( *c->second )( scalarParameters );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "gkg::FiberDistanceFunctor* "
             "gkg::FiberDistanceFunctorFactory::create( "
             "const std::string& name, "
             "const std::vector< double >& scalarParameters ) const" );

}


bool gkg::FiberDistanceFunctorFactory::hasFiberDistanceFunctor(
                                                 const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       gkg::FiberDistanceFunctorFactory::
                       FiberDistanceFunctorCreator >::const_iterator
      c = _fiberDistanceFunctorCreators.find( name );
    if ( c != _fiberDistanceFunctorCreators.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "bool gkg::FiberDistanceFunctorFactory::hasFiberDistanceFunctor( "
             "const std::string& name ) const" );

}


std::set< std::string > gkg::FiberDistanceFunctorFactory::getNames() const
{

  try
  {

    std::set< std::string > names;
    typename std::map< std::string,
                       gkg::FiberDistanceFunctorFactory::
                       FiberDistanceFunctorCreator >::const_iterator
      c = _fiberDistanceFunctorCreators.begin(),
      ce = _fiberDistanceFunctorCreators.end();

    while ( c != ce )
    {

      names.insert( c->first );
      ++ c;

    }
    return names;

  }
  GKG_CATCH( "std::set< std::string > "
             "gkg::FiberDistanceFunctorFactory::getNames() const" );

}


std::string gkg::FiberDistanceFunctorFactory::getNameListHelp() const
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
             "gkg::FiberDistanceFunctorFactory::getNameListHelp() const" );

}


std::string
gkg::FiberDistanceFunctorFactory::getScalarParameterHelp(
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
             "gkg::FiberDistanceFunctorFactory::"
             "getScalarParameterHelp( "
             "const std::string& name ) const" );

}

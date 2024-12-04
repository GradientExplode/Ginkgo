#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFactory.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


template < class L, class Compare >
gkg::BundleMapOperatorFactory< L, Compare >::BundleMapOperatorFactory()
{
}


template < class L, class Compare >
gkg::BundleMapOperatorFactory< L, Compare >::~BundleMapOperatorFactory()
{
}


template < class L, class Compare >
bool 
gkg::BundleMapOperatorFactory< L, Compare >::registerBundleMapOperator(
 const std::string& name,
 typename gkg::BundleMapOperatorFactory< L, Compare >::BundleMapOperatorCreator 
                                                       bundleMapOperatorCreator,
 const std::string& stringParameterHelp,
 const std::string& scalarParameterHelp )
{

  try
  {

    if ( bundleMapOperatorCreator )
    {

      typename std::map< std::string,
                         typename gkg::BundleMapOperatorFactory< L,
                       Compare >::BundleMapOperatorCreator >::const_iterator
        c = _bundleMapOperatorCreators.find( name );

      if ( c == _bundleMapOperatorCreators.end() )
      {

        _bundleMapOperatorCreators[ name ] = bundleMapOperatorCreator;
        _stringParameterHelps[ name ] = stringParameterHelp;
        _scalarParameterHelps[ name ] = scalarParameterHelp;
        return true;

      }
      else
      {

        std::cerr << "gkg::BundleMapOperatorFactory< L, Compare >::"
                  << "registerBundleMapOperator: " << name
                  << " already exists"
                  << std::endl;
        return false;

      }

    }
    return false;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "bool gkg::BundleMapOperatorFactory< L, Compare >::"
             "registerBundleMapOperator( "
             "const std::string& name, "
             "typename gkg::BundleMapOperatorFactory< L, Compare >::"
             "BundleMapOperatorCreator" 
             "bundleMapOperatorCreator, "
             "const std::string& stringParameterHelp, "
             "const std::string& scalarParameterHelp )" );

}


template < class L, class Compare >
gkg::BundleMapOperator< L, Compare >* 
gkg::BundleMapOperatorFactory< L, Compare >::create( 
                           const std::string& name,
                           const std::vector< std::string >& stringParameters,
                           const std::vector< double >& scalarParameters,
                           bool verbose ) const
{

  try
  {

    typename std::map< std::string,
                       typename gkg::BundleMapOperatorFactory< L,
                       Compare >::BundleMapOperatorCreator >::const_iterator
      c = _bundleMapOperatorCreators.find( name );
    if ( c != _bundleMapOperatorCreators.end() )
    {

      return ( *c->second )( stringParameters, scalarParameters, verbose );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "gkg::BundleMapOperator< L, Compare >*  "
             "gkg::BundleMapOperatorFactory< L, Compare >::create(  "
             "const std::string& name, "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose ) const" );

}



template < class L, class Compare >
bool gkg::BundleMapOperatorFactory< L, Compare >::hasBundleMapOperator(
                                                 const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       typename gkg::BundleMapOperatorFactory< L,
                       Compare >::BundleMapOperatorCreator >::const_iterator
      c = _bundleMapOperatorCreators.find( name );
    if ( c != _bundleMapOperatorCreators.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "bool gkg::BundleMapOperatorFactory< L, Compare >::"
             "hasBundleMapOperator( "
             "const std::string& name ) const" );

}



template < class L, class Compare >
std::set< std::string >
gkg::BundleMapOperatorFactory< L, Compare >::getNames() const
{

  try
  {

    std::set< std::string > names;
    typename std::map< std::string,
                       typename gkg::BundleMapOperatorFactory< L,
                       Compare >::BundleMapOperatorCreator >::const_iterator
      c = _bundleMapOperatorCreators.begin(),
      ce = _bundleMapOperatorCreators.end();

    while ( c != ce )
    {

      names.insert( c->first );
      ++ c;

    }
    return names;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "std::set< std::string > "
             "gkg::BundleMapOperatorFactory< L, Compare >::getNames() "
             "const" );

}


template < class L, class Compare >
std::string
gkg::BundleMapOperatorFactory< L, Compare >::getNameListHelp() const
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
  GKG_CATCH( "template < class L, class Compare > "
             "std::string "
             "gkg::BundleMapOperatorFactory< L, Compare >::getNameListHelp() "
             "const" );

}


template < class L, class Compare >
std::string
gkg::BundleMapOperatorFactory< L, Compare >::getStringParameterHelp(
                                                 const std::string& name ) const
{

  try
  {

    std::map< std::string, std::string >::const_iterator
      h = _stringParameterHelps.find( name );
    if ( h == _stringParameterHelps.end() )
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }
    return h->second;


  }
  GKG_CATCH( "template < class L, class Compare > "
             "std::string "
             "gkg::BundleMapOperatorFactory< L, Compare >::"
             "getStringParameterHelp( "
             "const std::string& name ) const" );

}


template < class L, class Compare >
std::string
gkg::BundleMapOperatorFactory< L, Compare >::getScalarParameterHelp(
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
  GKG_CATCH( "template < class L, class Compare > "
             "std::string "
             "gkg::BundleMapOperatorFactory< L, Compare >::"
             "getScalarParameterHelp( "
             "const std::string& name ) const" );

}


template class gkg::BundleMapOperatorFactory< std::string >;
template class gkg::BundleMapOperatorFactory< int16_t >;

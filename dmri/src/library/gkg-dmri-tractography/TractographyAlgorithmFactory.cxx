#include <gkg-dmri-tractography/TractographyAlgorithmFactory.h>
#include <gkg-dmri-tractography/TractographyAlgorithm_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


template < class L, class Compare >
gkg::TractographyAlgorithmFactory< L, Compare >::TractographyAlgorithmFactory()
{
}


template < class L, class Compare >
gkg::TractographyAlgorithmFactory< L, Compare >::~TractographyAlgorithmFactory()
{
}


template < class L, class Compare >
bool 
gkg::TractographyAlgorithmFactory< L, Compare >::registerTractographyAlgorithm(
 const std::string& name,
 typename gkg::TractographyAlgorithmFactory< L,
                                       Compare >::TractographyAlgorithmCreator 
                                                  tractographyAlgorithmCreator )
{

  try
  {

    if ( tractographyAlgorithmCreator )
    {

      typename std::map< std::string,
                         typename gkg::TractographyAlgorithmFactory< L,
                       Compare >::TractographyAlgorithmCreator >::const_iterator
        a = _tractographyAlgorithmCreators.find( name );

      if ( a == _tractographyAlgorithmCreators.end() )
      {

        _tractographyAlgorithmCreators[ name ] = tractographyAlgorithmCreator;
        return true;

      }
      else
      {

        _tractographyAlgorithmCreators[ name ] = tractographyAlgorithmCreator;
        std::cerr << "gkg::TractographyAlgorithmFactory< L, Compare >::"
                  << "registerTractographyAlgorithm: " << name
                  << " already exists"
                  << std::endl;
        return false;

      }

    }
    return false;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "bool gkg::TractographyAlgorithmFactory< L, Compare >::"
             "registerTractographyAlgorithm( "
             "const std::string& name, "
             "typename gkg::TractographyAlgorithmFactory< L, Compare >::"
             "TractographyAlgorithmCreator" 
             "tractographyAlgorithmCreator )" );

}


template < class L, class Compare >
gkg::TractographyAlgorithm< L, Compare >* 
gkg::TractographyAlgorithmFactory< L, Compare >::create( 
                      const std::string& name,
                      const gkg::Vector3d< int32_t >& size,
                      const gkg::Vector3d< double >& resolution,
                      const std::vector< double >& scalarParameters,
                      const std::vector< std::string >& stringParameters ) const
{

  try
  {

    typename std::map< std::string,
                       typename gkg::TractographyAlgorithmFactory< L,
                       Compare >::TractographyAlgorithmCreator >::const_iterator
      a = _tractographyAlgorithmCreators.find( name );
    if ( a != _tractographyAlgorithmCreators.end() )
    {

      return ( *a->second )( size,
                             resolution,
                             scalarParameters,
                             stringParameters );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "gkg::TractographyAlgorithm< L, Compare >*  "
             "gkg::TractographyAlgorithmFactory< L, Compare >::create(  "
             "const std::string& name, "
             "const gkg::Vector3d< int32_t >& size, "
             "const gkg::Vector3d< double >& resolution, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters ) const" );

}



template < class L, class Compare >
bool gkg::TractographyAlgorithmFactory< L, Compare >::hasTractographyAlgorithm(
                                                 const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       typename gkg::TractographyAlgorithmFactory< L,
                       Compare >::TractographyAlgorithmCreator >::const_iterator
      a = _tractographyAlgorithmCreators.find( name );
    if ( a != _tractographyAlgorithmCreators.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "bool gkg::TractographyAlgorithmFactory< L, Compare >::"
             "hasTractographyAlgorithm( "
             "const std::string& name ) const" );

}



template < class L, class Compare >
std::list< std::string >
gkg::TractographyAlgorithmFactory< L, Compare >::getNames() const
{

  try
  {

    std::list< std::string > names;
    typename std::map< std::string,
                       typename gkg::TractographyAlgorithmFactory< L,
                       Compare >::TractographyAlgorithmCreator >::const_iterator
      a = _tractographyAlgorithmCreators.begin(),
      ae = _tractographyAlgorithmCreators.end();

    while ( a != ae )
    {

      names.push_back( a->first );
      ++ a;

    }
    return names;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "std::list< std::string > "
             "gkg::TractographyAlgorithmFactory< L, Compare >::getNames() "
             "const" );

}


template class gkg::TractographyAlgorithmFactory< std::string >;
template class gkg::TractographyAlgorithmFactory< int16_t >;
template class gkg::TractographyAlgorithmFactory< int32_t >;

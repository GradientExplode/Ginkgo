#include <gkg-global-tractography-algorithm/ParallelizedAlgorithmFactory.h>
#include <gkg-global-tractography-algorithm/ParallelizedAlgorithm.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::ParallelizedAlgorithmFactory::ParallelizedAlgorithmFactory()
{
}


gkg::ParallelizedAlgorithmFactory::~ParallelizedAlgorithmFactory()
{
}


bool gkg::ParallelizedAlgorithmFactory::registerParallelizedAlgorithm(
 const std::string& name,
 typename gkg::ParallelizedAlgorithmFactory::ParallelizedAlgorithmCreator 
                                                  ParallelizedAlgorithmCreator )
{

  try
  {

    if ( ParallelizedAlgorithmCreator )
    {

      typename std::map< std::string,
                         typename gkg::ParallelizedAlgorithmFactory::ParallelizedAlgorithmCreator >::const_iterator
        a = _ParallelizedAlgorithmCreators.find( name );

      if ( a == _ParallelizedAlgorithmCreators.end() )
      {

        _ParallelizedAlgorithmCreators[ name ] = ParallelizedAlgorithmCreator;
        return true;

      }
      else
      {

        _ParallelizedAlgorithmCreators[ name ] = ParallelizedAlgorithmCreator;
        std::cerr << "gkg::ParallelizedAlgorithmFactory::"
                  << "registerParallelizedAlgorithm: " << name
                  << " already exists"
                  << std::endl;
        return false;

      }

    }
    return false;

  }
  GKG_CATCH( " "
             "bool gkg::ParallelizedAlgorithmFactory::"
             "registerParallelizedAlgorithm( "
             "const std::string& name, "
             "typename gkg::ParallelizedAlgorithmFactory::"
             "ParallelizedAlgorithmCreator" 
             "ParallelizedAlgorithmCreator )" );

}



gkg::ParallelizedAlgorithm* 
gkg::ParallelizedAlgorithmFactory::create( 
                      const std::string& name, const std::string& str ) const
{

  try
  {

    typename std::map< std::string,
                       typename gkg::ParallelizedAlgorithmFactory::ParallelizedAlgorithmCreator >::const_iterator
      a = _ParallelizedAlgorithmCreators.find( name );
    if ( a != _ParallelizedAlgorithmCreators.end() )
    {

      return ( *a->second )( str );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( " "
             "gkg::ParallelizedAlgorithm*  "
             "gkg::ParallelizedAlgorithmFactory::create(  "
             "const std::string& name, "
             "const gkg::Vector3d< int32_t >& size, "
             "const gkg::Vector3d< double >& resolution, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters ) const" );

}




bool gkg::ParallelizedAlgorithmFactory::hasParallelizedAlgorithm(
                                                 const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       typename gkg::ParallelizedAlgorithmFactory::ParallelizedAlgorithmCreator >::const_iterator
      a = _ParallelizedAlgorithmCreators.find( name );
    if ( a != _ParallelizedAlgorithmCreators.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( " "
             "bool gkg::ParallelizedAlgorithmFactory::"
             "hasParallelizedAlgorithm( "
             "const std::string& name ) const" );

}




std::list< std::string >
gkg::ParallelizedAlgorithmFactory::getNames() const
{

  try
  {

    std::list< std::string > names;
    typename std::map< std::string,
                       typename gkg::ParallelizedAlgorithmFactory::ParallelizedAlgorithmCreator >::const_iterator
      a = _ParallelizedAlgorithmCreators.begin(),
      ae = _ParallelizedAlgorithmCreators.end();

    while ( a != ae )
    {

      names.push_back( a->first );
      ++ a;

    }
    return names;

  }
  GKG_CATCH( " "
             "std::list< std::string > "
             "gkg::ParallelizedAlgorithmFactory::getNames() "
             "const" );

}

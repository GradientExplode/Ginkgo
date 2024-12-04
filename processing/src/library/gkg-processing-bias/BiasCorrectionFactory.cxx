#include <gkg-processing-bias/BiasCorrectionFactory.h>
#include <gkg-processing-bias/BiasCorrection_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


template < class T >
gkg::BiasCorrectionFactory< T >::BiasCorrectionFactory()
{
}


template < class T >
gkg::BiasCorrectionFactory< T >::~BiasCorrectionFactory()
{
}


template < class T >
bool gkg::BiasCorrectionFactory< T >::registerFactory(
  const std::string& name,
  typename gkg::BiasCorrectionFactory< T >::BiasCorrectionCreator
                                                         biasCorrectionCreator )
{

  try
  {

    if ( biasCorrectionCreator )
    {

      typename
      std::map< std::string,
        typename gkg::BiasCorrectionFactory< T >::
                                         BiasCorrectionCreator >::const_iterator
        b = _biasCorrectionCreators.find( name );

      if ( b == _biasCorrectionCreators.end() )
      {

        _biasCorrectionCreators[ name ] = biasCorrectionCreator;
        return true;

      }

#ifdef GKG_DEBUG

      else
      {

        std::cerr << "gkg::BiasCorrectionFactory< T >::registerFactory: "
                  << name
                  << " already exists"
                  << std::endl;
        return false;

      }

#endif

    }
    return false;

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::BiasCorrectionFactory< T >::registerFactory( "
             "const std::string& name, "
             "typename gkg::BiasCorrectionFactory< T >::BiasCorrectionCreator "
             "biasCorrectionCreator )" );

}


template < class T >
gkg::BiasCorrection< T >* 
gkg::BiasCorrectionFactory< T >::createBiasCorrection(
                      const std::string& name,
                      const std::vector< double >& scalarParameters,
                      const std::vector< std::string >& stringParameters ) const
{

  try
  {

    typename
    std::map< std::string,
      typename gkg::BiasCorrectionFactory< T >::
                                         BiasCorrectionCreator >::const_iterator
      b = _biasCorrectionCreators.find( name );
    if ( b != _biasCorrectionCreators.end() )
    {

      return ( *b->second )( scalarParameters, stringParameters );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::BiasCorrection< T >* "
             "gkg::BiasCorrectionFactory< T >::createBiasCorrection( "
             "const std::string& name, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters ) const" );

}


template < class T >
bool
gkg::BiasCorrectionFactory< T >::hasBiasCorrection(
                                                 const std::string& name ) const
{

  try
  {

    typename
      std::map< std::string,
                typename gkg::BiasCorrectionFactory< T >::
                                         BiasCorrectionCreator >::const_iterator
      b = _biasCorrectionCreators.find( name );
    if ( b != _biasCorrectionCreators.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "template < class T > "
             "bool "
             "gkg::BiasCorrectionFactory< T >::hasBiasCorrection( "
             "const std::string& name ) const" );

}


template < class T >
std::list< std::string >
gkg::BiasCorrectionFactory< T >::getNames() const
{

  try
  {

    std::list< std::string > names;
    typename
    std::map< std::string,
              typename gkg::BiasCorrectionFactory< T >::
                                         BiasCorrectionCreator >::const_iterator
      b = _biasCorrectionCreators.begin(),
      be = _biasCorrectionCreators.end();

    while ( b != be )
    {

      names.push_back( b->first );
      ++ b;

    }
    return names;

  }
  GKG_CATCH( "template < class T > "
             "std::list< std::string > "
             "gkg::BiasCorrectionFactory< T >::getNames() const" );

}


template class gkg::BiasCorrectionFactory< uint8_t >;
template class gkg::BiasCorrectionFactory< int8_t >;
template class gkg::BiasCorrectionFactory< uint16_t >;
template class gkg::BiasCorrectionFactory< int16_t >;
template class gkg::BiasCorrectionFactory< uint32_t >;
template class gkg::BiasCorrectionFactory< int32_t >;
template class gkg::BiasCorrectionFactory< uint64_t >;
template class gkg::BiasCorrectionFactory< int64_t >;
template class gkg::BiasCorrectionFactory< float >;
template class gkg::BiasCorrectionFactory< double >;

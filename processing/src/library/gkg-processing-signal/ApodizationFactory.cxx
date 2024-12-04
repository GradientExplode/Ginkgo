#include <gkg-processing-signal/ApodizationFactory.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


template < class T >
gkg::ApodizationFactory< T >::ApodizationFactory()
{
}


template < class T >
gkg::ApodizationFactory< T >::~ApodizationFactory()
{
}


template < class T >
bool gkg::ApodizationFactory< T >::registerFactory(
  const std::string& name,
  typename gkg::ApodizationFactory< T >::ApodizationCreator apodizationCreator )
{

  if ( apodizationCreator )
  {

    typename std::map< std::string,
     typename gkg::ApodizationFactory< T >::ApodizationCreator >::const_iterator
      a = _apodizationCreators.find( name );

    if ( a == _apodizationCreators.end() )
    {

      _apodizationCreators[ name ] = apodizationCreator;
      return true;

    }
#ifdef GKG_DEBUG

    else
    {

      std::cerr << "gkg::ApodizationFactory< T >::registerFactory: " << name
                << " already exists"
                << std::endl;
      return false;

    }

#endif

  }
  return false;

}


template < class T >
gkg::Apodization< T >* 
gkg::ApodizationFactory< T >::createApodization(
                               const std::string& name,
                               int32_t size,
                               typename gkg::Apodization< T >::Type type ) const
{

  try
  {

    typename std::map< std::string,
     typename gkg::ApodizationFactory< T >::ApodizationCreator >::const_iterator
      a = _apodizationCreators.find( name );
    if ( a != _apodizationCreators.end() )
    {

      return ( *a->second )( size, type );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::Apodization< T >* "
             "gkg::ApodizationFactory< T >::createApodization( "
             "const std::string& name, "
             "int size, "
             "typename gkg::Apodization< T >::Type type ) const" );

}



template < class T >
bool
gkg::ApodizationFactory< T >::hasApodization( const std::string& name ) const
{

  typename std::map< std::string,
     typename gkg::ApodizationFactory< T >::ApodizationCreator >::const_iterator
    a = _apodizationCreators.find( name );
  if ( a != _apodizationCreators.end() )
  {

    return true;

  }
  return false;

}



template < class T >
std::list< std::string >
gkg::ApodizationFactory< T >::getNames() const
{

  std::list< std::string > names;
  typename std::map< std::string,
     typename gkg::ApodizationFactory< T >::ApodizationCreator >::const_iterator
    a = _apodizationCreators.begin(), ae = _apodizationCreators.end();

  while ( a != ae )
  {

    names.push_back( a->first );
    ++ a;

  }
  return names;

}

template class gkg::ApodizationFactory< float >;
template class gkg::ApodizationFactory< double >;

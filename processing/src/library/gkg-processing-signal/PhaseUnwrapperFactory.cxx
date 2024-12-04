#include <gkg-processing-signal/PhaseUnwrapperFactory.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


template < class T >
gkg::PhaseUnwrapperFactory< T >::PhaseUnwrapperFactory()
{
}


template < class T >
gkg::PhaseUnwrapperFactory< T >::~PhaseUnwrapperFactory()
{
}


template < class T >
bool gkg::PhaseUnwrapperFactory< T >::registerFactory(
               const std::string& name,
               typename gkg::PhaseUnwrapperFactory< T >::PhaseUnwrapperCreator 
                                                         phaseUnwrapperCreator )
{

  if ( phaseUnwrapperCreator )
  {

    typename std::map< std::string,
        typename 
        gkg::PhaseUnwrapperFactory< T >::PhaseUnwrapperCreator >::const_iterator
      a = _phaseUnwrapperCreators.find( name );

    if ( a == _phaseUnwrapperCreators.end() )
    {

      _phaseUnwrapperCreators[ name ] = phaseUnwrapperCreator;
      return true;

    }

#ifdef GKG_DEBUG

    else
    {

      std::cerr << "gkg::PhaseUnwrapperFactory< T >::registerFactory: " << name
                << " already exists"
                << std::endl;
      return false;

    }

#endif

  }
  return false;

}


template < class T >
gkg::PhaseUnwrapper< T >* 
gkg::PhaseUnwrapperFactory< T >::createPhaseUnwrapper( 
                                                 const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
        typename 
        gkg::PhaseUnwrapperFactory< T >::PhaseUnwrapperCreator >::const_iterator
      a = _phaseUnwrapperCreators.find( name );
    if ( a != _phaseUnwrapperCreators.end() )
    {

      return ( *a->second )();

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::PhaseUnwrapper< T >* "
             "gkg::PhaseUnwrapperFactory< T >::createPhaseUnwrapper( "
             "const std::string& name ) const" );

}



template < class T >
bool gkg::PhaseUnwrapperFactory< T >::hasPhaseUnwrapper(
                                                 const std::string& name ) const
{

  typename std::map< std::string,
        typename 
        gkg::PhaseUnwrapperFactory< T >::PhaseUnwrapperCreator >::const_iterator
    a = _phaseUnwrapperCreators.find( name );
  if ( a != _phaseUnwrapperCreators.end() )
  {

    return true;

  }
  return false;

}



template < class T >
std::list< std::string >
gkg::PhaseUnwrapperFactory< T >::getNames() const
{

  std::list< std::string > names;
  typename std::map< std::string,
        typename 
        gkg::PhaseUnwrapperFactory< T >::PhaseUnwrapperCreator >::const_iterator
    a = _phaseUnwrapperCreators.begin(), ae = _phaseUnwrapperCreators.end();

  while ( a != ae )
  {

    names.push_back( a->first );
    ++ a;

  }
  return names;

}


template class gkg::PhaseUnwrapperFactory< float >;
template class gkg::PhaseUnwrapperFactory< double >;

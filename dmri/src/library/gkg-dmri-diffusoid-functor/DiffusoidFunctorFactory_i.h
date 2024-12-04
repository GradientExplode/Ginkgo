#ifndef _gkg_dmri_diffusoid_functor_DiffusoidFunctorFactory_i_h_
#define _gkg_dmri_diffusoid_functor_DiffusoidFunctorFactory_i_h_


#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorFactory.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor_i.h>
#include <gkg-core-exception/Exception.h>


template < class C, class I>
inline
gkg::DiffusoidFunctorFactory< C, I >::DiffusoidFunctorFactory()
{
}


template < class C, class I>
inline
gkg::DiffusoidFunctorFactory< C, I >::~DiffusoidFunctorFactory()
{
}


template < class C, class I>
inline
bool gkg::DiffusoidFunctorFactory< C, I >::registerDiffusoidFunctor(
                        const gkg::DiffusoidFunctor< C, I >& diffusoidFunctor )
{

  try
  {

    typename std::map< std::string,
                       const gkg::DiffusoidFunctor< C, I >* >::const_iterator
      f = _diffusoidFunctors.find( diffusoidFunctor.getName() );

    if ( f == _diffusoidFunctors.end() )
    {

      _diffusoidFunctors[ diffusoidFunctor.getName() ] = &diffusoidFunctor;
      return true;

    }
    else
    {

      std::cerr << "gkg::DiffusoidFunctorFactory< C, I >::"
                << "registerDiffusoidFunctor: " << diffusoidFunctor.getName()
                << " already exists"
                << std::endl;
      return false;

    }
    return false;

  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "bool "
             "gkg::DiffusoidFunctorFactory< C, I >::registerDiffusoidFunctor( "
             "const gkg::DiffusoidFunctor< C, I >& diffusoidFunctor )" );

}


template < class C, class I>
inline
const gkg::DiffusoidFunctor< C, I >&
gkg::DiffusoidFunctorFactory< C, I >::getDiffusoidFunctor(
                                                const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       const gkg::DiffusoidFunctor< C, I >* >::const_iterator
      f = _diffusoidFunctors.find( name );
    if ( f != _diffusoidFunctors.end() )
    {

      return *( f->second );

    }
    else
    {

      throw std::runtime_error( std::string( "'"  ) + name + "' not found" );

    }



  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "const gkg::DiffusoidFunctor< C, I >& "
             "gkg::DiffusoidFunctorFactory< C, I >::getDiffusoidFunctor( "
             "const std::string& name ) const" );


}


template < class C, class I>
inline
bool
gkg::DiffusoidFunctorFactory< C, I >::hasDiffusoidFunctor(
                                                const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       const gkg::DiffusoidFunctor< C, I >* >::const_iterator
      f = _diffusoidFunctors.find( name );
    if ( f != _diffusoidFunctors.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "bool "
             "gkg::DiffusoidFunctorFactory< C, I >::hasDiffusoidFunctor( "
             "const std::string& name ) const" );

}


template < class C, class I>
inline
std::list< std::string >
gkg::DiffusoidFunctorFactory< C, I >::getNames() const
{

  try
  {

    std::list< std::string > names;
    typename std::map< std::string,
                       const gkg::DiffusoidFunctor< C, I >* >::const_iterator
      f = _diffusoidFunctors.begin(),
      fe = _diffusoidFunctors.end();

    while ( f != fe )
    {

      names.push_back( f->first );
      ++ f;

    }
    return names;

  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "std::list< std::string > "
             "gkg::DiffusoidFunctorFactory< C, I >::getNames() const" );

}


#endif

#ifndef _gkg_dmri_pdf_functor_EapFunctorFactory_i_h_
#define _gkg_dmri_pdf_functor_EapFunctorFactory_i_h_


#include <gkg-dmri-pdf-functor/EapFunctorFactory.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-core-exception/Exception.h>


template < class C, class I>
inline
gkg::EapFunctorFactory< C, I >::EapFunctorFactory()
{
}


template < class C, class I>
inline
gkg::EapFunctorFactory< C, I >::~EapFunctorFactory()
{
}


template < class C, class I>
inline
bool gkg::EapFunctorFactory< C, I >::registerEapFunctor(
                                    const gkg::EapFunctor< C, I >& eapFunctor )
{

  try
  {

    typename std::map< std::string,
                       const gkg::EapFunctor< C, I >* >::const_iterator
      f = _eapFunctors.find( eapFunctor.getName() );

    if ( f == _eapFunctors.end() )
    {

      _eapFunctors[ eapFunctor.getName() ] = &eapFunctor;
      return true;

    }
    else
    {

      std::cerr << "gkg::EapFunctorFactory< C, I >::"
                << "registerEapFunctor: " << eapFunctor.getName()
                << " already exists"
                << std::endl;
      return false;

    }
    return false;

  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "bool gkg::EapFunctorFactory< C, I >::registerEapFunctor( "
             "const gkg::EapFunctor< C, I >& eapFunctor )" );

}


template < class C, class I>
inline
const gkg::EapFunctor< C, I >&
gkg::EapFunctorFactory< C, I >::getEapFunctor( const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       const gkg::EapFunctor< C, I >* >::const_iterator
      f = _eapFunctors.find( name );
    if ( f != _eapFunctors.end() )
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
             "const gkg::EapFunctor< C, I >& "
             "gkg::EapFunctorFactory< C, I >::getEapFunctor( "
             "const std::string& name ) const" );

}


template < class C, class I>
inline
bool
gkg::EapFunctorFactory< C, I >::hasEapFunctor( const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       const gkg::EapFunctor< C, I >* >::const_iterator
      f = _eapFunctors.find( name );

    if ( f != _eapFunctors.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "bool "
             "gkg::EapFunctorFactory< C, I >::hasEapFunctor( "
             "const std::string& name ) const" );

}


template < class C, class I>
inline
std::list< std::string >
gkg::EapFunctorFactory< C, I >::getNames() const
{

  try
  {

    std::list< std::string > names;
    typename std::map< std::string,
                       const gkg::EapFunctor< C, I >* >::const_iterator
      f = _eapFunctors.begin(),
      fe = _eapFunctors.end();

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
             "gkg::EapFunctorFactory< C, I >::getNames() const" );

}


#endif

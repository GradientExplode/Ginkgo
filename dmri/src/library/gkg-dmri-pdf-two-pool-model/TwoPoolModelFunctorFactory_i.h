#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorFactory_i_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorFactory_i_h_


#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFactory.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor_i.h>
#include <gkg-core-exception/Exception.h>


template < class C, class I>
inline
gkg::TwoPoolModelFunctorFactory< C, I >::TwoPoolModelFunctorFactory()
{
}


template < class C, class I>
inline
gkg::TwoPoolModelFunctorFactory< C, I >::~TwoPoolModelFunctorFactory()
{
}


template < class C, class I>
inline
bool gkg::TwoPoolModelFunctorFactory< C, I >::registerTwoPoolModelFunctor(
                   const gkg::TwoPoolModelFunctor< C, I >& twoPoolModelFunctor )
{

  try
  {

    typename std::map< std::string,
                       const gkg::TwoPoolModelFunctor< C, I >* >::const_iterator
      f = _twoPoolModelFunctors.find( twoPoolModelFunctor.getName() );

    if ( f == _twoPoolModelFunctors.end() )
    {

      _twoPoolModelFunctors[ twoPoolModelFunctor.getName() ] =
                                                           &twoPoolModelFunctor;
      return true;

    }
    else
    {

      std::cerr << "gkg::TwoPoolModelFunctorFactory< C, I >::"
                << "registerTwoPoolModelFunctor: "
                << twoPoolModelFunctor.getName()
                << " already exists"
                << std::endl;
      return false;

    }
    return false;

  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "bool "
             "gkg::TwoPoolModelFunctorFactory< C, I >::"
             "registerTwoPoolModelFunctor( "
             "const gkg::TwoPoolModelFunctor< C, I >& twoPoolModelFunctor )" );

}


template < class C, class I>
inline
const gkg::TwoPoolModelFunctor< C, I >&
gkg::TwoPoolModelFunctorFactory< C, I >::getTwoPoolModelFunctor(
                                                 const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       const gkg::TwoPoolModelFunctor< C, I >* >::const_iterator
      f = _twoPoolModelFunctors.find( name );
    if ( f != _twoPoolModelFunctors.end() )
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
             "const gkg::TwoPoolModelFunctor< C, I >& "
             "gkg::TwoPoolModelFunctorFactory< C, I >::getTwoPoolModelFunctor( "
             "const std::string& name ) const" );


}


template < class C, class I>
inline
bool
gkg::TwoPoolModelFunctorFactory< C, I >::hasTwoPoolModelFunctor(
                                                 const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       const gkg::TwoPoolModelFunctor< C, I >* >::const_iterator
      f = _twoPoolModelFunctors.find( name );
    if ( f != _twoPoolModelFunctors.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "bool "
             "gkg::TwoPoolModelFunctorFactory< C, I >::hasTwoPoolModelFunctor( "
             "const std::string& name ) const" );

}


template < class C, class I>
inline
std::list< std::string >
gkg::TwoPoolModelFunctorFactory< C, I >::getNames() const
{

  try
  {

    std::list< std::string > names;
    typename std::map< std::string,
                       const gkg::TwoPoolModelFunctor< C, I >* >::const_iterator
      f = _twoPoolModelFunctors.begin(),
      fe = _twoPoolModelFunctors.end();

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
             "gkg::TwoPoolModelFunctorFactory< C, I >::getNames() const" );

}


#endif

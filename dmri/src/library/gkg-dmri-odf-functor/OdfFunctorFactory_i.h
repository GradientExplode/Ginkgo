#ifndef _gkg_dmri_odf_functor_OdfFunctorFactory_i_h_
#define _gkg_dmri_odf_functor_OdfFunctorFactory_i_h_


#include <gkg-dmri-odf-functor/OdfFunctorFactory.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-core-exception/Exception.h>


template < class C, class I>
inline
gkg::OdfFunctorFactory< C, I >::OdfFunctorFactory()
{
}


template < class C, class I>
inline
gkg::OdfFunctorFactory< C, I >::~OdfFunctorFactory()
{
}


template < class C, class I>
inline
bool gkg::OdfFunctorFactory< C, I >::registerOdfFunctor(
                                    const gkg::OdfFunctor< C, I >& odfFunctor )
{

  try
  {

    typename std::map< std::string,
                       const gkg::OdfFunctor< C, I >* >::const_iterator
      f = _odfFunctors.find( odfFunctor.getName() );

    if ( f == _odfFunctors.end() )
    {

      _odfFunctors[ odfFunctor.getName() ] = &odfFunctor;
      return true;

    }
    else
    {

      std::cerr << "gkg::OdfFunctorFactory< C, I >::"
                << "registerOdfFunctor: " << odfFunctor.getName()
                << " already exists"
                << std::endl;
      return false;

    }
    return false;

  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "bool gkg::OdfFunctorFactory< C, I >::registerOdfFunctor( "
             "const gkg::OdfFunctor< C, I >& odfFunctor )" );

}


template < class C, class I>
inline
const gkg::OdfFunctor< C, I >&
gkg::OdfFunctorFactory< C, I >::getOdfFunctor( const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       const gkg::OdfFunctor< C, I >* >::const_iterator
      f = _odfFunctors.find( name );
    if ( f != _odfFunctors.end() )
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
             "const gkg::OdfFunctor< C, I >& "
             "gkg::OdfFunctorFactory< C, I >::getOdfFunctor( "
             "const std::string& name ) const" );

}


template < class C, class I>
inline
bool
gkg::OdfFunctorFactory< C, I >::hasOdfFunctor( const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       const gkg::OdfFunctor< C, I >* >::const_iterator
      f = _odfFunctors.find( name );
    if ( f != _odfFunctors.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "bool "
             "gkg::OdfFunctorFactory< C, I >::hasOdfFunctor( "
             "const std::string& name ) const" );

}


template < class C, class I>
inline
std::list< std::string >
gkg::OdfFunctorFactory< C, I >::getNames() const
{

  try
  {

    std::list< std::string > names;
    typename std::map< std::string,
                       const gkg::OdfFunctor< C, I >* >::const_iterator
      f = _odfFunctors.begin(),
      fe = _odfFunctors.end();

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
             "gkg::OdfFunctorFactory< C, I >::getNames() const" );

}


#endif

#ifndef _gkg_dmri_pdf_dti_TensorFunctorFactory_i_h_
#define _gkg_dmri_pdf_dti_TensorFunctorFactory_i_h_


#include <gkg-dmri-pdf-dti/TensorFunctorFactory.h>
#include <gkg-dmri-pdf-dti/TensorFunctor_i.h>
#include <gkg-core-exception/Exception.h>


template < class C, class I>
inline
gkg::TensorFunctorFactory< C, I >::TensorFunctorFactory()
{
}


template < class C, class I>
inline
gkg::TensorFunctorFactory< C, I >::~TensorFunctorFactory()
{
}


template < class C, class I>
inline
bool gkg::TensorFunctorFactory< C, I >::registerTensorFunctor(
                              const gkg::TensorFunctor< C, I >& tensorFunctor )
{

  try
  {

    typename std::map< std::string,
                       const gkg::TensorFunctor< C, I >* >::const_iterator
      f = _tensorFunctors.find( tensorFunctor.getName() );

    if ( f == _tensorFunctors.end() )
    {

      _tensorFunctors[ tensorFunctor.getName() ] = &tensorFunctor;
      return true;

    }
    else
    {

      std::cerr << "gkg::TensorFunctorFactory< C, I >::"
                << "registerTensorFunctor: " << tensorFunctor.getName()
                << " already exists"
                << std::endl;
      return false;

    }
    return false;

  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "bool gkg::TensorFunctorFactory< C, I >::registerTensorFunctor( "
             "const gkg::TensorFunctor< C, I >& tensorFunctor )" );

}


template < class C, class I>
inline
const gkg::TensorFunctor< C, I >&
gkg::TensorFunctorFactory< C, I >::getTensorFunctor( const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       const gkg::TensorFunctor< C, I >* >::const_iterator
      f = _tensorFunctors.find( name );
    if ( f != _tensorFunctors.end() )
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
             "const gkg::TensorFunctor< C, I >& "
             "gkg::TensorFunctorFactory< C, I >::getTensorFunctor( "
             "const std::string& name ) const" );


}


template < class C, class I>
inline
bool
gkg::TensorFunctorFactory< C, I >::hasTensorFunctor( const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       const gkg::TensorFunctor< C, I >* >::const_iterator
      f = _tensorFunctors.find( name );
    if ( f != _tensorFunctors.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "bool "
             "gkg::TensorFunctorFactory< C, I >::hasTensorFunctor( "
             "const std::string& name ) const" );

}


template < class C, class I>
inline
std::list< std::string >
gkg::TensorFunctorFactory< C, I >::getNames() const
{

  try
  {

    std::list< std::string > names;
    typename std::map< std::string,
                       const gkg::TensorFunctor< C, I >* >::const_iterator
      f = _tensorFunctors.begin(),
      fe = _tensorFunctors.end();

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
             "gkg::TensorFunctorFactory< C, I >::getNames() const" );

}


#endif

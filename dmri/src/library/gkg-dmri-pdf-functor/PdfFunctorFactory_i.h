#ifndef _gkg_dmri_pdf_functor_PdfFunctorFactory_i_h_
#define _gkg_dmri_pdf_functor_PdfFunctorFactory_i_h_


#include <gkg-dmri-pdf-functor/PdfFunctorFactory.h>
#include <gkg-dmri-pdf-functor/PdfFunctor_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-core-exception/Exception.h>


template < class C, class I>
inline
gkg::PdfFunctorFactory< C, I >::PdfFunctorFactory()
{
}


template < class C, class I>
inline
gkg::PdfFunctorFactory< C, I >::~PdfFunctorFactory()
{
}


template < class C, class I>
inline
bool gkg::PdfFunctorFactory< C, I >::registerPdfFunctor(
                                    const gkg::PdfFunctor< C, I >& pdfFunctor )
{

  try
  {

    typename std::map< std::string,
                       const gkg::PdfFunctor< C, I >* >::const_iterator
      f = _pdfFunctors.find( pdfFunctor.getName() );

    if ( f == _pdfFunctors.end() )
    {

      _pdfFunctors[ pdfFunctor.getName() ] = &pdfFunctor;
      return true;

    }
    else
    {

      std::cerr << "gkg::PdfFunctorFactory< C, I >::"
                << "registerPdfFunctor: " << pdfFunctor.getName()
                << " already exists"
                << std::endl;
      return false;

    }
    return false;

  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "bool gkg::PdfFunctorFactory< C, I >::registerPdfFunctor( "
             "const gkg::PdfFunctor< C, I >& pdfFunctor )" );

}


template < class C, class I>
inline
const gkg::PdfFunctor< C, I >&
gkg::PdfFunctorFactory< C, I >::getPdfFunctor( const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       const gkg::PdfFunctor< C, I >* >::const_iterator
      f = _pdfFunctors.find( name );
    if ( f != _pdfFunctors.end() )
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
             "const gkg::PdfFunctor< C, I >& "
             "gkg::PdfFunctorFactory< C, I >::getPdfFunctor( "
             "const std::string& name ) const" );

}


template < class C, class I>
inline
bool
gkg::PdfFunctorFactory< C, I >::hasPdfFunctor( const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       const gkg::PdfFunctor< C, I >* >::const_iterator
      f = _pdfFunctors.find( name );
    if ( f != _pdfFunctors.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "template < class C, class I> "
             "inline "
             "bool "
             "gkg::PdfFunctorFactory< C, I >::hasPdfFunctor( "
             "const std::string& name ) const" );

}


template < class C, class I>
inline
std::list< std::string >
gkg::PdfFunctorFactory< C, I >::getNames() const
{

  try
  {

    std::list< std::string > names;
    typename std::map< std::string,
                       const gkg::PdfFunctor< C, I >* >::const_iterator
      f = _pdfFunctors.begin(),
      fe = _pdfFunctors.end();

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
             "gkg::PdfFunctorFactory< C, I >::getNames() const" );

}


#endif

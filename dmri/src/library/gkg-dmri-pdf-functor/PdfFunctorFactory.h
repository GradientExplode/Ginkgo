#ifndef _gkg_dmri_pdf_functor_PdfFunctorFactory_h_
#define _gkg_dmri_pdf_functor_PdfFunctorFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-pdf-functor/PdfFunctor.h>
#include <string>
#include <map>
#include <list>


namespace gkg
{


template < class C, class I >
class PdfFunctorFactory : public Singleton< PdfFunctorFactory< C, I > >
{

  public:

    ~PdfFunctorFactory();

    bool registerPdfFunctor( const PdfFunctor< C, I >& pdfFunctor );

    const PdfFunctor< C, I >& getPdfFunctor( const std::string& name ) const;

    bool hasPdfFunctor( const std::string& name ) const;
    std::list< std::string > getNames() const;

  protected:

    friend class Singleton< PdfFunctorFactory< C, I > >;

    PdfFunctorFactory();

    std::map< std::string, const PdfFunctor< C, I >* > _pdfFunctors;

};


}


#endif

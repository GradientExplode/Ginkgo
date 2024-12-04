#ifndef _gkg_dmri_pdf_functor_PdfFunctorDecomposition_h_
#define _gkg_dmri_pdf_functor_PdfFunctorDecomposition_h_


#include <gkg-dmri-pdf-functor/PdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class PdfFunctorDecomposition :
                           public PdfFunctor< Volume< float >, float >,
                           public Singleton< PdfFunctorDecomposition >
{

  public:

    ~PdfFunctorDecomposition();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( Volume< float >& output,
                  PdfCartesianField& pdfCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< PdfFunctorDecomposition >;

    PdfFunctorDecomposition();

};


}


#endif

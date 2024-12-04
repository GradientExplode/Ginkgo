#ifndef _gkg_dmri_pdf_functor_PdfFunctorNonGaussianity_h_
#define _gkg_dmri_pdf_functor_PdfFunctorNonGaussianity_h_


#include <gkg-dmri-pdf-functor/PdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class PdfFunctorNonGaussianity : public PdfFunctor< Volume< float >, float >,
                                 public Singleton< PdfFunctorNonGaussianity >
{

  public:

    ~PdfFunctorNonGaussianity();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< PdfFunctorNonGaussianity >;

    PdfFunctorNonGaussianity();

    void processItem(
                 float& item,
                 const ProbabilityDensityFunction& pdf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

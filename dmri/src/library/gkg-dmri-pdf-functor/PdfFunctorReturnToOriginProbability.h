#ifndef _gkg_dmri_pdf_functor_PdfFunctorReturnToOriginProbability_h_
#define _gkg_dmri_pdf_functor_PdfFunctorReturnToOriginProbability_h_


#include <gkg-dmri-pdf-functor/PdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class PdfFunctorReturnToOriginProbability :
                      public PdfFunctor< Volume< float >, float >,
                      public Singleton< PdfFunctorReturnToOriginProbability >
{

  public:

    ~PdfFunctorReturnToOriginProbability();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< PdfFunctorReturnToOriginProbability >;

    PdfFunctorReturnToOriginProbability();

    void processItem(
                 float& item,
                 const ProbabilityDensityFunction& pdf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

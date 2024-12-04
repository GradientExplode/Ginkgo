#ifndef _gkg_dmri_pdf_functor_PdfFunctorReturnToPlaneProbability_h_
#define _gkg_dmri_pdf_functor_PdfFunctorReturnToPlaneProbability_h_


#include <gkg-dmri-pdf-functor/PdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class PdfFunctorReturnToPlaneProbability :
                      public PdfFunctor< Volume< float >, float >,
                      public Singleton< PdfFunctorReturnToPlaneProbability >
{

  public:

    ~PdfFunctorReturnToPlaneProbability();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< PdfFunctorReturnToPlaneProbability >;

    PdfFunctorReturnToPlaneProbability();

    void processItem(
                 float& item,
                 const ProbabilityDensityFunction& pdf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

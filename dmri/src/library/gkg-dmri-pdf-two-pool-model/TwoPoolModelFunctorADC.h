#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorADC_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorADC_h_


#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class TwoPoolModelFunctorADC :
            public TwoPoolModelFunctor< Volume< float >, std::vector< float > >,
            public Singleton< TwoPoolModelFunctorADC >
{

  public:

    ~TwoPoolModelFunctorADC();

    std::string getName() const;

  protected:

    friend class Singleton< TwoPoolModelFunctorADC >;

    TwoPoolModelFunctorADC();

    void processItem(
                     std::vector< float >& item,
                     const TwoPoolModel& twoPoolModel,
                     const std::vector< double >& scalarParameters,
                     const std::vector< std::string >& stringParameters ) const;

};


}


#endif

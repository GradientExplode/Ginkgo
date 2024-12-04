#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorSlowRGB_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorSlowRGB_h_


#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class TwoPoolModelFunctorSlowRGB : 
                             public TwoPoolModelFunctor< Volume< RGBComponent >,
                                                                 RGBComponent >,
                             public Singleton< TwoPoolModelFunctorSlowRGB >
{

  public:

    ~TwoPoolModelFunctorSlowRGB();

    std::string getName() const;

  protected:

    friend class Singleton< TwoPoolModelFunctorSlowRGB >;

    TwoPoolModelFunctorSlowRGB();

    void processItem(
                 RGBComponent& item,
                 const TwoPoolModel& twoPoolModel,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

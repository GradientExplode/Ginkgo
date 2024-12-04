#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorFastRGB_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorFastRGB_h_


#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class TwoPoolModelFunctorFastRGB : 
                             public TwoPoolModelFunctor< Volume< RGBComponent >,
                                                                 RGBComponent >,
                             public Singleton< TwoPoolModelFunctorFastRGB >
{

  public:

    ~TwoPoolModelFunctorFastRGB();

    std::string getName() const;

  protected:

    friend class Singleton< TwoPoolModelFunctorFastRGB >;

    TwoPoolModelFunctorFastRGB();

    void processItem(
                 RGBComponent& item,
                 const TwoPoolModel& twoPoolModel,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

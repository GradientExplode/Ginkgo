#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorVolumeFractions_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorVolumeFractions_h_


#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class TwoPoolModelFunctorVolumeFractions :
                            public TwoPoolModelFunctor< Volume< float >,
                                                        std::vector< float > >,
                            public Singleton< TwoPoolModelFunctorVolumeFractions >
{

  public:

    ~TwoPoolModelFunctorVolumeFractions();

    std::string getName() const;


  protected:

    friend class Singleton< TwoPoolModelFunctorVolumeFractions >;

    TwoPoolModelFunctorVolumeFractions();

    void processItem(
                     std::vector< float >& item,
                     const TwoPoolModel& twoPoolModel,
                     const std::vector< double >& scalarParameters,
                     const std::vector< std::string >& stringParameters ) const;

};


}


#endif

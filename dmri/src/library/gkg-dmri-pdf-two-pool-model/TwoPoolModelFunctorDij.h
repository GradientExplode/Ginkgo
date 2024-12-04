#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorDij_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorDij_h_


#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class TwoPoolModelFunctorDij : public TwoPoolModelFunctor< Volume< float >,
                                                         std::vector< float > >,
                               public Singleton< TwoPoolModelFunctorDij >
{

  public:

    ~TwoPoolModelFunctorDij();

    std::string getName() const;


  protected:

    friend class Singleton< TwoPoolModelFunctorDij >;

    TwoPoolModelFunctorDij();

    void processItem(
                     std::vector< float >& item,
                     const TwoPoolModel& twoPoolModel,
                     const std::vector< double >& scalarParameters,
                     const std::vector< std::string >& stringParameters ) const;

};


}


#endif

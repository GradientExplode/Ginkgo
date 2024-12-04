#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorLambdaParallel_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorLambdaParallel_h_


#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class TwoPoolModelFunctorLambdaParallel :
                            public TwoPoolModelFunctor< Volume< float >,
                                                        std::vector< float > >,
                            public Singleton< TwoPoolModelFunctorLambdaParallel >
{

  public:

    ~TwoPoolModelFunctorLambdaParallel();

    std::string getName() const;


  protected:

    friend class Singleton< TwoPoolModelFunctorLambdaParallel >;

    TwoPoolModelFunctorLambdaParallel();

    void processItem(
                     std::vector< float >& item,
                     const TwoPoolModel& twoPoolModel,
                     const std::vector< double >& scalarParameters,
                     const std::vector< std::string >& stringParameters ) const;

};


}


#endif

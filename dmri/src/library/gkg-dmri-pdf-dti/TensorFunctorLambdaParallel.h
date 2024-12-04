#ifndef _gkg_dmri_pdf_dti_TensorFunctorLambdaParallel_h_
#define _gkg_dmri_pdf_dti_TensorFunctorLambdaParallel_h_


#include <gkg-dmri-pdf-dti/TensorFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class TensorFunctorLambdaParallel : 
                                public TensorFunctor< Volume< float >, float >,
                                public Singleton< TensorFunctorLambdaParallel >
{

  public:

    ~TensorFunctorLambdaParallel();

    std::string getName() const;

  protected:

    friend class Singleton< TensorFunctorLambdaParallel >;

    TensorFunctorLambdaParallel();

    void processItem(
                 float& item,
                 const Tensor& tensor,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

#ifndef _gkg_dmri_pdf_dti_TensorFunctorLambdaTransverse_h_
#define _gkg_dmri_pdf_dti_TensorFunctorLambdaTransverse_h_


#include <gkg-dmri-pdf-dti/TensorFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class TensorFunctorLambdaTransverse : 
                                public TensorFunctor< Volume< float >, float >,
                                public Singleton< TensorFunctorLambdaTransverse >
{

  public:

    ~TensorFunctorLambdaTransverse();

    std::string getName() const;

  protected:

    friend class Singleton< TensorFunctorLambdaTransverse >;

    TensorFunctorLambdaTransverse();

    void processItem(
                 float& item,
                 const Tensor& tensor,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

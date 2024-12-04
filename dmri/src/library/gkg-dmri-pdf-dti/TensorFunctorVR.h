#ifndef _gkg_dmri_pdf_dti_TensorFunctorVR_h_
#define _gkg_dmri_pdf_dti_TensorFunctorVR_h_


#include <gkg-dmri-pdf-dti/TensorFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class TensorFunctorVR : public TensorFunctor< Volume< float >, float >,
                        public Singleton< TensorFunctorVR >
{

  public:

    ~TensorFunctorVR();

    std::string getName() const;

  protected:

    friend class Singleton< TensorFunctorVR >;

    TensorFunctorVR();

    void processItem(
                 float& item,
                 const Tensor& tensor,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

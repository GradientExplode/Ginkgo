#ifndef _gkg_dmri_pdf_dti_TensorFunctorRGB_h_
#define _gkg_dmri_pdf_dti_TensorFunctorRGB_h_


#include <gkg-dmri-pdf-dti/TensorFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class TensorFunctorRGB : public TensorFunctor< Volume< RGBComponent >,
                                               RGBComponent >,
                         public Singleton< TensorFunctorRGB >
{

  public:

    ~TensorFunctorRGB();

    std::string getName() const;

  protected:

    friend class Singleton< TensorFunctorRGB >;

    TensorFunctorRGB();

    void processItem(
                 RGBComponent& item,
                 const Tensor& tensor,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

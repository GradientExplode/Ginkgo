#ifndef _gkg_dmri_pdf_dti_TensorFunctorTensorTextureMap_h_
#define _gkg_dmri_pdf_dti_TensorFunctorTensorTextureMap_h_


#include <gkg-dmri-pdf-dti/TensorFunctor.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class TensorFunctorTensorTextureMap :
            public TensorFunctor< TextureMap< Tensor >,
                                  Tensor >,
            public Singleton< TensorFunctorTensorTextureMap >
{

  public:

    ~TensorFunctorTensorTextureMap();

    std::string getName() const;

    void process( TextureMap< Tensor >& output,
                  TensorCartesianField& tensorCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< TensorFunctorTensorTextureMap >;

    TensorFunctorTensorTextureMap();

};


}


#endif

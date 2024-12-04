#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorTwoPoolModelTextureMap_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorTwoPoolModelTextureMap_h_


#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class TwoPoolModelFunctorTwoPoolModelTextureMap :
            public TwoPoolModelFunctor< TextureMap< TwoPoolModel >,
                                        TwoPoolModel >,
            public Singleton< TwoPoolModelFunctorTwoPoolModelTextureMap >
{

  public:

    ~TwoPoolModelFunctorTwoPoolModelTextureMap();

    std::string getName() const;

    void process( TextureMap< TwoPoolModel >& output,
                  TwoPoolModelCartesianField& twoPoolModelCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< TwoPoolModelFunctorTwoPoolModelTextureMap >;

    TwoPoolModelFunctorTwoPoolModelTextureMap();

};


}


#endif

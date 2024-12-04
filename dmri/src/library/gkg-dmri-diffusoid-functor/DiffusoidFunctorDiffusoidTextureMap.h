#ifndef _gkg_dmri_diffusoid_functor_DiffusoidFunctorDiffusoidTextureMap_h_
#define _gkg_dmri_diffusoid_functor_DiffusoidFunctorDiffusoidTextureMap_h_


#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-dmri-diffusoid/Diffusoid.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class DiffusoidFunctorDiffusoidTextureMap :
            public DiffusoidFunctor< TextureMap< Diffusoid >,
                                     Diffusoid >,
            public Singleton< DiffusoidFunctorDiffusoidTextureMap >
{

  public:

    ~DiffusoidFunctorDiffusoidTextureMap();

    std::string getName() const;

    void process( TextureMap< Diffusoid >& output,
                  DiffusoidCartesianField& tensorCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< DiffusoidFunctorDiffusoidTextureMap >;

    DiffusoidFunctorDiffusoidTextureMap();

};


}


#endif

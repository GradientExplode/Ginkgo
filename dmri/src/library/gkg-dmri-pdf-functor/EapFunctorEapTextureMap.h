#ifndef _gkg_dmri_pdf_functor_EapFunctorEapTextureMap_h_
#define _gkg_dmri_pdf_functor_EapFunctorEapTextureMap_h_


#include <gkg-dmri-pdf-functor/EapFunctor.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class EapFunctorEapTextureMap :
            public EapFunctor< TextureMap< OrientationDistributionFunction >,
                               OrientationDistributionFunction >,
            public Singleton< EapFunctorEapTextureMap >
{

  public:

    ~EapFunctorEapTextureMap();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( TextureMap< OrientationDistributionFunction >& output,
                  EapCartesianField& eapCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< EapFunctorEapTextureMap >;

    EapFunctorEapTextureMap();

};


}


#endif

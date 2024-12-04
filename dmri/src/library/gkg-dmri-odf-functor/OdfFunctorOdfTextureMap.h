#ifndef _gkg_dmri_odf_functor_OdfFunctorOdfTextureMap_h_
#define _gkg_dmri_odf_functor_OdfFunctorOdfTextureMap_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class OdfFunctorOdfTextureMap :
            public OdfFunctor< TextureMap< OrientationDistributionFunction >,
                               OrientationDistributionFunction >,
            public Singleton< OdfFunctorOdfTextureMap >
{

  public:

    ~OdfFunctorOdfTextureMap();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( TextureMap< OrientationDistributionFunction >& output,
                  OdfCartesianField& odfCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< OdfFunctorOdfTextureMap >;

    OdfFunctorOdfTextureMap();

};


}


#endif

#ifndef _gkg_dmri_odf_functor_OdfFunctorNormalizedEntropy_h_
#define _gkg_dmri_odf_functor_OdfFunctorNormalizedEntropy_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class OdfFunctorNormalizedEntropy :
                                public OdfFunctor< Volume< float >, float >,
                                public Singleton< OdfFunctorNormalizedEntropy >
{

  public:

    ~OdfFunctorNormalizedEntropy();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< OdfFunctorNormalizedEntropy >;

    OdfFunctorNormalizedEntropy();

    void processItem(
                 float& item,
                 const OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

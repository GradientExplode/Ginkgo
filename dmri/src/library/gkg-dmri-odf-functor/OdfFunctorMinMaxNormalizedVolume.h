#ifndef _gkg_dmri_odf_functor_OdfFunctorMinMaxNormalizedVolume_h_
#define _gkg_dmri_odf_functor_OdfFunctorMinMaxNormalizedVolume_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class OdfFunctorMinMaxNormalizedVolume :
                           public OdfFunctor< Volume< float >,
                                              std::vector< float > >,
                           public Singleton< OdfFunctorMinMaxNormalizedVolume >
{

  public:

    ~OdfFunctorMinMaxNormalizedVolume();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< OdfFunctorMinMaxNormalizedVolume >;

    OdfFunctorMinMaxNormalizedVolume();

    void processItem(
                 std::vector< float >& item,
                 const OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

#ifndef _gkg_dmri_odf_functor_OdfFunctorRGB_h_
#define _gkg_dmri_odf_functor_OdfFunctorRGB_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class OdfFunctorRGB : public OdfFunctor< Volume< RGBComponent >,
                                         RGBComponent >,
                      public Singleton< OdfFunctorRGB >
{

  public:

    ~OdfFunctorRGB();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< OdfFunctorRGB >;

    OdfFunctorRGB();

    // RGB scale must be stored in scalarParameters[ 0 ]
    void processItem(
                 RGBComponent& item,
                 const OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;


};


}


#endif

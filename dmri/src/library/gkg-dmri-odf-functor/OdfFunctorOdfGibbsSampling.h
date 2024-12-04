#ifndef _gkg_dmri_odf_functor_OdfFunctorOdfGibbsSampling_h_
#define _gkg_dmri_odf_functor_OdfFunctorOdfGibbsSampling_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


class OdfFunctorOdfGibbsSampling :
              public OdfFunctor< TextureMap< std::vector< Vector3d< float > > >,
                                 std::vector< Vector3d< float > > >,
              public Singleton< OdfFunctorOdfGibbsSampling >
{

  public:

    ~OdfFunctorOdfGibbsSampling();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< OdfFunctorOdfGibbsSampling >;

    OdfFunctorOdfGibbsSampling();

    // peakCount -> scalarParameters[ 5 ]
    // Gibb's temperature -> scalarParamerers[ 6 ]
    // resolution -> scalarParameters[ 7 ]
    void processItem(
                     std::vector< Vector3d< float > >& item,
                     const OrientationDistributionFunction& odf,
                     const std::vector< double >& scalarParameters,
                     const std::vector< std::string >& stringParameters ) const;

    mutable RCPointer< RandomGenerator > _randomGenerator;

};


}


#endif

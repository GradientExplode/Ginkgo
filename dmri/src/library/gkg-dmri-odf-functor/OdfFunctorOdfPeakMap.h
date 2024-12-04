#ifndef _gkg_dmri_odf_functor_OdfFunctorOdfPeakMap_h_
#define _gkg_dmri_odf_functor_OdfFunctorOdfPeakMap_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class OdfFunctorOdfPeakMap :
              public OdfFunctor< TextureMap< std::vector< Vector3d< float > > >,
                                 std::vector< Vector3d< float > > >,
              public Singleton< OdfFunctorOdfPeakMap >
{

  public:

    ~OdfFunctorOdfPeakMap();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< OdfFunctorOdfPeakMap >;

    OdfFunctorOdfPeakMap();

    // peakCount -> scalarParameters[ 5 ]
    // colatitudeDerivativeThreshold -> scalarParamerers[ 6 ]
    // resolution -> scalarParameters[ 7 ]
    // clusteringThreshold -> scalarParameters[ 8 ]
    // angularThreshold -> scalarParameters[ 9 ]
    // minimumIncrement -> scalarParameters[ 10 ]
    // maximumIncrement -> scalarParameters[ 11 ]
    void processItem(
                     std::vector< Vector3d< float > >& item,
                     const OrientationDistributionFunction& odf,
                     const std::vector< double >& scalarParameters,
                     const std::vector< std::string >& stringParameters ) const;

};


}


#endif

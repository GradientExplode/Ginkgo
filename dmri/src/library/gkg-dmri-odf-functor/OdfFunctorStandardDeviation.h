#ifndef _gkg_dmri_odf_functor_OdfFunctorStandardDeviation_h_
#define _gkg_dmri_odf_functor_OdfFunctorStandardDeviation_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class OdfFunctorStandardDeviation :
                                public OdfFunctor< Volume< float >, float >,
                                public Singleton< OdfFunctorStandardDeviation >
{

  public:

    ~OdfFunctorStandardDeviation();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< OdfFunctorStandardDeviation >;

    OdfFunctorStandardDeviation();

    void processItem(
                 float& item,
                 const OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;


};


}


#endif

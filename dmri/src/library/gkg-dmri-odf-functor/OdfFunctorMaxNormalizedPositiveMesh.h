#ifndef _gkg_dmri_odf_functor_OdfFunctorMaxNormalizedPositiveMesh_h_
#define _gkg_dmri_odf_functor_OdfFunctorMaxNormalizedPositiveMesh_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class OdfFunctorMaxNormalizedPositiveMesh :
                     public OdfFunctor< MeshMap< int32_t, float, 3U >,
                                        std::vector< float > >,
                     public Singleton< OdfFunctorMaxNormalizedPositiveMesh >
{

  public:

    ~OdfFunctorMaxNormalizedPositiveMesh();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< OdfFunctorMaxNormalizedPositiveMesh >;

    OdfFunctorMaxNormalizedPositiveMesh();

    // mesh scale must be contained in scalarParameters[ 1 ]
    void processItem(
                 std::vector< float >& item,
                 const OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

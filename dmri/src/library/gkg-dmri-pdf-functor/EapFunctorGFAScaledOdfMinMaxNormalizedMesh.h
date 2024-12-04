#ifndef _gkg_dmri_pdf_functor_EapFunctorGFAScaledOdfMinMaxNormalizedMesh_h_
#define _gkg_dmri_pdf_functor_EapFunctorGFAScaledOdfMinMaxNormalizedMesh_h_


#include <gkg-dmri-pdf-functor/EapFunctor.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class EapFunctorGFAScaledOdfMinMaxNormalizedMesh :
                    public EapFunctor< MeshMap< int32_t, float, 3U >,
                                       std::vector< float > >,
                    public Singleton< EapFunctorGFAScaledOdfMinMaxNormalizedMesh >
{

  public:

    ~EapFunctorGFAScaledOdfMinMaxNormalizedMesh();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< EapFunctorGFAScaledOdfMinMaxNormalizedMesh >;

    EapFunctorGFAScaledOdfMinMaxNormalizedMesh();

    // mesh scale must be contained in scalarParameters[ 1 ]
    // lower GFA threshold must be contained in scalarParameters[ 2 ]
    // upper GFA threshold must be contained in scalarParameters[ 3 ]
    void processItem(
                 std::vector< float >& item,
                 const OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;


};


}


#endif

#ifndef _gkg_dmri_pdf_functor_EapFunctorOdfMinMaxNormalizedMesh_h_
#define _gkg_dmri_pdf_functor_EapFunctorOdfMinMaxNormalizedMesh_h_


#include <gkg-dmri-pdf-functor/EapFunctor.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class EapFunctorOdfMinMaxNormalizedMesh :
                            public EapFunctor< MeshMap< int32_t, float, 3U >,
                              std::vector< float > >,
                            public Singleton< EapFunctorOdfMinMaxNormalizedMesh >
{

  public:

    ~EapFunctorOdfMinMaxNormalizedMesh();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< EapFunctorOdfMinMaxNormalizedMesh >;

    EapFunctorOdfMinMaxNormalizedMesh();

    // mesh scale must be contained in scalarParameters[ 1 ]
    void processItem(
                std::vector< float >& item,
                const OrientationDistributionFunction& odf,
                const std::vector< double >& scalarParameters,
                const std::vector< std::string >& stringParameters ) const;

};


}


#endif


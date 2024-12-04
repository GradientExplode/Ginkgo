#ifndef _gkg_dmri_odf_functor_OdfFunctorGFAScaledMaxNormalizedMesh_h_
#define _gkg_dmri_odf_functor_OdfFunctorGFAScaledMaxNormalizedMesh_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class OdfFunctorGFAScaledMaxNormalizedMesh :
                    public OdfFunctor< MeshMap< int32_t, float, 3U >,
                                       std::vector< float > >,
                    public Singleton< OdfFunctorGFAScaledMaxNormalizedMesh >
{

  public:

    ~OdfFunctorGFAScaledMaxNormalizedMesh();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( MeshMap< int32_t, float, 3U >& output,
                  OdfCartesianField& odfCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< OdfFunctorGFAScaledMaxNormalizedMesh >;

    OdfFunctorGFAScaledMaxNormalizedMesh();

    // mesh scale must be contained in scalarParameters[ 1 ]
    // lower GFA threshold must be contained in scalarParameters[ 2 ]
    // upper GFA threshold must be contained in scalarParameters[ 3 ]
    // normalized GFA value will be stored in scalarParameters[ 4 ]
    void processItem(
                 std::vector< float >& item,
                 const OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;


};


}


#endif

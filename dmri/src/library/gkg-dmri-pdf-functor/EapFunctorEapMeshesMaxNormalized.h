#ifndef _gkg_dmri_pdf_functor_EapFunctorEapMeshesMaxNormalized_h_
#define _gkg_dmri_pdf_functor_EapFunctorEapMeshesMaxNormalized_h_

#include <gkg-dmri-pdf-functor/EapFunctor.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class EapFunctorEapMeshesMaxNormalized :
  public EapFunctor< std::vector< MeshMap< int32_t, float, 3U > >,
    std::vector< float > >,
    public Singleton< EapFunctorEapMeshesMaxNormalized >
{

  public:

    ~EapFunctorEapMeshesMaxNormalized();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< EapFunctorEapMeshesMaxNormalized >;

    EapFunctorEapMeshesMaxNormalized();

    // mesh scale must be contained in scalarParameters[ 1 ]
    void processItem(
                 std::vector< float >& item,
                 const OrientationDistributionFunction& eap,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;
};


}


#endif

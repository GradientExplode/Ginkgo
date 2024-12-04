#ifndef _gkg_dmri_diffusoid_functor_DiffusoidFunctorMinMaxNormalizedMesh_h_
#define _gkg_dmri_diffusoid_functor_DiffusoidFunctorMinMaxNormalizedMesh_h_


#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class DiffusoidFunctorMinMaxNormalizedMesh :
                       public DiffusoidFunctor< MeshMap< int32_t, float, 3U >,
                                                std::vector< float > >,
                       public Singleton< DiffusoidFunctorMinMaxNormalizedMesh >
{

  public:

    ~DiffusoidFunctorMinMaxNormalizedMesh();

    std::string getName() const;

  protected:

    friend class Singleton< DiffusoidFunctorMinMaxNormalizedMesh >;

    DiffusoidFunctorMinMaxNormalizedMesh();

    // mesh scale must be contained in scalarParameters[ 1 ]
    void processItem(
                 std::vector< float >& item,
                 const Diffusoid& diffusoid,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;


};


}


#endif

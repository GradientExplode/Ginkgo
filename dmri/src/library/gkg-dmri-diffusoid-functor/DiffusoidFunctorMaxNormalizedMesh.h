#ifndef _gkg_dmri_diffusoid_functor_DiffusoidFunctorMaxNormalizedMesh_h_
#define _gkg_dmri_diffusoid_functor_DiffusoidFunctorMaxNormalizedMesh_h_


#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class DiffusoidFunctorMaxNormalizedMesh :
                       public DiffusoidFunctor< MeshMap< int32_t, float, 3U >,
                                                std::vector< float > >,
                       public Singleton< DiffusoidFunctorMaxNormalizedMesh >
{

  public:

    ~DiffusoidFunctorMaxNormalizedMesh();

    std::string getName() const;

  protected:

    friend class Singleton< DiffusoidFunctorMaxNormalizedMesh >;

    DiffusoidFunctorMaxNormalizedMesh();

    // mesh scale must be contained in scalarParameters[ 1 ]
    void processItem(
                 std::vector< float >& item,
                 const Diffusoid& diffusoid,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;


};


}


#endif

#ifndef _gkg_dmri_diffusoid_functor_DiffusoidFunctorRBSphericalMesh_h_
#define _gkg_dmri_diffusoid_functor_DiffusoidFunctorRBSphericalMesh_h_


#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class DiffusoidFunctorRBSphericalMesh :
                public DiffusoidFunctor< MeshMap< int32_t, float, 3U >,
                                         std::vector< std::vector< float > > >,
                public Singleton< DiffusoidFunctorRBSphericalMesh >
{

  public:

    ~DiffusoidFunctorRBSphericalMesh();

    std::string getName() const;
    bool isStandardBasisRequired() const;

  protected:

    friend class Singleton< DiffusoidFunctorRBSphericalMesh >;

    DiffusoidFunctorRBSphericalMesh();

    // mesh scale must be contained in scalarParameters[ 1 ]
    void processItem(
                 std::vector< std::vector< float > >& item,
                 const Diffusoid& diffusoid,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;


};


}


#endif

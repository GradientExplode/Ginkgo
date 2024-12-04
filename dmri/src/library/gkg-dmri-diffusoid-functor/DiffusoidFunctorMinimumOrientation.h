#ifndef _gkg_dmri_diffusoid_functor_DiffusoidFunctorMinimumOrientation_h_
#define _gkg_dmri_diffusoid_functor_DiffusoidFunctorMinimumOrientation_h_


#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class DiffusoidFunctorMinimumOrientation :
                         public DiffusoidFunctor< Volume< Vector3d< float > >,
                                                  Vector3d< float > >,
                         public Singleton< DiffusoidFunctorMinimumOrientation >
{

  public:

    ~DiffusoidFunctorMinimumOrientation();

    std::string getName() const;

  protected:

    friend class Singleton< DiffusoidFunctorMinimumOrientation >;

    DiffusoidFunctorMinimumOrientation();

    void processItem(
                 Vector3d< float >& item,
                 const Diffusoid& diffusoid,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

#ifndef _gkg_dmri_diffusoid_functor_DiffusoidFunctorMaximumOrientation_h_
#define _gkg_dmri_diffusoid_functor_DiffusoidFunctorMaximumOrientation_h_


#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class DiffusoidFunctorMaximumOrientation :
                         public DiffusoidFunctor< Volume< Vector3d< float > >,
                                                  Vector3d< float > >,
                         public Singleton< DiffusoidFunctorMaximumOrientation >
{

  public:

    ~DiffusoidFunctorMaximumOrientation();

    std::string getName() const;

  protected:

    friend class Singleton< DiffusoidFunctorMaximumOrientation >;

    DiffusoidFunctorMaximumOrientation();

    void processItem(
                 Vector3d< float >& item,
                 const Diffusoid& diffusoid,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif

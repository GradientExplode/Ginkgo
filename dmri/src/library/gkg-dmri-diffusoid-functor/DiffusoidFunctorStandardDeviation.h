#ifndef _gkg_dmri_diffusoid_functor_DiffusoidFunctorStandardDeviation_h_
#define _gkg_dmri_diffusoid_functor_DiffusoidFunctorStandardDeviation_h_


#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class DiffusoidFunctorStandardDeviation :
                          public DiffusoidFunctor< Volume< float >, float >,
                          public Singleton< DiffusoidFunctorStandardDeviation >
{

  public:

    ~DiffusoidFunctorStandardDeviation();

    std::string getName() const;

  protected:

    friend class Singleton< DiffusoidFunctorStandardDeviation >;

    DiffusoidFunctorStandardDeviation();

    void processItem(
                 float& item,
                 const Diffusoid& diffusoid,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;


};


}


#endif

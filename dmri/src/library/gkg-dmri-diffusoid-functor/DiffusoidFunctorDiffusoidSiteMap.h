#ifndef _gkg_dmri_diffusoid_functor_DiffusoidFunctorDiffusoidSiteMap_h_
#define _gkg_dmri_diffusoid_functor_DiffusoidFunctorDiffusoidSiteMap_h_


#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor.h>
#include <gkg-processing-container/SiteMap.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class DiffusoidFunctorDiffusoidSiteMap :
                           public DiffusoidFunctor<
                                            SiteMap< int32_t, int32_t >,
                                            Vector3d< int32_t > >,
                           public Singleton< DiffusoidFunctorDiffusoidSiteMap >
{

  public:

    ~DiffusoidFunctorDiffusoidSiteMap();

    std::string getName() const;

    void process( SiteMap< int32_t, int32_t >& output,
                  DiffusoidCartesianField& diffusoidCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< DiffusoidFunctorDiffusoidSiteMap >;

    DiffusoidFunctorDiffusoidSiteMap();

};


}


#endif

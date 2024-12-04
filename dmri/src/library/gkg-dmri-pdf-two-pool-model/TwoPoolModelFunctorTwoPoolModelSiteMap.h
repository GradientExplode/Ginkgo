#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorTwoPoolModelSiteMap_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctorTwoPoolModelSiteMap_h_


#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor.h>
#include <gkg-processing-container/SiteMap.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class TwoPoolModelFunctorTwoPoolModelSiteMap :
                      public TwoPoolModelFunctor< SiteMap< int32_t, int32_t >,
                                                  Vector3d< int32_t > >,
                      public Singleton< TwoPoolModelFunctorTwoPoolModelSiteMap >
{

  public:

    ~TwoPoolModelFunctorTwoPoolModelSiteMap();

    std::string getName() const;

    void process( SiteMap< int32_t, int32_t >& output,
                  TwoPoolModelCartesianField& twoPoolModelCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< TwoPoolModelFunctorTwoPoolModelSiteMap >;

    TwoPoolModelFunctorTwoPoolModelSiteMap();

};


}


#endif

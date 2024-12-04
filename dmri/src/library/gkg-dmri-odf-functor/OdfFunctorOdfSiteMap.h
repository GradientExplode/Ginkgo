#ifndef _gkg_dmri_odf_functor_OdfFunctorOdfSiteMap_h_
#define _gkg_dmri_odf_functor_OdfFunctorOdfSiteMap_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-processing-container/SiteMap.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class OdfFunctorOdfSiteMap : public OdfFunctor< SiteMap< int32_t, int32_t >,
                                                Vector3d< int32_t > >,
                             public Singleton< OdfFunctorOdfSiteMap >
{

  public:

    ~OdfFunctorOdfSiteMap();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( SiteMap< int32_t, int32_t >& output,
                  OdfCartesianField& odfCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< OdfFunctorOdfSiteMap >;

    OdfFunctorOdfSiteMap();

};


}


#endif

#ifndef _gkg_dmri_pdf_functor_EapFunctorEapSiteMap_h_
#define _gkg_dmri_pdf_functor_EapFunctorEapSiteMap_h_


#include <gkg-dmri-pdf-functor/EapFunctor.h>
#include <gkg-processing-container/SiteMap.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class EapFunctorEapSiteMap : public EapFunctor< SiteMap< int32_t, int32_t >,
                                                Vector3d< int32_t > >,
                             public Singleton< EapFunctorEapSiteMap >
{

  public:

    ~EapFunctorEapSiteMap();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( SiteMap< int32_t, int32_t >& output,
                  EapCartesianField& eapCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< EapFunctorEapSiteMap >;

    EapFunctorEapSiteMap();

};


}


#endif

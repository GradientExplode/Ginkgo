#ifndef _gkg_dmri_pdf_dti_TensorFunctorTensorSiteMap_h_
#define _gkg_dmri_pdf_dti_TensorFunctorTensorSiteMap_h_


#include <gkg-dmri-pdf-dti/TensorFunctor.h>
#include <gkg-processing-container/SiteMap.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class TensorFunctorTensorSiteMap :
                                 public TensorFunctor<
                                            SiteMap< int32_t, int32_t >,
                                            Vector3d< int32_t > >,
                                 public Singleton< TensorFunctorTensorSiteMap >
{

  public:

    ~TensorFunctorTensorSiteMap();

    std::string getName() const;

    void process( SiteMap< int32_t, int32_t >& output,
                  TensorCartesianField& odfCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< TensorFunctorTensorSiteMap >;

    TensorFunctorTensorSiteMap();

};


}


#endif

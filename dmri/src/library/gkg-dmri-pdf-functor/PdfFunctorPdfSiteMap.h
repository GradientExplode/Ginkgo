#ifndef _gkg_dmri_pdf_functor_PdfFunctorPdfSiteMap_h_
#define _gkg_dmri_pdf_functor_PdfFunctorPdfSiteMap_h_


#include <gkg-dmri-pdf-functor/PdfFunctor.h>
#include <gkg-processing-container/SiteMap.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class PdfFunctorPdfSiteMap : public PdfFunctor< SiteMap< int32_t, int32_t >,
                                                Vector3d< int32_t > >,
                             public Singleton< PdfFunctorPdfSiteMap >
{

  public:

    ~PdfFunctorPdfSiteMap();

    std::string getName() const;
    bool isStandardBasisRequired() const;

    void process( SiteMap< int32_t, int32_t >& output,
                  PdfCartesianField& pdfCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< PdfFunctorPdfSiteMap >;

    PdfFunctorPdfSiteMap();

};


}


#endif

#include <gkg-dmri-pdf-functor/PdfFunctorPdfSiteMap.h>
#include <gkg-dmri-pdf-functor/PdfFunctor_i.h>
#include <gkg-dmri-pdf-functor/PdfFunctorFactory_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::PdfFunctorPdfSiteMap::PdfFunctorPdfSiteMap()
                          : gkg::PdfFunctor< gkg::SiteMap< int32_t, int32_t >,
                                             gkg::Vector3d< int32_t > >()
{
}


gkg::PdfFunctorPdfSiteMap::~PdfFunctorPdfSiteMap()
{
}


std::string gkg::PdfFunctorPdfSiteMap::getName() const
{

  try
  {

    return "pdf_site_map";

  }
  GKG_CATCH( "std::string gkg::PdfFunctorPdfSiteMap::getName() const" );

}


bool gkg::PdfFunctorPdfSiteMap::isStandardBasisRequired() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool "
             "gkg::PdfFunctorPdfSiteMap::isStandardBasisRequired() const" );

}


void gkg::PdfFunctorPdfSiteMap::process(
                             gkg::SiteMap< int32_t, int32_t >& output,
                             PdfCartesianField& pdfCartesianField,
                             const std::vector< double >& /*scalarParameters*/,
                             const std::vector< std::string >& /*stringParameters*/,
                             bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    output = pdfCartesianField.getSiteMap();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::PdfFunctorPdfSiteMap::process( "
             "gkg::SiteMap< int32_t, int32_t >& output, "
             "gkg::PdfCartesianField& pdfCartesianField, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_PdfFunctorPdfSiteMap __attribute__((unused)) =
gkg::PdfFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                        gkg::Vector3d< int32_t > >::getInstance().
                                                           registerPdfFunctor(
                                    gkg::PdfFunctorPdfSiteMap::getInstance() );

#include <gkg-dmri-pdf-functor/PdfFunctorDecomposition.h>
#include <gkg-dmri-pdf-functor/PdfFunctor_i.h>
#include <gkg-dmri-pdf-functor/PdfFunctorFactory_i.h>
#include <gkg-dmri-pdf-shore/ShorePdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::PdfFunctorDecomposition::PdfFunctorDecomposition()
                             : gkg::PdfFunctor< gkg::Volume< float >,
                                                float >()
{
}


gkg::PdfFunctorDecomposition::~PdfFunctorDecomposition()
{
}


std::string gkg::PdfFunctorDecomposition::getName() const
{

  try
  {

    return "pdf_decomposition";

  }
  GKG_CATCH( "std::string gkg::PdfFunctorDecomposition::getName() const" );

}


bool gkg::PdfFunctorDecomposition::isStandardBasisRequired() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool "
             "gkg::PdfFunctorDecomposition::isStandardBasisRequired() const" );

}


void gkg::PdfFunctorDecomposition::process(
                         gkg::Volume< float >& output,
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

    gkg::ShorePdfCartesianFieldFunctor*
     shorePdfCartesianFieldFunctor =
      dynamic_cast< gkg::ShorePdfCartesianFieldFunctor* >(
                                               pdfCartesianField.getFunctor() );

    if ( shorePdfCartesianFieldFunctor )
    {

      output = shorePdfCartesianFieldFunctor->
	                                     getPdfPolarHarmonicsCoefficients();

    }
    else
    {
    
      std::cerr << "warning: not possible to get the functor"
                << " 'pdf_decomposition'"
                << std::endl; 

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::PdfFunctorDecomposition::process( "
             "gkg::Volume< float >& output, "
             "gkg::PdfCartesianField& pdfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_PdfFunctorDecomposition __attribute__((unused)) =
gkg::PdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                             registerPdfFunctor(
                                gkg::PdfFunctorDecomposition::getInstance() );

#include <gkg-dmri-pdf-functor/PdfFunctorNonGaussianity.h>
#include <gkg-dmri-pdf-functor/PdfFunctor_i.h>
#include <gkg-dmri-pdf-functor/PdfFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::PdfFunctorNonGaussianity::PdfFunctorNonGaussianity()
                                    : gkg::PdfFunctor< gkg::Volume< float >,
                                                       float >()
{
}


gkg::PdfFunctorNonGaussianity::~PdfFunctorNonGaussianity()
{
}


std::string gkg::PdfFunctorNonGaussianity::getName() const
{

  try
  {

    return "non_gaussianity";

  }
  GKG_CATCH( "std::string "
             "gkg::PdfFunctorNonGaussianity::getName() const" );

}


bool gkg::PdfFunctorNonGaussianity::isStandardBasisRequired() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool "
             "gkg::PdfFunctorNonGaussianity::isStandardBasisRequired() "
             "const" );

}


void gkg::PdfFunctorNonGaussianity::processItem(
                 float& item,
                 const gkg::ProbabilityDensityFunction& pdf,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = pdf.getNonGaussianity();

  }
  GKG_CATCH( "void gkg::PdfFunctorNonGaussianity::processItem( "
             "float& item, "
             "const gkg::ProbabilityDensityFunction& pdf, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_PdfFunctorNonGaussianity __attribute__((unused)) =
gkg::PdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                           registerPdfFunctor(
                           gkg::PdfFunctorNonGaussianity::getInstance() );

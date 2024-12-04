#include <gkg-dmri-pdf-functor/PdfFunctorReturnToOriginProbability.h>
#include <gkg-dmri-pdf-functor/PdfFunctor_i.h>
#include <gkg-dmri-pdf-functor/PdfFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::PdfFunctorReturnToOriginProbability::PdfFunctorReturnToOriginProbability()
                                         : gkg::PdfFunctor<
                                                           gkg::Volume< float >,
                                                           float >()
{
}


gkg::PdfFunctorReturnToOriginProbability::~PdfFunctorReturnToOriginProbability()
{
}


std::string gkg::PdfFunctorReturnToOriginProbability::getName() const
{

  try
  {

    return "return_to_origin_probability";

  }
  GKG_CATCH( "std::string "
             "gkg::PdfFunctorReturnToOriginProbability::getName() const" );

}


bool gkg::PdfFunctorReturnToOriginProbability::isStandardBasisRequired() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool "
             "gkg::PdfFunctorReturnToOriginProbability::"
             "isStandardBasisRequired() "
             "const" );

}


void gkg::PdfFunctorReturnToOriginProbability::processItem(
                 float& item,
                 const gkg::ProbabilityDensityFunction& pdf,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = pdf.getReturnToOriginProbability();

  }
  GKG_CATCH( "void gkg::PdfFunctorReturnToOriginProbability::processItem( "
             "float& item, "
             "const gkg::ProbabilityDensityFunction& pdf, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_PdfFunctorReturnToOriginProbability 
                                                       __attribute__((unused)) =
gkg::PdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                           registerPdfFunctor(
                      gkg::PdfFunctorReturnToOriginProbability::getInstance() );

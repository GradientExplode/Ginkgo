#include <gkg-dmri-pdf-functor/PdfFunctorReturnToAxisProbability.h>
#include <gkg-dmri-pdf-functor/PdfFunctor_i.h>
#include <gkg-dmri-pdf-functor/PdfFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::PdfFunctorReturnToAxisProbability::PdfFunctorReturnToAxisProbability()
                                       : gkg::PdfFunctor< gkg::Volume< float >,
                                                          float >()
{
}


gkg::PdfFunctorReturnToAxisProbability::~PdfFunctorReturnToAxisProbability()
{
}


std::string gkg::PdfFunctorReturnToAxisProbability::getName() const
{

  try
  {

    return "return_to_axis_probability";

  }
  GKG_CATCH( "std::string "
             "gkg::PdfFunctorReturnToAxisProbability::getName() const" );

}


bool gkg::PdfFunctorReturnToAxisProbability::isStandardBasisRequired() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool "
             "gkg::PdfFunctorReturnToAxisProbability::"
             "isStandardBasisRequired() "
             "const" );

}


void gkg::PdfFunctorReturnToAxisProbability::processItem(
                 float& item,
                 const gkg::ProbabilityDensityFunction& pdf,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = pdf.getReturnToAxisProbability();

  }
  GKG_CATCH( "void gkg::PdfFunctorReturnToAxisProbability::processItem( "
             "float& item, "
             "const gkg::ProbabilityDensityFunction& pdf, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_PdfFunctorReturnToAxisProbability 
                                                       __attribute__((unused)) =
gkg::PdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                           registerPdfFunctor(
                        gkg::PdfFunctorReturnToAxisProbability::getInstance() );

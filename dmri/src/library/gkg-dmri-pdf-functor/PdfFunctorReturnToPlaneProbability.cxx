#include <gkg-dmri-pdf-functor/PdfFunctorReturnToPlaneProbability.h>
#include <gkg-dmri-pdf-functor/PdfFunctor_i.h>
#include <gkg-dmri-pdf-functor/PdfFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::PdfFunctorReturnToPlaneProbability::PdfFunctorReturnToPlaneProbability()
                                        : gkg::PdfFunctor< gkg::Volume< float >,
                                                           float >()
{
}


gkg::PdfFunctorReturnToPlaneProbability::~PdfFunctorReturnToPlaneProbability()
{
}


std::string gkg::PdfFunctorReturnToPlaneProbability::getName() const
{

  try
  {

    return "return_to_plane_probability";

  }
  GKG_CATCH( "std::string "
             "gkg::PdfFunctorReturnToPlaneProbability::getName() const" );

}


bool gkg::PdfFunctorReturnToPlaneProbability::isStandardBasisRequired() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool "
             "gkg::PdfFunctorReturnToPlaneProbability::"
             "isStandardBasisRequired() "
             "const" );

}


void gkg::PdfFunctorReturnToPlaneProbability::processItem(
                 float& item,
                 const gkg::ProbabilityDensityFunction& pdf,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = pdf.getReturnToPlaneProbability();

  }
  GKG_CATCH( "void gkg::PdfFunctorReturnToPlaneProbability::processItem( "
             "float& item, "
             "const gkg::ProbabilityDensityFunction& pdf, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_PdfFunctorReturnToPlaneProbability 
                                                       __attribute__((unused)) =
gkg::PdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                           registerPdfFunctor(
                       gkg::PdfFunctorReturnToPlaneProbability::getInstance() );

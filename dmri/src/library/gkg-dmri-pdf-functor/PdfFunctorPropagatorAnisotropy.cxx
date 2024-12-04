#include <gkg-dmri-pdf-functor/PdfFunctorPropagatorAnisotropy.h>
#include <gkg-dmri-pdf-functor/PdfFunctor_i.h>
#include <gkg-dmri-pdf-functor/PdfFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::PdfFunctorPropagatorAnisotropy::PdfFunctorPropagatorAnisotropy()
                                    : gkg::PdfFunctor< gkg::Volume< float >,
                                                       float >()
{
}


gkg::PdfFunctorPropagatorAnisotropy::~PdfFunctorPropagatorAnisotropy()
{
}


std::string gkg::PdfFunctorPropagatorAnisotropy::getName() const
{

  try
  {

    return "propagator_anisotropy";

  }
  GKG_CATCH( "std::string "
             "gkg::PdfFunctorPropagatorAnisotropy::getName() const" );

}


bool gkg::PdfFunctorPropagatorAnisotropy::isStandardBasisRequired() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool "
             "gkg::PdfFunctorPropagatorAnisotropy::isStandardBasisRequired() "
             "const" );

}


void gkg::PdfFunctorPropagatorAnisotropy::processItem(
                 float& item,
                 const gkg::ProbabilityDensityFunction& pdf,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = pdf.getPropagatorAnisotropy();

  }
  GKG_CATCH( "void gkg::PdfFunctorPropagatorAnisotropy::processItem( "
             "float& item, "
             "const gkg::ProbabilityDensityFunction& pdf, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_PdfFunctorPropagatorAnisotropy
                                                       __attribute__((unused)) =
gkg::PdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                           registerPdfFunctor(
                           gkg::PdfFunctorPropagatorAnisotropy::getInstance() );
